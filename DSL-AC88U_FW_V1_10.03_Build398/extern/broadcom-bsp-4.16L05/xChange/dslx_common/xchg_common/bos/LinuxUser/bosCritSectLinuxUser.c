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
*  @file    bosCritSectLinuxUser.c
*
*  @brief   LinuxUser implementation of the BOS Critical Section Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_CRITSECT

#include <bosError.h>
#include <bosErrorLinuxUser.h>
#include <bosCritSect.h>
#include <bosCritSectPrivate.h>


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

BOS_STATUS bosCritSectCreate( BOS_CRITSECT* critSect )
{
   return bosMutexCreate( "CRSN", (BOS_MUTEX*)critSect );

} /* bosCritSectCreate */

/***************************************************************************/

BOS_STATUS bosCritSectDestroy( BOS_CRITSECT* critSect )
{
   return bosMutexDestroy( (BOS_MUTEX*)critSect );

} /* bosCritSectDestroy */

/***************************************************************************/

BOS_STATUS bosCritSectEnter( BOS_CRITSECT* critSect )
{
   return bosMutexAcquire( (BOS_MUTEX*)critSect );
} /* bosCritSectEnter */

/***************************************************************************/

BOS_STATUS bosCritSectLeave( BOS_CRITSECT* critSect )
{
   return bosMutexRelease( (BOS_MUTEX*)critSect );

} /* bosCritSectLeave */

#endif   /* BOS_CFG_CRITSECT */

