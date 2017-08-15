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
*    Filename: slicSim.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      Subscriber Line Interface Circuits (SLIC).
*
****************************************************************************/


/* ---- Include Files ---------------------------------------------------- */

#include <xdrvLog.h>
#include <string.h>
#include "slicSim.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */

static void slicSimModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slicSimLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static void slicPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicSimIsOffhook( XDRV_SLIC *pDrv );
static int slicSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
static int slicGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static int slicSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static int slicSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicGetOverCurrentStatus( XDRV_SLIC *pDrv );


/* ---- Private Variables ------------------------------------------------ */

/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS  gSlicSimDrvFuncs =
{
   slicSimModeControl,
   slicSimLedControl,
   slicPhaseReversalControl,
   slicSimIsOffhook,
   slicSetRingParms,
   slicGetRingParms,
   slicSetBoostedLoopCurrent,
   slicSetPowerSource,
   slicGetOverCurrentStatus
};


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   slicSimInit
**
** PURPOSE:    Initialize and open the SLIC
**
** PARAMETERS: chan       - the line number ( 0 referenced )
**             pDev       - pointer to the Sim SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSimInit
(
   int                     chan,
   SLIC_SIM_DRV           *pDev
)
{
   memset( pDev, 0, sizeof( SLIC_SIM_DRV ) );

   pDev->pDrvFuncs   = &gSlicSimDrvFuncs;
   pDev->chan        = chan;
   pDev->isOffHook   = XDRV_FALSE;
}




/*
*****************************************************************************
** FUNCTION:   slicSimDeinit
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: pDev  - pointer to the Sim SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSimDeinit( SLIC_SIM_DRV *pDev )
{
   memset( pDev, 0, sizeof( SLIC_SIM_DRV ) );
}

/* Wrappers for SLIC functions when actual SLIC functionality is not used */

static void slicSimModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   (void) pDrv;
   (void) mode;
}


static void slicSimLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   (void) pDrv;
   (void) value;
}


static void slicPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   (void) pDrv;
   (void) value;
}


static XDRV_BOOL slicSimIsOffhook( XDRV_SLIC *pDrv )
{
   SLIC_SIM_DRV  *pDev = (SLIC_SIM_DRV *) pDrv;

   return( pDev->isOffHook );
}


static int slicSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   (void)pDrv;
   (void)ringFrequency;
   (void)ringWaveshape;
   (void)ringVoltage;
   (void)ringOffset;
   (void)ringOffsetCal;

   return ( 0 );
}


static int slicGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   (void)pDrv;
   (void)ringFrequency;
   (void)ringWaveshape;
   (void)ringVoltage;
   (void)ringOffset;
   (void)ringOffsetCal;

   return ( 0 );
}


static int slicSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   (void) pDrv;
   (void) value;

   return( 0 );
}


static int slicSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   (void) pDrv;
   (void) value;

   return( 0 );
}


static XDRV_BOOL slicGetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   (void)pDrv;

   return ( XDRV_FALSE );
}
