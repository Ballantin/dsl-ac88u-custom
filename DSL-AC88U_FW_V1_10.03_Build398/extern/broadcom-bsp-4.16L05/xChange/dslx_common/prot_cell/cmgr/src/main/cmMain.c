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
                       
static void cmAppNetCb ( CMNETEVT eEvent, BOS_UINT32 uCid, void *pData );

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
            if ( cmCfgInit () && !cmPhyIsStarted () )
            {
               cmPhyStartup ( &cmPhyCb );
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

         CMGRLOG_DEBUG (( "cmUsrCmdHandler: Starting up PHY" ));

         /* Startup the physical interface.
         */
         if ( ( eStatus == eCMGRSTATUS_STARTUP_SUCCESS ) &&
              ( cmPhyStartup ( &cmPhyCb ) != eCMGRSTATUS_STARTUP_SUCCESS ) )
         {
            eStatus = eCMGRSTATUS_STARTUP_FAILED;
         }
         
         CMGRLOG_DEBUG (( "cmUsrCmdHandler: Starting up APPNET" ));

         /* Startup the App Net interface.
         */
         if ( ( eStatus == eCMGRSTATUS_STARTUP_SUCCESS ) &&
              ( cmAppNetStartup ( &cmAppNetCb ) != eCMGRSTATUS_STARTUP_SUCCESS ) )
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
         else if ( cmCfgUser () )
         {
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

      case eCMCMD_RESTART:
      {
         if ( cmCfgInit () && !cmPhyIsStarted () )
         {
            cmPhyStartup ( &cmPhyCb );
         }
      }
      break;

      case eCMCMD_SHUTDOWN:
      {
         /* Apply shutdown right away.
         */
         bosSleep(2000);
         gbCmShutdown = BOS_TRUE;
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
            }
            break;

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


      case eCMCMD_STATUS:
      {
         /* If CMGR status information has been requested.
         */
         if ( (BOS_BOOL) pEvt->uDat1 )
         {
            cmShow();
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
      
      /****** TODO: Remove? ******/
      // /* ... or it is the network user handle in some special situation.
      // */
      // if ( (pUsr == NULL) &&
           // ((CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_USER_IN_SERVICE) )
      // {
         // pUsr = cmUsrFindNetHdl ( pEvt->uDat1 );
      // }
      /***************************/

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
      /*********** TODO: remove? *********/
      // else if ( (CMGR_EVT_CLASS__EVT_MASK & pEvt->uEvt) == eCMEVT_RESET_COMP )
      // {
         // /* If we are still waiting for an answer to the asynchronous
         // ** reset command sent to the CCTK layer, do not do anything right
         // ** now, instead wait for that command to terminate before applying
         // ** any changes.
         // */
         // if ( !gbCmResetAsynch )
         // {
            // if ( cmPhyTerminate () != eCMGRSTATUS_COMMAND_SUCCESS )
            // {
               // CMGRLOG_CRITICAL ((
                  // "cmCctkCmdHandler: cmPhyTerminate FAILED!" ));
            // }

            // /* Module cleanup.  Order is important.
            // */
            // cmTmr ( BOS_FALSE );
            // cmCdr ( BOS_FALSE );
            // cmSes ( BOS_FALSE );
            // cmUsr ( BOS_FALSE );
// #if CMGRCFG_MULTILINE
            // cmCctkAccnt ( BOS_FALSE );
// #endif /* CMGRCFG_MULTILINE */

            // /* Flush the event queue in case there were any new events that
            // ** were added while cmgr/cctk was cleaning up
            // */
            // cmFlushEvt();

            // /* Module re-creation.
            // */
// #if CMGRCFG_MULTILINE
            // cmCctkAccnt ( BOS_TRUE );
// #endif /* CMGRCFG_MULTILINE */
            // cmCdr ( BOS_TRUE );
            // cmSes ( BOS_TRUE );
            // cmUsr ( BOS_TRUE );
            // cmTmr ( BOS_TRUE );

            // cmCfgInit ();
         // }
      // }

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
         case eCMEVT_INCOMING_CALL:
         case eCMEVT_CONNECT_CALL:

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

/***********************************************************************************
**  FUNCTION:   cmAppNetCb
**
**  PURPOSE:    Callback from vodsl upon NET event notification.
**
**  PARAMETERS: eEvent - the event reported.
**              uCid   - call or service reference identifier - may not be set.
**              pData  - event data, event dependent - may be NULL.
**
**  RETURNS:    Nothing.
**
**  NOTES:      The event type alone will dictate what of the rest of the
**              parameters are valid when passed during the callback.
***********************************************************************************/
void cmAppNetCb ( CMNETEVT eEvent, BOS_UINT32 uCid, void *pData )
{
   CMEVT evt;
   CMFSMEVT eFsmEvt;
   CMUSR *pUsr = NULL;
   CMSES *pSes = NULL;
   BOS_BOOL bValid = BOS_TRUE;
   
   memset ( &evt, 0, sizeof(CMEVT) );
   eFsmEvt = cmMapGetId ( cmMapNetEvt2CmEvt, eEvent );

   CMGRLOG_DEBUG (( "cmAppNetCb: evt %s (%d), cid 0x%X, data 0x%X",
                    (char *) cmMapGetStr ( cmMapCmEvent, eFsmEvt ),
                    eEvent, uCid, pData ));
                    
   /* Verify the validity of the event. */
   if ( eFsmEvt == CMGR_INVALID_HANDLE )
   {
      bValid = BOS_FALSE;
   }

   /* Find the user for this call.
   */
   if ( ( pUsr = cmUsrFindCid( uCid, BOS_FALSE ) ) == NULL )
   {
      bValid = BOS_FALSE;
   }

   /* In case of an 'in-service' event, we may not have the valid user
   ** identifier just yet because the user handle may have been swapped
   ** and the 'update' event has not been processed, in such case bypass
   ** the check.  Further processing will take place in the state machine
   ** to validate the event.
   */
   /******** TODO: remove? ************/
   // if ( ( pUsr == NULL  ) &&
        // ( eFsmEvt == eCMEVT_USER_IN_SERVICE ) )
   // {
      // bValid = BOS_TRUE;
   // }

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
          ( eFsmEvt == eCMEVT_CLEANED_CALL )
        ) )
   {
      bValid = BOS_TRUE;
      
      /* There's only one appnet user, so we should be able to find it anyways.
      */
      pUsr = cmUsrFindNetHdl( CMGR_APPNET_HANDLE );
   }
   
   /* All is well, post the event.
   */
   if ( bValid )
   {
      /* Fill in event block parameters
      */
      evt.uEvt = ( eFsmEvt | CMGR_EVT_CLASS__CCTK );

      /* uDat1 should contain the user handle if valid
      ** If usr handle is not valid and the event is an
      ** IN_SERVICE event, pass the CCTK network handle instead
      */
      evt.uDat1 = ( (pUsr != NULL) ? pUsr->uHdl : CMGR_INVALID_HANDLE );

      /* uDat2 should contain the session handle if valid
      ** If the session handle is not valid then then for
      ** a few select events pass the callid instead
      */
      evt.uDat2 = ( (pSes != NULL) ? pSes->uHdl :
         ( ( ( eFsmEvt == eCMEVT_INCOMING_CALL ) ||
             ( eFsmEvt == eCMEVT_CLEANED_CALL )  ||
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
      
      /* In the non multiline case there is no reason to halt processing until the
      ** event has been posted to all the users.
      */
      evt.bLast = BOS_TRUE;

      /* uDat4 should contain the Network account handle
      */
      evt.uDat4 = CMGR_APPNET_HANDLE;


      /* Extra data may be associated with this event depending on the event
      ** type.
      */
      switch ( eFsmEvt )
      {
         case eCMEVT_ALERT_CALL:
         case eCMEVT_CONNECT_CALL:
         case eCMEVT_INCOMING_CALL:
         {
            /* Copy over the call information.
            */
            if ( pData )
            {
               CMNETCALLINFO *pCallInfo =
                  (CMNETCALLINFO *) malloc ( sizeof ( CMNETCALLINFO ) );

               memcpy ( pCallInfo,
                        pData,
                        sizeof ( CMNETCALLINFO ) );
               evt.pDat3 = pCallInfo;
            }
         }
         break;

          case eCMEVT_RELEASE_CALL:
          case eCMEVT_CLEANED_CALL:
          {
          }
         // break;

          default:
         {
            evt.pDat3 = NULL;
         }
         break;
      }

      if ( bValid &&
           (cmPostEvt ( &evt ) == BOS_STATUS_OK)  )
      {
         cmSignalEvt ();
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

   // /* For 'off-hook' events, we will need to assign the CCTK account if
   // ** one is not yet assigned.  This is for the case of an outgoing call.
   // */
   // if ( bValid && ( eFsmEvt == eCMEVT_OFFHOOK ) )
   // {
      // if ( pUsr->uUindex == CMGR_INVALID_HANDLE )
      // {
         // if ( !cmUsrCctkAccntAssociate( pUsr ) )
         // {
            // /* Failed to associate a CCTK account.  We will need to continue
            // ** passing this event to the CMGR thread in case this is a request
            // ** for internal calls of which no CCTK acccount is required.
            // */
            // /* TODO - assign special uUId handles here */
         // }
      // }
   // }

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
   BOS_UINT32 uVarLen = 0;
   CFGSIPDIGITMAPEXTVAR *pDmExtVar = NULL;
   PROV_CFG_IDX cfgIdxUsr;
   PROV_CFG_IDX cfgIdxVar;

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

