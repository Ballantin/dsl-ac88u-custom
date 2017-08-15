#if defined(CONFIG_BCM_KF_MTD_BCMNAND)
/*
 *
 *  drivers/mtd/brcmnand/bcm7xxx-nand.c
 *
    <:copyright-BRCM:2011:DUAL/GPL:standard
    
       Copyright (c) 2011 Broadcom 
       All Rights Reserved
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :> 


    File: bcm7xxx-nand.c

    Description: 
    This is a device driver for the Broadcom NAND flash for bcm97xxx boards.
when    who what
-----   --- ----
051011  tht codings derived from OneNand generic.c implementation.

 * THIS DRIVER WAS PORTED FROM THE 2.6.18-7.2 KERNEL RELEASE
 */
 
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <asm/io.h>
#include <bcm_map_part.h>
#include <board.h>
#include "brcmnand_priv.h"
#include <linux/slab.h> 
#include <flash_api.h>

// ARCADYAN
// ASUS 
#ifdef ARCADYAN
#include <linux/proc_fs.h>

static int brcmnand_boot_partition = -1;
static int brcmnanddrv_proc_get_bootpartition(char *buf, char **start, off_t off, int cnt, int *eof, void *data)
{
	sprintf(buf, "%s\n", ( NP_ROOTFS_1 == brcmnand_boot_partition )?"RFS1":"RFS2");
    return strlen(buf);
}
#endif
// ASUS
// ARCADYAN

#define PRINTK(...)
//#define PRINTK printk

#define DRIVER_NAME     "brcmnand"
#define DRIVER_INFO     "Broadcom NAND controller"

// ARCADYAN
// ASUS 
#define NAND_ANSWER_MACHINE_SIZE	40*1024*1024	// 40M
#define NAND_RESERVED_SIZE			9856*1024		// 9.625M
#define NAND_BOOT_FLAG_SIZE		256*1024		// 0.25M
// ASUS
// ARCADYAN

extern bool kerSysIsRootfsSet(void);

static int __devinit brcmnanddrv_probe(struct platform_device *pdev);
static int __devexit brcmnanddrv_remove(struct platform_device *pdev);

static struct mtd_partition bcm63XX_nand_parts[] = 
{
    {name: "rootfs",        offset: 0, size: 0},
    {name: "rootfs_update", offset: 0, size: 0},
    {name: "data",          offset: 0, size: 0},
    {name: "nvram",         offset: 0, size: 0},
    {name: "image",         offset: 0, size: 0},
    {name: "image_update",  offset: 0, size: 0},
// ARCADYAN
// ASUS 
#ifdef BRCML05
    {name: "dummy1",        offset: 0, size: 0},
    {name: "dummy2",        offset: 0, size: 0},
    {name: "dummy3",        offset: 0, size: 0},
    {name: "dummy4",        offset: 0, size: 0},
    {name: "dummy5",        offset: 0, size: 0},
    {name: "dummy6",        offset: 0, size: 0},
#else
#ifdef ANSWERING_MACHINE
    {name: "answer_machine",offset: 0, size: 0},
#endif    
    {name: "boot_flag",     offset: 0, size: 0},
    {name: "pricfg",        offset: 0, size: 0},
    {name: NULL,            offset: 0, size: 0},

#endif
// ASUS
// ARCADYAN
    {name: NULL,            offset: 0, size: 0}
};

#ifndef ARCADYAN
static char* misc_mtd_partition_names[BCM_MAX_EXTRA_PARTITIONS] =
{
	"misc1",
	"misc2",
	"misc3",
	NULL,
};
#endif

static struct platform_driver brcmnand_platform_driver =
{
    .probe      = brcmnanddrv_probe,
    .remove     = __devexit_p(brcmnanddrv_remove),
    .driver     =
     {
        .name   = DRIVER_NAME,
     },
};

static struct resource brcmnand_resources[] =
{
    [0] = {
            .name   = DRIVER_NAME,
            .start  = BPHYSADDR(BCHP_NAND_REG_START),
            .end    = BPHYSADDR(BCHP_NAND_REG_END) + 3,
            .flags  = IORESOURCE_MEM,
          },
};

static struct brcmnand_info
{
    struct mtd_info mtd;
    struct brcmnand_chip brcmnand;
    int nr_parts;
    struct mtd_partition* parts;
} *gNandInfo[NUM_NAND_CS];

int gNandCS[NAND_MAX_CS];
/* Number of NAND chips, only applicable to v1.0+ NAND controller */
int gNumNand = 0;
int gClearBBT = 0;
char gClearCET = 0;
uint32_t gNandTiming1[NAND_MAX_CS], gNandTiming2[NAND_MAX_CS];
uint32_t gAccControl[NAND_MAX_CS], gNandConfig[NAND_MAX_CS];

static unsigned long t1[NAND_MAX_CS] = {0};
static int nt1 = 0;
static unsigned long t2[NAND_MAX_CS] = {0};
static int nt2 = 0;
static unsigned long acc[NAND_MAX_CS] = {0};
static int nacc = 0;
static unsigned long nandcfg[NAND_MAX_CS] = {0};
static int ncfg = 0;
static void* gPageBuffer = NULL;

static int __devinit 
is_split_partition (struct mtd_info* mtd, unsigned long offset, unsigned long size, unsigned long *split_offset)
{
    uint8_t buf[0x100];
    size_t retlen;
    int split_found = 0;

    /* Search RootFS partion for split marker.
     * Marker is located in the last 0x100 bytes of the last BootFS Erase Block
     * If marker is found, we have separate Boot and Root Partitions.
     */
    for (*split_offset = offset + mtd->erasesize; *split_offset <= offset + size; *split_offset += mtd->erasesize)
    {
        if (mtd->_block_isbad(mtd, *split_offset - mtd->erasesize)) {
            continue;
        }
        mtd->_read(mtd, *split_offset - 0x100, 0x100, &retlen, buf);

        if (!strncmp (BCM_BCMFS_TAG, buf, strlen (BCM_BCMFS_TAG))) {
            if (!strncmp (BCM_BCMFS_TYPE_UBIFS, &buf[strlen (BCM_BCMFS_TAG)], strlen (BCM_BCMFS_TYPE_UBIFS)))
            {
                printk("***** Found UBIFS Marker at 0x%08lx\n", *split_offset - 0x100); 
                split_found = 1;
                break;
            }
        }
    }

    return split_found;
}


int __devinit setup_mtd_parts(struct mtd_info* mtd)
{
// ARCADYAN
// ASUS 
#ifdef BRCML05
    int i=0;
    uint64_t extra=0, extra_single_part_size=0;
#endif
// ASUS 
// ARCADYAN

    static NVRAM_DATA nvram;
    unsigned long rootfs_ofs;

// ARCADYAN
// ASUS 
#ifdef BRCML05
    int nr_parts = 6;
#else
    struct proc_dir_entry *proc_boot_partition = NULL;
//20151106.TercesTsai: Add partition "anser_machine" & "boot_flag" before pricfg"
//    int nr_parts = 9;
	int nr_parts = ARC_PARTITION_NUM;

#endif
// ASUS 
// ARCADYAN

    int rootfs, rootfs_update;
    unsigned long  split_offset;

    kerSysBlParmsGetInt(NAND_RFS_OFS_NAME, (int *) &rootfs_ofs);
    kerSysNvRamGet((char *)&nvram, sizeof(nvram), 0);

        /* Root FS.  The CFE RAM boot loader saved the rootfs offset that the
         * Linux image was loaded from.
         */
    PRINTK("rootfs_ofs=0x%8.8lx, part1ofs=0x%8.8lx, part2ofs=0x%8.8lx\n",
        rootfs_ofs, (unsigned long)nvram.ulNandPartOfsKb[NP_ROOTFS_1],
        (unsigned long)nvram.ulNandPartOfsKb[NP_ROOTFS_2]);

    if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
    {
        rootfs = NP_ROOTFS_1;
        rootfs_update = NP_ROOTFS_2;
    }
    else
    {
        if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_2] )
        {
            rootfs = NP_ROOTFS_2;
            rootfs_update = NP_ROOTFS_1;
        }
        else
        {
            /* Backward compatibility with old cferam. */
            extern unsigned char _text;
            unsigned long rootfs_ofs = *(unsigned long *) (&_text - 4);

            if( rootfs_ofs == nvram.ulNandPartOfsKb[NP_ROOTFS_1] )
            {
                rootfs = NP_ROOTFS_1;
                rootfs_update = NP_ROOTFS_2;
            }
            else
            {
                rootfs = NP_ROOTFS_2;
                rootfs_update = NP_ROOTFS_1;
            }
        }
    }

    /* RootFS partition */
    bcm63XX_nand_parts[0].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
    bcm63XX_nand_parts[0].size = nvram.ulNandPartSizeKb[rootfs]*1024;
    bcm63XX_nand_parts[0].ecclayout = mtd->ecclayout;

    /* This partition is used for flashing images */
    bcm63XX_nand_parts[4].offset = bcm63XX_nand_parts[0].offset;
    bcm63XX_nand_parts[4].size = bcm63XX_nand_parts[0].size;
    bcm63XX_nand_parts[4].ecclayout = mtd->ecclayout;

    if (is_split_partition (mtd, bcm63XX_nand_parts[0].offset, bcm63XX_nand_parts[0].size, &split_offset))
    {
        /* RootFS partition */
        bcm63XX_nand_parts[0].offset = split_offset;
        bcm63XX_nand_parts[0].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs]*1024);

        /* BootFS partition */
        bcm63XX_nand_parts[nr_parts].name = "bootfs";
        bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs]*1024;
        bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs]*1024;
        bcm63XX_nand_parts[nr_parts].ecclayout = mtd->ecclayout;

        if (kerSysIsRootfsSet() == false)
        {
            kerSysSetBootParm("ubi.mtd", "0");
            kerSysSetBootParm("root=", "ubi:rootfs_ubifs");
            kerSysSetBootParm("rootfstype=", "ubifs");
        }
    }
    else
    {
        if (kerSysIsRootfsSet() == false)
        {
            kerSysSetBootParm("root=", "mtd:rootfs");
            kerSysSetBootParm("rootfstype=", "jffs2");
        }
    }

    nr_parts++;

    /* RootFS_update partition */
    bcm63XX_nand_parts[1].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
    bcm63XX_nand_parts[1].size = nvram.ulNandPartSizeKb[rootfs_update]*1024;
    bcm63XX_nand_parts[1].ecclayout = mtd->ecclayout;

    /* This partition is used for flashing images */
    bcm63XX_nand_parts[5].offset = bcm63XX_nand_parts[1].offset;
    bcm63XX_nand_parts[5].size = bcm63XX_nand_parts[1].size;
    bcm63XX_nand_parts[5].ecclayout = mtd->ecclayout;

    if (is_split_partition (mtd, bcm63XX_nand_parts[1].offset, bcm63XX_nand_parts[1].size, &split_offset))
    {
        /* rootfs_update partition */
        bcm63XX_nand_parts[1].offset = split_offset;
        bcm63XX_nand_parts[1].size -= (split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024);

        /* bootfs_update partition */
        bcm63XX_nand_parts[nr_parts].name = "bootfs_update";
        bcm63XX_nand_parts[nr_parts].offset = nvram.ulNandPartOfsKb[rootfs_update]*1024;
        bcm63XX_nand_parts[nr_parts].size = split_offset - nvram.ulNandPartOfsKb[rootfs_update]*1024;
        bcm63XX_nand_parts[nr_parts].ecclayout = mtd->ecclayout;
    }

    nr_parts++;

    /* Data (psi, scratch pad) */
    bcm63XX_nand_parts[2].offset = nvram.ulNandPartOfsKb[NP_DATA] * 1024;
    bcm63XX_nand_parts[2].size = nvram.ulNandPartSizeKb[NP_DATA] * 1024;
    bcm63XX_nand_parts[2].ecclayout = mtd->ecclayout;

// ARCADYAN
// ASUS 
#ifdef BRCML05
    i=BCM_MAX_EXTRA_PARTITIONS-2; // skip DATA partition
    while(i >= 0)
    {
        if(nvram.part_info[i].size != 0xffff)
        {
            //sz_bits -- 0b01 -- MB, 0b10 - GB , 0b10, 0b11 - reserved
            switch((nvram.part_info[i].size & 0xc000)>>14)
            {
                case 0:
                    extra_single_part_size=1<<20;//1024*1024;
                    break;
                case 1:
                    extra_single_part_size=1<<30;//1024*1024*1024;
                    break;
                default:
                    extra_single_part_size=0;
                    break;
            }

            extra_single_part_size=(nvram.part_info[i].size&0x3fff)*extra_single_part_size;
            if((extra_single_part_size&(~((uint64_t)mtd->erasesize-1))) != extra_single_part_size)
                extra_single_part_size=(extra_single_part_size+mtd->erasesize);
            extra_single_part_size =  (extra_single_part_size) & (~((uint64_t)mtd->erasesize-1));
            if(extra_single_part_size > mtd->erasesize)
            {
                extra+=extra_single_part_size;
                bcm63XX_nand_parts[nr_parts].name = misc_mtd_partition_names[i];
                bcm63XX_nand_parts[nr_parts].offset = (nvram.ulNandPartOfsKb[NP_DATA] * 1024) - extra;
                bcm63XX_nand_parts[nr_parts].size = extra_single_part_size;
                bcm63XX_nand_parts[nr_parts].ecclayout = mtd->ecclayout;
                nr_parts++;
            }
        }
        i--;
    }
#endif
// ASUS
// ARCADYAN

    /* Boot and NVRAM data */
    bcm63XX_nand_parts[3].offset = nvram.ulNandPartOfsKb[NP_BOOT] * 1024;
    bcm63XX_nand_parts[3].size = nvram.ulNandPartSizeKb[NP_BOOT] * 1024;
    bcm63XX_nand_parts[3].ecclayout = mtd->ecclayout;

// ARCADYAN
// ASUS 
//20151106.TercesTsai: add arcadyan used answer machine and reserved part 
#ifdef ARC_ANSWERING_MACHINE
		// answer_machine
        bcm63XX_nand_parts[ANS_MACHINE_PART].offset = bcm63XX_nand_parts[2].offset + bcm63XX_nand_parts[2].size;
        bcm63XX_nand_parts[ANS_MACHINE_PART].size = NAND_ANSWER_MACHINE_SIZE;
        bcm63XX_nand_parts[ANS_MACHINE_PART].ecclayout = mtd->ecclayout;
#endif		
		// boot_flag
        bcm63XX_nand_parts[BOOT_FLAG_PART].offset = bcm63XX_nand_parts[BOOT_FLAG_PART-1].offset + bcm63XX_nand_parts[BOOT_FLAG_PART-1].size + NAND_RESERVED_SIZE; // 0x600000 for reserved
        bcm63XX_nand_parts[BOOT_FLAG_PART].size = NAND_BOOT_FLAG_SIZE;
        bcm63XX_nand_parts[BOOT_FLAG_PART].ecclayout = mtd->ecclayout;
		// pricfg
        bcm63XX_nand_parts[PRICFG_PART].offset = bcm63XX_nand_parts[PRICFG_PART-1].offset + bcm63XX_nand_parts[PRICFG_PART-1].size;
        bcm63XX_nand_parts[PRICFG_PART].size =  0x100000;
        bcm63XX_nand_parts[PRICFG_PART].ecclayout = mtd->ecclayout;
// ASUS
// ARCADYAN

    PRINTK("Part[0] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[0].name,
        bcm63XX_nand_parts[0].size, bcm63XX_nand_parts[0].offset);
    PRINTK("Part[1] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[1].name,
        bcm63XX_nand_parts[1].size, bcm63XX_nand_parts[1].offset);
    PRINTK("Part[2] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[2].name,
        bcm63XX_nand_parts[2].size, bcm63XX_nand_parts[2].offset);
    PRINTK("Part[3] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[3].name,
        bcm63XX_nand_parts[3].size, bcm63XX_nand_parts[3].offset);

// ARCADYAN
// ASUS 
#ifdef ARCADYAN

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,30)
		//New linux no longer requires proc_dir_entry->owner field.
#else
		proc_boot_partition->owner	= THIS_MODULE;
#endif

		proc_boot_partition = create_proc_entry("boot_partition", 0444, NULL);
		if( NULL == proc_boot_partition ){
			printk(KERN_WARNING "WARNING: failed to create proc/boot_partition \n");
		}else{
			brcmnand_boot_partition = rootfs;
			proc_boot_partition->read_proc = brcmnanddrv_proc_get_bootpartition;
		}

#endif	/* ARCADYAN */
// ASUS
// ARCADYAN

    mtd_device_register(mtd, bcm63XX_nand_parts, nr_parts);

    return(nr_parts);
}


static void __devinit 
brcmnanddrv_setup_mtd_partitions(struct brcmnand_info* nandinfo)
{
    int boot_from_nand;

    if (flash_get_flash_type() == FLASH_IFC_NAND)
        boot_from_nand = 1;
    else
        boot_from_nand = 0;

    if( boot_from_nand == 0 )
    {
        nandinfo->nr_parts = 1;
        nandinfo->parts = bcm63XX_nand_parts;

        bcm63XX_nand_parts[0].name = "data";
        bcm63XX_nand_parts[0].offset = 0;
        if( device_size(&(nandinfo->mtd)) < NAND_BBT_THRESHOLD_KB )
        {
            bcm63XX_nand_parts[0].size =
                device_size(&(nandinfo->mtd)) - (NAND_BBT_SMALL_SIZE_KB*1024);
        }
        else
        {
            bcm63XX_nand_parts[0].size =
                device_size(&(nandinfo->mtd)) - (NAND_BBT_BIG_SIZE_KB*1024);
        }
        bcm63XX_nand_parts[0].ecclayout = nandinfo->mtd.ecclayout;

        PRINTK("Part[0] name=%s, size=%llx, ofs=%llx\n", bcm63XX_nand_parts[0].name,
            bcm63XX_nand_parts[0].size, bcm63XX_nand_parts[0].offset);
    }
    else
    {
        nandinfo->nr_parts = setup_mtd_parts(&nandinfo->mtd);
        nandinfo->parts = bcm63XX_nand_parts;
    }
}


static int __devinit brcmnanddrv_probe(struct platform_device *pdev)
{
    static int csi = 0; // Index into dev/nandInfo array
    int cs = 0;  // Chip Select
    int err = 0;
    struct brcmnand_info* info = NULL;
    static struct brcmnand_ctrl* ctrl = (struct brcmnand_ctrl*) 0;

    if(!gPageBuffer &&
       (gPageBuffer = kmalloc(sizeof(struct nand_buffers),GFP_KERNEL)) == NULL)
    {
        err = -ENOMEM;
    }
    else
    {
        if( (ctrl = kmalloc(sizeof(struct brcmnand_ctrl), GFP_KERNEL)) != NULL)
        {
            memset(ctrl, 0, sizeof(struct brcmnand_ctrl));
            ctrl->state = FL_READY;
            init_waitqueue_head(&ctrl->wq);
            spin_lock_init(&ctrl->chip_lock);

            if((info=kmalloc(sizeof(struct brcmnand_info),GFP_KERNEL)) != NULL)
            {
                gNandInfo[csi] = info;
                memset(info, 0, sizeof(struct brcmnand_info));
                info->brcmnand.ctrl = ctrl;
                info->brcmnand.ctrl->numchips = gNumNand = 1;
                info->brcmnand.csi = csi;

                /* For now all devices share the same buffer */
                info->brcmnand.ctrl->buffers =
                    (struct nand_buffers*) gPageBuffer;

                info->brcmnand.ctrl->numchips = gNumNand; 
                info->brcmnand.chip_shift = 0; // Only 1 chip
                info->brcmnand.priv = &info->mtd;
                info->mtd.name = dev_name(&pdev->dev);
                info->mtd.priv = &info->brcmnand;
                info->mtd.owner = THIS_MODULE;

                /* Enable the following for a flash based bad block table */
                info->brcmnand.options |= NAND_BBT_USE_FLASH;

                /* Each chip now will have its own BBT (per mtd handle) */
                if (brcmnand_scan(&info->mtd, cs, gNumNand) == 0)
                {
                    PRINTK("Master size=%08llx\n", info->mtd.size); 
                    brcmnanddrv_setup_mtd_partitions(info);
                    dev_set_drvdata(&pdev->dev, info);
                }
                else
                    err = -ENXIO;

            }
            else
                err = -ENOMEM;

        }
        else
            err = -ENOMEM;
    }

    if( err )
    {
        if( gPageBuffer )
        {
            kfree(gPageBuffer);
            gPageBuffer = NULL;
        }

        if( ctrl )
        {
            kfree(ctrl);
            ctrl = NULL;
        }

        if( info )
        {
            kfree(info);
            info = NULL;
        }
    }

    return( err );
}

static int __devexit brcmnanddrv_remove(struct platform_device *pdev)
{
    struct brcmnand_info *info = dev_get_drvdata(&pdev->dev);

    dev_set_drvdata(&pdev->dev, NULL);

    if (info)
    {
        mtd_device_unregister(&info->mtd);

        brcmnand_release(&info->mtd);
        kfree(gPageBuffer);
        kfree(info);
    }

    return 0;
}

static int __init brcmnanddrv_init(void)
{
    int ret = 0;
    int csi;
    int ncsi;
    char cmd[32] = "\0";
    struct platform_device *pdev;

    if (flash_get_flash_type() != FLASH_IFC_NAND)
        return -ENODEV;

    kerSysBlParmsGetStr(NAND_COMMAND_NAME, cmd, sizeof(cmd));
    PRINTK("%s: brcmnanddrv_init - NANDCMD='%s'\n", __FUNCTION__, cmd);

    if (cmd[0])
    {
        if (strcmp(cmd, "rescan") == 0)
            gClearBBT = 1;
        else if (strcmp(cmd, "showbbt") == 0)
            gClearBBT = 2;
        else if (strcmp(cmd, "eraseall") == 0)
            gClearBBT = 8;
        else if (strcmp(cmd, "erase") == 0)
            gClearBBT = 7;
        else if (strcmp(cmd, "clearbbt") == 0)
            gClearBBT = 9;
        else if (strcmp(cmd, "showcet") == 0)
            gClearCET = 1;
        else if (strcmp(cmd, "resetcet") == 0)
            gClearCET = 2;
        else if (strcmp(cmd, "disablecet") == 0)
            gClearCET = 3;
        else
            printk(KERN_WARNING "%s: unknown command '%s'\n",
                __FUNCTION__, cmd);
    }
    
    for (csi=0; csi<NAND_MAX_CS; csi++)
    {
        gNandTiming1[csi] = 0;
        gNandTiming2[csi] = 0;
        gAccControl[csi] = 0;
        gNandConfig[csi] = 0;
    }

    if (nacc == 1)
        PRINTK("%s: nacc=%d, gAccControl[0]=%08lx, gNandConfig[0]=%08lx\n", \
            __FUNCTION__, nacc, acc[0], nandcfg[0]);

    if (nacc>1)
        PRINTK("%s: nacc=%d, gAccControl[1]=%08lx, gNandConfig[1]=%08lx\n", \
            __FUNCTION__, nacc, acc[1], nandcfg[1]);

    for (csi=0; csi<nacc; csi++)
        gAccControl[csi] = acc[csi];

    for (csi=0; csi<ncfg; csi++)
        gNandConfig[csi] = nandcfg[csi];

    ncsi = max(nt1, nt2);
    for (csi=0; csi<ncsi; csi++)
    {
        if (nt1 && csi < nt1)
            gNandTiming1[csi] = t1[csi];

        if (nt2 && csi < nt2)
            gNandTiming2[csi] = t2[csi];
        
    }

    printk (KERN_INFO DRIVER_INFO " (BrcmNand Controller)\n");
    if( (pdev = platform_device_alloc(DRIVER_NAME, 0)) != NULL )
    {
        platform_device_add(pdev);
        platform_device_put(pdev);
        ret = platform_driver_register(&brcmnand_platform_driver);
        if (ret >= 0)
            request_resource(&iomem_resource, &brcmnand_resources[0]);
        else
            printk("brcmnanddrv_init: driver_register failed, err=%d\n", ret);
    }
    else
        ret = -ENODEV;

    return ret;
}

static void __exit brcmnanddrv_exit(void)
{
    release_resource(&brcmnand_resources[0]);
    platform_driver_unregister(&brcmnand_platform_driver);
}


module_init(brcmnanddrv_init);
module_exit(brcmnanddrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ton Truong <ttruong@broadcom.com>");
MODULE_DESCRIPTION("Broadcom NAND flash driver");

#endif //CONFIG_BCM_KF_MTD_BCMNAND
