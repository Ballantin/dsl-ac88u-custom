/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    memQueueDeq.c 
*
*  @brief   Implements the memQueueDeqXxx functions.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <memBlock.h>
#include <memQueue.h>
#include <memQueueCheck.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup memQueue
 * @{
 */

/***************************************************************************/
/**
*  Dequeus a memory block from the queue. If no objects are available, 
*  this function will wait for one to show up before returning.
*
*  @return  A pointer to the dequeued object.
*/

void *MEM_DEBUG_FUNCNAME(memQueueDeq)
(
   MEM_QUEUE *queue
   MEM_DEBUG_PARAMS_DECL
)
{
   void  *deqData = NULL;

   if ( bosSemTake( &queue->availSem ) == BOS_STATUS_OK )
   {
      deqData = MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)( queue MEM_DEBUG_PARAMS );

      BOS_ASSERT( deqData != NULL );
   }

   return deqData;

} /* memQueueDeq */

/***************************************************************************/
/**
*  Dequeues a memory block from the queue.
*
*  @return  A pointer to the dequeued object, or NULL if no objects 
*           are available.
*/

void *MEM_DEBUG_FUNCNAME(memQueueDeqNoBlock)
(
   MEM_QUEUE     *queue
   MEM_DEBUG_PARAMS_DECL
)
{
   void  *deqData = NULL;

   if ( bosSemTimedTake( &queue->availSem, 0 ) == BOS_STATUS_OK )
   {
      deqData = MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)( queue MEM_DEBUG_PARAMS );

      BOS_ASSERT( deqData != NULL );
   }
   
   return deqData;

} /* memQueueTimedDeq */

/***************************************************************************/
/**
*  Dequeus a memory block from the queue, but doesn't perform a bosSemTake()
*  operation on the semaphore (assumes that the caller has already done this).
*
*  @return  A pointer to the dequeued object, or NULL if there aren't any
*           objects on the queue.
*/

void *MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)
(
   MEM_QUEUE *queue
   MEM_DEBUG_PARAMS_DECL
)
{
   MEM_BLOCK_HDR *headHdr;
   void          *deqData;

   bosMutexAcquire( &queue->mutex );
   {
      MEM_CHECK_QUEUE( queue );

      if (( headHdr = queue->headPtr ) == NULL )
      {
         /*
          * There isn't anything on the queue
          */

         BOS_ASSERT( queue->tailPtr == NULL );
         BOS_ASSERT( queue->count == 0 );

         deqData = NULL;
      }
      else
      {
         if ( queue->headPtr == queue->tailPtr )
         {
            /*
             * There's exactly one object on the queue
             */

            BOS_ASSERT( queue->count == 1 );

            queue->tailPtr = NULL;
         }
         else
         {
            /*
             * There's more than one item on the queue
             */

            BOS_ASSERT( queue->tailPtr != NULL );
            BOS_ASSERT( queue->count > 1 );
         }

         queue->headPtr = headHdr->nextp;
         headHdr->nextp = NULL;

         queue->count--;

         MEM_DEBUG_FILL_LOCATION( headHdr );

         deqData = MEM_DATA_FROM_BLOCK_HDR( headHdr );
      }
      MEM_CHECK_QUEUE( queue );
   }
   bosMutexRelease( &queue->mutex );

   return deqData;

} /* memQueueDeqNoTake */

/***************************************************************************/
/**
*  Dequeues a memory block from the queue. If no objects are available, 
*  this function will wait for upto @a timeoutMsec millseconds for one to 
*  show up before returning.
*
*  @return  A pointer to the dequeued object, or NULL if the timeout 
*           expired and no objects became available.
*/

void *MEM_DEBUG_FUNCNAME(memQueueTimedDeq)
(
   MEM_QUEUE     *queue,
   BOS_TIME_MS    timeoutMsec
   MEM_DEBUG_PARAMS_DECL
)
{
   void  *deqData = NULL;

   if ( bosSemTimedTake( &queue->availSem, timeoutMsec ) == BOS_STATUS_OK )
   {
      deqData = MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)( queue MEM_DEBUG_PARAMS );

      BOS_ASSERT( deqData != NULL );
   }
   
   return deqData;

} /* memQueueTimedDeq */

/** @} */

