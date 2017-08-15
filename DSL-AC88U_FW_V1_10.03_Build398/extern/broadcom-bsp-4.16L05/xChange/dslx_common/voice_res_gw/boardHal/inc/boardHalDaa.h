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
*    Filename: boardHalDaa.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Data Access Arrangement (DAA) present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_DAA_H
#define BOARD_HAL_DAA_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgCountry.h>
#include <xdrvDaa.h>
#include <board.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalDaaInit
**
** PURPOSE:    Initialize the DAA
**
** PARAMETERS: VRG_COUNTRY
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalDaaInit( VRG_COUNTRY locale );


/*
*****************************************************************************
** FUNCTION:   boardHalDaaSetLocale
**
** PURPOSE:    Sets the locale to be used by the DAA
**
** PARAMETERS: Locale - the locale to use.
**
** RETURNS:    Nothing
**
*****************************************************************************
*/
void boardHalDaaSetLocale( VRG_COUNTRY locale );


/*
*****************************************************************************
** FUNCTION:   boardHalDaaDeinit
**
** PURPOSE:    Shutdown the DAA
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalDaaDeinit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalIsDaa
**
** PURPOSE:    Determine whether a particular line uses a DAA
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    XDRV_TRUE if DAA, XDRV_FALSE otherwise (currently means SLIC)
**
*****************************************************************************
*/
XDRV_BOOL boardHalIsDaa( int chan );


/*
*****************************************************************************
** FUNCTION:   boardHalDaaGetDriver
**
** PURPOSE:    Get the DAA driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the DAA driver interface
**
*****************************************************************************
*/
XDRV_DAA* boardHalDaaGetDriver( int chan );

#ifdef DYING_GASP_API
void boardHalDaaShutdown( void *context );
#endif

#if __cplusplus
}
#endif


#endif   /* BOARD_HAL_DAA_H */
