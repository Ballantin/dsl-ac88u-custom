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

      

      if ( gCmCfgCb.cmgrEvtCb )
      {
         ( gCmCfgCb.cmgrEvtCb ) ( eCMGREVT_CALL_ENDED,
                                  pSes->uNet,
                                  pSes->uUsr,
                                  CMGR_INVALID_HANDLE,
                                  (void *) pUsr->uUid );
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
            /* TODO: Add support here when multiple calls are handled.
            */
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

      /* Apply the action as necessary.
      */
      switch ( eAction )
      {
         case eCMDM_REORDER:
         {
            CMGREPSIG sig;

            memset ( &sig, 0, sizeof ( CMGREPSIG ) );

            sig.eAct = eCMGREPSIGACT_START;
            sig.uSes = CMGR_INVALID_HANDLE;
            sig.uSig = eEPTSIG_REORDER;
            sig.uApp = CMGR_INVALID_HANDLE;
            sig.uCid = CMGR_INVALID_HANDLE;
            
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
   sig.pData = &uCallIdent;

   /* Insert a unique identifier for this call by means of the
   ** calling number.  Note that this information may contain no
   ** data of interest, it is just there for the application to make
   ** use of it as needed.
   */
   sprintf ( (char *) uCallIdent,
             "%s",
             pSes->ses.med.callInfo.clid.cNumber );

   sig.uSig =
      ( ( bClidName || bClidNumber ) ?
                           eEPTSIG_CALLID_RINGING : eEPTSIG_RINGING );
    

   pUsr->uCpt = sig.uSig;

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

   tmr.uSes = pSes->uHdl;
   tmr.uUsr = pUsr->uHdl;
   tmr.eType = eCMEVT_TMR_RINGING;

   cmUsrNewState ( pUsr, eCMST_RINGING );
   cmTmrStart ( &tmr );
   cmPhySignal ( pUsr->uPhys, &sig );

   /* Issue caller identification information if necessary.
   */
   if ( bClidName || bClidNumber )
   {
      cmSesClid ( pSes, bClidName, bClidNumber );
   }
}