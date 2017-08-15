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
**      This module implements the finite state machine that runs the Call Manager
**      application.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMGR_SINGLE_REMAINING_SESSION        1

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFsmRun
**
** PURPOSE:    Main entry for running the FSM.
**
** PARAMETERS: eType - the FSM type to run.
**             pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRun ( CMFSMTYPE eType, void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   BOS_UINT32 uSta;
   BOS_UINT32 uEvt;

   uSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
   uEvt = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ];

   /* Run the proper FSM for the given resource.
   **
   ** Note that there is no real reason to split out into different FSMs,
   ** but it makes things a little more organized and hopefully easy to follow.
   */
   switch ( eType )
   {
      case eCMFSM_CCTK:
      {
          cmFsmCctkRun ( pRes );
      }
      break;

      case eCMFSM_PHY:
      {
          cmFsmPhyRun ( pRes );
      }
      break;

      case eCMFSM_TMR:
      {
          cmFsmTmrRun ( pRes );
      }
      break;

      default:
      {
         CMGRLOG_WARNING (( "cmFsmRun: usr 0x%X invalid FSM: %d (%d, %d)",
                            pUsr->uHdl, eType, uSta, uEvt ));
      }
      break;
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmInvalid
**
** PURPOSE:    FSM invalid state/event function prototype.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInvalid ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_WARNING (( "cmFsmInvalid: 0x%X - [%s (%d), %s (%d)]",
                      pUsr->uHdl,
                      (char *) cmMapGetStr ( cmMapCmState, pUsr->eSta[0] ),
                      pUsr->eSta[0],
                      (char *) cmMapGetStr ( cmMapCmEvent, pUsr->eEvt[0][0] ),
                      pUsr->eEvt[0][0] ));

   CMGR_REMOVE_UNUSED_PARAM_WARNING( pUsr );
}


/***********************************************************************************
** FUNCTION:   cmFsmNoOp
**
** PURPOSE:    FSM no-operation state/event function prototype.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNoOp ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_DEBUG (( "cmFsmNoOp: 0x%X - [%s (%d), %s (%d)]",
                    pUsr->uHdl,
                    (char *) cmMapGetStr ( cmMapCmState, pUsr->eSta[0] ),
                    pUsr->eSta[0],
                    (char *) cmMapGetStr ( cmMapCmEvent, pUsr->eEvt[0][0] ),
                    pUsr->eEvt[0][0] ));

   CMGR_REMOVE_UNUSED_PARAM_WARNING( pUsr );
}


/***********************************************************************************
** FUNCTION:   cmFsmCallEnd
**
** PURPOSE:    FSM call terminated handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnd ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pOtherSes = NULL;
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );
   BOS_UINT32 uIx;
   BOS_UINT32 uCnt;
   CMTMR tmr;
   CMGREPSIG sig;
   BOS_BOOL bCfv = BOS_FALSE;
   BOS_BOOL bHeld = BOS_FALSE;
   BOS_BOOL bEndFocus = BOS_FALSE;
   BOS_BOOL bCntReset = BOS_FALSE;
   BOS_BOOL bBargedIn = BOS_FALSE;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 uConfHdl = CMGR_INVALID_HANDLE;

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmCallEnd: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

#if CMGRCFG_REMOTE_USER_CONTROL
      if (pUsr->eType != eCMUSRTYPE_REMOTE)
      {
         /* Cancel call leg towards possible 'other' remote users.
         */
         cmFeatProxyCalleeEnd ( pUsr );
      }
#endif

      /* If we were running the on-hook timer for this session, terminate
      ** it now since the session is now terminated from the remote end.
      */
      tmr.eType = eCMEVT_TMR_ONHOOK;
      tmr.uUsr = pUsr->uHdl;
      tmr.uSes = pSes->uHdl;
      if ( cmTmrIsRunning ( &tmr ) )
      {
         cmTmrStop ( &tmr );
      }

      /* Save the information whether this session was a call forwarding
      ** variable activation that succeeded (got connected) as it may be
      ** used later on to define how to behave next.
      */
      bCfv = ( pSes->bEstab && pSes->ses.med.bCfv );

      /* Save the information whether we are ending an held call.
      */
      bHeld = pSes->ses.med.bHeld;

      /* Save the information whether we are ending the focus call.
      */
      bEndFocus = ( pFocusSes && ( pFocusSes == pSes ) );

      /* Save the information whether we are ending a conference call.
      */
      uConfHdl = pSes->ses.med.uConf;

      /* Save the information whether we are ending a call which other
      ** users have barged into.
      */
      bBargedIn = ( cmNumSesBargedIn ( pSes->uHdl ) > 0 ) ? BOS_TRUE : BOS_FALSE;

      /* If this session is established and is the last session and isn't
      ** an internal call.
      */
      if ( ( pSes->bEstab ) && ( cmNumSesWithCid( pSes->uNet ) == 1 ) &&
           ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
      {
         CCTKCALLMETRICS stats;
         BOS_UINT8 cPubTo[CMGR_IP_ADDRESS_STRING_LEN+1];

         CFGEPTCODECCFG *pCodecCfg = NULL;


         memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );

         cPubTo[0] = '\0';
         cfgIdx.uIndex = pUsr->uPhys;
         cfgIdx.pcCharIdx = pUsr->uUid;
         if ( gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                    &cfgIdx,
                                    (void *) &pCodecCfg,
                                    CFG_ITEM_GET ) == BOS_STATUS_OK )
         {
            if ( pCodecCfg )
            {
               if (pCodecCfg->pcPubReportAddr)
               {
                  strcpy((char*)cPubTo, (char*)pCodecCfg->pcPubReportAddr);
               }
            }

            gCmCfgCb.cmgrProvCb ( CFGITEM( EPT, CodecConcept ),
                                  &cfgIdx,
                                  (void *) pCodecCfg,
                                  CFG_ITEM_DEL );
            pCodecCfg = NULL;
         }

         if (strlen((char*)cPubTo))
         {
            BOS_UINT8 ipAddr[CMGR_IP_ADDRESS_STRING_LEN + 7];
            BOS_IP_ADDRESS pubTo;
            BOS_STATUS status;

            status = bosIpAddrCreateFromStr( (char *)&cPubTo, &pubTo);
            if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
            {
               sprintf ( (char *)ipAddr, "sip:[" );
            }
            else
            {
               sprintf ( (char *)ipAddr, "sip:" );
            }

            strcpy((char*)&ipAddr[strlen( (const char *) ipAddr )], (char*)cPubTo);

            if ( (status == BOS_STATUS_OK) && bosIpAddrIsV6( &pubTo ) )
            {
               strcat ( (char *)ipAddr, "]" );
            }

            /* Gather and publish the statistic information for this call.
            */
            cmSesGetStats ( pSes, (void *) &stats );
            stats.uUserGuid = pSes->uNetUsr;
            cctkSetParm ( pSes->uNet,
                          eCCTKPARM_EC_CALLSTATS,
                          (void *) &stats );

            cctkPublish ( stats.uUserGuid,
                          pSes->uNet,
                          eCCTKPUBLISH_CALLMETRICS,
                          ipAddr );
         }
      }

      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_ENDED,
                                  pSes->uNet,
#if CMGRCFG_MULTILINE
                                  pSes->uUsr,
                                  (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_MULTILINE */
                                  (void *) pUsr->uUid );
      }

      /* If the session that ended was the one waiting for this user, clear the
      ** session waiting information as well.
      */
      if ( pUsr->pSesWait && ( pUsr->pSesWait == pSes ) )
      {
         CMGREPSIG sig;
#if CMGRCFG_MULTILINE
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

         memset ( &sig, 0, sizeof ( CMGREPSIG ) );

         pUsr->pSesWait = NULL;

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_CALLWAIT;

         cmTmrStop ( &tmr );

         /* End the call-waiting tone if it's still playing.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uSig = pUsr->uCpt;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );
      }

      /* If the session that ended was the one held for this user, clear the
      ** session held information as well.
      */
      if ( pUsr->pSesHeld && ( pUsr->pSesHeld == pSes ) )
      {
         pUsr->pSesHeld = NULL;
         pUsr->bPresrvHeld = BOS_FALSE;
      }

      /* If the session that ended had sessions barged into it, then
      ** clear those as well.
      */
      if ( bBargedIn )
      {
         CMSESLIST sesList;

         memset( &sesList, 0, sizeof( CMSESLIST ) );

         /* Find the barged-in sessions and release them
         */
         cmSesListWithBargeIn( pSes->uHdl, &sesList );
         cmSesListRelease( &sesList, pSes );
      }


      cmSesPhyDelete ( pSes );
      cmUsrUnLinkSes ( pUsr, pSes );
      /* Reset the network handle and free the session resource.
      */
      pSes->uNet = CMGR_INVALID_HANDLE;
      cmSesFree ( pSes, pUsr->uHdl );
   }

   /* Update the state and processing based on the status of the remaining
   ** media session associated with this user.
   */
   uCnt = 0;
   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] != NULL )
      {
         uCnt++;
         pOtherSes = pUsr->pMedSes [ uIx ];
      }
   }

   CMGRLOG_DEBUG(( "%s: pSes: 0x%X, uCnt: %X, pOtherSes: 0x%X, pSesWait: 0x%X, pSesHeld: 0x%X\n",
            __FUNCTION__,
            (unsigned int)pSes,
            uCnt,
            (unsigned int)pOtherSes,
            (unsigned int)pUsr->pSesWait,
            (unsigned int)pUsr->pSesHeld ));

   if ( uCnt )
   {
      switch ( uCnt )
      {
         case CMGR_SINGLE_REMAINING_SESSION:
         {
            /* Only remaining session is the call waiting one.
            */
            if ( pUsr->pSesWait )
            {
               /* End the call waiting timer as the session becomes either
               ** a ringing call or an 'on-hold' waiting for local on-hook.
               */
               tmr.uSes = pUsr->pSesWait->uHdl;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_CALLWAIT;

               cmTmrStop ( &tmr );

               /* If we have a special ring reminder tone to use for this session
               ** alerting (either now or later), make sure we keep that data
               ** handy.
               */
               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                          &cfgIdx,
                                          (void *) &pBasicCall,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  if ( pBasicCall && pBasicCall->pHoldRemTone )
                  {
                     strcpy ( (char *) pUsr->pSesWait->ses.med.callInfo.cAlertInfo[ 0 ],
                              (const char *) pBasicCall->pHoldRemTone );
                  }

                  gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                        &cfgIdx,
                                        (void *) pBasicCall,
                                        CFG_ITEM_DEL );
                  pBasicCall = NULL;
               }

               /* If on-hook, ring the phone for the new incoming call right
               ** away.
               */
               if ( !pUsr->bOffHook )
               {
                  pUsr->pSesWait->ses.med.bFocus = BOS_TRUE;

                  /* No longer a call waiting session.
                  */
                  pUsr->pSesWait = NULL;

                  cmFsmRing ( pRes );
               }
               /* If off-hook, continue with off-hook termination processing as if
               ** there was not left over call to deal with.
               */
               else
               {
                  if ( pUsr->pSesWait->bEstab )
                  {
#if CMGRCFG_DECT
                     /* CAT-iq requires that if the active call is terminated, the held calls
                     ** will remain held and will be available to be unheld in the future
                     */
                     if (uConfHdl != pOtherSes->uHdl)
                     {
                        /* Set to flash state */
                        cmUsrNewState ( pUsr, eCMST_FLASH ); 
                     }
                     else
                     {
                        /* end the conference call */
                        cmUsrNewState ( pUsr, eCMST_TALK );
                        if ( gCmCfgCb.cmgrEvtCb )
                        {
                           CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pOtherSes->uNetUsr );
                           ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                           pOtherSes->uNet,
                                           pOtherSes->uUsr,
                                           (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
                                           (void *) pUsr->uUid );
                        }
                     }
#else
                     CFGSIPCALLWAIT *pUsrCallWait = NULL;

                     cmUsrNewState ( pUsr, eCMST_TALK );

                     pUsr->pSesWait->ses.med.bFocus = BOS_TRUE;

                     /* start the auto recovery timeout, 10 seconds*/
                     memset ( &tmr, 0, sizeof ( CMTMR ) );
                     tmr.uTimeOut = CMGR_AUTO_RECOVER_TIMEOUT;

                     memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                     cfgIdx.pcCharIdx = pUsr->uUid;
                     if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept),
                                                &cfgIdx,
                                                (void *) &pUsrCallWait,
                                                CFG_ITEM_GET ) == BOS_STATUS_OK )
                     {
                        if ( pUsrCallWait )
                        {
                           tmr.uTimeOut = pUsrCallWait->uCallWaitDisconnectTiming;
                        }

                        gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept),
                                              &cfgIdx,
                                              (void *) pUsrCallWait,
                                              CFG_ITEM_DEL );
                        pUsrCallWait = NULL;
                     }

                     if ( tmr.uTimeOut == 0 )
                     {
                        /* Restore the audio path to the session waiting.
                         */
                        if( pUsr->pSesWait->uNetUsr != CMGR_INVALID_HANDLE )
                        {
                           cctkCallUnhold ( pUsr->pSesWait->uNet );
                        }
                        else
                        {
                           /* Send an unhold ok event */
                           CMEVT evt;

                           memset ( &evt, 0, sizeof( CMEVT ) );

                           evt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                           evt.uDat1 = pUsr->uHdl;           /* User handle */
                           evt.uDat2 = pUsr->pSesWait->uHdl; /* Session handle */
                           evt.uDat4 = CMGR_INVALID_HANDLE;  /* CCTK user id */
                           evt.bLast = BOS_TRUE;             /* Last event */

                           if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
                           {
                              cmSignalEvt ();
                           }
                        }
                     }
                     else
                     {
                        /* Make sure flash is handled during auto recovery in this case */
                        pUsr->pSesWait->ses.med.eAutoRec = eCMSESAUTOREC_LCL;

                        /* if this is not a release from the remote end, play reorder
                         * tone for the duration of auto recovery
                         * */
                        if ( pUsr->uCpt == eEPTSIG_NULL &&
                              (( pUsr->evt.uEvt & CMGR_EVT_CLASS__EVT_MASK )
                               != eCMEVT_RELEASE_CALL ))
                        {
                           memset ( &sig, 0, sizeof ( CMGREPSIG ) );
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
                        }

                        /* and start the timer */
                        tmr.uSes = CMGR_INVALID_HANDLE;
                        tmr.uUsr = pUsr->uHdl;
                        tmr.eType = eCMEVT_TMR_AUTORECOVER;

                        cmTmrStart ( &tmr );
                     }
#endif /* CMGRCFG_MULTILINE */

                     /* There is no call waiting session any longer at this time.
                      */
                     pUsr->pSesWait = NULL;
                  }
                  else
                  {
                     /* This is call replacement scenario, so we will asnswer the waiting call */
                     CMSES *pCwSes = pUsr->pSesWait; 
                     CMSESLIST sesList;
#if CMGRCFG_DECT
                     CMCCTKACCNT* pCctkAccnt = NULL;
#endif /* CMGRCFG_DECT */
                     CMGRLOG_INFORMATION ((
                                  "%s: eCMCMD_REPLACE_CALL: accept call waiting 0x%X (0x%X)",
                                  __FUNCTION__, pCwSes->uHdl, pCwSes->uNet ));

                     memset ( &sesList, 0, sizeof ( CMSESLIST ) );
                     memset ( &sig, 0, sizeof ( CMGREPSIG ) );
                     memset ( &tmr, 0, sizeof ( CMTMR ) );

                     /* Replace the current call with the waiting session. */
                     pUsr->pSesWait = NULL;
                     pCwSes->ses.med.bFocus = BOS_TRUE;

                     tmr.uSes = pCwSes->uHdl;
                     tmr.uUsr = pUsr->uHdl;
                     tmr.eType = eCMEVT_TMR_CALLWAIT;

                     cmTmrStop ( &tmr );

#if CMGRCFG_DECT
                     pCctkAccnt = cmCctkAccntFindByNet( pCwSes->uNetUsr );
#endif /* CMGRCFG_DECT */

                     /* Initial answer of call waiting. */

                     if ( pCwSes->ses.med.uPhy != CMGR_INVALID_HANDLE )
                     {
                        /* We would be playing ringback to the peer. Terminate it.
                        */
                        sig.eAct = eCMGREPSIGACT_STOP;
                        sig.uSes = pCwSes->ses.med.uPhy;
                        sig.uCid = pCwSes->uNet;
#if CMGRCFG_DECT
                        sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_DECT */
                        sig.uApp = CMGR_INVALID_HANDLE;
                        sig.uSig = eEPTSIG_RINGBACK;

                        cmPhySignal ( pUsr->uPhys, &sig );
                     }

                     pCwSes->bEstab = BOS_TRUE;
#if CMGRCFG_DECT
                     if( pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        cmCctkAccntCallAdd( pCwSes->uNetUsr );
                     }
#endif

                     /* Notify the application that the call has connected */
                     if ( gCmCfgCb.cmgrEvtCb )
                     {
                        ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                                 pCwSes->uNet,
#if CMGRCFG_DECT
                                                 pUsr->uHdl,
                                                 (pCctkAccnt ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE),
#endif /* CMGRCFG_DECT */
                                                 (void *) pUsr->uUid );
                     }

                     /* If we need to finalize the media capabilities, do it now
                     ** prior to call answer.
                     */
                     if ( pCwSes->ses.med.bNeedFinal &&
                          pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        pCwSes->ses.med.bNeedFinal = BOS_FALSE;
                        cctkCallMediaNotify( pCwSes->uNet,
                                             eCCTKMUDPRSN_FINALIZECAPS );
                     }

                     if( pCwSes->uNetUsr != CMGR_INVALID_HANDLE )
                     {
                        cmSesListWithCid( pCwSes->uNet, &sesList );
                        for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
                        {
                           CMSES* pSesRelease = cmSesValid( sesList.uSes[ uIx ] );
                           CMUSR* pUsrRelease = (pSesRelease) ? cmUsrValid( pSesRelease->uUsr ) : NULL;

                           if( ( pSesRelease ) && ( pUsrRelease ) && ( pSesRelease->uHdl != pCwSes->uHdl ) )
                           {
                              CMEVT cmEvt;

                              memset( &cmEvt, 0, sizeof( CMEVT ) );

                              cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                              cmEvt.uDat1 = pSesRelease->uUsr;
                              cmEvt.uDat2 = pSesRelease->uHdl;
                              cmEvt.uDat4 = pSesRelease->uNetUsr;

                              /* Take care of any specific data coming with the event that need
                              ** to be saved.
                              */
                              memcpy ( &pUsrRelease->evt, &cmEvt, sizeof ( CMEVT ) );

                              /* Run the state machine.
                              */
                              cmFsmCallEnding( pUsrRelease );
                           }
                        }
                        cctkCallAnswer ( pCwSes->uNet );
                     }
                     else
                     {
                        /* If any other sessions exist with this call id, we need to
                        ** perform some action:
                        ** [1] - if the session is not yet established, we should
                        **       free it since we just took the call here.
                        ** [2] - if the session is established, this means it was the
                        **       originator of an internal call. We should send it a
                        **       "CONNECT_CALL" event to finish setting up the call.
                        */
                        cmSesListWithCid( pCwSes->uNet, &sesList );
                        for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
                        {
                           CMSES* pSesClosing;
                           CMUSR* pUsrClosing;
                           CMEVT cmEvt;

                           /* Release the call on all other associated sessions */
                           if( sesList.uSes[ uIx ] == pCwSes->uHdl )
                           {
                              continue;
                           }

                           pSesClosing = cmSesValid( sesList.uSes[ uIx ] );
                           pUsrClosing = cmUsrValid( pSesClosing->uUsr );

                           /* If the session is already "established", that means that the user is in
                           ** the call on this session. The only time this can happen is if that user
                           ** is the one initiating the call, in which case we don't want to tear down
                           ** its session.
                           */
                           if( !pSesClosing->bEstab )
                           {
                              cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                              cmEvt.uDat1 = pUsrClosing->uHdl;
                              cmEvt.uDat2 = pSesClosing->uHdl;
                              cmEvt.uDat4 = pSesClosing->uNetUsr;

                              /* Take care of any specific data coming with the event that need
                              ** to be saved.
                              */
                              memcpy ( &pUsrClosing->evt, &cmEvt, sizeof ( CMEVT ) );

                              /* Run the state machine right away.
                              */
                              cmFsmCallEnding( pUsrClosing );
                           }
                           /* If the session is already established, the user is the originator of
                           ** the call.
                           */
                           else
                           {
                              CCTKCALLINFO *pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );

                              memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

                              /* Internal CLID is handled in cmSesClid, so no need to
                              ** fill anything in here */
                              pCallInfo->bEmergency = BOS_FALSE;
                              pCallInfo->bLoopBack = BOS_FALSE;

                              cmEvt.uEvt = ( eCMEVT_CONNECT_CALL | CMGR_EVT_CLASS__CCTK );
                              cmEvt.uDat1 = pUsrClosing->uHdl;
                              cmEvt.uDat2 = pSesClosing->uHdl;
                              cmEvt.pDat3 = pCallInfo;
                              cmEvt.uDat4 = pSesClosing->uNetUsr;

                              /* Post the message */
                              if( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                              {
                                 cmSignalEvt();
                              }
                              else
                              {
                                 CMGRLOG_CRITICAL(( "%s: cmPostEvt failed. Call cannot be established.", __FUNCTION__ ));
                              }
                           }
                        } /* for each session */
                     } /* internal call */

                     /* Generate an internal media event to setup the session properly
                     ** now. */
                     pUsr->evt.uDat2 = pCwSes->uHdl;
                     cmFsmMedia ( pUsr );
                  }
               }
            }
            /* Only remaining session is the call held one.  Play reorder to the
            ** user in this case and await for action.  User can hook flash now and
            ** retrieve the held call automatically without having to key in the
            ** service code.
            */
            else if ( pUsr->pSesHeld )
            {
               if ( pUsr->bOffHook )
               {
                  /* If the call was ended by the remote party.
                  */
                  if ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ]
                                       [ CMFSM_CURRENT_EVENT_IX  ] ==
                                                      eCMEVT_RELEASE_CALL )
                  {
                     /* Move to 'flash' state, play special (recall) dial tone,
                     ** wait for digit input (start timer).
                     */
                     pUsr->eDigDetState = CMDIGITDETSTA_NONE;
                     pUsr->uDigCnt = 0;
                     memset ( pUsr->uDigCol, 0, sizeof ( pUsr->uDigCol ) );

                     /* Reset the internal call detection flag
                     */
                     pUsr->bIntCallDet = BOS_FALSE;

                     cmUsrNewState ( pUsr, eCMST_FLASH );

                     /* Reset the digit collection action information.
                     */
                     cmDmReset ( (CMDMACTIONSTATE *)pUsr->pDmState,
                                 CMDM_ACTION_BUFFER_SIZE );

                     sig.eAct = eCMGREPSIGACT_START;
                     sig.uSes = CMGR_INVALID_HANDLE;
                     sig.uSig = eEPTSIG_SPECIAL_DIAL;
                     sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                     sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                     pUsr->uCpt = sig.uSig;

                     cmPhySignal ( pUsr->uPhys, &sig );

                     tmr.uUsr = pUsr->uHdl;
                     tmr.uSes = CMGR_INVALID_HANDLE;
                     tmr.eType = eCMEVT_TMR_DIALTONE;

                     memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                     cfgIdx.pcCharIdx = pUsr->uUid;
                     if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                                &cfgIdx,
                                                (void *) &pBasicCall,
                                                CFG_ITEM_GET ) == BOS_STATUS_OK )
                     {
                        if ( pBasicCall )
                        {
                           tmr.uTimeOut = pBasicCall->uOrigDTTimer;
                        }

                        gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                              &cfgIdx,
                                              (void *) pBasicCall,
                                              CFG_ITEM_DEL );
                        pBasicCall = NULL;
                     }
                     else
                     {
                        tmr.uTimeOut = CMGR_DIAL_TONE_TIMEOUT;
                     }

                     cmTmrStart ( &tmr );
                  }
                  /* If the call was ended for other reason (dial error, timeout,
                  ** call imcomplete, etc)...
                  */
                  else
                  {
                     cmUsrNewState ( pUsr, eCMST_RELEASING );

                     sig.eAct = eCMGREPSIGACT_START;
                     sig.uSes = CMGR_INVALID_HANDLE;
                     sig.uSig = eEPTSIG_REORDER;
                     sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                     sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                     pUsr->uCpt = sig.uSig;

                     cmPhySignal ( pUsr->uPhys, &sig );

                     tmr.uUsr = pUsr->uHdl;
                     tmr.uSes = CMGR_INVALID_HANDLE;
                     tmr.eType = eCMEVT_TMR_OHWARN;

                     memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                     cfgIdx.pcCharIdx = pUsr->uUid;
                     if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                                &cfgIdx,
                                                (void *) &pBasicCall,
                                                CFG_ITEM_GET ) == BOS_STATUS_OK )
                     {
                        if ( pBasicCall )
                        {
                           tmr.uTimeOut = pBasicCall->uTermErrSigTimer;
                        }
                        else
                        {
                           tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;
                        }

                        gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                              &cfgIdx,
                                              (void *) pBasicCall,
                                              CFG_ITEM_DEL );
                        pBasicCall = NULL;
                     }
                     else
                     {
                        tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;
                     }

                     cmTmrStart ( &tmr );
                  }
               }
               else
               {
                  pUsr->pSesHeld->ses.med.bFocus = BOS_TRUE;

                  cmFsmRing ( pRes );
               }
            }
            else
            {
               /* Make sure this is marked as the focus session since it is the
               ** remaining one.
               */
               pOtherSes->ses.med.bFocus = BOS_TRUE;

               /* If we ended the focus session, and we still have an active session
               ** that was not in conference with this session, apply auto-recovery
               */
               if ( pUsr->bOffHook && bEndFocus &&
                     (uConfHdl != pOtherSes->uHdl))
               {
#if CMGRCFG_DECT
                  /* CAT-iq requires that if the active call is terminated, the held calls
                  ** will remain held and will be available to be unheld in the future
                   */

                  /* Set to flash state */
                  cmUsrNewState ( pUsr, eCMST_FLASH );
#else

                  CMTMR tmr;
                  CFGSIPTHREEWAYCALL *pUsr3WayCall = NULL;

                  memset ( (void *) &tmr,
                           0,
                           sizeof ( CMTMR ) );

                  tmr.uTimeOut = CMGR_AUTO_RECOVER_TIMEOUT;
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  cfgIdx.pcCharIdx = pUsr->uUid;
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept),
                                             &cfgIdx,
                                             (void *) &pUsr3WayCall,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pUsr3WayCall )
                     {
                        tmr.uTimeOut = pUsr3WayCall->uCallDisconnectTiming;
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, ThreeWayCallConcept),
                                           &cfgIdx,
                                           (void *) pUsr3WayCall,
                                           CFG_ITEM_DEL );
                     pUsr3WayCall = NULL;
                  }

                  cmUsrNewState ( pUsr, eCMST_TALK );

                  if ( tmr.uTimeOut == 0 )
                  {
                     /* Restore the audio path to the session that is pending.
                      */
                     if (pOtherSes->uNetUsr != CMGR_INVALID_HANDLE)
                     {
                        cctkCallUnhold ( pOtherSes->uNet );
                     }
                     else
                     {
                        /* Send an unhold ok event */
                        CMEVT evt;

                        memset ( &evt, 0, sizeof( CMEVT ) );

                        evt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
                        evt.uDat1 = pOtherSes->uUsr;      /* User handle */
                        evt.uDat2 = pOtherSes->uHdl;      /* Session handle */
                        evt.uDat4 = pOtherSes->uNetUsr;;  /* CCTK user id */
                        evt.bLast = BOS_TRUE;             /* Last event */

                        if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
                        {
                           cmSignalEvt ();
                        }
                     }
                  }
                  else
                  {
                     CMGREPSIG sig;

                     memset ( (void *) &sig,
                              0,
                              sizeof ( CMGREPSIG ) );

                     /* Play the reorder if no other tone is being played and
                     ** if this is not a realease from the remote end.
                     */
                     if ( pUsr->uCpt == eEPTSIG_NULL &&
                           (( pUsr->evt.uEvt & CMGR_EVT_CLASS__EVT_MASK )
                                                         != eCMEVT_RELEASE_CALL ))
                     {
                        sig.eAct = eCMGREPSIGACT_START;
                        sig.uSes = CMGR_INVALID_HANDLE;
                        sig.uApp = CMGR_INVALID_HANDLE;
                        sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                        sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                        pUsr->uCpt = eEPTSIG_REORDER;
                        sig.uSig = pUsr->uCpt;

                        cmPhySignal ( pUsr->uPhys, &sig );
                     }

                     tmr.uSes = CMGR_INVALID_HANDLE;
                     tmr.uUsr = pUsr->uHdl;
                     tmr.eType = eCMEVT_TMR_AUTORECOVER;

                     cmTmrStart ( &tmr );

                     /* Keep track of the auto-recovery sub-state.
                     */
                     pOtherSes->ses.med.eAutoRec = eCMSESAUTOREC_LCL;
                  }
               }
               /* If we ended the focus session, and it was in conference with the
               ** the remaining session force the most appropriate state.
               */
               else if( pUsr->bOffHook && bEndFocus )
               {
                     cmUsrNewState ( pUsr, eCMST_TALK );
#endif /* CMGRCFG_MULTILINE */
               }
            }

            /* no longer need to wait for DTMF digit for Register Recall */
            pUsr->rrFlash.bRRWaitKey = BOS_FALSE;
         }
         break;

         default:
         {
            /* If we ended the focus session, force the most appropriate state,
            ** otherwise do not change the current state.
            */
            if ( bEndFocus )
            {
               cmUsrNewState ( pUsr, eCMST_TALK );
            }
         }
         break;
      }
   }

   /* Note that the counter value may have been reset during the process
   ** of dealing with the call remaining in order to force application of
   ** the following behavior.
   */
   if ( !uCnt &&
        ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_ERROR ) )
   {
      /* Terminate usage of the user resources.
      */
      cmUsrNewState ( pUsr,
                      bCntReset ? eCMST_RELEASING : eCMST_IDLE );

      /* Signal reorder in case necessary...
      */
      if ( pUsr->bOffHook )
      {
         if ( bCfv && !bCntReset )
         {
            /* Be ready to place a new call right away.
            */
            cmFsmOutgoingCall( pRes );
         }
         else
         {
            CMGREPSIG sig;
            CMTMR tmr;
            BOS_BOOL bReorder = BOS_TRUE;

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );
            memset ( &tmr, 0, sizeof ( CMTMR ) );

            /* If we are configured to do so, issue a network disconnect
            ** signal.
            */
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pBasicCall && pBasicCall->bIssueNetDisc )
               {
                  /* If we are already playing busy tone, we would have issued the
                  ** OSI first if needed, in such case bypass this behavior.
                  */
                  if ( pUsr->uCpt != eEPTSIG_BUSY )
                  {
                     /* Apply configuration delay prior to sending the
                     ** OSI signal.
                     */
                     if ( pBasicCall->uNetDiscDelay )
                     {
                        tmr.uSes = CMGR_INVALID_HANDLE;
                        tmr.uUsr = pUsr->uHdl;
                        tmr.eType = eCMEVT_TMR_OSI_DELAY;
                        tmr.uTimeOut = pBasicCall->uNetDiscDelay;

                        cmTmrStart ( &tmr );
                     }
                     /* Issue the OSI signal right away unless we already did
                     ** so (in case of busy, the OSI is sent right away before the
                     ** tone is started if needed).
                     */
                     else
                     {
                        cmFsmOsi ( pRes );
                     }
                  }
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                     &cfgIdx,
                                     (void *) pBasicCall,
                                     CFG_ITEM_DEL );
               pBasicCall = NULL;
            }

            /* Play reorder if:
            **
            **   1) We are not already playing busy tone.
            **   2) This is a release from local end.
            **   3) This is a release from remote end of an held call.
            */
            if ( ( ( pUsr->uCpt == eEPTSIG_BUSY ) ||
                   ( !bHeld && ( pUsr->evt.uEvt & CMGR_EVT_CLASS__EVT_MASK )
                                                      == eCMEVT_RELEASE_CALL ) ) )
            {
               bReorder = BOS_FALSE;
            }

            if ( bReorder )
            {
               sig.eAct = eCMGREPSIGACT_START;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
               sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
               /* If a tone was specified, use it now, otherwise use the
               ** default tone handler.
               */
               if ( pUsr->uCfgTone != CMGR_INVALID_HANDLE )
               {
                  sig.uSig = pUsr->uCfgTone;
                  pUsr->uCfgTone = CMGR_INVALID_HANDLE;
               }
               else
               {
                  sig.uSig = eEPTSIG_REORDER;
               }
               pUsr->uCpt = sig.uSig;

               cmPhySignal ( pUsr->uPhys, &sig );
            }
            else
            {
               if( pUsr->uCpt == eEPTSIG_RINGBACK )
               {                  
                  if ( pSes->uNetUsr == CMGR_INVALID_HANDLE )
                  {
                     /* If this was an internal call play reorder instead
                     ** since a callend during rinback now indicates
                     ** something being wrong in the system
                     */
                     pUsr->uCpt = eEPTSIG_REORDER;
                     sig.uSig = pUsr->uCpt;   
                     sig.eAct = eCMGREPSIGACT_START;
                  }
                  else
                  {                     
                     /* Just end the ringback
                     **/
                     sig.uSig = pUsr->uCpt;   
                     pUsr->uCpt = eEPTSIG_NULL;
                     sig.eAct = eCMGREPSIGACT_STOP;                     
                  }                                         
                                    
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            
                  tmr.uSes = pSes->uHdl;
                  tmr.uUsr = pUsr->uHdl;
                  tmr.eType = eCMEVT_TMR_RINGBACK;
            
                  cmTmrStop ( &tmr );
            
                  cmPhySignal ( pUsr->uPhys, &sig );
               }
            }

            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.uUsr = pUsr->uHdl;
            tmr.eType = eCMEVT_TMR_OHWARN;
            tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

            cmTmrStart ( &tmr );
         }
      }
      /* Release the physical resource allocated to this user now that we
      ** do not need it for anything.
      */
      else
      {
         /* Only release resources if it has not previously been released.
         ** If the resources have been released earlier, the "line inactive"
         ** event would have been reported earlier so no need to report it
         ** here again.
         */
         if ( pUsr->uPhys != CMGR_INVALID_HANDLE )
         {
            /* Publish a 'line inactive' event.
            */
            if ( gCmCfgCb.cmgrEvtCb &&
                 (pUsr->eType != eCMUSRTYPE_REMOTE) )
            {
               gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                                    pUsr->uPhys,
#if CMGRCFG_MULTILINE
                                    pUsr->uHdl,
                                    CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                    NULL );
            }

            /* If we are pending on an action, don't release
            ** the resource.
            */
            if ( pUsr->bPendingActPhy == BOS_FALSE )
            {
               cmUsrPhyDisassociate( pUsr );
               cmUsrCctkAccntDisassociate( pUsr );
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDigColAction
**
** PURPOSE:    FSM generic handle to process an action as the result of digit
**             collection.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigColAction ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMDMACTIONTYPE eAction;
   BOS_BOOL bRet = BOS_TRUE;

   while ( ( cmDmGetAction ( (CMDMACTIONSTATE *)pUsr->pDmState,
                              &eAction ) == eCMDM_SUCCESS ) &&
           bRet )
   {
      bRet = cmFeatApplyAction( pRes, eAction );
   };
}


/***********************************************************************************
** FUNCTION:   cmFsmFeatAction
**
** PURPOSE:    FSM specific handle to process an action as the result of digit
**             collection while waiting for a feature execution.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       This function is only invoked as the result of a 'hook flash'
**             followed by some digit collection or a timeout.
***********************************************************************************/
void cmFsmFeatAction ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = NULL;
   CMDMACTIONTYPE eAction;
   BOS_BOOL bRet = BOS_TRUE;

   while ( ( cmDmGetAction ( (CMDMACTIONSTATE *)pUsr->pDmState,
                              &eAction ) == eCMDM_SUCCESS ) &&
           bRet )
   {
      /* Catch all case(s) prior to going further...
      **
      **   - If the only allowed action is 'eCMDM_CWTOGGLE' but
      **     that is not what we see here, apply default escape
      **     behavior (action deny tone, and resume call).
      */
      if ( cmFeatFlashCwOnly ( pRes ) &&
           ( eAction != eCMDM_CWTOGGLE ) )
      {
         /* Apply 'escape action' processing to the current action.
         */
         cmFeatFlashEscAct ( pRes );

         /* Skip the action processing in the following switch block.
         */
         eAction = eCMDM_UNKNOWNACTION;
         /* Terminate any further action processing.
         */
         bRet = BOS_FALSE;
      }

      /* Apply the action as necessary.
      */
      switch ( eAction )
      {
         case eCMDM_HOLDACTIVATE:
         {
            pSes = cmUsrActiveSes ( pUsr );
            /* Note: do not allow emergency session to be put on hold by
            **       the local user action.
            */
            if ( pSes &&
                 !pSes->ses.med.bEmergency &&
                 cmFeatEnabled ( pRes, eAction ) )
            {
               bRet = cmFeatAccept ( pRes, eAction );
            }
            else
            {
               cmFeatReject ( pRes, eAction );
               /* Terminate any further action processing.
               */
               bRet = BOS_FALSE;
            }
         }
         break;

         case eCMDM_REORDER:
         {
            CMGREPSIG sig;

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uSig = eEPTSIG_REORDER;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }
         break;

         /* Need to create a new session resource for this action and then apply
         ** to it the action in question.
         */
         case eCMDM_MAKECALL:
         case eCMDM_MAKECALLINT:
         case eCMDM_ACRACTIVATE:
         case eCMDM_ACRDEACTIVATE:
         case eCMDM_ACACTIVATE:
         case eCMDM_ACDEACTIVATE:
         case eCMDM_ARACTIVATE:
         case eCMDM_ARDEACTIVATE:
         case eCMDM_CIDSUPPRESS:
         case eCMDM_CIDDELIVER:
         case eCMDM_CNDBTOGGLE:
         case eCMDM_COTACTIVATE:
         case eCMDM_CWTOGGLE:
         case eCMDM_EMERGENCYCALL:
         case eCMDM_RECALL:
         {
            CMTMR tmr;

            memset ( &tmr, 0, sizeof ( CMTMR ) );

            /* The long interdigit timer may still be running if we came here
            ** because of a short interdigit timer expiry.
            */
            tmr.uUsr = pUsr->uHdl;
            tmr.uSes = CMGR_INVALID_HANDLE;
            tmr.eType = eCMEVT_TMR_INTERDIGIT_LONG;
            cmTmrStop ( &tmr );

            /* See if we can reuse an already existing session.
            */
            if ( ( eAction == eCMDM_RECALL ) &&
                 ( ( pSes = cmUsrFocusSes ( pUsr ) ) != NULL ) &&
                 ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_DIALING ) )
            {
               /* Apply the action.
               */
               bRet = cmFeatApplyAction ( pRes, eAction );
            }
            /* Create the new session resource support.
            */
            else
            {
               pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl );
               if ( pSes )
               {
                  CMSES *pOtherSes = cmUsrFocusSes ( pUsr );

                  /* Switch the focus.
                  */
                  if ( pOtherSes )
                  {
                     pOtherSes->ses.med.bFocus = BOS_FALSE;
                  }
                  pSes->ses.med.bFocus = BOS_TRUE;

                  cmUsrLinkSes ( pUsr, pSes );

                  if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_FLASH )
                  {
                     memcpy ( pSes->ses.med.uDigCol,
                              pUsr->uDigCol,
                              CMGR_COLLECTED_DIGIT_LEN + 1 );
                     pSes->ses.med.uDigCnt = pUsr->uDigCnt;
                  }

                  /* Put the user in the correct state to manage this action on
                  ** the new session.
                  */
                  cmUsrNewState ( pUsr, eCMST_DIALING );

                  /* Apply the action.
                  */
                  bRet = cmFeatApplyAction ( pRes, eAction );
               }
            }
         }
         break;

         /* Actions which are either:
         **
         **    - Triggering a lookup on a specific feature then decide what to do.
         **    - Triggeriung an immediate local change with no consequences for the
         **      call processing.
         */
         case eCMDM_ACTIVATEDCHECK:
         case eCMDM_CFPROGRAM:
         case eCMDM_CFACTIVATE:
         case eCMDM_CFDEACTIVATE:
         case eCMDM_CBACTIVATEALL:
         case eCMDM_CBACTIVATEDM:
         case eCMDM_CBDEACTIVATE:
         case eCMDM_DEACTIVATEDCHECK:
         case eCMDM_DNDACTIVATE:
         case eCMDM_DNDDEACTIVATE:
         case eCMDM_HOOKFLASHACTIVATE:
         case eCMDM_HOOKFLASHDEACTIVATE:
         case eCMDM_FEATURECHECK:
         {
            /* Apply the action.
            */
            bRet = cmFeatApplyAction ( pRes, eAction );
         }
         break;

         case eCMDM_UNKNOWNACTION:
         default:
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmRing
**
** PURPOSE:    FSM ring this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRing ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   CFGSIPNOANS *pNoAnsCfg = NULL;
   CFGSIPAUTOCALL *pAutoCallCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
   BOS_BOOL bInternalCall = ( pSes->uNetUsr == CMGR_INVALID_HANDLE )? BOS_TRUE : BOS_FALSE;

   CMTMR tmr;
   BOS_UINT32 uIx;
   BOS_UINT8 uCallIdent[ CMGR_CLID_LEN + 1];

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( uCallIdent, 0, sizeof ( uCallIdent ));

   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                              &cfgIdx,
                              (void *) &pUsrClidDsp,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      bClidNumber = pUsrClidDsp->bCNDActStat;
      bClidName = pUsrClidDsp->bCNAMDActStat;

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                            &cfgIdx,
                            (void *) pUsrClidDsp,
                            CFG_ITEM_DEL );
      pUsrClidDsp = NULL;
   }

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = pSes->uApp;
   sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
   sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
   sig.pData = &uCallIdent;

   /* Insert a unique identifier for this call by means of the
   ** calling number.  Note that this information may contain no
   ** data of interest, it is just there for the application to make
   ** use of it as needed.
   */
   sprintf ( (char *) uCallIdent,
             "%s",
             pSes->ses.med.callInfo.clid.cNumber );

   sig.uSig = CMGR_INVALID_HANDLE;

   /* If this is not an internal call detemrmine which ringing signal is to be applied
   */
   if ( !bInternalCall )
   {
      uIx = 0;
      while ( (sig.uSig == CMGR_INVALID_HANDLE) && (uIx < CCTKCFG_MAX_ALERT_INFO) )
      {
         /* Extract tone information from the alert-info that was passed if any.
         */
         sig.uSig = cmPhyToneFromString ( pSes->ses.med.callInfo.cAlertInfo[ uIx ] );
         /* Validate that the tone corresponds to the intended usage.
         */
         sig.uSig = cmPhyToneValidateRing ( sig.uSig );

         /* Continue to loop through the different possible tones until we find
         ** the appropriate one (if any).
         */
         uIx++;
      }

      if ( sig.uSig == CMGR_INVALID_HANDLE )
      {
         sig.uSig =
            ( ( bClidName || bClidNumber ) ?
                                 eEPTSIG_CALLID_RINGING : eEPTSIG_RINGING );
      }
      else
      {
         sig.uSig = cmPhyToneWithClid ( sig.uSig, ( bClidName || bClidNumber ) );
      }
   }
   /* Internal call - apply internal ring signal
   */
   else
   {
      sig.uSig =
            ( ( bClidName || bClidNumber ) ?
                                 eEPTSIG_CALLID_RINGINT : eEPTSIG_RINGINT );
   }
   pUsr->uCpt = sig.uSig;

   if ( pUsr->pAutoCall && ( pUsr->pAutoCall->eType == eCMDM_ACACTIVATE ) )
   {
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                                 &cfgIdx,
                                 (void *) &pAutoCallCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         tmr.uTimeOut = pAutoCallCfg->uSpRingDuration;

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoCallbackConcept ),
                               &cfgIdx,
                               (void *) pAutoCallCfg,
                               CFG_ITEM_DEL );
         pAutoCallCfg = NULL;
      }
      else
      {
         tmr.uTimeOut = CMGR_RING_TONE_TIMEOUT;
      }

   }
   else if ( pUsr->pAutoCall && ( pUsr->pAutoCall->eType == eCMDM_ARACTIVATE ) )
   {
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                                 &cfgIdx,
                                 (void *) &pAutoCallCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         tmr.uTimeOut = pAutoCallCfg->uSpRingDuration;

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, AutoRecallConcept ),
                               &cfgIdx,
                               (void *) pAutoCallCfg,
                               CFG_ITEM_DEL );
         pAutoCallCfg = NULL;
      }
      else
      {
         tmr.uTimeOut = CMGR_RING_TONE_TIMEOUT;
      }

   }
   else
   {
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, NoAnsConcept ),
                                 &cfgIdx,
                                 (void *) &pNoAnsCfg,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         tmr.uTimeOut = pNoAnsCfg->uTODuration;

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, NoAnsConcept ),
                               &cfgIdx,
                               (void *) pNoAnsCfg,
                               CFG_ITEM_DEL );
         pNoAnsCfg = NULL;
      }
      else
      {
         tmr.uTimeOut = CMGR_RING_TONE_TIMEOUT;
      }
   }

   tmr.uSes = pSes->uHdl;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_RINGING;

#if CMGRCFG_MULTILINE

   /* This code fills in the called number field and passes to the phy interface.
   ** This is mostly to pass the called party information during internal calls
   */

   BOS_UINT8* uCalledNum[ CMGR_CLID_LEN + 1 ];
   CMSESLIST sesList;

   memset( &sesList, 0, sizeof( CMSESLIST ) );
   memset( uCalledNum, 0, ( CMGR_CLID_LEN + 1 ) * sizeof( BOS_UINT8 ) );

   cmSesListWithCid( pSes->uNet, &sesList );
   for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
   {
      CMSES* pOtherSes = cmSesValid( sesList.uSes[ uIx ] );
      if( pOtherSes && (pOtherSes->bEstab == BOS_TRUE) && ( pOtherSes->uUsr != pSes->uUsr ) )
      {
         snprintf( (char*)&( uCalledNum[ 0 ] ), (CMGR_CLID_LEN + 1),
                   "%s", (char*)&( pOtherSes->ses.med.uDigCol[ 0 ] ) );
         break;
      }
   }

   if( strlen( (char*)&uCalledNum[ 0 ] ) <= 0 &&
       pCctkAccnt &&
       strlen( (char*)&pCctkAccnt->pcDefDialNum[ 0 ] ) > 0 )
   {
      snprintf( (char*)&uCalledNum[ 0 ], CMGR_USER_IDENTIFIER_LEN, "%s", (char*)&(pCctkAccnt->pcDefDialNum[0]) );
   }

   sig.pData = &uCalledNum;
#endif /* CMGRCFG_MULTILINE */

   cmUsrNewState ( pUsr, eCMST_RINGING );
   cmTmrStart ( &tmr );
   cmPhySignal ( pUsr->uPhys, &sig );

#if CMGRCFG_MULTILINE
   sig.pData = &uCallIdent;
#endif /* CMGRCFG_MULTILINE */

   /* Issue caller identification information if necessary.
   */
   if ( bClidName || bClidNumber )
   {
      cmSesClid ( pSes, bClidName, bClidNumber );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallWait
**
** PURPOSE:    FSM call waiting for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallWait ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = pUsr->pSesWait;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   CFGSIPCALLWAIT *pUsrCallWait = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;
   CMGREPSIG sig;
#if CMGRCFG_MULTILINE
   CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */

   CMTMR tmr;
   BOS_UINT32 uIx;
   BOS_UINT8 uCallIdent[ CMGR_CLID_LEN + 1];

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );
   memset ( uCallIdent, 0, sizeof ( uCallIdent ));

   tmr.uSes = pSes->uHdl;
   tmr.uUsr = pUsr->uHdl;
   tmr.uTimeOut = CMGR_CALL_WAIT_TIMEOUT;
   tmr.eType = eCMEVT_TMR_CALLWAIT;

   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                              &cfgIdx,
                              (void *) &pUsrCallWait,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      /* Override the call waiting time out with a zero value if
      ** we do not want to run timer on call waiting unanswered.
      */
      if ( pUsrCallWait &&
           pUsrCallWait->bNoCallWaitTO )
      {
         tmr.uTimeOut = 0;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallWaitConcept ),
                            &cfgIdx,
                            (void *) pUsrCallWait,
                            CFG_ITEM_DEL );
      pUsrCallWait = NULL;
   }

   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                              &cfgIdx,
                              (void *) &pUsrClidDsp,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pUsrClidDsp != NULL )
      {
         bClidNumber = pUsrClidDsp->bCNDActStat;
         bClidName = pUsrClidDsp->bCNAMDActStat;

         /* Special case of call waiting CLID overwrite to prevent
         ** CLID information to be sent with the call waiting signal.
         */
         if ( pUsrClidDsp->bCIDCWBlock )
         {
            bClidNumber = BOS_FALSE;
            bClidName = BOS_FALSE;
         }
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                            &cfgIdx,
                            (void *) pUsrClidDsp,
                            CFG_ITEM_DEL );
      pUsrClidDsp = NULL;
   }

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = pSes->uApp;
   sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
   sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
   sig.pData = &uCallIdent;

   /* Insert a unique identifier for this call by means of the
   ** calling number.  Note that this information may contain no
   ** data of interest, it is just there for the application to make
   ** use of it as needed.
   */
   sprintf ( (char *) uCallIdent,
             "%s",
             pSes->ses.med.callInfo.clid.cNumber );

   sig.uSig = CMGR_INVALID_HANDLE;
   uIx = 0;
   while ( (sig.uSig == CMGR_INVALID_HANDLE) && (uIx < CCTKCFG_MAX_ALERT_INFO) )
   {
      /* Extract tone information from the alert-info that was passed if any.
      */
      sig.uSig = cmPhyToneFromString ( pSes->ses.med.callInfo.cAlertInfo[ uIx ] );
      /*
      ** If this is a ring tone, transform it into a corresponding call waiting tone
      ** before further processing.
      */
      if ( cmPhyToneValidateRing ( sig.uSig ) != CMGR_INVALID_HANDLE )
      {
         sig.uSig = cmPhyToneRing2CallWait ( sig.uSig );
      }
      /*
      ** Validate that the tone corresponds to the intended usage.
      */
      sig.uSig = cmPhyToneValidateCallWait ( sig.uSig );

      /* Continue to loop through the different possible tones until we find
      ** the appropriate one (if any).
      */
      uIx++;
   }

   if ( sig.uSig == CMGR_INVALID_HANDLE )
   {
      sig.uSig =
         ( ( bClidName || bClidNumber ) ?
                              eEPTSIG_SAS_CAS1 : eEPTSIG_CALLW1 );
   }
   else
   {
      sig.uSig = cmPhyToneWithClid ( sig.uSig, ( bClidName || bClidNumber ) );
   }
   pUsr->uCpt = sig.uSig;


   /* Force duration of the call waiting.
   */
   if ( tmr.uTimeOut )
   {
      cmTmrStart ( &tmr );
   }

   cmPhySignal ( pUsr->uPhys, &sig );

   /* Issue caller identification information if necessary.
   */
   if ( bClidName || bClidNumber )
   {
      cmSesClid ( pSes, bClidName, bClidNumber );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAutoCallApply
**
** PURPOSE:    FSM auto-call apply to this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAutoCallApply ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < ( CMGRCFG_AUTOCALL_MAX / 2 ) ; uIx++ )
   {
      if ( pUsr->pAc[ uIx ] && pUsr->pAc[ uIx ]->bUsed &&
           pUsr->pAc[ uIx ]->bPending )
      {
         pUsr->pAutoCall = pUsr->pAc[ uIx ];
         break;
      }
      else if ( pUsr->pAr[ uIx ] && pUsr->pAr[ uIx ]->bUsed &&
                pUsr->pAr[ uIx ]->bPending )
      {
         pUsr->pAutoCall = pUsr->pAc[ uIx ];
         break;
      }
   }

   if ( pUsr->pAutoCall )
   {
      /* Ring the local user.
      */
      CMSES *pSes = NULL;

      if ( ( pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl ) ) != NULL )
      {
         if ( cmUsrLinkSes ( pUsr, pSes ) )
         {
            if ( ( pUsr->uPhys == CMGR_INVALID_HANDLE ) &&
                 ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS ) )
            {
               BOS_UINT8 *pAt = NULL;
               BOS_UINT8 *pClid = NULL;

               /* Publish a 'line active' event.
               */
               if ( gCmCfgCb.cmgrEvtCb &&
                    (pUsr->eType != eCMUSRTYPE_REMOTE) )
               {
                  gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_ACTIVE,
                                       pUsr->uPhys,
#if CMGRCFG_MULTILINE
                                       pUsr->uHdl,
                                       CMGR_INVALID_HANDLE,
#endif /* CMGRCFG_MULTILINE */
                                       NULL );
               }

               /* Create the call info block for this call.  The CLID is derived
               ** from the peer URI which is used for the auto-call information.
               */
               memset ( &pSes->ses.med.callInfo,
                        0,
                        sizeof ( CCTKCALLINFO ) );

               if ( ( pClid = (BOS_UINT8 *) strchr (
                                             (const char *) pUsr->pAutoCall->uPeer,
                                             ':' ) ) == NULL )
               {
                  pClid = pUsr->pAutoCall->uPeer;
               }

               if ( ( pAt = (BOS_UINT8 *) strchr (
                                           (const char *) pClid, '@' ) ) != NULL )
               {
                  *pAt = '\0';
               }
               strMaxCat ( (char *) pSes->ses.med.callInfo.clid.cName,
                           (const char *) pClid,
                           CCTK_CLID_LEN );
               if ( pAt )
               {
                  *pAt = '@';
               }

               strMaxCat ( (char *) pSes->ses.med.callInfo.clid.cNumber,
                           "O",
                           CCTK_CLID_LEN );

               pSes->ses.med.bFocus = BOS_TRUE;

               cmFsmRing ( pUsr );
            }
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmCallForwardViaDialString
**
** PURPOSE:    Call forwarding via dialstring match.
**
** PARAMETERS: pRes - pointer to forwarding cmUsr
**             cid  - cid of the call to be forwarded
**             dialString - the fwd to number.
** RETURNS:    None.
**
** NOTES:
***********************************************************************************/
void cmFsmCallForwardViaDialString( void * pRes, BOS_UINT32 cid, char * dialString )
{
   CMDMACTIONTYPE eAction;
   BOS_UINT8 *pActParm;
   BOS_BOOL bOriginatorExternal = BOS_TRUE;
   CMSESLIST sesList;
   CMSES *pSes;
   CMDMSTATUS dmStatus;
   CMUSR *pFwdingUsr = (CMUSR*)pRes;
   BOS_UINT32 cctkUsrHdl = cmUsrGetNtwkHdl( pFwdingUsr );
#if CMGRCFG_MULTILINE
   CMCCTKACCNT*  pCctkAccnt = cmCctkAccntFindByNet( getUsrCctkHdlFromRouting( pFwdingUsr, BOS_FALSE ) );
#endif /* CMGRCFG_CATIQ_2_0 */

   CMGRLOG_INFORMATION ((
      "cmFsmCallForwardViaDialString: fwd cid 0x[%08x] to dialstring \'%s\'",
       cid, dialString  ));

   /* 0 - Figure out if call to be forwarded is internal/external */
   cmSesListWithCid( cid, &sesList );
   if( sesList.uLength )
   {
      pSes = cmSesValid( sesList.uSes[ 0 ] );
      if ( pSes && ( pSes->uNetUsr == CMGR_INVALID_HANDLE )  )
      {
         bOriginatorExternal = BOS_FALSE;
      }
   }

   CMGRLOG_INFORMATION (("cmFsmCallForwardViaDialString: bOriginatorExternal %d", bOriginatorExternal));

   /* 1 - Put dial string through digitmap mapper and get action and arguments */

   /* Update used digitmap if it hasnt been set yet */
   if( pFwdingUsr->pDmInUse == NULL )
   {
#if CMGRCFG_MULTILINE
      if( pCctkAccnt )
      {
         CMGRLOG_DEBUG(( "%s: Using SIP Digit map.\n", __FUNCTION__ ));
         pFwdingUsr->pDmInUse = pCctkAccnt->pDmDigMap;
      }
      else
      {
         CMGRLOG_DEBUG(( "%s: Using INTERNAL Digit map.\n", __FUNCTION__ ));
         pFwdingUsr->pDmInUse = pFwdingUsr->pDmDigMap;
      }
#else
      pFwdingUsr->pDmInUse = pFwdingUsr->pDmDigMap;
#endif /* CMGRCFG_MULTILINE */
   }

   /* Reset the digitmap state on the user */
   cmDmReset ( (CMDMACTIONSTATE *)pFwdingUsr->pDmState,
               CMDM_ACTION_BUFFER_SIZE );

   dmStatus = cmDmCheckDialString (
#if CMGRCFG_MULTILINE
                                    (pCctkAccnt) ? pCctkAccnt->uNet : pFwdingUsr->uHdl,
#else
                                    pFwdingUsr->uHdl,
#endif /* CMGRCFG_MULTILINE */
                                    (CMDMDIGITMAP *) pFwdingUsr->pDmInUse,
                                    (BOS_UINT8 *)dialString,
                                    (CMDMACTIONSTATE *) pFwdingUsr->pDmState );

   /* 2 - based on digitmap action, handle forward request */
   switch ( dmStatus )
   {
      /* Process the actions associated with this user.
      */
      case eCMDM_NOMATCH:
      case eCMDM_SYNTAXERROR:
      case eCMDM_ERROR:
      {
         CMGRLOG_INFORMATION (("cmFsmCallForwardViaDialString: Bad dmStatus %d", dmStatus));
      }
      break;

      case eCMDM_SUCCESS:
      default:
      {
         cmDmGetAction ( (CMDMACTIONSTATE *)pFwdingUsr->pDmState, &eAction );
         cmDmGetParameter ( (CMDMACTIONSTATE *)pFwdingUsr->pDmState, &pActParm );

         CMGRLOG_INFORMATION (("cmFsmCallForwardViaDialString: Action %s Parameter %s",
                                             (char *) cmMapGetStr ( cmMapDmAction, eAction ),
                                             (char *) pActParm));

         /* Handle fwd request basedon fwd target call type */
         switch ( eAction )
         {
            case eCMDM_MAKECALL:
            {
               if( bOriginatorExternal )
               {
                  /* case A: Ext -> Ext forwarding */
                  cctkCallForward( cid, pActParm );
               }
               else
               {
                  /* case C: Int -> Ext forwarding
                  ** Delete all session informaion on originating user
                  ** Change state of originating user to DIALING
                  ** End call on fwding user
                  ** originate new call on originating user via its Unet
                  */
               }
            }
            break;

            case eCMDM_MAKECALLINT:
            {
               if( bOriginatorExternal )
               {
                  /* case B: Ext -> Int forwarding
                  ** End call on pFwdingUser
                  ** eCMEVT_INCOMING_CALL and eCMEVT_ALERT_CALL on fwd target
                  ** +line force
                  */
                  CMEVT evt;
                  BOS_BOOL * pbEmergency = malloc( sizeof( BOS_BOOL ) );                                                        
                  CCTKCALLINFO * pCallInfo = NULL;
                  char * puUid = NULL;
                  char * pCol = NULL;
                  CMUSR* pFwdToUsr = NULL;    
                                
                  /* Find target user via uUid */
                  if( pActParm )
                  {
                     pCol = strchr( (char*)pActParm, ':' );
                     if( pCol == NULL )
                     {
                        CMGRLOG_CRITICAL(( "%s: No \':\' in dialstring! Failing call.", __FUNCTION__ ));
                        break;
                     }      
                     *pCol = '\0';
                     puUid = (char*)pActParm;
                     if( puUid != NULL )
                     {
                        /* Attempt to find the other user */
                        pFwdToUsr = cmUsrFindUidHdl( (BOS_UINT8*)puUid );   
                     }
                     else
                     {
                        CMGRLOG_CRITICAL(( "%s: Cannot find target internal usr! Failing callfwd cid:0x%08x DialString:%s fwdingUsr:0x%08x",
                                                                                  __FUNCTION__, cid, dialString, pFwdingUsr->uHdl ));
                        break;
                     }               
                  }
                  else
                  {
                     CMGRLOG_CRITICAL(( "%s: no pActParm retrieved! Failing callfwd cid:0x%08x DialString:%s fwdingUsr:0x%08x",
                                                                                  __FUNCTION__, cid, dialString, pFwdingUsr->uHdl ));
                     break;
                  }
                 
                  /* Setup the emergency block */
                  memset( pbEmergency, 0, sizeof( BOS_BOOL ) );
                  *pbEmergency = BOS_FALSE;      

                  /* Copy the callinfo block over if it exists*/
                  if( pSes )
                  {
                     pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );  
                     memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );     
                     memcpy ( pCallInfo, &pSes->ses.med.callInfo, sizeof ( CCTKCALLINFO ) );
                     pCallInfo->bEmergency = BOS_FALSE;
                     pCallInfo->bLoopBack = BOS_FALSE;
                  }
                  
                  /* Create "dummy" INCOMING_CALL CCTK event to trigger session creation */
                  memset ( &evt, 0, sizeof( CMEVT ) );
                  evt.uEvt = ( eCMEVT_INCOMING_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
                  evt.uDat1 = pFwdToUsr->uHdl;        /* User handle */
                  evt.uDat2 = cid;                    /* Call ID */
                  evt.pDat3 = pbEmergency;            /* pData */
                  evt.uDat4 = cctkUsrHdl;             /* CCTK user id */
                  evt.bLast = BOS_TRUE;               /* Last event */
                        
                  /* Allocate a new session for the fwdToUsr */
                  CMSES * pNewSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pFwdToUsr->uHdl );
                  
                  if( pNewSes )
                  {
                     pNewSes->uNet = cid;                      /* CallId handle */
                     pNewSes->uNetUsr = pFwdingUsr->evt.uDat4; /* CCTK user handle */
                     pNewSes->bEstab = BOS_FALSE;              /* Clear the established flag */
                     pNewSes->bForwarded = BOS_TRUE;           /* Set the forwarded flag */
                     
                     /* Copy over dialstring */
                     memset( pNewSes->ses.med.uDigCol, 0, (CMGR_COLLECTED_DIGIT_LEN + 1) * sizeof( BOS_UINT8 ) );
                     snprintf( (char*)&( pNewSes->ses.med.uDigCol[ 0 ] ), CMGR_COLLECTED_DIGIT_LEN, "%s", dialString );
                     
                     if ( cmUsrLinkSes ( pFwdToUsr, pNewSes ) )
                     {
                        pNewSes->ses.med.bEmergency = *pbEmergency;
                        pNewSes->ses.med.bRemInit = BOS_TRUE;
                     }
                     else
                     {
                        CMGRLOG_CRITICAL(( "%s: Cannot link session fwdToUsr:0x%08x cid:0x%08x", __FUNCTION__, cid, pFwdToUsr->uHdl ));                        
                        break;
                     }
                  }
                  else
                  {
                     CMGRLOG_CRITICAL(( "%s: No new session on fwdToUsr:0x%08x cid:0x%08x", __FUNCTION__, cid, pFwdToUsr->uHdl ));
                     break;
                  }
                    
                  /* Handle INCOMING_CALL event on fwdtoUsr rightaway */
                  cmUsrNewEvent ( pFwdToUsr, eCMEVT_INCOMING_CALL );
                  memcpy ( &pFwdToUsr->evt, &evt, sizeof ( CMEVT ) );
                  cmFsmRun ( eCMFSM_CCTK, (void *) pFwdToUsr );
                  
   
                  /* If we have valid callinfo this means that the forwarding user
                  ** received an ALERT. We need to create DUMMY ALERT for forwarded
                  ** to user as well
                  */
                  if( pCallInfo )
                  {
                     evt.uEvt = ( eCMEVT_ALERT_CALL | CMGR_EVT_CLASS__CCTK );
                     evt.uDat1 = pFwdToUsr->uHdl;        /* User handle */
                     evt.uDat2 = cid;                    /* Call ID */
                     evt.pDat3 = pCallInfo;              /* pData */
                     evt.uDat4 = cctkUsrHdl;             /* CCTK user id */
                     evt.bLast = BOS_TRUE;               /* Last event */

                     /* Handle ALERT_CALL event on fwdtoUsr rightaway */
                     cmUsrNewEvent ( pFwdToUsr, eCMEVT_ALERT_CALL );
                     memcpy ( &pFwdToUsr->evt, &evt, sizeof ( CMEVT ) );
                     cmFsmRun ( eCMFSM_CCTK, (void *) pFwdToUsr );
                  }
               }
               else
               {
                  /* case D: Int -> Int forwarding
                  ** Delete all session informaion on originating user
                  ** Change state of originating user to DIALING
                  ** End call on fwding user
                  ** Make internal call to fwded user by sending INCOMING_CALL AND ALERT CALL
                  */
               }
            }
            break;

            default:
            break;
         }
      }
      break;
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmXferInt
**
** PURPOSE:    FSM internal transfer handler.
**
** PARAMETERS: pRes       - the pointer to the user on which to run the FSM.
**             ppSesFrom  - pointer to pointer to session to be transferred
**             ppSesTo    - pointer to pointer to internal session to transfer to
**
** RETURNS:    None.
**
** NOTES:      This function assumes two things:
**                1) pSesTo points to an INTERNAL session. pSesFrom can be either internal
**                      or external.
**                2) pSesFrom is already put on hold.
***********************************************************************************/
void cmFsmXferInt ( void* pRes, void* ppSesFrom, void* ppSesTo )
{
   CMSES* pSesFrom = NULL;       /* Session to transfer - can be External or Internal */
   CMSES* pSesFrom_Other = NULL; /* If internal session from which we are transferring, this is the other user's session */
   CMUSR* pUsrFrom_Other = NULL; /* The other user if pSesFrom is internal */
   CMSES* pSesTo = NULL;         /* Session to transfer to - MUST be internal */
   CMSES* pSesTo_Other = NULL;   /* Other side of the internal call that we are transferring to */
   CMUSR* pUsrTo_Other = NULL;   /* Other user of the internal call that we are transferring to */
   CMUSR* pUsr = NULL;           /* This user */
   CMEVT evt;
   CMSESLIST sesList;
   BOS_UINT32 uIx;
   CCTKCALLINFO *pCallInfo = NULL;

   BOS_ASSERT( pRes );
   BOS_ASSERT( ppSesFrom );
   BOS_ASSERT( ppSesTo );

   pSesFrom = *(CMSES**)ppSesFrom;
   pSesTo = *(CMSES**)ppSesTo;
   pUsr = (CMUSR*)pRes;

   /* Structure of call regardless of which case this falls under
    ** (internal to external, external to internal, or
    ** internal to internal)
    ********************************************************************************************** *
    * |------------------|           |-----------------|          |------------------------------| *
    * |    pSesFrom_Other|  external |pSesFrom   pSesTo| internal |pSesTo_Other   pSesTo_OtherNew| *
    * |        A         |<--------->|         B       |<-------->|             C                | *
    * |------------------|           |-----------------|          |------------------------------| *
    ***********************************************************************************************/

   /**************************************************************************
   ** Find the other session that is part of this internal call.
   **************************************************************************/
   memset( &sesList, 0, sizeof( CMSESLIST ) );
   cmSesListWithCid( pSesTo->uNet, &sesList );
   for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
   {
      pSesTo_Other = cmSesValid( sesList.uSes[ uIx ] );
      if( pSesTo_Other && pSesTo_Other->uUsr != pUsr->uHdl )
      {
         /* Found a session with this call id whose
          ** user is not us (ie, the other internal
          ** call user).
          */
         pUsrTo_Other = cmUsrValid( pSesTo_Other->uUsr );
         break;
      }
      pSesTo_Other = NULL;
   }
   if( pSesTo_Other == NULL || pUsrTo_Other == NULL )
   {
      CMGRLOG_CRITICAL(( "%s: Unable to find other user for internal call [%X]!", __FUNCTION__, pSesFrom->uNet ));

      /* External call */
      if( pSesFrom->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Delete the external call */
         cctkCallDelete( pSesFrom->uNet, eCCTKCALLDELRSN_NORMAL );
      }
      /* Internal call */
      else
      {
         memset( &evt, 0, sizeof( CMEVT ) );

         evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
         evt.uDat1 = pUsr->uHdl;          /* User handle */
         evt.uDat2 = pSesFrom->uHdl;      /* Session handle of the call to transfer (pSesFrom -> pSesTo) */
         evt.pDat3 = NULL;                /* pData */
         evt.uDat4 = pSesFrom->uNetUsr;   /* CCTK user id */
         evt.bLast = BOS_TRUE;            /* Last event */
         if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }
      return;
   }

   /* If the from session is an internal call, find the other
   ** session & user.
   */
   if( pSesFrom->uNetUsr == CMGR_INVALID_HANDLE )
   {
      memset( &sesList, 0, sizeof( CMSESLIST ) );
      cmSesListWithCid( pSesFrom->uNet, &sesList );
      for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
      {
         pSesFrom_Other = cmSesValid( sesList.uSes[ uIx ] );
         if( pSesFrom_Other && pSesFrom_Other->uUsr != pUsr->uHdl )
         {
            /* Found a session with this call id whose
            ** user is not us (ie, the other internal
            ** call user).
            */
            pUsrFrom_Other = cmUsrValid( pSesFrom_Other->uUsr );
            break;
         }
         pSesFrom_Other = NULL;
      }
   }

   /***************************
   ** STANDARD CALL TRANSFER **
   ***************************/
   /* If this internal call is already established, just copy over the session */
   if( pSesTo_Other->bEstab == BOS_TRUE )
   {
      CMSES* pSesFrom_OtherNew = NULL;
      CMSES* pSesTo_OtherNew = NULL;
      BOS_UINT32 uCnt;
      BOS_UINT32 uCntFrom;
      BOS_UINT32 uTempCid;
      CMGREPSIG sig;
#if CMGRCFG_MULTILINE
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSesFrom->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
      BOS_UINT8 uCallIdent[ CMGR_CLID_LEN + 1];

      /* If this session is established, we simply need to:
      ** 1) replace the internal session on the other user with the external session
      ** 2) remove the connection on this user
      ** 3) setup the connection on the other user (re-negotiate media info)
      */
      memset( &sig, 0, sizeof( CMGREPSIG ) );
      memset( uCallIdent, 0, ( CMGR_CLID_LEN + 1 ) * sizeof( BOS_UINT8 ) );

      /* Copy the CLID data to use if we need to send CW */
      sprintf ( (char *) uCallIdent,
                "%s",
                pSesFrom->ses.med.callInfo.clid.cNumber );

      /* Count the number of media sessions associated
      ** with this user.
      */
      uCnt = 0;
      for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
      {
         if ( pUsrTo_Other->pMedSes [ uIx ] != NULL )
         {
            uCnt++;
         }
      }
      /* If both sides are internal, count the alternal user's media sessions too. */
      uCntFrom = 0;
      if( pUsrFrom_Other )
      {
         for( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
         {
            if ( pUsrFrom_Other->pMedSes [ uIx ] != NULL )
            {
               uCntFrom++;
            }
         }
      }

      /* 1) Replace the internal session on the other user */
      if( uCnt <= 1 )
      {
         /* If we only have one session left, we need to first add the new session
         ** to properly keep the user in the same state. Do this by sending call
         ** waiting, tear down the other call, then answer the new call.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSig = eEPTSIG_CALLW1;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSesFrom->uApp;
         sig.uCid = pSesFrom->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.pData = &uCallIdent;

         cmPhySignal ( pUsrTo_Other->uPhys, &sig );
      }
      cmSesPhyDelete ( pSesTo_Other );
      cmUsrUnLinkSes ( pUsrTo_Other, pSesTo_Other );
      pSesTo_Other->uNet = CMGR_INVALID_HANDLE;
      cmSesFree ( pSesTo_Other, pUsrTo_Other->uHdl );

      /* Release the internal call on us */
      memset( &evt, 0, sizeof( CMEVT ) );
      evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
      evt.uDat1 = pUsr->uHdl;          /* User handle */
      evt.uDat2 = pSesTo->uHdl;        /* Session handle of the call to transfer (pSesFrom -> pSesTo) */
      evt.pDat3 = NULL;                /* pData */
      evt.uDat4 = pSesTo->uNetUsr;     /* CCTK user id */
      evt.bLast = BOS_TRUE;            /* Last event */
      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }

      /* Create the new session info. */
      pSesTo_OtherNew = cmSesAlloc( eCMSESTYPE_MEDIA, pUsrTo_Other->uHdl );
      uTempCid = pSesTo_OtherNew->uHdl; /* Make sure to keep the handle */

      /* Can safely copy the session information across */
      memcpy( pSesTo_OtherNew, pSesFrom, sizeof( CMSES ) );
      pSesTo_OtherNew->uHdl = uTempCid;
      pSesTo_OtherNew->ses.med.pAutoCall = NULL;
      pSesTo_OtherNew->ses.med.pLinkedSes = NULL;
      pSesTo_OtherNew->ses.med.uPhy = CMGR_INVALID_HANDLE;
      pSesTo_OtherNew->uApp = pSesTo_OtherNew->uHdl;
      pSesTo_OtherNew->bEstab = BOS_TRUE;
      pSesTo_OtherNew->ses.med.bFocus = BOS_TRUE;

      if( cmUsrLinkSes ( pUsrTo_Other, pSesTo_OtherNew ) == BOS_FALSE )
      {
         CMGRLOG_CRITICAL(( "%s: Unable to link Session [%X] to User [%X]!", __FUNCTION__, pSesTo_OtherNew->uHdl, pUsrTo_Other->uHdl ));
      }

      /* If this is an internal to internal call transfer and the "from" user has only
      ** one call, we need to create the internal sessions between both users before
      ** the first calls are torn down.
      */
      if( uCntFrom == 1 )
      {
         pSesFrom_OtherNew = cmSesAlloc( eCMSESTYPE_MEDIA, pUsrFrom_Other->uHdl );

         pSesFrom_OtherNew->uNetUsr = CMGR_INVALID_HANDLE; /* No CCTK user */
         pSesFrom_OtherNew->uNet = cctkGenerateId();       /* Generate a unique call id */
         /* TODO: Notify DECT of this new call id!!! */
         pSesTo_OtherNew->uNetUsr = CMGR_INVALID_HANDLE;
         pSesTo_OtherNew->uNet = pSesFrom_OtherNew->uNet;

         cmUsrLinkSes( pUsrFrom_Other, pSesFrom_OtherNew );

         /* Set to established and focus */
         pSesFrom_OtherNew->bEstab = BOS_TRUE;
         pSesFrom_OtherNew->ses.med.bFocus = BOS_TRUE;

         /* Create the session */
         cmSesPhyCreate( pSesFrom_OtherNew );
      }

      /* 2) End the from call on this user */
      if( pSesFrom->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Add the new call to the cctkAccnt number of active calls because
         ** cmFsmCallEnd will remove the active call.
         */
         cmCctkAccntCallAdd( pSesFrom->uNetUsr );
      }
      pUsr->evt.uDat2 = pSesFrom->uHdl;
      cmFsmCallEnd( pRes );

      /* 3) Setup the connection on the other user */
      cmSesPhyCreate( pSesTo_OtherNew );
      if( uCnt <= 1 )
      {
         /* Turn off CW if we were playing it */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSig = eEPTSIG_CALLW1;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSesTo_OtherNew->uApp;
         sig.uCid = pSesTo_OtherNew->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.pData = &uCallIdent;

         cmPhySignal ( pUsrTo_Other->uPhys, &sig );
      }

      /* External call */
      if( pSesTo_OtherNew->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Unhold the external call */
         cctkCallUnhold( pSesTo_OtherNew->uNet );
      }
      /* Internal call */
      else
      {
         memset( &evt, 0, sizeof( CMEVT ) );
         evt.uEvt = ( eCMEVT_UNHOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
         evt.uDat1 = pSesTo_OtherNew->uUsr;       /* User handle */
         evt.uDat2 = pSesTo_OtherNew->uHdl;       /* Session handle of the call to transfer (pSesFrom -> pSesTo) */
         evt.pDat3 = NULL;                        /* pData */
         evt.uDat4 = pSesTo_OtherNew->uNetUsr;    /* CCTK user id */
         evt.bLast = BOS_TRUE;                    /* Last event */
         if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }
   } /* pSesTo_Other->bEstab == BOS_TRUE */
   /************************
   ** BLIND CALL TRANSFER **
   ************************/
   else
   {
      CMUSRLIST cmUsrList;
      CMSES* pSesTo_OtherNew = NULL;

      /* Essentially what we will be doing is ending the "to" internal calls, then
      ** creating new session(s) for the transferees with the call id of the from call.
      ** We will also force the "from" call to be inactive which ensures once a terminal
      ** answers it, it renegotiates media and sets up its streams as expected.
      */
      memset( &evt, 0, sizeof( CMEVT ) );

      /* Release the internal call on us first */
      evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
      evt.uDat1 = pUsr->uHdl;          /* User handle */
      evt.uDat2 = pSesTo->uHdl;        /* Session handle of the call */
      evt.pDat3 = NULL;                /* pData */
      evt.uDat4 = pSesTo->uNetUsr;     /* CCTK user id */
      evt.bLast = BOS_TRUE;            /* Last event */

      *(CMSES**)ppSesTo = NULL; /* pSesTo is now cleared */

      /* Run the FSM right away */
      memcpy( &pUsr->evt, &evt, sizeof( CMEVT ) );
      cmUsrNewEvent( pUsr, (CMGR_EVT_CLASS__EVT_MASK & evt.uEvt) );
      cmFsmCctkRun( pUsr );

      /* Since it's possible we are making an internal general call (to all internal
      ** terminals), we have to run through the list of all terminals with the cid and
      ** end the call on each of them separately.
      ** Additionally, we need to construct the list of all other users to whom this
      ** call was sent so that we can re-send it.
      */
      memset( &cmUsrList, 0, sizeof( CMUSRLIST ) );
      memset( &sesList, 0, sizeof( CMSESLIST ) );
      cmSesListWithCid( pSesTo_Other->uNet, &sesList ); /* pSesTo is cleared, but pSesTo_Other isn't at this point */
      for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
      {
         pSesTo_Other = cmSesValid( sesList.uSes[ uIx ] );
         if( !pSesTo_Other || pSesTo_Other->uUsr == pUsr->uHdl )
         {
            continue;
         }

         pUsrTo_Other = cmUsrValid( pSesTo_Other->uUsr );

         memset ( &evt, 0, sizeof( CMEVT ) );

         /* Release the internal call on the other user */
         evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
         evt.uDat1 = pUsrTo_Other->uHdl;      /* User handle */
         evt.uDat2 = pSesTo_Other->uHdl;      /* Session handle of the call to transfer (pSesFrom -> pSesTo) */
         evt.pDat3 = NULL;                    /* pData */
         evt.uDat4 = pSesTo_Other->uNetUsr;   /* CCTK user id */
         evt.bLast = BOS_TRUE;                /* Last event */

         /* Run the FSM right away */
         memcpy( &pUsrTo_Other->evt, &evt, sizeof( CMEVT ) );
         cmUsrNewEvent( pUsrTo_Other, (CMGR_EVT_CLASS__EVT_MASK & evt.uEvt) );
         cmFsmCctkRun( pUsrTo_Other );

         /* Add the user to the users list */
         cmUsrList.uUsr[ cmUsrList.uLength++ ] = pUsrTo_Other->uHdl;
      }

      /* Make sure that the external call on us is no longer set to established. This
      ** will ensure that the other user can accept the call properly.
      ** If this is an internal to internal call transfer, we need to keep this established
      ** flag as true to ensure the other call knows we're the originator.
      */
      pSesFrom->bEstab = ( pSesFrom->uNetUsr != CMGR_INVALID_HANDLE ? BOS_FALSE : BOS_TRUE );

      /* Iterate through our newly created user list and send incoming call events to each
      ** valid user in the list, excluding ourselves and PSTN lines.
      */
      for( uIx = 0 ; uIx < cmUsrList.uLength ; uIx++ )
      {
         pUsrTo_Other = cmUsrValid( cmUsrList.uUsr[ uIx ] );
         if( !pUsrTo_Other || pUsrTo_Other->uHdl == pUsr->uHdl ||
             pUsrTo_Other->eType == eCMUSRTYPE_PSTN )
         {
            continue;
         }

         pCallInfo = malloc ( sizeof ( CCTKCALLINFO ) );

         memset( pCallInfo, 0, sizeof( CCTKCALLINFO ) );

         memcpy( pCallInfo, &(pSesFrom->ses.med.callInfo), sizeof( CCTKCALLINFO ) );
         pCallInfo->bEmergency = BOS_FALSE;

         memset ( &evt, 0, sizeof( CMEVT ) );

         /* Send two "dummy" CCTK events. INCOMING_CALL will trigger session creation
         **for the terminal and ALERT_CALL will cause the terminal to start ringing/play CW.
         */
         evt.uEvt = ( eCMEVT_INCOMING_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
         evt.uDat1 = pUsrTo_Other->uHdl; /* User handle */
         evt.uDat2 = pSesFrom->uNet;     /* Call ID */
         evt.pDat3 = pCallInfo;        /* pData */
         evt.uDat4 = pSesFrom->uNetUsr;  /* CCTK user id */
         evt.bLast = BOS_TRUE;           /* Last event */

         /* Run the FSM right away */
         memcpy( &pUsrTo_Other->evt, &evt, sizeof( CMEVT ) );
         cmUsrNewEvent( pUsrTo_Other, (CMGR_EVT_CLASS__EVT_MASK & evt.uEvt) );
         cmFsmCctkRun( pUsrTo_Other );

         memset ( &evt, 0, sizeof( CMEVT ) );

         evt.uEvt = ( eCMEVT_ALERT_CALL | CMGR_EVT_CLASS__CCTK );
         evt.uDat1 = pUsrTo_Other->uHdl;
         evt.uDat2 = pSesFrom->uNet;
         evt.pDat3 = pCallInfo;
         evt.uDat4 = pSesFrom->uNetUsr;
         evt.bLast = BOS_TRUE;

         /* Run the FSM right away */
         memcpy( &pUsrTo_Other->evt, &evt, sizeof( CMEVT ) );
         cmUsrNewEvent( pUsrTo_Other, (CMGR_EVT_CLASS__EVT_MASK & evt.uEvt) );
         cmFsmCctkRun( pUsrTo_Other );

         free( pCallInfo );
      }

      /* External to internal call transfer case only */
      if( pSesFrom->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Since this new session is actually held, make sure to set the held flag for
         ** the session so that when a terminal accepts the incoming transferred call,
         ** the call unholds and renegotiates media instead of treating it as a brand
         ** new call.
         */
         memset( &sesList, 0, sizeof( CMSESLIST ) );
         cmSesListWithCid( pSesFrom->uNet, &sesList );
         for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
         {
            pSesTo_OtherNew = cmSesValid( sesList.uSes[ uIx ] );
            if( pSesTo_OtherNew && pSesTo_OtherNew->uHdl != pSesFrom->uHdl )
            {
               pSesTo_OtherNew->bEstab = BOS_TRUE;
               pSesTo_OtherNew->ses.med.uPhy = CMGR_INVALID_HANDLE; /* Ensure we create a new connection */
               pSesTo_OtherNew->ses.med.bFocus = BOS_TRUE;
               pSesTo_OtherNew->ses.med.bBgFocus = BOS_FALSE;
            }
            pSesTo_OtherNew = NULL;
         }
      }

      /*  Finally, release the external call locally on us (but not in CCTK) */
      memset ( &evt, 0, sizeof( CMEVT ) );
      evt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK ); /* Event */
      evt.uDat1 = pUsr->uHdl;          /* User handle */
      evt.uDat2 = pSesFrom->uHdl;      /* Session handle of the call to transfer (pSesFrom -> pSesTo) */
      evt.pDat3 = NULL;                /* pData */
      evt.uDat4 = pSesFrom->uNetUsr;   /* CCTK user id */
      evt.bLast = BOS_TRUE;            /* Last event */

      /* Run the FSM right away */
      memcpy( &pUsr->evt, &evt, sizeof( CMEVT ) );
      cmUsrNewEvent( pUsr, (CMGR_EVT_CLASS__EVT_MASK & evt.uEvt) );
      cmFsmCctkRun( pUsr );

      /* Make sure the new session is now focused for this user */
      if( pSesTo_OtherNew )
      {
         pSesTo_OtherNew->ses.med.bFocus = BOS_TRUE;
      }
   } /* End blind call transfer case */
}

/***********************************************************************************
** FUNCTION:   cmFsmCallSwitchActive
**
** PURPOSE:    Toggle active and waiting calls.
**
** PARAMETERS: pRes - the pointer to the resource.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallSwitchActive ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );

   CMGRLOG_INFORMATION (( "cmFsmCallFlashSwitchActive: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetNtwkHdl( pUsr ) ));

   /* Two sessions are active.
   **
   ** One is a call waiting session, the other is the currently active
   ** session.  Call waiting takes precedence over call conferencing, so
   ** we apply call waiting processing here.
   **
   */
   CMTMR tmr;
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   tmr.uSes = pUsr->pSesWait->uHdl;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_CALLWAIT;

   cmTmrStop ( &tmr );

   /* If this is an external call, tell CCTK to put the call on hold. */
   if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
   {
      /* Put the focus call on hold.
      */
      cctkCallHold ( pSes->uNet );
   }
   /* If this is an internal call, run the hold state machine right away */
   else
   {
      /* Send a hold event */
      CMEVT evt;

      memset ( &evt, 0, sizeof( CMEVT ) );

      evt.uEvt = ( eCMEVT_HOLD_LOCAL | CMGR_EVT_CLASS__CCTK ); /* Event */
      evt.uDat1 = pSes->uUsr;     /* User handle */
      evt.uDat2 = pSes->uHdl;     /* Session handle */
      evt.uDat4 = pSes->uNetUsr;  /* CCTK user id */
      evt.bLast = BOS_TRUE;       /* Last event */

      if ( cmPostEvt ( &evt ) == BOS_STATUS_OK )
      {
         cmSignalEvt ();
      }
   }
}