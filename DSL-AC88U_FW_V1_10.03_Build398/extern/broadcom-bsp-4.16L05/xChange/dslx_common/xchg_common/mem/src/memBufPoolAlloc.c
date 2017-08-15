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
*  @file    memBufPoolAlloc.c 
*
*  @brief   Implementation for a fixed size block allocator.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <memBufPool.h>
#include <string.h>

/* ---- Public Variables ------------------------------------------------- */

/**
 * List of buffer pools which have been created. This list is scanned in
 * order to figure out which buffer pool a buffer belongs to.
 */

MEM_BUFPOOL *gMemBufPoolList = NULL;

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
*  Allocates a block from the buffer pool. 
*
*  @note    The contents of the memory block is NOT initialized.
*
*  @return  Pointer to the allocated block, or NULL if no more blocks are
*           available.
*/
void *MEM_DEBUG_FUNCNAME(memBufPoolAlloc)
(
   MEM_BUFPOOL *bufPool
   MEM_DEBUG_PARAMS_DECL
)
{
   void *allocMem;

   bosMutexAcquire( &bufPool->core.mutex );
   {
      allocMem = MEM_DEBUG_FUNCNAME(memBufPoolCoreAlloc)
                     ( &bufPool->core MEM_DEBUG_PARAMS );

      if ( allocMem != NULL )
      {
         bufPool->numFreeBuf--;
         if ( bufPool->numFreeBuf < bufPool->lowWaterMark )
         {
            bufPool->lowWaterMark = bufPool->numFreeBuf;
         }
      }
      MEM_BUFPOOL_TRACE(( "memBufPoolAllocAlloc( 0x%x ) returning 0x%x, numFreeBuf: %d\n",
                          bufPool, allocMem, bufPool->numFreeBuf ));

#if MEM_CFG_DUMP_ON_FAILURE
      if ( allocMem == NULL )
      {
         memBufPoolDump( bufPool );
      }
#endif   /* MEM_CFG_DUMP_ON_FAILURE */

      BOS_ASSERT( allocMem != NULL );
   }
   bosMutexRelease( &bufPool->core.mutex );

   return allocMem;

} /* memBufPoolAlloc */

/***************************************************************************/
/**
*  Allocates a block from the buffer pool. The contents of the memory 
*  block is initialized to binary zeros (0x00)
*
*  @return  Pointer to the allocated block, or NULL if no more blocks are
*           available.
*/
void *MEM_DEBUG_FUNCNAME(memBufPoolAllocClear)
(
   MEM_BUFPOOL *bufPool
   MEM_DEBUG_PARAMS_DECL
)
{
   void *mem;

   mem = MEM_DEBUG_FUNCNAME(memBufPoolAlloc)( bufPool MEM_DEBUG_PARAMS );
   if ( mem != NULL )
   {
      memset( mem, 0, bufPool->bufSize );
   }

   return mem;

} /* memBufPoolAllocClear */

/***************************************************************************/
/**
*  Allocates a block from the buffer pool. 
*
*  @note    The contents of the memory block is NOT initialized.
*
*  @return  Pointer to the allocated block, or NULL if no more blocks are
*           available.
*/
void *MEM_DEBUG_FUNCNAME(memBufPoolCoreAlloc)
(
   MEM_BUFPOOL_CORE *bufPoolCore
   MEM_DEBUG_PARAMS_DECL
)
{
   void  *allocData = NULL;

   bosMutexAcquire( &bufPoolCore->mutex );
   {
      MEM_BLOCK_HDR *memp;

      /*
       * Unlink a block from the head of the free list
       */

      if (( memp = bufPoolCore->freep ) != NULL )
      {
         #if MEM_CFG_DEBUG
         {
            MEM_BLOCK_HDR *nextHdr = MEM_NEXT_BLOCK( memp );

            BOS_ASSERT( memp->debugStartTag  == MEM_BLOCK_BUFPOOL_DEBUG_FREE_START );
            BOS_ASSERT( nextHdr->debugEndTag == MEM_BLOCK_BUFPOOL_DEBUG_FREE_END );

            memp->debugStartTag  = MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_START;
            nextHdr->debugEndTag = MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_END;
         }
         #endif

         bufPoolCore->freep = memp->nextp;
         memp->nextp = NULL;
         MEM_DEBUG_FILL_LOCATION( memp );

         allocData = MEM_DATA_FROM_BLOCK_HDR( memp );
      }
   }
   bosMutexRelease( &bufPoolCore->mutex );

   return allocData;

} /* memBufPoolCoreAlloc */

/** @} */

