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
*    Filename: hal_dectI.h
*
****************************************************************************
*    Description:
*
*     Interface for 3368 hardware abstraction layer (HAL) for the thread
*     processor running the bottom half of hausware (aka "DSP" thread processor).
*
****************************************************************************/

#ifndef HAL_DECTI_H
#define HAL_DECTI_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <xdrvTypes.h>
#include <stddef.h>

/* ---- Constants and Types ---------------------------------------------- */
typedef void (*DECT_FUNC_START) ( XDRV_UINT32 ddrAddrTxV, XDRV_UINT32 ddrAddrRxV );
typedef void (*DECT_FUNC_STOP) ( void );

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   halDectIRegisterStartBP
**
** PURPOSE:    register the function for starting DECT buffer processing. 
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halDectIRegisterStartBP( DECT_FUNC_START customFunc );

/*
*****************************************************************************
** FUNCTION:   halDectIRegisterStopBP
**
** PURPOSE:    register the function for stopping DECT buffer processing. 
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halDectIRegisterStopBP( DECT_FUNC_STOP customFunc );

/*
*****************************************************************************
** FUNCTION:   halDectIStartDectBuffProcessing
**
** PURPOSE:    Executes function pointer if registered to start DECT buffer
**             processing.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halDectIStartDectBuffProcessing( XDRV_UINT32 ddrAddrTxV, XDRV_UINT32 ddrAddrRxV );


/*
*****************************************************************************
** FUNCTION:   halDectIStopDectBuffProcessing
**
** PURPOSE:    Executes function pointer if registered to stop DECT buffer
**             processing.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void halDectIStopDectBuffProcessing( void );


#ifdef __cplusplus
    }
#endif

#endif  /* HAL_DECTI_H  */
