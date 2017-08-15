/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This module implements the main loop for the Call Manager thread.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
CMGRSTATUSCB gCmStatusCb;
CMGRCFGBLK   gCmCfgCb;

BOS_BOOL     gbCmInit = BOS_FALSE;
BOS_TASK_ID  gCmTaskId = 0;
BOS_MSGQ     gCmQid = 0;
BOS_BOOL     gbCmQueueCreated = BOS_FALSE;
BOS_SEM      gCmSem;
BOS_BOOL     gbCmSemCreated = BOS_FALSE;
BOS_SEM      gCmShutdownSem;
BOS_BOOL     gbCmShutdownSemCreated = BOS_FALSE;
BOS_BOOL     gbCmReset = BOS_FALSE;
BOS_BOOL     gbCmShutdown = BOS_FALSE;
BOS_BOOL     gbCmShutdownPending = BOS_FALSE;
BOS_BOOL     gbCmResetAsynch = BOS_FALSE;
BOS_BOOL     gbMainLoop = BOS_FALSE;
BOS_BOOL     gbCmShutdownNoPhy = BOS_FALSE;
CMGRSTATUS   gCmState;


/* ---- Private Function Prototypes --------------------------------------------- */
static void cmMainRun ( BOS_TASK_ARG args );
CMEVT *cmGetEvt ( void );
void cmFlushEvt();
void cmCleanup ( void );

void cmUsrCmdHandler ( CMEVT *pEvt );
void cmPhysIfCmdHandler ( CMEVT *pEvt );
void cmCctkCmdHandler ( CMEVT *pEvt );

static void cmCctkCb ( CCTKEVT eEvent, BOS_UINT32 uUid,
                       BOS_UINT32 uCid, void *pData );

static void cmPhyCb ( BOS_UINT32 uEvt, BOS_UINT32 uPhy,
                      BOS_UINT32 uSes, void *pData );

static void cmDmExtVarCb( BOS_UINT32 uGuid, const BOS_UINT8 *pVarName,
                          BOS_UINT8 *pVarValue, BOS_UINT32 *puVarValueLen );


/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmMainRemoveUnusedParamWarning
**
** PURPOSE:    Utility function used to remove warnings for unused parameters
**             within function call.
**
** PARAMETERS: pParam - the parameter not used.
**
** RETURNS:    None.
***********************************************************************************/
void *cmMainRemoveUnusedParamWarning( void *pParam )
{
   return pParam;
}


/***********************************************************************************
** FUNCTION:   cmVer
**
** PURPOSE:    Sets and displays the Call Manager current version.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmVer ( void )
{
   CMGRLOG_INFORMATION (( CM_VER_DATE_TIME ));
   CMGRLOG_INFORMATION (( CM_VER_NUMBERING ));
}


/***********************************************************************************
** FUNCTION:   cmMain
**
** PURPOSE:    Creates the Call Manager support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing, however the status callback will be invoked when this
**             action is terminated.  The controlling application must wait until
**             that callback has been invoked prior to using the Call Manager for
**             any functionality.
***********************************************************************************/
void cmMainInit ( void )
{
   if ( gbCmInit )
   {
      CMGRLOG_CRITICAL (( "cmMain: CMGR already initialized!" ));
      cmUpdateState ( CMGR_INVALID_HANDLE,
                      eCMGRSTATUS_INIT_FAILURE );
      return;
   }

   /* Display the Call Manager revision.
   */
   cmVer ();

   gbCmInit = BOS_TRUE;
   /* Initialize all the resources and data collection related information.
   */
   gbCmShutdownNoPhy = BOS_FALSE;
#if CMGRCFG_MULTILINE
   cmCctkAccnt ( BOS_TRUE );
#endif /* CMGRCFG_MULTILINE */
   cmCdr ( BOS_TRUE );
   cmSes ( BOS_TRUE );
   cmUsr ( BOS_TRUE );
   cmTmr ( BOS_TRUE );
   cmDmAssignExtVarCb( &cmDmExtVarCb );

   cmDateInit ();
   /* Create necessary thread associated resources.
   */
   if ( bosMsgQCreate ( CMGRCFG_THREAD_QNAME,
                        CMGRCFG_THREAD_QDEPTH,
                        &gCmQid ) != BOS_STATUS_OK )
   {
      CMGRLOG_CRITICAL (( "cmMainInit: CMGR failed to create msg-q!" ));
      cmMainDeinit();
      cmUpdateState ( CMGR_INVALID_HANDLE,
                      eCMGRSTATUS_INIT_FAILURE );
      return;
   }
   gbCmQueueCreated = BOS_TRUE;
   if ( bosSemCreate ( CMGRCFG_THREAD_SEMNAME,
                       BOS_CFG_SEM_INIT_COUNT,
                       BOS_CFG_SEM_MAX_COUNT,
                       &gCmSem ) != BOS_STATUS_OK )
   {
      CMGRLOG_CRITICAL (( "cmMainInit: CMGR failed to create sema-4!" ));
      cmMainDeinit();
      cmUpdateState ( CMGR_INVALID_HANDLE,
                      eCMGRSTATUS_INIT_FAILURE );
      return;
   }
   gbCmSemCreated = BOS_TRUE;
   /* Create and launch the Call Manager thread.
   */
   gbMainLoop = BOS_TRUE;
#if BOS_CFG_RESET_SUPPORT
   if ( bosTaskCreateEx ( CMGRCFG_THREAD_NAME,
                          CMGRCFG_THREAD_STACK,
                          CMGRCFG_THREAD_PRI,
                          NULL,
                          cmMainRun,
                          NULL,
                          (BOS_TASK_ARG *) NULL,
                          &gCmTaskId ) != BOS_STATUS_OK )
#else
   if ( bosTaskCreate   ( CMGRCFG_THREAD_NAME,
                          CMGRCFG_THREAD_STACK,
                          CMGRCFG_THREAD_PRI,
                          cmMainRun,
                          (BOS_TASK_ARG *) NULL,
                          &gCmTaskId ) != BOS_STATUS_OK )
#endif
   {
      CMGRLOG_CRITICAL (( "cmMainInit: CMGR failed to create thread!" ));
      cmMainDeinit();
      /* Report startup status.
      */
      cmUpdateState ( CMGR_INVALID_HANDLE,
                      eCMGRSTATUS_INIT_FAILURE );
      return;
   }

   /* Report startup status to the application.
   */
   cmUpdateState ( CMGR_INVALID_HANDLE,
                   eCMGRSTATUS_INIT_COMPLETE );

#if BOS_CFG_RESET_SUPPORT
   bosTaskSetNoReset( &gCmTaskId );
#endif
}


/***********************************************************************************
** FUNCTION:   cmMainDeinit
**
** PURPOSE:    Destroy the Call Manager support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing, however the status callback will be invoked when this
**             action is terminated.
***********************************************************************************/
void cmMainDeinit ( void )
{
   CMGRSTATUS cmgr = eCMGRSTATUS_DEINIT_COMPLETE;
   if ( gbCmInit == BOS_FALSE )
   {
      CMGRLOG_CRITICAL (( "cmMainDeinit: CMGR not yet initialized!" ));
      cmUpdateState ( CMGR_INVALID_HANDLE,
                      eCMGRSTATUS_DEINIT_FAILURE );
   }

   /* Ensure that the cmgr shutdown process has been completed. If not, we should
   ** wait until the shutdown is complete before proceeding any further
   */
   if ( gbCmShutdownSemCreated && (bosSemTake( &gCmShutdownSem ) != BOS_STATUS_OK) )
   {
      CMGRLOG_WARNING (( "cmMainDeinit: Failed to take shutdown semaphore!" ));
      cmgr = eCMGRSTATUS_CRITICAL_ERROR;
   }

   /* Invoke each module cleanup as necessary.  Order is important.
   */
   cmDmAssignExtVarCb( NULL );
   cmCdr ( BOS_FALSE );
   cmSes ( BOS_FALSE );
   cmUsr ( BOS_FALSE );
   cmTmr ( BOS_FALSE );
#if CMGRCFG_MULTILINE
   cmCctkAccnt ( BOS_FALSE );
#endif /* CMGRCFG_MULTILINE */

   if ( gbCmSemCreated && bosSemDestroy ( &gCmSem ) != BOS_STATUS_OK )
   {
      CMGRLOG_WARNING (( "cmMainDeinit: Fail semaphore destruction!" ));
      cmgr = eCMGRSTATUS_CRITICAL_ERROR;
   }

   if ( gbCmShutdownSemCreated && bosSemDestroy ( &gCmShutdownSem ) != BOS_STATUS_OK )
   {
      CMGRLOG_WARNING (( "cmMainDeinit: Fail shutdown semaphore destruction!" ));
      cmgr = eCMGRSTATUS_CRITICAL_ERROR;
   }

   if ( gbCmQueueCreated && (bosMsgQDestroy ( &gCmQid ) != BOS_STATUS_OK) )
   {
      CMGRLOG_WARNING (( "cmMainDeinit: Fail message queue destruction!" ));
      cmgr = eCMGRSTATUS_CRITICAL_ERROR;
   }

   /* Report the final status to the application.
   */
   cmUpdateState ( CMGR_INVALID_HANDLE,
                   cmgr );

   /* Reset internal variables to initial value.
   */
   gCmTaskId = 0;
   gCmQid = 0;
   gbCmQueueCreated = BOS_FALSE;
   gbCmSemCreated = BOS_FALSE;
   gbCmShutdownSemCreated = BOS_FALSE;

   gbCmInit = BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   cmMainRun
**
** PURPOSE:    Run the Call Manager.
**
** PARAMETERS: args - the arguments passed when this main is invoked by BOS
**                    framework.
**
** RETURNS:    Never.  This runs the infinite loop which takes care of the
**             Call Manager processing.
***********************************************************************************/
void cmMainRun ( BOS_TASK_ARG args )
{
   BOS_STATUS status;
   BOS_TIME_MS curTime;
   BOS_TIME_MS prevTime = 0;
   BOS_TIME_MS deltaTime;
   CMEVT *pEvt;
   BOS_UINT32 uEvt;
   BOS_UINT32 uIx;
   CMUSR *pUsr;

   CMGR_REMOVE_UNUSED_PARAM_WARNING( args );

   bosTimeGetMs ( &prevTime );
   while ( gbMainLoop )
   {
      status = bosSemTimedTake( &gCmSem, CMGRCFG_THREAD_SLEEP );

      /* Shutdown processing.  Exit this task at the end of it.
      */
      if ( gbCmShutdown )
      {
         gbCmShutdown = BOS_FALSE;
         gbCmShutdownPending = BOS_FALSE;
         gbCmResetAsynch = BOS_FALSE;
         gbMainLoop = BOS_FALSE;

         /* Create the shutdown semaphore. This semaphore is used
         ** to ensure the task has exited before cmMainDeinit is called
         */
         if ( bosSemCreate ( CMGRCFG_THREAD_SHUTDOWN_SEMNAME,
                             BOS_CFG_SEM_INIT_COUNT,
                             BOS_CFG_SEM_MAX_COUNT,
                             &gCmShutdownSem ) != BOS_STATUS_OK )
         {
            CMGRLOG_CRITICAL (( "cmMainRun: CMGR failed to create shutdown sema-4!" ));
         }
         else
         {
            gbCmShutdownSemCreated = BOS_TRUE;
         }

         /* Terminate the CCTK processing (forcefully).
         */
         if ( cctkShutdown () != eCCTKSTATUS_SUCCESS )
         {
            CMGRLOG_CRITICAL (( "cmMainRun: failed SHUTDOWN!" ));

            /* Clean up the shutdown semaphore if it was previously created
            */
            if ( gbCmShutdownSemCreated && bosSemDestroy ( &gCmShutdownSem ) != BOS_STATUS_OK )
            {
               CMGRLOG_CRITICAL (( "cmMainRun: Fail shutdown semaphore destruction!" ));
            }

            cmCfgDeinit();

            cmUpdateState ( CMGR_INVALID_HANDLE,
                            eCMGRSTATUS_SHUTDOWN_FAILURE );
         }
         else
         {
            /* Will report the final status of the shutdown to the application.
             * This also shuts down phy.
            */
            cmCleanup ();

            /* Release the shutdown semaphore to indicate shutdown is complete.
            */
            if ( gbCmShutdownSemCreated && (bosSemGive( &gCmShutdownSem ) != BOS_STATUS_OK) )
            {
               CMGRLOG_CRITICAL (( "cmMainRun: CMGR failed to give shutdown semaphore" ));
            }

            cmCfgDeinit();

            cmUpdateState ( CMGR_INVALID_HANDLE,
                            eCMGRSTATUS_SHUTDOWN_COMPLETE );

         }


         /* **** WARNING: Exiting the task now.
         */
         return;
      }

      /* Reset processing.
      */
      if ( gbCmReset )
      {
         /* Defer the reset until the user reconfiguration is done.  This is to
         ** avoid deleting the user while the reconfiguration is still in progress.
         */
         for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
         {
            pUsr = cmUsrValid ( uIx );
            if ( pUsr )
            {
               if ( !strlen ( (char *)pUsr->uUid ) )
               {
                  if ( pUsr->bBeRem )
                  {
                     break;
                  }
               }
            }
         }

         if ( uIx == CMGRCFG_USER_MAX )
         {
            /* No outstanding user reconfiguration.  Proceed with the reset.
            **
            ** Reset the CCTK layer if we need to, otherwise (if CCTK layer was never
            ** configured), simply try to configure it again.
            */
            if ( cmCfgIsCctkInit () )
            {
               cctkReset ( eCCTKRSTMODE_GRACEFULNONBLOCKING );
               gbCmResetAsynch = BOS_TRUE;
            }
            else
            {
               if ( cmCfgInit () && !cmPhyIsStarted () )
               {
                  cmPhyStartup ( &cmPhyCb );
               }
            }

            gbCmReset = BOS_FALSE;
         }
      }

      if ( status == BOS_STATUS_TIMEOUT )
      {
         bosTimeGetMs ( &curTime );
         bosTimeCalcDeltaMs ( &prevTime, &curTime, &deltaTime );
         prevTime = curTime;

         /* Process timers if necessary.
         */
         cmTmrTick ( deltaTime );

         /* Process reconfiguration action is any is pending.
         */
#if !CMGRCFG_MULTILINE
         cmUsrReConfig ( NULL );
#else
         cmCctkAccntReConfig( NULL );
#endif
      }
      /* Dequeue and process events.
      */
      while ( ( pEvt = cmGetEvt () ) != NULL )
      {
         uEvt = pEvt->uEvt & CMGR_EVT_CLASS__CLASS_MASK;
         /* Ignore all user and phy events if we are in the middle of a reset.
         */
         if ( gbCmResetAsynch &&
              ( ( uEvt == CMGR_EVT_CLASS__USR_CMD ) || ( uEvt == CMGR_EVT_CLASS__PHY_IF ) ) &&
              ( pEvt->pDat3 ) )
         {
            free ( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
            continue;
         }

         switch ( uEvt )
         {
            case CMGR_EVT_CLASS__USR_CMD:
               {
                  cmUsrCmdHandler ( pEvt );
               }
               break;

            case CMGR_EVT_CLASS__CCTK:
               {
                  cmCctkCmdHandler ( pEvt );
               }
               break;

            case CMGR_EVT_CLASS__PHY_IF:
               {
                  cmPhysIfCmdHandler ( pEvt );
               }
               break;

            default:
               break;
         }
      };
   }

   /* Must have exit the main loop for some reason.  Report a critical
   ** error since there is not much else we will be able to do here!
   */
   cmUpdateState ( CMGR_INVALID_HANDLE,
                   eCMGRSTATUS_CRITICAL_ERROR );
}


/***********************************************************************************
** FUNCTION:   cmGetEvt
**
** PURPOSE:    Get an event from the Call Manager queue.
**
** PARAMETERS: None.
**
** RETURNS:    A pointer to the dequeued event if available or NULL if nothing
**             is in the queue at this time.
***********************************************************************************/
CMEVT *cmGetEvt ( void )
{
   BOS_MSGQ_MSG msgQ;
   static CMEVT evt;

   if ( bosMsgQTimedReceive ( &gCmQid, 0, &msgQ ) == BOS_STATUS_OK )
   {
      evt.uEvt  = msgQ.buffer[0];
      evt.uDat1 = msgQ.buffer[1];
      evt.uDat2 = msgQ.buffer[2];
      evt.pDat3 = (void *) msgQ.buffer[3];
      evt.uDat4 = msgQ.buffer[4];
      evt.bLast = msgQ.buffer[5];
      return &evt;
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
** FUNCTION:   cmPostEvt
**
** PURPOSE:    Post an event to the Call Manager queue.
**
** PARAMETERS: pEvt - pointer to the event to queue.
**
** RETURNS:    BOS_STATUS_OK in case of success, an error code otherwise.
***********************************************************************************/
BOS_STATUS cmPostEvt ( CMEVT *pEvt )
{
   BOS_MSGQ_MSG msgQ;

   msgQ.buffer[0] = pEvt->uEvt;
   msgQ.buffer[1] = pEvt->uDat1;
   msgQ.buffer[2] = pEvt->uDat2;
   msgQ.buffer[3] = (BOS_UINT32) pEvt->pDat3;
   msgQ.buffer[4] = pEvt->uDat4;
   msgQ.buffer[5] = (BOS_UINT32) pEvt->bLast;

   return ( bosMsgQSend( &gCmQid, &msgQ ));
}


/***********************************************************************************
** FUNCTION:   cmFlushEvt
**
** PURPOSE:    Flush the Call Manager queue.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmFlushEvt()
{
   CMEVT *pEvt;

   while ( ( pEvt = cmGetEvt () ) != NULL )
   {
      /* Flush out all events in the queue and release any allocated memory.
      */
      if ( pEvt->pDat3 )
      {
         free ( pEvt->pDat3 );
         pEvt->pDat3 = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmSignalEvt
**
** PURPOSE:    Signals an event to the Call Manager.
**
** PARAMETERS: None.
**
** RETURNS:    None.
***********************************************************************************/
void cmSignalEvt ( void )
{
   bosSemGive ( &gCmSem );
}


/***********************************************************************************
** FUNCTION:   cmCleanup
**
** PURPOSE:    Terminates all the resources associated with the Call Manager thread.
**             This is typically the ultimate step of shutdown for the Call Manager,
**             or a step taken in case of critical failure of some sort that requires
**             complete termination of the Call Manager.
**
** PARAMETERS: None.
**
** RETURNS:    None.  The status callback will be invoked however to inform the
**             application of the result of this action.
**
**             If a failure is reported at this point, the safest way to recover is
**             through a complete system reboot.
**
**             If a success is reported after this point, it should be possible to
**             invoke the startup on the Call Manager once more.
***********************************************************************************/
void cmCleanup ( void )
{
   CMGRLOG_WARNING (( "cmCleanup: CMGR is bailing!" ));
   /* Terminate external physical interface.
   */
   if ( cmPhyIsStarted () && !gbCmShutdownNoPhy )
   {
      cmPhyShutdown();
   }
}


/***********************************************************************************
** FUNCTION:   cmUsrCmdHandler
**
** PURPOSE:    Handle a user command event.
**
** PARAMETERS: pEvt - the event pointer.
**
** RETURNS:    None.
***********************************************************************************/
void cmUsrCmdHandler ( CMEVT *pEvt )
{
   CMGRLOG_DEBUG ((
      "cmUsrCmdHandler: processing 0x%lX",
      (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) ));

   switch ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) )
   {
      case eCMCMD_STARTUP:
      {
         CMGRSTATUS eStatus = eCMGRSTATUS_STARTUP_SUCCESS;

         CMGRLOG_DEBUG (( "cmUsrCmdHandler: Starting up CCTK" ));

         /* Startup the network layer processing - CCTK.
         */
         if ( cctkStartup ( &cmCctkCb ) != eCCTKSTATUS_SUCCESS )
         {
            eStatus = eCMGRSTATUS_STARTUP_FAILED;
         }

         CMGRLOG_DEBUG (( "cmUsrCmdHandler: Starting up CFG" ));

         /* Configure the application with the proper users.
         */
         if ( ( eStatus == eCMGRSTATUS_STARTUP_SUCCESS ) &&
              ( cmCfgInit () != BOS_TRUE ) )
         {
            eStatus = eCMGRSTATUS_STARTUP_FAILED;
         }

         CMGRLOG_DEBUG (( "cmUsrCmdHandler: Starting up PHY" ));

         /* Startup the physical interface.
         */
         if ( ( eStatus == eCMGRSTATUS_STARTUP_SUCCESS ) &&
              ( cmPhyStartup ( &cmPhyCb ) != eCMGRSTATUS_STARTUP_SUCCESS ) )
         {
            eStatus = eCMGRSTATUS_STARTUP_FAILED;
         }

         /* Signal to the application immediately if some component failed to start.
         */
         if ( eStatus == eCMGRSTATUS_STARTUP_FAILED )
         {
            cmUpdateState ( CMGR_INVALID_HANDLE,
                            eCMGRSTATUS_STARTUP_FAILED );

         }
         else
         {
            CMGRLOG_DEBUG (( "cmUsrCmdHandler: CM startup complete, Waiting for ACK from CCTK" ));
         }
      }
      break;

      case eCMCMD_MEMSTATS:
      {
         cctkMemStatusShow( (BOS_BOOL) pEvt->uDat1,
                            (BOS_BOOL) pEvt->uDat2 );
      }
      break;

      case eCMCMD_RESTART:
      {
         /* Initiate the reset of CCTK, wait for it to complete prior to
         ** restarting.
         */
         if ( cmCfgIsCctkInit () )
         {
            if( cctkReset ( (CCTKRSTMODE)pEvt->uDat1 ) != eCCTKSTATUS_SUCCESS )
            {
               CMGRLOG_CRITICAL (( "cmUsrCmdHandler: failed RESTART!" ));
               cmUpdateState ( CMGR_INVALID_HANDLE,
                               eCMGRSTATUS_CRITICAL_ERROR );
            }
            else
            {
               gbCmResetAsynch = BOS_TRUE;
            }
         }
         /* No need to reset CCTK, this is like a new configuration of the application.
         */
         else
         {
            if ( cmCfgInit () && !cmPhyIsStarted () )
            {
               cmPhyStartup ( &cmPhyCb );
            }
         }
      }
      break;

      case eCMCMD_SHUTDOWN:
      {
         if ( cmCfgIsCctkInit () )
         {
            /* Intercept reset mode of local interest to CMGR that have no
            ** meaning per say at the CCTK layer, remap accordingly.
            */
            if ( (CCTKRSTMODE) pEvt->uDat1 == eCCTKRSTMODE_IMMEDIATENOPHY )
            {
               pEvt->uDat1 = eCCTKRSTMODE_IMMEDIATE;
               gbCmShutdownNoPhy = BOS_TRUE;
            }

            /* Go through a reset first so we terminate the
            ** signaling if need be.
            */
            cctkReset ( (CCTKRSTMODE) pEvt->uDat1 );
            gbCmShutdownPending = BOS_TRUE;
            gbCmResetAsynch = BOS_TRUE;
         }
         else
         {
            /* Apply shutdown right away.
            */
            /* Delay added to avoid cctkShutdown stuck issue due to
            ** CAsynResolver */
            bosSleep(2000);
            gbCmShutdown = BOS_TRUE;
         }
      }
      break;

      case eCMCMD_NEWCONFIG:
      {
         switch ( (CMCFGACT) pEvt->uDat1 )
         {
            case eCMCFGACT_CFGDEV:
            {
               /* Device reconfiguration...
               */
               CMGRLOG_CRITICAL (( "cmUsrCmdHandler: device reset enabled." ));
               gbCmReset = BOS_TRUE;
            }
            break;

            case eCMCFGACT_CFGUSRNONET:
            case eCMCFGACT_CFGUSRWITHNET:
            case eCMCFGACT_CFGUSRTERM:
            {
#if CMGRCFG_MULTILINE
               CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByUid( pEvt->pDat3 );
               if ( pCctkAccnt )
               {
                  pCctkAccnt->eCfgAction = (CMCFGACT) pEvt->uDat1;
                  CMGRLOG_CRITICAL (( "cmUsrCmdHandler: account reconfig - cfgtype %u",  pCctkAccnt->eCfgAction ));
               }
               /* Account addition.  Locate a free account block and attempt to
               ** go ahead with the new addition.
               */
               else if ( ( pCctkAccnt == NULL ) &&
                         strlen ( (const char *) pEvt->pDat3 ) &&
                         ( (CMCFGACT) pEvt->uDat1 == eCMCFGACT_CFGUSRWITHNET ) )
               {
                  /* Grab a account block.
                  */
                  CMGRLOG_CRITICAL (( "cmUsrCmdHandler: account addition" ));
                  if ( ( pCctkAccnt = cmCctkAccntAlloc () ) != NULL )
                  {
                     PROV_CFG_IDX   cfgIdx;
                     CFGSIPUSERCFG *pCctkAccntCfg = NULL;


                     /* Associate this account with a valid identifier, this is the
                     ** way to tell that this account is in fact used.
                     */
                     strncpy ( (char *)pCctkAccnt->uUid,
                               (char *)pEvt->pDat3,
                               CMGR_USER_IDENTIFIER_LEN );

                     /* Set up the account handles first before adding the CCTK account */
                     memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                     cfgIdx.pcCharIdx = pCctkAccnt->uUid;
                     if( gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                                              &cfgIdx,
                                              (void *) &pCctkAccntCfg,
                                              CFG_ITEM_GET ) == BOS_STATUS_OK )
                     {
                        pCctkAccnt->uHdl = pCctkAccntCfg->uIMPUIndex;

                        cmCctkAccntAdd( pCctkAccnt );

                        /* TODO - Create a corresponding CMUSER if the new account's
                        ** endpoint map contains a new ept entry.
                        */

                        gCmCfgCb.cmgrProvCb( CFGITEM( SIP, UserConcept ),
                                             &cfgIdx,
                                             (void *) pCctkAccntCfg,
                                             CFG_ITEM_DEL );
                        pCctkAccntCfg = NULL;
                     }
                  }
                  else
                  {
                     CMGRLOG_CRITICAL (( "cmUsrCmdHandler: failed new account (%s) add",
                                         pEvt->pDat3 ));
                  }
               }

#else
               CMUSR *pUsr = cmUsrFindUidHdl ( pEvt->pDat3 );

               /* User reconfiguration.  Locate the correct user and set it up
               ** accordingly, the framework will take care of the rest.
               */
               if ( pUsr )
               {
                  pUsr->eCfgAction = (CMCFGACT) pEvt->uDat1;
                  CMGRLOG_CRITICAL (( "cmUsrCmdHandler: user reconfig - cfgtype %u",  pUsr->eCfgAction ));
               }
               /* User addition.  Locate a free user block and attempt to
               ** go ahead with the new addition.
               */
               else if ( ( pUsr == NULL ) &&
                         strlen ( (const char *) pEvt->pDat3 ) &&
                         ( (CMCFGACT) pEvt->uDat1 == eCMCFGACT_CFGUSRWITHNET ) )
               {
                  /* Grab a user block.
                  */
                  CMGRLOG_CRITICAL (( "cmUsrCmdHandler: user addition" ));
                  if ( ( pUsr = cmUsrAlloc () ) != NULL )
                  {
                     BOS_UINT32 strLen;

                     /* Associate this user with a valid identifier, this is the
                     ** way to tell that this user is in fact used.
                     */
                     strLen =
                        ( ( strlen ( (char *)pEvt->pDat3 ) >
                                                CMGR_USER_IDENTIFIER_LEN ) ?
                             CMGR_USER_IDENTIFIER_LEN :
                                             strlen ( (char *)pEvt->pDat3 ) );
                     strncpy ( (char *)pUsr->uUid,
                               (char *)pEvt->pDat3,
                               strLen );

                     cmCfgUserAdd ( (void *) pUsr );
                  }
                  else
                  {
                     CMGRLOG_CRITICAL (( "cmUsrCmdHandler: failed new user (%s) add",
                                         pEvt->pDat3 ));
                  }
               }
#endif /* CMGRCFG_MULTILINE */
            }
            break;
#if CMGRCFG_REMOTE_USER_CONTROL
            case eCMCFGACT_CFGREMUSRCFG:
            {
               /* TO_DO: add or update existing remote user */
            }
            break;
            case eCMCFGACT_CFGREMUSRSYS:
            {
               if ( !cmCfgRemUsrSys() )
               {
                  CMGRLOG_CRITICAL (( "cmUsrCmdHandler: failed reconfig SIP remote user system configuration"));
               }
            }
            break;
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_MULTILINE
            case eCMCFGACT_CFGROUTING:
            {
               cmCctkUpdateRouting();
            }
            break;
            case eCMCFGACT_CFGALARM:
            {
               BOS_UINT32 uIx;
               CMUSR *pUsr;
               PROV_CFG_IDX cfgIdx;
               CFGEPTLINECFG *pEptLineCfg = NULL;
               for ( uIx = 0 ; uIx < CMGRCFG_USER_MAX ; uIx++ )
               {
                  pUsr = cmUsrFindPhyHdl(uIx);
                  if (pUsr)
                  {
                     memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                     cfgIdx.uIndex = pUsr->uPhys;
                     if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                                &cfgIdx,
                                                (void *) &pEptLineCfg,
                                                CFG_ITEM_GET ) == BOS_STATUS_OK )
                     {
                        pUsr->eType = ( (pEptLineCfg->eTermType == CFG_SIP_USER_LINE) ?
                                           eCMUSRTYPE_PSTN : eCMUSRTYPE_LINE );
                        pUsr->eAlarmCfgState = pEptLineCfg->sAnalogAlarmCfgState;
                        gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                              &cfgIdx,
                                              (void *) pEptLineCfg,
                                              CFG_ITEM_DEL );
                        pEptLineCfg = NULL;
                     }
                  }
               }
            }
            break;
#endif /* CMGRCFG_MULTILINE */
#if CMGRCFG_DECT
            case eCMCFGACT_CFGDECTSHUTDOWN:
            {
               BOS_UINT32 uIx, uIy;
               CMUSR *pUsr;
               PROV_CFG_IDX cfgIdx;
               CFGEPTLINECFG *pEptLineCfg = NULL;

               /* Release any existing calls to the PP */
               for ( uIx = 0; uIx < CMGRCFG_USER_MAX; uIx++ )
               {
                  /* Get the user from the list */
                  pUsr = cmUsrValid( uIx );

                  if (pUsr == NULL)
                  {
                     /* pUsr not valid */
                     continue;
                  }
                  /* usr and phy are 1-1 */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.uIndex = uIx;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                             &cfgIdx,
                                             (void *) &pEptLineCfg,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if (pEptLineCfg && (pEptLineCfg->sMdiType == CFG_EPT_MDITYPE_DECTPP))
                     {
                        /* need to release calls of this Dect PP*/
                        /* clean up any other media sessions */
                        for ( uIy = 0 ; uIy < (CMGRCFG_MEDSES_MAX + 1) ; uIy++ )
                        {
                           if ( pUsr->pMedSes [ uIy ] )
                           {
                              cmSesTerminate ( pUsr->pMedSes [ uIy ]  );
                              cmUsrNewState ( pUsr, eCMST_RELEASING );
                           }
                        }
                        cmFsmOnHook ( pUsr );
                        cmUsrNewState ( pUsr, eCMST_IDLE );
                     }
                     gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, LineConcept ),
                                           &cfgIdx,
                                           (void *) pEptLineCfg,
                                           CFG_ITEM_DEL );
                     pEptLineCfg = NULL;
                  }
               }
               /* update routing table, as dect handsets should no longer
               ** be part of the routing table
               */
               cmCctkUpdateRouting();
            }
            break;
#endif /* CMGRCFG_DECT */
            default:
            break;
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_SVCSTART:
      case eCMCMD_SVCEND:
      {
#if CMGRCFG_MULTILINE
         CMCCTKACCNT *pUsr =  cmCctkAccntFindByUid ( pEvt->pDat3 );
#else
         CMUSR *pUsr = cmUsrFindUidHdl ( pEvt->pDat3 );
#endif

         if ( pUsr )
         {
            CMGRLOG_DEBUG ((
               "cmUsrCmdHandler: feature \'%d\' ctrl \'%s\' on 0x%X (0x%X)",
               pEvt->uDat1,
               ((CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMCMD_SVCSTART) ?
                                                        "Enable" : "Disable",
               pUsr->uHdl, pUsr->uNet ));

            cmFeatCtrl ( (void *) pUsr,
                         pEvt->uDat1,
                         ((CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt)
                                                      == eCMCMD_SVCSTART) ?
                                                            BOS_TRUE : BOS_FALSE );
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_PLACECALL:
      {
         /* Locate the user that is going to place this call.
         */
         BOS_BOOL bPlaceCall = BOS_FALSE;
         CMSES *pSes = NULL;
         CMSES *pSesFocus = NULL;
         CMUSR *pUsr = cmUsrFindUidHdl (
                           ((CMGRPLACECALL *) pEvt->pDat3)->uUid );

         if ( pUsr )
         {
            /* Allocate the physical interface for this call if none already set
            ** for the user.
            */
            if ( pUsr->uPhys == CMGR_INVALID_HANDLE )
            {
               cmPhyAllocate ( pUsr );
            }

            /* Allocate the media session for this call.
            */
            if ( ( pUsr->uPhys != CMGR_INVALID_HANDLE ) &&
                 ( ( pSes =
                     cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl ) ) != NULL ) )
            {
               /* If this is not the first call associated with this user,
               ** make sure it becomes the focus call and the current focus
               ** call is out in the background.
               */
               pSesFocus = cmUsrFocusSes ( pUsr );
               if ( pSesFocus )
               {
                  pSesFocus->ses.med.bFocus = BOS_FALSE;
               }
               pSes->ses.med.bFocus = BOS_TRUE;
               pSes->uApp = pEvt->uDat1;
               pSes->uNetUsr = cmUsrGetNtwkHdl( pUsr );

               cmUsrLinkSes ( pUsr, pSes );

               /* Put the user in the correct state to manage this action on
               ** the new session.
               */
               cmUsrNewState ( pUsr, eCMST_DIALING );

#if CMGRCFG_MULTILINE
               if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
               {
                  CMGREPSIG sig;
                  CMTMR tmr;

                  memset ( &sig, 0, sizeof ( CMGREPSIG ) );
                  memset ( &tmr, 0, sizeof ( CMTMR ) );

                  CMGRLOG_DEBUG (( "%s: No CCTK user available for [0x%X]", __FUNCTION__, pUsr->uHdl ));

                  sig.eAct = eCMGREPSIGACT_START;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = eEPTSIG_REORDER;
                  pUsr->uCpt = sig.uSig;

                  cmPhySignal ( pUsr->uPhys, &sig );

                  /* Don't start offhook warning timer if there are
                  ** other sessions for this user.
                  */
                  if( ( pUsr->pSesWait == NULL ) && ( pUsr->pSesHeld == NULL ) )
                  {
                     tmr.uSes = CMGR_INVALID_HANDLE;
                     tmr.uUsr = pUsr->uHdl;
                     tmr.eType = eCMEVT_TMR_OHWARN;
                     tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

                     cmTmrStart ( &tmr );
                  }

                  cmUsrUnLinkSes ( pUsr, pSes );
                  cmSesFree ( pSes, pUsr->uHdl );
                  return;
               }
#endif /* CMGRCFG_MULTILINE */

               /* Initiate the call to the network.
               */
               if ( strlen( (const char *) ((CMGRPLACECALL *) pEvt->pDat3)->uDest )
                    && ( cctkCallCreate ( &pSes->uNet,
                                          pSes->uNetUsr,
                                          ((CMGRPLACECALL *) pEvt->pDat3)->uDest )
                                                          == eCCTKSTATUS_SUCCESS ) )
               {
                  CMGRLOG_DEBUG ((
                     "cmUsrCmdHandler: call \'%s\' on 0x%X (0x%X) with 0x%X (0x%X)",
                     ((CMGRPLACECALL *) pEvt->pDat3)->uDest,
                     pUsr->uHdl, pSes->uNetUsr, pSes->uHdl, pSes->uNet ));

                  cmCfgInitCallMedia ( (void *) pUsr, pSes );

                  cctkCallMake ( pSes->uNet );
#if CMGRCFG_MULTILINE
                  cmCctkAccntUnforce( pSes->uNetUsr, pUsr->uHdl );
#endif /* CMGRCFG_MULTILINE */

                  cmFeatFaxOnly ( (void *) pUsr );

                  bPlaceCall = BOS_TRUE;
               }
            }
         }

         /* Reject the call attempt.
         */
         if ( !bPlaceCall )
         {
            CMGREPSIG sig;

            if ( pSes != NULL )
            {
               cmUsrUnLinkSes ( pUsr, pSes );
               cmSesFree ( pSes, pUsr->uHdl );
            }

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = pEvt->uDat1;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = eEPTSIG_REORDER;

            cmPhySignal ( pUsr->uPhys, &sig );
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_ENDCALL:
      {
         /* Locate the session for which this command will apply.  This is a forcefull call
         ** termination from the application.
         */
         BOS_BOOL bEndCall = BOS_FALSE;
         CMSES *pSes = cmSesFindAppHdl ( pEvt->uDat1 );

         if ( pSes )
         {
            CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

            if ( pUsr )
            {
               CMEVT evt;

               memset ( (void *) &evt,
                        0,
                        sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                  "cmUsrCmdHandler: end-call 0x%X (0x%X) on 0x%X (0x%X)",
                  pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_ENDCALL);
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;

               /* Apply call termination as dictated by the external application
               ** command via the physical endpoint handler state machine.
               */
               cmPhysIfCmdHandler ( &evt );
               bEndCall = BOS_TRUE;
            }
         }

         /* If we did not apply this event to any call, it may still be a valid
         ** event used by the application to idle any user that would not have
         ** been completely idled yet.  Check and apply idling if warranted.
         */
         if ( !bEndCall )
         {
            CMGRLOG_DEBUG ((
               "cmUsrCmdHandler: idling all applicable users" ));

            cmUsrIdle();
         }
      }
      break;

      case eCMCMD_APPCALLHDL:
      {
         /* Locate the session for which this command will apply.  This overrides any
         ** prior knowledge of the application handle for the session, the application
         ** handle is used to direct subsequent commands to the session from the above
         ** application.
         */
         CMSES *pSes = cmSesValid ( pEvt->uDat1 );

         if ( pSes )
         {
            CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

            /* Save the call handle in the session information.
            */
            pSes->uApp = pEvt->uDat2;

            CMGRLOG_DEBUG ((
               "cmUsrCmdHandler: set app hdl 0x%X on 0x%X (0x%X)",
               pSes->uApp, pSes->uHdl, pSes->uNet ));

            /* If we already had received media information for this particular session
            ** but the application handle was not set yet, that information is likely lost
            ** by the upper application.  For that, we trigger another media event instead.
            */
            if ( pUsr &&
                 ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE ) )
            {
               pUsr->evt.uDat2 = pSes->uHdl;
               pUsr->evt.pDat3 = NULL;

               cmFsmMedia ( (void *) pUsr );
            }
         }
      }
      break;

      case eCMCMD_ANSCALL:
      {
         /* Locate the session for which this command will apply.
         */
         CMSES *pSes = cmSesFindAppHdl ( pEvt->uDat1 );

         if ( pSes )
         {
            CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

            if ( pUsr )
            {
               CMEVT evt;

               memset ( (void *) &evt,
                        0,
                        sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                  "cmUsrCmdHandler: answer-call 0x%X (0x%X) on 0x%X (0x%X)",
                  pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_ANSWER);
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;

               /* Apply answer as dictated by the external application
               ** command via the physical endpoint handler state machine.
               */
               cmPhysIfCmdHandler ( &evt );
            }
         }
      }
      break;

      case eCMCMD_HOLDSVC:
      {
         /* Locate the session for which this command will apply.
         */
         BOS_BOOL bValid = BOS_FALSE;
         CMSES * pSes = NULL;
         CMGRHOLDSVC *pHoldSvc = pEvt->pDat3;
         CMUSR *pUsr = NULL;

         /* If apphdl is specified, use it to retreive session */
         if( pHoldSvc->uAppIx != CMGR_INVALID_HANDLE )
         {
            pSes = cmSesFindAppHdl ( pHoldSvc->uAppIx );
         }
         else
         {
            /* If apphdl is not specified, then find session by cid and userid */
            if( pHoldSvc->uUid != NULL && pHoldSvc->uCid != CMGR_INVALID_HANDLE )
            {
               /* Retrieve user */
               pUsr = cmUsrFindUidHdl( (BOS_UINT8*) pHoldSvc->uUid );
               if( pUsr )
               {
                  /* Retrieve session */
                  pSes = cmSesFindNetHdl( pHoldSvc->uCid, ( pUsr ) ? pUsr->uHdl : CMGR_INVALID_HANDLE );
               }
               else
               {
                  CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find User with uUid %s",
                                      (pHoldSvc->bHold ? "hold" : "unhold"),
                                      pHoldSvc->uUid));
               }
            }
         }

         if ( pSes )
         {
            pUsr = cmUsrValid ( pSes->uUsr );

            if ( pUsr )
            {
               CMEVT evt;

               memset ( (void *) &evt,
                        0,
                        sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                  "cmUsrCmdHandler: %s-svc 0x%X (0x%X) on 0x%X (0x%X)",
                  (pHoldSvc->bHold ? "hold" : "unhold"),
                  pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | (
                                 pHoldSvc->bHold ? eCMEVT_HOLD_SVC : eCMEVT_UNHOLD_SVC ));
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;

               /* Apply hold/unhold service as dictated by the external application
               ** command via the physical endpoint handler state machine.
               */
               bValid = BOS_TRUE;
               cmPhysIfCmdHandler ( &evt );
            }
            else
            {
               CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find User with session with cid 0x%X and appIdx 0x%X userHdl 0x%X",
                                   (pHoldSvc->bHold ? "hold" : "unhold"),
                                   pHoldSvc->uCid,
                                   pHoldSvc->uAppIx,
                                   pSes->uUsr));
            }
         }
         else
         {
            CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find session with cid 0x%X and appIdx 0x%X",
                                (pHoldSvc->bHold ? "hold" : "unhold"),
                                pHoldSvc->uCid,
                                pHoldSvc->uAppIx));
         }

         if ( !bValid && gCmCfgCb.cmgrEvtCb )
         {
            CMGRSVC eSvc = ( pHoldSvc->bHold ? eCMGRSVC_LOCAL_HOLD :
                                           eCMGRSVC_LOCAL_UNHOLD );
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                     pHoldSvc->uAppIx,
#if CMGRCFG_MULTILINE
                                     (pSes ? pSes->uUsr : CMGR_INVALID_HANDLE),
                                     (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &eSvc );
         }
      }
      break;

      case eCMCMD_CONFSVC:
      {
         /* Locate and validate the sessions for which this command will apply.
         */
         BOS_BOOL bValid = BOS_FALSE;

         if ( pEvt->pDat3 )
         {
            CMGRCONFSVC *pConfSvc = (CMGRCONFSVC *) pEvt->pDat3;
            CMSES *pSes_1 = cmSesFindAppHdl ( pConfSvc->uAppIx );
            CMSES *pSes_2 = cmSesFindAppHdl ( pConfSvc->uSesHdl );

            if ( pSes_1 && pSes_2 )
            {
               CMUSR *pUsr_1 = cmUsrValid ( pSes_1->uUsr );
               CMUSR *pUsr_2 = cmUsrValid ( pSes_2->uUsr );

               /* Replace external handles with internal ones for further
               ** processing of the event.
               */
               pConfSvc->uAppIx = pSes_1->uHdl;
               pConfSvc->uSesHdl = pSes_2->uHdl;

               if ( pUsr_1 &&
                    pUsr_2 &&
                    /* The owner of those sessions must be the same.
                    */
                    (pUsr_1 == pUsr_2) )
               {
                  CMEVT evt;

                  memset ( (void *) &evt,
                           0,
                           sizeof ( CMEVT ) );

                  CMGRLOG_DEBUG ((
                     "cmUsrCmdHandler: conf-svc 0x%X (0x%X) and 0x%X (0x%X)" \
                     "on 0x%X (0x%X)",
                     pSes_1->uHdl, pSes_1->uNet,
                     pSes_2->uHdl, pSes_2->uNet,
                     pUsr_1->uHdl, pUsr_2->uNet ));

                  evt.uEvt =
                     (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_CONF_SVC);
                  evt.uDat1 = pUsr_1->uHdl;
                  evt.pDat3 = pEvt->pDat3;

                  /* Apply conference service as dicated by the application.
                  */
                  bValid = BOS_TRUE;
                  cmPhysIfCmdHandler ( &evt );
               }
            }
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }

         if ( !bValid && gCmCfgCb.cmgrEvtCb )
         {
            CMGRSVC eSvc = eCMGRSVC_CONF;

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                     pEvt->uDat1,
#if CMGRCFG_MULTILINE
                                     pEvt->uDat1, /* pUsr->uHdl */
                                     CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &eSvc );
         }
      }
      break;

      case eCMCMD_CXFERSVC:
      {
         /* Locate and validate the sessions for which this command will apply.
         */
         BOS_BOOL bValid = BOS_FALSE;

         if ( pEvt->pDat3 )
         {
            CMGRAPPCALLHDL *pXferSvc = (CMGRAPPCALLHDL *) pEvt->pDat3;
            CMSES *pSes_1 = cmSesFindAppHdl ( pXferSvc->uAppIx );
            CMSES *pSes_2 = cmSesFindAppHdl ( pXferSvc->uSesHdl );

            if ( pSes_1 && pSes_2 )
            {
               CMUSR *pUsr_1 = cmUsrValid ( pSes_1->uUsr );
               CMUSR *pUsr_2 = cmUsrValid ( pSes_2->uUsr );

               /* Replace external handles with internal ones for further
               ** processing of the event.
               */
               pXferSvc->uAppIx = pSes_1->uHdl;
               pXferSvc->uSesHdl = pSes_2->uHdl;

               if ( pUsr_1 &&
                    pUsr_2 &&
                    /* The owner of those sessions must be the same.
                    */
                    (pUsr_1 == pUsr_2) )
               {
                  CMEVT evt;

                  memset ( (void *) &evt,
                           0,
                           sizeof ( CMEVT ) );

                  CMGRLOG_DEBUG ((
                     "cmUsrCmdHandler: cxfer-svc 0x%X (0x%X) and 0x%X (0x%X)" \
                     "on 0x%X (0x%X)",
                     pSes_1->uHdl, pSes_1->uNet,
                     pSes_2->uHdl, pSes_2->uNet,
                     pUsr_1->uHdl, pUsr_2->uNet ));

                  evt.uEvt =
                     (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_CXFER_SVC);
                  evt.uDat1 = pUsr_1->uHdl;
                  evt.pDat3 = pEvt->pDat3;

                  /* Apply conference service as dicated by the application.
                  */
                  bValid = BOS_TRUE;
                  cmPhysIfCmdHandler ( &evt );
               }
            }
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }

         if ( !bValid && gCmCfgCb.cmgrEvtCb )
         {
            CMGRSVC eSvc = eCMGRSVC_CXFER;

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                     pEvt->uDat1,
#if CMGRCFG_MULTILINE
                                     pEvt->uDat1, /* pUsr->uHdl */
                                     CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &eSvc );
         }
      }
      break;

      case eCMCMD_BXFERSVC:
      {
         /* Locate the session for which this command will apply.
         */
         BOS_BOOL bValid = BOS_FALSE;
         CMSES *pSes = cmSesFindAppHdl ( pEvt->uDat1 );

         if ( pSes )
         {
            CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

            if ( pUsr )
            {
               CMEVT evt;

               memset ( (void *) &evt,
                        0,
                        sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                  "cmUsrCmdHandler: bxfer-svc 0x%X (0x%X) on 0x%X (0x%X) to \'%s\'",
                  pSes->uHdl, pSes->uNet,
                  pUsr->uHdl, pSes->uNetUsr,
                  ((CMGRPLACECALL *) pEvt->pDat3)->uDest ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_BXFER_SVC);
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;
               evt.pDat3 = pEvt->pDat3;

               /* Apply blind transfer service as dictated by the external application
               ** command via the physical endpoint handler state machine.
               */
               bValid = BOS_TRUE;
               cmPhysIfCmdHandler ( &evt );
            }
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }

         if ( !bValid && gCmCfgCb.cmgrEvtCb )
         {
            CMGRSVC eSvc = eCMGRSVC_BXFER;
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( (pSes ? pSes->uNetUsr : CMGR_INVALID_HANDLE) );

            ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_SVC_DENY,
                                     pEvt->uDat1,
#if CMGRCFG_MULTILINE
                                     (pSes ? pSes->uUsr : CMGR_INVALID_HANDLE),
                                     (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                     (void *) &eSvc );
         }
      }
      break;

      case eCMCMD_DTMFPT:
      {
         /* Locate the session for which this command will apply.
         */
         if ( pEvt->pDat3 )
         {
            CMGRAPPCALLHDL *pDtmfSvc = (CMGRAPPCALLHDL *) pEvt->pDat3;
            CMSES *pSes = cmSesFindAppHdl ( pDtmfSvc->uAppIx );

            if ( pSes )
            {
               CMUSR *pUsr = cmUsrValid ( pSes->uUsr );

               if ( pUsr )
               {
                  CMEVT evt;

                  memset ( (void *) &evt,
                           0,
                           sizeof ( CMEVT ) );

                  CMGRLOG_DEBUG ((
                     "cmUsrCmdHandler: dtmf-pass-through 0x%X (0x%X) on 0x%X (0x%X)",
                     pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

                  evt.uEvt =
                     (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_DTMFPT);
                  evt.uDat1 = pUsr->uHdl;
                  evt.uDat2 = pSes->uHdl;
                  evt.pDat3 = pEvt->pDat3;

                  /* Apply dtmf passthrough as dictated by the external application
                  ** command via the physical endpoint handler state machine.
                  */
                  cmPhysIfCmdHandler ( &evt );
               }
            }
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_CLEARMWI:
      case eCMCMD_SETMWI:
      {
         CMUSR *pUsr = cmUsrFindUidHdl ( (BOS_UINT8 *) pEvt->pDat3 );

         if ( pUsr )
         {
            CCTKMWIINFO mwiInfo;

            /* Create the equivalent of a MWI indication that would terminate
            ** message waiting to cleanly stop any processing associated with
            ** MWI.
            */
            memset ( &mwiInfo, 0, sizeof ( CCTKMWIINFO ) );
            /*Status of messages waiting - true if message waiting, false otherwise*/
            mwiInfo.bMsgStatus =
                  ((CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt)== eCMCMD_CLEARMWI) ? BOS_FALSE : BOS_TRUE;

            CMGRLOG_DEBUG ((
               "cmUsrCmdHandler: mwiInfo.bMsgStatus %d  user 0x%X (0x%X)",
               mwiInfo.bMsgStatus, pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

            pUsr->evt.pDat3 = (void *) &mwiInfo;

            /* Directly invoke the correct FSM functionality.
            */
            cmFsmMwi ( (void *) pUsr );
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_STATUS:
      {
         /* If CMGR status information has been requested.
         */
         if ( (BOS_BOOL) pEvt->uDat1 )
         {
            cmShow();
         }

         /* If CCTK status information has been requested.
         */
         if ( (BOS_BOOL) pEvt->uDat2 )
         {
            cctkStatusShow();
         }

#if CMGRCFG_MULTILINE
         /* Display CCTK Account settings */
         cmCctkAccntShow();
#endif /* CMGRCFG_MULTILINE */
      }
      break;

      case eCMCMD_DATALNK:
      {
         /* Pass the data link status information to the user
         ** manager module.
         */
         cmUsrDtLnk ( (BOS_BOOL) pEvt->uDat1 );
      }
      break;

#if CMGRCFG_PASS_THRU_MSG
      case eCMCMD_MESSAGE:
      {
         /* Locate the user that is going to place this request.
         */
         CMGRPASSTHRUMSG *pMsg = (CMGRPASSTHRUMSG *) pEvt->pDat3;
         CMUSR *pUsr = cmUsrFindUidHdl ( pMsg->toDest.uUid );
         BOS_UINT32 uMsgGuid;

         if ( pUsr )
         {
            BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );
            /* Send the pass through message to the destination specified
            ** via the CCTK layer.
            */
            cctkPassThruMsg ( &uMsgGuid,
                              uAccount,
                              pMsg->toDest.uDest,
                              pMsg->puMsg,
                              pMsg->uSize );

            CMGRLOG_DEBUG ((
               "cmUsrCmdHandler: msg (%p, %u) to \'%s\' on 0x%X (0x%X) with 0x%X",
               pMsg->puMsg, pMsg->uSize,
               pMsg->toDest.uDest,
               pUsr->uHdl, uAccount, uMsgGuid ));

            /* Issue an event to the application to let it know about the mapping
            ** of the CMGR handle to the Application Handle. When a final response
            ** will be received for the message, the CMGR will publish it to the
            ** application using the CMGR handle, that is because CMGR does not
            ** allocate resources to track this pass through information.
            */
            if ( gCmCfgCb.cmgrEvtCb )
            {
               CMGRAPPCALLHDL hdl;

               /* This is the handle assigned by the system to be remembered
               ** because the same handle will be used to pass the final response.
               */
               hdl.uSesHdl = uMsgGuid;
               /* This is the handle provided by the external application when it
               ** sent the message via CMGR.
               */
               hdl.uAppIx = pEvt->uDat1;

               gCmCfgCb.cmgrEvtCb ( eCMGREVT_MSG_HDL,
                                    pUsr->uHdl,
#if CMGRCFG_MULTILINE
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                    (void *) &hdl );
            }
         }

         /* Clean up allocated resources.
         */
         if ( pMsg->puMsg != NULL )
         {
            free ( (void *) pMsg->puMsg );
            pMsg->puMsg = NULL;
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }

      }
      break;

      case eCMCMD_MSG_RESP:
      {
         /* Send the message pass through response via CCTK directly, there
         ** is no need for any processing at the CMGR layer at this time.
         */
         cctkPassThruRsp ( pEvt->uDat1,
                           pEvt->uDat2 );
      }
      break;
#endif /* CMGRCFG_PASS_THRU_MSG */

#if CMGRCFG_DECT
      case eCMCMD_START_DIGCOLL:
      {
         CMGRSTRTDIGCOLL* pStDigCol = (CMGRSTRTDIGCOLL *)pEvt->pDat3;
         CMUSR           *pUsr = NULL;
         CMEVT evt;

         if( pStDigCol )
         {
             CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_START_DIGCOLL uPhy %u", pStDigCol->uPhy));
             pUsr = cmUsrFindPhyHdl( pStDigCol->uPhy );
         }

         if(pUsr)
         {
            CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_START_DIGCOLL uUid %s", pUsr->uUid));

            /* We are pending a parallel call setup - set this to prevent flash from performing
             * Callwaiting switching or 3-way call conferencing. */
#if CMGRCFG_MULTILINE
            pUsr->bDisableAutoCallSwitch = BOS_TRUE;
#endif
            /* If there is a focused established session, put it on hold */
            memset ( (void *) &evt,
                     0,
                     sizeof ( CMEVT ) );

            CMGRLOG_DEBUG ((
                "cmUsrCmdHandler: eCMCMD_START_DIGCOLL on usrHdl 0x%X",
                pUsr->uHdl));

            evt.uEvt =
               (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_FLASH);
            evt.uDat1 = pUsr->uHdl;
            evt.uDat2 = CMGR_INVALID_HANDLE;
            evt.pDat3 = NULL;
            evt.bLast = BOS_TRUE;

            /* Run the event right away */
            cmPhysIfCmdHandler( &evt );
         }
         else
         {
            CMGRLOG_CRITICAL (( "cmUsrCmdHandler: eCMCMD_START_DIGCOLL cannot find user object for uPhy %u",
                                pStDigCol->uPhy));
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_TERMINATE_CALL:
      {
         CMUSR* pUsr = NULL;
         CMSES* pSes = NULL;
         CMGRTERMCALL* pTermCall = (CMGRTERMCALL *)pEvt->pDat3;
         CMSESLIST sesList;
         BOS_UINT32 uIy;

         CMGRLOG_INFORMATION (( "cmUsrCmdHandlerI: eCMCMD_TERMINATE_CALL cid 0x%X",
                                   pTermCall->uCid));

         /* Get list of sessions with same cid */
         memset( &sesList, 0, sizeof( CMSESLIST ) );
         cmSesListWithCid( pTermCall->uCid, &sesList );

         /* User not specified, Terminate this call on all users */
         for( uIy = 0 ; uIy < sesList.uLength ; uIy++ )
         {
            pSes = cmSesValid( sesList.uSes[ uIy ] );

            if( pSes )
            {
               pUsr = cmUsrValid( pSes->uUsr );

               /* If its an internal call, simply end the call for all users with matchin cid */
               if( pSes->uNetUsr == CMGR_INVALID_HANDLE)
               {
                  CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_TERMINATE_CALL Internal call being terminated on usrHdl 0x%X cid 0x%X",
                                pSes->uUsr, pTermCall->uCid ));

                   /* Set which session to close and end the call. */
                  pUsr->evt.uDat2 = pSes->uHdl;
                  cmFsmCallEnding( (void*)pUsr );
               }
               else
               {
                  /* External call need to delete call from system */
                  CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_TERMINATE_CALL External call being terminated on usrHdl 0x%X cid 0x%X",
                                pSes->uUsr, pTermCall->uCid ));

                  cctkCallDelete ( pTermCall->uCid, eCCTKCALLDELRSN_NORMAL);
                  break;
               }
            }
            else
            {
               CMGRLOG_CRITICAL (( "cmUsrCmdHandler: eCMCMD_TERMINATE_CALL cannot find session object for sesHdl 0x%X cid 0x%X",
                                   sesList.uSes[ uIy ],
                                   pTermCall->uCid));
            }
         }

         if( pUsr == NULL )
         {
            CMGRLOG_CRITICAL(( "%s: No user found with for call with cid [%X]", __FUNCTION__,  pTermCall->uCid ));
         }
         else if( pSes == NULL )
         {
            CMGRLOG_CRITICAL(( "%s: No session found with cid [%X] for userHandle [%X]", __FUNCTION__, pTermCall->uCid, pUsr->uHdl ));
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_REJECT_CALL:
      {
         CMUSR* pUsr = NULL;
         CMSES* pSes = NULL;
         CMGRREJCALL* pRejCall = (CMGRREJCALL *)pEvt->pDat3;
         CMSESLIST sesList;
         BOS_UINT32 uIy;

         if( pRejCall )
         {
            CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL uPhy %u cid 0x%X",
                                      pRejCall->uPhy,
                                      pRejCall->uCid));

            /* Get user object */
            pUsr = cmUsrFindPhyHdl( pRejCall->uPhy );
         }

         /* Get list of sessions with same cid */
         memset( &sesList, 0, sizeof( CMSESLIST ) );
         cmSesListWithCid( pRejCall->uCid, &sesList );

         if( pUsr )
         {
            /* User is specified - This is a call waiting reject */
            CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL on userHdl 0x%X cid 0x%X",
                                   pUsr->uHdl,
                                   pRejCall->uCid));

            /* Get session object */
            pSes = cmSesFindNetHdl( pRejCall->uCid, ( pUsr ) ? pUsr->uHdl : CMGR_INVALID_HANDLE );

            if( pSes )
            {
               /* Check If multiple sessions exist with the same cid ( this  *
                * indicates calls inringing states and also internal calls ) */
               if( cmNumSesWithCid( pSes->uNet ) > 1 )
               {
                  CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL Multiple sessions exist with uNet 0x%X",
                                   pSes->uNet ));

                  /* Check if this is an internal call with only two parties */
                  if( (pSes->uNetUsr == CMGR_INVALID_HANDLE) && (cmNumSesWithCid( pSes->uNet ) == 2) )
                  {
                     for( uIy = 0 ; uIy < sesList.uLength ; uIy++ )
                     {
                        pSes = cmSesValid( sesList.uSes[ uIy ] );

                        if( pSes )
                        {
                           /* Get user from session */
                           pUsr = cmUsrValid( pSes->uUsr );

                           CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL Internal call being terminated on usrHdl 0x%X cid 0x%X",
                                            pSes->uUsr, pRejCall->uCid ));

                           if( pUsr )
                           {
                              pUsr->evt.uDat2 = pSes->uHdl;

                              /* differentiate between caller and callee and take appropriate action */
                              if( pSes->bEstab )
                              {
                                 /* Internal call caller - end call and play busy tone*/
                                 cmFsmCallRejectBusy( (void*)pUsr );
                              }
                              else
                              {
                                 /* Internal call callee - end call*/
                                 cmFsmCallEnding( (void*)pUsr );
                              }
                           }
                           else
                           {
                              /* Invalid user */
                              CMGRLOG_CRITICAL (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL cannot find User object for usrHdl 0x%X cid 0x%X",
                                                  pSes->uUsr,
                                                  pRejCall->uCid));
                              if ( pEvt->pDat3 != NULL )
                              {
                                 free ( (void *) pEvt->pDat3  );
                                 pEvt->pDat3 = NULL;
                              }
                              break;
                           }
                        }
                        else
                        {
                           /* Invalid session */
                           CMGRLOG_CRITICAL (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL cannot find session object for sesHdl 0x%X cid 0x%X",
                                               sesList.uSes[ uIy ],
                                               pRejCall->uCid));
                           if ( pEvt->pDat3 != NULL )
                           {
                              free ( (void *) pEvt->pDat3  );
                              pEvt->pDat3 = NULL;
                           }
                           break;
                        }
                     }
                  }
                  else
                  {
                     /* external call with multiple sessions or internal call with more than 2 sessions - Just end call on this user */
                     pUsr->evt.uDat2 = pSes->uHdl;
                     cmFsmCallEnding( (void*)pUsr );
                  }
               }
               else if ( cmNumSesWithCid( pSes->uNet ) == 1 )
               {
                  /* There is only one session with this cid - external call , must delete the call from the system */
                  CMGRLOG_INFORMATION (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL Single external session exists with uNet 0x%X",
                                   pSes->uNet ));
                  cctkCallDelete( pRejCall->uCid, eCCTKCALLDELRSN_NORMAL);
               }
            }
            else
            {
               CMGRLOG_CRITICAL (( "cmUsrCmdHandler: eCMCMD_REJECT_CALL cannot find session for userHdl 0x%X cid 0x%X",
                                   pUsr->uHdl,
                                   pRejCall->uCid));
            }
         }
         else
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_REJECT_CALL No user found with uPhy [%u] and cid [%X]", __FUNCTION__,  pRejCall->uPhy, pRejCall->uCid ));
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_SET_PREF_LINE:
      {
         CMUSR* pUsr = NULL;
         CMCCTKACCNT* pLine = NULL;
         CMGRSETPREFLINE* pSetPrefLine = (CMGRSETPREFLINE *)pEvt->pDat3;

         if(pSetPrefLine)
         {
            pLine = cmCctkAccntFindByHdl( pSetPrefLine->uLine );
            pUsr  = cmUsrFindPhyHdl( pSetPrefLine->uPhy );
         }

         if( (pUsr != NULL) && (pLine != NULL) )
         {
            cmCctkAccntForce( pLine->uNet, pUsr->uHdl );
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_TOGGLE_CALL:
      {
         CMUSR* pUsr = NULL;
         CMGRTOGGLECALL* pToggle = (CMGRTOGGLECALL *) pEvt->pDat3;

         if( pToggle )
         {
            CMGRLOG_INFORMATION (( "%s: eCMCMD_TOGGLE_CALL - uPhy [%u], cid [%X]",
                                   __FUNCTION__,
                                   pToggle->uPhy,
                                   pToggle->uCid ));


            pUsr = cmUsrFindPhyHdl( pToggle->uPhy );
         }

         if( pUsr != NULL )
         {
            /* Search through the media sessions for the specified session */
            CMEVT evt;
            CMSES* pSes = cmUsrFocusSes ( pUsr );
            CMSES* pOtherSes = NULL;
            BOS_UINT32 uIx;

            /* If a call id was specified from the controlling application, attemp to find it and
             * switch to it specifically. If the call id wasn't specified, this command acts like
             * a simple flash event. */
            if( pToggle->uCid != CMGR_INVALID_HANDLE )
            {
               for( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
               {
                  if( pUsr->pMedSes[ uIx ] && pUsr->pMedSes[ uIx ]->uNet == pToggle->uCid )
                  {
                     pOtherSes = pUsr->pMedSes [ uIx ];
                     break;
                  }
               }

               if( !pOtherSes )
               {
                  CMGRLOG_CRITICAL(( "%s: eCMCMD_TOGGLE_CALL - Unable to find session with cid [%X]", __FUNCTION__, pToggle->uCid ));

                  /* Fix me!! */
                  /* Basically finds another session if the specified session is not found and
                  ** makes it the waiting session so on the flash event generated we will connect to
                  ** the waiting session - but need to do this properly
                  */


                  /* Try to find any other session and switch to it instead */
                  for( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
                  {
                     if( pUsr->pMedSes[ uIx ] && pUsr->pMedSes[ uIx ] != pSes )
                     {
                        pOtherSes = pUsr->pMedSes [ uIx ];
                        break;
                     }
                  }

                  /* If we still couldn't find another session, we're done here */
                  if( !pOtherSes )
                  {
                     free( (void *) pEvt->pDat3 );
                     pEvt->pDat3 = NULL;
                     break;
                  }
                  /* If another session exists for the user, attempt to switch to it regardless */
                  else
                  {
                     CMGRLOG_DEBUG(( "%s: eCMCMD_TOGGLE_CALL - Switching to session [%X] anyways.", __FUNCTION__, pOtherSes->uNet ));
                  }
                  /* Fix me!! */
               }
               else if( pOtherSes == pSes )
               {
                  CMGRLOG_DEBUG(( "%s: eCMCMD_TOGGLE_CALL - Call [%X] is already in focus. Ignoring.", __FUNCTION__, pToggle->uCid ));
                  free( (void *) pEvt->pDat3 );
                  pEvt->pDat3 = NULL;
                  break;
               }
               /* If we get this far, the specified call must exist. Set it to the call waiting session and
                * signal a FLASH event to switch to the call. */
               pUsr->pSesWait = pOtherSes;

               cmFsmCallSwitchActive ( (void *)pUsr );
            }
            else
            {
                evt.uEvt =
                    (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_FLASH);
                    evt.uDat1 = pUsr->uHdl;
                    evt.bLast = BOS_TRUE;
                    cmPhysIfCmdHandler( &evt );
            }
         }

         if( pEvt->pDat3 )
         {
            free( (void *) pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      /* This is essentially a duplicate of the CONFSVC command, but does slightly
       * different processing to handle internal calls. */
      case eCMCMD_CONFERENCE_CALL:
      {
         /* Locate and validate the sessions for which this command will apply. */
         CMSES *pSes_1 = NULL;
         CMSES *pSes_2 = NULL;
         CMUSR *pUsr = NULL;
         CMGRCONFCALL* pConf = (CMGRCONFCALL *) pEvt->pDat3;

         if( !pEvt->pDat3 )
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_CONFERENCE_CALL - NULL parameter!!", __FUNCTION__ ));
            break;
         }


         pUsr = cmUsrFindPhyHdl( pConf->uPhy );
         if( !pUsr )
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_CONFERENCE_CALL - Unable to find uPhy [%u]",
                               __FUNCTION__,
                               (char*)pConf->uPhy ));

            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
            break;
         }

         pSes_1 = cmSesFindNetHdl( pConf->uCid1, pUsr->uHdl );
         pSes_2 = cmSesFindNetHdl( pConf->uCid2, pUsr->uHdl );

         if ( pSes_1 && pSes_2 )
         {
            CMEVT evt;
            /* The CMGR conference service expects a CMGRCONFSVC item as pData */
            CMGRCONFSVC cmgrConfSvc;

            memset( &cmgrConfSvc, 0, sizeof( CMGRCONFSVC ) );
            memset ( &evt, 0, sizeof ( CMEVT ) );

            cmgrConfSvc.uAppIx  = pSes_1->uHdl;
            cmgrConfSvc.uSesHdl = pSes_2->uHdl;
            cmgrConfSvc.bLocalConf = BOS_FALSE;

            CMGRLOG_DEBUG(( "%s: conf-svc 0x%X (0x%X) and 0x%X (0x%X)" \
                            "on 0x%X",
                            __FUNCTION__,
                            pSes_1->uHdl, pSes_1->uNet,
                            pSes_2->uHdl, pSes_2->uNet,
                            pUsr->uHdl ));

            evt.uEvt =
               (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_CONF_SVC);
            evt.uDat1 = pUsr->uHdl;
            evt.pDat3 = (void*)&cmgrConfSvc;

            /* Apply conference service as dicated by the application. */
            cmPhysIfCmdHandler ( &evt );
         }

         free ( (void *) pEvt->pDat3 );
         pEvt->pDat3 = NULL;
      }
      break;

      case eCMCMD_XFER_CALL:
      {
         CMEVT evt;
         CMUSR* pUsr = NULL;
         CMGRXFERCALL* pXfer = (CMGRXFERCALL *) pEvt->pDat3;

         if( pXfer )
         {
            CMGRLOG_INFORMATION(( "%s: eCMCMD_XFER_CALL - on uPhy [%u]!", __FUNCTION__, pXfer->uPhy ));
            pUsr = cmUsrFindPhyHdl( pXfer->uPhy );
         }

         if( pUsr )
         {
            /* Fix me!! */
            /* Clear the wait and held sessions to ensure the transfer scenarios get executed properly. */
            pUsr->pSesWait = NULL;
            pUsr->pSesHeld = NULL;
            /* Send an onhook event - this will force CMGR to apply any necessary transfer processing */
            evt.uEvt =
               (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_ONHOOK);
            evt.uDat1 = pUsr->uHdl;

            cmPhysIfCmdHandler( &evt );
            /* Fix me!! */
         }
         else
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_XFER_CALL - Unable to find user on uPhy [%u]",
                               __FUNCTION__,
                               (char*)pXfer->uPhy ));
         }

         if( pEvt->pDat3 );
         {
            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      /* This is essentially a duplicate of the HOLDSVC command, but does slightly
       * different processing. */
      case eCMCMD_HOLD_CALL:
      {
         CMSES * pSes = NULL;
         CMGRHOLDCALL *pHold = NULL;
         CMUSR *pUsr = NULL;

         if( !pEvt->pDat3 )
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_HOLD_CALL - NULL parameter!!", __FUNCTION__ ));
            break;
         }

         pHold = (CMGRHOLDCALL *) pEvt->pDat3;

         /* Retrieve user */
         pUsr = cmUsrFindPhyHdl( pHold->uPhy );
         if( !pUsr )
         {
            CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find User with uPhy 0x%X",
                                     (pHold->bHold ? "hold" : "unhold"),
                                      pHold->uPhy));
            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
            break;
         }

         /* Retrieve session */
         pSes = cmSesFindNetHdl( pHold->uCid, ( pUsr ) ? pUsr->uHdl : CMGR_INVALID_HANDLE );
         if ( pSes )
         {
            pUsr = cmUsrValid ( pSes->uUsr );

            if ( pUsr )
            {
               CMEVT evt;

               memset ( (void *) &evt,
                        0,
                        sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                  "cmUsrCmdHandler: %s-svc 0x%X (0x%X) on 0x%X (0x%X)",
                  (pHold->bHold ? "hold" : "unhold"),
                  pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | (
                                 pHold->bHold ? eCMEVT_HOLD_SVC : eCMEVT_UNHOLD_SVC ));
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;

               /* Apply hold/unhold service as dictated by the external application
               ** command via the physical endpoint handler state machine.
               */
               cmPhysIfCmdHandler ( &evt );
            }
            else
            {
               CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find User with session with cid 0x%X and userHdl 0x%X",
                                   (pHold->bHold ? "hold" : "unhold"),
                                   pHold->uCid,
                                   pSes->uUsr));
            }
         }
         else
         {
            CMGRLOG_WARNING (( "cmUsrCmdHandler: %s-svc cant find session with cid 0x%X",
                                (pHold->bHold ? "hold" : "unhold"),
                                pHold->uCid));
         }

         free( pEvt->pDat3 );
         pEvt->pDat3 = NULL;
      }
      break;

      case eCMCMD_INTERCEPT_CALL:
      {
         CMEVT evt;
         CMTMR tmr;
         CMGREPSIG sig;
         CMGRINTERCEPTCALL* pCallIntercept = (CMGRINTERCEPTCALL *) pEvt->pDat3;
         CMUSR* pUsr = NULL;
         CMUSR* pOtherUsr = NULL;
         CMSES* pSes = NULL;
         BOS_UINT32 uIx;

         if( pCallIntercept )
         {
            pUsr = cmUsrFindUidHdl( pCallIntercept->uPhy );
            pOtherUsr = cmUsrFindUidHdl( pCallIntercept->uPhyOther );
         }

         if( pOtherUsr )
         {
            /* Check or locate other active session.
            */
            for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
            {
               if ( pOtherUsr->pMedSes[ uIx ] &&
                    pOtherUsr->pMedSes[ uIx ]->uNet == pCallIntercept->uCid )
               {
                  pSes = pOtherUsr->pMedSes [ uIx ];
                  break;
               }
            }
            if( !pSes )
            {
               pSes = cmUsrFocusSes( pOtherUsr );
            }
         }

         if( pUsr && pOtherUsr && pSes )
         {
            /* Turn off dialtone/dial timers on this terminal */
            memset ( &tmr, 0, sizeof ( CMTMR ) );
            memset ( &sig, 0, sizeof ( CMGREPSIG ) );
            pUsr->eDigDetState = CMDIGITDETSTA_INPROGRESS;

            /* Stop dial tone and dial tone timer.
            */
            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;   /* eEPTSIG_DIAL or eEPTSIG_SPECIAL_DIAL */
            pUsr->uCpt = eEPTSIG_NULL;

            cmPhySignal ( pUsr->uPhys, &sig );

            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.eType = eCMEVT_TMR_DIALTONE;

            cmTmrStop ( &tmr );

#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
            /* Stop warm line timer in case it was running.
            */
             if ( pUsr->bWarmLineActive )
             {
                tmr.uUsr = pUsr->uHdl;
                tmr.uSes = CMGR_INVALID_HANDLE;
                tmr.eType = eCMEVT_TMR_WARMLINE;

                cmTmrStop ( &tmr );
             }
#endif
            if( pSes->bEstab &&
                pSes->ses.med.uPhy != CMGR_INVALID_HANDLE &&
                ( pOtherUsr->eSta[CMFSM_CURRENT_STATE_IX] == eCMST_TALK ||
                  pOtherUsr->eSta[CMFSM_CURRENT_STATE_IX] == eCMST_FLASH ) )
            {
               /* If the call which we are attempting to intercept is currently established,
               ** we must take the following steps:
               **
               ** 1) Copy the other user's call media information to this user's focus session
               ** 2) Teardown the other user's session
               ** 3) Create the identical session for this user
               **
               ** Note: CCTK thinks the call is still up, so we might leak RTP here.
               */
               CMSES* pSes1 = cmUsrFocusSes( pUsr );
               BOS_UINT32 uCid1 = pSes->uNet;
               BOS_UINT32 uHdl1 = pSes1->uHdl;

               CMGRLOG_INFORMATION(( "%s: INTERCEPT - Call established; intercepting...", __FUNCTION__ ));

               memcpy( pSes1, pSes, sizeof( CMSES ) );
               pSes1->ses.med.pAutoCall = NULL;
               pSes1->uUsr = pUsr->uHdl;
               pSes1->uHdl = uHdl1;
               /* When we perform the cmSesPhyDelete on the other user, if this was an internal call,
               ** it will also teardown this newly created session. To get around this, temporarily
               ** set the call id of the new call to invalid, then reset it after tearing down the
               ** other user's call.
               */
               pSes1->uNet = CMGR_INVALID_HANDLE;

               /* Teardown the session on the other user */
               pSes->bEstab = BOS_FALSE; /* This will prevent cmSesFree from removing the call from the CCTK Account */

               /* Ensure the call gets ended on the other user */
               pOtherUsr->evt.uDat2 = pSes->uHdl;
               cmFsmCallEnd( (void *)pOtherUsr );

               /* Restore the call id and setup this user's media */
               pSes1->uNet = uCid1;
               cmUsrLinkSes( pUsr, pSes1 );
               /* Since this session actually doesn't have a valid stream, we must create one. */
               pSes1->ses.med.uPhy = CMGR_INVALID_HANDLE;
               cmSesPhyCreate( pSes1 );
               pSes1->bEstab = BOS_TRUE;

               /* Set us into talk state */
               cmUsrNewState ( pUsr, eCMST_TALK );
            }
            else if( pOtherUsr->eSta[CMFSM_CURRENT_STATE_IX] == eCMST_RINGING ||
                     ( pOtherUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_TALK &&
                       pOtherUsr->pSesWait != NULL ) )
            {
               /* If the call is currently ringing, or we have a call-waiting call
               ** on the terminal, we should take the following steps:
               **
               ** 1) Copy the call info into the focus session for this terminal
               ** 2) Put this terminal into "ringing" state
               ** 3) Fake an offhook event to answer the incoming call again on this terminal
               **
               ** Answering the call will implicitly tear-down the call info on the
               ** other terminal.
               */
               CMSES* pSes1 = cmUsrFocusSes( pUsr );

               CMGRLOG_INFORMATION(( "%s: INTERCEPT - Call ringing; intercepting...", __FUNCTION__ ));

               /* Copy in the relevant info */
               pSes1->uNet = pSes->uNet;
               pSes1->uApp = pSes->uApp;
               pSes1->eType = pSes->eType;
               pSes1->uNetUsr = pSes->uNetUsr;
               pSes1->bEstab = pSes->bEstab;

               pUsr->bOffHook = BOS_FALSE; /* Fake us being onhook */

               /* Copy the call info */
               memcpy ( &pSes1->ses.med,
                        &pSes->ses.med,
                        sizeof ( CMSESMED ) );
               pSes1->ses.med.pAutoCall = NULL;

               cmUsrNewState ( pUsr, eCMST_RINGING ); /* Put us in the "ringing" state */

               /* Finally, send an offhook event to this terminal to properly answer the call */
               memset( &evt, 0, sizeof( CMEVT ) );
               evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_OFFHOOK);
               evt.uDat1 = pUsr->uHdl;

               /* Run the event right away */
               cmPhysIfCmdHandler( &evt );
            }
            else if( pOtherUsr->eSta[CMFSM_CURRENT_STATE_IX] == eCMST_RINGBACK )
            {
               /* If the other terminal is currently in ringback, we should take the following steps:
               **
               ** 1) Copy the session info from the other terminal to this term inal
               ** 2) End the call on the other terminal
               ** 3) Start ringback on this user
               */
               CMSES* pSes1 = cmUsrFocusSes( pUsr );
               BOS_UINT32 uCid1 = pSes->uNet;
               BOS_UINT32 uHdl1 = pSes1->uHdl;

               CMGRLOG_INFORMATION(( "%s: INTERCEPT - Call in ringback; intercepting...", __FUNCTION__ ));

               memcpy( pSes1, pSes, sizeof( CMSES ) );
               pSes1->ses.med.pAutoCall = NULL;
               pSes1->uUsr = pUsr->uHdl;
               pSes1->uHdl = uHdl1;
               /* When we perform the cmSesPhyDelete on the other user, if this was an internal call,
               ** it will also teardown this newly created session. To get around this, temporarily
               ** set the call id of the new call to invalid, then reset it after tearing down the
               ** other user's call.
               */
               pSes1->uNet = CMGR_INVALID_HANDLE;

               /* Release the call on the other user */
               pOtherUsr->evt.uDat2 = pSes->uHdl;
               pSes->bEstab = BOS_FALSE; /* This will prevent cmSesFree from removing the call from the CCTK Account */
               cmFsmCallEnd( pOtherUsr );

               /* Restore the call id */
               pSes1->uNet = uCid1;
               /* Start ringback on this user */
               pUsr->evt.uDat2 = pSes1->uHdl;
               cmFsmCallRingback( pUsr );
            }
         }
         else
         {
            CMGRLOG_DEBUG(( "%s: INTERCEPT - Invalid parameters: pUsr [%X], pOtherUsr [%X], pSes [%X].",
                            __FUNCTION__,
                            (pUsr) ? pUsr->uHdl : CMGR_INVALID_HANDLE,
                            (pOtherUsr) ? pOtherUsr->uHdl : CMGR_INVALID_HANDLE,
                            (pSes) ? pSes->uHdl : CMGR_INVALID_HANDLE ));
         }

         if( pEvt->pDat3 )
         {
            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_INTERNAL_CALL:
      {
         CMEVT evt;
         CMUSR *pUsr = NULL;
         CMSES *pSes = NULL;
         CMGRINTERNALCALL* pIntCall = pEvt->pDat3;

         if( pIntCall )
         {
            pUsr = cmUsrFindPhyHdl( pIntCall->uPhy );
            if(pUsr)
            {
               pSes = cmUsrFocusSes ( pUsr );
            }
         }

         if( pUsr )
         {
            /* Create the new session resource support if we're in the
            ** flash state.
             */
            if ( !pSes &&
                  ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_FLASH ) )
            {
               pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );
               if ( pSes )
               {
                  CMSES *pOtherSes = cmUsrFocusSes ( pUsr );

                  /* Set this new session as the focus
                  */
                  pSes->ses.med.bFocus = BOS_TRUE;

                  cmUsrLinkSes ( pUsr, pSes );

                  /* Put the user in the correct state to manage this action on
                  ** the new session.
                  */
                  cmUsrNewState ( pUsr, eCMST_DIALING );
               }
            }

            /* If a session was found, then run the event eCMEVT_INTCALL_SVC to
            ** service the request.
            */
            if ( pSes )
            {
               memset ( (void *) &evt,
                     0,
                     sizeof ( CMEVT ) );

               CMGRLOG_DEBUG ((
                   "cmUsrCmdHandler: eCMCMD_INTERNAL_CALL on uPhy(0x%X) usr(0x%X) -> uPhyDst(0x%X)",
                   pIntCall->uPhy, pUsr->uHdl, pIntCall->uPhyRemote ));

               evt.uEvt =
                  (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_INTCALL_SVC);
               evt.uDat1 = pUsr->uHdl;
               evt.uDat2 = pSes->uHdl;
               evt.pDat3 = pEvt->pDat3;;
               evt.bLast = BOS_TRUE;

               /* Run the event right away */
               cmPhysIfCmdHandler( &evt );
            }
            else
            {
               CMGRLOG_WARNING ((
               "cmUsrCmdHandler: eCMCMD_INTERNAL_CALL -  Can't find session for uPhy(0x%X) to uPhyDst(0x%X)",
                  pIntCall->uPhy, pIntCall->uPhyRemote));
            }
         }
         else
         {
            CMGRLOG_WARNING ((
               "cmUsrCmdHandler: eCMCMD_INTERNAL_CALL -  Can't find user for uPhy(0x%X) to uPhyDst(0x%X)",
                  pIntCall->uPhy, pIntCall->uPhyRemote));
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      case eCMCMD_RELEASE_USR:
      {
         CMUSR* pUsr = NULL;
         CMGRRELUSR *pRel = (CMGRRELUSR *)pEvt->pDat3;

         if( pRel )
         {
            pUsr = cmUsrFindPhyHdl( pRel->uPhy );
         }

         if( pUsr != NULL )
         {
            BOS_UINT32 uIx;
            CMGREPSIG sig;

            /* Check or locate the pending session for this user.
             */
            for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
            {
               /* Pending session must not be a remote session */
               if ( pUsr->pMedSes [ uIx ] )
               {
                  CMSES *pSes = pUsr->pMedSes [ uIx ];
                  cmSesTerminate ( pSes );
               }
            }

            /* Terminate any possible signal being played (like off-hook warning).
             */
            memset( &sig, 0, sizeof( sig ) );
            sig.eAct = eCMGREPSIGACT_STOP;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            sig.uSig = pUsr->uCpt;
            pUsr->uCpt = eEPTSIG_NULL;

            if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
            {
               cmPhySignal ( pUsr->uPhys, &sig );
            }

            pUsr->pSesWait = NULL;

            cmUsrNewState ( pUsr, eCMST_IDLE );
         }

         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;
#endif /* CMGRCFG_DECT */

      case eCMCMD_REPLACE_CALL:
      {
         CMGRREPLACECALL* pReplaceCall = (CMGRREPLACECALL*) pEvt->pDat3 ;
         CMUSR* pUsr = NULL;
         CMSES *pSes = NULL;
         CMSES *pCwSes = NULL;
         CMSESLIST sesList;
         CMGREPSIG sig;
#if CMGRCFG_MULTILINE
         CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_MULTILINE */

         BOS_UINT32 uIx;
         pUsr = cmUsrFindPhyHdl( pReplaceCall->uPhy );
         if( !pUsr )
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_REPLACE_CALL - Unable to find user phy [%d]",
                               __FUNCTION__,
                               (char*)pReplaceCall->uPhy ));
            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
            break;
         }

         pSes = cmUsrFocusSes ( pUsr );
         pCwSes = pUsr->pSesWait;

         /* Check or locate the other session.
          */
         for ( uIx = 0 ; !pCwSes && uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
         {
            if ( pUsr->pMedSes[ uIx ] &&
                  pUsr->pMedSes[ uIx ]->uNet != pReplaceCall->uCid )
            {
               pCwSes = pUsr->pMedSes [ uIx ];
            }
         }

         if( !pSes || !pCwSes )
         {
            CMGRLOG_CRITICAL(( "%s: eCMCMD_REPLACE_CALL - Couldn't find %s call [%X]",
                               __FUNCTION__,
                               (pSes ? "inactive" : "active"),
                               (pSes ? pSes->uNet : pReplaceCall->uCid) ));
            free( pEvt->pDat3 );
            pEvt->pDat3 = NULL;
            break;
         }

         CMGRLOG_INFORMATION ((
                  "%s: eCMCMD_REPLACE_CALL - Terminate session 0x%X (0x%X)",
                  __FUNCTION__, pSes->uHdl, pSes->uNet ));
         /* Replace call procedure terminates the current call and forces the
         ** call waiting call to be accepted.
         */
         cmSesTerminate( pSes );


         if ( pEvt->pDat3 != NULL )
         {
            free ( (void *) pEvt->pDat3  );
            pEvt->pDat3 = NULL;
         }
      }
      break;

      default:
      break;
   }
}


/***********************************************************************************
** FUNCTION:   cmPhysIfCmdHandler
**
** PURPOSE:    Handle an event from the physical interface (i.e. the line side).
**
** PARAMETERS: pEvt - the event pointer.
**
** RETURNS:    None.
***********************************************************************************/
void cmPhysIfCmdHandler ( CMEVT *pEvt )
{
   if ( ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) > eCMEVT_PHY_START ) &&
        ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) < eCMEVT_PHY_END ) )
   {
      CMUSR *pUsr = NULL;

      /* The first item in the event data is the user handle.
      */
      if ( (pUsr = cmUsrValid ( pEvt->uDat1 )) != NULL )
      {
         /* Push the event on top of the user block.
         */
         cmUsrNewEvent ( pUsr, (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) );

         /* Take care of any specific data coming with the event that need
         ** to be saved.
         */
         memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );

         /* Run the correct FSM.
         */
         cmFsmRun ( eCMFSM_PHY, (void *) pUsr );
      }
      else
      {
         CMGRLOG_WARNING (( "cmPhysIfCmdHandler: evt %s (%d) on invalid usr 0x%X",
                            (char *) cmMapGetStr ( cmMapCmEvent,
                                          (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) ),
                            pEvt->uEvt, pEvt->uDat1 ));
      }

      /* Free any event specific data that may have been allocated for the
      ** purpose of processing this event correctly.
      */
      switch ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) )
      {
         case eCMEVT_SES_STATUS:
         case eCMEVT_FAX_TONE:
         case eCMEVT_MODEM_TONE:
         case eCMEVT_DTMF_0:
         case eCMEVT_DTMF_1:
         case eCMEVT_DTMF_2:
         case eCMEVT_DTMF_3:
         case eCMEVT_DTMF_4:
         case eCMEVT_DTMF_5:
         case eCMEVT_DTMF_6:
         case eCMEVT_DTMF_7:
         case eCMEVT_DTMF_8:
         case eCMEVT_DTMF_9:
         case eCMEVT_DTMF_STAR:
         case eCMEVT_DTMF_HASH:
         case eCMEVT_DTMF_A:
         case eCMEVT_DTMF_B:
         case eCMEVT_DTMF_C:
         case eCMEVT_DTMF_D:
         {
            if ( pEvt->pDat3 != NULL )
            {
               free ( (void *) pEvt->pDat3  );
               pEvt->pDat3 = NULL;
            }
         }
         break;

         case eCMEVT_CONF_SVC:   /* Taken care of in the cmd-user handler. */
         case eCMEVT_BXFER_SVC:  /* Taken care of in the cmd-user handler. */
         case eCMEVT_CXFER_SVC:  /* Taken care of in the cmd-user handler. */
         case eCMEVT_DTMFPT:     /* Taken care of in the cmd-user handler. */
         case eCMEVT_INTCALL_SVC:/* Taken care of in the cmd-user handler. */
         default:
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmCctkCmdHandler
**
** PURPOSE:    Handle an event from the CCTK interface.
**
** PARAMETERS: pEvt - the event pointer.
**
** RETURNS:    None.
***********************************************************************************/
void cmCctkCmdHandler ( CMEVT *pEvt )
{
   /* Those events are call or service related events from the CCTK layer.
   */
   if ( ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) > eCMEVT_CCTK_START ) &&
        ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) < eCMEVT_CCTK_END ) )
   {
      /* The first item in the event data is the user handle.
      */
      CMUSR *pUsr = cmUsrValid ( pEvt->uDat1 );
      /* ... or it is the network user handle in some special situation.
      */
      if ( (pUsr == NULL) &&
           ((CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_USER_IN_SERVICE) )
      {
         pUsr = cmUsrFindNetHdl ( pEvt->uDat1 );
      }

      if ( pUsr != NULL )
      {
         /* Check to see if the pSes is valid if the event is a eCMEVT_CLEANED_CALL
         ** event. This check is deferred from cmCctkCb().
         */
         if ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_CLEANED_CALL )
         {
            BOS_UINT32 uCid = pEvt->uDat2;
            CMSES *pSes = NULL;

            /* Bypass processing the eCMEVT_CLEANED_CALL event if the pSes is NULL
            */
            if ( ( uCid != CMGR_INVALID_HANDLE ) &&
                 ( ( pSes = cmSesFindNetHdl ( uCid, pUsr->uHdl ) ) == NULL ) )
            {
               CMGRLOG_DEBUG
                  (( "cmCctkCmdHandler: pSes == NULL for eCMEVT_CLEANED_CALL" ));
               return;
            }
            else
            {
               /* pSes is valid. Assign the local handle of the session resource to
               ** uDat2 for FSM processing
               */
               pEvt->uDat2 = ( (pSes != NULL) ? pSes->uHdl : CMGR_INVALID_HANDLE );
            }
         }

         /* Push the event on top of the user block.
         */
         cmUsrNewEvent ( pUsr, (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) );

         /* Take care of any specific data coming with the event that need
         ** to be saved.
         */
         memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );

         /* Run the correct FSM.
         */
         cmFsmRun ( eCMFSM_CCTK, (void *) pUsr );
      }
      /* The only case where we may receive an event for an invalid user is when
      ** this is the reset complete event, in which case we need to restart the
      ** processing.
      */
      else if ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_RESET_COMP )
      {
         /* If we are still waiting for an answer to the asynchronous
         ** reset command sent to the CCTK layer, do not do anything right
         ** now, instead wait for that command to terminate before applying
         ** any changes.
         */
         if ( !gbCmResetAsynch )
         {
            if ( cmPhyTerminate () != eCMGRSTATUS_COMMAND_SUCCESS )
            {
               CMGRLOG_CRITICAL ((
                  "cmCctkCmdHandler: cmPhyTerminate FAILED!" ));
            }

            /* Module cleanup.  Order is important.
            */
            cmTmr ( BOS_FALSE );
            cmCdr ( BOS_FALSE );
            cmSes ( BOS_FALSE );
            cmUsr ( BOS_FALSE );
#if CMGRCFG_MULTILINE
            cmCctkAccnt ( BOS_FALSE );
#endif /* CMGRCFG_MULTILINE */

            /* Flush the event queue in case there were any new events that
            ** were added while cmgr/cctk was cleaning up
            */
            cmFlushEvt();

            /* Module re-creation.
            */
#if CMGRCFG_MULTILINE
            cmCctkAccnt ( BOS_TRUE );
#endif /* CMGRCFG_MULTILINE */
            cmCdr ( BOS_TRUE );
            cmSes ( BOS_TRUE );
            cmUsr ( BOS_TRUE );
            cmTmr ( BOS_TRUE );

            cmCfgInit ();
         }
      }
#if CMGRCFG_REMOTE_USER_CONTROL
      else if ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_REMOTE_USER_ADD )
      {
         /* Request to add a new remote user */

         /* Find out the local user to associate this new user with. */
         CMUSR *pLocalUsr = NULL;
         BOS_UINT32 uDftAssociatedUserId = 0;

         CFGSIPREMOTEUSERSYSCFG *pCfg = NULL;
         /* Get current default configuration.
         */
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                                    NULL,
                                    (void *) &pCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            uDftAssociatedUserId = pCfg->uDftAssociatedIMPUIndexRef;
            pLocalUsr = cmUsrFindUidHdl ( pCfg->pcDftAssociatedUserId );
            gCmCfgCb.cmgrProvCb ( CFGITEM(SIP, RemoteUserSysConcept),
                                  NULL,
                                  (void *) pCfg,
                                  CFG_ITEM_DEL );
         }
         /* If associated local user can not be found, use the first
         ** local user in CMGR as the associated local user */
         if ( pLocalUsr == NULL )
         {
            pLocalUsr = cmUsrFirstLocalUser ();
         }

         /* Grab a user block.
         */
         if ( (( pUsr = cmUsrAlloc () ) != NULL) &&
              ( pLocalUsr != NULL ) )
         {
            BOS_UINT32 strLen;
            CCTKREMUSRINFO *pRemUsrInfo = (CCTKREMUSRINFO *) pEvt->pDat3;

            /* Associate this user with a valid identifier, this is the
            ** way to tell that this user is in fact used.
            */
            strLen =
               ( ( strlen ( (const char *) pRemUsrInfo->uUsrName ) >
                                       CMGR_USER_IDENTIFIER_LEN ) ?
                    CMGR_USER_IDENTIFIER_LEN :
                        strlen ( (const char *) pRemUsrInfo->uUsrName ) );
            strncpy ( (char *) pUsr->uUid,
                      (const char *) pRemUsrInfo->uUsrName,
                      strLen );

            /* The network GUID handle is carried in the uDat2 parameter to
            ** prevent breaking the above checks against uDat1 in the case of
            ** a received event.
            */
            cmUsrSetNtwkHdl( pUsr, pEvt->uDat2 );

            /* Mark this user as a remote user, mark this user in service.
            */
            pUsr->eType = eCMUSRTYPE_REMOTE;
            pUsr->bInSrv = BOS_TRUE;

            /* Linked the new user and the base user */
            cmUsrLinkRemoteUser( pLocalUsr, pUsr->uHdl );
            cmUsrLinkLocalUser( pUsr, pLocalUsr->uHdl );

            {
               BOS_BOOL bFailed = BOS_TRUE;
               CFGSIPREMOTEUSERCFG remUsrCfg;
               memset ( &remUsrCfg, 0, sizeof ( CFGSIPREMOTEUSERCFG ) );

               CMGRLOG_INFORMATION (( "cmCctkCmdHandler: new user (%s) config",
                                      pUsr->uUid ));

               /* Save the remote user target credentials, needed to send request
               ** to this remote user.
               */
               remUsrCfg.pcUserId = pUsr->uUid;
               remUsrCfg.pcAddr = pRemUsrInfo->uUsrSigHost;
               remUsrCfg.uSipPort = pRemUsrInfo->uUsrSigPort;
               remUsrCfg.uAssociatedIMPUIndexRef = uDftAssociatedUserId;
               /* This value gets updated by application based on uAssociatedIMPUIndexRef */
               /* remUsrCfg.pcAssociatedUserId = NULL; */

               /* TO_DO: when we do auth, provide the credentials used, this
               ** may come from CCTKREMUSRINFO for dynamically created user too */
               remUsrCfg.uIMPIIndexRef = 0; /* no credential for dynamically created user */

               /* These values get updated by application based on uIMPIIndexRef */
               /*
               remUsrCfg.pcPrivateUserId = NULL;
               remUsrCfg.credentials.CredType = CFG_SIP_USER_CRED_TYPE_NONE;
               remUsrCfg.credentials.CredValue = NULL;
               */

               remUsrCfg.bDynCreated = BOS_TRUE;
               remUsrCfg.pNext = NULL;

               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, RemoteUserEntryConcept ),
                                     NULL,
                                     (void *) &remUsrCfg,
                                     CFG_ITEM_SET ) == BOS_STATUS_OK )
               {
                  CMGRLOG_INFORMATION (( "cmCctkCmdHandler: remote %s@%s:%d ",
                                         pUsr->uUid,
                                         pRemUsrInfo->uUsrSigHost,
                                         pRemUsrInfo->uUsrSigPort ));
                  bFailed = BOS_FALSE;
               }

               if ( bFailed )
               {
                  CMGRLOG_CRITICAL (( "cmCctkCmdHandler: failed new remote user (%s) config",
                                      pUsr->uUid ));

                  /* Delete the user further.
                  */
                  pUsr->bBeRem = BOS_TRUE;
                  pUsr->eCfgAction = eCMCFGACT_CFGUSRTERM;
                  cctkUserRemove ( pEvt->uDat4 );
               }
            }
         }
         else
         {
            CMGRLOG_CRITICAL (( "cmCctkCmdHandler: failed new user (%s) add",
                                pEvt->pDat3 ));
            cctkUserRemove ( pEvt->uDat2 );
         }
      }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
      else
      {
         CMGRLOG_INFORMATION (( "cmCctkCmdHandler: evt %s (%d) on invalid usr 0x%X",
                            (char *) cmMapGetStr ( cmMapCmEvent,
                                          (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) ),
                            pEvt->uEvt, pEvt->uDat1 ));
      }

      /* Free any event specific data that may have been allocated for the
      ** purpose of processing this event correctly.
      */
      switch ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) )
      {
         case eCMEVT_ALERT_CALL:
         case eCMEVT_HOLD_LOCAL_FAIL:
         case eCMEVT_MEDIA_INFO:
         case eCMEVT_MEDIA_XFER:
         case eCMEVT_MWI_INFO:
         case eCMEVT_DLG_INFO:
         case eCMEVT_FORWARD_INFO:
         case eCMEVT_USER_UPDATE:
         case eCMEVT_PAU_UPDATE:
         case eCMEVT_OOB_DTMF:
         case eCMEVT_CWT_INFO:
         case eCMEVT_CDR_INFO:
         case eCMEVT_USER_IN_SERVICE:
         case eCMEVT_USER_OUT_OF_SERVICE:
         case eCMEVT_XFERED_CALL:
         case eCMEVT_XFER_REQUEST:
         case eCMEVT_INCOMING_CALL:
         case eCMEVT_CONNECT_CALL:
         case eCMEVT_ERROR_CALL:
         case eCMEVT_TARGETS_UPDATE:
         case eCMEVT_PUBLISH_TARGET:
         case eCMEVT_SUB_STATUS:
#if CMGRCFG_REMOTE_USER_CONTROL
         case eCMEVT_REMOTE_USER_ADD:
         case eCMEVT_REMOTE_USER_REMOVE:
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
         case eCMEVT_MESSAGE_RESP:
#endif /* CMGRCFG_PASS_THRU_MSG */
         {
            if ( pEvt->pDat3 )
            {
               free ( pEvt->pDat3 );
               pEvt->pDat3 = NULL;
            }
         }
         break;

#if CMGRCFG_PASS_THRU_MSG
         case eCMEVT_MESSAGE:
         {
            if ( pEvt->pDat3 )
            {
               CCTKMSGPASSTHRU *pMsg = (CCTKMSGPASSTHRU *) pEvt->pDat3;

               if ( pMsg->puData )
               {
                  free ( pMsg->puData );
                  pMsg->puData = NULL;
               }

               free ( pEvt->pDat3 );
               pEvt->pDat3 = NULL;
            }
         }
         break;
#endif /* CMGRCFG_PASS_THRU_MSG */

         default:
         break;
      }
   }
#if CMGRCFG_MULTILINE
   /* These events are sip account related events.
   */
   else if ( ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) > eCMEVT_CCTK_ACCNT_START ) &&
        ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) < eCMEVT_CCTK_ACCNT_END ) )
   {
      /* Determine which account resource the event is for.
      */
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pEvt->uDat4 );


      if( pCctkAccnt )
      {
         /* Copy over the event information to the line so that
         ** the event handler can access it when needed.
         */
         memcpy ( &pCctkAccnt->evt, pEvt, sizeof ( CMEVT ) );

         pCctkAccnt->evt.uEvt = (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt);

         /* Process the event
         */
         cmAccntEventHandler((void *) pCctkAccnt);

         /* Free any event specific data that may have been allocated for the
         ** purpose of processing this event correctly.
         */
         switch ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) )
         {
            case eCMEVT_ACCNT_OUT_OF_SERVICE:
            case eCMEVT_ACCNT_IN_SERVICE:
            case eCMEVT_ACCNT_TARGETS_UPDATE:
            case eCMEVT_ACCNT_PUBLISH_TARGET:
            case eCMEVT_ACCNT_SUB_STATUS:
            case eCMEVT_ACCNT_UPDATE:
            case eCMEVT_ACCNT_PAU_UPDATE:
            case eCMEVT_ACCNT_CDR_INFO:
            {
               if ( pEvt->pDat3 )
               {
                  free ( pEvt->pDat3 );
                  pEvt->pDat3 = NULL;
               }
            }
            break;

            default:
            break;
         }
      }
   }
#endif /* CMGRCFG_MULTILINE */
   /* Those events are processing related events such as status of an action
   ** invoked, etc...
   */
   else if ( ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) > eCMEVT_CCTK_PROC_START ) &&
             ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) < eCMEVT_CCTK_PROC_END ) )
   {
      switch ( CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt )
      {
         case eCMEVT_CCTK_CFG_OK:
         {
            /* The initial step of the CCTK configuration was successful (including
            ** setup of underlying resources).  Now continue with configuring the
            ** actual users and finish 'boot up' process.
            */
            if ( cmCfgUser () )
            {
#if CMGRCFG_REMOTE_USER_CONTROL
               /* Update remote user setting after CCTK configuration is completed */
               cmCfgRemUsrSys ( );
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
               cmUsrPhyCreate ( CMGR_INVALID_HANDLE );

               cmUpdateState ( CMGR_INVALID_HANDLE,
                               eCMGRSTATUS_STARTUP_SUCCESS );
            }
            else
            {
               cmUpdateState ( CMGR_INVALID_HANDLE,
                               eCMGRSTATUS_STARTUP_FAILED );
            }
         }
         break;

         case eCMEVT_CCTK_CFG_FAIL:
         {
            /* CCTK configuration and startup has failed.  There is nothing we
            ** can do at this point as CCTK will not be functional.  Return an
            ** error to the application.
            */
            cmUpdateState ( CMGR_INVALID_HANDLE,
                            eCMGRSTATUS_STARTUP_FAILED );
         }
         break;

         case eCMEVT_CCTK_CALADD_FAIL:
         case eCMEVT_CCTK_CALMAK_FAIL:
         {
            CMUSR *pUsr = NULL;

            /* Failure to create or place a call.  Terminate local user call as if
            ** the call was ended by the network.
            */
            if ( (pUsr = cmUsrValid ( pEvt->uDat1 )) != NULL )
            {
               cmUsrNewEvent ( pUsr, eCMEVT_CLEANED_CALL );
               memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );
               cmFsmRun ( eCMFSM_CCTK, (void *) pUsr );
            }
         }
         break;

         case eCMEVT_CCTK_EVTADD_FAIL:
         case eCMEVT_CCTK_EVTSUB_FAIL:
         {
            CMUSR *pUsr = NULL;

            /* Failure to create or subscribe to a service.  Terminate local user
            ** service as if the service was ended by the network.
            */
            if ( (pUsr = cmUsrValid ( pEvt->uDat1 )) != NULL )
            {
               cmUsrNewEvent ( pUsr, eCMEVT_SERVICE_END );
               memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );
               cmFsmRun ( eCMFSM_CCTK, (void *) pUsr );
            }
         }
         break;

         case eCMEVT_CCTK_USRADD_OK:
         {
#if !CMGRCFG_MULTILINE
            CMUSR *pUsr = NULL;

            if ( (pUsr = cmUsrValid ( pEvt->uDat1 )) != NULL )
            {
               /* Check whether we need to create physical interface support
               ** for this user that was successfully added.  It is possible
               ** that we still need to do this step in case the original
               ** configuration was not complete at the time we query it or
               ** when a new user is added to the already running system.
               */
               cmUsrPhyCreate ( pUsr->uHdl );

               /* If our configuration profile does not allow for service
               ** state reporting based on the registration status of the
               ** user, we force an 'in-service' event on successful user
               ** addition in order to offer service for that user.
               */
#if  ( (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE) ||    \
       (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_CUSTOM) )
               cmUsrNewEvent ( pUsr, eCMEVT_USER_IN_SERVICE );
               memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );
               pUsr->evt.uDat2 = pUsr->uNet;
               cmFsmRun ( eCMFSM_CCTK, (void *) pUsr );
#else
               /* If our configuration profile allows for service state
               ** reporting, or requires to have the user register in order
               ** for it to be activated, then register now.
               */
               cmUsrRegister ( pUsr->uHdl );
#endif
            }
#else /* !CMGRCFG_MULTILINE */
            /* If multiline, we need to register the CCTK user separate from the CM user */
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pEvt->uDat4 );

            /* Only register if this is the last event */
            if( pCctkAccnt && pEvt->bLast )
            {
               cmCctkAccntRegister ( pCctkAccnt );
            }
            else
            {
               if( !pCctkAccnt )
               {
                  CMGRLOG_CRITICAL (( "cmCctkCmdHandler: failed to retrieve CMCCTKACCNT object for uNet [%X]",
                    pEvt->uDat4 ));
               }
            }
#endif /* CMGRCFG_MULTILINE */
         }
         break;

         case eCMEVT_CCTK_USRADD_FAIL:
         {
#if !CMGRCFG_MULTILINE
            CMUSR *pUsr = cmUsrValid ( pEvt->uDat1 );
            BOS_BOOL bRetry = BOS_FALSE;

            if ( pEvt->pDat3 &&
                 /* This may happen when we are trying to add a user that would
                 ** be making use of a connection (persistent connection in this
                 ** case) which is in the process of being terminated, that happens
                 ** if the same connection was used by a different user which is
                 ** being removed and that operation did not complete.  In such
                 ** case, we need to wait for the operation to finish off in order
                 ** to be able to add this user, so we delay the addition to a later
                 ** time.
                 */
                 ( *((CCTKSTATUS *)pEvt->pDat3) == eCCTKSTATUS_CNXNOTREADY ) )
            {
               /* Simply mark the user as needing to be added, the framework will
               ** take care of the rest.
               */
               if ( pUsr )
               {
                  pUsr->eCfgAction = eCMCFGACT_CFGUSRREADD;
                  bRetry = BOS_TRUE;
               }
            }

            if ( pEvt->pDat3 )
            {
               free ( pEvt->pDat3 );
               pEvt->pDat3 = NULL;
            }

            if ( !bRetry && pUsr )
            {
               cmUsrNewEvent ( pUsr, eCMEVT_USER_OUT_OF_SERVICE );
               memcpy ( &pUsr->evt, pEvt, sizeof ( CMEVT ) );
               cmFsmRun ( eCMFSM_CCTK, (void *) pUsr );
            }
#else /* !CMGRCFG_MULTILINE */
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pEvt->uDat4 );
            if ( pCctkAccnt )
            {
               /* Copy over the event information to the line so that
               ** the event handler can access it when needed.
               */
               memcpy ( &pCctkAccnt->evt, pEvt, sizeof ( CMEVT ) );

               pCctkAccnt->evt.uEvt = eCMEVT_ACCNT_OUT_OF_SERVICE;

               /* Process the event immediately
               */
               cmAccntEventHandler((void *) pCctkAccnt);
            }

            if ( pEvt->pDat3 )
            {
               free ( pEvt->pDat3 );
               pEvt->pDat3 = NULL;
            }
#endif /* CMGRCFG_MULTILINE */
         }
         break;

         case eCMEVT_CCTK_RST_OK:
         {
            gbCmResetAsynch = BOS_FALSE;

            /* If we have a shutdown pending, continue with the shutdown
            ** process.
            */
            if ( gbCmShutdownPending )
            {
               /* Delay added to avoid cctkShutdown stuck issue due to
               ** CAsynResolver */
               bosSleep(2000);
               gbCmShutdown = BOS_TRUE;
               gbCmShutdownPending = BOS_FALSE;
            }
            /* Apply the remainder of the reset.
            */
            else
            {
               /* CCTK reset was successful.  Restart configuration.
               */
               CMGRLOG_INFORMATION (( "cmCctkCmdHandler: Restarting CMGR/CCTK" ));

               /* Reset the Call Manager internal data.
               */
               if ( cmPhyTerminate () != eCMGRSTATUS_COMMAND_SUCCESS )
               {
                  CMGRLOG_CRITICAL ((
                     "cmCctkCmdHandler: cmPhyTerminate FAILED!" ));
               }

               /* Module cleanup.  Order is important.
               */
               cmTmr ( BOS_FALSE );
               cmCdr ( BOS_FALSE );
               cmSes ( BOS_FALSE );
               cmUsr ( BOS_FALSE );
#if CMGRCFG_MULTILINE
               cmCctkAccnt ( BOS_FALSE );
#endif /* CMGRCFG_MULTILINE */

               /* Flush the event queue in case there were any new events that
               ** were added while cmgr/cctk was cleaning up
               */
               cmFlushEvt();


               /* Module re-creation.
               */
#if CMGRCFG_MULTILINE
               cmCctkAccnt ( BOS_TRUE );
#endif /* CMGRCFG_MULTILINE */
               cmCdr ( BOS_TRUE );
               cmSes ( BOS_TRUE );
               cmUsr ( BOS_TRUE );
               cmTmr ( BOS_TRUE );

               if ( cmCfgInit () != BOS_TRUE )
               {
                  CMGRLOG_CRITICAL ((
                     "cmCctkCmdHandler: restart cmCfgInit FAILED!" ));
                  cmUpdateState ( CMGR_INVALID_HANDLE,
                                  eCMGRSTATUS_CRITICAL_ERROR );
               }
            }
         }
         break;

         case eCMEVT_CCTK_RST_FAIL:
         {
            gbCmResetAsynch = BOS_FALSE;

            /* Failed to reset CCTK, this is essentially a critical problem since
            ** we will not be able to do anything (in particular not be able to
            ** apply any new critical configuration which motivated such reset).
            **
            ** Should probably 'reboot'.  Issue a critical error such that the
            ** application can choose to take action.
            */
            CMGRLOG_CRITICAL (( "cmCctkCmdHandler: RESET FAILED!" ));
            cmUpdateState ( CMGR_INVALID_HANDLE,
                            eCMGRSTATUS_CRITICAL_ERROR );

            /* If we have a shutdown pending, apply it now.
            */
            if ( gbCmShutdownPending )
            {
               /* Delay added to avoid cctkShutdown stuck issue due to
               ** CAsynResolver */
               bosSleep(2000);
               gbCmShutdown = BOS_TRUE;
               gbCmShutdownPending = BOS_FALSE;
            }
         }
         break;

         case eCMEVT_CCTK_USRDEL_OK:
         case eCMEVT_CCTK_USRDEL_FAIL:
         {
#if !CMGRCFG_MULTILINE
            CMUSR *pUsr = NULL;

            if ( (pUsr = cmUsrValid ( pEvt->uDat1 )) != NULL )
            {
               pUsr->bBeRem = BOS_FALSE;

               /* If there was an outstanding configuration action, this is where
               ** we would want to process it.
               */
               switch ( pUsr->eCfgAction )
               {
                  case eCMCFGACT_CFGUSRWITHNET:
                  {
                     /* Reconfigure the user entirely.
                     */
                     pUsr->eCfgAction = eCMCFGACT_NOACTION;

                     cmCfgUserAdd ( (void *) pUsr );

                     CMGRLOG_INFORMATION ((
                          "cmCctkCmdHandler: user %s (0x%x) is reconfigured",
                          pUsr->uUid, pUsr->uHdl ));
                  }
                  break;

                  case eCMCFGACT_CFGUSRTERM:
                  {
                     /* User has been terminated.  Free the user block such that
                     ** it could be re-used later on.
                     */
                     CMGRLOG_CRITICAL ((
                          "cmCctkCmdHandler: user %s (0x%x) is terminated",
                          pUsr->uUid, pUsr->uHdl ));

                     memset ( (void *) pUsr->uUid,
                              0,
                              CMGR_USER_IDENTIFIER_LEN + 1 );
                     pUsr->uUindex = CMGR_INVALID_HANDLE;
                     cmUsrSetNtwkHdl( pUsr, CMGR_INVALID_HANDLE );
                     pUsr->uPhys = CMGR_INVALID_HANDLE;
                     pUsr->eType = eCMUSRTYPE_NONE;
                     pUsr->eCfgAction = eCMCFGACT_NOACTION;
                     pUsr->bBeRem = BOS_FALSE;
                     pUsr->bRegNoDelay = BOS_FALSE;
                     pUsr->bInUse = BOS_FALSE;
                  }
                  break;

                  default:
                  break;
               }
            }
#else
            CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pEvt->uDat4 );
            if ( pCctkAccnt )
            {
               pCctkAccnt->bBeRem = BOS_FALSE;

               /* If there was an outstanding configuration action, this is where
               ** we would want to process it.
               */
               switch (  pCctkAccnt->eCfgAction )
               {
                  case eCMCFGACT_CFGUSRWITHNET:
                  {
                     /* Reconfigure the account entirely.
                     */
                     pCctkAccnt->eCfgAction = eCMCFGACT_NOACTION;

                     /* TODO! - Free this account and re-create it from scratch
                     ** The reconfig action could result from a characteristic
                     ** change to the account such as the uuid, if this is the
                     ** case we should free this account and allocate a new one
                     ** to handle the cctk resource created.
                     */

                     /* Clean up all prior associations
                     */
                     cmCctkAccntDeinit( pCctkAccnt );

                     cmCctkAccntAdd( pCctkAccnt );

                     CMGRLOG_INFORMATION ((
                          "cmCctkCmdHandler: account %s (0x%x) is reconfigured",
                          pCctkAccnt->uUid, pCctkAccnt->uHdl ));
                  }
                  break;

                  case eCMCFGACT_CFGUSRTERM:
                  {
                     /* Account has been terminated.  Free the account block such that
                     ** it could be re-used later on.
                     */
                     CMGRLOG_CRITICAL ((
                          "cmCctkCmdHandler: account %s (0x%x) is terminated",
                          pCctkAccnt->uUid, pCctkAccnt->uHdl ));

                     cmCctkAccntFree( pCctkAccnt );

                     /* TODO - If later cmgr users are assigned a default account,
                     ** the user will also need to clear it's knowledge of this
                     ** deleted account.
                     */
                  }
                  break;

                  default:
                  break;
               }
            }

#endif /* !CMGRCFG_MULTILINE */
         }
         break;

         /* This may happen if the CCTK layer did terminate the network leg
         ** of the call already but we have not processed the event for it yet.
         ** This would be a valid race condition, so we can safely ignore it.
         */
         case eCMEVT_CCTK_CALDEL_FAIL:
         break;

         /* Non handled status events at this time.  Some of those do carry
         ** information of importance that we should feed back to the application
         ** through the appropriate mechanism.  In particular those related to
         ** failure events.
         */
         case eCMEVT_CCTK_SETPARM_OK:
         case eCMEVT_CCTK_SETPARM_FAIL:
         case eCMEVT_CCTK_USRREG_OK:
         case eCMEVT_CCTK_USRREG_FAIL:
         case eCMEVT_CCTK_USRDEREG_OK:
         case eCMEVT_CCTK_USRDEREG_FAIL:
         case eCMEVT_CCTK_EVTADD_OK:
         case eCMEVT_CCTK_EVTSUB_OK:
         case eCMEVT_CCTK_EVTDESUB_OK:
         case eCMEVT_CCTK_EVTDESUB_FAIL:
         case eCMEVT_CCTK_EVTDEL_OK:
         case eCMEVT_CCTK_EVTDEL_FAIL:
         case eCMEVT_CCTK_CALADD_OK:
         case eCMEVT_CCTK_CALMAK_OK:
         case eCMEVT_CCTK_CALANS_OK:
         case eCMEVT_CCTK_CALANS_FAIL:
         case eCMEVT_CCTK_CALFWD_OK:
         case eCMEVT_CCTK_CALFWD_FAIL:
         case eCMEVT_CCTK_CALHLD_OK:
         case eCMEVT_CCTK_CALHLD_FAIL:
         case eCMEVT_CCTK_CALUNHLD_OK:
         case eCMEVT_CCTK_CALUNHLD_FAIL:
         case eCMEVT_CCTK_CALXFR_OK:
         case eCMEVT_CCTK_CALXFR_FAIL:
         case eCMEVT_CCTK_CALXFRB_OK:
         case eCMEVT_CCTK_CALXFRB_FAIL:
         case eCMEVT_CCTK_CALMED_OK:
         case eCMEVT_CCTK_CALMED_FAIL:
         case eCMEVT_CCTK_CALDEL_OK:
         case eCMEVT_CCTK_CALDTPT_OK:
         case eCMEVT_CCTK_CALDTPT_FAIL:
         case eCMEVT_CCTK_CALHF_OK:
         case eCMEVT_CCTK_CALHF_FAIL:
         case eCMEVT_CCTK_CALPRIV_OK:
         case eCMEVT_CCTK_CALPRIV_FAIL:
         case eCMEVT_CCTK_PUB_OK:
         case eCMEVT_CCTK_PUB_FAIL:
         case eCMEVT_CCTK_SVCCTL_OK:
         case eCMEVT_CCTK_SVCCTL_FAIL:
#if CMGRCFG_REMOTE_USER_CONTROL
         case eCMEVT_CCTK_PROXY_OK:
         case eCMEVT_CCTK_PROXY_FAIL:
#endif /* CMGRCFG_REMOTE_USER_CONTROL */
#if CMGRCFG_PASS_THRU_MSG
         case eCMEVT_CCTK_MSGPT_OK:
         case eCMEVT_CCTK_MSGPT_FAIL:
         case eCMEVT_CCTK_MSGPT_RSP_OK:
         case eCMEVT_CCTK_MSGPT_RSP_FAIL:
#endif /* CMGRCFG_PASS_THRU_MSG */
         default:
         break;
      }
   }
}


/***********************************************************************************
**  FUNCTION:   cmCctkCb
**
**  PURPOSE:    Callback from CCTK upon event notification.
**
**  PARAMETERS: eEvent - the event reported.
**              uUid   - user identifier.
**              uCid   - call or service reference identifier - may not be set.
**              pData  - event data, event dependent - may be NULL.
**
**  RETURNS:    Nothing.
**
**  NOTES:      The event type alone will dictate what of the rest of the
**              parameters are valid when passed during the callback.
***********************************************************************************/
void cmCctkCb ( CCTKEVT eEvent, BOS_UINT32 uUid, BOS_UINT32 uCid, void *pData )
{
   CMEVT evt;
   BOS_BOOL bValid = BOS_TRUE;
   CMFSMEVT eFsmEvt;
   CMUSR *pUsr = NULL;
   CMSES *pSes = NULL;
   CMCCTKACCNT *pCctkAccnt = NULL;

#if CMGRCFG_FIREWALL_CONTROL
   CMGRFWCMD cmgrFwCmd;
   CMGRFWINFO cmgrFwInfo;
   BOS_UINT32 uUsr;
   CMGRMEDINFO media;
#endif

   memset ( &evt, 0, sizeof(CMEVT) );
   eFsmEvt = cmMapGetId ( cmMapCctkEvt2CmEvt, eEvent );

   CMGRLOG_DEBUG (( "cmCctkCb: evt %s (%d), uid 0x%X, cid 0x%X, data 0x%X",
                    (char *) cmMapGetStr ( cmMapCmEvent, eFsmEvt ),
                    eEvent, uUid, uCid, pData ));

   /* What needs to happen here...
   **
   **    [1] Map the CCTK event to the corresponding CMGR event.
   **    [2] Ensure that the mapped event is for a valid identifier.
   **    [3] If necessary, copy some of the data passed with the event that may
   **        be of interest during process of such event.
   **
   ** After the three above steps have been taken successfully, the event will be
   ** posted for processing.
   */

   /* Verify the validity of the event. */
   if ( eFsmEvt == CMGR_INVALID_HANDLE )
   {
      bValid = BOS_FALSE;
   }

   /* Verify the user of this event is valid. */
   /* Find the user based on established session with this cid.
   */
   if ( cmNumCidAssigned( uCid ) == 1 )
   {
      pUsr = cmUsrFindCid( uCid, BOS_TRUE );
   }

#if !CMGRCFG_MULTILINE
   /* Find user using the CCTK network handle since no users were found using the uCid */
   if ( ( uUid != CMGR_INVALID_HANDLE ) &&
        ( pUsr == NULL ) &&
        ( ( pUsr = cmUsrFindNetHdl ( uUid ) ) == NULL ) )
   {
      /* Last attempt to find the user based on the call id (regardless of whether
      ** the session is established or not).
      */
      if ( ( pUsr = cmUsrFindCid( uCid, BOS_FALSE ) ) == NULL )
      {
         bValid = BOS_FALSE;
      }
   }
#endif /* !CMGRCFG_MULTILINE */

   /* In case of an 'in-service' event, we may not have the valid user
   ** identifier just yet because the user handle may have been swapped
   ** and the 'update' event has not been processed, in such case bypass
   ** the check.  Further processing will take place in the state machine
   ** to validate the event.
   */
   if ( ( pUsr == NULL  ) &&
        ( eFsmEvt == eCMEVT_USER_IN_SERVICE ) )
   {
      bValid = BOS_TRUE;
   }

   /* Verify the session associated with this event is valid (if any). */
   if ( ( uCid != CMGR_INVALID_HANDLE ) &&
        ( ( pSes = cmSesFindNetHdl ( uCid, ( pUsr != NULL ) ? pUsr->uHdl : CMGR_INVALID_HANDLE ) ) == NULL ) )
   {
      bValid = BOS_FALSE;
   }

   /* In case this is for a new incoming or transferred call, we will not have a
   ** valid session just yet, so bypass the above check.
   */
   if ( ( pSes == NULL ) &&
        ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
          ( eFsmEvt == eCMEVT_ALERT_CALL ) ||
          ( eFsmEvt == eCMEVT_MEDIA_INFO ) ||
          ( eFsmEvt == eCMEVT_XFER_REQUEST ) ||
          ( eFsmEvt == eCMEVT_CLEANED_CALL )
#if CMGRCFG_MULTILINE
          || ( eFsmEvt == eCMEVT_RELEASE_CALL )
#endif /* CMGRCFG_MULTILINE */
        ) )
   {
      bValid = BOS_TRUE;
   }

#if CMGRCFG_MULTILINE
   /* CCTK processing and account related events don't necessarily have a
   ** valid CM user or session, but should be processed nonetheless.
   */
   if( ( eFsmEvt > eCMEVT_CCTK_ACCNT_START && eFsmEvt < eCMEVT_CCTK_ACCNT_END ) ||
       ( eFsmEvt > eCMEVT_CCTK_PROC_START && eFsmEvt < eCMEVT_CCTK_PROC_END ) )
   {
      bValid = BOS_TRUE;
   }
#endif /* CMGRCFG_MULTILINE */

#if CMGRCFG_PASS_THRU_MSG
   /* Very special case for message pass through related events,
   ** we do not allocate a session at the CMGR layer for those
   ** messages because such is tracked outside of the CMGR and within
   ** CCTK only.
   */
   if ( ( pSes == NULL ) &&
        ( ( eFsmEvt == eCMEVT_MESSAGE ) ||
          ( eFsmEvt == eCMEVT_MWI_INFO ) ||
          ( eFsmEvt == eCMEVT_MESSAGE_RESP ) ||
          ( eFsmEvt == eCMEVT_CCTK_MSGPT_OK ) ||
          ( eFsmEvt == eCMEVT_CCTK_MSGPT_FAIL ) ||
          ( eFsmEvt == eCMEVT_CCTK_MSGPT_RSP_OK ) ||
          ( eFsmEvt == eCMEVT_CCTK_MSGPT_RSP_FAIL ) ) )
   {
      bValid = BOS_TRUE;
   }
#endif /* CMGRCFG_PASS_THRU_MSG */

#if CMGRCFG_REMOTE_USER_CONTROL
   if ( eFsmEvt == eCMEVT_REMOTE_USER_REMOVE || ( eFsmEvt == eCMEVT_REMOTE_USER_ADD ) )
   {
      bValid = BOS_TRUE;
   }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

   /* All is well, post the event.
   */
   if ( bValid )
   {
#if CMGRCFG_MULTILINE
      CMUSRLIST cmUsrList;
      BOS_UINT32 uIx;
      BOS_UINT32 uActiveCalls = cmCctkNumActiveCalls( uUid );

      memset( &cmUsrList, 0, sizeof( CMUSRLIST ) );

      /* If this is an incoming call and the CCTK user from
      ** which the call is coming has all available active
      ** VoIP slots used up, reject the call immediately.
      */
      if( ( eFsmEvt == eCMEVT_INCOMING_CALL ) &&
          ( uActiveCalls >= CMGRCFG_MAX_CALLS_PER_USER ) )
      {
         CMGRLOG_INFORMATION (( "%s: Incoming call rejected. Net [%X] already has [%X/%X] active calls.",
                                __FUNCTION__,
                                uUid,
                                uActiveCalls,
                                CMGRCFG_MAX_CALLS_PER_USER ));
         cctkCallDelete ( uCid, eCCTKCALLDELRSN_BUSY );
         return;
      }

      /* If the event is an incoming call, transfer request, MWI
      ** or call release request, we may have to propagate it
      ** to multiple CMUSRs.
      */
      if ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
           ( eFsmEvt == eCMEVT_ALERT_CALL ) ||
           ( eFsmEvt == eCMEVT_FORWARD_INFO ) ||
           ( eFsmEvt == eCMEVT_XFER_REQUEST ) ||
           ( eFsmEvt == eCMEVT_MWI_INFO ) ||
           ( eFsmEvt == eCMEVT_RELEASE_CALL ) ||
           ( eFsmEvt == eCMEVT_CDR_INFO )     ||
           ( eFsmEvt == eCMEVT_CLEANED_CALL ) )
      {
#if !(CMGRCFG_MULTILINE_MODE_ACTIVE_ACCNT_MULTIPLE_USR)
         /* If we are in "Single Line" mode and this is not the first call for
         ** this CCTK user, make sure to only send the event to the CM user that
         ** has the other ongoing session(s) assigned to this CCTK user.
         */
         if( eFsmEvt == eCMEVT_INCOMING_CALL && cmNumSesWithNet( uUid ) > 0 )
         {
            CMSES* pOtherSes = NULL;
            CMSESLIST sesList;
            BOS_UINT32 uIy;

            memset( &sesList, 0, sizeof( CMSESLIST ) );

            /* In single-line mode, the only time this list includes more than
            ** one user is when multiple users are ringing; otherwise, this list
            ** will always hold only one user. In either case we should be able
            ** to use just the first user from the list.
            */
            cmSesListWithNet( uUid, &sesList );
            for( uIy = 0 ; uIy < sesList.uLength ; uIy++ )
            {
               pOtherSes = cmSesValid( sesList.uSes[ uIy ] );

               if( pOtherSes )
               {
                  memset( &cmUsrList, 0, sizeof( CMUSRLIST ) );

                  cmUsrList.uUsr[ 0 ] = pOtherSes->uUsr;
                  cmUsrList.uLength++;
                  break;
               }
            }
         }
         else
#endif /* !CMGRCFG_MULTILINE_MODE_ACTIVE_ACCNT_MULTIPLE_USR */
         {
            cmGetAssocUsrList( uUid, &cmUsrList );

            /* If this event is cannot be routed to any CMUSR, reject the call.
            */
            if( cmUsrList.uLength == 0 )
            {
               CMGRLOG_INFORMATION (( "%s: Incoming call rejected. Net [%X] has no associated CMUSRs",
                                      __FUNCTION__,
                                      uUid ));
               cctkCallDelete ( uCid, eCCTKCALLDELRSN_UNAVAILABLE );
               return;
            }
         }
      }
      /* The eCMEVT_CCTK_USRADD_OK event is a special case
      ** in which we want to send only one event, but not
      ** to any particular CMUSR. The cctkCmdHdlr will hanldle
      ** this case separately for multiline and start CCTK user
      ** registration.
      */
      else if ( eFsmEvt == eCMEVT_CCTK_USRADD_OK )
      {
         cmUsrList.uUsr[ 0 ] = CMGR_INVALID_HANDLE;
         cmUsrList.uLength++;
      }
      /* If there is no assigned session yet but we have a
      ** CCTK processing event, we should pass it along to
      ** all possible users that can use it.
      */
      else if ( ( pSes == NULL ) &&
                ( ( eFsmEvt > eCMEVT_CCTK_PROC_START ) &&
                  ( eFsmEvt < eCMEVT_CCTK_PROC_END ) ) &&
                  ( eFsmEvt != eCMEVT_CCTK_USRADD_OK ) )
      {
         cmGetAssocUsrList( uUid, &cmUsrList );
      }

      /* If this event is to be processed for a single user only,
      ** set that single user.
      */
      if( cmUsrList.uLength == 0 )
      {
         cmUsrList.uUsr[ 0 ] = ( pUsr != NULL ) ? pUsr->uHdl : CMGR_INVALID_HANDLE;
         cmUsrList.uLength++;
      }

      /* Process the list of users for this event
      ** that we have compile so far.
      */
      for( uIx = 0 ; uIx < cmUsrList.uLength ; uIx++ )
      {
         /* Get the user from the list */
         pUsr = cmUsrValid( cmUsrList.uUsr[ uIx ] );

         if ( pUsr )
         {
            /* Only update the account information if an account is not already
            ** assigned.
            **
            ** Note that account update should only apply to a few selected
            ** events.
            */
            if ( pUsr->uUindex == CMGR_INVALID_HANDLE )
            {
               if ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
                    ( eFsmEvt == eCMEVT_XFER_REQUEST ) ||
                    ( eFsmEvt == eCMEVT_ALERT_CALL ) ||
                    ( eFsmEvt == eCMEVT_FORWARD_INFO ) ||
                    ( eFsmEvt == eCMEVT_MWI_INFO ) )
               {
                  pCctkAccnt = cmCctkAccntFindByNet( uUid );
                  if ( pCctkAccnt )
                  {
                     cmUsrUpdateCctkAccnt( pUsr, pCctkAccnt->uUid, pCctkAccnt->uHdl );
                  }
               }
            }
         }

         /* Get the session if we aren't sending this event to multiple users.
         ** If we are sending to multiple users and no session can yet exist,
         ** we need to pass the Call ID instead of a session handle.
         */
         pSes = ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
                  ( eFsmEvt == eCMEVT_XFER_REQUEST )  ||
                  ( eFsmEvt == eCMEVT_ALERT_CALL ) ) ? NULL : cmSesFindNetHdl ( uCid, ( pUsr != NULL ) ? pUsr->uHdl : CMGR_INVALID_HANDLE );

         CMGRLOG_INFORMATION (( "%s: (%s) :: Routed usr [%X], ses [%X], cid [%X], net [%X], uIx [%d/%d]",
                                __FUNCTION__,
                                (char *) cmMapGetStr ( cmMapCmEvent, eFsmEvt ),
                                ( pUsr ) ? pUsr->uHdl : CMGR_INVALID_HANDLE,
                                ( pSes ) ? pSes->uHdl : CMGR_INVALID_HANDLE,
                                uCid,
                                uUid,
                                (uIx + 1),
                                cmUsrList.uLength ));

         /* If we dont have a session associated with the Call Id ( i.e session = NULL )
         ** only the following listed events will be processed
         */
         if( ( pSes == NULL ) &&
             ( ( eFsmEvt != eCMEVT_INCOMING_CALL )       &&
               ( eFsmEvt != eCMEVT_XFER_REQUEST )        &&
               ( eFsmEvt != eCMEVT_ALERT_CALL )          &&
               ( eFsmEvt != eCMEVT_CCTK_CFG_OK )         &&
               ( eFsmEvt != eCMEVT_CCTK_CFG_FAIL )       &&
               ( eFsmEvt != eCMEVT_CCTK_EVTADD_FAIL )    &&
               ( eFsmEvt != eCMEVT_CCTK_EVTSUB_FAIL )    &&
               ( eFsmEvt != eCMEVT_CCTK_USRADD_OK )      &&
               ( eFsmEvt != eCMEVT_CCTK_USRADD_FAIL )    &&
               ( eFsmEvt != eCMEVT_CCTK_RST_OK )         &&
               ( eFsmEvt != eCMEVT_CCTK_RST_FAIL )       &&
               ( eFsmEvt != eCMEVT_FORWARD_INFO )         &&
               ( eFsmEvt != eCMEVT_ACCNT_IN_SERVICE )     &&
               ( eFsmEvt != eCMEVT_ACCNT_OUT_OF_SERVICE ) &&
               ( eFsmEvt != eCMEVT_ACCNT_UPDATE )         &&
               ( eFsmEvt != eCMEVT_ACCNT_PAU_UPDATE )     &&
               ( eFsmEvt != eCMEVT_ACCNT_TARGETS_UPDATE ) &&
               ( eFsmEvt != eCMEVT_ACCNT_SUB_STATUS )     &&
               ( eFsmEvt != eCMEVT_ACCNT_PUBLISH_TARGET ) &&
               ( eFsmEvt != eCMEVT_ACCNT_CDR_INFO )       &&
               ( eFsmEvt != eCMEVT_CCTK_USRDEL_OK )      &&
               ( eFsmEvt != eCMEVT_CLEANED_CALL  )       &&
               ( eFsmEvt != eCMEVT_MWI_INFO  )            &&
               ( eFsmEvt != eCMEVT_CCTK_USRDEL_FAIL ) ) )
         {
            CMGRLOG_INFORMATION (( "%s: Skipping for usr [%X]", __FUNCTION__,
                                   ( pUsr ) ? pUsr->uHdl : CMGR_INVALID_HANDLE ));
            continue;
         }
#else
      {
#endif /* CMGRCFG_MULTILINE */

         /* Fill in event block parameters
         */
         evt.uEvt = ( eFsmEvt | CMGR_EVT_CLASS__CCTK );

         /* uDat1 should contain the user handle if valid
         ** If usr handle is not valid and the event is an
         ** IN_SERVICE event, pass the CCTK network handle instead
         */
         evt.uDat1 = ( (pUsr != NULL) ? pUsr->uHdl :
            ( ( eFsmEvt == eCMEVT_USER_IN_SERVICE ) ? uUid : CMGR_INVALID_HANDLE ));

         /* uDat2 should contain the session handle if valid
         ** If the session handle is not valid then then for
         ** a few select events pass the callid instead
         */
         evt.uDat2 = ( (pSes != NULL) ? pSes->uHdl :
            ( ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
                ( eFsmEvt == eCMEVT_XFER_REQUEST )  ||
                ( eFsmEvt == eCMEVT_CLEANED_CALL )  ||
#if CMGRCFG_MULTILINE
                ( eFsmEvt == eCMEVT_RELEASE_CALL )  ||
#endif /* CMGRCFG_MULTILINE */
                ( eFsmEvt == eCMEVT_ALERT_CALL ) ) ? uCid : CMGR_INVALID_HANDLE ));

         /* Override if eCMEVT_CLEANED_CALL event. This override is needed because we
         ** are delaying the pSes validity check to cmCctkCmdHandler(). Due to resource
         ** sharing between CCTK and CMGR, pSes may not be in the correct or most up to
         ** date state in this call back. As a result, the validity check is delayed to
         ** cmCctkCmdHandler(), which is where the event will be processed. This issue
         ** is currently only evident in the eCMEVT_CLEANED_CALL event. As a note for
         ** future consideration, the validity checks and pUsr/pSes processing should
         ** be deferred to cmCctkCmdHandler().
         */
         if ( eFsmEvt == eCMEVT_CLEANED_CALL )
         {
            evt.uDat2 = uCid;
         }

#if CMGRCFG_MULTILINE
         /* If this is the last user on which to process the event and
         ** the bLast flag has not already been set, then set it.
         */
         evt.bLast = ( ( uIx + 1 ) == cmUsrList.uLength ) ? BOS_TRUE : BOS_FALSE;
#else
      /* In the non multiline case there is no reason to halt processing until the
         ** event has been posted to all the users.
         */
         evt.bLast = BOS_TRUE;
#endif /* CMGRCFG_MULTILINE */

         /* uDat4 should contain the CCTK Network handle
         */
         evt.uDat4 = uUid;

#if CMGRCFG_PASS_THRU_MSG
         /* Assign the uDat2 for this event to be the handle for the CCTK resource
         ** that we do not track at the CMGR level.
         */
         if ( ( eFsmEvt == eCMEVT_MESSAGE ) ||
              ( eFsmEvt == eCMEVT_MESSAGE_RESP ) ||
              ( eFsmEvt == eCMEVT_CCTK_MSGPT_OK ) ||
              ( eFsmEvt == eCMEVT_CCTK_MSGPT_FAIL ) ||
              ( eFsmEvt == eCMEVT_CCTK_MSGPT_RSP_OK ) ||
              ( eFsmEvt == eCMEVT_CCTK_MSGPT_RSP_FAIL ) )
         {
           evt.uDat2 = uCid;
         }
#endif /* CMGRCFG_PASS_THRU_MSG */

         /* Extra data may be associated with this event depending on the event
         ** type.
         */
         switch ( eFsmEvt )
         {
            case eCMEVT_CONNECT_CALL:
            case eCMEVT_INCOMING_CALL:
            case eCMEVT_ALERT_CALL:
            case eCMEVT_MEDIA_INFO:
            case eCMEVT_ERROR_CALL:
            {
               /* Copy over the call information.
               */
               if ( pData )
               {
                  CCTKCALLINFO *pCallInfo =
                     (CCTKCALLINFO *) malloc ( sizeof ( CCTKCALLINFO ) );

                  memcpy ( pCallInfo,
                           pData,
                           sizeof ( CCTKCALLINFO ) );
                  evt.pDat3 = pCallInfo;
               }
            }
            break;

            case eCMEVT_MWI_INFO:
            {
               /* Copy over the MWI information.
               */
               if ( pData )
               {
                  CCTKMWIINFO *pMwi =
                     (CCTKMWIINFO *) malloc ( sizeof ( CCTKMWIINFO ) );

                  memcpy ( pMwi,
                           pData,
                           sizeof ( CCTKMWIINFO ) );
                  evt.pDat3 = pMwi;
               }
            }
            break;

            case eCMEVT_DLG_INFO:
            {
               /* Copy over the Dialog information.
               */
               if ( pData )
               {
                  BOS_BOOL *pDlg =
                     (BOS_BOOL *) malloc ( sizeof ( BOS_BOOL ) );

                  memcpy ( pDlg,
                           pData,
                           sizeof ( BOS_BOOL ) );
                  evt.pDat3 = pDlg;
               }
            }
            break;

            case eCMEVT_FORWARD_INFO:
            {
               /* Copy over the Forward information.
               */
               if ( pData )
               {
                  CCTKFEATINFO *pFwdInfo =
                     (CCTKFEATINFO *) malloc ( sizeof ( CCTKFEATINFO ) );

                  memcpy ( pFwdInfo,
                           pData,
                           sizeof ( CCTKFEATINFO ) );
                  evt.pDat3 = pFwdInfo;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_UPDATE:
#endif
            case eCMEVT_USER_UPDATE:
            {
               /* Copy over the user update information.
               */
               if ( pData )
               {
                  BOS_UINT32 *pUser =
                     (BOS_UINT32 *) malloc ( sizeof ( BOS_UINT32 ) );

                  memcpy ( pUser,
                           pData,
                           sizeof ( BOS_UINT32 ) );
                  evt.pDat3 = pUser;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_PAU_UPDATE:
#endif
            case eCMEVT_PAU_UPDATE:
            {
               /* Copy over the P-Associated-URI update information.
               */
               if ( pData )
               {
                  CCTKPAUINFO *pPAURIs = (CCTKPAUINFO *) malloc (
                     sizeof ( CCTKPAUINFO )
                  );
                  if ( pPAURIs )
                  {
                     memcpy( pPAURIs, pData, sizeof ( CCTKPAUINFO ) );
                     evt.pDat3 = pPAURIs;
                  }
               }
            }
            break;

            case eCMEVT_OOB_DTMF:
            {
               /* Copy over the out-of-band data information
               */
               if ( pData )
               {
                  CCTKOOBDTMF *pOobDtmf =
                     (CCTKOOBDTMF *) malloc ( sizeof ( CCTKOOBDTMF ) );

                  memcpy ( pOobDtmf,
                           pData,
                           sizeof ( CCTKOOBDTMF ) );
                  evt.pDat3 = pOobDtmf;
               }
            }
            break;

            case eCMEVT_CWT_INFO:
            {
               /* Copy over the Call Waiting Tone information
               */
               if ( pData )
               {
                  CCTKCWTINFO *pCwtInfo =
                     (CCTKCWTINFO *) malloc ( sizeof ( CCTKCWTINFO ) );

                  memcpy ( pCwtInfo,
                           pData,
                           sizeof ( CCTKCWTINFO ) );
                  evt.pDat3 = pCwtInfo;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_CDR_INFO:
#endif
            case eCMEVT_CDR_INFO:
            {
               /* Copy over the call data record information
               */
               if ( pData )
               {
                  CCTKCDRINFO *pCdrInfo =
                     (CCTKCDRINFO *) malloc ( sizeof ( CCTKCDRINFO ) );

                  memcpy ( pCdrInfo,
                           pData,
                           sizeof ( CCTKCDRINFO ) );
                  evt.pDat3 = pCdrInfo;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_IN_SERVICE:
#endif
            case eCMEVT_USER_IN_SERVICE:
            {
               evt.uDat2 = uUid;

               /* Copy over the Service information (if present)
               */
               if ( pData )
               {
                  CCTKKAINFO *pInfo =
                     (CCTKKAINFO *) malloc ( sizeof ( CCTKKAINFO ) );

                  memcpy ( pInfo,
                           pData,
                           sizeof ( CCTKKAINFO ) );
                  evt.pDat3 = pInfo;
               }
            }
            break;

            case eCMEVT_HOLD_LOCAL_FAIL:
#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_OUT_OF_SERVICE:
#endif
            case eCMEVT_USER_OUT_OF_SERVICE:
            {

               /* Copy over the Response Code (if present)
               */
               if ( pData )
               {
                  CCTKRESPCODE *pRCode =
                     (CCTKRESPCODE *) malloc ( sizeof ( CCTKRESPCODE ) );

                  memcpy ( pRCode,
                           pData,
                           sizeof ( CCTKRESPCODE ) );
                  evt.pDat3 = pRCode;
               }
            }
            break;

            case eCMEVT_XFERED_CALL:
            case eCMEVT_XFER_REQUEST:
            case eCMEVT_MEDIA_XFER:
            {
               /* Copy over the call transfer related information.
               */
               if ( pData )
               {
                  BOS_UINT32 *pXfer =
                     (BOS_UINT32 *) malloc ( sizeof ( BOS_UINT32 ) );

                  memcpy ( pXfer,
                           pData,
                           sizeof ( BOS_UINT32 ) );
                  evt.pDat3 = pXfer;
               }
            }
            break;

            case eCMEVT_CCTK_USRADD_FAIL:
            {
               /* Copy over the status information.
               */
               if ( pData )
               {
                  CCTKSTATUS *pStatus =
                     (CCTKSTATUS *) malloc ( sizeof ( CCTKSTATUS ) );

                  memcpy ( pStatus,
                           pData,
                           sizeof ( CCTKSTATUS ) );
                  evt.pDat3 = pStatus;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_TARGETS_UPDATE:
#endif
            case eCMEVT_TARGETS_UPDATE:
            {
               /* Copy over the target information
               */
               if ( pData )
               {
                  CCTKTARGETINFO *pTargetInfo =
                     (CCTKTARGETINFO *) malloc ( sizeof ( CCTKTARGETINFO ) );

                  memcpy ( pTargetInfo,
                           pData,
                           sizeof ( CCTKTARGETINFO ) );
                  evt.pDat3 = pTargetInfo;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_PUBLISH_TARGET:
#endif
            case eCMEVT_PUBLISH_TARGET:
            {
               /* Copy over the publish target information
               */
               if ( pData )
               {
                  BOS_IP_ADDRESS *pPublishTargetAddr =
                     (BOS_IP_ADDRESS *) malloc ( sizeof ( BOS_IP_ADDRESS ) );

                  memcpy ( pPublishTargetAddr,
                           pData,
                           sizeof ( BOS_IP_ADDRESS ) );
                  evt.pDat3 = pPublishTargetAddr;
               }
            }
            break;

#if CMGRCFG_MULTILINE
            case eCMEVT_ACCNT_SUB_STATUS:
#endif
            case eCMEVT_SUB_STATUS:
            {
               /*Copy over subscription status information*/
               if (pData)
               {
                  BOS_BOOL *pbMWISubscribed = malloc (sizeof (BOS_BOOL));
                  memcpy (pbMWISubscribed, pData, sizeof(BOS_BOOL));
                  evt.pDat3 = pbMWISubscribed;
               }
            }
            break;

#if CMGRCFG_REMOTE_USER_CONTROL
            case eCMEVT_REMOTE_USER_ADD:
            case eCMEVT_REMOTE_USER_REMOVE:
            {
               /* Pass over the network handle (GUID) for this user.
               */
               evt.uDat2 = uUid;

               /* Copy over the user identification information.
               */
               if ( pData )
               {
                  CCTKREMUSRINFO *pRemUsrInfo =
                     (CCTKREMUSRINFO *) malloc ( sizeof ( CCTKREMUSRINFO ) );

                  memcpy ( pRemUsrInfo,
                           pData,
                           sizeof ( CCTKREMUSRINFO ) );
                  evt.pDat3 = pRemUsrInfo;
               }
            }
            break;
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

#if CMGRCFG_PASS_THRU_MSG
            case eCMEVT_MESSAGE:
            {
               /* Copy over the message information.
               */
               if ( pData )
               {
                  CCTKMSGPASSTHRU *pMsg =
                     (CCTKMSGPASSTHRU *) malloc ( sizeof ( CCTKMSGPASSTHRU ) );
                  BOS_UINT8 *puData = NULL;

                  memcpy ( pMsg,
                           pData,
                           sizeof ( CCTKMSGPASSTHRU ) );

                  if ( ((CCTKMSGPASSTHRU *)pData)->puData )
                  {
                     puData = (BOS_UINT8 *) malloc (
                           sizeof ( BOS_UINT8 ) * ((CCTKMSGPASSTHRU *)pData)->uSize );

                     memcpy ( puData,
                              (void *) (((CCTKMSGPASSTHRU *)pData)->puData),
                              sizeof ( BOS_UINT8 ) *
                                 ((CCTKMSGPASSTHRU *)pData)->uSize );
                  }

                  pMsg->puData = puData;
                  evt.pDat3 = pMsg;
               }
            }
            break;

            case eCMEVT_MESSAGE_RESP:
            {
               /* Copy over the code status information.
               */
               if ( pData )
               {
                  BOS_UINT32 *pStatus =
                     (BOS_UINT32 *) malloc ( sizeof ( BOS_UINT32 ) );

                  memcpy ( pStatus,
                           pData,
                           sizeof ( BOS_UINT32 ) );
                  evt.pDat3 = pStatus;
               }
            }
            break;
#endif /* CMGRCFG_PASS_THRU_MSG */

             case eCMEVT_RELEASE_CALL:
             case eCMEVT_CLEANED_CALL:
             {
             }
             break;

             default:
            {
               evt.pDat3 = NULL;
            }
            break;
         }

         /* If we just received a media information event and the session
         ** for which this applies is marked as a fast-track media session, we
         ** need to process the event right away.  This is a very special case
         ** where the event does not go through the framework because we cannot
         ** afford the time.
         */
         if ( (eFsmEvt == eCMEVT_MEDIA_INFO) &&
              (pUsr != NULL) &&
              (pSes != NULL) &&
              pSes->ses.med.bFastTrac )
         {
            CMEVT curEvt;

            memcpy ( &curEvt, &pUsr->evt, sizeof ( CMEVT ) );

            memcpy ( &pUsr->evt, &evt, sizeof ( CMEVT ) );
            cmFsmMedia ( (void *) pUsr );
            pSes->ses.med.bFastTrac = BOS_FALSE;

            /* Consume the event such that it is not passed to the
            ** framework.
            */
            bValid = BOS_FALSE;

            /* Cleanup and restore.
            */
            if ( evt.pDat3 )
            {
               free ( evt.pDat3 );
               evt.pDat3 = NULL;
            }
            memcpy ( &pUsr->evt, &curEvt, sizeof ( CMEVT ) );
         }

         if ( bValid &&
              (cmPostEvt ( &evt ) == BOS_STATUS_OK)  )
         {
            cmSignalEvt ();
         }
      }
   } /* bValid */
}


/***********************************************************************************
**  FUNCTION:   cmPhyCb
**
**  PURPOSE:    Callback from physical (endpoint) upon event notification.
**
**  PARAMETERS: uEvt   - the event reported.
**              uPhy   - physical identifier.
**              uSes   - session identifier - may not be set.
**              pData  - event data, event dependent - may be NULL.
**
**  RETURNS:    Nothing.
**
**  NOTES:      The event type alone will dictate what of the rest of the
**              parameters are valid when passed during the callback.
***********************************************************************************/
void cmPhyCb ( BOS_UINT32 uEvt, BOS_UINT32 uPhy, BOS_UINT32 uSes, void *pData )
{
   CMEVT evt;
   BOS_BOOL bValid = BOS_TRUE;
   CMFSMEVT eFsmEvt;
   CMUSR *pUsr = NULL;
   CMSES *pSes = NULL;

   memset ( &evt, 0, sizeof(CMEVT) );
   eFsmEvt = cmMapGetId ( cmMapPhysEvt2CmEvt, uEvt );

   CMGRLOG_DEBUG (( "cmPhyCb: evt %s (%d), phy 0x%X, ses 0x%X, data 0x%X",
                    (char *) cmMapGetStr ( cmMapCmEvent, eFsmEvt ),
                    uEvt, uPhy, uSes, pData ));

   /* What needs to happen here...
   **
   **    [1] Map the Endpoint event to the corresponding CMGR event.
   **    [2] Ensure that the mapped event is for a valid identifier.
   **    [3] If necessary, copy some of the data passed with the event that may
   **        be of interest during process of such event.
   **
   ** After the three above steps have been taken successfully, the event will be
   ** posted for processing.
   */

   /* Verify the validity of the event. */
   if ( eFsmEvt == CMGR_INVALID_HANDLE )
   {
      bValid = BOS_FALSE;
   }

   /* Verify the user of this event is valid. */
   if ( (pUsr = cmUsrFindPhyHdl ( uPhy )) == NULL )
   {
      /* If this was an initial 'off-hook' event not yet associated to
      ** any user, then we are likely here, in such case, we want to setup
      ** the association now.
      */
      if ( eFsmEvt == eCMEVT_OFFHOOK )
      {
         pUsr = cmUsrPhyAssociate ( uPhy );
      }

      /* Here we are doomed...
      */
      if ( pUsr == NULL )
      {
         bValid = BOS_FALSE;
      }
   }

   /* For 'off-hook' events, we will need to assign the CCTK account if
   ** one is not yet assigned.  This is for the case of an outgoing call.
   */
   if ( bValid && ( eFsmEvt == eCMEVT_OFFHOOK ) )
   {
      if ( pUsr->uUindex == CMGR_INVALID_HANDLE )
      {
         if ( !cmUsrCctkAccntAssociate( pUsr ) )
         {
            /* Failed to associate a CCTK account.  We will need to continue
            ** passing this event to the CMGR thread in case this is a request
            ** for internal calls of which no CCTK acccount is required.
            */
            /* TODO - assign special uUId handles here */
         }
      }
   }

   /* Verify the session associated with this event is valid (if any). */
   if ( (uSes != CMGR_INVALID_HANDLE) && (pSes = cmSesFindPhyHdl ( uSes )) == NULL )
   {
      /* If this is a media event from the endpoint, it is possible that we had
      ** reach a race condition whereby we are not yet aware of the session
      ** association for this event.  In such case, we still queue up the event
      ** in an attempt to give it another chance.
      */
      if ( eFsmEvt == eCMEVT_MEDIA )
      {
         eFsmEvt = eCMEVT_MEDIA_DELAY;
      }
      else
      {
         bValid = BOS_FALSE;
      }
   }
   /* If we received an invalid session handle but the user
   ** has a valid focus session, assume we want the event to
   ** be applied to the focus session.
   */
   if( uSes == CMGR_INVALID_HANDLE )
   {
      pSes = (pUsr) ? cmUsrFocusSes( pUsr ) : NULL;
   }

   /* All is well, post the event.
   */
   if ( bValid )
   {
      /* If this is an offhook event, indicate we have a pending
      ** offhook action.
      */
      if( eFsmEvt == eCMEVT_OFFHOOK )
      {
         pUsr->bPendingActPhy = BOS_TRUE;
      }

      evt.uEvt = ( eFsmEvt | CMGR_EVT_CLASS__PHY_IF );
      evt.uDat1 = pUsr->uHdl;
      evt.uDat2 = ( (pSes != NULL) ? pSes->uHdl :
         ((eFsmEvt == eCMEVT_MEDIA_DELAY) ? uSes : CMGR_INVALID_HANDLE) );

      /* Extra data may be associated with this event depending on the event
      ** type.
      */
      switch ( eFsmEvt )
      {
         case eCMEVT_SES_STATUS:
         {
            if ( pData != NULL )
            {
               CMGRCNX *pCnx = (CMGRCNX *) malloc ( sizeof ( CMGRCNX ) );

               memcpy ( pCnx,
                        pData,
                        sizeof ( CMGRCNX ) );
               evt.pDat3 = pCnx;
            }
         }
         break;

         case eCMEVT_FAX_TONE:
         case eCMEVT_MODEM_TONE:
         {
            if ( pData != NULL )
            {
               BOS_UINT32 *pTone = (BOS_UINT32 *) malloc ( sizeof ( BOS_UINT32 ) );

               memcpy ( pTone,
                        pData,
                        sizeof ( BOS_UINT32 ) );
               evt.pDat3 = pTone;
            }
         }
         break;

         case eCMEVT_DTMF_0:
         case eCMEVT_DTMF_1:
         case eCMEVT_DTMF_2:
         case eCMEVT_DTMF_3:
         case eCMEVT_DTMF_4:
         case eCMEVT_DTMF_5:
         case eCMEVT_DTMF_6:
         case eCMEVT_DTMF_7:
         case eCMEVT_DTMF_8:
         case eCMEVT_DTMF_9:
         case eCMEVT_DTMF_STAR:
         case eCMEVT_DTMF_HASH:
         case eCMEVT_DTMF_A:
         case eCMEVT_DTMF_B:
         case eCMEVT_DTMF_C:
         case eCMEVT_DTMF_D:
         {
            if ( pData != NULL )
            {
               EPTDTMF *pType = (EPTDTMF *) malloc ( sizeof ( EPTDTMF ) );

               memcpy ( pType,
                        pData,
                        sizeof ( EPTDTMF ) );
               evt.pDat3 = (void *)pType;
            }
            /* if user is in hookflash and wait for key code state, then set
            ** last event to true
            */
            if ( pUsr->rrFlash.bRRWaitKey )
            {
               evt.bLast = BOS_TRUE;
            }

         }
         break;

         case eCMEVT_INGRESS_DTMF:
         {
            evt.pDat3 = (void *)pData;
         }
         break;

         default:
         {
            evt.pDat3 = NULL;
         }
         break;
      }

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK  )
      {
         cmSignalEvt ();
      }
   }
}


/***********************************************************************************
**  FUNCTION:   cmDmExtVarCb
**
**  PURPOSE:    Callback from the digit map module when this latter needs to get
**              the value of an external to the digit map variable.
**
**  PARAMETERS: uGuid  - unique identifier associated with who invoked this
**                       callback.  In practice it corresponds to the user
**                       handle that the information needs to be retrieved for.
**              pVarName - pointer to the variable name.
**              pVarValue - pointer to the placeholder for the variable value.
**              uVarValueLen - [IN] size of the placeholder, [OUT] actual value
**                             length.
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function needs to retrieve the information queried in a
**              synchronous manner otherwise the digit map processing will be
**              incorrect.
***********************************************************************************/
void cmDmExtVarCb( BOS_UINT32 uGuid, const BOS_UINT8 *pVarName,
                   BOS_UINT8 *pVarValue, BOS_UINT32 *puVarValueLen )
{
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( uGuid );

   BOS_UINT32 uVarLen = 0;
   CFGSIPDIGITMAPEXTVAR *pDmExtVar = NULL;
   PROV_CFG_IDX cfgIdxUsr;
   PROV_CFG_IDX cfgIdxVar;

   if ( pCctkAccnt && *puVarValueLen && pVarValue && pVarName )
   {
      memset ( pVarValue,
               0,
               *puVarValueLen );

      memset ( &cfgIdxUsr, 0, sizeof ( PROV_CFG_IDX ) );
      memset ( &cfgIdxVar, 0, sizeof ( PROV_CFG_IDX ) );

      cfgIdxUsr.pcCharIdx = pCctkAccnt->uUid;
      cfgIdxVar.pcCharIdx = (BOS_UINT8 *) pVarName;
      cfgIdxUsr.pNext = &cfgIdxVar;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapExtVarConcept ),
                                 &cfgIdxUsr,
                                 (void *) &pDmExtVar,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pDmExtVar && pDmExtVar->Act.bCfgStatus )
         {
            uVarLen =
               (strlen ( (const char *) pDmExtVar->pValue ) > *puVarValueLen ) ?
                  *puVarValueLen : strlen ( (const char *) pDmExtVar->pValue );

            memcpy ( pVarValue,
                     pDmExtVar->pValue,
                     uVarLen );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, DigitMapExtVarConcept ),
                               &cfgIdxUsr,
                               (void *) pDmExtVar,
                               CFG_ITEM_DEL );
         pDmExtVar = NULL;
      }

      CMGRLOG_DEBUG (( "cmDmExtVarCb: 0x%X (0x%X) - \'%s\' = \'%s\' (%d)",
                       pCctkAccnt->uHdl, pCctkAccnt->uNet,
                       pVarName, pVarValue, uVarLen ));
   }

   /* Update the external variable value length returned to the
   ** digit map.  If an error occured, this value is set to 0 and the
   ** digit map should not attempt to use the pVarValue information as
   ** it may be invalid.
   */
   *puVarValueLen = uVarLen;
}


/***********************************************************************************
** FUNCTION:   cmUpdateState
**
** PURPOSE:    sets the state of the Call Manager.
**
** PARAMETERS: uRid - ID, not used by Call Manager, just pass up to the app
**             eStatus - state of the Call Manager
**
** RETURNS:    None.
***********************************************************************************/
void cmUpdateState( BOS_UINT32 uRid,
                    CMGRSTATUS eStatus )
{
   gCmStatusCb ( uRid,
                 eStatus );

   gCmState = eStatus;

}

/***********************************************************************************
** FUNCTION:   cmGetState
**
** PURPOSE:    returns the state of the Call Manager.
**
** PARAMETERS:  uRid - ID, not used currently
**
** RETURNS:    The state of Call Manager
***********************************************************************************/
CMGRSTATUS cmGetState( BOS_UINT32 uRid )
{
   return ( gCmState );
}

