/****************************************************************************
*
* Copyright © 2002-2008 Broadcom Corporation
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
****************************************************************************
* Description:
*   This file implements the API functions to access the inter-processer
*   communication between the host and DSP.
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <xdrvTypes.h>
#include <xchgAssert.h>
#include <stddef.h>
#include <ipcSharedMemCfg.h>
#include <ipcSharedMem.h>
#include "ipcSharedMemPriv.h"

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
#include <xdrvGlobalInterrupt.h>
#endif

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
#include <flowCtrl.h>
#endif

/* ---- Private Constants and Types -------------------------------------- */

#define IPC_HEADER_SIZE           1

/*
** For each IPC shared memory channel there is a 16-bit read and write descriptor
** that is stored in shared memory. Each descriptor consists of a "wrap counter"
** and an "index counter". The index counter is used to index the circular
** buffer, indicating the next location to read from or write to. The wrap
** counter is incremented each time the index counter wraps the bottom of the
** circular buffer.
**
** The wrap counter is used to distinguish between a full and empty buffer. If
** the index and wrap counter values of the read and write descriptor are equal,
** the buffer is empty. If the index counters are equal, but the wrap counters
** are not, then the buffer is full. Else, the buffer is partially full.
**
** Currently, the least significant 15 bits are used for the index value, and
** most significant bit is used for the wrap value.
**
** It is currently assumed that shared memory access only provides 16-bit
** atomic reads/writes. Therefore, the combined index/wrap value must be contained
** within a single 16-bit value. Storing the index and wrap values in separate
** 16-bits words will likely result in a race condition.
*/
#define INCREMENT_WRAP_COUNTER( counter )    ( (( counter ) ^ 0x8000 ) & 0x8000 )
#define INCREMENT_INDEX_COUNTER( counter )   ( ( counter ) + 1 )

#define GET_WRAP_COUNTER( counter )          ( ( counter ) & 0x8000 )
#define GET_INDEX_COUNTER( counter )         ( ( counter ) & 0x7fff )

/*
**  HPI circular buffer structure
*/
typedef struct
{
   volatile XDRV_UINT16 writeDescriptor;
   volatile XDRV_UINT16 readDescriptor;
   volatile XDRV_UINT16 buf[1];     /**< Buffer of Data */

} SHAREDMEM_CHANNEL;

typedef struct
{
   volatile SHAREDMEM_CHANNEL *pCb;

   /*
   ** Both of these indexes are not need, but the code reads better
   ** using the more descriptive names.
   */
   XDRV_UINT16    ucWriteDescriptor;

   XDRV_UINT16    ucReadDescriptor;

   XDRV_UINT16    bufferSize;

} IPCSHAREDMEM_SIMPLEX_DRV;

typedef struct IPCSHAREDMEM_STATS
{
   /* The total number of buffers read, but not freed. */
   unsigned int   totalReadNoFreeCount;

   /* The total number of times a group of buffers are freed. */
   unsigned int   totalReadFreeCount;

   /* Total number of buffers written. */
   unsigned int   totalWriteCount;

   /* The current number of buffers queued for flow control. */
   unsigned int   totalWriteOverflow;

   unsigned int   peakReadLevel;
   unsigned int   peakWriteLevel;
   unsigned int   largestReadMsg;
   unsigned int   largestWriteMsg;

} IPCSHAREDMEM_STATS;

/*
** HPI Handle structure to abstract source and destination buffers
*/
typedef struct
{
   XDRV_BOOL                     inUse;

   IPCSHAREDMEM_SIMPLEX_DRV      ipcRead;
   IPCSHAREDMEM_SIMPLEX_DRV      ipcWrite;

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
   FLOWCTRL                      flowCtrl;
#endif

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   IPCSHAREDMEM_STATS            stats;
#endif

} IPCSHAREDMEM_DUPLEX_DRV;


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
#if IPCSHAREDMEM_CFG_SLAVE_SUPPORT
static void ipclowlvlInitSlave( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                                volatile XDRV_UINT16 *pMem,
                                 unsigned totalMemSize );
#endif

#if IPCSHAREDMEM_CFG_MASTER_SUPPORT
static void ipclowlvlInitMaster( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                                 volatile XDRV_UINT16 *pMem,
                                 unsigned totalMemSize );
#endif

static int ipclowlvlWriteUc( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                             XDRV_UINT16 *pSrc1, unsigned length1,
                             XDRV_UINT16 *pSrc2, unsigned length2 );
static void ipclowlvlWriteCommit( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv );
static void ipclowlvlReadCommit( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv );
static int  ipclowlvlReadUc( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv, XDRV_UINT16 **ppBuf );
#if IPCSHAREDMEM_CFG_STATS_SUPPORT
static int ipclowlvlSpaceUsed( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv );
#endif

static unsigned ipcSharedMemBufferWrite
(
   XDRV_IPC *pIpcDrv,
   XDRV_UINT16 *dp1, unsigned len1,
   XDRV_UINT16 *dp2, unsigned len2
);

static unsigned ipcSharedMemReadWithoutFree( XDRV_IPC *pIpcDrv, XDRV_UINT16 **ppDestBuf );
static void ipcSharedMemFree( XDRV_IPC *pIpcDrv );

static void ipcSharedMemPrint( XDRV_IPC *pIpcXDrv, XDRV_IPC_PRINT_MODE mode );

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
static void PrintSharedChannelInfo( XDRV_IPC *pIpcXDrv, XDRV_IPC_PRINT_MODE mode );
#endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */

/* ---- Private Variables ------------------------------------------------ */
static IPCSHAREDMEM_DRV    IPC_gDrv[IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS];
static IPCSHAREDMEM_DUPLEX_DRV gDuplexDrv[IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS];

/* IPC "meta-driver" interface */
static const XDRV_IPC_FUNCS  ipcSharedMemDrvFuncs =
{
   ipcSharedMemReadWithoutFree,
   ipcSharedMemFree,
   ipcSharedMemBufferWrite,
   ipcSharedMemPrint
};


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   ipcSharedMemInit
**
** PURPOSE:    Initialize the IPC shared memory driver instance memory.
**
** PARAMETERS: none.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void ipcSharedMemInit( void )
{
   memset( IPC_gDrv,   0, sizeof( IPC_gDrv ) );
   memset( gDuplexDrv, 0, sizeof( gDuplexDrv ) );
}


#if IPCSHAREDMEM_CFG_SLAVE_SUPPORT
/***************************************************************************
*
***************************************************************************/
IPCSHAREDMEM_DRV *ipcSharedMemOpenSlave( volatile XDRV_UINT16 *pMem,
                            unsigned dspToHostMemSize,
                            unsigned hostToDspMemSize )
{
   volatile XDRV_UINT16      *pDspToHostMem = pMem;
   volatile XDRV_UINT16      *pHostToDspMem = pMem+dspToHostMemSize;
   IPCSHAREDMEM_DRV          *pNewDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *pNewDuplexDrv;
   unsigned int               chan;


   for ( chan = 0; chan < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; chan++ )
   {
      if ( gDuplexDrv[chan].inUse == XDRV_FALSE )
      {
         gDuplexDrv[chan].inUse = XDRV_TRUE;
         break;
      }
   }

   XCHG_ASSERT( chan < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS );


   pNewDrv       = &IPC_gDrv[chan];
   pNewDuplexDrv = &gDuplexDrv[chan];

   ipclowlvlInitSlave( &(pNewDuplexDrv->ipcRead),  pHostToDspMem, hostToDspMemSize );
   ipclowlvlInitSlave( &(pNewDuplexDrv->ipcWrite), pDspToHostMem, dspToHostMemSize );

   pNewDrv->pDrvFuncs = &ipcSharedMemDrvFuncs;
   pNewDrv->ipcHandle = pNewDuplexDrv;

   return( pNewDrv );
}


/******************************************************************************
*
******************************************************************************/
void ipcSharedMemCloseSlave( IPCSHAREDMEM_DRV *ipcDrv )
{
   IPCSHAREDMEM_DUPLEX_DRV   *ipcDuplexDrv;

   ipcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *) (ipcDrv->ipcHandle);

   memset( ipcDuplexDrv, 0, sizeof( IPCSHAREDMEM_DUPLEX_DRV ) );
}


/******************************************************************************
*
******************************************************************************/
static void ipclowlvlInitSlave( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                                volatile XDRV_UINT16 *pMem,
                         unsigned totalMemSize )
{
   unsigned bufferSize;

   bufferSize = totalMemSize-2;

   pIpcSimplexDrv->pCb = (volatile SHAREDMEM_CHANNEL *)pMem;
   pIpcSimplexDrv->ucWriteDescriptor      = 0;
   pIpcSimplexDrv->ucReadDescriptor       = 0;
   pIpcSimplexDrv->bufferSize          = bufferSize;
}
#endif


#if IPCSHAREDMEM_CFG_MASTER_SUPPORT
/***************************************************************************
*
***************************************************************************/
IPCSHAREDMEM_DRV *ipcSharedMemOpenMaster( volatile XDRV_UINT16 *pMem,
                             unsigned dspToHostMemSize,
                             unsigned hostToDspMemSize )
{
   volatile XDRV_UINT16      *pDspToHostMem = pMem;
   volatile XDRV_UINT16      *pHostToDspMem = pMem+dspToHostMemSize;
   IPCSHAREDMEM_DRV          *pNewDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *pNewDuplexDrv;
   unsigned int               chan;

   for ( chan = 0; chan < IPCSHAREDMEM_CFG_MAX_DUPLEX_CHANNELS; chan++ )
   {
      if ( gDuplexDrv[chan].inUse == XDRV_FALSE )
      {
         gDuplexDrv[chan].inUse = XDRV_TRUE;
         break;
      }
   }

   pNewDrv       = &IPC_gDrv[chan];
   pNewDuplexDrv = &gDuplexDrv[chan];

   ipclowlvlInitMaster( &(pNewDuplexDrv->ipcRead),  pDspToHostMem, dspToHostMemSize );
   ipclowlvlInitMaster( &(pNewDuplexDrv->ipcWrite), pHostToDspMem, hostToDspMemSize );

   pNewDrv->pDrvFuncs = &ipcSharedMemDrvFuncs;
   pNewDrv->ipcHandle = pNewDuplexDrv;


#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
   flowctrlInit( &pNewDuplexDrv->flowCtrl );
#endif

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   memset( &pNewDuplexDrv->stats, 0, sizeof( IPCSHAREDMEM_STATS ) );
#endif

   return( pNewDrv );
}

/******************************************************************************
*
******************************************************************************/
void ipcSharedMemCloseMaster( IPCSHAREDMEM_DRV *ipcDrv )
{
   IPCSHAREDMEM_DUPLEX_DRV   *ipcDuplexDrv;

   ipcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *) (ipcDrv->ipcHandle);

   memset( ipcDuplexDrv, 0, sizeof( IPCSHAREDMEM_DUPLEX_DRV ) );
}


/******************************************************************************
*
******************************************************************************/
static void ipclowlvlInitMaster( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                                 volatile XDRV_UINT16 *pMem,
                          unsigned totalMemSize )
{
   unsigned i;
   unsigned bufferSize;
   volatile SHAREDMEM_CHANNEL *pCircBuf;

   bufferSize = totalMemSize-2;

   pCircBuf = (volatile SHAREDMEM_CHANNEL *)pMem;
   pCircBuf->writeDescriptor  = 0;
   pCircBuf->readDescriptor   = 0;

   for( i=0; i<bufferSize; i++ )
   {
      pCircBuf->buf[i] = 0xAB00;
   }

   pIpcSimplexDrv->pCb = pCircBuf;
   pIpcSimplexDrv->ucWriteDescriptor      = 0;
   pIpcSimplexDrv->ucReadDescriptor       = 0;
   pIpcSimplexDrv->bufferSize          = bufferSize;

}
#endif

/******************************************************************************
*
******************************************************************************/
static unsigned ipcSharedMemReadWithoutFree( XDRV_IPC *pIpcXDrv, XDRV_UINT16 **ppDestBuf )
{
   IPCSHAREDMEM_DRV          *pIpcDrv = (IPCSHAREDMEM_DRV *)pIpcXDrv;
   IPCSHAREDMEM_SIMPLEX_DRV  *pIpcSimplexDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *pIpcDuplexDrv;
   unsigned                   numAvailable;

   pIpcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   pIpcSimplexDrv = &(pIpcDuplexDrv->ipcRead);
   XCHG_ASSERT( pIpcSimplexDrv != NULL );

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
   /*
   ** Service buffers that are queued in the flow-control pool.
   */
   ipcSharedMemProcess( pIpcDrv );
#endif   /* IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */


#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   /*
   ** Get the buffer levels before doing a read
   */
   numAvailable = ipclowlvlSpaceUsed( pIpcSimplexDrv );
   if( numAvailable > pIpcDuplexDrv->stats.peakReadLevel )
   {
      pIpcDuplexDrv->stats.peakReadLevel = numAvailable;
   }
#endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */

   /*
   ** If there is data available to read in the shared memory
   ** Then get a pointer to the start of the message.
   **
   ** We only write complete message to shared memory, so if there is any
   ** data available then it must be an entire message.
   */
   numAvailable = ipclowlvlReadUc( pIpcSimplexDrv, (XDRV_UINT16 **) ppDestBuf );

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   /*
   ** Update stats.
   */
   if( numAvailable > 0 )
   {
      pIpcDuplexDrv->stats.totalReadNoFreeCount++;

      if( numAvailable > pIpcDuplexDrv->stats.largestReadMsg )
      {
         pIpcDuplexDrv->stats.largestReadMsg = numAvailable;
      }

   }
#endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */


   return( numAvailable );
}


/******************************************************************************
*
******************************************************************************/
static void ipcSharedMemFree( XDRV_IPC *pIpcXDrv )
{
   IPCSHAREDMEM_DRV          *pIpcDrv = (IPCSHAREDMEM_DRV *)pIpcXDrv;
   IPCSHAREDMEM_SIMPLEX_DRV  *pIpcSimplexDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *pIpcDuplexDrv;

   pIpcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   pIpcSimplexDrv = &(pIpcDuplexDrv->ipcRead);
   XCHG_ASSERT( pIpcSimplexDrv != NULL );

   ipclowlvlReadCommit( pIpcSimplexDrv );

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   /*
   ** Update stats.
   */
   pIpcDuplexDrv->stats.totalReadFreeCount++;
#endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */
}


/******************************************************************************
* FUNCTION:   ipcSharedMemBufferWrite
*
******************************************************************************/
static unsigned ipcSharedMemBufferWrite
(
   XDRV_IPC *pIpcXDrv,
   XDRV_UINT16 *pMsg1, unsigned msgLength1,
   XDRV_UINT16 *pMsg2, unsigned msgLength2
)
{
   IPCSHAREDMEM_DRV          *pIpcDrv = (IPCSHAREDMEM_DRV *)pIpcXDrv;
   IPCSHAREDMEM_SIMPLEX_DRV  *pIpcSimplexDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *pIpcDuplexDrv;

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
   FLOWCTRL *pFlowCtrl;
#endif

   pIpcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   pIpcSimplexDrv = &(pIpcDuplexDrv->ipcWrite);
   XCHG_ASSERT( pIpcSimplexDrv != NULL );

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
   if( (msgLength1+msgLength2) > pIpcDuplexDrv->stats.largestWriteMsg )
   {
         pIpcDuplexDrv->stats.largestWriteMsg = (msgLength1+msgLength2);
   }
#endif

#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
   pFlowCtrl = &(((IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle))->flowCtrl);

   /*
   ** If there is data in the flow control buffer
   ** then attempt to empty this data first
   */
   ipcSharedMemProcess( pIpcDrv );


   {
      int retCode;
      XDRV_GLOBAL_INTERRUPT_LOCK lockKey;
 
      /* Lock critical section */
      xdrvGlobalInterruptDisable( &lockKey );
      retCode = flowctrlIsEmpty(pFlowCtrl);
      xdrvGlobalInterruptRestore( &lockKey );

      if ( !retCode )
      {
         /*
         ** The flow-control buffer pool is not empty. This means that the physical
         ** medium between processors is full. Simply enqueue this buffer and return.
         */
         xdrvGlobalInterruptDisable( &lockKey );
         retCode = flowctrlEnqueue( pFlowCtrl, pMsg1, msgLength1, pMsg2, msgLength2 );
         xdrvGlobalInterruptRestore( &lockKey );

         if( retCode )
         {
            return ( 0 );
         }
 
         return( msgLength1+msgLength2 );
      }
   }

#endif   /* IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */


   /*
   ** If we reach here, either flow control is disabled or we have emptied
   ** the flow control buffer.
   ** Try and write the new message to the ipc channel.
   */
   if ( ipclowlvlWriteUc( pIpcSimplexDrv, pMsg1, msgLength1, pMsg2, msgLength2 ) )
   {
      ipclowlvlWriteCommit( pIpcSimplexDrv );


#if IPCSHAREDMEM_CFG_STATS_SUPPORT
      {
         unsigned  numAvailable;

         /*
         ** Get the buffer levels before doing a read
         */
         numAvailable = ipclowlvlSpaceUsed( pIpcSimplexDrv );
         if( numAvailable > pIpcDuplexDrv->stats.peakWriteLevel )
         {
            pIpcDuplexDrv->stats.peakWriteLevel = numAvailable;
         }
      }

      /*
      ** Update stats.
      */
      pIpcDuplexDrv->stats.totalWriteCount++;
#endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */

   }
   else
   {
#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT
      int retCode;
      XDRV_GLOBAL_INTERRUPT_LOCK lockKey;
 
      /* Lock critical section */
      xdrvGlobalInterruptDisable( &lockKey );
      retCode = flowctrlEnqueue( pFlowCtrl, pMsg1, msgLength1, pMsg2, msgLength2 );
      xdrvGlobalInterruptRestore( &lockKey );

      if( retCode == -1 )
      {
         return( 0 );
      }

#else    /* IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */

   #if IPCSHAREDMEM_CFG_STATS_SUPPORT

         /*
         ** Update stats.
         */
         pIpcDuplexDrv->stats.totalWriteOverflow++;

   #endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */

      return( 0 );

#endif   /* !IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */
   }

   return( msgLength1+msgLength2 );
}


/******************************************************************************
*
******************************************************************************/
void ipcSharedMemProcess( IPCSHAREDMEM_DRV *pIpcDrv )
{
#if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT

   IPCSHAREDMEM_DUPLEX_DRV  *pIpcDuplexDrv;
   IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv;
   FLOWCTRL *pFlowCtrl;
   XDRV_GLOBAL_INTERRUPT_LOCK lockKey;

   pIpcDuplexDrv  = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   pIpcSimplexDrv = &(pIpcDuplexDrv->ipcWrite);
   XCHG_ASSERT( pIpcSimplexDrv != NULL );

   pFlowCtrl = &(((IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle))->flowCtrl);
   XCHG_ASSERT( pFlowCtrl != NULL );

   xdrvGlobalInterruptDisable( &lockKey );
   while( !flowctrlIsEmpty( pFlowCtrl ) )
   {
      unsigned       msgLen;
      XDRV_UINT16   *pMsg;

      /*
      ** We are here because there is data in the flow control.  Queued data
      ** takes precidence over new data.
      ** If the next msg in the flow control buffer will fit into the ipc buffer
      ** Then write the message.
      ** Else we can not write to the ipc so just enqueue the new data into the
      **      flow control buffer.
      */
      msgLen = flowctrlGetMsgLength( pFlowCtrl );
      pMsg   = flowctrlGetMsgPointer( pFlowCtrl );

      if ( ipclowlvlWriteUc( pIpcSimplexDrv, pMsg, msgLen, NULL, 0 ) )
      {
         flowctrlFreeMsg( pFlowCtrl );
         ipclowlvlWriteCommit( pIpcSimplexDrv );

   #if IPCSHAREDMEM_CFG_STATS_SUPPORT

         /*
         ** Update stats.
         */
         pIpcDuplexDrv->stats.totalWriteCount++;

   #endif   /* IPCSHAREDMEM_CFG_STATS_SUPPORT */

      }
      else
      {
         /*
         ** No more room in the IPC memory channel. Let's bail...
         */
         break;
      }
   }

   xdrvGlobalInterruptRestore( &lockKey );

#endif   /* IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */

   /* Avoid compiler warning for unused variable if flow-ctrl support is disabled. */
   (void) pIpcDrv;
}


/******************************************************************************
* Description:
*
* Parameters:
*
* Returns:
*
* Do not call this function unless you know your message will fit
******************************************************************************/
static int ipclowlvlWriteUc( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv,
                             XDRV_UINT16 *pSrc1, unsigned length1,
                             XDRV_UINT16 *pSrc2, unsigned length2 )
{

   unsigned totalFrameLength;
   unsigned i;
   unsigned spaceAvailToBufEnd;
   unsigned spaceAvailAtBufStart;
   int      contigSpaceAvail;


   XDRV_UINT16 ucWritei = pIpcSimplexDrv->ucWriteDescriptor;
   XDRV_UINT16 readi    = pIpcSimplexDrv->pCb->readDescriptor;

   /*
   ** The wrap counter is used to distinguish between a full and empty buffer. If
   ** the index and wrap counter values of the read and write descriptor are equal,
   ** the buffer is empty. If the index counters are equal, but the wrap counters
   ** are not, then the buffer is full. Else, the buffer is partially full.
   */
   if ( GET_INDEX_COUNTER( ucWritei ) == GET_INDEX_COUNTER( readi ) )
   {
      if ( GET_WRAP_COUNTER( ucWritei ) != GET_WRAP_COUNTER( readi ) )
      {
         /* No room in circular buffer to write. */
         return ( 0 );
      }
   }


   /*
   ** Length of message with header.
   */
   totalFrameLength = (length1 + length2) + IPC_HEADER_SIZE;
   XCHG_ASSERT( totalFrameLength < pIpcSimplexDrv->bufferSize );


   spaceAvailToBufEnd = pIpcSimplexDrv->bufferSize - GET_INDEX_COUNTER( ucWritei );

   /*
   ** Calculate the amount of contiguous space in the cicular buffer.
   ** If totalSpace is negative
   ** Then the available space is wrapped around the buffer end, find the
   **      largest contiguous block.
   ** Else the available space is in 1 big block, continue with function.
   */
   contigSpaceAvail = GET_INDEX_COUNTER( readi ) - GET_INDEX_COUNTER( ucWritei );
   if( contigSpaceAvail <= 0 )
   {
      spaceAvailAtBufStart = GET_INDEX_COUNTER( readi );
      if( spaceAvailToBufEnd >= spaceAvailAtBufStart )
      {
         contigSpaceAvail = spaceAvailToBufEnd;
      }
      else
      {
         contigSpaceAvail = spaceAvailAtBufStart;
      }
   }

   if( contigSpaceAvail < (int)totalFrameLength )
   {
      return( 0 );
   }

   /*
   ** If we reach here then we know that the message can fit in the buffer.
   */

   /*
   ** If the message is going to wrap the circular buffer
   ** Then add a padding frame
   ** Else just go ahead an write the message
   */
   if( spaceAvailToBufEnd < totalFrameLength )
   {
      pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucWritei ) ] = ((spaceAvailToBufEnd - 1) | 0x8000);

      ucWritei  = INCREMENT_WRAP_COUNTER( ucWritei );
   }

   /*
   ** Write the header (1 word indicating the length of the message)
   */
   pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucWritei ) ] = (length1+length2);
   ucWritei = INCREMENT_INDEX_COUNTER( ucWritei );


   /*
   ** Write the body of the message, given to us in two buffers.
   **
   ** memcpy() is not used because some shared memory interfaces
   ** require 16-bit accesses.
   */
   for( i=0; i < length1; i++ )
   {
      pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucWritei ) ] = *pSrc1++;
      ucWritei = INCREMENT_INDEX_COUNTER( ucWritei );
   }

   for( i=0; i < length2; i++ )
   {
      pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucWritei )] = *pSrc2++;
      ucWritei = INCREMENT_INDEX_COUNTER( ucWritei );
   }


   if ( GET_INDEX_COUNTER( ucWritei ) == pIpcSimplexDrv->bufferSize )
   {
      ucWritei = INCREMENT_WRAP_COUNTER( ucWritei );
   }

   XCHG_ASSERT( GET_INDEX_COUNTER( ucWritei ) < pIpcSimplexDrv->bufferSize );

   pIpcSimplexDrv->ucWriteDescriptor = ucWritei;

   return( length1+length2 );
}

/******************************************************************************
*
******************************************************************************/
static void ipclowlvlWriteCommit( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv )
{
   XCHG_ASSERT( GET_INDEX_COUNTER( pIpcSimplexDrv->ucWriteDescriptor ) < pIpcSimplexDrv->bufferSize );

   pIpcSimplexDrv->pCb->writeDescriptor = pIpcSimplexDrv->ucWriteDescriptor;

}


/******************************************************************************
* Function:  ipclowlvlReadUc
*
* Description:
*   Uncommitted read of the ipc interface.
*   The function previews the data it will read from the interface.
*   The user does not really read the data until they "commit" the read by
*   calling the function ipclowlvlReadCommit.
*
* Parameters:
*   pIpcSimplexDrv (in)  - pointer to the ipc driver structure
*   pDest  (in)  - pointer to buffer where the data will be put,
*   length (in)  - number of 16 bit words to read
*
* Returns:
*   Number of 16 bit words read
******************************************************************************/
static int ipclowlvlReadUc( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv, XDRV_UINT16 **ppBuf )
{
   unsigned    msgLength      = 0;

   XDRV_BOOL   isBufferEmpty  = XDRV_FALSE;

   XDRV_UINT16 ucReadi  = pIpcSimplexDrv->ucReadDescriptor;
   XDRV_UINT16 writei   = pIpcSimplexDrv->pCb->writeDescriptor;


   /*
   ** The wrap counter is used to distinguish between a full and empty buffer. If
   ** the index and wrap counter values of the read and write descriptor are equal,
   ** the buffer is empty. If the index counters are equal, but the wrap counters
   ** are not, then the buffer is full. Else, the buffer is partially full.
   */
   if ( ( GET_WRAP_COUNTER( ucReadi ) == GET_WRAP_COUNTER( writei ) ) &&
        ( GET_INDEX_COUNTER( ucReadi ) == GET_INDEX_COUNTER( writei )) )
   {
      isBufferEmpty = XDRV_TRUE;
   }


   if ( !isBufferEmpty )
   {
      /* This is a hack to get around the compiler warning of
       * dropping the volatile keyword */
      union
      {
         volatile XDRV_UINT16  *pBufVol;
         XDRV_UINT16  *pBufNonVol;

      } Hack;



      /*
      ** If this is a padding frame
      ** Then advance to start of buffer
      **
      ** We assume there will always be a real message written if there is a
      ** padding frame.
      */
      if( pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucReadi ) ] & 0x8000 )
      {
         ucReadi = INCREMENT_WRAP_COUNTER( ucReadi );
      }

      msgLength = pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucReadi ) ];
      ucReadi   = INCREMENT_INDEX_COUNTER( ucReadi );

      Hack.pBufVol = &(pIpcSimplexDrv->pCb->buf[ GET_INDEX_COUNTER( ucReadi ) ]);
      *ppBuf = Hack.pBufNonVol;

      ucReadi = ucReadi + msgLength;


      /*
      ** The message may have ended exactly at the end of the buffer.
      */
      if( GET_INDEX_COUNTER( ucReadi ) == pIpcSimplexDrv->bufferSize )
      {
         ucReadi = INCREMENT_WRAP_COUNTER( ucReadi );
      }


      XCHG_ASSERT( GET_INDEX_COUNTER( ucReadi ) < pIpcSimplexDrv->bufferSize );
      XCHG_ASSERT( msgLength < pIpcSimplexDrv->bufferSize );

      pIpcSimplexDrv->ucReadDescriptor = ucReadi;

   }

   return( msgLength );
}


#if IPCSHAREDMEM_CFG_STATS_SUPPORT
/******************************************************************************
* Return the number of words separating the read and write pointer
******************************************************************************/
static int ipclowlvlSpaceUsed( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv )
{
   XDRV_BOOL   isBufferEmpty  = XDRV_FALSE;

   XDRV_UINT16 writei   = pIpcSimplexDrv->pCb->writeDescriptor;
   XDRV_UINT16 readi    = pIpcSimplexDrv->pCb->readDescriptor;

   /*
   ** The wrap counter is used to distinguish between a full and empty buffer. If
   ** the index and wrap counter values of the read and write descriptor are equal,
   ** the buffer is empty. If the index counters are equal, but the wrap counters
   ** are not, then the buffer is full. Else, the buffer is partially full.
   */
   if ( ( GET_WRAP_COUNTER( readi ) == GET_WRAP_COUNTER( writei ) ) &&
        ( GET_INDEX_COUNTER( readi ) == GET_INDEX_COUNTER( writei )) )
   {
      isBufferEmpty = XDRV_TRUE;
      (void)isBufferEmpty;
   }
   else
   {
      int temp;

      temp = GET_INDEX_COUNTER(writei) - GET_INDEX_COUNTER(readi);
      if( temp == 0 )
      {
         return( pIpcSimplexDrv->bufferSize );
      }
      else if( temp < 0 )
      {
         return( temp + pIpcSimplexDrv->bufferSize );
      }

      return( temp );
 
   }

   return( 0 );
}
#endif

/******************************************************************************
*
******************************************************************************/
static void ipclowlvlReadCommit( IPCSHAREDMEM_SIMPLEX_DRV *pIpcSimplexDrv )
{
   XCHG_ASSERT( GET_INDEX_COUNTER( pIpcSimplexDrv->ucReadDescriptor ) < pIpcSimplexDrv->bufferSize );

   pIpcSimplexDrv->pCb->readDescriptor = pIpcSimplexDrv->ucReadDescriptor;
}

/*
*****************************************************************************
** FUNCTION:   ipcSharedMemPrint
**
** PURPOSE:    Print info/stats related to the shared IPC channel.
**
** PARAMETERS: pIpcXDrv  (mod) IPC driver.
**             mode      IPC print mode
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void ipcSharedMemPrint( XDRV_IPC *pIpcXDrv, XDRV_IPC_PRINT_MODE mode )
{

#if IPCSHAREDMEM_CFG_STATS_SUPPORT

   IPCSHAREDMEM_DRV          *pIpcDrv = (IPCSHAREDMEM_DRV *)pIpcXDrv;
   IPCSHAREDMEM_STATS        *stats;
   IPCSHAREDMEM_DUPLEX_DRV   *duplexDrv;

   duplexDrv = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   stats     = &(duplexDrv->stats);


   /*
   ** Dump shared channel config info.
   */
   PrintSharedChannelInfo( pIpcXDrv, mode );


   IPC_LOG(( "\nIPC shared memory stats (%p)\n\n", stats ));
   IPC_LOG(( "Read buf (no free) count:   %d\n", stats->totalReadNoFreeCount ));
   IPC_LOG(( "Write buf count:            %d\n", stats->totalWriteCount ));
   IPC_LOG(( "Free read bufs count:       %d\n", stats->totalReadFreeCount ));
   IPC_LOG(( "Peak Read Level:            %d\n", stats->peakReadLevel ));
   IPC_LOG(( "Peak Write Level:           %d\n", stats->peakWriteLevel ));
   IPC_LOG(( "Largest Read Msg:           %d\n", stats->largestReadMsg ));
   IPC_LOG(( "Largest Write Msg:          %d\n", stats->largestWriteMsg ));

   #if !IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT

   IPC_LOG(( "Total write overflow count: %d\n", stats->totalWriteOverflow ));

   #endif   /* !IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */


   /* Clear stats each time it is queried. */
   memset( stats, 0, sizeof( IPCSHAREDMEM_STATS ) );


   #if IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT

   flowctrlPrintStats( &(duplexDrv->flowCtrl) );

   #endif   /* IPCSHAREDMEM_CFG_FLOWCTRL_SUPPORT */

#endif   /* !IPCSHAREDMEM_CFG_STATS_SUPPORT */


   /* Avoid compiler warning for unused variable if stats support is disabled. */
   (void) pIpcXDrv;
   (void) mode;
}

#if IPCSHAREDMEM_CFG_STATS_SUPPORT
/*
*****************************************************************************
** FUNCTION:   PrintSharedChannelInfo
**
** PURPOSE:    Print shared channel configuration info.
**
** PARAMETERS: pIpcXDrv (mod) IPC driver.
**             mode     IPC print mode
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void PrintSharedChannelInfo( XDRV_IPC *pIpcXDrv, XDRV_IPC_PRINT_MODE mode )
{
   IPCSHAREDMEM_DRV          *pIpcDrv = (IPCSHAREDMEM_DRV *)pIpcXDrv;
   IPCSHAREDMEM_DUPLEX_DRV   *duplexDrv;
   IPCSHAREDMEM_SIMPLEX_DRV  *readDrv;
   IPCSHAREDMEM_SIMPLEX_DRV  *writeDrv;

   duplexDrv = (IPCSHAREDMEM_DUPLEX_DRV *)(pIpcDrv->ipcHandle);
   readDrv   = &(duplexDrv->ipcRead);
   writeDrv  = &(duplexDrv->ipcWrite);

   switch (mode)
   {
      case XDRV_IPC_PRINT_MODE_INFO:
      {
         IPC_LOG(( "\nIPC shared memory channel info (%p)\n\n", duplexDrv ));
         IPC_LOG(( "Read  (DSP-->host) buf - size: %d, addr: %p\n", readDrv->bufferSize, readDrv->pCb ));
         IPC_LOG(( "Write (host-->DSP) buf - size: %d, addr: %p\n", writeDrv->bufferSize, writeDrv->pCb ));
      }
      break;
 
      case XDRV_IPC_PRINT_MODE_FULL_DUMP:
      {
         XDRV_UINT16 size;
         volatile XDRV_UINT16 *srcp;
         XDRV_UINT16 i;

         IPC_LOG(( "\n\n**************************************************************\n"));
         IPC_LOG(( "IPC shared memory dump (%p)\n", duplexDrv ));

         size      = readDrv->bufferSize+2;
         srcp      = (volatile XDRV_UINT16 *)readDrv->pCb;
         IPC_LOG(( "\n\nRead  (DSP-->host) buf - size: %d, addr: %p\n", readDrv->bufferSize, srcp ));
         for (i=0; i<size; i++)
         {
            if ((i % 16)==0)
            {
               IPC_LOG(( "\n%p:",srcp));
            }
            IPC_LOG(( " %.4x", *srcp++ ));
         }

         size      = writeDrv->bufferSize+2;
         srcp      = (volatile XDRV_UINT16 *)writeDrv->pCb;
         IPC_LOG(( "\n\nWrite (host-->DSP) buf - size: %d, addr: %p\n", writeDrv->bufferSize, srcp ));
         for (i=0; i<size; i++)
         {
            if ((i % 16)==0)
            {
               IPC_LOG(( "\n%p:",srcp));
            }
            IPC_LOG(( " %.4x", *srcp++ ));
         }
         IPC_LOG(( "\n\nEnd of IPC shared memory dump (%p)\n", duplexDrv ));
         IPC_LOG(( "**************************************************************\n\n"));
      }
      break;
 
      default:
      {
         IPC_LOG(( "\n\nIPC SHARED MEM: Unknown IPC print mode (%d)!!!!\n\n", mode ));
      }
      break;
   }
}
#endif   /* !IPCSHAREDMEM_CFG_STATS_SUPPORT */

