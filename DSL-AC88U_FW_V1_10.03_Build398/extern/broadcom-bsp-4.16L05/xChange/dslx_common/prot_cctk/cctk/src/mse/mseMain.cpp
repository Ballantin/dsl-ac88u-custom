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
**      This file implements the MSE SDP Session Class.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <mseApi.h>
#include <mseMain.h>

#if CCTKCFG_REMOTE_USER_CONTROL
#include <cfgApi.h>   /* needed for the RG NAT CTL functions */
#endif

#if CCTKCFG_ICE_LITE_SUPPORT
#include <cfgApi.h>
#include <ice.h>
#include <nsIceApi.h>
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

MX_NAMESPACE_START( MXD_GNS )

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* Loopback type bitmap */
#define MSELBTYPE_PKTLB    0x00000001
#define MSELBTYPE_MEDIALB  0x00000002
#define MSELBTYPE_STARTLB  0x00000004

#define NATPROT_UDP 0
#define NATPROT_TCP 1

#define MSEEMSTRMINDXMSK   0x80000000

#define MSE_PTIMELEN		10
#define MSE_BWATTRLEN		32
#define MSE_MODELEN		32
#define MSE_MODEVALUELEN	10
#define MSE_CRYPTOTAGLEN	2

/* SDP tags */
static const char cMptimeStr[] = "mptime";               /* a=mptime */
static const char cMaxPtimeStr[] = "maxptime";           /* a=maxptime */
static const char cMaxPrateStr[] = "maxprate";           /* a=maxprate */
static const char cTiasStr[] = "TIAS";                   /* b=TIAS, RFC 3890 */
static const char cAsStr[] = "AS";                       /* b=AS, RFC 3890 */
static const char cRsStr[] = "RS";                       /* b=RS, RFC 3556 */
static const char cRrStr[] = "RR";                       /* b=RR, RFC 3556 */
static const char cRrZeroStr[] = "RR:0";                 /* b=RR:0, RFC 3556 */
static const char cQuadZerosStr[] = "0.0.0.0";           /* Quad-zero m= line */
static const char cT38Str[] = "t38";                     /* T.38 */
static const char cT38XferTCFStr[] = "transferredTCF";   /* T.38 */
static const char cT38UdpRedStr[] = "t38UDPRedundancy";  /* T.38 */
static const char cT38UdpFECStr[] = "t38UDPFEC";         /* T.38 */
static const char cRtcpXrStr[] = "rtcp-xr";              /* RTCP-XR, RFC 3611 */
static const char cVoipMetricStr[] = "voip-metrics";     /* RTCP-XR, RFC 3611 */
static const char cDftSesNameStr[] = "-";                /* RFC 3264 */
static const char cKeyMethodStr[] = "inline";            /* RFC 4568 */
static const char cAmrWbModeSet[] = "mode-set";          /* RFC 3267 */

static const char cAmrModeSet[] = "mode-set";            /* RFC 3267 */
static const char cAmrOctetAlign[] = "octet-align";      /* RFC 4865 */
static const char cAmrModeChangePeriod[] = "mode-change-period";     /* RFC4867 */
static const char cAmrModeChangeNeighbor[] = "mode-change-neighbor"; /* RFC4867 */
static const char cAmrModeChangeCapability[] = "mode-change-capability"; /* RFC4867 */
static const char cAmrMaxRed[] = "max-red";              /* RFC4867 */

/* SDP tags - draft-ietf-mmusic-media-loopback-08 */
static const char cLbTypeStr[] = "loopback";
static const char cLbTypePktLbStr[] = "rtp-pkt-loopback";
static const char cLbTypeMediaLbStr[] = "rtp-media-loopback";
static const char cLbTypeStartLbStr[] = "rtp-start-loopback";
static const char cLbModeSrcStr[] = "loopback-source";
static const char cLbModeMirrorStr[] = "loopback-mirror";

/* Silence suppression parameter (ref: RFC3108) */
static const char cSilSuppNameStr[] = "silenceSupp";
static const char cSilSuppOnStr[] = "on - - - -";
static const char cSilSuppOffStr[] = "off - - - -";

/* iLBC mode parameter (ref: RFC3952) */
static const char cIlbcMode20msStr[] = "mode=20";
static const char cIlbcMode30msStr[] = "mode=30";

static const BOS_UINT32 uIpAddrStrSize = 64;
static const BOS_UINT32 uSdpVerStrSize = 16;
static const BOS_UINT32 uMaxKeysaltLength = 100;
/* ---- Private Variables ------------------------------------------------------- */

/* CCTK-M5T codec type mapping */
static MSECODECMAP mseCodecMap[] =
{
   /* Audio map - Narrowband codec */
   { eCCTKCODEC_PCMU,    CSdpParser::ePCMU,            8000,  80, 10 },
   { eCCTKCODEC_PCMA,    CSdpParser::ePCMA,            8000,  80, 10 },
   { eCCTKCODEC_G7231,   CSdpParser::eG723,            8000,  24, 30 },
   { eCCTKCODEC_G726_16, CSdpParser::eG72616,          8000,  20, 10 },
   { eCCTKCODEC_G726_24, CSdpParser::eG72624,          8000,  30, 10 },
   { eCCTKCODEC_G726_32, CSdpParser::eG72632,          8000,  40, 10 },
   { eCCTKCODEC_G726_40, CSdpParser::eG72640,          8000,  50, 10 },
   { eCCTKCODEC_G728,    CSdpParser::eG728,            8000,  20, 10 },
   { eCCTKCODEC_G729,    CSdpParser::eG729,            8000,  10, 10 },
   { eCCTKCODEC_G729E,   CSdpParser::eG729E,           8000,  15, 10 },
   { eCCTKCODEC_ILBC,    CSdpParser::eILBC,            8000,  38, 20 },
   { eCCTKCODEC_BV16,    CSdpParser::eBV16,            8000,  20, 10 },
   { eCCTKCODEC_GSMAMR,  CSdpParser::eAMR,             8000,  32, 20 },
   { eCCTKCODEC_GSMEFR,  CSdpParser::eEFR,             8000,  32, 20 },

   /* Audio map - Wideband codec */
   /* Note for G.722, 8kHz is used even though it is a wideband (16kHz) codec.
   ** See RFC 3551 for reason.
   */
   { eCCTKCODEC_BV32,    CSdpParser::eBV32,            16000, 40, 10 },
   { eCCTKCODEC_G722,    CSdpParser::eG722,            8000,  80, 10 },
   { eCCTKCODEC_G722_48, CSdpParser::eG72248,          8000,  80, 10 },
   { eCCTKCODEC_G722_56, CSdpParser::eG72256,          8000,  80, 10 },
   { eCCTKCODEC_G722_64, CSdpParser::eG72264,          8000,  80, 10 },
   { eCCTKCODEC_G7221,   CSdpParser::eG7221,           16000, 40, 30 },
   { eCCTKCODEC_AMRWB,   CSdpParser::eAMR_WB,          16000, 61, 20 },
   { eCCTKCODEC_LPCMWB,  CSdpParser::eL16_2,           16000, 320, 10 },

#if CCTKCFG_VIDEO
   /* Video map */
   { eCCTKCODEC_H263,    CSdpParser::eH263,            90000, 0, 0 },
   { eCCTKCODEC_H264,    CSdpParser::eH264,            90000, 0, 0 },
   { eCCTKCODEC_MPEG4,   CSdpParser::eMPEG4,           90000, 0, 0 },
#endif
#if CCTKCFG_RED
   { eCCTKCODEC_RED,     CSdpParser::eRED,             8000, 0, 10 },
#endif

   /* End of map */
   { eCCTKCODEC_MAX, CSdpParser::eUNKNOWN_RTP_COMPRESSION_ALGORITHM, 0, 0, 0 }
};

#if CCTKCFG_SRTP
/* Crypto enum-length mapping */
static const CCTKKEYLENMAP cctkKeyLen[] =
{
   { eCCTKCSUITE_AES_CM_128_HMAC_SHA1_80, 30 },
   { eCCTKCSUITE_AES_CM_128_HMAC_SHA1_32, 30 },
   { eCCTKCSUITE_F8_128_HMAC_SHA1_80,     30 },
   { eCCTKCSUITE_MAX,                     0 }
};

/* Crypto enum-text mapping */
static const CCTKCRYPTOMAP cctkCryptoSuite[] =
{
   { eCCTKCSUITE_AES_CM_128_HMAC_SHA1_80, CCTK_STR( "AES_CM_128_HMAC_SHA1_80" ) },
   { eCCTKCSUITE_AES_CM_128_HMAC_SHA1_32, CCTK_STR( "AES_CM_128_HMAC_SHA1_32" ) },
   { eCCTKCSUITE_F8_128_HMAC_SHA1_80,     CCTK_STR( "F8_128_HMAC_SHA1_80" ) },
   { eCCTKCSUITE_MAX,                     NULL }
};
#endif

/* Telephone-Event event id map (RFC 4733) */
/* Must appear in the same order as the CCTKNTE_XXX bitmap */
static const BOS_UINT8 mseNteMap[] =
{
   0,    /* CCTKNTE_DTMF (DTMF group) */
   16,   /* CCTKNTE_FLASH */
   64,   /* CCTKNTE_OFFHOOK */
   65,   /* CCTKNTE_ONHOOK */
   66,   /* CCTKNTE_DIALTONE */
   70,   /* CCTKNTE_RING */
   88    /* CCTKNTW_OFFHOOKWARN */
};

static const BOS_UINT32 uMseNteMapSize =
                                    sizeof( mseNteMap ) / sizeof ( mseNteMap[0] );

/* List of port info */
MseSdpSession::MSEPTINFO MseSdpSession::mPtInfo;

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

MX_NAMESPACE_END( MXD_GNS )
/***********************************************************************************
**  FUNCTION:   mseInit
**
**  PURPOSE:    Initializes the MSE.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void mseInit()
{
   MX_NS MseSdpSession::initializeMseSdpSession();

   /* Enable T.38 fax negotiation */
   MX_NS CSdpCapabilitiesMgr::EnableT38(
      MX_NS CSdpCapabilitiesMgr::eSDP_CAPS_MGR_T38_MEDIA_ENABLED_WITH_OTHER_MEDIA );

#if CCTKCFG_RANDOM_PORTS
   /* Seed the random number generator */
   BOS_STATUS status;
   BOS_TIME_MS msec;

   /* Get current system time */
   status = bosTimeGetMs( &msec );
   if ( status != BOS_STATUS_OK )
   {
      CCTK_ASSERT( BOS_FALSE );
      CCTK_TRACEI0( "mseInit - Cannot get system time" );
   }

   /* Seed our randomizer using the current system tick */
   srand((BOS_UINT32)BOS_MS_TO_TICKS(msec));
#endif
}

/***********************************************************************************
**  FUNCTION:   mseDeInit
**
**  PURPOSE:    Deinitializes the MSE.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void mseDeInit()
{
   MX_NS MseSdpSession::finalizeMseSdpSession();
}

MX_NAMESPACE_START( MXD_GNS )

/* ---- Class Implementation ---------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   MseSdpSession::initializeMseSdpSession
**
**  PURPOSE:    Initialize MSE SDP session.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This is a static function.
***********************************************************************************/
mxt_result MseSdpSession::initializeMseSdpSession()
{
   /* Initialize port info */
   memset( &mPtInfo, 0, sizeof( MSEPTINFO ));
   for ( BOS_UINT32 i = 0; i < CCTKCFG_MAX_CALL; i++ )
   {
      mPtInfo.ptInfo[i].uCId = 0xFFFFFFFF;
   }

   /* Register the related ECOM. */
   return RegisterECom( CLSID_MseSdpSession, MseSdpSession::createInstance );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::finalizeMseSdpSession
**
**  PURPOSE:   Unregister the ECom instance.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This is a static function.
***********************************************************************************/
void MseSdpSession::finalizeMseSdpSession()
{
   /* Unregister the ECOM. */
   UnregisterECom( CLSID_MseSdpSession );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::createInstance
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              pOuterIEComUnknown - Not used.
**              ppCEComUnknown - Returns the created MseSdpSession instance.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This is a static function.
***********************************************************************************/
mxt_result MseSdpSession::createInstance( IEComUnknown *pOuterIEComUnknown,
                                          CEComUnknown **ppCEComUnknown )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pOuterIEComUnknown );
   *ppCEComUnknown = MX_NEW( MseSdpSession );
   return resS_OK;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::NonDelegatingQueryIf
**
**  PURPOSE:    Non-delegating QueryIf() handler for IID_IMspOfferAnswerSession.
**
**  PARAMETERS:
**              iidRequested - Interface requested.
**              ppInterface - Returned interface.
**
**
**  RETURNS:    resS_OK - Success.
**
**  NOTES:      This function is needed to be a M5T ECom instance.
***********************************************************************************/
mxt_result MseSdpSession::NonDelegatingQueryIf( IN mxt_iid iidRequested,
                                                OUT void** ppInterface )
{
    mxt_result res = resS_OK;

    CCTK_TRACED3( "MseSdpSession(%p)::NonDelegatingQueryIf(%p, %p)",
                   this, iidRequested, *ppInterface );

    if ( IsEqualEComIID( iidRequested, IID_IMspOfferAnswerSession ))
    {
       *ppInterface = static_cast<IMspOfferAnswerSession *>( this );
       AddIfRef();
    }
    else
    {
       res = CEComUnknown::NonDelegatingQueryIf( iidRequested, ppInterface );
    }

    return res;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::MseSdpSession
**
**  PURPOSE:    Class default constructor.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
MseSdpSession::MseSdpSession()
:  mbIsCaller( BOS_FALSE ),
   mbNoQuadZeros( BOS_FALSE ),
   mbNotifyUser( BOS_FALSE ),
   mbEmergencyMode( BOS_FALSE ),
   mb100RelUsed( BOS_FALSE ),
   mbPrecondRequired( BOS_FALSE ),
   mbDlgEstablished( BOS_FALSE ),
   muCid( 0 ),
   muSdpVer( 0 ),
   meLocHoldSt( eMSEHOLDST_NOT_HOLD ),
   meRemHoldSt( eMSEHOLDST_NOT_HOLD ),
   meState( eMSESTATE_IDLE ),
   mpMgr( NULL ),
   mpLocCapsMgr( NULL ),
   mpRemCapsMgr( NULL ),
   mpEarlyMedInfo( NULL ),
   mpMedInfo( NULL ),
   mpPrevSdpPacket( NULL ),
   mpEarlySdpBlob( NULL ),
   mpSdpSession( NULL ),
#if CCTKCFG_REMOTE_USER_CONTROL
   mpOffSdpBlob( NULL ),
   mpAnsSdpBlob( NULL ),
   mpNatSesHdl( NULL ),
#endif
#if CCTKCFG_ICE_LITE_SUPPORT
   meIceRmtMode( eMSEICEMODE_UNKNOWN ),
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
   meEarlyMediaFlow( eMSEEMFLOW_SENDRECV ),
   muMedUpdateRetryTmr( 0 )
{
   CCTK_TRACED1( "MseSdpSession (%p) - MseSdpSession()", this );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::MseSdpSession
**
**  PURPOSE:    Class copy constructor.
**
**  PARAMETERS:
**              rFrom - Source instance to duplicate.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
MseSdpSession::MseSdpSession( const MseSdpSession &rFrom )
:  CEComDelegatingUnknown(),
   IMspOfferAnswerSession(),
   mbIsCaller( rFrom.mbIsCaller ),
   mbNoQuadZeros( rFrom.mbNoQuadZeros ),
   mbNotifyUser( rFrom.mbNotifyUser ),
   mbEmergencyMode( rFrom.mbEmergencyMode ),
   mb100RelUsed( rFrom.mb100RelUsed ),
   mbPrecondRequired( rFrom.mbPrecondRequired ),
   mbDlgEstablished( rFrom.mbDlgEstablished ),
   muCid( rFrom.muCid ),
   muSdpVer( rFrom.muSdpVer ),
   meLocHoldSt( rFrom.meLocHoldSt ),
   meRemHoldSt( rFrom.meRemHoldSt ),
   meState( rFrom.meState ),
   mpMgr( NULL ),
   mpLocCapsMgr( NULL ),
   mpRemCapsMgr( NULL ),
   mpEarlyMedInfo( NULL ),
   mpMedInfo( NULL ),
   mpPrevSdpPacket( NULL ),
   mpEarlySdpBlob( NULL ),
   mpSdpSession( NULL ),
#if CCTKCFG_REMOTE_USER_CONTROL
   mpOffSdpBlob( NULL ),
   mpAnsSdpBlob( NULL ),
   mpNatSesHdl( NULL ),
#endif
#if CCTKCFG_ICE_LITE_SUPPORT
   meIceRmtMode( rFrom.meIceRmtMode ),
   mStrIceUfrag( rFrom.mStrIceUfrag ), 
   mStrIcePwd( rFrom.mStrIcePwd ), 
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
   meEarlyMediaFlow( eMSEEMFLOW_SENDRECV ), 
   muMedUpdateRetryTmr( rFrom.muMedUpdateRetryTmr )

{
   CCTK_TRACED1( "MseSdpSession (%p) - MseSdpSession()", this );

   if ( rFrom.mpLocCapsMgr != NULL )
   {
      mpLocCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr )( *rFrom.mpLocCapsMgr );
   }

   if ( rFrom.mpRemCapsMgr != NULL )
   {
      mpRemCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr )( *rFrom.mpRemCapsMgr );
   }

   if ( rFrom.mpEarlyMedInfo != NULL )
   {
      mpEarlyMedInfo = BRCM_NEW( CCTKMEDINFO );
      *mpEarlyMedInfo = *rFrom.mpEarlyMedInfo;
   }

   if ( rFrom.mpMedInfo != NULL )
   {
      mpMedInfo = BRCM_NEW( CCTKMEDINFO );
      *mpMedInfo = *rFrom.mpMedInfo;
   }

   if ( rFrom.mpPrevSdpPacket != NULL )
   {
      mpPrevSdpPacket = BRCM_NEW( CSdpPacket )( *rFrom.mpPrevSdpPacket );
   }

   if ( rFrom.mpEarlySdpBlob != NULL )
   {
      mpEarlySdpBlob = BRCM_NEW( CBlob )( *rFrom.mpEarlySdpBlob );
   }

   if ( rFrom.mpSdpSession != NULL )
   {
      mpSdpSession = BRCM_NEW( CBlob )( *rFrom.mpSdpSession );
   }

#if CCTKCFG_REMOTE_USER_CONTROL
   if ( rFrom.mpOffSdpBlob != NULL )
   {
      mpOffSdpBlob = BRCM_NEW( CBlob )( *rFrom.mpOffSdpBlob );
   }

   if ( rFrom.mpAnsSdpBlob != NULL )
   {
      mpAnsSdpBlob = BRCM_NEW( CBlob )( *rFrom.mpAnsSdpBlob );
   }
#endif
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::~MseSdpSession
**
**  PURPOSE:    Class destructor.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
MseSdpSession::~MseSdpSession()
{
   CCTK_TRACED1( "MseSdpSession (%p) - ~MseSdpSession()", this );

   /* Stop retry timer */
   if ( muMedUpdateRetryTmr != 0 )
   {
      tkmTimerStop( muMedUpdateRetryTmr );
      muMedUpdateRetryTmr = 0;
   }

   /* Free all allocated ports */
   freePorts( muCid );

   mpMgr = NULL;

   /* Free up the allocated pointers */
   if ( mpLocCapsMgr != NULL )
   {
      BRCM_DELETE( mpLocCapsMgr );
      mpLocCapsMgr = NULL;
   }

   if ( mpRemCapsMgr != NULL )
   {
      BRCM_DELETE( mpRemCapsMgr );
      mpRemCapsMgr = NULL;
   }

   if ( mpEarlyMedInfo != NULL )
   {
      BRCM_DELETE( mpEarlyMedInfo );
      mpEarlyMedInfo = NULL;
   }

   if ( mpMedInfo != NULL )
   {
      BRCM_DELETE( mpMedInfo );
      mpMedInfo = NULL;
   }

   if ( mpPrevSdpPacket != NULL )
   {
      BRCM_DELETE( mpPrevSdpPacket );
      mpPrevSdpPacket = NULL;
   }

   if ( mpEarlySdpBlob != NULL )
   {
      BRCM_DELETE( mpEarlySdpBlob );
      mpEarlySdpBlob = NULL;
   }

   if ( mpSdpSession != NULL )
   {
      BRCM_DELETE( mpSdpSession );
      mpSdpSession = NULL;
   }

#if CCTKCFG_REMOTE_USER_CONTROL
   if ( mpOffSdpBlob != NULL )
   {
      BRCM_DELETE( mpOffSdpBlob );
      mpOffSdpBlob = NULL;
   }

   if ( mpAnsSdpBlob != NULL )
   {
      BRCM_DELETE( mpAnsSdpBlob );
      mpAnsSdpBlob = NULL;
   }
#endif
#if CCTKCFG_ICE_LITE_SUPPORT
   /* free all the ice agents from this MSE */
   nsIceMseFreeAgents( this );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setCid
**
**  PURPOSE:    Set CE Call id.
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This API sets the Call id.  The id is used in the ceNotify()
**              callback.
***********************************************************************************/
void MseSdpSession::setCid( BOS_UINT32 uCid )
{
   CCTK_TRACED2( "MseSdpSession (%p) - setCid(0x%lX)", this, uCid );

   muCid = uCid;

   /* Update hold settings */
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
#if CCTKCFG_IPV6
   /* Use 0.0.0.0 only if the local IP is in v4 format */
   if ( bosIpAddrIsV6( &pCfgCall->xtl.media.ipaddr ))
   {
      mbNoQuadZeros = BOS_TRUE;
   }
   else
#endif
   {
      mbNoQuadZeros = pCfgCall->xtl.supp.hold.bNoQuadZeros;
   }

   mb100RelUsed = ( pCfgCall->xtl.basic.adv.ePrack != eCCTKPRACK_DISABLED );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::mediaUpdate
**
**  PURPOSE:    Initiate a media update to the remote party.
**
**  PARAMETERS:
**              eReason - Reason for the media update.
**              pParm - Parameter for the media update.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This API is invoked typically when there is a change in SDP.
***********************************************************************************/
void MseSdpSession::mediaUpdate( CCTKMUPDRSN eReason, void *pParm )
{
   CCTK_TRACED3( "MseSdpSession (%p) - mediaUpdate(%d, %p)", this, eReason, pParm );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pParm );

   fsmProcess( ( (eReason == eCCTKMUDPRSN_NOCAPS)? 
                  eMSEEVT_CE_MEDIA_NOT_READY : eMSEEVT_CE_MEDIA ), NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::hold
**
**  PURPOSE:    Hold the current call (local hold).
**
**  PARAMETERS:
**              bEmergency - Indicates whether this is a special hold for an
**                           emergency call.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::hold( BOS_BOOL bEmergencyMode /* = BOS_FALSE */ )
{
   CCTK_TRACED2( "MseSdpSession (%p) - hold(%d)", this, bEmergencyMode );

   fsmProcess( eMSEEVT_CE_HOLD, NULL );

   /* Special Emergency call handling.  In this call hold situation, the call would
   ** be put on hold as usual, but a flag will be turned on.  At a later time, the
   ** remote party may choose to send us a re-INVITE.  With the flag enabled,
   ** the re-INVITE would do the following:
   **
   ** 1) Unhold the local call state.
   ** 2) Generate a 180.
   */
   mbEmergencyMode = bEmergencyMode;
   if ( bEmergencyMode )
   {
      mbNotifyUser = BOS_TRUE;
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::unhold
**
**  PURPOSE:    Unhold the current call (local unhold).
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::unhold()
{
   CCTK_TRACED1( "MseSdpSession (%p) - unhold()", this );

   /* Unholding a call would automatically reset the Emergency call state
   */
   mbEmergencyMode = BOS_FALSE;
   mbNotifyUser = BOS_FALSE;

   fsmProcess( eMSEEVT_CE_UNHOLD, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getMediaInfo
**
**  PURPOSE:    Retrieve stored media info.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Pointer to the stored media info (NULL if none exist).
**
**  NOTES:      MSE has ownership of the media info structure.  The media info is
**              valid until the next media info refresh, or until the call is
**              terminated.
**
**              When early media is used, then the early media info structure would
**              be returned until the call is accepted.  Once the call is accepted,
**              then the regular session media info structure will be returned.
***********************************************************************************/
const CCTKMEDINFO *MseSdpSession::getMediaInfo()
{
   CCTKMEDINFO *pMedInfo;

   CCTK_TRACED1( "MseSdpSession (%p) - getMediaInfo()", this );

   if ( mpEarlyMedInfo != NULL )
   {
      pMedInfo = mpEarlyMedInfo;
   }
   else
   {
      pMedInfo = mpMedInfo;
   }

   /* No media info is available yet */
   if ( pMedInfo == NULL )
   {
      /* Check if we have any local preferences */
      if ( mpLocCapsMgr != NULL )
      {
         mpMedInfo = BRCM_NEW( CCTKMEDINFO );
         memset(mpMedInfo, 0, sizeof( CCTKMEDINFO ));
         convertSdpFromMx( mpLocCapsMgr, &mpMedInfo->egress, BOS_FALSE, BOS_FALSE );
#if CCTKCFG_ICE_LITE_SUPPORT
         mpMedInfo->iceInfo.eCctkEvt = eCCTKEVT_MEDIA;
         /* no media info, remote SDP is not yet avaliable
            create media info based on local capability - 
            ICE is default to be not ready when remote SDP is not avaliable */
         mpMedInfo->iceInfo.audio.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
#if CCTKCFG_VIDEO
         mpMedInfo->iceInfo.video.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */         
#endif
         mpMedInfo->iceInfo.t38.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
         pMedInfo = mpMedInfo;
      }
      else
      {
         CCTK_TRACEI0( "getMediaInfo - Returning an empty media info" );
      }
   }
   else if ( meEarlyMediaFlow != eMSEEMFLOW_SENDRECV )
   {
      /* Early Media Authorization  (from perspective of the UAS) */

      if ( meEarlyMediaFlow == eMSEEMFLOW_INACTIVE ||
           meEarlyMediaFlow == eMSEEMFLOW_RECVONLY )
      {
         pMedInfo->egress.audio.mode = eCCTKMEDMODE_INACTIVE;
#if CCTKCFG_VIDEO
         pMedInfo->egress.video.mode = eCCTKMEDMODE_INACTIVE;
#endif
      }

      if ( meEarlyMediaFlow == eMSEEMFLOW_INACTIVE ||
           meEarlyMediaFlow == eMSEEMFLOW_SENDONLY )
      {
         pMedInfo->ingress.audio.mode = eCCTKMEDMODE_INACTIVE;
#if CCTKCFG_VIDEO
         pMedInfo->ingress.video.mode = eCCTKMEDMODE_INACTIVE;
#endif
      }
   }

#if CCTKCFG_ICE_LITE_SUPPORT
   if ( pMedInfo )
   {
      /* remote peer does not support ice */
      if ( meIceRmtMode == eMSEICEMODE_NONE )
      {
         /* clear all the all the ICE info flags */
         memset ( &pMedInfo->iceInfo, 0, sizeof (CCTKICEINFO) );
      }
      else
      {
         /* ICE is doing connectivity check / waiting for nomination 
          * put egress media mode to inactive, so media flow will not send */
         if ( pMedInfo->iceInfo.audio.bIceTsNotReady )
         {
            pMedInfo->ingress.audio.mode = eCCTKMEDMODE_INACTIVE;   
         }
   #if CCTKCFG_VIDEO
         if ( pMedInfo->iceInfo.video.bIceTsNotReady )
         {
            pMedInfo->ingress.video.mode = eCCTKMEDMODE_INACTIVE;
         }
   #endif
      }
   }
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

   /* Returns the cached media info block here */
   return pMedInfo;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isPreconditionRequired
**
**  PURPOSE:    Indicates if peer required precondition.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    BOS_TRUE if precondition is required.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL MseSdpSession::isPreconditionRequired()
{
   CCTK_TRACED1( "MseSdpSession (%p) - isPreconditionRequired()", this );

   return mbPrecondRequired;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::answerEmergencyCall
**
**  PURPOSE:    Answer the current call (used for emergency call only).
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function sends a 200 OK with no SDP, and is needed only for
**              re-INVITEs when the MSE is responsible for sending out the 200.
***********************************************************************************/
void MseSdpSession::answerEmergencyCall()
{
   CCTK_TRACED1( "MseSdpSession (%p) - answerEmergencyCall()", this );

   /* SDP answer was sent in 180 */
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
   if ( pCfgCall->xtl.media.neg.bSdpAnswerIn180 )
   {
      CCTK_ASSERT( meState == eMSESTATE_IDLE );

      mxt_result result = mpMgr->EvNewSdpToSend( 
                                             NULL,
                                             NULL,
                                             IMspOfferAnswerSessionMgr::eAUTOMATIC,
                                             0 );
      if ( MX_RIS_F( result ))
      {
         CCTK_TRACEI1( "answerEmergencyCall - EvNewSdpToSend() returns error %d",
                       result );
      }
   }
   /* No SDP in 180, send SDP now. */
   else
   {
      fsmProcess( eMSEEVT_CE_MEDIA, NULL );
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::rejectEmergencyCall
**
**  PURPOSE:    Reject the current call
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function sends a 488, and is needed only for re-INVITEs when
**              the MSE is responsible for sending out the final response.
***********************************************************************************/
void MseSdpSession::rejectEmergencyCall()
{
   CCTK_TRACED1( "MseSdpSession (%p) - rejectEmergencyCall()", this );


      CCTK_ASSERT( meState == eMSESTATE_IDLE );

      /* Unknown rejection reason */
      mpMgr->EvNegotiationFailure(
         static_cast<IMspOfferAnswerSessionMgr::ENegotiationFailureReason>( -1 ));

      /* Revert/undo hold state */
      meLocHoldSt = eMSEHOLDST_HOLD;
      mbEmergencyMode = BOS_TRUE;
      mbNotifyUser = BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setEarlyMediaFlow
**
**  PURPOSE:    Set early media authorization.
**
**  PARAMETERS:
**              eEarlyMediaFlow - Early media flow direction.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      This function allows the caller to override the early media flow
**              should one is setup.  By default, early media is authorized
**              (eMSEEMFLOW_SENDRECV).
***********************************************************************************/
void MseSdpSession::setEarlyMediaFlow( MSEEMFLOW eEarlyMediaFlow )
{
   CCTK_TRACED2( "MseSdpSession (%p) - setEarlyMediaFlow(%d)",
                 this,
                 eEarlyMediaFlow );

   meEarlyMediaFlow = eEarlyMediaFlow;
}

#if CCTKCFG_REMOTE_USER_CONTROL
/***********************************************************************************
**  FUNCTION:   MseSdpSession::setOfferSdp
**
**  PURPOSE:    Set Offer SDP to send.
**
**  PARAMETERS:
**              pBlob - Offer SDP Caps.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::setOfferSdp( CBlob *pBlob )
{
   if ( pBlob == NULL )
   {
      return;
   }

   if ( mpOffSdpBlob != NULL )
   {
      BRCM_DELETE( mpOffSdpBlob );
   }
   mpOffSdpBlob = BRCM_NEW( CBlob )( *pBlob );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setAnswerSdp
**
**  PURPOSE:    Set Answer SDP to send.
**
**  PARAMETERS:
**              pBlob - Answer SDP Caps.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::setAnswerSdp( CBlob *pBlob )
{
   if ( pBlob == NULL )
   {
      return;
   }
   if ( mpAnsSdpBlob != NULL )
   {
      BRCM_DELETE( mpAnsSdpBlob );
   }
   mpAnsSdpBlob = BRCM_NEW( CBlob )( *pBlob );
}
#endif

/***********************************************************************************
**  FUNCTION:   MseSdpSession::SetMspOfferAnswerSessionManager
**
**  PURPOSE:    Sets the IMspOfferAnswerSessionMgr interface.
**
**  PARAMETERS:
**              pMgr - Pointer to the IMspOfferAnswerSessionMgr interface.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      pMgr will never be NULL.
***********************************************************************************/
void MseSdpSession::SetMspOfferAnswerSessionManager(
                                             IMspOfferAnswerSessionMgr *pMgr )
{
   CCTK_TRACED2(
      "MseSdpSession (%p) - SetMspOfferAnswerSessionManager(%p)", this, pMgr );

   CCTK_ASSERT( pMgr != NULL );
   mpMgr = pMgr;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::SetUserAgentMode
**
**  PURPOSE:    Sets whether we originate or terminate the call.
**
**  PARAMETERS:
**              eMode - eMODE_ORIGINATING or eMODE_TERMINATING.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::SetUserAgentMode( EUserAgentMode eMode )
{
   CCTK_TRACED2( "MseSdpSession (%p) - SetUserAgentMode(%d)", this, eMode );

   mbIsCaller = ( eMode == eMODE_ORIGINATING );

   if ( !mbIsCaller )
   {
      /* Wait for caps approval before sending SDP */
      meState = eMSESTATE_PENDINGCAPS;

      /* Only notify user for callee */
      mbNotifyUser = BOS_TRUE;
   }
#if CCTKCFG_REMOTE_USER_CONTROL
   /* Wait for caps approval before sending SDP */
   meState = eMSESTATE_PENDINGCAPS;
#endif
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::GetSipCapabilities
**
**  PURPOSE:    Gets the capabilities supported by the MSE Offer Answer Session.
**
**  PARAMETERS:
**              ruSupported - Supported header.
**              ruRequire - Required header.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::GetSipCapabilities( uint32_t &ruSupported,
                                        uint32_t &ruRequire )
{
   CCTK_TRACED3( "MseSdpSession (%p) - GetSipCapabilities(%u, %u)", this,
      ruSupported, ruRequire );

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( ruSupported );
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( ruRequire );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::SetPeerSipCapabilities
**
**  PURPOSE:    Informs the MSE Offer Answer Session of the options that the
**              peer support and require.
**
**  PARAMETERS:
**              uSupported - Supported header.
**              uRequire - Required header.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::SetPeerSipCapabilities( const uint32_t uSupported,
                                            const uint32_t uRequire )
{
   CCTK_TRACED3( "MseSdpSession (%p) - SetPeerSipCapabilities(%u, %u)",
      this, uSupported, uRequire );

   /* 100rel is used only if supposed by both parties. */
   mb100RelUsed = mb100RelUsed &&
      ( UTIL_BMP_IS_ENABLED( uSupported, eOPTIONTAG_100REL ) ||
        UTIL_BMP_IS_ENABLED( uRequire, eOPTIONTAG_100REL ));

   /* Keep track whether peer requires precondition. */
   mbPrecondRequired = mbPrecondRequired ||
      UTIL_BMP_IS_ENABLED( uRequire, eOPTIONTAG_PRECONDITION );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::PrepareOffer
**
**  PURPOSE:    Asynchronously prepares an SDP offer.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::PrepareOffer(IN bool bIsMediaSessionRefresh /*= false*/)
{
   CCTK_TRACED2( "MseSdpSession (%p) - PrepareOffer(%d)", this, bIsMediaSessionRefresh );

   fsmProcess( eMSEEVT_SCE_PREPAREOFFER, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::CancelOffer
**
**  PURPOSE:    Cancels the current offer/answer exchange when the offer was
**              generated by the MSE Offer Answer Session.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::CancelOffer()
{
   CCTK_TRACED2( "MseSdpSession (%p) - CancelOffer(%d)", this, meLocHoldSt );

   fsmProcess( eMSEEVT_SCE_CANCELOFFER, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::GlareError
**
**  PURPOSE:    Informs the MSE Offer Answer Session that a network glare error
**              occured.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::GlareError( uint32_t uRetryAfterMs )
{
   CCTK_TRACED2( "MseSdpSession (%p) - GlareError(%u)", this, uRetryAfterMs );

   fsmProcess( eMSEEVT_SCE_GLAREERROR, reinterpret_cast<void *>( uRetryAfterMs ));
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::HandleSdp
**
**  PURPOSE:    Informs the MSE Offer Answer Session of the reception of new
**              SDP packet.
**
**  PARAMETERS:
**              pSdpSession - The received SDP packet with Content-Disposition
**                            of session.
**              pSdpEarlySession - The received SDP packet with
**                                 Content-Disposition of early-session.
**              eSessionType - Session type in pSdpSession.
**              eEarlySessionType - Session type in pSdpEarlySession.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::HandleSdp( const CBlob *pSdpSession,
                               const CBlob *pSdpEarlySession,
                               ESdpType eSessionType,
                               ESdpType eEarlySessionType )
{
   CCTK_TRACED4( "MseSdpSession (%p) - HandleSdp(%p, %p, %d)", this, pSdpSession,
      pSdpEarlySession, eSessionType );

   BOS_BOOL bContinue = BOS_TRUE;

   /* Sanity check */
   CCTK_ASSERT( pSdpSession != NULL || pSdpEarlySession != NULL );

   /* Early Session processing */
   if ( pSdpEarlySession != NULL )
   {
      /* We never send out an early session offer, so we should never receive an
      ** answer.
      */
      CCTK_ASSERT( eEarlySessionType != eANSWER_PREVIEW &&
                   eEarlySessionType != eANSWER_FINAL );

      bContinue = fsmProcess( eMSEEVT_SCE_EARLYSDPOFFER,
                reinterpret_cast<void *>( const_cast<CBlob *>( pSdpEarlySession )));
   }

   /* Session processing */
   if ( bContinue && pSdpSession != NULL )
   {
      MSEEVT eEvt = eMSEEVT_MAX;

      switch ( eSessionType )
      {
      /* Received an offer SDP */
      case eOFFER_LONG:
         eEvt = eMSEEVT_SCE_SDPOFFERLONG;
         break;

      /* Received an offer SDP */
      case eOFFER_SHORT:
         eEvt = eMSEEVT_SCE_SDPOFFERSHORT;
         break;

      case eOFFER_SHORT_CANT_FAIL:
         eEvt = eMSEEVT_SCE_SDPOFFERSHORTCANTFAIL;
         break;

      /* Received an answer SDP */
      case eANSWER_PREVIEW:
         eEvt = eMSEEVT_SCE_SDPANSWERPREVIEW;
         break;

      case eANSWER_FINAL:
         eEvt = eMSEEVT_SCE_SDPANSWERFINAL;
         break;

      default:
         CCTK_TRACEI1( "HandleSdp: Warning - unknown session type %d",
            eSessionType );
         break;
      }

      if ( eEvt != eMSEEVT_MAX )
      {
         bContinue = fsmProcess( eEvt,
                     reinterpret_cast<void *>( const_cast<CBlob *>( pSdpSession )));
      }
   }

   /* Special case when early is available but not yet sent.  This situation would
   ** happen when either:
   **
   ** 1) We do not have any regular session SDP to send (e.g. we received an
   **    answer).
   ** 2) We fail to parse the regualr session SDP.
   */
   if ( bContinue && mpEarlySdpBlob != NULL )
   {
      const CFGCALL *pCfgCall = CFG_CALL( muCid );
      IMspOfferAnswerSessionMgr::EMessageTypeForPayload eMsgType;

      if ( mbNotifyUser && pCfgCall->xtl.media.neg.bSdpAnswerIn180 )
      {
         eMsgType = IMspOfferAnswerSessionMgr::eINVITE_RINGING;
         mbNotifyUser = BOS_FALSE;
      }
      else
      {
         eMsgType = IMspOfferAnswerSessionMgr::eAUTOMATIC;
      }
      mxt_result result = mpMgr->EvNewSdpToSend( NULL,
                                                 mpEarlySdpBlob,
                                                 eMsgType,
                                                 0 );

      if ( MX_RIS_F( result ))
      {
         CCTK_TRACEI1( "HandleSdp - EvNewSdpToSend() returns error %d", result );
      }
      else if ( eMsgType == IMspOfferAnswerSessionMgr::eINVITE_RINGING )
      {
         /* Notify CE for the ringback */
         ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
      }
      mpEarlySdpBlob = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::RemoteUserNotified
**
**  PURPOSE:    Informs the MSE Offer Answer Session that the remote user was
**              notified of an incoming session.
**
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      180 Ringing (for INVITE) received.
***********************************************************************************/
void MseSdpSession::RemoteUserNotified()
{
   CCTK_TRACED1( "MseSdpSession (%p) - RemoteUserNotified()", this );

   fsmProcess( eMSEEVT_SCE_REMOTENOTIFIED, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::RemoteUserAccepted
**
**  PURPOSE:    Informs the MSE Offer Answer Session that the remote user has
**              accepted the session.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      200 OK (for INVITE) received.
***********************************************************************************/
void MseSdpSession::RemoteUserAccepted()
{
   CCTK_TRACED1( "MseSdpSession (%p) - RemoteUserAccepted()", this );

   fsmProcess( eMSEEVT_SCE_REMOTEACCEPTED, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::LocalUserAccepted
**
**  PURPOSE:    TBD.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::LocalUserAccepted()
{
   CCTK_TRACED1( "MseSdpSession (%p) - LocalUserAccepted()", this );

   fsmProcess( eMSEEVT_SCE_LOCALACCEPTED, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::Fork
**
**  PURPOSE:    Forks the current offer answer session to create a new one.
**
**  PARAMETERS:
**              ppForkedOfferAnswerSession - New offer answer session.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::Fork( IMspOfferAnswerSession **ppForkedOfferAnswerSession )
{
   CCTK_TRACED1( "MseSdpSession (%p) - Fork()", this );

   /* Duplicate session here. */
   *ppForkedOfferAnswerSession = BRCM_NEW( MseSdpSession )( *this );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::ResetNegotiation
**
**  PURPOSE:    Resets the negotiation to its original state.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::ResetNegotiation()
{
   CCTK_TRACED1( "MseSdpSession (%p) - ResetNegotiation()", this );

   fsmProcess( eMSEEVT_SCE_RESETNEGOTIATION, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::PreTerminateSession
**
**  PURPOSE:    Informs that the associated session is about to be terminated.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::PreTerminateSession()
{
   CCTK_TRACED1( "MseSdpSession (%p) - PreTerminateSession()", this );

   fsmProcess( eMSEEVT_SCE_PRETERMINATE, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::TerminateSession
**
**  PURPOSE:    Terminates the Offer Answer Session.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::TerminateSession()
{
   CCTK_TRACED1( "MseSdpSession (%p) - TerminateSession()", this );

   fsmProcess( eMSEEVT_SCE_TERMINATE, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::addBandwidthAttrib
**
**  PURPOSE:    Insert bandwidth attributes (b=TIAS, b=AS, a=maxprate) to the SDP
**              caps.
**
**  PARAMETERS:
**              pCapsMgr - SDP caps manager.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::addBandwidthAttrib( CSdpCapabilitiesMgr *pCapsMgr )
{
   BOS_UINT32 i, j, uNumStreams;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

   uNumStreams = pCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      CSdpLevelMedia &rStream = pCapsMgr->GetStream( i );

      /* Do nothing if stream is disabled */
      if ( pCapsMgr->GetStreamPort( i ) == 0 )
      {
         continue;
      }

      if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
           ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
             || pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#endif
           ))
      {
         BOS_UINT32 uMaxPrate = 0;
         BOS_UINT32 uBTias = 0;
         BOS_UINT32 uBAs = 0xFFFFFFFF;

         /* Check if mptime is available */
         BOS_BOOL bMptime = isMptimeAvailableMx( rStream );

         /* If mptime does not exist, check for ptime */
         BOS_UINT32 uPtime = 0;
         if ( !bMptime )
         {
            uPtime = getPtimeFromMx( rStream );
         }

         BOS_BOOL bIsSrc;
         CString lbFmt;
         BOS_UINT32 uPayloadCount = 0; /* Payload to remove after parsing */
         if ( getLoopbackMode( rStream, bIsSrc, lbFmt ) )
         {
            BOS_UINT32 uLbType = getLoopbackType( rStream );

            if (( uLbType & MSELBTYPE_PKTLB ) > 0 )
            {
               /* Add the Media Format to the Payload list for parsing
               */
               BOS_UINT32 uFmtSize = lbFmt.GetSize();
               char *pcMediaFmt = BRCM_NEW_ARRAY( char, uFmtSize + 1 );
               strncpy( pcMediaFmt, lbFmt.CStr(), uFmtSize + 1);

               char *pcPayload = strtok( pcMediaFmt, " " );
               unsigned int uFmtIdx;
               while ( pcPayload != NULL )
               {
                  pCapsMgr->AddMediaFormat( i, pcPayload, uFmtIdx );
                  uPayloadCount++;
                  pcPayload = strtok( NULL, " " );
               }
               BRCM_DELETE_ARRAY( pcMediaFmt );
               pcMediaFmt = NULL;
            }
         }

         /* Iterate through the RTP payload list */
         BOS_BOOL bEncapRtpUsed = BOS_FALSE; /* Encapulated RTP (loopback mode) */
         BOS_UINT32 uNumRtps = pCapsMgr->GetNbPayloadTypes( i );
         for ( j = 0; j < uNumRtps; j++ )
         {
            CSdpFieldAttributeRtpmap rtpmap;
            pCapsMgr->GetPayloadType( i, j, rtpmap );

            CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();

            if ( eMxCodec == CSdpParser::eLBENCAPRTP )
            {
               bEncapRtpUsed = BOS_TRUE;
            }
            else
            {
               BOS_UINT32 uRate = static_cast<BOS_UINT32>( rtpmap.GetClockRate() );
               const MSECODECMAP *pCodecMap = getCodecMap( eMxCodec, uRate );
               if ( pCodecMap->eCodec >= eCCTKCODEC_MAX )
               {
                  /* Bypass unrecognized RTP codec type */
                  continue;
               }

               /* The 'frame base' is used for calculation of bandwidth requirements
               ** and it cannot be 0 as it would cause division by zero exception,
               ** in other words that value must have been properly set.
               */
               CCTK_ASSERT( pCodecMap->uFrameBase > 0 );

               /* As per-mptime specification, use ptime only if mptime is
               ** unavailable.
               */
               if ( bMptime )
               {
                  uPtime = getMptimeFromMx( rStream, j );
               }

               /* Update bandwidth attributes, use default ptime if not set.
               ** IPv4 header size: 20 bytes
               ** IPv6 header size: 40 bytes
               ** UDP header size: 8 bytes
               ** RTP header size: 12 bytes
               ** Encapsulated RTP header size: 16 bytes
               **
               ** a=maxprate: Packets per second
               ** b=TIAS: bps (RTP payload only)
               ** b=AS: kbps (include IP/UDP/RTP headers)
               */

               if ( uPtime == 0 )
               {
                  /* Default ptime value when not set */
                  uPtime = 20;
               }

               /* a=maxprate
               ** Note, result is multiplied by 10, as we want up to 1 decimal precision
               ** and will be divided by 10 when forming the a=maxprate line. */
               uMaxPrate = MX_MAX( uMaxPrate, 1000 * 10 / uPtime );

               /* Payload size */
               BOS_UINT32 uPktSize =
                  pCodecMap->uPktSize * ( uPtime / pCodecMap->uFrameBase );
               if ( bEncapRtpUsed )
               {
                  uPktSize += 16;
               }

               /* b=TIAS */
               if ( bEncapRtpUsed )
               {
                  uBTias = MX_MAX( uBTias, uPktSize * uMaxPrate * 8 / 10 );
               }
               else
               {
                  uBTias =
                     MX_MAX( uBTias,
                       pCodecMap->uPktSize * 1000 * 8 / pCodecMap->uFrameBase );
               }

               /* b=AS */
#if BOS_CFG_IP_ADDR_V6_SUPPORT
               if ( bosIpAddrIsV6( &pCfgCall->xtl.media.ipaddr ))
               {
                  /* Add IPv6 header */
                  uPktSize += 40;
               }
               else
#endif
               {
                  /* Add IPv4 header */
                  uPktSize += 20;
               }

               /* Add UDP/RTP headers */
               uPktSize += ( 8 + 12 );

               uBAs = MX_MIN( uBAs, ( 1000 * uPktSize * 8 / uPtime + 999 ) / 1000 );
            }
         }

         /* Remove payload (for loopback support only) */
         while ( uPayloadCount > 0 )
         {
            BOS_UINT32 uNumRtps = pCapsMgr->GetNbPayloadTypes( i );
            CCTK_ASSERT( uNumRtps > 0 );
            pCapsMgr->RemovePayloadType( i, uNumRtps - 1 );
            uPayloadCount--;
         }

         /* Insert b=TIAS, b=AS and a=maxprate if used */
         if ( pCfgCall->xtl.media.neg.bUseBandwidth )
         {
            addBandwidthAttrib( rStream, uMaxPrate, uBTias, uBAs );
         }
      }
      else if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eIMAGE &&
                ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eUDPTL ||
                  pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eudptl ))
      {
         CSdpFieldMediaAnnouncement& rMediaAnn = rStream.GetMediaAnnouncement();
         if ( rMediaAnn.GetNbMediaFormats() > 0 &&
              strcmp( rMediaAnn.GetMediaFormat( 0 ), cT38Str ) == 0 )
         {
            /* Assume T.38 / UDPTL / V.34 */
            BOS_UINT32 uPktSize, uMaxPrate, uBTias, uBAs, uPtime;

            uPtime = pCfgCall->xtl.media.pFax->uPtime;
            if ( uPtime == 0 )
            {
               /* Default packet rate */
               uPtime = 20;
            }
            uMaxPrate = 1000 * 10 / uPtime;

            /* Packet size:
            **    10ms frame: 50 bytes
            **    10 + (10 * n) ms frame: 50 + n * 36 bytes
            */
            uPktSize = 50 + ( uPtime / 10 - 1 ) * 36;

            uBTias = uPktSize * uMaxPrate * 8 / 10;
#if BOS_CFG_IP_ADDR_V6_SUPPORT
            if ( bosIpAddrIsV6( &pCfgCall->xtl.media.ipaddr ))
            {
               /* IPv6 header size */
               uPktSize += 40;
            }
            else
#endif
            {
               /* IPv4 header size */
               uPktSize += 20;
            }
            /* UDPTL header */
            uPktSize += 10;

            uBAs = ( 1000 * uPktSize * 8 / uPtime + 999 ) / 1000;

            addBandwidthAttrib( rStream, uMaxPrate, uBTias, uBAs );
         }
      }
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::addBandwidthAttrib
**
**  PURPOSE:    Insert bandwidth attributes (b=TIAS, b=AS, a=maxprate) to the SDP
**              caps.
**
**  PARAMETERS:
**              CSdpLevelMedia - Reference to stream to insert the attributes.
**              uMaxPrate - Max packet rate, multiplied by 10 (for a=maxprate).
**              uBTias - TIAS bandwidth modifier (for b=TIAS).
**              uBAs - AS bandwidth modifier (for b=AS).
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      uMaxPrate is multiplied by 10 as the number is rounded up to up to
**              1 decimal precision.
***********************************************************************************/
void MseSdpSession::addBandwidthAttrib( CSdpLevelMedia &rStream,
                                        BOS_UINT32 uMaxPrate,
                                        BOS_UINT32 uBTias,
                                        BOS_UINT32 uBAs )
{
   BOS_UINT32 uPrinted = 0;
   char cStr[MSE_BWATTRLEN];

   /* Insert a=maxprate */
   CSdpFieldAttributeOther maxPrate;
   maxPrate.SetName( cMaxPrateStr );
   uPrinted = snprintf( &cStr[0], MSE_BWATTRLEN, 
                        "%d", static_cast<int> (uMaxPrate / 10) );
   if (MSE_BWATTRLEN > uPrinted) 
   {
      snprintf( &cStr[uPrinted], (MSE_BWATTRLEN - uPrinted), ".%d", 
                static_cast<int> (uMaxPrate % 10) );
   }
   maxPrate.SetValue( cStr );
   rStream.AddOtherAttribute( maxPrate );

   /* Insert b=TIAS, b=AS */
   CVector <CString> &bandwidth = rStream.GetBandwidth();
   CString bandwidthStr;

   snprintf( &cStr[0], MSE_BWATTRLEN, "%s:%d", cTiasStr, static_cast<int> (uBTias) );
   bandwidthStr.EraseAll();
   bandwidthStr.Append( &cStr[0], strlen( &cStr[0] ));
   bandwidth.Append( bandwidthStr );

   snprintf( &cStr[0], MSE_BWATTRLEN, "%s:%d", cAsStr, static_cast<int> (uBAs) );
   bandwidthStr.EraseAll();
   bandwidthStr.Append( &cStr[0], strlen( &cStr[0] ));
   bandwidth.Append( bandwidthStr);
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::addBandwidthAttrib
**
**  PURPOSE:    Insert bandwidth attributes (b=RS, b=RR) to the SDP caps.
**
**  PARAMETERS:
**              CSdpLevelMedia - Reference to stream to insert the attributes.
**              uBRs - Sender Report bandwidth (for b=RS).
**              uBRr - Sender Report bandwidth (for b=RR).
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::addBandwidthAttrib( CSdpLevelMedia &rStream,
                                        BOS_UINT32 uBRs,
                                        BOS_UINT32 uBRr )
{
   char cStr[MSE_BWATTRLEN];

   /* Insert b=RS, b=RR */
   CVector <CString> &bandwidth = rStream.GetBandwidth();
   CString bandwidthStr;

   snprintf( &cStr[0], MSE_BWATTRLEN, 
             "%s:%d", cRsStr, static_cast<int> (uBRs) );
   bandwidthStr.EraseAll();
   bandwidthStr.Append( &cStr[0], strlen( &cStr[0] ));
   bandwidth.Append( bandwidthStr );

   snprintf( &cStr[0], MSE_BWATTRLEN, 
             "%s:%d", cRrStr, static_cast<int> (uBRr) );
   bandwidthStr.EraseAll();
   bandwidthStr.Append( &cStr[0], strlen( &cStr[0] ));
   bandwidth.Append( bandwidthStr);
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::addCodec
**
**  PURPOSE:    Insert codec to the SDP caps.
**
**  PARAMETERS:
**              pCapsMgr - SDP caps manager.
**              uStreamIdx - Stream index to the caps manager.
**              eCodec - Codec type to add.
**              uRtpCode - RTP payload code to use.
**
**
**  RETURNS:    BOS_UINT32 - Packet size per 10 ms "frame" (in bytes).
**
**  NOTES:      .
***********************************************************************************/
BOS_UINT32 MseSdpSession::addCodec( CSdpCapabilitiesMgr *pCapsMgr,
                                    BOS_UINT32 uStreamIdx,
                                    CCTKCODEC eCodec,
                                    BOS_UINT8 uRtpCode )
{
   const MSECODECMAP *pCodecMap;

   pCodecMap = getCodecMap( eCodec );

   addCodec( pCapsMgr, uStreamIdx, pCodecMap->eMxCodec, pCodecMap->uRate, uRtpCode );

   return pCodecMap->uPktSize;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::addCodec
**
**  PURPOSE:    Insert codec to the SDP caps.
**
**  PARAMETERS:
**              pCapsMgr - SDP caps manager.
**              uStreamIdx - Stream index to the caps manager.
**              eMxCodec - M5T codec type.
**              uRate - Packet rate
**              uRtpCode - RTP payload code to use.
**
**
**  RETURNS:    BOS_UINT32 - Packet size per 10 ms "frame" (in bytes).
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::addCodec( CSdpCapabilitiesMgr *pCapsMgr,
                              BOS_UINT32 uStreamIdx,
                              CSdpParser::ERtpCompressionAlgorithm eMxCodec,
                              BOS_UINT32 uRate,
                              BOS_UINT8 uRtpCode )
{
   CSdpFieldAttributeRtpmap rtpmap;

   rtpmap.Reset();
   rtpmap.SetEncoding( eMxCodec );
   rtpmap.SetClockRate( uRate );
   rtpmap.SetPayloadType( uRtpCode );
   rtpmap.Validate();
   CCTK_ASSERT( rtpmap.IsValid() );

   /* TODO: a=fmtp support */

   /* Set the supported media along with the a=rtpmap line */
   unsigned int uPlIdx;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

   pCapsMgr->AddPayloadType( static_cast<unsigned int>( uStreamIdx ),
                             rtpmap,
                             uPlIdx,
                             pCfgCall->xtl.media.neg.bSdpNoStaticRtpmap
                                == BOS_TRUE );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::createLocalSdp
**
**  PURPOSE:    Create a SDP based on the local configured media profile.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP struct in CSdpCapabilitiesMgr format.
**              bIsOffer - Indicates whether the local SDP is created for offer.
**              bEarlySession - Indicates whether the local SDP is created for an 
**                              early session.
**
**  RETURNS:    Nothing.
**
**  NOTES:      - The caller provides storage to pCapsMgr.
**              - When bUseMediaLst is BOS_FALSE, it is assumed that pMediaLst is
**                set to NULL and the entire media caps will be used.
***********************************************************************************/
void MseSdpSession::createLocalSdp( CSdpCapabilitiesMgr *pCapsMgr,
                                    BOS_BOOL bIsOffer,
                                    BOS_BOOL bEarlySession )
{
   BOS_UINT32 i;
   CCTKMEDLST *pMediaLst, mediaLst;
   BOS_BOOL bUseMediaLst = bIsOffer;

   /* Retrieve media profile */
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
   CCTK_ASSERT( pCfgCall != NULL );
   CCTKMEDAUD *pAudio = pCfgCall->xtl.media.pAudio;
#if CCTKCFG_VIDEO
   CCTKMEDVID *pVideo = pCfgCall->xtl.media.pVideo;
#endif

#if CCTKCFG_IPV6
   /* Set c= type (required for GenerateAnswer()) */
   pCapsMgr->GetSdpSession()->GetConnectionData().SetAddressTypeId( 
      bosIpAddrIsV4( &pCfgCall->xtl.media.ipaddr ) ? CSdpParser::eIP4 : CSdpParser::eIP6
   );
#endif

   /* Set s= Session Name/Subject */
   setSessionName( pCapsMgr, pCfgCall->xtl.media.pcSesName );

   /* Create SDP - If pMediaLst is not empty, then the SDP would be formatted based
   ** on pMediaLst.  If pMediaLst is empty, then the SDP would be formatted based on
   ** the capability list.
   */

   /* pMediaLst is empty (or bUseMediaLst is FALSE), create a full list locally */
   if ( pCfgCall->xtl.media.pMediaLst == NULL || !bUseMediaLst )
   {
      memset( &mediaLst, 0, sizeof( CCTKMEDLST ));
      pMediaLst = &mediaLst;

      /* Insert all supported audio codec to the list */
      if ( pAudio != NULL )
      {
         pMediaLst->mMediaTypes |= CCTK_MEDIA_AUDIO_MASK;
         pMediaLst->uNumAudCodec = pAudio->gen.uNumCodec;
         for ( i = 0; i < pAudio->gen.uNumCodec; i++ )
         {
            pMediaLst->audCodeLst[i] = pAudio->gen.codec[i].eType;
         }
      }

#if CCTKCFG_VIDEO
      /* Insert all supported video codec to the list */
      CCTKMEDVID *pVideo = pCfgCall->xtl.media.pVideo;
      if ( pVideo != NULL )
      {
         pMediaLst->mMediaTypes |= CCTK_MEDIA_VIDEO_MASK;
         pMediaLst->uNumVidCodec = pVideo->gen.uNumCodec;
         for ( i = 0; i < pVideo->gen.uNumCodec; i++ )
         {
            pMediaLst->vidCodeLst[i] = pVideo->gen.codec[i].eType;
         }
      }
#endif

      if ( pCfgCall->xtl.media.pFax != NULL )
      {
         pMediaLst->mMediaTypes |= CCTK_MEDIA_T38_MASK;
      }
   }
   else
   {
      pMediaLst = pCfgCall->xtl.media.pMediaLst;
   }

   /* Did MediaLst change?  If it did, we need to restart ICE-lite */
   /* Fix me, no proper ICE-lite restart code yet */

   /*
   ** Create SDP
   */

   /* Format audio m= line(s) */
#if CCTKCFG_SRTP
   if ( pAudio != NULL && pAudio->sec.eSrtpPref != eCCTKSRTPPREF_DISABLED )
   {
      /* Create SAVP audio m= */
      createLocalAudioSdp( pCapsMgr, pAudio, pMediaLst, bIsOffer, BOS_TRUE, bEarlySession );
   }
   if ( pAudio != NULL && pAudio->sec.eSrtpPref != eCCTKSRTPPREF_MANDATORY )
#endif
   {
      /* Create AVP audio m= */
      createLocalAudioSdp( pCapsMgr, pAudio, pMediaLst, bIsOffer, BOS_FALSE, bEarlySession );
   }

   /* Format video m= line(s) */
#if CCTKCFG_VIDEO
#if CCTKCFG_SRTP
   if ( pVideo != NULL && pVideo->sec.eSrtpPref != eCCTKSRTPPREF_DISABLED )
   {
      /* Create SAVP video m= */
      createLocalVideoSdp( pCapsMgr, pVideo, pMediaLst, bIsOffer, BOS_TRUE, bEarlySession );
   }
   if ( pVideo != NULL && pVideo->sec.eSrtpPref != eCCTKSRTPPREF_MANDATORY )
#endif
   {
      /* Create AVP video m= */
      createLocalVideoSdp( pCapsMgr, pVideo, pMediaLst, bIsOffer, BOS_FALSE, bEarlySession );
   }
#endif

   /* Format T.38 m= line(s) */
   createLocalT38Sdp( pCapsMgr, pCfgCall->xtl.media.pFax, pMediaLst, bEarlySession );

   /* Offer only attributes */
   if ( bIsOffer )
   {
      /* Insert b=TIAS, b=AS and a=maxprate based on the codecs and ptime */
      addBandwidthAttrib( pCapsMgr );

      /* Insert RTCP-XR (voip-metric) if enabled */
      if ( !pCfgCall->xtl.media.neg.bDisableRTCP )
      {
         BOS_UINT32 uNumStreams = pCapsMgr->GetNbStreams();
         for ( i = 0; i < uNumStreams; i++ )
         {
            /* Do nothing if stream is disabled */
            if ( pCapsMgr->GetStreamPort( i ) == 0 )
            {
               continue;
            }

            if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
                 ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP 
#if CCTKCFG_SRTP
                   || pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP 
#endif
                 ) && pAudio != NULL && pAudio->qos.bRtcpXrSupport )
            {
               /* Insert a=rtcp-xr:voip-metrics */
               CSdpFieldAttributeOther rtcpXr;
               rtcpXr.SetName( cRtcpXrStr );
               rtcpXr.SetValue( cVoipMetricStr );
               pCapsMgr->GetStream( i ).AddOtherAttribute( rtcpXr );
            }
         }
      }
   }

}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::createLocalAudioSdp
**
**  PURPOSE:    Create a audio SDP based on the local configured media profile.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP struct in CSdpCapabilitiesMgr format.
**              pAudio - Pointer to the audio profile.
**              pMediaLst - Pointer to the media list.
**              bIsOffer - Indicates whether the local SDP is created for offer.
**              bSecured - Indicates whether this is a secured stream.
**              bEarlySession - Indicates whether the local SDP is created for an 
**                              early session.
**
**  RETURNS:    Nothing.
**
**  NOTES:      - The caller provides storage to pCapsMgr.
**              - When bUseMediaLst is BOS_FALSE, it is assumed that pMediaLst is
**                set to NULL and the entire media caps will be used.
***********************************************************************************/
void MseSdpSession::createLocalAudioSdp( CSdpCapabilitiesMgr *pCapsMgr,
                                         CCTKMEDAUD *pAudio,
                                         CCTKMEDLST *pMediaLst,
                                         BOS_BOOL bIsOffer,
                                         BOS_BOOL bSecured,
                                         BOS_BOOL bEarlySession )
{
   BOS_UINT32 i, j, uStreamIdx;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
   BOS_BOOL bInclWB = BOS_FALSE;

   if ( ( pMediaLst->mMediaTypes & CCTK_MEDIA_AUDIO_MASK ) == 0 ||
        pAudio == NULL || pAudio->gen.uNumCodec == 0 )
   {
      return;
   }

   /* Allocate a RTP/RTCP port */
   BOS_UINT16 uRtpPort =
      allocatePort( muCid, bSecured ? eMSEPTTYPE_AUD_SRTP : eMSEPTTYPE_AUD_RTP );
   BOS_UINT16 uRtcpPort =
      allocatePort( muCid, bSecured ? eMSEPTTYPE_AUD_SRTCP : eMSEPTTYPE_AUD_RTCP );

   BOS_BOOL bUseMptime = pCfgCall->xtl.media.neg.bUseMptime;

   /* Add audio stream */
   pCapsMgr->AddStream( CSdpParser::eAUDIO,
#if CCTKCFG_SRTP
                        bSecured ? CSdpParser::eRTPSAVP : CSdpParser::eRTPAVP,
#else
                        CSdpParser::eRTPAVP,
#endif
                        uRtpPort,
                        reinterpret_cast<unsigned int &>( uStreamIdx ));

   CSdpLevelMedia &rStream = pCapsMgr->GetStream( uStreamIdx );

   /* Insert a=rtcp if used and not disabled */
   if ( !pCfgCall->xtl.media.neg.bDisableRTCP && pAudio->gen.rtcpPort.uMinPort > 0 )
   {
      CSdpFieldAttributeRtcp rtcpAttrib;
      rtcpAttrib.SetPort( uRtcpPort );
      rStream.SetSdpFieldAttributeRtcp( rtcpAttrib );
   }

   CString mptimeStr;

   for ( i = 0; i < pAudio->gen.uNumCodec; i++ )
   {
      CCTKCODEC eCodec = pAudio->gen.codec[i].eType;
      BOS_UINT8 uRtpCode = pCfgCall->xtl.media.neg.uRtpCode[eCodec];
      BOS_BOOL bCodecFound = BOS_FALSE;

      /* Add codec only if it is on the media codec list */
      for ( j = 0; j < pMediaLst->uNumAudCodec; j++ )
      {
         if ( pMediaLst->audCodeLst[j] == eCodec )
         {
            bCodecFound = BOS_TRUE;
            break;
         }
      }

      /* Codec not found on the media codec list, try the next codec. */
      if ( !bCodecFound )
      {
         CCTK_TRACEI1("Codec not found, codec num:%u", eCodec);
         continue;
      }

      addCodec( pCapsMgr, uStreamIdx, eCodec, uRtpCode );

      /* Insert a=fmtp for iLBC */
      if ( eCodec == eCCTKCODEC_ILBC &&
           pAudio->gen.codec[i].parm.eIlbcMode != eCCTKILBCMODE_NOTUSED )
      {
         CSdpFieldAttributeFmtp fmtpIlbc;
         fmtpIlbc.SetMediaFormat( uRtpCode );

         switch ( pAudio->gen.codec[i].parm.eIlbcMode )
         {
            case eCCTKILBCMODE_20MS:
               fmtpIlbc.SetValue( cIlbcMode20msStr );
               break;
            case eCCTKILBCMODE_30MS:
            default:
               fmtpIlbc.SetValue( cIlbcMode30msStr );
               break;
         }

         rStream.AddFmtp( fmtpIlbc );
      }

      /* Insert a=fmtp for G.723.1 */
      if ( eCodec == eCCTKCODEC_G7231 &&
           pAudio->gen.codec[i].parm.eG7231Vad != eCCTKG7231VAD_NOTUSED )
      {
         pCapsMgr->AddVadFmtp(
            CSdpParser::ms_apszRtpCompressionAlgorithmMap[CSdpParser::eG723],
            uRtpCode,
            pAudio->gen.codec[i].parm.eG7231Vad == eCCTKG7231VAD_YES,
            rStream );
      }

      /* Insert a=fmtp for G.729x */
      if ( ( eCodec == eCCTKCODEC_G729E || eCodec == eCCTKCODEC_G729 ) &&
           pAudio->gen.codec[i].parm.eG729Vad != eCCTKG729VAD_NOTUSED )
      {
         CSdpParser::ERtpCompressionAlgorithm eMxCodec;

         if ( eCodec == eCCTKCODEC_G729E )
         {
            eMxCodec = CSdpParser::eG729E;
         }
         else
         {
            eMxCodec = CSdpParser::eG729;
         }

         pCapsMgr->AddVadFmtp(
            CSdpParser::ms_apszRtpCompressionAlgorithmMap[eMxCodec],
            uRtpCode,
            pAudio->gen.codec[i].parm.eG729Vad == eCCTKG729VAD_YES,
            rStream );
      }

      /* Insert a=fmtp for AMR-WB */
      if ( eCodec == eCCTKCODEC_AMRWB ||
           eCodec == eCCTKCODEC_GSMAMR )
      {
         CCTKAMRPARM *pAmrCfg = &pAudio->gen.codec[i].parm.amrCfg;
         CSdpFieldAttributeFmtp fmtpAmr;

         constructAmrFmtp( fmtpAmr, pAmrCfg, (eCodec == eCCTKCODEC_AMRWB) );
         fmtpAmr.SetMediaFormat( uRtpCode );
         rStream.AddFmtp( fmtpAmr );
      }

      /* Create a=mptime string if enabled */
      BOS_UINT32 uPtime = pAudio->gen.codec[i].uPtime;
      if ( uPtime > 0 && bUseMptime )
      {
         char cPtimeStr[MSE_PTIMELEN];
         snprintf( &cPtimeStr[0], MSE_PTIMELEN, "%d ", 
                   static_cast<int> (uPtime) );
         mptimeStr.Append( &cPtimeStr[0], strlen( &cPtimeStr[0] ));
      }

      /* check if this is a wideband codec */
      if (!bInclWB && isWidebandCodec(eCodec))
      {
         /* a wideband codec is included in the offer */
         bInclWB = BOS_TRUE;
      }
   }

   /* Insert telephone-event */
   if ( pAudio->gen.bTelEvt )
   {
      BOS_UINT8 uRtpcode = pCfgCall->xtl.media.neg.uTelEvtRtpCode;

      /* Prepare a=fmtp for telephone-event, e.g. a=fmtp:97 0-15
      */
      BOS_UINT32 uTelEvtTypes = pAudio->gen.uTelEvtTypes;
      if ( uTelEvtTypes > 0 )
      {
         CSdpFmtpTelEvent telEvt;
         telEvt.SetMediaFormat( uRtpcode );

         for ( j = 0; j < uMseNteMapSize && uTelEvtTypes > 0; j++ )
         {
            if ( uTelEvtTypes & 0x1 )
            {
               if ( mseNteMap[j] == 0 )
               {
                  /* DTMF group */
                  telEvt.SetTelEventSupport( CSdpFmtpTelEvent::eDTMF, true );
               }
               else
               {
                  telEvt.SetTelEventSupport( mseNteMap[j], true );
               }
            }
            uTelEvtTypes >>= 1;
         }
         telEvt.GetValue();
         telEvt.Validate();
         rStream.AddFmtp( telEvt );
      }

      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eTELEPHONE_EVENT,
                8000,
                uRtpcode );

      /* When a wideband codec is offered, we must add a codec for 
      ** telephone-event with timestamp that reflects wideband
      */
      if (bInclWB)
      {
         uRtpcode = pCfgCall->xtl.media.neg.uTelEvtWbRtpCode;

         /* Prepare a=fmtp for telephone-event, e.g. a=fmtp:97 0-15
                */
         uTelEvtTypes = pAudio->gen.uTelEvtTypes;
         if ( uTelEvtTypes > 0 )
         {
            CSdpFmtpTelEvent telEvtWb;
            telEvtWb.SetMediaFormat( uRtpcode );

            for ( j = 0; j < uMseNteMapSize && uTelEvtTypes > 0; j++ )
            {
               if ( uTelEvtTypes & 0x1 )
               {
                  if ( mseNteMap[j] == 0 )
                  {
                     /* DTMF group */
                     telEvtWb.SetTelEventSupport( CSdpFmtpTelEvent::eDTMF, true );
                  }
                  else
                  {
                     telEvtWb.SetTelEventSupport( mseNteMap[j], true );
                  }
               }
               uTelEvtTypes >>= 1;
            }
            telEvtWb.GetValue();
            telEvtWb.Validate();
            rStream.AddFmtp( telEvtWb );
         }
         addCodec( pCapsMgr,
                   uStreamIdx,
                   CSdpParser::eTELEPHONE_EVENT,
                   16000,
                   uRtpcode );
        }    
   }

   /* Insert Comfort Noise */
   if ( pAudio->gen.bCn )
   {
      BOS_UINT8 uRtpcode = pCfgCall->xtl.media.neg.uCnRtpCode;

      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eCN,
                8000,
                uRtpcode );
      
      /* When a wideband codec is offered, we must add a codec for 
      ** comfort-noise with timestamp that reflects wideband
                */
      if (bInclWB)
      {
         uRtpcode = pCfgCall->xtl.media.neg.uCnWbRtpCode;
         addCodec( pCapsMgr,
                   uStreamIdx,
                   CSdpParser::eCN,
                   16000,
                uRtpcode );
      }
   }

   /* Insert loopback caps to our local caps for offer processing */
   if ( pCfgCall->xtl.media.neg.bLbSupport && !bIsOffer )
   {
      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eLBENCAPRTP,
                8000,
                pCfgCall->xtl.media.neg.uEncapRtpCode );

      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eLBLBRTPLOOPBACK,
                8000,
                pCfgCall->xtl.media.neg.uRtpLbRtpCode );
   }

   /* a=silenceSupp */
   setSilSupp( rStream, pAudio->gen.eSilSupp );

   /* a=ptime / a=mptime */
   if ( bUseMptime && !mptimeStr.IsEmpty() )
   {
      CSdpFieldAttributeOther mptime;
      mptime.SetName( cMptimeStr );
      mptimeStr.TrimRightSide();
      mptime.SetValue( mptimeStr.CStr() );
      rStream.AddOtherAttribute( mptime );
   }
   else
   {
      /* Insert a=ptime, using the ptime for the first codec, assuming all
      ** audio codecs share the same ptime.
      */
      BOS_UINT32 uPtime = pAudio->gen.codec[0].uPtime;
      if ( uPtime > 0 )
      {
         CSdpFieldAttributePtime ptime;
         ptime.SetPacketTime( static_cast<int32_t>( uPtime ));
         rStream.SetPtime( ptime );
      }
   }

   /* a=maxptime */
   if ( pAudio->gen.uMaxPtime > 0 )
   {
      char cMaxPtimeValStr[MSE_PTIMELEN];
      snprintf( &cMaxPtimeValStr[0], MSE_PTIMELEN, "%d",
               static_cast<int> (pAudio->gen.uMaxPtime) );
      CSdpFieldAttributeOther maxptime;
      maxptime.SetName( cMaxPtimeStr );
      maxptime.SetValue( cMaxPtimeValStr );
      rStream.AddOtherAttribute( maxptime );
   }

   /* Media mode based on hold state */
   setMediaMode( rStream );

   /* Insert b=RS:0, b=RR:0 to disable RTCP */
   if ( pCfgCall->xtl.media.neg.bDisableRTCP )
   {
      addBandwidthAttrib( rStream, 0, 0 );
   }

#if CCTKCFG_SRTP
   /* Crypto attributes for SRTP */
   if ( bSecured )
   {
      addCryptoAttrib( rStream, &pAudio->sec.cryptoLst );
   }
#endif
}

#if CCTKCFG_SRTP
/***********************************************************************************
**  FUNCTION:   MseSdpSession::addCryptoAttrib
**
**  PURPOSE:    Set the list of local SRTP crypto information.
**
**  PARAMETERS: rStream - m-line stream object
**              pCryptoLst - pointer to the list of crypto information
**
**  RETURNS:    Nothing.
**
**  NOTES:      - The caller provides storage to pCapsMgr.
***********************************************************************************/
void MseSdpSession::addCryptoAttrib( CSdpLevelMedia &rStream,
                                     CCTKCRYPTOLST *pCryptoLst )
{
   BOS_UINT32 i, j;
   CCTKCSUITE eCryptoSuite;

   /* Sanity check */
   CCTK_ASSERT( pCryptoLst != NULL );

   /* Loop through a list of Crypto attributes */
   for ( i = 0; i < pCryptoLst->uNumCrypto; i++ )
   {
      CSdpFieldAttributeCrypto crypto;
      crypto.Reset();

      /* Set Tag - tag starts from 1 + index */
      BOS_UINT8 tag[MSE_CRYPTOTAGLEN];
      snprintf( reinterpret_cast<char *>( &tag[0] ), 
                MSE_CRYPTOTAGLEN, "%d", i + 1 );
      crypto.SetTag( reinterpret_cast<char *>( &tag[0] ));

      /* Set crypto suite */
      eCryptoSuite = pCryptoLst->crypto[i].eCryptoSuite;
      crypto.SetCryptoSuite( reinterpret_cast<const char *>
         ( findCryptoText( cctkCryptoSuite, eCryptoSuite )));

      /* Set key params */
      CCryptoKeyParamList *pKeyParmLst = crypto.GetKeyParams();
      pKeyParmLst->Reset();

      CCryptoKeyParam *pKeyParm = BRCM_NEW( CCryptoKeyParam );
      pKeyParm->Reset();

      /* Key method is always "inline" per RFC 4568 */
      pKeyParm->SetKeyMethod( cKeyMethodStr );

      /* Base64 encoding */
      BOS_UINT32 uKeyLength = findKeyLen( cctkKeyLen, eCryptoSuite );
      BOS_UINT8 cKeySalt[uMaxKeysaltLength];
      mxt_result res;
      CBase64 base64;
      res = base64.Begin( CCipher::eACTION_ENCRYPT, CCipher::eMODE_DEFAULT );
      CCTK_ASSERT( MX_RIS_S( res ));
      BOS_UINT8 cKeySaltBase64[uMaxKeysaltLength];
      uint8_t *pKeySalt = &cKeySaltBase64[0];
      unsigned int uOutSize;
      generateKeySalt( eCryptoSuite, uKeyLength, &cKeySalt[0] );
      res = base64.Update( &cKeySalt[0],
                           uKeyLength,
                           pKeySalt,
                           &uOutSize );
      CCTK_ASSERT( MX_RIS_S( res ) && uOutSize <= uMaxKeysaltLength );
      pKeySalt += uOutSize;
      res = base64.End( pKeySalt, &uOutSize );
      CCTK_ASSERT( MX_RIS_S( res ));
      pKeySalt[uOutSize] = '\0';

      pKeyParm->SetKey( reinterpret_cast<char *>( &cKeySaltBase64[0] ));
      pKeyParm->SetLifeTime( pCryptoLst->crypto[i].uLifeTime );
      pKeyParm->SetMkiValue( 0 ); /* MKI value not used */
      pKeyParm->SetMkiLength( 0 ); /* MKI value not used */
      CCTK_ASSERT( pKeyParm->Validate() == true );

      pKeyParmLst->Append( TO pKeyParm );

#if CCTKCFG_MAX_CRYPTO_SESPARM > 0
      /* If configured, loop through a list of session params per Crypto attribute */
      if ( pCryptoLst->crypto[i].uNumSessionParm > 0 )
      {
         /* Set session params if present */
         CCryptoSessionParamList *pSesParmList = crypto.GetSessionParams();
         pSesParmList->Reset();

         for ( j = 0; j < pCryptoLst->crypto[i].uNumSessionParm; j++ )
         {
            CCryptoSessionParam *pSesParm = BRCM_NEW( CCryptoSessionParam );
            pSesParm->Reset();
            pSesParm->SetName( reinterpret_cast<char *>
               ( &pCryptoLst->crypto[i].session[j].cToken[0] ));
            pSesParm->SetValue( reinterpret_cast<char *>
               ( &pCryptoLst->crypto[i].session[j].cVal[0] ));
            CCTK_ASSERT( pSesParm->Validate() == true );
            pSesParmList->Append( TO pSesParm );
         }
      }
#endif
      /* Validate and form the a=crypto line */
      CCTK_ASSERT( crypto.Validate() == true );
      rStream.AddCrypto( crypto );
   }
}
#endif

#if CCTKCFG_VIDEO
/***********************************************************************************
**  FUNCTION:   MseSdpSession::createLocalVideoSdp
**
**  PURPOSE:    Create a video SDP based on the local configured media profile.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP struct in CSdpCapabilitiesMgr format.
**              pVideo - Pointer to the video profile.
**              pMediaLst - Pointer to the media list.
**              bIsOffer - Indicates whether the local SDP is created for offer.
**              bSecured - Indicates whether this is a secured stream.
**              bEarlySession - Indicates whether the local SDP is created for an 
**                              early session.
**
**  RETURNS:    Nothing.
**
**  NOTES:      - The caller provides storage to pCapsMgr.
**              - When bUseMediaLst is BOS_FALSE, it is assumed that pMediaLst is
**                set to NULL and the entire media caps will be used.
***********************************************************************************/
void MseSdpSession::createLocalVideoSdp( CSdpCapabilitiesMgr *pCapsMgr,
                                         CCTKMEDVID *pVideo,
                                         CCTKMEDLST *pMediaLst,
                                         BOS_BOOL bIsOffer,
                                         BOS_BOOL bSecured,
                                         BOS_BOOL bEarlySession ) 
{
   BOS_UINT32 i, j, uStreamIdx;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

   if ( ( pMediaLst->mMediaTypes & CCTK_MEDIA_VIDEO_MASK ) == 0 ||
        pVideo == NULL || pVideo->gen.uNumCodec == 0 )
   {
      return;
   }

   /* Allocate a RTP/RTCP port */
   BOS_UINT16 uRtpPort =
      allocatePort( muCid, bSecured ? eMSEPTTYPE_VID_SRTP : eMSEPTTYPE_VID_RTP );
   BOS_UINT16 uRtcpPort =
      allocatePort( muCid, bSecured ? eMSEPTTYPE_VID_SRTCP : eMSEPTTYPE_VID_RTCP );

   /* Add video stream */
   pCapsMgr->AddStream( CSdpParser::eVIDEO,
#if CCTKCFG_SRTP
                        bSecured ? CSdpParser::eRTPSAVP : CSdpParser::eRTPAVP,
#else
                        CSdpParser::eRTPAVP,
#endif
                        uRtpPort,
                        reinterpret_cast<unsigned int &>( uStreamIdx ));

   CSdpLevelMedia &rStream = pCapsMgr->GetStream( uStreamIdx );

   /* Insert a=rtcp if used and not disabled */
   if ( !pCfgCall->xtl.media.neg.bDisableRTCP && pVideo->gen.rtcpPort.uMinPort > 0 )
   {
      CSdpFieldAttributeRtcp rtcpAttrib;
      rtcpAttrib.SetPort( uRtcpPort );
      rStream.SetSdpFieldAttributeRtcp( rtcpAttrib );
   }

   for ( i = 0; i < pVideo->gen.uNumCodec; i++ )
   {
      CCTKCODEC eCodec = pVideo->gen.codec[i].eType;
      BOS_BOOL bCodecFound = BOS_FALSE;

      /* Add codec only if it is on the media codec list */
      for ( j = 0; j < pMediaLst->uNumVidCodec; j++ )
      {
         if ( pMediaLst->vidCodeLst[j] == eCodec )
         {
            bCodecFound = BOS_TRUE;
            break;
         }
      }

      if ( !bCodecFound )
      {
         break;
      }

      BOS_UINT8 uRtpCode = pCfgCall->xtl.media.neg.uRtpCode[eCodec];

      addCodec( pCapsMgr, uStreamIdx, eCodec, uRtpCode );

      if ( eCodec == eCCTKCODEC_H263 )
      {
         CCTKH263PARM *pH263Parm = pVideo->gen.codec[i].parm.pH263;
         if ( pH263Parm != NULL && pH263Parm->bIsSpecified )
         {
            CSdpFmtpH263 fmtpH263;

            /* Set H.263 fmtp - e.g., a=fmtp:34 profile=0; level=30 */
            fmtpH263.SetMediaFormat( uRtpCode );
            fmtpH263.SetProfile( pH263Parm->uProfile );
            fmtpH263.SetLevel( pH263Parm->uLevel );

            fmtpH263.GetValue();
            fmtpH263.Validate();

            rStream.AddFmtp( fmtpH263 );
         }
      }
      else if ( eCodec == eCCTKCODEC_H264 )
      {
         CCTKH264PARM *pH264Parm = pVideo->gen.codec[i].parm.pH264;
         if ( pH264Parm != NULL && pH264Parm->bIsSpecified )
         {
            CSdpFmtpH264 fmtpH264;
            CSdpFmtpH264::EProfile eSipProf;
            CSdpFmtpH264::ELevel eSipLevel;

            switch ( pH264Parm->eProfile )
            {
#define CASE_H264_PROFILE( _sipProf, _ccProf  ) \
            case (_ccProf): { eSipProf = (_sipProf); } break;
                CASE_H264_PROFILE( CSdpFmtpH264::eBASELINE,
                                       eCCTKH264PROFILE_BASELINE );
                CASE_H264_PROFILE( CSdpFmtpH264::eMAIN,
                                       eCCTKH264PROFILE_MAIN );
                CASE_H264_PROFILE( CSdpFmtpH264::eEXTENDED,
                                       eCCTKH264PROFILE_EXTENDED );
#undef CASE_H264_PROFILE
                default:
                  eSipProf = CSdpFmtpH264::eBASELINE;
                  CCTK_ASSERT( eSipProf != CSdpFmtpH264::eBASELINE );
            }


            switch ( pH264Parm->eLevel )
            {
#define CASE_H264_LEVEL( _sipLevel, _ccLevel  ) \
            case (_ccLevel): { eSipLevel = (_sipLevel); } break;
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_0, eCCTKH264LEVEL_1_0 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_1, eCCTKH264LEVEL_1_1 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_2, eCCTKH264LEVEL_1_2 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_3, eCCTKH264LEVEL_1_3 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_0, eCCTKH264LEVEL_2_0 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_1, eCCTKH264LEVEL_2_1 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_2, eCCTKH264LEVEL_2_2 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_0, eCCTKH264LEVEL_3_0 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_1, eCCTKH264LEVEL_3_1 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_2, eCCTKH264LEVEL_3_2 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_0, eCCTKH264LEVEL_4_0 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_1, eCCTKH264LEVEL_4_1 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_2, eCCTKH264LEVEL_4_2 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_0, eCCTKH264LEVEL_5_0 );
               CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_1, eCCTKH264LEVEL_5_1 );
#undef CASE_H264_LEVEL
                default:
                  eSipLevel = CSdpFmtpH264::eLEVEL_1_0;
                  CCTK_ASSERT( eSipLevel != CSdpFmtpH264::eLEVEL_1_0 );
            }

            /* e.g., a=fmtp:97 profile-level-id=42A01E; packetization-mode=0;
            **         parameter-add=1; sprop-parameter-sets=Z0IACpZTBYmI,aMljiA==
            */
            fmtpH264.SetMediaFormat( uRtpCode );
            fmtpH264.SetProfile( eSipProf );
            fmtpH264.SetConstraintSet0(
               static_cast<char>( pH264Parm->bConstraintSet0 ));
            fmtpH264.SetConstraintSet1(
               static_cast<char>( pH264Parm->bConstraintSet1 ));
            fmtpH264.SetConstraintSet2(
               static_cast<char>( pH264Parm->bConstraintSet2 ));
            fmtpH264.SetLevel( eSipLevel );
            fmtpH264.SetPacketizationMode(
               static_cast<CSdpFmtpH264::EPacketizationMode>
               ( pH264Parm->ePacketizationMode ));
            fmtpH264.SetParameterAdd( pH264Parm->bParameterAdd == BOS_TRUE );
            fmtpH264.SetParameterSetsHex(
               reinterpret_cast<char *>( &pH264Parm->cPropParamSets[0] ));
            fmtpH264.SetMaxBr( pH264Parm->uMaxBr );
            fmtpH264.SetMaxMbps( pH264Parm->uMaxMbps );

            fmtpH264.GetValue();
            fmtpH264.Validate();

            rStream.AddFmtp( fmtpH264 );
         }
      }
   }

   /* Insert loopback caps to our local caps for offer processing */
   if ( pCfgCall->xtl.media.neg.bLbSupport && !bIsOffer )
   {
      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eLBENCAPRTP,
                8000,
                pCfgCall->xtl.media.neg.uEncapRtpCode );

      addCodec( pCapsMgr,
                uStreamIdx,
                CSdpParser::eLBLBRTPLOOPBACK,
                8000,
                pCfgCall->xtl.media.neg.uRtpLbRtpCode );
   }

   /* Media mode based on hold state */
   setMediaMode( rStream );

   /* Insert b=RS:0, b=RR:0 to disable RTCP */
   if ( pCfgCall->xtl.media.neg.bDisableRTCP )
   {
      addBandwidthAttrib( rStream, 0, 0 );
   }

#if CCTKCFG_SRTP
   /* Crypto attributes for SRTP */
   if ( bSecured )
   {
      addCryptoAttrib( rStream, &pVideo->sec.cryptoLst );
   }
#endif
}
#endif

/***********************************************************************************
**  FUNCTION:   MseSdpSession::createLocalT38Sdp
**
**  PURPOSE:    Create a T.38 SDP based on the local configured media profile.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP struct in CSdpCapabilitiesMgr format.
**              pFax - Pointer to the fax profile.
**              pMediaLst - Pointer to the media list.
**              bIsOffer - Indicates whether the local SDP is created for offer.
**              bEarlySession - Indicates whether the local SDP is created for an 
**                              early session.
**
**  RETURNS:    Nothing.
**
**  NOTES:      - The caller provides storage to pCapsMgr.
**              - When bUseMediaLst is BOS_FALSE, it is assumed that pMediaLst is
**                set to NULL and the entire media caps will be used.
***********************************************************************************/
void MseSdpSession::createLocalT38Sdp( CSdpCapabilitiesMgr *pCapsMgr,
                                       CCTKMEDFAX *pFax,
                                       CCTKMEDLST *pMediaLst, 
                                       BOS_BOOL bEarlySession ) 
{
   BOS_UINT32 uStreamIdx;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

   if ( ( pMediaLst->mMediaTypes & CCTK_MEDIA_T38_MASK ) && pFax != NULL )
   {
      /* Add T.38 stream */
      /* Allocate a T.38 port */
      BOS_UINT16 uT38Port = allocatePort( muCid, eMSEPTTYPE_T38 );

      pCapsMgr->AddStream( CSdpParser::eIMAGE,
                           CSdpParser::eUDPTL,
                           uT38Port,
                           reinterpret_cast<unsigned int &>( uStreamIdx ));

      unsigned int uFmtIdx;
      pCapsMgr->AddMediaFormat( static_cast<unsigned int>( uStreamIdx ),
                                cT38Str,
                                uFmtIdx );

      CSdpLevelMedia &rStream = pCapsMgr->GetStream( uStreamIdx );

      CSdpFieldAttributeVersion faxVersion;
      CSdpFieldAttributeT38FacsimileRateMgmnt faxRateMgmnt;
      CSdpFieldAttributeMaxDatagram maxDatagram;
      CSdpFieldAttributeT38ErrorControl faxUdpErrorControl;

      /* Set up the capacities of the T.38 stream */
      faxVersion.SetVersion( 0 );
      faxRateMgmnt.SetFacsimileRateMgmnt( cT38XferTCFStr );
      maxDatagram.SetMaxDatagram( pCfgCall->xtl.media.pFax->uMaxDatagram );
      faxUdpErrorControl.SetErrorControl( cT38UdpRedStr );

      /* Set the configured values in the media */
      rStream.SetVersion(faxVersion);
      rStream.SetT38FacsimileRateMgmnt(faxRateMgmnt);
      rStream.SetMaxDatagram(maxDatagram);
      rStream.SetT38ErrorControl(faxUdpErrorControl);

      /* Set ptime, when used */
      BOS_UINT32 uPtime = pCfgCall->xtl.media.pFax->uPtime;
      if ( uPtime > 0 )
      {
         CSdpFieldAttributePtime ptime;
         ptime.SetPacketTime( static_cast<int32_t>( uPtime ));
         rStream.SetPtime( ptime );
      }
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::createSdpBlob
**
**  PURPOSE:    Creates SDP blob storage.
**
**  PARAMETERS:
**              pCapsMgr - SDP capability manager.
**              bUseQuadZero - Indicates whether quad-zero c= address should be
**                             used.
**
**
**  RETURNS:    Pointer to blob storage.
**
**  NOTES:      Caller has ownership of CBlob.
***********************************************************************************/
CBlob *MseSdpSession::createSdpBlob( CSdpCapabilitiesMgr *pCapsMgr,
                                     BOS_BOOL bUseQuadZero )
{
   char cVersion[uSdpVerStrSize];
   CSdpPacket *pSdpPacket;
   BOS_BOOL bSdpChanged = BOS_FALSE;
   const CFGCALL *pCfgCall = CFG_CALL( muCid );


   char cLocAddr[uIpAddrStrSize];
   const char *pcMedAddr;
   
   /* Convert media address to string format */
   bosIpAddrGetStr( &pCfgCall->xtl.media.ipaddr,
                    &cLocAddr[0],
                    uIpAddrStrSize );

   /* Media IP address */
   pcMedAddr = &cLocAddr[0];

   if ( bUseQuadZero || ( isHold( meLocHoldSt ) && !mbNoQuadZeros ))
   {
      pcMedAddr = &cQuadZerosStr[0];
   }

   /* Create SDP packet using the current version number */
   pSdpPacket = BRCM_NEW( CSdpPacket );
   snprintf( &cVersion[0], uSdpVerStrSize, "%lu", muSdpVer );
   pCapsMgr->CreateSdpPacket( &cLocAddr[0],
                              pcMedAddr,
                              &cVersion[0],
                              *pSdpPacket );

   if (mpPrevSdpPacket == NULL)
   {
      /* First SDP, tag this as a "change" so that we cache the SDP */
      bSdpChanged = BOS_TRUE;

      /* since this is the first time we create the SDP, set the session ID
      ** and version based on the NTP timestamp
      */
      BOS_TIME_SEC osTimeSec;
      BOS_TIME_USEC osTimeMicroSec;

      bosTimeGetTimeOfDay (&osTimeSec, &osTimeMicroSec);
      muSdpVer = osTimeSec + pCfgCall->xtl.media.sNtpTimeAdj;
      snprintf( &cVersion[0], uSdpVerStrSize, "%lu", muSdpVer );

      pSdpPacket->GetSession().GetOrigin().SetSessionId ( &cVersion[0] );
      pSdpPacket->GetSession().GetOrigin().SetVersion( &cVersion[0] );
   }
   /* Increments SDP version if the SDP is changed from the previous one */
   else if ( mpPrevSdpPacket != NULL && !( *mpPrevSdpPacket == *pSdpPacket ))
   {
      /* Set Session ID for the SDP packet */
      pSdpPacket->GetSession().GetOrigin().SetSessionId ( 
               mpPrevSdpPacket->GetSession().GetOrigin().GetSessionId() );

      /* Increments SDP version */
      muSdpVer++;
      snprintf( &cVersion[0], uSdpVerStrSize, "%lu", muSdpVer );
      pSdpPacket->GetSession().GetOrigin().SetVersion( &cVersion[0] );

      bSdpChanged = BOS_TRUE;
   }

   /* Create storage for the SDP */
   CBlob *pSdpBlob = BRCM_NEW( CBlob );
   pSdpPacket->Serialize( *pSdpBlob );

   /* SDP changed, cache the caps manager and media address */
   if ( bSdpChanged )
   {
      if ( mpPrevSdpPacket != NULL )
      {
         BRCM_DELETE( mpPrevSdpPacket );
      }
      mpPrevSdpPacket = pSdpPacket;
   }
   else
   {
      BRCM_DELETE( pSdpPacket );
   }
   pSdpPacket = NULL;

   return pSdpBlob;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::convertSdpFromMx
**
**  PURPOSE:    Convert SDP from M5T CSdpCapabilitiesMgr to CCTK format
**              (CCTKSTRMINFO).
**
**  PARAMETERS:
**              pCapsMgr - M5T caps structure.
**              pStreamInfo - CCTK stream info format (CCTKSTRMINFO).
**              bIngress - Indicates whether or not the stream is Ingress
**                         (or egress).
**              bUnsecuredOnly - instructs to build only unsecured media information, i.e. 
**                         drops the secured one. Needed when near end does not support secured conns
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      pIpAddr->addr would be set to 0xFFs if the media address is a FQDN
**              and the lookup failed.
***********************************************************************************/
void MseSdpSession::convertSdpFromMx( CSdpCapabilitiesMgr *pCapsMgr,
                                      CCTKSTRMINFO *pStreamInfo,
                                      BOS_BOOL bIngress,
                                      BOS_BOOL bUnsecuredOnly )
{
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
   BOS_UINT32 i, j;

   /* Reset the codec info */
   memset( pStreamInfo, 0, sizeof( CCTKSTRMINFO ));

   /* Convert IP address */
   if ( bIngress )
   {
      convertIpFromMx( pCapsMgr, &pStreamInfo->addr );
   }
   else
   {
      /* For egress IP address, we copy from the configured media address as the
      ** information is not available from CSdpCapsMgr.  In the case we are locally
      ** hold and "quad-zeroes" is used, then we set the address accordingly.
      **
      ** Essentially what is set here should be identical to the media address that
      ** appears in the sent SDP. */
      if ( isHold( meLocHoldSt ) && !mbNoQuadZeros )
      {
         bosIpAddrCreateZero( BOS_IPADDRESS_TYPE_V4, &pStreamInfo->addr );
      }
      else
      {
         bosIpAddrCreateCopy( &pCfgCall->xtl.media.ipaddr, &pStreamInfo->addr );
#if CCTKCFG_ICE_LITE_SUPPORT
         /* When EDVA is in dual-stack, the address used for media can be 
         ** different from the Call configured media address.  
         ** ICE-lite code has populated/updated the media address of the 
         ** CSdpCapsMgr to the correct address.  Thus, EDVA can just use
         ** the media address in the CSdpCapsMgr.
         */
         convertIpFromMx( pCapsMgr, &pStreamInfo->addr );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */         
      }
   }

   /* Iterate through the stream list */
   BOS_UINT32 uNumStreams = pCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      CSdpLevelMedia &rStream = pCapsMgr->GetStream( i );

      if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
           ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
             || pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#endif
           ))
      {
         if ((pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP) && 
             ((rStream.GetNbCrypto() == 0) || (bUnsecuredOnly == BOS_TRUE)))
         {
            /* skip secure RTP m-line if no crypto information is present */
            continue;
         }

         /* Audio RTP support */
         CCTKAUDINFO *pAudInfo = &pStreamInfo->audio;

         /* RTP port */
         BOS_UINT16 uRtpPort =
            static_cast<BOS_UINT16>( pCapsMgr->GetStreamPort( i ));

         /* RTCP port, use the RTP port info (+ 1) by default */
         BOS_UINT16 uRtcpPort = uRtpPort + 1;
         if ( rStream.GetSdpFieldAttributeRtcp().GetPort() != -1 )
         {
            /* a=rtcp exists */
            uRtcpPort = static_cast<BOS_UINT16>
               ( rStream.GetSdpFieldAttributeRtcp().GetPort() );
         }

         /* Check if b=RR:0 is included.  If so, disable RTCP. */
         CVector <CString> &bandwidth = rStream.GetBandwidth();
         CString bandwidthStr = cRrZeroStr;
         if ( bandwidth.Find( 0, bandwidthStr ) != bandwidth.GetSize() )
         {
            /* Match found, disable RTCP by setting the port to 0. */
            uRtcpPort = 0;
         }

         BOS_BOOL bRtcpXrSupport = BOS_FALSE;
         if ( uRtcpPort != 0 && isRtcpXrMetricsValidMx( rStream ))
         {
            CCTKMEDAUD *pAudio = pCfgCall->xtl.media.pAudio;
            if (!((pAudio != NULL) && (pAudio->qos.bRtcpXrSupport == BOS_FALSE)))
            {
               /* only set rtcp-xr support if cfg does not disable it */
            bRtcpXrSupport = BOS_TRUE;
         }
         }

         /* Check if mptime is available */
         BOS_BOOL bMptime = isMptimeAvailableMx( rStream );

         /* If mptime does not exist, check for ptime */
         BOS_UINT32 uPtime = 0;
         if ( !bMptime )
         {
            uPtime = getPtimeFromMx( rStream );
         }

         /* Retrieve a=silenceSupp */
         CCTKSILSUPP eSilSupp = getSilSuppFromMx( rStream );

         /* Loopback support, if enabled */
         BOS_BOOL bIsSrc;
         CString lbFmt;
         BOS_UINT32 uPayloadCount = 0; /* Payload to remove after parsing */
         if ( pCfgCall->xtl.media.neg.bLbSupport &&
              getLoopbackMode( rStream, bIsSrc, lbFmt ) )
         {
            pAudInfo->mode = eCCTKMEDMODE_LOOPBACK;
            pAudInfo->lb.bIsSrc = bIsSrc;

            BOS_UINT32 uLbType = getLoopbackType( rStream );

            /* Media loopback preferred over Packet loopback */
            if (( uLbType & MSELBTYPE_MEDIALB ) > 0 )
            {
               pAudInfo->lb.eLbType = eCCTKLBTYPE_MEDIALB;
            }
            else /* if (( uLbType & MSELBTYPE_PKTLB ) > 0 ) */
            {
               pAudInfo->lb.eLbType = eCCTKLBTYPE_PKTLB;

               /* Add the Media Format to the Payload list for parsing
               */
               BOS_UINT32 uFmtSize = lbFmt.GetSize();
               char *pcMediaFmt = BRCM_NEW_ARRAY( char, uFmtSize + 1 );
               strncpy( pcMediaFmt, lbFmt.CStr(), uFmtSize + 1 );

               char *pcPayload = strtok( pcMediaFmt, " " );
               unsigned int uFmtIdx;
               while ( pcPayload != NULL )
               {
                  pCapsMgr->AddMediaFormat( i, pcPayload, uFmtIdx );
                  uPayloadCount++;
                  pcPayload = strtok( NULL, " " );
               }
               BRCM_DELETE_ARRAY( pcMediaFmt );
               pcMediaFmt = NULL;
            }
            
            if ((uLbType & MSELBTYPE_STARTLB ) > 0)
            {
               pAudInfo->lb.bIsStart = BOS_TRUE;
            }
         }

         /* Iterate through the RTP payload list */
         BOS_UINT32 uNumRtps = pCapsMgr->GetNbPayloadTypes( i );
         BOS_UINT8  first = 0;
#if CCTKCFG_RED
         /* Index of redundancy payload mapping */
         BOS_SINT32 sRedIdx = -1;
#endif
         for ( j = 0; j < uNumRtps; j++ )
         {
            CSdpFieldAttributeRtpmap rtpmap;
            pCapsMgr->GetPayloadType( i, j, rtpmap );

            CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
            BOS_UINT8 uRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

            /* Telephone-event */
            if ( eMxCodec == CSdpParser::eTELEPHONE_EVENT && first == 0)
            {
               /* Assume that there is only one instance of telephone-event if there
               ** are multiple m= lines.  If there are multiple instances, then only
               ** the last one would be used.
               */
               first = 1;
               /* The above assumption has limit that while both wideband and narrowband
               DTMF NTE existed. The first preference should be used. */
               pAudInfo->bTelEvtUsed = BOS_TRUE;
               if (static_cast<BOS_UINT32>( rtpmap.GetClockRate() ) == 16000)
               {
                  pAudInfo->televt.uRtpCodeWB = uRtpCode;
               }
               else
               {
                  pAudInfo->televt.uRtpCode = uRtpCode;
               }
               pAudInfo->televt.uRtpPort = uRtpPort;
               pAudInfo->televt.uRtcpPort = uRtcpPort;
               pAudInfo->televt.bRtcpXrSupport = bRtcpXrSupport;

               /* telephone-event fmtp */
               /* Here we assume that the entire DTMF group is supported as long as
               ** the DTMF_0 event is on the list.
               */
               const CSdpFmtpTelEvent *telEvt = rStream.GetFmtpTelEvent();
               if ( telEvt )
               {
                  BOS_UINT32 k, uEvtTypes = 0;
                  for ( k = 0; k < uMseNteMapSize; k++ )
                  {
                     if ( telEvt->IsTelephoneEventSupported( mseNteMap[k] ))
                     {
                        uEvtTypes |= ( 1 << k );
                     }
                  }
                  pAudInfo->televt.uEvtTypes = uEvtTypes;
               }
               else
               {
                  /* As per RFC 2833, events 0 - 15 are supported by default */
                  pAudInfo->televt.uEvtTypes = CCTKNTE_DTMF;
               }
            }
            /* Comfort noise */
            else if ( eMxCodec == CSdpParser::eCN )
            {
               /* Assume that there is only one instance of telephone-event if there
               ** are multiple m= lines.  If there are multiple instances, then only
               ** the last one would be used.
               */
               pAudInfo->bCnUsed = BOS_TRUE;
               if (static_cast<BOS_UINT32>( rtpmap.GetClockRate() ) == 16000)
               {
                  pAudInfo->cn.uRtpCodeWB = uRtpCode;
               }
               else
               {
                  pAudInfo->cn.uRtpCode = uRtpCode;
               }
               pAudInfo->cn.uRtpPort = uRtpPort;
               pAudInfo->cn.uRtcpPort = uRtcpPort;
               pAudInfo->cn.bRtcpXrSupport = bRtcpXrSupport;
            }
            else if ( ( eMxCodec == CSdpParser::eLBENCAPRTP ||
                        eMxCodec == CSdpParser::eLBLBRTPLOOPBACK ) &&
                      pAudInfo->mode == eCCTKMEDMODE_LOOPBACK &&
                      pAudInfo->lb.eLbType == eCCTKLBTYPE_PKTLB &&
                      pAudInfo->lb.uRtpCode == 0 /* Assign only once */ )
            {
               if ( eMxCodec == CSdpParser::eLBENCAPRTP )
               {
                  pAudInfo->lb.ePktLbType = eCCTKPKTLBTYPE_ENCAPRTP;
               }
               else
               {
                  pAudInfo->lb.ePktLbType = eCCTKPKTLBTYPE_RTPLB;
               }
               pAudInfo->lb.uRtpCode = uRtpCode;
            }
            else if ( pAudInfo->uNumCodec < CCTKCFG_MAX_AUDIO_CODEC )
            {
               BOS_UINT32 uRate = static_cast<BOS_UINT32>( rtpmap.GetClockRate() );
               const MSECODECMAP *pCodecMap = getCodecMap( eMxCodec, uRate );
               if ( pCodecMap->eCodec >= eCCTKCODEC_MAX )
               {
                  /* Bypass unrecognized RTP codec type */
                  continue;
               }

               CCTKCODECINFO *pCodecInfo = &pAudInfo->codec[pAudInfo->uNumCodec];
               pCodecInfo->eType = pCodecMap->eCodec;
               pCodecInfo->uRtpCode = uRtpCode;
               pCodecInfo->uRtpPort = uRtpPort;
               pCodecInfo->uRtcpPort = uRtcpPort;
               pCodecInfo->bRtcpXrSupport = bRtcpXrSupport;
               pCodecInfo->eSilSupp = eSilSupp;

               /* As per-mptime specification, use ptime only if mptime is
               ** unavailable.
               */
               pCodecInfo->uPtime =
                  bMptime ? getMptimeFromMx( rStream, j ) : uPtime;

               if ( pCodecMap->eCodec == eCCTKCODEC_ILBC )
               {
                  CSdpFieldAttributeFmtp *pFmtpIlbc =
                     rStream.GetFmtpFromPayloadType( uRtpCode );

                  if ( pFmtpIlbc != NULL )
                  {
                     const char *pcIlbcParm = pFmtpIlbc->GetValue();
                     if ( strcmp( pcIlbcParm, cIlbcMode20msStr ) == 0 )
                     {
                        pCodecInfo->parm.eIlbcMode = eCCTKILBCMODE_20MS;
                     }
                     else if ( strcmp( pcIlbcParm, cIlbcMode30msStr ) == 0 )
                     {
                        pCodecInfo->parm.eIlbcMode = eCCTKILBCMODE_30MS;
                     }
                     else
                     {
                        pCodecInfo->parm.eIlbcMode = eCCTKILBCMODE_NOTUSED;
                     }
                  }
               }
#if CCTKCFG_RED
               else if (pCodecMap->eCodec == eCCTKCODEC_RED)
               {
                  /* If redundancy is specified in the SDP, we will need to map
                   * all the specified payloads to the appropriate CCTK codec
                   * types, but this should be done after all the codecs are
                   * parsed. For now, save the index of where the redundancy
                   * codec is in the audio info list. */
                  sRedIdx = pAudInfo->uNumCodec;
               }
#endif
               else if ( pCodecMap->eCodec == eCCTKCODEC_G7231 )
               {
                  BOS_BOOL bAnnexAUsed =
                     static_cast<BOS_BOOL>( pCapsMgr->IsVadSupportedInStream(
                        CSdpParser::ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                        uRtpCode,
                        rStream ));

                  if ( bAnnexAUsed )
                  {
                     pCodecInfo->parm.eG7231Vad = eCCTKG7231VAD_YES;
                  }
                  else
                  {
                     pCodecInfo->parm.eG7231Vad = eCCTKG7231VAD_NO;
                  }
               }
               else if ( pCodecMap->eCodec == eCCTKCODEC_G729E ||
                         pCodecMap->eCodec == eCCTKCODEC_G729 )
               {
                  BOS_BOOL bAnnexBUsed =
                     static_cast<BOS_BOOL>( pCapsMgr->IsVadSupportedInStream(
                        CSdpParser::ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                        uRtpCode,
                        rStream ));

                  if ( bAnnexBUsed )
                  {
                     pCodecInfo->parm.eG729Vad = eCCTKG729VAD_YES;
                  }
                  else
                  {
                     pCodecInfo->parm.eG729Vad = eCCTKG729VAD_NO;
                  }
               }
               else if ( pCodecMap->eCodec == eCCTKCODEC_AMRWB ||
                         pCodecMap->eCodec == eCCTKCODEC_GSMAMR )
               {
                  CSdpFieldAttributeFmtp *pFmtpAmr =
                     rStream.GetFmtpFromPayloadType( uRtpCode );
                  CCTKAMRPARM *pAmrCfg = &pCodecInfo->parm.amrCfg;

                  getAmrCfgFromMx( pFmtpAmr, pAmrCfg,
                        (pCodecMap->eCodec == eCCTKCODEC_AMRWB) );
               }

               /* TODO: RTP fmtp support */

               pAudInfo->uNumCodec++;
            }
         }

         /* Look for maxptime (if any) */
         pAudInfo->uMaxPtime = getMaxPtimeFromMx( rStream );

         /* Audio mode */
         if ( pAudInfo->mode != eCCTKMEDMODE_LOOPBACK )
         {
            if ( rStream.IsInactive() || rStream.IsSendOnly() )
            {
               pAudInfo->mode = eCCTKMEDMODE_INACTIVE;
            }
            else
            {
               pAudInfo->mode = eCCTKMEDMODE_ACTIVE;
            }
         }

         /* Remove payload (for loopback support only) */
         while ( uPayloadCount > 0 )
         {
            BOS_UINT32 uNumRtps = pCapsMgr->GetNbPayloadTypes( i );
            CCTK_ASSERT( uNumRtps > 0 );
            pCapsMgr->RemovePayloadType( i, uNumRtps - 1 );
            uPayloadCount--;
         }

#if CCTKCFG_RED
         if (sRedIdx != -1)
         {
            const CSdpFmtpRedundancy *pRedFmtp = rStream.GetFmtpRedundancy();
            CCTKCODECINFO *pRedCodecInfo = &pAudInfo->codec[sRedIdx];

            pRedCodecInfo->parm.redCfg.uNumCodec = 0;
            /* If the redundancy fmtp is specified, parse all the rtp payloads
             * and match them to the appropriate CCTK codecs. If none are
             * specified, it means the remote can receive any configuration of
             * redundancy, so no need to fill anything in. */
            if (pRedFmtp)
            {
               BOS_UINT32 k, l;
               CVector <CString> redCodecs = pRedFmtp->GetRedundancyMediaFormats();

               for (k = 0; k < redCodecs.GetSize(); k++)
               {
                  BOS_UINT32 uRtpPayload = atoi(redCodecs[k].CStr());
                  CCTK_TRACEI1("fmt rtp payload: %d", uRtpPayload);
                  for (l = 0; l < pAudInfo->uNumCodec; l++)
                  {
                     if(pAudInfo->codec[l].uRtpCode == uRtpPayload)
                     {
                        pRedCodecInfo->parm.redCfg.eCodec[pRedCodecInfo->parm.redCfg.uNumCodec] =
                           pAudInfo->codec[l].eType;
                        pRedCodecInfo->parm.redCfg.uNumCodec++;
                        break;
                     }
                  }
               }
            }
         }
#endif

#if CCTKCFG_SRTP
         /* Get crypto info */
         getCrypto( rStream, &pAudInfo->crypto );
#endif
      }
#if CCTKCFG_VIDEO
      else if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eVIDEO &&
           ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
             || pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#endif
             ))
      {
         /* Video RTP support */
         CCTKVIDINFO *pVidInfo = &pStreamInfo->video;

         /* RTP port */
         BOS_UINT16 uRtpPort =
            static_cast<BOS_UINT16>( pCapsMgr->GetStreamPort( i ));

         /* RTCP port, use the RTP port info (+ 1) by default */
         BOS_UINT16 uRtcpPort = uRtpPort + 1;
         if ( rStream.GetSdpFieldAttributeRtcp().GetPort() != -1 )
         {
            /* a=rtcp exists */
            uRtcpPort = static_cast<BOS_UINT16>
               ( rStream.GetSdpFieldAttributeRtcp().GetPort() );
         }

         /* Check if b=RR:0 is included.  If so, disable RTCP. */
         CVector <CString> &bandwidth = rStream.GetBandwidth();
         CString bandwidthStr = cRrZeroStr;
         if ( bandwidth.Find( 0, bandwidthStr ) != bandwidth.GetSize() )
         {
            /* Match found, disable RTCP by setting the port to 0. */
            uRtcpPort = 0;
         }

         BOS_BOOL bRtcpXrSupport = BOS_FALSE;
         if ( uRtcpPort != 0 && isRtcpXrMetricsValidMx( rStream ))
         {
            CCTKMEDAUD *pAudio = pCfgCall->xtl.media.pAudio;
            if (!((pAudio != NULL) && (pAudio->qos.bRtcpXrSupport == BOS_FALSE)))
            {
               /* only set rtcp-xr support if cfg does not disable it */
            bRtcpXrSupport = BOS_TRUE;
         }
         }

         /* Iterate through the RTP payload list */
         BOS_UINT32 uNumRtps = pCapsMgr->GetNbPayloadTypes( i );
         for ( j = 0; j < uNumRtps; j++ )
         {
            CSdpFieldAttributeRtpmap rtpmap;
            pCapsMgr->GetPayloadType( i, j, rtpmap );

            CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
            BOS_UINT8 uRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

            if ( pVidInfo->uNumCodec < CCTKCFG_MAX_VIDEO_CODEC )
            {
               BOS_UINT32 uRate = static_cast<BOS_UINT32>( rtpmap.GetClockRate() );
               const MSECODECMAP *pCodecMap = getCodecMap( eMxCodec, uRate );
               if ( pCodecMap->eCodec >= eCCTKCODEC_MAX )
               {
                  /* Bypass unrecognized RTP codec type */
                  continue;
               }

               CCTKCODECINFO *pCodecInfo = &pVidInfo->codec[pVidInfo->uNumCodec];
               pCodecInfo->eType = pCodecMap->eCodec;
               pCodecInfo->uRtpCode = uRtpCode;
               pCodecInfo->uRtpPort = uRtpPort;
               pCodecInfo->uRtcpPort = uRtcpPort;
               pCodecInfo->bRtcpXrSupport = bRtcpXrSupport;

               /* Handle RTP fmtp */
               if ( eMxCodec == CSdpParser::eH263 )
               {
                  CSdpFmtpH263 *pFmtpH263 = rStream.GetFmtpH263();
                  CCTKH263PARM *pH263Parm = &pCodecInfo->parm.h263;

                  if ( pFmtpH263 != NULL )
                  {
                     pH263Parm->bIsSpecified = BOS_TRUE;
                     pH263Parm->uProfile = pFmtpH263->GetProfile();
                     pH263Parm->uLevel = pFmtpH263->GetLevel();
                  }
                  else
                  {
                     pH263Parm->bIsSpecified = BOS_FALSE;
                  }
               }
               else if ( eMxCodec == CSdpParser::eH264 )
               {
                  CSdpFmtpH264 *pFmtpH264 = rStream.GetFmtpH264();
                  CCTKH264PARM *pH264Parm = &pCodecInfo->parm.h264;

                  if ( pFmtpH264 != NULL )
                  {
                     pH264Parm->bIsSpecified = BOS_TRUE;
                     pH264Parm->bConstraintSet0 = pFmtpH264->GetConstraintSet0();
                     pH264Parm->bConstraintSet1 = pFmtpH264->GetConstraintSet1();
                     pH264Parm->bConstraintSet2 = pFmtpH264->GetConstraintSet2();
                     pH264Parm->uMaxBr = pFmtpH264->GetMaxBr();
                     pH264Parm->uMaxMbps = pFmtpH264->GetMaxMbps();
                     pH264Parm->ePacketizationMode =
                        static_cast<CCTKH264PACKETIZATIONMODE>
                           ( pFmtpH264->GetPacketizationMode() );
                     pH264Parm->bParameterAdd = pFmtpH264->GetParameterAdd();
                     pFmtpH264->GetParameterSetsHex(
                        reinterpret_cast<char *>( &pH264Parm->cPropParamSets[0] ),
                        CCTKSDP_MAX_H264_PARM_SETS_LEN );

                     switch ( pFmtpH264->GetProfile() )
                     {
#define CASE_H264_PROFILE( sipProf, ccProf ) \
            case (sipProf): { pH264Parm->eProfile = (ccProf); } break;
                        CASE_H264_PROFILE( CSdpFmtpH264::eBASELINE,
                                           eCCTKH264PROFILE_BASELINE );
                        CASE_H264_PROFILE( CSdpFmtpH264::eMAIN,
                                           eCCTKH264PROFILE_MAIN );
                        CASE_H264_PROFILE( CSdpFmtpH264::eEXTENDED,
                                           eCCTKH264PROFILE_EXTENDED );
#undef CASE_H264_PROFILE
                     }

                     switch ( pFmtpH264->GetLevel() )
                     {
#define CASE_H264_LEVEL( sipLevel, ccLevel ) \
            case (sipLevel): { pH264Parm->eLevel = (ccLevel); } break;
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_0,
                                               eCCTKH264LEVEL_1_0 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_1,
                                               eCCTKH264LEVEL_1_1 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_2,
                                               eCCTKH264LEVEL_1_2 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_3,
                                               eCCTKH264LEVEL_1_3 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_0,
                                               eCCTKH264LEVEL_2_0 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_1,
                                               eCCTKH264LEVEL_2_1 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_2,
                                               eCCTKH264LEVEL_2_2 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_0,
                                               eCCTKH264LEVEL_3_0 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_1,
                                               eCCTKH264LEVEL_3_1 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_2,
                                               eCCTKH264LEVEL_3_2 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_0,
                                               eCCTKH264LEVEL_4_0 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_1,
                                               eCCTKH264LEVEL_4_1 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_2,
                                               eCCTKH264LEVEL_4_2 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_0,
                                               eCCTKH264LEVEL_5_0 );
                        CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_1,
                                               eCCTKH264LEVEL_5_1 );
#undef CASE_H264_LEVEL
                     }
                  }
                  else
                  {
                     pH264Parm->bIsSpecified = BOS_FALSE;
                  }
               }

               pVidInfo->uNumCodec++;
            }
         }

         /* Video mode */
         if ( pVidInfo->mode != eCCTKMEDMODE_LOOPBACK )
         {
            if ( rStream.IsInactive() || rStream.IsSendOnly() )
            {
               pVidInfo->mode = eCCTKMEDMODE_INACTIVE;
            }
            else
            {
               pVidInfo->mode = eCCTKMEDMODE_ACTIVE;
            }
         }

#if CCTKCFG_SRTP
         /* Get crypto info */
         getCrypto( rStream, &pVidInfo->crypto );
#endif
      }
#endif
      else if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eIMAGE &&
                ( pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eUDPTL ||
                  pCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eudptl ))
      {
         CSdpFieldMediaAnnouncement& rMediaAnn = rStream.GetMediaAnnouncement();
         if ( rMediaAnn.GetNbMediaFormats() > 0 &&
              strcmp( rMediaAnn.GetMediaFormat( 0 ), cT38Str ) == 0 )
         {
            /* T.38 support */
            CCTKT38INFO *pT38Info = &pStreamInfo->t38;

            /* T.38 port */
            pT38Info->uPort =
               static_cast<BOS_UINT16>( pCapsMgr->GetStreamPort( i ));

            /* T.38 Redundancy */
            CSdpFieldAttributeT38ErrorControl &rFaxUdpErrorControl =
               rStream.GetT38ErrorControl();

            if (
              strcmp( rFaxUdpErrorControl.GetErrorControl(), cT38UdpRedStr ) == 0 ||
              strcmp( rFaxUdpErrorControl.GetErrorControl(), cT38UdpFECStr ) == 0 )
            {
               pT38Info->bRedundancy = BOS_TRUE;
            }
            else
            {
               pT38Info->bRedundancy = BOS_FALSE;
            }
         }
      }
   }

   /* Override the generated ingress media info when we put the remote party on
   ** hold.
   */
   if ( bIngress &&
        ( meLocHoldSt == eMSEHOLDST_HOLD ||
          meLocHoldSt == eMSEHOLDST_HOLD_PENDING ))
   {
      if ( !mbNoQuadZeros )
      {
         memset( &pStreamInfo->addr, 0, sizeof( BOS_IP_ADDRESS ));
      }

      if ( pStreamInfo->audio.mode == eCCTKMEDMODE_ACTIVE )
      {
         pStreamInfo->audio.mode = eCCTKMEDMODE_INACTIVE;
      }

#if CCTKCFG_VIDEO
      if ( pStreamInfo->video.mode == eCCTKMEDMODE_ACTIVE )
      {
         pStreamInfo->video.mode = eCCTKMEDMODE_INACTIVE;
      }
#endif
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::convertIpFromMx
**
**  PURPOSE:    Extract media IP address from the M5T caps structure.
**
**  PARAMETERS:
**              pCapsMgr - M5T caps structure.
**              pIpAddr - Media IP address.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      pIpAddr->addr would be set to 0xFFs if the media address is a FQDN
**              and the lookup failed.
***********************************************************************************/
void MseSdpSession::convertIpFromMx( CSdpCapabilitiesMgr *pCapsMgr,
                                     BOS_IP_ADDRESS *pIpAddr )
{
   /* Sanity check */
   CCTK_ASSERT( pCapsMgr != NULL && pIpAddr != NULL );

   memset( pIpAddr, 0, sizeof( BOS_IP_ADDRESS ));

   if ( pCapsMgr->GetNbStreams() > 0 &&
        pCapsMgr->GetStream( 0 ).GetNbConnectionDatas() > 0 )
   {
      char *pcStreamName;
      CString streamAddr;

      /* Retrive media address.
      ** Assume that all m= line share the same IP, using stream index 0.
      */
      pCapsMgr->GetStreamAddr( 0, streamAddr );
      pcStreamName = const_cast<char *>( streamAddr.CStr() );

      CSocketAddr socketAddr;
#if CCTKCFG_IPV6
      if ( pCapsMgr->GetStream( 0 ).GetConnectionData( 0 ).GetAddressTypeId() ==
              CSdpParser::eIP6 )
      {
         bosIpAddrV6CreateZero( pIpAddr );

         if ( !GetHostByName( pcStreamName,
                              socketAddr,
                              CSocketAddr::eINET6 ))
         {
            /* Distinguish between IPv6 address and failure FQDN lookup */
            if ( MX_RIS_F( socketAddr.SetAddress( streamAddr.CStr() )))
            {
               /* We have a FQDN lookup failure */
               memset( &pIpAddr->u.ipv6Address,
                       0xFF,
                       sizeof( pIpAddr->u.ipv6Address ));
               return;
            }
         }
      }
      else
#endif
      {
         char cStreamNameBuf[uIpAddrStrSize];

         if ( GetHostByName( pcStreamName, socketAddr ))
         {
            /* Media address is a FQDN */
            socketAddr.GetAddress( sizeof( cStreamNameBuf ),
                                   &cStreamNameBuf[0] );
            pcStreamName = &cStreamNameBuf[0];
         }
         else if ( strcmp( pcStreamName, cQuadZerosStr ) != 0 )
         {
            /* We have a FQDN lookup failure */
            bosIpAddrV4CreateFromU32( 0xFFFFFFFF, pIpAddr );
            return;
         }
      }

      /* Convert IP address to BOS format */
      bosIpAddrCreateFromStr( pcStreamName, pIpAddr );
   }
}

#if CCTKCFG_SRTP
/***********************************************************************************
**  FUNCTION:   findCryptoText
**
**  PURPOSE:    Find the crypto text given a crypto in enum.
**
**  PARAMETERS: pMap - Pointer to the crypto enum/text mapping table.
**              eCryptoSuite - Crypto suite.
**
**  RETURNS:    Crypto text.
**
**  NOTES:
***********************************************************************************/
const BOS_UINT8 *MseSdpSession::findCryptoText( const CCTKCRYPTOMAP *pMap,
                                                CCTKCSUITE eCryptoSuite )
{
   CCTK_ASSERT( pMap != NULL );

   while ( pMap->eCryptoSuite != eCryptoSuite &&
           pMap->eCryptoSuite != eCCTKCSUITE_MAX )
   {
      pMap++;
   }

   return pMap->pText;
}

/***********************************************************************************
**  FUNCTION:   findCryptoType
**
**  PURPOSE:    Find the crypto enum type given a crypto in text.
**
**  PARAMETERS: pMap - Pointer to the crypto enum/text mapping table.
**              pText - Crypto text.
**
**  RETURNS:    Crypto enum.
**
**  NOTES:
***********************************************************************************/
CCTKCSUITE MseSdpSession::findCryptoType( const CCTKCRYPTOMAP *pMap,
                                          const BOS_UINT8 *pText )
{
   CCTK_ASSERT( pMap != NULL );

   while ( strcmp( reinterpret_cast<const char *>( pMap->pText ),
                   reinterpret_cast<const char *>( pText )) &&
           pMap->eCryptoSuite != eCCTKCSUITE_MAX )
   {
      pMap++;
   }

   return pMap->eCryptoSuite;
}

/***********************************************************************************
**  FUNCTION:   findKeyLen
**
**  PURPOSE:    Find the key length of the specified crypto suite.
**
**  PARAMETERS: pMap - Pointer to the crypto / key length mapping table.
**              eCryptoSuite - Crypto suite.
**
**  RETURNS:    Key length for the specified crypto suite.
**
**  NOTES:
***********************************************************************************/
BOS_UINT32 MseSdpSession::findKeyLen( const CCTKKEYLENMAP *pMap,
                                      const CCTKCSUITE eCryptoSuite )
{
   CCTK_ASSERT( pMap != NULL );

   while ( pMap->eCryptoSuite != eCryptoSuite &&
           pMap->eCryptoSuite != eCCTKCSUITE_MAX )
   {
      pMap++;
   }

   return pMap->uLength;
}

/***********************************************************************************
**  FUNCTION:   generateKeySalt
**
**  PURPOSE:    Generate a concatenated key and salt.
**
**  PARAMETERS: eCryptoSuite - Crypto suite.
**              uKeySaltLength - Length of key and salt.
**              pKeySalt - Pointer to the key and salt storage.
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::generateKeySalt( const CCTKCSUITE eCryptoSuite,
                                     const BOS_UINT32 uKeySaltLength,
                                     BOS_UINT8 *pKeySalt )
{
   BOS_UINT32 i;

   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( eCryptoSuite );
   CCTK_ASSERT( pKeySalt != NULL );

   srand( rand() );

   for ( i = 0; i < uKeySaltLength; i++ )
   {
      /* Generate an alpha-numeric character (i.e., ASCII 33 - 126) */
      pKeySalt[i] = static_cast<BOS_UINT8>(( rand() % ( 126 - 33 + 1 )) + 33 );
   }
}
#endif

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getCodecMap
**
**  PURPOSE:    Retrieve the codec map info for the specific codec type.
**
**  PARAMETERS:
**              eCodec - Codec type to retrieve the codec map info.
**
**
**  RETURNS:    Codec map info for eCodec.
**
**  NOTES:      .
***********************************************************************************/
const MSECODECMAP *MseSdpSession::getCodecMap( CCTKCODEC eCodec )
{
   const MSECODECMAP *pCodecMap = &mseCodecMap[0];

   /* Search for the codec mapping */
   while ( pCodecMap->eCodec != eCCTKCODEC_MAX )
   {
      if ( pCodecMap->eCodec == eCodec )
      {
         return pCodecMap;
      }
      pCodecMap++;
   }
   CCTK_ASSERT( pCodecMap->eCodec != eCCTKCODEC_MAX );

   return pCodecMap;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getCodecMap
**
**  PURPOSE:    Retrieve the codec map info for the specific codec type and rate.
**
**  PARAMETERS:
**              eMxCodec - M5T codec type to retrieve the codec map info.
**              uRate - Codec rate.
**
**
**  RETURNS:    Codec map info for eCodec.
**
**  NOTES:      .
***********************************************************************************/
const MSECODECMAP *MseSdpSession::getCodecMap(
                                    CSdpParser::ERtpCompressionAlgorithm eMxCodec,
                                    BOS_UINT32 uRate )
{
   const MSECODECMAP *pCodecMap = &mseCodecMap[0];

   /* Search for the codec mapping */
   while ( pCodecMap->eCodec != eCCTKCODEC_MAX )
   {
      if ( pCodecMap->eMxCodec == eMxCodec &&
           pCodecMap->uRate == uRate )
      {
         return pCodecMap;
      }
      pCodecMap++;
   }

   return pCodecMap;
}

#if CCTKCFG_SRTP
/***********************************************************************************
**  FUNCTION:   MseSdpSession::getCrypto
**
**  PURPOSE:    Get crypto information from M5T SDP structure.
**
**  PARAMETERS:
**              rStream - M5T stream (m=) data class.
**              pCrypto - Pointer to the storage for crypto information.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::getCrypto( CSdpLevelMedia &rStream, CCTKCRYPTOINFO *pCrypto )
{
   BOS_UINT32 i, j;
   CSdpFieldAttributeCrypto crypto;

   for ( i = 0; i < rStream.GetNbCrypto(); i++ )
   {
      /* Extract one a=crypto attribute at a time */
      crypto = rStream.GetCrypto( i );
      crypto.Validate();

      /* Get crypto suite */
      pCrypto->eCryptoSuite = findCryptoType( cctkCryptoSuite,
                                              CCTK_STR( crypto.GetCryptoSuite() ));

      /* Get key param */
      CCryptoKeyParamList *pKeyParmList;
      CCryptoKeyParam *pKeyParm;
      pKeyParmList = crypto.GetKeyParams();
      pKeyParmList->Validate();

      if ( !pKeyParmList->IsEmpty() )
      {
         /* Go through the list of keys */
         for ( j = 0; j < pKeyParmList->Length(); j++ )
         {
            pKeyParm = ( *pKeyParmList )[j];
            pKeyParm->Validate();

            /* base64 decoding */
            mxt_result res;
            MX_NS CBase64 base64;
            res = base64.Begin( CCipher::eACTION_DECRYPT, CCipher::eMODE_DEFAULT );
            CCTK_ASSERT( MX_RIS_S( res ));
            unsigned int uInSize, uOutSize;
            uInSize = strlen( pKeyParm->GetKey() );
            res = base64.Update(
                           reinterpret_cast<const uint8_t *>( pKeyParm->GetKey() ),
                           uInSize,
                           &pCrypto->cKeySalt[0],
                           &uOutSize );
            CCTK_ASSERT( MX_RIS_S( res ) && uOutSize <= ( CCTK_KEYSALT_LEN + 1 ));
            res = base64.End( &pCrypto->cKeySalt[0],
                              &uOutSize );
            CCTK_ASSERT( MX_RIS_S( res ));

            /* Lifetime */
            pCrypto->uLifeTime = pKeyParm->GetLifeTime();
         }

         pCrypto->bIsSpecified = BOS_TRUE;
      }
      else
      {
         /* No keys */
         pCrypto->bIsSpecified = BOS_FALSE;
      }

#if CCTKCFG_MAX_CRYPTO_SESPARM > 0
      BOS_UINT32 uNumSessions;
      CCryptoSessionParamList *pSesParmList;
      CCryptoSessionParam *pSesParm;

      /* Get session parms */
      pSesParmList = crypto.GetSessionParams();
      pSesParmList->Validate();

      /* Support only max session parms */
      uNumSessions = MX_MIN( pSesParmList->Length(), CCTKCFG_MAX_CRYPTO_SESPARM );
      pCrypto->uNumSessionParm = uNumSessions;

      if ( !pSesParmList->IsEmpty() )
      {
         for ( j = 0; j < uNumSessions; j++ )
         {
            pSesParm = ( *pSesParmList )[j];
            pSesParm->Validate();
            strncpy( reinterpret_cast<char *>( &pCrypto->session[j].cToken[0] ),
                     pSesParm->GetName(),
                     CCTK_SESPARM_MAX_TOKEN_LEN );
            strncpy( reinterpret_cast<char *>( &pCrypto->session[j].cVal[0] ),
                     pSesParm->GetValue(),
                     CCTK_SESPARM_MAX_VAL_LEN );
         }
      }
      else
      {
         pCrypto->uNumSessionParm = 0;
      }
#endif
   }
}
#endif

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getLoopbackMode
**
**  PURPOSE:    Retrieve the Loopback type attribute parameters from the supplied
**              stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**              rbIsSrc - Indicates if this is a Source or Mirror loopback type.
**              rLbFmt - Loopback media format list.
**
**
**  RETURNS:    BOS_TRUE if Loopback mode found, BOS_FALSE otherwise.
**
**  NOTES:      rpcLbFmt would be set to NULL if attribute is not found.
***********************************************************************************/
BOS_BOOL MseSdpSession::getLoopbackMode( CSdpLevelMedia &rStream,
                                         BOS_BOOL &rbIsSrc,
                                         CString &rLbFmt )
{
   BOS_UINT32 i;
   CSdpFieldAttributeOther other;
   BOS_BOOL bFound = BOS_FALSE;

   rLbFmt.EraseAll();

   /* Look for loopback-source and loopback-mirror */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cLbModeSrcStr, other.GetName() ) == 0 )
      {
         rbIsSrc = BOS_TRUE;
         bFound = BOS_TRUE;
      }
      else if ( strcmp( cLbModeMirrorStr, other.GetName() ) == 0 )
      {
         rbIsSrc = BOS_FALSE;
         bFound = BOS_TRUE;
      }

      if ( bFound )
      {
         rLbFmt = other.GetValue();
         break;
      }
   }

   return bFound;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getLoopbackType
**
**  PURPOSE:    Retrieve the Loopback type attribute parameters from the supplied
**              stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**
**
**  RETURNS:    Bitmapped Loopback type.
**
**  NOTES:      rpcLbFmt would be set to NULL if attribute is not found.
***********************************************************************************/
BOS_UINT32 MseSdpSession::getLoopbackType( CSdpLevelMedia &rStream )
{
   BOS_UINT32 i, uLbType = 0;
   CSdpFieldAttributeOther other;

   /* Look for loopback-source or loopback-mirror */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cLbTypeStr, other.GetName() ) == 0 )
      {
         if ( strstr( other.GetValue(), cLbTypePktLbStr ) != NULL )
         {
            uLbType |= MSELBTYPE_PKTLB;
         }
         if ( strstr( other.GetValue(), cLbTypeMediaLbStr ) != NULL )
         {
            uLbType |= MSELBTYPE_MEDIALB;
            /* Assume rtp-start-loopback mode always active for media loopback */
            uLbType |= MSELBTYPE_STARTLB;
         }
         if ( strstr( other.GetValue(), cLbTypeStartLbStr ) != NULL )
         {
            uLbType |= MSELBTYPE_STARTLB;
         }
      }
   }

   return uLbType;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getPtimeFromMx
**
**  PURPOSE:    Retrieve the ptime value from M5T stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**
**
**  RETURNS:    ptime value.
**
**  NOTES:      Returned value would be 0 if the attribute does not exist.
***********************************************************************************/
BOS_UINT32 MseSdpSession::getPtimeFromMx( CSdpLevelMedia &rStream )
{
   CSdpFieldAttributePtime &rPtime = rStream.GetPtime();
   BOS_UINT32 uPtime = rPtime.IsValid() ? rPtime.GetPacketTime() : 0;

   return uPtime;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getMaxPtimeFromMx
**
**  PURPOSE:    Retrieve the maxptime value from M5T stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**
**
**  RETURNS:    maxptime value.
**
**  NOTES:      Returned value would be 0 if the attribute does not exist.
***********************************************************************************/
BOS_UINT32 MseSdpSession::getMaxPtimeFromMx( CSdpLevelMedia &rStream )
{
   BOS_UINT32 i;
   CSdpFieldAttributeOther other;
   BOS_UINT32 uMaxPtime = 0;

   /* Look for maxptime */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cMaxPtimeStr, other.GetName() ) == 0 )
      {
         uMaxPtime = static_cast<BOS_UINT32>( atoi( other.GetName() ));
         break;
      }
   }

   return uMaxPtime;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getMptimeFromMx
**
**  PURPOSE:    Retrieve the mptime value from M5T stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**              uRtpIdx - Zero-based mptime index.
**
**
**  RETURNS:    mptime value.
**
**  NOTES:      Returned value would be 0 if the attribute does not exist.
***********************************************************************************/
BOS_UINT32 MseSdpSession::getMptimeFromMx( CSdpLevelMedia &rStream,
                                           BOS_UINT32 uRtpIdx )
{
   BOS_UINT32 i, uMptime = 0;
   BOS_BOOL bFound = BOS_FALSE;
   CSdpFieldAttributeOther other;

   /* Look for mptime */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cMptimeStr, other.GetName() ) == 0 )
      {
         bFound = BOS_TRUE;
         break;
      }
   }

   /* mptime not found */
   if ( !bFound )
   {
      return 0;
   }

   /* Allocate storage for the mptime string */
   BOS_UINT32 uMptimeSize = strlen( other.GetValue() ) + 1;
   char *pcMptime, *pcTok;
   pcMptime = BRCM_NEW_ARRAY( char, uMptimeSize );
   memcpy( pcMptime, other.GetValue(), uMptimeSize );

   /* Parse the uRtpIdx'th mptime */
   pcTok = strtok( pcMptime, " " );
   while ( pcTok && uRtpIdx > 0 )
   {
      pcTok = strtok( NULL, " " );
      uRtpIdx--;
   }

   /* Convert string to BOS_UINT32 */
   uMptime = pcTok != NULL ? atoi( pcTok ) : 0;

   /* Clean up */
   BRCM_DELETE_ARRAY( pcMptime );
   pcMptime = NULL;

   return uMptime;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getSilSuppFromMx
**
**  PURPOSE:    Retrieve the a=silenceSupp value from M5T stream structure.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**
**
**  RETURNS:    Silence suppression info.
**
**  NOTES:      .
***********************************************************************************/
CCTKSILSUPP MseSdpSession::getSilSuppFromMx( CSdpLevelMedia &rStream )
{
   BOS_UINT32 i;
   CSdpFieldAttributeOther other;
   CCTKSILSUPP eSilSupp = eCCTKSILSUPP_NOTUSED;

   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cSilSuppNameStr, other.GetName() ) == 0 )
      {
         if ( strcmp( cSilSuppOnStr, other.GetValue() ) == 0 )
         {
            eSilSupp = eCCTKSILSUPP_ON;
         }
         else if ( strcmp( cSilSuppOffStr, other.GetValue() ) == 0 )
         {
            eSilSupp = eCCTKSILSUPP_OFF;
         }
         break;
      }
   }

   return eSilSupp;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::getAmrCfgFromMx
**
**  PURPOSE:    Retrieve the amr config from M5T stream structure.
**
**  PARAMETERS:
**              pFmtpAmr - pointer to amr fmtp structure
**              pAmrCfg - Output amr configuration
**              bWideband - whether this is wideband or narrowband
**
**
**  RETURNS:    Nothing
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::getAmrCfgFromMx( CSdpFieldAttributeFmtp* pFmtp,
                                     CCTKAMRPARM* pAmrCfg,
                                     BOS_BOOL bWideband )
{
   const char *pcAmrParm;
   const char *pcTmp = NULL;

   if ( pAmrCfg == NULL || pFmtp == NULL )
   {
      return;
   }

   pcAmrParm = pFmtp->GetValue();
   if( pcAmrParm == NULL )
   {
      return;
   }

   /* If 'mode-set' attribute is present, extract its content.
    */
   if ( (pcTmp = strstr( pcAmrParm, cAmrModeSet )) != NULL )
   {
      while ( (pcTmp != NULL) && (*pcTmp != ';') && (*pcTmp != '\0') )
      {
         if ( isdigit ( *pcTmp ) )
         {
            BOS_UINT16 uMask = 1;
            char cModeValue[10];
            BOS_UINT16 uMaxMode = (bWideband)
               ? CCTK_AMR_WB_MAX_MODE
               : CCTK_AMR_NB_MAX_MODE;
            sprintf( &cModeValue[0], "%c", *pcTmp );
            if ( atoi ( cModeValue ) <= uMaxMode )
            {
               uMask <<= static_cast<BOS_UINT16> (atoi ( cModeValue ));
               pAmrCfg->uModeSet |= uMask;
            }
         }

         ++pcTmp;
      }
   }

   /* If 'octet-align' attribute is present, extract its content.
    */
   if ( (pcTmp = strstr( pcAmrParm, cAmrOctetAlign )) != NULL )
   {
      while ( (pcTmp != NULL) && (*pcTmp != ';') && (*pcTmp != '\0') )
      {
         if ( *pcTmp == '0' )
         {
            pAmrCfg->bOctetAlign = eCCTKAMR_NO;
            break;
         }
         else if ( *pcTmp == '1' )
         {
            pAmrCfg->bOctetAlign = eCCTKAMR_YES;
            break;
         }
         ++pcTmp;
      }
   }

   /* If 'mode-change-period' attribute is present, extract
    * its content.
    */
   if ( (pcTmp = strstr( pcAmrParm, cAmrModeChangePeriod )) != NULL )
   {
      while ( (pcTmp != NULL) && (*pcTmp != ';') && (*pcTmp != '\0') )
      {
         if ( *pcTmp == '1' )
         {
            pAmrCfg->bModeChangePeriod = eCCTKAMR_NO;
            break;
         }
         else if ( *pcTmp == '2' )
         {
            pAmrCfg->bModeChangePeriod = eCCTKAMR_YES;
            break;
         }
         ++pcTmp;
      }
   }

   /* If 'mode-change-neighbor' attribute is present, extract
    * its content.
    */
   if ( (pcTmp = strstr( pcAmrParm, cAmrModeChangeNeighbor )) != NULL )
   {
      while ( (pcTmp != NULL) && (*pcTmp != ';') && (*pcTmp != '\0') )
      {
         if ( *pcTmp == '0' )
         {
            pAmrCfg->bModeChangeNeighbor = eCCTKAMR_NO;
            break;
         }
         else if ( *pcTmp == '1' )
         {
            pAmrCfg->bModeChangeNeighbor = eCCTKAMR_YES;
            break;
         }
         ++pcTmp;
      }
   }

   /* If 'max-red' attribute is present, extract its content.
    */
   if ( (pcTmp = strstr( pcAmrParm, cAmrMaxRed )) != NULL )
   {
      pAmrCfg->uMaxRed = 0;
      while ( (pcTmp != NULL ) && (*pcTmp != ';') && (*pcTmp != '\0') )
      {
         if ( isdigit ( *pcTmp ) )
         {
            pAmrCfg->uMaxRed = (pAmrCfg->uMaxRed * 10) + (*pcTmp - '0');
         }
         ++pcTmp;
      }
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isMptimeAvailableMx
**
**  PURPOSE:    Check if the mptime attribute exists, if at all.
**
**  PARAMETERS:
**              rStream - M5T stream structure.
**
**
**  RETURNS:    BOS_TRUE if the attribute exists.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL MseSdpSession::isMptimeAvailableMx( CSdpLevelMedia &rStream )
{
   BOS_UINT32 i;
   CSdpFieldAttributeOther other;
   BOS_BOOL bMptime = BOS_FALSE;

   /* Search through the a= attribute list */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cMptimeStr, other.GetName() ) == 0 )
      {
         bMptime = BOS_TRUE;
         break;
      }
   }

   return bMptime;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isRtcpXrMetricsValidMx
**
**  PURPOSE:    Generate and send SDP offer based on local capability.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL MseSdpSession::isRtcpXrMetricsValidMx( CSdpLevelMedia &rStream )
{
   BOS_UINT32 i;
   CSdpFieldAttributeOther other;

   /* Look for rtcp-xr */
   for ( i = 0; i < rStream.GetNbOtherAttributes(); i++ )
   {
      other = rStream.GetOtherAttribute( static_cast<uint16_t>( i ));
      other.Validate();

      if ( strcmp( cRtcpXrStr, other.GetName() ) == 0 )
      {
         /* Compare sub-string */
         return strstr( other.GetValue(), cVoipMetricStr ) != NULL;
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isSdpPrecondApproved
**
**  PURPOSE:    Check if at least one of the streams precondition is approved.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP caps.
**
**
**  RETURNS:    BOS_TRUE if stream approved.  BOS_FALSE otherwise.
**
**  NOTES:      BOS_TRUE is returned also if Conditions is not used.
***********************************************************************************/
BOS_BOOL MseSdpSession::isSdpPrecondApproved( CSdpCapabilitiesMgr *pCapsMgr )
{
   BOS_UINT32 i, j;

   /* Copy SDP a=, b= lines from the local cap with the same media type */
   for ( i = 0; i < pCapsMgr->GetNbStreams(); i++ )
   {
      CSdpLevelMedia &rStream = pCapsMgr->GetStream( i );

      CVector<CSdpFieldAttributePreCondCurr> &rvPreCondCurr =
                                               rStream.GetCurrPreCondVector();

      if ( rvPreCondCurr.GetSize() == 0 )
      {
         /* Preconditions not used - Approved */
         return BOS_TRUE;
      }

      for ( j = 0; j < rvPreCondCurr.GetSize(); j++ )
      {
         CSdpFieldAttributePreCondCurr &rPreCondCurr = rvPreCondCurr.GetAt( j );

         if ( rPreCondCurr.GetStatusTag() == CSdpParser::ePRE_COND_STATUS_REMOTE &&
              rPreCondCurr.GetDirectionTag() == CSdpParser::ePRE_COND_DIRECTION_SENDRECV )
         {
            /* Stream approved */
            return BOS_TRUE;
         }
      }
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isSdpSupported
**
**  PURPOSE:    Check if the remote party supports the offer SDP.
**
**  PARAMETERS:
**              pAnsCapsMgr - Pointer to the answer SDP caps to check.
**              pOffCapsMgr - Pointer to the offer SDP caps to check.
**
**
**  RETURNS:    BOS_TRUE if stream supported.  BOS_FALSE otherwise.
**
**  NOTES:      Set pOffCapsMgr to NULL to bypass checks that involve the offer
**              SDP.
***********************************************************************************/
BOS_BOOL MseSdpSession::isSdpSupported( CSdpCapabilitiesMgr *pAnsCapsMgr,
                                        CSdpCapabilitiesMgr *pOffCapsMgr )
{
   BOS_BOOL bSupported = BOS_TRUE;
   unsigned int uIdx = 0;

   CCTK_ASSERT( pAnsCapsMgr != NULL );

   /* If the answer SDP contains media streams with transport port set to 0, then
   ** the stream is not supported.  For audio media stream, the stream is considered
   ** supported only if a codec other than telephone-event or comfort noice is
   ** supported.
   **
   ** There are a few exceptions:
   ** 1. An empty SDP is still considered as supported and is treated as an no-op.
   ** 2. If the offer SDP has all the streams with transport port set to 0, then the
   **    operation is treated as a mean of turning off all the media streams, and is
   **    considered as supported.
   */
   if ( !pAnsCapsMgr->GetFirstSupportedStream( uIdx ) ||
        pAnsCapsMgr->GetNbPayloadTypes( uIdx ) == 0 )
   {
      bSupported = BOS_FALSE;
   }
   else if ( pAnsCapsMgr->GetStreamType( uIdx ) == CSdpParser::eAUDIO &&
            ( pAnsCapsMgr->GetStreamTransportProtocol( uIdx ) == CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
          || pAnsCapsMgr->GetStreamTransportProtocol( uIdx ) == CSdpParser::eRTPSAVP
#endif
             ) )
   {
      BOS_UINT32 uNumAudCodec = pAnsCapsMgr->GetNbPayloadTypes( uIdx );

      for ( BOS_UINT32 i = 0; i < pAnsCapsMgr->GetNbPayloadTypes( uIdx ); i++ )
      {
         CSdpFieldAttributeRtpmap rtpmap;
         pAnsCapsMgr->GetPayloadType( uIdx, i, rtpmap );
         if ( rtpmap.GetEncoding() == CSdpParser::eTELEPHONE_EVENT ||
              rtpmap.GetEncoding() == CSdpParser::eCN )
         {
            uNumAudCodec--;
         }
      }

      if ( uNumAudCodec == 0 )
      {
         bSupported = BOS_FALSE;
      }
   }

   /* Exceptions (see description above) */
   if ( pAnsCapsMgr->GetNbStreams() == 0 ||
        ( pOffCapsMgr && !pOffCapsMgr->GetFirstSupportedStream( uIdx )))
   {
      bSupported = BOS_TRUE;
   }

   return bSupported;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isTagRequire
**
**  PURPOSE:    Returns whether the specified tag is listed as part of the Require
**              header.
**
**  PARAMETERS:
**              pHdr - Require header (list).
**              pcOptionTag - Tag to check against.
**
**
**  RETURNS:    BOS_TRUE if found in the header, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL MseSdpSession::isTagRequire( const CSipHeader *pHdr,
                                       const char* const pcOptionTag )
{
   while( pHdr != NULL )
   {
      if ( pHdr->GetRequire() == pcOptionTag )
      {
         return BOS_TRUE;
      }
      pHdr = pHdr->GetNextHeader();
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isTagSupported
**
**  PURPOSE:    Returns whether the specified tag is listed as part of the Supported
**              header.
**
**  PARAMETERS:
**              pHdr - Supported header (list).
**              pcOptionTag - Tag to check against.
**
**
**  RETURNS:    BOS_TRUE if found in the header, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/

BOS_BOOL MseSdpSession::isTagSupported( const CSipHeader *pHdr,
                                        const char* const pcOptionTag )
{
   while( pHdr != NULL )
   {
      if ( pHdr->GetSupported() == pcOptionTag )
      {
         return BOS_TRUE;
      }
      pHdr = pHdr->GetNextHeader();
   }

   return BOS_FALSE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::processAnswerSdp
**
**  PURPOSE:    The purpose of this function is to process the answer SDP before it
**              is sent to the remote peer.
**
**  PARAMETERS:
**              pAnsCapsMgr - Pointer to the answer SDP caps manager to send.
**              bUseQuadZero - Indicates whether quad-zero c= address should be
**                             used.
**              pOffCapsMgr - Pointer to the offer SDP caps manager received.
**              pLocCapsMgr - Pointer to the local SDP caps manager.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::processAnswerSdp( CSdpCapabilitiesMgr *pAnsCapsMgr,
                                      BOS_BOOL &bUseQuadZero,
                                      CSdpCapabilitiesMgr *pOffCapsMgr,
                                      CSdpCapabilitiesMgr *pLocCapsMgr )
{
   const CFGCALL *pCfgCall = CFG_CALL( muCid );
   BOS_UINT32 i, j, k, l;

   bUseQuadZero = BOS_FALSE;

   /* Configures s= Session Name/Subject */
   setSessionName( pAnsCapsMgr, pCfgCall->xtl.media.pcSesName );

   /* Copy SDP a=, b= lines from the local cap with the same media type */
   for ( i = 0; i < pAnsCapsMgr->GetNbStreams(); i++ )
   {
      /* Do nothing if stream is disabled */
      if ( pAnsCapsMgr->GetStreamPort( i ) == 0 )
      {
         continue;
      }

      CSdpLevelMedia &rAnsStream = pAnsCapsMgr->GetStream( i );
      for ( j = 0; j < pLocCapsMgr->GetNbStreams(); j++ )
      {
         CSdpLevelMedia &rLocStream = pLocCapsMgr->GetStream( j );
         if ( pAnsCapsMgr->GetStreamType( i ) == pLocCapsMgr->GetStreamType( j ) &&
              pAnsCapsMgr->GetStreamTransportProtocol( i ) ==
                 pLocCapsMgr->GetStreamTransportProtocol( j ) )
         {
            /* Copy a= attributes (except a=silenceSupp, copied below) */
            for ( k = 0; k < rLocStream.GetNbOtherAttributes(); k++ )
            {
               if ( strcmp( 
                  rLocStream.GetOtherAttribute( static_cast<uint16_t>( k )).GetName(),
                  cSilSuppNameStr ) != 0 )
               {
                  rAnsStream.AddOtherAttribute(
                     rLocStream.GetOtherAttribute( static_cast<uint16_t>( k )));
               }
            }

            /* Copy a=rtcp attributes */
            rAnsStream.SetSdpFieldAttributeRtcp( const_cast<CSdpFieldAttributeRtcp &>
               ( rLocStream.GetSdpFieldAttributeRtcp() ));

            /* Copy b= attributes */
            rAnsStream.GetBandwidth() = rLocStream.GetBandwidth();

            /* Only copy once per media type and transport match */
            break;
         }
      }
   }

   /* Handle preconditions attributes */
   for ( i = 0; i < pAnsCapsMgr->GetNbStreams(); i++ )
   {
      /* Do nothing if stream is disabled */
      if ( pAnsCapsMgr->GetStreamPort( i ) == 0 )
      {
         continue;
      }

      CSdpLevelMedia &rOffStream = pOffCapsMgr->GetStream( i );
      CSdpLevelMedia &rAnsStream = pAnsCapsMgr->GetStream( i );

      /* Copy the desired status */
      rAnsStream.GetDesPreCondVector() = rOffStream.GetDesPreCondVector();

      /* Current status */
      CVector<CSdpFieldAttributePreCondCurr> &rvOffPreCondCurr =
                                                rOffStream.GetCurrPreCondVector();
      for ( j = 0; j < rvOffPreCondCurr.GetSize(); j++ )
      {
         CVector<CSdpFieldAttributePreCondCurr> &rvAnsPreCondCurr =
                                                  rAnsStream.GetCurrPreCondVector();
         CSdpFieldAttributePreCondCurr preCondCurr = rvOffPreCondCurr.GetAt( j );

         if ( preCondCurr.GetStatusTag() == CSdpParser::ePRE_COND_STATUS_LOCAL )
         {
            /* Copy Remote status */
            preCondCurr.SetStatusTag( CSdpParser::ePRE_COND_STATUS_REMOTE );
         }
         else
         {
            /* Always approve Local status */
            preCondCurr.SetStatusTag( CSdpParser::ePRE_COND_STATUS_LOCAL );
            preCondCurr.SetDirectionTag( CSdpParser::ePRE_COND_DIRECTION_SENDRECV );
         }

         rvAnsPreCondCurr.Append( preCondCurr );
      }

      /* Include a=conf only if the offer does not include one */
      CVector<CSdpFieldAttributePreCondConf> &rvPreCondConf =
                                                rOffStream.GetConfPreCondVector();
      if ( rvPreCondConf.GetSize() == 0 && rvOffPreCondCurr.GetSize() > 0 )
      {
         CSdpFieldAttributePreCondConf preCondConf;
         preCondConf.SetPrecondType( "qos" );
         preCondConf.SetStatusTag( CSdpParser::ePRE_COND_STATUS_REMOTE );
         preCondConf.SetDirectionTag( CSdpParser::ePRE_COND_DIRECTION_SENDRECV );
         rAnsStream.GetConfPreCondVector().Append( preCondConf );
      }
   }

   /* Update answer SDP as per PC 2.0 specification */
   if ( pCfgCall->xtl.supp.hold.bUseInactive )
   {
      /* If remote is off hold and local is on hold, media mode must be set
      ** to a=inactive.
      */
      if ( isHold( meLocHoldSt ) && !isHold( meRemHoldSt ))
      {
         updateMediaMode( pAnsCapsMgr );
      }

      /* When remote SDP is using the 0.0.0.0 method, then the answer
      ** SDP must have the media address also set to 0.0.0.0.
      */
      if ( isHold( meRemHoldSt ) && !mbNoQuadZeros )
      {
         bUseQuadZero = BOS_TRUE;
      }
   }
   else
   {
      if ( isHold( meRemHoldSt ) && !mbNoQuadZeros )
      {
         bUseQuadZero = BOS_TRUE;
      }
   }

   /* Symmetric ptime - respond with the received ptime */
   if ( pCfgCall->xtl.media.neg.bSymmetricPtime )
   {
      BOS_UINT32 uNumStreams = pOffCapsMgr->GetNbStreams();
      for ( i = 0; i < uNumStreams; i++ )
      {
         CSdpLevelMedia &rOffStream = pOffCapsMgr->GetStream( i );

         BOS_UINT32 uPtime = getPtimeFromMx( rOffStream );

         /* If a=ptime exists, copy the ptime. */
         if ( uPtime > 0 )
         {
            CCTK_ASSERT( pAnsCapsMgr->GetNbStreams() > i );
            pAnsCapsMgr->SetStreamPtimeMs( i, uPtime );
         }
      }
   }

   /* RTCP-XR / voip-metrics support, only if both sides support it */
   if ( pCfgCall->xtl.media.pAudio != NULL &&
        pCfgCall->xtl.media.pAudio->qos.bRtcpXrSupport )
   {
      BOS_UINT32 uNumStreams = pOffCapsMgr->GetNbStreams();
      for ( i = 0; i < uNumStreams; i++ )
      {
         if ( isRtcpXrMetricsValidMx( pOffCapsMgr->GetStream( i )))
         {
            /* Insert a=rtcp-xr:voip-metrics */
            CSdpFieldAttributeOther rtcpXr;
            rtcpXr.SetName( cRtcpXrStr );
            rtcpXr.SetValue( cVoipMetricStr );
            pAnsCapsMgr->GetStream( i ).AddOtherAttribute( rtcpXr );
         }
      }
   }

   /* Loopback support */
   if ( pCfgCall->xtl.media.neg.bLbSupport )
   {
      BOS_UINT32 uNumStreams = pOffCapsMgr->GetNbStreams();
      for ( i = 0; i < uNumStreams; i++ )
      {
         CSdpLevelMedia &rOffStream = pOffCapsMgr->GetStream( i );
         BOS_UINT32 uLbType = getLoopbackType( rOffStream );
         BOS_BOOL bIsSrc;
         CString lbFmt;

         /* One of the loopback types are supported */
         if ( uLbType > 0 && getLoopbackMode( rOffStream, bIsSrc, lbFmt ))
         {
            /* Disable the a= direction attribute, replaced by the loopback attribute */
            pAnsCapsMgr->GetStream( i ).SetSendDirection( false );

            /* Insert Loopback type, e.g. a=loopback:rtp-media-loopback */
            CSdpFieldAttributeOther lbAttr;
            lbAttr.SetName( cLbTypeStr );

            /* Preference to support rtp-media-loopback */
            if (( uLbType & MSELBTYPE_MEDIALB ) > 0 )
            {
               lbAttr.SetValue( cLbTypeMediaLbStr );

               /* Remove all the packet loopback type (if any) as we are using media
               ** loopback.
               */
               BOS_UINT32 uNumRtps = pAnsCapsMgr->GetNbPayloadTypes( i );
               for ( j = 0; j < uNumRtps; j++ )
               {
                  CSdpFieldAttributeRtpmap rtpmap;
                  pAnsCapsMgr->GetPayloadType( i, j, rtpmap );

                  CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();

                  /* The uNumRtps > 1 check here is to make sure we do not end
                  ** up having an empty codec list if no codecs are present.
                  */
                  if ( ( eMxCodec == CSdpParser::eLBENCAPRTP ||
                         eMxCodec == CSdpParser::eLBLBRTPLOOPBACK ) &&
                       uNumRtps > 1 )
                  {
                     pAnsCapsMgr->RemovePayloadType( i, j );
                     j--;
                     uNumRtps--;
                  }
               }
            }
            else /* if (( uLbType & MSELBTYPE_PKTLB ) > 0 ) */
            {
               lbAttr.SetValue( cLbTypePktLbStr );

               /* Remove media payloads except the packet loopback types */
               /* If both packet loopback types are supported, then the
               ** first one on the list would be picked (i.e. not removed) .
               */
               BOS_BOOL bLbPlFound = BOS_FALSE;
               BOS_UINT32 uNumRtps = pAnsCapsMgr->GetNbPayloadTypes( i );
               for ( j = 0; j < uNumRtps; j++ )
               {
                  CSdpFieldAttributeRtpmap rtpmap;
                  pAnsCapsMgr->GetPayloadType( i, j, rtpmap );

                  CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();

                  /* The uNumRtps > 1 check here is to make sure we do not end up
                  ** having an empty codec list if the packet loopback attributes
                  ** are not present (which would be a bug).
                  */
                  if ( ( bLbPlFound ||
                         ( eMxCodec != CSdpParser::eLBENCAPRTP &&
                           eMxCodec != CSdpParser::eLBLBRTPLOOPBACK ) ) &&
                        uNumRtps > 1 )
                  {
                     pAnsCapsMgr->RemovePayloadType( i, j );
                     j--;
                     uNumRtps--;
                  }
                  else
                  {
                     bLbPlFound = BOS_TRUE;
                  }
               }

               /* Special case when no loopback attributes are found, we will reject
               ** the offer by setting the media port to 0.
               */
               if ( !bLbPlFound )
               {
                  pAnsCapsMgr->SetStreamPort( i, 0 );
               }
            }
            pAnsCapsMgr->GetStream( i ).AddOtherAttribute( lbAttr );

            /* Insert Loopback mode, e.g. a=loopback-source:0 8 */
            lbAttr.Reset();
            if ( bIsSrc )
            {
               lbAttr.SetName( cLbModeMirrorStr );
            }
            else
            {
               lbAttr.SetName( cLbModeSrcStr );
            }
            lbAttr.SetValue( lbFmt.CStr() );
            pAnsCapsMgr->GetStream( i ).AddOtherAttribute( lbAttr );

            if (( uLbType & MSELBTYPE_STARTLB ) > 0 )
            {
               lbAttr.Reset();
               lbAttr.SetName( cLbTypeStr );
               lbAttr.SetValue( cLbTypeStartLbStr );
               pAnsCapsMgr->GetStream( i ).AddOtherAttribute( lbAttr );
            }
         }
      }
   }

   /* a=silenceSupp/a=fmtp negotiation */
   /* Iterate through the offer stream list */
   BOS_UINT32 uNumStreams = pOffCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      CSdpLevelMedia &rOffStream = pOffCapsMgr->GetStream( i );

      if ( pOffCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
           ( pOffCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
             || pOffCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#endif
           ))
      {
         if ( pCfgCall->xtl.media.pAudio != NULL )
         {
            CCTKAUDGEN *pAudioGenCap = &pCfgCall->xtl.media.pAudio->gen;

            BOS_UINT32 uNumRtps = pAnsCapsMgr->GetNbPayloadTypes( i );
            for ( j = 0; j < uNumRtps; j++ )
            {
               CSdpFieldAttributeRtpmap rtpmap;
               pAnsCapsMgr->GetPayloadType( i, j, rtpmap );

               CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
               BOS_UINT8 uRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

               if ( eMxCodec == CSdpParser::eILBC )
               {
                  /* Fill in the iLBC fmtp if used */
                  for ( k = 0; k < pAudioGenCap->uNumCodec; k++ )
                  {
                     if ( pAudioGenCap->codec[k].eType == eCCTKCODEC_ILBC )
                     {
                        CCTKILBCMODE eLocIlbcMode =
                           pAudioGenCap->codec[k].parm.eIlbcMode;

                        if ( eLocIlbcMode != eCCTKILBCMODE_NOTUSED )
                        {
                           CSdpFieldAttributeFmtp fmtpIlbc;

                           fmtpIlbc.SetMediaFormat( uRtpCode );

                           switch ( eLocIlbcMode )
                           {
                              case eCCTKILBCMODE_20MS:
                                 fmtpIlbc.SetValue( cIlbcMode20msStr );
                                 break;

                              case eCCTKILBCMODE_30MS:
                              default:
                                 fmtpIlbc.SetValue( cIlbcMode30msStr );
                                 break;
                           }

                           pAnsCapsMgr->GetStream( i ).AddFmtp( fmtpIlbc );
                        }

                        break;
                     }
                  }
               }
               else if ( eMxCodec == CSdpParser::eG723 )
               {
                  BOS_BOOL bOffVad = static_cast<BOS_BOOL>(
                     pOffCapsMgr->IsVadSupportedInStream(
                        CSdpParser::ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                        uRtpCode,
                        rOffStream ));

                  /* Fill in the G.723.1 Annex A fmtp if used */
                  for ( k = 0; k < pAudioGenCap->uNumCodec; k++ )
                  {
                     if ( pAudioGenCap->codec[k].eType == eCCTKCODEC_G7231 )
                     {
                        CCTKG7231VAD eAnsG7231Vad =
                           filterG7231Vad( bOffVad,
                                           pAudioGenCap->codec[k].parm.eG7231Vad );

                        if ( eAnsG7231Vad != eCCTKG7231VAD_NOTUSED )
                        {
                           pAnsCapsMgr->AddVadFmtp(
                              CSdpParser::
                                 ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                              uRtpCode,
                              eAnsG7231Vad == eCCTKG7231VAD_YES,
                              pAnsCapsMgr->GetStream( i )
                           );
                        }
                        break;
                     }
                  }
               }
               else if ( eMxCodec == CSdpParser::eG729E ||
                         eMxCodec == CSdpParser::eG729 )
               {
                  BOS_BOOL bOffVad = static_cast<BOS_BOOL>(
                     pOffCapsMgr->IsVadSupportedInStream(
                        CSdpParser::ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                        uRtpCode,
                        rOffStream ));

                  /* Fill in the G.729 Annex B fmtp if used */
                  for ( k = 0; k < pAudioGenCap->uNumCodec; k++ )
                  {
                     if ( pAudioGenCap->codec[k].eType == eCCTKCODEC_G729E ||
                          pAudioGenCap->codec[k].eType == eCCTKCODEC_G729 )
                     {
                        CCTKG729VAD eAnsG729Vad =
                           filterG729Vad( bOffVad,
                                          pAudioGenCap->codec[k].parm.eG729Vad );

                        if ( eAnsG729Vad != eCCTKG729VAD_NOTUSED )
                        {
                           pAnsCapsMgr->AddVadFmtp(
                              CSdpParser::
                                 ms_apszRtpCompressionAlgorithmMap[eMxCodec],
                              uRtpCode,
                              eAnsG729Vad == eCCTKG729VAD_YES,
                              pAnsCapsMgr->GetStream( i )
                           );
                        }
                        break;
                     }
                  }
               }
               else if ( eMxCodec == CSdpParser::eAMR_WB ||
                         eMxCodec == CSdpParser::eAMR )
               {
                  CSdpFieldAttributeFmtp *pFmtpAmr =
                     rOffStream.GetFmtpFromPayloadType( uRtpCode );
                  CCTKAMRPARM outAmrCfg;
                  CCTKAMRPARM inAmrCfg;
                  memset(&inAmrCfg, 0, sizeof(inAmrCfg));
                  memset(&outAmrCfg, 0, sizeof(outAmrCfg));

                  /* Parse the offer fmtp and reconstruct a response */
                  getAmrCfgFromMx( pFmtpAmr, &inAmrCfg, (eMxCodec == CSdpParser::eAMR_WB) );

                  /* Fill in the AMR fmtp if used */
                  for ( k = 0; k < pAudioGenCap->uNumCodec; k++ )
                  {
                     if ( ( eMxCodec == CSdpParser::eAMR_WB &&
                              pAudioGenCap->codec[k].eType == eCCTKCODEC_AMRWB ) ||
                          ( eMxCodec == CSdpParser::eAMR &&
                              pAudioGenCap->codec[k].eType == eCCTKCODEC_GSMAMR ) )
                     {
                        CSdpFieldAttributeFmtp fmtpAmr;
                        BOS_UINT32 uPtime;

                        filterAmr( &inAmrCfg, &outAmrCfg, &(pAudioGenCap->codec[k].parm.amrCfg) );

                        constructAmrFmtp( fmtpAmr, &outAmrCfg, (eMxCodec == CSdpParser::eAMR_WB) );

                        fmtpAmr.SetMediaFormat( uRtpCode );
                        pAnsCapsMgr->GetStream( i ).AddFmtp( fmtpAmr );

                        /* If a=ptime exists, copy the ptime. */
                        uPtime = getPtimeFromMx( rOffStream );
                        if ( uPtime > 0 )
                        {
                           CCTK_ASSERT( pAnsCapsMgr->GetNbStreams() > i );
                           pAnsCapsMgr->SetStreamPtimeMs( i, uPtime );
                        }
                     }
                     break;
                  }
               }
            }

            CCTKSILSUPP eAnsSilSupp = 
               filterSilSuppSdp( getSilSuppFromMx( rOffStream ),
                                 pAudioGenCap->eSilSupp );
            setSilSupp( pAnsCapsMgr->GetStream( i ), eAnsSilSupp );
         }
      }
#if CCTKCFG_VIDEO
      else if ( pOffCapsMgr->GetStreamType( i ) == CSdpParser::eVIDEO &&
                ( pOffCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP
#   if CCTKCFG_SRTP
                 || pOffCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#   endif
           ))
      {
         if ( pCfgCall->xtl.media.pVideo != NULL )
         {
            CCTKVIDGEN *pVidGenCap = &pCfgCall->xtl.media.pVideo->gen;
            BOS_UINT32 uNumRtps = pAnsCapsMgr->GetNbPayloadTypes( i );

            for ( j = 0; j < uNumRtps; j++ )
            {
               CSdpFieldAttributeRtpmap rtpmap;
               pAnsCapsMgr->GetPayloadType( i, j, rtpmap );

               CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
               BOS_UINT8 uRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

               if ( eMxCodec == CSdpParser::eH264 )
               {
                  for ( k = 0; k < pVidGenCap->uNumCodec; k++ )
                  {
                     /* Negotiate H.264 fmtp if used.
                     **
                     **  - If the offer SDP contains H.264 fmtp, locate that one and
                     **    negotiate in consequence by either accepting, downgrading
                     **    or rejecting the stream all together.
                     **  - If the offer SDP does not contain H.264 fmtp, assign our
                     **    own.
                     */
                     if ( (pVidGenCap->codec[k].eType == eCCTKCODEC_H264) &&
                          (pVidGenCap->codec[k].parm.pH264 != NULL) &&
                          (pVidGenCap->codec[k].parm.pH264->bIsSpecified) )
                     {
                        CCTKH264PARM *pH264Parm = pVidGenCap->codec[k].parm.pH264;
                        BOS_BOOL bReject = BOS_FALSE;
                        CCTKH264PARM offerH264Parm;
                        CCTKH264PARM negotiatedH264Parm;
                        BOS_UINT32 uNumOfferRtps = pOffCapsMgr->GetNbPayloadTypes( i );
            
                        memset ( &offerH264Parm, 0, sizeof ( CCTKH264PARM ) );
                        /* Initially, the negotiated capabilities will be the same as
                        ** the local one we support.  This will be updated later on in
                        ** the process once we located the proper offer capabilities.
                        */
                        memcpy ( &negotiatedH264Parm, pH264Parm, sizeof ( CCTKH264PARM ) );

                        /* Locate the corresponding H.264 codec in the offer capabilities.
                        */
                        for ( l = 0; l < uNumOfferRtps; l++ )
                        {
                           CSdpFieldAttributeRtpmap rtpmap;
                           pOffCapsMgr->GetPayloadType( i, l, rtpmap );

                           CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
                           BOS_UINT8 uOfferRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

                           if ( (eMxCodec == CSdpParser::eH264) &&
                                (uOfferRtpCode == uRtpCode) )
                           {
                              CSdpFmtpH264 *pFmtpH264 = pOffCapsMgr->GetStream( i ).GetFmtpH264();

                              if ( pFmtpH264 != NULL )
                              {
                                 offerH264Parm.bIsSpecified = BOS_TRUE;
                                 offerH264Parm.bConstraintSet0 = pFmtpH264->GetConstraintSet0();
                                 offerH264Parm.bConstraintSet1 = pFmtpH264->GetConstraintSet1();
                                 offerH264Parm.bConstraintSet2 = pFmtpH264->GetConstraintSet2();
                                 offerH264Parm.uMaxBr = pFmtpH264->GetMaxBr();
                                 offerH264Parm.uMaxMbps = pFmtpH264->GetMaxMbps();
                                 offerH264Parm.ePacketizationMode =
                                    static_cast<CCTKH264PACKETIZATIONMODE>
                                       ( pFmtpH264->GetPacketizationMode() );
                                 offerH264Parm.bParameterAdd = pFmtpH264->GetParameterAdd();
                                 pFmtpH264->GetParameterSetsHex(
                                    reinterpret_cast<char *>( &offerH264Parm.cPropParamSets[0] ),
                                    CCTKSDP_MAX_H264_PARM_SETS_LEN );

                                 switch ( pFmtpH264->GetProfile() )
                                 {
#define CASE_H264_PROFILE( sipProf, ccProf ) \
                                    case (sipProf): { offerH264Parm.eProfile = (ccProf); } break;
                                                CASE_H264_PROFILE( CSdpFmtpH264::eBASELINE,
                                                                   eCCTKH264PROFILE_BASELINE );
                                                CASE_H264_PROFILE( CSdpFmtpH264::eMAIN,
                                                                   eCCTKH264PROFILE_MAIN );
                                                CASE_H264_PROFILE( CSdpFmtpH264::eEXTENDED,
                                                                   eCCTKH264PROFILE_EXTENDED );
#undef CASE_H264_PROFILE
                                 }

                                 switch ( pFmtpH264->GetLevel() )
                                 {
#define CASE_H264_LEVEL( sipLevel, ccLevel ) \
                                    case (sipLevel): { offerH264Parm.eLevel = (ccLevel); } break;
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_0,
                                                                       eCCTKH264LEVEL_1_0 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_1,
                                                                       eCCTKH264LEVEL_1_1 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_2,
                                                                       eCCTKH264LEVEL_1_2 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_3,
                                                                       eCCTKH264LEVEL_1_3 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_0,
                                                                       eCCTKH264LEVEL_2_0 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_1,
                                                                       eCCTKH264LEVEL_2_1 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_2,
                                                                       eCCTKH264LEVEL_2_2 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_0,
                                                                       eCCTKH264LEVEL_3_0 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_1,
                                                                       eCCTKH264LEVEL_3_1 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_2,
                                                                       eCCTKH264LEVEL_3_2 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_0,
                                                                       eCCTKH264LEVEL_4_0 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_1,
                                                                       eCCTKH264LEVEL_4_1 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_2,
                                                                       eCCTKH264LEVEL_4_2 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_0,
                                                                       eCCTKH264LEVEL_5_0 );
                                                CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_1,
                                                                       eCCTKH264LEVEL_5_1 );
#undef CASE_H264_LEVEL
                                 }
                              }
                           }
                        }

                        if ( offerH264Parm.bIsSpecified )
                        {
                           /* Cannot accept a packetization mode that we do not
                           ** support.
                           */
                           if ( offerH264Parm.ePacketizationMode !=
                                                   pH264Parm->ePacketizationMode )
                           {
                              bReject = BOS_TRUE;
                           }

                           /* Cannot accept a profile that we do not support.
                           */
                           if ( offerH264Parm.eProfile !=
                                                pH264Parm->eProfile )
                           {
                              bReject = BOS_TRUE;
                           }

                           /* Downgrade the supported capabilities level if necessary.
                           */
                           if ( offerH264Parm.eLevel != pH264Parm->eLevel )
                           {
                              negotiatedH264Parm.eLevel =
                                 (offerH264Parm.eLevel < pH264Parm->eLevel) ?
                                          offerH264Parm.eLevel : pH264Parm->eLevel;
                           }

                           if ( offerH264Parm.uMaxBr != pH264Parm->uMaxBr )
                           {
                              negotiatedH264Parm.uMaxBr =
                                 (offerH264Parm.uMaxBr < pH264Parm->uMaxBr) ?
                                    offerH264Parm.uMaxBr : pH264Parm->uMaxBr;
                           }

                           if ( offerH264Parm.uMaxMbps != pH264Parm->uMaxMbps )
                           {
                              negotiatedH264Parm.uMaxMbps =
                                 (offerH264Parm.uMaxMbps < pH264Parm->uMaxMbps) ?
                                          offerH264Parm.uMaxMbps : pH264Parm->uMaxMbps;
                           }
                        }

                        /* We cannot accept the video stream, reject it.
                        */
                        if ( bReject )
                        {
                           pAnsCapsMgr->SetStreamPort( i, 0 );
                        }
                        /* Include the (possibly adjusted) fmtp information for the
                        ** H.264 media.
                        */
                        else
                        {
                           CSdpFmtpH264 fmtpH264;
                           CSdpFmtpH264::EProfile eSipProf;
                           CSdpFmtpH264::ELevel eSipLevel;

                           switch ( negotiatedH264Parm.eProfile )
                           {
#define CASE_H264_PROFILE( _sipProf, _ccProf  ) \
                              case (_ccProf): { eSipProf = (_sipProf); } break;
                                  CASE_H264_PROFILE( CSdpFmtpH264::eBASELINE,
                                                         eCCTKH264PROFILE_BASELINE );
                                  CASE_H264_PROFILE( CSdpFmtpH264::eMAIN,
                                                         eCCTKH264PROFILE_MAIN );
                                  CASE_H264_PROFILE( CSdpFmtpH264::eEXTENDED,
                                                         eCCTKH264PROFILE_EXTENDED );
#undef CASE_H264_PROFILE
                               default:
                                 eSipProf = CSdpFmtpH264::eBASELINE;
                                 CCTK_ASSERT( eSipProf != CSdpFmtpH264::eBASELINE );
                           }


                           switch ( negotiatedH264Parm.eLevel )
                           {
#define CASE_H264_LEVEL( _sipLevel, _ccLevel  ) \
                              case (_ccLevel): { eSipLevel = (_sipLevel); } break;
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_0, eCCTKH264LEVEL_1_0 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_1, eCCTKH264LEVEL_1_1 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_2, eCCTKH264LEVEL_1_2 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_1_3, eCCTKH264LEVEL_1_3 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_0, eCCTKH264LEVEL_2_0 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_1, eCCTKH264LEVEL_2_1 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_2_2, eCCTKH264LEVEL_2_2 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_0, eCCTKH264LEVEL_3_0 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_1, eCCTKH264LEVEL_3_1 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_3_2, eCCTKH264LEVEL_3_2 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_0, eCCTKH264LEVEL_4_0 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_1, eCCTKH264LEVEL_4_1 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_4_2, eCCTKH264LEVEL_4_2 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_0, eCCTKH264LEVEL_5_0 );
                                 CASE_H264_LEVEL( CSdpFmtpH264::eLEVEL_5_1, eCCTKH264LEVEL_5_1 );
#undef CASE_H264_LEVEL
                               default:
                                 eSipLevel = CSdpFmtpH264::eLEVEL_1_0;
                                 CCTK_ASSERT( eSipLevel != CSdpFmtpH264::eLEVEL_1_0 );
                           }

                           fmtpH264.SetMediaFormat( uRtpCode );
                           fmtpH264.SetProfile( eSipProf );
                           fmtpH264.SetConstraintSet0(
                              static_cast<char>( negotiatedH264Parm.bConstraintSet0 ));
                           fmtpH264.SetConstraintSet1(
                              static_cast<char>( negotiatedH264Parm.bConstraintSet1 ));
                           fmtpH264.SetConstraintSet2(
                              static_cast<char>( negotiatedH264Parm.bConstraintSet2 ));
                           fmtpH264.SetLevel( eSipLevel );
                           fmtpH264.SetPacketizationMode(
                              static_cast<CSdpFmtpH264::EPacketizationMode>
                              ( negotiatedH264Parm.ePacketizationMode ));
                           fmtpH264.SetParameterAdd( negotiatedH264Parm.bParameterAdd == BOS_TRUE );
                           fmtpH264.SetParameterSetsHex(
                              reinterpret_cast<char *>( &negotiatedH264Parm.cPropParamSets[0] ));
                           fmtpH264.SetMaxBr( negotiatedH264Parm.uMaxBr );
                           fmtpH264.SetMaxMbps( negotiatedH264Parm.uMaxMbps );

                           fmtpH264.GetValue();
                           fmtpH264.Validate();

                           pAnsCapsMgr->GetStream( i ).AddFmtp( fmtpH264 );
                        }

                        break;
                     }
                  }
               }
               else if ( eMxCodec == CSdpParser::eH263 )
               {
                  for ( k = 0; k < pVidGenCap->uNumCodec; k++ )
                  {
                     /* Negotiate H.263 fmtp if used.
                     **
                     **  - If the offer SDP contains H.263 fmtp, locate that one and
                     **    negotiate in consequence by either accepting, downgrading
                     **    or rejecting the stream all together.
                     **  - If the offer SDP does not contain H.263 fmtp, assign our
                     **    own.
                     */
                     if ( (pVidGenCap->codec[k].eType == eCCTKCODEC_H263) &&
                          (pVidGenCap->codec[k].parm.pH263 != NULL) &&
                          (pVidGenCap->codec[k].parm.pH263->bIsSpecified) )
                     {
                        CCTKH263PARM *pH263Parm = pVidGenCap->codec[k].parm.pH263;
                        BOS_BOOL bReject = BOS_FALSE;
                        CCTKH263PARM offerH263Parm;
                        CCTKH263PARM negotiatedH263Parm;
                        BOS_UINT32 uNumOfferRtps = pOffCapsMgr->GetNbPayloadTypes( i );
            
                        memset ( &offerH263Parm, 0, sizeof ( CCTKH263PARM ) );
                        /* Initially, the negotiated capabilities will be the same as
                        ** the local one we support.  This will be updated later on in
                        ** the process once we located the proper offer capabilities.
                        */
                        memcpy ( &negotiatedH263Parm, pH263Parm, sizeof ( CCTKH263PARM ) );

                        /* Locate the corresponding H.263 codec in the offer capabilities.
                        */
                        for ( l = 0; l < uNumOfferRtps; l++ )
                        {
                           CSdpFieldAttributeRtpmap rtpmap;
                           pOffCapsMgr->GetPayloadType( i, l, rtpmap );

                           CSdpParser::ERtpCompressionAlgorithm eMxCodec = rtpmap.GetEncoding();
                           BOS_UINT8 uOfferRtpCode = static_cast<BOS_UINT8>( rtpmap.GetPayloadType() );

                           if ( (eMxCodec == CSdpParser::eH263) &&
                                (uOfferRtpCode == uRtpCode) )
                           {
                              CSdpFmtpH263 *pFmtpH263 = pOffCapsMgr->GetStream( i ).GetFmtpH263();

                              if ( pFmtpH263 != NULL )
                              {
                                 offerH263Parm.bIsSpecified = BOS_TRUE;
                                 offerH263Parm.uLevel =
                                    static_cast<BOS_UINT32> (pFmtpH263->GetLevel());
                                 offerH263Parm.uProfile =
                                    static_cast<BOS_UINT32> (pFmtpH263->GetProfile());
                              }
                           }
                        }

                        if ( offerH263Parm.bIsSpecified )
                        {
                           /* Cannot accept a profile that we do not support.
                           */
                           if ( offerH263Parm.uProfile !=
                                                pH263Parm->uProfile )
                           {
                              bReject = BOS_TRUE;
                           }

                           /* Downgrade the supported capabilities level if necessary.
                           */
                           if ( offerH263Parm.uLevel != pH263Parm->uLevel )
                           {
                              negotiatedH263Parm.uLevel =
                                 (offerH263Parm.uLevel < pH263Parm->uLevel) ?
                                          offerH263Parm.uLevel : pH263Parm->uLevel;
                           }
                        }

                        /* We cannot accept the video stream, reject it.
                        */
                        if ( bReject )
                        {
                           pAnsCapsMgr->SetStreamPort( i, 0 );
                        }
                        /* Include the (possibly adjusted) fmtp information for the
                        ** H.263 media.
                        */
                        else
                        {
                           CSdpFmtpH263 fmtpH263;

                           fmtpH263.SetMediaFormat( uRtpCode );
                           fmtpH263.SetProfile( negotiatedH263Parm.uProfile );
                           fmtpH263.SetLevel( negotiatedH263Parm.uLevel );

                           fmtpH263.GetValue();
                           fmtpH263.Validate();

                           pAnsCapsMgr->GetStream( i ).AddFmtp( fmtpH263 );
                        }

                        break;
                     }
                  }
               }
            }
         }
      }
#endif /* CCTKCFG_VIDEO */
   }

   /* Insert b=TIAS, b=AS and a=maxprate based on the codecs and ptime */
   addBandwidthAttrib( pAnsCapsMgr );

   /* Disable RTCP if Offer SDP has disabled RTCP */
   for ( i = 0; i < pAnsCapsMgr->GetNbStreams(); i++ )
   {
      CSdpLevelMedia &rOffStream = pOffCapsMgr->GetStream( i );
      if ( rOffStream.IsRtcpDeactivated(NULL, NULL) )
      {
         /* RTCP is disabled for the Offer stream, disable RTCP for our Answer stream. */
         CSdpLevelMedia &rAnsStream = pAnsCapsMgr->GetStream( i );            
         addBandwidthAttrib( rAnsStream, 0, 0 );
      }
   }
   
   /* T.38 post-processing */
   for ( i = 0; i < pAnsCapsMgr->GetNbStreams(); i++ )
   {
      if ( pAnsCapsMgr->GetStreamType( i ) == CSdpParser::eIMAGE &&
           ( pAnsCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eUDPTL ||
             pAnsCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eudptl ))
      {
         CSdpLevelMedia &rAnsStream = pAnsCapsMgr->GetStream( i );

         /* Insert a=T38FaxVersion if missing in Offer SDP */
         if ( !rAnsStream.GetVersion().IsValid() )
         {
            rAnsStream.GetVersion().SetVersion( 0 );
         }
      }
   }   
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::sendSdpOffer
**
**  PURPOSE:    Generate and send SDP offer based on local capability.
**
**  PARAMETERS:
**              None.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::sendSdpOffer()
{
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

   /* Allocate storage for the SDP */
   CSdpCapabilitiesMgr *pCapsMgr = NULL;
   CBlob *pSdpBlob = NULL;

#if CCTKCFG_REMOTE_USER_CONTROL
   if ( mpOffSdpBlob != NULL )
   {
      /* There is a received Offer SDP, from an remote user */
      pSdpBlob = BRCM_NEW( CBlob )( *mpOffSdpBlob );
      /* NAT ALG, update the SDP if necessary (for LAN remote user) */
      natUpdateProxyUsrSdp ( *pSdpBlob );
      /* remove the NULL at the end of the SDP if nessary */
      if ( pSdpBlob->GetAt( pSdpBlob->GetLastIndex() ) == 0 )
      {
         pSdpBlob->Erase( pSdpBlob->GetLastIndex() );
      }
   }
   else
#endif
   {
      pCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr );
      createLocalSdp( pCapsMgr, BOS_TRUE, BOS_FALSE );

#if CCTKCFG_ICE_LITE_SUPPORT
      ICENegotiation ( mpRemCapsMgr, pCapsMgr, BOS_FALSE );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */      
      
      /* Prepare SDP based on the configured media profile */
      pSdpBlob = createSdpBlob( pCapsMgr );
   }

   IMspOfferAnswerSessionMgr::EMessageTypeForPayload eMsgType;

   if ( mbNotifyUser && pCfgCall->xtl.media.neg.bSdpAnswerIn180 )
   {
      eMsgType = IMspOfferAnswerSessionMgr::eINVITE_RINGING;
      mbNotifyUser = BOS_FALSE;
   }
   else
   {
      eMsgType = IMspOfferAnswerSessionMgr::eAUTOMATIC;
   }

   /* Send out the SDP offer */
   mxt_result result = mpMgr->EvNewSdpToSend( pSdpBlob,
                                              mpEarlySdpBlob,
                                              eMsgType,
                                              0 );
   if ( MX_RIS_F( result ))
   {
      CCTK_TRACEI1( "mediaUpdate - EvNewSdpToSend() returns error %d", result );
   }
   else if ( eMsgType == IMspOfferAnswerSessionMgr::eINVITE_RINGING )
   {
      /* Notify CE for the ringback */
      ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
   }
   pSdpBlob = NULL;
   mpEarlySdpBlob = NULL;

   /* Store the local SDP */
   if ( mpLocCapsMgr != NULL )
   {
      BRCM_DELETE( mpLocCapsMgr );
   }
   mpLocCapsMgr = pCapsMgr;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setMediaMode
**
**  PURPOSE:    Set media mode based on current hold state.
**
**  PARAMETERS:
**              rStream - Reference to the m= line class.
**
**
**  RETURNS:    rStream with the audio mode set.
**
**  NOTES:      Media mode (a= attribute) would not be used unless the RFC 3264
**              hold method is used or previously detected.
***********************************************************************************/
void MseSdpSession::setMediaMode( CSdpLevelMedia &rStream )
{
   const CFGCALL *pCfgCall = CFG_CALL( muCid );

#if CCTKCFG_IPV6
   /* Update hold settings
   ** Always use the RFC 3261 hold method for IPv6. */
   if ( bosIpAddrIsV6( &pCfgCall->xtl.media.ipaddr ))
   {
      mbNoQuadZeros = BOS_TRUE;
   }
#endif

   BOS_BOOL bUseInactive = pCfgCall->xtl.supp.hold.bUseInactive;

   if ( mbNoQuadZeros )
   {
      CSdpParser::EAttributeType eMode;

      if ( isHold( meLocHoldSt ))
      {
         /* Use a=inactive regardless of remote hold state as per PC 2.0 */
         if ( bUseInactive || isHold( meRemHoldSt ))
         {
            eMode = CSdpLevelMedia::eINACTIVE;
         }
         else
         {
            eMode = CSdpLevelMedia::eSENDONLY;
         }
      }
      else
      {
         /* Use a=sendrecv regardless of remote hold state as per PC 2.0 */
         if ( bUseInactive || !isHold( meRemHoldSt ))
         {
            eMode = CSdpLevelMedia::eSENDRECV;
         }
         else
         {
            eMode = CSdpLevelMedia::eRECVONLY;
         }
      }

      rStream.SetDirection( eMode );
   }
   rStream.SetSendDirection( mbNoQuadZeros == BOS_TRUE );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setSessionName
**
**  PURPOSE:    Set the SDP Session (s=) name.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP caps manager to set.
**              pcSesName - Pointer to the Session name.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      If pcSesName is set to NULL, then the session would be set to "-" by
**              default as per RFC 3264.
***********************************************************************************/
void MseSdpSession::setSessionName( CSdpCapabilitiesMgr *pCapsMgr,
                                    BOS_UINT8 *pcSesName )
{
   CSdpFieldSessionName fieldName;
   fieldName.SetSessionName(
      pcSesName != NULL ?
      reinterpret_cast<char *>( pcSesName ) : &cDftSesNameStr[0] );
   pCapsMgr->GetSdpSession()->SetSessionName( fieldName );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::setSilSupp
**
**  PURPOSE:    Set the (a=silenceSupp) parameter
**
**  PARAMETERS:
**              rStream - Pointer to the SDP stream to set.
**              eSilSupp - Silence suppression setting.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::setSilSupp( CSdpLevelMedia &rStream, CCTKSILSUPP eSilSupp )
{
   /* Note: Nothing will be added when eSilSupp is eCCTKSILSUPP_NOTUSED. */
   
   if (( eSilSupp == eCCTKSILSUPP_ON )||(eSilSupp == eCCTKSILSUPP_OFF))
   {
      CSdpFieldAttributeOther silSupp;
      silSupp.SetName( cSilSuppNameStr );
      silSupp.SetValue( eSilSupp == eCCTKSILSUPP_ON ?
         cSilSuppOnStr : cSilSuppOffStr );
      rStream.AddOtherAttribute( silSupp );
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::undoLocalCallState
**
**  PURPOSE:    Undo the current hold/unhold state and notify CE the change.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::undoLocalCallState()
{
   /* Update app with the parse failure */
   CCTKEVT eCctkEvt;
   if ( meLocHoldSt == eMSEHOLDST_HOLD_PENDING )
   {
      /* Report a local hold failure */
      eCctkEvt = eCCTKEVT_LHOLD_FAIL;

      /* Update local hold state */
      meLocHoldSt = eMSEHOLDST_NOT_HOLD;
   }
   else if ( meLocHoldSt == eMSEHOLDST_UNHOLD_PENDING )
   {
      /* Report a local unhold failure */
      eCctkEvt = eCCTKEVT_LUNHOLD_FAIL;

      /* Update local hold state */
      meLocHoldSt = eMSEHOLDST_HOLD;
   }
   else
   {
      /* Report a local media change failure */
      eCctkEvt = eCCTKEVT_MEDIA_FAIL;
   }
   ceNotify( muCid, static_cast<CEEVT>( eCctkEvt ), NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::updateMediaMode
**
**  PURPOSE:    Update media mode as per PC 2.0 specification.
**
**  PARAMETERS:
**              pCapsMgr - Media capability manager.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::updateMediaMode( CSdpCapabilitiesMgr *pCapsMgr )
{
   BOS_UINT32 i, uNumStreams = pCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      setMediaMode( pCapsMgr->GetStream( i ));
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::updateRemHoldState
**
**  PURPOSE:    Detect remote hold state change and update accordingly.
**
**  PARAMETERS:
**              pRemCapsMgr - Remote SDP caps.
**
**
**  RETURNS:    Remote hold state updated.
**
**  NOTES:
***********************************************************************************/
BOS_BOOL MseSdpSession::updateRemHoldState( CSdpCapabilitiesMgr *pRemCapsMgr )
{
   BOS_UINT32 i;

   /* Get remote audio mode, check if the incoming SDP is a hold */
   BOS_BOOL bRemHoldChange = BOS_FALSE;
   BOS_BOOL bRemHold = BOS_FALSE;

   BOS_UINT32 uNumStreams = pRemCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      /* Look at a= mode attribute in the audio stream */
      if ( pRemCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
           ( pRemCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPAVP 
#if CCTKCFG_SRTP
           || pRemCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eRTPSAVP
#endif
           ))
      {
         CSdpLevelMedia &rStream = pRemCapsMgr->GetStream( i );

         if ( rStream.IsSendOnly() || rStream.IsInactive() )
         {
            bRemHold = BOS_TRUE;

            /* RFC 3264 (non-0.0.0.0) hold method */
            mbNoQuadZeros = BOS_TRUE;

            break;
         }
      }
   }

   /* Convert IP address */
   BOS_IP_ADDRESS addr;
   convertIpFromMx( pRemCapsMgr, &addr );

   /* Classic hold method using 0.0.0.0 IPv4 address */
   if ( bosIpAddrIsZero( &addr ))
   {
      bRemHold = BOS_TRUE;
      mbNoQuadZeros = BOS_FALSE;
   }

   if ( bRemHold && !isHold( meRemHoldSt ))
   {
      bRemHoldChange = BOS_TRUE;
      meRemHoldSt = eMSEHOLDST_HOLD;
   }
   else if ( ( !bRemHold && isHold( meRemHoldSt ) ) ||
             ( !bRemHold && meRemHoldSt == eMSEHOLDST_UNHOLD_PENDING ) )
   {
      bRemHoldChange = BOS_TRUE;
      meRemHoldSt = eMSEHOLDST_NOT_HOLD;
   }

   return bRemHoldChange;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::validateRemoteSdpT38
**
**  PURPOSE:    Parses and validates the received SDP.
**
**  PARAMETERS:
**              pRemCapsMgr - Remote Caps Manager.
**
**
**  RETURNS:    SDP failure reason code.  0 if the SDP is free from syntax error or
**              any related logical checks.
**
**  NOTES:      . 
***********************************************************************************/
BOS_UINT16 MseSdpSession::validateRemoteSdpT38( CSdpCapabilitiesMgr *pRemCapsMgr )
{
   BOS_UINT32 i;

   BOS_UINT32 uNumStreams = pRemCapsMgr->GetNbStreams();
   for ( i = 0; i < uNumStreams; i++ )
   {
      /* Look at a= mode attribute in the audio stream */
      if ( pRemCapsMgr->GetStreamType( i ) == CSdpParser::eIMAGE &&
           ( pRemCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eUDPTL ||
             pRemCapsMgr->GetStreamTransportProtocol( i ) == CSdpParser::eudptl ))
      {
         CSdpLevelMedia &rStream = pRemCapsMgr->GetStream( i );

         /* When UDPTL is used, the T38FaxRateManagement attribute either MUST be
         ** present with a value of "transferredTCF" or it MUST be absent, in which
         ** case transferred TCF is assumed. All other values of the attribute MUST
         ** be rejected (error code 415 - Unsupported Media Type).
         */
         if ( rStream.GetT38FacsimileRateMgmnt().IsValid() &&
              strcmp( rStream.GetT38FacsimileRateMgmnt().GetFacsimileRateMgmnt(),
                      cT38XferTCFStr ) != 0 )
         {
            return uUNSUPPORTED_MEDIA_TYPE;
         }

         /* If the T38FaxTranscodingMMR attribute is present for UDPTL-based T.38,
         ** the command MUST be rejected (error code 415 - Unsupported Media Type).
         */
         if ( rStream.GetTranscodingMMR().IsValid() )
         {
            return uUNSUPPORTED_MEDIA_TYPE;
         }

         /* If the T38FaxTranscodingJBIG attribute is present for UDPTL-based T.38,
         ** the command MUST be rejected (error code 415 - Unsupported Media Type).
         */
         if ( rStream.GetTranscodingJBIG().IsValid() )
         {
            return uUNSUPPORTED_MEDIA_TYPE;
         }
      }
   }

   return 0;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::validateRemoteSdp
**
**  PURPOSE:    Parses and validates the received SDP.
**
**  PARAMETERS:
**              rReceivedSdp - The buffer that contains the received SDP.
**              rpRemoteSdp - Remote SDP (if exists).  NULL if not exist.
**
**
**  RETURNS:    SDP failure reason code.  0 if the SDP is free from syntax error or
**              any related logical checks.
**
**  NOTES:      rpRemoteSdp is allocated only when BOS_TRUE is returned. 
***********************************************************************************/
BOS_UINT16 MseSdpSession::validateRemoteSdp( const CBlob &rReceivedSdp,
                                             CSdpCapabilitiesMgr *&rpRemoteSdp )
{
   BOS_UINT16 uFailureCode = uNOT_ACCEPTABLE_HERE;

   CCTK_ASSERT( rpRemoteSdp == NULL );

   const char* pReceivedSdp =
      reinterpret_cast<const char*>( rReceivedSdp.GetFirstIndexPtr() );

   mxt_result result;
   CSdpPacket receivedSdp;
   receivedSdp.Parse( pReceivedSdp, result );

   if ( MX_RIS_S( result ) && receivedSdp.IsValid() )
   {
      uFailureCode = 0;
      rpRemoteSdp = BRCM_NEW( CSdpCapabilitiesMgr );
      rpRemoteSdp->CopyCapsFromPacket( receivedSdp );

#if (CCTKCFG_ICE_LITE_SUPPORT == 0)
      /*******************************************************
      ** Check if we have the IP support needed by the peer **
      *******************************************************/

      BOS_IP_ADDRESS remIpAddr;
      /* Get peer connection address */      
      convertIpFromMx( rpRemoteSdp, &remIpAddr );

      /* EDVA has no ICE-Lite support.
      ** Check if peer connection address matches with the EDVA connection
      ** address type.  If this does not match, peer is unreachable. */
      const CFGCALL *pCfgCall = CFG_CALL( muCid );
      if ( bosIpAddrGetType( &pCfgCall->xtl.media.ipaddr ) != 
            bosIpAddrGetType( &remIpAddr ) )
      {
         /* No match, reject the call */
         uFailureCode = IMspOfferAnswerSessionMgr::eINCOMPATIBE_ADDRESS_FORMAT;
      }
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
      
      if ( uFailureCode == 0 )
      {
      /* T.38 SDP validity checks */
      uFailureCode = validateRemoteSdpT38( rpRemoteSdp );
      }

      if ( uFailureCode != 0 )
      {
         BRCM_DELETE( rpRemoteSdp );
         rpRemoteSdp = NULL;
      }
   }

   return uFailureCode;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterSdp
**
**  PURPOSE:    Filter the input stream info structure with the supplied filter.
**
**  PARAMETERS:
**              pIn - Pointer to the input stream info structure.
**              pFilter - Pointer to the filter stream info structure.
**              pOut - Pointer to the output stream info structure.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::filterSdp( CCTKSTRMINFO *pIn,
                               CCTKSTRMINFO *pFilter,
                               CCTKSTRMINFO *pOut )
{
   BOS_UINT32 i, j, uNumCodec;

   CCTK_ASSERT( pIn != NULL && pFilter != NULL && pOut != NULL );
   memset( pOut, 0, sizeof( CCTKSTRMINFO ));

   /* Copy media address */
   pOut->addr = pIn->addr;

   /* Filter audio info */
#if CCTKCFG_SRTP
   if ( pIn->audio.crypto.bIsSpecified == pFilter->audio.crypto.bIsSpecified )
#endif
   {
      /* Direct copy info */
      pOut->audio.mode = pIn->audio.mode;
      pOut->audio.lb = pIn->audio.lb;
      pOut->audio.uMaxPtime = pIn->audio.uMaxPtime;

      /* Copy individual audio codec info */
      uNumCodec = 0;
      for ( i = 0; i < pIn->audio.uNumCodec; i++ )
      {
         for ( j = 0; j < pFilter->audio.uNumCodec; j++ )
         {
            if ( pIn->audio.codec[i].eType == pFilter->audio.codec[j].eType )
            {
               /* Copy audio codec info */
               pOut->audio.codec[uNumCodec] = pIn->audio.codec[i];

               /* Override silence suppression settings */
               pOut->audio.codec[uNumCodec].eSilSupp = 
                  filterSilSuppSdp( pIn->audio.codec[i].eSilSupp,
                                    pFilter->audio.codec[j].eSilSupp );

               /* Copy iLBC parameter */
               if ( pIn->audio.codec[i].eType == eCCTKCODEC_ILBC )
               {
                  pOut->audio.codec[uNumCodec].parm.eIlbcMode =
                     filterIlbcMode( pIn->audio.codec[i].parm.eIlbcMode,
                                     pFilter->audio.codec[j].parm.eIlbcMode );
               }
               else if ( pIn->audio.codec[i].eType == eCCTKCODEC_G7231 )
               {
                  pOut->audio.codec[uNumCodec].parm.eG7231Vad =
                     filterG7231Vad( pIn->audio.codec[i].parm.eG7231Vad,
                                     pFilter->audio.codec[j].parm.eG7231Vad );
               }
               else if ( pIn->audio.codec[i].eType == eCCTKCODEC_G729E ||
                         pIn->audio.codec[i].eType == eCCTKCODEC_G729 )
               {
                  pOut->audio.codec[uNumCodec].parm.eG729Vad =
                     filterG729Vad( pIn->audio.codec[i].parm.eG729Vad,
                                    pFilter->audio.codec[j].parm.eG729Vad );
               }
               else if ( pIn->audio.codec[i].eType == eCCTKCODEC_AMRWB ||
                         pIn->audio.codec[i].eType == eCCTKCODEC_GSMAMR )
               {
                  filterAmr( &(pIn->audio.codec[i].parm.amrCfg),
                             &(pOut->audio.codec[uNumCodec].parm.amrCfg),
                             &(pFilter->audio.codec[j].parm.amrCfg) );
               }

               uNumCodec++;
               break;
            }
         }
      }
      pOut->audio.uNumCodec = uNumCodec;

      /* Copy telephone-event info */
      if ( pIn->audio.bTelEvtUsed && pFilter->audio.bTelEvtUsed )
      {
         pOut->audio.televt = pIn->audio.televt;
         pOut->audio.bTelEvtUsed = BOS_TRUE;
      }
      else
      {
         pOut->audio.bTelEvtUsed = BOS_FALSE;
      }

      /* Filter Comfort Noise info */
      if ( pIn->audio.bCnUsed && pFilter->audio.bCnUsed )
      {
         pOut->audio.cn = pIn->audio.cn;
         pOut->audio.bCnUsed = BOS_TRUE;
      }
      else
      {
         pOut->audio.bCnUsed = BOS_FALSE;
      }

#if CCTKCFG_SRTP
      /* Copy SRTP info */
      pOut->audio.crypto = pIn->audio.crypto;
#endif
   }
#if CCTKCFG_SRTP
   else
   {
      /* Crypto info mismatch, disable media */
      pOut->audio.mode = eCCTKMEDMODE_INACTIVE;
      pOut->audio.uNumCodec = 0;
      pOut->audio.bTelEvtUsed = BOS_FALSE;
      pOut->audio.bCnUsed = BOS_FALSE;
   }
#endif

#if CCTKCFG_VIDEO
   /* Filter video info */
#if CCTKCFG_SRTP
   if ( pIn->video.crypto.bIsSpecified == pFilter->video.crypto.bIsSpecified )
#endif
   {
      /* Direct copy info */
      pOut->video.mode = pIn->video.mode;

      /* Copy individual video codec info */
      uNumCodec = 0;
      for ( i = 0; i < pIn->video.uNumCodec; i++ )
      {
         for ( j = 0; j < pFilter->video.uNumCodec; j++ )
         {
            if ( pIn->video.codec[i].eType == pFilter->video.codec[j].eType )
            {
               pOut->video.codec[uNumCodec] = pIn->video.codec[i];

               /* For H.264, if parameters are set from both remote and local
               ** sides, filter them out to use the lowest common denominator
               ** possible.
               **
               ** At this time in the process, we would have ignored/rejected
               ** non compatible settings. 
               */
               if ( (pOut->video.codec[uNumCodec].eType == eCCTKCODEC_H264) &&
                    pOut->video.codec[uNumCodec].parm.h264.bIsSpecified &&
                    pFilter->video.codec[j].parm.h264.bIsSpecified )
               {
                  pOut->video.codec[uNumCodec].parm.h264.eLevel =
                     (pOut->video.codec[uNumCodec].parm.h264.eLevel <
                                    pFilter->video.codec[j].parm.h264.eLevel) ?
                              pOut->video.codec[uNumCodec].parm.h264.eLevel :
                              pFilter->video.codec[j].parm.h264.eLevel;

                  pOut->video.codec[uNumCodec].parm.h264.uMaxBr =
                     (pOut->video.codec[uNumCodec].parm.h264.uMaxBr <
                                    pFilter->video.codec[j].parm.h264.uMaxBr) ?
                              pOut->video.codec[uNumCodec].parm.h264.uMaxBr :
                              pFilter->video.codec[j].parm.h264.uMaxBr;

                  pOut->video.codec[uNumCodec].parm.h264.uMaxMbps =
                     (pOut->video.codec[uNumCodec].parm.h264.uMaxMbps <
                                    pFilter->video.codec[j].parm.h264.uMaxMbps) ?
                              pOut->video.codec[uNumCodec].parm.h264.uMaxMbps :
                              pFilter->video.codec[j].parm.h264.uMaxMbps;
               }
               /* 
               ** For H.263, if parameters are set from both remote and local
               ** sides, filter them out to use the lowest common denominator
               ** possible.
               **
               ** At this time in the process, we would have ignored/rejected
               ** non compatible settings.
               */ 
               if ( (pOut->video.codec[uNumCodec].eType == eCCTKCODEC_H263) &&
                    pOut->video.codec[uNumCodec].parm.h263.bIsSpecified &&
                    pFilter->video.codec[j].parm.h263.bIsSpecified )
               {
                  pOut->video.codec[uNumCodec].parm.h263.uLevel =
                     (pOut->video.codec[uNumCodec].parm.h263.uLevel <
                                    pFilter->video.codec[j].parm.h263.uLevel) ?
                              pOut->video.codec[uNumCodec].parm.h263.uLevel :
                              pFilter->video.codec[j].parm.h263.uLevel;
               }

               uNumCodec++;
               break;
            }
         }
      }
      pOut->video.uNumCodec = uNumCodec;

#if CCTKCFG_SRTP
      /* Copy SRTP info */
      pOut->video.crypto = pIn->video.crypto;
#endif
   }
#if CCTKCFG_SRTP
   else
   {
      /* Crypto info mismatch, disable media */
      pOut->video.mode = eCCTKMEDMODE_INACTIVE;
      pOut->video.uNumCodec = 0;
   }
#endif
#endif

   /* Copy fax info */
   pOut->t38 = pIn->t38;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterSilSuppSdp
**
**  PURPOSE:    Filter the input silsupp setting with the supplied filter.
**
**  PARAMETERS:
**              eIn - Input Silence Suppression setting.
**              eFilter - Reference Silence Suppression setting.
**
**
**  RETURNS:    Output Silence Suppression setting.
**
**  NOTES:
***********************************************************************************/
CCTKSILSUPP MseSdpSession::filterSilSuppSdp( CCTKSILSUPP eIn,
                                             CCTKSILSUPP eFilter )
{
   CCTKSILSUPP eOut;

   /* Generate Output silence suppression setting based on the following logic.
   ** In \ Filter | none |  on  | off  
   ** ------------+------+------+------
   **    none     | none | none | none
   ** ------------+------+------+------
   **    on       | none | on   | off
   ** ------------+------+------+------
   **    off      | none | off  | off
   ** ------------+------+------+------
   */
   if ( eIn == eCCTKSILSUPP_NOTUSED )
   {
      eOut = eCCTKSILSUPP_NOTUSED;
   }
   else if ( eIn == eCCTKSILSUPP_ON )
   {
      eOut = eFilter;
   }
   else /* if ( eOffSilSupp == eCCTKSILSUPP_OFF ) */
   {
      eOut = eCCTKSILSUPP_OFF;

      if ( eFilter == eCCTKSILSUPP_NOTUSED )
      {
         eOut = eCCTKSILSUPP_NOTUSED;
      }
   }

   return eOut;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterIlbcMode
**
**  PURPOSE:    Filter the input iLBC mode setting with the supplied filter.
**
**  PARAMETERS:
**              eIn - Input iLBC mode setting.
**              eFilter - Reference iLBC mode setting.
**
**
**  RETURNS:    Output iLBC mode setting.
**
**  NOTES:
***********************************************************************************/
CCTKILBCMODE MseSdpSession::filterIlbcMode( CCTKILBCMODE eIn,
                                            CCTKILBCMODE eFilter )
{
   CCTKILBCMODE eOut;

   /* Generate Output iLBC mode setting based on the following logic.
   ** In \ Filter | none |  20  | 30
   ** ------------+------+------+------
   **    none     | none |  20  | 30
   ** ------------+------+------+------
   **    20       |  20  |  20  | 30
   ** ------------+------+------+------
   **    30       |  30  |  30  | 30
   ** ------------+------+------+------
   */
   if ( eFilter == eCCTKILBCMODE_NOTUSED )
   {
      eOut = eIn;
   }
   else if ( eIn == eCCTKILBCMODE_NOTUSED )
   {
      eOut = eFilter;
   }
   else if ( eIn == eCCTKILBCMODE_30MS || eFilter == eCCTKILBCMODE_30MS )
   {
      eOut = eCCTKILBCMODE_30MS;
   }
   else
   {
      eOut = eCCTKILBCMODE_20MS;
   }

   return eOut;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterG7231Vad
**
**  PURPOSE:    Filter the input G.723.1 VAD setting with the supplied filter.
**
**  PARAMETERS:
**              eIn - Input G.723.1 VAD setting.
**              eFilter - Reference G.723.1 VAD setting.
**
**
**  RETURNS:    Output G.723.1 VAD setting.
**
**  NOTES:
***********************************************************************************/
CCTKG7231VAD MseSdpSession::filterG7231Vad( BOS_BOOL bIn, CCTKG7231VAD eFilter )
{
   CCTKG7231VAD eOut;

   /* Generate Output G.723.1 VAD setting based on the following logic.
   ** In \ Filter | none | yes  | no
   ** ------------+------+------+------
   **    true     | none | yes  | no
   ** ------------+------+------+------
   **    false    | no   | no   | no
   ** ------------+------+------+------
   */
   if ( bIn )
   {
      eOut = eFilter;
   }
   else
   {
      eOut = eCCTKG7231VAD_NO;
   }

   return eOut;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterG729Vad
**
**  PURPOSE:    Filter the input G.729 VAD setting with the supplied filter.
**
**  PARAMETERS:
**              eIn - Input G.729 VAD setting.
**              eFilter - Reference G.729 VAD setting.
**
**
**  RETURNS:    Output G.729 VAD setting.
**
**  NOTES:
***********************************************************************************/
CCTKG729VAD MseSdpSession::filterG729Vad( BOS_BOOL bIn, CCTKG729VAD eFilter )
{
   CCTKG729VAD eOut;

   /* Generate Output G.729 VAD setting based on the following logic.
   ** In \ Filter | none | yes  | no
   ** ------------+------+------+------
   **    true     | none | yes  | no
   ** ------------+------+------+------
   **    false    | no   | no   | no
   ** ------------+------+------+------
   */
   if ( bIn )
   {
      eOut = eFilter;
   }
   else
   {
      eOut = eCCTKG729VAD_NO;
   }

   return eOut;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::filterAmr
**
**  PURPOSE:    Filter the input AMR settings with the supplied filter.
**
**  PARAMETERS:
**              pIn - Input AMR settings
**              pOut - Output AMR settings
**              pFilter - Filter AMR settings
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::filterAmr( CCTKAMRPARM* pIn, CCTKAMRPARM* pOut, CCTKAMRPARM* pFilter )
{
   pOut->uModeSet = pIn->uModeSet;
   pOut->uMaxRed = pIn->uMaxRed;
   pOut->bOctetAlign = pIn->bOctetAlign;

   if( pIn->bModeChangeNeighbor != eCCTKAMR_NOTSET )
   {
      pOut->bModeChangeNeighbor = pIn->bModeChangeNeighbor;
   }
   else
   {
      pOut->bModeChangeNeighbor = pFilter->bModeChangeNeighbor;
   }

   if( pIn->bModeChangePeriod != eCCTKAMR_NOTSET )
   {
      pOut->bModeChangePeriod = pIn->bModeChangePeriod;
   }
   else
   {
      pOut->bModeChangePeriod = pFilter->bModeChangePeriod;
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::fsmProcess
**
**  PURPOSE:    Process event based on the current state.
**
**  PARAMETERS:
**              eEvt - FSM event
**              pParm - Event parameter.  NULL if none.
**
**
**  RETURNS:    BOS_BOOL - Whether or not to continue processing.
**
**  NOTES:
***********************************************************************************/
#define FSM( eEvt, eState ) ((( eEvt ) * eMSESTATE_MAX ) + eState )
BOS_BOOL MseSdpSession::fsmProcess( MSEEVT eEvt, void *pParm )
{
   CCTK_TRACED4( "MseSdpSession (%p) - fsmProcess(%d, %d, %p)",
                 this, eEvt, meState, pParm );
   switch ( FSM( eEvt, meState ))
   {
      case FSM( eMSEEVT_CE_MEDIA_NOT_READY, eMSESTATE_PENDINGOFFER ):
      case FSM( eMSEEVT_CE_MEDIA_NOT_READY, eMSESTATE_PENDINGCAPS ):
      case FSM( eMSEEVT_CE_MEDIA_NOT_READY, eMSESTATE_PENDINGANSWER ):
      case FSM( eMSEEVT_CE_MEDIA_NOT_READY, eMSESTATE_IDLE ):
      {
         /* Indicate the media cap will not be ready.
         ** The can be a remote user, and we can not offer the 
         ** SDP, until we know which user picks up.
         ** Thus, Notity user and force an empty 180 RINGING
         ** even if it is indicated that SDP in 180 is preferred */
         
         /* TODO: what do we do if 100rel is set, when
         ** there is a remote user, we need the SDP now. */

         /* TODO: Preconditions support */
         if ( mbNotifyUser )
         {
            /* Notify user, so it will send empty 180 RINGING. */
            ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
            mbNotifyUser = BOS_FALSE;
         }
         break;
      }
      case FSM( eMSEEVT_CE_MEDIA, eMSESTATE_PENDINGCAPS ):
      {
         /* Caps ready for use */
         meState = eMSESTATE_IDLE;
      }
      break;

      case FSM( eMSEEVT_CE_MEDIA, eMSESTATE_IDLE ):
      {
         /* Clear glare error retry timer */
         muMedUpdateRetryTmr = 0;

         /* Send SDP offer */
         sendSdpOffer();

         meState = eMSESTATE_OFFERSENT;
      }
      break;

      case FSM( eMSEEVT_CE_HOLD, eMSESTATE_IDLE ):
      {
         const CFGCALL *pCfgCall = CFG_CALL( muCid );

         if ( pCfgCall->xtl.supp.hold.bLocalHold )
         {
            /* Update hold state and media info */
            meLocHoldSt = eMSEHOLDST_HOLD;
            mpMedInfo->ingress.audio.mode = eCCTKMEDMODE_INACTIVE;
            mpMedInfo->egress.audio.mode = eCCTKMEDMODE_INACTIVE;

            /* Notify app with the hold status */
            ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_LHOLD ), NULL );
         }
         else
         {
            meLocHoldSt = eMSEHOLDST_HOLD_PENDING;
            mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
         }
      }
      break;

      case FSM( eMSEEVT_CE_UNHOLD, eMSESTATE_IDLE ):
      {
         const CFGCALL *pCfgCall = CFG_CALL( muCid );

         if ( pCfgCall->xtl.supp.hold.bLocalHold )
         {
            /* Update hold state and media info. */
            meLocHoldSt = eMSEHOLDST_NOT_HOLD;
            mpMedInfo->ingress.audio.mode =
               ( meRemHoldSt == eMSEHOLDST_HOLD ) ? eCCTKMEDMODE_INACTIVE :
                                                    eCCTKMEDMODE_ACTIVE;
            mpMedInfo->egress.audio.mode = eCCTKMEDMODE_ACTIVE;

            /* Notify app with the hold status. */
            ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_LUNHOLD ), NULL );
         }
         else
         {
            meLocHoldSt = eMSEHOLDST_UNHOLD_PENDING;
            mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
         }
      }
      break;

      case FSM( eMSEEVT_SCE_PREPAREOFFER, eMSESTATE_PENDINGCAPS ):
      {
         /* Pending offer (wait for caps approval) */
         meState = eMSESTATE_PENDINGOFFER;

#if CCTKCFG_REMOTE_USER_CONTROL
         /* Notify CE that we are pending to send an offer */
         ceNotify( muCid, eCEEVT_CALL_NET_PEND_OFFER_SDP, NULL );
#endif
      }
      break;

      case FSM( eMSEEVT_CE_MEDIA, eMSESTATE_PENDINGOFFER ):
      case FSM( eMSEEVT_SCE_PREPAREOFFER, eMSESTATE_IDLE ):
      {
         const CFGCALL *pCfgCall = CFG_CALL( muCid );

         /* Send a 180 with no SDP if we are sending SDP answer in 200 or if PRACK
         ** is not used.
         */
         if ( mbNotifyUser && 
              ( !pCfgCall->xtl.media.neg.bSdpAnswerIn180 || !mb100RelUsed ))
         {
            /* TODO: Preconditions support */

            /* Notify user of the 180. */
            ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );

            mbNotifyUser = BOS_FALSE;
         }

         /* We could be here because we recieved an INVITE without
         ** an SDP, in which case we should reset any remote hold states
         */
         if ( ( meState == eMSESTATE_IDLE ) && isHold( meRemHoldSt ) )
         {
            meRemHoldSt = eMSEHOLDST_UNHOLD_PENDING;
         }
         /* Prepare SDP offer for SCE */
         sendSdpOffer();

         meState = eMSESTATE_OFFERSENT;
      }
      break;
      /* Incoming call, save the peer SDP */
      case FSM( eMSEEVT_SCE_SDPOFFERLONG, eMSESTATE_PENDINGCAPS ):
      {
         /* Cache the offer for delayed processing.  Note that we NULL-append the
         ** blob here because the SDP parser needs it for detecting the end of SDP.
         */
         mpSdpSession = BRCM_NEW( CBlob )( *reinterpret_cast<CBlob *>( pParm ));
         mpSdpSession->Append( static_cast<uint8_t>( 0 ));

         /* Pending SDP answer (wait for caps approval) */
         meState = eMSESTATE_PENDINGANSWER;

#if CCTKCFG_REMOTE_USER_CONTROL
         /* Notify CE that we are pending to send an answer */
         /* SDP is stored in to the Call */
         ceNotify( muCid, eCEEVT_CALL_NET_PEND_ANSWER_SDP, mpSdpSession );
#endif
      }
      break;
      /* Incoming call, MSE now get the media cap */
      case FSM( eMSEEVT_CE_MEDIA, eMSESTATE_PENDINGANSWER ):
      {
         CCTK_ASSERT( pParm == NULL );
         pParm = mpSdpSession;
      }
      /* Note: Intentional fall through */

      /* Incoming call, we got an update/re-invite */
      case FSM( eMSEEVT_SCE_SDPOFFERLONG, eMSESTATE_IDLE ):
      {
#if CCTKCFG_REMOTE_USER_CONTROL
         const CFGCALL *pCfgCall = CFG_CALL( muCid );
         if ( mbNotifyUser && !pCfgCall->xtl.media.neg.bSdpAnswerIn180 
               && !mbEmergencyMode )
         {
            /* Send a 180 with no SDP */
            /* TODO: Preconditions support */

            /* Notify user of the 180.  Note this is posted as an event to CE as
            ** processing has to be asychronous. */
            
            ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
         }
#endif
      }
      /* Note: Intentional fall through */

      case FSM( eMSEEVT_SCE_SDPOFFERSHORT, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_SDPOFFERSHORTCANTFAIL, eMSESTATE_IDLE ):
      {
#if CCTKCFG_REMOTE_USER_CONTROL
         if ( mpAnsSdpBlob != NULL )
         {
            CBlob *pSdpBlob = BRCM_NEW( CBlob )( *mpAnsSdpBlob );
            /* NAT ALG, update the SDP if necessary (for LAN remote user) */
            natUpdateProxyUsrSdp ( *pSdpBlob );  
            /* remove the NULL at the end of the SDP if nessary */
            if ( pSdpBlob->GetAt( pSdpBlob->GetLastIndex() ) == 0 )
            {
               pSdpBlob->Erase( pSdpBlob->GetLastIndex() );
            }
            
            IMspOfferAnswerSessionMgr::EMessageTypeForPayload eMsgType;

            /* Assume answer in 200 for now */
            eMsgType = IMspOfferAnswerSessionMgr::eAUTOMATIC;

            /* Send the answer SDP */
            mxt_result result = mpMgr->EvNewSdpToSend( pSdpBlob,
                                                       mpEarlySdpBlob,
                                                       eMsgType,
                                                       0 );
            if ( MX_RIS_F( result ))
            {
               CCTK_TRACEI1( "fsmProcess - EvNewSdpToSend() returns error %d",
                             result );
            }
            pSdpBlob = NULL;
            mpEarlySdpBlob = NULL;

            /* Free up the cached SDP offer now that we are done with it */
            if ( mpSdpSession != NULL )
            {
               BRCM_DELETE( mpSdpSession );
               mpSdpSession = NULL;
            }

            /* we have sent our SDP */
            meState = eMSESTATE_OFFERSENT;
            break;
         }
#endif
         const CFGCALL *pCfgCall = CFG_CALL( muCid );
         CBlob *pSdpSession = reinterpret_cast<CBlob *>( pParm );
         CSdpCapabilitiesMgr *pRemCapsMgr = NULL;
         BOS_UINT16 uFailureCode;

         /* Parse the offer SDP */
         uFailureCode = validateRemoteSdp( *pSdpSession, pRemCapsMgr );
         if ( uFailureCode != 0 )
         {
            CCTK_ASSERT( pRemCapsMgr == NULL );

            /* Failure parsing remote SDP */
            CCTK_TRACEI0( "fsmProcess - Failure parsing remote SDP" );

            /* Report negotiation failure.  For pre-established dialog, the call
            ** would be terminated as well.  For established dialog, we have to
            ** manually terminate.  Note the member variables are destoryed after
            ** mpMgr->EvNegotiationFailure() is called.
            */
            BOS_BOOL bManualTerminate = mbDlgEstablished;

            IMspOfferAnswerSessionMgr::ENegotiationFailureReason eFailureReason;
            if ( uFailureCode == uNOT_ACCEPTABLE_HERE )
            {
               eFailureReason = IMspOfferAnswerSessionMgr::eSYNTAX_ERROR_SDP_SESSION;
            }
            else
            {
               eFailureReason = 
                  static_cast<IMspOfferAnswerSessionMgr::ENegotiationFailureReason>( 
                     uFailureCode );
            }
            mpMgr->EvNegotiationFailure(
               eFailureReason,
               uFailureCode );

            if ( bManualTerminate )
            {
               mpMgr->EvTerminalFailure();
            }

            /* The MSE object is already destoryed at this point, no clean up
            ** necessary.
            */
            return BOS_FALSE;
         }
         else
         {
            BOS_BOOL bHoldChange = BOS_FALSE;
            BOS_BOOL bEmerg = BOS_FALSE;

            /* Check if the received SDP is a hold / unhold for re-INVITE / UPDATE.
            ** We always assume that the first SDP received is not an hold/unhold
            ** event.
            */
            if ( mpRemCapsMgr != NULL )
            {
               bHoldChange = updateRemHoldState( pRemCapsMgr );
            }

            /* Emergency mode, resume current hold state and generate a 180. See
            ** MseSdpSession::hold() for detailed description of the behavior in
            ** this mode.
            */
            if ( mbEmergencyMode )
            {
               meLocHoldSt = eMSEHOLDST_NOT_HOLD;
               mbEmergencyMode = BOS_FALSE;
               bEmerg = BOS_TRUE;
            }

            /* Allocate storage for the local SDP */
            CSdpCapabilitiesMgr *pLocCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr );
            createLocalSdp( pLocCapsMgr, BOS_FALSE, BOS_FALSE );
            
#if CCTKCFG_ICE_LITE_SUPPORT            
            /* ICE will update connection address, set the connection address type 
            ** to be the same as the peer connection address type.
            ** (This is required by GenerateAnswer() */
            pLocCapsMgr->GetSdpSession()->GetConnectionData().SetAddressTypeId( 
               pRemCapsMgr->GetSdpSession()->GetConnectionData().GetAddressTypeId() ); 
#endif /* CCTKCFG_ICE_LITE_SUPPORT */     

            /* Generate an answer SDP based on the policies */
            CSdpCapabilitiesMgr *pAnsCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr );
            pAnsCapsMgr->GenerateAnswer( *pRemCapsMgr, *pLocCapsMgr );

            /* Post answer SDP processing */
            BOS_BOOL bUseQuadZero = BOS_FALSE;
            processAnswerSdp( pAnsCapsMgr, bUseQuadZero, pRemCapsMgr, pLocCapsMgr );

#if CCTKCFG_ICE_LITE_SUPPORT
            {
               /* pAnsCapsMgr holds the answer corresponding to the peer SDP */
               BOS_BOOL bRes;
               bRes = ICENegotiation ( pRemCapsMgr, pAnsCapsMgr, BOS_FALSE );            
               if ( bRes == BOS_FALSE )
               {
                  CCTK_TRACED0( "fsmProcess - ICE Negotiation failed." );
                  mpMgr->EvNegotiationFailure(
                      IMspOfferAnswerSessionMgr::eINCOMPATIBE_ADDRESS_FORMAT );                  
                  BRCM_DELETE( pLocCapsMgr );
                  BRCM_DELETE( pRemCapsMgr );
                  BRCM_DELETE( pAnsCapsMgr );
                  return BOS_FALSE;
               }
            }            
#endif /* CCTKCFG_ICE_LITE_SUPPORT */     
            
            if ( isSdpSupported( pAnsCapsMgr, pRemCapsMgr ))
            {
               /* Prepare SDP based on the configured media profile */
               CBlob *pSdpBlob = createSdpBlob( pAnsCapsMgr, bUseQuadZero );

               IMspOfferAnswerSessionMgr::EMessageTypeForPayload eMsgType;
               
               if ( ( eEvt == eMSEEVT_SCE_SDPOFFERLONG || 
                      eEvt == eMSEEVT_CE_MEDIA ) &&
                      mbNotifyUser )
               {
                  if ( pCfgCall->xtl.media.neg.bSdpAnswerIn180 )
                  {
                     eMsgType = IMspOfferAnswerSessionMgr::eINVITE_RINGING;
                  }
                  else
                  {
                     /* Notify local user and send a 180 with no SDP */
                     if ( bEmerg )
                     {
                        /* Emergency call */
                        eMsgType = IMspOfferAnswerSessionMgr::eINVITE_RINGING;
                        pSdpBlob = NULL;
                        mpEarlySdpBlob = NULL;
                     }
                     else
                     {
                        eMsgType = IMspOfferAnswerSessionMgr::eAUTOMATIC;
                     }
                     ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
                  }
                  mbNotifyUser = BOS_FALSE;
               }
               else
               {
                  eMsgType = IMspOfferAnswerSessionMgr::eAUTOMATIC;
               }

               /* Send the answer SDP */
               mxt_result result = mpMgr->EvNewSdpToSend( pSdpBlob,
                                                          mpEarlySdpBlob,
                                                          eMsgType,
                                                          0 );
               if ( MX_RIS_F( result ))
               {
                  CCTK_TRACEI1( "fsmProcess - EvNewSdpToSend() returns error %d",
                                result );
               }
               else if ( eMsgType == IMspOfferAnswerSessionMgr::eINVITE_RINGING )
               {
                  /* Notify CE for the ringback */
                  ceNotify( muCid, eCEEVT_CALL_NET_RING, NULL );
               }
               pSdpBlob = NULL;
               mpEarlySdpBlob = NULL;
            }
            else if ( eEvt != eMSEEVT_SCE_SDPOFFERSHORTCANTFAIL )
            {
               CCTK_TRACED0( "fsmProcess - No common media format found in offer SDP" );

               /* No match!  Dump the local, remote, and answer SDP 
               ** mimic code from createSdpBlob */
               {
                  unsigned int i;
                  unsigned int size;
                  char *str;
                  char *endstr;
                  char line[256];
                  CBlob *pSdpBlob;
                  CSdpPacket *pSdpPacket;
                  
                  CCTK_TRACED0( "pLocCapsMgr:  SDP - ");   
                  pSdpPacket = BRCM_NEW( CSdpPacket );                  
                  pLocCapsMgr->CopyCapsToPacket( *pSdpPacket );
                  pSdpBlob = BRCM_NEW( CBlob );
                  pSdpPacket->Serialize( *pSdpBlob );
                  BRCM_DELETE( pSdpPacket );

                  pSdpBlob->Append((uint8_t)0);
                  str = (char*)pSdpBlob->GetFirstIndexPtr();
                  do 
                  {
                     endstr = strchr(str, '\n');
                     if ( endstr )
                     {
                        *endstr = 0;
                        CCTK_TRACED1( "%s", str );
                        str = endstr + 1;
                     }
                  } while ( endstr );
                  BRCM_DELETE( pSdpBlob );

                  
                  CCTK_TRACED0( "pRemCapsMgr:  SDP - ");   
                  pSdpPacket = BRCM_NEW( CSdpPacket );                  
                  pRemCapsMgr->CopyCapsToPacket( *pSdpPacket );
                  pSdpBlob = BRCM_NEW( CBlob );
                  pSdpPacket->Serialize( *pSdpBlob );
                  BRCM_DELETE( pSdpPacket );

                  pSdpBlob->Append((uint8_t)0);
                  str = (char*)pSdpBlob->GetFirstIndexPtr();
                  do 
                  {
                     endstr = strchr(str, '\n');
                     if ( endstr )
                     {
                        *endstr = 0;
                        CCTK_TRACED1( "%s", str );
                        str = endstr + 1;
                     }
                  } while ( endstr );
                  BRCM_DELETE( pSdpBlob );                  
                  
                  CCTK_TRACED0( "pAnsCapsMgr:  SDP - ");   
                  pSdpPacket = BRCM_NEW( CSdpPacket );                  
                  pAnsCapsMgr->CopyCapsToPacket( *pSdpPacket );
                  pSdpBlob = BRCM_NEW( CBlob );
                  pSdpPacket->Serialize( *pSdpBlob );
                  BRCM_DELETE( pSdpPacket );

                  pSdpBlob->Append((uint8_t)0);
                  str = (char*)pSdpBlob->GetFirstIndexPtr();
                  do 
                  {
                     endstr = strchr(str, '\n');
                     if ( endstr )
                     {
                        *endstr = 0;
                        CCTK_TRACED1( "%s", str );
                        str = endstr + 1;
                     }
                  } while ( endstr );
                  BRCM_DELETE( pSdpBlob );   
               }
               
               /* Report negotiation failure.  For pre-established dialog, the call
               ** would be terminated as well.  For established dialog, we do not
               ** terminate the call.  Note the member variables are destoryed after
               ** mpMgr->EvNegotiationFailure() is called.
               */
               BOS_BOOL bTerminate = !mbDlgEstablished;

               mpMgr->EvNegotiationFailure(
                  IMspOfferAnswerSessionMgr::eINCOMPATIBLE_MEDIA_FORMAT );

               if ( bTerminate )
               {
                  BRCM_DELETE( pRemCapsMgr );
                  pRemCapsMgr = NULL;

                  BRCM_DELETE( pLocCapsMgr );
                  pLocCapsMgr = NULL;

                  BRCM_DELETE( pAnsCapsMgr );
                  pAnsCapsMgr = NULL;

                  /* The MSE object is already destoryed at this point, no additional
                  ** clean up necessary.
                  */
                  return BOS_FALSE;
               }
            }

            /* Check if we support the offer */
            if ( !isSdpSupported( pAnsCapsMgr, pRemCapsMgr ))
            {
               CCTK_TRACED0( "fsmProcess - Unsupported offer SDP" );

               /* Revert remote hold state */
               if ( bHoldChange )
               {
                  meRemHoldSt =
                     isHold( meRemHoldSt ) ? eMSEHOLDST_NOT_HOLD : eMSEHOLDST_HOLD;
               }

               /* Note: We assume here that we would never run into any SDP
               ** negotiation failure for the supervisory emergency call unhold
               ** here.
               */
            }
            else if ( isSdpPrecondApproved( pAnsCapsMgr ) &&
                      pAnsCapsMgr->GetNbStreams() > 0 )
            {
               /* Prepare media info for the app */
               if ( mpMedInfo != NULL )
               {
                  BRCM_DELETE( mpMedInfo );
               }
               mpMedInfo = BRCM_NEW( CCTKMEDINFO );
               memset(mpMedInfo, 0, sizeof( CCTKMEDINFO ));
#if CCTKCFG_ICE_LITE_SUPPORT
               mpMedInfo->iceInfo.eCctkEvt = eCCTKEVT_MEDIA;               
#endif
               convertSdpFromMx( pAnsCapsMgr, &mpMedInfo->egress, BOS_FALSE, BOS_FALSE );
               CCTKSTRMINFO remStreamInfo;
               
               /* if local SRTP capability is not present, use build unsecured-only capability into remote stream */
               convertSdpFromMx( pRemCapsMgr, &remStreamInfo, BOS_TRUE, !(mpMedInfo->egress.audio.crypto.bIsSpecified) );
               filterSdp( &remStreamInfo, &mpMedInfo->egress, &mpMedInfo->ingress );

               /* We notify the app for regular session media only if early media is
               ** not used.  If early media is used, then we hold off the regular
               ** session media notification until the call is accepted.
               */
               if ( mpEarlyMedInfo == NULL )
               {
                  /* Update app with the media info */
                  CCTKEVT eCctkEvt;
                  if ( !bHoldChange )
                  {
                     /* Report a media change event */
                     eCctkEvt = eCCTKEVT_MEDIA;
                  }
                  else if ( isHold( meRemHoldSt ))
                  {
                     /* Report a remote hold event */
                     eCctkEvt = eCCTKEVT_HOLD;
                  }
                  else /* if ( !isHold( meRemHoldSt )) */
                  {
                     /* Report a remote unhold event */
                     eCctkEvt = eCCTKEVT_UNHOLD;
                  }
#if CCTKCFG_ICE_LITE_SUPPORT
                  /* notify Application if no ICE, else ICE callback will notify */
                  if ( meIceRmtMode == eMSEICEMODE_NONE )
                  {
                     /* Update app with the media change event */
                  ceNotify( muCid, static_cast<CEEVT>( eCctkEvt ), mpMedInfo );
               }
                  else
                  {
                     mpMedInfo->iceInfo.eCctkEvt = eCctkEvt;
                     mpMedInfo->iceInfo.audio.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
#if CCTKCFG_VIDEO
                     mpMedInfo->iceInfo.video.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */         
#endif
                     mpMedInfo->iceInfo.t38.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
                  }
#else
                  ceNotify( muCid, static_cast<CEEVT>( eCctkEvt ), mpMedInfo );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
               }

               /* Cache the remote SDP */
               if ( mpRemCapsMgr != NULL )
               {
                  BRCM_DELETE( mpRemCapsMgr );
               }
               mpRemCapsMgr = pRemCapsMgr;
               pRemCapsMgr = NULL;
            }

            BRCM_DELETE( pRemCapsMgr );
            pRemCapsMgr = NULL;

            BRCM_DELETE( pLocCapsMgr );
            pLocCapsMgr = NULL;

            BRCM_DELETE( pAnsCapsMgr );
            pAnsCapsMgr = NULL;
         }

         /* Free up the cached SDP offer now that we are done with it */
         if ( mpSdpSession != NULL )
         {
            BRCM_DELETE( mpSdpSession );
            mpSdpSession = NULL;
         }

         meState = eMSESTATE_IDLE;
      }
      break;

      case FSM( eMSEEVT_SCE_EARLYSDPOFFER, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_EARLYSDPOFFER, eMSESTATE_OFFERSENT ):
      {
         CBlob *pSdpSession = reinterpret_cast<CBlob *>( pParm );
         CSdpCapabilitiesMgr *pRemCapsMgr = NULL;
         BOS_UINT16 uFailureCode;

         /* Parse the offer SDP */
         uFailureCode = validateRemoteSdp( *pSdpSession, pRemCapsMgr );
         if ( uFailureCode != 0 )
         {
            CCTK_ASSERT( pRemCapsMgr == NULL );

            /* Failure parsing remote SDP */
            CCTK_TRACEI0( "fsmProcess - Failure parsing early remote SDP" );
            IMspOfferAnswerSessionMgr::ENegotiationFailureReason eFailureReason;
            if ( uFailureCode == uNOT_ACCEPTABLE_HERE )
            {
               eFailureReason = IMspOfferAnswerSessionMgr::eSYNTAX_ERROR_SDP_EARLY_SESSION;
            }
            else
            {
               eFailureReason = 
                  static_cast<IMspOfferAnswerSessionMgr::ENegotiationFailureReason>( 
                     uFailureCode );
            }            
            mpMgr->EvNegotiationFailure(
               eFailureReason,
               uFailureCode );

            /* The MSE object is already destoryed at this point, no clean up
            ** necessary.
            */
            return BOS_FALSE;
         }
         else
         {
            /* Allocate storage for the local SDP */
            CSdpCapabilitiesMgr *pLocCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr );
            createLocalSdp( pLocCapsMgr, BOS_FALSE, BOS_TRUE );
#if CCTKCFG_ICE_LITE_SUPPORT            
            /* ICE will update connection address, set the connection address type 
            ** to be the same as the peer connection address type.
            ** (This is required by GenerateAnswer() */
            pLocCapsMgr->GetSdpSession()->GetConnectionData().SetAddressTypeId( 
               pRemCapsMgr->GetSdpSession()->GetConnectionData().GetAddressTypeId() ); 
#endif /* CCTKCFG_ICE_LITE_SUPPORT */                 

            /* Generate an answer SDP based on the policies */
            CSdpCapabilitiesMgr *pAnsCapsMgr = BRCM_NEW( CSdpCapabilitiesMgr );
            pAnsCapsMgr->GenerateAnswer( *pRemCapsMgr, *pLocCapsMgr );

            /* Post answer SDP processing */
            BOS_BOOL bUseQuadZero = BOS_FALSE;
            processAnswerSdp( pAnsCapsMgr, bUseQuadZero, pRemCapsMgr, pLocCapsMgr );

#if CCTKCFG_ICE_LITE_SUPPORT
            {
               /* pAnsCapsMgr holds the answer corresponding to the peer SDP */
               BOS_BOOL bRes;
               bRes = ICENegotiation ( pRemCapsMgr, pAnsCapsMgr, BOS_TRUE );            
               if ( bRes == BOS_FALSE )
               {
                  CCTK_TRACED0( "fsmProcess - ICE Negotiation failed." );
                  mpMgr->EvNegotiationFailure(
                      IMspOfferAnswerSessionMgr::eINCOMPATIBE_ADDRESS_FORMAT );                  
                  BRCM_DELETE( pLocCapsMgr );
                  BRCM_DELETE( pRemCapsMgr );
                  BRCM_DELETE( pAnsCapsMgr );
                  return BOS_FALSE;
               }
            }            
#endif /* CCTKCFG_ICE_LITE_SUPPORT */    
            
            /* Prepare SDP based on the configured media profile.  We cache the SDP
            ** response and send it together with the regular SDP session. */
            CCTK_ASSERT( mpEarlySdpBlob == NULL );
            mpEarlySdpBlob = createSdpBlob( pAnsCapsMgr, bUseQuadZero );

            /* Check if we support the offer */
            if ( !isSdpSupported( pAnsCapsMgr, pRemCapsMgr ))
            {
               CCTK_TRACED0( "fsmProcess - Unsupported early offer SDP" );
            }
            else if ( pAnsCapsMgr->GetNbStreams() > 0 )
            {
               /* Prepare media info for the app */
               if ( mpEarlyMedInfo != NULL )
               {
                  BRCM_DELETE( mpEarlyMedInfo );
               }
               mpEarlyMedInfo = BRCM_NEW( CCTKMEDINFO );
               memset( mpEarlyMedInfo, 0, sizeof(CCTKMEDINFO) );
               convertSdpFromMx( pAnsCapsMgr, &mpEarlyMedInfo->egress, BOS_FALSE, BOS_FALSE );
               CCTKSTRMINFO remStreamInfo;
               /* if local SRTP capability is not present, use build unsecured-only capability into remote stream */
               convertSdpFromMx( pRemCapsMgr, &remStreamInfo, BOS_TRUE, !(mpEarlyMedInfo->egress.audio.crypto.bIsSpecified) );
               filterSdp( &remStreamInfo,
                          &mpEarlyMedInfo->egress,
                          &mpEarlyMedInfo->ingress );

#if CCTKCFG_ICE_LITE_SUPPORT
               /* notify Application if no ICE, else ICE callback will notify */
               if ( meIceRmtMode == eMSEICEMODE_NONE )
               {
               /* Update app with the media change event */
               ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_MEDIA ), mpEarlyMedInfo );
            }
               else
               {
                  mpEarlyMedInfo->iceInfo.audio.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
#if CCTKCFG_VIDEO
                  mpEarlyMedInfo->iceInfo.video.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */         
#endif
                  mpEarlyMedInfo->iceInfo.t38.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
               }
#else
               ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_MEDIA ), mpEarlyMedInfo );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
            }

            BRCM_DELETE( pRemCapsMgr );
            pRemCapsMgr = NULL;

            BRCM_DELETE( pLocCapsMgr );
            pLocCapsMgr = NULL;

            BRCM_DELETE( pAnsCapsMgr );
            pAnsCapsMgr = NULL;
         }
      }
      break;
      case FSM( eMSEEVT_SCE_SDPANSWERPREVIEW, eMSESTATE_OFFERSENT ):
      case FSM( eMSEEVT_SCE_SDPANSWERFINAL, eMSESTATE_OFFERSENT ):
      {
         /* Cache the offer for delayed processing.  Note that we NULL-append the
         ** blob here because the SDP parser needs it for detecting the end of SDP.
         */      
         CBlob *pSdpSession = reinterpret_cast<CBlob *>( pParm );
         pSdpSession->Append( static_cast<uint8_t>( 0 ));
         
#if CCTKCFG_REMOTE_USER_CONTROL
         /* This is the SDP received from network.
         ** This can be coming from 180 Ringing or 200 OK.
         ** we will need to include this SDP when we send the 
         ** 200 OK to our remote user.
         */
         /* implicitly say this is a remote MSE, since we have a SDP from
         ** a remote user */
         if ( mpOffSdpBlob != NULL )
         {
            /* Notify CE the Answer SDP */
            ceNotify( muCid, eCEEVT_CALL_NET_ANSWER_SDP, pSdpSession );
            
            /* change state to IDLE, so we will send 200 OK when handling
            ** eMSEEVT_SCE_REMOTEACCEPTED event */
            meState = eMSESTATE_IDLE;
            break;
         }
#endif
         CSdpCapabilitiesMgr *pRemCapsMgr = NULL;

         /* Parse the answer SDP */
         if ( validateRemoteSdp( *pSdpSession, pRemCapsMgr ) != 0 )
         {
            CCTK_ASSERT( pRemCapsMgr == NULL );

            /* Failure parsing remote SDP */
            CCTK_TRACEI0( "fsmProcess - Failure parsing remote SDP" );

            undoLocalCallState();
            meState = eMSESTATE_IDLE;
            break;
         }
         
#if CCTKCFG_ICE_LITE_SUPPORT
         {
            /* pAnsCapsMgr holds the answer corresponding to the peer SDP */
            BOS_BOOL bRes;
            bRes = ICENegotiation ( pRemCapsMgr, mpLocCapsMgr, BOS_FALSE );            
            if ( bRes == BOS_FALSE )
            {
               CCTK_TRACED0( "fsmProcess - ICE Negotiation failed." );
               BRCM_DELETE( mpLocCapsMgr );
               mpLocCapsMgr = NULL;
               BRCM_DELETE( pRemCapsMgr );
               undoLocalCallState();
               meState = eMSESTATE_IDLE;
               break;               
            }
         }            
#endif /* CCTKCFG_ICE_LITE_SUPPORT */    
                     
         /* Check if the remote party rejected the offer. */
         if ( !isSdpSupported( pRemCapsMgr ))
         {
            CCTK_TRACEI0( "fsmProcess - Offered SDP unsupported" );

            undoLocalCallState();
            meState = eMSESTATE_IDLE;
            BRCM_DELETE( pRemCapsMgr );
            pRemCapsMgr = NULL;
            break;
         }

         BOS_BOOL bResendInvite = BOS_FALSE;
         BOS_BOOL bNotifyCe = BOS_TRUE;

#if CCTKCFG_MULTI_HOLD_METHOD
         /* For Hold and Unhold, we specifically check if the remote party
         ** supports media attribute.  If not we will resend the SDP without
         ** media attribute.
         */
         if ( mbNoQuadZeros &&
              ( meLocHoldSt == eMSEHOLDST_HOLD_PENDING ||
                meLocHoldSt == eMSEHOLDST_UNHOLD_PENDING ))
         {
            BOS_UINT32 i, uNumStreams = pRemCapsMgr->GetNbStreams();
            for ( i = 0; i < uNumStreams; i++ )
            {
               /* Look at a= mode attribute in the audio stream */
               if ( pRemCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
                    ( pRemCapsMgr->GetStreamTransportProtocol( i ) ==
                       CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
                      || pRemCapsMgr->GetStreamTransportProtocol( i ) ==
                       CSdpParser::eRTPSAVP
#endif
                    ))
               {
                  CSdpLevelMedia &rStream = pRemCapsMgr->GetStream( i );

                  /* Declare that the remote party is not compliant to
                  ** RFC 3261.
                  */
                  if ( rStream.GetDirection() ==
                          CSdpParser::eUNKNOWN_ATTRIBUTE_TYPE ||
                       ( meLocHoldSt == eMSEHOLDST_HOLD_PENDING &&
                         !rStream.IsInactive() ))
                  {
                     /* Quad-zero hold method */
                     mbNoQuadZeros = BOS_FALSE;
                     break;
                  }
               }
            }

            /* Resend the hold/unhold and do not notify CE if the quad-zero hold
            ** method is used.
            */
            if ( !mbNoQuadZeros )
            {
               /* Temporarily override the current state */
               meState = eMSESTATE_IDLE;

               /* Send the updated offer */
               mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );

               /* Do not notify CE as we are not done yet */
               bNotifyCe = BOS_FALSE;

               /* Skip updating the state */
               bResendInvite = BOS_TRUE;
            }
         }
#endif
         /* Check if the remote party properly accepted the offer.
         */
         const CFGCALL *pCfgCall = CFG_CALL( muCid );
         if ( !bResendInvite &&
              meLocHoldSt == eMSEHOLDST_HOLD_PENDING &&
              ( !pCfgCall->xtl.supp.hold.bBypassHoldRspChk &&
                !isSdpHoldAcceptable( pRemCapsMgr,
                                      !mbNoQuadZeros,
                                      pCfgCall->xtl.supp.hold.bUseInactive )))
         {
            CCTK_TRACEI0( "fsmProcess - Invalid 'SDP Hold' Answer" );

            undoLocalCallState();
            meState = eMSESTATE_IDLE;
            BRCM_DELETE( pRemCapsMgr );
            pRemCapsMgr = NULL;
            break;
         }

         /* If this answer is for an offer triggered by an empty INVITE,
         ** the remote hold state could be pending an unhold, 
         ** check to see if this unhold is accepted.
         */
         MSEHOLDST prevRemHoldSt = meRemHoldSt;
         if( meRemHoldSt == eMSEHOLDST_UNHOLD_PENDING )
         {
            updateRemHoldState( pRemCapsMgr );
         }
         
         if ( bNotifyCe )
         {
            CCTK_ASSERT( mpLocCapsMgr != NULL );

            /* Prepare media info for CE */
            if ( mpMedInfo != NULL )
            {
               BRCM_DELETE( mpMedInfo );
            }
            mpMedInfo = BRCM_NEW( CCTKMEDINFO );
            convertSdpFromMx( pRemCapsMgr, &mpMedInfo->ingress, BOS_TRUE, BOS_FALSE );
            CCTKSTRMINFO locStreamInfo;
            convertSdpFromMx( mpLocCapsMgr, &locStreamInfo, BOS_FALSE, BOS_FALSE );
            if (mpMedInfo->ingress.audio.crypto.bIsSpecified == BOS_FALSE && 
                locStreamInfo.audio.crypto.bIsSpecified == BOS_TRUE)
            {
               /* Special case not handled by current negotiation code. 
                * Just redo the conversion for unsecured connection 
                */
                convertSdpFromMx( mpLocCapsMgr, &locStreamInfo, BOS_FALSE, BOS_TRUE );
            }     
            filterSdp( &locStreamInfo, &mpMedInfo->ingress, &mpMedInfo->egress );

            /* We notify the app for regular session media only if early media is
            ** not used.  If early media is used, then we hold off the regular
            ** session media notification until the call is accepted.
            */
            if ( mpEarlyMedInfo == NULL )
            {
               /* Update app with the media info */
               CCTKEVT eCctkEvt;
               if ( meLocHoldSt == eMSEHOLDST_HOLD_PENDING )
               {
                  /* Report a local hold accepted */
                  eCctkEvt = eCCTKEVT_LHOLD;

                  /* Update local hold state */
                  meLocHoldSt = eMSEHOLDST_HOLD;
               }
               else if ( meLocHoldSt == eMSEHOLDST_UNHOLD_PENDING )
               {
                  /* Report a local unhold accepted */
                  eCctkEvt = eCCTKEVT_LUNHOLD;

                  /* Update local hold state */
                  meLocHoldSt = eMSEHOLDST_NOT_HOLD;
               }
               else if ( ( eEvt == eMSEEVT_SCE_SDPANSWERFINAL ) &&
                         ( prevRemHoldSt == eMSEHOLDST_UNHOLD_PENDING ) &&
                         ( meRemHoldSt == eMSEHOLDST_NOT_HOLD ) )
               {
                  eCctkEvt = eCCTKEVT_UNHOLD;
               }
               else
               {
                  eCctkEvt = eCCTKEVT_MEDIA;
               }
#if CCTKCFG_ICE_LITE_SUPPORT
               /* notify Application if no ICE, else ICE callback will notify */
               if ( meIceRmtMode == eMSEICEMODE_NONE )
               {
                  /* Update app with the media change event */
               ceNotify( muCid, static_cast<CEEVT>( eCctkEvt ), mpMedInfo );
            }
               else
               {
                  mpMedInfo->iceInfo.eCctkEvt = eCctkEvt;
                  mpMedInfo->iceInfo.audio.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
#if CCTKCFG_VIDEO
                  mpMedInfo->iceInfo.video.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */         
#endif
                  mpMedInfo->iceInfo.t38.bIceTsNotReady = BOS_TRUE;  /* no ice valid pair not ready */
         }
#else
               ceNotify( muCid, static_cast<CEEVT>( eCctkEvt ), mpMedInfo );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
            }
         }

         /* Cache the remote SDP */
         if ( mpRemCapsMgr != NULL )
         {
            BRCM_DELETE( mpRemCapsMgr );
         }
         mpRemCapsMgr = pRemCapsMgr;
         pRemCapsMgr = NULL;

         if ( !bResendInvite && 
               ( pCfgCall->xtl.media.neg.bFirstSdpIsFinal || 
                  ( eEvt == eMSEEVT_SCE_SDPANSWERFINAL ) ) )
         {
            meState = eMSESTATE_IDLE;
         }
      }
      break;

#if CCTKCFG_ICE_LITE_SUPPORT  
      case FSM( eMSEEVT_SCE_SDPANSWERFINAL ,eMSESTATE_IDLE ):
      {
         /* We are here because the first (preview) answer received was to be used
         ** as the final answer but ice negotiation requires us to wait till 
         ** the offer/answer negotiation is complete.
         */
         if ( mpRemCapsMgr != NULL  )
         {
            /* pAnsCapsMgr holds the answer corresponding to the peer SDP */
            BOS_BOOL bRes;
            bRes = ICENegotiation ( mpRemCapsMgr, mpLocCapsMgr, BOS_FALSE );            
            if ( bRes == BOS_FALSE )
            {
               CCTK_TRACED0( "fsmProcess - ICE Negotiation failed." );
               BRCM_DELETE( mpLocCapsMgr );
               mpLocCapsMgr = NULL;
               BRCM_DELETE( mpRemCapsMgr );
               mpRemCapsMgr = NULL;
               undoLocalCallState();
               meState = eMSESTATE_IDLE;
               break;               
            }                    
         }
      }
      break;
#endif

      case FSM( eMSEEVT_SCE_CANCELOFFER, eMSESTATE_OFFERSENT ):
      {
         undoLocalCallState();

         meState = eMSESTATE_IDLE;
      }
      break;

      case FSM( eMSEEVT_SCE_RESETNEGOTIATION, eMSESTATE_IDLE ):
      {
         /* Reset remote SDP info as if we just sent an offer */
         mbNoQuadZeros = BOS_FALSE;
         meRemHoldSt = eMSEHOLDST_NOT_HOLD;

         if ( mpRemCapsMgr != NULL )
         {
            BRCM_DELETE( mpRemCapsMgr );
            mpRemCapsMgr = NULL;
         }

         if ( mpMedInfo != NULL )
         {
            BRCM_DELETE( mpMedInfo );
            mpMedInfo = NULL;
         }

         meState = eMSESTATE_OFFERSENT;
      }
      break;
      case FSM( eMSEEVT_SCE_REMOTEACCEPTED, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_REMOTEACCEPTED, eMSESTATE_OFFERSENT ):
      case FSM( eMSEEVT_SCE_LOCALACCEPTED, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_LOCALACCEPTED, eMSESTATE_OFFERSENT ):
      {
         /*
         ** Call accepted.
         */

         /* Reset media flow authorization as call is now accepted. */
         meEarlyMediaFlow = eMSEEMFLOW_SENDRECV;

         /*
         ** If early media info is used, then notify application with the media
         ** info.
         */
         mbDlgEstablished = BOS_TRUE;

         if ( mpEarlyMedInfo != NULL )
         {
            BRCM_DELETE( mpEarlyMedInfo );
            mpEarlyMedInfo = NULL;

            if ( mpMedInfo != NULL )
            {
#if CCTKCFG_ICE_LITE_SUPPORT
               /* notify Application if no ICE, else ICE callback will notify */
               if ( meIceRmtMode == eMSEICEMODE_NONE )
               {
                  /* Update app with the media change event */
               ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_MEDIA ), mpMedInfo );
            }
#else
               ceNotify( muCid, static_cast<CEEVT>( eCCTKEVT_MEDIA ), mpMedInfo );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
         }
      }
      }
      break;

      case FSM( eMSEEVT_SCE_GLAREERROR, eMSESTATE_OFFERSENT ):
      {
         /* SDP updates collided.  Schedule a timer for retry. */
         BOS_UINT32 uRetryAfterMs = reinterpret_cast<BOS_UINT32>( pParm );

         CCTK_ASSERT( muMedUpdateRetryTmr == 0 );
         CCTKSTATUS eStatus = tkmTimerStart( &muMedUpdateRetryTmr,
                                             uRetryAfterMs,
                                             BOS_FALSE,
                                             this,
                                             &MseSdpSession::tkmTimerCb );
         CCTK_ASSERT( CCTKSTATUS_IS_SUCCESS( eStatus ) );

         /* Update session state */
         meState = eMSESTATE_IDLE;
      }
      break;

      case FSM( eMSEEVT_SCE_PRETERMINATE, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_PRETERMINATE, eMSESTATE_OFFERSENT ):
      case FSM( eMSEEVT_SCE_PRETERMINATE, eMSESTATE_PENDINGANSWER ):
      case FSM( eMSEEVT_SCE_TERMINATE, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_TERMINATE, eMSESTATE_OFFERSENT ):
      case FSM( eMSEEVT_SCE_TERMINATE, eMSESTATE_PENDINGANSWER ):
#if CCTKCFG_REMOTE_USER_CONTROL      
      {
         if ( mpNatSesHdl ) 
         {
            MSENATSESHDL *pNextNatSesHdl;
            MSENATSESHDL *pCurNatSesHdl;
            pNextNatSesHdl = mpNatSesHdl;
            while ( pNextNatSesHdl ) 
            {
               pCurNatSesHdl = pNextNatSesHdl;
               pNextNatSesHdl = pCurNatSesHdl->pNext; 
               if ( CFG_SYS->xtl.sec.nat.rg.ctl.pNatSesDelFunc )
               {
                  CFG_SYS->xtl.sec.nat.rg.ctl.pNatSesDelFunc( pCurNatSesHdl->uNatSesHdl );
               }
               BRCM_DELETE( pCurNatSesHdl );
            }
            mpNatSesHdl = NULL;
         }
      }
      break;
#endif     
      case FSM( eMSEEVT_SCE_REMOTENOTIFIED, eMSESTATE_IDLE ):
      case FSM( eMSEEVT_SCE_REMOTENOTIFIED, eMSESTATE_OFFERSENT ):
      {
         /* No actions */
      }
      break;      

      default:
      {
         CCTK_TRACEI2( "fsmProcess - Error: Unhandled evt %d (state %d)", eEvt, meState );
      }
      break;
   }

   return BOS_TRUE;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::allocatePort
**
**  PURPOSE:    Allocate a port for the specific port type.
**
**  PARAMETERS:
**              uCId - Call Id.
**              eType - Port type.
**
**
**  RETURNS:    Port allocated.  0 if failed.
**
**  NOTES:
***********************************************************************************/
BOS_UINT16 MseSdpSession::allocatePort( BOS_UINT32 uCId, MSEPTTYPE eType )
{
   const CFGCALL *pCfgCall = CFG_CALL( uCId );
   BOS_UINT32 i, uCIdx;
   BOS_BOOL bFound, bUseEveryOtherPort;

   /* Sanity check */
   CCTK_ASSERT( eType < eMSEPTTYPE_MAX );
#if CCTKCFG_VIDEO
   CCTK_ASSERT(
      (( eType >= eMSEPTTYPE_AUD_START && eType <= eMSEPTTYPE_AUD_END ) &&
       pCfgCall->xtl.media.pAudio != NULL ) ||
      ( eType == eMSEPTTYPE_T38 && pCfgCall->xtl.media.pFax != NULL ) ||
      (( eType >= eMSEPTTYPE_VID_START && eType <= eMSEPTTYPE_VID_END ) &&
       pCfgCall->xtl.media.pVideo != NULL ));
#else
   CCTK_ASSERT(
      (( eType >= eMSEPTTYPE_AUD_START && eType <= eMSEPTTYPE_AUD_END ) &&
       pCfgCall->xtl.media.pAudio != NULL ) ||
      ( eType == eMSEPTTYPE_T38 && pCfgCall->xtl.media.pFax != NULL ));
#endif

   bUseEveryOtherPort = BOS_FALSE;
   if ((( eType >= eMSEPTTYPE_AUD_START && eType <= eMSEPTTYPE_AUD_END ) &&
        pCfgCall->xtl.media.pAudio->gen.rtcpPort.uMinPort == 0 )
#if CCTKCFG_VIDEO
        || (( eType >= eMSEPTTYPE_VID_START && eType <= eMSEPTTYPE_VID_END ) &&
        pCfgCall->xtl.media.pVideo->gen.rtcpPort.uMinPort == 0 )
#endif
      )
   {
#if CCTKCFG_RANDOM_PORTS
   /* Try to allocate ports randomly */
      bUseEveryOtherPort = BOS_FALSE;
#else
      /* RTCP port range undefined.  Use RTP port + 1 for RTCP port. */
      bUseEveryOtherPort = BOS_TRUE;
#endif
   }

   /* Find a matched call on the list */
   bFound = BOS_FALSE;
   uCIdx = CCTKCFG_MAX_CALL;
   for ( i = 0; i < CCTKCFG_MAX_CALL; i++ )
   {
      if ( mPtInfo.ptInfo[i].uCId == uCId )
      {
         if ( mPtInfo.ptInfo[i].uPort[eType] != 0 )
         {
            /* Port already allocated, return port */
            return mPtInfo.ptInfo[i].uPort[eType];
         }
         bFound = BOS_TRUE;
         uCIdx = i;
         break;
      }
      else if ( uCIdx == CCTKCFG_MAX_CALL && mPtInfo.ptInfo[i].uCId == 0xFFFFFFFF )
      {
         /* Found a free index.  Cache it in case needed. */
         uCIdx = i;
      }
   }

   CCTK_ASSERT( uCIdx != CCTKCFG_MAX_CALL );
   MSECALLPTINFO *pPtInfo = &mPtInfo.ptInfo[uCIdx];

   /* Allocate room to store the port info */
   if ( !bFound )
   {
      for ( i = 0; i < eMSEPTTYPE_MAX; i++ )
      {
         CCTK_ASSERT( pPtInfo->uPort[i] == 0 );
      }

      pPtInfo->uCId = uCId;
      pPtInfo->uUsrId = pCfgCall->in.uUsrId;
   }

   /* Allocate a port from the port range for the specified type */
   if ( isRtcpType( eType ) && bUseEveryOtherPort )
   {
      /* Allocate a RTP port and use the following port */
      BOS_UINT16 uRtpPort = allocatePort( uCId,
      static_cast<MSEPTTYPE>( static_cast<int>( eType ) - 1 ));

      if ( uRtpPort == 0 )
      {
         /* Port not allocated */
         pPtInfo->uPort[eType] = 0;
      }
      else
      {
         pPtInfo->uPort[eType] = uRtpPort + 1;
      }  
   }
   else if ( eType == eMSEPTTYPE_T38 &&
             pPtInfo->uPort[eMSEPTTYPE_AUD_RTP] != 0 &&
             pCfgCall->xtl.media.pAudio &&
             pCfgCall->xtl.media.pFax &&
             ( pCfgCall->xtl.media.pAudio->gen.rtpPort.uMinPort ==
               pCfgCall->xtl.media.pFax->t38Port.uMinPort ) &&
             ( pCfgCall->xtl.media.pAudio->gen.rtpPort.uMaxPort ==
               pCfgCall->xtl.media.pFax->t38Port.uMaxPort ))
   {
      /* Special case when T.38 and RTP (audio) share the same port range, then the
      ** same port would be reused for best interoperability in NAT scenarios.
      */
      pPtInfo->uPort[eType] = pPtInfo->uPort[eMSEPTTYPE_AUD_RTP];
   }
   else if ( eType == eMSEPTTYPE_AUD_RTP &&
             pPtInfo->uPort[eMSEPTTYPE_T38] != 0 &&
             pCfgCall->xtl.media.pAudio &&
             pCfgCall->xtl.media.pFax &&
             ( pCfgCall->xtl.media.pAudio->gen.rtpPort.uMinPort ==
               pCfgCall->xtl.media.pFax->t38Port.uMinPort ) &&
             ( pCfgCall->xtl.media.pAudio->gen.rtpPort.uMaxPort ==
               pCfgCall->xtl.media.pFax->t38Port.uMaxPort ))
   {
      /* Special case when T.38 and RTP (audio) share the same port range, then the
      ** same port would be reused for best interoperability in NAT scenarios.
      */
      pPtInfo->uPort[eType] = pPtInfo->uPort[eMSEPTTYPE_T38];
   }
   else
   {
      pPtInfo->uPort[eType] = findFreePort( uCId,
                                            eType,
                                            bUseEveryOtherPort );
   }

   if ( pPtInfo->uPort[eType] == 0 )
   {
      CCTK_TRACEI2(
         "allocatePort - Error: Failed to allocate port for call 0x%lX(%d)",
         uCId, eType );
   }

   return pPtInfo->uPort[eType];
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::findFreePort
**
**  PURPOSE:    Find an available port.
**
**  PARAMETERS:
**              uCId - Call Id.
**              eType - Port type.
**              bUseEveryOtherPort - Specifies whether or not to allocate port
**                                   consecutively or every other port
**                                   (for RTP/RTCP).
**
**
**  RETURNS:    Port allocated.  0 if failed.
**
**  NOTES:
***********************************************************************************/
BOS_UINT16 MseSdpSession::findFreePort( BOS_UINT32 uCId,
                                        MSEPTTYPE eType,
                                        BOS_BOOL bUseEveryOtherPort )
{
   BOS_UINT16 uPort, uIncrement, uOffset, uAllocatedPort, *puLastAllocatedPort;
   BOS_UINT32 i, uUsrId;
   const CFGCALL *pCfgCall = CFG_CALL( uCId );
   CCTKPTRANGE *pPortRange = NULL;

   switch ( eType )
   {
      case eMSEPTTYPE_AUD_RTP:
      case eMSEPTTYPE_AUD_SRTP:
         pPortRange = &pCfgCall->xtl.media.pAudio->gen.rtpPort;
         break;
      case eMSEPTTYPE_AUD_RTCP:
      case eMSEPTTYPE_AUD_SRTCP:
         pPortRange = &pCfgCall->xtl.media.pAudio->gen.rtcpPort;
         break;
#if CCTKCFG_VIDEO
      case eMSEPTTYPE_VID_RTP:
      case eMSEPTTYPE_VID_SRTP:
         pPortRange = &pCfgCall->xtl.media.pVideo->gen.rtpPort;
         break;
      case eMSEPTTYPE_VID_RTCP:
      case eMSEPTTYPE_VID_SRTCP:
         pPortRange = &pCfgCall->xtl.media.pVideo->gen.rtcpPort;
         break;
#endif
      case eMSEPTTYPE_T38:
         pPortRange = &pCfgCall->xtl.media.pFax->t38Port;
         break;
      default:
         break;
   }

   /* Sanity checks */
   CCTK_ASSERT( pPortRange != NULL );
   CCTK_ASSERT( pPortRange->uMinPort <= pPortRange->uMaxPort );

   BOS_UINT16 uMinPort = pPortRange->uMinPort;
   BOS_UINT16 uMaxPort = pPortRange->uMaxPort;

   /* When we want to allocate every other port (e.g. when RTCP uses RTP + 1), we
   ** want to make sure the specified port range would possibly go out of bound.
   */
   if ( bUseEveryOtherPort )
   {
      if ( uMinPort == uMaxPort )
      {
         /* Invalid port range to support allocating every other port */
         CCTK_TRACED1( "findFreePort - Error: Invalid port range (%d)", uMinPort );
         return 0;
      }

      if ((( uMaxPort - uMinPort ) % 2 ) == 0 )
      {
         uMaxPort--;
      }
   }

   /* Go through the potential ports from the available range, starting from the
   ** last allocated port if available.
   */
   uUsrId = pCfgCall->in.uUsrId;
   uIncrement = bUseEveryOtherPort ? 2 : 1;
   uAllocatedPort = 0;

   /* Go through the list of last allocated ports and find one that matches
   ** the specified port range.
   */
   uOffset = 0;
   puLastAllocatedPort = NULL;
   for ( i = 0; i < eMSEPTTYPE_MAX * CCTKCFG_MAX_CALL; i++ )
   {
      if ( mPtInfo.uLastAllocatedPort[i] != 0 )
      {
         if ( mPtInfo.uLastAllocatedPort[i] >= uMinPort &&
              mPtInfo.uLastAllocatedPort[i] <= uMaxPort )
         {
            uOffset =
               mPtInfo.uLastAllocatedPort[i] - uMinPort + uIncrement;
            puLastAllocatedPort = &mPtInfo.uLastAllocatedPort[i];
            break;
         }
      }
      else if ( puLastAllocatedPort == NULL )
      {
         /* Remember the first available last allocated port storage if not found */
         puLastAllocatedPort = &mPtInfo.uLastAllocatedPort[i];
         break;
      }
   }

   /* Here, we have exhausted all the storage available.  Use the first storage. */
   if ( puLastAllocatedPort == NULL )
   {
      puLastAllocatedPort = &mPtInfo.uLastAllocatedPort[0];
   }

#if CCTKCFG_RANDOM_PORTS
   /* Try to allocate ports randomly */
   BOS_BOOL bAllocateRandomPorts = BOS_TRUE;
#endif

   uPort = uMinPort;
   while ( uPort <= uMaxPort )
   {
      /* Actual port used with offset */
      if ( uMinPort != uMaxPort )
      {
#if CCTKCFG_RANDOM_PORTS
         if( bAllocateRandomPorts )
         {
            /* Random port allocation */
            uAllocatedPort = static_cast<BOS_UINT16>( rand() );
            uAllocatedPort = (uAllocatedPort % (uMaxPort - uMinPort)) + uMinPort;
         }
         else
#endif /* CCTKCFG_RANDOM_PORTS */
         {
            /* Sequential port allocation */
            uAllocatedPort = uMinPort +
                             (( uPort - uMinPort + uOffset ) %
                              ( uMaxPort - uMinPort + 1 ));
         }
      }
      else
      {
         /* Special case when uMinPort and uMaxPort are equal */
         uAllocatedPort = uMinPort;
      }

      /* Find a matched port on the call list */
      BOS_BOOL bConflictFound = BOS_FALSE;
      for ( i = 0; i < CCTKCFG_MAX_CALL; i++ )
      {
         if ( mPtInfo.ptInfo[i].uCId != 0xFFFFFFFF )
         {
            int j;
            for( j = 0; j < eMSEPTTYPE_MAX; j++ )
            {
               if( mPtInfo.ptInfo[i].uPort[j] == uAllocatedPort )
               {
                  bConflictFound = BOS_TRUE;
                  break;
               }
            }
            if( bConflictFound )
               break;
         }
      }

      /* No conflict found */
      if ( !bConflictFound )
      {
         break;
      }

      uPort = uPort + uIncrement;

#if CCTKCFG_RANDOM_PORTS
      if( bAllocateRandomPorts )
      {
         /* If random allocation wasnt able to come up with a non conflicting port */
         if( uPort > uMaxPort || uPort < uMinPort )
         {
            /* Fall back to sequential allocation scheme */
            bAllocateRandomPorts = BOS_FALSE;
            uPort = uMinPort;
         }
      }
      else
#endif /* CCTKCFG_RANDOM_PORTS */
      {
         /* Overflow check, in case uMaxPort is set to 0xFFFF */
         if ( uPort < uMinPort )
         {
            /* Port exhausted - cannot allocate a port at this time */
            return 0;
         }
      }
   }

   if ( uPort > uMaxPort )
   {
      /* No ports available */
      uAllocatedPort = 0;
   }
   else
   {
      /* Update last allocated port */
      *puLastAllocatedPort = uAllocatedPort;
   }

#if 0
   CCTK_TRACED1( "****findFreePort - uAllocatedPort = %d", uAllocatedPort );
   for ( i = 0; i < eMSEPTTYPE_MAX * CCTKCFG_MAX_CALL; i++ )
   {
      if ( mPtInfo.uLastAllocatedPort[i] == 0 )
      {
         break;
      }
      CCTK_TRACED2( "****findFreePort - uLastAllocatedPort[%d] = %d", i,
                    mPtInfo.uLastAllocatedPort[i] );
   }
#endif

   return uAllocatedPort;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::freePorts
**
**  PURPOSE:    Free all allocated port for the Call Id.
**
**  PARAMETERS:
**              uCId - Call Id.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void MseSdpSession::freePorts( BOS_UINT32 uCId )
{
   BOS_UINT32 i;

   /* Find a matched call on the list */
   for ( i = 0; i < CCTKCFG_MAX_CALL; i++ )
   {
      if ( mPtInfo.ptInfo[i].uCId == uCId )
      {
         memset( &mPtInfo.ptInfo[i], 0, sizeof( MSECALLPTINFO ));
         mPtInfo.ptInfo[i].uCId = 0xFFFFFFFF;
         break;
      }
   }
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isWidebandCodec
**
**  PURPOSE:    Returns whether the specified codec is a wideband codec or not.
**
**  PARAMETERS:
**              eCodec - The codec of interest.
**
**
**  RETURNS:    BOS_TRUE if it is, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
BOS_BOOL MseSdpSession::isWidebandCodec( CCTKCODEC eCodec )
{
   BOS_BOOL bRet = BOS_FALSE;

   switch( eCodec )
   {
      case eCCTKCODEC_G722:
      case eCCTKCODEC_G722_48:
      case eCCTKCODEC_G722_56:
      case eCCTKCODEC_G722_64:
      case eCCTKCODEC_BV32:
      case eCCTKCODEC_G7221:
      case eCCTKCODEC_AMRWB:
      case eCCTKCODEC_LPCMWB:
      {
         bRet = BOS_TRUE;
         break;
      }

      default:
      break;
   }

   return bRet;
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::ceNotifyCb
**
**  PURPOSE:    Notifies CE asynchronously.
**
**  PARAMETERS:
**              uCId - Call id.
**              pData - CE event (typecasted).
**
**
**  RETURNS:    BOS_TRUE if it is, BOS_FALSE otherwise.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::ceNotifyCb( BOS_UINT32 uCId, void *pData )
{
   CEEVT eEvt = *reinterpret_cast<CEEVT *>( &pData );
   ceNotify( uCId, eEvt, NULL );   
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::tkmTimerCb
**
**  PURPOSE:    CCTK timer callback.  Called when the scheduled timer expires.
**              Currently, only used for SDP update retry due to glare error (both
**              sides refresh simultaneously).
**
**  PARAMETERS:
**              uId - Unique Id for the scheduled timer.
**              pData - Pointer to the MseSdpSession instance (typecasted).
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::tkmTimerCb( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( uId );

   MseSdpSession *pMseSdpSession = static_cast<MseSdpSession *>( pData );

   /* Initiate a SDP update again now that the timer has expires. */
   pMseSdpSession->mediaUpdate( eCCTKMUPDRSN_REFRESHSDP, NULL );
}

/***********************************************************************************
**  FUNCTION:   MseSdpSession::isSdpHoldAcceptable
**
**  PURPOSE:    Returns whether the remote SDP information is an acceptable HOLD
**              SDP.
**
**  PARAMETERS:
**              pCapsMgr - Pointer to the SDP caps.
**              bUseQuadZeroes - Indicates whether quad-zero method is used.
**              bUseInactive - Indicates whether a=inactive is used.
**
**
**  RETURNS:    BOS_TRUE if it acceptable, BOS_FALSE otherwise.
**
**  NOTES:      This method is primarily used due to possible interoperability
**              issue seen in the field where a 'HOLD' SDP offer is actually not
**              correctly responded to in some cases and may confuse MSE and the
**              rest of the application in believing that the HOLD succeeded when
**              it should not be the case.
***********************************************************************************/
BOS_BOOL MseSdpSession::isSdpHoldAcceptable( CSdpCapabilitiesMgr *pCapsMgr,
                                             BOS_BOOL bUseQuadZeroes,
                                             BOS_BOOL bUseInactive )
{
   BOS_UINT32 i, uNumStreams = pCapsMgr->GetNbStreams();
   BOS_BOOL bAcceptable = BOS_FALSE;

   /* If the connection address is NULL.  Note that we use the first 
   ** stream address.
   */
   if ( bUseInactive &&
        bUseQuadZeroes &&
        pCapsMgr->GetNbStreams() > 0 &&
        pCapsMgr->GetStream( 0 ).GetNbConnectionDatas() > 0 )
   {
      char *pcStreamName;
      CString streamAddr;

      pCapsMgr->GetStreamAddr( 0, streamAddr );
      pcStreamName = const_cast<char *>( streamAddr.CStr() );

      if ( !strcmp( pcStreamName, cQuadZerosStr ) )
      {
         bAcceptable = BOS_TRUE;
      }
   }

   /* If the stream mode attribute is inactive.
   */
   if ( bUseInactive )
   {
      for ( i = 0; i < uNumStreams; i++ )
      {
         /* Look at a= mode attribute in the audio stream */
         if ( pCapsMgr->GetStreamType( i ) == CSdpParser::eAUDIO &&
              ( pCapsMgr->GetStreamTransportProtocol( i ) ==
                 CSdpParser::eRTPAVP
#if CCTKCFG_SRTP
                || pCapsMgr->GetStreamTransportProtocol( i ) ==
                 CSdpParser::eRTPSAVP
#endif
              ))
         {
            CSdpLevelMedia &rStream = pCapsMgr->GetStream( i );

            /* Check whether the stream is set to inactive.
            */
            if ( rStream.IsInactive() )
            {
               bAcceptable = BOS_TRUE;
               break;
            }
         }
      }
   }

   /* No special checks.
   */
   if ( !bUseInactive )
   {
      bAcceptable = BOS_TRUE;
   }

   return bAcceptable;
}

#if CCTKCFG_REMOTE_USER_CONTROL
BOS_BOOL MseSdpSession::natUpdateProxyUsrSdp( CBlob &rProxySdp )
{

   BOS_BOOL result = BOS_FALSE;

   /* Check if there is RG NAT Control */
   if ( CFG_SYS->xtl.sec.nat.rg.ctl.pNatIsLanAddrFunc == NULL ||
      CFG_SYS->xtl.sec.nat.rg.ctl.pNatSesAddFunc == NULL )
   {
      return BOS_FALSE;
   }

   CSdpPacket sdpPacket;
   CSdpLevelSession sdpSes;
   CSdpFieldConnectionData sesConnectData;
   CSdpLevelMedia media;
   CSdpFieldConnectionData medConnectData;
   CSdpFieldMediaAnnouncement announcement;
   CSdpFieldAttributeRtcp rtcp;

   mxt_result res;
   const char* pSdpRaw = reinterpret_cast<const char*> ( rProxySdp.GetFirstIndexPtr() );
   sdpPacket.Parse( pSdpRaw, res );
   if ( res == resS_OK )
   {
      /* assume connection parameter in media level */
      BOS_BOOL bNatSesRes;
      BOS_BOOL bConnectSesLevel = BOS_FALSE;
      BOS_BOOL bConnectMedLevel = BOS_FALSE;

      BOS_IP_ADDRESS sesConnectAddr;
      BOS_IP_ADDRESS medConnectAddr;

      BOS_UINT16 uRtpPort;
      BOS_UINT16 uRtcpPort;
      int iProt;
      BOS_UINT32 uNatTimeout = 60;  // 60 sec for the remote user to pick up
      BOS_UINT32 uNatSesHandle;

      BOS_UINT16 uNatMappedRtpPort;
      BOS_UINT16 uNatMappedRtcpPort;
      BOS_IP_ADDRESS natMappedConnectAddr;

      BOS_IP_ADDRESS internalIpAdd;
      BOS_UINT16 uInternalPort;
      BOS_UINT16 uPortCnt;

      BOS_IP_ADDRESS remoteIpAdd;
      bosIpAddrCreateZero ( BOS_IPADDRESS_TYPE_V4, &remoteIpAdd );
      BOS_UINT16     uRemotePort = 0;

      sdpSes = sdpPacket.GetSession();
      if ( sdpSes.IsValidConnectionData() )
      {
         sesConnectData = sdpSes.GetConnectionData();
         bosIpAddrCreateFromStr( sesConnectData.GetAddress(), &sesConnectAddr );
         if ( CFG_SYS->xtl.sec.nat.rg.ctl.pNatIsLanAddrFunc ( &sesConnectAddr ) == BOS_TRUE )
         {
            bConnectSesLevel = BOS_TRUE;
         }
      }

      BOS_UINT32 uMediaCnt = sdpSes.GetNbMedias();
      BOS_UINT16 uIx;
      BOS_UINT16 uJx;

      for ( uIx = 0; uIx < uMediaCnt; uIx++ )
      {
         bConnectMedLevel = BOS_FALSE;
         media = sdpSes.GetMedia( uIx );
         announcement = media.GetMediaAnnouncement();
         uRtpPort = (BOS_UINT16) ( announcement.GetTransportPort() );

         CSdpParser::ETransportProtocol eMediaProt = announcement.GetTransportProtocolId();
         switch ( eMediaProt )
         {
            case CSdpParser::eTCP:
            case CSdpParser::eTCPTL:
            case CSdpParser::eSIPTCP:
            case CSdpParser::eSIPTLS:
               iProt = NATPROT_TCP;
               break;
            /* If it is not TCP, then it is UDP */
            default:
               iProt = NATPROT_UDP;
               break;
         }

         /* get RTCP port if there is one */
         rtcp = media.GetSdpFieldAttributeRtcp();
         uRtcpPort = (BOS_UINT16) ( rtcp.GetPort() );

         BOS_UINT32 uMediaConnectCnt = media.GetNbExplicitConnectionDatas();
         uJx = 0;
         if ( uMediaConnectCnt != 0 )
         {
            
            /* only remap the first LAN address if there are multiple 
            ** connection datas */
            for ( uJx = 0; uJx < uMediaConnectCnt; uJx++ )
            {
               medConnectData = media.GetExplicitConnectionData(uJx);
               bosIpAddrCreateFromStr( medConnectData.GetAddress(), &medConnectAddr );
               if ( CFG_SYS->xtl.sec.nat.rg.ctl.pNatIsLanAddrFunc ( &medConnectAddr ) == BOS_TRUE )
               {
                  bConnectMedLevel = BOS_TRUE;
                  break;
               }
            }
         }

         /* remap now */
         if ( ( bConnectMedLevel && uJx < uMediaConnectCnt ) || 
              ( bConnectSesLevel && uMediaConnectCnt == 0 ) )
         {
            result = BOS_TRUE;
            if ( bConnectMedLevel && uJx < uMediaConnectCnt ) 
            {
               bosIpAddrCreateCopy( &medConnectAddr, &internalIpAdd );
            }
            else
            {
               bosIpAddrCreateCopy( &sesConnectAddr, &internalIpAdd );
            }

            uInternalPort = uRtpPort;
            uPortCnt = ( uRtcpPort == (BOS_UINT16)(-1) )? 2: 1;

            bNatSesRes = CFG_SYS->xtl.sec.nat.rg.ctl.pNatSesAddFunc( 
               &internalIpAdd, /* (IN) LAN client address */
               uInternalPort, /* (IN) LAN client starting port */
               uPortCnt,      /* (IN) number of continuous port needed */   
               &remoteIpAdd,   /* (IN) Remote party address (Optional) */
               uRemotePort,   /* (IN) Remote party starting port (Optional) */
               iProt,         /* (IN) protocol */
               uNatTimeout,   /* (IN) value for inactive timeout that frees the mapping */
               &natMappedConnectAddr, /* (OUT) WAN mapped address */
               &uNatMappedRtpPort,    /* (OUT) WAN mapped starting port */
               &uNatSesHandle         /* (OUT) Handle assigned for this new mapping */
            );
            if ( bNatSesRes )
            {
               struct MSENATSESHDL *pNatSesHdl = BRCM_NEW ( struct MSENATSESHDL );
               pNatSesHdl->uNatSesHdl = uNatSesHandle;
               pNatSesHdl->pNext = mpNatSesHdl;
               mpNatSesHdl = pNatSesHdl;
            }


            if ( uRtcpPort != (BOS_UINT16)(-1) ) 
            {
               bNatSesRes = CFG_SYS->xtl.sec.nat.rg.ctl.pNatSesAddFunc( 
                  &internalIpAdd, /* (IN) LAN client address */
                  uInternalPort, /* (IN) LAN client starting port */
                  uPortCnt,      /* (IN) number of continuous port needed */   
                  &remoteIpAdd,   /* (IN) Remote party address (Optional) */
                  uRemotePort,   /* (IN) Remote party starting port (Optional) */
                  iProt,         /* (IN) protocol */
                  uNatTimeout,   /* (IN) value for inactive timeout that frees the mapping */
                  &natMappedConnectAddr, /* (OUT) WAN mapped address */
                  &uNatMappedRtcpPort,   /* (OUT) WAN mapped starting port */
                  &uNatSesHandle         /* (OUT) Handle assigned for this new mapping */
               );
               if ( bNatSesRes )
               {
                  struct MSENATSESHDL *pNatSesHdl = BRCM_NEW ( struct MSENATSESHDL );
                  pNatSesHdl->uNatSesHdl = uNatSesHandle;
                  pNatSesHdl->pNext = mpNatSesHdl;
                  mpNatSesHdl = pNatSesHdl;
               }               
            }
            /* update up the SDP media connection address */
            if ( bConnectMedLevel && uJx < uMediaConnectCnt ) 
            {
               char natMappedAddrStr [BOS_IP_ADDRESS_MAX_STR_LEN];
               bosIpAddrGetStr( &natMappedConnectAddr, 
                  natMappedAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN );
               medConnectData.SetAddress( natMappedAddrStr );
            }
            /* update up the SDP media rtp port */
            announcement.SetTransportPort( uNatMappedRtpPort );
            media.SetMediaAnnouncement( announcement );

            /* update up the SDP media rtcp port */
            if ( uRtcpPort != (BOS_UINT16)(-1) ) 
            {
               rtcp.SetPort( uNatMappedRtcpPort );
               media.SetSdpFieldAttributeRtcp( rtcp );
            }
            sdpSes.GetMedia( uIx ) = media;
         }
      }
      /* update up the SDP session connection address */
      if ( bConnectSesLevel ) 
      {
         char natMappedAddrStr [BOS_IP_ADDRESS_MAX_STR_LEN];
         bosIpAddrGetStr( &natMappedConnectAddr, 
            natMappedAddrStr, BOS_IP_ADDRESS_MAX_STR_LEN );
         sesConnectData.SetAddress( natMappedAddrStr );
         sdpSes.SetConnectionData ( sesConnectData );
      }
      sdpPacket.SetSession ( sdpSes );
   }
   
   /* re-write the new SDP back to the buffer if there */
   if ( result )
   {
      rProxySdp.EraseAll();
      sdpPacket.Serialize( rProxySdp );
   }
   return result;
}

#endif

/***********************************************************************************
**  FUNCTION:   MseSdpSession::constructAmrFmtp
**
**  PURPOSE:    Create an M5T fmtp attribute with the specified AMR settings
**
**  PARAMETERS:
**              fmtpAmr - The fmtp attribute as a reference
**              pAmrCfg - Pointer to the AMR config block
**              bWideband - Whether this is a wideband or narrowband codec
**
**
**  RETURNS:    Nothing
**
**  NOTES:      .
***********************************************************************************/
void MseSdpSession::constructAmrFmtp( CSdpFieldAttributeFmtp &fmtpAmr,
                                      CCTKAMRPARM* pAmrCfg,
                                      BOS_BOOL bWideband )
{
   char cParmStr[256];
   int i;

   memset(cParmStr, 0, sizeof(cParmStr));

   if ( pAmrCfg->uModeSet )
   {
      BOS_UINT16 uMask;
      BOS_UINT16 uMaxMode = (bWideband)
         ? CCTK_AMR_WB_MAX_MODE
         : CCTK_AMR_NB_MAX_MODE;
      BOS_BOOL bFirst = BOS_TRUE;

      /* Insert 'mode-set' attribute */
      sprintf( &cParmStr[0], "%s=", cAmrModeSet );

      for ( i = 0 ; i < uMaxMode ; i++ )
      {
         uMask = 1 << static_cast<BOS_UINT16> (i);
         if ( pAmrCfg->uModeSet & uMask )
         {
            if ( !bFirst )
            {
               sprintf( &cParmStr[ strlen(cParmStr) ], "," );
            }
            bFirst = BOS_FALSE;
            sprintf( &cParmStr[ strlen(cParmStr) ],
                  "%d",
                  static_cast<int> (i) );
         }
      }
   }

   if ( pAmrCfg->bOctetAlign != eCCTKAMR_NOTSET )
   {
      if ( strlen(cParmStr) )
      {
         sprintf(&cParmStr[ strlen(cParmStr) ], "; ");
      }
      /* Insert 'octet-aligned' attribute */
      sprintf( &cParmStr[ strlen(cParmStr) ], "%s=%d",
            cAmrOctetAlign,
            (pAmrCfg->bOctetAlign == eCCTKAMR_YES ? 1 : 0) );
   }

   if ( pAmrCfg->bModeChangeNeighbor != eCCTKAMR_NOTSET )
   {
      if( strlen(cParmStr) )
      {
         sprintf(&cParmStr[ strlen(cParmStr) ], "; ");
      }
      /* Insert 'mode-change-neighbor' attribute */
      sprintf( &cParmStr[ strlen(cParmStr) ], "%s=%d",
            cAmrModeChangeNeighbor,
            (pAmrCfg->bModeChangeNeighbor == eCCTKAMR_YES ? 1 : 0) );
   }

   if ( pAmrCfg->bModeChangePeriod != eCCTKAMR_NOTSET )
   {
      if( strlen(cParmStr) )
      {
         sprintf(&cParmStr[ strlen(cParmStr) ], "; ");
      }
      /* Insert 'mode-change-neighbor' attribute */
      sprintf( &cParmStr[ strlen(cParmStr) ], "%s=%d",
            cAmrModeChangePeriod,
            (pAmrCfg->bModeChangePeriod == eCCTKAMR_YES ? 2 : 1) );
   }

   if( strlen(cParmStr) )
   {
      sprintf(&cParmStr[ strlen(cParmStr) ], "; ");
   }
   /* Insert 'mode-change-capability' attribute always */
   sprintf( &cParmStr[ strlen(cParmStr) ], "%s=2", cAmrModeChangeCapability );

   if( pAmrCfg->uMaxRed )
   {
      sprintf(&cParmStr[ strlen(cParmStr) ], "; ");
      /* Insert 'maxptime' attribute */
      sprintf( &cParmStr[ strlen(cParmStr) ], "%s=%u", cAmrMaxRed, pAmrCfg->uMaxRed );
   }

   fmtpAmr.SetValue( cParmStr );
}

#if CCTKCFG_ICE_LITE_SUPPORT
void MseSdpSession::UpdateMediaInfoWithIceParm( MSEMEDIATYPE eMediaType,
   CCTKMEDINFO *pMedInfo, ICE_MEDIA_TRANSPORT* pTransport )
{
   int i;
   int j;
   int k;
   ICE_CNDT_PAIR *pRtpCndtPair = NULL;
   ICE_CNDT_PAIR *pRtcpCndtPair = NULL;
   BOS_BOOL bIPv6CndtPair;
   BOS_BOOL bIPv6PrefMedia;
   BOS_BOOL bUpdateMode;
   
   dumpMediaTransport(pTransport);
   
   bUpdateMode = ( pMedInfo->iceInfo.eCctkEvt == eCCTKEVT_MEDIA );
   
   /* clear all the old information first */
   memset(&pMedInfo->iceInfo, 0, sizeof( CCTKICEINFO ));   
   
   /* Update the CCTKMEDINFO icelite parameters */
   CCTKICEMEDIAINFO *pIceMedInfo;   
   if ( eMediaType == eMSEMEDIATYPE_AUDIO )
   {
      pIceMedInfo = &pMedInfo->iceInfo.audio;
   }
#if CCTKCFG_VIDEO
   else if ( eMediaType == eMSEMEDIATYPE_VIDEO )
   {
      pIceMedInfo = &pMedInfo->iceInfo.video;
   }
#endif
   else if ( eMediaType == eMSEMEDIATYPE_T38 )
   {
      pIceMedInfo = &pMedInfo->iceInfo.t38;
   }
   pIceMedInfo->bIceTsNotReady = BOS_FALSE;

   for ( i = 0, j = 0; i < pTransport->num && j < CCTKCFG_MAX_TRANSPORT_ADDRESS; i++ )
   {
      if (pTransport->cndtPair[i].compId == ICE_COMP_RTP)  
      {
         pRtpCndtPair = &pTransport->cndtPair[i];
         pRtcpCndtPair = NULL;
         /* look for the matching RTCP component */
         for ( k = 0; k < pTransport->num; k++ )
         {  
            if ( (pTransport->cndtPair[k].compId == ICE_COMP_RTCP) &&
                  bosIpAddrIsEqual(&pTransport->cndtPair[k].local.ipAddr,
                     &pTransport->cndtPair[i].local.ipAddr) )
            {
               pRtcpCndtPair = &pTransport->cndtPair[k];
               break;
            }
         }
         
         bosIpAddrCreateCopy( &(pRtpCndtPair->local.ipAddr),
            &(pIceMedInfo->transportAddr[j].addrLcl) );
         pIceMedInfo->transportAddr[j].rtpPortLcl = pRtpCndtPair->local.port;
         pIceMedInfo->transportAddr[j].rtcpPortLcl = pRtcpCndtPair? 
            pRtcpCndtPair->local.port: 0;
         
         bosIpAddrCreateCopy( &(pRtpCndtPair->remote.ipAddr),
            &(pIceMedInfo->transportAddr[j].addrRmt) );
         pIceMedInfo->transportAddr[j].rtpPortRmt = pRtpCndtPair->remote.port;
         pIceMedInfo->transportAddr[j].rtcpPortRmt = pRtcpCndtPair? 
            pRtcpCndtPair->remote.port: 0;
         j++;
      }
   }
   pIceMedInfo->uNumTsAddr = j;
   
   /* Update the CCTKMEDINFO default */
   pRtpCndtPair = NULL;
   pRtcpCndtPair = NULL;
   bIPv6PrefMedia = CFG_SYS->xtl.net.prot.gen.ice.bIPv6PrefMedia;
   
   /* If there is more than 1 candidate pairs, locate the first pair matching 
   ** the Pref Media IP type for each component */
   for ( i = 0; i < pTransport->num; i++ )
   {
      bIPv6CndtPair = bosIpAddrIsV6( &pTransport->cndtPair[i].local.ipAddr );
      if ( (pRtpCndtPair == NULL) &&
           (pTransport->cndtPair[i].compId == ICE_COMP_RTP) &&
           (!pTransport->bNonUnique || (bIPv6CndtPair == bIPv6PrefMedia)) )
      {
         pRtpCndtPair = &pTransport->cndtPair[i];
      }
      if ( (pRtcpCndtPair == NULL) && 
           (pTransport->cndtPair[i].compId == ICE_COMP_RTCP) &&
           (!pTransport->bNonUnique || (bIPv6CndtPair == bIPv6PrefMedia)) )
      {
         pRtcpCndtPair = &pTransport->cndtPair[i];
      }
   }

   if ( pRtpCndtPair )
   {
      /* copy the new destination address */
      bosIpAddrCreateCopy( &(pRtpCndtPair->remote.ipAddr), &(pMedInfo->ingress.addr) );
      /* copy the new destination port */
      if ( eMediaType == eMSEMEDIATYPE_AUDIO )
      {
         pMedInfo->ingress.audio.codec[0].uRtpPort = pRtpCndtPair->remote.port;
         if ( pRtcpCndtPair )
         {
            pMedInfo->ingress.audio.codec[0].uRtcpPort = pRtcpCndtPair->remote.port;
         }
         else
         {
            pMedInfo->ingress.audio.codec[0].uRtcpPort = 0;
         }
      }
      else if ( eMediaType == eMSEMEDIATYPE_VIDEO )
      {
         pMedInfo->ingress.video.codec[0].uRtpPort = pRtpCndtPair->remote.port;
         if ( pRtcpCndtPair )
         {
            pMedInfo->ingress.video.codec[0].uRtcpPort = pRtcpCndtPair->remote.port;
         }
         else
         {
            pMedInfo->ingress.video.codec[0].uRtcpPort = 0;
         }
      }
      else if ( eMediaType == eMSEMEDIATYPE_T38 )
      {
         pMedInfo->ingress.t38.uPort = pRtpCndtPair->remote.port;
      }
      
      /* copy the new source address */
      bosIpAddrCreateCopy( &(pRtpCndtPair->local.ipAddr), &(pMedInfo->egress.addr) );
      /* copy the new source port */
      if ( eMediaType == eMSEMEDIATYPE_AUDIO )
      {
         pMedInfo->egress.audio.codec[0].uRtpPort = pRtpCndtPair->local.port;
         if ( pRtcpCndtPair )
         {
            pMedInfo->egress.audio.codec[0].uRtcpPort = pRtcpCndtPair->local.port;
         }
         else
         {
            pMedInfo->egress.audio.codec[0].uRtcpPort = 0;
         }
      }
      else if ( eMediaType == eMSEMEDIATYPE_VIDEO )
      {
         pMedInfo->egress.video.codec[0].uRtpPort = pRtpCndtPair->local.port;
         if ( pRtcpCndtPair )
         {
            pMedInfo->egress.video.codec[0].uRtcpPort = pRtcpCndtPair->local.port;
         }
         else
         {
            pMedInfo->egress.video.codec[0].uRtcpPort = 0;
         }            
      }
      else if ( eMediaType == eMSEMEDIATYPE_T38 )
      {
         pMedInfo->egress.t38.uPort = pRtpCndtPair->local.port;
      }
   }

   /* put the ingress mode to active, so DSP generates RTP */
   if ( eMediaType == eMSEMEDIATYPE_AUDIO)
   {
      if ( bUpdateMode )
      {
         pMedInfo->ingress.audio.mode = eCCTKMEDMODE_ACTIVE;
      }
   }            
#if CCTKCFG_VIDEO
   else if ( eMediaType == eMSEMEDIATYPE_VIDEO)
   {
      if ( bUpdateMode )
      {
         pMedInfo->ingress.video.mode = eCCTKMEDMODE_ACTIVE;
      }      
   }
#endif
   else if ( eMediaType == eMSEMEDIATYPE_T38)
   {
      pMedInfo->iceInfo.t38.bIceTsNotReady = BOS_FALSE;
   }
}

void MseSdpSession::ICEReOffer( void )
{
   /* trigger an Re-INVITE or UPDATE */
   fsmProcess( eMSEEVT_CE_MEDIA, NULL );
}

void MseSdpSession::ICEReject( void )
{
   /* ICE indicates that peer is unreachable. */
   /* End this call.  Trigger an CANCAL if needed. */
   mpMgr->EvTerminalFailure();
}

/* static callback function */
void MseSdpSession::NotifyIceConcludedParameter( BOS_UINT32 evt, 
   void* pTransportData )
{
   switch ( evt )
   {
      case ICE_EVT_CONCLUDED_FAILURE:
      {
         MSEICEMEDIATRANSPORT *pIceTransport = (MSEICEMEDIATRANSPORT*) pTransportData;
         MseSdpSession * pMse = pIceTransport->pMse;
         MSEMEDIATYPE eMediaType = pIceTransport->eMediaType;
         BOS_BOOL bIsEarlySession = pIceTransport->bEarlySession;   
         CCTK_TRACEI1( "**** Notify CE about Media FAILURE - ICE indicates that no candidate pair can be found. (%x)", pMse );
         pMse->ICEReject();
      }
      break;

      case ICE_EVT_CONCLUDED_SUCCESS:
      case ICE_EVT_REOFFER:
      {
         MSEICEMEDIATRANSPORT *pIceTransport = (MSEICEMEDIATRANSPORT*) pTransportData;
         ICE_MEDIA_TRANSPORT* pTransport = pIceTransport->pMediaTransport;
         MseSdpSession * pMse = pIceTransport->pMse;
         MSEMEDIATYPE eMediaType = pIceTransport->eMediaType;
         BOS_BOOL bIsEarlySession = pIceTransport->bEarlySession;

         CCTKMEDINFO *pMedInfo;
         if ( pMse )
         {
            CCTK_TRACEI1( "**** Notify CE about Media Change - ICE completed. (%x)", pMse  );
            pMedInfo = bIsEarlySession? pMse->mpEarlyMedInfo: pMse->mpMedInfo;
            if ( pMedInfo )
            {
               CCTKEVT event = pMedInfo->iceInfo.eCctkEvt;
               pMse->UpdateMediaInfoWithIceParm ( eMediaType, pMedInfo, pTransport ); 
               ceNotify( pMse->muCid, static_cast<CEEVT>( event ), pMedInfo );
            }
            else
            {
               CCTK_TRACEI0( "**** Notify CE about Media Change - ICE completed -- But pMedInfo is NULL!!!!!" );      
            }
            
            if ( ICE_EVT_REOFFER == evt )
            {
               /* ICE module requests for a re-OFFER */
               pMse->ICEReOffer();
            }
         }
         BRCM_DELETE( pTransport );
         BRCM_DELETE( pIceTransport );         
      }
   }
}

BOS_BOOL MseSdpSession::ICENegotiation ( CSdpCapabilitiesMgr *pRemCapsMgr,
   CSdpCapabilitiesMgr *pLocCapsMgr, BOS_BOOL bEarlySession )
{
   BOS_BOOL bRes = BOS_TRUE;
   unsigned int uIdx;   
   unsigned int uIdx2;   
   BOS_UINT32 uIceStreamIndex;      
   MSEICEAGENTHANDLE iceAgentHdl = 0;
   MSEICEAGENTHANDLE iceAgentHdlFirstStream = 0;
   MSEMEDIATYPE eMseMedType;
   MSEICESTATUS iceRes;
   BOS_IP_ADDRESS lclDftCnxAddr;
   
   BOS_UINT32 uAudioStreamIndex = 0;
   BOS_UINT32 uT38StreamIndex = 0;
   BOS_UINT32 uVideoStreamIndex = 0; 
   
   CCTK_ASSERT ( pLocCapsMgr != NULL );

   /* Parse ICE attributes from peer SDP */
   if ( pRemCapsMgr )
   {
      BOS_UINT32 uIceStreamIndex;          
      
      for ( uIdx = 0; uIdx < pRemCapsMgr->GetNbStreams(); uIdx++ )
      {
         CSdpLevelMedia &rStream = pRemCapsMgr->GetStream( uIdx );
         CSdpParser::EMediaType eMedType = rStream.GetMediaAnnouncement().
            GetMediaTypeId();
         switch ( eMedType )
         {
            case CSdpParser::eIMAGE:
            {
               eMseMedType = eMSEMEDIATYPE_T38;
               uIceStreamIndex = uT38StreamIndex;
               uT38StreamIndex++;
               break;
            }                  
            case CSdpParser::eVIDEO:
            {
               eMseMedType = eMSEMEDIATYPE_VIDEO;
               uIceStreamIndex = uVideoStreamIndex;
               uVideoStreamIndex++;                        
               break;
            }
            case CSdpParser::eAUDIO:
            default:
            {
               eMseMedType = eMSEMEDIATYPE_AUDIO;
               uIceStreamIndex = uAudioStreamIndex;
               uAudioStreamIndex++;                               
               break;
            }
         }
         /* If this is an early media, create ICE agent for
          * early media stream */         
         uIceStreamIndex |= (bEarlySession? MSEEMSTRMINDXMSK : 0);         
         iceAgentHdl = nsIceMseAddAgent( this, 
            uIceStreamIndex,
            bEarlySession,
            eMseMedType );

         meIceRmtMode = nsIceDecodeRemoteIceParams( mbIsCaller,
            rStream, iceAgentHdl );
      }            
   }      
   
   uAudioStreamIndex = 0;
   uT38StreamIndex = 0;
   uVideoStreamIndex = 0;    
   /* Encode ICE attributes for local SDP */
   for ( uIdx = 0; uIdx < pLocCapsMgr->GetNbStreams(); uIdx++ )   
   {
      CSdpLevelMedia &rStream = pLocCapsMgr->GetStream( uIdx );

      CSdpParser::EMediaType eMedType = rStream.GetMediaAnnouncement().
         GetMediaTypeId();
      switch ( eMedType )
      {
         case CSdpParser::eIMAGE:
         {
            eMseMedType = eMSEMEDIATYPE_T38;
            uIceStreamIndex = uT38StreamIndex;
            uT38StreamIndex++;
            break;
         }                  
         case CSdpParser::eVIDEO:
         {
            eMseMedType = eMSEMEDIATYPE_VIDEO;
            uIceStreamIndex = uVideoStreamIndex;
            uVideoStreamIndex++;                        
            break;
         }
         case CSdpParser::eAUDIO:
         default:
         {
            eMseMedType = eMSEMEDIATYPE_AUDIO;
            uIceStreamIndex = uAudioStreamIndex;
            uAudioStreamIndex++;                               
            break;
         }
      }
      /* If this is an early media, create ICE agent for
       * early media stream */         
      uIceStreamIndex |= (bEarlySession? MSEEMSTRMINDXMSK : 0);
      
      iceAgentHdl = nsIceMseAddAgent( this, 
         uIceStreamIndex,
         bEarlySession,
         eMseMedType );
      
      if ( iceAgentHdlFirstStream == 0 )
      {
         iceAgentHdlFirstStream = iceAgentHdl;
      }

      nsIceEncodeLocalIceParams(mbIsCaller, rStream, iceAgentHdl, mStrIceUfrag, mStrIcePwd);
   }
      
   /* Check if Peer is reachable.
   ** If it is, update the default connection address on the local/answer SDP. */
   if ( iceAgentHdlFirstStream )
   {
      /* Get the ICE result from the found ICE agent */
      iceRes = nsIceGetResult( iceAgentHdlFirstStream, &lclDftCnxAddr); 
      
      switch (iceRes)
      {
         case eMSEICESTATUS_FAIL:
         {
            /* Not reachable. */
            bRes = BOS_FALSE;
         }
         break;
         case eMSEICESTATUS_IN_PROGRESS:            
         case eMSEICESTATUS_CONCLUDED:
         default:
         break;
      }
   }

   if ( bRes == BOS_TRUE )
   {
      /* The ICE operation is IN_PROGRESS or CONCLUDED.  
      ** Add ICE parameters to the local/answer SDP */ 
   
      /* Always encode the ice-lite attributes into our SDP, even when remote peer
         does not support ICE. */
      /* Add ICE-lite parameter if we are the caller, or remote has ICE support */
      CCTK_TRACEI1( "MseSdpSession (%p) - ICENegotiation() - adding ICE-LITE attribute.", this );
      
      /* set ICE-LITE */
      pLocCapsMgr->GetSdpSession()->SetIceLite(BOS_TRUE);
   
      /* set Session level ICE Frag and Pwd from the first media */
      if ( pLocCapsMgr->GetNbStreams() > 0 )
      {
         CSdpLevelMedia &rStream = pLocCapsMgr->GetStream( 0 );  
         
         pLocCapsMgr->GetSdpSession()->GetIceUserFragment().SetUserFrag( 
            rStream.GetIceUserFragment().GetUserFrag() );
         pLocCapsMgr->GetSdpSession()->GetIcePassword().SetPassword(
            rStream.GetIcePassword().GetPassword() );
         
         /* Remove the ICE Frag and Pwd in the media level if they are same as ones in
         ** session level */
         for ( uIdx = 0; uIdx < pLocCapsMgr->GetNbStreams(); uIdx++ )           
         {
            rStream = pLocCapsMgr->GetStream( uIdx );
            if ( pLocCapsMgr->GetSdpSession()->GetIceUserFragment() == rStream.GetIceUserFragment() &&
               pLocCapsMgr->GetSdpSession()->GetIcePassword() == rStream.GetIcePassword() )
            {
               rStream.GetIceUserFragment().Reset();
               rStream.GetIcePassword().Reset();
            }
         }
      }

      /* validate the ICE attributes, such IsIceAttributePresent flag is set  */
      pLocCapsMgr->GetSdpSession()->Validate();
   }

   return bRes;
}
         
#endif /* CCTKCFG_ICE_LITE_SUPPORT */


MX_NAMESPACE_END( MXD_GNS )
