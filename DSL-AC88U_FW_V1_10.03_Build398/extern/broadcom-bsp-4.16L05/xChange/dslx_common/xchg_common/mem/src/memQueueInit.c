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
*  @file    memQueueInit.c 
*
*  @brief   Contains functions for initializing a MEM_QUEUE
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosSem.h>
#include <memQueue.h>
#include <memQueueCheck.h>
#include <str.h>

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
*  Initializes the queue to be empty.
*/
void MEM_DEBUG_FUNCNAME(memQueueInit)
(
   MEM_QUEUE *queue,       /**< (mod)  Queue object to initialize                   */
   size_t maxItems         /**< (in) Maximum number of items that will be queued.   */
   MEM_DEBUG_PARAMS_DECL 
)
{
   char  name[ 16 ];

   BOS_ASSERT( queue != NULL );

   strPrintf( name, sizeof( name ), "mQS-%08x", &queue->availSem );
   bosSemCreate( name, 0, maxItems, &queue->availSem );

   strPrintf( name, sizeof( name ), "mQM-%08x", &queue->mutex );
   bosMutexCreate( name, &queue->mutex );

   queue->headPtr = NULL;
   queue->tailPtr = NULL;
   queue->count = 0;

   MEM_CHECK_QUEUE( queue );

} /* memQueueInit */

/** @} */

