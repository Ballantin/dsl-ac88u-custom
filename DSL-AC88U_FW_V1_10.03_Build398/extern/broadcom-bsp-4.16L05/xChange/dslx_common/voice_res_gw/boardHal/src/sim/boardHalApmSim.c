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
*    Filename: boardHalApmSim.c
*
****************************************************************************
*    Description:
*
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalApm.h>
#include <xdrvApm.h>
#include <apmSim.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

static APM_SIM_DRV   gApmDrv;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalApmInit
**
** PURPOSE:    Initialize the APM
**
** PARAMETERS: widebandMode      - 0=8kHz sampling; 1=16kHz sampling
**             packetRateSamples - packet rate in 8kHz samples
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL widebandMode, int packetRateSamples, int useRingGen  )
{

   /*
   ** Initialize the analog processor module.
   */
   apmSimInit( &gApmDrv );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalApmDeinit
**
** PURPOSE:    Shutdown the APM
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmDeinit( void )
{
   /*
   ** Shutdown the analog processor module.
   */
   apmSimDeinit( &gApmDrv );



   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalApmGetDriver
**
** PURPOSE:    Get the APM driver
**
** PARAMETERS:
**
** RETURNS:    pointer to the APM driver interface
**
*****************************************************************************
*/
XDRV_APM* boardHalApmGetDriver( int dspNum )
{
   return ( (XDRV_APM *) &gApmDrv );
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmGetChanSwapStatus
**
** PURPOSE:    Determines if channel swap is set in the APM
**
** PARAMETERS: none
**
** RETURNS:    True if set, false otherwise
**
*****************************************************************************
*/
int boardHalApmGetChanSwapStatus( void )
{   
        return XDRV_FALSE;
}
