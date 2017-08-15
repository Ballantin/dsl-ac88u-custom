/**********************************************************************************
** Copyright (c) 2007-2013 Broadcom Corporation
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
**      This file implements the call hold finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmHold.h>
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
      CeFsmBase::eCEFSMSTATE_HOLD_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_HOLD_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_HOLD_SET_FINISH - eCEEVT_HOLD_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_HOLD_IDLE ------------------------------------------------ */
   {
        /* ---- eCEEVT_HOLD_NET_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_NET_HOLD -------------- */ &CeFsmHold::fsmHeld,
        /* ---- eCEEVT_HOLD_NET_UNHOLD ------------ */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_NET_RING -------------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_NET_TERMNET ----------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_NET_TERMUSR ----------- */ &CeFsmHold::fsmEnded,
        /* ---- eCEEVT_HOLD_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_USR_HOLD -------------- */ &CeFsmHold::fsmHold,
        /* ---- eCEEVT_HOLD_USR_UNHOLD ------------ */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_HOLD_OK ----------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_OK --------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_HOLD_FAIL --------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_FAIL ------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_HOLD_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
   },

   /* ---- eCEFSMSTATE_HOLD_PENDING --------------------------------------------- */
   {
        /* ---- eCEEVT_HOLD_NET_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_NET_HOLD -------------- */ &CeFsmHold::fsmHeld,
        /* ---- eCEEVT_HOLD_NET_UNHOLD ------------ */ &CeFsmHold::fsmUnHeld,
        /* ---- eCEEVT_HOLD_NET_RING -------------- */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_HOLD_NET_TERMNET ----------- */ &CeFsmHold::fsmEnded,
        /* ---- eCEEVT_HOLD_NET_TERMUSR ----------- */ &CeFsmHold::fsmEnded,
        /* ---- eCEEVT_HOLD_NET_XFER -------------- */ &CeFsmCall::fsmXfer,
        /* ---- eCEEVT_HOLD_USR_HOLD -------------- */ &CeFsmHold::fsmHoldCheck,
        /* ---- eCEEVT_HOLD_USR_UNHOLD ------------ */ &CeFsmHold::fsmUnHoldCheck,
        /* ---- eCEEVT_HOLD_USR_HOLD_OK ----------- */ &CeFsmHold::fsmHoldOk,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_OK --------- */ &CeFsmHold::fsmUnHoldOk,
        /* ---- eCEEVT_HOLD_USR_HOLD_FAIL --------- */ &CeFsmHold::fsmHoldFail,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_FAIL ------- */ &CeFsmHold::fsmUnHoldFail,
        /* ---- eCEEVT_HOLD_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_HOLD_USR_XFER -------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_HOLD_USR_BXFER ------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_HOLD_USR_BXFER_NOTERM ------ */ &CeFsmCall::fsmXfering,
   },

   /* ---- eCEFSMSTATE_HOLD_ACCEPT ---------------------------------------------- */
   {
        /* ---- eCEEVT_HOLD_NET_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_HOLD_NET_HOLD -------------- */ &CeFsmHold::fsmHeld,
        /* ---- eCEEVT_HOLD_NET_UNHOLD ------------ */ &CeFsmHold::fsmUnHeld,
        /* ---- eCEEVT_HOLD_NET_RING -------------- */ &CeFsmHold::fsmRing,
        /* ---- eCEEVT_HOLD_NET_TERMNET ----------- */ &CeFsmHold::fsmEnded,
        /* ---- eCEEVT_HOLD_NET_TERMUSR ----------- */ &CeFsmHold::fsmEnded,
        /* ---- eCEEVT_HOLD_NET_XFER -------------- */ &CeFsmCall::fsmXfer,
        /* ---- eCEEVT_HOLD_USR_HOLD -------------- */ &CeFsmHold::fsmHoldCheck,
        /* ---- eCEEVT_HOLD_USR_UNHOLD ------------ */ &CeFsmHold::fsmUnHold,
        /* ---- eCEEVT_HOLD_USR_HOLD_OK ----------- */ &CeFsmHold::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_OK --------- */ &CeFsmHold::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_HOLD_FAIL --------- */ &CeFsmHold::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_UNHOLD_FAIL ------- */ &CeFsmHold::fsmInvalid,
        /* ---- eCEEVT_HOLD_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_HOLD_USR_XFER -------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_HOLD_USR_BXFER ------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_HOLD_USR_BXFER_NOTERM ------ */ &CeFsmCall::fsmXfering,
   },
};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmRun
**
**  PURPOSE:    Run the call hold state machine.
**
**  PARAMETERS:
**              rRes - reference to the call resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_HOLD_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_HOLD_SET_FINISH) &&
        (eEvent > eCEEVT_HOLD_SET_BEGIN) &&
        (eEvent < eCEEVT_HOLD_SET_FINISH) )
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
                           - (CeFsmBase::eCEFSMSTATE_HOLD_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_HOLD_SET_BEGIN + 1);

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
**  FUNCTION:   CeFsmHold::fsmHold
**
**  PURPOSE:    Try to put the call on hold per user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmHold( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourceGetMse( pMse );

      if ( pMse )
      {
         CeCdr *pCdr = NULL;

         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );
         pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_FALSE, BOS_TRUE );

         pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                               pResCall->resourceGuid() );

         /* Apply the hold to the media session.
         */
         pMse->hold( ( pCdr ? pCdr->cdrIsEmergency() : BOS_FALSE ) );
      }
      else
      {
         /* Roll back on failure to execute the action.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         /* Report a failure to the application.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_LHOLD_FAIL,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmHoldCheck
**
**  PURPOSE:    Perform checks on correct status before trying to put the call
**              on hold per user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmHoldCheck( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   BOS_BOOL bCurrent;
   BOS_BOOL bHold;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceGetHold( eCEEVT_HOLD_USR_HOLD, bHold, bPending );
      if (!bHold && bPending) 
      {
         /* Silently ignore the hold request from the user if we are already
         ** processing a hold request.
         */
         return;
      }

      pResCall->resourceGetHold( eCEEVT_HOLD_USR_UNHOLD, bCurrent, bPending );
      if ( bCurrent ) /*If the phone is already on hold*/
      {
         if (bPending) 
         {
            /* We are pending an un-hold request already, we cannot process hold request
            ** right away, we have to queue it.
            */
            pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, bHold, BOS_TRUE );
         }
         
         /*Already on hold, no unhold request pending, so we can simply return */
         return;
      }

      /* We can apply the hold.
      */
      pResCall->resourceGetMse( pMse );
      if ( pMse )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );
         pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_FALSE, BOS_TRUE );

         /* Apply the hold to the media session.
         */
         pMse->hold();
      }
      else
      {
         /* Report a failure to the application.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_LHOLD_FAIL,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmHoldOk
**
**  PURPOSE:    Attempt to put the call on hold per user action succeeded.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmHoldOk( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   BOS_BOOL bCurrent;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_TRUE, BOS_FALSE );

      /* Report a success to local hold to the application.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_LHOLD,
                NULL );


      /* If we were pending an un-hold request, process it now.
      */
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_UNHOLD, bCurrent, bPending );
      if ( bPending )
      {
         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );

            /* Apply the un-hold to the media session.
            */
            pMse->unhold();
         }
         else
         {
            /* Report a failure to the application.
            */
            tkmEvent( pResCall->resourceParent(),
                      pResCall->resourceGuid(),
                      eCCTKEVT_LUNHOLD_FAIL,
                      NULL );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmHoldFail
**
**  PURPOSE:    Attempt to put the call on hold per user action failed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmHoldFail( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   BOS_BOOL bCurrent;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_IDLE );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_FALSE, BOS_FALSE );

      /* Check to see if the hold failed from a REINVITE timeout
      */
      const MXD_GNS::CSipPacket *pPacket = NULL;
      const CFGSYS *pSysCfg = CFG_SYS;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      BOS_BOOL bTargetFailure = BOS_FALSE;
      void *pData = NULL;
      
      mxt_result res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* Handle locally generated timeout reponse
            */
            if ( pPacket->IsResponse() &&
                       pPacket->IsLocallyGenerated() &&
                          pPacket->GetStatusLine()->GetCode() 
                                      == MXD_GNS::uREQUEST_TIMEOUT )
            {
               if ( ( pSysCfg &&
                      UTIL_BMP_IS_ENABLED_ALL(
                                       pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                       CCTK_SIGBEHVRX_REGONHLDREINVTIMEOUT ) ) &&
                        !pResCall->resourceIsSigRestrictedToAddr() )
               {
                  /* Notify that a target failure has occured, and registration
                  ** action is required. Note, the specific registration action 
                  ** depends on the configuration - either a registration refresh
                  ** to the same target or a new registration to a new target.
                  */ 
                  CCTKRESPCODE eResponseCode = eCCTKRESPCODE_TIMEOUT;
                  pData = (void *)&eResponseCode;
                  bTargetFailure = BOS_TRUE;
                  
               }
            }
            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }         

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      /* Report a failure to local hold to the application.
      */

      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_LHOLD_FAIL,
                pData );



      /* If we were pending an un-hold request, process it now.
      */
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_UNHOLD, bCurrent, bPending );
      if ( bPending )
      {
         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );

            /* Apply the un-hold to the media session.
            */
            pMse->unhold();
         }
         else
         {
            /* Report a failure to the application.
            */
            tkmEvent( pResCall->resourceParent(),
                      pResCall->resourceGuid(),
                      eCCTKEVT_LUNHOLD_FAIL,
                      NULL );
         }
      }
      /* Check whether we are still in hold state (possible if we have been held)
      ** or if we should defer processing to the Call FSM.
      */
      else
      {
         pResCall->resourceGetHold( eCEEVT_HOLD_NET_HOLD, bCurrent, bPending );
         if ( bCurrent )
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
         }
         else
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
            pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
         }
      }
      
      /* Notify the parent about the target failure if it occured.
      */
      if ( bTargetFailure )
      {
         ceNotify ( pResCall->resourceParent(),
                             eCEEVT_REG_USR_TARGET_FAIL,
                             NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmUnHold
**
**  PURPOSE:    Try to un-hold a call on hold per user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmUnHold( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourceGetMse( pMse );

      if ( pMse )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );
         pResCall->resourceSetHold( eCEEVT_HOLD_USR_UNHOLD, BOS_TRUE, BOS_TRUE );

         /* Apply the un-hold to the media session.
         */
         pMse->unhold();
      }
      else
      {
         /* Report a failure to the application.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_LUNHOLD_FAIL,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmUnHoldCheck
**
**  PURPOSE:    Perform checks on correct status before trying to put the call
**              off hold (unhold) per user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmUnHoldCheck( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   BOS_BOOL bHold;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceGetHold( eCEEVT_HOLD_USR_UNHOLD, bHold, bPending );
      if ( bPending )
      {
         /* Silently ignore the un-hold request from the user if we are already
         ** processing a un-hold request for it.
         */
         return;
      }

      pResCall->resourceGetMse( pMse );
      if ( pMse )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );
         pResCall->resourceSetHold( eCEEVT_HOLD_USR_UNHOLD, bHold, BOS_TRUE );

         /* Apply the un-hold to the media session.
         */
         pMse->unhold();
      }
      else
      {
         /* Report a failure to the application.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_LUNHOLD_FAIL,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmUnHoldOk
**
**  PURPOSE:    Attempt to put the call off hold (un-hold) per user action
**              succeeded.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmUnHoldOk( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   BOS_BOOL bCurrent;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_IDLE );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_UNHOLD, BOS_FALSE, BOS_FALSE );

      /* Report a success to local un-hold to the application.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_LUNHOLD,
                NULL );


      /* If we were pending an hold request, process it now.
      */
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_HOLD, bCurrent, bPending );
      if ( bPending )
      {
         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_PENDING );

            /* Apply the hold to the media session.
            */
            pMse->hold();
         }
         else
         {
            /* Report a failure to the application.
            */
            tkmEvent( pResCall->resourceParent(),
                      pResCall->resourceGuid(),
                      eCCTKEVT_LHOLD_FAIL,
                      NULL );
         }
      }
      /* Check whether we are still in hold state (possible if we have been held)
      ** or if we should defer processing to the Call FSM.
      */
      else
      {
         pResCall->resourceGetHold( eCEEVT_HOLD_NET_HOLD, bCurrent, bPending );
         if ( bCurrent )
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
         }
         else
         {
            pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
            pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmUnHoldFail
**
**  PURPOSE:    Attempt to put the call off hold (un-hold) per user action failed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmUnHoldFail( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* We are still on hold.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );

      /* This will also cancel a possible pending hold request that was
      ** queued and to be processed after the un-hold request termination.
      */
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_TRUE, BOS_FALSE );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_UNHOLD, BOS_TRUE, BOS_FALSE );

      /* Report a failure to local un-hold to the application.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_LUNHOLD_FAIL,
                NULL );

      /* Check to see if the unhold failed from a REINVITE timeout and if
      ** so whether a target failure needs to be reported to the parent
      ** resource to handle.
      */
      const MXD_GNS::CSipPacket *pPacket = NULL;
      const CFGSYS *pSysCfg = CFG_SYS;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      
      mxt_result res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* Handle locally generated timeout reponse
            */
            if ( pPacket->IsResponse() &&
                       pPacket->IsLocallyGenerated() &&
                          pPacket->GetStatusLine()->GetCode() 
                                      == MXD_GNS::uREQUEST_TIMEOUT )
            {
               if ( ( pSysCfg &&
                      UTIL_BMP_IS_ENABLED_ALL(
                                       pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                       CCTK_SIGBEHVRX_REGONHLDREINVTIMEOUT ) ) &&
                        !pResCall->resourceIsSigRestrictedToAddr() )
               {
                  /* Notify that a target failure has occured, and registration
                  ** action is required. Note, the specific registration action 
                  ** depends on the configuration - either a registration refresh
                  ** to the same target or a new registration to a new target.
                  */ 
                  ceNotify ( pResCall->resourceParent(),
                             eCEEVT_REG_USR_TARGET_FAIL,
                             NULL );
                  
               }
            }
            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }         

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmHeld
**
**  PURPOSE:    The call has been put on hold (i.e. held) by the peer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmHeld( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceSetHold( eCEEVT_HOLD_NET_HOLD, BOS_TRUE, BOS_FALSE );

      /* Report a status to the application.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_HOLD,
                NULL );

      /* Update our state unless we are pending a local action already.
      */
      if ( pResCall->resourceState() != CeFsmBase::eCEFSMSTATE_HOLD_PENDING )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmUnHeld
**
**  PURPOSE:    The call has been put off hold (i.e. un-held) by the peer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmUnHeld( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   BOS_BOOL bCurrent;
   BOS_BOOL bPending;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceSetHold( eCEEVT_HOLD_NET_HOLD, BOS_FALSE, BOS_FALSE );

      /* Report a status to the application.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_UNHOLD,
                NULL );

      /* Update our state based on what we are doing locally.
      */
      pResCall->resourceGetHold( eCEEVT_HOLD_USR_HOLD, bCurrent, bPending );
      if ( bCurrent &&
           (pResCall->resourceState() != CeFsmBase::eCEFSMSTATE_HOLD_PENDING) )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
      }
      else if ( pResCall->resourceState() != CeFsmBase::eCEFSMSTATE_HOLD_PENDING )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmRing
**
**  PURPOSE:    The call rings again.  This is the case when a 'network hold'
**              scenario is putting the call off hold.  Such example of service is
**              used during emergency calling.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmRing( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      CeCdr *pCdr = NULL;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Clean up the hold state.
      */
      pResCall->resourceSetHold( eCEEVT_HOLD_NET_HOLD, BOS_FALSE, BOS_FALSE );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_FALSE, BOS_FALSE );

      /* Exit back to call incoming FSM and pass the event to the state
      ** machine.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLIN_IDLE );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_IN );

      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );

      /* Operator ringback type of scenario for an emergency call, mark it as
      ** such because of needed special processing.
      */
      if ( pCdr && pCdr->cdrIsEmergency() )
      {
         pResCall->resourceSetEmergRingbck( BOS_TRUE );
      }

      ceNotify( pResCall->resourceGuid(),
                eCEEVT_HOLD_NET_RING,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmHold::fsmEnded
**
**  PURPOSE:    The call was on hold but has been terminated.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmHold::fsmEnded( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Clean up the hold state.
      */
      pResCall->resourceSetHold( eCEEVT_HOLD_NET_HOLD, BOS_FALSE, BOS_FALSE );
      pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD, BOS_FALSE, BOS_FALSE );

      /* Exit back to call incoming FSM and pass the event to the state
      ** machine.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      ceNotify( pResCall->resourceGuid(),
                eCEEVT_HOLD_NET_TERMNET,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}
