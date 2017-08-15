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
*    Filename: boardHalSlic3348vcm.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface for
*     the 3348vcm.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <boardHalSlic.h>
#include "boardHalSim.h"

/* include driver-specific defines */
#include <slicSim.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* SLIC information for each endpoint */
SLIC_SIM_DRV gSlicDriver[ BOARD_HAL_SIM_NUM_ENDPTS ];


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalSlicInit
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: country - current country code
**             arg - reserved argument.
** PARAMETERS: country - reserved argument.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInit( VRG_BOOL arg, VRG_COUNTRY country )
{
   SLIC_SIM_DRV        *pDev;
   int                  endpt;

   for ( endpt = 0; endpt < BOARD_HAL_SIM_NUM_ENDPTS; endpt++ )
   {
      pDev = &(gSlicDriver[endpt]);

      /* Initialize the Agere SLIC */
      slicSimInit( endpt, pDev );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinit
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinit( void )
{
   SLIC_SIM_DRV        *pDev;
   int                  endpt;

   for ( endpt = 0; endpt < BOARD_HAL_SIM_NUM_ENDPTS; endpt++ )
   {
      pDev = &(gSlicDriver[endpt]);

      /* Disable the Agere SLIC */
      slicSimDeinit( pDev );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicForceHvgShutdown
**
** PURPOSE:    Forces the HVG shutdown regarless of the 3341 state
**             (tries to even before 3341 is initialized).
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
** NOTE:       This is intended to be used by the maintanace thread
**             for interroperability with home security systems.
**
*****************************************************************************
*/
int boardHalSlicForceHvgShutdown( void )
{
   /* Nothing to do here */
   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefresh
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a  
**             higher application layer (such as Maintanance Task). Satisfying these 
**             requirements may necessitate to shutdown the HVG even after the 
**             boardHalInit has configured the HVG for the last time. 
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.            
**
*****************************************************************************
*/
int boardHalSlicHvgRefresh( void )
{
   /* Nothing to do here */
   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriver
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriver( int chan )
{
   XCHG_ASSERT( ( 0 <= chan ) && ( chan < BOARD_HAL_SIM_NUM_ENDPTS ) );

   return ( (XDRV_SLIC*)&(gSlicDriver[chan]) );
}
