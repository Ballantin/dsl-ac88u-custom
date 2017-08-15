/**********************************************************************************
** Copyright (c) 2011 Broadcom Corporation
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
**      This file implements the publisher finite state machine class.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <ceFsmPub.h>
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
      CeFsmBase::eCEFSMSTATE_PUB_SET_FINISH -
         CeFsmBase::eCEFSMSTATE_PUB_SET_BEGIN - 1;

static const BOS_UINT32 gFsmNumberOfEvents =
      eCEEVT_NOT_SET_FINISH - eCEEVT_NOT_SET_BEGIN - 1;

static CeFsmBase::CEFSMVOIDFUNCTIONPTR
   gFsmFnc[gFsmNumberOfStates][gFsmNumberOfEvents] =
{
   /* ---- eCEFSMSTATE_PUB_IDLE --------------------------------------------- */
   {
        /* ---- eCEEVT_NOT_NET_OK ----------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_NOT_NET_FAIL --------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_NOT_NET_MSG ---------------- */ &CeFsmPub::fsmIncMsg,
        /* ---- eCEEVT_NOT_USR_PUBLISH ------------ */ &CeFsmPub::fsmPublish,
        /* ---- eCEEVT_NOT_USR_ENDED -------------- */ &CeFsmPub::fsmEnd,
        /* ---- eCEEVT_NOT_USR_RSP ---------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_NOT_USR_MSG ---------------- */ &CeFsmPub::fsmOutMsg,
   },

   /* ---- eCEFSMSTATE_PUB_PUBING ------------------------------------------- */
   {
        /* ---- eCEEVT_NOT_NET_OK ----------------- */ &CeFsmPub::fsmEnd,
        /* ---- eCEEVT_NOT_NET_FAIL --------------- */ &CeFsmPub::fsmEnd,
        /* ---- eCEEVT_NOT_NET_MSG ---------------- */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_NOT_USR_PUBLISH ------------ */ &CeFsmBase::fsmNoOp,
        /* ---- eCEEVT_NOT_USR_ENDED -------------- */ &CeFsmPub::fsmEnd,
        /* ---- eCEEVT_NOT_USR_RSP ---------------- */ &CeFsmPub::fsmRsp,
        /* ---- eCEEVT_NOT_USR_MSG ---------------- */ &CeFsmBase::fsmNoOp,
   },
};

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CeFsmPub::fsmRun
**
**  PURPOSE:    Run the publisher state machine.
**
**  PARAMETERS:
**              rRes - reference to the resource this FSM applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmRun( CeResBase &rRes )
{
   CeFsmBase::CEFSMSTATE eState = rRes.resourceState();
   CEEVT eEvent = rRes.resourceEvent();

   /* Verifies that the state and event are valid for this resource.
   */
   if ( (eState > CeFsmBase::eCEFSMSTATE_PUB_SET_BEGIN) &&
        (eState < CeFsmBase::eCEFSMSTATE_PUB_SET_FINISH) &&
        (eEvent > eCEEVT_NOT_SET_BEGIN) &&
        (eEvent < eCEEVT_NOT_SET_FINISH) )
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
                           - (CeFsmBase::eCEFSMSTATE_PUB_SET_BEGIN + 1);
      uEventIx = static_cast<BOS_UINT32>(eEvent) - (eCEEVT_NOT_SET_BEGIN + 1);

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
**  FUNCTION:   CeFsmPub::fsmEnd
**
**  PURPOSE:    Notification has ended.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmEnd( CeResBase &rRes )
{
   CeResNot *pResNot = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_NOT )
   {
      pResNot = reinterpret_cast<CeResNot *> ( rRes.resourceAddress() );

#if CCTKCFG_MESSAGE_PASS_THROUGH
      /* If the notification was for a pass through message, send an event
      ** to the application to let it know about the final response seen
      ** for that reuqest (notification).
      */
      if ( pResNot->resourceType() == eCCTKPUBLISH_PASSTHRUMSG )
      {
         BOS_UINT32 uRspCode = pResNot->resourceGetFinalRsp();

         tkmEvent ( pResNot->resourceParent(),
                    pResNot->resourceGuid(),
                    eCCTKEVT_MSG_PASS_THRU_RSP,
                    static_cast<void *> ( &uRspCode ) );
      }
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */

      /* Notification terminated.
      */
      pResNot->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_IDLE );

      pResNot->resourceTerminate();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmPub::fsmPublish
**
**  PURPOSE:    Publish a notification due to user command.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmPublish( CeResBase &rRes )
{
   CeResNot *pResNot = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_NOT )
   {
      pResNot = reinterpret_cast<CeResNot *> ( rRes.resourceAddress() );

      switch ( pResNot->resourceType() )
      {
         case eCCTKPUBLISH_CALLMETRICS:
         {
            CeCdr *pCdr;

            pCdr =
                CeMain::ceCdrMgr()->locatePrevCdr( pResNot->resourceParent(),
                                                   pResNot->resourceGetCall() );
            if ( pCdr )
            {
               MXD_GNS::CNameAddr addr;
               MXD_GNS::IUri *pUri;
               mxt_result res;
               MXD_GNS::CNameAddr *pAddr;
               const CFGUSR *pCfgUsr;

               /* Fetch the user name to be included in the Request-URI, To, 
               ** and Contact header.  Per RFC 3903, these headers must be
               ** populated with the address of the resource for which the
               ** user wishes to publish the event state.
               */
               pCfgUsr = CFG_USR( pResNot->resourceParent() );
               if ( pCfgUsr )
               {
                  pAddr = BRCM_NEW( MXD_GNS::CNameAddr )( 
                              (pCfgUsr->in.pSceUsrCfg)->GetUserAddress() );
                  if ( pAddr )
                  {
                     pResNot->resourceNetOodHdl()->SetPeerAddr( *pAddr );
                  }
               }

               /* Retrieve the target address to send the PUBLISH message to
               */
               pUri = cfgCvtAddrToMx( pResNot->resourceGetPeer() );
               addr.SetUri( pUri );
               pResNot->resourceNetOodHdl()->SetTargetAddr( addr );

               /* Create and format the publication data.
               */
               MXD_GNS::CSipMessageBody *pPubBody =
                                             BRCM_NEW( MXD_GNS::CSipMessageBody );
               MXD_GNS::CHeaderList *pPubHdr =
                                             BRCM_NEW( MXD_GNS::CHeaderList );

               if ( pPubBody )
               {
                  MXD_GNS::CBlob *pBlob;
                  MXD_GNS::CSipHeader *pCtTypeHdr;
                  BOS_UINT8 pubData[CCTK_STR_LEN * 16];
                  BOS_UINT32 pubLen;

                  pubLen = pResNot->resourceMetrics( pubData, CCTK_STR_LEN * 16 );
                  pBlob =
                     BRCM_NEW( MXD_GNS::CBlob )(
                        reinterpret_cast<const uint8_t *> (pubData),
                        static_cast<unsigned int> (pubLen),
                        static_cast<unsigned int> (pubLen) );

                  pCtTypeHdr =
                     BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
                  pCtTypeHdr->GetContentTypeMType() = 
                     MXD_GNS::CSipMessageBody::
                                       ms_pszCONTENT_TYPE_APPLICATION_MEDIA_TYPE;
                  pCtTypeHdr->GetContentTypeMSubType() =
                                                   CE_RES_NOT__VQ_RTCP_XR_SUBTYPE;

                  res = pPubBody->AddBody( pBlob, pCtTypeHdr );
                  if ( MX_RIS_S( res ) )
                  {
                     pBlob = NULL;
                     pCtTypeHdr = NULL;
                  }
                  else
                  {
                     BRCM_DELETE( pPubBody );
                     pPubBody = NULL;

                     BRCM_DELETE( pCtTypeHdr );
                     pCtTypeHdr = NULL;
                  }
               }
               
               if ( pPubBody && pPubHdr )
               {
                  MXD_GNS::CSipHeader *pEvtHdr;

                  pEvtHdr =
                     BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_EVENT );
                  pEvtHdr->GetEvent() = CE_RES_NOT__VQ_RTCP_XR_SUBTYPE;

                  pPubHdr->Append( pEvtHdr );
                  pEvtHdr = NULL;

                  /* Append the contact header */
                     if ( pAddr )
                     {
                        MXD_GNS::CSipHeader *pContactHdr;
                        pContactHdr =
                           BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTACT );
                        pContactHdr->GetContact() = *pAddr;

                        pPubHdr->Append( pContactHdr );
                     pContactHdr = NULL;
                  }
               }

               if ( pAddr )
               {
                        BRCM_DELETE( pAddr );
                        pAddr = NULL;
               }
               
               /* Send it.
               */
               if ( pPubBody && pPubHdr )
               {
                  res = pResNot->resourceNetOodHdl()->SendOodRequest(
                                                        MXD_GNS::eMETHODTYPE_PUBLISH,
                                                        0 /* Not used - opaque */,
                                                        pPubHdr,
                                                        pPubBody,
                                                        NULL );
                  /* Note that the above call has taken ownership of the created
                  ** resources.
                  */
                  pPubBody = NULL;
                  pPubHdr = NULL;
                  
                  if ( MX_RIS_S( res ) )
                  {
                     pResNot->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_PUBING );
                  }
                  else
                  {
                     fsmEnd( rRes );
                  }
               }
               else
               {
                  fsmEnd( rRes );
               }
            }
            else
            {
               fsmEnd( rRes );
            }
         }
         break;

         default:
         {
            /* We do not handle this type of publication, terminate the
            ** resource.
            */
            fsmEnd( rRes );
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
**  FUNCTION:   CeFsmPub::fsmRsp
**
**  PURPOSE:    Issue a response to an outstanding pass through request.  The
**              response to this request was dependent on the application.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmRsp( CeResBase &rRes )
{
   CeResNot *pResNot = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_NOT )
   {
      pResNot = reinterpret_cast<CeResNot *> ( rRes.resourceAddress() );

      if ( pResNot->resourceNetEvtCtlHdl() )
      {
         unsigned int uResp = static_cast<unsigned int>
                                         (pResNot->resourceGetFinalRsp()); 

         /* Filter out response code that would require the framework to 
         ** do more work because we do not want to have to deal with this
         ** at this time.  This is a known limitation.
         */
         if ( ( MXD_GNS::MxGetSipStatusClass( static_cast<uint16_t> (uResp) ) ==
                    MXD_GNS::eSIP_STATUS_CLASS_INFORMATIONAL ) ||
               ( MXD_GNS::MxGetSipStatusClass( static_cast<uint16_t> (uResp) ) ==
                    MXD_GNS::eSIP_STATUS_CLASS_REDIRECTION ) )
         {
            /* Default to success final response.
            */
            uResp = MXD_GNS::uOK;
         }

         pResNot->resourceNetEvtCtlHdl()->
                     SendResponse( uResp,
                                   NULL,           /* Reason = use default. */
                                   NULL,           /* No extra headers. */ 
                                   NULL  );        /* No body. */
      }

      /* Notification terminated.
      */
      pResNot->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_IDLE );
      pResNot->resourceTerminate();
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmPub::fsmOutMsg
**
**  PURPOSE:    Issue a pass through request as a message with the body containing
**              the xml data passed by the application.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmOutMsg( CeResBase &rRes )
{
   CeResNot *pResNot = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_NOT )
   {
      pResNot = reinterpret_cast<CeResNot *> ( rRes.resourceAddress() );

      MXD_GNS::CNameAddr addr;
      MXD_GNS::IUri *pUri;
      mxt_result res;

      pUri = cfgCvtAddrToMx( pResNot->resourceGetPeer() );
      addr.SetUri( pUri );
      pResNot->resourceNetOodHdl()->SetPeerAddr( addr );

      /* Create and format the message data.
      */
      MXD_GNS::CSipMessageBody *pPubBody =
                                    BRCM_NEW( MXD_GNS::CSipMessageBody );
      MXD_GNS::CHeaderList *pPubHdr =
                                    BRCM_NEW( MXD_GNS::CHeaderList );

      if ( pPubBody )
      {
         MXD_GNS::CBlob *pBlob;
         MXD_GNS::CSipHeader *pCtTypeHdr;
         BOS_UINT32 pubLen;

         pubLen = pResNot->resourceGetSize();
         pBlob =
            BRCM_NEW( MXD_GNS::CBlob )(
               reinterpret_cast<const uint8_t *> (pResNot->resourceGetBody()),
               static_cast<unsigned int> (pubLen),
               static_cast<unsigned int> (pubLen) );

         pCtTypeHdr =
            BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
         pCtTypeHdr->GetContentTypeMType() = 
            MXD_GNS::CSipMessageBody::
                              ms_pszCONTENT_TYPE_TEXT_MEDIA_TYPE;
         pCtTypeHdr->GetContentTypeMSubType() =
                                          CE_RES_NOT__XML_SMML_SUBTYPE;

         res = pPubBody->AddBody( pBlob, pCtTypeHdr );
         if ( MX_RIS_S( res ) )
         {
            pBlob = NULL;
            pCtTypeHdr = NULL;
         }
         else
         {
            BRCM_DELETE( pPubBody );
            pPubBody = NULL;
         }
      }
     
      /* Send it.
      */
      if ( pPubBody && pPubHdr )
      {
         res = pResNot->resourceNetOodHdl()->SendOodRequest(
                                               MXD_GNS::eMETHODTYPE_MESSAGE,
                                               0 /* Not used - opaque */,
                                               pPubHdr,
                                               pPubBody,
                                               NULL );
         /* Note that the above call has taken ownership of the created
         ** resources.
         */
         pPubBody = NULL;
         pPubHdr = NULL;
                  
         if ( MX_RIS_S( res ) )
         {
            pResNot->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_PUBING );
         }
         else
         {
            fsmEnd( rRes );
         }
      }
      else
      {
         fsmEnd( rRes );
      }
   }
   else
   {
      fsmError( rRes );
   }
}


/***********************************************************************************
**  FUNCTION:   CeFsmPub::fsmIncMsg
**
**  PURPOSE:    Receives a pass through request as a message with the body
**              containing the xml data to be passed by the application.
**
**  PARAMETERS:
**              rRes - reference to the resource this function applies to.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void CeFsmPub::fsmIncMsg( CeResBase &rRes )
{
   CeResNot *pResNot = NULL;

   if ( rRes.resourceGender() == CeResBase::eCERESTYPE_NOT )
   {
      CCTKMSGPASSTHRU data;

      pResNot = reinterpret_cast<CeResNot *> ( rRes.resourceAddress() );

#if CCTKCFG_MESSAGE_PASS_THROUGH
      /* Add a reference for the server control event because the final response
      ** for this request will be issued by the application asynchronously.
      */
      pResNot->resourceNetEvtCtlHdl()->AddIfRef();
   
      /* If need be, we could issue a provisional response for this request
      ** at this time, however it is not currently done.  Note that the CMGR
      ** (and upper level application in general) is not allowed to send such
      ** provisional response per the framework implementation, so it would
      ** have to be done here if needed.
      */
      pResNot->resourcePushState( CeFsmBase::eCEFSMSTATE_PUB_PUBING );

      data.uSize = pResNot->resourceGetSize();
      data.puData = pResNot->resourceGetBody();

      /* Pass the message to the application for further processing.
      */
      tkmEvent ( pResNot->resourceParent(),
                 pResNot->resourceGuid(),
                 eCCTKEVT_MSG_PASS_THRU,
                 static_cast<void *> ( &data ) );
#else
      MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( data );

      /* Reject the message.
      */
      pResNot->resourceNetEvtCtlHdl()->
                  SendResponse( MXD_GNS::uSERVICE_UNAVAILABLE,
                                NULL,           /* Reason = use default. */
                                NULL,           /* No extra headers. */ 
                                NULL  );        /* No body. */

#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
   }
   else
   {
      fsmError( rRes );
   }
}
