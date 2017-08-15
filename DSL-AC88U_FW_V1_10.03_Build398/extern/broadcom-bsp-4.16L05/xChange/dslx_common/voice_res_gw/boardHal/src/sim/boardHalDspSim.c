/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: boardHalDspSim.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent DSP interface.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <codec.h>
#include <boardHalDsp.h>
#include "boardHalSim.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalDspGetCfg
**
** PURPOSE:    Retrieves DSP configuration and capabilities.
**
** PARAMETERS: dspCfg   (out) DSP config info.
**
** RETURNS:    none.
**
** NOTE:
**
*****************************************************************************
*/
void boardHalDspGetCfg( BOARD_HAL_DSP_CFG *dspCfg )
{
   VRG_UINT32 vocoderCapabilities;

   vocoderCapabilities = CODEC_NULL;

#if 1
   vocoderCapabilities |= ( CODEC_PCMU | CODEC_PCMA );
#endif

#if 1
   /* Only support G729A and G729E. Not informing support of Annex B*/
   #if 0
      vocoderCapabilities |= ( CODEC_G729A );
   #endif

   #if 1
      vocoderCapabilities |= ( CODEC_G729 | CODEC_G729E );
   #endif
#endif

#if 0
   /* Will not inform support of G.723.1A at this time */
   vocoderCapabilities |= ( CODEC_G7231_53 | CODEC_G7231_63 |
                            CODEC_G723x_VAR );
#endif

#if 1
   vocoderCapabilities |= ( CODEC_G728 );
#endif

#if 0
   vocoderCapabilities |= ( CODEC_G726_16 |
                            CODEC_G726_24 |
                            CODEC_G726_32 |
                            CODEC_G726_40 );
#endif

#if 0
   vocoderCapabilities |= ( CODEC_BV16 );
#endif

#if 0
   vocoderCapabilities |= ( CODEC_BV32 );
#endif

   vocoderCapabilities |= ( CODEC_NTE );

   dspCfg->codecCapability = vocoderCapabilities;

   /* Set maximum number of endpoints and connections available */
   dspCfg->maxEndPts    = BOARD_HAL_SIM_NUM_ENDPTS;
   dspCfg->maxFullCnxs  = BOARD_HAL_SIM_NUM_ENDPTS;
   dspCfg->maxLiteCnxs  = BOARD_HAL_SIM_NUM_ENDPTS;
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspDisplayCfg
**
** PURPOSE:    Displays DSP configuration and capabilities.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
**
*****************************************************************************
*/
void boardHalDspDisplayCfg( void )
{
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskRegisterTimerCallback
**
** PURPOSE:    Registers callback with the HAL that should be invoked
**             periodically in order to invoke the kernel of the voice
**             processing stack. The timer implementation is HAL specific
**             since it may have dependencies on APM/TDM drivers, or require
**             communication with the DSP HAL.
**
** PARAMETERS: Callback to invoke periodically.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspRegisterTimerCallback( BOARD_HAL_DSP_TIMER_FUNC timerFunc )
{
}
