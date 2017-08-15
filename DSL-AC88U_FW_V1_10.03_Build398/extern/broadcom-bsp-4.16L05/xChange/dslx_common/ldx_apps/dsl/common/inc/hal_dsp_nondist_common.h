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
*    Filename: hal_dsp_nondist_common.h
*
****************************************************************************
*    Description:
*
*     Interface for the hardware abstraction layer (HAL) for the thread
*     processor running the bottom half of hausware (aka "DSP" thread processor).
*     This header file is included by hal_dsp_nondist.h
*
****************************************************************************/

#ifndef HAL_DSP_NONDIST_COMMON_H
#define HAL_DSP_NONDIST_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/* MIPS chips only, copied from bcmChipCommon.h */
#ifndef BCMCHIPTYPE
#define BCMCHIPTYPE
typedef enum bcmChipType
{
   BCM96358  = 0x6358,
   BCM96368  = 0x6368,
   BCM96362  = 0x6362,
   BCM96328  = 0x6328,
   BCM96816  = 0x6816,
   BCM96838  = 0x6838,
   BCM96828  = 0x6828,
   BCM963381 = 0x63381,
   BCM96848  = 0x6848,
   BCM963268 = 0x63268,
   BCM_UNKNOWN
} BCMCHIP;
#endif

/* moved from hal_dsp_nondist.h */
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
void halInitIsr( volatile void *halShimConfig, XDRV_UINT32 halShimEntryCount, XDRV_UINT32 dmaBufferSize , BCMCHIP bcmChipId, XDRV_UINT32 apmModeEnabled );

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
