/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: hal_dsp_nondist_arm_common.h
*
****************************************************************************
*    Description:
*
*     Interface for the hardware abstraction layer (HAL) for the thread
*     processor running the bottom half of hausware (aka "DSP" thread processor).
*     This header file is included by hal_dsp_nondist.h
*
****************************************************************************/

#ifndef HAL_DSP_NONDIST_ARM_COMMON_H
#define HAL_DSP_NONDIST_ARM_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <xdrvSpinLock.h>
#include <xchg_dma.h>

/* ARM chips only copied from bcmChipCommon.h */
#ifndef BCMCHIPTYPE
#define BCMCHIPTYPE
typedef enum bcmChipType
{
   BCM963138 = 0x63138,
   BCM963148 = 0x63148,
   BCM_UNKNOWN
} BCMCHIP;
#endif

/*
*****************************************************************************
** FUNCTION:   halInitIsr
**
** PURPOSE:    Initializes isr dependent on apm or pcm mode
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halInitIsr( volatile void *halShimConfig, XDRV_UINT32 halShimEntryCount, BCMCHIP bcmChipId, BCM_IUDMA_ADDR_CTL* dmaCtl, XDRV_UINT32 dmaCtlCount );

/* moved from hal_dsp_nondist.h */
/*
*****************************************************************************
** FUNCTION:   halInitIsr
**
** PURPOSE:    Deinitializes isr dependent on apm or pcm mode
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halDeinitIsr( void );

/* moved from hal_private.h */
/*
*****************************************************************************
** FUNCTION:   halIsrEnable
**
** PURPOSE:    This function calls the implementation-specific isr enable/disable
**             function.
**
** PARAMETERS: none
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halIsrEnable( void );
void halIsrDisable( void );

/* moved from hal_private.h */
/*
*****************************************************************************
** FUNCTION:   halIntCountGet/halBackCountGet
**
** PURPOSE:    Get background and interrupt counters. There can be queried
**             from the primary thread processor, and is used for debugging
**             purposes.
**
** PARAMETERS: None.
**
** RETURNS:    Count value.
**
** NOTE:
*****************************************************************************
*/
unsigned int halIntCountGet( void );


#ifdef __cplusplus
    }
#endif

#endif  /* HAL_DSP_NONDIST_COMMON_H  */
