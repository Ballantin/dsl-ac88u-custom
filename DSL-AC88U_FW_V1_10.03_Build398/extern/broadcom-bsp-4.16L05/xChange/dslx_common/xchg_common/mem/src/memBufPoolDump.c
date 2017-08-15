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
*  @file    memBufPoolDump.c 
*
*  @brief   Implements the memBufPoolDump routine.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosError.h>
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
*  Dumps the contents of a buffer pool.
*/
void memBufPoolDump
(
   MEM_BUFPOOL *bufPool    /**< Buffer pool to dump.                       */
)
{
   MEM_BLOCK_HDR  *hdr;
   size_t          totalBlockSize;
   size_t          numBlocks;
   size_t          idx;

   BOS_LOG(( "BufPool @ %p, # free = %d, lowWater = %d, size = %d, Free = %p\n",
             bufPool, bufPool->numFreeBuf, bufPool->lowWaterMark, 
             bufPool->bufSize, bufPool->core.freep ));

   totalBlockSize = MEM_TOTAL_BLOCK_SIZE( bufPool->bufSize );
   numBlocks = ( (char *)bufPool->endp - (char *)bufPool->startp ) / totalBlockSize;

   hdr = (MEM_BLOCK_HDR *)bufPool->startp;

   for ( idx = 0; idx < numBlocks; idx++ )
   {
      BOS_LOG(( "  Hdr @ %p, Data @ %p", hdr, MEM_DATA_FROM_BLOCK_HDR( hdr )));

#if MEM_CFG_DEBUG
      if ( hdr->debugStartTag == MEM_BLOCK_BUFPOOL_DEBUG_FREE_START )
      {
         BOS_LOG(( ", Free " ));
      }
      else
      if ( hdr->debugStartTag == MEM_BLOCK_BUFPOOL_DEBUG_ALLOC_START )
      {
         BOS_LOG(( ", Alloc" ));
      }
      else
      {
         BOS_LOG(( ", *****" ));
      }
#endif   /* MEM_CFG_DEBUG */
#if MEM_CFG_DEBUG_LOCATION
      if ( hdr->fileName != NULL )
      {
         BOS_LOG(( ", %s@%d", hdr->fileName, hdr->lineNum ));
      }
#endif   /* MEM_CFG_DEBUG_LOCATION */

      BOS_LOG(( ", nextp = %p\n", hdr->nextp ));

      hdr = MEM_NEXT_BLOCK( hdr );
   }


} /* memBufPoolDump */

/** @} */

