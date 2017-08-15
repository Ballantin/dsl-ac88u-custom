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
*    Filename: apmSim.c
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      Analog Processsor Module (APM) API.
*
****************************************************************************/


/* ---- Include Files ---------------------------------------------------- */

#include <xdrvLog.h>
#include <string.h>
#include "apmSim.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */

static void apmSetRingParms( XDRV_APM *pDrv, int ringFrequency, int ringWaveshape, int ringVoltage, int ringOffset, int ringOffsetCal );
static void apmGetRingParms( XDRV_APM *pDrv, int *ringFrequency, int *ringWaveshape, int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static XDRV_SINT16 apmGetDlp( XDRV_APM *pDrv);
static XDRV_SINT32 apmDebugStatusDump( XDRV_APM *pDrv );
static XDRV_SINT16 apmGetElp( XDRV_APM *pDrv);
static XDRV_BOOL hvgGetOverCurrentStatus( XDRV_APM *pDrv, int chan );
static void hvgGetOverVoltageStatus( XDRV_APM *pDrv, int chan );
static void apmPulseSignal( XDRV_APM *pDrv, int endpt, int duration, int period,
                                int repetition, int amplitude, int frequency );
static int hvgSetVoltage( XDRV_APM *pApmDrv, int chan, int voltage );

/* ---- Private Variables ------------------------------------------------ */

/* SLIC "meta-driver" interface */
static const XDRV_APM_FUNCS  gApmSimDrvFuncs =
{
   apmSetRingParms,
   apmGetRingParms,
   apmGetDlp,
   apmDebugStatusDump,
   apmGetElp,
   hvgGetOverCurrentStatus,
   hvgGetOverVoltageStatus,
   apmPulseSignal,
   hvgSetVoltage
};


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   apmSimInit
**
** PURPOSE:    Initialize the APM
**
** PARAMETERS: pDev       - pointer to the APM info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void apmSimInit
(
   APM_SIM_DRV   *pDev
)
{
   memset( pDev, 0, sizeof( APM_SIM_DRV ) );

   pDev->pDrvFuncs   = &gApmSimDrvFuncs;

}


/*
*****************************************************************************
** FUNCTION:   apmSimDeinit
**
** PURPOSE:    Shutdown the APM
**
** PARAMETERS: pDev  - pointer to the APM info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void apmSimDeinit( APM_SIM_DRV *pDev )
{
   (void) pDev;
}


/* Wrappers for APM functions when actual APM functionality is not used */

void apmSetRingParms( XDRV_APM *pDrv, int ringFrequency, int ringWaveshape, int ringVoltage, int ringOffset, int ringOffsetCal )
{
}


void apmGetRingParms( XDRV_APM *pDrv, int *ringFrequency, int *ringWaveshape, int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
}


XDRV_SINT16 apmGetDlp( XDRV_APM *pDrv)
{
   return ( 0xFFF );
}


XDRV_SINT32 apmDebugStatusDump( XDRV_APM *pDrv )
{
   return ( 0 );
}


XDRV_SINT16 apmGetElp( XDRV_APM *pDrv)
{
   return ( 0 );
}


XDRV_BOOL hvgGetOverCurrentStatus( XDRV_APM *pDrv, int chan )
{
   return ( XDRV_FALSE );
}


void hvgGetOverVoltageStatus( XDRV_APM *pDrv, int chan )
{
}


void apmPulseSignal( XDRV_APM *pDrv, int endpt, int duration, int period,
                         int repetition, int amplitude, int frequency )
{
}


int hvgSetVoltage( XDRV_APM *pApmDrv, int chan, int voltage )
{
   return ( 0 );
}
