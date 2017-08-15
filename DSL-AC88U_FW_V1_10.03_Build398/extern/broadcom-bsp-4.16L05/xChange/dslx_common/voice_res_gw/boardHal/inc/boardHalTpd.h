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
*    Filename: halCas.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware dependent public interface for the
*      Telephony Port Diagnostics (TPD) drivers present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_TPD_H
#define BOARD_HAL_TPD_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTpd.h>

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalApmTpdInit
**
** PURPOSE:    Initialize the Tpd drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmTpdInit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalApmTpdDeinit
**
** PURPOSE:    De-initialize the Tpd drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmTpdDeinit( void );

/*
*****************************************************************************
** FUNCTION:   boardHalTpdGetDriver
**
** PURPOSE:    Get the TPD driver
**
** PARAMETERS: none
**
** RETURNS:    pointer to the TPD driver interface
**
*****************************************************************************
*/
XDRV_TPD* boardHalTpdGetDriver( void );



/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdInit
**
** PURPOSE:    Initialize the Tpd drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalPcmTpdInit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdDeinit
**
** PURPOSE:    De-initialize the Tpd drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalPcmTpdDeinit( void );

/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdGetDriver
**
** PURPOSE:    Get the TPD driver
**
** PARAMETERS: none
**
** RETURNS:    pointer to the TPD driver interface
**
*****************************************************************************
*/
XDRV_TPD* boardHalPcmTpdGetDriver( void );

/*
*****************************************************************************
** FUNCTION:   boardHalPcmTpdMapLineId
**
** PURPOSE:    Map FXS Tpd Line ID to its init Line IDs
**
** PARAMETERS: Line ID
**
** RETURNS:    pointer to the TPD driver interface
**
*****************************************************************************
*/
int boardHalPcmTpdMapLineId( int lineId);

/*
*****************************************************************************
** FUNCTION:   boardHalTpdIsSupportedLine
**
** PURPOSE:    Get FXS line is TPD supported line or not
**
** PARAMETERS: Line ID
**
** RETURNS:    pointer to the TPD driver interface
**
*****************************************************************************
*/
XDRV_BOOL boardHalTpdIsSupportedLine( int lineId );

/*
*****************************************************************************
** FUNCTION:   boardHalTpdIsZarlinkLine
**
** PURPOSE:    Get the Line is Zarlink or not
**
** PARAMETERS: endpoint id
**
** RETURNS:    XDRV_TRUE when line is Zarlink Line, otherwise XDRV_FALSE
**
*****************************************************************************
*/
XDRV_BOOL boardHalTpdIsZarlinkLine( int lineId );

#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_TPD_H  */
