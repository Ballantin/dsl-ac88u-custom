/**********************************************************************************
** Copyright (c) 2010-2013 Broadcom Corporation
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
**      This file implements the Call Manager API.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
BOS_BOOL gbShutDown = BOS_TRUE;


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmgrInit
**
** PURPOSE:    Init the Call Manager stack.
**
** PARAMETERS: statusCb - the status callback function to be registered, this
**                        callback cannot be NULL or Call Manager will
**                        not proceed initialization.  This callback will be
**                        invoked with the resulting information..
**
** RETURNS:    None.
**
** NOTE:       This starts up the Call Manager processing task and event queue.
**             Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrInit( CMGRSTATUSCB statusCb )
{
   /* Validate that all the mandatory callbacks are properly set.
   */
   BOS_ASSERT ( statusCb != NULL );
   gCmStatusCb = statusCb;

   gbShutDown = BOS_FALSE;

   /* Launch the Call Manager.
   */
   cmMainInit();
}

/***********************************************************************************
** FUNCTION:   cmgrDeinit
**
** PURPOSE:    Shuts down the Call Manager stack.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       This closes the Call Manager processing task and event queue.
**             Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrDeinit( void )
{

   /* if no shutdown has been queued, queue one now. At this point we don't
    * know if the cmgr init and startup was completed so we need to clean up
    * and shutdown completely.
    * */
   if ( !gbShutDown )
   {
      CMEVT       evt;
      BOS_TIME_MS timeout = 0;      

      gbShutDown = BOS_TRUE;

      /* Post the shutdown event.
      */
      memset ( &evt, 0, sizeof(CMEVT) );
      evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCMCMD_SHUTDOWN);

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }

      /* wait for the CM state to change, then deinit */
      /* CMGR MUST notify me with statusCB */
      while ( ! ( ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_COMPLETE  ) &&
                  ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_FAILURE ) ) )
      {
         bosSleep(20);  
         timeout += 20;
         if ( timeout > 500 ) 
         {
            CMGRLOG_CRITICAL (( "cmgrDeinit() - shutdown did not complete after 500 ms, deiniting anyways." ));
            cmMainDeinit();
            break;
         }
         else
         {
            if ( ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_COMPLETE ) ||
                 ( cmGetState( CMGR_INVALID_HANDLE ) == eCMGRSTATUS_SHUTDOWN_FAILURE ) )
            {
               cmMainDeinit();
               break;
            }
         }
      }
   }
   else
   {
      cmMainDeinit();
   }
}

/***********************************************************************************
** FUNCTION:   cmgrStartup
**
** PURPOSE:    Startups the Call Manager processing.
**
** PARAMETERS: statusCb - the status callback function to be registered, this
**                        callback cannot be NULL or Call Manager will
**                        not proceed to startup.  This callback will be
**                        invoked with the resulting information.
**             pCfg     - pointer to the configuration block containing in
**                        particular the necessary external callbacks that
**                        the Call Manager will use during its process.
**
** RETURNS:    None.
**
** NOTE:       Invoking this function will do the following:
**
**                 [I]  - Register all the necessary callbacks.
**                [II]  - Setup the Call Manager with all third
**                        party modules and setup those modules as
**                        necessary.
**               [III]  - Create and launch the Call Manager
**                        execution thread.
**                [IV]  - Signal to the application the readiness
**                        of the Call Manager to accept further
**                        commands and actions.
**
***********************************************************************************/
void cmgrStartup ( CMGRSTATUSCB statusCb, CMGRCFGBLK *pCfg )
{
   CMEVT evt;
   
   /* Validate that all the mandatory callbacks are properly set.
   */
   BOS_ASSERT ( statusCb != NULL );
   BOS_ASSERT ( pCfg != NULL );
   BOS_ASSERT ( pCfg->cmgrEptCtlCb != NULL );
   BOS_ASSERT ( pCfg->cmgrProvCb != NULL );
   BOS_ASSERT ( pCfg->cmgrSesCtlCb != NULL );

   gCmStatusCb = statusCb;
   memcpy ( &gCmCfgCb, pCfg, sizeof(CMGRCFGBLK) );

   /* Post the initial startup event.
   */
   memset ( &evt, 0, sizeof(CMEVT) );
   evt.uEvt = (BOS_UINT32) ( CMGR_EVT_CLASS__USR_CMD | eCMCMD_STARTUP );

   if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
   {
      cmSignalEvt ();
   }
}


/***********************************************************************************
** FUNCTION:   cmgrShutdown
**
** PURPOSE:    Shuts down the Call Manager processing.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       Result of this action will be invoked on the status callback
**             registered interface.
**
***********************************************************************************/
void cmgrShutdown ( void )
{
   if ( !gbShutDown )
   {
      CMEVT evt;

      gbShutDown = BOS_TRUE;

      /* Post the shutdown event.
      */
      memset ( &evt, 0, sizeof(CMEVT) );
      evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCMCMD_SHUTDOWN);
      evt.uDat1 = eCCTKRSTMODE_GRACEFULNONBLOCKING;

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }
   }
   else
   {
      /* If we already ended the main loop, shutdown took place already, this
      ** could be some rogue application call or could be for a valid reason,
      ** either ways we should let the application know that the shutdown is a
      ** success as it may be waiting on this to proceed further.
      */
      if ( cmPhyIsStarted () )
      {
         cmPhyShutdown ();
      }

      if ( gCmStatusCb )
      {
         cmUpdateState ( CMGR_INVALID_HANDLE,
                         eCMGRSTATUS_SHUTDOWN_COMPLETE );
      }
   }
}


/*****************************************************************************
** FUNCTION:    cmgrCmd
**
** PURPOSE:     Send a command to the call manager for execution of some
**              associated functionality.
**
** PARAMETERS:  eCmd     - the command to be executed.
**              pCmdInfo - the command related parameters (when necessary).
**
** RETURNS:     None.
**
** NOTE:        Unless explicitely stated, commands are to be executed in
**              an asynchronous fashion as they are queued up to the
**              Call Manager thread for execution.  The command response
**              (when such is required) is passed back through the status
**              callback registered during initialization.
******************************************************************************/
void cmgrCmd ( CMGRCMD eCmd, void *pCmdInfo )
{
   CMEVT evt;

   memset ( &evt, 0, sizeof(CMEVT) );

   CMGRLOG_DEBUG (( "cmgrCmd: 0x%lX (0x%X)", eCmd, pCmdInfo ));

   /* Return immediately if SIP stack is already shuts down or never initialized
   */
   if ( gbShutDown )
   {
      CMGRLOG_INFORMATION (( "cmgrCmd: 0x%lX (0x%X) returning immediately, CMGR is "
         "shutdown or not yet initialized.", eCmd, pCmdInfo ));
      return;  
   }
   
   switch ( eCmd )
   {
      /* Command information should be the global user identifier.
      */
      case eCMCMD_REGISTER:
      case eCMCMD_DEREGISTER:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = *( (BOS_UINT32 *) pCmdInfo );
      }
      break;

      /* Command information should be the connection information status
      ** block.
      */
      case eCMCMD_CNXSTATUS:
      {
         CMGRCNX *pCnx = (CMGRCNX *) malloc ( sizeof ( CMGRCNX ) );
         CMSES *pSes = NULL;

         if ( ( pSes = cmSesFindPhyHdl ( ((CMGRCNX *) pCmdInfo)->uCnx ) ) != NULL )
         {
            if ( pCnx )
            {
               memcpy ( (void *) pCnx, pCmdInfo, sizeof ( CMGRCNX ) );
            }

            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCMEVT_SES_STATUS);
            evt.uDat1 = (BOS_UINT32) pSes->uUsr;
            evt.uDat2 = (BOS_UINT32) pSes->uHdl;
            evt.pDat3 = (void *) pCnx;
         }
      }
      break;

      /* Command information should be the connection statistics block.
      */
      case eCMCMD_CNXSTATS:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__PHY_IF | eCmd);

         /* Process is TBD... */
      }
      break;

#if CMGRCFG_ANNOUNCEMENT
      /* Command information should be the announcement information status
      ** block.
      */
      case eCMCMD_ANNSTATUS:
      {
         CMGRANNSTA *pAnn = (CMGRCNX *) pCmdInfo;
      }
      break
#endif

      case eCMCMD_MEMSTATS:
      {
         CMGRMEMSTATS *pMemStat = (CMGRMEMSTATS *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pMemStat->bCctkOnly;
         evt.uDat2 = (BOS_UINT32) pMemStat->bTraceTable;
      }
      break;

      case eCMCMD_NEWCONFIG:
      {
         CMGRRECONFIG *pReConfig = (CMGRRECONFIG *) pCmdInfo;
         BOS_UINT8 *pUid = (BOS_UINT8 *) malloc ( sizeof ( pReConfig->uUid ) );

         if ( pUid )
         {
            memcpy ( (void *) pUid,
                     (void *) pReConfig->uUid,
                     sizeof ( pReConfig->uUid ) ); 
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pReConfig->eCfgAction;
         evt.pDat3 = (void *) pUid;
      }
      break;

      case eCMCMD_SVCSTART:
      case eCMCMD_SVCEND:
      {
         CMGRSVCACT *pSvcAct = (CMGRSVCACT *) pCmdInfo;
         BOS_UINT8 *pUid = (BOS_UINT8 *) malloc ( sizeof ( pSvcAct->uUid ) );

         if ( pUid )
         {
            memcpy ( (void *) pUid,
                     (void *) pSvcAct->uUid,
                     sizeof ( pSvcAct->uUid ) );
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pSvcAct->service;
         evt.pDat3 = (void *) pUid;
      }
      break;

      case eCMCMD_PLACECALL:
      {
         CMGRPLACECALL *pCallInfo = (CMGRPLACECALL *) pCmdInfo;
         CMGRPLACECALL *pCallOut = 
           (CMGRPLACECALL *) malloc ( sizeof( CMGRPLACECALL ) );

         if ( pCallOut )
         {
            memcpy ( (void *) pCallOut->uDest,
                     (void *) pCallInfo->uDest,
                     sizeof ( pCallInfo->uDest ) );

            memcpy ( (void *) pCallOut->uUid,
                     (void *) pCallInfo->uUid,
                     sizeof ( pCallInfo->uUid ) );
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pCallInfo->uAppIx;
         evt.pDat3 = (void *) pCallOut;
      }
      break;

      case eCMCMD_ENDCALL:
      case eCMCMD_ANSCALL:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = *( (BOS_UINT32 *) pCmdInfo );
      }
      break;

      case eCMCMD_HOLDSVC:
      {
         CMGRHOLDSVC *pHoldSvc = (CMGRHOLDSVC *) pCmdInfo;
         CMGRHOLDSVC *pHold = (CMGRHOLDSVC *) malloc ( sizeof ( CMGRHOLDSVC ) );

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         pHold->uAppIx = pHoldSvc->uAppIx;
         pHold->bHold = pHoldSvc->bHold;

         /* Copy over supplementary information incase app index is unknown */
         pHold->uCid = pHoldSvc->uCid;
         memcpy ( (void *) pHold->uUid,
                  (void *) pHoldSvc->uUid,
                  ( CMGR_USER_IDENTIFIER_LEN + 1 ) * sizeof ( BOS_UINT8 ) );

         evt.pDat3 = (void *)pHold;
      }
      break;

      case eCMCMD_CONFSVC:
      {
         /* The CMGRAPPCALLHDL structure is used to carry the information
         ** about the two sessions (belonging to the same user) that are to
         ** be placed into conference.
         */
         CMGRAPPCALLHDL *pConfSvc = (CMGRAPPCALLHDL *) pCmdInfo;
         CMGRCONFSVC *pConf =
            (CMGRCONFSVC *) malloc ( sizeof ( CMGRCONFSVC ) );

         pConf->uAppIx = pConfSvc->uAppIx;
         pConf->uSesHdl = pConfSvc->uSesHdl;
         pConf->bLocalConf = BOS_TRUE;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.pDat3 = (void *) pConf;
      }
      break;

      case eCMCMD_CXFERSVC:
      {
         /* The CMGRAPPCALLHDL structure is used to carry the information
         ** about the two sessions (belonging to the same user) that are to
         ** be applied the transfer on.
         */
         CMGRAPPCALLHDL *pXferSvc = (CMGRAPPCALLHDL *) pCmdInfo;
         CMGRAPPCALLHDL *pXfer =
            (CMGRAPPCALLHDL *) malloc ( sizeof ( CMGRAPPCALLHDL ) );

         pXfer->uAppIx = pXferSvc->uAppIx;
         pXfer->uSesHdl = pXferSvc->uSesHdl;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.pDat3 = (void *) pXfer;
      }
      break;

      case eCMCMD_BXFERSVC:
      {
         CMGRPLACECALL *pXferInfo = (CMGRPLACECALL *) pCmdInfo;
         CMGRPLACECALL *pXferOut = 
           (CMGRPLACECALL *) malloc ( sizeof( CMGRPLACECALL ) );

         if ( pXferOut )
         {
            memcpy ( (void *) pXferOut->uDest,
                     (void *) pXferInfo->uDest,
                     sizeof ( pXferInfo->uDest ) );

            memcpy ( (void *) pXferOut->uUid,
                     (void *) pXferInfo->uUid,
                     sizeof ( pXferInfo->uUid ) );
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pXferInfo->uAppIx;
         evt.pDat3 = (void *) pXferOut;
      }
      break;

      case eCMCMD_APPCALLHDL:
      {
         CMGRAPPCALLHDL *pCallHdl = (CMGRAPPCALLHDL *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = pCallHdl->uSesHdl;
         evt.uDat2 = pCallHdl->uAppIx;
      }
      break;

      case eCMCMD_DTMFPT:
      {
         /* The CMGRAPPCALLHDL structure is used to carry the information
         ** about DTMF pass through to be sent via signaling for the user.
         */
         CMGRAPPCALLHDL *pDtmfPt = (CMGRAPPCALLHDL *) pCmdInfo;
         CMGRAPPCALLHDL *pDtmf =
            (CMGRAPPCALLHDL *) malloc ( sizeof ( CMGRAPPCALLHDL ) );

         pDtmf->uAppIx = pDtmfPt->uAppIx;
         pDtmf->uSesHdl = pDtmfPt->uSesHdl;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.pDat3 = (void *) pDtmf;
      }
      break;

      case eCMCMD_CLEARMWI:
      case eCMCMD_SETMWI:      
      {
         BOS_UINT8 *pUsr = 
           (BOS_UINT8 *) malloc ( CMGR_USER_IDENTIFIER_LEN * sizeof( BOS_UINT8 ) );

         if ( pUsr )
         {
            memset ( (void *) pUsr,
                     0,
                     CMGR_USER_IDENTIFIER_LEN * sizeof( BOS_UINT8 ) );

            strPrintf ( (char *) pUsr,
                        CMGR_USER_IDENTIFIER_LEN,
                        (char *) pCmdInfo );
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.pDat3 = (void *) pUsr;
      }
      break;

      case eCMCMD_STATUS:
      {
         CMGRSHOWSTATUS *pStatus = (CMGRSHOWSTATUS *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pStatus->bShowCmgr;
         evt.uDat2 = (BOS_UINT32) pStatus->bShowCctk;
      }
      break;

      case eCMCMD_SHUTDOWN:
      {
         gbShutDown = BOS_TRUE;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 =
            (BOS_UINT32) ((pCmdInfo != NULL) ?
                  *((CCTKRSTMODE *) pCmdInfo) : eCCTKRSTMODE_GRACEFULNONBLOCKING);
      }
      break;

      case eCMCMD_DATALNK:
      {
         BOS_BOOL *pStatus = (BOS_BOOL *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) (*pStatus);
      }
      break;

#if CMGRCFG_PASS_THRU_MSG
      case eCMCMD_MESSAGE:
      {
         CMGRPASSTHRUMSG *pMsg = (CMGRPASSTHRUMSG *) pCmdInfo;
         CMGRPASSTHRUMSG *pMsgOut = 
           (CMGRPASSTHRUMSG *) malloc ( sizeof( CMGRPASSTHRUMSG ) );

         if ( pMsgOut )
         {
            BOS_UINT8 *puData =
               (BOS_UINT8 *) malloc ( sizeof( BOS_UINT8 ) * pMsg->uSize );

            memset ( (void *) pMsgOut,
                     0,
                     sizeof ( CMGRPASSTHRUMSG ) );
            memcpy ( (void *) pMsgOut->toDest.uDest,
                     (void *) pMsg->toDest.uDest,
                     sizeof ( pMsg->toDest.uDest ) );

            memcpy ( (void *) pMsgOut->toDest.uUid,
                     (void *) pMsg->toDest.uUid,
                     sizeof ( pMsg->toDest.uUid ) );

            if ( puData )
            {
               pMsgOut->uSize = pMsg->uSize;
               pMsgOut->puMsg = puData;

               memcpy ( (void *) pMsgOut->puMsg,
                        (void *) pMsg->puMsg,
                        pMsg->uSize * sizeof( BOS_UINT8 ) );
            }
         }

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = (BOS_UINT32) pMsg->toDest.uAppIx;
         evt.pDat3 = (void *) pMsgOut;
      }
      break;

      case eCMCMD_MSG_RESP:
      {
         /* Note the expected usage of the CMGRAPPCALLHDL structure to carry
         ** the information since we only need to know the GUID of the resource
         ** to send the response for (which GUID would have been given to the
         ** application when receiving the message) and the response code.
         */
         CMGRAPPCALLHDL *pData = (CMGRAPPCALLHDL *) pCmdInfo;

         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
         evt.uDat1 = pData->uSesHdl;
         evt.uDat2 = pData->uAppIx;
      }
      break;
#endif

      case eCMCMD_RESTART:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);

         if( pCmdInfo != NULL )
         {
            /* Pass user specified restart mode */
            evt.uDat1 = *( (BOS_UINT32 *) pCmdInfo );
         }
         else
         {
            /* Default restart mode */
            evt.uDat1 = (BOS_UINT32) eCCTKRSTMODE_IMMEDIATE;
         }
      }
      break;

#if CMGRCFG_DECT
      case eCMCMD_START_DIGCOLL:
      {
         CMGRSTRTDIGCOLL* pCall = (CMGRSTRTDIGCOLL*) pCmdInfo;
         CMGRSTRTDIGCOLL* pStartDigColl = (CMGRSTRTDIGCOLL *) malloc ( sizeof ( CMGRSTRTDIGCOLL ) );

         if( pCall && pStartDigColl )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pStartDigColl->uPhy = pCall->uPhy;

            evt.pDat3 = (void *)pStartDigColl;
         }
      }
      break;

      case eCMCMD_TERMINATE_CALL:
      {
         CMGRTERMCALL* pCall = (CMGRTERMCALL*) pCmdInfo;
         CMGRTERMCALL* pTermCall = (CMGRTERMCALL *) malloc ( sizeof ( CMGRTERMCALL ) );

         if( pCall && (pTermCall != NULL) )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pTermCall->uCid = (BOS_UINT32)pCall->uCid;
            evt.pDat3 = (void *)pTermCall;
         }
      }
      break;

      case eCMCMD_REJECT_CALL:
      {
         CMGRREJCALL* pCall = (CMGRREJCALL*) pCmdInfo;
         CMGRREJCALL* pRejCall = (CMGRREJCALL *) malloc ( sizeof ( CMGRREJCALL ) );

         if( pCall && pRejCall )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pRejCall->uCid = pCall->uCid;
            pRejCall->uPhy = pCall->uPhy;

            evt.pDat3 = (void *)pRejCall;
         }
      }
      break;

      case eCMCMD_SET_PREF_LINE:
      {
         CMGRSETPREFLINE* pSetPrefLine = (CMGRSETPREFLINE*) pCmdInfo;
         CMGRSETPREFLINE* pPrefLine = (CMGRSETPREFLINE *) malloc ( sizeof ( CMGRSETPREFLINE ) );

         if( pSetPrefLine && pPrefLine )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pPrefLine->uLine = pSetPrefLine->uLine;
            pPrefLine->uPhy =  pSetPrefLine->uPhy;
            evt.pDat3 = (void *)pPrefLine;
         }
      }
      break;

      case eCMCMD_TOGGLE_CALL:
      {
         CMGRTOGGLECALL* pToggle = (CMGRTOGGLECALL *) pCmdInfo;
         CMGRTOGGLECALL* pToggleCall = (CMGRTOGGLECALL *) malloc ( sizeof( CMGRTOGGLECALL ) );;

         if( pToggle && pToggleCall )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pToggleCall->uCid = pToggle->uCid;
            pToggleCall->uPhy = pToggle->uPhy;
            evt.pDat3 = (void *)pToggleCall;
         }
      }
      break;

      case eCMCMD_CONFERENCE_CALL:
      {
         CMGRCONFCALL* pConf = (CMGRCONFCALL *) pCmdInfo;
         CMGRCONFCALL* pConfCall = (CMGRCONFCALL *) malloc( sizeof( CMGRCONFCALL ) );;

         if( pConf && pConfCall )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pConfCall->uCid1 = pConf->uCid1;
            pConfCall->uCid2 = pConf->uCid2;
            pConfCall->uPhy = pConf->uPhy;
            evt.pDat3 = (void *) pConfCall;
         }
      }
      break;

      case eCMCMD_XFER_CALL:
      {
         CMGRXFERCALL* pXfer = (CMGRXFERCALL *) pCmdInfo;
         CMGRXFERCALL* pXferCall = (CMGRXFERCALL *) malloc( sizeof( CMGRXFERCALL ) );

         if( pXfer && (pXferCall != NULL) )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pXferCall->uCid1 = pXfer->uCid1;
            pXferCall->uCid2 = pXfer->uCid2;
            pXferCall->uPhy = pXfer->uPhy;
            evt.pDat3 = (void *) pXferCall;
         }
      }
      break;

      case eCMCMD_HOLD_CALL:
      {
         CMGRHOLDCALL *pHold = (CMGRHOLDCALL *) pCmdInfo;
         CMGRHOLDCALL *pHoldCall = (CMGRHOLDCALL *) malloc ( sizeof ( CMGRHOLDCALL ) );

         if( pHold && (pHoldCall != NULL) )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pHoldCall->bHold = pHold->bHold;
            pHoldCall->uCid = pHold->uCid;
            pHoldCall->uPhy = pHold->uPhy;
            evt.pDat3 = (void *)pHoldCall;
         }
      }
      break;

      case eCMCMD_INTERCEPT_CALL:
      {
         CMGRINTERCEPTCALL* pIntercept = (CMGRINTERCEPTCALL *) pCmdInfo;
         CMGRINTERCEPTCALL* pInterceptCall = (CMGRINTERCEPTCALL *) malloc( sizeof( CMGRINTERCEPTCALL ) );

         if( pIntercept && pInterceptCall )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pInterceptCall->uCid = pIntercept->uCid;
            pInterceptCall->uPhy = pIntercept->uPhy;
            pInterceptCall->uPhyOther = pIntercept->uPhyOther;
            evt.pDat3 = (void *) pInterceptCall;
         }
      }
      break;

      case eCMCMD_INTERNAL_CALL:
      {
         CMGRINTERNALCALL* pInternal = (CMGRINTERNALCALL *) pCmdInfo;
         CMGRINTERNALCALL* pInternalCall = (CMGRINTERNALCALL *) malloc( sizeof( CMGRINTERNALCALL ) );

         if( pInternal && (pInternalCall != NULL) )
         {
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            pInternalCall->uPhy = pInternal->uPhy;
            pInternalCall->uPhyRemote = pInternal->uPhyRemote;
            evt.pDat3 = (void *) pInternalCall;
         }
      }
      break;

      case eCMCMD_RELEASE_USR:
      {
         CMGRRELUSR *pRel = (CMGRRELUSR *)pCmdInfo;
         CMGRRELUSR *pRelCmd = (CMGRRELUSR *) malloc( sizeof( CMGRRELUSR ) );;

         if( pRelCmd && pRel )
         {
            pRelCmd->uPhy = pRel->uPhy;
            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            evt.pDat3 = (void *) pRelCmd;
         }
      }
      break;

      case eCMCMD_REPLACE_CALL:
      {
         CMGRREPLACECALL* pReplace = (CMGRREPLACECALL*) pCmdInfo;
         CMGRREPLACECALL* pReplaceCall;

         if( pReplace == NULL )
         {
            CMGRLOG_CRITICAL (( "%s: eCMCMD_REPLACE_CALL with NULL parameter.", __FUNCTION__ ));
            break;
         }

         pReplaceCall = (CMGRREPLACECALL*) malloc( sizeof( CMGRREPLACECALL ) );

         if( pReplace && pReplaceCall )
         {
            memcpy( pReplaceCall, pReplace, sizeof( CMGRREPLACECALL ) );

            evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
            evt.pDat3 = (void *) pReplaceCall;
         }
      }
      break;

#endif /* CMGRCFG_DECT */

      /* Any other cases, we do not need further information but the
      ** command itself.
      */
      default:
      {
         evt.uEvt = (BOS_UINT32) (CMGR_EVT_CLASS__USR_CMD | eCmd);
      }
      break;
   }

   if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
   {
      cmSignalEvt ();
   }
   else 
   {
      CMGRLOG_CRITICAL (( "cmgrCmd: failed to post 0x%lX!", eCmd ));

      if ( evt.pDat3 != NULL )
      {
         free ( evt.pDat3 );
         evt.pDat3 = NULL;
      }
   }
}

