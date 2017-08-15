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
*    Filename: exceptionHdlrCmtMips.c
*
****************************************************************************
*    Description:
*
*     Implements a generic exception handler task for a concurrent multi-threaded
*     (CMT) processor. The module will register with the inter-thread processor
*     communication module to be notified when an exception occurs on the
*     other thread processor. This notification will trigger an ISR,
*     which will signal the exception handler task. The task will invoke
*     a user registered callback to perform application specific exception
*     handling.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <bosSem.h>
#include <bosTask.h>
#include <exceptionHdlrCmtMips.h>
#include <exceptionHdlrCmtMipsCfg.h>
#include <itpcSharedCmds.h>
#include <xdrvLog.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* Sema4 used by the ISR to signal interrupt service task that an exception occurred. */
static BOS_SEM                      gExceptionSignalSema4;

/* Interrupt service task signalled by ISR. */
static BOS_TASK_ID                  gExceptionTaskId;


/* User registered callback, invoked when an exception occurs. */
static EXCEPTION_HDLR_CMT_CALLBACK        gCallbackFnc;
static EXCEPTION_HDLR_CMT_CALLBACK_PARM   gCallbackParam;

/* Inter-thread processor communication driver used to communication with other
** thread processor. */
static XDRV_ITPC    *gItpcDrv;


/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int ExceptionIsr( volatile XDRV_ITPC_CMD*, XDRV_ITPC_CMD_HANDLER_PARM );
static void ExceptionIstMain( BOS_TASK_ARG taskArg );


/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   exceptionHdlrCmtInit
**
** PURPOSE:    To initialize the CMT exception handler module.
**
** PARAMETERS: callbackFnc    (in)  Callback invoked when exception occurs on
**                                  other thread processor.
**             callbackParam  (in)  Parameter passed to callback function.
**             itpcDrv        (in)  Inter-thread processor communication
**                                  driver used to communication with other
**                                  thread processor.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrCmtInit
(
   EXCEPTION_HDLR_CMT_CALLBACK         callbackFnc,
   EXCEPTION_HDLR_CMT_CALLBACK_PARM    callbackParam,
   XDRV_ITPC                          *itpcDrv
)
{
   gCallbackFnc   = callbackFnc;
   gCallbackParam = callbackParam;
   gItpcDrv       = itpcDrv;


   /*
   ** Create sema4 that will be used by the ISR to signal the interrupt
   ** service task of exceptions on other thread processors.
   */
   if ( bosSemCreate( "CMT_EXCEPTION_IST_SEMA4", 0, 1, &gExceptionSignalSema4 ) != BOS_STATUS_OK )
   {
      XCHG_ASSERT( 0 );
   }

   /*
   ** Create an interrupt service task for DSP assertions.
   */
   gExceptionTaskId = 0;
   if ( BOS_STATUS_OK != bosTaskCreateEx( "CMT_EXCEPTION_IST",
                                          (8 * 1024),
                                          EXCEPTION_HDRL_CMT_CFG_TASK_PRIORITY,
                                          NULL,
                                          ExceptionIstMain,
                                          NULL,
                                          0,
                                          &gExceptionTaskId ) )
   {
      XCHG_ASSERT( 0 );
   }


   /*
   ** Install handler for remote exception.
   */
   xdrvItpcRegisterHandler( gItpcDrv,
                            ITPC_EXCEPTION_NOTIFY_CMD_ID,
                            ExceptionIsr,
                            NULL );
}


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrCmtDeinit
**
** PURPOSE:    To de-initialize the CMT exception handler module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrCmtDeinit( void )
{
   /*
   ** Uninstall handler for remote exception.
   */
   xdrvItpcUnregisterHandler( gItpcDrv, ITPC_EXCEPTION_NOTIFY_CMD_ID );


   if ( gExceptionTaskId != 0 )
   {
      /* Currently, the interrupt service task should be blocked in the reset-
      ** acknowledgement state. Advance the task to the reset-active state.
      ** This will cause the task to resume operation. In this state, the task
      ** should perform any operations required for proper shutdown before
      ** exiting from it's main task function.
      */
      bosTaskProceedToShutdown( &gExceptionTaskId );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &gExceptionTaskId );
   }


   /*
   ** De-allocate sema4.
   */
   if ( bosSemDestroy( &gExceptionSignalSema4 ) != BOS_STATUS_OK )
   {
      XCHG_ASSERT( 0 );
   }
}


/*
*****************************************************************************
** FUNCTION:   ExceptionIsr
**
** PURPOSE:
**
** PARAMETERS:
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static unsigned int ExceptionIsr
(
   volatile XDRV_ITPC_CMD       *data,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
)
{
   /* Not used. */
   (void) data;
   (void) parm;


   /* Signal the interrupt service task. */
   bosSemGive( &gExceptionSignalSema4 );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   ExceptionIstMain
**
** PURPOSE:    Interrupt service task that is signalled by the interrupt
**             service routine when a remote exception occurs on another
**             thread processor.
**
** PARAMETERS: taskArg  (in)
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void ExceptionIstMain( BOS_TASK_ARG taskArg )
{

   while ( 1 )
   {
      BOS_STATUS status;
      /*
      ** Wait for an event from the DSP indicating that an assertion has occurred.
      */
      status = bosSemTake( &gExceptionSignalSema4 );
      if ( status == BOS_STATUS_EXIT )
      {
         XDRV_LOG_NOTICE(( XDRV_LOG_MOD_XDRV, "Exiting task CMT_EXCEPTION_IST" ));
         /* The CAS task should stop running by exiting from this main function */
         return;
      }

      /*
      ** Note that we do not check the return code of the bosSemTake above for
      ** a BOS_STATUS_RESET return code. Instead, we query BOS to determine
      ** if a reset is pending. This is a subtle difference.
      **
      ** If we simply check the return code, it is possible that this task has
      ** been notified of a reset, but that it hasn't yet realized that a reset
      ** has occurred. Tasks only realize that a reset has occurred on
      ** reset granularity boundaries.
      **
      ** This is important because the DSP may have asserted because an
      ** application reset was initiated. This will cause this module's ISR
      ** to give the signaling sema4. This may occur _before_ this task
      ** realizes that it has been notified of a reset. In this case, we will
      ** think that this is a normal DSP assert, and assert (on the host) below.
      ** This may prevent the application reset from succeeding.
      **
      ** Instead, if we query BOS to determine if a reset is pending, this
      ** eliminates the situation above. We will know for sure whether a
      ** task reset is pending. In this case, we can choose to ignore the DSP
      ** reset, since the DSP will be hard-reset during a software application
      ** reset.
      **
      ** Clear as mud? :-)
      */

      if ( bosTaskIsResetPending() )
      {
         /*
         ** This task has been notified that a reset is pending. Acknowledge
         ** the notification and then block until the we are resumed.
         */
         bosTaskResetAck();


         /* The task should stop running by exiting from this main function */
         return;
      }


      /*
      ** Call the ASSERT callback.
      */
      if( gCallbackFnc != NULL )
      {
         (*gCallbackFnc)(gCallbackParam);
      }
   }
}
