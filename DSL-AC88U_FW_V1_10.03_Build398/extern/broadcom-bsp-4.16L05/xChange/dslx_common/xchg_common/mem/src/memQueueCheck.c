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
*  @file    memQueueCheck.c 
*
*  @brief   Implements a Queue debugging function.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosTypes.h>
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

#define  MEM_ALLOW_FORCE_CHECK   0

/* ---- Public Variables ------------------------------------------------- */


#if MEM_ALLOW_FORCE_CHECK

/*
 * Set the following variable to TRUE to force a Queue Check Failure (allows
 * debugging stuff to be checked).
 */

BOS_BOOL gMemForceQueueCheckFailure = FALSE;

#endif

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/
/**
*  This function checks the integrity of a queue. It is primarily
*  intended for debugging purposes.
*
*  @return  BOS_TRUE if the queue integrity looks good, BOS_FALSE if it
*           doesn't.
*/

BOS_BOOL memCheckQueue
(
   MEM_QUEUE *queue  /**< (in) Queue to check                              */
)
{
   BOS_BOOL  rc;

#if MEM_ALLOW_FORCE_CHECK
   if ( gMemForceQueueCheckFailure )
   {
      return BOS_FALSE;
   }
#endif

   bosMutexAcquire( &queue->mutex );
   {
      size_t          count   = queue->count;
      MEM_BLOCK_HDR  *scanHdr = queue->headPtr;

      /*
       * Initially we verify that the count is equal to the number of
       * items actually queued.
       */

      while (( scanHdr != NULL ) && ( count > 0 ))
      {
         scanHdr = scanHdr->nextp;
         count--;
      }

      rc = (( count == 0 ) && ( scanHdr == NULL ));
   }
   bosMutexRelease( &queue->mutex );

   return rc;

} /* memCheckQueue */

/** @} */

