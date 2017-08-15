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
*  @file    bosLinuxUser.h
*
*  @brief   Common LinuxUser header file.
*
****************************************************************************/

#if !defined( BOSLINUXUSER_H )
#define BOSLINUXUSER_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>
#include <errno.h>

#define BOS_MSEC_PER_TICK    10     //Userspace tick rate is always 100 = 10ms/tick

/***************************************************************************/
/**
 * Conversion routines between seconds and ticks
 */

/* msec to ticks macro
 * NOTE: this macro employs a "round-up" approach.
 *       (i.e, msec are rounded-up to the nearest tick interval before converting) */
#define  BOS_MS_TO_TICKS( mSec )   (  ( (mSec) + BOS_MSEC_PER_TICK - 1 ) \
                                    / BOS_MSEC_PER_TICK )

/* ticks to msec macro */
#define  BOS_TICKS_TO_MS( ticks )  ( (ticks) * BOS_MSEC_PER_TICK )


#endif /* BOSLINUXUSER_H */

