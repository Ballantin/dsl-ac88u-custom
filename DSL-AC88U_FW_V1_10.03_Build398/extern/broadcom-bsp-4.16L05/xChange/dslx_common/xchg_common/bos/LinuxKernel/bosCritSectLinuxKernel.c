/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosCritSectLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Critical Section Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_CRITSECT

#include <linux/sched.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosCritSect.h>
#include <bosCritSectPrivate.h>
#include <bosMutex.h>

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_CRITSECT_USE_GLOBAL
BOS_CRITSECT gBosGlobalCritSect;
BOS_CRITSECT *BOS_CFG_GLOBAL_CRITSECT = &gBosGlobalCritSect;
#endif

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gbosCritSectInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosCritSectInit( void )
{
   BOS_ASSERT( !gbosCritSectInitialized );

   bosCritSectInitFunctionNames();

#if BOS_CFG_CRITSECT_USE_GLOBAL
   bosCritSectCreate( BOS_CFG_GLOBAL_CRITSECT );
#endif

   gbosCritSectInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosCritSectInit */

/***************************************************************************/

BOS_STATUS bosCritSectTerm( void )
{
   BOS_ASSERT( gbosCritSectInitialized );

   gbosCritSectInitialized = BOS_FALSE;

#if BOS_CFG_CRITSECT_USE_GLOBAL
   bosCritSectDestroy( BOS_CFG_GLOBAL_CRITSECT );
#endif

   return BOS_STATUS_OK;

} /* bosCritSectTerm */

/***************************************************************************/

BOS_STATUS bosCritSectCreate( BOS_CRITSECT *critSect )
{
   BOS_MUTEX *critSectMutex = NULL;
   
   critSectMutex = kmalloc(sizeof(BOS_MUTEX), GFP_KERNEL);
   BOS_ASSERT(critSect);
   
   if ( bosMutexCreate("MUTEX", critSectMutex) != BOS_STATUS_OK )
   {
      printk("%s: bosMutexCreate failed\n", __FUNCTION__);
      kfree(critSectMutex);
      return ( BOS_STATUS_ERR );
   }
   
   *critSect = critSectMutex;

   return ( BOS_STATUS_OK );

} /* bosCritSectCreate */

/***************************************************************************/

BOS_STATUS bosCritSectDestroy( BOS_CRITSECT *critSect )
{
   BOS_STATUS status = BOS_STATUS_OK;

   BOS_ASSERT(critSect && *critSect);

   if ( bosMutexDestroy(*critSect) != BOS_STATUS_OK )
   {
      printk("%s: bosMutexDestroy failed\n", __FUNCTION__);
      status = BOS_STATUS_ERR;
   }
   kfree(*critSect);
   
   return ( status );

} /* bosCritSectDestroy */

/***************************************************************************/

BOS_STATUS bosCritSectEnter( BOS_CRITSECT *critSect )
{
   BOS_ASSERT(critSect && *critSect);

   if ( bosMutexAcquire(*critSect) != BOS_STATUS_OK )
   {
      printk("%s: bosMutexAcquire failed\n", __FUNCTION__);
      return ( BOS_STATUS_ERR );
   }

   return ( BOS_STATUS_OK );

} /* bosCritSectEnter */

/***************************************************************************/

BOS_STATUS bosCritSectLeave( BOS_CRITSECT *critSect )
{
   BOS_ASSERT(critSect && *critSect);

   if ( bosMutexRelease(*critSect) != BOS_STATUS_OK )
   {
      printk("%s: bosMutexRelease failed\n", __FUNCTION__);
      return ( BOS_STATUS_ERR );
   }

   return ( BOS_STATUS_OK );

} /* bosCritSectLeave */

#endif   /* BOS_CFG_CRITSECT */

