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
*  @file    memBufPoolFree.c 
*
*  @brief   Implementation of the memBufPoolFree() function.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosError.h>
#include <bosMutex.h>
#include <memBufPool.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup memBufPool
 * @{
 */

/***************************************************************************/
/**
*  Releases a block back into the buffer pool.
*
*  @param   bufPool  (in)  Buffer pool that the data was allocated from,
*                          or NULL if unknown. Passing in NULL only works
*                          if the data was allocated using memBufPoolAlloc()
*                          or memBufPoolAllocClear().
*
*  @param   data     (in)  Previously allocated data buffer to free.
*/
void memBufPoolFree( MEM_BUFPOOL *bufPool, void *data )
{
   bosMutexAcquire( &bufPool->core.mutex );
   {
      if ( bufPool == NULL )
      {
         bufPool = gMemBufPoolList;

         while ( bufPool != NULL )
         {
            if (( data >= bufPool->startp ) && ( data < bufPool->endp ))
            {
               break;
            }

            bufPool = bufPool->nextp;
         }

         BOS_ASSERT( bufPool != NULL );
      }

      if ( bufPool != NULL )
      {
         BOS_ASSERT( data >= bufPool->startp );
         BOS_ASSERT( data < bufPool->endp );

         memBufPoolCoreFree( &bufPool->core, data );

         bufPool->numFreeBuf++;

         MEM_BUFPOOL_TRACE(( "MEM_BufPoolFree( 0x%x ) released 0x%x, numFreeBuf: %d\n",
                             bufPool, data, bufPool->numFreeBuf ));
      }
   }
   bosMutexRelease( &bufPool->core.mutex );

} /* memBufPoolFree */

/***************************************************************************/
/**
*  Releases a block back into the buffer pool.
*
*  @param   bufPool  (in)  Buffer pool that the data was allocated from.
*
*  @param   data     (in)  Previously allocated data buffer to free.
*/

void memBufPoolCoreFree( MEM_BUFPOOL_CORE *bufPoolCore, void *data )
{
   MEM_BLOCK_HDR  *memp;

   BOS_ASSERT( data != NULL );

   memp = MEM_BLOCK_HDR_FROM_DATA( data );

#if MEM_CFG_DEBUG
   {
      MEM_BLOCK_HDR *nextHdr = MEM_NEXT_BLOCK( memp );

      BOS_ASSERT( memp->debugStartTag  == MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_START );
      BOS_ASSERT( nextHdr->debugEndTag == MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_END );

      memp->debugStartTag  = MEM_BLOCK_BUFPOOL_DEBUG_FREE_START;
      nextHdr->debugEndTag = MEM_BLOCK_BUFPOOL_DEBUG_FREE_END;
   }
#endif
#if MEM_CFG_DEBUG_LOCATION
      memp->fileName = NULL;
      memp->lineNum  = 0;
#endif

   /*
    * Link the buffer into the head of the free list
    */

   bosMutexAcquire( &bufPoolCore->mutex );
   {
      memp->nextp = bufPoolCore->freep;
      bufPoolCore->freep = memp;
   }
   bosMutexRelease( &bufPoolCore->mutex );

} /* memBufPoolCoreFree */

/** @} */

