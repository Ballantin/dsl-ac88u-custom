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
*  @file    bosSleep.c
*
*  @brief   LinuxKernel implementation of the BOS Sleep Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <kernelMainWrapper.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <bosCfg.h>
#include <linux/delay.h>

#if BOS_CFG_SLEEP

#include <string.h>
#include <bosError.h>
#include <bosSleep.h>
#include <bosSleepPrivate.h>
#include <bosErrorLinuxKernel.h>
#include <bosLinuxKernel.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosSleepInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosSleepInit( void )
{
   BOS_ASSERT( !gBosSleepInitialized );

   bosSleepInitFunctionNames();

   gBosSleepInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosSleepInit */

/***************************************************************************/

BOS_STATUS bosSleepTerm( void )
{
   BOS_ASSERT( gBosSleepInitialized );

   gBosSleepInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosSleepTerm */

/***************************************************************************/

BOS_STATUS bosSleep( BOS_TIME_MS mSec )
{
   unsigned long state = 0;
   wait_queue_head_t wait;
   unsigned int timeout;
   int ticks = BOS_MS_TO_TICKS( mSec );

   BOS_ASSERT( gBosSleepInitialized );

   init_waitqueue_head(&wait);
   timeout = interruptible_sleep_on_timeout(&wait, ticks );
   if ( timeout != 0 )
   {
      spin_lock_irqsave(&current->sighand->siglock, state); 
      if ( sigismember(&current->pending.signal, SIGKILL) ||
           sigismember(&current->pending.signal, SIGTERM) ||
           sigismember(&current->signal->shared_pending.signal, SIGKILL) ||
           sigismember(&current->signal->shared_pending.signal, SIGTERM) 
         )
      {
         spin_unlock_irqrestore(&current->sighand->siglock, state); 
         return ( BOS_STATUS_EXIT );
      }
      spin_unlock_irqrestore(&current->sighand->siglock, state); 
   }

   return ( BOS_STATUS_OK );

} /* bosSleep */

/***************************************************************************/

BOS_STATUS bosSleepNs( BOS_TIME_NS nSec )
{
   BOS_ASSERT( gBosSleepInitialized );

   ndelay( nSec );
   
   return( BOS_STATUS_OK );
   
} /* bosSleepNs */

/** @} */

#endif   /* BOS_CFG_SLEEP */

