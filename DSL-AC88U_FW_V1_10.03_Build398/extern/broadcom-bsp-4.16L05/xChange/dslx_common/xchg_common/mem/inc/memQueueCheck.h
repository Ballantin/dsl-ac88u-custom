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
*  @file    memQueueCheck.h
*
*  @brief   Contains macros to aid in debugging queues.
*
****************************************************************************/

#if !defined( MEMQUEUECHECK_H )
#define MEMQUEUECHECK_H       /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSTYPES_H )
#  include <bosTypes.h>
#endif

#if !defined( MEMQUEUE_H )
#  include <memQueue.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/**
 * @addtogroup memQueue
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

#if MEM_CFG_QUEUE_CHECK_ENABLED
#  if MEM_CFG_DEBUG_LOCATION
#     if !defined( BOSERROR_H )
#        include <bosError.h>
#     endif

#     define   MEM_CHECK_QUEUE(q)   \
      if ( !memCheckQueue( q ))    \
      {                             \
         BOS_LOG(( "memCheckQueue failed: file %s, line %d, Called from file %s, line %d\n", \
                   __FILE__, __LINE__, fileName, lineNum )); \
         BOS_ASSERT( FALSE );  \
      }
#  else
#     define   MEM_CHECK_QUEUE(q)   BOS_ASSERT( memCheckQueue( q ))
#  endif
#else
#  define   MEM_CHECK_QUEUE(q)
#endif


/* ---- Variable Externs ------------------------------------------------- */

extern BOS_BOOL gMemForceQueueCheckFailure;

/* ---- Function Prototypes ---------------------------------------------- */

BOS_BOOL memCheckQueue( MEM_QUEUE *queue );

/** @} */

#if defined( __cplusplus )
}
#endif

#endif /* MEMQUEUECHECK_H */

