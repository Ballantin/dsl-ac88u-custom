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
*    Filename: lhapiExtension.h
*
****************************************************************************
*    Description:
*
*     This provides an "extension" of the "low-level" HAPI interface,
*     provided by lhapi.h.
*
****************************************************************************/

#ifndef LHAPI_EXTENSION_H
#define LHAPI_EXTENSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <lhapi.h>
#include <vrgTypes.h> 


/* ---- Constants and Types ---------------------------------------------- */

/* Define this feature based on some global config */
#ifdef BRCM_HAL_CAPTURE_TO_FILE
#define FEAT_HAL_CAPTURE_TO_FILE 1
#endif

/* Disable capture if DSP has no support for it */
#if !HAL_CAPTURE_ENABLED
#  undef  FEAT_HAL_CAPTURE_TO_FILE
#endif

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   lhapiInit
**
** PURPOSE:    Initialize the low-level HAPI module.
**
** PARAMETERS: isHauswareDistributed (in) Boolean indicating whether hausware
**                                        runs a distributed configuration
**                                        across host and DSP. Or whether it
**                                        only runs on the host processor.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiInit( VRG_BOOL isHauswareDistributed );


/*
*****************************************************************************
** FUNCTION:   lhapiDeinit
**
** PURPOSE:    De-init the low-level module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiDeinit( void );

/*
*****************************************************************************
** FUNCTION:   lhapiKNL_RunReadyThreads.
**
** PURPOSE:    Dispatches hausware threads by calling KNL_RunReadyThreads().
**             Note that we must save and restore the state of some 
**             important registers prior to and after calling KNL_RunReadyThreads(),
**             respectively.
**
** PARAMETERS: None
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiKNL_RunReadyThreads( unsigned long a );

/*
*****************************************************************************
** FUNCTION:   lhapi_ProcessEntrySync
**
** PURPOSE:    A wrapper for void lhapProcessEntrySync(void)
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapi_ProcessEntrySync(unsigned long arg);

#ifdef FEAT_HAL_CAPTURE_TO_FILE
/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureInit
**
** PURPOSE:    Sets up the HAL capture block for a specific channel
**
** PARAMETERS: chan - Hal index to capture from
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureInit( int chan );

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureStart
**
** PURPOSE:    Starts hal capture on all captureblocks
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureStart( void );

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureStop
**
** PURPOSE:    Stops all hal captures and closes all open files
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureStop( void );

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureDeInit
**
** PURPOSE:    Destroys all capture blocks
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureDeInit( void );

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureThreadDispatch
**
** PURPOSE:    This function is called by hausware when it is done to kick off 
**             the capture thread.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/

void lhapiHalCaptureThreadDispatch( void );

/*
*****************************************************************************
** FUNCTION:  lhapiHalCaptureThreadDeInit.
**
** PURPOSE:    De-initialize hal capture thread.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureThreadDeInit( void );

#endif

#ifdef __cplusplus
    }
#endif

#endif  /* LHAPI_EXTENSION_H  */
