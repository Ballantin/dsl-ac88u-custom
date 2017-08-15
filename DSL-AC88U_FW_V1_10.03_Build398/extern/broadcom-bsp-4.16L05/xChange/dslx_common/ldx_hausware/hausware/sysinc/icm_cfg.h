/****************************************************************************
*
*     Copyright (c) 2001-2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  ICM_Cfg.h - Configuration file for Idle CPU Monitor
*
*  PURPOSE:
*
*     This file contains configuration information for the Idle CPU
*     Monitor.
*
*  NOTES:
*
****************************************************************************/

#ifndef ICM_CFG_H
#define ICM_CFG_H

/* ---- Include Files ---------------------------------------------------- */

#ifndef XCFG_RM_H
#include <xcfg_rm.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

/***************************************************************************
 * Calibration Phase Timer Constants
 */
#define ICM_ZSP_CLK_RATE        108000000UL    /* VPM Clock Rate                       */
#define ICM_BLOCK_INTERVAL      30UL          /*  time for estimate idle cpu in ms  */
#define ICM_BLOCK_SIZE          (ICM_BLOCK_INTERVAL * 8)
/* if the idle cpu only has 8% of the whole cpu, turn on low complexity flag */                                              
#define ICM_LOWCOMPLEXITY_THRESHOLD     (((ICM_ZSP_CLK_RATE/1000) *  ICM_BLOCK_INTERVAL) * 8/100)
/* if the idle cpu has 18% of the whole cpu, turn off low complexity flag */      
#define ICM_HIGHCOMPLEXITY_THRESHOLD    (((ICM_ZSP_CLK_RATE/1000) *  ICM_BLOCK_INTERVAL) * 18/100)

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#endif /* ICM_CFG_H */
