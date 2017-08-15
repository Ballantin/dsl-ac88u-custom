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
*    Filename: boardHalSlic.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Subscriber Line Interface Circuits (SLIC) present on a board.
*
****************************************************************************/

#ifndef BOARD_HAL_RELAY_H
#define BOARD_HAL_RELAY_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvRelay.h>
#include <vrgTypes.h>
#include <board.h>


/* ---- Constants and Types ---------------------------------------------- */
#define BOARD_HAL_NUM_RELAY_MAX    2
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalRelayInit
**
** PURPOSE:    Initialize the voice channel RELAY
**
** PARAMETERS: none 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalRelayInit( VRG_BOOL arg );


/*
*****************************************************************************
** FUNCTION:   boardHalRelayDeinit
**
** PURPOSE:    Shutdown the RELAY control
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalRelayDeinit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalRelayGetDriver
**
** PURPOSE:    Get the RELAY driver associated with a channel
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the relay driver interface
**
*****************************************************************************
*/
XDRV_RELAY* boardHalRelayGetDriver( int chan );

#if __cplusplus
}
#endif


#endif   /* BOARD_HAL_RELAY_H */
