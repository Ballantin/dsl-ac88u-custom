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
*****************************************************************************
*
*    Filename: flowCtrl.c
*
****************************************************************************
*    Description:
*
*     This file implements flow control.
*
****************************************************************************/


#include <xchgAssert.h>
#include <xdrvTypes.h>
#include <flowCtrlCfg.h>
#include <flowCtrl.h>
#include <string.h>
#include <xdrvLog.h>


/*
*****************************************************************************
** FUNCTION:   flowctrlInit
**
** PURPOSE:    To initialize a flow-control instance.
**
** PARAMETERS: flowCtrl struct to initialize.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void flowctrlInit( FLOWCTRL *flowCtrl )
{
   memset( flowCtrl, 0, sizeof( FLOWCTRL ) );
}


/*****************************************************************************
*
*****************************************************************************/
int flowctrlIsEmpty( FLOWCTRL *pFlowCtrl )
{
   XCHG_ASSERT( pFlowCtrl->readIndex  < FLOW_CONTROL_RING_SIZE );
   XCHG_ASSERT( pFlowCtrl->writeIndex < FLOW_CONTROL_RING_SIZE );

   return( pFlowCtrl->readIndex == pFlowCtrl->writeIndex );
}


/*****************************************************************************
*
*****************************************************************************/
int flowctrlIsFull( FLOWCTRL *pFlowCtrl )
{
   XCHG_ASSERT( pFlowCtrl->readIndex  < FLOW_CONTROL_RING_SIZE );
   XCHG_ASSERT( pFlowCtrl->writeIndex < FLOW_CONTROL_RING_SIZE );

   return ( (( pFlowCtrl->writeIndex + 1 ) % FLOW_CONTROL_RING_SIZE ) == pFlowCtrl->readIndex );
}



/*****************************************************************************
*
*****************************************************************************/
int flowctrlGetMsgLength( FLOWCTRL *pFlowCtrl )
{
   int readIdx;
   int numToWrite=0;

   readIdx = pFlowCtrl->readIndex;

   XCHG_ASSERT( readIdx < FLOW_CONTROL_RING_SIZE );

   /*
   ** copy the data to the HPI buffer only when there is enough free space
   */
   numToWrite = pFlowCtrl->pool[readIdx].length;
   XCHG_ASSERT( numToWrite <= FLOWCTRL_CFG_BUFFER_SIZE_WORDS );

   return( numToWrite );
}

/*****************************************************************************
*
*****************************************************************************/
void *flowctrlGetMsgPointer( FLOWCTRL *pFlowCtrl )
{
   int readIdx;

   readIdx = pFlowCtrl->readIndex;
   XCHG_ASSERT( readIdx < FLOW_CONTROL_RING_SIZE );

   return( pFlowCtrl->pool[readIdx].buffer );
}


/*****************************************************************************
*
*****************************************************************************/
void flowctrlFreeMsg( FLOWCTRL *pFlowCtrl )
{
   /*
   ** increment the read index in the flow control buffer
   */
   if( pFlowCtrl->readIndex == (FLOW_CONTROL_RING_SIZE - 1) )
   {
      pFlowCtrl->readIndex = 0;
   }
   else
   {
      pFlowCtrl->readIndex++;
   }

   /*
   ** Update stats.
   */
   pFlowCtrl->stats.totalDequeueCount++;
   pFlowCtrl->stats.currentEnqueueCount--;

   XCHG_ASSERT( pFlowCtrl->readIndex < FLOW_CONTROL_RING_SIZE );
}


/*****************************************************************************
*
*****************************************************************************/
int flowctrlEnqueue
(
   FLOWCTRL *pFlowCtrl,
   XDRV_UINT16   *srcPtr1, unsigned srcLen1,
   XDRV_UINT16   *srcPtr2, unsigned srcLen2
)
{
   unsigned writeIdx = pFlowCtrl->writeIndex;

   XCHG_ASSERT( srcLen1 + srcLen2 <= FLOWCTRL_CFG_BUFFER_SIZE_WORDS );

   if ( flowctrlIsFull( pFlowCtrl ))
   {
      pFlowCtrl->stats.totalOverflowCount++;
      return ( -1 );
   }

   memcpy( pFlowCtrl->pool[writeIdx].buffer,
           srcPtr1,
           srcLen1 * sizeof(XDRV_UINT16) );

   memcpy( pFlowCtrl->pool[writeIdx].buffer + srcLen1,
           srcPtr2,
           srcLen2 * sizeof(XDRV_UINT16) );

   pFlowCtrl->pool[writeIdx].length = srcLen1 + srcLen2;

   /* update the write index */
   if( pFlowCtrl->writeIndex == (FLOW_CONTROL_RING_SIZE - 1) )
   {
      pFlowCtrl->writeIndex = 0;
   }
   else
   {
      pFlowCtrl->writeIndex++;
   }

   /*
   ** Update stats.
   */
   pFlowCtrl->stats.totalEnqueueCount++;
   pFlowCtrl->stats.currentEnqueueCount++;

   if ( pFlowCtrl->stats.currentEnqueueCount > pFlowCtrl->stats.highWaterEnqueueCount )
   {
      pFlowCtrl->stats.highWaterEnqueueCount = pFlowCtrl->stats.currentEnqueueCount;
   }

   XCHG_ASSERT( pFlowCtrl->writeIndex < FLOW_CONTROL_RING_SIZE );

   return 0;
}

/*
*****************************************************************************
** FUNCTION:   flowctrlPrintStats
**
** PURPOSE:    Display statistics for flow control channel.
**
** PARAMETERS: flowCtrl - flow control object.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void flowctrlPrintStats( FLOWCTRL *flowCtrl )
{
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "" ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "Flow control (host-->DSP) stats (0x%x)\n", (unsigned int) flowCtrl ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "Total dequeue count:      %d", flowCtrl->stats.totalDequeueCount ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "Total enqueue count:      %d", flowCtrl->stats.totalEnqueueCount ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "Current enqueue count:    %d", flowCtrl->stats.currentEnqueueCount ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "High water enqueue count: %d", flowCtrl->stats.highWaterEnqueueCount ));
   XDRV_LOG_FORCED(( XDRV_LOG_MOD_XDRV, "Total overflow count:     %d", flowCtrl->stats.totalOverflowCount ));

   /*
   ** Reset stats each time it is queried. Do not clear the current number of
   ** buffers enqueued. :)
   */
   flowCtrl->stats.totalDequeueCount   = 0;
   flowCtrl->stats.totalEnqueueCount   = 0;
   flowCtrl->stats.totalOverflowCount  = 0;

   flowCtrl->stats.highWaterEnqueueCount = flowCtrl->stats.currentEnqueueCount;
}
