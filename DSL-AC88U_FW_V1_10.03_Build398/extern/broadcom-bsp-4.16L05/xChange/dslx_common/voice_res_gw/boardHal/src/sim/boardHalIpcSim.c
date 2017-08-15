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
*    Filename: boardHalIpcSim.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent IPC interface.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <boardHalIpc.h>
#include <string.h>
#include "boardHalSim.h"
#include <xdrvIpc.h>
#include <ipcSim.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Resman + 3 VHDs per endpt. */
#define MAX_IPC_CHANNELS   ( 1 + 3 * BOARD_HAL_SIM_NUM_ENDPTS )

/* ---- Private Variables ------------------------------------------------ */

/*
** IPC driver instance memory.
*/
static IPC_SIM_DRV gIpcDrv[ MAX_IPC_CHANNELS ];


/*
** Number of allocated channels.
*/
static unsigned int  gChannelAllocCount;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalIpcInit
**
** PURPOSE:    Initialize the IPC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalIpcInit
(
   volatile VRG_UINT16 *pIpcMem,
   unsigned int         ipcHeapSizeWords
)
{

   gChannelAllocCount   = 0;

   memset( gIpcDrv, 0, sizeof( gIpcDrv ) );


   /*
   ** Initialize the IPC module.
   */
   ipcSimInit();


   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDeinit
**
** PURPOSE:    Shutdown the IPC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalIpcDeinit( void )
{

   /*
   ** De-initialize the IPC module.
   */
   ipcSimDeinit();


   gChannelAllocCount   = 0;
   memset( gIpcDrv, 0, sizeof( gIpcDrv ) );


   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalIpcCreateChannel
**
** PURPOSE:    To create an IPC channel.
**
** PARAMETERS: deviceNum   (in)  Device (DSP) index for which the IPC channel
**                               should be created.
**             chanType    (in)  Channel type to create.
**             comHandle   (in)  Communication handle that can be used to
**                               uniquely identify the IPC channel on both
**                               processors.
**
** RETURNS:    Pointer to IPC driver instance.
**
** NOTE:
*****************************************************************************
*/
XDRV_IPC* boardHalIpcCreateChannel
(
   unsigned int                  deviceNum,
   BOARD_HAL_IPC_CHANNEL_TYPE    chanType,
   BOARD_HAL_IPC_COM_HANDLE      comHandle,
   BOARD_HAL_IPC_INIT_INFO      *pIpcInfo
)


{
   (void) deviceNum;
   (void) chanType;
   (void) comHandle;
   (void) pIpcInfo;


   /*
   ** Open the channel.
   */
   ipcSimOpen( &gIpcDrv[ gChannelAllocCount ] );

   return ( (XDRV_IPC*) &gIpcDrv[ gChannelAllocCount++ ] );
}


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDestroyChannel
**
** PURPOSE:    To destroy a previously created IPC channel.
**
** PARAMETERS: ipcDrv   (in)  IPC driver.
**
** RETURNS:    void
**
** NOTE:       This module does NOT support the dynamic allocation and
**             de-allocation of individual IPC channels due to the extremely
**             simplictic mechanism used for heap allocation. It is assumed
**             that ALL channels are allocated as a group, and similarly
**             de-allocated as a group. This is currently sufficient to
**             meet our requirements.
*****************************************************************************
*/
void boardHalIpcDestroyChannel( XDRV_IPC  *ipcDrv )
{
   unsigned int   chan;

   for( chan = 0; chan < MAX_IPC_CHANNELS; chan++ )
   {
      if ( ipcDrv == (XDRV_IPC*) &gIpcDrv[ chan ] )
      {
         /*
         ** Close IPC channel.
         */
         ipcSimClose( &gIpcDrv[ chan ] );
      }
   }

   /*
   ** Note that 'gChannelAllocCount' and 'gHeapAllocIndex' haven't been updated
   ** here because we don't really support de-allocation of individual IPC
   ** channels (see NOTE above). It is assumed that all channels will be
   ** de-allocated as a group. These variables will be cleared when
   ** boardHalIpcDeinit() is invoked.
   */
}


int boardHalIpcReady()
{
   return(0);
}

void boardHalIpcDump( BOARD_HAL_IPC_COM_HANDLE comHandle, int mode )
{
   (void) mode;
}
