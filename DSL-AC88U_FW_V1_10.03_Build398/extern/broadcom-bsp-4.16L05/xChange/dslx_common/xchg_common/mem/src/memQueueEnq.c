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
*  @file    memQueueEnq.c 
*
*  @brief   Implements the memQueueEnq function.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

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
*  Appends a memory buffer to the end of the queue. 'data' must have been
*  allocated from a heap or buffer pool.
*/

void MEM_DEBUG_FUNCNAME(memQueueEnq)
(
   MEM_QUEUE  *queue,      /**< Queue to append to.                        */
   void       *data        /**< Data object to append.                     */
   MEM_DEBUG_PARAMS_DECL
)
{
   MEM_BLOCK_HDR  *dataHdr;

   BOS_ASSERT( queue != NULL );

   dataHdr = MEM_BLOCK_HDR_FROM_DATA( data );
   dataHdr->nextp = NULL;
   MEM_DEBUG_FILL_LOCATION( dataHdr );

   bosMutexAcquire( &queue->mutex );
   {
      MEM_CHECK_QUEUE( queue );

      if ( queue->tailPtr == NULL )
      {
         queue->headPtr = dataHdr;
      }
      else
      {
         queue->tailPtr->nextp = dataHdr;
      }
      queue->tailPtr = dataHdr;

      queue->count++;

      MEM_CHECK_QUEUE( queue );
   }
   bosMutexRelease( &queue->mutex );
   bosSemGive( &queue->availSem );

} /* memQueueEnq */

/** @} */

