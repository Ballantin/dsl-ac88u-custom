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
*  @file    memBlock.h
*
*  @brief   Contains macros for determining memory alignment for memory blocks.
*
****************************************************************************/
/**
*  @defgroup   mem      Memory Management Routines
*
*  @brief   Routines for performing various type of memory allocations.
*
****************************************************************************/

#if !defined( MEMBLOCK_H )
#define MEMBLOCK_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSTYPES_H )
#  include <bosTypes.h>
#endif

#if !defined( MEMCFG_H )
#  include "memCfg.h"
#endif

#include <stddef.h>  /* For size_t                                         */

/**
 * @addtogroup mem
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
 *
 * Calculates the "aligned" size of an entity.
 */

#define  MEM_BLOCK_SIZE( size )  (( (size) + MEM_CFG_BLOCK_BOUNDARY ) & ~MEM_CFG_BLOCK_BOUNDARY )

/**
 *
 * Calculates the aligned address greater than or equal to addr.
 */
#define  MEM_BLOCK_ADDR_UP( addr ) \
   ((void *)(((size_t)(addr) + MEM_CFG_BLOCK_BOUNDARY ) & ~MEM_CFG_BLOCK_BOUNDARY ))

/**
 *
 * Calculates the aligned address less than or equal to addr.
 */
#define  MEM_BLOCK_ADDR_DOWN( addr ) \
   ((void *)((size_t)(addr) & ~MEM_CFG_BLOCK_BOUNDARY ))

#if MEM_CFG_DEBUG

/** Used to identify the beginning of a free heap block.                   */
#define  MEM_BLOCK_HEAP_DEBUG_FREE_START     0xEAFB

/** Used to identify the end of a free heap block.                         */
#define  MEM_BLOCK_HEAP_DEBUG_FREE_END       0xEAFE

/** Used to identify the beginning of an allocated heap block.             */
#define  MEM_BLOCK_HEAP_DEBUG_ALLOC_START    0xEAAB

/** Used to identify the end of an allocated heap block.                   */
#define  MEM_BLOCK_HEAP_DEBUG_ALLOC_END      0xEAAE

/** Used to identify the beginning of an free buffer pool block.           */
#define  MEM_BLOCK_BUFPOOL_DEBUG_FREE_START  0xBBFB

/** Used to identify the end of an free buffer pool block.                 */
#define  MEM_BLOCK_BUFPOOL_DEBUG_FREE_END    0xBBFE

/** Used to identify the beginning of an allocated buffer pool block.      */
#define  MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_START 0xBBAB

/** Used to identify the end of an allocated buffer pool block.            */
#define  MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_END   0xBBAE

#endif   /* MEM_CFG_DEBUG */

#if MEM_CFG_DEBUG_LOCATION

/**
 * Generates the version of a function which takes a filename and line 
 * number as parameters.
 */

#define  MEM_DEBUG_FUNCNAME(x)   x##Dbg

/** 
 * Optionally adds the filename and line number parameters to the argument 
 * list of the function.
 */

#define  MEM_DEBUG_PARAMS_DECL   , const char *fileName, int lineNum

/**
 * Optionally adds the filename and line number parameters when calling
 * other debug functions within the same module.
 */

#define  MEM_DEBUG_PARAMS        , fileName, lineNum

/**
 * Copies the filename and line number information into the header block
 * indicated by @a x.
 */

#define  MEM_DEBUG_FILL_LOCATION(x) \
   do                               \
   {                                \
      (x)->fileName = fileName;     \
      (x)->lineNum  = lineNum;      \
   } while ( 0 )

#else

#define  MEM_DEBUG_FUNCNAME(x)   x
#define  MEM_DEBUG_PARAMS_DECL
#define  MEM_DEBUG_PARAMS
#define  MEM_DEBUG_FILL_LOCATION(x)

#endif   /* MEM_CFG_DEBUG_LOCATION */

/*-------------------------------------------------------------------------*/
/**
 * The MEM_BLOCK_HDR sits in front of each user allocated block.
 *
 * For simplicity, the size that we record is the size including the header.
 */

typedef struct MEM_BLOCK_HDR_s
{
#if MEM_CFG_DEBUG

   /*
    * We store the EndTag in this block to help ease alignment. Otherwise
    * it's highly likely that we'll waste memory by padding the start out to
    * the alignment, and then padding the end out as well.
    */

   BOS_UINT16     debugEndTag;   /**< For the previous block               */
   BOS_UINT16     debugStartTag; /**< For this block                       */

#endif /* MEM_CFG_DEBUG */

#if MEM_CFG_DEBUG_LOCATION

   const char *   fileName;   /**< Name of file allocation occurred in.    */
   int            lineNum;    /**< Line number allocation occured at.      */

#endif   /* MEM_CFG_DEBUG_LOCATION */


   struct MEM_BLOCK_HDR_s *nextp; /**< Ptr to next block                    */
   size_t                  size; /**< Size of the block (units = sizeof)   */

} MEM_BLOCK_HDR;

/**
 * Calculates the aligned size of a header block.
 */
#define  MEM_BLOCK_HDR_SIZE  MEM_BLOCK_SIZE( sizeof( MEM_BLOCK_HDR ))

/**
 * Given the desired size of the user portion of a memory block, this macro
 * calculates the total size of the header and the user portion, bumping
 * everything up to the next alignment.
 */
#define  MEM_TOTAL_BLOCK_SIZE( size )  ( MEM_BLOCK_SIZE( size ) + MEM_BLOCK_HDR_SIZE )

/**
 * Given a pointer to the user portion of the data, this returns a pointer
 * the beginning of the header block.
 */
#define  MEM_BLOCK_HDR_FROM_DATA( addr ) \
   ((MEM_BLOCK_HDR *)((char *)(addr) - MEM_BLOCK_HDR_SIZE ))

/**
 * Given a pointer to the header block, this returns a pointer to the user
 * portion of the data.
 */
#define  MEM_DATA_FROM_BLOCK_HDR( hdr ) \
   ((void *)((char *)(hdr) + MEM_BLOCK_HDR_SIZE ))

/**
 * Given a pointer to a header block, this macro returns a pointer to the
 * next header block (after the user data).
 */
#define  MEM_NEXT_BLOCK( hdrp )  ((MEM_BLOCK_HDR *)((char *)(hdrp) + (hdrp)->size ))

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif   /* MEMBLOCK_H */

