/***************************************************************************
*    Copyright 2013  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: cacheArm.c
*
****************************************************************************
*    Description:
*
*     ARM-specific implementation of cache operations.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>
#include <linux/dma-direction.h>

#include <xdrvCache.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvDCacheClearByAddress
**
** PURPOSE:    Clears data stored in the cache of the given address range.
**             Data may be flushed and/or invalidated depending on the given mask.
**
** PARAMETERS: clearMask   (in)  Flush, invalidate, or both.
**             startAddr   (in)  Start address to clear.
**             lengthBytes (in)  Length in bytes of the address range to clear.
**
** RETURNS:    Nothing.
**
** NOTE:
**             void xdrvDCacheClearByAddress
**             (
**                unsigned int   clearMask,
**                void          *startAddr,
**                unsigned int   lengthBytes
**             );
** 
*****************************************************************************
*/
void xdrvDCacheClearByAddress
(
   unsigned int   clearMask,
   void          *startAddr,
   unsigned int   lengthBytes
)
{
   if( lengthBytes > 0 && (clearMask & (XDRV_CACHE_FLUSH | XDRV_CACHE_INVALIDATE)) )
   {
      if( clearMask & XDRV_CACHE_FLUSH )
      {
         dma_sync_single_for_device( NULL, (dma_addr_t) startAddr, (size_t)lengthBytes, DMA_BIDIRECTIONAL );
      }

      if( clearMask & XDRV_CACHE_INVALIDATE )
      {
         dma_sync_single_for_cpu( NULL, (dma_addr_t) startAddr, (size_t)lengthBytes, DMA_BIDIRECTIONAL );
      }
   }
}

