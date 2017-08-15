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
*  @file    crtStdio.h.h
*
*  @brief   Contains the stdio interface.
*
****************************************************************************/
/**
*  @defgroup   crtStdio.h
*
*  @brief      Wrapper for C run-time stdio functions
*
****************************************************************************/

#if !defined( CRT_STDIO_H )
#define CRT_STDIO_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include "string.h"
#include <kernelMainWrapper.h>
#include <linux/kernel.h>      /* for printk */
#include <linux/stddef.h>      /* for NULL */

/* ---- Constants and Types ---------------------------------------------- */

typedef int FILE;

#define printf           printk

#endif /* CRT_STDIO_H */
