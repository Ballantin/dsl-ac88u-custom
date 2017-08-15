/****************************************************************************
*
* Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom Corporation
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom Corporation
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosTimerECOS.c
*
*  @brief   ECOS implementation of the BOS Timer Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_TIMER

#include <stdio.h>
#include <string.h>
#include <bosError.h>
#include <bosErrorLinuxUser.h>
#include <bosLog.h>
#include <bosSleep.h>
#include <bosTimer.h>
#include <bosTimerPrivate.h>
#include <bosLinuxUser.h>
#include <bosEvent.h>
#include <xchgAssert.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosTimerInitialized = BOS_FALSE;


/* ---- Private Function Prototypes -------------------------------------- */

static void bosTimerLinuxUserMain( BOS_TASK_ARG taskArg );
static void bosTimerTaskInitCB( BOS_TASK_ARG taskArg );
static void bosTimerTaskDeinitCB( BOS_TASK_ARG taskArg );


/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosTimerInit( void )
{
   BOS_ASSERT( !gBosTimerInitialized );

   blogPrintf(("bosTimerInit\n"));

   bosTimerInitFunctionNames();

   gBosTimerInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosTimerInit */

/***************************************************************************/

BOS_STATUS bosTimerTerm( void )
{
   BOS_ASSERT( gBosTimerInitialized );

   gBosTimerInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosTimerTerm */

/***************************************************************************/

BOS_STATUS bosTimerCreate( BOS_TIMER *timer )
{
   BOS_STATUS            status;
   BOS_TASK_EXTENDED_ARG argument = { BOS_TASK_AFFINITY_ALL_TP, timer };

   blogPrintf(("bosTimerCreate\n"));

   BOS_ASSERT( gBosTimerInitialized );

   memset( timer, 0, sizeof( *timer ) );

   timer->lastTimeoutMsec  = BOS_WAIT_FOREVER;
 
   status = bosTaskGetMyTaskId( &timer->taskId );
   XCHG_ASSERT( status == BOS_STATUS_OK );

   status = bosEventCreate( &timer->taskId, &timer->event );
   XCHG_ASSERT( status == BOS_STATUS_OK );


   /* Get the kernel to create the alarm object, associated with the realtime
   ** clock.  We need to specify the callout function (and data) that will be
   ** called every time the alarm occurs. */
   status = bosTaskCreateEx( BOS_TIMER_LINUXUSER_TASK_NAME,
                           BOS_TIMER_LINUXUSER_TASK_STACK,
                           BOS_TIMER_LINUXUSER_TASK_PRIORITY,
                           bosTimerTaskInitCB,
                           bosTimerLinuxUserMain,
                           bosTimerTaskDeinitCB,
                           &argument,
                           &(timer->timerId));

   return ( BOS_STATUS_OK );

} /* bosTimerCreate */

/***************************************************************************/

BOS_STATUS bosTimerDestroy( BOS_TIMER *timer )
{
   BOS_ASSERT( gBosTimerInitialized );
   BOS_ASSERT( timer  != NULL );

   if ( timer->bIsRunning )
   {
      bosTimerStop( timer );
   }

   bosTaskDestroy( &timer->timerId );

   bosEventDestroy( &timer->event );


   return ( BOS_STATUS_OK );

} /* bosTimerDestroy */

/***************************************************************************/

BOS_STATUS bosTimerStart
(
   BOS_TIMER        *timer,
   unsigned int      timeoutMsec,
   BOS_TIMER_MODE    mode
)
{
   BOS_TASK_ID    taskId;

   blogPrintf(("bosTimerStart\n"));
   /* Error checks. */
   bosTaskGetMyTaskId( &taskId );
   if ( taskId != timer->taskId )
   {
      blogStrErr(( gBosBlog, "bosTimerStart - Calling task does not own timer" ));
      return ( BOS_STATUS_ERR );
   }

   /* If the timer is already running, then stop it before restarting it. */
   if ( timer->bIsRunning )
   {
      bosTimerStop( timer );
   }

   /* Store these for use by the Restart() method in the base class. */
   timer->lastTimeoutMsec  = timeoutMsec;
   timer->lastTimerMode    = mode;

   /* Clear any pending event notifications. */
   bosEventClear( &timer->event );

   /* Start the alarm. */
   bosEventSend( &timer->eventStart );

   return ( BOS_STATUS_OK );

} /* bosTimerStart */

/***************************************************************************/

BOS_STATUS bosTimerRestart( BOS_TIMER *timer )
{
   blogPrintf(("bosTimerRestart\n"));
   /* See if Start() has been called; if not, then we can't do this. */
   if ( !timer->bIsRunning )
   {
      blogStrErr(( gBosBlog, "bosTimerRestart - Timer not started. Can't restart..." ));
      return ( BOS_STATUS_ERR );
   }

   /* Start the timer with the previous values. */
   return ( bosTimerStart( timer, timer->lastTimeoutMsec, timer->lastTimerMode ) );

} /* bosTimerRestart */

/***************************************************************************/

BOS_STATUS bosTimerStop( BOS_TIMER *timer )
{
   BOS_TASK_ID    taskId;

   blogPrintf(("bosTimerStop\n"));

   /* Error checks. */
   bosTaskGetMyTaskId( &taskId );
   if ( taskId != timer->taskId )
   {
      blogStrErr(( gBosBlog, "bosTimerStop - Calling task does not own timer" ));
      return ( BOS_STATUS_ERR );
   }


   if ( timer->bIsRunning )
   {
      timer->lastTimeoutMsec  = BOS_WAIT_FOREVER;
      bosEventSend( &timer->eventStop );
   }

   return ( BOS_STATUS_OK );

} /* bosTimerStop */

/***************************************************************************/

BOS_STATUS bosTimerWait( BOS_TIMER *timer )
{
   BOS_TASK_ID    taskId;

   blogPrintf(("bosTimerWait\n"));

   /* Error checks... */
   if ( !timer->bIsRunning )
   {
      blogStrErr(( gBosBlog, "bosTimerWait - Timer not running. Can't wait for it..." ));
      return ( BOS_STATUS_ERR );
   }

   bosTaskGetMyTaskId( &taskId );
   if ( taskId != timer->taskId )
   {
      blogStrErr(( gBosBlog, "bosTimerWait - Calling task does not own timer" ));
      return ( BOS_STATUS_ERR );
   }


#if BOS_CFG_RESET_SUPPORT
   /* Wait on the timer event that will be posted by the timer callout.
   ** Note that we need to call the reset-able version of EventReceive so
   ** that we will unblock if there is an application reset. */
   return ( bosEventReceiveR( &timer->event ) );
#else
   /* Wait on the timer event. */
   return ( bosEventReceive( &timer->event ) );
#endif

} /* bosTimerWait */


/***************************************************************************/

BOS_STATUS bosTimerGetEvent( BOS_TIMER *timer, BOS_EVENT **event )
{
   blogPrintf(("bosTimerGetEvent\n"));

   *event = &timer->event;

   return ( BOS_STATUS_OK );
}



static void bosTimerTaskInitCB( BOS_TASK_ARG taskArg )
{
   BOS_TIMER *timer = (BOS_TIMER *)taskArg;

   XCHG_ASSERT( bosEventSetCreate( &timer->timerId, &timer->apiEventSet ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventCreate( &timer->timerId, &timer->eventStart ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventCreate( &timer->timerId, &timer->eventStop ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventCreate( &timer->timerId, &timer->eventRestart ) == BOS_STATUS_OK);

   XCHG_ASSERT( bosEventSetAddEvent( &timer->apiEventSet, &timer->eventStart ) == BOS_STATUS_OK );
   XCHG_ASSERT( bosEventSetAddEvent( &timer->apiEventSet, &timer->eventStop ) == BOS_STATUS_OK );
   XCHG_ASSERT( bosEventSetAddEvent( &timer->apiEventSet, &timer->eventRestart ) == BOS_STATUS_OK );

}

static void bosTimerTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   BOS_TIMER *timer = (BOS_TIMER *)taskArg;

   /* Cleanup */
   XCHG_ASSERT( bosEventSetRemoveAllEvents( &timer->apiEventSet ) == BOS_STATUS_OK);

   XCHG_ASSERT( bosEventDestroy( &timer->eventStart ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventDestroy( &timer->eventStop ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventDestroy( &timer->eventRestart ) == BOS_STATUS_OK);

   XCHG_ASSERT( bosEventSetDestroy( &timer->apiEventSet ) == BOS_STATUS_OK);
}

/**
 * @addtogroup bosTimerInternal
 * @{
 */

/***************************************************************************/
/**
*  eCos alarm handler function. This will be invoked each time the alarm
*  timer expires.
*
*  @param   alarmHandle (in)  Alarm id.
*  @param   data        (in)  User-specified data.
*/

static void bosTimerLinuxUserMain( void *data )
{
   BOS_STATUS status;
   BOS_TIMER  *timer = (BOS_TIMER *) data;

   while(BOS_TRUE)
   {
      /* Signal that the timer expired by sending the event. */
      status = bosEventSetTimedReceive( &timer->apiEventSet, BOS_EVENT_SET_WAIT_MODE_ANY, timer->lastTimeoutMsec );

      /* check the reason why we are awaken */ 
      if(status == BOS_STATUS_RESET )
      {
         /* this timer will be destroyed, clear any pending events */
         bosEventClear( &timer->event );
         break;
      }

      else if(status == BOS_STATUS_TIMEOUT /* && timer->bIsRunning == BOS_TRUE */ )
      {
         /* timer is running and timeout */
         bosEventSend( &timer->event );
      }
      else if(status == BOS_STATUS_OK )
      {
         /* get event from timer api interface */
         if( bosEventSetIsEventSet( &timer->apiEventSet, &timer->eventStart ) )
	 {
            /* timer started or restart*/
            timer->bIsRunning      = BOS_TRUE;
	 }
         else if( bosEventSetIsEventSet( &timer->apiEventSet, &timer->eventStop ) )
	 {
            timer->bIsRunning      = BOS_FALSE;
	 }
      }
   }
}

#endif   /* BOS_CFG_TIMER */
