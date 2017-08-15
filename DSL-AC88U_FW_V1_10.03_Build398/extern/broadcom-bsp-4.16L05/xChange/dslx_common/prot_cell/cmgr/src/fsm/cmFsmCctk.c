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
**      This is the FSM for CCTK interface events.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
static const CCTKCLIDINFO defaultClid =
{
   "",      /* Name */
   "P"      /* Number */
};


/* The finite state machine for handling the CCTK events.
*/
static const CMFSMVOIDFUNCTIONPTR
   gpCmCctkFsm[eCMST_MAX][eCMEVT_CCTK_END - eCMEVT_CCTK_START - 1] =
{
   /* ---- eCMST_IDLE ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallNew,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmCallAlert,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_ACCEPTED ------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmCallAlert,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_DIALING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallRejected,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmCallRingback,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmCallBusy,
   },

   /* ---- eCMST_RINGBACK ------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallRejected,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmCallBusy,
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnding,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnding,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_TALK ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmCallConnect,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_RELEASING ------------------------------------------------------ */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallNew,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },

   /* ---- eCMST_ERROR ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_INCOMING_CALL ---------- */ &cmFsmCallRejectBusy,
      /* ---- eCMEVT_CLEANED_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_ALERT_CALL ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RINGING_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_CONNECT_CALL ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_RELEASE_CALL ----------- */ &cmFsmCallEnd,
      /* ---- eCMEVT_BUSY_CALL -------------- */ &cmFsmNoOp,
   },
};

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFsmCctkRun
**
** PURPOSE:    Main entry for running the CCTK FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCctkRun ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   BOS_UINT32 uSta;
   BOS_UINT32 uEvt;

   uSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
   uEvt = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ];

   CMGRLOG_INFORMATION (( "cmFsmCctkRun: usr 0x%X - [%s (0x%X), %s (0x%X)]",
                          pUsr->uHdl,
                          (char *) cmMapGetStr ( cmMapCmState, uSta ), uSta,
                          (char *) cmMapGetStr ( cmMapCmEvent, uEvt ), uEvt ));

   if ( uEvt != eCMEVT_INVALID )
   {
      (gpCmCctkFsm [ uSta ][ uEvt - (eCMEVT_CCTK_START + 1) ]) ( pRes );
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmInSrv
**
** PURPOSE:    FSM in-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInSrv ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   CMGRLOG_INFORMATION (( "cmFsmInSrv: usr 0x%X, net 0x%X",
                          pUsr->uHdl,  uAccount ));

   /* Validate that the network user handle is the one currently assigned
   ** to the local user.  If it is not, ignore the event and warn the user.
   ** If it is, continue with processing.  This check is in place because
   ** an 'in-service' event may be triggered at the same time as an 'update'
   ** event from the network layer but in the end at this level we want to
   ** process only the event that is associated with the correct user owning
   ** the resource.
   */
   if ( cmUsrGetNtwkHdl( pUsr ) != pUsr->evt.uDat2 )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmInSrv [IGNORE]: usr 0x%X - (net 0x%X != evt-net 0x%X)",
         pUsr->uHdl, uAccount, pUsr->evt.uDat2 ));

      /* Exit now, the event does not need to be processed such that no
      ** false information is being reported.
      */
      return;
   }

   /* User is in service.
   */
   pUsr->bInSrv = BOS_TRUE;

   if ( gCmCfgCb.cmgrEvtCb )
   {
      ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                               pUsr->uHdl,
                               pUsr->uHdl,
                               CMGR_INVALID_HANDLE,
                               (void *) pUsr->uUid );
   }


   /*If already off-hook, and configured to not generate a tone for out of service
     call off-hook state handler so in-service behavior takes place*/
   if (pUsr->bOffHook)
   {
      CFGPROVDEVICECFG *pDevCfg = NULL;
      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                              &cfgIdx,
                              (void *) &pDevCfg,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pDevCfg && pDevCfg->bNoToneOutOfSrv )
         {
            cmFsmOffHook ( (void *) pUsr );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( PRV, DevConcept ),
                   &cfgIdx,
                   (void *) pDevCfg,
                   CFG_ITEM_DEL );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOutOfSrv
**
** PURPOSE:    FSM out-of-service handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOutOfSrv ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   BOS_UINT32 uIx;
   BOS_BOOL bFree = BOS_FALSE;
   BOS_BOOL bIncSes = BOS_FALSE;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bEndAllCalls = BOS_FALSE;
   BOS_UINT32 uAccount = cmUsrGetNtwkHdl( pUsr );

   CMGRLOG_WARNING (( "cmFsmOutOfSrv: usr 0x%X, net 0x%X!!!",
                       pUsr->uHdl,  uAccount ));


   /* User is out of service (lost registration).
   */
   pUsr->bInSrv = BOS_FALSE;




   if ( gCmCfgCb.cmgrEvtCb )
   {
      CMGREVT eEvent = eCMGREVT_OPER_NOK;
      ( gCmCfgCb.cmgrEvtCb ) ( eEvent,
                               pUsr->uHdl,
                               pUsr->uHdl,
                               CMGR_INVALID_HANDLE,
                               NULL );
   }

   /* Determine whether active calls need to be terminated.
   */
   memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
   cfgIdx.pcCharIdx = pUsr->uUid;
   if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                              &cfgIdx,
                              (void *) &pBasicCall,
                              CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pBasicCall &&
           pBasicCall->bEndCallsOnOos )
      {
         bEndAllCalls = BOS_TRUE;
      }

      gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                            &cfgIdx,
                            (void *) pBasicCall,
                            CFG_ITEM_DEL );
      pBasicCall = NULL;
   }


   /* Transient session - as well as possibly active ones depending on the
   ** desired configured behavior - for this user should be terminated now.
   **
   ** One exception - unless instructed to terminate all resources - is if
   ** we have received an incoming call for this user and we consider that
   ** it is valid scenario to stay in service.
   */
   for ( uIx = 0 ; uIx < CMGRCFG_MEDSES_MAX + 1 ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] &&
           ( /* Transient session(s). */
             !pUsr->pMedSes [ uIx ]->bEstab ||
             /* Or established session with configured behavior overrule. */
             ( pUsr->pMedSes [ uIx ]->bEstab && bEndAllCalls ) ) )
      {
         if ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING ) &&
              ( pUsr->pMedSes [ uIx ] == cmUsrFocusSes( pUsr ) ) &&
              !bEndAllCalls )
         {
            bIncSes = BOS_TRUE;
         }
         else
         {
            CMSES *pFreeSes = pUsr->pMedSes [ uIx ];

            CMGRLOG_INFORMATION ((
               "cmFsmOutOfSrv: Terminating call 0x%x (0x%X) for out-of-service user 0x%X (0x%X)",
               pFreeSes->uHdl, pFreeSes->uNet, pUsr->uHdl,  uAccount ));

            if( pFreeSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cmSesTerminate ( pFreeSes );
            }
            bFree = BOS_TRUE;
         }
      }
      /* If the session was configured to be maintained, then the mark
      ** it as such so that special processing can be applied later if
      ** needed.
      */
      else if (  !bEndAllCalls &&
                 pUsr->pMedSes [ uIx ] && pUsr->pMedSes [ uIx ]->bEstab )
      {
         CMGRLOG_INFORMATION ((
               "cmFsmOutOfSrv: Maintaining call 0x%x (0x%X) for out-of-service user 0x%X (0x%X)",
               pUsr->pMedSes [ uIx ]->uHdl, pUsr->pMedSes [ uIx ]->uNet, pUsr->uHdl,  uAccount ));

         pUsr->pMedSes [ uIx ]->bMaintdOnOos = BOS_TRUE;
      }
   }


   /* Revert the out-of-service decision if needed (there is an incoming
   ** call from the network, we assume we are in service even if registration
   ** says otherwise).
   */
   if ( bIncSes )
   {
      pUsr->bInSrv = BOS_TRUE;

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_OPER_OK,
                                  pUsr->uHdl,
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
                                  (void *) pUsr->uUid );
      }
   }
   /* Update state if needed.
   */
   else if ( bFree )
   {
      cmUsrNewState ( pUsr, eCMST_RELEASING );

      if ( ( cmUsrActiveSes ( pUsr ) == NULL ) &&
           ( cmUsrFocusSes ( pUsr ) == NULL ) &&
           ( pUsr->bOffHook ) )
      {
         CMGREPSIG sig;
         CMTMR tmr;

         memset ( (void *) &sig,
                  0,
                  sizeof ( CMGREPSIG ) );

         memset ( (void *) &tmr,
                  0,
                  sizeof ( CMTMR ) );

         /* Terminate any possible signal being played.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uSig = pUsr->uCpt;
         sig.uCid = CMGR_INVALID_HANDLE;
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         /* Terminate any possible timer.
         */
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_INVALID;

         cmTmrStop ( &tmr );

         /* Play reorder tone and start off-hook warning timer.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = CMGR_INVALID_HANDLE;
         sig.uSig = eEPTSIG_REORDER;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_OHWARN;
         tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;

         cmTmrStart ( &tmr );
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmCallNew
**
** PURPOSE:    FSM new incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallNew ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = NULL;
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );
   BOS_BOOL bAccept = BOS_FALSE;
   BOS_BOOL bTmrRun = BOS_FALSE;
   BOS_BOOL bEmergency = BOS_FALSE;
   BOS_BOOL bCallWait = BOS_FALSE;
   BOS_BOOL bHotlineNoCW = BOS_FALSE;
   BOS_BOOL bProxy = BOS_FALSE;
   BOS_BOOL bFwd = BOS_FALSE;
   BOS_BOOL bAlertInfoRS = BOS_FALSE;
   CMTMR tmr;
   CFGSIPCALLFWD *pFwdCfg;
   CFGSIPHOTLINE *pHotLineCfg = NULL;
   PROV_CFG_IDX cfgIdx;

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_ONHOOK;
   bTmrRun = cmTmrIsRunning ( &tmr );
 

   if ( pFocusSes )
   {
      /* TODO: When multiple calls are supported, determine
      ** if this is a call waiting scenario.
      */
      {
         cmFsmCallRejectBusy ( pRes );
         /* Do not process any longer.
         */
         return;
      }
   }

   /* Sanity check to ensure we are not max'ed out in resources usage as well
   ** as we can still associate resources to this user.
   **
   ** [1] Cannot accept new call if one is already held (i.e. hold activate is
   **     enabled for this call).  This is because it would collide with call
   **     waiting feature usage.
   **
   ** [2] Cannot accept new call if one is already 'waiting'.
   **
   ** [3] Cannot accept new call if an auto-call is ongoing (auto recall or auto
   **     callback) because of resources limitations.
   **
   ** [4] Cannot accept new call if already engaged in a 3-way call.
   **
   ** [5] Cannot accept new call if already engaged in an emergency call and this
   **     new call is not an emergency call as well.
   **
   ** [6] Finally, cannot accept new call if we cannot allocate a session for
   **     it (obvious reason).
   */

   if ( ( pSes = cmSesAlloc ( eCMSESTYPE_MEDIA, pUsr->uHdl ) ) != NULL )
   {
      /* Associate this local resource with the network one.
      */
      pSes->uNet = pUsr->evt.uDat2; /* CallId handle */
      pSes->uNetUsr = pUsr->evt.uDat4; /* CCTK user handle */

      if ( cmUsrLinkSes ( pUsr, pSes ) )
      {
         bAccept = BOS_TRUE;

         /* Keep track that this session was originated by the remote
         ** end as opposed to by the local user.
         */
         pSes->ses.med.bRemInit = BOS_TRUE;
      }
   }

   /* If we can accept this call, we also need to ensure we have physical
   ** resource to be able to have media working on it.
   */
   if ( bAccept && ( pUsr->uPhys == CMGR_INVALID_HANDLE ) )
   {
       bAccept = ( cmPhyAllocate ( pUsr ) == eCMGRSTATUS_COMMAND_SUCCESS );
   }

   /* Publish a 'line active' event as necessary.
   */
   if ( gCmCfgCb.cmgrEvtCb &&
        bAccept )
   {
      gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_ACTIVE,
                           pUsr->uPhys,
                           pUsr->uHdl,
                           CMGR_INVALID_HANDLE,
                           NULL );
   }
   /* Finally, if we are in a 'off-hook' situation on this user and the user
   ** is not engaged in a different call already, this could be a problem, likely
   ** an off-hook warning tone is being played anyways, reject the call (unless
   ** in special emergency case).
   */
   if ( pUsr->bOffHook &&
        ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] != eCMST_TALK ) )
   {
      bAccept = BOS_FALSE;
   }

   /* Cannot accept the call for resources reason.
   */
   if ( !bAccept )
   {
      CMGRLOG_WARNING (( "cmFsmCallNew: Fail accept call on 0x%X (0x%X)",
                         pUsr->uHdl, pUsr->evt.uDat4 ));

      /* Release the network call.
      */
      cmFsmCallRejectBusy ( pRes );

      if ( pSes )
      {
         cmUsrUnLinkSes ( pUsr, pSes );
         /* Reset the network handle and free the session resource.
         */
         pSes->uNet = CMGR_INVALID_HANDLE;
         cmSesFree ( pSes, pUsr->uHdl );
      }
   }
   else
   {
      pSes->uApp = pSes->uHdl;

      /* Update the media capabilities to be used for this call.
      */
      CMGRLOG_DEBUG (( "cmFsmCallNew: Accepting 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( gCmCfgCb.cmgrEvtCb )
      {
         CMGRCALLDATA tData;

         tData.eType = eCMGRCALLTYPE_INCOMING;
         tData.uSrc = CMGR_INVALID_HANDLE;

         /* For the internal call case, find and fill in the source terminal.
         */
         if( pSes->uNetUsr == CMGR_INVALID_HANDLE )
         {
            CMSESLIST sesList;
            BOS_UINT32 uIx;

            memset( &sesList, 0, sizeof( CMSESLIST ) );

            cmSesListWithCid( pSes->uNet, &sesList );
            for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
            {
               CMSES* pSesSrc = cmSesValid( sesList.uSes[ uIx ] );

               if( ( pSesSrc ) && ( pSesSrc->bEstab == BOS_TRUE ) )
               {
                  tData.uSrc = pSesSrc->uUsr;
                  break;
               }
            }
         }

         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_NEW,
                                  pSes->uNet,
                                  pUsr->uHdl,
                                  CMGR_INVALID_HANDLE,
                                  (void *) &tData );
      }

      /* If this is the only call on the system and we are otherwise idle,
      ** transition to a state where can handle race condition with user
      ** action (hook event).
      */
      if ( !pUsr->bOffHook &&
           ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_IDLE ) )
      {
         cmUsrNewState ( pUsr, eCMST_ACCEPTED );
      }
      
      pSes->ses.med.bNeedFinal = BOS_TRUE;
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRejectBusy
**
** PURPOSE:    FSM reject busy incoming call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejectBusy ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   if( pUsr )
   {
      CMGRLOG_WARNING (( "cmFsmCallRejectBusy: 0x%X on 0x%X (0x%X)",
                            pUsr->evt.uDat2, pUsr->uHdl, pUsr->evt.uDat4 ));


      /* Reject the network call.
      **
      ** Note that the network handle is carried within the event information because
      ** there is no session resource that we have associated with this incoming call
      ** from the network, so there is no media session reference from the user block.
      */
      cmAppNetCallRelease ( pUsr->evt.uDat2 );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallEnded
**
** PURPOSE:    FSM remote terminated call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnded ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGREPSIG sig;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmCallEnded: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));


      if( cmNumSesWithCid( pSes->uNet ) == 1 )
      {
         if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            /* Terminate the network resource.
            */
            cmAppNetCallRelease ( pSes->uNet );
         }
      }

      /* If this is the primary focus call, we need to inform the user that
      ** it is terminated now, user may choose to hang up, or swap to a
      ** different call (via hook flash or hook flash + feature code).
      */
      if ( pSes->ses.med.bFocus )
      {
         /* If we are not already playing busy tone.
         */
         if ( pUsr->uCpt != eEPTSIG_BUSY )
         {
            /* Play some re-order tone such that local user hangs up.
            */
            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = pSes->uNet;
            sig.uSig = eEPTSIG_REORDER;
            pUsr->uCpt = sig.uSig;

            cmPhySignal ( pUsr->uPhys, &sig );
         }
      }

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_ENDED,
                                  pSes->uNet,
                                  pSes->uUsr,
                                  CMGR_INVALID_HANDLE,
                                  (void *) pUsr->uUid );
      }

      cmUsrNewState ( pUsr, eCMST_RELEASING );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallConnect
**
** PURPOSE:    FSM remote connected call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallConnect ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bClidName = BOS_TRUE;
   BOS_BOOL bClidNumber = BOS_TRUE;
   CFGSIPCIDDISPLAY *pUsrClidDsp = NULL;
   int uIx;
   CMSES *pOtherSes = NULL;

   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmCallConnect: 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Callee identification update (if needed).
      */
      if ( pUsr->evt.pDat3 )
      {
         memcpy ( &pSes->ses.med.callInfo,
                  pUsr->evt.pDat3,
                  sizeof ( CMNETCALLINFO ) );
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

            gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CIDDisplayConcept ),
                                  &cfgIdx,
                                  (void *) pUsrClidDsp,
                                  CFG_ITEM_DEL );
            pUsrClidDsp = NULL;
         }
      }

      /* Issue callee identification information if necessary.
      */
      if ( pUsr->evt.pDat3 && (bClidName || bClidNumber) )
      {
         cmSesClid ( pSes, bClidName, bClidNumber );
      }

      /* Terminate temporary monitoring of the media activity for this
      ** connection.
      */
      if ( !pSes->ses.med.bBgFocus )
      {
         cmSesPhyActMonThresh ( pSes, 0 );
      }

      if ( pSes->ses.med.bFocus ||
           (pSes->ses.med.uConf != CMGR_INVALID_HANDLE && !pSes->ses.med.bBgFocus) )
      {
         /* Terminate the ring-back tone.
         */
         CMGREPSIG sig;

         CMTMR tmr;

         memset ( &sig, 0, sizeof ( CMGREPSIG ) );
         memset ( &tmr, 0, sizeof ( CMTMR ) );

         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
         sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGBACK */
         pUsr->uCpt = eEPTSIG_NULL;

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_RINGBACK;

         cmTmrStop ( &tmr );

         cmPhySignal ( pUsr->uPhys, &sig );

      }
      /* If this is an internal call, we need to create the physical
      ** session information here.
      */
      if( pSes->ses.med.uPhy == CMGR_INVALID_HANDLE )
      {
         cmSesPhyCreate( pSes );
      }

      if( !pSes->bEstab )
      {
         pSes->bEstab = BOS_TRUE;
      }


      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_CONNECT,
                                  pSes->uNet,
                                  pSes->uUsr,
                                  CMGR_INVALID_HANDLE,
                                  (void *) pUsr->uUid );
      }

      if ( !pSes->ses.med.bBgFocus )
      {
         cmUsrNewState ( pUsr, eCMST_TALK );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRingback
**
** PURPOSE:    FSM remote ringing call handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRingback ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bPresetMedSes = BOS_FALSE;

   if ( pSes )
   {
      CMTMR tmr;

      memset ( &tmr, 0, sizeof ( CMTMR ) );

      CMGRLOG_DEBUG (( "cmFsmCallRingback: 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( pSes->ses.med.bFocus && !pSes->ses.med.bMediaRx )
      {
         /* Start the ring-back tone.
         */
         CMGREPSIG sig;


         memset ( &sig, 0, sizeof ( CMGREPSIG ) );

         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSes->uApp;
         sig.uCid = pSes->uNet;
         sig.uSig = eEPTSIG_RINGBACK;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );
      }

      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_RINGBACK;
      tmr.uTimeOut = CMGR_RINGBACK_TONE_TIMEOUT;

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall )
         {
            tmr.uTimeOut = pBasicCall->uOrigRBKTimer;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      cmTmrStart ( &tmr );

      /* Inform application that the call is ringing remotely.
      */
      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_RINGBACK,
                                  pSes->uNet,
                                  pSes->uUsr,
                                  CMGR_INVALID_HANDLE,
                                  NULL );
      }

      cmUsrNewState ( pUsr, eCMST_RINGBACK );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallEnding
**
** PURPOSE:    FSM call ending while ringing handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallEnding ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );
   CMGREPSIG sig;
   CMTMR tmr;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   CMGRLOG_DEBUG (( "cmFsmCallEnding: 0x%X (0x%X)", pUsr->uHdl, ( pSes ) ? pSes->uNetUsr : CMGR_INVALID_HANDLE ));

   /* If there is no focus session or this is the focus session being
   ** terminated, stop the ring.  Otherwise, it is a transient session being
   ** terminated and we should not stop the ring.
   */
   if ( ( pFocusSes == NULL ) || ( pSes && ( pFocusSes == pSes ) ) )
   {
      tmr.uSes = pSes->uHdl;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_RINGING;

      cmTmrStop ( &tmr );

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGING */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      if ( pSes && ( pSes->ses.med.uPhy != CMGR_INVALID_HANDLE ) )
      {
         /* We would be playing ringback to the peer. Terminate it.
         */
         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = pSes->ses.med.uPhy;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
         sig.uSig = eEPTSIG_RINGBACK;

         cmPhySignal ( pUsr->uPhys, &sig );
     }
   }

   cmFsmCallEnd ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmCallRejected
**
** PURPOSE:    FSM call ended in rejection while attempting handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallRejected ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pFocusSes = cmUsrFocusSes ( pUsr );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallRejected: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Clean up this call since terminated.
      */
      cmFsmCallEnd( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmCallBusy
**
** PURPOSE:    FSM received busy treatment.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallBusy ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMGREPSIG sig;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;


   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallBusy: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

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
            cmFsmOsi ( pRes );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      if ( pSes->ses.med.bFocus )
      {
         /* Play some busy tone such that local user hangs up.
         */
         sig.eAct = eCMGREPSIGACT_START;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = pSes->uApp;
         sig.uCid = pSes->uNet;
         sig.uSig = eEPTSIG_BUSY;
         pUsr->uCpt = sig.uSig;

         cmPhySignal ( pUsr->uPhys, &sig );
      }

      /* Terminate this call now.
      */
      cmFsmCallEnded( pRes );
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmCallAlert
**
** PURPOSE:    FSM incoming call alert handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallAlert ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
   CMSES *pOtherSes = NULL;


   /* If the event had arrived early, the handle to lookup may be the
   ** actual network handle.  Give it another chance.
   */
   if ( pSes == NULL )
   {
      pSes = cmSesFindNetHdl ( pUsr->evt.uDat2, pUsr->uHdl  );

      /* Update the event content to reflect the reality now that we
      ** have properly identified the session owner.
      */
      if ( pSes )
      {
         pUsr->evt.uDat2 = pSes->uHdl;
      }
   }

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmCallAlert: 0x%X (0x%X) on 0x%X (0x%X) pDat (%p)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr, pUsr->evt.pDat3 ));

      /* Copy the call information into the session block.
      */
      if ( pUsr->evt.pDat3 )
      {
         memcpy ( &pSes->ses.med.callInfo,
                  pUsr->evt.pDat3,
                  sizeof ( CMNETCALLINFO ) );
      }
      else
      {
         memset ( &pSes->ses.med.callInfo,
                  0,
                  sizeof ( CMNETCALLINFO ) );
         memcpy ( &pSes->ses.med.callInfo.clid,
                  &defaultClid,
                  sizeof ( CCTKCLIDINFO ) );
      }


      /* Already in active media mode, call waiting scenario.
      */
      if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_TALK )
      {
         /* TODO: call waiting handling here
         */
      }
      else
      {
         CFGSIPEMERGE *pEmergCfg = NULL;
         PROV_CFG_IDX cfgIdx;
         BOS_BOOL bAcptIncEmerg = BOS_FALSE;

         /* Make this session the focus session.
         */
         pSes->ses.med.bFocus = BOS_TRUE;

         cmFsmRing ( pRes );
      }
   }
}

