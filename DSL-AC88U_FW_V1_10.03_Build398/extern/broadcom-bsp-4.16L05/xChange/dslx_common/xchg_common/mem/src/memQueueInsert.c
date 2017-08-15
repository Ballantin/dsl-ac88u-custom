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
*  @file    memQueueInsert.c 
*
*  @brief   Implements memQueueInsert
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosError.h>
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

/****************************************************************************
*
* MEM_QueueInsert
*
* Inserts a memory object into the queue, after the memory block indicated
* by 'insertp'. If insertp is NULL, then the memory block is inserted at the
* beginning of the queue.
*
* If 'insertp' is not found on the queue, then the memory block will be
* queued at the end.
*/
void MEM_DEBUG_FUNCNAME(memQueueInsert)
(
   MEM_QUEUE  *queue,
   void       *data,
   void       *insertAfter
   MEM_DEBUG_PARAMS_DECL
)
{
   MEM_BLOCK_HDR  *dataHdr;
   MEM_BLOCK_HDR  *insertAfterHdr;

   BOS_ASSERT( queue != NULL );
   BOS_ASSERT( data != NULL );

   bosMutexAcquire( &queue->mutex );
   {
      MEM_CHECK_QUEUE( queue );

      dataHdr = MEM_BLOCK_HDR_FROM_DATA( data );
      MEM_DEBUG_FILL_LOCATION( dataHdr );

      /*
       * Check for the special case of inserting at the head of the queue
       */

      if ( insertAfter == NULL  )
      {
         if (( dataHdr->nextp = queue->headPtr ) == NULL )
         {
            queue->tailPtr = dataHdr;
         }
         queue->headPtr = dataHdr;
         queue->count++;
      }
      else
      {
         MEM_BLOCK_HDR  *scanHdr = queue->headPtr;

         /*
          * IMPORTANT: We NEED to verify that insertAfter is actually on 
          *            the queue. If it isn't then we can royally screw 
          *            things up here.
          *
          * Typically, people assume that they can just make this function
          * thread safe and everything will be fine. This function can only
          * truly be thread safe if we confirm that insertAfter is still on
          * the list.
          */

         insertAfterHdr = MEM_BLOCK_HDR_FROM_DATA( insertAfter );

         while (( scanHdr != NULL ) && ( scanHdr != insertAfterHdr ))
         {
            scanHdr = scanHdr->nextp;
         }
         BOS_ASSERT( scanHdr == insertAfterHdr );

         dataHdr->nextp = insertAfterHdr->nextp;
         insertAfterHdr->nextp = dataHdr;

         if ( queue->tailPtr == insertAfterHdr )
         {
            queue->tailPtr = dataHdr;
         }
      }

      queue->count++;

      MEM_CHECK_QUEUE( queue );
   }
   bosMutexRelease( &queue->mutex );

} /* memQueueInsert */

/** @} */

