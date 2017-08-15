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
*    Filename: hal_dsp_nondist.h
*
****************************************************************************
*    Description:
*
*     Interface for the hardware abstraction layer (HAL) for the thread
*     processor running the bottom half of hausware (aka "DSP" thread processor).
*
****************************************************************************/

#ifndef HAL_DSP_NONDIST_H
#define HAL_DSP_NONDIST_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvItpc.h>
#include <xdrvSpinLock.h>
#include <knl_cfg.h>
#include <xcfg_halshim.h>
#include <bcmChipCommon.h>
#include "hal_dsp_nondist_common.h"

/* ---- Constants and Types ---------------------------------------------- */

/*
** Set to '1' to install a special overflow exception handler. This will be
** used to mask this exception type. Set to '0' to use the same exception
** handler for overflows as all other exception types.
*/
#define HAL_INSTALL_OVERFLOW_EXCEPTION_HANDLER   1

#if XCFG_HSF_WIDEBAND_SUPPORT
#define PCM_IRQ_BUFFER_SIZE                       KNL_CFG_FRAME_SYNC_SAMPLES_WIDEBAND
#else
#define PCM_IRQ_BUFFER_SIZE                       KNL_CFG_FRAME_SYNC_SAMPLES
#endif /* XCFG_HSF_WIDEBAND_SUPPORT */

#define PCM_NUM_VOICE_CHANNELS                    8 /* TODO: use a define from chip header */

typedef struct halIngress
{
  XDRV_UINT16 buffer[PCM_IRQ_BUFFER_SIZE];
} halIngress;

typedef struct halEgress
{
  XDRV_UINT16 buffer[PCM_IRQ_BUFFER_SIZE];
} halEgress;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   halPcmEnable
**
** PURPOSE:    To enable the PCM interrupt, which is used to transfer digital
**             samples to/from hausware. The client should be ready to service
**             PCM interrupts after calling this code.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halPcmEnable( void );
void halApmEnable( void );


/*
*****************************************************************************
** FUNCTION:   halPcmDisable
**
** PURPOSE:    To disable the PCM interrupt.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halPcmDisable( void );
void halApmDisable( void );


/****************************************************************************
*  halBackCountInc
*
*  PURPOSE:
*     Increment the background counter. This can be queried from the primary
*     thread processor to ensure that the background main loop is running.
*
*  RETURNS:
*     Nothing
****************************************************************************/
void halBackCountInc( void );

/*
*****************************************************************************
** FUNCTION:   hal6362StartDectBuffProcessing
**
** PURPOSE:    Modifies dect flag to start dect buffer processing 
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
unsigned int hal6362StartDectBuffProcessing( void );

/*
*****************************************************************************
** FUNCTION:   hal63268StartDectBuffProcessing
**
** PURPOSE:    Modifies dect flag to start dect buffer processing 
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
unsigned int hal63268StartDectBuffProcessing( void );

/*
*****************************************************************************
** FUNCTION:   halIsPowerSaveApmShutdownEnabled
**
** PURPOSE:    Indicates if the APM shutdown has been triggered by the 
**             main app. 
**
** PARAMETERS: 
** 
** RETURNS:   1 - enabled; 0 - disabled 
**
** NOTE:
*****************************************************************************
*/
int halIsPowerSaveApmShutdownEnabled( void );


/*
*****************************************************************************
** FUNCTION:   halRegisterFramework
**
** PURPOSE:    Registers the framework functions, whether we are in APM or
**             PCM mode.
**
** PARAMETERS: apmEnabled - 0 if we are in PCM mode, otherwise we are in APM
**                          mode.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halRegisterFramework(XDRV_BOOL apmEnabled);

#ifdef __cplusplus
    }
#endif

#endif  /* HAL_DSP_NONDIST_H  */
