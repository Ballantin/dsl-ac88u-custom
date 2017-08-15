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
*    Filename: debug.c
*
****************************************************************************
*    Description:
*
*     This file contains functions related to the Debug Module.
*     Debug module is used to retrieve info of interest from the DSP upon
*     failure conditions (assert, command timeouts, etc.). For now this is
*     going to dump out the entire IPC shared memory, but may be enhanced
*     in the future to dump internal DSP memory, registers, etc.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xdrvIpc.h>
#include <debug.h>
#include <string.h>
#include "debugIpcPrivate.h"



/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Max number of channels Debug module would attempt to debug */
#define DEBUG_MAX_NUMBER_OF_CHANNELS    20

typedef struct DEBUG_IPC_CHANNEL
{
   XDRV_IPC      *ipcDrvp;
   XDRV_UINT16    handle;
   XDRV_BOOL      debugDone;
} DEBUG_IPC_CHANNEL;


/*
**  IPC debug driver structure.
*/
typedef struct DEBUG_IPC_DRV
{
   /* Number of IPC channels registered with the module */
   int registerCount;

   /* Debug module internal state info. */
   DEBUG_IPC_CHANNEL chanState[DEBUG_MAX_NUMBER_OF_CHANNELS];

} DEBUG_IPC_DRV;


/* ---- Private Variables ------------------------------------------------ */

static DEBUG_IPC_DRV    gDebugIpc;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   debugIpcInit
**
** PURPOSE:    Initialize debug IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void debugIpcInit( void )
{
   unsigned int chan;

   memset( &gDebugIpc, 0, sizeof( gDebugIpc ) );


   /* Do not attempt to debug a channel until registered */
   for ( chan = 0; chan < DEBUG_MAX_NUMBER_OF_CHANNELS; chan++ )
   {
      gDebugIpc.chanState[chan].debugDone = XDRV_TRUE;
   }
}

/*
*****************************************************************************
** FUNCTION:   debugIpcDeinit
**
** PURPOSE:    De-initialize debug IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void debugIpcDeinit( void )
{
   debugIpcInit();
}

/*
*****************************************************************************
** FUNCTION:   debugIpcRegisterChannel
**
** PURPOSE:    Register IPC channel with debug module
**
** PARAMETERS: pDrv    - pointer to the Debug module info structure
**             pIpcDrv - IPC channel to register
**
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int debugIpcRegisterChannel( XDRV_IPC *pIpcDrv, XDRV_UINT16 handle )
{
   DEBUG_IPC_DRV *pDrv = &gDebugIpc;

   int result = 0;

   if ( pDrv->registerCount < DEBUG_MAX_NUMBER_OF_CHANNELS )
   {
      pDrv->chanState[pDrv->registerCount].debugDone  = XDRV_FALSE;
      pDrv->chanState[pDrv->registerCount].ipcDrvp = pIpcDrv;
      pDrv->chanState[pDrv->registerCount].handle  = handle;
   }
   else
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Max number of debug channels already reached (%d/%d) !",
                      pDrv->registerCount, DEBUG_MAX_NUMBER_OF_CHANNELS));
      result = 1;
   }

   pDrv->registerCount++;

   return ( result );
}

/*
*****************************************************************************
** FUNCTION:   debugIpcQueryChannel
**
** PURPOSE:    Query DSP Channel for info of interest
**
** PARAMETERS: handle  - channel to query
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void debugIpcQueryChannel( XDRV_UINT16 handle)
{
   DEBUG_IPC_DRV *pDrv  = &gDebugIpc;
   XDRV_BOOL      found = XDRV_FALSE;
   int            chan;

   /* Find the channel if registered */
   for (chan=0; chan<DEBUG_MAX_NUMBER_OF_CHANNELS; chan++)
   {
      if (pDrv->chanState[chan].handle == handle)
      {
         found = XDRV_TRUE;
         break;
      }
   }

   if (found && (!pDrv->chanState[chan].debugDone))
   {
      /* We found the channel and havent attempted to debug it yet */

      /* Print the IPC stats on the channel */
      if ((pDrv->chanState[chan].ipcDrvp != NULL) &&
          (pDrv->chanState[chan].handle  != 0))
      {
         xdrvIpcPrint(pDrv->chanState[chan].ipcDrvp, XDRV_IPC_PRINT_MODE_FULL_DUMP);
         pDrv->chanState[chan].debugDone = XDRV_TRUE;
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   debugIpcQueryAllChannels
**
** PURPOSE:    Query all DSP Channels for info of interest
**
** PARAMETERS:
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void debugIpcQueryAllChannels( void )
{
   DEBUG_IPC_DRV  *pDrv = &gDebugIpc;
   int chan;

   /* Find the channel if registered */
   for (chan=0; chan<DEBUG_MAX_NUMBER_OF_CHANNELS; chan++)
   {
      if (!pDrv->chanState[chan].debugDone)
      {
         /* Print the IPC stats on the channel */
         if (pDrv->chanState[chan].ipcDrvp != NULL)
         {
            xdrvIpcPrint(pDrv->chanState[chan].ipcDrvp, XDRV_IPC_PRINT_MODE_FULL_DUMP);
            pDrv->chanState[chan].debugDone = XDRV_TRUE;
         }
      }
   }
}
