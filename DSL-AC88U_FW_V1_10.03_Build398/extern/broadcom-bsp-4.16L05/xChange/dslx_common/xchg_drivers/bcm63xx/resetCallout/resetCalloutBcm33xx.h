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
*    Filename: resetCalloutBcm33xx.c
*
****************************************************************************
*    Description:
*
*     Implements an interface to the Cable Modem Board Support Package (BSP)
*     for registering reset Callout routines.
*
*     All functions declared in this file must be implemented by the CM BSP.
*
****************************************************************************/

#ifndef RESET_CALLOUT_BCM_33XX_H
#define RESET_CALLOUT_BCM_33XX_H

#if __cplusplus
extern "C" {
#endif

/*
*****************************************************************************
** FUNCTION:   BcmResetBoard_RegisterCallout
**
** PURPOSE:  
**   This provides an opportunity for a single outside modules to register a 
**   callout to be invoked just before board reset.  The original need was 
**   derived from 3341 based platforms that needed to gracefully shutdown the
**   HVG before reset to prevent hardware problems.  
**   If the need grows, this could easily become a list of callouts. 
**
** PARAMETERS: pfunc       - pointer to callout routine
**
** RETURNS:    
**
** NOTES: This routine is implemented within Board Support Package. 
**
*****************************************************************************
*/
extern void BcmResetBoard_RegisterCallout( void (*pCalloutFunction)(void) );


#if __cplusplus
}
#endif

#endif   /* RESET_CALLOUT_BCM_33XX_H */
