/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*  Filename: stdlib.h
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    crtStdlib.h.h
*
*  @brief   Contains the stdlib interface.
*
****************************************************************************/
/**
*  @defgroup   crtStdlib.h
*
*  @brief      Wrapper for C run-time stdlib functions
*
****************************************************************************/

#if !defined( CRT_STDLIB_H )
#define CRT_STDLIB_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */
#if 0
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)

#else
#include <asm/system.h>     /* for cli */
#endif
#endif

#include <kernelMainWrapper.h>

#define STDLIB_ABORT_DISABLES_INTERRUPTS  0

/* ---- Constants and Types ---------------------------------------------- */

#if STDLIB_ABORT_DISABLES_INTERRUPTS

#define abort()	\
{                                         \
   cli();   /* Disable interrupts */      \
   while(1){ ; }                          \
}

#else /* STDLIB_ABORT_DISABLES_INTERRUPTS */

/* Don't disable interrupts, just crash the process, which would send SIGSEGV to the application */
#define abort()	                        \
{                                         \
   char* p=NULL;                          \
   printk("%s %d Aborting and purposefully crashing the process !!! \n\n", __FUNCTION__, __LINE__);   \
   *p = 0;                                \
}


#endif /* STDLIB_ABORT_DISABLES_INTERRUPTS */

/* conversion of string to int */
int atoi(const char *nptr);

#endif /* CRT_STDLIB_H */
