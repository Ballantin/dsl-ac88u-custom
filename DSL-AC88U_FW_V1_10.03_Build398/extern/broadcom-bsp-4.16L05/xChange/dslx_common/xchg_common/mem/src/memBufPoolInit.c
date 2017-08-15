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
*  @file    memBufPoolInit.c 
*
*  @brief   Contains Buffer Pool initialization functions.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosMutex.h>
#include <bosError.h>
#include <memBufPool.h>
#include <memCfg.h>
#include <str.h>
#include <string.h>

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
*  Initializes a buffer pool, putting all of the blocks on the free list.
*
*  @return  The number of buffers actually allocated.
*
*  @note    There is currently an overhead of MEM_BUFPOOL_HDR_SIZE for 
*           each block.
*/
size_t memBufPoolInit
(
   MEM_BUFPOOL   *bufPool,    /**< (mod) Buffer Pool to initialize.                 */
   size_t         blockSize,  /**< (in) Size of each buffer (in units ofr size_t)   */
   void          *bufPoolPtr, /**< (in) Beginning of memory to use for buffer pool. */
   size_t         bufPoolSize /**< (in) Size of memory to use for buffer pool.      */
)
{
   bufPool->numFreeBuf = memBufPoolCoreInit( &bufPool->core, blockSize,
                                             bufPoolPtr, bufPoolSize );
   bufPool->lowWaterMark = bufPool->numFreeBuf;

   bufPool->startp = MEM_BLOCK_ADDR_UP( bufPoolPtr );
   bufPool->endp   = MEM_BLOCK_ADDR_DOWN( (char *)bufPoolPtr + bufPoolSize );
   bufPool->bufSize = blockSize;

   bosMutexAcquire( &bufPool->core.mutex );
   {
      bufPool->nextp = gMemBufPoolList;
      gMemBufPoolList = bufPool;
   }
   bosMutexRelease( &bufPool->core.mutex );

   return bufPool->numFreeBuf;

} /* memBufPoolInit */

/***************************************************************************/
/**
*  Initializes a buffer pool core, putting all of the blocks on the free 
*  list. A buffer pool core is simpler than a buffer pool, in that it
*  only retains the free list. No statistics or other information about the
*  buffer pool is retained once it's initialized.
*
*  @return  The number of buffers actually allocated.
*
*  @note    There is currently an overhead of MEM_BUFPOOL_HDR_SIZE for 
*           each block.
*/
size_t memBufPoolCoreInit
(
   MEM_BUFPOOL_CORE *bufPoolCore,/**< (mod) Buffer Pool Core to initialize.            */
   size_t            blockSize,  /**< (in) Size of each buffer (in units ofr size_t)   */
   void             *bufPoolPtr, /**< (in) Beginning of memory to use for buffer pool. */
   size_t            bufPoolSize /**< (in) Size of memory to use for buffer pool.      */
)
{
   size_t          blkIdx;
   size_t          totalBlockSize;
   size_t          numBlocks;
   MEM_BLOCK_HDR  *freep;
   MEM_BLOCK_HDR  *prevp;
   char           *bufPoolStart;
   char           *bufPoolEnd;
   char            mutexName[ 16 ];

   BOS_ASSERT( bufPoolCore != NULL );
   BOS_ASSERT( blockSize > 0 );
   BOS_ASSERT( bufPoolPtr != NULL );
   BOS_ASSERT( bufPoolSize > 0 );

   /*
    * Align the buffer pool
    */

#if MEM_CFG_DEBUG

   /*
    * We need room for a single word at the end of the heap to store the
    * debugEndTag. We reference through a MEM_BlockHdr but we don't need
    * a whole MEM_BlockHdr.
    */

   bufPoolSize -= sizeof( BOS_UINT16 );

   memset( bufPoolPtr, (int)0xEEEE,  bufPoolSize );
#endif

   strPrintf( mutexName, sizeof( mutexName ), "MBP-%08x", &bufPoolCore->mutex );
   if ( bosMutexCreate( mutexName, &bufPoolCore->mutex ) != BOS_STATUS_OK )
   {
      return 0;
   }

   bufPoolStart = MEM_BLOCK_ADDR_UP( bufPoolPtr );
   bufPoolEnd   = MEM_BLOCK_ADDR_DOWN( (char *)bufPoolPtr + bufPoolSize );
   bufPoolSize  = bufPoolEnd - bufPoolStart;

   /*
    * Figure out how many blocks we can store
    */

   totalBlockSize = MEM_TOTAL_BLOCK_SIZE( blockSize );
   numBlocks = bufPoolSize / totalBlockSize;

   BOS_ASSERT( numBlocks > 0 );

   /*
    * Chain all of the blocks together onto the free list
    */

   prevp = NULL;
   freep = (MEM_BLOCK_HDR *)bufPoolStart;

   for ( blkIdx = 0; blkIdx < numBlocks; blkIdx++ )
   {
      freep->nextp = prevp;
      freep->size  = totalBlockSize;

#if MEM_CFG_DEBUG_LOCATION
      freep->fileName = NULL;
      freep->lineNum  = 0;
#endif

      prevp = freep;

      freep = MEM_NEXT_BLOCK( freep );

#if MEM_CFG_DEBUG
      prevp->debugStartTag = MEM_BLOCK_BUFPOOL_DEBUG_FREE_START;
      freep->debugEndTag   = MEM_BLOCK_BUFPOOL_DEBUG_FREE_END;
#endif
   }

   bufPoolCore->freep = prevp;

   return numBlocks;

} /* memBufPoolCoreInit */

/** @} */

