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
*    Filename: boardHalMspi.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      Master Serial Peripheral Interface (MSPI) driver present on a board
*
****************************************************************************/

#ifndef BOARD_HAL_MSPI_H
#define BOARD_HAL_MSPI_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalMspiInit
**
** PURPOSE:    Initialize the MSPI drivers.
**
** PARAMETERS: 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalMspiInit( void );


/*
*****************************************************************************
** FUNCTION:   boardHalMspiDeinit
**
** PURPOSE:    De-initialize the MSPI drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalMspiDeinit( void );


#ifdef __cplusplus
}
#endif

#endif  /* BOARD_HAL_MSPI_H  */
