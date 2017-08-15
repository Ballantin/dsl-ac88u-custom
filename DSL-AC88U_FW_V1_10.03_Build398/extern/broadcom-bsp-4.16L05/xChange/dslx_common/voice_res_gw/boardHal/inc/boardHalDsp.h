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
*    Filename: halInitBoard.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      DSPs present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_DSP_H
#define BOARD_HAL_DSP_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <xdrvDlb.h>
#include <xdrvDebug.h>
#include <codec.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** Callback invoked periodically by the HAL. Used to invoke the voice
** processing kernel.
*/
typedef void (*BOARD_HAL_DSP_TASK_FUNC)( unsigned long );


/*
** DSP configuration info.
*/
typedef struct BOARD_HAL_DSP_CFG
{
   /*
   ** Bit-field of codec values (VRG_CODEC_xxx) that define the capabilities
   ** of the DSP image.
   */
   VRG_UINT8   codecCapabilities[CODEC_MAX_TYPES];

   /*
   ** Bit-field of VBD values (EPVBDMODE_xxx) that define the VBD capabilities
   ** of the DSP image.
   */
   VRG_UINT8   vbdCapability;

   /*
   ** Is idle CPU profiler supported.
   */
   VRG_BOOL    isIcpEnabled;

   /*
   ** Is voice stack distributed.
   */
   VRG_BOOL    isDistributed;

   /*
   ** Boolean indicating whether wideband (16kHz) is used ( 0 means narrowbannd - 8kHz)
   */
   VRG_BOOL    isWidebandEnabled;

   /*
   ** DSP frame sync rate (number of 8kHz samples that are processed each time the hausware runs)
   */
   VRG_UINT16  frameSyncSamples;

   /*
   ** Maximum number of endpoints and connections available
   */
   VRG_UINT16  maxEndPts;
   VRG_UINT16  maxFullCnxs;
   VRG_UINT16  maxLiteCnxs;

   /*
   ** Maximum number of redundancy level(s) for RFC 2198 that can be supported.
   ** This information is needed to be able to properly calculate bandwidth requirements.
   */
   VRG_UINT8   maxRfc2198Level;

   VRG_UINT16  gplcSupport;
   VRG_UINT16  bvcSupport;

   /*
   ** Is software equalizer distributed.
   */
   VRG_BOOL    isEquEnabled;

} BOARD_HAL_DSP_CFG;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#if defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD)
/*
*****************************************************************************
** FUNCTION:   boardHalDspRegisterCallback
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
void boardHalDspRegisterCallback( BOARD_HAL_DSP_TASK_FUNC dspTaskFunc );
#endif /* defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD) */

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
void boardHalDspGetCfg( BOARD_HAL_DSP_CFG *dspCfg );


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
void boardHalDspDisplayCfg( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspIngressProcess
**
** PURPOSE:    This function is invoked by the client as a trigger to transfer
**             digital samples from the HAL to the DSP ingress buffers. This
**             function will only be invoked if it's the host's responsibility
**             to service sample interrupts. If the DSP HAL services the
**             interrupts, then this function should not be called.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspIngressProcess( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspEgressProcess
**
** PURPOSE:    This function is invoked by the client as a trigger to transfer
**             digital samples from the the DSP egress buffers to the HAL. This
**             function will only be invoked if it's the host's responsibility
**             to service sample interrupts. If the DSP HAL services the
**             interrupts, then this function should not be called.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspEgressProcess( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspGetDlbDriver
**
** PURPOSE:    Get the dynamic load balancing (DLB) driver.
**
** PARAMETERS: None.
**
** RETURNS:    Pointer to the DLB driver interface
**
*****************************************************************************
*/
XDRV_DLB* boardHalDspGetDlbDriver( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspGetDebugDriver
**
** PURPOSE:    Get the dsp debug driver.
**
** PARAMETERS: None
**
** RETURNS:    Pointer to the Debug driver interface
**
*****************************************************************************
*/
XDRV_DEBUG* boardHalDspGetDebugDriver( void );

/*
*****************************************************************************
** FUNCTION:   boardHalDspIsNarrowbandHal
**
** PURPOSE:    Checks if the specified HAL device is configured to run in
**             narrow band mode.
**
** PARAMETERS: devIndex (in) HAL device index in config entry table.  
**
** RETURNS:    XDRV_TRUE if HAL device is narrowband.
**             XDRV_FALSE if HAL device is wideband.
**
*****************************************************************************
*/
XDRV_BOOL boardHalDspIsNarrowbandHal( XDRV_UINT16 devIndex );

#ifdef CONFIG_SMP
/*
*****************************************************************************
** FUNCTION:   DspTaskSchedule
**
** PURPOSE:    Releases the semahpore and allows the DSP task to invoke
**             Hausware.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
void DspTaskSchedule( void );
#endif /* CONFIG_SMP */

#ifndef BRCM_NONDIST_BUILD
/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskStartTimer
**
** PURPOSE:    Start the DSP task timer.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskStartTimer( void );
#endif /* BRCM_NONDIST_BUILD */

#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_DSP_H  */
