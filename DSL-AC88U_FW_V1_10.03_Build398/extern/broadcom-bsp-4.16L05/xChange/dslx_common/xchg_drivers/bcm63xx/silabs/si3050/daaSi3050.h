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
*    Filename: daaSi3050.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      SiLabs Si3050 Data Access Arrangement circuit (DAA) API.
*
****************************************************************************/

#ifndef DAA_Si3050_H
#define DAA_Si3050_H

#if __cplusplus
extern "C" {
#endif

#include <xdrvDaa.h>
#include <bcmChip.h>
#include "daaSi3050Defs.h"
#include <vrgCountry.h>
#include <board.h>

/*
** Si3050 DAA driver structure. Conceptually, this structure is similar to
** a C++ sub-class that is derived from the 'XDRV_DAA' base-class(structure).
** The base DAA structure specifies a set of function pointers that defines
** the generic hardware-independent API that must be provided by all chip
** specific DAA drivers.
**
** When the DAA driver initialization function is invoked, the base HPI
** structure function pointers will be intialized to a set of hardware specific
** DAA driver functions. Therefore, given a pointer to the base DAA driver,
** clients can interface to a hardware-independent API that will vector to the
** hardware-specific DAA driver function.
**
** Si3050 DAA driver state should be stored in the
** 'DAA_Si3050_DRV' structure.  Note that since Si3050 is a single-channel
** DAA, one instance of 'DAA_Si3050_DRV' contains information for 1
** physical FXO (PSTN) endpoints.  Information specific to each endpoint is saved inside
** the 'DAA_Si3050_CHAN' structure.
*/

/* 
 * Si3050 Single-channel information
 */
typedef struct DAA_Si3050_CHAN
{
   /* Must be first element of this struct */
   const XDRV_DAA_FUNCS  *pDrvFuncs;

   int                    chan;        /* Channel ID            */

} DAA_Si3050_CHAN;

/* 
 * Si3050 Dual-DAA information
 */
typedef struct DAA_Si3050_DRV
{
   /* Two channels per Si3050 DAA */
   DAA_Si3050_CHAN         daaInfo;
   int                     daaId;        /* DAA ID */
   VRG_COUNTRY             daaLocale;
   int                     mspiId;       /* device ID on MSPI */
   int                     pcmMode;       /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     txTimeSlot;    /* pcm highway transmit timeslot */
   int                     rxTimeSlot;    /* pcm highway receive timeslot */

} DAA_Si3050_DRV;


/*
   Si3050 Register Value Definition.
*/
typedef struct
{
   XDRV_UINT16    reg;
   XDRV_UINT8     field;
   XDRV_UINT8     val;

} SI3050_REGISTER_VALUE;


/*
*****************************************************************************
** FUNCTION:   daaSi3050Init
**
** PURPOSE:    Initialize and open the Si3050 DAA
**
** PARAMETERS: chan      - the line number ( 0 referenced )
**             pmmr  - pointer to the  memory map
**             pDev      - pointer to the Si3050 DAA info structure
**             mspiHdl   - MSPI handle
**             rstGpioPin-  GPIO pin used to reset the DAA
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaSi3050Init
(
   VRG_COUNTRY             locale,
   int                     chan,
   DAA_Si3050_DRV     *pDev, 
   XDRV_UINT16             rstGpioPin
);

/*****************************************************************************
*
* FUNCTION:   daaSi3050Reset
*
* PURPOSE:    Reset the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaSi3050Reset
(
   XDRV_UINT16 rstGpioPin
);

/*
*****************************************************************************
** FUNCTION:   daaSi3050Deinit
**
** PURPOSE:    Shutdown the Si3050 DAA
**
** PARAMETERS: pDev  - pointer to the Si3050 DAA info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaSi3050Deinit( DAA_Si3050_DRV *pDev );

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   daaSi3050Shutdown
*
* PURPOSE:    Shutdown the DAAs
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void daaSi3050Shutdown ( int rstGpioPin );
#endif

#if __cplusplus
}
#endif


#endif   /* DAA__Si3050_H */
