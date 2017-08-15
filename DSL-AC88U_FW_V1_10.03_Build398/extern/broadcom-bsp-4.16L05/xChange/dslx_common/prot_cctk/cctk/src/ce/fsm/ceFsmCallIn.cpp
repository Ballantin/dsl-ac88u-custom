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
**      This file implements the incomig call finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmCallIn.h>
#include <ceFsmCall.h>
#include <ceMain.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

#define CE_FSM_VALUE__ANS_MODE__MANUAL          "Manual"
#define CE_FSM_VALUE__ANS_MODE__AUTO            "Auto"
#define CE_FSM_VALUE__ANON_SIPURI               "anonymous@anonymous.invalid"

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- State Machine Definition ------------------------------------------------ */
/* ------------------------------------------------------------------------------ */
/* ---- The specific state machine for this processing is defined below.  The --- */
/* ---- number of states and events processed by this FSM are defined and the --- */
/* ---- FSM is populated. ------------------------------------------------------- */
/* ------------------------------------------------------------------------------ */

static const BOS_UINT32 gFsmNumberOfStates =
      CeFsmBase::eCEFSMSTATE_CALLIN_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_CALLIN_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_CALL_SET_FINISH - eCEEVT_CALL_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_CALLIN_IDLE -------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmCallIn::fsmCalled,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmCallIn::fsmCanceled,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmCallIn::fsmAlert,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmCallIn::fsmReplace,
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
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmCallIn::fsmSetup,
        /* ---- eCEEVT_CALL_USR_ANSWER ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_UNHOLD ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FORWARD ----------- */ &CeFsmCallIn::fsmForward,
        /* ---- eCEEVT_CALL_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ENDED ------------- */ &CeFsmCallIn::fsmReject,
        /* ---- eCEEVT_CALL_USR_PROXY ------------- */ &CeFsmCallIn::fsmProxy,
        /* ---- eCEEVT_CALL_USR_DTMF -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FLASH-------------- */ &CeFsmBase::fsmNoOp,
   },

   /* ---- eCEFSMSTATE_CALLIN_ALERT -------------------------------------------- */
   {
        /* ---- eCEEVT_CALL_NET_CALLED ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_TERMUSR ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_TERMNET ----------- */ &CeFsmCall::fsmEnded,
        /* ---- eCEEVT_CALL_NET_FAILED  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER  ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_RING -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_EARLYMEDIA -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_REPLACE ----------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_MEDIA ------------- */ &CeFsmCall::fsmMedia,
        /* ---- eCEEVT_CALL_NET_MEDIA_FAIL -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_OFFER_SDP ---- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_PEND_ANSWER_SDP --- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_ANSWER_SDP -------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_NET_INFO -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_CREATE ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MAKE -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_MEDIA ------------- */ &CeFsmCallIn::fsmSetup,
        /* ---- eCEEVT_CALL_USR_ANSWER ------------ */ &CeFsmCallIn::fsmAnswer,
        /* ---- eCEEVT_CALL_USR_HOLD -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_UNHOLD ------------ */ &CeFsmCallIn::fsmUnhold,
        /* ---- eCEEVT_CALL_USR_FORWARD ----------- */ &CeFsmCallIn::fsmForward,
        /* ---- eCEEVT_CALL_USR_XFER -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_BXFER ------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_XFER_NOTERM ------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_ENDED ------------- */ &CeFsmCallIn::fsmReject,
        /* ---- eCEEVT_CALL_USR_PROXY ------------- */ &CeFsmCallIn::fsmProxy,
        /* ---- eCEEVT_CALL_USR_DTMF -------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_CALL_USR_FLASH-------------- */ &CeFsmBase::fsmNoOp,
   },

};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmRun
**
**  PURPOSE:    Run the incoming call state machine.
**
**  PARAMETERS:
**              rRes - reference to the call resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_CALLIN_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_CALLIN_SET_FINISH) &&
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
                           - (CeFsmBase::eCEFSMSTATE_CALLIN_SET_BEGIN + 1);
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
**  FUNCTION:   CeFsmCallIn::fsmCalled
**
**  PURPOSE:    There is an new incoming call to be handled.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmCalled( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      BOS_BOOL bAccept = BOS_TRUE;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Create the corresponding MSE session for the call at this time.
      **
      ** Note that the call can still be rejected following a failed attempt to
      ** allocated the MSE resources.
      */
      if ( bAccept )
      {
         MXD_GNS::MseSdpSession *pMse;

         pMse = static_cast<MXD_GNS::MseSdpSession *>
                  (BRCM_NEW( MXD_GNS::MseSdpSession ));

         if ( pMse )
         {
            pMse->setCid( pResCall->resourceGuid() );
            pResCall->resourceSetMse( pMse );
            pResCall->resourceNetHdl()->SetMspSession( pMse );
         }
         else
         {
            CCTK_TRACEI1( "fsmCalled (0x%lX) - rejecting due to lack of MSE.",
                          rRes.resourceGuid() );
            bAccept = BOS_FALSE;
         }
      }

      if ( bAccept )
      {
         /* Get the calling party information to be passed to the local appplication
         **
         ** We attempt the get the calling party information from the NAI first.  The
         ** rules applied to get this information are derived from the PacketCable 2.0
         ** RSTF specification which in accordance with Bellcore telephony specs is
         ** offering a mapping from the NAI information to CLID information.
         **
         ** If the above fails, then we attempt a second pass to get CLID information
         ** from the From header.  This second pass is configurable.
         **
         */
         CCTKCALLINFO call;
         const MXD_GNS::CNameAddr *pNaiSip = NULL;
         const MXD_GNS::CNameAddr *pNaiTel = NULL;
         const MXD_GNS::CSipPacket *pPacket = NULL;
         MXD_GNS::ISceGenericEventInfo *pSceGenEvt = NULL;
         MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
         MXD_GNS::IEComUnknown *pEcomInfo = NULL;
         MXD_GNS::IUaSspCallEventInfo *pEvtInfo = NULL;
         MXD_GNS::CBlob hdrValue;
         BOS_UINT32 uXferGuid = guCeInvalidGuid;
         mxt_result res;
         call.bAnonymous = BOS_FALSE;  

         memset ( reinterpret_cast<void *> ( &call ),
                  0,
                  sizeof ( CCTKCALLINFO ) );

         CeCdr *pCdr =
            CeMain::ceCdrMgr()->allocateCdr( pResCall->resourceParent(),
                                             CeCdr::eCECDRTYPE_IN );
         if ( pCdr )
         {
            pCdr->cdrSetRes( pResCall->resourceGuid() );
            pCdr->cdrStart();
         }

         pEcomInfo = (pResCall->resourceAddtionalCallData() ?
                              static_cast<MXD_GNS::IEComUnknown *> (
                                 pResCall->resourceAddtionalCallData()) : NULL);
         if ( pEcomInfo )
         {
            pEcomInfo->QueryIf( &pEvtInfo );
            pEcomInfo->QueryIf( &pSceGenEvt );
            pEcomInfo = NULL;
         }

         if ( pEvtInfo != NULL )
         {
            MXD_GNS::IUaSspCall *pReplacedCall = NULL;

            /* Check whether this call is replacing an existing call (i.e. this is
            ** a call that has been transfered).
            */
            pEvtInfo->GetReplacedCall( pReplacedCall );
            if ( pReplacedCall != NULL )
            {
               CeResBase *pRes = NULL;

               if ( CeMain::locateCallRes( pReplacedCall, pRes ) &&
                    ( pRes != NULL ) )
               {
                  uXferGuid = pRes->resourceGuid();
               }
               
               pReplacedCall->ReleaseIfRef();
               pReplacedCall = NULL;
            }

            pEvtInfo->ReleaseIfRef();
            pEvtInfo = NULL;
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
               /* No NAI information, set 'P' in the number field, nothing in the
               ** name field.
               */
               snprintf( reinterpret_cast<char *> ( call.clid.cNumber ), 
                         (CCTK_CLID_LEN + 1), "P" );
                        
               /* Indicate Anonymous call.
               */         
               call.bAnonymous = BOS_TRUE;                        
               
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

                  strncpy( reinterpret_cast<char *> ( call.clid.cName ),
                           name.CStr(),
                           (( strLen > CCTK_CLID_LEN ) ? CCTK_CLID_LEN : strLen) );
               }
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
               /* Otherwise see if the can use a TEL URI to derive the name field
               ** if configured to do so.
               */
               else if ( pNaiTel && CFG_SYS->xtl.net.prot.gen.imsCfg.bCnamePaiSip )
               {
                  BOS_UINT32 strLen;
                  MXD_GNS::CString name = pNaiTel->GetDisplayName().GetString();

                  if ( name.GetAt( 0 ) == '\"' )
                  {
                     MXD_GNS::CStringHelper::QuotedStringToString( name );
                  }
                  strLen = strlen( name.CStr() );

                  strncpy( reinterpret_cast<char *> ( call.clid.cName ),
                           name.CStr(),
                           (( strLen > CCTK_CLID_LEN ) ? CCTK_CLID_LEN : strLen) );
               }
#endif
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
                        call.clid.cNumber,
                        CCTK_CLID_LEN );
                  }
                  /* Note that we do the same thing as above, while we could remove
                  ** the country identification from the TEL URI information in this
                  ** step, but we leave this up to the upper level application to
                  ** decide.
                  */
                  else
                  {
                     utilTelRemNonDig(
                        reinterpret_cast<const BOS_UINT8 *> (
                              pUri->GetPhoneNumber()->GetString().CStr() ),
                        call.clid.cNumber,
                        CCTK_CLID_LEN );
                  }

                  if ( pCdr )
                  {
                     hdrValue.EraseAll();
                     pUri->Serialize( hdrValue );
                     pCdr->cdrPeer(
                        reinterpret_cast<const BOS_UINT8 *> (
                                    hdrValue.GetFirstIndexPtr()),
                        static_cast<const BOS_UINT32> (
                                    hdrValue.GetSize()) );
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
                        call.clid.cNumber,
                        CCTK_CLID_LEN );
                  }
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
                  /* Extract the information we can from the SIP URI of the
                  ** network asserted identity no matter what.
                  */
                  else if ( CFG_SYS->xtl.net.prot.gen.imsCfg.bClidInfoPaiSip )
                  {
                     utilTelRemNonDig(
                        reinterpret_cast<const BOS_UINT8 *> (
                              pUri->GetUser().GetString().CStr() ),
                        call.clid.cNumber,
                        CCTK_CLID_LEN );
                  }
#endif

                  if ( pCdr )
                  {
                     hdrValue.EraseAll();
                     pUri->Serialize( hdrValue );
                     pCdr->cdrPeer(
                        reinterpret_cast<const BOS_UINT8 *> (
                                    hdrValue.GetFirstIndexPtr()),
                        static_cast<const BOS_UINT32> (
                                    hdrValue.GetSize()) );
                  }
               }
               /* If no number field was set through the above step, then set
               ** the default value...
               */
               if ( !strlen( reinterpret_cast<char *> ( call.clid.cNumber ) ) )
               {
                  sprintf( reinterpret_cast<char *> ( call.clid.cNumber ),
                           "O" );
                           
                  /* Indicate Anonymous call.
                  */         
                  call.bAnonymous = BOS_TRUE;  
               }
               else
               {
                  /* Indicate Non-Anonymous call.
                  */         
                  call.bAnonymous = BOS_FALSE;  
               }
            }

            pSceGenEvt->ReleaseIfRef();
            pSceGenEvt = NULL;
         }
         else
         {
            /* No NAI information, set 'P' in the number field, nothing in the
            ** name field.
            */
            sprintf( reinterpret_cast<char *> ( call.clid.cNumber ),
                     "P" );
                     
            /* Indicate Anonymous call.
            */         
            call.bAnonymous = BOS_TRUE;  
         }


         /* Locate some other information directly from the signaling packet.
         */
         if ( pResCall->resourceNetHdl() )
         {
            res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
            if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
            {
               pSceBasExtCtl->GetLastReceivedPacket( pPacket );
               if ( pPacket )
               {
                  /* Alert-Info
                  */
                  /* Check whether some Alert-Info information was present with the
                  ** packet or not.  If yes, use it.
                  **
                  ** Note that we are able to accept multiple Alert-Info pattern, the
                  ** application will make the decision on which one to use for the
                  ** particular scenario it is running.
                  */
                  const MXD_GNS::CSipHeader *pAlertInfo =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_ALERT_INFO, res );
                  BOS_UINT32 uIx = 0;

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
                     while ( (pAlertInfo != NULL) && (uIx < CCTKCFG_MAX_ALERT_INFO) )
                     {
                        hdrValue.EraseAll();
                        pAlertInfo->Serialize( hdrValue );
                        memcpy( static_cast<void *> ( call.cAlertInfo[ uIx ] ),
                                hdrValue.GetFirstIndexPtr(),
                                (( hdrValue.GetSize() > CCTK_STR_LEN ) ?
                                                CCTK_STR_LEN : hdrValue.GetSize()) );

                        /* Increment.
                        */
                        uIx++;
                        pAlertInfo = pAlertInfo->GetNextHeader();
                     };

                     /* Check whether we need to reverse the order of the Alert-Info
                     ** retrieved information for interop purposes.
                     */
                     if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                                   CCTK_INTEROP_REVALRTNFO ) )
                     {
                        BOS_UINT32 uJx, uKx = 0;
                        BOS_UINT8 cAlertInfo[CCTKCFG_MAX_ALERT_INFO][CCTK_STR_LEN + 1];

                        memset( static_cast<void *> ( cAlertInfo ),
                                0,     
                                sizeof( cAlertInfo ) );

                        uKx = uIx - 1;
                        for ( uJx = 0 ;
                              (uJx < uIx) && (uJx < CCTKCFG_MAX_ALERT_INFO) ;
                              uJx++ )
                        {
                           memcpy( static_cast<void *> ( cAlertInfo[ uJx ] ),
                                   static_cast<const void *> ( call.cAlertInfo[ uKx ] ),     
                                   strlen ( reinterpret_cast<char *> (
                                                            call.cAlertInfo[ uKx ]) ));
                           /* Safety check, should not be necessary though.
                           */
                           if ( uKx != 0 )
                           {
                              uKx--;
                           }
                        }

                        memcpy( static_cast<void *> ( call.cAlertInfo ),
                                static_cast<const void *> ( cAlertInfo ),     
                                sizeof( cAlertInfo ) );
                     }
                  }

                  /* Emergency call.
                  */
                  const MXD_GNS::CSipHeader *pPriority =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_PRIORITY, res );
                  if ( MX_RIS_S( res ) && ( pPriority != NULL ) )
                  {
                     const MXD_GNS::CToken priority = pPriority->GetPriority();
                     if ( !priority.GetString().CaseInsCmp(
                                                   CE_MAIN__PRI__EMERGENCY ) )
                     {
                        call.bEmergency = BOS_TRUE;
                     }
                  }

                  /* Process the P-Asserted Identity header. If it exists, update 
                  ** caller-id info and check whether the uri specifies service sos urn
                  ** to indicate emergency callback. 
                  */
                  const MXD_GNS::CSipHeader *pPAssertedIdentityHeader =
                    pPacket->GetHeaderList().Get( MXD_GNS::eHDR_P_ASSERTED_IDENTITY, res);
                  if ( MX_RIS_S( res ) && ( pPAssertedIdentityHeader != NULL )
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
                      && ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoClidInfoPai )
#endif
                  )
                  {
                     /* A P-Asserted-Identity header exists - update the caller-id number  
                     ** if a valid network asserted identity wasn't found.
                     */
                     if ( strlen( reinterpret_cast<char *> ( call.clid.cNumber ) )&&
                           !strCmpNoCase( reinterpret_cast<char *> ( call.clid.cNumber ), "P" ) 
                        )
                     {
                        sprintf( reinterpret_cast<char *> ( call.clid.cNumber ),
                           "O" );
                     }
                                       
                     /* Check to see whether this is an emergency call
                     */
                     if ( !call.bEmergency )
                     {
                        while ( pPAssertedIdentityHeader != NULL )
                        {
                           const MXD_GNS::CNameAddr assertedIdentity = pPAssertedIdentityHeader->GetPAssertedIdentity();
                           const MXD_GNS::IUri *pUri = assertedIdentity.GetUri();
                           
                           if ( pUri && 
                                pUri->GetUriType() == MXD_GNS::IUri::eABSOLUTE )
                           {
                              const MXD_GNS::CAbsoluteUri *pAbsUri =
                              reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                               pUri );

                              if ( !strCmpNoCase( pAbsUri->GetScheme(),
                                                  "urn" ) &&
                                    !strCmpNoCase( pAbsUri->GetBody().CStr(),
                                                  CE_MAIN_SERVICE_SOS ) )
                              {
                                 call.bEmergency = BOS_TRUE;
                                 pPAssertedIdentityHeader = NULL;
                              }
                           }
                           
                           if ( pPAssertedIdentityHeader )
                           {
                              pPAssertedIdentityHeader = pPAssertedIdentityHeader->GetNextHeader();
                           }
                        }
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

                  const MXD_GNS::CSipHeader *pTo =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_TO, res );
                  if ( MX_RIS_S( res ) && ( pTo != NULL ) )
                  {
#if CCTKCFG_REMOTE_USER_CONTROL
                     /* Pass the final target destination such
                     ** that the application layer can do what it needs to proxy
                     ** this call to the final intended destination.
                     */
                     {     
                        BOS_UINT32 strLen;
                        const char *pcNumber = NULL;
                        const MXD_GNS::CNameAddr to = pTo->GetTo();
                        const MXD_GNS::IUri *pUri = to.GetUri();

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
                              reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                            pUri );
                           /* Get the number field from the body.
                           */
                           pcNumber = pAbsUri->GetBody().CStr();
                        }

                        strLen = strlen( pcNumber );
                        strncpy( reinterpret_cast<char *> ( call.cTarget ),
                                 pcNumber,
                                 (( strLen > CCTK_STR_LEN ) ?
                                       CCTK_STR_LEN : strLen) );
                     }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */

                     if ( pCdr )
                     {
                        /* Incoming call, To header is used for local
                        ** identification.
                        */
                        hdrValue.EraseAll();
                        pTo->Serialize( hdrValue );
                        pCdr->cdrLocal( 
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
#if CCTKCFG_REMOTE_USER_CONTROL
                     /* Pass the "From" destination such
                     ** that the application layer can determine if this call is 
                     ** coming from an remote user.
                     */
                     {                     
                        BOS_UINT32 strLen;
                        const char *pcNumber = NULL;
                        const MXD_GNS::CNameAddr from = pFrom->GetFrom();
                        const MXD_GNS::IUri *pUri = from.GetUri();

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
                              reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                            pUri );
                           /* Get the number field from the body.
                           */
                           pcNumber = pAbsUri->GetBody().CStr();
                        }

                        strLen = strlen( pcNumber );
                        strncpy( reinterpret_cast<char *> ( call.cFromUserName ),
                                 pcNumber,
                                 (( strLen > CCTK_STR_LEN ) ?
                                       CCTK_STR_LEN : strLen) );
                     }
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
                     if ( pCdr )
                     {
                        /* Incoming call, From header is used for remote
                        ** identification.
                        */
                        hdrValue.EraseAll();
                        pFrom->Serialize( hdrValue );
                        pCdr->cdrRemote( 
                              reinterpret_cast<const BOS_UINT8 *> (
                                          hdrValue.GetFirstIndexPtr()),
                              static_cast<const BOS_UINT32> (
                                          hdrValue.GetSize()) );
                     }

                     /* If no Network Asserted Identity information was received, see if we
                     ** should get the information from the From header.
                     */
                     if ( ( pNaiSip == NULL ) && ( pNaiTel == NULL )
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
                          && CFG_SYS->xtl.net.prot.gen.imsCfg.bClidInfoFrom
#endif
                        )
                     {
                        BOS_UINT32 strLen;
                        const MXD_GNS::CNameAddr from = pFrom->GetFrom();
                        /* Get the name field from the display.
                        */
                        MXD_GNS::CString name = from.GetDisplayName().GetString();
                        if ( name.GetAt( 0 ) == '\"' )
                        {
                           MXD_GNS::CStringHelper::QuotedStringToString( name );
                        }
                        strLen = strlen( name.CStr() );
                        strncpy( reinterpret_cast<char *> ( call.clid.cName ),
                                 name.CStr(),
                                 (( strLen > CCTK_CLID_LEN ) ?
                                          CCTK_CLID_LEN : strLen) );

                        const MXD_GNS::IUri *pUri = from.GetUri();
                        const char *pcNumber = NULL;
                        if ( pUri->GetUriType() == MXD_GNS::IUri::eSIP ||
                             pUri->GetUriType() == MXD_GNS::IUri::eSIPS )
                        {
                           const MXD_GNS::CSipUri *pSipUri =
                              reinterpret_cast<const MXD_GNS::CSipUri *> ( pUri );
                           /* Get the number field from the username.
                           */
                           pcNumber = pSipUri->GetUser().GetString().CStr();
                           
                           /* Re-evaluate anonymous status based on from header.
                           */
                           MXD_GNS::CBlob blob;
                           pSipUri->Serialize( blob );
                           if( strstr( reinterpret_cast<const char *> (blob.GetFirstIndexPtr()), CE_FSM_VALUE__ANON_SIPURI ) == NULL )
                           {
                              call.bAnonymous = BOS_FALSE;  
                           }
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
                              reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                            pUri );
                           /* Get the number field from the body.
                           */
                           pcNumber = pAbsUri->GetBody().CStr();
                        }

                        strLen = strlen( pcNumber );
                        strncpy( reinterpret_cast<char *> ( call.clid.cNumber ),
                                 pcNumber,
                                 (( strLen > CCTK_CLID_LEN ) ?
                                       CCTK_CLID_LEN : strLen) );

                        if ( pCdr )
                        {
                           hdrValue.EraseAll();
                           pUri->Serialize( hdrValue );
                           pCdr->cdrPeer(
                              reinterpret_cast<const BOS_UINT8 *> (
                                          hdrValue.GetFirstIndexPtr()),
                              static_cast<const BOS_UINT32> (
                                          hdrValue.GetSize()) );
                        }
                     }

                     /* Get the 'From' tag to save dialog related information.
                     */
                     const MXD_GNS::CToken *pFromTag =
                                    pFrom->GetParam( CE_CDR__PARAMETER_TAG );
                     if ( pFromTag )
                     {
                        if ( pCdr )
                        {
                           pCdr->cdrFromTag(
                              reinterpret_cast<const BOS_UINT8 *> (
                                                pFromTag->GetString().CStr()) );
                        }
                     }
                  }

                  
                  /* If configured to do so, check for inbound failover scenario, 
                  ** where an incoming call is received from an address we aren't
                  ** registered to.
                  */
                  const CFGUSR *pCfgUsr = CFG_USR( pResCall->resourceParent() );
                  if ( ( pCfgUsr != NULL ) &&
                       pCfgUsr->xtl.usr.net.brCfg.bEnableInbdReg )
                  {
                     MXD_GNS::CSocketAddr peerAddr = pPacket->GetPeerAddr();
                     MXD_GNS::CSocketAddr actTgtAddr;
                     if ( nsBrGetActiveTgt( pResCall->resourceParent(), actTgtAddr ) &&
                          actTgtAddr != peerAddr )
                     {
                        /* The current active target doesn't match the peer address.
                        ** Mark that the signalling for this call needs to directed to
                        ** peer address and shouldn't be tied to its parent's target
                        ** selection.
                        */
                        pResCall->resourceSetSigRestrictedToAddr( BOS_TRUE, &peerAddr );
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

         /* Update the cdr with the call info.
         */
         if ( pCdr )
         {
            /* Record caller-id info
            */
            pCdr->cdrClidInfo( &call.clid );
            
            /* Record anonymity
            */
            pCdr->cdrAnonymous( call.bAnonymous );
         }
         
         pResCall->resourceSetCallInfo( &call );

         /* Issue a management event targeted to the parent resource.
         */
         ceNotify ( pResCall->resourceParent(),
                    eCEEVT_CALL_ADD,
                    NULL );

         /* Pass the information to the application about the new call.
         ** Note that the call could be rejected by the application after
         ** this event but the end user should not be informed of the call
         ** yet.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   (( uXferGuid == guCeInvalidGuid ) ?
                                eCCTKEVT_NEWCALL : eCCTKEVT_XFER),
                   (( uXferGuid == guCeInvalidGuid ) ?
                            reinterpret_cast<void *> (&call) :
                            static_cast<void *> (&uXferGuid)) );
      }
      else
      {
         if ( pResCall->resourceNetHdl() )
         {
            pResCall->resourceNetHdl()->RejectCall();
         }
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmAlert
**
**  PURPOSE:    The new incoming call must be reported to the application.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      We may still reject the call internally at this time in case we
**              do not like something while validating the content.  In such case
**              the upper layer application would not have been made aware of the
**              call at all.
***********************************************************************************/
void CeFsmCallIn::fsmAlert( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   BOS_BOOL bRejectInternal = BOS_FALSE;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      MXD_GNS::ISceBasicExtensionControl *pSceBasExtCtl = NULL;
      const MXD_GNS::CSipPacket *pPacket = NULL;
      MXD_GNS::CBlob hdrValue;
      mxt_result res;
      CCTKCALLINFO *pCall = NULL;
      CeCdr *pCdr = NULL;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALLIN_ALERT );

      pCall = pResCall->resourceCallingParty();
      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );
      if ( pCall )
      {
         pCall->uJoin = guCeInvalidGuid;
      }

      /* Get some information from the actual signaling packet.
      */
      res = pResCall->resourceNetHdl()->QueryIf( &pSceBasExtCtl );
      if ( MX_RIS_S( res ) && ( pSceBasExtCtl != NULL ) )
      {
         pSceBasExtCtl->GetLastReceivedPacket( pPacket );
         if ( pPacket )
         {
            /* Emergency call.
            **
            ** Emergency call and emergency related services from the network are
            ** marked with priority header setting (see RFC 4119).  Unless this is
            ** an operator ringback on an existing emergency call, in which case
            ** we do not need to check the emergency status of the call since we
            ** already know it.
            */
            if ( !pResCall->resourceIsEmergRingbck() )
            {
               if ( pCdr )
               {
                  pCdr->cdrEmergency( BOS_FALSE );
                  
                  const MXD_GNS::CSipHeader *pPriority =
                     pPacket->GetHeaderList().Get( MXD_GNS::eHDR_PRIORITY, res );
                  if ( MX_RIS_S( res ) && ( pPriority != NULL ) )
                  {
                     const MXD_GNS::CToken priority = pPriority->GetPriority();
                     if ( !priority.GetString().CaseInsCmp(
                                                   CE_MAIN__PRI__EMERGENCY ) )
                     {
                        pCdr->cdrEmergency( BOS_TRUE );
                     }
                  }
                  
                  /* Check the P-Asserted Identity header for the sos service urn which
                  ** can indicate emergency call as well.
                  */
                  if(!pCdr->cdrIsEmergency())
                  {
                     const MXD_GNS::CSipHeader *pPAssertedIdentityHeader =
                       pPacket->GetHeaderList().Get( MXD_GNS::eHDR_P_ASSERTED_IDENTITY, res);
                     if ( MX_RIS_S( res ) && ( pPAssertedIdentityHeader != NULL ) )
                     {
                        while ( pPAssertedIdentityHeader != NULL )
                        {
                           const MXD_GNS::CNameAddr assertedIdentity = pPAssertedIdentityHeader->GetPAssertedIdentity();
                           const MXD_GNS::IUri *pUri = assertedIdentity.GetUri();
                           
                           if ( pUri && 
                                pUri->GetUriType() == MXD_GNS::IUri::eABSOLUTE )
                           {
                              const MXD_GNS::CAbsoluteUri *pAbsUri =
                              reinterpret_cast<const MXD_GNS::CAbsoluteUri *> (
                                                                               pUri );
                              if ( !strCmpNoCase( pAbsUri->GetScheme(),
                                                  "urn" ) &&
                                    !strCmpNoCase( pAbsUri->GetBody().CStr(),
                                                  CE_MAIN_SERVICE_SOS ) )
                              {
                                 pCdr->cdrEmergency( BOS_TRUE );
                                 pPAssertedIdentityHeader = NULL;
                              }
                           }
                           
                           if ( pPAssertedIdentityHeader )
                           {
                              pPAssertedIdentityHeader = pPAssertedIdentityHeader->GetNextHeader();
                           }
                        }
                     }
                  }
               }
            }

            /* Barge-in calls.
            **
            ** Barge in calls are used typically through operator service when
            ** there is a requirement to barge into an existing call for monitoring
            ** or other type of third party control.  Such barge-in is achieved
            ** through the usage of the 'Join' header (see RFC 3911).
            **
            ** Our level of support is limited to the ability to join this new
            ** incoming call with an existing one that we can identify as being
            ** associated with the user (through parsing of the Join header).
            */
            const MXD_GNS::CSipHeader *pJoin =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_JOIN, res );
            if ( MX_RIS_S( res ) && ( pJoin != NULL ) )
            {
               const MXD_GNS::CToken join = pJoin->GetJoin();

               /* Try to match the credentials for any existing calls for this same
               ** user with the ones advertized in the Join header.  If one match
               ** is found, this is the one we want to barge into.  If none can be
               ** found, we need to reject this call because it is asking us to
               ** perform an action that cannot be done.
               */
               if ( pCall )
               {
                  pCall->uJoin = CeMain::matchCall(
                                       pResCall->resourceGuid(),
                                       pResCall->resourceParent(),
                                       reinterpret_cast<const BOS_UINT8 *>
                                                    (join.GetString().CStr()) );
                  if ( pCall->uJoin == guCeInvalidGuid )
                  {
                     bRejectInternal = BOS_TRUE;
                  }
               }
            }

            /* Loopback calls.
            **
            ** Loopback calls can be used to verify connectivity and/or run
            ** quality assessement without user knowledge.  Such calls are
            ** signaled to the UE using extension SIP header defined in the
            ** draft: 'draft-ietf-sip-answermode-06.txt'.
            **
            ** Look for either 'Answer-Mode' or 'Priv-Answer-Mode' header.
            ** At this time, the stack does not support those headers, so we
            ** are seeing them as 'extension' headers.
            **
            ** We look into the header content and act upon content indifferently
            ** of the content as long as it matches one of the two mode we support
            ** (auto or manual).  We also indifferently act the same upon either
            ** of the header being present.  We would finally reject the request
            ** in case either of the header is present but we do not see either
            ** of the expected value content.
            */
            const MXD_GNS::CSipHeader *pAnswerMode =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_ANSWER_MODE, res );
            if ( MX_RIS_S( res ) && ( pAnswerMode != NULL ) )
            {
               const MXD_GNS::CToken answerMode = pAnswerMode->GetAnswerMode();
               if ( !answerMode.GetString().CaseInsCmp(
                                             CE_FSM_VALUE__ANS_MODE__AUTO ) ||
                    !answerMode.GetString().CaseInsCmp(
                                             CE_FSM_VALUE__ANS_MODE__MANUAL ) )
                  {
                     if ( pCall )
                     {
                        pCall->bLoopBack = BOS_TRUE;
                     }
                  }
                  else
                  {
                     bRejectInternal = BOS_TRUE;
                  }
               }

            
            const MXD_GNS::CSipHeader *pPrivAnswerMode =
               pPacket->GetHeaderList().Get( MXD_GNS::eHDR_PRIV_ANSWER_MODE, res );
            if ( MX_RIS_S( res ) && ( pPrivAnswerMode != NULL ) )
            {
               const MXD_GNS::CToken privAnswerMode = pPrivAnswerMode->GetPrivAnswerMode();
               if ( !privAnswerMode.GetString().CaseInsCmp(
                                             CE_FSM_VALUE__ANS_MODE__AUTO ) ||
                    !privAnswerMode.GetString().CaseInsCmp(
                                             CE_FSM_VALUE__ANS_MODE__MANUAL ) )
                  {
                     if ( pCall )
                     {
                        pCall->bLoopBack = BOS_TRUE;
                     }
                  }
                  else
                  {
                     bRejectInternal = BOS_TRUE;
                  }
               }

            /* Copy the Call-ID for this call to the CDR.
            */
            const MXD_GNS::CSipHeader *pCid =
                  pPacket->GetHeaderList().Get( MXD_GNS::eHDR_CALL_ID, res );
            if ( MX_RIS_S( res ) && ( pCid != NULL ) && ( pCdr != NULL ))
            {
               if ( !pCid->GetCallId().IsEmpty() )
               {
                  pCdr->cdrCid(
                     reinterpret_cast<const BOS_UINT8 *> (
                                 pCid->GetCallId().GetString().CStr()) );
               }
               else
               {
                  pCdr->cdrCid( NULL );
               }
            }

            /* Finished with the signaling packet.
            */
            pPacket->Release();
         }

         pSceBasExtCtl->ReleaseIfRef();
         pSceBasExtCtl = NULL;
      }

      if ( bRejectInternal )
      {
         if ( pCdr )
         {
            /* Set the CDR termination type to failed.
            */
            pCdr->cdrSetTerm(CeCdr::eCECDRTERM_FAILED);
            pCdr->cdrEnd();
         }

         fsmReject( rRes );
      }
      else
      {
         if ( pCdr && pCall && !pCall->bLoopBack )
         {
            CeMain::associateLastCall( pResCall->resourceParent(),
                                       pCdr,
                                       CeCdr::eCECDRTYPE_IN );

            pCall->bAnonymous = pCdr->cdrIsAnonymous();
         }

         if ( pCall )
         {
            pResCall->resourceSetCallInfo( pCall );
         }

         /* Let the UASSP framework know that we did in fact alert
         ** the local user successfully.
         */
         if ( !pResCall->resourceIsEmergRingbck() )
         {
            pResCall->resourceNetHdl()->CallUserNotified();
         }
         pResCall->resourceSetEmergRingbck( BOS_FALSE );

         /* If configured to apply inbound failover,check to see if this 
         ** call is from a target we're not registered with. If so, notify 
         ** the parent user so it can act on this event as needed.
         */
         const CFGUSR *pCfgUsr = CFG_USR( pResCall->resourceParent() );
         if ( ( pCfgUsr != NULL ) &&
              pCfgUsr->xtl.usr.net.brCfg.bEnableInbdReg )
         {
            if ( pResCall->resourceIsSigRestrictedToAddr() )
            {
               ceNotify ( pResCall->resourceParent(),
                                   eCEEVT_REG_USR_INBOUND_TARGET,
                                   static_cast<void *> ( pResCall->resourceGetSigRestrictedToAddr() ) );
            }
         }

         /* Alert the application of the call.
         */
         tkmEvent( pResCall->resourceParent(),
                   pResCall->resourceGuid(),
                   eCCTKEVT_ALERT,
                   reinterpret_cast<void *> ( pResCall->resourceCallingParty() ) );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmReplace
**
**  PURPOSE:    This new incoming call replaces an existing call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmReplace( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmAnswer
**
**  PURPOSE:    Application is answering an incoming call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmAnswer( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeCdr *pCdr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );

      
      /* Indicate that this call was accepted.
      */
      if ( pCdr )
      {
         pCdr->cdrAccepted(  BOS_TRUE );
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_CONNECTED);
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_ANSWERED);

      }

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      /* Notify the network that the call has been answered.
      */
      pResCall->resourceNetHdl()->AcceptCall();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmUnhold
**
**  PURPOSE:    Application is unholding an incoming call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This is only applicable when the call is an emergency call.
***********************************************************************************/
void CeFsmCallIn::fsmUnhold( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   CeCdr *pCdr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );

      if ( pCdr && pCdr->cdrIsEmergency() )
      {
         /* Update the resource state and FSM.
         */
         pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CONNECT );
         pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

         /* Set the CDR termination type to connected
         */
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_CONNECTED);
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_ANSWERED);

         /* Notify the network that the call has been answered.
         */
         pResCall->resourceGetMse( pMse );
         if ( pMse )
         {
            /* Apply the 'un-hold' to the media session.  This is treated as an answer
            ** because the scenario is just as if we were answering an incoming call.
            */
            pMse->answerEmergencyCall();
         }
      }
      else
      {
         fsmError( rRes );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmForward
**
**  PURPOSE:    Forward this call to a new destination.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmForward( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      MXD_GNS::CNameAddr addr;
      MXD_GNS::IUri *pUri;

      /* Convert the called party information into a proper URI structure, if
      ** necessary.
      */
      pUri = cfgCvtAddrToMx( pResCall->resourceForwardParty() );
      addr.SetUri( pUri );

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      /* Ask the network to place the call.
      */
      pResCall->resourceNetHdl()->ForwardCall( addr );
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmReject
**
**  PURPOSE:    Application is rejecting an incoming call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmReject( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   CeCdr *pCdr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );
      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );

      /* Set the CDR termination type to failed.
      */
      if ( pCdr )
      {
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_FAILED);
      }

      /* Update the resource state and FSM.
      */
      pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_CALL_CLEAN );
      pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_CALL );

      /* Notify the network that the call has been rejected.
      */
      switch ( pResCall->resourceCallEndReason() )
      {
         case eCCTKCALLDELRSN_EMERGEXPIRE:
         {
            pResCall->resourceNetHdl()->TerminateCall();
         }
         break;

         case eCCTKCALLDELRSN_UNAVAILABLE:
         {
            pResCall->resourceNetHdl()->RejectCall(
                                    MXD_GNS::uTEMPORARILY_NOT_AVAILABLE );
         }
         break;

         case eCCTKCALLDELRSN_RGBCKREJECT:
         {
            if ( pCdr && pCdr->cdrIsEmergency() )
            {
               /* Rejecting a ringback on an emergency call, we need to roll
               ** back to a pre-ringback state, which in the case of such means
               ** the emergency call was on hold by the user.
               */
               MXD_GNS::MseSdpSession *pMse = NULL;
               
               pResCall->resourceGetMse( pMse );
               if ( pMse )
               {
                  pMse->rejectEmergencyCall();
               }

               pResCall->resourcePushState( CeFsmBase::eCEFSMSTATE_HOLD_ACCEPT );
               pResCall->resourcePushFsm( CeFsmBase::eCEFSMTYPE_HOLD );
               pResCall->resourceSetHold( eCEEVT_HOLD_USR_HOLD,
                                          BOS_TRUE, BOS_FALSE );
            }
            else
            {
               pResCall->resourceNetHdl()->TerminateCall();
            }
         }
         break;

         case eCCTKCALLDELRSN_ANONYMOUS:
         {
            pResCall->resourceNetHdl()->RejectCall(
                                    MXD_GNS::uANONYMOUS_CALL_REJECT );
         }
         break;

         case eCCTKCALLDELRSN_TERMINATED:
         {
            pResCall->resourceNetHdl()->RejectCall(
                                    MXD_GNS::uREQUEST_TERMINATED );
         }
         break;

         case eCCTKCALLDELRSN_BUSY:
         /* By default UASSP rejects as 'busy'.
         */
         default:
         {
            pResCall->resourceNetHdl()->RejectCall();
         }
         break;
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmCanceled
**
**  PURPOSE:    This new incoming call is canceled by the network before the
**              user has been alerted of the call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmCallIn::fsmCanceled( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;
   CeCdr *pCdr = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Inform application of call termination.
      */
      tkmEvent( pResCall->resourceParent(),
                pResCall->resourceGuid(),
                eCCTKEVT_ENDCALL,
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
      pCdr = CeMain::ceCdrMgr()->locateCdr( pResCall->resourceParent(),
                                            pResCall->resourceGuid() );
      if ( pCdr )
      {
         /* Set the CDR termination type to failed.
         */
         pCdr->cdrSetTerm(CeCdr::eCECDRTERM_FAILED);
         pCdr->cdrEnd();
      }

      /* Clean up the resource.
      */
      pResCall->resourceEnd();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCall::fsmSetup
**
**  PURPOSE:    New media (setup) information has been set by the local user
**              to be used for replying to the incoming call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void CeFsmCallIn::fsmSetup( CeResBase &rRes )
{
   CeResCall *pResCall = NULL;
   MXD_GNS::MseSdpSession *pMse = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      pResCall->resourceGetMse( pMse );
      if ( pMse )
      {
         pMse->mediaUpdate( pResCall->resourceCallMediaReason(), NULL );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmCallIn::fsmProxy
**
**  PURPOSE:    Application is proxying an incoming call.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This function is invoked when a remote user resource that is
**              associated with this local device is receiving an incoming call
**              from that remote user.  At this time, the local device needs to
**              transform the incoming call into an outgoing call to the final
**              detination as well as keep the incoming call informed of the
**              status (call progress).
***********************************************************************************/
void CeFsmCallIn::fsmProxy( CeResBase &rRes )
{
#if CCTKCFG_REMOTE_USER_CONTROL
   CeResCall *pResCall = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_CALL )
   {
      CeResBase *pProxyCall = NULL;

      pResCall = reinterpret_cast<CeResCall *> ( rRes.resourceAddress() );

      /* Associate the two calls together for further cross tracking of
      ** call progress as well as for proper call information setup (in
      ** particular the media associated with the proxy call is being
      ** extracted from the original call and pass to the MSE of the proxy
      ** call thanks to the association made here).
      */
      if ( CeMain::locateRes( pResCall->resourceUsrProxyEventData(),
                              pProxyCall ) )
      {
         if ( pProxyCall->resourceGender() == CeResBase::eCERESTYPE_CALL )
         {
            CeResCall *pResProxyCall =
               reinterpret_cast<CeResCall *> ( pProxyCall->resourceAddress() );
            pResProxyCall->resourceProxyGuid( pResCall->resourceGuid() );
   
            /* Since this is an incoming call from network, association is
            ** done when the call is picked up.  There can be multiple
            ** remote users, the proxy association is set when the
            ** remote/local user picks up.  So the call is not ended, when
            ** the proxy call for the non-picked remote user ends.
            */
         }
         else
         {
            fsmError( rRes );
            return;
         }
      }

      /* Place the proxy'ed call to the network now.
      */
      ceNotify( pResCall->resourceUsrProxyEventData(),
                eCEEVT_CALL_USR_MAKE,
                NULL );
   }
   else
   {
      fsmError( rRes );
   }
#else
   fsmError( rRes );
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
}
