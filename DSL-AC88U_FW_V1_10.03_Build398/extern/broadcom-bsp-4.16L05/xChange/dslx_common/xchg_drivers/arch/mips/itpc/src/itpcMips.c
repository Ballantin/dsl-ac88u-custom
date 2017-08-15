/****************************************************************************
*
* Copyright © 2000-2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: itpcMips.c
*
****************************************************************************
*    Description:
*
*     Implements inter-thread processor communication (ITPC) module.
*     This provides an interrupt based communication layer between the two
*     cores contained within a single Broadcom CMT (concurrent
*     multi-threaded) processor.
*
*     The API allows users to register handlers (i.e. ISRs) for each type
*     of command that can be sent between cores (thread-processors). The
*     handler will be invoked when a command is sent from one core
*     to another. A response may also be returned to the sending core.
*
*     The API supports both asynchronous and synchronous commands. Asynchronous
*     commands are sent without blocking for an acknowledgement
*     of successful completion of the command by the other thread-processor,
*     or response data. Synchronous commands block until the other thread
*     processor has acknowledged reception of the command (and optionally
*     provided a response). This will result in a serialization of synchronous
*     command requests.
*
*     As of June 2005, Broadcom CMT cores are only used with the MIPS
*     architecture. This implementation isn't really MIPS specific.
*     We may be able to use it for all CMT architectures. It depends on
*     how future CMTs are designed...
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <stddef.h>
#include <string.h>

#include <xdrvItpc.h>
#include <xchgAssert.h>
#include <xdrvGlobalInterrupt.h>
#include <xdrvSpinLock.h>
#include <xdrvIntCtrl.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */


/*
** Spin-lock macros.
**
** Return errors if we fail to acquire the spin-lock.
*/
#define SPIN_LOCK_ACQUIRE( spinLock, intLock )                                      \
            if ( XDRV_STATUS_OK != xdrvSpinLockAcquire( (spinLock), (intLock) ) )   \
            {                                                                       \
               break;                                                               \
            }

#define SPIN_LOCK_RELEASE( spinLock, intLock )                                      \
            xdrvSpinLockRelease( (spinLock), (intLock) )


#define SPIN_LOCK_ACQUIRE_NO_INT( spinLock )                                        \
            if ( XDRV_STATUS_OK != xdrvSpinLockAcquireNoInt( (spinLock) ) )         \
            {                                                                       \
               return ( XDRV_STATUS_ERR );                                          \
            }

#define SPIN_LOCK_RELEASE_NO_INT( spinLock )                                        \
            xdrvSpinLockReleaseNoInt( (spinLock) )


/*
** Mutex macros.
*/
#define SYNC_CMD_MUTEX_ACQUIRE()                                              \
            if ( itpcDrv->mutex != NULL )                                     \
            {                                                                 \
               if (  itpcDrv->mutex->acquireFunc( itpcDrv->mutex->data ) )    \
               {                                                              \
                  return ( -1 );                                              \
               }                                                              \
            }                                                                 \
            do                                                                \
            {

#define SYNC_CMD_MUTEX_RELEASE()                                              \
            }                                                                 \
            while ( 0 );                                                      \
            if ( itpcDrv->mutex != NULL )                                     \
            {                                                                 \
               itpcDrv->mutex->releaseFunc( itpcDrv->mutex->data );           \
            }



/*
** Internal defines used to indicate whether commands were successfully
** executed.
*/
enum
{
   ITPC_CMD_SUCCESS = XDRV_ITCP_MAX_CMD_ID + 1,
   ITPC_CMD_FAIL
};


/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int CommandHandlerIsr( XDRV_INT_CTRL_ISR_PARM data );


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   xdrvItpcInit
**
** PURPOSE:    To initialize the inter-thread processor communcation module.
**
** PARAMETERS: itpcInterruptId   (in)  Software interrupt id used for thread
**                                     processor communication. Setting this
**                                     software interrupt on one thread processor
**                                     will trigger an interrupt on the other
**                                     thread processor.
**             sharedMemData     (in)  Data structures that shared between
**                                     thread-processors.
**             spinLock          (in)  Spin-lock used to provide concurrency
**                                     protection between shared data structs.
**             mutex             (in)  Serialization mutex for synchronous cmds.
**                                     Optionally NULL if no mutex required.
**             itpcDrv           (out) ITPC instance memory state.
**
** RETURNS:    Nothing.
**
** NOTE: It is assumed that the appropriate registers have been configured
**       to route the software interrupt from one thread processor to the
**       other thread processor prior to calling this function.
*****************************************************************************
*/
void xdrvItpcInit
(
   XDRV_INT_CTRL_INTERRUPT    itpcInterruptId,
   XDRV_ITPC_SHARED_MEM_DATA *sharedMemData,
   XDRV_SPIN_LOCK            *spinLock,
   XDRV_ITPC_MUTEX           *mutex,
   XDRV_ITPC                 *itpcDrv
)
{

   memset( &itpcDrv->commandHandler,     0, sizeof( itpcDrv->commandHandler ) );
   memset( &itpcDrv->commandHandlerData, 0, sizeof( itpcDrv->commandHandlerData ) );

   itpcDrv->itpcInterruptId   = itpcInterruptId;
   itpcDrv->sharedMemData     = sharedMemData;
   itpcDrv->spinLock          = spinLock;
   itpcDrv->mutex             = mutex;


   /* Init shared memory locations. Only init the 'send' data structures. The
   ** other thread processor will init its 'send' data, which is our 'receive'
   ** data. */
   *(sharedMemData->sendStatusReg)    = 0;
   sharedMemData->syncSendCmd->cmdId  = ITPC_CMD_FAIL;


   /*
   ** Install internal software interrupt service routine. This will be used to
   ** bridge the interrupt controller with user registered command handlers. A
   ** software interrupt is used to signal between thread processors.
   */
   xdrvIntCtrlDisableInterrupt( itpcDrv->itpcInterruptId );
   xdrvIntCtrlInstallInterrupt( itpcDrv->itpcInterruptId, CommandHandlerIsr, itpcDrv );

   xdrvIntCtrlEnableInterrupt ( itpcDrv->itpcInterruptId );

}


/*
*****************************************************************************
** FUNCTION:   xdrvItpcDeinit
**
** PURPOSE:    To de-initialize the inter-thread communication module.
**
** PARAMETERS: itpcDrv   (mod) ITPC instance memory state.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcDeinit( XDRV_ITPC *itpcDrv )
{
   /*
   ** Un-install the software interrupt ISR.
   */
   xdrvIntCtrlDisableInterrupt( itpcDrv->itpcInterruptId );
   xdrvIntCtrlUninstallInterrupt( itpcDrv->itpcInterruptId );

}


/*
*****************************************************************************
** FUNCTION:   xdrvItpcRegisterHandler
**
** PURPOSE:    Allows users to register a handler (i.e. ISR) for each ITPC
**             command type. The handler will be invoked when a command is
**             sent from one thread processor to another.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Install handler for this command.
**             handler     (in)  Handler to invoke when command is sent.
**             parm        (in)  Function parameter for handler.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcRegisterHandler
(
   XDRV_ITPC                    *itpcDrv,
   XDRV_ITPC_CMD_ID              cmdId,
   XDRV_ITPC_CMD_HANDLER         handler,
   XDRV_ITPC_CMD_HANDLER_PARM    parm
)
{
   XDRV_GLOBAL_INTERRUPT_LOCK    lock;

   XCHG_ASSERT( cmdId <= XDRV_ITCP_MAX_CMD_ID );


   /* Disable interrupt when registering new command handlers (ISRs). */
   xdrvGlobalInterruptDisable( &lock );
   {
      /*
      ** Register user command handler.
      */

      itpcDrv->commandHandler    [ cmdId ] = handler;
      itpcDrv->commandHandlerData[ cmdId ] = parm;
   }
   xdrvGlobalInterruptRestore( &lock );
}


/*
*****************************************************************************
** FUNCTION:   xdrvItpcUnregiserHandler
**
** PURPOSE:    To un-install a command handler.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Uninstall handler for this command.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvItpcUnregisterHandler
(
   XDRV_ITPC        *itpcDrv,
   XDRV_ITPC_CMD_ID  cmdId
)
{
   xdrvItpcRegisterHandler( itpcDrv, cmdId, NULL, NULL );
}


/*
*****************************************************************************
** FUNCTION:   xdrvItpcSendSyncCommand
**
** PURPOSE:    To send a synchronous command to the other thread processor.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Command to send to other thread processor.
**             cmdData     (in)  Command specific data (optionally NULL).
**             rspData     (out) Response data (optionally NULL if no resp required).
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       This function will block until the other thread processor
**             has acknowledged the command (and optionally provided a response).
**             This will result in a serialization of sync command requests.
*****************************************************************************
*/
XDRV_STATUS xdrvItpcSendSyncCommand
(
   XDRV_ITPC           *itpcDrv,
   XDRV_ITPC_CMD_ID     cmdId,
   XDRV_ITPC_DATA       cmdData,
   XDRV_ITPC_DATA       rspData
)
{
   volatile XDRV_ITPC_CMD       *itpcCmd;
   XDRV_GLOBAL_INTERRUPT_LOCK    intLock;
   volatile unsigned int         i = 0;
   XDRV_STATUS                   status = XDRV_STATUS_ERR;


   XCHG_ASSERT( cmdId <= XDRV_ITCP_MAX_CMD_ID );

   /* This will serialize concurrent sync command requests made by multiple tasks. */
   SYNC_CMD_MUTEX_ACQUIRE();
   {
      /*
      ** Fill shared memory locations with command information. I don't need
      ** concurrency protection here between the thread processors. We only
      ** write 'send' command data and response locations. And the other
      ** thread processor only reads these locations. Also, the writes and
      ** reads are sequenced via the software interrupt and cmdId.
      */
      itpcCmd = itpcDrv->sharedMemData->syncSendCmd;
      itpcCmd->cmdData  = cmdData;
      itpcCmd->rspData  = rspData;
      itpcCmd->cmdId    = cmdId;


      /*
      ** Update the status register bit-field. This indicates to the other
      ** thread-procssor which commands need to be processed. Note that the
      ** read-modify-write of the status register needs to be performed
      ** atomically by using spin-locks. This prevents concurrent access by
      ** both thread-processors.
      */
      SPIN_LOCK_ACQUIRE( itpcDrv->spinLock, &intLock );
      {
         *(itpcDrv->sharedMemData->sendStatusReg) |= ( 1 << cmdId );
      }
      SPIN_LOCK_RELEASE( itpcDrv->spinLock, &intLock );



      /*
      ** Signal the other thread processor.
      */
      xdrvIntCtrlGenerateSoftInterrupt( itpcDrv->itpcInterruptId );


      /*
      ** Wait for the other thread processor to ack the command.
      */
      while (     ( i++ < 100000 )
               && ( (int)itpcCmd->cmdId != (int)ITPC_CMD_SUCCESS )
               && ( (int)itpcCmd->cmdId != (int)ITPC_CMD_FAIL    ) )
      {
      }

      if ( (int)itpcCmd->cmdId == (int)ITPC_CMD_SUCCESS )
      {
         status = XDRV_STATUS_OK;
      }
   }
   SYNC_CMD_MUTEX_RELEASE();


   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   xdrvItpcSendAsyncCommand
**
** PURPOSE:    To send an asynchronous command to the other thread processor.
**
** PARAMETERS: itpcDrv     (mod) ITPC instance memory state.
**             cmdId       (in)  Command to send to other thread processor.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       This function returns immediately. It does NOT wait for
**             the other thread processor to ack the command.
*****************************************************************************
*/
XDRV_STATUS xdrvItpcSendAsyncCommand
(
   XDRV_ITPC        *itpcDrv,
   XDRV_ITPC_CMD_ID  cmdId
)
{
   XDRV_GLOBAL_INTERRUPT_LOCK    intLock;
   XDRV_STATUS                   status = XDRV_STATUS_ERR;


   XCHG_ASSERT( cmdId <= XDRV_ITCP_MAX_CMD_ID );


   /* Use do-while(0) to allow code 'break' out upon error. */
   do
   {
      /*
      ** Update the status register bit-field. This indicates to the other
      ** thread-procssor which commands need to be processed. Note that the
      ** read-modify-write of the status register needs to be performed
      ** atomically by using spin-locks. This prevents concurrent access by
      ** both thread-processors.
      */
      SPIN_LOCK_ACQUIRE( itpcDrv->spinLock, &intLock );
      {
         *(itpcDrv->sharedMemData->sendStatusReg) |= ( 1 << cmdId );
      }
      SPIN_LOCK_RELEASE( itpcDrv->spinLock, &intLock );



      /*
      ** Signal the other thread processor.
      */
      xdrvIntCtrlGenerateSoftInterrupt( itpcDrv->itpcInterruptId );

      /* Success. */
      status = XDRV_STATUS_OK;
   }
   while( 0 );


   return ( status );
}



/*
*****************************************************************************
** FUNCTION:   CommandHandlerIsr
**
** PURPOSE:    Internal interrupt handler. This is registered with the generic
**             interrupt controller and will be invoked when a software interrupt
**             is triggered by the other thread processor. It will determine
**             which commands have been sent, and invoke the user registered
**             handlers (if they exist).
**
**
** PARAMETERS: data  (in)  User registered data.
**
** RETURNS:    0
**
** NOTE:
*****************************************************************************
*/
static unsigned int CommandHandlerIsr( XDRV_INT_CTRL_ISR_PARM data )
{
   volatile XDRV_ITPC_CMD    *itpcCmd;
   XDRV_ITPC_SHARED_MEM_DATA *sharedMemData;
   XDRV_ITPC                 *itpcDrv;
   XDRV_ITPC_CMD              itpcAsyncCmd;
   XDRV_BOOL                  isSyncCmd;
   XDRV_ITPC_CMD_HANDLER      handler;
   unsigned int               status;
   unsigned int               activeInterrupts;
   unsigned int               currentInterrupt;
   int                        cmdId;


   /* ISR parm is actually a pointer to the ITPC instance state. */
   itpcDrv = (XDRV_ITPC *) data;

   sharedMemData = itpcDrv->sharedMemData;

   itpcAsyncCmd.cmdData = NULL;
   itpcAsyncCmd.rspData = NULL;

   /* Determine which commands have been sent by the other thread-processor. */
   while (( activeInterrupts = *(sharedMemData->recvStatusReg) ) != 0 )
   {
      currentInterrupt = 1 << 31;

      /* Test all interrupt bits starting with bit 31.  If an interrupt is active
      ** and there is an interrupt handler registered, call the handler. */
      for ( cmdId = 31; cmdId >= 0; cmdId-- )
      {
         status = 1;

         if (( activeInterrupts & currentInterrupt ) != 0 )
         {
            /*
            ** Clear the interrupt. Note that the read-modify-write of the
            ** status register needs to be performed atomically by using
            ** spin-locks. This prevents concurrent access by both thread-processors.
            */
            SPIN_LOCK_ACQUIRE_NO_INT( itpcDrv->spinLock );
            {
               *(sharedMemData->recvStatusReg) &= (~currentInterrupt);
            }
            SPIN_LOCK_RELEASE_NO_INT( itpcDrv->spinLock );



            /*
            ** Decode which type of command has been sent by the other thread processor.
            */
            isSyncCmd = ( sharedMemData->syncRecvCmd->cmdId == (XDRV_ITPC_CMD_ID) cmdId );
            if ( isSyncCmd )
            {
               /* Sync command. Get the command and response data locations
               ** specified by the other thread-processor. */
               itpcCmd = sharedMemData->syncRecvCmd;
            }
            else
            {
               /* Async command. Command and response data not supported. */
               itpcCmd = &itpcAsyncCmd;
               itpcCmd->cmdId = cmdId;
            }


            /*
            ** Invoke the user registered command handler (if one exists).
            */
            handler = itpcDrv->commandHandler[cmdId];
            if ( handler != NULL )
            {
               status = handler( itpcCmd, itpcDrv->commandHandlerData[cmdId] );
            }


            if ( isSyncCmd )
            {
               /*
               ** This signals to the other thread processor that we have completed
               ** processing the command. Only required for sync commands.
               */
               status = status ? ITPC_CMD_FAIL : ITPC_CMD_SUCCESS;
               itpcCmd->cmdId = status;
            }
         }

         currentInterrupt >>= 1;
      }
   }



   /*
   ** Re-enable ITPC interrupts.
   */
   xdrvIntCtrlEnableInterrupt( itpcDrv->itpcInterruptId );


   return ( 0 );
}
