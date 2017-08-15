/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    memQueue.h
*
*  @brief   Interface for the memory queue.
*
****************************************************************************/
/**
*  @defgroup   memQueue Memory Queue
*
*  @brief   Implements a queue of memory objects
*
*  Allows objects allocted from Memory Buffer Pool, or Memory Heap to be
*  queued up.
*
****************************************************************************/

#if !defined( MEMQUEUE_H )
#define MEMQUEUE_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSTYPES_H )
#  include <bosTypes.h>
#endif

#if !defined( BOSMUTEX_H )
#  include <bosMutex.h>
#endif

#if !defined( BOSSEM_H )
#  include <bosSem.h>
#endif

#if !defined( MEMBLOCK_H )
#  include <memBlock.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/**
 * @addtogroup memQueue
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * Describes a singly linked list of memory blocks.
 */

typedef struct
{
   BOS_MUTEX      mutex;      /**< Ensure mutual exclusion.                      */
   BOS_SEM        availSem;   /**< Used when waiting for objects to be queued.   */
   MEM_BLOCK_HDR *headPtr;    /**< Pointer to first object in the queue.         */
   MEM_BLOCK_HDR *tailPtr;    /**< Pointer to the last object in the queue.      */
   size_t          count;     /**< Number of objects in the queue.               */

} MEM_QUEUE;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*-------------------------------------------------------------------------*/

void MEM_DEBUG_FUNCNAME(memQueueInit)( MEM_QUEUE *queue, size_t maxItems MEM_DEBUG_PARAMS_DECL );

#if MEM_CFG_DEBUG_LOCATION

#define  memQueueInit( q, maxItems )  MEM_DEBUG_FUNCNAME(memQueueInit)( q, maxItems, __FILE__, __LINE__ )

#endif

/*-------------------------------------------------------------------------*/

BOS_INLINE size_t memQueueCount( MEM_QUEUE *queue )
{
   return queue->count;
}

/*-------------------------------------------------------------------------*/

void MEM_DEBUG_FUNCNAME(memQueueEnq)
(
   MEM_QUEUE  *queue,
   void       *data
   MEM_DEBUG_PARAMS_DECL
);

#if MEM_CFG_DEBUG_LOCATION

#define  memQueueEnq( q, data )    \
   MEM_DEBUG_FUNCNAME(memQueueEnq)( q, data, __FILE__, __LINE__ )

#endif   /* MEM_CFG_DEBUG_LOCATION */

/*-------------------------------------------------------------------------*/

void *MEM_DEBUG_FUNCNAME(memQueueDeq)
(
   MEM_QUEUE *queue
   MEM_DEBUG_PARAMS_DECL
);

void *MEM_DEBUG_FUNCNAME(memQueueDeqNoBlock)
(
   MEM_QUEUE *queue
   MEM_DEBUG_PARAMS_DECL
);

void *MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)
(
   MEM_QUEUE *queue
   MEM_DEBUG_PARAMS_DECL
);

void *MEM_DEBUG_FUNCNAME(memQueueTimedDeq)
(
   MEM_QUEUE     *queue,
   BOS_TIME_MS    timtoueMsec
   MEM_DEBUG_PARAMS_DECL
);

#if MEM_CFG_DEBUG_LOCATION

#define  memQueueDeq( q )        MEM_DEBUG_FUNCNAME(memQueueDeq)(        q, __FILE__, __LINE__ )
#define  memQueueDeqNoBlock( q ) MEM_DEBUG_FUNCNAME(memQueueDeqNoBlock)( q, __FILE__, __LINE__ )
#define  memQueueDeqNoTake( q )  MEM_DEBUG_FUNCNAME(memQueueDeqNoTake)(  q, __FILE__, __LINE__ )
#define  memQueueTimedDeq( q )   MEM_DEBUG_FUNCNAME(memQueueTimedDeq)(   q, timeoutMsec, __FILE__, __LINE__ )

#endif   /* MEM_CFG_DEBUG_LOCATION */

/*-------------------------------------------------------------------------*/

void MEM_DEBUG_FUNCNAME(memQueueInsert)
(
   MEM_QUEUE  *queue,
   void       *data,
   void       *insertAfter
   MEM_DEBUG_PARAMS_DECL
);

#if MEM_CFG_DEBUG_LOCATION

#define  memQueueInsert( q, data, ia )   \
   MEM_DEBUG_FUNCNAME(memQueueInsert)( q, data, ia, __FILE__, __LINE__ )

#endif   /* MEM_CFG_DEBUG_LOCATION */

/** @} */

#if defined( __cplusplus )
}
#endif

#endif /* MEMQUEUE_H */

