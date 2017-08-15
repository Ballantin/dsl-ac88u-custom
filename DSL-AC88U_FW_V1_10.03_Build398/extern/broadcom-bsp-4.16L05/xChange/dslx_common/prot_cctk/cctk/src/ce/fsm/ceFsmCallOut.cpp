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
**      This file implements the outgoing call finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmCallOut.h>
#include <ceFsmCall.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
#define CE_FSM__XML_VALUE_RESTORATION     "restoration"
#define CE_FSM__XML_VALUE_INIT_REG        "initial-registration"

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_CALLOUT_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_CALLOUT_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_CALL_SET_FINISH - eCEEVT_CALL_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_CALLOUT_IDLE -------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
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
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmCallOut::fsmCreate,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmCallOut::fsmMake,
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

   /* ---- eCEFSMSTATE_CALLOUT_ATTEMPT ----------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmCallOut::fsmFailed,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmCallOut::fsmAnswer,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmCallOut::fsmAlerted,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmCallOut::fsmAlerted,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmCall::fsmMedia,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmCallOut::fsmPendSdpOfr,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* SDP comes from 180 RINGING */
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmCallOut::fsmSdpAns, 
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmInvalid,
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

   /* ---- eCEFSMSTATE_CALLOUT_ALERT ------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmCallOut::fsmFailed,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmCallOut::fsmAnswer,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmCallOut::fsmAlerted,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmCallOut::fsmAlerted,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmCall::fsmMedia,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmCall::fsmEnd,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* SDP comes from 200 OK */
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmCallOut::fsmSdpAns, 
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmInvalid,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmCall::fsmUpdate,
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
**  FUNCTION:   CeFsmCallOut::fsmRun
**
**  PURPOSE:    Run the outgoing call state machine.
**
**  PARAMETERS:
**              rRes - reference to the call resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_CALLOUT_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_CALLOUT_SET_FINISH) &&
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
                           - (CeFsmBase::eCEFSMSTATE_CALLOUT_SET_BEGIN + 1);
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
**  FUNCTION:   CeFsmCallOut::fsmFailed
**
**  PURPOSE:    Call attempt has failed at the UASSP layer.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmFailed( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Set up the resource for cleaning completion.  Do not do anything else
      ** at this time, the call has failed.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      /* Set the CDR termination type to a failure for the time being. */
      CeCdr *pCdr = NULL;
      CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                     pResCall->resourceGuid() );
      if ( pCdr )
      {
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_FAILED);
      }

      /* If the call failed due to a 'busy' condition, report it to upper
      ** layer application since some feature may trigger on such response to
      ** an original call attempt.
      **
      ** If the call failed due to a 'request terminated' condition, treat it
      ** just as if the call had been connected first since in such scenario
      ** it is typically a call ended after having being redirected to some
      ** announcement or media server but before connection occurs such that
      ** the user is not charged for this call.
      **
      ** If the call failed due to a 'forbidden' or 'request timeout' and we
      ** are configured to do so, treat this as a keep alive failure because
      ** it will generate a new registration.
      */
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      mxt_result res;
      MXD_GNS::CBlob hdrValue;
      CCTKCALLINFO call;
      const CFGSYS *pSysCfg = CFG_SYS;
      const CFGUSR *pCfgUsr = CFG_USR( pResCall->resourceParent() );
      CeResUser *pResUser = NULL;
      CeResBase *pResBase = NULL;

      memset ( reinterpret_cast<void *> ( &call ),
               0,
               sizeof ( CCTKCALLINFO ) );

      /* Locate the parent resource
      */
      if ( CeMain::locateRes( pResCall->resourceParent(),
                              pResBase ) )
      {
         if ( pResBase->resourceGender() == CeResBase::eCERESTYPE_USR )
         {
            pResUser = reinterpret_cast<CeResUser *>
                                    ( pResBase->resourceAddress() );
         }
      }


               
      /* End the cdr now, otherwise in certain failover scenario's
      ** the cdr will be freed before call clean up.
      */
      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                                   pResCall->resourceGuid() );
      if ( pCdr)
      {
         pCdr->cdrEnd();
      }

      res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* Check whether we have an error information header in this packet
            ** if so, its use takes precedence over the response code information.
            */
            const MXD_GNS::CSipHeader *pErrorInfo =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_ERROR_INFO, res );
            if ( MX_RIS_S( res ) && ( pErrorInfo != NULL ) )
            {
               pErrorInfo->Serialize( hdrValue );

               memcpy( static_cast<void *> ( call.cErrorInfo ),
                       hdrValue.GetFirstIndexPtr(),
                       (( hdrValue.GetSize() > CCTK_STR_LEN ) ?
                                       CCTK_STR_LEN : hdrValue.GetSize()) );

               tkmEvent( pResCall->resourceParent(),
                         pResCall->resourceGuid(),
                         eCCTKEVT_ERROR_INFO,
                         &call );
            }
            else
            {
               if ( pPacket->IsResponse() &&
                    (( pPacket->GetStatusLine()->GetCode() 
                                          == MXD_GNS::uBUSY_HERE ) ||
                     ( pPacket->GetStatusLine()->GetCode() 
                                          == MXD_GNS::uBUSY_EVERYWHERE )) )
               {
                  tkmEvent( pResCall->resourceParent(),
                            pResCall->resourceGuid(),
                            eCCTKEVT_BUSY,
                            NULL );
               }
               else if ( pPacket->IsResponse() &&
                         ( pPacket->GetStatusLine()->GetCode() 
                                          == MXD_GNS::uREQUEST_TERMINATED ) )
               {
                  pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
               }
               else if ( pPacket->IsResponse() &&
                           (( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uFORBIDDEN ) ||
                            ( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uTEMPORARILY_NOT_AVAILABLE ) ||
                            ( (!pPacket->IsLocallyGenerated()) && 
                              ( pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uREQUEST_TIMEOUT ))))
               {                  
                  if ( ( pSysCfg &&
                       UTIL_BMP_IS_ENABLED_ALL(
                                          pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                          CCTK_SIGBEHVRX_REGONINV4XX ) ) &&
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
               /* Handle locally generated timeout reponse
               */
               else if ( pPacket->IsResponse() &&
                          pPacket->IsLocallyGenerated() &&
                             pPacket->GetStatusLine()->GetCode() 
                                         == MXD_GNS::uREQUEST_TIMEOUT )
               {
                  if ( ( pSysCfg &&
                       UTIL_BMP_IS_ENABLED_ALL(
                                          pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                          CCTK_SIGBEHVRX_REGONINVTIMEOUT ) ) &&
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

               /* Handle 5XX response ( 500, 503, 504 )
               */
               else if ( ( pPacket->GetStatusLine()->GetCode()
                                       == MXD_GNS::uINTERNAL_SERVER_ERROR ) ||
                         ( pPacket->GetStatusLine()->GetCode()
                                       == MXD_GNS::uSERVICE_UNAVAILABLE ) ||
                         ( pPacket->GetStatusLine()->GetCode()
                                       == MXD_GNS::uSERVER_TIME_OUT ) )
               {
                     BOS_BOOL bTargetFailure = BOS_FALSE;

                     if ( pCfgUsr &&
                           ( pPacket->GetStatusLine()->GetCode()
                                          == MXD_GNS::uSERVER_TIME_OUT ) )
                     {
                        char *pType = NULL;
                        char *pReason = NULL;
                        char *pAction = NULL;
                        MXD_GNS::IXmlDocument *pDocument = NULL;
                        MXD_GNS::CSubAllocator *pAllocator = NULL;

                        /* Check to see if restoration procedures are
                        ** specified.
                        */
                        /*  First confirm the P-Asserted-Identity
                        */
                        const MXD_GNS::CSipHeader* pPAsserted =
                           pPacket->GetHeaderList().Get(
                              MXD_GNS::eHDR_P_ASSERTED_IDENTITY, res );
                        if ( MX_RIS_S( res ) && ( pPAsserted != NULL ) )
                        {
                           MXD_GNS::ISceUserConfig *pSceCfg =
                                                   pCfgUsr->in.pSceUsrCfg;
                           const MXD_GNS::CNameAddr *pSvcRoute = NULL;

                           if ( pSceCfg && pSceCfg->GetServiceRoute() )
                           {
                              pSvcRoute =
                                 &pSceCfg->GetServiceRoute()->GetServiceRoute();
                           }
                           if ( pResUser  &&
                                 ( ( pPAsserted->GetPAssertedIdentity() ==
                                          *pResUser->resourceGetRegPath() ) ||
                                   ( pSvcRoute && (
                                       pPAsserted->GetPAssertedIdentity() ==
                                          *pSvcRoute ) ) ) )
                           {
                              bTargetFailure = BOS_TRUE;
                           }
                        }

                        /* Verify the content-type and xml body
                        */
                        if ( bTargetFailure )
                        {
                           bTargetFailure = BOS_FALSE;

                           if ( CeMain::parse3gppImsXmlContent(
                                                   pPacket, pDocument, pAllocator,
                                                   pType, pReason, pAction ) &&
                                 ( ( pType != NULL &&
                                    MxStringCaseCompareLength( pType,
                                       CE_FSM__XML_VALUE_RESTORATION,
                                          sizeof( CE_FSM__XML_VALUE_RESTORATION ) )
                                             == 0 ) &&
                                    ( pAction != NULL &&
                                       MxStringCaseCompareLength( pAction,
                                          CE_FSM__XML_VALUE_INIT_REG,
                                          sizeof( CE_FSM__XML_VALUE_INIT_REG ) )
                                             == 0 ) ) )
                           {
                              bTargetFailure = BOS_TRUE;
                           }
                           /* Cleanup allocation from parser utility
                           */
                           if ( pDocument )
                           {
                              pDocument->ReleaseIfRef();
                              pDocument = NULL;
                           }

                           if ( pAllocator )
                           {
                              BRCM_DELETE( pAllocator );
                              pAllocator = NULL;
                           }
                        }
                     }

                     if ( bTargetFailure ||
                           ( pSysCfg &&
                             UTIL_BMP_IS_ENABLED_ALL(
                                             pSysCfg->xtl.net.prot.gen.sigBehave.uRx,
                                             CCTK_SIGBEHVRX_REGONINV5XX ) ) )
                  {
                     /* Notify that a target failure has occured, and
                     ** registration action is required. Note, the specific
                     ** registration action depends on the configuration -
                     ** either a registration refresh to the same target or a
                     ** new registration to a new target.
                     */
                     ceNotify ( pResCall->resourceParent(),
                                eCEEVT_REG_USR_TARGET_FAIL,
                                NULL );
                  }
               }
            }

            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }         

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
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
**  FUNCTION:   CeFsmCallOut::fsmAnswer
**
**  PURPOSE:    Outgoing called was answered.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmAnswer( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      CCTKCALLINFO *pCall = NULL;
      const MXD_GNS::CSipPacket *pPacket = NULL;
      const MXD_GNS::CNameAddr *pNaiSip = NULL;
      const MXD_GNS::CNameAddr *pNaiTel = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
      MXD_GNS::ISceGenericEventInfo *pSceGenEvt = NULL;
      MXD_GNS::IEComUnknown *pEcomInfo = NULL;

      MXD_GNS::CBlob hdrValue;
      mxt_result res;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      /* Get the call informaiton to update the callee identifier if
      ** applicable. 
      */
      pCall = pResCall->resourceCallingParty();

      if ( pCall->bLoopBack != BOS_FALSE )
      {
         memset ( reinterpret_cast<void *> ( pCall ),
                  0,
                  sizeof ( CCTKCALLINFO ) );
      }

      CeCdr *pCdr =
      CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                     pResCall->resourceGuid() );

      if ( pCdr )
      {
         pCdr->cdrCallRespRcvTime( BOS_TRUE );
      }

      /* Get the Network Asserted Identity for the Callee Id.  The process is
      ** the same as the Caller Id.
      */
      pEcomInfo = ( pResCall->resourceAddtionalCallData() ?
                    static_cast<MXD_GNS::IEComUnknown *> (
                    pResCall->resourceAddtionalCallData()) : NULL );
      if ( pEcomInfo )
      {
         pEcomInfo->QueryIf( &pSceGenEvt );
         pEcomInfo = NULL;
      }

      if ( pSceGenEvt != NULL )
      {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
         /*If extraction of CLID info from PAI enabled*/
         if ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoClidInfoPai )
#endif
         {
            pSceGenEvt->GetNetworkAssertedIdentity( pNaiSip, pNaiTel );
         }

         if ( ( pNaiSip == NULL ) && ( pNaiTel == NULL ) )
         {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
            /*If extraction of CLID info from PAI enabled
            **Otherwise keep the CLID info extracted from To header
            */
            if ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoClidInfoPai )
            {
               /* No NAI information, set 'P' in the number field, nothing in the
               ** name field.
               */
               snprintf( reinterpret_cast<char *> ( pCall->clid.cName ), 
                         (CCTK_CLID_LEN + 1), "P" );
               if ( pCdr )
               {
                  pCdr->cdrAnonymous( BOS_TRUE );
               }
            }
#endif
         }
         else
         {
            /* If a SIP/SIPS NAI is present, use it do derive the name field.
            */
            if ( pNaiSip )
            {
               BOS_UINT32 strLen;
               MXD_GNS::CString name = pNaiSip->GetDisplayName().GetString();

               if ( name.GetAt( 0 ) == '\"' )
               {
                  MXD_GNS::CStringHelper::QuotedStringToString( name );
               }
               strLen = strlen( name.CStr() );

               strncpy( reinterpret_cast<char *> ( pCall->clid.cName ),
                        name.CStr(),
                        ( ( strLen > CCTK_CLID_LEN ) ? CCTK_CLID_LEN : strLen ) );
            }
            /* Derive the number field from either the TEL NAI (if present) or
            ** the SIP/SIPS NAI if properly formatted.
            */
            if ( pNaiTel )
            {
               const MXD_GNS::CTelUri *pUri =
                  reinterpret_cast<const MXD_GNS::CTelUri *> ( pNaiTel->GetUri() );
               if ( !pUri->IsGlobalPhoneNumber() )
               {
                  utilTelRemNonDig(
                     reinterpret_cast<const BOS_UINT8 *> (
                        pUri->GetPhoneNumber()->GetString().CStr() ),
                     pCall->clid.cNumber,
                     CCTK_CLID_LEN );
               }
               /* Note that we do the same thing as above, while we could remove
               ** the country identification from the TEL URI information in this
               ** step.
               */
               else
               {
                  utilTelRemNonDig(
                     reinterpret_cast<const BOS_UINT8 *> (
                        pUri->GetPhoneNumber()->GetString().CStr() ),
                     pCall->clid.cNumber,
                     CCTK_CLID_LEN );
               }

               if ( pCdr )
               {
                  hdrValue.EraseAll();
                  pUri->Serialize( hdrValue );
                  pCdr->cdrPeer(
                     reinterpret_cast<const BOS_UINT8 *> (
                                 hdrValue.GetFirstIndexPtr() ),
                     static_cast<const BOS_UINT32> (
                                 hdrValue.GetSize() ) );
               }
            }
            else if ( pNaiSip )
            {
               const MXD_GNS::CSipUri *pUri =
                  reinterpret_cast<const MXD_GNS::CSipUri *> ( pNaiSip->GetUri() );
               if ( pUri->GetUser().GetString().GetAt( 0 ) == '+' )
               {
                  utilTelRemNonDig(
                     reinterpret_cast<const BOS_UINT8 *> (
                           pUri->GetUser().GetString().CStr() ),
                     pCall->clid.cNumber,
                     CCTK_CLID_LEN );
               }

               if ( pCdr )
               {
                  hdrValue.EraseAll();
                  pUri->Serialize( hdrValue );
                  pCdr->cdrPeer(
                     reinterpret_cast<const BOS_UINT8 *> (
                                 hdrValue.GetFirstIndexPtr() ),
                     static_cast<const BOS_UINT32> (
                                 hdrValue.GetSize() ) );
               }
            }
            /* If no number field was set through the above step, then set
            ** the default value...
            */
            if ( !strlen( reinterpret_cast<char *> ( pCall->clid.cNumber ) ) )
            {
               sprintf( reinterpret_cast<char *> ( pCall->clid.cNumber ),
                        "O" );
               if ( pCdr )
               {
                  pCdr->cdrAnonymous( BOS_TRUE );
               }
            }
            else
            {
               if ( pCdr )
               {
                  pCdr->cdrAnonymous( BOS_FALSE );
               }
            }
         }
         pSceGenEvt->ReleaseIfRef();
         pSceGenEvt = NULL;
      }
      else
      {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
         /*If extraction of CLID info from PAI enabled
         **otherwise keep the CLID info extracted from To header
         */
         if ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoClidInfoPai )
         {
            /* No NAI information, set 'P' in the number field, nothing in the
            ** name field.
            */
            sprintf( reinterpret_cast<char *> ( pCall->clid.cNumber ), "P" );
            if ( pCdr )
            {
               pCdr->cdrAnonymous( BOS_TRUE );
            }
         }
#endif /* CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS */
      }

      /* Locate some other information directly from the signaling packet.
      */
      res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            const MXD_GNS::CSipHeader *pTo =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_TO, res );
            if ( MX_RIS_S( res ) && ( pTo != NULL ) )
            {
               if ( pCdr )
               {
                  /* Outgoing call, To header is used for remote
                  ** identification.
                  */
                  hdrValue.EraseAll();
                  pTo->Serialize( hdrValue );
                  pCdr->cdrRemote( 
                        reinterpret_cast<const BOS_UINT8 *> (
                                    hdrValue.GetFirstIndexPtr()),
                        static_cast<const BOS_UINT32> (
                                    hdrValue.GetSize()) );
               }
            }

            const MXD_GNS::CSipHeader *pFrom =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_FROM, res );
            if ( MX_RIS_S( res ) && ( pFrom != NULL ) )
            {
               if ( pCdr )
               {
                  /* Outgoing call, From header is used for local
                  ** identification.
                  */
                  hdrValue.EraseAll();
                  pFrom->Serialize( hdrValue );
                  pCdr->cdrLocal( 
                        reinterpret_cast<const BOS_UINT8 *> (
                                    hdrValue.GetFirstIndexPtr()),
                        static_cast<const BOS_UINT32> (
                                    hdrValue.GetSize()) );
               }
            }

            /* Process the P-Asserted Identity header. If it exists, update 
            ** caller-id info if no valid network asserted identities were
            ** found. 
            */
            const MXD_GNS::CSipHeader *pPAssertedIdentityHeader =
              pPacket->GetHeaderList().Get( MXD_GNS::eHDR_P_ASSERTED_IDENTITY, res);
            if ( MX_RIS_S( res ) && ( pPAssertedIdentityHeader != NULL )
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
               && ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoClidInfoPai )
#endif
            )
            {
               if ( strlen( reinterpret_cast<char *> ( pCall->clid.cNumber )) &&
                     !strCmpNoCase( reinterpret_cast<char *> ( pCall->clid.cNumber ), "P" ))
               {
                  sprintf( reinterpret_cast<char *> ( pCall->clid.cNumber ),
                     "O" );
               }
            }

            const MXD_GNS::CSipHeader *pSupported =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_SUPPORTED, res );
            if ( MX_RIS_S( res ) && ( pSupported != NULL ) )
            {
               const MXD_GNS::CString &rstrTdialog =
                                 MXD_GNS::GetOptionTag(
                                    MXD_GNS::eOPTIONTAG_TDIALOG );

               while ( pSupported != NULL )
               {
                  if ( pSupported->GetSupported().GetString().
                                          CaseInsCmp( rstrTdialog ) == 0 )
                  {
                     if ( pCdr )
                     {
                        pCdr->cdrTgtDlg( BOS_TRUE );
                     }
                     pSupported = NULL;
                  }
                  else
                  {
                     pSupported = pSupported->GetNextHeader();
                  }
               }
            }
            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }  
         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      /* Update the cdr with the caller-id info
      */
      if ( pCdr )
      {
         pCdr->cdrClidInfo( &pCall->clid );
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_CONNECTED);
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_ANSWERED);
      }

      pResCall->resourceSetCallInfo( pCall );

      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_ANSWER,
                reinterpret_cast<void *> ( pCall ) );

#if CCTKCFG_REMOTE_USER_CONTROL
      /* If there is a proxy call associated with this call, the
      ** call is now answered, so we need to signal this to the
      ** other leg as well.
      */
      if ( pResCall->resourceProxyGuid() != guCeInvalidGuid )
      {
         CeResBase *pProxyCall = NULL;

         if ( CeMain::locateRes( pResCall->resourceProxyGuid(),
                                 pProxyCall ) )
         {
            /* Link the original call on the local user with this call from
            ** the remote user since now we know which call leg has taken
            ** ownership of the call.
            */
            CeResBase *pResUser = NULL;

            if ( CeMain::locateRes( pProxyCall->resourceParent(),
                                    pResUser ) )
            {
               if ( pResUser->resourceGender() == CeResBase::eCERESTYPE_USR )
               {
                  CeResUser *pUser = reinterpret_cast<CeResUser *>
                     ( pResUser->resourceAddress() );

                  if ( !(pUser->resourceIsRemUsr()) )
                  {
                     if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
                     {
                        CeResCall *pResProxyCall = reinterpret_cast<CeResCall *>
                           ( pProxyCall->resourceAddress() );

                        pResProxyCall->resourceProxyGuid( pResCall->resourceGuid() );
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
                  fsmError( rRes );
                  return;
               }
            }

            /* Answer the corresponding leg of the call now.
            */
            ceNotify( pProxyCall->resourceGuid(),
                      eCEEVT_CALL_USR_ANSWER,
                      NULL );

            /* Inform the upper application layer that the call is
            ** now connected.  Note that the call may still fail at this
            ** point in time, but upon failure a cleanup event would also
            ** be generated to the upper layer anyways.
            */
            tkmEvent( pProxyCall->resourceParent(),
                      pProxyCall->resourceGuid(),
                      eCCTKEVT_ANSWER,
                      NULL );
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
**  FUNCTION:   CeFsmCallOut::fsmAlerted
**
**  PURPOSE:    Outgoing call is ringing on the remote side or is providing early
**              media (remote user has been alerted).
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmAlerted( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      CCTKCALLINFO *pCall = NULL;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pCall = pResCall->resourceCallingParty();

      CeCdr *pCdr =
      CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                     pResCall->resourceGuid() );

      if ( pCdr )
      {
         pCdr->cdrCallRespRcvTime( BOS_FALSE );
      }

      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl;
      mxt_result res;

      /* Update the resource state.
      */
      if ( pResCall->resourceState() == CeFsmBase::eCEFSMSTATE_CALLOUT_ATTEMPT )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLOUT_ALERT );
      }

      /* Get some information from the actual signaling packet.
      */
      res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* Check whether some Alert-Info information was present with the
            ** packet or not.  If yes, use it.
            */
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
               MXD_GNS::CBlob hdrValue;

               hdrValue.EraseAll();
               pAlertInfo->Serialize( hdrValue );
               memcpy( static_cast<void *> ( pCall->cAlertInfo[ 0 ] ),
                       hdrValue.GetFirstIndexPtr(),
                       (( hdrValue.GetSize() > CCTK_STR_LEN ) ?
                                       CCTK_STR_LEN : hdrValue.GetSize()) );
            }
            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }         

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      pResCall->resourceSetCallInfo( pCall );
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_ALERTED,
                reinterpret_cast<void *> ( pResCall->resourceCallingParty() ) );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallOut::fsmEarlyMedia
**
**  PURPOSE:    Outgoing call is providing early media information.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      The early media information should come from the MSE callback
**              via a 'media' event.  Therefore we simply update the call state
**              at this point.
***********************************************************************************/
void CeFsmCallOut::fsmEarlyMedia( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Update the resource state.
      */
      if ( pResCall->resourceState() == CeFsmBase::eCEFSMSTATE_CALLOUT_ATTEMPT )
      {
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLOUT_ALERT );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallOut::fsmCreate
**
**  PURPOSE:    Creating support for an outgoing call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmCreate( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   BOS_BOOL bCreate = BOS_FALSE;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      MXD_GNS::MseSdpSession *pMse;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pMse = static_cast<MXD_GNS::MseSdpSession *>
               (BRCM_NEW( MXD_GNS::MseSdpSession ));

      const CFGCALL *pCfgCall = CFG_CALL( pResCall->resourceGuid() );

      if ( pMse )
      {
         pMse->setCid( pResCall->resourceGuid() );
         pResCall->resourceSetMse( pMse );
         pResCall->resourceNetHdl()->SetMspSession( pMse );

         bCreate = BOS_TRUE;
      }

      /* If something went wrong, we need to release this resource
      ** and inform the user of the failure.
      */
      if ( !bCreate )
      {
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_ENDCALL,
                   NULL );

         pResCall->resourceEnd();
      }
      else
      {
         CeCdr *pCdr =
            CeMain::ceCdrMgr()->allocateCdr( pResCall->resourceParent(),
                                             CeCdr::eCECDRTYPE_OUT );
         if ( pCdr )
         {
            pCdr->cdrSetRes( pResCall->resourceGuid() );
            pCdr->cdrStart();
            pCdr->cdrEmergency(
               pCfgCall ? 
                  pCfgCall->xtl.basic.gen.emgCallCfg.bEmergency : 
                  BOS_FALSE );
            
            CeMain::associateLastCall( pResCall->resourceParent(),
                                       pCdr,
                                       CeCdr::eCECDRTYPE_OUT );
         }

         /* Issue a management event targeted to the parent resource.
         */
         ceNotify ( pResCall->resourceParent(),
                    eCEEVT_CALL_ADD,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallOut::fsmMake
**
**  PURPOSE:    Place the outgoing call to the network.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallOut::fsmMake( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::CNameAddr addr;
      MXD_GNS::IUri *pUri;

      CeCdr *pCdr =
         CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                        pResCall->resourceGuid() );
      if ( pCdr )
      {
         pCdr->cdrPeer( reinterpret_cast<const BOS_UINT8 *> (
                                       pResCall->resourceCalledParty()),
                        static_cast<const BOS_UINT32> ( strlen(
                           reinterpret_cast<const char *> (
                              pResCall->resourceCalledParty()))) );
      }

      /* Convert the called party information into a proper URI structure, if
      ** necessary.
      */
      pUri = cfgCvtAddrToMx( pResCall->resourceCalledParty() );
      if ( pUri == NULL )
      {
         /* Conversion failed, must be an invalid URI, clean up and exit.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );
         pResCall->resourceLocalError( BOS_TRUE );

         pResCall->resourceNetHdl()->TerminateCall();

         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_ENDCALL,
                   NULL );
      }
      else
      {
         BOS_UINT32 strLen;
         const char *pcNumber = NULL;
         CCTKCALLINFO *pCall = pResCall->resourceCallingParty();

         /* Proceed with the call to the network.
         */
         addr.SetUri( pUri );

         /* Update the resource state.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLOUT_ATTEMPT );

         /* Ask the network to place the call.
         */
         pResCall->resourceNetHdl()->MakeCall( addr, NULL );

         /* Extracted the CLID info from "To:" header
         ** Since we don't know the callee name at this moment
         ** we use number here, it will be replaced by the real
         ** name if it presented in call answer stage
         */
         if ( pUri->GetUriType() == MXD_GNS::IUri::eSIP ||
              pUri->GetUriType() == MXD_GNS::IUri::eSIPS )
         {
            const MXD_GNS::CSipUri *pSipUri =
                  reinterpret_cast<const MXD_GNS::CSipUri *> ( pUri );
            /* Get the number field from the username.
            */
            pcNumber = pSipUri->GetUser().GetString().CStr();
                           
         }
         else if ( pUri->GetUriType() == MXD_GNS::IUri::eTEL )
         {
            const MXD_GNS::CTelUri *pTelUri =
                  reinterpret_cast<const MXD_GNS::CTelUri *> ( pUri );
            /* Get the number field from the phone number.
            */
            pcNumber = pTelUri->GetPhoneNumber()->GetString().CStr();
         }
         else if ( pUri->GetUriType() == MXD_GNS::IUri::eABSOLUTE )
         {
            const MXD_GNS::CAbsoluteUri *pAbsUri =
                  reinterpret_cast<const MXD_GNS::CAbsoluteUri *> ( pUri );
            /* Get the number field from the body.
            */
            pcNumber = pAbsUri->GetBody().CStr();
         }

         strLen = strlen( pcNumber );
         strncpy( reinterpret_cast<char *> ( pCall->clid.cNumber ),
                  pcNumber,
                  (( strLen > CCTK_CLID_LEN ) ?  CCTK_CLID_LEN : strLen) );

         strncpy( reinterpret_cast<char *> ( pCall->clid.cName ),
                  pcNumber,
                  (( strLen > CCTK_CLID_LEN ) ?  CCTK_CLID_LEN : strLen) );

         if ( pCdr )
         {
            pCdr->cdrClidInfo( &pCall->clid );
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallOut::fsmPendSdpOfr
**
**  PURPOSE:    MSE is letting CE know that a pending offer is waited on.  This
**              means we are to provide an SDP offer for the outgoing call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This function should only be used when managing remote user.
**
**              When this event is received for a call associated with a remote
**              user call it means the call is the locally generated leg of the
**              call and we need to set the offer SDP from the original call.
**
**              Otherwise, trigger a call failure on invalid event.
***********************************************************************************/
void CeFsmCallOut::fsmPendSdpOfr( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResCall *pResCall = NULL;
   BOS_BOOL bReject = BOS_FALSE;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      if ( pResCall->resourceProxyGuid() != guCeInvalidGuid )
      {
         CeResBase *pProxyCall = NULL;
         CeResBase *pProxyUsr = NULL;

         /* Only apply this processing for call that are associated
         ** with a proxy'ed call.
         */
         bReject = BOS_TRUE;

         if ( CeMain::locateRes( pResCall->resourceProxyGuid(),
                                 pProxyCall ) )
         {
            if ( CeMain::locateRes( pProxyCall->resourceParent(),
                                    pProxyUsr ) )
            {
               if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
               {
                  CeResCall *pResProxyCall =
                     reinterpret_cast<CeResCall *> ( pProxyCall->resourceAddress() );

                  if ( /* We have a valid SDP offer associated with the proxy
                       ** call.  Note that at this time, this should be true
                       ** always.
                       ** We do not support not receiving media information
                       ** in the original call INVITE for now in particular.
                       */
                       pResProxyCall->resourceSdpOffer() )
                  {
                     MXD_GNS::MseSdpSession *pMse;

                     pResCall->resourceGetMse( pMse );
                     if ( pMse )
                     {
                        /* Pass the SDP offer from the remote user call which
                        ** initiated the process of creating this local user
                        ** proxy'ed outgoing call.
                        */
                        pMse->setOfferSdp( pResProxyCall->resourceSdpOffer() );

                        /* Notify the MSE to send the INVITE */
                        pMse->mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );

                        /* We have processed all according to intended expectations,
                        ** no need to reject this call locally.
                        */
                        bReject = BOS_FALSE;
                     }
                  }
               }
               else
               {
                  fsmError( rRes );
                  return;
               }
               // TO_DO: Set the call-id??
               
            }
         }
      }
      else if ( pResCall->resourceProxyGuid() == guCeInvalidGuid )
      {
         MXD_GNS::MseSdpSession *pMse;

         /* This is a local call only (not associated with any remote
         ** resource).  The framework requires that in this case we still
         ** issue a media update for the MSE to continue its processing
         ** of the outgoing call.
         */
         bReject = BOS_TRUE;

         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            pMse->mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
            bReject = BOS_FALSE;
         }
      }

      /* If this failed for some reason, end the call now.
      */
      if ( bReject )
      {
         ceNotify ( pResCall->resourceGuid(),
                    eCEEVT_CALL_NET_FAILED,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallOut::fsmSdpAns
**
**  PURPOSE:    MSE is letting CE know that an answer SDP has been received.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This function should only be used when managing remote user.
**
**              When this event is received for a call associated with a remote
**              user call it means the call is the locally generated leg of the
**              call and we have received an answer SDP for that call.  We need
**              to propagate this SDP answer back to the original call from the
**              remote user.
***********************************************************************************/
void CeFsmCallOut::fsmSdpAns( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResCall *pResCall = NULL;
   BOS_BOOL bReject = BOS_FALSE;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      if ( pResCall->resourceProxyGuid() != guCeInvalidGuid )
      {
         CeResBase *pProxyCall = NULL;
         CeResBase *pProxyUsr = NULL;

         /* Only apply this processing for call that are associated
         ** with a proxy'ed call.
         */
         bReject = BOS_TRUE;

         if ( CeMain::locateRes( pResCall->resourceProxyGuid(),
                                 pProxyCall ) )
         {

            if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
            {
               CeResCall *pResProxyCall =
                  reinterpret_cast<CeResCall *> ( pProxyCall->resourceAddress() );

               /* associate the proxy call with this call */
               pResProxyCall->resourceProxyGuid( pResCall->resourceGuid() );

               if ( CeMain::locateRes( pProxyCall->resourceParent(),
                                       pProxyUsr ) )
               {
                  if ( pResCall->resourceSdpAnswer() )
                  {
                     MXD_GNS::MseSdpSession *pMse;
                     pResProxyCall->resourceGetMse( pMse );
                     if ( pMse )
                     {
                        /* Pass the SDP answer from this local call to the
                        ** initial remote user call that has been proxy'ed.
                        */
                        pMse->setAnswerSdp( pResCall->resourceSdpAnswer() );
                        pMse->mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
                     
                        /* We have processed all according to intended expectations,
                        ** no need to reject this call locally.
                        */
                        bReject = BOS_FALSE;
                     }
                  }
               }
            }
            else
            {
               fsmError( rRes );
            }
         }
      }

      /* If this failed for some reason, end the call now.
      */
      if ( bReject )
      {
         ceNotify ( pResCall->resourceGuid(),
                    eCEEVT_CALL_NET_FAILED,
                    NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}
