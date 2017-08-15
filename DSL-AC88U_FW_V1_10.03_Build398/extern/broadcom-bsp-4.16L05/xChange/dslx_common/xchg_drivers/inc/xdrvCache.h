/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: xdrvCache.h
*
****************************************************************************
*    Description:
*
*     Architecture-independent interface for cache operations. Each supported
*     architecture should implement to this common API.
*
****************************************************************************/

#ifndef XDRV_CACHE_H
#define XDRV_CACHE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

enum
{
   /* Flushes the cache (ie: does writeback). */
   XDRV_CACHE_FLUSH        = 0x01,

   /* invalidates cache entries. */
   XDRV_CACHE_INVALIDATE   = 0x02
};



/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

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
*****************************************************************************
*/
void xdrvDCacheClearByAddress
(
   unsigned int   clearMask,
   void          *startAddr,
   unsigned int   lengthBytes
);


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_CACHE_H  */
