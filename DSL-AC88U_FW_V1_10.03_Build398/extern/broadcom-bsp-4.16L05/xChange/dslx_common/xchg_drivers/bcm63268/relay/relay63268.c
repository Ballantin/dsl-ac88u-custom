/***************************************************************************
*
*    Copyright 2005  Broadcom Corporation
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
*    Description: 
*      This is the Legerity Le88221 - Single Channel Digital SLIC Device
*      driver implementation.
*
***************************************************************************/


/* ---- Include Files ---------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include "relay63268.h"

/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */
/* Used to remove the active-high or active-low qualifying bit in the pin definition */
#define GPIO_PIN_MASK     0xFF

/* ---- Private Variables ------------------------------------ */

/* Create a static array of each country's WinSLAC parameters */

/* ---- Private Function Prototypes -------------------------- */
static XDRV_BOOL relayIsActive( XDRV_RELAY *pDrv );
static void relaySetActive( XDRV_RELAY *pDrv, XDRV_RELAY_MODE mode );

/* SLIC "meta-driver" interface */
static const XDRV_RELAY_FUNCS relayDrvFuncs =
{
   relayIsActive,
   relaySetActive
};

/* Private Functions */

/*
*****************************************************************************
** FUNCTION:   relayInit
**
** PURPOSE:    Initialize relay driver
**
** PARAMETERS: 
**
** RETURNS:    RELAY_STATUS
**
*****************************************************************************
*/
RELAY_STATUS relayInit( RELAY_DRV *pDev )
{
   pDev->pDrvFuncs = &relayDrvFuncs;

   return( RELAY_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   relayDeinit
**
** PURPOSE:    Shutdown relay driver 
**
** PARAMETERS: 
**
** RETURNS:    RELAY_STATUS
**
*****************************************************************************
*/
RELAY_STATUS relayDeinit( RELAY_DRV *pDev )
{

   return( RELAY_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   relayIsActive
**
** PURPOSE:    Determine if a channel is relay on or off
**
** PARAMETERS: 
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL relayIsActive( XDRV_RELAY *pDrv )
{
   RELAY_DRV *pDev = (RELAY_DRV *)pDrv;

   if( GPIO->GPIOio & (1 << (pDev->ctrlGpioPin && GPIO_PIN_MASK)) )
   {
       XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, " Relay is active on channel %d", pDev->chan));
       return (XDRV_TRUE);
   }
   else
   {
       XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, " Relay is deactive on channel %d", pDev->chan));
       return (XDRV_FALSE);
   }
}


/*
*****************************************************************************
** FUNCTION:   relaySetActive
**
** PURPOSE:    Sets the RELAY into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLIC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void relaySetActive( XDRV_RELAY *pDrv, XDRV_RELAY_MODE mode )
{
   RELAY_DRV *pDev = (RELAY_DRV *)pDrv;

   switch ( mode )
   {
      case RELAY_MODE_ACTIVE:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, " Active relay on channel %d pin = %d ", pDev->chan, (int)pDev->ctrlGpioPin ));
         kerSysSetGpio( pDev->ctrlGpioPin, kGpioActive );
      }
      break;

      case RELAY_MODE_DEACTIVE:
      default:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, " Deactive relay on channel %d pin = %d ", pDev->chan, (int)pDev->ctrlGpioPin ));
         kerSysSetGpio( pDev->ctrlGpioPin, kGpioInactive );
      }
      break;
   }

   return;
}


