/***************************************************************************
*    Copyright 2000  Broadcom Corporation
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
*    Filename: lhapiThreadDispatchHostOnly.c
*
****************************************************************************
*    Description:
*
*     Implements functions related to dispatching LDX hausware threads
*     when they are ready to run.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <bosTask.h>
#include <bosSem.h>
#include <knl_framesync.h>
#include "lhapiPrivate.h"
#include <vrgLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Task properties */
#define LHAPI_THREAD_DISPATCH_TASK_NAME            "Lhapi Thread Dispatch"
#define LHAPI_THREAD_DISPATCH_TASK_STACKSIZE       ( 1024 * 8 )

#ifndef LHAPI_THREAD_DISPATCH_TASK_PRIORITY
#define LHAPI_THREAD_DISPATCH_TASK_PRIORITY        BOS_TASK_CLASS_HIGH
#endif


/* ---- Private Variables ------------------------------------------------ */

/* Task and timer objects. */
static BOS_TASK_ID      gThreadDispatchTaskId;
static BOS_SEM          gThreadDispatchSema4;

/* ---- Private Function Prototypes -------------------------------------- */

static void ThreadDispatchTaskMain( BOS_TASK_ARG taskArg );


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchInit.
**
** PURPOSE:    Init lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchInit( void )
{
   BOS_STATUS        status;


   /*
   ** Create a semaphore that will be used to signal the task created below
   ** to run the hausware kernel.
   */
   status = bosSemCreate( "lhapiThreadDispatchSem", 0, 1, &gThreadDispatchSema4 );
   XCHG_ASSERT( status == BOS_STATUS_OK );


   /*
   ** Create the task within who's context hausware will run.
   */
   gThreadDispatchTaskId = 0;
   status = bosTaskCreateEx( LHAPI_THREAD_DISPATCH_TASK_NAME,
                             LHAPI_THREAD_DISPATCH_TASK_STACKSIZE,
                             LHAPI_THREAD_DISPATCH_TASK_PRIORITY,
                             NULL,
                             ThreadDispatchTaskMain,
                             NULL,
                             0,
                             &gThreadDispatchTaskId );

   XCHG_ASSERT( status == BOS_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchDeinit.
**
** PURPOSE:    De-initialize lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchDeinit( void )
{
   /* Shutdown task */
   if ( gThreadDispatchTaskId != 0 )
   {
      /* Currently, the task should be blocked in the reset-acknowledgement state.
       * Advance the task to the reset-active state. This will cause the task to
       * resume operation. In this state, the task should perform any operations
       * required for proper shutdown before exiting from its main task function. */
      bosTaskProceedToShutdown( &gThreadDispatchTaskId );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &gThreadDispatchTaskId );
   }


   /* Cleanup. */
   bosSemDestroy( &gThreadDispatchSema4 );
}


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchHostOnly
**
** PURPOSE:    This function is called by hausware when it has threads that
**             are ready to run. It is the application's responsibility to
**             dispatch the threads by calling KNL_RunReadyThreads(). Note that
**             lhapiKnlScheduleReadyThreads may be called in ISR context. The
**             application must decide whether to invoke KNL_RunReadyThreads()
**             in ISR context or task context. If it invokes it in ISR context,
**             interrupts MUST be enabled prior to calling KNL_RunReadyThreads().
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchHostOnly( void )
{
   /*
   ** This function may be called in ISR context. We should transfer to task
   ** context before running the hausware threads. So, let's signal a task, and
   ** the have it call KNL_RunReadyThreads().
   */
   bosSemGive( &gThreadDispatchSema4 );
}


/*
****************************************************************************
** FUNCTION:   ThreadDispatchTaskMain
**
** PURPOSE:    Main task function for DSP processing timer task. This task
**             simply calls the callback registered by the client.
**
** PARAMETERS: taskArg - not used
**
** RETURNS:    none
**
** NOTE:
******************************************************************************
*/
static void ThreadDispatchTaskMain( BOS_TASK_ARG taskArg )
{
   BOS_STATUS  status;

   while ( 1 )
   {
      /* Wait to be notified to run hausware threads. */
      status = bosSemTake( &gThreadDispatchSema4 );

      if ( status == BOS_STATUS_RESET )
      {
         /* This task has been notified that a reset is pending.
          * Acknowledge the notification and then block until the
          * task is resumed. */
         bosTaskResetAck();

         /* The task should stop running by exiting from this main function */
         return;
      }


      /*
      ** Call the hausware kernel to schedule tasks that are ready to run.
      */
      KNL_RunReadyThreads();
   }
}
