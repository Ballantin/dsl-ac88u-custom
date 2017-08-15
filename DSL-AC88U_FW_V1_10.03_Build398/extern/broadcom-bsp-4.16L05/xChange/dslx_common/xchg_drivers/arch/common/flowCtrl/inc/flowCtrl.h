/****************************************************************************
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
****************************************************************************/
#ifndef FLOW_CTRL_H
#define FLOW_CTRL_H

#include <flowCtrlCfg.h>
#include <xdrvTypes.h>


/*
** The circular buffer ring size is one greater than the reqeusted number of
** buffers. The ring buffer works properly only if it always has at least one
** empty element. If there were no empty element, it would be impossible to
** distinguish a full queue from an empty queue. An empty ring can be detected
** if "writeIndex == readIndex". A full ring can be detected if
** "( writeIndex + 1 ) % RING_SIZE == readIndex".
*/
#define FLOW_CONTROL_RING_SIZE   ( FLOWCTRL_CFG_NUM_BUFFER + 1 )

typedef struct
{
   XDRV_UINT16 length;
   XDRV_UINT16 buffer[FLOWCTRL_CFG_BUFFER_SIZE_WORDS];

} FLOWCTRLBUF;


typedef struct FLOW_CTRL_STATS
{
   /* The total number of buffers dequeued of flow control. */
   unsigned int   totalDequeueCount;

   /* Total number of buffers queued for flow control. */
   unsigned int   totalEnqueueCount;

   /* The current number of buffers queued for flow control. */
   unsigned int   currentEnqueueCount;

   /* Maximum enqueue count. */
   unsigned int   highWaterEnqueueCount;

   /* The total number of buffers droped due to overflow. */
   unsigned int   totalOverflowCount;
} FLOW_CTRL_STATS;


typedef struct
{
   unsigned          readIndex;
   unsigned          writeIndex;

   FLOWCTRLBUF       pool[FLOW_CONTROL_RING_SIZE];

   FLOW_CTRL_STATS   stats;
} FLOWCTRL;


void flowctrlInit( FLOWCTRL *flowCtrl );
int flowctrlIsEmpty( FLOWCTRL *pFlowCtrl );
int flowctrlIsFull( FLOWCTRL *pFlowCtrl );
int flowctrlGetMsgLength( FLOWCTRL *pFlowCtrl );
void *flowctrlGetMsgPointer( FLOWCTRL *pFlowCtrl );
void flowctrlFreeMsg( FLOWCTRL *pFlowCtrl );
int flowctrlEnqueue( FLOWCTRL *pFlowCtrl,
                     XDRV_UINT16 *srcPtr1, unsigned srcLen1,
                     XDRV_UINT16 *srcPtr2, unsigned srcLen2 );
void flowctrlPrintStats( FLOWCTRL *flowCtrl );

#endif /* FLOW_CTRL_H */
