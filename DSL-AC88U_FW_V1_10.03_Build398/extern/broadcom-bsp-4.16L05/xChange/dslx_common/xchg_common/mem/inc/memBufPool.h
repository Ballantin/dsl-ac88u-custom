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
*  @file    memBufPool.h
*
*  @brief   Interface for a fixed size block allocator.
*
****************************************************************************/
/**
*  @defgroup   memBufPool  Fixed size buffer pool
*  @ingroup    mem
*
*  @brief   Allows fixed sized blocks to be allocated and released very 
*           efficiently.
*
*  Manages a pool of fixed sized buffers, in a thread safe manner.
*
****************************************************************************/

#if !defined( MEMBUFPOOL_H )
#define MEMBUFPOOL_H          /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSERROR_H )
#  include <bosError.h>
#endif

#if !defined( BOSMUTEX_H )
#  include <bosMutex.h>
#endif

#if !defined( MEMBLOCK_H )
#  include <memBlock.h>
#endif

#if !defined( MEMCFG_H )
#  include <memCfg.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/**
 * @addtogroup memBufPool
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 * Macro used to perform conditional tracing. Typically only used for 
 * debugging.
 */

#if MEM_CFG_BUFPOOL_TRACE_ENABLED
#  define  MEM_BUFPOOL_TRACE(x)    BOS_LOG(x)
#else
#  define  MEM_BUFPOOL_TRACE(x)
#endif

/**
 * Calculates how much memory will be required to store @a numBlocks blocks
 * of memory, each being @a size units big (units of size_t).
 */

#if MEM_CFG_DEBUG

#define  MEM_BUF_POOL_SIZE( numBlocks, size ) \
   (((numBlocks) * MEM_TOTAL_BLOCK_SIZE( (size) )) + MEM_BLOCK_HDR_SIZE )

#else

#define  MEM_BUF_POOL_SIZE( numBlocks, size ) \
   (((numBlocks) * MEM_TOTAL_BLOCK_SIZE( (size) )))

#endif   /* MEM_CFG_DEBUG */

/**
 * For occaisons where just a pool of buffers is required, and you know which
 * buffer pool to free a block into, the MEM_BUFPOOL_CORE is more appropriate.
 */

typedef struct
{
   BOS_MUTEX      mutex;   /**< Ensures mutual exclusion                   */
   MEM_BLOCK_HDR *freep;   /**< Free list of buffers                       */

} MEM_BUFPOOL_CORE;

/**
 * When you have many different buffer pools, and you just want to release
 * a buffer back to where it came from, but you don't know where that is,
 * this more heavy weight version will work.
 *
 * Note that the units for bufSize are in terms of the sizeof operator.
 */

typedef struct MEM_BUFPOOL_s
{
   /*
    * IMPORTANT: The 'core' member MUST be the first member of this structure.
    */

   MEM_BUFPOOL_CORE  core;          /**< List of free blocks               */

   size_t            numFreeBuf;    /**< Number of free buffers in pool    */
   size_t            lowWaterMark;  /**< Minimum number of free buffers    */

   void             *startp;        /**< Pointer to the beginning of the buffer pool      */
   void             *endp;          /**< Pointer just beyond the end of the last buffer   */
   size_t            bufSize;       /**< Size of each buffer (in units of size_t)         */

   struct MEM_BUFPOOL_s *nextp;     /**< Next buffer pool in the list      */

} MEM_BUFPOOL;

/* ---- Variable Externs ------------------------------------------------- */

extern   MEM_BUFPOOL *gMemBufPoolList;

/* ---- Function Prototypes ---------------------------------------------- */

/*-------------------------------------------------------------------------*/

void *MEM_DEBUG_FUNCNAME(memBufPoolAlloc)
(
   MEM_BUFPOOL   *bufPool
   MEM_DEBUG_PARAMS_DECL
);

void *MEM_DEBUG_FUNCNAME(memBufPoolCoreAlloc)
(
   MEM_BUFPOOL_CORE *bufPoolCore
   MEM_DEBUG_PARAMS_DECL
);

#if MEM_CFG_DEBUG_LOCATION

#define  memBufPoolAlloc( bufPool )      \
   memBufPoolAllocDbg( bufPool, __FILE__, __LINE__ )

#define  memBufPoolCoreAlloc( bufPool )  \
   memBufPoolCoreAllocDbg( bufPool, __FILE__, __LINE__ )

#endif   /* MEM_CFG_DEBUG_LOCATION */

/*-------------------------------------------------------------------------*/

void *MEM_DEBUG_FUNCNAME(memBufPoolAllocClear)
(
   MEM_BUFPOOL   *bufPool
   MEM_DEBUG_PARAMS_DECL
);

#if MEM_CFG_DEBUG_LOCATION

#define  memBufPoolAllocClear( bufPool ) \
   memBufPoolAllocClearDbg( bufPool, __FILE__, __LINE__ )

#endif

/*-------------------------------------------------------------------------*/

void memBufPoolDump( MEM_BUFPOOL *bufPool );

void memBufPoolFree( MEM_BUFPOOL *bufPool, void *block );

void memBufPoolCoreFree( MEM_BUFPOOL_CORE *bufPoolCore, void *block );

size_t memBufPoolInit
(
   MEM_BUFPOOL   *bufPoolBig,
   size_t         blockSize,
   void          *bufPoolPtr,
   size_t         bufPoolSize
);

size_t memBufPoolCoreInit
(
   MEM_BUFPOOL_CORE *bufPool,
   size_t            blockSize,
   void             *bufPoolPtr,
   size_t            bufPoolSize
);

void memBufPoolTerm( MEM_BUFPOOL *bufPool );

/** @} */

#if defined( __cplusplus )
}
#endif

#endif   /* MEMBUFPOOL_H */

