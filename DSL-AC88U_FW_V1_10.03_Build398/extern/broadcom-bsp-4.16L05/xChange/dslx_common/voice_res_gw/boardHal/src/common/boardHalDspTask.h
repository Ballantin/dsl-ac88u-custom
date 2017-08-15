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
*    Filename: boardHalDspTask.h
*
****************************************************************************
*    Description:
*
*     This file contains the interface used by the DSP processing timer task.
*
****************************************************************************/

#ifndef BOARD_HAL_DSP_TASK_H
#define BOARD_HAL_DSP_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <boardHalDsp.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:      boardHalDspTaskInit
**
** PURPOSE:       Initialization of the DSP timer processing task.
**
** PARAMETERS:    None.
**
** RETURNS:       0 on success, -1 on failure
**
** NOTE:
*****************************************************************************
*/
int boardHalDspTaskInit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskDeinit
**
** PURPOSE:    DSP task deinit - call once during system shutdown.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that this function is only called after the
**             task has been notified of a pending application reset, and it
**             has acknowledged the notification. This implies that the
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
void boardHalDspTaskDeinit( void );

/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskRegisterCallback
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
void boardHalDspTaskRegisterCallback( BOARD_HAL_DSP_TASK_FUNC dspTaskFunc );


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskHalt
**
** PURPOSE:    Halt the DSP task
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskHalt( void );


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskResume
**
** PURPOSE:    Resume the DSP task
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskResume( void );


#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_DSP_TASK_H  */
