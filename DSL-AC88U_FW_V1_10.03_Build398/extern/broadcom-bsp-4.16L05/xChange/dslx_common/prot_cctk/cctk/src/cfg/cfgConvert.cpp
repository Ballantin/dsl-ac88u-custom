/**********************************************************************************
** Copyright © 2007-2010 Broadcom Corporation
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
**      This file implements the configuration conversion functions.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <cfgApi.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* Trace map */
typedef struct TRACEMAP
{
   BOS_UINT16  uCctk;
   int         sMx;
} TRACEMAP;

/* Transport map */
typedef struct TRANSPORTMAP
{
   BOS_UINT32  uCctk;
   int         sMx;
} TRANSPORTMAP;

/* Header strings */
const char *CFGCVT_HDRSTR_CALL_INFO = "Call-Info";
const char *CFGCVT_HDRSTR_WARNING = "Warning";

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */
/* DNS mode map */
static const int gsDnsModeMap[] =
{
   /* NOTE: Size and order of the array are important */
   ( MX_NS eNAPTR | MX_NS eSRV ),      /* eCCTKDNSMODE_ALL */
   ( MX_NS eSRV ),                     /* eCCTKDNSMODE_NO_NAPTR */
   ( MX_NS eNONE ),                    /* eCCTKDNSMODE_NO_NAPTR_SRV */
};

static const BOS_UINT32 guMethodMap[] =
{
   /* NOTE: Size and order of the array are important */
   CCTK_METHOD_ACK,                    /* eSIP_METHOD_ACK */
   CCTK_METHOD_BYE,                    /* eSIP_METHOD_BYE */
   CCTK_METHOD_NONE,                   /* eSIP_METHOD_CANCEL */
   CCTK_METHOD_INFO,                   /* eSIP_METHOD_INFO */
   CCTK_METHOD_INVITE,                 /* eSIP_METHOD_INVITE */
   CCTK_METHOD_MESSAGE,                /* eSIP_METHOD_MESSAGE */
   CCTK_METHOD_NOTIFY,                 /* eSIP_METHOD_NOTIFY */
   CCTK_METHOD_OPTIONS,                /* eSIP_METHOD_OPTIONS */
   CCTK_METHOD_NONE,                   /* eSIP_METHOD_PING */
   CCTK_METHOD_PRACK,                  /* eSIP_METHOD_PRACK */
   CCTK_METHOD_NONE,                   /* eSIP_METHOD_PUBLISH */
   CCTK_METHOD_REFER,                  /* eSIP_METHOD_REFER */
   CCTK_METHOD_REGISTER,               /* eSIP_METHOD_REGISTER */
   CCTK_METHOD_NONE,                   /* eSIP_METHOD_SERVICE */
   CCTK_METHOD_SUBSCRIBE,              /* eSIP_METHOD_SUBSCRIBE */
   CCTK_METHOD_UPDATE,                 /* eSIP_METHOD_UPDATE */
   CCTK_METHOD_NONE,                   /* eSIP_METHOD_UNKNOWN */
};

/* Trace level map */
static const TRACEMAP gTraceLvlMap[] =
{
   { CCTK_TRACELVL_NONE,      eLEVEL_NONE },
   { CCTK_TRACELVL_WARNING,   eLEVEL0 | eLEVEL1 },
   { CCTK_TRACELVL_INFO,      eLEVEL2 | eLEVEL3 },
   { CCTK_TRACELVL_DEBUG,     eLEVEL4 | eLEVEL5 | eLEVEL6 | eLEVEL7 | eLEVEL8 | 
                              eLEVEL9 },

   /* Must be last - always. */
   { 0xFFFF,                  0xFFFFFFFF },
};

/* Transport map */
static const TRANSPORTMAP gTransportMap[] =
{
   { CCTK_TRANSPORT_UDP,      MX_NS eUDP },
   { CCTK_TRANSPORT_TCP,      MX_NS eTCP },
   { CCTK_TRANSPORT_TLS,      MX_NS eTLS },

   /* Must be last - always. */
   { 0xFFFFFFFF,              0xFFFFFFFF },
};

/* Connected Id map */
static const MX_NS IUaSspCallConfig::EConnectedIdConfig geCnctdIdLvlMap[] =
{
   MX_NS IUaSspCallConfig::eCONNECTED_ID_SUPPORT,   /* eCCTKCONNECTEDID_SUPPORTED */
   MX_NS IUaSspCallConfig::eCONNECTED_ID_DISABLE,   /* eCCTKCONNECTEDID_DISABLED */

};

/* Privacy level map */
static const MX_NS ISceUserIdentityConfig::EPrivacyMode gePrivLvlMap[] =
{
   /* NOTE: Size and order of the array are important */
   MX_NS ISceUserIdentityConfig::ePRIV_DISABLE,          /* eCCTKPRIVLVL_NONE */
   MX_NS ISceUserIdentityConfig::ePRIV_LOCAL,            /* eCCTKPRIVLVL_LOCAL */
   MX_NS ISceUserIdentityConfig::ePRIV_ID_RESTRICTION_LEVEL_3,
                                                         /* eCCTKPRIVLVL_NETID */
   MX_NS ISceUserIdentityConfig::ePRIV_ID_NON_CRIT_LEVEL_4,
                                                         /* eCCTKPRIVLVL_NETIDNC */
   MX_NS ISceUserIdentityConfig::ePRIV_NETWORK_LEVEL_1,
                                                         /* eCCTKPRIVLVL_NETHDR */
   MX_NS ISceUserIdentityConfig::ePRIV_NETWORK_LEVEL_2,
                                                         /* eCCTKPRIVLVL_NETSES */
   MX_NS ISceUserIdentityConfig::ePRIV_NETWORK_LEVEL_2,
                                                         /* eCCTKPRIVLVL_ALL */
};

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */
/***********************************************************************************
**  FUNCTION:   cfgCvtAddrToMx
**
**  PURPOSE:    Convert address to M5T format.
**
**  PARAMETERS:
**              pcCctk  - Address in CCTK format.
**
**
**  RETURNS:    URI in M5T format. 
**              NULL if address is invalid or its URI type is not supported by CCTK.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS IUri *cfgCvtAddrToMx( const BOS_UINT8 *pcCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtAddrToMx(%p)", pcCctk );

   MX_NS IUri *pRes = NULL;

   if ( !cfgIsAddrValid( pcCctk ) )
   {
      CCTK_TRACEI0( "cfg - cfgCvtAddrToMx: Invalid input address." );
      return pRes;
   }

   const char *pcAddr = reinterpret_cast< const char* >( pcCctk );
   mxt_result res = MX_NS CUriFactory::ParseUri( MX_NS IUri::eALLOW_SPECIAL_CHARS,
                                                 pcAddr,
                                                 pRes );
   if ( MX_RIS_F( res ) || pRes == NULL )
   {
      /* The address string does not contain an URI scheme or it is invalid */

#if ( CCTKCFG_TEL_MODE != CCTKCFG_TEL_DISABLED )
      /* FIXME: Insert digit map here. We treat all invalid address string as
      ** tel-URI for now.
      */
      MX_NS CTelUri *pTelUri = BRCM_NEW( MX_NS CTelUri );

      pcAddr = reinterpret_cast< const char* >( pcCctk );
      res = pTelUri->Parse( MX_NS IUri::eALLOW_SPECIAL_CHARS, pcAddr );
      if ( MX_RIS_F( res ) )
      {
         /* Invalid address string */
         CCTK_TRACEI1( "cfg - cfgCvtAddrToMx: Invalid address string (%d).",
                       res );
         
         BRCM_DELETE( pTelUri );
         pTelUri = NULL;
      }
      else
      {
         pRes = pTelUri;
      }
#endif
   }

#if ( CCTKCFG_TEL_MODE == CCTKCFG_TEL_DISABLED )
   /* Report error when telephone call support is disabled */
   if ( pRes != NULL && pRes->GetUriType() == MX_NS IUri::eTEL )
   {
      CCTK_TRACEI0( 
         "cfg - cfgCvtAddrToMx: Error - telephone call support is disabled." );

      BRCM_DELETE( pRes );
      pRes = NULL;
   }
#endif

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtIpAddrToMx
**
**  PURPOSE:    Convert BOS_IP_ADDRESS to M5T format.
**
**  PARAMETERS:
**              rCctk   - IP address in CCTK format.
**              rMx     - Socket address in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtIpAddrToMx( const BOS_IP_ADDRESS &rCctk, MX_NS CSocketAddr &rMx )
{
   CCTK_TRACED2( "cfg - cfgCvtIpAddrToMx(%p, %p)", &rCctk, &rMx );

   rMx.Reset();

   /* Retrieve IP address from BOS address */
   char cIpAddrStr[64];
   BOS_STATUS eBosStatus = 
      bosIpAddrGetStr( &rCctk, cIpAddrStr, sizeof( cIpAddrStr ) );
   if ( eBosStatus != BOS_STATUS_OK )
   {
      CCTK_TRACEI1( "cfg - cfgCvtIpAddrToMx: "
         "Error retrieving address from BOS address (%d).",
         eBosStatus );
      return eCFGSTATUS_INTERR;
   }

   /* Set the retrieved IP address in M5T format */
   mxt_result res = rMx.SetAddress( cIpAddrStr );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "cfg - cfgCvtIpAddrToMx: "
         "Error setting IPv4 socket address (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtIpPortToMx
**
**  PURPOSE:    Convert CCTKIPPORT to M5T format.
**
**  PARAMETERS:
**              rCctk   - IP-Port in CCTK format.
**              rMx     - Socket address in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtIpPortToMx( const CCTKIPPORT &rCctk, MX_NS CSocketAddr &rMx )
{
   CCTK_TRACED2( "cfg - cfgCvtIpPortToMx(%p, %p)", &rCctk, &rMx );

   CFGSTATUS eStatus = cfgCvtIpAddrToMx( rCctk.addr, rMx );
   if ( eStatus == eCFGSTATUS_SUCCESS )
   {
      rMx.SetPort( rCctk.uPort );
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtLocPortToMx
**
**  PURPOSE:    Convert CCTKLOCPORT to M5T format.
**
**  PARAMETERS:
**              rCctk   - Local port in CCTK format.
**              bSec    - True to convert SIPS port. False to convert SIP port.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
uint16_t cfgCvtLocPortToMx( const CCTKLOCPORT &rCctk, BOS_BOOL bSec )
{
   CCTK_TRACED2( "cfg - cfgCvtLocPortToMx(%p, %d)", &rCctk, bSec );

   uint16_t uRes = bSec ? rCctk.uSipsPort : rCctk.uSipPort;
   if ( uRes == 0 )
   {
      uRes = bSec ? 5061 : 5060;
   }

   return uRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtMethodFromMx
**
**  PURPOSE:    Convert to CCTK_METHOD_XXX from M5T format.
**
**  PARAMETERS:
**              eMx - Method in M5T format.
**
**
**  RETURNS:    CCTK_METHOD_XXX.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT32 cfgCvtMethodFromMx( MX_NS ESipMethod eMx )
{
   CCTK_TRACED1( "cfg - cfgCvtMethodFromMx(%d)", eMx );

   /* Internal validity check (for detecting the enum list size change) */
   BOS_SINT32 sLstSize = MX_NS eSIP_METHOD_UNKNOWN + 1;
   CCTK_ASSERT( sizeof( guMethodMap ) / sizeof( BOS_UINT32 ) == sLstSize );

   if ( eMx > MX_NS eSIP_METHOD_UNKNOWN )
   {
      CCTK_TRACEI0( "cfg - cfgCvtMethodFromMx: Invalid method parameter." );
      return CCTK_METHOD_NONE;
   }

   return guMethodMap[eMx];
}

/***********************************************************************************
**  FUNCTION:   cfgCvtDnsModeToMx
**
**  PURPOSE:    Convert CCTKDNSMODE to M5T format.
**
**  PARAMETERS:
**              eCctk   - DNS mode in CCTK format.
**
**
**  RETURNS:    DNS mode bitmap in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
int cfgCvtDnsModeToMx( CCTKDNSMODE eCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtDnsModeToMx(%d)", eCctk );

   /* Internal validity check (for detecting the enum list size change) */
   BOS_SINT32 sLstSize = eCCTKDNSMODE_NO_NAPTR_SRV + 1;
   CCTK_ASSERT( sizeof( gsDnsModeMap ) / sizeof( int ) == sLstSize );

   int sRes = MX_NS eNONE;
   if ( eCctk > eCCTKDNSMODE_NO_NAPTR_SRV )
   {
      CCTK_TRACEI0( "cfg - cfgCvtDnsModeToMx: Invalid DNS policy parameter." );
      return sRes;
   }

   sRes = gsDnsModeMap[eCctk];
   return sRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtHdrToMx
**
**  PURPOSE:    Convert CCTKHDR to M5T format.
**
**  PARAMETERS:
**              rCctk - Header in CCTK format.
**
**
**  RETURNS:    Header in M5T format.
**              NULL if header cannot be created.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CSipHeader *cfgCvtHdrToMx( const CCTKHDR &rCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtHdrToMx(%p)", &rCctk );

   MX_NS CSipHeader *pRes = NULL;

   if ( !cfgIsHdrValid( rCctk ) )
   {
      CCTK_TRACEI0( "cfg - cfgCvtHdrToMx: Invalid header config" );
      return NULL;
   }

   MX_NS CString *pHdrName = cfgCvtHdrNameToMx( rCctk );
   if ( pHdrName == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgCvtHdrToMx: Invalid header name config" );
      return NULL;
   }

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   MX_NS CRawHeader *pRawHdr = BRCM_NEW( MX_NS CRawHeader );
   CCTK_ASSERT( pRawHdr != NULL );
   if ( pRawHdr == NULL )
   {
      CCTK_TRACEI0( "cfg - cfgCvtHdrToMx: Unable to create raw header." );
      eStatus = eCFGSTATUS_NORSC;
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      pRes = 
         BRCM_NEW( MXD_GNS::CSipHeader )( 
            MXD_GNS::MxConvertSipHeader( *pHdrName ) );
      CCTK_ASSERT( pRes != NULL );
      if ( pRes == NULL )
      {
         CCTK_TRACEI0( "cfg - cfgCvtHdrToMx: Unable to create SIP header." );
         eStatus = eCFGSTATUS_NORSC;
      }
   }

   if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
   {
      mxt_result res;

      /* Set the header name in case if the header is unknown to the stack */
      if ( pRes->GetHeaderType() == MXD_GNS::eHDR_EXTENSION )
      {
         pRes->GetExtensionHeaderName() = *pHdrName;
      }

      /* Set the raw header */
      MX_NS CString hdrVal( reinterpret_cast< char* >( rCctk.pcValue ) );
      pRawHdr->GetBody() = hdrVal;
      pRawHdr->GetName() = *pHdrName;
      res = pRes->SetRawHeader( pRawHdr );
      pRawHdr = NULL;
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtHdrToMx: Error setting raw header (%d).", res );
         eStatus = eCFGSTATUS_BADPARM;
      }
   }

   BRCM_DELETE( pHdrName );
   pHdrName = NULL;

   /* Clean-ups on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      BRCM_DELETE( pRawHdr );
      pRawHdr = NULL;
      BRCM_DELETE( pRes );
      pRes = NULL;
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtHdrNameToMx
**
**  PURPOSE:    Convert CCTKHDR header name to M5T format.
**
**  PARAMETERS:
**              rCctk - Header in CCTK format.
**
**
**  RETURNS:    Header in M5T format.
**              NULL if header name cannot be converted.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CString *cfgCvtHdrNameToMx( const CCTKHDR &rCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtHdrNameToMx(%p)", &rCctk );

   MX_NS CString *pRes = NULL;

   switch ( rCctk.eType )
   {
   case eCCTKHDRTYPE_PRIVATE:
      {
         if ( rCctk.pcName == NULL )
         {
            CCTK_TRACEI0( 
               "cfg - cfgCvtHdrNameToMx: NULL private header name pointer" );
         }
         else
         {
            pRes = 
               BRCM_NEW( MX_NS CString )( 
                  reinterpret_cast< char* >( rCctk.pcName ) );
         }
      }
      break;
   case eCCTKHDRTYPE_CALL_INFO:
      {
         pRes = BRCM_NEW( MX_NS CString )( CFGCVT_HDRSTR_CALL_INFO );
      }
      break;
   case eCCTKHDRTYPE_WARNING:
      {
         pRes = BRCM_NEW( MX_NS CString )( CFGCVT_HDRSTR_WARNING );
      }
      break;
   default:
      {
         CCTK_TRACEI1( 
            "cfg - cfgCvtHdrNameToMx: Invalid header type (%d).", rCctk.eType );
      }
      break;
   }

   return pRes;
}

#if CCTKCFG_TLS
/***********************************************************************************
**  FUNCTION:   cfgCvtTlsCfgToMx
**
**  PURPOSE:    Convert CCTKTLSCFG to M5T format.
**
**  PARAMETERS:
**              rCctk      - TLS configuration in CCTK format.
**              rMxClient  - TLS client context in M5T format.
**              rMxServer  - TLS server context in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtTlsCfgToMx( const CCTKTLSCFG &rCctk, 
                            MX_NS CTlsContext &rMxClient,
                            MX_NS CTlsContext &rMxServer )
{
   CCTK_TRACED3( "cfg - cfgCvtTlsCfgToMx(%p, %p, %p)", 
                 &rCctk, 
                 &rMxClient, 
                 &rMxServer );

   mxt_result res;

   /* Configure client TLS context.
   ** Both SSLv3 and TLSv1 are supported by default.
   */

   /* Configure the allowed ciphers */
   if ( rCctk.pcCipher != NULL)
   {
      MX_NS CString cipherStr = reinterpret_cast< const char* >( rCctk.pcCipher );
      res = rMxClient.SetCiphers( cipherStr );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1(
            "cfg - cfgCvtTlsCfgToMx: Error setting ciphers (%d).",
            res );
         return eCFGSTATUS_BADPARM;
      }
   }

   /* Configure client peer authentication */
   rMxClient.SetPeerAuthentication( rCctk.bClientAuth == BOS_TRUE );

   /* Configure private key */
   MX_NS CBlob privateKeyBlob;
   res = privateKeyBlob.Append( 
      reinterpret_cast< const char* >( rCctk.pcPrivKey ) );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( 
         "cfg - cfgCvtTlsCfgToMx: Invalid private key (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   MX_NS CPrivateKey privateKey;
   res = privateKey.RestorePem( &privateKeyBlob, NULL, 0 );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1(
         "cfg - cfgCvtTlsCfgToMx: Error restoring private key (%d).",
         res );
      return eCFGSTATUS_BADPARM;
   }

   MX_NS CVector< MX_NS CCertificate > vecCerts;
   const BOS_UINT8 **ppcChain = NULL;

   /* Configure certificate chain */
   MX_NS CCertificateChain certChain;
   ppcChain = const_cast< const BOS_UINT8** >( rCctk.ppcCertChain );
   while ( *ppcChain != NULL )
   {
      MX_NS CBlob x509Blob;
      res = x509Blob.Append( reinterpret_cast< const char* >( *ppcChain ) );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Invalid certificate (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_INTERR;
      }

      MX_NS CCertificate cert;
      res = cert.RestorePem( &x509Blob );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Error restoring certificate (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_BADPARM;
      }

      /* Peek ahead if this is the last certificate (i.e. the local 
      ** certificate).
      */
      if ( *( ppcChain + 1 ) == NULL )
      {
          res = cert.SetPrivateKey( &privateKey );
          if ( MX_RIS_F( res ) )
          {
             CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
                "Error setting private key in local certificate (%p, %d).",
                ppcChain,
                res );
             return eCFGSTATUS_BADPARM;
          }
      }

      /* Insert certificate to the top of the certificate chain */
      res = vecCerts.Insert( 0, 1, cert );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Error inserting certificate to certificate vector (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_INTERR;
      }

      ppcChain++;
   }
      
   res = certChain.Extend( &vecCerts );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "cfg - cfgCvtTlsCfgToMx: "
         "Error inserting certificate to certificate chain (%d).",
         res );
      return eCFGSTATUS_INTERR;
   }

   rMxClient.SetCertificateChain( &certChain );

   /* Configure trusted certificate list */
   vecCerts.EraseAll();
   ppcChain = const_cast< const BOS_UINT8** >( rCctk.ppcTrustedCerts );
   while ( *ppcChain != NULL )
   {
      MX_NS CBlob x509Blob;
      res = x509Blob.Append( reinterpret_cast< const char* >( *ppcChain ) );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Invalid trusted certificate (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_INTERR;
      }

      MX_NS CCertificate cert;
      res = cert.RestorePem( &x509Blob );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Error restoring trusted certificate (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_BADPARM;
      }

      /* Append trusted certificate to the certificate chain */
      res = vecCerts.Append( cert );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI2( "cfg - cfgCvtTlsCfgToMx: "
            "Error appended trusted certificate to certificate chain (%p, %d).",
            ppcChain,
            res );
         return eCFGSTATUS_INTERR;
      }

      ppcChain++;
   }
   rMxClient.SetTrustedCertificates( &vecCerts );

   /* Configure Diffie-Hellman parameters for DH key exchange */
   BOS_UINT32 uDhParmLen = 256;  /* Default prime length */
   if ( rCctk.preGenDhPrime.uLen > 0 )
   {
      uDhParmLen = rCctk.preGenDhPrime.uLen;
   }

   MX_NS CDiffieHellman dhParm;
   if ( rCctk.preGenDhPrime.puPrime == NULL )
   {
      /* Generate prime at real time */
      res = dhParm.GeneratePrime( uDhParmLen );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtTlsCfgToMx: "
            "Error generating prime at real time (%d).",
            res );
         return eCFGSTATUS_INTERR;
      }

      res = dhParm.GeneratePublicAndPrivateKeys();
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtTlsCfgToMx: "
            "Error generating private and public keys at real time (%d).",
            res );
         return eCFGSTATUS_INTERR;
      }
   }
   else
   {
      /* Use the pre-generated prime, default generator (2) and 
      ** convert the prime length from bits to bytes.
      */
      res = dhParm.SetParameters( rCctk.preGenDhPrime.puPrime, 
                                  uDhParmLen / 8, 
                                  2, 
                                  NULL, 
                                  0, 
                                  NULL, 
                                  0);
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtTlsCfgToMx: "
            "Error setting Diffie-Hellman parameters (%d).",
            res );
         return eCFGSTATUS_BADPARM;
      }

      res = dhParm.GeneratePublicAndPrivateKeys();
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtTlsCfgToMx: "
            "Error generating private and public keys (%d).",
            res );
         return eCFGSTATUS_INTERR;
      }
   }
   rMxClient.SetEphemeralDiffieHellman( &dhParm );

   /* Configure server TLS context (same settings as the TLS client) */
   rMxServer = rMxClient;
   rMxServer.SetPeerAuthentication( rCctk.bServerAuth == BOS_TRUE );

   return eCFGSTATUS_SUCCESS;
}
#endif /* #if CCTKCFG_TLS */

#if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL )
/***********************************************************************************
**  FUNCTION:   cfgCvtTraceGrpFromMx
**
**  PURPOSE:    Convert trace group from M5T format to CCTK format.
**
**  PARAMETERS:
**              uMx - Trace group in M5T format.
**
**
**  RETURNS:    Trace group in CCTK format.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT16 cfgCvtTraceGrpFromMx( uint32_t uMx )
{
   BOS_UINT16 uRes;

   if ( uMx == CCTK_TRACE_ID )
   {
      uRes = CCTK_TRACEGRP_CCTK;
   }
   else
   {
      uRes = ( CCTK_TRACEGRP_ALL ^ CCTK_TRACEGRP_CCTK );
   }

   return uRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtTraceLvlFromMx
**
**  PURPOSE:    Convert trace level from M5T format to CCTK format.
**
**  PARAMETERS:
**              eMx - Trace level in M5T format.
**
**
**  RETURNS:    Trace level in CCTK format.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT16 cfgCvtTraceLvlFromMx( EMxTraceLevel eMx )
{
   BOS_UINT16 uRes = CCTK_TRACELVL_NONE;
   
   const TRACEMAP *pMap = &gTraceLvlMap[0];
   while ( pMap->sMx != static_cast<int>(0xFFFFFFFF) )
   {
      if ( UTIL_BMP_IS_ENABLED( eMx, pMap->sMx ) )
      {
         uRes |= pMap->uCctk;
      }
      pMap++;
   }

   return uRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtTraceLvlToMx
**
**  PURPOSE:    Convert trace level to M5T format.
**
**  PARAMETERS:
**              uCctk   - Trace level in CCTK format.
**
**
**  RETURNS:    Trace level in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
EMxTraceLevel cfgCvtTraceLvlToMx( BOS_UINT16 uCctk )
{
   int sRes = eLEVEL_NONE;

   const TRACEMAP *pMap = &gTraceLvlMap[0];
   while ( pMap->uCctk != 0xFFFF )
   {
      if ( UTIL_BMP_IS_ENABLED( uCctk, pMap->uCctk ) )
      {
         sRes |= pMap->sMx;
      }
      pMap++;
   }

   return static_cast< EMxTraceLevel >( sRes );
}
#endif /* #if ( CCTKCFG_TRACE_MODE == CCTKCFG_TRACE_ALL ) */

/***********************************************************************************
**  FUNCTION:   cfgCvtTransportToMx
**
**  PURPOSE:    Convert transport support bitmap to M5T format.
**
**  PARAMETERS:
**              uCctk   - Transport support bitmap in CCTK format.
**
**
**  RETURNS:    Transport support bitmap in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
int cfgCvtTransportToMx( BOS_UINT32 uCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtTransportToMx(%lu)", uCctk );

   int sRes = MX_NS eINVALID;

   const TRANSPORTMAP *pMap = &gTransportMap[0];
   while ( pMap->uCctk != 0xFFFFFFFF )
   {
      if ( UTIL_BMP_IS_ENABLED( uCctk, pMap->uCctk ) )
      {
         sRes |= pMap->sMx;
      }
      pMap++;
   }

   return sRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtNaiModeToMx
**
**  PURPOSE:    Convert network-asserted identity mode to M5T format.
**
**  PARAMETERS:
**              rCctk - Network-asserted identity config in CCTK format.
**
**
**  RETURNS:    Network-asserted identity mode in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS ISceUserIdentityConfig::ENetworkAssertedIdMode
               cfgCvtNaiModeToMx( const CCTKNAICFG &rCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtNaiModeToMx(%p)", &rCctk );

   MX_NS ISceUserIdentityConfig::ENetworkAssertedIdMode eRes =
      MX_NS ISceUserIdentityConfig::eNAIMODE_ENABLE;

   if ( rCctk.svr.eMode == eCCTKNAISVRMODE_WHTLST )
   {
      eRes = MX_NS ISceUserIdentityConfig::eNAIMODE_ENFORCE;
   }
   else if ( rCctk.svr.eMode == eCCTKNAISVRMODE_IGNORED &&
             rCctk.cli.eMode == eCCTKNAICLTMODE_NET )
   {
      eRes = MX_NS ISceUserIdentityConfig::eNAIMODE_DISABLE;
   }

   return eRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtNameAddrToMx
**
**  PURPOSE:    Convert name-addr to M5T format.
**
**  PARAMETERS:
**              rCctkSipHost  - SIP host-port of name-addr in CCTK format.
**              rCctkSipName  - SIP name information in CCTK format.
**
**
**  RETURNS:    Name-Addr in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CNameAddr *cfgCvtNameAddrToMx( const CCTKSIPHOST &rCctkSipHost,
                                     const CCTKSIPNAME &rCctkSipName )
{
   CCTK_TRACED2( "cfg - cfgCvtNameAddrToMx(%p, %p)",
                 &rCctkSipHost,
                 &rCctkSipName );

   return cfgCvtNameAddrToMx( rCctkSipHost.hostPort, 
                              rCctkSipHost.bSec, 
                              rCctkSipName );
}

/***********************************************************************************
**  FUNCTION:   cfgCvtNameAddrToMx
**
**  PURPOSE:    Convert name-addr to M5T format.
**
**  PARAMETERS:
**              rCctkHostPort - Host-port of name-addr in CCTK format.
**              bCctkSec      - True to indicate a SIPS name-addr.
**              rCctkSipName  - SIP name information in CCTK format.
**
**
**  RETURNS:    Name-Addr in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CNameAddr *cfgCvtNameAddrToMx( const CCTKHOSTPORT &rCctkHostPort,
                                     BOS_BOOL bCctkSec,
                                     const CCTKSIPNAME &rCctkSipName )
{
   CCTK_TRACED3( "cfg - cfgCvtNameAddrToMx(%p, %d, %p)",
                 &rCctkHostPort,
                 bCctkSec,
                 &rCctkSipName );

   MX_NS CNameAddr *pRes = NULL;

   /* Convert SIP-URI */
   MX_NS CSipUri *pUri =
      cfgCvtSipUriToMx( rCctkHostPort, bCctkSec, rCctkSipName.pcUsrName );
   if ( pUri != NULL )
   {
      pRes = BRCM_NEW( MX_NS CNameAddr );

      /* Configure display name */
      pRes->SetUri( pUri,
                    reinterpret_cast< const char* >( rCctkSipName.pcDisplay ) );
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtPrackCfgToMx
**
**  PURPOSE:    Convert PRACK config to M5T format.
**
**  PARAMETERS:
**              uCctk   - PRACK config in CCTK format.
**
**
**  RETURNS:    PRACK config in M5T format. 
**
**  NOTES:      None.
***********************************************************************************/
MX_NS IUaSspCallConfig::ERel1xxConfig cfgCvtPrackCfgToMx( CCTKPRACK eCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtPrackCfgToMx(%d)", eCctk );

   MX_NS IUaSspCallConfig::ERel1xxConfig ePrackCfg;

   switch ( eCctk )
   {
   case eCCTKPRACK_SUPPORTED:
      ePrackCfg = MX_NS IUaSspCallConfig::eREL1xx_SUPPORT;
      break;
   case eCCTKPRACK_REQUIRED:
      ePrackCfg = MX_NS IUaSspCallConfig::eREL1xx_REQUIRE;
      break;
   case eCCTKPRACK_DISABLED:
   default:
      ePrackCfg = MX_NS IUaSspCallConfig::eREL1xx_DISABLE;
      break;
   }

   return ePrackCfg;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtConnectedIdCfgToMx
**
**  PURPOSE:    Convert Connected Id config to M5T format.
**
**  PARAMETERS:
**              eCctk   - Connected Id config in CCTK format.
**
**
**  RETURNS:    Connected Id config in M5T format. 
**
**  NOTES:      None.
***********************************************************************************/
MX_NS IUaSspCallConfig::EConnectedIdConfig 
   cfgCvtConnectedIdCfgToMx( CCTKCONNECTEDID eCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtConnectedIdCfgToMx(%d)", eCctk );

   /* Internal validity check (for detecting the enum list size change) */
   BOS_SINT32 sLstSize = eCCTKCONNECTEDID_DISABLED + 1;
   CCTK_ASSERT( sizeof( geCnctdIdLvlMap ) / 
                sizeof( MX_NS IUaSspCallConfig::EConnectedIdConfig ) == 
                sLstSize );

   if ( eCctk > eCCTKCONNECTEDID_DISABLED )
   {
      CCTK_TRACEI0(
         "cfg - cfgCvtConnectedIdCfgToMx: Invalid connected id config." );
      return MX_NS IUaSspCallConfig::eCONNECTED_ID_DISABLE;
   }

   return geCnctdIdLvlMap[eCctk];
}

/***********************************************************************************
**  FUNCTION:   cfgCvtPrivLvlToMx
**
**  PURPOSE:    Convert privacy level to M5T format.
**
**  PARAMETERS:
**              eCctk         - Privacy level in CCTK format.
**              bCritical     - True to indicate the privacy level is critical.
**              bIdRequired   - True to indicate that "id" is required in the
**                              privacy header.
**
**  RETURNS:    Privacy mode in M5T format. 
**
**  NOTES:      None.
***********************************************************************************/
MX_NS ISceUserIdentityConfig::EPrivacyMode
   cfgCvtPrivLvlToMx( CCTKPRIVLVL eCctk, BOS_BOOL bCritical, BOS_BOOL bIdRequired)
{
   CCTK_TRACED3(
      "cfg - cfgCvtPrivLvlToMx(%d, %d, %d)", eCctk, bCritical, bIdRequired );

   /* Internal validity check (for detecting the enum list size change) */
   BOS_SINT32 sLstSize = eCCTKPRIVLVL_ALL + 1;
   CCTK_ASSERT( sizeof( gePrivLvlMap ) / 
                sizeof( MX_NS ISceUserIdentityConfig::EPrivacyMode ) == 
                sLstSize );

   MX_NS ISceUserIdentityConfig::EPrivacyMode eRes 
      = MX_NS ISceUserIdentityConfig::ePRIV_USE_USER_CONFIG;
   if ( eCctk > eCCTKPRIVLVL_ALL )
   {
      CCTK_TRACEI0( "cfg - cfgCvtPrivLvlToMx: Invalid privacy level parameter." );
      return eRes;
   }
   else if ( bIdRequired && ( eCctk <= eCCTKPRIVLVL_LOCAL ) )
   {
      eCctk = eCCTKPRIVLVL_NETID;
   }
   else if ( !bCritical && ( eCctk > eCCTKPRIVLVL_LOCAL ) )
   {
      eCctk = eCCTKPRIVLVL_NETIDNC;
   }

   return gePrivLvlMap[eCctk];
}


/***********************************************************************************
**  FUNCTION:   cfgCvtRealmToMx
**
**  PURPOSE:    Convert realm structure to M5T format.
**
**  PARAMETERS:
**              rCctk         - Realm structure in CCTK format.
**              rMxRealm      - Realm in M5T format.
**              rMxUser       - User name in M5T format.
**              rMxPwd        - Password in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtRealmToMx( const CCTKRLM &rCctk,
                           MX_NS CString &rMxRealm,
                           MX_NS CString &rMxUser,
                           MX_NS CString &rMxPwd )
{
   CCTK_TRACED1( "cfg - cfgCvtRealmToMx(%p)", 
                 &rCctk );

   return cfgCvtRealmToMx( rCctk.pcRlm, 
                           rCctk.pcUsrName, 
                           rCctk.pcPwd, 
                           rMxRealm, 
                           rMxUser, 
                           rMxPwd );
}

/***********************************************************************************
**  FUNCTION:   cfgCvtRealmToMx
**
**  PURPOSE:    Convert realm structure to M5T format.
**
**  PARAMETERS:
**              pcCctkRealm   - Realm in CCTK format.
**              pcCctkUser    - User name in CCTK format.
**              pcCctkPwd     - Password in CCTK format.
**              rMxRealm      - Realm in M5T format.
**              rMxUser       - User name in M5T format.
**              rMxPwd        - Password in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtRealmToMx( const BOS_UINT8 *pcCctkRealm,
                           const BOS_UINT8 *pcCctkUser,
                           const BOS_UINT8 *pcCctkPwd,
                           MX_NS CString &rMxRealm,
                           MX_NS CString &rMxUser,
                           MX_NS CString &rMxPwd )
{
   CCTK_TRACED3( "cfg - cfgCvtRealmToMx(%p, %p, %p)", 
                 &pcCctkRealm, 
                 &pcCctkUser, 
                 &pcCctkPwd );

   rMxRealm.EraseAll();
   rMxUser.EraseAll();
   rMxPwd.EraseAll();

   if ( ( pcCctkRealm == NULL || pcCctkRealm[0] == '\0' ) ||
        ( pcCctkUser == NULL || pcCctkUser[0] == '\0' ) ||
        ( pcCctkPwd == NULL ) )
   {
      CCTK_TRACEI0( "cfg - cfgCvtRealmToMx: Invalid credentials." );
      return eCFGSTATUS_BADPARM; 
   }

   rMxRealm = reinterpret_cast< const char * >( pcCctkRealm );
   rMxUser = reinterpret_cast< const char * >( pcCctkUser );
   rMxPwd = reinterpret_cast< const char * >( pcCctkPwd );

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtRouteToMx
**
**  PURPOSE:    Convert routing information to M5T format.
**
**  PARAMETERS:
**              cctk   - Routing information in CCTK format.
**
**
**  RETURNS:    Route list in M5T format. 
**              NULL if route list is empty or conversion fails.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CSipHeader *cfgCvtRouteToMx( const CCTKROUTE cctk[] )
{
   CCTK_TRACED1( "cfg - cfgCvtRouteToMx(%p)", cctk );

   CFGSTATUS eStatus = eCFGSTATUS_SUCCESS;

   MX_NS CSipUri::ESecurityFlag eSecFlag;
   MX_NS CSipHeader *pRouteHdr = NULL, *pRes = NULL;
   MX_NS CSipUri *pRouteUri = NULL;
   BOS_UINT32 uIdx;
   for ( uIdx = 0; uIdx < CCTKCFG_MAX_OBPROXY; uIdx++ )
   {
      if ( !cfgIsSipHostEnabled( cctk[uIdx].addr ) )
      {
         /* The end of route set. Stop processing anymore routes. */
         break;
      }

      pRouteUri = BRCM_NEW( MX_NS CSipUri );

      /* Configure SIPS */
      eSecFlag = cctk[uIdx].addr.bSec ? MX_NS CSipUri::eSECURE :
                                               MX_NS CSipUri::eUNSECURE;
      pRouteUri->SetSecured( eSecFlag );

      /* Convert the host-port portion of the route URI */
      eStatus = cfgCvtHostPortToMx( cctk[uIdx].addr.hostPort, 
                                    pRouteUri->GetHostPort() );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtRouteToMx: "
            "Error converting host-port portion of route URI (%d).", 
            eStatus );
         break;
      }

      /* Insert loose route parameter if enabled */
      if ( cctk[uIdx].bLr )
      {
         /* Create loose routing parameter */
         MX_NS CGenericParam *pLrParm = BRCM_NEW( MX_NS CGenericParam )
                                        ( MX_NS CGenericParam::eCS_SIPURI_PARAM );
         pLrParm->GetName() = MX_NS szURIPARAM_LR;

         /* Add the parameter to the parameter list */
         MX_NS CGenParamList *pRouteUriParm = BRCM_NEW( MX_NS CGenParamList );
         pRouteUriParm->Append( pLrParm );
         pLrParm = NULL;
         pRouteUri->SetParamList( pRouteUriParm );
         pRouteUriParm = NULL;
      }

      /* Create the Route header and set it with the configured URI */
      pRouteHdr = BRCM_NEW( MX_NS CSipHeader )( MX_NS eHDR_ROUTE );
      pRouteHdr->GetRoute().SetUri( pRouteUri );
      pRouteUri = NULL;

      /* Append the route header to the route list */
      if ( pRes == NULL )
      {
         pRes = pRouteHdr;
      }
      else
      {
         mxt_result res = pRes->AppendNextHeader( pRouteHdr );
         CCTK_ASSERT( MX_RIS_S( res ) );
         if ( MX_RIS_F( res ) )
         {
            CCTK_TRACEI2( "cfg - cfgCvtRouteToMx: "
               "Error appending route URI (%d, %lu).", 
               res,
               uIdx );
            eStatus = eCFGSTATUS_INTERR;
            break;
         }
      }
      pRouteHdr = NULL;
   }

   /* Clean up on failure */
   if ( CFGSTATUS_IS_FAIL( eStatus ) )
   {
      BRCM_DELETE( pRouteUri );
      pRouteUri = NULL;
      BRCM_DELETE( pRouteHdr );
      pRouteHdr = NULL;
      BRCM_DELETE( pRes );
      pRes = NULL;
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtRouteLstToMx
**
**  PURPOSE:    Convert route list to M5T format.
**
**  PARAMETERS:
**              uCctk   - Route list in CCTK format.
**
**
**  RETURNS:    Route list in M5T format. 
**              NULL if route list is empty or conversion fails.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CSipHeader *cfgCvtRouteLstToMx( const CCTKROUTELST &rCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtRouteLstToMx(%p)", &rCctk );

   /* FIXME: Preloaded route set only. For local policy, we append the first route 
   ** in the Route header to the outgoing request. Make sure to remove the Route 
   ** header afterwards at output controlling service.
   */
   return cfgCvtRouteToMx( rCctk.route );
}

/***********************************************************************************
**  FUNCTION:   cfgCvtSesRefreshCfgToMx
**
**  PURPOSE:    Convert session refresh config to M5T format.
**
**  PARAMETERS:
**              eCctk   - Session refresh config in CCTK format.
**
**
**  RETURNS:    Session refresh config in M5T format. 
**
**  NOTES:      None.
***********************************************************************************/
MX_NS IUaSspCallConfig::ESessionRefreshConfig
               cfgCvtSesRefreshCfgToMx( CCTKSESTMRMODE eCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtSesRefreshCfgToMx(%d)", eCctk );

   MX_NS IUaSspCallConfig::ESessionRefreshConfig eSesRefreshCfg;
   switch ( eCctk )
   {
   case eCCTKSESTMRMODE_UPDATE:
      eSesRefreshCfg = MX_NS IUaSspCallConfig::eREFRESH_AUTOMATIC;
      break;
   case eCCTKSESTMRMODE_INVITE:
   default:
      eSesRefreshCfg = MX_NS IUaSspCallConfig::eREFRESH_INVITE;
      break;
   }

   return eSesRefreshCfg;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtSipUriToMx
**
**  PURPOSE:    Convert SIP-URI to M5T format.
**
**  PARAMETERS:
**              rCctkHostPort - Host-port of SIP-URI in CCTK format.
**              bCctkSec      - True to indicate a SIPS URI.
**              pcCctkUsrName - User name of SIP-URI in CCTK format.
**
**
**  RETURNS:    SIP-URI in M5T format.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CSipUri *cfgCvtSipUriToMx( const CCTKHOSTPORT &rCctkHostPort,
                                 BOS_BOOL bCctkSec,
                                 const BOS_UINT8 *pcCctkUsrName )
{
   CCTK_TRACED3( "cfg - cfgCvtSipUriToMx(%p, %d, %p)",
                 &rCctkHostPort,
                 bCctkSec,
                 pcCctkUsrName );

   MX_NS CSipUri *pRes = NULL;

   if ( cfgIsHostPortEnabled( rCctkHostPort ) )
   {
      pRes = BRCM_NEW( MX_NS CSipUri );

      /* Configure the host-port portion of the SIP URI */
      CFGSTATUS eStatus = 
         cfgCvtHostPortToMx( rCctkHostPort, pRes->GetHostPort() );
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtSipUriToMx: "
            "Error setting host-port portion of a SIP-URI (%d)",
            eStatus );
      }

      if ( CFGSTATUS_IS_SUCCESS( eStatus ) )
      {
         /* Configure user name */
         if ( pcCctkUsrName != NULL )
         {
            pRes->GetUser() = reinterpret_cast< const char* >( pcCctkUsrName );
         }

         /* Configure SIPS */
         MX_NS CSipUri::ESecurityFlag eSecFlag;
         eSecFlag = bCctkSec ? MX_NS CSipUri::eSECURE : MX_NS CSipUri::eUNSECURE;
         pRes->SetSecured( eSecFlag );
      }

      /* Cleanups on failure */
      if ( CFGSTATUS_IS_FAIL( eStatus ) )
      {
         BRCM_DELETE( pRes );
         pRes = NULL;
      }
   }

   return pRes;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtSockAddrFromMx
**
**  PURPOSE:    Convert socket address from M5T format.
**
**  PARAMETERS:
**              rMxSockAddr   - Socket address in M5T format.
**              rCctkIpPort   - IP address with port in CCTK format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtSockAddrFromMx( const MX_NS CSocketAddr &rMxSockAddr,
                                CCTKIPPORT &rCctkIpPort )
{
   CCTK_TRACED2( "cfg - cfgCvtSockAddrFromMx(%p, %p)",
                 &rMxSockAddr,
                 &rCctkIpPort );

   /* Retrieve the socket IP address */
   BOS_STATUS sRes =
      bosIpAddrCreateFromStr( rMxSockAddr.GetAddress( false ).CStr(),
                              &rCctkIpPort.addr );
   if ( sRes != BOS_STATUS_OK )
   {
      CCTK_TRACEI1( "cfg - cfgCvtSockAddrFromMx: Invalid socket IP address (%d).",
                    sRes );
      return eCFGSTATUS_BADPARM;
   }

   /* Retrieve the socket port */
   rCctkIpPort.uPort = rMxSockAddr.GetPort();

   return eCFGSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   cfgCvtSvcRouteToMx
**
**  PURPOSE:    Convert Service-Route header list to M5T format.
**
**  PARAMETERS:
**              rCctk   - Route list in CCTK format.
**
**
**  RETURNS:    Route list in M5T format. 
**              NULL if route list is empty or conversion fails.
**
**  NOTES:      None.
***********************************************************************************/
MX_NS CSipHeader *cfgCvtSvcRouteToMx( const CCTKSVCROUTE &rCctk )
{
   CCTK_TRACED1( "cfg - cfgCvtSvcRouteToMx(%p)", &rCctk );

   return cfgCvtRouteToMx( rCctk.route );
}

/***********************************************************************************
**  FUNCTION:   cfgCvtHostPortToMx
**
**  PURPOSE:    Convert host-port to M5T format.
**
**  PARAMETERS:
**              rCctk   - Host-port in CCTK format.
**              rMx     - Host-port in M5T format.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      None.
***********************************************************************************/
CFGSTATUS cfgCvtHostPortToMx( const CCTKHOSTPORT &rCctk, MX_NS CHostPort &rMx )
{
   CCTK_TRACED2( "cfg - cfgCvtHostPortToMx(%p, %p)", &rCctk, &rMx );

   rMx.Reset();

   /* Return success if host-port is disabled */
   if ( !cfgIsHostPortEnabled( rCctk ) )
   {
      return eCFGSTATUS_SUCCESS;
   }

   /* Insert [] (if not exist) for IPv6 address */
   if ( cfgIsHostPortIpv6( rCctk ) && rCctk.pcAddr[0] != '[' )
   {
      MX_NS CString hostAddr;
      mxt_result res =
         hostAddr.Format( 0,
                          "[%s]",
                          reinterpret_cast< const char* >( rCctk.pcAddr ) );
      CCTK_ASSERT( MX_RIS_S( res ) );
      if ( MX_RIS_F( res ) )
      {
         CCTK_TRACEI1( "cfg - cfgCvtHostPortToMx: Error setting host address (%d).",
                       res );
         return eCFGSTATUS_BADPARM;
      }

      rMx.SetHost( hostAddr );
   }
   else
   {
      rMx.SetHost( reinterpret_cast< const char* >( rCctk.pcAddr ) );
   }
   
   rMx.GetPort() = rCctk.uPort;

   return eCFGSTATUS_SUCCESS;
}
