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
*    Filename: boardHalDspTask.c
*
****************************************************************************
*    Description:
*
*     This file implements the task that periodically invokes the
*     DSP kernel processing task.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosTask.h>
#include <bosTimer.h>
#include <boardHalDspTask.h>
#include <xchgAssert.h>
#include <bosSem.h>
#include <vrgLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/* Specify the duration (in ms) of the DSP processing timer task. */
#define DSP_TIMER_PERIODIC_RATE_MSEC   5

/* Task properities */
#define DSP_TIMER_TASK_NAME            "HTSK"
#define DSP_TIMER_TASK_STACKSIZE       ( 1024 * 8 )

#ifndef DSP_TIMER_TASK_PRIORITY
#define DSP_TIMER_TASK_PRIORITY        BOS_TASK_CLASS_HIGH
#endif


/* ---- Private Variables ------------------------------------------------ */

/* Task and timer objects. */
static BOS_TASK_ID       gTimerTaskId;
static BOS_TIMER         gTickTimer;
static BOS_SEM           gTickSem;                 

/*
** Callback invoked periodically by the HAL. Used to invoke the voice
** processing kernel.
*/
static BOARD_HAL_DSP_TASK_FUNC  gTimerCallback;

/*
** Global flag to indicate whether DSP is to be halted
*/
static VRG_BOOL      gHaltDsp;


/* ---- Private Function Prototypes -------------------------------------- */

static void DspTimerTaskMain     ( BOS_TASK_ARG taskArg );
static void DspTimerTaskInitCB   ( BOS_TASK_ARG taskArg );
static void DspTimerTaskDeinitCB ( BOS_TASK_ARG taskArg );
static void DspTimerHandler      ( unsigned long data   );


/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:      boardHalDspTaskInit
**
** PURPOSE:       Initialization of the DSP timer processing task.
**
** PARAMETERS:    None.
**
** RETURNS:       0 on success, -1 on failure.
**
** NOTE:
*****************************************************************************
*/
int boardHalDspTaskInit( void )
{
   BOS_STATUS        status;

   gTimerCallback = NULL;

   gTimerTaskId = 0;
   status = bosTaskCreateEx( DSP_TIMER_TASK_NAME,
                             DSP_TIMER_TASK_STACKSIZE,
                             DSP_TIMER_TASK_PRIORITY,
                             DspTimerTaskInitCB,
                             DspTimerTaskMain,
                             DspTimerTaskDeinitCB,
                             0,
                             &gTimerTaskId );

   if ( status == BOS_STATUS_OK )
   {
      gHaltDsp = VRG_FALSE;
      return ( 0 );
   }
   else
   {
      return ( -1 );
   }
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskRegisterCallback
**
** PURPOSE:    Registers callback with the HAL that should be invoked
**             periodically in order to invoke the kernel of the voice
**             processing stack. The timer implementation is HAL specific
**             since it may have dependencies on APM/TDM drivers, or require
**             communication with the DSP HAL.
**
** PARAMETERS: Callback to invoke periodically.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskRegisterCallback( BOARD_HAL_DSP_TASK_FUNC timerFunc )
{
   gTimerCallback = timerFunc;
}


/*
*****************************************************************************
** FUNCTION:   DspTimerTaskInitCB
**
** PURPOSE:    DSP task initialization function. This function will be
**             automatically called by the OS prior to invoking the
**             task's main function. Task OS resource allocation and
**             initialization may be performed here.
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************
*/
static void DspTimerTaskInitCB( BOS_TASK_ARG taskArg )
{
   bosSemCreate( "HTSK",
                 0,
                 1,
                 &gTickSem );

   /* Initialize timer parameters */
   gTickTimer.data = 0;
   gTickTimer.function = DspTimerHandler;

   /* Create timer used to drive DSP processing task. */
   bosTimerCreate( &gTickTimer );
}


/*
*****************************************************************************
** FUNCTION:   DspTimerTaskDeinitCB
**
** PURPOSE:    DSP task deinitialization function. This function will be
**             automatically called by the OS after the task's main
**             function has exited. Any task OS resources allocated by the
**             task initialization function (DspTimerTaskInitCB) should be
**             de-allocated here.
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************
*/
static void DspTimerTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Cleanup */
   bosTimerDestroy( &gTickTimer );
   bosSemDestroy( &gTickSem );
}


/*****************************************************************************
** FUNCTION:   DspTimerTaskMain
**
** PURPOSE:    Main task function for DSP processing timer task. This task
**             simply calls the callback registered by the client.
**
** PARAMETERS: taskArg - not used
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************/
static void DspTimerTaskMain( BOS_TASK_ARG taskArg )
{
   BOS_STATUS  status;

   while ( 1 )
   {
      /* sleep before next execution */
      status = bosSemTake( &gTickSem );
      if ( status == BOS_STATUS_RESET )
      {
         /* This task has been notified that a reset is pending.
          * Acknowledge the notification and then block until the
          * task is resumed. */
         bosTaskResetAck();

         /* The task should stop running by exiting from this main function */
         return;
      }
      else if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Exiting task %s ", DSP_TIMER_TASK_NAME ));
         return;
      }

      /* Do not invoke the DSP task if we have been told to halt the DSP */
      if ( gHaltDsp != VRG_TRUE )
      {
         /* Invoke the client register callback. */
         if ( gTimerCallback != NULL )
         {
            (*gTimerCallback)(0);
         }
      }
   }
}

void DspTimerHandler(unsigned long data)
{
   (void)data;

   /* Release the HTSK semaphore */
   bosSemGive( &gTickSem );

   /* Reset time to trigger after 5ms */
   bosTimerStart( &gTickTimer, DSP_TIMER_PERIODIC_RATE_MSEC, BOS_TIMER_MODE_REPEAT );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskDeinit
**
** PURPOSE:    DSP task deinit - call once during system shutdown.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that this function is only called after the
**             task has been notified of a pending application reset, and it
**             has acknowledged the notification. This implies that the
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
void boardHalDspTaskDeinit( void )
{
   /* Shutdown task */
   if ( gTimerTaskId != 0 )
   {
      /* Currently, the task should be blocked in the reset-acknowledgement state.
       * Advance the task to the reset-active state. This will cause the task to
       * resume operation. In this state, the task should perform any operations
       * required for proper shutdown before exiting from its main task function. */
      bosTaskProceedToShutdown( &gTimerTaskId );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &gTimerTaskId );
   }
}


/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskHalt
**
** PURPOSE:    Halt the DSP task
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskHalt( void )
{
   gHaltDsp = VRG_TRUE;
}

/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskResume
**
** PURPOSE:    Resume the DSP task
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskResume( void )
{
   gHaltDsp = VRG_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   boardHalDspTaskStartTimer
**
** PURPOSE:    Start the DSP task timer.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boardHalDspTaskStartTimer( void )
{
   /* Start the timer in repeating mode. */
   bosTimerStart( &gTickTimer,
                  DSP_TIMER_PERIODIC_RATE_MSEC,
                  BOS_TIMER_MODE_REPEAT );
}


