/***************************************************************************
*    Copyright 2013  Broadcom
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
*    Filename: xdrvNeon.h
*
****************************************************************************
*    Description:
*
*     NEON core header.
*
****************************************************************************/

#ifndef XDRV_NEON_H
#define XDRV_NEON_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvNeonPowerup
**
** PURPOSE:    Powerup and enable NEON core
**
** PARAMETERS: None.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvNeonPowerup( void );


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_NEON_H  */
