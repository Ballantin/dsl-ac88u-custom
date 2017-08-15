/**********************************************************************************
** Copyright © 2007 - 2011 Broadcom Corporation
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
**      This file implements the call transfer finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmXfer.h>
#include <ceFsmCall.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_XFER_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_XFER_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_XFER_SET_FINISH - eCEEVT_XFER_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_XFER_IDLE -------------------------------------------- */
   {
        /* ---- eCEEVT_XFER_NET_XFER -------------- */ &CeFsmXfer::fsmXfer,
        /* ---- eCEEVT_XFER_NET_OKAY -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_FAIL -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_PROC -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_ONGOING ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_TERMNET ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_REFENDED ---------- */ &CeFsmXfer::fsmRefEnded,
        /* ---- eCEEVT_XFER_NET_ANSWER ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_RING -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_ACCEPT ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_USR_XFER -------------- */ &CeFsmXfer::fsmXfering,
        /* ---- eCEEVT_XFER_USR_BXFER ------------- */ &CeFsmXfer::fsmBxfering,
        /* ---- eCEEVT_XFER_USR_XFER_NOTERM-------- */ &CeFsmXfer::fsmXfering,
        /* ---- eCEEVT_XFER_USR_NTTO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
   },

   /* ---- eCEFSMSTATE_XFER_ACCEPT ------------------------------------------ */
   {
        /* ---- eCEEVT_XFER_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_NET_OKAY -------------- */ &CeFsmXfer::fsmSuccess,
        /* ---- eCEEVT_XFER_NET_FAIL -------------- */ &CeFsmXfer::fsmFailure,
        /* ---- eCEEVT_XFER_NET_PROC -------------- */ &CeFsmXfer::fsmProceed,
        /* ---- eCEEVT_XFER_NET_ONGOING ----------- */ &CeFsmXfer::fsmProceed,
        /* ---- eCEEVT_XFER_NET_TERMNET ----------- */ &CeFsmXfer::fsmEnded,
        /* ---- eCEEVT_XFER_NET_REFENDED ---------- */ &CeFsmXfer::fsmRefEnded,
        /* ---- eCEEVT_XFER_NET_ANSWER ------------ */ &CeFsmXfer::fsmAnswered,
        /* ---- eCEEVT_XFER_NET_RING -------------- */ &CeFsmXfer::fsmRing,
        /* ---- eCEEVT_XFER_NET_EARLYMEDIA -------- */ &CeFsmXfer::fsmEarlyMedia,
        /* ---- eCEEVT_XFER_NET_ACCEPT ------------ */ &CeFsmXfer::fsmAccepted,
        /* ---- eCEEVT_XFER_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_XFER_USR_NTTO -------------- */ &CeFsmXfer::fsmNtfyTimeOut,
        /* ---- eCEEVT_XFER_USR_ENDED ------------- */ &CeFsmXfer::fsmEnd,
   },

};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmRun
**
**  PURPOSE:    Run the call transfer state machine.
**
**  PARAMETERS:
**              rRes - reference to the call resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_XFER_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_XFER_SET_FINISH) &&
        (eEvent > eCEEVT_XFER_SET_BEGIN) &&
        (eEvent < eCEEVT_XFER_SET_FINISH) )
   {
      CCTK_TRACEI4( "fsmRun (0x%lX - %s) - state (%s)/event (%s).",
                    rRes.resourceGuid(),
                    gpcCeFsmName[rRes.resourceFsm()],
                    gpcCeFsmState[eState],
                    gpcCeFsmEvent[static_cast<BOS_UINT32>(eEvent) - eCEEVT_INV] );

      /* Map the state and event such that they are in range.
      */
      BOS_UINT32 uStateIx, uEventIx;

      uStateIx = static_cast<BOS_UINT32>(eState)
                           - (CeFsmBase::eCEFSMSTATE_XFER_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_XFER_SET_BEGIN + 1);

      (gFsmFnc[ uStateIx ][ uEventIx ])( rRes );
   }
   else if ( eEvent != eCEEVT_DNP )
   {
      /* 'System' error.
      */
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmXfer
**
**  PURPOSE:    Call transfer from network is attempted.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmXfer( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      BOS_BOOL bCreated = BOS_FALSE;
      BOS_UINT32 uResGuid = CeMain::allocateGuid();
      CeResBase *pXferRes = NULL;
      MXD_GNS::MseSdpSession *pMse = NULL;

      /* Associate the new call configuration with the current call
      ** configuration such that per-call configuration is carried over
      ** into the new call in order to save the characteristics of the
      ** former (such as emergency status, etc...).
      */
      CFGSTATUS eStatus = cfgAddCall( uResGuid,
                                      pResCall->resourceGuid() );
      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         bCreated =
            (CeMain::allocateRes( CeResBase::eCERESTYPE_CALL,
                                  uResGuid,
                                  pResCall->resourceParent() ) != guCeInvalidGuid);
         if ( bCreated )
         {
            pMse = reinterpret_cast<MXD_GNS::MseSdpSession *>
                                    (BRCM_NEW( MXD_GNS::MseSdpSession ));

            if ( pMse )
            {
               CeMain::locateRes( uResGuid, pXferRes );

               pMse->setCid( pXferRes->resourceGuid() );

               if ( pXferRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall* pXferCall =
                     reinterpret_cast<CeResCall *> ( pXferRes->resourceAddress() );
                  pXferCall->resourceSetMse( pMse );
                  pXferCall->resourceNetHdl()->SetMspSession( pMse );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }

               pXferRes->resourcePushFsm( CeFsmBase::eCEFSMTYPE_XFER );
               pXferRes->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_ACCEPT );
            }
            else
            {
               if ( pXferRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall* pXferCall =
                     reinterpret_cast<CeResCall *> ( pXferRes->resourceAddress() );
                  pXferCall->resourceTerminate();
                  cfgRemoveCall( uResGuid );

                  bCreated = BOS_FALSE;
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }
         }
         else
         {
            cfgRemoveCall( uResGuid );
         }
      }


      if ( bCreated )
      {
         /* Move to the next transfer state to handle the next steps.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_ACCEPT );

         /* Proper support for the call transfer was created.  Accept the
         ** transfer.
         */
         pResCall->resourceSetXferGuid( pXferRes->resourceGuid() );
         
         if ( pXferRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
         {
            CeResCall* pXferCall =
               reinterpret_cast<CeResCall *> ( pXferRes->resourceAddress() );
            
            /* Link the calls together so we know how one may interact with the
            ** other.
            */
            pXferCall->resourceSetLink( pResCall->resourceNetHdl() );
            pResCall->resourceSetLink( pXferCall->resourceNetHdl() );

            /* Out of dialog transfer.
            */
            if ( pResCall->resourceNetReferrer() )
            {
               MXD_GNS::CNameAddr addr;

               /* Make mention that we accept this transfer.
               */
               pResCall->resourceNetReferrer()->AcceptRefer();

               /* Place the call now.
               */
               pResCall->resourceGetSvc( addr );
               pXferCall->resourceNetHdl()->MakeCall( addr, NULL );
            }
            /* In dialog transfer.
            */
            else
            {
               pResCall->resourceNetHdl()->AcceptTransfer(
                           pXferCall->resourceNetHdl() );
            }
         }
         else
         {
            fsmError( rRes );
            return;
         }
      }
      else
      {
         /* Roll back on failure to execute the action.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         /* Proper support for the call transfer could not be created.  Decline the
         ** transfer.
         */
         if ( pResCall->resourceNetReferrer() )
         {
            CeResBase *pResUser = NULL;

            pResCall->resourceNetReferrer()->RejectRefer();

            /* Pass referrer ownership back to the parent.
            */
            if ( CeMain::locateRes( pResCall->resourceParent(),
                                    pResUser ) &&
                 ( pResUser != NULL ) )
            {
               if ( pResUser->resourceGender() == CeResBase::eCERESTYPE_USR )
               {
                  CeResUser *pUser = reinterpret_cast<CeResUser *>
                     ( pResUser->resourceAddress() );
                  pUser->resourceSetReferrer( pResCall->resourceNetReferrer() );
                  pResCall->resourceSetReferrer( NULL );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }
         }
         else
         {
            pResCall->resourceNetHdl()->DeclineTransfer();
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmFailure
**
**  PURPOSE:    Call transfer from network has failed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmFailure( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Pass the ownership of the referrer back to the user for the final
      ** cleanup if applicable.
      */
      if ( pResCall->resourceNetReferrer() )
      {
         CeResBase *pResUser = NULL;

         /* Pass referrer ownership back to the parent.
         */
         if ( CeMain::locateRes( pResCall->resourceParent(),
                                 pResUser ) &&
              ( pResUser != NULL ) )
         {
            if ( pResUser->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               CeResUser *pUser = reinterpret_cast<CeResUser *>
                  ( pResUser->resourceAddress() );
               pUser->resourceSetReferrer( pResCall->resourceNetReferrer() );
               pResCall->resourceSetReferrer( NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Terminate the notification timer if we were running it.
      */
      if ( pResCall->resourceIsXferer() &&
           ( pResCall->resourceXferTimerGuid() != guCeInvalidGuid ) )
      {
         tkmTimerStop( pResCall->resourceXferTimerGuid() );
         pResCall->resourceSetXferTimerGuid( guCeInvalidGuid );
      }

      /* Transfer failed for some reason.  If we are the transferer, terminate
      ** this call as well as the associated call that we were transfering to
      ** (if valid).  If we are the transferee, just terminate the current call.
      */
      if ( pResCall->resourceIsXferer() &&
           (pResCall->resourceXferGuid() != guCeInvalidGuid) )
      {
         CeResBase *pXferCall = NULL;

         CeMain::locateRes( pResCall->resourceXferGuid(),
                            pXferCall );
         if ( pXferCall )
         {
            /* Update the resource state and FSM.
            */
            pXferCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
            pXferCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

            if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResXferCall =
                  reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );
               pResXferCall->resourceNetHdl()->TerminateCall();
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      pResCall->resourceNetHdl()->TerminateCall();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmSuccess
**
**  PURPOSE:    Call transfer from network is successful.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmSuccess( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   BOS_UINT32 uXferGuid;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Pass the ownership of the referrer back to the user for the final
      ** cleanup if applicable.
      */
      if ( pResCall->resourceNetReferrer() )
      {
         CeResBase *pResUser = NULL;

         /* Pass referrer ownership back to the parent.
         */
         if ( CeMain::locateRes( pResCall->resourceParent(), pResUser ) &&
              ( pResUser != NULL ) )
         {
            if ( pResUser->resourceGender() == CeResBase::eCERESTYPE_USR )
            {
               CeResUser *pUser = reinterpret_cast<CeResUser *>
                  ( pResUser->resourceAddress() );
               pUser->resourceSetReferrer( pResCall->resourceNetReferrer() );
               pResCall->resourceSetReferrer( NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Terminate the notification timer if we were running it.
      */
      if ( pResCall->resourceNoXferTerm() &&  pResCall->resourceIsXferer() &&
           ( pResCall->resourceXferTimerGuid() != guCeInvalidGuid ) )
      {
         CeResBase *pXferCall = NULL;
		 BOS_UINT32 uXferGuid = pResCall->resourceXferGuid();

         tkmTimerStop( pResCall->resourceXferTimerGuid() );

         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_XFERCALL,
                   reinterpret_cast<void *> ( &uXferGuid ) );

         pResCall->resourceSetXferTimerGuid( guCeInvalidGuid );
      }
      else if ( pResCall->resourceIsXferer() &&
           ( pResCall->resourceXferTimerGuid() != guCeInvalidGuid ) )
      {
         CeResBase *pXferCall = NULL;

         tkmTimerStop( pResCall->resourceXferTimerGuid() );

         CeMain::locateRes( pResCall->resourceXferGuid(),
                            pXferCall );
         if ( pXferCall )
         {
            /* Update the resource state and FSM.
            */
            pXferCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
            pXferCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

            if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResXferCall =
                  reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );
               pResXferCall->resourceNetHdl()->TerminateCall();
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }

         pResCall->resourceSetXferTimerGuid( guCeInvalidGuid );
      }

      /* Transfer sucessful.  Inform the application of the call transfer,
      ** terminate the current call.
      */
      if ( !pResCall->resourceIsXferer() &&
           (pResCall->resourceXferGuid() != guCeInvalidGuid) )
      {
         BOS_UINT32 uXferGuid = pResCall->resourceXferGuid();
         CeResBase *pRes = NULL;

         /* Note that we should update the CDR information to now point to the
         ** new call.  Right now we do not do this.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_XFERCALL,
                   reinterpret_cast<void *> ( &uXferGuid ) );

         /* Update the fsm and state of the transfered call now that it is
         ** successful.
         */
         if ( CeMain::locateRes( uXferGuid, pRes ) &&
              ( pRes != NULL ) )
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pCall =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
               pCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
               pCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Update the resource state and FSM.
      */
      if ( pResCall->resourceNoXferTerm() )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
      }
      else
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
         pResCall->resourceNetHdl()->TerminateCall();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmXfering
**
**  PURPOSE:    Call transfer from local user is attempted.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmXfering( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeResBase *pXferCall = NULL;
   const CFGCALL *pCfgCall;
   BOS_BOOL bFail = BOS_FALSE;
   BOS_BOOL bHoldCurrent;
   BOS_BOOL bHoldPending;
   BOS_BOOL bUnHoldPending;
   BOS_BOOL bHeldCurrent;
   BOS_BOOL bHeldPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Retrieve some data we may use in case service fails to execute.
      */
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_HOLD,
                                 bHoldCurrent, bHoldPending );
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_UNHOLD,
                                 bHoldCurrent, bUnHoldPending );
      pResCall->resourceGetHold( eCEEVT_HOLD_NET_HOLD,
                                 bHeldCurrent, bHeldPending );

      CeCdr *pCdr =
         CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                        pResCall->resourceGuid() );
      CeMain::locateRes( pResCall->resourceXferGuid(),
                         pXferCall );
      if ( pXferCall )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_ACCEPT );

         pCfgCall = CFG_CALL( pResCall->resourceGuid() );

         /* Apply transfer through out-of-dialog processing, this involves
         ** creating a new service to support such scenario.
         */
         if ( pCfgCall &&
              pCfgCall->xtl.supp.xfer.tfererCfg.bUseOodRefer &&
              pCdr->cdrIsTgtDlg() )
         {
            /* Create the referrer which is used to manage the service.
            */
            pResCall->resourceReferrer();
            /* Upon success of the above action, we are issuing the REFER.
            */
            if ( pResCall->resourceNetReferrer() )
            {
               if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall *pResXferCall =
                     reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );
                  MXD_GNS::IUaSspCall *pTarget = pResXferCall->resourceNetHdl();

                  pResCall->resourceNetReferrer()->Refer(
                     static_cast<MXD_GNS::IEComUnknown *>
                        ( pResCall->resourceNetHdl() ),
                     static_cast<MXD_GNS::IEComUnknown *>
                        ( pTarget ),
                     BOS_FALSE,
                     NULL );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }
            else
            {
               bFail = BOS_TRUE;
            }
         }
         /* Apply transfer through in-dialog processing.
         */
         else if ( pCfgCall &&
                   (( pCfgCall->xtl.supp.xfer.tfererCfg.bUseOodRefer &&
                      !pCdr->cdrIsTgtDlg() &&
                      pCfgCall->xtl.supp.xfer.tfererCfg.bOodRefFallBack ) ||
                    ( !pCfgCall->xtl.supp.xfer.tfererCfg.bUseOodRefer )) )
         {
            if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResXferCall =
                  reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );

               pResCall->resourceNetHdl()->Transfer(
                  pResXferCall->resourceNetHdl() );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
         /* Transfer is not applicable.
         */
         else
         {
            bFail = BOS_TRUE;
         }

         /* Start (if configured to do so) a timer to wait for the notification
         ** of transfer progress.  If this timer expires while no notification
         ** of the progress has been received yet, we would terminate the calls
         ** involved in the transfer automatically.
         */
         if ( !bFail &&
              pCfgCall &&
              pCfgCall->xtl.supp.xfer.tfererCfg.uNotifyWaitTime )
         {
            BOS_UINT32 uTimerGuid = guCeInvalidGuid;

            tkmTimerStart ( &uTimerGuid,
                            pCfgCall->xtl.supp.xfer.tfererCfg.uNotifyWaitTime,
                            BOS_FALSE,
                            NULL,
                            (TKMMSGCB) &CeMain::EvXferNotificationTimeout );

            pResCall->resourceSetXferTimerGuid( uTimerGuid );
         }
         /* Fail the transfer attempt, we cannot execute such service.
         */
         else if ( bFail )
         {
            if ( pCfgCall->xtl.supp.xfer.tfererCfg.bCleanOnFail )
            {
               pResCall->resourcePushState(
                                 CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
               pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

               pResCall->resourceNetHdl()->TerminateCall();

               if ( pXferCall )
               {
                  pXferCall->resourcePushState(
                                    CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
                  pXferCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
                  if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
                  {
                     CeResCall *pResXferCall = reinterpret_cast<CeResCall *>
                        ( pXferCall->resourceAddress() );
                     pResXferCall->resourceNetHdl()->TerminateCall();
                  }
                  else
                  {
                     fsmError( rRes );
                     return;
                  }
               }
            }
            else
            {
               if ( bHoldCurrent || bHeldCurrent ||
                     bHoldPending || bUnHoldPending )
               {
                  pResCall->resourcePushState(
                     ( (bHoldPending || bUnHoldPending) ?
                         CeFsmBase::eCEFSMSTATE_HOLD_PENDING :
                         CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT ) );
                  pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_HOLD );
               }
               else
               {
                  pResCall->resourcePushState(
                                    CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
                  pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
               }
            }
         }
      }
      /* Roll back on failure to execute the action.
      */
      else
      {
         pCfgCall = CFG_CALL( pResCall->resourceGuid() );

         if ( pCfgCall->xtl.supp.xfer.tfererCfg.bCleanOnFail )
         {
            pResCall->resourcePushState(
                              CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
            pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

            pResCall->resourceNetHdl()->TerminateCall();
         }
         else
         {
            if ( bHoldCurrent || bHeldCurrent ||
                  bHoldPending || bUnHoldPending )
            {
               pResCall->resourcePushState(
                  ( (bHoldPending || bUnHoldPending) ?
                      CeFsmBase::eCEFSMSTATE_HOLD_PENDING :
                      CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT ) );
               pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_HOLD );
            }
            else
            {
               pResCall->resourcePushState(
                                 CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
               pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmBxfering
**
**  PURPOSE:    Blind call transfer from local user is attempted.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmBxfering( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   BOS_BOOL bFail = BOS_FALSE;
   MXD_GNS::CNameAddr addr;
   MXD_GNS::IUri *pUri;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pUri = cfgCvtAddrToMx( pResCall->resourceForwardParty() );
      if ( pUri == NULL )
      {
         bFail = BOS_TRUE;
      }
      else
      {
         /* Proceed with the call to the network.
         */
         addr.SetUri( pUri );

         /* Update the resource state.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_ACCEPT );

         /* Ask the network to transfer the call.
         */
         pResCall->resourceNetHdl()->Transfer( addr );
      }

      /* Fail the transfer attempt, terminate the call, service has failed.
      */
      if ( bFail )
      {
         pResCall->resourcePushState(
                           CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         pResCall->resourceNetHdl()->TerminateCall();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmProceed
**
**  PURPOSE:    Call transfer from network is proceeding (no final answer can be
**              determined yet).
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmProceed( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeResBase *pXferCall = NULL;
   const CFGCALL *pCfgCall;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pCfgCall = CFG_CALL( pResCall->resourceGuid() );

      if ( pCfgCall &&
           pCfgCall->xtl.supp.xfer.tfererCfg.bEndOnNotify  &&
           pResCall->resourceIsXferer() )
      {
         /* Pass the ownership of the referrer back to the user for the final
         ** cleanup if applicable.
         */
         if ( pResCall->resourceNetReferrer() )
         {
            CeResBase *pResUser = NULL;

            /* Pass referrer ownership back to the parent.
            */
            if ( CeMain::locateRes( pResCall->resourceParent(),
                                    pResUser ) &&
                 ( pResUser != NULL ) )
            {
               if ( pResUser->resourceGender() == CeResBase::eCERESTYPE_USR )
               {
                  CeResUser *pUser =
                     reinterpret_cast<CeResUser *> ( pResUser->resourceAddress() );
                  pUser->resourceSetReferrer( pResCall->resourceNetReferrer() );
                  pResCall->resourceSetReferrer( NULL );
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
            }
         }

         /* Terminate the notification timer if we were running it.
         */
         if ( pResCall->resourceXferTimerGuid() != guCeInvalidGuid )
         {
            tkmTimerStop( pResCall->resourceXferTimerGuid() );
            pResCall->resourceSetXferTimerGuid( guCeInvalidGuid );
         }

         CeMain::locateRes( pResCall->resourceXferGuid(),
                            pXferCall );

         if ( pXferCall )
         {
            pXferCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
            pXferCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

            if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResXferCall =
                  reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );
               pResXferCall->resourceNetHdl()->TerminateCall();
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }

         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         pResCall->resourceNetHdl()->TerminateCall();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmNtfyTimeOut
**
**  PURPOSE:    Time out on a notification from the call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmXfer::fsmNtfyTimeOut( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeResBase *pXferCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* We timed out waiting for a notification of the transfer status, we are
      ** taking down the calls involved in the initial transfer.
      */
      if ( pResCall->resourceIsXferer() )
      {
         CeMain::locateRes( pResCall->resourceXferGuid(),
                            pXferCall );

         if ( pXferCall )
         {
            pXferCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
            pXferCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

            if ( pXferCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResXferCall =
                  reinterpret_cast<CeResCall *> ( pXferCall->resourceAddress() );
               pResXferCall->resourceNetHdl()->TerminateCall();
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }

         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         pResCall->resourceNetHdl()->TerminateCall();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmRefEnded
**
**  PURPOSE:    A referrer resource associated with this call has been terminated.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      When this happens, it means the referrer ended prematurely while
**              a transfer service was ongoing, in such case chances are the
**              service will simply not work.
***********************************************************************************/
void CeFsmXfer::fsmRefEnded( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceReferEnd();

      /* Roll back to a known (better in this situation) state.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmAnswered
**
**  PURPOSE:    A resource received answer while in call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmXfer::fsmAnswered( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::IUaSspCall *pCall = NULL;
      CeResBase *pRes = NULL;

      /* If this resource is linked to a referrer (i.e. it is linked to the party
      ** which initiated the call following a transfer), then we need to inform of
      ** the transfer status.
      */
      pResCall->resourceGetLink( pCall );
      if ( pCall )
      {
         if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
              ( pRes != NULL ))
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pC =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );
               
               MXD_GNS::ISceReferrer *pReferrer = pC->resourceNetReferrer();
               if ( pReferrer )
               {
                  pReferrer->NotifyReferProgress( MXD_GNS::uOK,
                                                  BOS_TRUE );
               }

               pResCall->resourceSetLink( NULL );

               ceNotify( pRes->resourceGuid(),
                         eCEEVT_XFER_NET_OKAY,
                         NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmRing
**
**  PURPOSE:    A resource received ring while in call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmXfer::fsmRing( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::IUaSspCall *pCall = NULL;
      CeResBase *pRes = NULL;

      /* If this resource is linked to a referrer (i.e. it is linked to the party
      ** which initiated the call following a transfer), then we need to inform of
      ** the transfer status.
      */
      pResCall->resourceGetLink( pCall );
      if ( pCall )
      {
         if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
              ( pRes != NULL ))
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pC =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

               MXD_GNS::ISceReferrer *pReferrer = pC->resourceNetReferrer();
               if ( pReferrer )
               {
                  pReferrer->NotifyReferProgress( MXD_GNS::uRINGING,
                                                  BOS_FALSE );
               }

               ceNotify( pRes->resourceGuid(),
                         eCEEVT_XFER_NET_ONGOING,
                         NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }
      
      /*
      ** If we are not the transferer, issue a media information event to force
      ** media update as needed now that the transfer is ongoing and the new remote
      ** is alerted.
      */
      if ( !pResCall->resourceIsXferer() )
      {
         pResCall->resourceGetLink( pCall );
         if ( pCall )
         {
            if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
                 ( pRes != NULL ))
            {
               BOS_UINT32 uResGuid = pResCall->resourceGuid();

               tkmEvent( pResCall->resourceParent(),
                         pRes->resourceGuid(),
                         eCCTKEVT_MEDIA_XFERING,
                         static_cast<void *> (&uResGuid) );
            }
         }
      }

   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmEarlyMedia
**
**  PURPOSE:    A resource received early media while in call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmXfer::fsmEarlyMedia( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::IUaSspCall *pCall = NULL;
      CeResBase *pRes = NULL;

      /* If this resource is linked to a referrer (i.e. it is linked to the party
      ** which initiated the call following a transfer), then we need to inform of
      ** the transfer status.
      */
      pResCall->resourceGetLink( pCall );
      if ( pCall )
      {
         if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
              ( pRes != NULL ))
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pC =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

               MXD_GNS::ISceReferrer *pReferrer = pC->resourceNetReferrer();
               if ( pReferrer )
               {
                  pReferrer->NotifyReferProgress( MXD_GNS::uSESSION_PROGRESS,
                                                  BOS_FALSE );
               }

               ceNotify( pRes->resourceGuid(),
                         eCEEVT_XFER_NET_ONGOING,
                         NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }
      
      /*
      ** If we are not the transferer, issue a media information event to force
      ** media update as needed now that the transfer is ongoing and the new remote
      ** is alerted.
      */
      if ( !pResCall->resourceIsXferer() )
      {
         pResCall->resourceGetLink( pCall );
         if ( pCall )
         {
            if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
                 ( pRes != NULL ))
            {
               BOS_UINT32 uResGuid = pResCall->resourceGuid();

               tkmEvent( pResCall->resourceParent(),
                         pRes->resourceGuid(),
                         eCCTKEVT_MEDIA_XFERING,
                         static_cast<void *> (&uResGuid) );
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmEnded
**
**  PURPOSE:    A resource ended while in call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmXfer::fsmEnded( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::IUaSspCall *pCall = NULL;
      CeResBase *pRes = NULL;

      /* If this resource is linked to a referrer (i.e. it is linked to the party
      ** which initiated the call following a transfer), then we need to inform of
      ** the transfer status.
      */
      pResCall->resourceGetLink( pCall );
      if ( pCall )
      {
         if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
              ( pRes != NULL ))
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pC =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

               MXD_GNS::ISceReferrer *pReferrer = pC->resourceNetReferrer();
               if ( pReferrer )
               {
                  pReferrer->NotifyReferProgress( MXD_GNS::uDECLINE,
                                                  BOS_TRUE );
               }

               pResCall->resourceSetLink( NULL );

               ceNotify( pRes->resourceGuid(),
                         eCEEVT_XFER_NET_FAIL,
                         NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Continue with running the common action. Note if the transfer failed
      ** it's not necessary to update the media path at this time. If required 
      ** at a later time eCCTKEVT_MEDIA_XFERING can be used to trigger the update.
      */
      CeFsmCall::fsmEnded( rRes );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmAccepted
**
**  PURPOSE:    A resource received information that the transfer attempt has 
**              been accepted.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This is *NOT* the final status of the transfer, this is simply an
**              indication that the iniitial transfer request has been successfully
**              processed and that the transfer is now ongoing, however the user
**              application may want to do something in this case as well, so we
**              use this information to publish an adequate event.  Further status
**              information about the actual progress of the transfer will come
**              next.
***********************************************************************************/
void CeFsmXfer::fsmAccepted( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_XFERACCEPT,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}

/***********************************************************************************
**  FUNCTION:   CeFsmXfer::fsmEnd
**
**  PURPOSE:    A resource termination is needed while in call transfer service.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmXfer::fsmEnd( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::IUaSspCall *pCall = NULL;
      CeResBase *pRes = NULL;

      /* If this resource is linked to  another call, then it's being terminated while
      ** a transfer is in progress, so notify the other party of the transfer failure.
      ** Also notify the referrer of the transfer failure.
      */
      pResCall->resourceGetLink( pCall );
      if ( pCall )
      {
         if ( CeMain::locateCallRes( pCall, pRes, BOS_TRUE ) &&
              ( pRes != NULL ))
         {
            if ( pRes->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pC =
                  reinterpret_cast<CeResCall *> ( pRes->resourceAddress() );

               MXD_GNS::ISceReferrer *pReferrer = pC->resourceNetReferrer();
               if ( pReferrer )
               {
                  pReferrer->NotifyReferProgress( MXD_GNS::uDECLINE,
                                                  BOS_TRUE );
               }
               else
               {
                  pResCall->resourceNetHdl()->DeclineTransfer();
               }
               pResCall->resourceSetLink( NULL );

               ceNotify( pRes->resourceGuid(),
                         eCEEVT_XFER_NET_FAIL,
                         NULL );
            }
            else
            {
               fsmError( rRes );
               return;
            }
         }
      }

      /* Continue with running the common action. Note if the transfer failed
      ** it's not necessary to update the media path at this time. If required 
      ** at a later time eCCTKEVT_MEDIA_XFERING can be used to trigger the update.
      */
      CeFsmCall::fsmEnd( rRes );
   }
   else
   {
      fsmError( rRes );
   }
}
