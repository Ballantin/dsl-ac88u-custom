/*
<:copyright-BRCM:2012:DUAL/GPL:standard 

   Copyright (c) 2012 Broadcom 
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
*/                       

/***********************************************************************/
/*                                                                     */
/*   MODULE:  bcm_hwdefs.h                                             */
/*   PURPOSE: Define all base device addresses and HW specific macros. */
/*                                                                     */
/***********************************************************************/
#ifndef _BCM_HWDEFS_H
#define _BCM_HWDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef INC_BTRM_BOOT
#define INC_BTRM_BOOT         0
#endif

#define	DYING_GASP_API

// ARCADYAN
#include "arc_hwdefs.h"
/// ARCADYAN

/*****************************************************************************/
/*                    Physical Memory Map                                    */
/*****************************************************************************/

#define PHYS_DRAM_BASE          0x00000000      /* Dynamic RAM Base */
#if defined(CONFIG_BRCM_IKOS)
#define PHYS_FLASH_BASE         0x18000000      /* Flash Memory     */
#else
#define PHYS_FLASH_BASE         0x1FC00000      /* Flash Memory     */
#endif

/*****************************************************************************/
/* Note that the addresses above are physical addresses and that programs    */
/* have to use converted addresses defined below:                            */
/*****************************************************************************/
#if defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
/* cacheablity is set in the mmu entry. cfe rom set ddr to none-cacheable, cfe ram 
   set to cacheable 
*/
#define DRAM_BASE           PHYS_DRAM_BASE   
#define DRAM_BASE_NOCACHE   PHYS_DRAM_BASE   
#else
#define DRAM_BASE           (0x80000000 | PHYS_DRAM_BASE)   /* cached DRAM */
#define DRAM_BASE_NOCACHE   (0xA0000000 | PHYS_DRAM_BASE)   /* uncached DRAM */
#endif
/* Binary images are always built for a standard MIPS boot address */
#define IMAGE_BASE          (0xA0000000 | PHYS_FLASH_BASE)

/* Some chips don't support alternative boot vector */
#if defined(CONFIG_BRCM_IKOS)
#define BOOT_OFFSET         0
#if defined(CONFIG_BCM63138_SIM) || defined(CONFIG_BCM63148_SIM)
#define FLASH_BASE          0xFFFD0000
#elif defined(_BCM963381_) || defined(CONFIG_BCM963381)
#if (INC_BTRM_BOOT==1)
#define FLASH_BASE          0xB8100000
#else
#define FLASH_BASE          0xB8000000
#endif
#else
#define FLASH_BASE          (0xA0000000 | PHYS_FLASH_BASE)  /* uncached Flash  */
#endif
#else //CONFIG_BRCM_IKOS
#if defined(_BCM96838_) || defined(CONFIG_BCM96838)
#define FLASH_BASE	    0xBFC00000
#elif defined(_BCM96328_) || defined(CONFIG_BCM96328) || defined(_BCM96362_) || defined(CONFIG_BCM96362) || defined(_BCM963268_) || defined(CONFIG_BCM963268) || defined(_BCM96828_) || defined(CONFIG_BCM96828) || defined(_BCM96318_) || defined(CONFIG_BCM96318) || defined(_BCM960333_) || defined(CONFIG_BCM960333) ||  defined(_BCM96848_) || defined(CONFIG_BCM96848)
#define FLASH_BASE          0xB8000000
#elif defined(_BCM963381_) || defined(CONFIG_BCM963381)
#if (INC_BTRM_BOOT==1)
#define FLASH_BASE          0xB8100000
#else
#define FLASH_BASE          0xB8000000
#endif
#elif defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
#ifdef _CFE_
/* for hardware mapped flash address 0xffxx-xxxx, we only have 3 or 4MB before address wrap.
This is good for CFE ROM. However for CFE RAM, this is not enough when flash linux image.
Actually the base address does not really matter as long as the flash driver and cfe code
use the same value because it eventually convert to block number. So use 0 for cfe ram */
#if defined(CFG_RAMAPP)
#define FLASH_BASE          0x00000000
#else
#if (INC_SPI_FLASH_DRIVER == 1)
#define FLASH_BASE          0xFFD00000
#else
#define FLASH_BASE          0xFFE00000
#endif//(INC_SPI_FLASH_DRIVER == 1)
#endif//(CFG_RAMAPP)
#else 
/* linux use block number for SPI flash and most of the NAND code. Only use of the direct
mappaed address is for NAND in the kerSysEarlyFlashInit in board driver bcm63xx_flash.c */
#define FLASH_BASE          0x00000000
#define FLASH_BASE_NAND     0xFFE00000
#endif//_CFE
#else
#define FLASH_BASE          (0xA0000000 | (MPI->cs[0].base & 0xFFFFFF00))
#endif
#define BOOT_OFFSET         (FLASH_BASE - IMAGE_BASE)
#endif

/*****************************************************************************/
/*  ARM Kernel boot requires MACH_ID and ATAG_LOC                            */
/*****************************************************************************/
#if defined(_BCM963138_) || defined(CONFIG_BCM963138)
#define ARM_ATAG_LOC        0x100
#define BCM63XX_MACH_ID    0x270a
#endif

#if defined(_BCM963148_) || defined(CONFIG_BCM963148)
#define ARM_ATAG_LOC        0x100
#define BCM63XX_MACH_ID    0x2709
#endif

/*****************************************************************************/
/*  Select the PLL value to get the desired CPU clock frequency.             */
/*****************************************************************************/
#define FPERIPH            50000000

/*****************************************************************************/
/* Board memory type offset                                                  */
/*****************************************************************************/
#define ONEK                            1024
 
#if defined(_BCM963268_) || defined(CONFIG_BCM963268)
#define FLASH_LENGTH_SECURE_BOOT_ROM    (64*2*ONEK)
#elif defined(_BCM963381_) || defined(CONFIG_BCM963381)
#define FLASH_LENGTH_SECURE_BOOT_ROM    (64*3*ONEK)
#elif defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
#define FLASH_LENGTH_SECURE_BOOT_ROM    (64*6*ONEK)
#else
#define FLASH_LENGTH_SECURE_BOOT_ROM    (64*ONEK)
#endif

#if (INC_BTRM_BOOT==1)	/* SPI NOR secure boot build */
#define FLASH_LENGTH_BOOT_ROM           FLASH_LENGTH_SECURE_BOOT_ROM
#else 			/* SPI NOR legacy build */
#if  defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
#define FLASH_LENGTH_BOOT_ROM           (64*3*ONEK)
#else
#define FLASH_LENGTH_BOOT_ROM           (64*ONEK)
#endif
#endif

/*****************************************************************************/
/*       NVRAM Offset and definition                                         */
/*****************************************************************************/
#if defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
/* 63138 image always start from 64KB offset in the flash, the first 64KB is used for PMC */
#define IMAGE_OFFSET                    0x10000                    
#define NVRAM_SECTOR                    (IMAGE_OFFSET/((unsigned int)flash_get_sector_size(0)))
#define NVRAM_DATA_OFFSET               (0x0580+(IMAGE_OFFSET-(IMAGE_OFFSET/((unsigned int)flash_get_sector_size(0)))*((unsigned int)flash_get_sector_size(0))))
#else
#define IMAGE_OFFSET                    0                    
#define NVRAM_SECTOR                    0
#define NVRAM_DATA_OFFSET               0x0580
#endif

#define NVRAM_DATA_ID                   0x0f1e2d3c  // This is only for backwards compatability

#if (INC_BTRM_BOOT==1) && (defined(_BCM963268_) || defined(CONFIG_BCM963268))
#define NVRAM_LENGTH                    (3*ONEK) // 3k nvram
#define NVRAM_VERSION_NUMBER    	7
#else
#define NVRAM_LENGTH                    ONEK     // 1k nvram
#define NVRAM_VERSION_NUMBER    	6
#endif


#define NVRAM_FULL_LEN_VERSION_NUMBER   5 /* version in which the checksum was
                                             placed at the end of the NVRAM
                                             structure */

#define NVRAM_BOOTLINE_LEN              256
#define NVRAM_BOARD_ID_STRING_LEN       16
#define NVRAM_MAC_ADDRESS_LEN           6

#define NVRAM_GPON_SERIAL_NUMBER_LEN    13
#define NVRAM_GPON_PASSWORD_LEN         11


#define  WLAN_FEATURE_DHD_NIC_ENABLE     0x01
#define  WLAN_FEATURE_DHD_MFG_ENABLE     0x02
#define  WLAN_FEATURE_LAST_FEATURE       0x80

#define  WLAN_MFG_PARTITION_ISNAND       0x01
#define  WLAN_MFG_PARTITION_MFGSET       0x02
#define  WLAN_MFG_PARTITION_HASSIZE      0x04
#define  WLAN_MFG_PARTITION_NAME         "misc3"
#define  WLAN_MFG_PARTITION_INDEX         2

#define NVRAM_WLAN_PARAMS_LEN      256
#define NVRAM_WPS_DEVICE_PIN_LEN   8

#define NVRAM_BOOTLDR_SIG_LEN           256
#define NVRAM_BOOTLDR_SIG_OFFSET        1024

#define NVRAM_CV_KEY_LEN                514
#define NVRAM_MFG_CV_KEY_OFFSET         1280
#define NVRAM_OP_CV_KEY_OFFSET          1794

#define NVRAM_ENC_KEY_LEN               32

#define NVRAM_BOOTLDR_ENC_KEY_OFFSET    2308
#define NVRAM_IMAGE_ENC_KEY_OFFSET      2340

#define NVRAM_ENC_IV_LEN                32

#define NVRAM_BOOTLDR_ENC_IV_OFFSET     2372
#define NVRAM_IMAGE_ENC_IV_OFFSET       2404

#define NVRAM_SECURITY_CREDENTIALS_LEN  2044

// ARCADYAN
#if 0
/* Add arcadyan manufacture data jingwei_liang 2014/12 */
#define NVRAM_ARCADYAN_SN_LEN      	0X20
#define DEFAULT_ARCADYAN_SN        "ARCADYAN2015000001"
#define NVRAM_ARCADYAN_MODEL_LEN	0X20
#define NVRAM_ARCADYAN_HW_VER_LEN	0x04
#define NVRAM_ARCADYAN_CLONE_FLAG	0x01
#define DEFAULT_ARCADYAN_CLONE_FLAG	0x00
#define NVRAM_ARCADYAN_MPTEST_LEN		0x01
#define DEFAULT_ARCADYAN_MPTEST		0x00

#ifdef CONFIG_WEBJSMIN_PROJ_UI_PRODUCT_NAME
#define DEFAULT_ARCADYAN_MODEL		CONFIG_WEBJSMIN_PROJ_UI_PRODUCT_NAME
#else
#define DEFAULT_ARCADYAN_MODEL		"DSL-AC88U"
#endif	// CONFIG_ARCADYAN_MODEL_NAME //

#ifdef CONFIG_ARCADYAN_HW_VER
#define DEFAULT_ARCADYAN_HW_VER		CONFIG_ARCADYAN_HW_VER
#else
#define DEFAULT_ARCADYAN_HW_VER		"R0A"
#endif	// CONFIG_ARCADYAN_HW_VER //

/* Arcadyan Simon Chu 2015/07/07 add DECT Rfpi in NVRAM  */
#define NVRAM_ARCADYAN_DECT_RFPI_LEN      5
#define DEFAULT_ARCADYAN_DECT_RFPI        "0016814500"
/* Arcadyan Simon Chu 2015/09/01 add DECT RXTUN in NVRAM  */
#define NVRAM_ARCADYAN_DECT_RXTUN_LEN     2
#define NVRAM_BRCM_DEFAULT_LEN			272
#define NVRAM_ARC_BRCM_UNUSED		(NVRAM_BRCM_DEFAULT_LEN - NVRAM_ARCADYAN_SN_LEN - \
									NVRAM_ARCADYAN_MODEL_LEN - NVRAM_ARCADYAN_HW_VER_LEN - \
									NVRAM_ARCADYAN_CLONE_FLAG - NVRAM_ARCADYAN_DECT_RFPI_LEN - \
									NVRAM_ARCADYAN_DECT_RXTUN_LEN)
#endif
/// ARCADYAN

#define THREAD_NUM_ADDRESS_OFFSET       (NVRAM_DATA_OFFSET + 4 + NVRAM_BOOTLINE_LEN + NVRAM_BOARD_ID_STRING_LEN)
#define THREAD_NUM_ADDRESS              (0x80000000 + THREAD_NUM_ADDRESS_OFFSET)

#define DEFAULT_BOOTLINE    "e=192.168.1.1:ffffff00 h=192.168.1.100 g= r=f f=vmlinux i=bcm963xx_fs_kernel d=1 p=0 c= a= "
#define DEFAULT_BOARD_IP    "192.168.1.1"
#define DEFAULT_MAC_NUM     10
#define DEFAULT_BOARD_MAC   "00:10:18:00:00:00"
#define DEFAULT_TP_NUM      0
#if defined(_BCM963268_) || defined(CONFIG_BCM963268) || defined(_BCM963138_) || defined(CONFIG_BCM963138) || defined(_BCM963148_) || defined(CONFIG_BCM963148)
#define DEFAULT_PSI_SIZE    128
#else
#define DEFAULT_PSI_SIZE    24
#endif
#define DEFAULT_GPON_SN     "BRCM12345678"
#define DEFAULT_GPON_PW     "          "
#define DEFAULT_LOG_SIZE    0
#define DEFAULT_FLASHBLK_SIZE  64
#define MAX_FLASHBLK_SIZE      128
#define DEFAULT_AUXFS_PERCENT 0
#define MAX_AUXFS_PERCENT   80
#define DEFAUT_BACKUP_PSI  0

#define DEFAULT_WPS_DEVICE_PIN     "12345670"
#define DEFAULT_WLAN_DEVICE_FEATURE  0

#define DEFAULT_VOICE_BOARD_ID     "NONE"

#if  defined(_BCM96838_) || defined(_BCM963138_) || defined(_BCM963148_) || defined(_BCM96848_)
/* any better value for these two? */
#define MAX_TM_SIZE	150
#define MAX_MC_SIZE	8
#define DEFAULT_NVRAM_RDP_TMSIZE	20
#define DEFAULT_NVRAM_RDP_MCSIZE	4
#endif

#define NVRAM_MAC_COUNT_MAX         32
#define NVRAM_MAX_PSI_SIZE          128
#define NVRAM_MAX_SYSLOG_SIZE       256

#define NP_BOOT             0
#define NP_ROOTFS_1         1
#define NP_ROOTFS_2         2
#define NP_DATA             3
#define NP_BBT              4
#define NP_TOTAL            5

// ARCADYAN
#ifndef NAND_DATA_SIZE_KB
#define NAND_DATA_SIZE_KB       4096
#endif
#if 0
#ifdef BRCML05
#define NAND_DATA_SIZE_KB       4096
#else
//#define NAND_DATA_SIZE_KB       3072 //Split 1M for arc glbcfg   jingwei 2015/03
#define NAND_DATA_SIZE_KB			(24*ONEK)
#define NAND_PRI_CFG_SIZE_KB		(1024) 
#define NAND_RESERVED_SIZE_KB			(1664)
#define NAND_BOOT_FLAG_SIZE_KB		(256) // 0.25MB = 256KB
#endif
#endif
/// ARCADYAN

#define NAND_BBT_THRESHOLD_KB   (512 * 1024)
#define NAND_BBT_SMALL_SIZE_KB  1024
#define NAND_BBT_BIG_SIZE_KB    4096

#define NAND_CFE_RAM_NAME           "cferam.000"
#define NAND_CFE_RAM_SECBT_NAME     "secram.000"
#define NAND_RFS_OFS_NAME           "NAND_RFS_OFS"
#define NAND_COMMAND_NAME           "NANDCMD"
#define NAND_BOOT_STATE_FILE_NAME   "boot_state_x"
#define NAND_SEQ_MAGIC              0x53510000


#define BCM_MAX_EXTRA_PARTITIONS 4

#define NAND_FULL_PARTITION_SEARCH  0

#if (NAND_FULL_PARTITION_SEARCH == 1)
#define MAX_NOT_JFFS2_VALUE         0 /* infinite */
#else
#define MAX_NOT_JFFS2_VALUE         10
#endif

#ifndef _LANGUAGE_ASSEMBLY

struct allocs_rdp {
    unsigned char tmsize;
    unsigned char mcsize;
    unsigned char reserved[2];
};

struct allocs_dhd {
    unsigned char dhd_size[3];
    unsigned char reserved;
};


typedef struct
{
    unsigned int ulVersion;
    char szBootline[NVRAM_BOOTLINE_LEN];
    char szBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned int ulMainTpNum;
    unsigned int ulPsiSize;
    unsigned int ulNumMacAddrs;
    unsigned char ucaBaseMacAddr[NVRAM_MAC_ADDRESS_LEN];
    char pad;
    char backupPsi;  /**< if 0x01, allocate space for a backup PSI */
    unsigned int ulCheckSumV4;
    char gponSerialNumber[NVRAM_GPON_SERIAL_NUMBER_LEN];
    char gponPassword[NVRAM_GPON_PASSWORD_LEN];
    char wpsDevicePin[NVRAM_WPS_DEVICE_PIN_LEN];
    char wlanParams[NVRAM_WLAN_PARAMS_LEN];
    unsigned int ulSyslogSize; /**< number of KB to allocate for persistent syslog */
    unsigned int ulNandPartOfsKb[NP_TOTAL];
    unsigned int ulNandPartSizeKb[NP_TOTAL];
    char szVoiceBoardId[NVRAM_BOARD_ID_STRING_LEN];
    unsigned int afeId[2];
    unsigned short opticRxPwrReading;   // optical initial rx power reading
    unsigned short opticRxPwrOffset;    // optical rx power offset
    unsigned short opticTxPwrReading;   // optical initial tx power reading
    unsigned char ucUnused2[58];
    unsigned char ucFlashBlkSize;
    unsigned char ucAuxFSPercent;
    unsigned char ucUnused3[2];
    unsigned int ulBoardStuffOption;   // board options. bit0-3 is for DECT    
    union {
        unsigned int reserved;
        struct allocs_rdp alloc_rdp;
    } allocs;
    unsigned int ulMemoryConfig;
    struct partition_info {
	/*
		2MSB represent the 
			00 = MB 
			01 = GB
			10 = reserved
			11 = reserved
		14LSB represent multiple of 2MSB
	*/
	
	unsigned short size;
    } part_info[BCM_MAX_EXTRA_PARTITIONS];
    struct allocs_dhd alloc_dhd;
    /* Add any new non-secure related elements here */

// ARCADYAN
#ifdef BRCML05
    char chUnused[272]; /* Adjust chUnused such that everything above + chUnused[] + ulCheckSum = 1k */
#else
	/* Add any new non-secure related elements here */
	char arcadyanSN[NVRAM_ARCADYAN_SN_LEN];             //Add Arcadyan Serial Number  jingwei_liang 2014/12
	char arcadyanModel[NVRAM_ARCADYAN_MODEL_LEN];
	char arcadyanHwVer[NVRAM_ARCADYAN_HW_VER_LEN];
	/* 20150723.TercesTsai: Add a flag to check auto image clone mechanism */
	unsigned char arcadyanCloneFlag;
	/* Arcadyan Simon Chu 2015/07/07 add DECT Rfpi in NVRAM  */
	unsigned char DECTRfpi[NVRAM_ARCADYAN_DECT_RFPI_LEN];
	/* Arcadyan Simon Chu 2015/09/01 add DECT RXTUN in NVRAM  */
	char DECTRxtun[NVRAM_ARCADYAN_DECT_RXTUN_LEN];
#if 0
	unsigned char arcadyanMPTEST;
	char arcadyanWlanCcode[NVRAM_ARCADYAN_WLAN_CCODE_LEN];
	char arcadyanWlanTcode[NVRAM_ARCADYAN_WLAN_TCODE_LEN];
#endif
	char chUnused[NVRAM_ARC_BRCM_UNUSED];
#endif
/// ARCADYAN

#if (INC_BTRM_BOOT==1) && (defined(_BCM963268_) || defined(CONFIG_BCM963268))
    unsigned int ulCheckSum_1k;
    char signature[NVRAM_BOOTLDR_SIG_LEN];
    char mfgCvKey[NVRAM_CV_KEY_LEN];
    char opCvKey[NVRAM_CV_KEY_LEN];
    char bek[NVRAM_ENC_KEY_LEN];
    char iek[NVRAM_ENC_KEY_LEN];
    char biv[NVRAM_ENC_IV_LEN];
    char iiv[NVRAM_ENC_IV_LEN];
    /* Add any secure boot related elements here */
    char chSecUnused[632];
#endif

    unsigned int ulCheckSum;
} NVRAM_DATA, *PNVRAM_DATA;

// ARCADYAN
#if 0 //def ARCADYAN /* 20151111.TercesTsai: Add a new structure for boot_flag partition */
typedef struct {
	char sig_h[4];
	char manuf;
	char boot_img;
	unsigned char reserved[ONEK-4-1-1-4]; // 1024 - 4 - 1 - 1 - 4 remains
	char sig_t[4];
} ANVRAM_DATA, *pANVRAM_DATA;
#endif
/// ARCADYAN

#endif

/*****************************************************************************/
/*       Misc Offsets                                                        */
/*****************************************************************************/
#define CFE_VERSION_OFFSET           (NVRAM_DATA_OFFSET-0x10)
#define CFE_VERSION_MARK_SIZE        5
#define CFE_VERSION_SIZE             5

/*****************************************************************************/
/*       Scratch Pad Defines                                                 */
/*****************************************************************************/
/* SP - Persistent Scratch Pad format:
       sp header        : 32 bytes
       tokenId-1        : 8 bytes
       tokenId-1 len    : 4 bytes
       tokenId-1 data    
       ....
       tokenId-n        : 8 bytes
       tokenId-n len    : 4 bytes
       tokenId-n data    
*/

#define MAGIC_NUM_LEN       8
#define MAGIC_NUMBER        "gOGoBrCm"
#define TOKEN_NAME_LEN      16
#define SP_VERSION          1
#define CFE_NVRAM_DATA2_LEN 20

#ifndef _LANGUAGE_ASSEMBLY
typedef struct _SP_HEADER
{
    char SPMagicNum[MAGIC_NUM_LEN];             // 8 bytes of magic number
    int SPVersion;                              // version number
    char NvramData2[CFE_NVRAM_DATA2_LEN];       // not related to scratch pad
                                                // additional NVRAM_DATA
} SP_HEADER, *PSP_HEADER;                       // total 32 bytes

typedef struct _TOKEN_DEF
{
    char tokenName[TOKEN_NAME_LEN];
    int tokenLen;
} SP_TOKEN, *PSP_TOKEN;

#endif

/*****************************************************************************/
/*       Boot Loader Parameters                                              */
/*****************************************************************************/

#define BLPARMS_MAGIC               0x424c504d

#define BOOTED_IMAGE_ID_NAME        "boot_image"

#define BOOTED_NEW_IMAGE            1
#define BOOTED_OLD_IMAGE            2
#define BOOTED_ONLY_IMAGE           3
#define BOOTED_PART1_IMAGE          4
#define BOOTED_PART2_IMAGE          5

#define BOOT_SET_NEW_IMAGE          '0'
#define BOOT_SET_OLD_IMAGE          '1'
#define BOOT_SET_NEW_IMAGE_ONCE     '2'
#define BOOT_GET_IMAGE_VERSION      '3'
#define BOOT_GET_BOOTED_IMAGE_ID    '4'
#define BOOT_SET_PART1_IMAGE        '5'
#define BOOT_SET_PART2_IMAGE        '6'
#define BOOT_SET_PART1_IMAGE_ONCE   '7'
#define BOOT_SET_PART2_IMAGE_ONCE   '8'
#define BOOT_GET_BOOT_IMAGE_STATE   '9'

#define BOARD_ID_NAME               "BOARD_ID"
#define VOICE_BOARD_ID_NAME         "VOICE_ID"
#define BOARD_STUFF_NAME            "STUFF_ID"

#define FLASH_PARTDEFAULT_REBOOT    0x00000000
#define FLASH_PARTDEFAULT_NO_REBOOT 0x00000001
#define FLASH_PART1_REBOOT          0x00010000
#define FLASH_PART1_NO_REBOOT       0x00010001
#define FLASH_PART2_REBOOT          0x00020000
#define FLASH_PART2_NO_REBOOT       0x00020001

#define FLASH_IS_NO_REBOOT(X)       ((X) & 0x0000ffff)
#define FLASH_GET_PARTITION(X)      ((unsigned long) (X) >> 16)

/*****************************************************************************/
/*       Split Partition Parameters                                          */
/*****************************************************************************/
#define BCM_BCMFS_TAG		"BcmFs-"
#define BCM_BCMFS_TYPE_UBIFS	"ubifs"
#define BCM_BCMFS_TYPE_JFFS2	"jffs2"

/*****************************************************************************/
/*       Global Shared Parameters                                            */
/*****************************************************************************/

#define BRCM_MAX_CHIP_NAME_LEN	16


#ifdef __cplusplus
}
#endif

#endif /* _BCM_HWDEFS_H */

