/****************************************************************************
*
* Copyright © 2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: boardHalIpc6828.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent IPC interface for
*     the 6828.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <boardHalIpc.h>
#include <xdrvIpc.h>
#include <ipcSharedMemCfg.h>
#include <ipcSharedMem.h>
#include <vrgLog.h>
#include <debug.h>
#include <boardHal6828.h>
#include <handshake.h>
#include <sharedMemCommon.h>
#include <bosSleep.h>
#include <pcm6828.h>
#include <map6828.h>
#include <boardparms_voice.h>
#include <boardHalProv.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/*
** UNFINISHED: How big should the IPC buffers be?
*/
#define DSP_TO_HOST_SIZE_WORDS   4096
#define HOST_TO_DSP_SIZE_WORDS   4096

/* Number of IPC channels to allocate. (GW + Conf + Line + PSTN + RM) */
#undef IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS
#define IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS (GLOBAL_CFG_MAX_CNX + GLOBAL_CFG_MAX_ENDPT + 2)

/*
** IPC channel.
*/
typedef struct BOARD_HAL_IPC_CHAN
{
   VRG_BOOL                      inUse;
   VRG_BOOL                      hasBeenUsed;
   BOARD_HAL_IPC_CHANNEL_TYPE    chanType;
   XDRV_IPC                     *ipcDrv;
   BOARD_HAL_IPC_COM_HANDLE      comHandle;

} BOARD_HAL_IPC_CHAN;


typedef struct BOARD_HAL_IPC_DUPLEX_CHANNEL
{
   volatile XDRV_UINT16    dspToHostBuf[ DSP_TO_HOST_SIZE_WORDS ];
   volatile XDRV_UINT16    hostToDspBuf[ HOST_TO_DSP_SIZE_WORDS ];
} BOARD_HAL_IPC_DUPLEX_CHANNEL;

typedef struct BOARD_HAL_IPC_SHARED_MEM
{
   volatile BOARD_HAL_IPC_INIT_TABLE   initInfo;
   BOARD_HAL_IPC_DUPLEX_CHANNEL       *chan[ IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS ];

} BOARD_HAL_IPC_SHARED_MEM;


/* ---- Private Variables ------------------------------------------------ */

/*
** IPC driver instance memory.
*/
static BOARD_HAL_IPC_CHAN  gIpcChan[ IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS ];

/*
** Shared IPC memory.
*/
static BOARD_HAL_IPC_SHARED_MEM* gIpcSharedMem;


/* RESMAN channel (for reset) */
static XDRV_IPC *gResManIpcDrv;


/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalIpcInit
**
** PURPOSE:    Initialize the IPC
**
** PARAMETERS: pIpcMem           - pointer to IPC memory
**             ipcHeapsizeWords  - heap words available
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalIpcInit
(
   int notUsed1,
   volatile VRG_UINT16 *notUsed2,
   unsigned int         notUsed3
)
{
   int i;
   
   /* Initialize driver instance memory */
   memset( gIpcChan, 0, sizeof( gIpcChan ) );

   /* Allocate memory for the IPC shared memory structure */
   gIpcSharedMem = malloc( sizeof(BOARD_HAL_IPC_SHARED_MEM) );
   if ( gIpcSharedMem == NULL )
   {
      VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "Could NOT allocate memory for gIpcSharedMem !!!"));
      return ( -1 );
   }

   /* Allocate memory for the IPC channels */
   for(i = 0; i < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; i++)
   {
      gIpcSharedMem->chan[i] = malloc( sizeof(BOARD_HAL_IPC_DUPLEX_CHANNEL) );
      if ( gIpcSharedMem->chan[i] == NULL )
      {
         VRG_LOG_CRIT(( VRG_LOG_MOD_BOARDHAL, "Could NOT allocate memory for IPC channel %d!", i));
 
         /* Free previouly allocated memory before quitting */
         while( --i >= 0 )
         {
            free( gIpcSharedMem->chan[i] );
         }
         free( gIpcSharedMem );          
 
         return ( -1 );
      }
   }

   /*
   ** Initialize the shared memory IPC module.
   */
   ipcSharedMemInit();


   gIpcSharedMem->initInfo.newDataFlag = BOARD_HAL_IPC_DATA_READ;

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
int boardHalIpcDeinit( int notUsed )
{
   int i;

   /* HACK - because hausware does not properly support deinit, we need
   **        to explictly shut down the RESMAN channel here in order for
   **        come out of a reset successfully */
   boardHalIpcDestroyChannel( gResManIpcDrv );

   /*
   ** De-initialize the IPC module....hmmm, there doesn't appear to be a
   ** de-init function for ipcSharedMem.
   */
   for(i = 0; i < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; i++)
   {
      if ( gIpcSharedMem->chan[i] )
      {
         free( gIpcSharedMem->chan[i] );
      }
   }

   /* Deallocate shared memory */
   free( gIpcSharedMem );

   /*
   ** Clear driver instance memory
   */
   memset( gIpcChan, 0, sizeof( gIpcChan ) );

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
**             pIpcInfo    (out) pointer to IPC communication table
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
   VRG_UINT16                          dspToHostSizeWords = 0;
   VRG_UINT16                          hostToDspSizeWords = 0;
   unsigned int                        chanNum;
   volatile BOARD_HAL_IPC_INIT_TABLE  *dspInitInfo;

   dspInitInfo = &gIpcSharedMem->initInfo;

   /* The 6828 has only one DSP. */
   XCHG_ASSERT( deviceNum == 0 );


   /*
   ** Get channel sizes based upon channel type.
   */
   switch ( chanType )
   {
      case BOARD_HAL_IPC_MASTER_CONTROL_CHANNEL:   /* Fall through... */
      case BOARD_HAL_IPC_CONTROL_CHANNEL:
      case BOARD_HAL_IPC_DATA_CHANNEL:
      {
         dspToHostSizeWords = DSP_TO_HOST_SIZE_WORDS;
         hostToDspSizeWords = HOST_TO_DSP_SIZE_WORDS;
      }
      break;

      default:
      {
         XCHG_ASSERT( 0 );
      }
      break;
   }

   /*
   ** Take special action for the first Ipc channel create (i.e., the RESMAN
   ** channel).  We use a few words in shared memory to communicate to the
   ** DSP the parameters of this channel.  Subsequent opens of new Ipc channels
   ** will be communicated throught this ipc channel.
   */
   if( chanType == BOARD_HAL_IPC_MASTER_CONTROL_CHANNEL )
   {
      XCHG_ASSERT_COND( dspInitInfo->newDataFlag, == ,BOARD_HAL_IPC_DATA_READ );

      dspInitInfo->ipcChanId     = 0;
      dspInitInfo->dspToHostSize = dspToHostSizeWords;
      dspInitInfo->hostToDspSize = hostToDspSizeWords;
      dspInitInfo->newDataFlag   = BOARD_HAL_IPC_DATA_NEW;
   }


   for ( chanNum = 0; chanNum < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; chanNum++ )
   {
      /*
      ** In order to make management of the shared memory area easy, once
      ** a block of memory has been set aside for a certain type of vhd we do
      ** not allow it to be reused by another type.
      **
      ** If the location has never been used OR
      **     it has been used, is not currently in use, and was use for the same type
      ** Then try and open an ipc channel with this memory
      */
      if ( ( gIpcChan[chanNum].hasBeenUsed == 0) ||
           ( (gIpcChan[chanNum].inUse == 0 ) && ( gIpcChan[chanNum].chanType == chanType ) ) )
      {
         XDRV_IPC   *pIpcXdrv;

         /*
         ** Open the channel, and put the necessary IPC channel info into
         ** the shared memory block.
         */
         pIpcXdrv = (XDRV_IPC *) ipcSharedMemOpenMaster( (XDRV_UINT16*)gIpcSharedMem->chan[chanNum],
                                                         dspToHostSizeWords,
                                                         hostToDspSizeWords );
         if ( pIpcXdrv == NULL )
         {
            /* Failed to open IPC channel */
            XCHG_ASSERT( 0 );
         }
         else
         {
            /* Save IPC info for hausware */
            pIpcInfo->dspToHostSize = dspToHostSizeWords;
            pIpcInfo->hostToDspSize = hostToDspSizeWords;
            pIpcInfo->ipcChanId     = chanNum;

            gIpcChan[chanNum].inUse       = XDRV_TRUE;
            gIpcChan[chanNum].hasBeenUsed = XDRV_TRUE;
            gIpcChan[chanNum].chanType    = chanType;
            gIpcChan[chanNum].ipcDrv      = pIpcXdrv;
            gIpcChan[chanNum].comHandle   = comHandle;

            /* HACK - save the RESMAN IPC handle so that we can close it upon reset */
            if ( chanType == BOARD_HAL_IPC_MASTER_CONTROL_CHANNEL )
            {
               gResManIpcDrv = pIpcXdrv;
            }

            /*
            ** Register this channel with Debug module
            */
            debugIpcRegisterChannel(pIpcXdrv, comHandle);
         }

         return ( pIpcXdrv );
      }
   }


   XCHG_ASSERT( 0 );
   return ( NULL );
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
** NOTE:
*****************************************************************************
*/
void boardHalIpcDestroyChannel( XDRV_IPC  *ipcDrv )
{
   unsigned int                     chanNum;

   for( chanNum = 0; chanNum < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; chanNum++ )
   {
      if (( ipcDrv == gIpcChan[ chanNum ].ipcDrv ) &&
          ( gIpcChan[ chanNum ].inUse ) )
      {
         gIpcChan[ chanNum ].inUse        = VRG_FALSE;
         gIpcChan[ chanNum ].hasBeenUsed  = VRG_FALSE;
         gIpcChan[ chanNum ].chanType     = 0 ;

         /*
         ** Close IPC channel.
         */
         ipcSharedMemCloseMaster( (IPCSHAREDMEM_DRV *) gIpcChan[ chanNum ].ipcDrv );

         gIpcChan[ chanNum ].ipcDrv = NULL;
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   boardHalIpcReady
**
** PURPOSE:    To indicate to the DSP that the necessary IPC initialization
**             information is available
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else error code.
**
** NOTE:
*****************************************************************************
*/
int boardHalIpcReady( int notUsed )
{
   SHARED_MEM_DESCRIPTOR               sharedMemDescr;
   VRG_BOOL                            threadProcessorInited;
   unsigned int                        i;

   /*
   ** Fill in the shared memory descriptor, that contains regions of memory
   ** used to communicate between the MIPS thread-processors...and then
   ** fire it off...
   */
   sharedMemDescr.ipcSharedMem         = gIpcSharedMem;
   sharedMemDescr.itpcSharedMemData    = gHalItpcSharedMemData;
   sharedMemDescr.spinLock             = gHalSpinLock;
   sharedMemDescr.dspStackDump         = gHalDspStackDump;
   sharedMemDescr.halShimEntryCount    = gHalshimEntryCount;   
   sharedMemDescr.halShimConfig        = (void*)gHalshimConfig;
   sharedMemDescr.dmaBufferSize        = PCM6828_DMA_MAX_BUFFER_SIZE;
   sharedMemDescr.bcmChip              = BCM96828;
   sharedMemDescr.apmModeEnabled       = ( boardHalProvGetFlags() & BP_FLAG_DSP_APMHAL_ENABLE ) ? XDRV_TRUE : XDRV_FALSE;

   handshakeSendMsg( (int) &sharedMemDescr );


   /*
   ** Wait for an ACK from the DSP thread-processor.
   */
   threadProcessorInited = XDRV_FALSE;
   for ( i = 0; i < 50; i++ )
   {
      int   msg;

      handshakeGetMsg( &msg );

      if ( msg == HANDSHAKE_THREAD_PROCESSOR_APP_INIT_DONE )
      {
         threadProcessorInited = XDRV_TRUE;
         break;
      }

      bosSleep( 10 );
   }


   if ( !threadProcessorInited )
   {
      return ( -1 );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalIpcDump
**
** PURPOSE:    Dump IPC info for all active channels
**
** PARAMETERS: comHandle   (in)  Communication handle that can be used to
**                               uniquely identify the IPC channel on both
**                               processors.
**             mode        (IN)  IPC dump mode (full dump or stats only)
**
** RETURNS:    nothing
**
** NOTE:      comHandle equal -1 means print all channels in use
*****************************************************************************
*/
void boardHalIpcDump( BOARD_HAL_IPC_COM_HANDLE comHandle, int mode )
{
   int   i;

   /* Find the matching IPC driver or print all channels in use (comHandle = -1) */
   for ( i = 0; i < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; i++ )
   {
      if (( gIpcChan[i].inUse == XDRV_TRUE ) &&
          ((gIpcChan[i].comHandle == comHandle) || (comHandle == -1)))
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "********************************************************"));
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Dumping IPC info for: channel %d; handle 0x%x; type %d", i, gIpcChan[i].comHandle, gIpcChan[i].chanType));
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "********************************************************"));

         xdrvIpcPrint( gIpcChan[i].ipcDrv, mode );
      }
   }
}
