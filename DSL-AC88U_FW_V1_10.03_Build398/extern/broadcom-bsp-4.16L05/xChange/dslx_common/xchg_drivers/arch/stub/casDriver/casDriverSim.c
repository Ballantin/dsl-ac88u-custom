/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: casDriver.c
*
****************************************************************************
*    Description:
*
*     This file implements the CAS driver. The purpose of the CAS driver is:
*
*     1) To interface to the SLIC driver to control the SLIC mode and
*        obtain hookstate information.
*
*     2) To optionally provide a CAS history buffer. The history buffer provides
*        a periodic log of hook-state sampled by an ISR. The samples should be
*        processed by a client state-machine in order to detect CAS events
*        (e.g. onhook, offhook, flash, wink, pulse dial).
*
*     3) To provide low-level debounce logic. Samples may be debounced prior to
*        being logged to the history buffer.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xchgAssert.h>
#include <xdrvCas.h>
#include <casDriverSim.h>
#include <string.h>



/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   casDriverInit
**
** PURPOSE:    This function initializes the hook state history log
**             needed for CAS
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverSimInit( XDRV_SLIC *slicDrv, XDRV_BOOL fastSlicSupported, XDRV_CAS *casDriver )
{
   (void) fastSlicSupported;

   memset( casDriver, 0, sizeof( XDRV_CAS ) );

   /* Save the SLIC driver function pointer to the CAS driver*/
   casDriver->slicDriver = slicDrv;
   casDriver->nextDrv = NULL;

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   casDriverDeinit
**
** PURPOSE:    This function de-initializes a CAS driver
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int casDriverSimDeinit( XDRV_CAS *casDriver )
{
   memset( casDriver, 0, sizeof( XDRV_CAS ) );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   casNumHookSamples
**
** PURPOSE:    Retrieves the number of states available in the hook state
**             history buffer since the last run
**
** PARAMETERS: chan - channel to get the history size on
**
** RETURNS:    Number of states available
**
*****************************************************************************
*/
int xdrvCasNumHookSamples( XDRV_CAS *casDriver )
{
   return ( 1 );
}

/*
*****************************************************************************
** FUNCTION:   casGetHookState
**
** PURPOSE:    To determine if a channel is on or off hook
**
** PARAMETERS: chan - the channel number
**
** RETURNS:    XDRV_CAS_ONHOOK or XDRV_CAS_OFFHOOK
**
*****************************************************************************
*/
XDRV_CAS_HOOKSTATE xdrvCasGetHookState( XDRV_CAS *casDriver )
{

   /* Retrieve Slic State */
   return ( xdrvSlicIsOffhook( casDriver->slicDriver ) );

}
