/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosSocketLinuxKernel.h
*
*  @brief   LinuxKernel specific definitions for the BOS Socket module.
*
****************************************************************************/

#if !defined( BOSFILELINUXKERNEL_H )
#define BOSFILELINUXKERNEL_H         /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSLINUXKERNEL_H )
#  include "bosLinuxKernel.h"
#endif

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>   

/**
 * @addtogroup bosSocket
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */



/**
 * This value is considered to be an "error" by LinuxKernel.
 */

#define  BOS_FILE_INVALID   (-1)
#define  BOS_FILE_NAME_STR_LEN   64

/* Map file open flags */
#define BOS_FILE_FLAG_ACCMODE   O_ACCMODE     
#define BOS_FILE_FLAG_RDONLY    O_RDONLY      
#define BOS_FILE_FLAG_WRONLY    O_WRONLY      
#define BOS_FILE_FLAG_RDWR      O_RDWR        
#define BOS_FILE_FLAG_CREAT     O_CREAT       /* not fcntl */
#define BOS_FILE_FLAG_EXCL      O_EXCL        /* not fcntl */
#define BOS_FILE_FLAG_NOCTTY    O_NOCTTY      /* not fcntl */
#define BOS_FILE_FLAG_TRUNC     O_TRUNC       /* not fcntl */
#define BOS_FILE_FLAG_APPEND    O_APPEND      
#define BOS_FILE_FLAG_NONBLOCK  O_NONBLOCK    
#define BOS_FILE_FLAG_DSYNC     O_DSYNC       /* used to be O_SYNC, see below */
#define BOS_FILE_FLAG_FASYNC    FASYNC        /* fcntl, for BSD compatibility */
#define BOS_FILE_FLAG_DIRECT    O_DIRECT      /* direct disk access hint */
#define BOS_FILE_FLAG_LARGEFILE O_LARGEFILE   
#define BOS_FILE_FLAG_DIRECTORY O_DIRECTORY   /* must be a directory */
#define BOS_FILE_FLAG_NOFOLLOW  O_NOFOLLOW    /* don't follow links */
#define BOS_FILE_FLAG_NOATIME   O_NOATIME     
#define BOS_FILE_FLAG_CLOEXEC   O_CLOEXEC     /* set close_on_exec */

/* Map SEEK origin flags */
#define BOS_FILE_SEEK_SET       SEEK_SET      /* seek relative to beginning of file */
#define BOS_FILE_SEEK_CUR       SEEK_CUR      /* seek relative to current file position */
#define BOS_FILE_SEEK_END       SEEK_END      /* seek relative to end of file */
                                                    

typedef struct file * BOS_FILE_HDL;    /* LINUXKERNEL file type */

typedef struct
{
   char              fileName[ BOS_FILE_NAME_STR_LEN + 1 ];
   BOS_FILE_HDL      fileHdl;
}BOS_FILE;

/**
 * LinuxKernel version of a BOS_FILE.
 */

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* BOSFILELINUXKERNEL_H */

