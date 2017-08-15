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
*    Filename: boardHalCas3348vcm.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent CAS driver interface for
*     the 3348vcm.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <xchgAssert.h>
#include <boardHalCas.h>
#include <boardHalSlic.h>
#include <casDriverSim.h>
#include "boardHalSim.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

XDRV_CAS gCasDriver[ BOARD_HAL_SIM_NUM_ENDPTS ];


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalCasInit
**
** PURPOSE:    This function initializes the CAS drivers
**
** PARAMETERS: fastSlicSupported (in) True if fast SLIC mode changing should
**                                    be supported on the platform.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasInit( VRG_BOOL fastSlicSupported )
{
   int   endpt;
   int   result;

   for ( endpt = 0; endpt < BOARD_HAL_SIM_NUM_ENDPTS; endpt++ )
   {
      XDRV_SLIC *slicDrv = boardHalSlicGetDriver( endpt );
      XCHG_ASSERT( slicDrv != NULL );

      if ( 0 != ( result = casDriverSimInit( slicDrv,
                                             fastSlicSupported,
                                             &gCasDriver[ endpt ] )))
      {
         return ( result );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalCasDeinit
**
** PURPOSE:    This function de-initializes CAS drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalCasDeinit( void )
{
   int  endpt;
   int  result;

   for ( endpt = 0; endpt < BOARD_HAL_SIM_NUM_ENDPTS; endpt++ )
   {
      if (( result = casDriverSimDeinit( &gCasDriver[ endpt ] )) != 0 )
      {
         return ( 1 );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalCasGetDriver
**
** PURPOSE:    Get the CAS driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the CAS driver interface
**
*****************************************************************************
*/
XDRV_CAS* boardHalCasGetDriver( int chan )
{
   XCHG_ASSERT( ( 0 <= chan ) && ( chan < BOARD_HAL_SIM_NUM_ENDPTS ) );

   return ( (XDRV_CAS*)&(gCasDriver[chan]) );
}
