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
*    Filename: xdrvResetCallout.h
*
****************************************************************************
*    Description:
*
*        This module provides interface to register a callout with the 
*        Board Support Package. This callout gets invoked every time a 
*        software board reset is performed (command line reset, wdog 
*        timer reset, exception handler, etc.).
*        The original need was derived from 3341 based platforms that 
*        needed to gracefully shutdown the HVG before reset to prevent 
*        hardware problems.
*
****************************************************************************/

#ifndef XDRV_RESET_CALLOUT_H
#define XDRV_RESET_CALLOUT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** Callout function prototype that may be registered.
*/
typedef void ( *XDRV_RESET_CALLOUT_FNC )( void );


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvResetCalloutInstall
**
** PURPOSE:    Register a callout to be invoked just before board reset.
**             The callout then gets invoked every time a software board 
**             reset is performed (command line reset, wdog timer reset, 
**             exception handler, etc.)
**             
**             The original need was derived from 3341 based platforms 
**             that needed to gracefully shutdown the HVG before 
**             reset to prevent hardware problems.
**
** PARAMETERS: pfunc       - pointer to callout routine
**
** RETURNS:    Nothing.
**
** NOTE:       
*****************************************************************************
*/
void xdrvResetCalloutInstall( XDRV_RESET_CALLOUT_FNC pfunc );


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_RESET_CALLOUT_H  */
