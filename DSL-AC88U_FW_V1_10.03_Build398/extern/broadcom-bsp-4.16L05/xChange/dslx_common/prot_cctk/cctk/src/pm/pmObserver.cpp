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
**      This file implements the Payload Modifier Observer main.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <pmObserver.h>

#include <pmApi.h>


/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define PM__SIGNALING_AUTHORIZATION_HEADER         "Authorization"
#define PM__SIGNALING_PROXY_AUTHORIZATION_HEADER   "Proxy-Authorization"
#define PM__SIGNALING_PRIORITY_HEADER              "Priority"
#define PM__SIGNALING_ACCEPT_HEADER                "Accept"
#define PM__SIGNALING_ACCEPT_LANGUAGE_HEADER       "Accept-Language"
#define PM__SIGNALING_REQUIRE_HEADER               "Require"
#define PM__SIGNALING_GEOLOCATION_HEADER           "Geolocation"
#define PM__SIGNALING_CONTENTID_HEADER             "Content-ID"
#define PM__SIGNALING_SUPPORTED_HEADER             "Supported"
#define PM__SIGNALING_ACCESS_NETWORK_INFO_HEADER   "P-Access-Network-Info"

#define PM__SIGNALING_REGISTRATION_IDENTIFICATION  "reg-id"
#define PM__SIGNALING_TAG_PARAMETER                "tag"
#define PM__SIGNALING_EMERGENCY_PARAMETER          "emergency"
#define PM__SIGNALING_LANGUAGE_ENGLISH             "en"

#define PM__SIGNALING_PROF_TYPE_PARAMETER          "profile-type"
#define PM__SIGNALING_AUID_TYPE_PARAMETER          "auid"

#define PM__SIGNALING_APPLICATION_TYPE             "application"
#define PM__SIGNALING_PIDF_XML_SUBTYPE             "pidf+xml"
#define PM__SIGNALING_MSG_SUM_SUBTYPE              "simple-message-summary"
#define PM__SIGNALING_XML_SUBTYPE                  "xml"

#define PM__SIGNALING_SUB_STATE_ACTIVE             "active"

#define PM__SIGNALING_CONTENT_TYPE_MESSAGE         "message"
#define PM__SIGNALING_CONTENT_TYPE_SUB_SIPFRAG     "sipfrag"

#define PM__SIGNALING_P_EARLY_MEDIA_SUPPORTED      "supported"

#define PM__SIGNALING_X_RTP_STAT                   "X-RTP-Stat"

#define PM_PKT_BUFFER_SIZE                         2048
#define PM_IP_ADDRESS_STRING_LEN                   45
#define PM_XRTPSTAT_LEN                            (CCTK_STR_LEN * 4)

/* ---- Private Variables ------------------------------------------------------- */
PmObserver *gpPmObserver = NULL;
const BOS_UINT8 *PmObserver::puPktBuffer = NULL;
BOS_BOOL PmObserver::mbRteHdrOverwriteFqdn = BOS_FALSE;

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/* ---- "C" API (START OF) ------------------------------------------------------ */
#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************************
**  FUNCTION:   pmInit
**
**  PURPOSE:    External API to initialize (and launch) the Payload Modifier module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This API must be invoked prior to any further Payload Modifier work
**              is possible.
***********************************************************************************/
void pmInit ( void )
{
   if ( gpPmObserver == NULL )
   {
      BOS_BOOL bRteHdrOverwriteFqdn = 
                     UTIL_BMP_IS_ENABLED_ALL(
                              CFG_SYS->xtl.net.prot.gen.sigBehave.uTx,
                              CCTK_SIGBEHVTX_NOFQDNINRTE );

      if (( gpPmObserver = BRCM_NEW( PmObserver )( bRteHdrOverwriteFqdn ) ) == NULL )
      {
         /* Failed... we are doomed.
         */
         CCTK_TRACEI0( "pmInit - Failure to initialize properly!" );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   pmDeInit
**
**  PURPOSE:    External API to de-initialize the Payload Modifier module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      After this API has been invoked, there is no further processing
**              possible from the Payload Modifier.
***********************************************************************************/
void pmDeInit ( void )
{
   if ( gpPmObserver != NULL )
   {
      BRCM_DELETE( gpPmObserver );
      gpPmObserver = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   pmGetPktObs
**
**  PURPOSE:    External API to retrieve the payload modifier observer.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    The pointer to the payload modifier observer object.
**
**  NOTES:      .
***********************************************************************************/
void *pmGetPktObs( void )
{
   return (void *)gpPmObserver;
}


/***********************************************************************************
**  FUNCTION:   pmLastChance
**
**  PURPOSE:    External API to pass the payload to be modified to the observer
**              at the last minute prior to actually sending out the packet, hence
**              the 'last chance' denomination.
**
**  PARAMETERS:
**              ppNewData - the new data information to use as replacement of the
**                          current data.  This pointer must be assigned to some
**                          valid value or the replacement data will be ignored.
**              destInfo - the destination information associated with this
**                         data, this is the resolved address of the target.
**              pData    - the data currently being sent, if ppNewData does not
**                         reallocate some new data, the pData data is the one
**                         used.
**              uSize    - the size of the current data (pData).
**
**  RETURNS:    The size of the new data (ppNewData) to be consider for sending
**              or zero if no data is reallocated.
**
**  NOTES:      The newly reallocated data will live untouched until the next
**              call to pmLastChance is placed during which the data may be
**              replaced.  Therefore the information returned by this call is
**              not meant to be cached by the observer.  If caching must take
**              place, it should take place at the layer calling this API.
***********************************************************************************/
BOS_UINT32 pmLastChance( BOS_UINT8 **ppNewData,
                         CCTKIPPORT *destInfo,
                         const BOS_UINT8 *pData,
                         BOS_UINT32 uSize )
{
   if ( gpPmObserver != NULL )
   {
      return gpPmObserver->lastChance( ppNewData,
                                       destInfo,
                                       pData,
                                       uSize );
   }
   else
   {
      *ppNewData = NULL;
      return 0;
   }
}

#ifdef __cplusplus
}
#endif
/* ---- "C" API (END OF) -------------------------------------------------------- */


/***********************************************************************************
**  FUNCTION:   PmObserver::PmObserver
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              bRteHdrOverwriteFqdn - static configuration for the 'Route header
**                                     overwrite FQDN' feature.
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
PmObserver::PmObserver( BOS_BOOL bRteHdrOverwriteFqdn )
{
   puPktBuffer = BRCM_NEW_ARRAY( BOS_UINT8, PM_PKT_BUFFER_SIZE );
   mbRteHdrOverwriteFqdn = bRteHdrOverwriteFqdn;
}


/***********************************************************************************
**  FUNCTION:   PmObserver::~PmObserver
**
**  PURPOSE:    Class desctructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
PmObserver::~PmObserver()
{
   if ( puPktBuffer )
   {
      BRCM_DELETE_ARRAY( puPktBuffer );
      puPktBuffer = NULL;
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::insertGenHdr
**
**  PURPOSE:    Insert generic header to outgoing SIP packet.
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void PmObserver::insertGenHdr( BOS_UINT32 uGuid,
                               MXD_GNS::CSipPacket &rPkt )
{
   CCTK_TRACED3( "PmObserver(%p) - insertGenHdr(0x%lX, %p)", this, uGuid, &rPkt );

   const CCTKGENHDRCFG *pHdrCfg = NULL;

   CFGTYPE eCfgType = cfgGetType( uGuid );
   switch ( eCfgType )
   {
   case eCFGTYPE_USR:
      {
         pHdrCfg = &( CFG_USR( uGuid )->xtl.usr.loc.genHdr );
      }
      break;
   case eCFGTYPE_CALL:
      {
         pHdrCfg = CFG_CALL( uGuid )->xtl.usr.pGenHdr;
      }
      break;
   case eCFGTYPE_EVT:
      {
         pHdrCfg = CFG_EVT( uGuid )->xtl.usr.pGenHdr;
      }
      break;
   default:
      {
         CCTK_TRACEI1( "PmObserver - insertGenHdr: Invalid GUID type (%d)",
                       eCfgType );
         return;
      }
      break;
   }

   CCTK_ASSERT( pHdrCfg != NULL );

   BOS_BOOL bRequest;
   BOS_UINT32 uPktMethod;
   MXD_GNS::ESipMethod ePktMethod;
   MXD_GNS::CSipHeader *pHdr = NULL;
   mxt_result res;

   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < pHdrCfg->uNum; uIdx++ )
   {
      /* Check for packet direction */
      bRequest = rPkt.IsRequest();
      if ( ( bRequest && pHdrCfg->hdr[uIdx].eMap == eCCTKHDRMAP_RSP ) ||
           ( !bRequest && pHdrCfg->hdr[uIdx].eMap == eCCTKHDRMAP_REQ ) )
      {
         continue;
      }

      /* Check for packet method.
      ** NOTE: We assume CSeq will always exist in outgoing SIP packet.
      */
      ePktMethod = MxConvertSipMethod(
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CSEQ )
            ->GetCSeqMethod().GetString() );
      uPktMethod = cfgCvtMethodFromMx( ePktMethod );
      if ( !UTIL_BMP_IS_ENABLED( pHdrCfg->hdr[uIdx].uMethod, uPktMethod ) )
      {
         continue;
      }

      /* Check for insertion policy */
      switch ( pHdrCfg->hdr[uIdx].eMode )
      {
      case eCCTKHDRMODE_APPEND:
         {
            /* No need for removing any headers */
         }
         break;
      case eCCTKHDRMODE_OVRWRITE:
      case eCCTKHDRMODE_ONCE:
         {
            MXD_GNS::CString *pStrHdrName =
               cfgCvtHdrNameToMx( pHdrCfg->hdr[uIdx].hdr );
            CCTK_ASSERT( pStrHdrName != NULL );
            if ( pStrHdrName == NULL )
            {
               CCTK_TRACEI1(
                  "PmObserver - insertGenHdr: Invalid header name (%lu)", uIdx );
               continue;
            }

            pHdr = rPkt.GetHeaderList().Get( *pStrHdrName );
            if ( pHdr != NULL )
            {
               if ( pHdrCfg->hdr[uIdx].eMode == eCCTKHDRMODE_ONCE )
               {
                  BRCM_DELETE( pStrHdrName );
                  pStrHdrName = NULL;
                  continue;
               }

               /* NOTE: we don't care about the return status */
               rPkt.GetHeaderList().RemoveHeaderType( *pStrHdrName );
            }

            BRCM_DELETE( pStrHdrName );
         }
         break;
      default:
         {
            CCTK_TRACEI1( "PmObserver - insertGenHdr: Invalid policy (%d)",
                          pHdrCfg->hdr[uIdx].eMode );
            continue;
         }
         break;
      }

      /* Create generic header */
      pHdr = cfgCvtHdrToMx( pHdrCfg->hdr[uIdx].hdr );
      if ( pHdr == NULL )
      {
         CCTK_TRACEI1(
            "PmObserver - insertGenHdr: Error converting header (%lu)", uIdx );
         continue;
      }

      /* Insert the header to the packet */
      res = rPkt.GetHeaderList().Append( pHdr );
      pHdr = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "PmObserver - insertGenHdr: "
            "Error inserting header to packet (%lu, %d).",
            uIdx,
            res );
         continue;
      }
   }
}

#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
/***********************************************************************************
**  FUNCTION:   PmObserver::updatePktIms
**
**  PURPOSE:    Update outgoing SIP packet to be IMS-compliant.
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void PmObserver::updatePktIms( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt )
{
   CCTK_TRACED3( "PmObserver(%p) - updatePktIms(0x%lX, %p)", this, uGuid, &rPkt );
   mxt_result res;

   /* If a Proxy-Authorization header was inserted by one of our service, remove
   ** it from any outgoing REGISTER request.
   */
   if ( (cfgGetType( uGuid ) == eCFGTYPE_USR) &&
        rPkt.IsRequest() &&
        (rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_REGISTER) )
   {
      MXD_GNS::CSipHeader *pProxyAuth =
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_PROXY_AUTHORIZATION, res );
      if ( MX_RIS_S( res ) && ( pProxyAuth != NULL ) )
      {
         rPkt.GetHeaderList().RemoveHeaderType( MXD_GNS::eHDR_PROXY_AUTHORIZATION );
      }
   }

   /* If a Proxy-Authorization header was inserted remove it from any outgoing ACK requests
   */
   if ( rPkt.IsRequest() 
        && (rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_ACK))
   {
     MXD_GNS::CSipHeader *pProxyAuth =
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_PROXY_AUTHORIZATION, res );
      if ( MX_RIS_S( res ) && ( pProxyAuth != NULL ) )
      {
         rPkt.GetHeaderList().RemoveHeaderType( MXD_GNS::eHDR_PROXY_AUTHORIZATION );
      }
   }
   
   if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                 CCTK_INTEROP_NO_ROUTE_HEADER_IN_REQUESTS ) )
   {
      /* Forcibly remove route header from all requests */
      if ( rPkt.IsRequest() )
      {
         MXD_GNS::CSipHeader *pRte =
            rPkt.GetHeaderList().Get( MXD_GNS::eHDR_ROUTE, res );
         /* Remove the Route header if present.
          */
         if ( MX_RIS_S( res ) && ( pRte != NULL ) )
         {
            rPkt.GetHeaderList().RemoveHeaderType( MXD_GNS::eHDR_ROUTE );
         }
      }
   }
   

   if ( (cfgGetType( uGuid ) == eCFGTYPE_USR) &&
        rPkt.IsRequest() &&
        (rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_SUBSCRIBE) )
   {
      MXD_GNS::CSipHeader *pAuth =
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_AUTHORIZATION, res );
      if ( MX_RIS_S( res ) && ( pAuth != NULL ) )
      {
         /* Set the Authorization header */
         CCTKHDR authHdr;
         MXD_GNS::CBlob blob;

         pAuth->Serialize( blob );

         MXD_GNS::CString newName( PM__SIGNALING_PROXY_AUTHORIZATION_HEADER );
         MXD_GNS::CString newValue( reinterpret_cast<const char*> (blob.GetFirstIndexPtr()),
                                    blob.GetSize() );

         rPkt.GetHeaderList().RemoveHeader( MXD_GNS::eHDR_AUTHORIZATION, 0 );

         /* Intentionally set the header as private so we can make use of
         ** the header conversion routine.
         */
         authHdr.eType = eCCTKHDRTYPE_PRIVATE;
         authHdr.pcName =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
         authHdr.pcValue =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );
         /* Create the Proxy-Authorization header */
         MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( authHdr );
         if ( pSipHeader == NULL )
         {
            CCTK_TRACEI0( "PmObserver - updatePktIms: "
               "Error creating the Proxy-Authorization header." );
            return;
         }
         /* Insert the header into the packet */
         res = rPkt.GetHeaderList().Append( pSipHeader );
         pSipHeader = NULL;
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI1( "PmObserver - updatePktIms: "
               "Error inserting Proxy-Authorization header to packet (%d).",
               res );
            return;
         }
      }
   }

   if ( !CFG_SYS->xtl.net.prot.gen.imsCfg.bNoAuthHdrReg &&
        cfgGetType( uGuid ) == eCFGTYPE_USR &&
        rPkt.IsRequest() )
   {
      if ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_REGISTER )
      {
         MXD_GNS::ISceCoreConfig *pSceCfg = NULL;

         res = MXD_GNS::CreateEComInstance( MXD_GNS::CLSID_CSceCoreConfig,
                                            NULL,
                                            &pSceCfg );
         if ( MX_RIS_S( res ) && pSceCfg )
         {
            if ( UTIL_BMP_IS_ENABLED( pSceCfg->GetSupportedExtensions(),
                                      MXD_GNS::eOPTIONTAG_OUTBOUND ) )
            {
               MXD_GNS::CSipHeader *pCt =
                        rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CONTACT, res );
               /* If there is a Contact header and if we are supporting 'outbound', we
               ** need to insert a reg-id parameter (draft-ietf-sip-outbound).
               */
               if ( pCt && MX_RIS_S(res) )
               {
                  MXD_GNS::CString regId;

                  regId.Format( 0, "%d", ceGetRegId( uGuid ) );
                  do
                  {
                     if ( pCt->GetParam(
                           PM__SIGNALING_REGISTRATION_IDENTIFICATION ) == NULL )
                     {
                        pCt->SetParam( PM__SIGNALING_REGISTRATION_IDENTIFICATION,
                                       regId );
                     }

                     pCt = pCt->GetNextHeader();
                  }
                  while ( pCt );
               }
            }
            pSceCfg->ReleaseIfRef();
            pSceCfg = NULL;
         }

         if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.sigBehave.uTx,
                                       CCTK_SIGBEHVTX_NOSVCRTEREG ) )
         {
            MXD_GNS::CSipHeader *pRte =
               rPkt.GetHeaderList().Get( MXD_GNS::eHDR_ROUTE, res );
            /* If there is a Route header present in the request, we need to
            ** replace it with the default preloaded route information that
            ** we have configured.
            */
            if ( MX_RIS_S( res ) && ( pRte != NULL ) )
            {
               const CFGUSR *pCfgUsr = CFG_USR( uGuid );
               CCTK_ASSERT( pCfgUsr != NULL );
               if ( pCfgUsr == NULL )
               {
                  CCTK_TRACEI0( "PmObserver - updatePktIms: "
                     "Error retrieving user configuration." );
                  return;
               }

               /* Remove the current header.
               */
               rPkt.GetHeaderList().RemoveHeaderType( MXD_GNS::eHDR_ROUTE );

               /* Create the new Route header.
               */
               MXD_GNS::CSipHeader *pRouteSet =
                  cfgCvtRouteLstToMx( pCfgUsr->xtl.usr.net.obProxyLst );

               /* Insert the header into the packet.
               */
               res = rPkt.GetHeaderList().Append( pRouteSet );
               pRouteSet = NULL;
               if ( MX_RIS_F( res ) )
               {
                  CCTK_TRACEI1( "PmObserver - updatePktIms: "
                     "Error inserting Route header to packet (%d).",
                     res );
                  return;
               }
            }
         }

         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_AUTHORIZATION, res );
         /* If there is no Authorization header, we assume this is the initial
         ** registration attempt.  Need to include a 'dummy' one.
         */
         if ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND )
         {
            const CFGUSR *pCfgUsr = CFG_USR( uGuid );
            CCTK_ASSERT( pCfgUsr != NULL );
            if ( pCfgUsr == NULL )
            {
               CCTK_TRACEI0( "PmObserver - updatePktIms: "
                  "Error retrieving user configuration." );
               return;
            }

            /* Set the Authorization header */
            CCTKHDR authHdr;
            MXD_GNS::CString newName( PM__SIGNALING_AUTHORIZATION_HEADER );
            MXD_GNS::CString newValue;

            newValue += "Digest";
            newValue += " username=\"";
            newValue += reinterpret_cast<const char *>(
                           pCfgUsr->xtl.usr.sec.auth.client.pcAuthName );
            newValue += "\", realm=\"";
            newValue += reinterpret_cast<const char *>(
                           pCfgUsr->xtl.reg.gen.registrar.hostPort.pcAddr );
            newValue += "\", uri=\"sip:";
            newValue += reinterpret_cast<const char *>(
                           pCfgUsr->xtl.reg.gen.registrar.hostPort.pcAddr );
            newValue += "\", nonce=\"\", response=\"\", algorithm=MD5";

            /* Intentionally set the header as private so we can make use of
            ** the header conversion routine.
            */
            authHdr.eType = eCCTKHDRTYPE_PRIVATE;
            authHdr.pcName =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
            authHdr.pcValue =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

            /* Create the Authorization header */
            MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( authHdr );
            if ( pSipHeader == NULL )
            {
               CCTK_TRACEI0( "PmObserver - updatePktIms: "
                  "Error creating the Authorization header." );
               return;
            }

            /* Insert the header into the packet */
            res = rPkt.GetHeaderList().Append( pSipHeader );
            pSipHeader = NULL;
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "PmObserver - updatePktIms: "
                  "Error inserting Authorization header to packet (%d).",
                  res );
               return;
            }
         }
      }
   }

   /* If this is the request for a subscription to the User-Agent Profile
   ** event package, we need to add some custom parameters.
   */
   if ( cfgGetType( uGuid ) == eCFGTYPE_EVT &&
        rPkt.IsRequest() )
   {
      if ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_SUBSCRIBE )
      {
         MXD_GNS::CString paramFormat;

         MXD_GNS::CSipHeader *pEvt =
                  rPkt.GetHeaderList().Get( MXD_GNS::eHDR_EVENT, res );

         if ( MX_RIS_S( res ) && ( pEvt != NULL ) )
         {
            if ( !strCmpNoCase( pEvt->GetEvent().GetString().CStr(),
                                PM__SIGNALING_PROF_TYPE_EVENT ) )
            {
               if ( pEvt->GetParam(
                     PM__SIGNALING_PROF_TYPE_PARAMETER ) == NULL )
               {
                     paramFormat.EraseAll();
                     paramFormat.Append( PM__SIGNALING_APPLICATION_TYPE,
                                         strlen( PM__SIGNALING_APPLICATION_TYPE ) );
                     pEvt->SetParam( PM__SIGNALING_PROF_TYPE_PARAMETER,
                                     paramFormat );
               }

               if ( pEvt->GetParam(
                     PM__SIGNALING_AUID_TYPE_PARAMETER ) == NULL )
               {
                  BOS_UINT32 uUsrGuid;
                  BOS_UINT32 uIx;

                  ceGetProfInfo( uGuid, &uUsrGuid, &uIx );

                  const CFGUSR *pCfgUsr = CFG_USR( uUsrGuid );

                  if ( pCfgUsr && (uIx != guCeInvalidGuid) &&
                        pCfgUsr->xtl.usr.net.uapEvt.pcProfile[uIx] )
                  {
                     paramFormat.EraseAll();
                     paramFormat.Append(
                        reinterpret_cast<const char *> (
                           pCfgUsr->xtl.usr.net.uapEvt.pcProfile[uIx] ),
                        strlen( reinterpret_cast<const char *> (
                              pCfgUsr->xtl.usr.net.uapEvt.pcProfile[uIx] ) ));
                     pEvt->SetParam( PM__SIGNALING_AUID_TYPE_PARAMETER,
                                     paramFormat );
                  }
               }
            }
         }
      }
   }

   /* If this is an INVITE, UPDATE or PRACK request, indicate that one can support
   ** and understand the P-Early-Media (RFC 5009) header indication if configured
   ** to do so.
   */
   if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                 CCTK_INTEROP_EARLY_MEDIA_HDR ) )
   {
      if ( cfgGetType( uGuid ) == eCFGTYPE_CALL &&
           rPkt.IsRequest() &&
           ( ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_INVITE ) ||
             ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_PRACK ) ||
             ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_UPDATE ) ) )
      {
         /* Set the P-Early-Media header */
         CCTKHDR pEarlMedHdr;
         MXD_GNS::CString newName( PM__SIGNALING_P_EARLY_MEDIA );
         MXD_GNS::CString newValue( PM__SIGNALING_P_EARLY_MEDIA_SUPPORTED );

         rPkt.GetHeaderList().Get( newName, res );

         if ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND )
         {
            /* Intentionally set the header as private so we can make use of
            ** the header conversion routine.
            */
            pEarlMedHdr.eType = eCCTKHDRTYPE_PRIVATE;
            pEarlMedHdr.pcName =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
            pEarlMedHdr.pcValue =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

            /* Create the P-Early-Media header */
            MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( pEarlMedHdr );
            if ( pSipHeader == NULL )
            {
               CCTK_TRACEI0( "PmObserver - updatePktIms: "
                  "Error creating the P-Early-Media header." );
               return;
            }

            /* Insert the header into the packet */
            res = rPkt.GetHeaderList().Append( pSipHeader );
            pSipHeader = NULL;
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "PmObserver - updatePktIms: "
                  "Error inserting P-Early-Media header to packet (%d).",
                  res );
               return;
            }
         }
      }
   }

   /* If this is a response ('180' provisional or '200' final) to an INVITE
   ** transaction and we are requiring the usage of preconditions, then we need
   ** to say so.
   */
   if ( cfgGetType( uGuid ) == eCFGTYPE_CALL &&
        rPkt.IsResponse() &&
        ( ( rPkt.GetStatusLine()->GetCode() == MXD_GNS::uRINGING ) ||
          ( rPkt.GetStatusLine()->GetCode() == MXD_GNS::uOK ) ) )
   {
      MXD_GNS::CSipHeader *pCSeq =
               rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CSEQ, res );

      if ( MX_RIS_S( res ) && ( pCSeq != NULL ) )
      {
         if ( !strCmpNoCase(
                  pCSeq->GetCSeqMethod().GetString().CStr(),
                  MXD_GNS::g_aszMETHOD_NAME[MXD_GNS::eSIP_METHOD_INVITE] ) &&
               ceGetPrecond( uGuid ) )
         {
            CCTKHDR requireHdr;
            MXD_GNS::CString newName( PM__SIGNALING_REQUIRE_HEADER );
            MXD_GNS::CString newValue;

            newValue += MXD_GNS::pszOPTIONTAG_PRECONDITION;

            /* Intentionally set the header as private so we can make use of
            ** the header conversion routine.
            */
            requireHdr.eType = eCCTKHDRTYPE_PRIVATE;
            requireHdr.pcName =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
            requireHdr.pcValue =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

            /* Create the Require header */
            MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( requireHdr );
            if ( pSipHeader == NULL )
            {
               CCTK_TRACEI0( "PmObserver - updatePktIms: "
                  "Error creating the Require header." );
               return;
            }

            res = rPkt.GetHeaderList().Append( pSipHeader );
            pSipHeader = NULL;
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "PmObserver - updatePktIms: "
                  "Error inserting Require header to packet (%d).",
                  res );
               return;
            }
         }
      }
   }
 
   /* If there is an accept header determine whether the it needs to be removed */
   MXD_GNS::CSipHeader *pAccept =
               rPkt.GetHeaderList().Get( MXD_GNS::eHDR_ACCEPT, res );
   
   if ( MX_RIS_S( res ) && ( pAccept != NULL ) )
   {
      /* If the Accept header is present, remove or update it according to the following:
      ** Msg\Action | Remove        |   Update   | Leave as is
      ** ------------------------------------------------------
      ** OPTIONS    |   -           |     -      |  yes
      ** INVITE     | if configured |     -      |  if configured
      ** SUB - UA   | yes           |    yes     |
      ** 415 resp   |   -           |     -      |  yes
      ** all other  | yes           |     -      |   -
      */
      
      MXD_GNS::CSipHeader *pCSeq =
              rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CSEQ, res );
      BOS_BOOL bRemAcptHdr = BOS_TRUE;
      if ( MX_RIS_S( res ) && ( pCSeq != NULL ) )
      {
         /* If this is an OPTIONS request/response, or a 415 response the header must
         ** not be removed
         */
         if ( !strCmpNoCase(
                  pCSeq->GetCSeqMethod().GetString().CStr(),
                  MXD_GNS::g_aszMETHOD_NAME[MXD_GNS::eSIP_METHOD_OPTIONS] ) ||
                     ( rPkt.IsResponse() &&
                     ( rPkt.GetStatusLine()->GetCode() == MXD_GNS::uUNSUPPORTED_MEDIA_TYPE )))
         {
            bRemAcptHdr = BOS_FALSE;
         }
         /* If this is an INVITE request, check if the configuration requires removal 
         ** of the header.
         */
         else if ( rPkt.IsRequest() &&
                  ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_INVITE ) )
         {      
            /* The default is not to remove the accept headr on INVITE requests. */
            bRemAcptHdr = BOS_FALSE;
            
            const CFGCALL *pCfgCall = CFG_CALL( uGuid );
            CCTK_ASSERT( pCfgCall != NULL );
            if ( pCfgCall == NULL )
            {
               CCTK_TRACEI0( "PmObserver - UpdatePktIms: "
                             "Error retrieving call configuration." );
               return;
            }

            /* Check to see if configuration requires the accept header to be removed */
            if ( ( pCfgCall->xtl.basic.gen.emgCallCfg.bEmergency &&
                     ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                                CCTK_INTEROP_EMGNOACCEPT ) )) ||
                        ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                                   CCTK_INTEROP_INVNOACCEPT ) ))
            {
               
               bRemAcptHdr = BOS_TRUE;                  
            }
         }
         
         /* If decided to do so remove the accept header from the message. */
         if ( bRemAcptHdr )
         {
            rPkt.GetHeaderList().RemoveHeaderType( MXD_GNS::eHDR_ACCEPT );
            
            /* If this is a SUBSCRIBE request for the UA Profile event, we need to update
            ** the header value to jusy "application/xml".
            */
            if ( cfgGetType( uGuid ) == eCFGTYPE_EVT &&
                 rPkt.IsRequest() && 
                 ( rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_SUBSCRIBE ))
            {

               MXD_GNS::CSipHeader *pEvt =
                        rPkt.GetHeaderList().Get( MXD_GNS::eHDR_EVENT, res );

               if ( MX_RIS_S( res ) && ( pEvt != NULL ) )
               {
                  if ( !strCmpNoCase( pEvt->GetEvent().GetString().CStr(),
                                      PM__SIGNALING_PROF_TYPE_EVENT ) )
                  {
                  
                     /* Set the Accept header */
                     CCTKHDR acceptHdr;
                     MXD_GNS::CString newName( PM__SIGNALING_ACCEPT_HEADER );
                     MXD_GNS::CString newValue (PM__SIGNALING_APPLICATION_TYPE);

                     newValue += "/";
                     newValue += PM__SIGNALING_XML_SUBTYPE;

                     /* Intentionally set the header as private so we can make use of
                     ** the header conversion routine.
                     */
                     acceptHdr.eType = eCCTKHDRTYPE_PRIVATE;
                     acceptHdr.pcName =
                        const_cast< BOS_UINT8* >(
                           reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
                     acceptHdr.pcValue =
                        const_cast< BOS_UINT8* >(
                           reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

                     /* Create the Accept header */
                     MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( acceptHdr );
                     if ( pSipHeader == NULL )
                     {
                        CCTK_TRACEI0( "PmObserver - updatePktIms: "
                           "Error creating the Accept header." );
                        return;
                     }

                     /* Insert the header into the packet */
                     res = rPkt.GetHeaderList().Append( pSipHeader );
                     pSipHeader = NULL;
                     if ( MX_RIS_F( res ) )
                     {
                        CCTK_TRACEI1( "PmObserver - updatePktIms: "
                           "Error inserting Accept header to packet (%d).",
                           res );
                        return;
                     }
                  }
               }
            }
         }
      }
   }

   /* If this is a BYE request or 200 OK response for BYE, include X-RTP-Stat header
   ** if configured to do so.
   */
   if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                 CCTK_INTEROP_XRTPSTAT_IN_CALLEND ) )
   {
      if ( cfgGetType( uGuid ) == eCFGTYPE_CALL )
      {
         MXD_GNS::CSipHeader *pCSeq =
                  rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CSEQ, res );

         if ( MX_RIS_S( res ) && ( pCSeq != NULL ) )
         {
            if ( !strCmpNoCase(
                     pCSeq->GetCSeqMethod().GetString().CStr(),
                     MXD_GNS::g_aszMETHOD_NAME[MXD_GNS::eSIP_METHOD_BYE] ) &&
                 (   rPkt.IsRequest() || 
                   ( rPkt.IsResponse() && ( rPkt.GetStatusLine()->GetCode() == MXD_GNS::uOK ) ) )
               )
            {
               /* Set the X-RTP-Stat header */
               CCTKHDR pXRTPStatHdr;

               /* retrieve the call metrics associated with this call. */
               CCTKXRTPSTAT statInfo;

               if (ceGetCallXRtpStat( uGuid, &statInfo ))
               {

                 BOS_UINT8 puData[PM_XRTPSTAT_LEN];
                 memset( reinterpret_cast<void *> (puData),
                         0,
                         PM_XRTPSTAT_LEN );
                 BOS_UINT8 ipAddrLcl[CCTK_STR_LEN];
                 BOS_UINT8 ipAddrRem[CCTK_STR_LEN];
                 bosIpAddrGetStr ( &statInfo.ipLcl,
                                   reinterpret_cast<char *> (ipAddrLcl),
                                   CCTK_STR_LEN );
                 bosIpAddrGetStr ( &statInfo.ipRem,
                                   reinterpret_cast<char *> (ipAddrRem),
                                   CCTK_STR_LEN );
        
                 snprintf( reinterpret_cast<char *> (&puData[strlen(
                             reinterpret_cast<const char *> (puData))]),
                           PM_XRTPSTAT_LEN,
                           "CS=%u;PS=%lu;ES=%u;OS=%lu;"\
                           "SP=%lu/%lu;SO=%lu;QS=%lu;"\
                           "PR=%lu;ER=%u;OR=%lu;CR=%lu;"\
                           "SR=%lu;QR=%lu;PL=%ld,%ld;"\
                           "BL=%u;LS=%u;RB=%u/%u;"\
                           "SB=%u/%u;EN=%s;DE=%s;"\
                           "JI=%u,%u;DL=%u,%u,%u;"\
                           "IP=%s:%u,%s:%u",
                           statInfo.tSetupDuration,  // CS
                           statInfo.callMetrics.callStats.uLclPktSent,      //PS
                           statInfo.uPtimeLcl ? (statInfo.tCallDuration / statInfo.uPtimeLcl) : 0, //ES
                           statInfo.callMetrics.callStats.uLclOctetSent,  //OS
                           statInfo.callMetrics.callStats.uLclSilencePktSent, //SP
                           statInfo.callMetrics.callStats.uLclSilencePktSent, //SP
                           statInfo.callMetrics.callStats.uLclSilenceOctetSent,     //SO
                           statInfo.callMetrics.callStats.uLclSilencePktSent,       //QS (note QS can be "-" if silsupp disabled)
                           statInfo.callMetrics.callStats.uLclPktRecvd,           //PR
                           statInfo.uPtimeRem ? (statInfo.tCallDuration / statInfo.uPtimeRem) : 0, //ER
                           statInfo.callMetrics.callStats.uLclOctetRecvd,  //OR
                           statInfo.callMetrics.callStats.uLclSilencePktRecvd,   //CR
                           statInfo.callMetrics.callStats.uLclSilenceOctetRecvd,    //SR
                           statInfo.callMetrics.callStats.uLclSilencePktRecvd,      //QR  (note QR can be "-" if silsupp disabled)
                           statInfo.callMetrics.callStats.sLclPktLost,    //PL
                           statInfo.callMetrics.callStats.sRemPktLost,    //PL
                           statInfo.callMetrics.callStats.uBurstLost,   //BL
                           statInfo.callMetrics.callStats.uLostSequence,   //LS
                           statInfo.callMetrics.extCallStatsLoc.uBurstDuration,     
                           statInfo.callMetrics.extCallStatsLoc.uBurstLossDensity,  // RB=<receive burst duration>/<receive Burst density>
                           statInfo.callMetrics.extCallStatsRem.uBurstDuration,     // SB=<send burst duration>/<send burst density>
                           statInfo.callMetrics.extCallStatsRem.uBurstLossDensity,  //SB (note SB can be "-/-" if not supported) 
                           statInfo.callMetrics.callStats.uIngressCodecList, //EN
                           statInfo.callMetrics.callStats.uEgressCodecList,  //DE
                           statInfo.callMetrics.callStats.uRemJitter,statInfo.callMetrics.callStats.uLclJitter, //JI
                           statInfo.callMetrics.extCallStatsLoc.uRdTripNetDelay, //DL:mean
                           statInfo.callMetrics.extCallStatsLoc.uRdTripNetDelay, //DL:min
                           statInfo.callMetrics.extCallStatsLoc.uRdTripNetDelay, //DL:max
                           ipAddrLcl,statInfo.uPortLcl,ipAddrRem,statInfo.uPortRem ); //IP 
        
                  MXD_GNS::CString newName( PM__SIGNALING_X_RTP_STAT );

                  rPkt.GetHeaderList().Get( newName, res );

                  if ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND )
                  {
                     /* Intentionally set the header as private so we can make use of
                     ** the header conversion routine.
                     */
                     pXRTPStatHdr.eType = eCCTKHDRTYPE_PRIVATE;
                     pXRTPStatHdr.pcName =
                        const_cast< BOS_UINT8* >(
                           reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
                     pXRTPStatHdr.pcValue = puData;

                     /* Create the X-RTP-Stat header */
                     MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( pXRTPStatHdr );
                     if ( pSipHeader == NULL )
                     {
                        CCTK_TRACEI0( "PmObserver - updatePktIms: "
                           "Error creating the X-RTP-Stat header." );
                        return;
                     }

                     /* Insert the header into the packet */
                     res = rPkt.GetHeaderList().Append( pSipHeader );
                     pSipHeader = NULL;
                     if ( MX_RIS_F( res ) )
                     {
                        CCTK_TRACEI1( "PmObserver - updatePktIms: "
                           "Error inserting X-RTP-Stat header to packet (%d).",
                           res );
                        return;
                     }
                  }
               } /* end of "if (ceGetXRtpStat( uGuid, &statInfo ))" */

               /* in the next phase when T38 stats are available, add the X-RTP-Stat-T38
               ** header as well
               */
            }
         }
      }
   }
}
#endif /* #if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS ) */

/***********************************************************************************
**  FUNCTION:   PmObserver::updatePktProperty
**
**  PURPOSE:    Update outgoing SIP packet's property.
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void PmObserver::updatePktProperty( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt )
{
   CCTK_TRACED3( "PmObserver(%p) - updatePktProperty(0x%lX, %p)",
                 this, uGuid, &rPkt );

   if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                 CCTK_INTEROP_PRECONDNOADVT ) )
   {
      mxt_result res;
      CCTKHDR supportedHdr;
      MXD_GNS::CString newName( PM__SIGNALING_SUPPORTED_HEADER );
      MXD_GNS::CString newValue;
      BOS_BOOL bComma = BOS_FALSE;

      const MXD_GNS::CSipHeader *pSupported =
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_SUPPORTED, res );

      if ( MX_RIS_S( res ) && ( pSupported != NULL ) )
      {
         /* Loop through the currently set 'Supported' header, remove the
         ** option tag for preconditions, but keep the rest as is.
         */
         while ( pSupported != NULL )
         {
            if ( pSupported->GetSupported().GetString().
                   CaseInsCmp( MXD_GNS::pszOPTIONTAG_PRECONDITION ) != 0 )
            {
               if ( bComma )
               {
                  newValue += ", ";
               }
               
               newValue += pSupported->GetSupported().GetString();
               bComma = BOS_TRUE;
            }

            pSupported = pSupported->GetNextHeader();
         }

         /* Intentionally set the header as private so we can make use of
         ** the header conversion routine.
         */
         supportedHdr.eType = eCCTKHDRTYPE_PRIVATE;
         supportedHdr.pcName =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
         supportedHdr.pcValue =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

         /* Create the Supported header.
         */
         MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( supportedHdr );
         if ( pSipHeader == NULL )
         {
            CCTK_TRACEI0( "PmObserver - updatePktProperty: "
               "Error creating the Supported header." );
            return;
         }

         /* Replace the existing header with the new one.
         */
         res = rPkt.GetHeaderList().ReplaceHeaderTypeWith( pSipHeader );
         pSipHeader = NULL;
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI1( "PmObserver - updatePktProperty: "
               "Error replacing header to packet (%d).",
               res );
            return;
         }
      }
   }

   /* Update Contact URI user parameter value, if needed.
   */
   const CFGUSR *pCfgUsr = CFG_USR( cfgGetUsrId( uGuid ) );
   if ( pCfgUsr != NULL && pCfgUsr->xtl.usr.net.contact.pcUsrParm != NULL )
   {
      MXD_GNS::CGenericParam *pUriParm = NULL;
      MXD_GNS::CGenParamList *pUriParmLst = NULL;
      const MXD_GNS::CGenParamList *pSrcUriParmLst = NULL;
      MXD_GNS::CSipUri *pContactUri = NULL;
      MXD_GNS::CSipHeader *pContactHdr =
         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_CONTACT );

      /* Loop through all Contact headers and insert the configured user parameters
      ** in the Contact URI.
      */
      while ( pContactHdr != NULL )
      {
         pContactUri = pContactHdr->GetContact().GetSipUri();
         if ( pContactUri != NULL )
         {
            pSrcUriParmLst = pContactUri->GetParamList();
            if ( pSrcUriParmLst != NULL )
            {
               pUriParmLst = BRCM_NEW( MXD_GNS::CGenParamList )( *pSrcUriParmLst );
            }
            else
            {
               pUriParmLst = BRCM_NEW( MXD_GNS::CGenParamList );
            }

            pUriParm =
               BRCM_NEW( MXD_GNS::CGenericParam )
                                 ( MXD_GNS::CGenericParam::eCS_SIPURI_PARAM );
            pUriParm->GetName() = "user";
            pUriParm->GetValue() =
               reinterpret_cast< const char * >(
                  pCfgUsr->xtl.usr.net.contact.pcUsrParm );

            pUriParmLst->Append( TO pUriParm );
            pUriParm = NULL;

            pContactUri->SetParamList( TO pUriParmLst );
            pUriParmLst = NULL;
         }

         pContactHdr = pContactHdr->GetNextHeader();
      }
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::insertFeatHdr
**
**  PURPOSE:    Insert outgoing SIP packet with feature specific header(s).
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void PmObserver::insertFeatHdr( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt )
{
   CCTK_TRACED3( "PmObserver(%p) - insertFeatHdr(0x%lX, %p)", this, uGuid, &rPkt );

   if ( rPkt.IsRequest() &&
        (rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_INVITE) )
   {
      const CFGCALL *pCfgCall = CFG_CALL( uGuid );
      CCTK_ASSERT( pCfgCall != NULL );
      if ( pCfgCall == NULL )
      {
         CCTK_TRACEI0( "PmObserver - insertFeatHdr: "
                       "Error retrieving call configuration." );
         return;
      }

      /* If this is an emergency call.
      */
      if ( pCfgCall->xtl.basic.gen.emgCallCfg.bEmergency )
      {
         mxt_result res;

         /* Insert the Geolocation header.  The geo-location header is inserted as
         ** a content-id since we are providing the content in the PIDF-LO format
         ** later on (see insertFeatBody method).
         */
         if ( pCfgCall->xtl.basic.gen.emgCallCfg.bAddLocInfo)
         {
            CCTKHDR geoLocHdr;
            MXD_GNS::CString newName( PM__SIGNALING_GEOLOCATION_HEADER );
            MXD_GNS::CString newValue;

            const CFGUSR *pCfgUsr = CFG_USR( pCfgCall->in.uUsrId );
            rPkt.GetHeaderList().Get( newName, res );

            if ( ( pCfgUsr != NULL ) &&
                 ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND ) )
            {
               newValue += "<cid:sos@";
               newValue += reinterpret_cast<const char *>(
                              pCfgUsr->xtl.reg.gen.registrar.hostPort.pcAddr );
               newValue += ">;inserted-by=\"";
               newValue += reinterpret_cast<const char *>(
                              pCfgUsr->xtl.usr.sec.auth.client.pcAuthName );
               newValue += "\", routing-allowed=\"yes\"";

               /* Intentionally set the header as private so we can make use of
               ** the header conversion routine.
               */
               geoLocHdr.eType = eCCTKHDRTYPE_PRIVATE;
               geoLocHdr.pcName =
                  const_cast< BOS_UINT8* >(
                     reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
               geoLocHdr.pcValue =
                  const_cast< BOS_UINT8* >(
                     reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

               /* Create the Geolocation header */
               MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( geoLocHdr );
               if ( pSipHeader == NULL )
               {
                  CCTK_TRACEI0( "PmObserver - insertFeatHdr: "
                     "Error creating the Geolocation header." );
                  return;
               }

               /* Insert the header into the packet */
               res = rPkt.GetHeaderList().Append( pSipHeader );
               pSipHeader = NULL;
               if ( MX_RIS_F( res ) )
               {
                  CCTK_TRACEI1( "PmObserver - insertFeatHdr: "
                     "Error inserting header to packet (%d).",
                     res );
                  return;
               }
            }
         }

         rPkt.GetHeaderList().Get( MXD_GNS::eHDR_PRIORITY, res );
         if ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND )
         {
            /* Set the Priority header */
            CCTKHDR priHdr;
            mxt_result res;
            MXD_GNS::CString newName( PM__SIGNALING_PRIORITY_HEADER );
            MXD_GNS::CString newValue( PM__SIGNALING_EMERGENCY_PARAMETER );

            /* Intentionally set the header as private so we can make use of
            ** the header conversion routine.
            */
            priHdr.eType = eCCTKHDRTYPE_PRIVATE;
            priHdr.pcName =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
            priHdr.pcValue =
               const_cast< BOS_UINT8* >(
                  reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

            /* Create the Authorization header */
            MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( priHdr );
            if ( pSipHeader == NULL )
            {
               CCTK_TRACEI0( "PmObserver - insertFeatHdr: "
                             "Error creating the Priority header." );
               return;
            }

            /* Insert the header to the packet */
            res = rPkt.GetHeaderList().Append( pSipHeader );
            pSipHeader = NULL;
            if ( MX_RIS_F( res ) )
            {
               CCTK_TRACEI1( "PmObserver - insertFeatHdr: "
                             "Error inserting header to packet (%d).",
                             res );
               return;
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::insertFeatBody
**
**  PURPOSE:    Insert outgoing SIP packet with feature specific body part(s).
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void PmObserver::insertFeatBody( BOS_UINT32 uGuid, MXD_GNS::CSipPacket &rPkt )
{
   CCTK_TRACED3( "PmObserver(%p) - insertFeatBody(0x%lX, %p)", this, uGuid, &rPkt );

   if ( rPkt.IsRequest() &&
        (rPkt.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_INVITE) )
   {
      const CFGCALL *pCfgCall = CFG_CALL( uGuid );
      CCTK_ASSERT( pCfgCall != NULL );
      if ( pCfgCall == NULL )
      {
         CCTK_TRACEI0( "PmObserver - insertFeatBody: "
                       "Error retrieving call configuration." );
         return;
      }

      /* If this is an emergency call, we need to include a RFC 4119 defined
      ** PIDF-LO (Presence Information Data Format - Location Object) component
      ** as part of the body.
      */
      if ( pCfgCall->xtl.basic.gen.emgCallCfg.bEmergency &&
           pCfgCall->xtl.basic.gen.emgCallCfg.bAddLocInfo )
      {
         /* Note here that we always send a SDP body at the minimum with our
         ** outgoing INVITE, note also that if there is already a multipart body
         ** set in this case, it is most likely because this is a retransmission,
         ** so do not add another PIDF-LO body since we must have one already.
         **
         ** If either of the above assumption change (note that this is an emergency
         ** call though, so none of those should change really, unless there is a
         ** change to the emergency call behavior required), then we may have to
         ** update this check to ensure we end up with what we want no matter
         ** what.
         */
         if ( rPkt.GetSipMessageBody() &&
              !(rPkt.GetSipMessageBody())->GetNumberOfBodies() )
         {
            MXD_GNS::CSipMessageBody *pTopBody =
                                          BRCM_NEW( MXD_GNS::CSipMessageBody );
            MXD_GNS::CSipMessageBody *pPidfLoBody =
                                          BRCM_NEW( MXD_GNS::CSipMessageBody );
            MXD_GNS::CSipMessageBody *pOldBody = NULL;

            if ( pPidfLoBody )
            {
               MXD_GNS::CBlob *pBlob;
               MXD_GNS::CSipHeader *pCtTypeHdr;
               MXD_GNS::CSipHeader *pCtIdHdr;
               MXD_GNS::CRawHeader *pRawHdr;
               MXD_GNS::CHeaderList *pAddHdrs = NULL;
               BOS_UINT8 locData[CCTK_STR_LEN * 8];
               BOS_UINT32 locLen;

               /* Format the location information data through the Call Engine.
               */
               locLen = ceGetLocInfo( locData, CCTK_STR_LEN * 8, uGuid );
               pBlob =
                  BRCM_NEW( MXD_GNS::CBlob )(
                     reinterpret_cast<const uint8_t *> (locData),
                     static_cast<unsigned int> (locLen),
                     static_cast<unsigned int> (locLen) );

               pCtTypeHdr =
                  BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
               pCtTypeHdr->GetContentTypeMType() = PM__SIGNALING_APPLICATION_TYPE;
               pCtTypeHdr->GetContentTypeMSubType() =
                                                   PM__SIGNALING_PIDF_XML_SUBTYPE;

               pCtIdHdr =
                  BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_ID );
               pRawHdr =
                  BRCM_NEW( MXD_GNS::CRawHeader );

               if ( pCtIdHdr && pRawHdr )
               {
                  MXD_GNS::CString newName( PM__SIGNALING_CONTENTID_HEADER );
                  MXD_GNS::CString newValue;
                  const CFGUSR *pCfgUsr = CFG_USR( pCfgCall->in.uUsrId );

                  if ( pCfgUsr != NULL )
                  {
                     newValue += "sos@";
                     newValue += reinterpret_cast<const char *>(
                                    pCfgUsr->xtl.reg.gen.registrar.hostPort.pcAddr );

                     pRawHdr->GetName() = newName;
                     pRawHdr->GetBody() = newValue;
                     pCtIdHdr->SetRawHeader( pRawHdr );
                  }

                  pAddHdrs = BRCM_NEW( MXD_GNS::CHeaderList );
                  if ( pAddHdrs && pCtIdHdr )
                  {
                     pAddHdrs->Append( pCtIdHdr );
                  }
               }

               pPidfLoBody->AddBody( pBlob, pCtTypeHdr, pAddHdrs );
               pBlob = NULL;
               pCtTypeHdr = NULL;
               pAddHdrs = NULL;
            }

            pOldBody =
               BRCM_NEW( MXD_GNS::CSipMessageBody )( *(rPkt.GetSipMessageBody()) );

            if ( pTopBody && pPidfLoBody && pOldBody )
            {
               MXD_GNS::CSipHeader* pContentType;

               pContentType =
                  BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_CONTENT_TYPE );
               pContentType->GetContentTypeMType() =
                  MXD_GNS::CSipMessageBody::ms_pszCONTENT_TYPE_MULTIPART_MEDIA_TYPE;
               pContentType->GetContentTypeMSubType() =
                  MXD_GNS::CSipMessageBody::ms_pszCONTENT_TYPE_MIXED_MEDIA_SUBTYPE;

               pTopBody->SetExternalMimeHeaders( pContentType );
               pContentType = NULL;

               pTopBody->AddBody( pPidfLoBody );
               pPidfLoBody = NULL;
               pTopBody->AddBody( pOldBody );
               pOldBody = NULL;

               /* Replacement occurs now...*/
               rPkt.SetSipMessageBody( pTopBody );
               pTopBody = NULL;
            }
            else
            /* Something went wrong, we need to release all the potentially
            ** created resources.
            */
            {
               if ( pTopBody )
               {
                  BRCM_DELETE( pTopBody );
                  pTopBody = NULL;
               }

               if ( pPidfLoBody )
               {
                  BRCM_DELETE( pPidfLoBody );
                  pPidfLoBody = NULL;
               }

               if ( pOldBody )
               {
                  BRCM_DELETE( pOldBody );
                  pOldBody = NULL;
               }
            }
         }
      }
   }
}


/* ------------------------------------------------------------------------------ */
/* ---- BELOW THIS POINT, THE CALLBACK INTERFACE FROM THE STACK IS IMPLEMENTED -- */
/* ---- THIS IS THE ONLY PORTION OF THE PACKET MODIFIER MODULE THAT MAY --------- */
/* ---- (SHOULD) BE IN DIRECT CONTACT WITH THIRD PARTY PROCESSING. -------------- */
/* ------------------------------------------------------------------------------ */

/***********************************************************************************
**  FUNCTION:   lastChance
**
**  PURPOSE:    Pass the payload to be modified to the observer at the last minute
**              prior to actually sending out the packet, hence the 'last chance'
**              denomination.
**
**  PARAMETERS:
**              ppNewData - the new data information to use as replacement of the
**                          current data.  This pointer must be assigned to some
**                          valid value or the replacement data will be ignored.
**              destInfo - the destination information associated with this
**                         data, this is the resolved address of the target.
**              pData    - the data currently being sent, if ppNewData does not
**                         reallocate some new data, the pData data is the one
**                         used.
**              uSize    - the size of the current data (pData).
**
**  RETURNS:    The size of the new data (ppNewData) to be consider for sending
**              or zero if no data is reallocated.
**
**  NOTES:      The newly reallocated data will live untouched until the next
**              call to pmLastChance is placed during which the data may be
**              replaced.  Therefore the information returned by this call is
**              not meant to be cached by the observer.  If caching must take
**              place, it should take place at the layer calling this API.
***********************************************************************************/
BOS_UINT32 PmObserver::lastChance( BOS_UINT8 **ppNewData,
                                   CCTKIPPORT *destInfo,
                                   const BOS_UINT8 *pData,
                                   BOS_UINT32 uSize )
{
   BOS_UINT32 uRet = 0;

   /* Make sure we will have enough space to save the possibly overwriten packet,
   ** if not, do not do anything to prevent corruption, something else will not
   ** work eventually if we fail to apply this special processing which will lead
   ** us back to perhops determine the necessity to increase this buffer, but that
   ** would be an application dependent decision then to make.
   **
   ** Due to the sensitive nature of the operation happening in this callback, the
   ** only feature currently available is the 'override FQDN in Route header' which
   ** consists in replacing the Route header which contains a FQDN with the actual
   ** IP address of the target destination.
   **
   ** No other packet modification feature should apply in this context.  Other
   ** packet modification features are applied in the context of the stack's 
   ** implemented callback (see ISceSipPacketObserver interface).
   */
   if ( mbRteHdrOverwriteFqdn &&
        ( uSize < PM_PKT_BUFFER_SIZE ) )
   {
      MXD_GNS::CSipPacket packet;
      mxt_result res;

      /* Create a parse-able packet out of the raw data, if a payload is
      ** associated with this packet, upon return of the AppendRawData
      ** function, the paylaod will be pointed to at the start of the pData
      ** pointer.  It can be re-used then.
      */
      packet.AppendRawData( reinterpret_cast<const char *&> (pData) );
      packet.CommitRawDataList();

      /* Locate whether there is a Route header included in this packet, if
      ** not, we do not care here.
      */
      const MXD_GNS::CSipHeader *pRte =
                     packet.GetHeaderList().Get( MXD_GNS::eHDR_ROUTE, res );
      if ( MX_RIS_S( res ) && ( pRte != NULL ) )
      {
         /* Check if the first route header (top most) contains a host name
         ** (i.e. FQDN) rather than an IP address.
         */
         if ( (pRte->GetRoute().GetSipUri() != NULL) &&
              (pRte->GetRoute().GetSipUri()->GetHostPort().IsHostName()) )
         {
            BOS_UINT8 uIpAddr[ PM_IP_ADDRESS_STRING_LEN + 1 ];

            bosIpAddrGetStr ( &destInfo->addr,
                              reinterpret_cast<char *> (uIpAddr),
                              PM_IP_ADDRESS_STRING_LEN );
 
            /* Only remove the first Route header is multiple are present.
            */
            packet.GetHeaderList().RemoveHeader( MXD_GNS::eHDR_ROUTE, 0 );

            /* Create the new Route header.
            */
            MXD_GNS::CString host( reinterpret_cast<char *> (uIpAddr) );
            MXD_GNS::CBlob blob;
            MXD_GNS::CSipHeader *pRouteHdr =
                  BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_ROUTE );
            MXD_GNS::CSipUri *pRouteUri =
                  BRCM_NEW( MXD_GNS::CSipUri );
            MXD_GNS::CGenericParam *pLrParm =
                  BRCM_NEW( MXD_GNS::CGenericParam )
                       ( MXD_GNS::CGenericParam::eCS_SIPURI_PARAM );
            MXD_GNS::CGenParamList *pRouteUriParm =
                  BRCM_NEW( MXD_GNS::CGenParamList );

            if ( ( pRouteHdr == NULL ) ||
                 ( pRouteUri == NULL ) ||
                 ( pLrParm == NULL ) ||
                 ( pRouteUriParm == NULL ) )
            {
               /* Exit now in case of failure of some sort.  Do not prevent
               ** processing from going on though.  We simply would not apply the
               ** change that we ought to.
               **
               ** Clean up before exiting.
               */
               if ( pRouteHdr )
               {
                  BRCM_DELETE( pRouteHdr );
                  pRouteHdr = NULL;
               }
               if ( pRouteUri )
               {
                  BRCM_DELETE( pRouteUri );
                  pRouteUri = NULL;
               }
               if ( pLrParm )
               {
                  BRCM_DELETE( pLrParm );
                  pLrParm = NULL;
               }
               if ( pRouteUriParm )
               {
                  BRCM_DELETE( pRouteUriParm );
                  pRouteUriParm = NULL;
               }
               /* Exit now.
               */
               return 0;
            }

            /* Assume some default setting for the Route header content, those
            ** are consistent with the most common use of this feature, if this requires
            ** to change in the future, one could parametize this a bit more but it is
            ** not anticipated need to do so for a while.
            */
            pRouteUri->SetSecured( MXD_GNS::CSipUri::eUNSECURE );
            pRouteUri->Set( host, destInfo->uPort );

            pLrParm->GetName() = MXD_GNS::szURIPARAM_LR;
            pRouteUriParm->Append( pLrParm );
            pLrParm = NULL;
            pRouteUri->SetParamList( pRouteUriParm );
            pRouteUriParm = NULL;

            pRouteHdr->GetRoute().SetUri( pRouteUri );
            pRouteUri = NULL;

            /* Insert the new header at the top position in the packet.
            */
            packet.GetHeaderList().Prepend( pRouteHdr );
            pRouteHdr = NULL;

            /* Get the newly created packet back as raw format for transmission
            ** over the physical layer.
            */
            packet.Serialize( blob );
            memcpy ( (void *)puPktBuffer,
                     blob.GetFirstIndexPtr(),
                     blob.GetSize() );

            /* Check whether there was a payload in the packet and if so, make
            ** sure we also copy it over to the new packet.
            */
            const MXD_GNS::CSipHeader *pContLen =
               packet.GetHeaderList().Get( MXD_GNS::eHDR_CONTENT_LENGTH, res );
            BOS_UINT32 uLen = 0;

            if ( MX_RIS_S( res ) && ( pContLen != NULL ) )
            {
               const MXD_GNS::CToken length = pContLen->GetContentLength();

               if ( length.IsUint32() )
               {
                  length.GetUint32( reinterpret_cast<uint32_t &> (uLen) );
                  /* Payload was present, copy it over.
                  */
                  if ( uLen )
                  {
                     memcpy ( (void *) (puPktBuffer + blob.GetSize()),
                              (void *) pData,
                              uLen );
                  }
               }
            }

            /* Overwrite the current data with the newly created one.
            */
            *ppNewData = (BOS_UINT8 *) puPktBuffer;
            uRet = blob.GetSize() + uLen;
         }
      }
   }

   return uRet;
}


/***********************************************************************************
**  FUNCTION:   PmObserver::EvSendingRequest
**
**  PURPOSE:    Callback from SCE ISceSipPacketObserver prior to sending out a
**              request.
**
**  PARAMETERS:
**              pComponent   - the registered component issuing the request.
**              rPacketOut   - the packet being sent.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void PmObserver::EvSendingRequest(
                               IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                               INOUT MXD_GNS::CSipPacket& rPacketOut )
{
   CCTK_TRACED3(
      "PmObserver(%p) - EvSendingRequest(%p, %p)", this, pComponent, &rPacketOut );

   BOS_UINT32 uGuid = ceGetPmRes( reinterpret_cast< void* >( pComponent ) );

   /* If allocated a resource with this callback, then do something about it
   ** based on the appropriate configuration setting.
   */
   if ( uGuid != guCeInvalidGuid )
   {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      /* Update packet to IMS-compliant (if needed) */
      updatePktIms( uGuid, rPacketOut );
#endif

      /* Update packet's property (if needed) */
      updatePktProperty( uGuid, rPacketOut );

      /* Insert private header (if needed) */
      insertGenHdr( uGuid, rPacketOut );

      /* Insert generic feature header (if needed) */
      insertFeatHdr( uGuid, rPacketOut );

      /* If this is a PUBLISH message, save the IP address to which this
      ** message is being sent to */
      if ( rPacketOut.IsRequest() &&
           ( rPacketOut.GetRequestLine()->GetMethod() ==
                                    MXD_GNS::eSIP_METHOD_PUBLISH ) )
      {
         MXD_GNS::CSocketAddr destAddr;
         destAddr = rPacketOut.GetPeerAddr();
         ceSetPublishAddr( uGuid, &destAddr );
      }

      if ( rPacketOut.IsRequest() &&
           ( rPacketOut.GetRequestLine()->GetMethod() ==
                                    MXD_GNS::eSIP_METHOD_INVITE ) )
      {
         mxt_result res;

         /* Copy the Call-ID and From tag for this call to the CDR if applicable.
         */
         const BOS_UINT8 *pCidStr = NULL;
         const BOS_UINT8 *pFromTagStr = NULL;
         const MXD_GNS::CSipHeader *pCid =
               rPacketOut.GetHeaderList().Get( MXD_GNS::eHDR_CALL_ID, res );
         if ( MX_RIS_S( res ) && ( pCid != NULL ) )
         {
            pCidStr = (!pCid->GetCallId().IsEmpty() ?
                              reinterpret_cast<const BOS_UINT8 *> (
                                       pCid->GetCallId().GetString().CStr())
                                                       : NULL );
         }

         const MXD_GNS::CSipHeader *pFrom =
               rPacketOut.GetHeaderList().Get( MXD_GNS::eHDR_FROM, res );
         if ( MX_RIS_S( res ) && ( pFrom != NULL ) )
         {
            const MXD_GNS::CToken *pFromTag =
                           pFrom->GetParam( PM__SIGNALING_TAG_PARAMETER );

            pFromTagStr = (pFromTag ?
                              reinterpret_cast<const BOS_UINT8 *> (
                                       pFromTag->GetString().CStr())
                                                      : NULL );
         }

         ceSetCdrInfo( uGuid, pCidStr, pFromTagStr, NULL );
      }

      /* Copy the To tag for this call to the CDR if applicable.
      */
      if ( rPacketOut.IsRequest() &&
           ( rPacketOut.GetRequestLine()->GetMethod() ==
                                    MXD_GNS::eSIP_METHOD_ACK ) )
      {
         mxt_result res;
         const BOS_UINT8 *pToTagStr = NULL;
         const MXD_GNS::CSipHeader *pTo =
               rPacketOut.GetHeaderList().Get( MXD_GNS::eHDR_TO, res );
         if ( MX_RIS_S( res ) && ( pTo != NULL ) )
         {
            const MXD_GNS::CToken *pToTag =
                           pTo->GetParam( PM__SIGNALING_TAG_PARAMETER );

            pToTagStr = (pToTag ?
                              reinterpret_cast<const BOS_UINT8 *> (
                                       pToTag->GetString().CStr())
                                                      : NULL );
         }

         ceSetCdrInfo( uGuid, NULL, NULL, pToTagStr );
      }

      /* Insert generic feature body (if needed) */
      insertFeatBody( uGuid, rPacketOut );
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::EvSendingResponse
**
**  PURPOSE:    Callback from SCE ISceSipPacketObserver prior to sending out a
**              response to a request.
**
**  PARAMETERS:
**              pComponent   - the registered component issuing the response.
**              rPacketOut   - the packet being sent.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void PmObserver::EvSendingResponse(
                                IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                                INOUT MXD_GNS::CSipPacket& rPacketOut )
{
   CCTK_TRACED3(
      "PmObserver(%p) - EvSendingResponse(%p, %p)", this, pComponent, &rPacketOut );

   BOS_UINT32 uGuid = ceGetPmRes( reinterpret_cast< void* >( pComponent ) );

   /* If allocated a resource with this callback, then do something about it
   ** based on the appropriate configuration setting.
   */
   if ( uGuid != guCeInvalidGuid )
   {
#if ( CCTKCFG_CCTK_MODE == CCTKCFG_CCTK_IMS )
      /* Update packet to IMS-compliant (if needed) */
      updatePktIms( uGuid, rPacketOut );
#endif

      /* Update packet's property (if needed) */
      updatePktProperty( uGuid, rPacketOut );

      /* Insert private header (if needed) */
      insertGenHdr( uGuid, rPacketOut );
   }

   /* Insert header lacking from the RFC 3261 compliant stack implementation.
   */
   if ( rPacketOut.IsResponse() &&
        ( rPacketOut.GetStatusLine()->GetCode() ==
                                 MXD_GNS::uUNSUPPORTED_MEDIA_TYPE ) )
   {
      mxt_result res;

      rPacketOut.GetHeaderList().Get( MXD_GNS::eHDR_ACCEPT_LANGUAGE, res );
      /* If there is no Accept-Language header, we need to add one.
      */
      if ( res == MXD_GNS::resFE_SIPPARSER_HEADERLIST_NOT_FOUND )
      {
         CCTKHDR alHdr;
         MXD_GNS::CString newName( PM__SIGNALING_ACCEPT_LANGUAGE_HEADER );
         MXD_GNS::CString newValue( PM__SIGNALING_LANGUAGE_ENGLISH );

         /* Intentionally set the header as private so we can make use of
         ** the header conversion routine.
         */
         alHdr.eType = eCCTKHDRTYPE_PRIVATE;
         alHdr.pcName =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newName.CStr() ) );
         alHdr.pcValue =
            const_cast< BOS_UINT8* >(
               reinterpret_cast< const BOS_UINT8* >( newValue.CStr() ) );

         /* Create the Accept-Language header */
         MXD_GNS::CSipHeader *pSipHeader = cfgCvtHdrToMx( alHdr );
         if ( pSipHeader == NULL )
         {
            CCTK_TRACEI0( "PmObserver - EvSendingResponse: "
               "Error creating the Accept-Language header." );
            return;
         }

         /* Insert the header to the packet */
         res = rPacketOut.GetHeaderList().Append( pSipHeader );
         pSipHeader = NULL;
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI1( "PmObserver - EvSendingResponse: "
               "Error inserting header to packet (%d).",
               res );
            return;
         }
      }
   }

   /* If this is a response to a blind (out-of-dialog) notification and we have
   ** accepted this notification internally, we would have rejected the request
   ** in order to be compliant with the stack framework, however at this time, we
   ** want to revert this decision to let the remote end know that we did in fact
   ** accept the request.
   ** This behavior is basically a work around for a framework limitation,
   ** it is subject to review and change as the framework evolves in the
   ** future.
   */
   if ( rPacketOut.IsResponse() &&
        ( rPacketOut.GetStatusLine()->GetCode() == MXD_GNS::uBAD_EXTENSION ) &&
        !strcmp( rPacketOut.GetStatusLine()->GetPhrase().CStr(),
                 PM__SIGNALING_ACCEPT_BLIND_NOT_REASON_INT ) )
   {
      MXD_GNS::CSipStatusLine *pStatusLine = BRCM_NEW( MXD_GNS::CSipStatusLine );
      if ( pStatusLine )
      {
         pStatusLine->Set( MXD_GNS::uOK,
                           MXD_GNS::szOK );
         /* Overwrite the status line with the success response code.
         */
         rPacketOut.SetStatusLine( pStatusLine );
      }
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::EvReceivedRequest
**
**  PURPOSE:    Callback from SCE ISceSipPacketObserver when receiving a
**              request.
**
**  PARAMETERS:
**              pComponent   - the registered component receiving the request.
**              rPacketIn    - the packet being received.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void PmObserver::EvReceivedRequest(
                                IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                                IN const MXD_GNS::CSipPacket& rPacketIn )
{
   CCTK_TRACED3(
      "PmObserver(%p) - EvReceivedRequest(%p, %p)", this, pComponent, &rPacketIn );

   MXD_GNS::CSipPacket *pPacket = const_cast< MXD_GNS::CSipPacket * >( &rPacketIn );

   BOS_UINT32 uGuid = ceGetPmRes( reinterpret_cast< void* >( pComponent ) );

   /* If allocated a resource with this callback, then read the information we
   ** would want to extract from it.
   */
   if ( uGuid != guCeInvalidGuid )
   {
      /* Copy the To tag for this call to the CDR if applicable.
      */
      if ( rPacketIn.IsRequest() &&
           ( rPacketIn.GetRequestLine()->GetMethod() ==
                                    MXD_GNS::eSIP_METHOD_ACK ) )
      {
         mxt_result res;
         const BOS_UINT8 *pToTagStr = NULL;
         const MXD_GNS::CSipHeader *pTo =
               rPacketIn.GetHeaderList().Get( MXD_GNS::eHDR_TO, res );
         if ( MX_RIS_S( res ) && ( pTo != NULL ) )
         {
            const MXD_GNS::CToken *pToTag =
                           pTo->GetParam( PM__SIGNALING_TAG_PARAMETER );

            pToTagStr = (pToTag ?
                              reinterpret_cast<const BOS_UINT8 *> (
                                       pToTag->GetString().CStr())
                                                      : NULL );
         }

         ceSetCdrInfo( uGuid, NULL, NULL, pToTagStr );
      }
   }

   /* Bypass checking of the Subscription-State header in the received NOTIFY.
   ** If checking is enabled, 400 response will be sent automatically by the
   ** stack if the received NOTIFY does not contain the Subscription-State
   ** header. Note that disabling this check will break the RFC3265 compliance.
   */
   if ( UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                 CCTK_INTEROP_NOTIFYSUBSTATE ) &&
        rPacketIn.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_NOTIFY &&
        rPacketIn.GetHeaderList().Get( MXD_GNS::eHDR_SUBSCRIPTION_STATE ) == NULL )
   {
      /* Prevent the stack from declining the received NOTIFY by inserting
      ** a mandatory Subscription-State header.
      */
      MXD_GNS::CSipHeader *pHdrSubState =
         BRCM_NEW( MXD_GNS::CSipHeader )( MXD_GNS::eHDR_SUBSCRIPTION_STATE );

      pHdrSubState->GetSubscriptionState() = PM__SIGNALING_SUB_STATE_ACTIVE;
      pPacket->GetHeaderList().Append( pHdrSubState );
   }

   /* Bypass checking of the terminating <cr><lf> in the received NOTIFY.
   **
   ** If checking is enabled, 400 response will be sent automatically by the stack
   ** if the received NOTIFY contains a payload ended without CRLF.  There are two
   ** (independent) practical cases where this applies:
   **
   **  - When receiving a 'message/sipfrag' NOTIFY which is used in conjunction
   **    with REFER and call transfer. Note that disabling this check will break
   **    the RFC3515 compliance.
   **
   **  - When receiving an 'application/simple-message-summary' NOTIFY which is
   **    used for message waiting indication. Note that disabling this check will
   **    break the RFC3842 compliance.
   */
   if ( rPacketIn.GetRequestLine()->GetMethod() == MXD_GNS::eSIP_METHOD_NOTIFY )
   {
      const MXD_GNS::CSipHeader *pHdr =
                  rPacketIn.GetHeaderList().Get( MXD_GNS::eHDR_CONTENT_TYPE );

      if ( pHdr &&
           UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                    CCTK_INTEROP_STATUSLINECRLF ) &&
           pHdr->GetContentTypeMType().GetString().CaseInsCmp(
                                 PM__SIGNALING_CONTENT_TYPE_MESSAGE ) == 0 &&
           pHdr->GetContentTypeMSubType().GetString().CaseInsCmp(
                                 PM__SIGNALING_CONTENT_TYPE_SUB_SIPFRAG ) == 0 )
      {
         MXD_GNS::CBlob *pContent = pPacket->GetPayload();
         if ( pContent != NULL )
         {
            MXD_GNS::CSipStatusLine statusLine;
            const char *pcBuffer =
               reinterpret_cast< const char * >( pContent->GetFirstIndexPtr() );
            if ( statusLine.Parse( pcBuffer ) == resFE_UNEXPECTED )
            {
               /* Prevent the stack from declining the received NOTIFY by
               ** inserting a CRLF at the end of the Status-Line payload.
               */
               pContent->Append( MXD_GNS::CStringHelper::ms_pszCRLF );
            }
         }
      }
      else if ( pHdr &&
                UTIL_BMP_IS_ENABLED_ALL( CFG_SYS->xtl.net.prot.gen.uInterop,
                                         CCTK_INTEROP_MSGSUMCRLF ) &&
                pHdr->GetContentTypeMType().GetString().CaseInsCmp(
                                 PM__SIGNALING_APPLICATION_TYPE ) == 0 &&
                pHdr->GetContentTypeMSubType().GetString().CaseInsCmp(
                                 PM__SIGNALING_MSG_SUM_SUBTYPE ) == 0 )
      {
         MXD_GNS::CBlob *pContent = pPacket->GetPayload();
         if ( pContent != NULL )
         {
            MXD_GNS::CMessageSummary msgSum;
            const char *pcBuffer =
               reinterpret_cast< const char * >( pContent->GetFirstIndexPtr() );
            if ( msgSum.Parse( pcBuffer ) == resFE_INVALID_ARGUMENT )
            {
               /* Prevent the stack from declining the received NOTIFY by
               ** inserting a CRLF at the end of the Message-Summary payload.
               */
               pContent->Append( MXD_GNS::CStringHelper::ms_pszCRLF );
            }
         }
      }
   }
}


/***********************************************************************************
**  FUNCTION:   PmObserver::EvReceivedResponse
**
**  PURPOSE:    Callback from SCE ISceSipPacketObserver when receiving a
**              response.
**
**  PARAMETERS:
**              pComponent   - the registered component receiving the response.
**              rPacketIn    - the packet being received.
**
**  RETURNS:    None
**
**  NOTES:      .
***********************************************************************************/
void PmObserver::EvReceivedResponse(
                                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                                 IN const MXD_GNS::CSipPacket& rPacketIn )
{
   CCTK_TRACED3(
      "PmObserver(%p) - EvReceivedResponse(%p, %p)", this, pComponent, &rPacketIn );
      
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pComponent );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( rPacketIn );

}
