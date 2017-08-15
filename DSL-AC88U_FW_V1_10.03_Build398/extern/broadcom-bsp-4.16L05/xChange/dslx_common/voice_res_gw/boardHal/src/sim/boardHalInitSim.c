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
*    Filename: boardHalInit3348vcm.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent initialization interface for
*     the 3348vcm.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalInit.h>
#include <boardHalIpc.h>
#include <boardHalSlic.h>
#include <boardHalCas.h>
#include <boardHalTpd.h>
#include <xchgAssert.h>
#include <vrgCountry.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalInit
**
** PURPOSE:    Initialize the HAL.
**
** PARAMETERS: country - current country code
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInit( VRG_COUNTRY country )
{

   /*
   ** Initialize the analog processor module.
   */
   boardHalApmInit( VRG_FALSE, 8 );

   /*
   ** Initialize the SLIC
   */
   if ( boardHalSlicInit(VRG_FALSE, country) )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL, "ERROR: SLIC driver init FAILED" ));
      return ( -1 );
   }

   /*
   ** Initialize the hook state history log for CAS
   */
   if ( boardHalCasInit() )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_BOARDHAL, "ERROR: CAS History init FAILED" ));
      return ( -2 );
   }


   /*
   ** Initialize the host-DSP communications layer
   */
   boardHalIpcInit( NULL, 0 );


   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDeinit
**
** PURPOSE:    Uninitialize the HAL. Reset any local storage and
**             free all resources.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, otherwise error code.
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
*****************************************************************************
*/
int boardHalDeinit( void )
{

   /*
   ** Shutdown the host-DSP communications layer
   */
   boardHalIpcDeinit();


   /*
   ** Shutdown the CAS driver.
   */
   boardHalCasDeinit();


   /*
   ** Shutdown the SLICs
   */
   boardHalSlicDeinit();

   /*
   ** Shutdown the analog processor module.
   */
   boardHalApmDeinit();


   return ( 0 );
}
