/***********************************************************************
 * <:copyright-BRCM:2007:DUAL/GPL:standard
 * 
 *    Copyright (c) 2007 Broadcom 
 *    All Rights Reserved
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // types
#include <string.h>
#include <net/if.h>

#include "bcm_crc.h"
#include "bcm_flashutil.h"

#include <sys/ioctl.h>
#include <unistd.h> // close

#include "bcmTag.h"
#include "board.h"

#include <fcntl.h> // for open
#include <mtd/mtd-user.h>
#include <linux/jffs2.h>
#include <linux/errno.h>

#define IS_ERR_OR_NULL(x) ((x)==0)
#define IS_ERR(x) ((x)<0)

#define ERROR -1
#define SUCCESS 0

#define MAX_MTD_NAME_SIZE 64
#define NAND_CFE_RAM_NAME_LEN     strlen(NAND_CFE_RAM_NAME)
#define NAND_CFE_RAM_NAME_CMP_LEN (strlen(NAND_CFE_RAM_NAME) - 3)


/* #define FLASHUTIL_DEBUG 1 */
#if defined(FLASHUTIL_DEBUG)
#define flashutil_debug(fmt, arg...) \
  fprintf(stderr, "%s.%u: " fmt "\n", __FUNCTION__, __LINE__, ##arg)
#else
#define flashutil_debug(fmt, arg...)
#endif
#define flashutil_print(fmt, arg...) \
  fprintf(stderr, "%s.%u: " fmt "\n", __FUNCTION__, __LINE__, ##arg)
#define flashutil_error(fmt, arg...) \
  fprintf(stderr, "ERROR[%s.%u]: " fmt "\n", __FUNCTION__, __LINE__, ##arg)


#ifdef DESKTOP_LINUX
static int fake_board_ioctl(uint32_t command, BOARD_IOCTL_PARMS *ioctlParms)
{
   flashutil_print("command=%x, parms=%p", command, ioctlParms);
   return 0;
}
#endif

int bcm_boardIoctl(uint32_t boardIoctl,
                         BOARD_IOCTL_ACTION action,
                         char *string,
                         int32_t strLen,
                         int32_t offset,
                         void *data)
{
    BOARD_IOCTL_PARMS ioctlParms;
    int boardFd = 0;
    int rc;

    int ret = 0;

#ifdef DESKTOP_LINUX
    /* don't open anything, ioctl to this fd will be faked anyways */
    boardFd = 77777;
#else
    boardFd = open(BOARD_DEVICE_NAME, O_RDWR);
#endif

    if ( boardFd != -1 )
    {
        ioctlParms.string = string;
        ioctlParms.strLen = strLen;
        ioctlParms.offset = offset;
        ioctlParms.action = action;
        ioctlParms.buf    = data;
        ioctlParms.result = -1;

#ifdef DESKTOP_LINUX
        rc = fake_board_ioctl(boardIoctl, &ioctlParms);
#else
        rc = ioctl(boardFd, boardIoctl, &ioctlParms);
        close(boardFd);
#endif

        if (rc < 0)
        {
           fprintf(stderr, "ERROR!!! ioctl boardIoctl=0x%x action=%d rc=%d", boardIoctl, action, rc);
           ret = ERROR;
        }

        /* ioctl specific return data */
        if (!IS_ERR(ret))
        {
           if ((boardIoctl == BOARD_IOCTL_GET_PSI_SIZE) ||
               (boardIoctl == BOARD_IOCTL_GET_BACKUP_PSI_SIZE) ||
               (boardIoctl == BOARD_IOCTL_GET_SYSLOG_SIZE) ||
               (boardIoctl == BOARD_IOCTL_GET_CHIP_ID) ||
               (boardIoctl == BOARD_IOCTL_GET_CHIP_REV) ||
               (boardIoctl == BOARD_IOCTL_GET_NUM_ENET_MACS) ||
               (boardIoctl == BOARD_IOCTL_GET_NUM_FE_PORTS) ||
               (boardIoctl == BOARD_IOCTL_GET_NUM_GE_PORTS) ||
               (boardIoctl == BOARD_IOCTL_GET_PORT_MAC_TYPE) ||
               (boardIoctl == BOARD_IOCTL_GET_NUM_VOIP_PORTS) ||
               (boardIoctl == BOARD_IOCTL_GET_SWITCH_PORT_MAP) ||
               (boardIoctl == BOARD_IOCTL_GET_NUM_ENET_PORTS) ||
               (boardIoctl == BOARD_IOCTL_GET_SDRAM_SIZE) ||
               (boardIoctl == BOARD_IOCTL_GET_BTRM_BOOT) ||
               (boardIoctl == BOARD_IOCTL_GET_BOOT_SECURE) ||
               (boardIoctl == BOARD_IOCTL_FLASH_READ && action == FLASH_SIZE))
           {
              if (data != NULL)
              {
                 *((uint32_t *)data) = (uint32_t) ioctlParms.result;
              }
           }
        }
    }
    else
    {
       fprintf(stderr, "ERROR!!! Unable to open device %s", BOARD_DEVICE_NAME);
       ret = ERROR;
    }

    return ret;
}


mtd_info_t * get_mtd_device_nm(const char * check, int * mtd_fd)
{
#ifndef DESKTOP_LINUX
    mtd_info_t * mtd;
    FILE *fp;
    char mtdn[MAX_MTD_NAME_SIZE];
    char line[MAX_MTD_NAME_SIZE];
    char name[MAX_MTD_NAME_SIZE];
    char compare[MAX_MTD_NAME_SIZE];

    if ( (mtd = malloc(sizeof(mtd_info_t))) == 0)
    {
        fprintf(stderr, "ERROR!!! Could not allocate memory for mtd structure!\n");
        return(0);
    }

    if ( (fp = fopen("/proc/mtd","r")) == 0)
    {
        fprintf(stderr, "ERROR!!! Could not open /proc/mtd\n");
        free(mtd);
        return(0);
    }

    snprintf(compare, sizeof(compare), "%s%s%s", "\"", check, "\"");

    while(fgets(line, sizeof(line), fp))
    {
        sscanf(line, "%s %*s %*s %s", mtdn, name);

        if(!strcmp(name, compare))
        {
            fclose(fp);

            mtdn[strlen(mtdn) - 1] = 0; // get rid of trailing colon
            sprintf(name, "%s%s", "/dev/", mtdn);

            if ((*mtd_fd = open(name, O_RDWR)) < 0)
            {
                fprintf(stderr, "ERROR!!! Could not open %s\n", name);
                free(mtd);
                return(0);
            }

            if (ioctl(*mtd_fd, MEMGETINFO, mtd) < 0)
            {
                fprintf(stderr, "ERROR!!! Could not get MTD information!\n");
                close(*mtd_fd);
                free(mtd);
                return(0);
            }

            flashutil_debug("MTD info: \n"
              "type=0x%x, flags=0x%x, size=0x%x \n"
              "erasesize=0x%x, writesize=0x%x, oobsize=0x%x",
              mtd->type, mtd->flags, mtd->size,
              mtd->erasesize, mtd->writesize, mtd->oobsize);
            return(mtd);
        }
    }

    fclose(fp);
    free(mtd);
    printf("MTD partition/device %s not opened\n", check);
    return(0);
#else
    mtd_info_t * mtd;

    flashutil_print("MTD partition/device %s", check);
    if ((mtd = malloc(sizeof(mtd_info_t))) == 0)
    {
        flashutil_error("malloc(mtd_info_t) failed");
        return(0);
    }

    mtd->type = 0;
    mtd->flags = 0;
    mtd->size = 0x60000000;
    mtd->erasesize = 0x20000;
    mtd->writesize = 0;
    mtd->oobsize = 0;
    mtd->padding = 0;
    *mtd_fd = 0;

    flashutil_debug("MTD info: \n"
      "type=0x%x, flags=0x%x, size=0x%x \n"
      "erasesize=0x%x, writesize=0x%x, oobsize=0x%x",
      mtd->type, mtd->flags, mtd->size,
      mtd->erasesize, mtd->writesize, mtd->oobsize);

    return(mtd);
#endif
}


void put_mtd_device(mtd_info_t * mtd, int mtd_fd __attribute__((unused)))
{
    flashutil_debug("mtd=%p, mtd_fd=%d", mtd, mtd_fd);
    free(mtd);
#ifndef DESKTOP_LINUX
    close(mtd_fd);
#endif
}


static unsigned int nvramDataOffset(const mtd_info_t * mtd  __attribute__((unused)))
{
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    return( (0x0580+(IMAGE_OFFSET-(IMAGE_OFFSET/((unsigned int)mtd->erasesize))*((unsigned int)mtd->erasesize))) );
#else
    return(NVRAM_DATA_OFFSET);
#endif
}


static unsigned int nvramSector(const mtd_info_t * mtd __attribute__((unused)))
{
#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    return( (IMAGE_OFFSET/((unsigned int)mtd->erasesize)) );
#else
    return(NVRAM_SECTOR);
#endif
}



/** read the nvramData struct from the in-memory copy of nvram.
 * The caller is not required to have flashImageMutex when calling this
 * function.  However, if the caller is doing a read-modify-write of
 * the nvram data, then the caller must hold flashImageMutex.  This function
 * does not know what the caller is going to do with this data, so it
 * cannot assert flashImageMutex held or not when this function is called.
 *
 * @return pointer to NVRAM_DATA buffer which the caller must free
 *         or NULL if there was an error
 */
int readNvramData(void *nvramData)
{
#ifndef DESKTOP_LINUX
    uint32_t crc = CRC_INITIAL_VALUE, savedCrc;
    mtd_info_t * mtd;
    int mtd_fd;
    int status = 0;
    NVRAM_DATA *pNvramData = (NVRAM_DATA*)nvramData;

    if ( (mtd = get_mtd_device_nm("nvram", &mtd_fd)) == 0)
    {
        fprintf(stderr, "ERROR!!! Could not open nvram partition!\n");
        return(0);
    }

    if (lseek(mtd_fd, nvramDataOffset(mtd), SEEK_SET) < 0)
    {
        fprintf(stderr, "ERROR!!! Could not seek in nvram partition!\n");
    }
    else if (read(mtd_fd, pNvramData, sizeof(NVRAM_DATA)) < 0)
    {
        fprintf(stderr, "ERROR!!! Cound not get NVRAM data!\n");
    }

    put_mtd_device(mtd, mtd_fd);

    savedCrc = pNvramData->ulCheckSum;
    pNvramData->ulCheckSum = 0;
    crc = crc_getCrc32((unsigned char *)pNvramData, sizeof(NVRAM_DATA), crc);
    if (savedCrc == crc)
    {
        // this can happen if we write a new cfe image into flash.
        // The new image will have an invalid nvram section which will
        // get updated to the inMemNvramData.  We detect it here and
        // commonImageWrite will restore previous copy of nvram data.
        //kfree(pNvramData);
        status = 1;
    }

    return(status);
#else
    flashutil_print("readNvramData(%p)", nvramData);
    return 1;
#endif
}


int getFlashInfo(unsigned int *flags)
{
    int found = 0;

    if (flags == NULL)
    {
       fprintf(stderr, "flags is NULL!");
       return ERROR;
    }
    else
    {
       *flags = 0;
    }

#ifndef DESKTOP_LINUX
    {
        FILE *fp;
        char line[256]={0};
        char name[MAX_MTD_NAME_SIZE]={0};

        fp = fopen("/proc/mtd","r");
        if (fp == NULL)
        {
           fprintf(stderr, "Could not open /proc/mtd");
           return ERROR;
        }

        while(fgets(line, sizeof(line), fp))
        {
            sscanf(line, "%*s %*s %*s %s", name);

            if(!strcmp("\"image_update\"", name))
            {
                found = 1;
            }
        }

        fclose(fp);
    }
#else
    found = 1;
#endif

    if (found)
    {
       *flags |= FLASH_INFO_FLAG_NAND;
    }
    else
    {
       *flags |= FLASH_INFO_FLAG_NOR;
    }

    return SUCCESS;
}


unsigned int otp_is_btrm_boot(void)
{
    unsigned int value = 0;

#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848) || defined(CONFIG_BCM963268)
#ifndef DESKTOP_LINUX
    bcm_boardIoctl(BOARD_IOCTL_GET_BTRM_BOOT, 0, NULL, 0, 0, (void *)&value);
#endif
#endif
    return(value);
}

unsigned int otp_is_boot_secure(void)
{
    unsigned int value = 0;

#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848) || defined(CONFIG_BCM963268)
#ifndef DESKTOP_LINUX
    bcm_boardIoctl(BOARD_IOCTL_GET_BOOT_SECURE, 0, NULL, 0, 0, (void *)&value);
#endif
#endif

    return(value);
}


int getSequenceNumber(int imageNumber)
{
#ifdef DESKTOP_LINUX
    int seqNumber = 1;

    flashutil_print("imageNumber=%d", imageNumber);
#else
    int seqNumber = -1;

    {
        /* NAND Flash */
        {
            char fname[] = NAND_CFE_RAM_NAME;
            char cferam_buf[32], cferam_fmt[32];
            int i;
            FILE *fp;

            /* If full secure boot is in play, the CFE RAM file is the encrypted version */
            if (otp_is_boot_secure())
            {
                strcpy(fname, NAND_CFE_RAM_SECBT_NAME);
            }

            if( imageNumber == 2 )
            { // get from other image
                int mtd0_fd;
                int rc;
                mtd_info_t * mtd0 = get_mtd_device_nm("bootfs_update", &mtd0_fd);

                strcpy(cferam_fmt, "/mnt/");

                if( !IS_ERR_OR_NULL(mtd0) )
                {
                    rc = system("mount -t jffs2 mtd:bootfs_update /mnt -r");
                    put_mtd_device(mtd0, mtd0_fd);
                }
                else
                {
                    rc = system("mount -t jffs2 mtd:rootfs_update /mnt -r");
                }

                if (rc < 0)
                {
                   fprintf(stderr, "ERROR!!! mount command failed!\n");
                }
            }
            else
            { // get from current booted image
                int mtd0_fd;
                mtd_info_t * mtd0 = get_mtd_device_nm("bootfs", &mtd0_fd);

                if( !IS_ERR_OR_NULL(mtd0) )
                {
                    strcpy(cferam_fmt, "/bootfs/");
                    put_mtd_device(mtd0, mtd0_fd);
                }
                else
                {
                    strcpy(cferam_fmt, "/");
                }
            }

            /* Find the sequence number of the specified partition. */
            fname[strlen(fname) - 3] = '\0'; /* remove last three chars */
            strcat(cferam_fmt, fname);
            strcat(cferam_fmt, "%3.3d");

            for( i = 0; i < 1000; i++ )
            {
                sprintf(cferam_buf, cferam_fmt, i);
                fp = fopen(cferam_buf, "r");
                if (fp != NULL)
                {
                    fclose(fp);

                    /* Seqence number found. */
                    seqNumber = i;
                    break;
                }
            }

            if( imageNumber == 2 )
            {
                if (system("umount /mnt") < 0)
                {
                   fprintf(stderr, "ERROR!!! umount command failed!\n");
                }
            }
        }
    }

#endif
    return(seqNumber);
}


#define je16_to_cpu(x) ((x).v16)
#define je32_to_cpu(x) ((x).v32)
#define cpu_to_je32(x) ((jint32_t){(x)})


char *searchCferamSeqNum(char *fname, char *dataPtr, int dataLen, int *donep, char **dirPP)
{
    char *p;
    struct jffs2_raw_dirent *pdir;
    unsigned long version = 0;
    int fname_actual_len = strlen(fname);
    int fname_cmp_len = strlen(fname) - 3;
    char *retPtr = NULL;
    int done = 0;

    p = dataPtr;
    while( p < dataPtr + dataLen )
    {
        pdir = (struct jffs2_raw_dirent *) p;
        if( je16_to_cpu(pdir->magic) == JFFS2_MAGIC_BITMASK )
        {
            if( je16_to_cpu(pdir->nodetype) == JFFS2_NODETYPE_DIRENT &&
                fname_actual_len == pdir->nsize &&
                !memcmp(fname, pdir->name, fname_cmp_len) &&
                je32_to_cpu(pdir->version) > version &&
                je32_to_cpu(pdir->ino) != 0 )
            {
                done = 1;
                *dirPP = (char*)pdir;
                retPtr = dataPtr;
                break;
            }

            p += (je32_to_cpu(pdir->totlen) + 0x03) & ~0x03;
        }
        else
        {
            done = 1;
            break;
        }
    }

    flashutil_debug("fname=%s, data=%p, len=%d, done=%d", fname, dataPtr, dataLen, done);

    *donep = done;
    return retPtr;
}

void updateCferamSeqNum(char *dirP, int seq)
{
    char *p;
    struct jffs2_raw_dirent *pdir = (struct jffs2_raw_dirent*)dirP;

    /* File cferam.000 found. Change the extension to the
     * new sequence number and recalculate file name CRC.
     */
    p = (char *)pdir->name + NAND_CFE_RAM_NAME_CMP_LEN;
    p[0] = (seq / 100) + '0';
    p[1] = ((seq % 100) / 10) + '0';
    p[2] = ((seq % 100) % 10) + '0';
    p[3] = '\0';

#if 0 /* Note: je32_to_cpu(pdir->name_crc) does not seem to be correct. */
    pdir->name_crc = cpu_to_je32(crc_getCrc32(pdir->name,
      (uint32_t)NAND_CFE_RAM_NAME_LEN, 0));
#else
    je32_to_cpu(pdir->name_crc) = crc_getCrc32(pdir->name,
      (uint32_t)NAND_CFE_RAM_NAME_LEN, 0);
#endif

    flashutil_debug("data=%p, seq=%d", dirP, seq);
}

char *getCferamName(void)
{
    /* If full secure boot is in play, the CFE RAM file is the encrypted version */
    if (otp_is_boot_secure())
    {
        return NAND_CFE_RAM_SECBT_NAME;
    }
    return NAND_CFE_RAM_NAME;
}

/*
 * nandUpdateSeqNum
 *
 * Read the sequence number from rootfs partition only.  The sequence number is
 * the extension on the cferam file.  Add one to the sequence number
 * and change the extenstion of the cferam in the image to be flashed to that
 * number.
 */
static char *nandUpdateSeqNum(char *imagePtr, int imageSize, int blkLen)
{
    char fname[] = NAND_CFE_RAM_NAME;
    int seq = -1;
    char *retPtr = NULL;

    strcpy(fname, getCferamName());
#ifndef DESKTOP_LINUX
    seq = getSequenceNumber(1);
#else
    seq = 1;
#endif

    if( seq != -1 )
    {
        char *buf;
        int done = 0;
        char *dirP;

        while (( *(unsigned short *) imagePtr != JFFS2_MAGIC_BITMASK ) && (imageSize > 0))
        {
            imagePtr += blkLen;
            imageSize -= blkLen;
        }

        /* Confirm that we did find a JFFS2_MAGIC_BITMASK. If not, we are done */
        if (imageSize <= 0)
        {
            done = 1;
        }

        /* Increment the new highest sequence number. Add it to the CFE RAM
         * file name.
         */
        seq++;
        if (seq > 999)
        {
            seq = 0;
        }

        /* Search the image and replace the last three characters of file
         * cferam.000 with the new sequence number.
         */
        for(buf = imagePtr; buf < imagePtr+imageSize && done == 0; buf += blkLen)
        {
            retPtr = searchCferamSeqNum(fname, buf, blkLen, &done, &dirP);
        }

        if ((retPtr != NULL) && (done == 1))
        {
            updateCferamSeqNum(dirP, seq);
        }
    }
    else
    {
        flashutil_error("Could not find current image sequence number");
    }

    return(retPtr);
}

static int nandIsBadBlk(int blk_addr, int mtd_fd)
{
#ifndef DESKTOP_LINUX
   loff_t offset = blk_addr;

   int ret = ioctl(mtd_fd, MEMGETBADBLOCK, &offset);  
   if ( ret > 0) {
       fprintf(stderr, "Bad block at 0x%x\n", blk_addr);
       return 1;
   }

   if( ret < 0 ) {
       /* bad block check not available?? Treat as bad block. We are only dealing with NAND */
       fprintf(stderr, "MTD get bad block failed at 0x%x, ret %d\n", blk_addr, ret);
       return 1;
   }
#else
   flashutil_print("addr=0x%x, fd=%d", blk_addr, mtd_fd);
#endif  
   return(0);
}


/* Erase the specified NAND flash block. */
int nandEraseBlk(mtd_info_t *mtd, int blk_addr, int mtd_fd)
{
#ifndef DESKTOP_LINUX
   erase_info_t erase;

   erase.start = blk_addr;
   erase.length = mtd->erasesize;

   if (ioctl(mtd_fd, MEMERASE, &erase) < 0)
   {
      fprintf(stderr, "Could not erase block, skipping\n");
      return(-1);
   }
   flashutil_debug("mtd=%p, addr=0x%x, fd=%d", mtd, blk_addr, mtd_fd);
#else
   flashutil_print("mtd=%p, addr=0x%x, fd=%d", mtd, blk_addr, mtd_fd);
#endif

   return(0);
}


/* Note: the following two functions are for CFEROM flashing debugging, will be removed. */
int nandEraseBlk_print(mtd_info_t *mtd, int blk_addr, int mtd_fd)
{
   flashutil_print("mtd=%p, addr=0x%x, fd=%d", mtd, blk_addr, mtd_fd);
   return(0);
}

int nandWriteBlk_print(mtd_info_t *mtd, int blk_addr, int data_len, char *data_ptr, int mtd_fd, int write_JFFS2_clean_marker)
{
   flashutil_print("mtd=%p, blkaddr=0x%x, len=%d, data=%p, fd=%d, marker=%d",
     mtd, blk_addr, data_len, data_ptr, mtd_fd, write_JFFS2_clean_marker);
   return 0;
}


/* Write data, must pass function an aligned block address */
int nandWriteBlk(mtd_info_t *mtd, int blk_addr, int data_len, char *data_ptr, int mtd_fd, int write_JFFS2_clean_marker)
{
#ifndef DESKTOP_LINUX
#ifdef CONFIG_CPU_LITTLE_ENDIAN
   const unsigned short jffs2_clean_marker[] = {JFFS2_MAGIC_BITMASK, JFFS2_NODETYPE_CLEANMARKER, 0x0008, 0x0000};
#else
   const unsigned short jffs2_clean_marker[] = {JFFS2_MAGIC_BITMASK, JFFS2_NODETYPE_CLEANMARKER, 0x0000, 0x0008};
#endif
   struct mtd_write_req ops;
   int page_addr, byte, sts = 0;

   flashutil_debug("mtd=%p, blkaddr=0x%x, len=%d, data=%p, fd=%d, marker=%d",
     mtd, blk_addr, data_len, data_ptr, mtd_fd, write_JFFS2_clean_marker);

   for (page_addr = 0; page_addr < data_len; page_addr += mtd->writesize)
   {
      memset(&ops, 0x00, sizeof(ops));

      // check to see if whole page is FFs
      for (byte = 0; (byte < (int)mtd->writesize) && ((page_addr + byte) < data_len); byte++)
      {
         if ( *(unsigned char *)(data_ptr + page_addr + byte) != 0xFF )
         {
            ops.start = blk_addr + page_addr;
            ops.len = (int) mtd->writesize < (data_len - page_addr) ? mtd->writesize : (unsigned int) data_len - page_addr;
            ops.usr_data = (uint64_t)(unsigned long)(data_ptr + page_addr);
            break;
         }
      }

      if (write_JFFS2_clean_marker)
      {
         ops.mode = MTD_OPS_AUTO_OOB;
         ops.usr_oob = (uint64_t)(unsigned long)jffs2_clean_marker;
         ops.ooblen = sizeof(jffs2_clean_marker);
         write_JFFS2_clean_marker = 0; // write clean marker to first page only
      }

      if (ops.len || ops.ooblen)
      {
         if ((sts = ioctl(mtd_fd, MEMWRITE, &ops)) != 0)
         {
            break;
         }
      }
   }

   return(sts);
#else
   flashutil_print("mtd=%p, blkaddr=0x%x, len=%d, data=%p, fd=%d, marker=%d",
     mtd, blk_addr, data_len, data_ptr, mtd_fd, write_JFFS2_clean_marker);
   return 0;
#endif
}


#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
static int nandReadBlk(mtd_info_t *mtd __attribute__((unused)),
                       int blk_addr, int data_len, char *data_ptr, int mtd_fd)
{
#ifndef DESKTOP_LINUX
   if (lseek(mtd_fd, blk_addr, SEEK_SET) < 0)
   {
      fprintf(stderr, "ERROR!!! Could not seek in block!\n");
      return(-1);
   }

   if (read(mtd_fd, data_ptr, data_len) < 0)
   {
      fprintf(stderr, "ERROR!!! Could not read block!\n");
      return(-1);
   }
#else
   flashutil_print("mtd=%p, blkaddr=0x%x, len=%d, data=%p, fd=%d",
     mtd, blk_addr, data_len, data_ptr, mtd_fd);
#endif

   return(0);
}
#endif


int get_image_version( uint8_t *imagePtr, int imageSize, int erasesize, char *image_name, int image_name_len )
{
    unsigned char *buf, *p;
    char fname[] = IMAGE_VERSION_FILE_NAME;
    int fname_len = strlen(fname);
    struct jffs2_raw_dirent *pdir;
    struct jffs2_raw_inode *pino;
    unsigned long version = 0;
    jint32_t ino;

    while (( *(unsigned short *) imagePtr != JFFS2_MAGIC_BITMASK ) && (imageSize > 0))
    {
        imagePtr += erasesize;
        imageSize -= erasesize;
    }

    /* Confirm that we did find a JFFS2_MAGIC_BITMASK. If not, we are done */
    if (imageSize <= 0)
    {
        return -1;
    }


    for(buf = imagePtr; buf < imagePtr+imageSize; buf += 4)
    {
        p = buf;
        while( p < buf + erasesize )
        {
            pdir = (struct jffs2_raw_dirent *) p;
            if( je16_to_cpu(pdir->magic) == JFFS2_MAGIC_BITMASK  &&
                  je32_to_cpu(pdir->hdr_crc) == crc_getCrc32(p,
                        sizeof(struct jffs2_unknown_node) - 4, 0) )
            {
                if( je16_to_cpu(pdir->nodetype) == JFFS2_NODETYPE_DIRENT)
                {

                   if(
                      fname_len == pdir->nsize &&
                      !memcmp(fname, pdir->name, fname_len) &&
                      je32_to_cpu(pdir->version) > version &&
                      je32_to_cpu(pdir->ino) != 0 )
                   {
                      ino.v32=je32_to_cpu(pdir->ino);
                      je32_to_cpu(pdir->name_crc) = crc_getCrc32(pdir->name,
                      (unsigned long) fname_len, 0);
                      version = je32_to_cpu(pdir->version);
                      p += (je32_to_cpu(pdir->totlen) + 0x03) & ~0x03;
                      pino = (struct jffs2_raw_inode *) p;
                      if( je16_to_cpu(pino->magic) == JFFS2_MAGIC_BITMASK )
                      {
                         if(je16_to_cpu(pino->nodetype)==JFFS2_NODETYPE_INODE &&
                            je32_to_cpu(pino->ino) == ino.v32)
                         {
                            if(image_name != NULL)
                            {
                               memcpy(image_name, pino->data, (int)pino->dsize.v32 > image_name_len ? image_name_len: (int)pino->dsize.v32);
                               return 0;
                            }
                            else
                               break;
                         }
                      }
                   }
               }
               p += (je32_to_cpu(pdir->totlen) + 0x03) & ~0x03;
           }
           else
              break;
       }
    }
    return(-1);// image version not found
}

// NAND flash bcm image
// return:
// 0 - ok
// !0 - the sector number fail to be flashed (should not be 0)
static int bcmNandImageSet( char *rootfs_part, char *image_ptr, int img_size, NVRAM_DATA * inMemNvramData_buf)
{
    int sts = -1;
    int blk_addr;
    char *cferam_ptr;
    int rsrvd_for_cferam;
    char *end_ptr = image_ptr + img_size;
    int mtd0_fd;
    mtd_info_t * mtd0 = get_mtd_device_nm("image", &mtd0_fd);
    WFI_TAG wt = {0,0,0,0,0};

    uint32_t btrmEnabled = otp_is_btrm_boot();

    /* Reserve room to flash block containing directory entry for CFERAM. */
    rsrvd_for_cferam = 8 * mtd0->erasesize;

    if( !IS_ERR_OR_NULL(mtd0) )
    {
        int blksize = mtd0->erasesize / 1024;

        memcpy(&wt, end_ptr, sizeof(wt));

        if (validateWfiTag((void*)&wt, blksize, btrmEnabled) < 0)
        {
            put_mtd_device(mtd0, mtd0_fd);
            return -1;
        }
        else
        {
            put_mtd_device(mtd0, mtd0_fd);
            mtd0 = get_mtd_device_nm(rootfs_part, &mtd0_fd);

            if( IS_ERR_OR_NULL(mtd0) )
            {
                fprintf(stderr, "ERROR!!! Could not access MTD partition %s\n", rootfs_part);
                return -1;
            }

            if( mtd0->size == 0LL )
            {
                fprintf(stderr, "ERROR!!! Flash device is configured to use only one file system!\n");
                put_mtd_device(mtd0, mtd0_fd);
                return -1;
            }
        }
    }

    if( !IS_ERR_OR_NULL(mtd0) )
    {
        int ofs;
        int writelen;
        int writing_ubifs;
        int cferom_offset = 0;

        if( *(unsigned short *) image_ptr == JFFS2_MAGIC_BITMASK )
        { /* Downloaded image does not contain CFE ROM boot loader */
            ofs = 0;
        }
        else
        {
            ofs = mtd0->erasesize;
            if (handleCferom(mtd0, image_ptr, wt.wfiFlags, (void*)inMemNvramData_buf) < 0)
            {
                put_mtd_device(mtd0, mtd0_fd);
                return -1;
            }
        }

        /*
         * Scan downloaded image for cferam.000 directory entry and change file extension
         * to cfe.YYY where YYY is the current cfe.XXX + 1. If full secure boot is in play,
         * the file to be updated is secram.000 and not cferam.000
         */
        cferam_ptr = nandUpdateSeqNum(image_ptr, img_size, mtd0->erasesize);

        if( cferam_ptr == NULL )
        {
            fprintf(stderr, "\nERROR: Invalid image. ram.000 not found.\n\n");
            put_mtd_device(mtd0, mtd0_fd);
            return -1;
        }

#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848) || defined(CONFIG_BCM963268)
        if ((wt.wfiFlags & WFI_FLAG_SUPPORTS_BTRM) && (ofs != 0))
        {
            /* These targets support bootrom boots which is currently enabled. the "nvram" */
            /* mtd device may be bigger than just the first nand block. Check that the new */
            /* image plays nicely with the current partition table settings. */
            int mtd1_fd;
            mtd_info_t *mtd1 = get_mtd_device_nm("nvram", &mtd1_fd);
            if( !IS_ERR_OR_NULL(mtd1) )
            {
                uint32_t *pHdr = (uint32_t *)image_ptr;
                pHdr += (mtd1->erasesize / 4); /* pHdr points to the top of the 2nd nand block */
                for( blk_addr = mtd1->erasesize; blk_addr < (int) mtd1->size; blk_addr += mtd1->erasesize )
                {
                    /* If we are inside the for() loop, "nvram" mtd is larger than 1 block */
                    pHdr += (mtd1->erasesize / 4);
                }

                if (*(unsigned short *)pHdr != JFFS2_MAGIC_BITMASK)
                {
                    fprintf(stderr, "New sw image does not match the partition table. Aborting.\n");
                    put_mtd_device(mtd0, mtd0_fd);
                    put_mtd_device(mtd1, mtd1_fd);
                    return -1;
                }
                put_mtd_device(mtd1, mtd1_fd);
            }
        else
            {
                fprintf(stderr, "Failed to get nvram mtd device\n");
                put_mtd_device(mtd0, mtd0_fd);
                return -1;
            }
        }
#endif
#ifndef DESKTOP_LINUX
        { // try to make sure we have enough memory to program the image
            char * temp;
            FILE *fp;
            int status = -ENOMEM;

            if ( (fp = fopen("/proc/sys/vm/drop_caches","w")) != NULL )
            { // clear the caches
                fwrite("3\n", sizeof(char), 2, fp);
                fclose(fp);
            }

            if ( (temp = calloc(mtd0->erasesize, sizeof(char))) != NULL )
            {
                status = (read(mtd0_fd, temp, mtd0->erasesize));
                free(temp);
            }

            if ((temp == NULL) || (status == -ENOMEM))
            {
                fprintf(stderr, "Failed to allocate memory, aborting image write!!!\n");
                put_mtd_device(mtd0, mtd0_fd);
                return -1;
            }
        }
#endif /* DESKTOP_LINUX */

        if( 0 != ofs ) /* Image contains CFE ROM boot loader. */
        {
            if (flashCferom(image_ptr, (wt.wfiFlags & WFI_FLAG_SUPPORTS_BTRM), ofs, &cferom_offset) < 0)
            {
                put_mtd_device(mtd0, mtd0_fd);
                return -1;
            }
            image_ptr += cferom_offset;
        }

        /* Erase blocks containing directory entry for CFERAM before flashing the image. */
        for( blk_addr = 0; blk_addr < rsrvd_for_cferam; blk_addr += mtd0->erasesize )
        {
            nandEraseBlk(mtd0, blk_addr, mtd0_fd);
        }

        /* Flash the image except for CFERAM directory entry, during which all the blocks in the partition (other than CFE) will be erased */
        writing_ubifs = 0;
        for( blk_addr = rsrvd_for_cferam; blk_addr < (int) mtd0->size; blk_addr += mtd0->erasesize )
        {
            printf(".");

            if (nandEraseBlk(mtd0, blk_addr, mtd0_fd) == 0)
            { // block was erased successfully, no need to put clean marker in a block we are writing JFFS2 data to but we do it for backward compatibility
                if ( image_ptr == cferam_ptr )
                { // skip CFERAM directory entry block
                    image_ptr += mtd0->erasesize;
                }
                else
                { /* Write a block of the image to flash. */
                    if( image_ptr < end_ptr )
                    { // if any data left, prepare to write it out
                        writelen = ((image_ptr + mtd0->erasesize) <= end_ptr)
                            ? (int) mtd0->erasesize : (int) (end_ptr - image_ptr);
                    }
                    else
                        writelen = 0;

                    if (writelen) /* Write data with or without JFFS2 clean marker */
                    {
                        if (nandWriteBlk(mtd0, blk_addr, writelen, image_ptr, mtd0_fd, !writing_ubifs) != 0 )
                        {
                            printf("Error writing Block 0x%8.8x, skipping\n", blk_addr);
                        }
                        else
                        { // successful write, increment counter and check for UBIFS split marker if data was written
                            image_ptr += writelen;

                            if (!strncmp(BCM_BCMFS_TAG, image_ptr - 0x100, strlen(BCM_BCMFS_TAG)))
                            {
                                if (!strncmp(BCM_BCMFS_TYPE_UBIFS, image_ptr - 0x100 + strlen(BCM_BCMFS_TAG), strlen(BCM_BCMFS_TYPE_UBIFS)))
                                { // check for UBIFS split marker
                                    writing_ubifs = 1;
                                    printf("U");
                                }
                            }
                        }
                    }
                }
            }
            fflush(stdout);
        }

        /* Flash the block containing directory entry for CFERAM. */
        if (flashCferam(mtd0, mtd0_fd, rsrvd_for_cferam, cferam_ptr) == 0)
        {
            /* block containing directory entry for CFERAM was written successfully! */
            /* Whole flash image is programmed successfully! */
            sts = 0;
        }

        printf("\n\n");

        if( sts )
        {
            /*
             * Even though we try to recover here, this is really bad because
             * we have stopped the other CPU and we cannot restart it.  So we
             * really should try hard to make sure flash writes will never fail.
             */
            printf("nandWriteBlk: write failed at blk=%d\n", blk_addr);
            sts = (blk_addr > (int) mtd0->erasesize) ? blk_addr / mtd0->erasesize : 1;
        }
    }

    if( !IS_ERR_OR_NULL(mtd0) )
    {
        put_mtd_device(mtd0, mtd0_fd);
    }

    return sts;
}


int writeImageToNand( char *string, int size )
{
    NVRAM_DATA * pNvramData;
    int ret = SUCCESS;

    if (NULL == (pNvramData = malloc(sizeof(NVRAM_DATA))))
    {
        fprintf(stderr, "Memory allocation failed");
        return ERROR;
    }

    // Get a copy of the nvram before we do the image write operation
    if (readNvramData(pNvramData))
    {
        unsigned int flags=0;
        ret = getFlashInfo(&flags);
        if (!IS_ERR(ret))
        {
           if (flags & FLASH_INFO_FLAG_NAND)
           { /* NAND flash */
              char *rootfs_part = "image_update";
              int rc;

              rc = bcmNandImageSet(rootfs_part, string, size, pNvramData);
              if (rc != 0)
              {
                 fprintf(stderr, "bcmNandImageSet failed, rc=%d", rc);
                 ret = ERROR;
              }
           }
           else
           { /* NOR flash */
              fprintf(stderr, "This function should not be called when using NOR flash, flags=0x%x", flags);
              ret = ERROR;
           }
        }
    }
    else
    {
        ret = ERROR;
    }

    free(pNvramData);

    return ret;
}


#if defined(CONFIG_BCM963268) || defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848)
int validateWfiTag(void *wtp, int blksize, uint32_t btrmEnabled)
#else
int validateWfiTag(void *wtp, int blksize, uint32_t btrmEnabled __attribute__((unused)))
#endif
{
    unsigned int chip_id = 0;
    WFI_TAG *wt = (WFI_TAG*)wtp;

    bcm_boardIoctl(BOARD_IOCTL_GET_CHIP_ID, 0, NULL, 0, 0, (void *)&chip_id);

#if defined(CHIP_FAMILY_ID_HEX)
    chip_id = CHIP_FAMILY_ID_HEX;
#endif

#ifdef DESKTOP_LINUX
    chip_id = wt->wfiChipId;
#endif

    if( (wt->wfiVersion & WFI_ANY_VERS_MASK) == WFI_ANY_VERS &&
        wt->wfiChipId != chip_id )
    {
        flashutil_error("Chip Id error. Image Chip Id = %x, Board Chip Id = %x",
          wt->wfiChipId, chip_id);
        return -1;
    }
    else if( wt->wfiFlashType == WFI_NOR_FLASH )
    {
        flashutil_error("Image does not support a NAND flash device.");
        return -1;
    }
    else if( (wt->wfiVersion & WFI_ANY_VERS_MASK) == WFI_ANY_VERS &&
        ((wt->wfiFlashType < WFI_NANDTYPE_FLASH_MIN && wt->wfiFlashType > WFI_NANDTYPE_FLASH_MAX) ||
        blksize != WFI_NANDTYPE_TO_BKSIZE(wt->wfiFlashType) ) )
    {
        flashutil_error("NAND flash block size %dKB does not work with an "
          "image built with %dKB block size", blksize, WFI_NANDTYPE_TO_BKSIZE(wt->wfiFlashType));
        return -1;
    }
#if defined(CONFIG_BCM963268) || defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848)
    else if ((( (wt->wfiFlags & WFI_FLAG_SUPPORTS_BTRM)) && (! btrmEnabled)) ||
        ((! (wt->wfiFlags & WFI_FLAG_SUPPORTS_BTRM)) && (  btrmEnabled)))
    {
        flashutil_error("The image type does not match the OTP configuration of the SoC. Aborting.");
        return -1;
    }
#endif
    return 0;
}

int flashCferam(mtd_info_t *mtd, int mtd_fd, int rsrvd_for_cferam, char *cferam_ptr)
{
    int blk_addr;
    int ret = -1;

    flashutil_debug("enter, mtd=%p, fd=%d, rsrvd_for_cferam=0x%x",
      mtd, mtd_fd, rsrvd_for_cferam);

    for( blk_addr = 0; blk_addr < rsrvd_for_cferam; blk_addr += mtd->erasesize )
    {
        if( nandIsBadBlk(blk_addr, mtd_fd) == 0 ) 
        {
            /* Write a block of the image to flash. */
            if (nandWriteBlk(mtd, blk_addr, cferam_ptr ? mtd->erasesize : 0, cferam_ptr, mtd_fd, 1) == 0)
            { // rsrvd_for_cferam number of blocks reserved for CFERAM block in order to guarantee at least one block good, CFERAM block with cferam entry written last, write only one block and leave the rest blank, cferom/filesystem must patch this back together
                printf(".");
                fflush(stdout);

                ret = 0;
                break;
            }
        }
    }

    return ret;
}

int handleCferom(mtd_info_t *mtd0, char *image_ptr, unsigned int wfiFlags, void *inMemNvramData)
{
    int nvramXferSize;
    NVRAM_DATA *inMemNvramData_buf = (NVRAM_DATA*)inMemNvramData;

    /* Downloaded image contains CFE ROM boot loader. */
    PNVRAM_DATA pnd = (PNVRAM_DATA) (image_ptr + nvramSector(mtd0) * ((unsigned int)mtd0->writesize) + nvramDataOffset(mtd0));

    flashutil_debug("enter, mtd=%p, image=%p, wfiFlags=%d, nvram=%p",
      mtd0, image_ptr, wfiFlags, inMemNvramData_buf);

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148)
    /* check if it is zero padded for backward compatiblity */
    if( (wfiFlags&WFI_FLAG_HAS_PMC) == 0 )
    {
        unsigned int *pImg  = (unsigned int*)image_ptr;
        char * pBuf = image_ptr;
        int block_start, block_end, remain, block;
        mtd_info_t *mtd1;
        int mtd1_fd;

        if( *pImg == 0 && *(pImg+1) == 0 && *(pImg+2) == 0 && *(pImg+3) == 0 )
        {
            /* the first 64KB are for PMC in 631x8, need to preserve that for cfe/linux image update if it is not for PMC image update. */
            block_start = 0;
            block_end = IMAGE_OFFSET/mtd0->erasesize;
            remain = IMAGE_OFFSET%mtd0->erasesize;

            mtd1 = get_mtd_device_nm("nvram", &mtd1_fd);
            if( !IS_ERR_OR_NULL(mtd1) )
            {
                for( block = block_start; block < block_end; block++ )
                {
                    nandReadBlk(mtd1, block*mtd1->erasesize, mtd1->erasesize, pBuf, mtd1_fd);
                    pBuf += mtd1->erasesize;
                }

                if( remain )
                {
                    block = block_end;
                    nandReadBlk(mtd1, block*mtd1->erasesize, remain, pBuf, mtd1_fd);
                }

                put_mtd_device(mtd1, mtd1_fd);
            }
            else
            {
                fprintf(stderr, "Failed to get nvram mtd device\n");
                return -1;
            }
        }
        else
        {
            fprintf(stderr, "Invalid NAND image.No PMC image or padding\n");
            return -1;
        }
    }
#else
    flashutil_print("wfiFlags=%d", wfiFlags);
#endif

    nvramXferSize = sizeof(NVRAM_DATA);
#if defined(CONFIG_BCM963268)
    if ((wfiFlags & WFI_FLAG_SUPPORTS_BTRM) && otp_is_boot_secure())
    {
       /* Upgrading a secure-boot 63268 SoC. Nvram is 3k. do not preserve the old */
       /* security credentials kept in nvram but rather use the new credentials   */
       /* embedded within the new image (ie the last 2k of the 3k nvram) */
       nvramXferSize = 1024;
    }
#endif

    /* Copy NVRAM data to block to be flashed so it is preserved. */
    memcpy((unsigned char *) pnd, inMemNvramData_buf, nvramXferSize);

    /* Recalculate the nvramData CRC. */
    pnd->ulCheckSum = 0;
    pnd->ulCheckSum = crc_getCrc32((unsigned char *)pnd, sizeof(NVRAM_DATA), CRC32_INIT_VALUE);
    return 0;
}

#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848) || defined(CONFIG_BCM963268)
int flashCferom(char *image_ptr, int btrmFlag, int ofs, int *cferom_offset)
#else
int flashCferom(char *image_ptr, int btrmFlag __attribute__((unused)), int ofs, int *cferom_offset)
#endif
{
    /* Prepare to flash the CFE ROM boot loader. */
    int mtd1_fd;
    mtd_info_t *mtd1 = get_mtd_device_nm("nvram", &mtd1_fd);

    flashutil_debug("enter, image=%p, btrmFlag=%d, ofs=%d",
      image_ptr, btrmFlag, ofs);

    *cferom_offset = 0;

    if( !IS_ERR_OR_NULL(mtd1) )
    {
        int iterations = 10;
        int status = -1;

        while(iterations--)
        {
            if (nandEraseBlk(mtd1, 0, mtd1_fd) == 0)
            {
                if ((status = nandWriteBlk(mtd1, 0, mtd1->erasesize, image_ptr, mtd1_fd, 1)) == 0)
                    break;

                printf("ERROR WRITING CFE ROM BLOCK!!!\n");
            }
        }

        if (status)
        {
            printf("Failed to write CFEROM, quitting\n");
            put_mtd_device(mtd1, mtd1_fd);
            return -1;
        }

        image_ptr += ofs;
        *cferom_offset += ofs;

#if defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848) || defined(CONFIG_BCM963268)
        if (btrmFlag)
        {
            int blk_addr;

            /* We have already checked that the new sw image matches the partition table. Therefore */
            /* burn the rest of the "nvram" mtd (if any) */
            for( blk_addr = mtd1->erasesize; blk_addr < (int) mtd1->size; blk_addr += mtd1->erasesize )
            {
                if (nandEraseBlk(mtd1, blk_addr, mtd1_fd) == 0)
                {
                    if (nandWriteBlk(mtd1, blk_addr, mtd1->erasesize, image_ptr, mtd1_fd, 1) != 0)
                        printf("ERROR WRITING BLOCK!!! at address 0x%x wihin NAND partition nvram\n", blk_addr);

                    image_ptr += ofs;
                    *cferom_offset += ofs;
                }
            }
        }
#endif

        put_mtd_device(mtd1, mtd1_fd);
    }
    else
    {
        flashutil_error("Failed to get nvram mtd device!!!");
        return -1;
    }

    return 0;
}

int getImageVersion(uint8_t *imagePtr __attribute__((unused)), int imageSize __attribute__((unused)), 
  char *image_name, int image_name_len __attribute__((unused)))
{
#ifdef DESKTOP_LINUX
    strcpy(image_name, "DESKTOPLINUX");
    return 0;

#else
    mtd_info_t *mtd;
    int mtd0_fd;
    int ret = -1;

    mtd = get_mtd_device_nm("image", &mtd0_fd);
    if (!IS_ERR_OR_NULL(mtd))
    {
        ret = get_image_version(imagePtr, imageSize, mtd->erasesize, image_name, image_name_len);
    }
    return ret;
#endif /* DESKTOP_LINUX */
}
