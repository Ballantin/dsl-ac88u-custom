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
*  @file    bosLinuxKernel.h
*
*  @brief   Common LinuxKernel header file.
*
****************************************************************************/

#if !defined( BOSLINUXKERNEL_H )
#define BOSLINUXKERNEL_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <bosTypes.h>

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>

/***************************************************************************/
/**
 * Conversion routines between seconds and ticks
 */

/* msec to ticks macro
 * NOTE: this macro employs a "round-up" approach.
 *       (i.e, msec are rounded-up to the nearest tick interval before converting) */
#define  BOS_MS_TO_TICKS   msecs_to_jiffies

/* ticks to msec macro */
#define  BOS_TICKS_TO_MS   jiffies_to_msecs


#endif /* BOSLINUXKERNEL_H */

