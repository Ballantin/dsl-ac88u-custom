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
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNetHoldTimer,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmInitReg,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
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
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
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
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmWarmLine,
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
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmTimeoutFax,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_RINGING -------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoAnswer,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNetHoldTimer,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
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
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmCallWaitEnd,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmEmergHowlTimer,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmTimeoutFax,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmAutoRecoverTimer,
      /* ---- eCMEVT_TMR_DTMF ---------------- */ &cmFsmDtmfTimer,
   },

   /* ---- eCMST_FLASH ---------------------------------------------------------- */
   {
      /* ---- eCMEVT_TMR_ONHOOK -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_DIALTONE ------------ */ &cmFsmAbandonFlash,
      /* ---- eCMEVT_TMR_RINGING ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_RINGBACK ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INTERDIGIT_SHORT ---- */ &cmFsmFlashTimer,
      /* ---- eCMEVT_TMR_INTERDIGIT_LONG ----- */ &cmFsmAbandonFlash,
      /* ---- eCMEVT_TMR_RENEW_SVC ----------- */ &cmFsmRenewSvc,
      /* ---- eCMEVT_TMR_WAITCONF_SVC -------- */ &cmFsmTimeoutSvc,
      /* ---- eCMEVT_TMR_OHWARN -------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_CALLWAIT ------------ */ &cmFsmCallWaitEnd,
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmKeyTimer,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_FAX_EVENT ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_WARMLINE ------------ */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_OSI_DELAY ----------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTORECOVER --------- */ &cmFsmAutoRecoverTimer,
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
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmAutoCallTimer,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
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
      /* ---- eCMEVT_TMR_AUTORECALL ---------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_AUTOCALLBACK -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_KEYHELD ------------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_NETWORK_HOLD -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_EMERG_HOWLER -------- */ &cmFsmNoOp,
      /* ---- eCMEVT_TMR_INIT_REG_DELAY ------ */ &cmFsmNoOp,
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
            if ( ( pUsr->pMedSes [ uIx ]->bEstab ||
                    ( !pUsr->pMedSes [ uIx ]->bEstab && pUsr->pSesWait &&
                       ( pUsr->pMedSes [ uIx ] == pUsr->pSesWait ) ) ) &&
                 ( pUsr->pMedSes [ uIx ]->uHdl != pSes->uHdl ) )
            {
               pOtherSes = pUsr->pMedSes [ uIx ];
            }
         }
      }

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                                 &cfgIdx,
                                 (void *) &pCfgXfer,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( ( pCfgXfer ) &&
         /* Make sure we do not attempt to transfer an emergency
         ** session.
         */
              ( !pSes->ses.med.bEmergency ) && 
              /* and make sure we do not attempt to transfer an outgoing
              ** call if only incoming calls can be transferred
              */
              !( (pCfgXfer->bIncomingOnly) && 
                 (pOtherSes && pOtherSes->uNet != CMGR_INVALID_HANDLE &&
                  !pOtherSes->ses.med.bRemInit) )
            )
         {
            bXfer = pCfgXfer->Act.bCfgStatus;
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallXfrConcept ),
                               &cfgIdx,
                               (void *) pCfgXfer,
                               CFG_ITEM_DEL );
         pCfgXfer = NULL;
      }

      /* Call hold has precedence over call waiting, call conference
      ** and call transfer in the case where the 'other' session
      ** happens to be the call hold one.
      */
      if ( pOtherSes &&
           pUsr->pSesHeld && ( pUsr->pSesHeld == pOtherSes ) )
      {
         cmSesTerminate ( pSes );

         /* Terminate knowledge of call hold and the 'other' session will
         ** become the de-facto focus session.
         */
         pUsr->pSesHeld = NULL;
         pUsr->bPresrvHeld = BOS_FALSE;

         cmUsrNewState ( pUsr, eCMST_RINGING );
      }
      /* Call waiting has precedence over call conference and call
      ** transfer in the case where the 'other' session happens to be the
      ** call waiting one.
      */
      else if ( pOtherSes &&
                pUsr->pSesWait && ( pUsr->pSesWait == pOtherSes ) )
      {
         cmSesTerminate ( pSes );

         /* Terminate knowledge of call waiter and the 'other' session will
         ** become the de-facto focus session.
         */
         pUsr->pSesWait = NULL;
         bCW = BOS_TRUE;
      }
      /* If the 'other' session is not held and is not the call waiting one,
      ** then it must be a session we created.  In this case, apply a call transfer
      ** now if enabled to do so.
      */
      else if ( bXfer && pOtherSes &&
                ( pSes->uNet != CMGR_INVALID_HANDLE ) &&
                ( pOtherSes->uNet != CMGR_INVALID_HANDLE ) )
      {
         cctkCallTransfer ( pOtherSes->uNet, pSes->uNet, BOS_FALSE );

         pSes->ses.med.uConf = CMGR_INVALID_HANDLE;
         pOtherSes->ses.med.uConf = CMGR_INVALID_HANDLE;
      }
      else
      {
         CMSESLIST sesList;

         memset( &sesList, 0, sizeof( CMSESLIST ) );
         
         if ( pOtherSes && ( pOtherSes->ses.med.uConf == pSes->uHdl ) )
         {
            cmSesTerminate ( pOtherSes );
         }

         /* If this is external call with barged-in sessions or an
         ** internal call, all sessions connected to this one need to be
         ** released.
         */
         if ( cmNumSesBargedIn ( pSes->uHdl ) )
         {
            cmSesListWithBargeIn( pSes->uHdl, &sesList );
         }
         else if( pSes->bEstab && pSes->uNetUsr == CMGR_INVALID_HANDLE )
         {
            cmSesListWithCid( pSes->uNet, &sesList );
         }


         /* Release the found sessions if any.
         */
         if ( sesList.uLength )
         {
            cmSesListRelease( &sesList, pSes );
         }
         
         cmSesTerminate ( pSes );

         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }
   }

   /* If there are transient session (not established), clean them up as well.
   */
   pSes = NULL;
   for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {
#if CMGRCFG_REMOTE_USER_CONTROL
         /* If the session is a background focus for a remote client, skip the
         ** cleanup. */
         if ( pUsr->pMedSes [ uIx ]->eType == eCMSESTYPE_MEDIA && 
              pUsr->pMedSes [ uIx ]->ses.med.bBgFocus )
         {
            continue;
         }
#endif /* CMGRCFG_REMOTE_USER_CONTROL */

         /* If the session is established, or if it is the call waiter session that
         ** may still be transient at this time, promote it as new active session.
         */
         if ( pUsr->pMedSes [ uIx ]->bEstab ||
              (bCW && (pUsr->pMedSes [ uIx ] == pOtherSes) &&
               !pUsr->pMedSes [ uIx ]->bEstab) )
         {
            pSes = pUsr->pMedSes [ uIx ];
         }
         else
         {
            if ( ( pUsr->pMedSes [ uIx ]->uNet != CMGR_INVALID_HANDLE ) &&
                 ( cmNumSesWithCid( pUsr->pMedSes [ uIx ]->uNet ) == 1 ) &&
                 ( pUsr->pMedSes [ uIx ]->uNetUsr != CMGR_INVALID_HANDLE ) )
            {
               cctkCallDelete ( pUsr->pMedSes [ uIx ]->uNet,
                                eCCTKCALLDELRSN_NORMAL );
            }
            /* If this is an internal call and one side needs to end, clean it
            ** and signal the other of an impending close.
            */
            else if( ( pUsr->pMedSes [ uIx ]->uNet != CMGR_INVALID_HANDLE ) &&
                     ( cmNumSesWithCid( pUsr->pMedSes [ uIx ]->uNet ) == 2 ) &&
                     ( pUsr->pMedSes [ uIx ]->uNetUsr == CMGR_INVALID_HANDLE ) )
            {
               CMSESLIST sesList;
               BOS_UINT32 uIx = 0;

               memset( &sesList, 0, sizeof( CMSESLIST ) );

               cmSesListWithCid( pUsr->pMedSes [ uIx ]->uNet, &sesList );
               for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
               {
                  CMSES* pSesRelease = cmSesValid( sesList.uSes[ uIx ] );
                  CMUSR* pUsrRelease = (pSesRelease) ? cmUsrValid( pSesRelease->uUsr ) : NULL;

                  CMGRLOG_CRITICAL(( "%s: In session list loop. Looking for ses %X\n", __FUNCTION__, sesList.uSes[ uIx ] ));
                  if( ( pSesRelease ) && 
                      ( pUsrRelease ) &&
                      ( pUsr->pMedSes [ uIx ]->uHdl != pSesRelease->uHdl ) )
                  {
                     CMEVT cmEvt;

                     memset( &cmEvt, 0, sizeof( CMEVT ) );

                     CMGRLOG_CRITICAL(( "%s: Releasing for user %X\n", __FUNCTION__, pSesRelease->uUsr ));
                     cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
                     cmEvt.uDat1 = pSesRelease->uUsr;
                     cmEvt.uDat2 = pSesRelease->uHdl;
                     cmEvt.uDat4 = pSesRelease->uNetUsr;

                     if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
                     {
                        cmSignalEvt ();
                     }
                  }
               }
            }

            cmUsrUnLinkSes ( pUsr, pUsr->pMedSes [ uIx ] );
            cmUsrNewState ( pUsr, eCMST_RELEASING );
         }
      }
   }

   /* If there is no session left, then that is all.  If there is some
   ** session left, then we need to activate ring reminder and it becomes
   ** the de-facto focus session.
   */
   if ( pSes )
   {
      CMGRLOG_INFORMATION (( "cmFsmOnHookGuard: focus 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      pSes->ses.med.bFocus = BOS_TRUE;

      /* Terminate the call waiting timer
      */
      if ( bCW )
      {
         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_CALLWAIT;

         cmTmrStop ( &tmr );
      }

      memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
      cfgIdx.pcCharIdx = pUsr->uUid;
      if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                                 &cfgIdx,
                                 (void *) &pBasicCall,
                                 CFG_ITEM_GET ) == BOS_STATUS_OK )
      {
         if ( pBasicCall && pBasicCall->pHoldRemTone )
         {
            strcpy ( (char *) pSes->ses.med.callInfo.cAlertInfo[ 0 ],
                     (const char *) pBasicCall->pHoldRemTone );
         }

         gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, BasicCallConcept ),
                               &cfgIdx,
                               (void *) pBasicCall,
                               CFG_ITEM_DEL );
         pBasicCall = NULL;
      }

      /* Ring the user in lieu of call reminder.
      */
      cmFsmRing ( pRes );
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
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
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
         cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
      }
      else
      {
         /* Signal call releasing for this user */
         CMEVT cmEvt;

         memset( &cmEvt, 0, sizeof( CMEVT ) );

         cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
         cmEvt.uDat1 = pUsr->uHdl;
         cmEvt.uDat2 = pSes->uHdl;
         cmEvt.uDat4 = pSes->uNetUsr;
         cmEvt.bLast = BOS_TRUE;

         if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
         {
            cmSignalEvt ();
         }
      }

      cmUsrNewState ( pUsr, eCMST_RELEASING );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmNoAnswer
**
** PURPOSE:    FSM waiting for local user call answer timer handler.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNoAnswer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );
#if CMGRCFG_MULTILINE   
   CMCCTKACCNT* pCctkAccnt = (pSes) ? cmCctkAccntFindByNet( pSes->uNetUsr ) : NULL;
#endif /* CMGRCFG_MULTILINE */
   
   CMGREPSIG sig;
   CCTKCALLDELRSN eReason = eCCTKCALLDELRSN_UNAVAILABLE;
   CFGSIPCALLFWD *pFwdCfg;
   CFGSIPCALLFWDNOANS *pFwdNoAnsCfg;
   PROV_CFG_IDX cfgIdx;
   BOS_BOOL bFwd = BOS_FALSE;

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   if ( pSes )
   {
      CMGRLOG_DEBUG (( "cmFsmNoAnswer: ending 0x%X (0x%X) on 0x%X (0x%X)",
                       pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      /* Terminate the ring.
      */
      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;   /* eEPTSIG_RINGING */
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      /* Reject the network call.
      */
      if ( !pUsr->pAutoCall &&
           ( pSes->uNet != CMGR_INVALID_HANDLE ) )
      {
         CMTMR tmr;

         memset ( &tmr, 0, sizeof ( CMTMR ) );
         
         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_NETWORK_HOLD;

         /* If this is an emergency session and we are holding it
         ** it is a ringback from the operator that times out.
         */
         if ( pSes->ses.med.bEmergency &&
              cmTmrIsRunning ( &tmr ) )
         {
            eReason = eCCTKCALLDELRSN_RGBCKREJECT;
         }
         
         if ( !pSes->ses.med.bEmergency )
         {
            memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
            cfgIdx.pcCharIdx = pUsr->uUid;

            if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdNoAnsConcept ),
                                       &cfgIdx,
                                       (void *) &pFwdNoAnsCfg,
                                       CFG_ITEM_GET ) == BOS_STATUS_OK )
            {
               if ( pFwdNoAnsCfg &&
                    pFwdNoAnsCfg->Act.bCfgStatus )
               {
                  if ( gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                             &cfgIdx,
                                             (void *) &pFwdCfg,
                                             CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     if ( pFwdCfg &&
                          pFwdCfg->bUsrSet )
                     {
                        bFwd = BOS_TRUE;

                        CMGRLOG_WARNING ((
                           "cmFsmNoAnswer: forward 0x%X (0x%X) on 0x%X (0x%X) to \'%s\'",
                           pSes->uHdl, pSes->uNet, 
                           pUsr->uHdl, pSes->uNetUsr,
                           ( pFwdCfg->bUseFwdDString ? 
                           ( pFwdCfg->pcFwdToDString ? pFwdCfg->pcFwdToDString : "NULL" ) : 
                           ( pFwdCfg->pcFwdToUri ? pFwdCfg->pcFwdToUri : "NULL" ))
                           ));

                        if ( !(pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToUri != NULL) )
                        {
                           cctkCallForward( pSes->uNet,
                                            pFwdCfg->pcFwdToUri );
                        }
                        else
                        {
                           if ( (pFwdCfg->bUseFwdDString) && (pFwdCfg->pcFwdToDString != NULL) )
                           {
                              cmFsmCallForwardViaDialString( pUsr,  pSes->uNet,pFwdCfg->pcFwdToDString); 
                           }
                        }                     
                     }

                     gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdConcept ),
                                           &cfgIdx,
                                           (void *) pFwdCfg,
                                           CFG_ITEM_DEL );
                     pFwdCfg = NULL;
                  }
               }

               gCmCfgCb.cmgrProvCb ( CFGITEM( SIP, CallFwdNoAnsConcept ),
                                     &cfgIdx,
                                     (void *) pFwdNoAnsCfg,
                                     CFG_ITEM_DEL );
               pFwdNoAnsCfg = NULL;
            }
         }   
         /*
         ** Release in all cases to get ocnfirmation of the cleanup even
         ** if the call has been forwarded.
         */
         if( cmNumSesWithCid( pSes->uNet ) == 1 )
         {          
            if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
            {
               cctkCallDelete ( pSes->uNet, eReason );
            }
         }
         else
         {
            /* Signal call releasing for this user */
            CMEVT cmEvt;

            memset( &cmEvt, 0, sizeof( CMEVT ) );

            cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
            cmEvt.uDat1 = pUsr->uHdl;
            cmEvt.uDat2 = pSes->uHdl;
            cmEvt.uDat4 = pSes->uNetUsr;
            cmEvt.bLast = BOS_TRUE;

            if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }

         cmUsrNewState ( pUsr, eCMST_RELEASING );
      }

      /* If this was an internal call to which there was no answer and was not forwarded
      ** atleast stop the ringback on the call originator
      */
      if( !bFwd && (pSes->uNetUsr == CMGR_INVALID_HANDLE) )
      {
         CMSESLIST sesList;
         CMUSR* pUsrRelease = NULL;
         BOS_UINT32 uIx = 0;
      
         memset( &sesList, 0, sizeof( CMSESLIST ) );     
         cmSesListWithCid( pSes->uNet, &sesList );
         for( uIx = 0 ; uIx < sesList.uLength ; uIx++ )
         {
            CMSES* pSesRelease = cmSesValid( sesList.uSes[ uIx ] );
            
            /* If Originator */
            if( pSesRelease->bEstab )
            {
               pUsrRelease = (pSesRelease) ? cmUsrValid( pSesRelease->uUsr ) : NULL;                              
               
               if( pUsrRelease )
               {
                  /* We would be playing ringback to the peer. Terminate it.
                  */
                  sig.eAct = eCMGREPSIGACT_STOP;
                  sig.uSes = pSesRelease->ses.med.uPhy; 
                  sig.uApp = CMGR_INVALID_HANDLE;
                  sig.uCid = pSesRelease->uNet;
#if CMGRCFG_MULTILINE
                  sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
                  sig.uSig = eEPTSIG_RINGBACK;
                  
                  cmPhySignal ( pUsrRelease->uPhys, &sig );
               }
            }                                 
         }
      }


      /* If this is an emergency call and the network hold timer is still
      ** applicable, move back to a proper state to handle such.
      */
      if ( pSes->ses.med.bEmergency &&
           ( eReason == eCCTKCALLDELRSN_RGBCKREJECT ) )
      {
         cmUsrNewState ( pUsr, eCMST_IDLE );
      }
      /* Otherwise continue with call termination and clean up.
      */
      else
      {
         /* If this is the expiration of an auto-call ring, release
         ** the physical resource.
         **
         ** Note: we should here start another time to attempt to ring
         **       the local user once more prior to expiration of the
         **       feature itself and as many times as necessary (per
         **       configuration.
         */
         if ( pUsr->pAutoCall )
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

            cmUsrPhyDisassociate( pUsr );
            cmUsrCctkAccntDisassociate( pUsr );
         }
      }
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
** FUNCTION:   cmFsmAbandonFlash
**
** PURPOSE:    FSM digit collection timer expiry during flash processing.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAbandonFlash ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = NULL;
   BOS_UINT32 uIx;
   CMGREPSIG sig;
   CMTMR tmr;
   CFGSIPBASICCALL *pBasicCall = NULL;
   PROV_CFG_IDX cfgIdx;

   CMGRLOG_INFORMATION (( "cmFsmAbandonFlash: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &tmr,
            0,
            sizeof ( CMTMR ) );

   memset ( &sig,
            0,
            sizeof ( CMGREPSIG ) );

   /* Stop dial tone.
   */
   sig.eAct = eCMGREPSIGACT_STOP;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
   sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
   sig.uSig = pUsr->uCpt;
   pUsr->uCpt = eEPTSIG_NULL;

   cmPhySignal ( pUsr->uPhys, &sig );

   /* If RRFlash feature is in use, and timed out waiting for DTMF signal, then 
   ** revert call state 
   */
   if ( pUsr->rrFlash.bRRWaitKey )
   {
      /* restore to previous state */
      cmFsmRRRevert ( pRes );
      return;
   }

   /* Check or locate the pending session for this user.
   */
   for ( uIx = 0 ; uIx < (CMGRCFG_MEDSES_MAX + 1) ; uIx++ )
   {
      if ( pUsr->pMedSes [ uIx ] )
      {
         pSes = pUsr->pMedSes [ uIx ];
      }
   }

   if ( pSes )
   {
      /* The session left is the held one.
      */
      if ( pUsr->pSesHeld == pSes )
      {
         /* Mark the fact that we want to preserve the held state.
         */
         pUsr->bPresrvHeld = BOS_TRUE;

         /* Run off-hook warning timer and move to the right state to handle
         ** this situation.
         */
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

         cmUsrNewState ( pUsr, eCMST_RELEASING );

         tmr.uSes = CMGR_INVALID_HANDLE;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_OHWARN;

         cmTmrStart ( &tmr );
      }
      /* A non-held session is the one left to deal with.
      */
      else
      {
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
#if CMGRCFG_MULTILINE
      sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
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
#if CMGRCFG_MULTILINE
                  sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
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
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
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
#if CMGRCFG_MULTILINE
            sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
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


/***********************************************************************************
** FUNCTION:   cmFsmCallWaitEnd
**
** PURPOSE:    FSM terminate call waiting for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmCallWaitEnd ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = pUsr->pSesWait;

   if ( pSes )
   {
      CMGREPSIG sig;
#if CMGRCFG_MULTILINE      
      CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
      

      memset ( &sig, 0, sizeof ( CMGREPSIG ) );

      CMGRLOG_INFORMATION ((
         "cmFsmCallWaitEnd: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      pUsr->pSesWait = NULL;

      if( cmNumSesWithCid( pSes->uNet ) == 1 )
      {
         if( pSes->uNetUsr != CMGR_INVALID_HANDLE )
         {
            /* Release the network call.
            */
            cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_UNAVAILABLE );
         }
         else
         {
            /* Signal call releasing for this user */
            CMEVT cmEvt;

            memset( &cmEvt, 0, sizeof( CMEVT ) );

            cmEvt.uEvt = ( eCMEVT_RELEASE_CALL | CMGR_EVT_CLASS__CCTK );
            cmEvt.uDat1 = pUsr->uHdl;
            cmEvt.uDat2 = pSes->uHdl;
            cmEvt.uDat4 = pSes->uNetUsr;
            cmEvt.bLast = BOS_TRUE;

            if ( cmPostEvt ( &cmEvt ) == BOS_STATUS_OK )
            {
               cmSignalEvt ();
            }
         }
      }

      sig.eAct = eCMGREPSIGACT_STOP;
      sig.uSes = CMGR_INVALID_HANDLE;
      sig.uApp = CMGR_INVALID_HANDLE;
      sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
      sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
      sig.uSig = pUsr->uCpt;
      pUsr->uCpt = eEPTSIG_NULL;

      cmPhySignal ( pUsr->uPhys, &sig );

      cmUsrUnLinkSes ( pUsr, pSes );
      cmSesFree ( pSes, pUsr->uHdl );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmAutoCallTimer
**
** PURPOSE:    FSM auto-call timer for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAutoCallTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes && ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmAutoCallTimer: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( !((CMAUTOCALL *)pSes->ses.srv.pAutoCall)->bAnonymous )
      {
         /* Terminate the auto-call service at the network level.
         */
         cctkEventUnsubscribe ( pSes->uNet );
      }
      else
      {
         /* Simply unlink and free the resource, this is the end of the
         ** network based auto-call service.
         */
         cmUsrUnLinkSes ( pUsr, pSes );
         cmSesFree ( pSes, pUsr->uHdl );
      }

      /* Terminate the auto-call service at the user level.
      */
      memset( (void *) (CMAUTOCALL *)pSes->ses.srv.pAutoCall,
              0,
              sizeof ( CMAUTOCALL ) );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmNetHoldTimer
**
** PURPOSE:    FSM network hold timer expired for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmNetHoldTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes )
   {
      CMGRLOG_INFORMATION ((
         "cmFsmNetHoldTimer: 0x%X (0x%X) on 0x%X (0x%X)",
         pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if ( pUsr->eSta[ CMFSM_CURRENT_STATE_IX ] == eCMST_RINGING )
      {
         CMGREPSIG sig;
#if CMGRCFG_MULTILINE         
         CMCCTKACCNT* pCctkAccnt = cmCctkAccntFindByNet( pSes->uNetUsr );
#endif /* CMGRCFG_MULTILINE */
         
         CMTMR tmr;

         memset ( &sig, 0, sizeof ( CMGREPSIG ) );
         memset ( &tmr, 0, sizeof ( CMTMR ) );

         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uApp = CMGR_INVALID_HANDLE;
         sig.uCid = pSes->uNet;
#if CMGRCFG_MULTILINE
         sig.uLine = (pCctkAccnt) ? pCctkAccnt->uHdl : CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         sig.uSig = pUsr->uCpt;
         pUsr->uCpt = eEPTSIG_NULL;

         cmPhySignal ( pUsr->uPhys, &sig );

         tmr.uSes = pSes->uHdl;
         tmr.uUsr = pUsr->uHdl;
         tmr.eType = eCMEVT_TMR_RINGING;

         cmTmrStop ( &tmr );
      }

      pSes->ses.med.bEmgExpired = BOS_TRUE;

      /* Apply the same as if we went on hook and the guard timer
      ** expired.
      */
      cmFsmOnHookGuard ( pRes );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmEmergHowlTimer
**
** PURPOSE:    FSM emergency howler timer expired for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmEmergHowlTimer ( void *pRes )
{
   CMGREPSIG sig;
   CMTMR tmr;
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_INFORMATION (( "cmFsmEmergHowlTimer: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );
   memset ( &tmr, 0, sizeof ( CMTMR ) );

   sig.eAct = eCMGREPSIGACT_STOP;
   sig.uSes = CMGR_INVALID_HANDLE;
   sig.uApp = CMGR_INVALID_HANDLE;
   sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
   sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
   sig.uSig = pUsr->uCpt;
   pUsr->uCpt = eEPTSIG_NULL;

   cmPhySignal ( pUsr->uPhys, &sig );

   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_EMERG_HOWLER;

   cmTmrStop ( &tmr );
}


/***********************************************************************************
** FUNCTION:   cmFsmInitReg
**
** PURPOSE:    FSM time out when waiting for initial registration delay.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmInitReg ( void *pRes )
{
   CMTMR tmr;
   CMUSR *pUsr = (CMUSR *) pRes;

   CMGRLOG_INFORMATION (( "cmFsmInitReg: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &tmr, 0, sizeof ( CMTMR ) );

   /* Terminate the timer (this function may be invoked prior to actual
   ** timer expiry as well).
   */
   tmr.uSes = CMGR_INVALID_HANDLE;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_INIT_REG_DELAY;

   cmTmrStop ( &tmr );

   /* Register the user now.
   */
#if (CMGRCFG_CFG_PROFILE == CMGRCFG_CFG_PROFILE_STANDALONE)
   pUsr->bInSrv = BOS_TRUE;
#else
   cctkUserRegister ( cmUsrGetNtwkHdl( pUsr ) );
#endif
}


/***********************************************************************************
** FUNCTION:   cmFsmTimeoutFax
**
** PURPOSE:    FSM timeout waiting for fax event detection.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmTimeoutFax ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmSesValid ( pUsr->evt.uDat2 );

   if ( pSes && ( pSes->uNetUsr != CMGR_INVALID_HANDLE ) )
   {
      CMGRLOG_INFORMATION (( "cmFsmTimeoutFax: Ending 0x%X (0x%X) on 0x%X (0x%X)",
                             pSes->uHdl, pSes->uNet, pUsr->uHdl, pSes->uNetUsr ));

      if( cmNumSesWithCid( pSes->uNet ) == 1 )
      {
         cctkCallDelete ( pSes->uNet, eCCTKCALLDELRSN_NORMAL );
      }
      cmUsrNewState ( pUsr, eCMST_RELEASING );

      cmUsrUnLinkSes ( pUsr, pSes );
      cmSesFree ( pSes, pUsr->uHdl );
   }
}


/***********************************************************************************
** FUNCTION:   cmFsmWarmLine
**
** PURPOSE:    FSM received warm line timeout.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void cmFsmWarmLine ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMGREPSIG sig;

   CMGRLOG_INFORMATION (( "cmFsmWarmLine: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   memset ( &sig, 0, sizeof ( CMGREPSIG ) );

   /* End the tone we would have been playing for the user to
   ** enter digits.
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

   /* Apply the hotline feature now that the warmline delay
   ** has expired.
   */
   cmFeatAccept ( pRes, eCMDM_HOTLINEINT );
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
#if CMGRCFG_MULTILINE
   sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
   sig.uSig = eEPTSIG_OSI;

   cmPhySignal ( pUsr->uPhys, &sig );
}


/***********************************************************************************
** FUNCTION:   cmFsmAutoRecoverTimer
**
** PURPOSE:    FSM auto-recover timer for this resource.
**
** PARAMETERS: pRes - the pointer to the resource to run the FSM on.
**
** RETURNS:    None.
***********************************************************************************/
void cmFsmAutoRecoverTimer ( void *pRes )
{
   CMUSR *pUsr = (CMUSR *) pRes;
   CMSES *pSes = cmUsrFocusSes ( pUsr );

   CMGRLOG_INFORMATION (( "cmFsmAutoRecoverTimer: 0x%X (0x%X)",
                          pUsr->uHdl, cmUsrGetCurrNtwkHdl( pUsr ) ));

   /* 'Auto-Recover' timer is run when we need to re-establish automatically
   ** the focus session that has been put on hold previously for other action
   ** and the other action failed or did not complete as intended.
   */
   if ( pSes )
   {
      /* Resume the held session if needed.
      */
      if ( (pSes->uNet != CMGR_INVALID_HANDLE) &&
           (pSes->ses.med.eAutoRec != eCMSESAUTOREC_TPF) )
      {
         CMGRLOG_INFORMATION ((
                  "cmFsmAutoRecoverTimer: 0x%X (0x%X) - resume 0x%X (0x%X)",
                  pSes->uNetUsr, pUsr->uNet, pSes->uHdl, pSes->uNet ));

         cctkCallUnhold ( pSes->uNet );
      }

      /* Terminate any local user feedback we may have been 
      ** providing.
      */
      if ( pUsr->uCpt != eEPTSIG_NULL )
      {
         CMGREPSIG sig;

         memset ( (void *) &sig,
                  0,
                  sizeof ( CMGREPSIG ) );

         sig.eAct = eCMGREPSIGACT_STOP;
         sig.uSes = CMGR_INVALID_HANDLE;
         sig.uSig = pUsr->uCpt;
         sig.uCid = CMGR_INVALID_HANDLE;
#if CMGRCFG_MULTILINE
         sig.uLine = CMGR_INVALID_HANDLE;
#endif /* CMGRCFG_MULTILINE */
         pUsr->uCpt = eEPTSIG_NULL;
         /* 
         ** Terminate local tone feedback.
         */
         cmPhySignal ( pUsr->uPhys, &sig );
         /* 
         ** Terminate remote tone feedback.
         */
         sig.uSes = pSes->ses.med.uPhy;
         cmPhySignal ( pUsr->uPhys, &sig );
      }

      pSes->ses.med.eAutoRec = eCMSESAUTOREC_NONE;
   }
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
