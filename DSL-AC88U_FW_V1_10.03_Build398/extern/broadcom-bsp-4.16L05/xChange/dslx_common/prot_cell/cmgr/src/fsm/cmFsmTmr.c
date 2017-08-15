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
**      This module implements the finite state machine that runs the Call Manager
**      application.
**
**      This is the FSM for timer events.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* The finite state machine for handling the Physical Interface (endpoint) events.
*/
static const CMFSMVOIDFUNCTIONPTR
   gpCmTmrFsm[eCMST_MAX][eCMEVT_TMR_END - eCMEVT_TMR_START - 1] =
{
   /* ---- eCMST_IDLE ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmOnHookGuard,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmOffHookWarn,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmOsi,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_ACCEPTED ------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_DIALING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmDialFailure,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmDigitTimer,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmDialFailure,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_RINGBACK ------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmConnectFailure,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp, /* was cmFsmNoAnswer */
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_TALK ----------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmOnHookGuard,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmDigitTimer,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp, /* TODO: remove? */
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmFlashTimer,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_RELEASING ------------------------------------------------------ */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmOnHookGuard,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmOffHookWarn,
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmOsi,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_ERROR ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmOffHookWarn,
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },
};


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmFsmTmrRun
**
** PURPOSE:    Main entry for running the TMR FSM.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmTmrRun ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   BOS_UINT32 uSta;
   BOS_UINT32 uEvt;

   uSta = pUsr->eSta[ CMFSM_CURRENT_STATE_IX ];
   uEvt = pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ CMFSM_CURRENT_EVENT_IX ];

   CMGRLOG_INFORMATION (( "cmFsmTmrRun: usr 0x%X - [%s (0x%X), %s (0x%X)]",
                          pUsr->uHdl,
                          (char *) cmMapGetStr ( cmMapCmState, uSta ), uSta,
                          (char *) cmMapGetStr ( cmMapCmEvent, uEvt ), uEvt ));

   if ( uEvt != eCMEVT_INVALID )
   {
      (gpCmTmrFsm [ uSta ][ uEvt - (eCMEVT_TMR_START + 1) ]) ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOnHookGuard
**
** PURPOSE:    FSM on-hook guard timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOnHookGuard ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes;
   CMSES *pOtherSes = NULL;
   CMGREPSIG sig;
   CMTMR tmr;
   BOS_UINT32 uIx;
   BOS_BOOL bXfer = BOS_FALSE;
   BOS_BOOL bCW = BOS_FALSE;
   CFGSIPBASICCALL *pBasicCall = NULL;
   CFGSIPCALLXFER *pCfgXfer = NULL;
   PROV_CFG_IDX cfgIdx;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   if ( pUsr->bOffHook )
   {
      /* If the user has gone back off-hook, ignore the timer event in case it
      ** was not terminated previously (no big deal!).
      */
      CMGRLOG_DEBUG (( "cmFsmOnHookGuard: ignoring due to off-hook on 0x%X",
                       pUsr->uHdl ));
      return;
   }

   if ( ( pSes = cmUsrFocusSes ( pUsr ) ) != NULL )
   {
      CMGRLOG_INFORMATION (( "cmFsmOnHookGuard: Ending 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      pOtherSes = NULL;
      for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
      {
         if ( pUsr->pMedSes [ uIx ] )
         {
            if ( pUsr->pMedSes [ uIx ]->uHdl != pSes->uHdl )
            {
               pOtherSes = pUsr->pMedSes [ uIx ];
               cmSesTerminate ( pOtherSes );
            }
         }
      }
         
      cmSesTerminate ( pSes );

      cmUsrNewState ( pUsr, eCMST_RELEASING );
   }

   /* If there are transient session (not established), clean them up as well.
   */
   pSes = NULL;
   for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {

         pSes = pUsr->pMedSes [ uIx ];
      }
   }

   /* If there is no session left, then that is all.  If there is some
   ** session left, then we need to activate ring reminder and it becomes
   ** the de-facto focus session.
   */
   if ( pSes )
   {
      CMGRLOG_WARNING (( "cmFsmOnHookGuard: unreleased 0x%X (0x%X) on 0x%X (0x%X)!",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

   }
   /* Release the physical resource allocated to this user now that we
   ** do not need it for anything.
   */
   else
   {
      cmUsrNewState ( pUsr, eCMST_IDLE );

      /* Publish a 'line inactive' event.
      */
      if ( gCmCfgCb.cmgrEvtCb &&
           (pUsr->eType != eCMUSRTYPE_REMOTE) )
      {
         gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                              pUsr->uPhys,
                              pUsr->uHdl,
                              CMGR_INVALID_HANDLE,
                              NULL );
      }

      /* If we are pending on an action, don't release
      ** the resource.
      */
      if ( pUsr->bPendingActPhy == BOS_FALSE ) 
      {
         cmUsrPhyDisassociate( pUsr );
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmRenewSvc
**
** PURPOSE:    FSM renewal of service timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmRenewSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      /* Renew the service at this time.
      */
      CMGRLOG_DEBUG (( "cmFsmRenewSvc: srv 0x%X (0x%X), usr 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmTimeoutSvc
**
** PURPOSE:    FSM waiting for confirmation of a service timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmTimeoutSvc ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      /* Timeout on service.  We can either try to renew it or
      ** simply cancel it.  Depending on the service type.
      */
      CMGRLOG_DEBUG (( "cmFsmTimeoutSvc: srv 0x%X (0x%X), usr 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDialFailure
**
** PURPOSE:    FSM waiting for initial digit dialing timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDialFailure ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );
   CMTMR tmr;
   CMGREPSIG sig;

   memset ( &tmr,
            0,
            sizeof ( CMTMR ) );

   memset ( &sig,
            0,
            sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      cmUsrUnLinkSes ( pUsr, pSes );
      cmSesFree ( pSes, pUsr->uHdl );
   }
      
   if ( pUsr->uCpt != eEPTSIG_NULL )
   {
      /* Stop dial tone if playing.
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );
   }

   /* If there is an active call on hold and no digit was ever detected
   ** in this dialing phase, we need to apply some special processing to
   ** it.
   */
   if ( ( pUsr->pSesHeld != NULL ) && ( pUsr->eDigDetState == CMDIGITDETSTA_NONE ) )
   {
      /* Mark the fact that we want to preserve the held state.
      */
      pUsr->bPresrvHeld = BOS_TRUE;

      /* Go straight into error state to apply origination mode extended
      ** offhook processing treatment.
      */
      cmUsrNewState ( pUsr, eCMST_ERROR );

      tmr.uSes = CMGR_INVALID_HANDLE;
      tmr.uUsr = pUsr->uHdl;
      tmr.eType = eCMEVT_TMR_OHWARN;
      tmr.uTimeOut = CMGR_IMMEDIATE_TIMEOUT;

      cmTmrStart ( &tmr );
   }
   else
   {
      /* Enter the extended off-hook error state and apply necessary
      ** procedure.
      */
      cmUsrNewState ( pUsr, eCMST_ERROR );

      /* Start applying the process right away.
      */
      cmFsmOffHookWarn ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmConnectFailure
**
** PURPOSE:    FSM waiting for remote connection answer timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmConnectFailure ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmConnectFailure: Ending 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
      {
         /* Terminate the network call.
         */
         cmAppNetCallRelease ( pSes->uNet );
      }

      cmUsrNewState ( pUsr, eCMST_RELEASING );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmDigitTimer
**
** PURPOSE:    FSM waiting for digit information timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDigitTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );

   if ( pSes )
   {
      /* If we just received the long interdigit timer, chances are the
      ** previous event was the short interdigit timer.  In such case, we
      ** check whether to 'remove' the short interdigit timer from the
      ** dial string since it would have been processed already and lead to
      ** no actions.
      */
      if ( pUsr->evt.uEvt == eCMEVT_TMR_INTERDIGIT_LONG )
      {
         if ( pSes->ses.med.uDigCol[ pSes->ses.med.uDigCnt - 1 ] ==
                                         (BOS_UINT8)CMDM_INTERDIGIT_TMR_SHORT )
         {
            pSes->ses.med.uDigCnt--;
         }
      }
   }

   /* Pass the event to the digit manipulation engine for analysis and
   ** to see what to do next.
   */
   cmFsmDigitDetect ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmFlashTimer
**
** PURPOSE:    FSM waiting for information during flash timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmFlashTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   /* If we just received the long interdigit timer, chances are the
   ** previous event was the short interdigit timer.  In such case, we
   ** check whether to 'remove' the short interdigit timer from the
   ** dial string since it would have been processed already and lead to
   ** no actions.
   */
   if ( pUsr->evt.uEvt == eCMEVT_TMR_INTERDIGIT_LONG )
   {
      if ( pUsr->uDigCol[ pUsr->uDigCnt - 1 ] ==
                                     (BOS_UINT8)CMDM_INTERDIGIT_TMR_SHORT )
      {
         pUsr->uDigCnt--;
      }
   }

   /* Pass the event to the digit manipulation engine for analysis and
   ** to see what to do next.
   */
   cmFsmFeature ( pRes );
}


/***********************************************************************************
** FUNCTION:   cmFsmKeyTimer
**
** PURPOSE:    FSM key hold monitoring timer expired for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmKeyTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );

   if ( pSes )
   {
      BOS_SINT32 uIx;

      /* The long duration key press timer has expired for this session, we need
      ** to mark it as such in the digit collected string and pass the previous
      ** digit back to the digit manipulation engine.
      */
      pSes->ses.med.uDigCol[ pSes->ses.med.uDigCnt - 1 ] =
                                            (BOS_UINT8) CMDM_LONG_DURATION_TMR;

      for ( uIx = CMGRCFG_EVENT_MAX ; uIx >= 0 ; uIx-- )
      {
         if ( ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ uIx ] >= eCMEVT_DTMF_0 ) &&
              ( pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ uIx ] <= eCMEVT_DTMF_D ) )
         {
            cmUsrNewEvent ( pUsr,
                            pUsr->eEvt[ CMFSM_CURRENT_STATE_IX ] [ uIx ] );
            break;
         }
      }

      cmFsmDigitDetect ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmOffHookWarn
**
** PURPOSE:    FSM off-hook warning timer expiry processing.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmOffHookWarn ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;

   if ( pUsr->bOffHook )
   {
      CMGREPSIG sig;
      CMTMR tmr;
      CFGSIPBASICCALL *pBasicCall = NULL;
      PROV_CFG_IDX cfgIdx;

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );
      memset ( &tmr, 0, sizeof ( CMTMR ) );

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = CMGR_INVALID_HANDLE;
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      switch ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] )
      {
         case eCMST_ERROR:
         {
            /* If we already went through all the error stages, we are now locked
            ** out indefinitively, until the user is back on hook.
            */
            if ( pUsr->eError >= eCMRESERROR_LEVEL4 )
            {
               pUsr->eError = eCMRESERROR_LOCKOUT;
            }
            else
            {
               /* Mark the next error state.
               */
               pUsr->eError++;

               memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
               cfgIdx.pcCharIdx = pUsr->uUid;
               if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                          &cfgIdx,
                                          (void *) &pBasicCall,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
               {
                  switch ( pUsr->eError )
                  {
                     case eCMRESERROR_LEVEL1:
                     {
                        tmr.uTimeOut = pBasicCall->uPermSeqTimer1;
                        sig.uSig = CMGR_INVALID_HANDLE;
                        if ( pBasicCall->pPermSeqTone1 )
                        {
                           sig.uSig =
                              cmPhyToneFromString ( pBasicCall->pPermSeqTone1 );
                        }
                        if ( sig.uSig == CMGR_INVALID_HANDLE )
                        {
                           sig.uSig = eEPTSIG_OHWARN;
                        }
                     }
                     break;

                     case eCMRESERROR_LEVEL2:
                     {
                        tmr.uTimeOut = pBasicCall->uPermSeqTimer2;
                        sig.uSig = CMGR_INVALID_HANDLE;
                        if ( pBasicCall->pPermSeqTone2 )
                        {
                           sig.uSig =
                              cmPhyToneFromString ( pBasicCall->pPermSeqTone2 );
                        }
                        if ( sig.uSig == CMGR_INVALID_HANDLE )
                        {
                           sig.uSig = eEPTSIG_OHWARN;
                        }
                     }
                     break;

                     case eCMRESERROR_LEVEL3:
                     {
                        tmr.uTimeOut = pBasicCall->uPermSeqTimer3;
                        sig.uSig = CMGR_INVALID_HANDLE;
                        if ( pBasicCall->pPermSeqTone3 )
                        {
                           sig.uSig =
                              cmPhyToneFromString ( pBasicCall->pPermSeqTone3 );
                        }
                        if ( sig.uSig == CMGR_INVALID_HANDLE )
                        {
                           sig.uSig = eEPTSIG_OHWARN;
                        }
                     }
                     break;

                     case eCMRESERROR_LEVEL4:
                     {
                        tmr.uTimeOut = pBasicCall->uPermSeqTimer4;
                        sig.uSig = CMGR_INVALID_HANDLE;
                        if ( pBasicCall->pPermSeqTone4 )
                        {
                           sig.uSig =
                              cmPhyToneFromString ( pBasicCall->pPermSeqTone4 );
                        }
                        if ( sig.uSig == CMGR_INVALID_HANDLE )
                        {
                           sig.uSig = eEPTSIG_OHWARN;
                        }
                     }
                     break;

                     default:
                     break;
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
                  sig.uSig = eEPTSIG_OHWARN;
               }

               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_OHWARN;

               /* If a timeout is defined for the state, we will apply the
               ** state action and timeout.
               */
               if ( tmr.uTimeOut )
               {
                  cmTmrStart ( &tmr );

                  sig.eAct = eCMGREPSIGACT_START;
                  sig.uSes = CMGR_INVALID_HANDLE;
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = CMGR_INVALID_HANDLE;
                  pUsr->uCpt = sig.uSig;

                  cmPhySignal ( pUsr->uPhys, &sig );
               }
               /* If no timeout is defined (zero duration), we need to skip
               ** this state action now and move to the 'lockout' state right
               ** away.
               */
               else
               {
                  pUsr->eError = eCMRESERROR_LOCKOUT;
               }
            }
         }
         break;

         default:
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;
            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                       &cfgIdx,
                                       (void *) &pBasicCall,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               tmr.uTimeOut = pBasicCall->uTermErrSigTimer;
               sig.uSig = CMGR_INVALID_HANDLE;
               if ( pBasicCall->pTermOHErrSig )
               {
                  sig.uSig =
                     cmPhyToneFromString ( pBasicCall->pTermOHErrSig );
               }
               if ( sig.uSig == CMGR_INVALID_HANDLE )
               {
                  tmr.uTimeOut = CMGR_OFF_HOOK_WARN_TIMEOUT;
                  sig.uSig = eEPTSIG_OHWARN;
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
               sig.uSig = eEPTSIG_OHWARN;
            }

            /* Transition to the error state to handle the situation from now on.
            */
            cmUsrNewState ( pUsr, eCMST_ERROR );

            /* NULL tone or NULL timeout, skip this step and move
            ** on to the permanent sequence error right away.
            */
            if ( (sig.uSig == eEPTSIG_NULL) ||
                 !tmr.uTimeOut )
            {
               cmFsmOffHookWarn ( pRes );
            }
            /* Apply process.
            */
            else
            {
               tmr.uSes = CMGR_INVALID_HANDLE;
               tmr.uUsr = pUsr->uHdl;
               tmr.eType = eCMEVT_TMR_OHWARN;

               cmTmrStart ( &tmr );

               sig.eAct = eCMGREPSIGACT_START;
               sig.uSes = CMGR_INVALID_HANDLE;
               sig.uApp = CMGR_INVALID_HANDLE;
               pUsr->uCpt = sig.uSig;

               cmPhySignal ( pUsr->uPhys, &sig );
            }
            sig.uCid = CMGR_INVALID_HANDLE;
         }
         break;
      }
   }
   else
   {
      /* We were in error state and locked out, this is the end of the lock out
      ** period.
      */
      if ( ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_ERROR ) &&
           ( pUsr->eError == eCMRESERROR_LOCKOUT ) )
      {
         if ( pUsr->bPendingMwi == BOS_TRUE )
         {
            CMGREPSIG sig;

            /* Signal visual message waiting indication to the physical interface
             ** of interest associated with this user.
             */
            memset ( (void *) &sig,
                  0,
                  sizeof ( CMGREPSIG ) );

            sig.eAct = ( pUsr->bMwi ?
                  eCMGREPSIGACT_START : eCMGREPSIGACT_STOP );
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
            sig.uSig = eEPTSIG_VMWI;

            cmPhySignal ( pUsr->uPhys, &sig );
            pUsr->bPendingMwi = BOS_FALSE;
         }

         cmUsrNewState ( pUsr, eCMST_IDLE );

         /* Publish a 'line inactive' event.
         */
         if ( gCmCfgCb.cmgrEvtCb &&
              (pUsr->eType != eCMUSRTYPE_REMOTE) )
         {
            gCmCfgCb.cmgrEvtCb ( eCMGREVT_LINE_INACTIVE,
                                 pUsr->uPhys,
                                 pUsr->uHdl,
                                 CMGR_INVALID_HANDLE,
                                 NULL );
         }

         /* If we are pending on an action, don't release
         ** the resource.
         */
         if ( pUsr->bPendingActPhy == BOS_FALSE ) 
         {
            cmUsrPhyDisassociate( pUsr );
         }
      }
   }
}

/***********************************************************************************
** FUNCTION:   cmFsmOsi
**
** PURPOSE:    FSM received OSI indication timeout.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmOsi ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;

   CMGRLOG_INFORMATION (( "cmFsmOsi: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   sig.eAct = eCMGREPSIGACT_START;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = CMGR_INVALID_HANDLE;
   sig.uSig = eEPTSIG_OSI;

   cmPhySignal ( pUsr->uPhys, &sig );
}


/***********************************************************************************
** FUNCTION:   cmFsmDtmfTimer
**
** PURPOSE:    FSM time out when playing out DTMF.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmDtmfTimer ( void *pRes )
{
   CMTMR tmr;
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_INFORMATION (( "%s: 0x%X (0x%X)",
                          __FUNCTION__,
                          pUsr->uHdl,
                          cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   /* Terminate the timer (this function may be invoked prior to actual
   ** timer expiry as well).
   */
   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_DTMF;

   cmTmrStop ( &tmr );

   /* Reset the user's last signal */
   pUsr->uCpt = eEPTSIG_NULL;
}
