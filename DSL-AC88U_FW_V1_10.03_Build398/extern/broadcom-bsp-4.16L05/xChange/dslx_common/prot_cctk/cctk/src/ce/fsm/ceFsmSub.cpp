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
**      This file implements the subscriber finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmSub.h>
#include <ceMain.h>
#include <ceDlg.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CE_FSM__SUB__USR_AGT_PROF__TIMEOUT_SEC         86400

#define CE_FSM__SUB__NTFY_REJ_NOT_SUB                  "Not Subscribed"


/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_SUB_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_SUB_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_SUB_SET_FINISH - eCEEVT_SUB_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_SUB_IDLE --------------------------------------------- */
   {
        /* ---- eCEEVT_SUB_NET_OK ----------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_SUBTRYING ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_REFRESH ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_FAIL --------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_ENDED -------------- */ &CeFsmSub::fsmSubEnd,
        /* ---- eCEEVT_SUB_NET_UNSUBTRYING -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_UNSUB -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_RETRY -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_NOTIFY ------------- */ &CeFsmSub::fsmUnsolNtfy,
        /* ---- eCEEVT_SUB_NET_REQ ---------------- */ &CeFsmSub::fsmSubAccept,
        /* ---- eCEEVT_SUB_NET_REQEND ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_CREATE ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_SUBSCRIBE ---------- */ &CeFsmSub::fsmSubAttempt,
        /* ---- eCEEVT_SUB_USR_UNSUBSCRIBE -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_NOTIFY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_ENDED -------------- */ &CeFsmSub::fsmSubEnd,
   },

   /* ---- eCEFSMSTATE_SUB_SUBING ------------------------------------------- */
   {
        /* ---- eCEEVT_SUB_NET_OK ----------------- */ &CeFsmSub::fsmSub,
        /* ---- eCEEVT_SUB_NET_SUBTRYING ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_REFRESH ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_FAIL --------------- */ &CeFsmSub::fsmSubFail,
        /* ---- eCEEVT_SUB_NET_ENDED -------------- */ &CeFsmSub::fsmSubEnd,
        /* ---- eCEEVT_SUB_NET_UNSUBTRYING -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_UNSUB -------------- */ &CeFsmSub::fsmSubUnsub,
        /* ---- eCEEVT_SUB_NET_RETRY -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_NOTIFY ------------- */ &CeFsmSub::fsmSolNtfy,
        /* ---- eCEEVT_SUB_NET_REQ ---------------- */ &CeFsmSub::fsmSubReject,
        /* ---- eCEEVT_SUB_NET_REQEND ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_CREATE ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_SUBSCRIBE ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_UNSUBSCRIBE -------- */ &CeFsmSub::fsmSubTerm,
        /* ---- eCEEVT_SUB_USR_NOTIFY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_ENDED -------------- */ &CeFsmSub::fsmSubTerm,
   },

   /* ---- eCEFSMSTATE_SUB_UNSUBING ----------------------------------------- */
   {
        /* ---- eCEEVT_SUB_NET_OK ----------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_SUBTRYING ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_REFRESH ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_FAIL --------------- */ &CeFsmSub::fsmSubFail,
        /* ---- eCEEVT_SUB_NET_ENDED -------------- */ &CeFsmSub::fsmSubEnd,
        /* ---- eCEEVT_SUB_NET_UNSUBTRYING -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_UNSUB -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_RETRY -------------- */ &CeFsmSub::fsmSubTry,
        /* ---- eCEEVT_SUB_NET_NOTIFY ------------- */ &CeFsmSub::fsmRejNtfy,
        /* ---- eCEEVT_SUB_NET_REQ ---------------- */ &CeFsmSub::fsmSubReject,
        /* ---- eCEEVT_SUB_NET_REQEND ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_CREATE ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_SUBSCRIBE ---------- */ &CeFsmSub::fsmSubPending,
        /* ---- eCEEVT_SUB_USR_UNSUBSCRIBE -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_NOTIFY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_ENDED -------------- */ &CeFsmSub::fsmSubPending,
   },

   /* ---- eCEFSMSTATE_SUB_SUBED -------------------------------------------- */
   {
        /* ---- eCEEVT_SUB_NET_OK ----------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_SUBTRYING ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_REFRESH ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_FAIL --------------- */ &CeFsmSub::fsmSubFail,
        /* ---- eCEEVT_SUB_NET_ENDED -------------- */ &CeFsmSub::fsmSubEnd,
        /* ---- eCEEVT_SUB_NET_UNSUBTRYING -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_UNSUB -------------- */ &CeFsmSub::fsmSubUnsub,
        /* ---- eCEEVT_SUB_NET_RETRY -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_NET_NOTIFY ------------- */ &CeFsmSub::fsmSolNtfy,
        /* ---- eCEEVT_SUB_NET_REQ ---------------- */ &CeFsmSub::fsmSubAccept,
        /* ---- eCEEVT_SUB_NET_REQEND ------------- */ &CeFsmSub::fsmSubEnded,
        /* ---- eCEEVT_SUB_USR_CREATE ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_SUBSCRIBE ---------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_SUB_USR_UNSUBSCRIBE -------- */ &CeFsmSub::fsmSubTerm,
        /* ---- eCEEVT_SUB_USR_NOTIFY ------------- */ &CeFsmSub::fsmSubAdvert,
        /* ---- eCEEVT_SUB_USR_ENDED -------------- */ &CeFsmSub::fsmSubTerm,
   },
};


/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmRun
**
**  PURPOSE:    Run the subscriber state machine.
**
**  PARAMETERS:
**              rRes - reference to the resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_SUB_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_SUB_SET_FINISH) &&
        (eEvent > eCEEVT_SUB_SET_BEGIN) &&
        (eEvent < eCEEVT_SUB_SET_FINISH) )
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
                           - (CeFsmBase::eCEFSMSTATE_SUB_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_SUB_SET_BEGIN + 1);

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
**  FUNCTION:   CeFsmSub::fsmSubFail
**
**  PURPOSE:    Subscription has failed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubFail( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      const CFGSYS *pSysCfg = CFG_SYS;

      BOS_BOOL bRefresh = (rRes.resourceState() == 
                           CeFsmBase::eCEFSMSTATE_SUB_SUBED)? BOS_TRUE : BOS_FALSE;

      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );
      pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );

      /* Check whether we need to apply some failover processing in case
      ** of abnormal termination.
      */
      if ( pResSub->resourceBasicCtl( &pSceBasExtCtl ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( bRefresh &&
              pPacket &&
              pPacket->IsResponse() &&
              pPacket->IsLocallyGenerated() && 
              ( pPacket->GetStatusLine()->GetCode() 
                                      == MXD_GNS::uREQUEST_TIMEOUT ) &&
              pSysCfg &&
              UTIL_BMP_IS_ENABLED_ALL(
                                 pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                 CCTK_SIGBEHVRX_REGONSUBTIMEOUT ) )
         {
            /* Notify that a target failure has occured, and registration
            ** action is required. Note, the specific registration action 
            ** depends on the configuration - either a registration refresh
            ** to the same target or a new registration to a new target.
            */ 
            ceNotify ( pResSub->resourceParent(),
                       eCEEVT_REG_USR_TARGET_FAIL,
                       NULL );
         }         
         /* 
         ** Finished with the signaling packet.
         */
         if ( pPacket )
         {
            pPacket->Release();
            pPacket = NULL;
         }

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      if (pResSub->resourceType() == eCCTKSUBEVT_MWI) 
      {
         BOS_BOOL bSubscribed = BOS_FALSE;
         /*For MWI subscription, report the status of the subscription*/
         tkmEvent( pResSub->resourceParent(),
                   guCeInvalidGuid,
                   eCCTKEVT_SUB_STATUS,
                   &bSubscribed );
      }

      /* Inform the application of the failure.  Let it decide what to do
      ** next (reconfigure, terminate, etc).
      */
      if ( pResSub->resourceInform() )
      {
         tkmEvent( pResSub->resourceParent(),
                   pResSub->resourceGuid(),
                   eCCTKEVT_ENDSVC,
                   NULL );
      }
      /* Do not inform the application of the failure because this is an
      ** internal event.  Terminate it now.
      */
      else
      {
         pResSub->resourceTerminate();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubUnsub
**
**  PURPOSE:    Subscription is unsubscribed (so failed).
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubUnsub( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      const CFGSYS *pSysCfg = CFG_SYS;

      BOS_BOOL bRefresh = (rRes.resourceState() == 
                           CeFsmBase::eCEFSMSTATE_SUB_SUBED)? BOS_TRUE : BOS_FALSE;
      
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      /* Check whether we need to apply some failover processing in case
      ** of abnormal termination.
      */
      if ( pResSub->resourceBasicCtl( &pSceBasExtCtl ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            if ( bRefresh &&
                 pPacket->IsResponse() &&
                 pPacket->IsLocallyGenerated() && 
                 ( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uREQUEST_TIMEOUT ) &&
                 pSysCfg &&
                 UTIL_BMP_IS_ENABLED_ALL(
                                    pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                    CCTK_SIGBEHVRX_REGONSUBTIMEOUT ) )
            {
               /* 
               ** Issue user-register event to trigger registration
               ** refresh.
               */
               cctkUserRegister ( pResSub->resourceParent() );
            }

            /* Finished with the signaling packet.
            */
            pPacket->Release();
            pPacket = NULL;
         }

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      if (pResSub->resourceType() == eCCTKSUBEVT_MWI) 
      {
         BOS_BOOL bSubscribed = BOS_FALSE;
         /*For MWI subscription, report the status of the subscription*/
         tkmEvent( pResSub->resourceParent(),
                   guCeInvalidGuid,
                   eCCTKEVT_SUB_STATUS,
                   &bSubscribed );
      }

   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubEnd
**
**  PURPOSE:    Subscription has ended.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubEnd( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );
      /* Subscription terminated.
      */
      BOS_BOOL bUnsubing = ( rRes.resourceState() == eCEFSMSTATE_SUB_UNSUBING )?
                           BOS_TRUE : BOS_FALSE;
     
      pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );

      /* If there is a pending subscribe request, try it now.
      */
      if ( bUnsubing && pResSub->resourceSubPending() )
      {
         pResSub->resourceSubPending( BOS_FALSE );
         
         ceNotify ( pResSub->resourceGuid(),
                    eCEEVT_SUB_USR_SUBSCRIBE,
                    NULL );
         
      }
      else
      {

         if (pResSub->resourceType() == eCCTKSUBEVT_MWI) 
         {
            BOS_BOOL bSubscribed = BOS_FALSE;
            /*For MWI subscription, report the status of the subscription*/
            tkmEvent( pResSub->resourceParent(),
                      guCeInvalidGuid,
                      eCCTKEVT_SUB_STATUS,
                      &bSubscribed );
         }

         /* If this resource was associated with a notification, we do not inform
         ** the upper layer application because it did not need to know about the
         ** particular resource (not managed by it, since managed exclusively within
         ** the Call Engine (CCTK/CE)).
         */
         if ( !pResSub->resourceNotHdl() && pResSub->resourceInform() )
         {
            tkmEvent( pResSub->resourceParent(),
                      pResSub->resourceGuid(),
                      eCCTKEVT_ENDSVC,
                      NULL );
         }

         /* If this is the end of a third party subscription to some data we were
         ** providing, free up the resource as it is no longer needed.
         */
         pResSub->resourceTerminate();
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubTry
**
**  PURPOSE:    Subscription is being attempted.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubTry( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );
      /* Reset any pending subscriptions
      */
      pResSub->resourceSubPending( BOS_FALSE );

      pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBING );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSub
**
**  PURPOSE:    Subscription is successful.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSub( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );
      /* Let the application know of the status.
      */
      pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBED );

      if (pResSub->resourceType() == eCCTKSUBEVT_MWI) 
      {
         BOS_BOOL bSubscribed = BOS_TRUE;
         /*For MWI subscription, report the status of the subscription*/
         tkmEvent( pResSub->resourceParent(),
                   guCeInvalidGuid,
                   eCCTKEVT_SUB_STATUS,
                   &bSubscribed );
      }

      if ( pResSub->resourceInform() )
      {
         tkmEvent( pResSub->resourceParent(),
                   pResSub->resourceGuid(),
                   eCCTKEVT_CONFSVC,
                   NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubAttempt
**
**  PURPOSE:    Subscription is being attempted by user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubAttempt( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;
   mxt_result res;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      /* Reset any pending requests to subscribe.
      */
      pResSub->resourceSubPending( BOS_FALSE );


      switch( pResSub->resourceType() )
      {
         case eCCTKSUBEVT_MWI:
         {
            const CFGUSR *pCfgUsr;

            pCfgUsr = CFG_USR( pResSub->resourceParent() );
            if ( pCfgUsr )
            {
               pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBING );
               pResSub->resourceMwiHdl()->Subscribe( NULL );
            }
         }
         break;

         case eCCTKSUBEVT_DLG:
         {
            MXD_GNS::CNameAddr *pAddr;
            MXD_GNS::IUri *pUri;

            pAddr = BRCM_NEW( MXD_GNS::CNameAddr );

            if ( pAddr )
            {
               /* Convert the called party information into a proper URI structure, if
               ** necessary.
               */
               pUri = cfgCvtAddrToMx( pResSub->resourceSubedParty() );
               pAddr->SetUri( pUri );

               res = pResSub->resourceSubHdl()->Subscribe(
                        pAddr,
                        NULL,
                        MXD_GNS::eEVENTTYPE_DIALOG,
                        static_cast<uint32_t> (
                          pResSub->resourceSubTimeOut()),
                        NULL,
                        NULL,
                        NULL );
               pAddr = NULL;

               if ( MX_RIS_S( res ) )
               {
                  pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBING );
               }
               else
               {
                  fsmSubFail( rRes );
               }
            }
            else
            {
               fsmSubFail( rRes );
            }
         }
         break;

         case eCCTKSUBEVT_UAP:
         {
            const CFGUSR *pCfgUsr;

            pCfgUsr = CFG_USR( pResSub->resourceParent() );
            if ( pCfgUsr )
            {
               MXD_GNS::CNameAddr *pAddr;
               pAddr = BRCM_NEW( MXD_GNS::CNameAddr )( 
                           (pCfgUsr->in.pSceUsrCfg)->GetUserAddress() );
               if ( pAddr )
               {
                  res = pResSub->resourceSubHdl()->Subscribe(
                           pAddr,
                           NULL,
                           MXD_GNS::eEVENTTYPE_USR_AGT_PROF,
                           ( pCfgUsr->xtl.usr.net.uapEvt.uSubExpTmr ?
                              static_cast<uint32_t> (
                                 pCfgUsr->xtl.usr.net.uapEvt.uSubExpTmr ):
                                 CE_FSM__SUB__USR_AGT_PROF__TIMEOUT_SEC ),
                           NULL,
                           NULL,
                           NULL );
                  pAddr = NULL;

                  if ( MX_RIS_S( res ) )
                  {
                     pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBING );
                  }
                  else
                  {
                     fsmSubFail( rRes );
                  }
               }
               else
               {
                  fsmSubFail( rRes );
               }
            }
         }
         break;

         case eCCTKSUBEVT_REG:
         case eCCTKSUBEVT_MAX:
         case eCCTKSUBEVT_BMWI:
         default:
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubTerm
**
**  PURPOSE:    Subscription is being terminated by user action.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubTerm( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      /* Reset any pending requests to subscribe.
      */
      pResSub->resourceSubPending( BOS_FALSE );


      switch( pResSub->resourceType() )
      {
         case eCCTKSUBEVT_MWI:
         {
            const CFGUSR *pCfgUsr;

            pCfgUsr = CFG_USR( pResSub->resourceParent() );
            if ( pCfgUsr )
            {
               pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_UNSUBING );
               pResSub->resourceMwiHdl()->Unsubscribe();
            }
         }
         break;

         case eCCTKSUBEVT_DLG:
         case eCCTKSUBEVT_UAP:
         {
            pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_UNSUBING );
            pResSub->resourceSubHdl()->Unsubscribe();
         }
         break;

         case eCCTKSUBEVT_BMWI:
         {
            pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_UNSUBING );
            pResSub->resourceBlindMwiHdl()->Terminate();
         }
         break;

         case eCCTKSUBEVT_REG:
         case eCCTKSUBEVT_MAX:
         default:
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmUnsolNtfy
**
**  PURPOSE:    Receives an unsolicited notification for this subscription.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      Unsolicited notification are for components which have not
**              'Subscribe' to any event at this time, yet are monitored for
**              notifications.  As example, message waiting indication uses
**              this mechanism.
***********************************************************************************/
void CeFsmSub::fsmUnsolNtfy( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      if ( ( pResSub->resourceType() == eCCTKSUBEVT_MWI ) ||
           /* This should not happen for blind MWI since the resource
           ** is always subscribed in that case anyways.
           */
           ( pResSub->resourceType() == eCCTKSUBEVT_BMWI ) ||
           /* This is the case when an unsolicited notification has
           ** been received for the user-agent event package information.
           */
           ( pResSub->resourceType() == eCCTKSUBEVT_UAP ) )
      {
         /* Just do as if this came from a solicited notification.
         */
         fsmSolNtfy( rRes );

         if ( pResSub->resourceRelease() )
         {
            /* If the resource was created temporarely to support this
            ** particular blind notification, release it now.
            */
            ceNotify ( pResSub->resourceGuid(),
                       eCEEVT_SUB_NET_ENDED,
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
**  FUNCTION:   CeFsmSub::fsmSolNtfy
**
**  PURPOSE:    Receives a solicited notification for this subscription.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmSub::fsmSolNtfy( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      const CFGUSR *pCfgUsr;

      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      switch ( pResSub->resourceType() )
      {
         case eCCTKSUBEVT_MWI:
         case eCCTKSUBEVT_BMWI:
         {
            BOS_BOOL bNew = BOS_FALSE;

            if ( pResSub->resourceGetNewMwiMsgSummary() != NULL )
            {
               CCTKMWIINFO *pNewMwi = pResSub->resourceGetNewMwiMsgSummary();
               CCTKMWIINFO *pCurMwi = pResSub->resourceGetCurMwiMsgSummary();

               if ( memcmp( static_cast<const void *> ( pNewMwi ),
                            static_cast<const void *> ( pCurMwi ),
                            sizeof( CCTKMWIINFO )) != 0 )
               {
                  bNew = BOS_TRUE;
                  memcpy( static_cast<void *> ( pCurMwi ),
                          static_cast<const void *> ( pNewMwi ),                        
                          sizeof( CCTKMWIINFO ) );
               }

               /* By default, we would only report an event to the upper layer if
               ** we have new information to share, but the application can change
               ** this behavior is desired.
               */
               pCfgUsr = CFG_USR( pResSub->resourceParent() );
               if ( pCfgUsr )
               {
                  if ( pCfgUsr->xtl.mwi.gen.cfg.mode.eEvtPolicy ==
                                                eCCTKMWIEVT_NOTIFY_ALWAYS )
                  {
                     bNew = BOS_TRUE;
                  }
               }

               if ( bNew )
               {
                  tkmEvent( pResSub->resourceParent(),
                            guCeInvalidGuid,
                            eCCTKEVT_MWINOTIFY,
                            static_cast<void *> (
                                 pResSub->resourceGetNewMwiMsgSummary()) );
               }
            }
         }
         break;

         case eCCTKSUBEVT_DLG:
         {
            BOS_BOOL bBusy = BOS_TRUE;
            BOS_UINT8 *pNotBody = pResSub->resourceNotBodyData( BOS_TRUE );

            if ( pNotBody )
            {
               /* The notification body contains information about dialog(s).
               **
               ** We only care (at this time) to know whether the notifier
               ** is busy or not.  We determine the 'busy' state by the number
               ** of dialog(s) associated with the notifier.  If this number
               ** is zero, we will assume 'idle'.  If this number is anything but
               ** zero, we will assume 'busy'.
               **
               ** Furthermore, the notifier may have hidden its dialog information
               ** from us for privacy (see RFC 4235 section 6.3 for example).  So
               ** another reason why we do not need to get too deeply involved with
               ** the actual content.
               */
               CeDlg dlgInfo;

               if ( dlgInfo.ceDlgParse ( pResSub->resourceNotBodySize(),
                                         pNotBody ) )
               {
                  if ( !dlgInfo.ceDlgInfo() )
                  {
                     bBusy = BOS_FALSE;
                  }
               }

               BRCM_DELETE_ARRAY ( pNotBody );
               pNotBody = NULL;
            }

            if ( pResSub->resourceAddtionalNotInfo() )
            {
               static_cast<MXD_GNS::ISipServerEventControl *>
                  (pResSub->resourceAddtionalNotInfo())->SendResponse( MXD_GNS::uOK,
                                                                       NULL,
                                                                       NULL,
                                                                       NULL );
            }

            tkmEvent( pResSub->resourceParent(),
                      pResSub->resourceGuid(),
                      eCCTKEVT_DLGNOTIFY,
                      static_cast<void *> ( &bBusy ) );
         }
         break;

         case eCCTKSUBEVT_UAP:
         {
            BOS_UINT8 *pNotBody = pResSub->resourceNotBodyData( BOS_TRUE );
            BOS_BOOL bFwdCnt = BOS_FALSE;

            if ( pNotBody )
            {
               CeUap uapInfo;

               if ( uapInfo.ceUapParse ( pResSub->resourceNotBodySize(),
                                         pNotBody ) )
               {
                  /* Save the result at the resource level.
                  */
                  CCTKFEATINFO featInfo;

                  featInfo.bCfv = uapInfo.ceUapCfv();
                  featInfo.bDnd = uapInfo.ceUapDnd();
                  featInfo.bScf = uapInfo.ceUapScf();
                  
                  featInfo.uCfvCnt = uapInfo.ceUapCfvCnt();
                  featInfo.uScfCnt = uapInfo.ceUapScfCnt();

                  pResSub->resourceSetFeatInfo( featInfo );

                  if ( ( featInfo.uCfvCnt != guCeInvalidGuid ) ||
                       ( featInfo.uScfCnt != guCeInvalidGuid ) )
                  {
                     bFwdCnt = BOS_TRUE;
                  }
               }

               BRCM_DELETE_ARRAY ( pNotBody );
               pNotBody = NULL;
            }

            if ( pResSub->resourceAddtionalNotInfo() )
            {
               static_cast<MXD_GNS::ISipServerEventControl *>
                  (pResSub->resourceAddtionalNotInfo())->SendResponse( MXD_GNS::uOK,
                                                                       NULL,
                                                                       NULL,
                                                                       NULL );
            }

            /* If we discovered some forwarded calls, we need to inform the
            ** application of the situation.
            */
            if ( bFwdCnt )
            {
               tkmEvent( pResSub->resourceParent(),
                         guCeInvalidGuid,
                         eCCTKEVT_FWDCALLNOTIFY,
                         static_cast<void *> ( pResSub->resourceGetFeatInfo() ) );
            }
         }
         break;

         default:
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmRejNtfy
**
**  PURPOSE:    Reject a notification for this subscription.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This happens when we receive a notification for a subscription
**              that is not in a proper state to receive such notification as
**              defined by the subscription resource state machine, we still need
**              to explicitely issue the final response for the notification
**              however.
***********************************************************************************/
void CeFsmSub::fsmRejNtfy( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      if ( pResSub->resourceAddtionalNotInfo() )
      {
         static_cast<MXD_GNS::ISipServerEventControl *>
            (pResSub->resourceAddtionalNotInfo())->SendResponse(
                                       MXD_GNS::uTEMPORARILY_NOT_AVAILABLE,
                                       CE_FSM__SUB__NTFY_REJ_NOT_SUB,
                                       NULL,
                                       NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubAccept
**
**  PURPOSE:    Accepting a solicitied subscription from a peer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      The subscription may still be rejected if we decide that it is not
**              acceptable, but in principle, at this stage, we would try to honor
**              the request since we have all the resources allocated for it.
***********************************************************************************/
void CeFsmSub::fsmSubAccept( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      if ( pResSub->resourceNotHdl() )
      {
         switch ( pResSub->resourceType() )
         {
            /* There are two types of third party user which may be attempting
            ** to subscribe to this event.
            **
            **    [1] Authenticated user.
            **          - User for which the P-Asserted-Identity is set and
            **            must match a pre-configured value, in which case
            **            this user is allowed to 'see' the full content
            **            and state of the current dialog(s) associated with
            **            the user.
            **          - This is used for third party operator service
            **            interrupt type of feature where an operator (aka an
            **            administrator) can request view of the user dialogs.
            **
            **    [2] Unauthenticated user.
            **          - User for which the P-Asserted-Identity is either not
            **            set or does not match a pre-configured value, in which
            **            case this user is allowed to 'see' only whether dialog(s)
            **            is(are) associated with the user.
            **          - This is used for peer-to-peer features such as auto
            **            recall or auto callback when a peer just needs to know
            **            the presence (or not) of at least one dialog in order
            **            to activate a feature on its side.
            */
            case eCCTKSUBEVT_DLG:
            {
               BOS_BOOL bDlg;
               BOS_BOOL bAccept = BOS_TRUE;
               const MXD_GNS::CNameAddr *pNaiSip = NULL;
               const MXD_GNS::CNameAddr *pNaiTel = NULL;
               MXD_GNS::ISceGenericEventInfo *pSceGenEvt = NULL;
               MXD_GNS::IEComUnknown *pEcomInfo = NULL;

               /* If this is a new subscription, ensure we have not reached our
               ** threshold of acceptable incoming subscription already.
               */
               if ( pResSub->resourceState() == CeFsmBase::eCEFSMSTATE_SUB_IDLE )
               {
                  const CFGUSR *pCfgUsr = CFG_USR( pResSub->resourceParent() );
                  BOS_UINT32 uDlgCnt = CeMain::countDlg( pResSub->resourceParent(),
                                                         BOS_FALSE );

                  if ( pCfgUsr )
                  {
                     if ( uDlgCnt > pCfgUsr->xtl.usr.net.diagEvt.uSubeeMax )

                     {
                        /* Do not allow more subscription.
                        */
                        bAccept = BOS_FALSE;
                     }
                  }
               }

               if ( bAccept )
               {
                  /* Accept the subscription.
                  */
                  pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_SUBED );
                  pResSub->resourceNotHdl()->AcceptSubscription(
                                                    MXD_GNS::ISceNotifier::eACTIVE,
                                                    pResSub->resourceSubTimeOut());

                  /* Determine whether this is a full or partial view of the user
                  ** dialog information.
                  */
                  pResSub->resourceDlgFullSet( BOS_FALSE );
                  pEcomInfo = (pResSub->resourceAddtionalSubInfo() ?
                                 static_cast<MXD_GNS::IEComUnknown *> (
                                    pResSub->resourceAddtionalSubInfo()) : NULL);
                  if ( pEcomInfo )
                  {
                     pEcomInfo->QueryIf( &pSceGenEvt );
                     pEcomInfo = NULL;
                  }

                  if ( pSceGenEvt != NULL )
                  {
                     const CFGUSR *pCfgUsr = CFG_USR( pResSub->resourceParent() );
                     pSceGenEvt->GetNetworkAssertedIdentity( pNaiSip, pNaiTel );

                     if ( ( pNaiSip != NULL ) && ( pCfgUsr != NULL ) )
                     {
                        const MXD_GNS::CSipUri *pUri =
                           reinterpret_cast<const MXD_GNS::CSipUri *> (
                                                            pNaiSip->GetUri() );

                        if ( !strCmpNoCase(
                                pUri->GetUser().GetString().CStr(),
                                  reinterpret_cast<const char *> (
                                    pCfgUsr->xtl.usr.net.diagEvt.pcSubscriber ) ) )
                        {
                           /* Match...  This is a full view.
                           */
                           pResSub->resourceDlgFullSet( BOS_TRUE );
                        }
                     }

                     pSceGenEvt->ReleaseIfRef();
                     pSceGenEvt = NULL;
                  }

                  /* Notify right away.
                  */
                  bDlg = CeMain::associatedDlg( pResSub->resourceParent() );

                  ceNotify ( pResSub->resourceGuid(),
                             eCEEVT_SUB_USR_NOTIFY,
                             static_cast<void *> (&bDlg) );
               }
               else
               {
                  fsmSubReject ( rRes );
               }
            }
            break;

            default:
            {
               fsmSubReject ( rRes );
            }
            break;
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubReject
**
**  PURPOSE:    Rejecting a solicited subscription from a peer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmSub::fsmSubReject( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      if ( pResSub->resourceNotHdl() )
      {
         pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
         pResSub->resourceNotHdl()->RejectSubscription( MXD_GNS::uBAD_EVENT,
                                                        NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubEnded
**
**  PURPOSE:    A solicited subscription from a peer has been terminated.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmSub::fsmSubEnded( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      pResSub->resourcePushState( CeFsmBase::eCEFSMSTATE_SUB_IDLE );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubAdvert
**
**  PURPOSE:    Advertize (notify) on a solicited subscription from a peer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmSub::fsmSubAdvert( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );

      if ( pResSub->resourceNotHdl() )
      {
         switch ( pResSub->resourceType() )
         {
            case eCCTKSUBEVT_DLG:
            {
               if ( pResSub->resourceNotDlg() )
               {
                  BOS_UINT8 uDlgEvt[CE_DLG_EVENT_LEN];

                  pResSub->resourceNotDlg()->ceDlgVerUp();
                  if ( pResSub->resourceNotDlg()->ceDlgWrite(
                                                   CE_DLG_EVENT_LEN,
                                                   uDlgEvt,
                                                   pResSub->resourceParent(),
                                                   pResSub->resourceDlgFullGet() ) )
                  {
                     MXD_GNS::CBlob *pBlob = 
                           BRCM_NEW( MXD_GNS::CBlob )
                                 ( static_cast<const uint8_t *> (uDlgEvt),
                                   CE_DLG_EVENT_LEN,
                                   CE_DLG_EVENT_LEN );
                     MXD_GNS::CSipMessageBody *pBody;
                     MXD_GNS::CSipHeader *pContentType;

                     pContentType =
                        BRCM_NEW( MXD_GNS::CSipHeader)
                              ( MXD_GNS::eHDR_CONTENT_TYPE );
                     pContentType->GetContentTypeMType() = CE_DLG__CONTENT_TYPE;
                     pContentType->GetContentTypeMSubType() =
                                                       CE_DLG__CONTENT_SUB_TYPE;

                     pBody = BRCM_NEW( MXD_GNS::CSipMessageBody );
                     pBody->AddBody( pBlob, pContentType );
 
                     pResSub->resourceNotHdl()->Notify(
                                          MXD_GNS::ISceNotifier::eACTIVE,
                                          pBody );
                  }
               }
            }
            break;

            default:
            break;
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmSub::fsmSubPending
**
**  PURPOSE:    Mark a subscription restart as pending, or cancel a pending 
**              subscription while unsubscribing.
**              Used to handle user actions for dynamic service control.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmSub::fsmSubPending( CeResBase &rRes )
{
   CeResSub *pResSub = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_SUB )
   {
      pResSub = reinterpret_cast<CeResSub *> ( rRes.resourceAddress() );
      
      /* Currently only valid for the MWI event.
      */
      if( pResSub->resourceType() == eCCTKSUBEVT_MWI )
      {
         /* Mark that the another subscription needs to be started
         ** after the current one has ended.
         */
         if ( rRes.resourceEvent() == eCEEVT_SUB_USR_SUBSCRIBE)
         {
            pResSub->resourceSubPending( BOS_TRUE );
         }
         /* Cancel any pending subscription requests.
         */
         else if ( rRes.resourceEvent() == eCEEVT_SUB_USR_ENDED )
         {
            pResSub->resourceSubPending( BOS_FALSE );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}
