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
#include <boardHalDspTask.h>
#include <xchgAssert.h>
#include <bosSem.h>
#include <vrgLog.h>

#if defined(BRCM_63138) || defined(BRCM_63148)
#include <xdrvNeon.h>
#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#if defined(BRCM_63138) || defined(BRCM_63148)
#define read_counters(x)    asm volatile("mrc p15, 0, %0, c9, c13, 0; isb" : "=r" (x))
#else
#define read_counters(x)   __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (x))
#endif

unsigned long startDspTaskCyc = 0;
unsigned long endDspTaskCyc = 0;
unsigned long lastDspTaskCyc = 0;
unsigned long dspTaskRunCyc;
unsigned long dspTaskIntRunCyc;

/* Task properities */
#define DSP_TASK_NAME            "HTSK"
#define DSP_TASK_STACKSIZE       ( 1024 * 8 )

#ifndef DSP_TASK_PRIORITY
#define DSP_TASK_PRIORITY        BOS_TASK_CLASS_HIGH
#endif
#define HTSK_SLEEP_DELAY         100


/* ---- Private Variables ------------------------------------------------ */

/* Task objects. */
static BOS_TASK_ID       gDspTaskId;
static BOS_SEM           gTickSem;

/*
** Callback invoked periodically by the HAL. Used to invoke the voice
** processing kernel.
*/
static BOARD_HAL_DSP_TASK_FUNC  gDspTaskCallback;

/*
** Global flag to indicate whether DSP is to be halted
*/
static VRG_BOOL      gHaltDsp;


/* ---- Private Function Prototypes -------------------------------------- */

static void DspTaskMain     ( BOS_TASK_ARG taskArg );
static void DspTaskInitCB   ( BOS_TASK_ARG taskArg );
static void DspTaskDeinitCB ( BOS_TASK_ARG taskArg );


/* ---- Functions -------------------------------------------------------- */

extern void dspTaskStatsCopy( unsigned long dspTskRunCyc, unsigned long dspTskIntRunCyc );

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
   BOS_STATUS            status;
#ifdef BRCM_63138
   /* The 63138 has NEON only on the first TP. Thus, we have to pin the DSP task to that TP */
   BOS_TASK_EXTENDED_ARG argument = { BOS_TASK_AFFINITY_TP0, 0 };
#else
   BOS_TASK_EXTENDED_ARG argument = { BOS_TASK_AFFINITY_TP1, 0 };
#endif

   gDspTaskCallback = NULL;

   gDspTaskId = 0;
   status = bosTaskCreateEx( DSP_TASK_NAME,
                             DSP_TASK_STACKSIZE,
                             DSP_TASK_PRIORITY,
                             DspTaskInitCB,
                             DspTaskMain,
                             DspTaskDeinitCB,
                             &argument,
                             &gDspTaskId );

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
void boardHalDspTaskRegisterCallback( BOARD_HAL_DSP_TASK_FUNC dspTaskFunc )
{
   gDspTaskCallback = dspTaskFunc;
}


/*
*****************************************************************************
** FUNCTION:   DspTaskInitCB
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
static void DspTaskInitCB( BOS_TASK_ARG taskArg )
{
   bosSemCreate( "HTSK",
                 0,
                 1,
                 &gTickSem );
}


/*
*****************************************************************************
** FUNCTION:   DspTaskDeinitCB
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
static void DspTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Cleanup */
   bosSemDestroy( &gTickSem );
}


/*****************************************************************************
** FUNCTION:   DspTaskMain
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
static void DspTaskMain( BOS_TASK_ARG taskArg )
{
   BOS_STATUS   status;

#if defined(BRCM_63138) || defined(BRCM_63148)
   /* Make sure the NEON core is powered and enabled */
   VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Checking NEON..." ));
   XCHG_ASSERT( xdrvNeonPowerup() == XDRV_STATUS_OK );
#endif

   while ( 1 )
   {
      /* sleep before next execution or shutdown to happen */
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
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Exiting task %s ", DSP_TASK_NAME ));
         return;
      }
      else if ( status != BOS_STATUS_OK )
      {
         /* If the status is not OK, it means we've timed out. In this case, just try again. */
         continue;
      }

      /* Do not invoke the DSP task if we have been told to halt the DSP */
      if ( gHaltDsp != VRG_TRUE )
      {
         /* Invoke the client register callback. */
         if ( gDspTaskCallback != NULL )
         {
            /* counter register value at the start DSP Task */
            read_counters(startDspTaskCyc);
            if(lastDspTaskCyc == 0)
            {
               /* only for first time */
               lastDspTaskCyc = startDspTaskCyc;
            }

            (*gDspTaskCallback)(0);

            /* counter register value at the end DSP Task */
            read_counters(endDspTaskCyc);

            dspTaskRunCyc = dspTaskRunCyc + (endDspTaskCyc - startDspTaskCyc);
            if(lastDspTaskCyc == 0)
            {
               /* only for first time */
               lastDspTaskCyc = startDspTaskCyc;

            } else
            {
               dspTaskIntRunCyc = dspTaskIntRunCyc + (startDspTaskCyc - lastDspTaskCyc);
               lastDspTaskCyc = startDspTaskCyc;
            }
            
            dspTaskStatsCopy( dspTaskRunCyc, dspTaskIntRunCyc );
         }
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   DspTaskSchedule
**
** PURPOSE:    Releases the semahpore and allows the DSP task to invoke
**             Hausware.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
*****************************************************************************
*/
void DspTaskSchedule(void)
{
   /* Release the HTSK semaphore */
   bosSemGive( &gTickSem );
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
   if ( gDspTaskId != 0 )
   {
      /* Currently, the task should be blocked in the reset-acknowledgement state.
       * Advance the task to the reset-active state. This will cause the task to
       * resume operation. In this state, the task should perform any operations
       * required for proper shutdown before exiting from its main task function. */
      bosTaskProceedToShutdown( &gDspTaskId );
      
      /* Wake the task in case it is sleeping */
      bosSemGive( &gTickSem );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &gDspTaskId );
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

