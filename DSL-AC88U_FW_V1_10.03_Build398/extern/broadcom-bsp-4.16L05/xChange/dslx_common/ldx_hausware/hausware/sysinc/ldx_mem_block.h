/****************************************************************************
*
*  MEM_Block.h
*
*  PURPOSE:
*     Has some macros for determining memory alignment for memory blocks.
*
*
*        (c)   Copyright Broadcom Inc. 2000
*              All Rights Reserved
*
***************************************************************************/

#if !defined( LDX_MEM_BLOCK_H )
#define  LDX_MEM_BLOCK_H

#include <stddef.h>            /* Needed for size_t */
#include <xcfg_mem.h>
#include <haus_cpu.h>
#include <hapi_types.h>

/*---------------------------------------------------------------------------
* The DSP processors, (ZSP) currently have 16 bit words and can have
* 32 bit words, so we'd like to return 32 bit aligned data in these situations.
*
* For other processors, we typically want to return 64 bit aligned data
* (since this is how big doubles are).
*
* Because of this, our header is big enough to store two pointers (or
* two size_t's), so we store our block size and link pointer in the header
* rather than introduce the complexity of only linking together free blocks.
*
* The macro MEM_BLOCK_BOUNDARY must be equal to a power of 2 minus 1, and
* it forms a mask on addresses that can be returned.
*/

#if defined(HAUS_CPU_ZSP) && HAUS_CPU_ZSP

#define  MEM_BLOCK_BOUNDARY   1u /* Two 16-bit words                       */

#else

#define  MEM_BLOCK_BOUNDARY   7u /* Eight 8-bit bytes                      */

#endif   /* HAUS_CPU_ZSP */


/*---------------------------------------------------------------------------
*
* MEM_BlockHdr
*
* The MEM_BlockHdr sits in front of each user allocated block.
*
* For simplicity, the size that we record is the size including the header.
*/

typedef struct MEM_BlockHdr_s
{
#if XCFG_MEM_DEBUG

   /*
    * We store the EndTag in this block to help ease alignment. Otherwise
    * it's highly likely that we'll waste memory by padding the start out to
    * the alignment, and then padding the end out as well.
    */

   HAPI_UINT16             debugEndTag;   /* For the previous block        */
   HAPI_UINT16             debugStartTag; /* For this block                */

#endif /* XCFG_MEM_DEBUG */

#if XCFG_MEM_DEBUG_LOCATION

   const char *            fileName;
   int                     lineNum;

#endif   /* XCFG_MEM_DEBUG_LOCATION */


   struct MEM_BlockHdr_s  *nextp; /* Ptr to next block                     */
   size_t                  blockSize;  /* Size of the block (units = sizeof)    */

} MEM_BlockHdr;

/*---------------------------------------------------------------------------
*
* MEM_BLOCK_SIZE calculates the "aligned" size of an entity
*/

#define  MEM_BLOCK_SIZE( size )  (( (size) + MEM_BLOCK_BOUNDARY ) & ~MEM_BLOCK_BOUNDARY )


#define  MEM_BLOCK_HDR_SIZE  MEM_BLOCK_SIZE( sizeof( MEM_BlockHdr ))

#define  MEM_TOTAL_BLOCK_SIZE( size )  ( MEM_BLOCK_SIZE( size ) + MEM_BLOCK_HDR_SIZE )

/* An extra block of memory is required to store the end tag (HAPI_UINT16) */

#if XCFG_MEM_DEBUG
#  define MEM_BLOCK_EXTRA_DBG_MEM_SIZE MEM_BLOCK_SIZE( sizeof( HAPI_UINT16 ))
#else
#  define MEM_BLOCK_EXTRA_DBG_MEM_SIZE 0
#endif

#endif   /* LDX_MEM_BLOCK_H */

