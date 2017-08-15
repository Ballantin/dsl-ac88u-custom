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
**      This file implements the call (active) finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmCall.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CEFSMSDPPRS                    MXD_GNS::CSdpParser

#define CE_FSM__ATTR__SENDRECV         "sendrecv"
#define CE_FSM__ATTR__SENDONLY         "sendonly"
#define CE_FSM__ATTR__RECVONLY         "recvonly"
#define CE_FSM__ATTR__INACTIVE         "inactive"

#define CE_FSM__SIGNAL_DATA            "Signal="
#define CE_FSM__DURATION_DATA          "Duration="

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_CALL_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_CALL_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_CALL_SET_FINISH - eCEEVT_CALL_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_CALL_IDLE -------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ANSWER ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_UNHOLD ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FORWARD ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ENDED ------------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_USR_PROXY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_DTMF -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FLASH-------------- */ &CeFsmBase::fsmNoOp,
   },

   /* ---- eCEFSMSTATE_CALL_CONNECT ----------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmCall::fsmMedia,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmCall::fsmMediaFail,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmCall::fsmHeld,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmCall::fsmXfer,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmCall::fsmInfoRcv,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmCall::fsmUpdate,
        /* ---- eCEEVT_CALL_USR_ANSWER ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_HOLD -------------- */ &CeFsmCall::fsmHold,
        /* ---- eCEEVT_CALL_USR_UNHOLD ------------ */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_CALL_USR_FORWARD ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER -------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_CALL_USR_BXFER ------------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_CALL_USR_XFER_NOTERM ------- */ &CeFsmCall::fsmXfering,
        /* ---- eCEEVT_CALL_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_CALL_USR_PROXY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_DTMF -------------- */ &CeFsmCall::fsmDtmfInfoSnd,
        /* ---- eCEEVT_CALL_USR_FLASH-------------- */ &CeFsmCall::fsmFlashInfoSnd,
   },

   /* ---- eCEFSMSTATE_CALL_CLEAN ------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ANSWER ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_UNHOLD ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FORWARD ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ENDED ------------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_CALL_USR_PROXY ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_DTMF -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FLASH-------------- */ &CeFsmBase::fsmNoOp,
   },

};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmRun
**
**  PURPOSE:    Run the (active) call state machine.
**
**  PARAMETERS:
**              rRes - reference to the call resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCall::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_CALL_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_CALL_SET_FINISH) &&
        (eEvent > eCEEVT_CALL_SET_BEGIN) &&
        (eEvent < eCEEVT_CALL_SET_FINISH) )
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
                           - (CeFsmBase::eCEFSMSTATE_CALL_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_CALL_SET_BEGIN + 1);

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
**  FUNCTION:   CeFsmCall::fsmEnded
**
**  PURPOSE:    Call has been terminated at the UASSP layer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCall::fsmEnded( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   CeCdr *pCdr = NULL;
   BOS_UINT32 uResParentGuid;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      if ( pResCall->resourceLocalTerm() &&
           rRes.resourceEvent() == eCEEVT_CALL_USR_ENDED )
      {
         /* If this call has already been terminated by the local user and we
         ** attempt to end it locally once more, this could lead to problems
         ** with the SCE processing of the resource.  We need to wait for the
         ** UASSP to come back to us before we can clean up the rest of the
         ** data for this call.  Note that this assumes UASSP will call us
         ** back eventually, if this is not the case, there will be a resource
         ** leakage, but supposedly the leakage would happen on the UASSP side
         ** as well anyways.
         */
         CCTK_TRACEI1( "fsmEnded (0x%lX) - delay call termination.",
                       rRes.resourceGuid() );
      }
      else
      {
         const MXD_GNS::CSipPacket *pPacket = NULL;
         MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
         mxt_result res;
         const CFGSYS *pSysCfg = CFG_SYS;
         CCTKEVT eCctkEvt = eCCTKEVT_ENDCALL;
         BOS_BOOL bTermConnected = BOS_FALSE;
         
         if ( (rRes.resourceEvent() == eCEEVT_CALL_NET_TERMNET) &&
              (rRes.resourceState() == CeFsmBase::eCEFSMSTATE_CALL_CONNECT) )
         {
            eCctkEvt = eCCTKEVT_DISCONNECT;
            bTermConnected = BOS_TRUE;
         }

         uResParentGuid = pResCall->resourceParent();

         tkmEvent( uResParentGuid,
                   pResCall->resourceGuid(),
                   eCctkEvt,
                   NULL );

         /* Delete the associated MSE session if applicable.
         */
         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            BRCM_DELETE( pMse );
            pResCall->resourceSetMse( NULL );
         }

         /* Clean up the configuration associated with this call.
         */
         cfgRemoveCall( pResCall->resourceGuid() );

         /* Mark the CDR as terminated.
         */
         pCdr = CeMain::ceCdrMgr()->locateCdr( uResParentGuid,
                                               pResCall->resourceGuid() );
         if ( pCdr )
         {
            pCdr->cdrEnd();
         }

         /* Check whether we need to apply some failover processing in case
         ** of abnormal termination.  This is applied only if we originated
         ** the call termination or if the call was terminated by the application
         ** after the call was connected.
         */
         if ( ( rRes.resourceEvent() == eCEEVT_CALL_NET_TERMUSR &&
                !pResCall->resourceLocalError() )||
              ( bTermConnected == BOS_TRUE ) )
         {
            res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
            if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
            {
               pSceBasExtCtl->GetLastReceivedPacket( pPacket );
               if ( pPacket &&
                    pPacket->IsResponse() &&
                    pSysCfg )
               {
                  /* Retrieve the method that the response pertains to */
                  const MXD_GNS::CSipHeader *pCSeq =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_CSEQ, res );
                     
                  if ( MX_RIS_S( res ) && ( pCSeq != NULL ) )
                  {
                     /* Check to see if the last response was for an INVITE */
                     if (!strCmpNoCase(
                                          pCSeq->GetCSeqMethod().GetString().CStr(),
                                          MXD_GNS::g_aszMETHOD_NAME[
                                                         MXD_GNS::eSIP_METHOD_INVITE] ))
                     {

                        if (( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uFORBIDDEN ) ||
                            ( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uTEMPORARILY_NOT_AVAILABLE ) ||
                            ( (!pPacket->IsLocallyGenerated()) && 
                              ( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uREQUEST_TIMEOUT )))
                        {                  
                           if ( UTIL_BMP_IS_ENABLED_ALL(
                                                   pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                                   CCTK_SIGBEHVRX_REGONINV4XX ) &&
                                 !pResCall->resourceIsSigRestrictedToAddr())
                           {
                              /* Notify that a target failure has occured, and registration
                              ** action is required. Note, the specific registration action 
                              ** depends on the configuration - either a registration refresh
                              ** to the same target or a new registration to a new target.
                              */ 
                              ceNotify ( uResParentGuid,
                                         eCEEVT_REG_USR_TARGET_FAIL,
                                         NULL );
                           }
                        }
                        /* Handle locally generated timeout reponse
                        */
                        else if ( pPacket->IsLocallyGenerated() &&
                                      pPacket->GetStatusLine()->GetCode() 
                                                  == MXD_GNS::uREQUEST_TIMEOUT )
                        {
                           if ( UTIL_BMP_IS_ENABLED_ALL(
                                                   pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                                   CCTK_SIGBEHVRX_REGONINVTIMEOUT ) &&
                                 !pResCall->resourceIsSigRestrictedToAddr() )
                           {
                              /* Notify that a target failure has occured, and registration
                              ** action is required. Note, the specific registration action 
                              ** depends on the configuration - either a registration refresh
                              ** to the same target or a new registration to a new target.
                              */ 
                              ceNotify ( uResParentGuid,
                                         eCEEVT_REG_USR_TARGET_FAIL,
                                         NULL );
                           }
                        }
                     }
                     /* Otherwise if this is a 4xx response to a BYE from the network (not
                     ** locally generated) then issue a re-register if configured to do so.
                     */
                     else if (!strCmpNoCase(
                                          pCSeq->GetCSeqMethod().GetString().CStr(),
                                          MXD_GNS::g_aszMETHOD_NAME[
                                                         MXD_GNS::eSIP_METHOD_BYE] ) &&
                              ( !pPacket->IsLocallyGenerated() )&&
                              ( MXD_GNS::MxGetSipStatusClass ( pPacket->GetStatusLine()->GetCode() )
                                                     == MXD_GNS::eSIP_STATUS_CLASS_CLIENT_ERROR ) &&
                                 UTIL_BMP_IS_ENABLED_ALL(
                                       pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                       CCTK_SIGBEHVRX_REREGONBYE4XX ) &&
                                 !pResCall->resourceIsSigRestrictedToAddr() )
                     {
                        /* Issue a re-register */
                        ceNotify ( uResParentGuid,
                                   eCEEVT_REG_USR_REG,
                                   NULL );
                     }
                  }
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
         }

#if CCTKCFG_REMOTE_USER_CONTROL
         /* If there is a proxy call associated with this call, clean up that
         ** leg as well and dissociate the two calls.
         */
         if ( pResCall->resourceProxyGuid() != guCeInvalidGuid )
         {
            BOS_UINT32 uResProxyGuid = pResCall->resourceProxyGuid();
            BOS_UINT32 uResOtherProxyGuid = guCeInvalidGuid;
            CeResBase *pProxyCall;

            pResCall->resourceProxyGuid( guCeInvalidGuid ); 
            
            if ( CeMain::locateRes( uResProxyGuid,
                                    pProxyCall ) )
            {
               if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall *pResProxyCall =
                     reinterpret_cast<CeResCall *> ( pProxyCall->resourceAddress() );

                  uResOtherProxyGuid = pResProxyCall->resourceProxyGuid();

                  /* if this is the connected, the proxy guid of the other leg will 
                  ** equal to the guid of this leg, then, end the other leg */
                  if ( uResOtherProxyGuid == pResCall->resourceGuid() )
                  {
                     pResProxyCall->resourceProxyGuid( guCeInvalidGuid );
                     /* End the proxy'ed call to the network now if needed.
                     */
                     if ( uResProxyGuid != guCeInvalidGuid )
                     {
                        ceNotify( uResProxyGuid,
                                  eCEEVT_CALL_USR_ENDED,
                                  NULL );
                     }
                  }
               }
               else
               {
                  fsmError( rRes );
               }
            }
         }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

         /* Clean up the resource.
         */
         pResCall->resourceEnd();

         /* Issue a management event targeted to the parent resource.
         */
         ceNotify ( uResParentGuid,
                    eCEEVT_CALL_REM,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmEnd
**
**  PURPOSE:    Terminate a call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCall::fsmEnd( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeCdr *pCdr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
      if ( pResCall->resourceFsm() != CeFsmBase::eCEFSMTYPE_CALL )
      {
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
      }

      /* If we are in reset mode, no action is required to terminate any
      ** outstanding calls as this has already been done in CeMain::shutDown.
      ** We may however be entering this function as a result of the connection
      ** termination triggered frin CeMain::shutDown.  For this case, we only
      ** need to update the resource states.  No further interactions with the
      ** stack is required.
      */
      if ( !CeMain::ceReset() )
      {
         /* Mark the CDR as terminated.
         */
         pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                               pResCall->resourceGuid() );
         if ( pCdr )
         {
            pCdr->cdrEnd();
         }

         /* Terminate the call.  This is a hard termination, but this
         ** is what we need here.
         */
         pResCall->resourceNetHdl()->TerminateCall();
         pResCall->resourceForkTerm();

         /* Mark that local termination has been asked for.
         */
         pResCall->resourceSetLocalTerm();
      }

#if CCTKCFG_REMOTE_USER_CONTROL
      /* If this is a connected call, then end other other leg, else do nothing */
      if ( pResCall->resourceProxyGuid() != guCeInvalidGuid )
      {
         BOS_UINT32 uResProxyGuid = pResCall->resourceProxyGuid();
         BOS_UINT32 uResOtherProxyGuid = guCeInvalidGuid;
         CeResBase *pProxyCall;

         pResCall->resourceProxyGuid( guCeInvalidGuid ); 
         
         if ( CeMain::locateRes( uResProxyGuid,
                                 pProxyCall ) )
         {
            if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResProxyCall =
                  reinterpret_cast<CeResCall *> ( pProxyCall->resourceAddress() );

               uResOtherProxyGuid = pResProxyCall->resourceProxyGuid();
               /* if this is the connected, the proxy guid of the other leg will 
               ** equal to the guid of this leg, then, end the other leg */
               if ( uResOtherProxyGuid == pResCall->resourceGuid() )
               {
                  pResProxyCall->resourceProxyGuid( guCeInvalidGuid );
                  /* End the proxy'ed call to the network now if needed.
                  */
                  if ( uResProxyGuid != guCeInvalidGuid )
                  {
                     ceNotify( uResProxyGuid,
                               eCEEVT_CALL_USR_ENDED,
                               NULL );
                  }
               }
            }
            else
            {
               fsmError( rRes );
            }            
         }
      }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmMedia
**
**  PURPOSE:    Updated media information for the current call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmMedia( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      CeCdr *pCdr = NULL;
      CCTKCALLINFO call;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::CBlob hdrValue;

      memset ( reinterpret_cast<void *> ( &call ),
               0,
               sizeof ( CCTKCALLINFO ) );

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );

      mxt_result res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         /* If session timer are in use for this call, no need to apply media
         ** keep alive.
         */
         call.bMedKeepAlive = 
            !pSceBasExtCtl->IsOptionSupported( MXD_GNS::eOPTIONTAG_TIMER );

         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* If this is an emergency call and we have received new media information
            ** on it, check also if there is a Alert-Info header associated with the
            ** request.  It is possible that there be such information which may contain
            ** action regarding tone to the played to the local user (such as a howler
            ** tone for example).
            */
            if ( pCdr && pCdr->cdrIsEmergency() )
            {
               const MXD_GNS::CSipHeader *pAlertInfo =
                  pPacket->GetHeaderList().Get( MXD_GNS::eHDR_ALERT_INFO, res );
               if ( MX_RIS_F( res ) )
               {
                  pAlertInfo = NULL;
                  /* If we failed to extract the parsed Alert-Info header, it may
                  ** be that the specified syntax is of a non-standard one.  Allow
                  ** this non-standard header if configured to do so */
                  if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                                 CCTK_INTEROP_RELAX_ALRTNFO ) )
                  {
                     pAlertInfo = pPacket->GetHeaderList().Get( MXD_GNS::eHDR_ALERT_INFO );
                  }
               }

               if ( pAlertInfo )
               {
                  pAlertInfo->Serialize( hdrValue );

                  call.bEmergency = BOS_TRUE;
                  memcpy( static_cast<void *> ( call.cAlertInfo[ 0 ] ),
                          hdrValue.GetFirstIndexPtr(),
                          (( hdrValue.GetSize() > CCTK_STR_LEN ) ?
                                          CCTK_STR_LEN : hdrValue.GetSize()) );
               }
            }

            if ( !pResCall->resourceIsConnected() )
            {
               /* If this packet contains P-Early-Media information per RFC 5009
               ** and such information should be applied to the current behavior
               ** then apply it now.
               */
               BOS_BOOL bPEarlMed = BOS_FALSE;

               if ( pPacket->IsResponse() )
               {
                  const MXD_GNS::CSipHeader *pCSeq =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_CSEQ, res );
                  
                  if ( MX_RIS_S( res ) && ( pCSeq != NULL ) )
                  {
                     if ( ( ( pPacket->GetStatusLine()->GetClass() ==
                                   MXD_GNS::eSIP_STATUS_CLASS_INFORMATIONAL ) &&
                            !strCmpNoCase(
                                 pCSeq->GetCSeqMethod().GetString().CStr(),
                                 MXD_GNS::g_aszMETHOD_NAME[
                                                MXD_GNS::eSIP_METHOD_INVITE] ))
                           || ( ( pPacket->GetStatusLine()->GetClass() ==
                                             MXD_GNS::eSIP_STATUS_CLASS_SUCCESS ) &&
                           ( !strCmpNoCase(
                                 pCSeq->GetCSeqMethod().GetString().CStr(),
                                 MXD_GNS::g_aszMETHOD_NAME[
                                             MXD_GNS::eSIP_METHOD_UPDATE] )
                           || !strCmpNoCase(
                                   pCSeq->GetCSeqMethod().GetString().CStr(),
                                   MXD_GNS::g_aszMETHOD_NAME[
                                             MXD_GNS::eSIP_METHOD_PRACK] ))) )
                     {
                        bPEarlMed = BOS_TRUE;
                     }
                  }
               }
               else if ( pPacket->IsRequest() &&
                         ( ( pPacket->GetRequestLine()->GetMethod() ==
                                                   MXD_GNS::eSIP_METHOD_INVITE ) ||
                           ( pPacket->GetRequestLine()->GetMethod() ==
                                                   MXD_GNS::eSIP_METHOD_UPDATE ) ||
                           ( pPacket->GetRequestLine()->GetMethod() ==
                                                   MXD_GNS::eSIP_METHOD_PRACK ) ) )
               {
                  bPEarlMed = BOS_TRUE;
               }

               if ( bPEarlMed )
               {
                  MXD_GNS::MseSdpSession *pMse = NULL;
                  const MXD_GNS::CString earlyMedia( PM__SIGNALING_P_EARLY_MEDIA );
                  const MXD_GNS::CSipHeader *pPEarlMed =
                     pPacket->GetHeaderList().Get( earlyMedia,
                                                   res );
                  pResCall->resourceGetMse( pMse );

                  if ( MX_RIS_S( res ) && ( pPEarlMed != NULL ) &&
                       ( pMse != NULL ) )
                  {
                     /* Get the P-Early-Media content and pass it to the
                     ** MSE for further processing.
                     */
                     while ( pPEarlMed != NULL )
                     {
                        if ( pPEarlMed->GetSupported().GetString().CaseInsCmp(
                                        CEFSMSDPPRS::ms_apszAttributeTypeMap[
                                                    CEFSMSDPPRS::eSENDRECV] ) == 0 )
                        {
                           pMse->setEarlyMediaFlow(
                                    MXD_GNS::MseSdpSession::eMSEEMFLOW_SENDRECV );
                        }
                        else if ( pPEarlMed->GetSupported().GetString().CaseInsCmp(
                                        CEFSMSDPPRS::ms_apszAttributeTypeMap[
                                                    CEFSMSDPPRS::eSENDONLY] ) == 0 )
                        {
                           pMse->setEarlyMediaFlow( 
                                    MXD_GNS::MseSdpSession::eMSEEMFLOW_SENDONLY );
                        }
                        else if ( pPEarlMed->GetSupported().GetString().CaseInsCmp(
                                        CEFSMSDPPRS::ms_apszAttributeTypeMap[
                                                    CEFSMSDPPRS::eRECVONLY] ) == 0 )
                        {
                           pMse->setEarlyMediaFlow(
                                    MXD_GNS::MseSdpSession::eMSEEMFLOW_RECVONLY );
                        }
                        else if ( pPEarlMed->GetSupported().GetString().CaseInsCmp(
                                        CEFSMSDPPRS::ms_apszAttributeTypeMap[
                                                    CEFSMSDPPRS::eINACTIVE] ) == 0 )
                        {
                           pMse->setEarlyMediaFlow( 
                                    MXD_GNS::MseSdpSession::eMSEEMFLOW_INACTIVE );
                        }

                        pPEarlMed = pPEarlMed->GetNextHeader();
                     }
                  }
               }
            }

            /* Done with the packet.
            */
            pPacket->Release();
         }

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      /* Save relevant information from the media session into the CDR.  Note
      ** that we are only interested in keeping information of value for the audio
      ** portion of the call at this time.
      */
      CCTKMEDINFO *pMedInfo = ceGetMediaInfo( pResCall->resourceGuid() );
      if ( pMedInfo )
      {
         MXD_GNS::MseSdpSession *pMse = NULL;
         pResCall->resourceGetMse( pMse );
         const MX_NS MSECODECMAP *pCodecMap;
         BOS_UINT8 uPrsrCode;

         if ( !bosIpAddrIsZero( &pMedInfo->egress.addr ) &&
              pMedInfo->egress.audio.uNumCodec &&
              pMedInfo->egress.audio.codec[ 0 ].uRtpPort )
         {
            if ( pCdr )
            {
               if (pMse)
               {
                  pCodecMap = pMse->getCodecMap(pMedInfo->egress.audio.codec[ 0 ].eType);
                  uPrsrCode = pCodecMap->eMxCodec;
               }
               pCdr->cdrLocal( &pMedInfo->egress.addr,
                               &pMedInfo->egress.audio.codec[ 0 ],
                               uPrsrCode );
            }
         }

         if ( !bosIpAddrIsZero( &pMedInfo->ingress.addr ) &&
              pMedInfo->ingress.audio.uNumCodec &&
              pMedInfo->ingress.audio.codec[ 0 ].uRtpPort )
         {
            if ( pCdr )
            {
               if (pMse)
               {
                  pCodecMap = pMse->getCodecMap(pMedInfo->ingress.audio.codec[ 0 ].eType);
                  uPrsrCode = pCodecMap->eMxCodec;
               }
               pCdr->cdrRemote( &pMedInfo->ingress.addr,
                                &pMedInfo->ingress.audio.codec[ 0 ],
                                uPrsrCode );
            }
         }
      }

      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_MEDIA,
                reinterpret_cast<void *> ( &call ) );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmMediaFail
**
**  PURPOSE:    Attempt to update media information for the current call
**              has failed.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmMediaFail( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_MEDIA_FAIL,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmHold
**
**  PURPOSE:    Handle a hold action coming from the local user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmHold( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Put ourselves in the correct state machine handling and forward
      ** the event for further processing.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_IDLE );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_HOLD );

      ceNotify( pResCall->resourceGuid(), eCEEVT_CALL_USR_HOLD, NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmHeld
**
**  PURPOSE:    Handle a hold action coming from the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmHeld( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Put ourselves in the correct state machine and state handling, we
      ** are already held at this time.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_IDLE );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_HOLD );

      ceNotify( pResCall->resourceGuid(), eCEEVT_CALL_NET_HOLD, NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmUpdate
**
**  PURPOSE:    New media (update) information has been set by the local user
**              to be used for the current call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This is used currently for cases like:
**
**                   - change the codec mid call.
**                   - change from audio to image (fax) mode and vice-versa.
**
***********************************************************************************/
void CeFsmCall::fsmUpdate( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceGetMse( pMse );
      if ( pMse )
      {
         pMse->mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmXfering
**
**  PURPOSE:    Handle a transfer action coming from the local user.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmXfering( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CEEVT event;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      if (pResCall->resourceNoXferTerm())
      {
         event = eCEEVT_CALL_USR_XFER_NOTERM;
      }
      /* Assume a blind trasnfer if the transfer-to GUID is invalid */
      else if (pResCall->resourceXferGuid() == guCeInvalidGuid)
      {
         event = eCEEVT_CALL_USR_BXFER;
      }
      else
      {
         event = eCEEVT_CALL_USR_XFER;
      }

      /* Put ourselves in the correct state machine handling and forward
      ** the event for further processing.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_IDLE );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_XFER );

      ceNotify( pResCall->resourceGuid(),
                event,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmXfer
**
**  PURPOSE:    Handle a transfer action coming from the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCall::fsmXfer( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Put ourselves in the correct state machine handling and forward
      ** the event for further processing.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_XFER_IDLE );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_XFER );

      ceNotify( pResCall->resourceGuid(),
                eCEEVT_CALL_NET_XFER,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmDtmfInfoSnd
**
**  PURPOSE:    Issue a notification about a DTMF information received for this
**              call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCall::fsmDtmfInfoSnd( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
      mxt_result res;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Issue the request transparently within the dialog via the basic
      ** extension manager component.  Note that we do not care about the
      ** response to this request since there is no post processing associated
      ** with it anyways.  It is a 'send and forget'.
      */
      res = (pResCall->resourceNetHdl())->QueryIf( &pMyBasicExtCtl );

      if ( MX_RIS_S( res ) && ( pMyBasicExtCtl != NULL ) )
      {
         /* Create and format the data.
         */
         MXD_GNS::CSipMessageBody *pBody =
                                       BRCM_NEW( MXD_GNS::CSipMessageBody );

         if ( pBody )
         {
            MXD_GNS::CBlob *pBlob;
            MXD_GNS::CSipHeader *pCtTypeHdr;
            BOS_UINT8 uData[ CCTK_STR_LEN ];
            BOS_UINT32 uLen;

            /* Create the body of the request.
            */
            memset( reinterpret_cast<void *> (uData),
                    0,
                    CCTK_STR_LEN );
            snprintf( reinterpret_cast<char *> (&uData[0]),
               CCTK_STR_LEN,
               "Signal=%c\nDuration=%d",
               (char)pResCall->resourceInBandDtmf() ,
               (int)pResCall->resourceIBDtmfDur() );
            uLen = (BOS_UINT32) strlen( reinterpret_cast<const char *> (uData) );

            pBlob =
               BRCM_NEW( MXD_GNS::CBlob )(
                  reinterpret_cast<const uint8_t *> (uData),
                  static_cast<unsigned int> (uLen),
                  static_cast<unsigned int> (uLen) );

            pCtTypeHdr =
               BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
            pCtTypeHdr->GetContentTypeMType() = 
                                 CE_RES_NOT__AUDIO_TYPE;
            pCtTypeHdr->GetContentTypeMSubType() =
                                 CE_RES_NOT__TEL_EVT_SUBTYPE;

            res = pBody->AddBody( pBlob, pCtTypeHdr );
            if ( MX_RIS_S( res ) )
            {
               pBlob = NULL;
               pCtTypeHdr = NULL;
            }
            else
            {
               BRCM_DELETE( pBody );
               pBody = NULL;

               BRCM_DELETE( pCtTypeHdr );
               pCtTypeHdr = NULL;
            }
         }
               
         /* Send it.
         */
         if ( pBody )
         {
            pMyBasicExtCtl->SendRequest( MXD_GNS::eMETHODTYPE_INFO,
                                         0,
                                         NULL,
                                         pBody,
                                         NULL );

            /* Note that the above call has taken ownership of the created
            ** resources.
            */
            pBody = NULL;
         }

         pMyBasicExtCtl->ReleaseIfRef();
         pMyBasicExtCtl = NULL;
      }
   }
   else
   {
      fsmError( rRes );
   }
}

/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmFlashInfoSnd
**
**  PURPOSE:    Issue a notification about a flash information received for this
**              call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCall::fsmFlashInfoSnd( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      MXD_GNS::ISceBasicExtensionControl *pMyBasicExtCtl = NULL;
      mxt_result res;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Issue the request transparently within the dialog via the basic
      ** extension manager component.  Note that we do not care about the
      ** response to this request since there is no post processing associated
      ** with it anyways.  It is a 'send and forget'.
      */
      res = (pResCall->resourceNetHdl())->QueryIf( &pMyBasicExtCtl );

      if ( MX_RIS_S( res ) && ( pMyBasicExtCtl != NULL ) )
      {
         /* Create and format the data.
         */
         MXD_GNS::CSipMessageBody *pBody =
                                       BRCM_NEW( MXD_GNS::CSipMessageBody );

         if ( pBody )
         {
            MXD_GNS::CBlob *pBlob;
            MXD_GNS::CSipHeader *pCtTypeHdr;
            BOS_UINT8 uData[ CCTK_STR_LEN ];
            BOS_UINT32 uLen;
            BOS_UINT8 uCtTypeHdr[ CCTK_STR_LEN ];
            char* pCtTypeTok, *pBaseType, *pSubType;
            
            /* Create the Content-Type header of the request */
            memset( reinterpret_cast<void *> (uCtTypeHdr),
                    0,
                    CCTK_STR_LEN );
            snprintf( reinterpret_cast<char *> (&uCtTypeHdr[0]),
               CCTK_STR_LEN,
               "%s", (char*)pResCall->resourceHfMimeType() );
            uLen = (BOS_UINT32) strlen( reinterpret_cast<const char *> (uCtTypeHdr) );

            if ((uCtTypeHdr[0] == 0) || (uLen == 0))
            {
               pBaseType = CE_RES_NOT__AUDIO_TYPE;
               pSubType  = CE_RES_NOT__TEL_EVT_SUBTYPE;
            }
            else
            {
               /* The type shall contain type and subtype with "/" between them */
               pCtTypeTok = reinterpret_cast<char *>(uCtTypeHdr);
               pBaseType = strtok(pCtTypeTok, "/");
               if (pBaseType == NULL)
               {
                  pBaseType = CE_RES_NOT__AUDIO_TYPE;
               }
               pSubType = strtok(NULL, "");
               if (pSubType == NULL)
               {
                  pSubType = CE_RES_NOT__TEL_EVT_SUBTYPE;
               }
            }

            pCtTypeHdr =
               BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
            pCtTypeHdr->GetContentTypeMType() = pBaseType;
            pCtTypeHdr->GetContentTypeMSubType() = pSubType;

            /* Create the body of the request.
            */
            memset( reinterpret_cast<void *> (uData),
                    0,
                    CCTK_STR_LEN );
            snprintf( reinterpret_cast<char *> (&uData[0]),
               CCTK_STR_LEN,
               "%s", (char*)pResCall->resourceHfContent() );
            uLen = (BOS_UINT32) strlen( reinterpret_cast<const char *> (uData) );

            pBlob =
               BRCM_NEW( MXD_GNS::CBlob )(
                  reinterpret_cast<const uint8_t *> (uData),
                  static_cast<unsigned int> (uLen),
                  static_cast<unsigned int> (uLen) );

            res = pBody->AddBody( pBlob, pCtTypeHdr );
            if ( MX_RIS_S( res ) )
            {
               pBlob = NULL;
               pCtTypeHdr = NULL;
            }
            else
            {
               BRCM_DELETE( pBody );
               pBody = NULL;

               BRCM_DELETE( pCtTypeHdr );
               pCtTypeHdr = NULL;
            }
         }
               
         /* Send it.
         */
         if ( pBody )
         {
            pMyBasicExtCtl->SendRequest( MXD_GNS::eMETHODTYPE_INFO,
                                         0,
                                         NULL,
                                         pBody,
                                         NULL );

            /* Note that the above call has taken ownership of the created
            ** resources.
            */
            pBody = NULL;
         }

         pMyBasicExtCtl->ReleaseIfRef();
         pMyBasicExtCtl = NULL;

      }
   }
   else
   {
      fsmError( rRes );
   }

}

/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmInfoRcv
**
**  PURPOSE:    Handles the payload of an information message received.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      At the present time, the only payload recognized and hence
**              processed here is DTMF information.
***********************************************************************************/
void CeFsmCall::fsmInfoRcv( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      CeMain::CEMAINDATA *pInfoData =
                  (pResCall->resourceAddtionalCallData() ?
                                static_cast<CeMain::CEMAINDATA *> (
                                    pResCall->resourceAddtionalCallData()) : NULL);
      if ( pInfoData )
      {
         if ( pInfoData->puData != NULL )
         {
            CCTKOOBDTMF oobDtmf;

            /* At this time we can parse the content of the paylaod to match some
            ** information we want to extract.  For the time being, the only use for
            ** this event is to extract out-of-band DTMF setting.
            */
            BOS_UINT8 *pDtmf =  (BOS_UINT8 *) /* C-casting */ (
                                strstr( reinterpret_cast<const char *>(pInfoData->puData),
                                        reinterpret_cast<const char *>(CE_FSM__SIGNAL_DATA) ));

            BOS_UINT8 *pDuration =  (BOS_UINT8 *) /* C-casting */ (
                                    strstr ( reinterpret_cast<const char *>(pInfoData->puData),
                                             reinterpret_cast<const char *>(CE_FSM__DURATION_DATA) ));

            if ( pDtmf )
            {
               oobDtmf.uDtmf = static_cast<BOS_UINT32>( *( reinterpret_cast<const char *>(pDtmf + sizeof(CE_FSM__SIGNAL_DATA) - 1) ) );   
            }

            if ( pDuration && pDtmf )
            {
               oobDtmf.uDuration = static_cast<BOS_UINT32>(
                                      atoi( reinterpret_cast<const char *>(pDuration + sizeof(CE_FSM__DURATION_DATA) - 1)) );   
            }
            else
            {
               if( pDtmf )
               {
                  oobDtmf.uDuration = 100;    /* Default 100 msec tone duration. */
               }
            }


            /* Done with this information.
            */
            BRCM_DELETE_ARRAY ( pInfoData->puData );
            pInfoData->puData = NULL;
         
            /* Issue an event is warranted.
            */
            if ( pDtmf )
            {
               tkmEvent( pResCall->resourceParent(),
                         pResCall->resourceGuid(),
                         eCCTKEVT_OOB_DTMF,
                         reinterpret_cast<void *> ( &oobDtmf ) );
            }
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}