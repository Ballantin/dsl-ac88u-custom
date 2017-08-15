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
*  @file    memCfg.h
*
*  @brief   Configurable parameters for the MEM module.
*
****************************************************************************/
#if !defined( MEMCFG_H )
#define MEMCFG_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( MEMCFGCUSTOM_H )
#  include <memCfgCustom.h>
#endif

/* ---- Constants and Types ---------------------------------------------- */

/**
*  @defgroup   memCfg   Memory Configuration
*  @ingroup    mem
*
*  @brief   Default values for configurable parameters.
*
*  All of the values in this file are defaults which can be overridden by
*  defining values for them in the project specific memCfgCustom.h
*  file.
*
*  @{
*/

/*-------------------------------------------------------------------------*/
/**
*  Determines the alignment of memory blocks.
*  
*  For non-DSP processors, we typically want to return 64 bit aligned data
*  (since this is how big doubles are).
*
*  Because of this, our header is big enough to store two pointers (or
*  two size_t's), so we store our block size and link pointer in the header
*  rather than introduce the complexity of only linking together free blocks.
*
*  The macro MEM_CFG_BLOCK_BOUNDARY must be equal to a power of 2 minus 1, and
*  it forms a mask on addresses that can be returned.
*/

#ifndef  MEM_CFG_BLOCK_BOUNDARY
#define  MEM_CFG_BLOCK_BOUNDARY   7u   /* Eight 8-bit bytes                */
#endif

/*-------------------------------------------------------------------------*/
/**
 * Determines if debug tracing is enabled.
 */
 
#ifndef  MEM_CFG_BUFPOOL_TRACE_ENABLED
#define  MEM_CFG_BUFPOOL_TRACE_ENABLED  0
#endif

/*-------------------------------------------------------------------------*/
/**
 * Determines if block debugging is enabled. This causes guards to be placed
 * before and after each memory block, which allows for detection of
 * double frees and memory overwrites.
 */

#ifndef  MEM_CFG_DEBUG
#define  MEM_CFG_DEBUG              1
#endif

/*-------------------------------------------------------------------------*/
/**
 * Determines if debug location tracking is enabled. This causes the file
 * name and line number of each block to be stored with the block.
 */
 
#ifndef  MEM_CFG_DEBUG_LOCATION
#define  MEM_CFG_DEBUG_LOCATION     1
#endif

/*-------------------------------------------------------------------------*/
/**
 * This causes the contents of the buffer pool to be dumped, if an 
 * allocation fails. This requires the use of 'printf'.
 */
 
#ifndef  MEM_CFG_DUMP_ON_FAILURE
#define  MEM_CFG_DUMP_ON_FAILURE    1
#endif

/*-------------------------------------------------------------------------*/
/**
 * This causes the additional queue checking code to be performed just 
 * prior and just after every queue insert and delete.
 */
 
#ifndef  MEM_CFG_QUEUE_CHECK_ENABLED
#define  MEM_CFG_QUEUE_CHECK_ENABLED   1
#endif

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
*
****************************************************************************/

#endif /* MEMCFG_H */

