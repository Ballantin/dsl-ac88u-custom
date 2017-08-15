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
*     This file implements the hardware-independent relay interface for
*     the 6328vw_fxo. 
*     
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalInit.h>
#include <bcmChip.h>
/* include driver-specific defines */
#include <boardparms.h>
#include <boardHalRelay.h>
#include <boardHalProv.h>
#include "boardHal6328.h"
#include "relay6328.h"

/* HW Specific relay configuration -- CONFIRM WITH BOARD SCHEMATIC!!! */
/* ---- Private Variables ------------------------------------------------ */
static RELAY_6328_DRV gRelayDriver[] =
{
   { NULL, 0, XDRV_FALSE, BP_NOT_DEFINED },
   { NULL, 1, XDRV_FALSE, BP_NOT_DEFINED },
};

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalRelayInit
**
** PURPOSE:    Initialize the HW Relay
**
** PARAMETERS: 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalRelayInit( VRG_BOOL arg )
{
   int i;
   RELAY_6328_DRV *pDev;
  
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "%s: ", __FUNCTION__ ));

   for ( i = 0; i < voiceParams->numFailoverRelayPins; i++ )
   {
      if ( voiceParams->pstnRelayCtrl.relayGpio[i] == BP_NOT_DEFINED )
      {  
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "GPIO for relay %d not set !!!", i ));
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "GPIO for relay %d is 0x%08X !!!", i, voiceParams->pstnRelayCtrl.relayGpio[i] ));
         gRelayDriver[i].ctrlGpioPin = voiceParams->pstnRelayCtrl.relayGpio[i];
         pDev = &( gRelayDriver[i] );
         relay6328Init( pDev ); 
      }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalRelayDeinit
**
** PURPOSE:    Shutdown the Relay
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalRelayDeinit( void )
{
   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "boardHalRelayDeInit()"));
   /* 
    * single call to deinitialization deinitializes both channels relays
    */
   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalRelayGetDriver
**
** PURPOSE:    Get the RELAY driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the RElay driver interface
**
*****************************************************************************
*/
XDRV_RELAY* boardHalRelayGetDriver( int chan )
{
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   XCHG_ASSERT( ( 0 <= chan ) && ( chan < voiceParams->numFailoverRelayPins ) );

   //VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "%s: GPIO for relay %d is 0x%08X !!!", __FUNCTION__, chan, (unsigned int)gRelayDriver[chan].ctrlGpioPin ));

   return ( (XDRV_RELAY*)(&gRelayDriver[chan]) );
}
