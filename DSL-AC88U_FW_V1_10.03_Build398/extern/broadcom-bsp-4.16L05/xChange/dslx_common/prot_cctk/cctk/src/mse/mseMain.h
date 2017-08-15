/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom
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
**      This file defines the MSE SDP Session Class which realizes the
**      M5T IMspOfferAnswerSession interface.
**
***********************************************************************************/
#ifndef __MSESDPSESSION__H__INCLUDED__
#define __MSESDPSESSION__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

#if CCTKCFG_ICE_LITE_SUPPORT
#include <ice.h>
#include <nsIceApi.h>
#endif /* CCTKCFG_ICE_LITE_SUPPORT */



MX_NAMESPACE_START( MXD_GNS )

/* ---- Constants and Types ----------------------------------------------------- */

/* This structure contains the CCTK-M5T codec mapping */
typedef struct MSECODECMAP
{
   CCTKCODEC      eCodec;           /* CCTK codec type */
   CSdpParser::ERtpCompressionAlgorithm
                  eMxCodec;         /* M5T codec type */
   BOS_UINT32     uRate;            /* Clock rate in Hz (e.g. 8000 Hz) */
   BOS_UINT32     uPktSize;         /* Packet size per uFrameBase ms "frame" */
   BOS_UINT32     uFrameBase;       /* Base "frame" rate */

} MSECODECMAP;

#if CCTKCFG_SRTP
typedef struct CCTKCRYPTOMAP
{
   CCTKCSUITE       eCryptoSuite;      /* Crypto suite */
   const BOS_UINT8  *pText;            /* Crypto suite text */
} CCCRYPTOMAP;

typedef struct CCTKKEYLENMAP
{
   CCTKCSUITE  eCryptoSuite;           /* Crypto suite */
   BOS_UINT32  uLength;                /* Length of Key and Salt combined */
} CCKEYLENMAP;
#endif

/* ---- Variable Externs -------------------------------------------------------- */
MX_DECLARE_ECOM_CLSID(MseSdpSession);

/* ---- Function Prototypes ----------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */

class MseSdpSession : private CEComDelegatingUnknown,
                      public IMspOfferAnswerSession
{
public:
   /* Early media flow, for P-Early-Media (RFC 5009) support */
   enum MSEEMFLOW
   {
      eMSEEMFLOW_SENDRECV,    /* Authorized early media flow for both directions
                              ** (default) */
      eMSEEMFLOW_SENDONLY,    /* Authorized early media flow for ingress only */
      eMSEEMFLOW_RECVONLY,    /* Authorized early media flow for egress only */
      eMSEEMFLOW_INACTIVE     /* No authorized early media flow */
   };

   /* Constructor / Destructor */
   MseSdpSession();
   MseSdpSession( const MseSdpSession &rSource );
   ~MseSdpSession();

   /* Initialization mechanism */
   static mxt_result initializeMseSdpSession();
   static void finalizeMseSdpSession();

   /* MSE-CE interface */
   void setCid( BOS_UINT32 uCid );
   void mediaUpdate( CCTKMUPDRSN eReason, void *pParm );
   void hold( BOS_BOOL bEmergency = BOS_FALSE );
   void unhold();
   const CCTKMEDINFO *getMediaInfo();
   BOS_BOOL isPreconditionRequired();
   void answerEmergencyCall();
   void rejectEmergencyCall();
   void setEarlyMediaFlow( MSEEMFLOW eEarlyMediaFlow );
#if CCTKCFG_REMOTE_USER_CONTROL
   void setOfferSdp( CBlob *pBlob );
   void setAnswerSdp( CBlob *pBlob );
#endif
   const MSECODECMAP *getCodecMap( CCTKCODEC eCodec );

   /* IMspOfferAnswerSession (MSE-UASSP) interface */
   virtual void SetMspOfferAnswerSessionManager( IMspOfferAnswerSessionMgr *pMgr );
   virtual void SetUserAgentMode( EUserAgentMode eMode );
   virtual void GetSipCapabilities( uint32_t &ruSupported,
                                    uint32_t &ruRequire );
   virtual void SetPeerSipCapabilities( const uint32_t uSupported,
                                        const uint32_t uRequire );
   virtual void PrepareOffer( bool bIsMediaSessionRefresh );
   virtual void CancelOffer();
   virtual void GlareError( uint32_t uRetryAfterMs );
   virtual void HandleSdp( const CBlob *pSdpSession,
                           const CBlob *pSdpEarlySession,
                           ESdpType eSessionType,
                           ESdpType eEarlySessionType );
   virtual void RemoteUserNotified();
   virtual void RemoteUserAccepted();
   virtual void LocalUserAccepted();
   virtual void Fork( IMspOfferAnswerSession **ppForkedOfferAnswerSession );
   virtual void ResetNegotiation();
   virtual void PreTerminateSession();
   virtual void TerminateSession();

   /* CEComDelegatingUnknown */
   MX_DECLARE_DELEGATING_IECOMUNKNOWN

   /* Inherited from CEComDelegatingUnknown */
   virtual mxt_result NonDelegatingQueryIf( IN mxt_iid iidRequested,
                                            OUT void** ppInterface );

                                            
#if CCTKCFG_ICE_LITE_SUPPORT
   static void NotifyIceConcludedParameter( BOS_UINT32 evt, 
      void* pTransportData );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

private:
   /* Hold state */
   enum MSEHOLDST
   {
      eMSEHOLDST_NOT_HOLD,
      eMSEHOLDST_HOLD_PENDING,
      eMSEHOLDST_HOLD,
      eMSEHOLDST_UNHOLD_PENDING
   };

   /* MSE FSM events */
   enum MSEEVT
   {
      /* SCE events */
      eMSEEVT_SCE_PREPAREOFFER,
      eMSEEVT_SCE_CANCELOFFER,
      eMSEEVT_SCE_GLAREERROR,
      eMSEEVT_SCE_EARLYSDPOFFER,
      eMSEEVT_SCE_SDPOFFERLONG,
      eMSEEVT_SCE_SDPOFFERSHORT,
      eMSEEVT_SCE_SDPOFFERSHORTCANTFAIL,
      eMSEEVT_SCE_SDPANSWERPREVIEW,
      eMSEEVT_SCE_SDPANSWERFINAL,
      eMSEEVT_SCE_REMOTENOTIFIED,
      eMSEEVT_SCE_REMOTEACCEPTED,
      eMSEEVT_SCE_LOCALACCEPTED,
      eMSEEVT_SCE_RESETNEGOTIATION,
      eMSEEVT_SCE_PRETERMINATE,
      eMSEEVT_SCE_TERMINATE,

      /* CE events */
      eMSEEVT_CE_MEDIA_NOT_READY,  /* tell MSE, SDP is not ready, but send 180 anyway */
      eMSEEVT_CE_MEDIA,
      eMSEEVT_CE_HOLD,
      eMSEEVT_CE_UNHOLD,

      eMSEEVT_MAX
   };

   /* MSE FSM states */
   enum MSESTATE
   {
      eMSESTATE_PENDINGCAPS,     /* Waiting for caps approval (callee) */
      eMSESTATE_PENDINGOFFER,    /* Pending SDP offer (wait for caps approval) */
      eMSESTATE_PENDINGANSWER,   /* Pending SDP answer (wait for caps approval) */
      eMSESTATE_IDLE,            /* Idle */
      eMSESTATE_OFFERSENT,       /* Local SDP offer sent */
      eMSESTATE_MAX
   };

   /* MSE Port type */
   enum MSEPTTYPE
   {
      eMSEPTTYPE_AUD_START,
      eMSEPTTYPE_AUD_RTP = eMSEPTTYPE_AUD_START,
      eMSEPTTYPE_AUD_RTCP,
      eMSEPTTYPE_AUD_SRTP,
      eMSEPTTYPE_AUD_SRTCP,
      eMSEPTTYPE_AUD_END = eMSEPTTYPE_AUD_SRTCP,

      eMSEPTTYPE_VID_START,
      eMSEPTTYPE_VID_RTP = eMSEPTTYPE_VID_START,
      eMSEPTTYPE_VID_RTCP,
      eMSEPTTYPE_VID_SRTP,
      eMSEPTTYPE_VID_SRTCP,
      eMSEPTTYPE_VID_END = eMSEPTTYPE_VID_SRTCP,

      eMSEPTTYPE_T38,
      eMSEPTTYPE_MAX
   };

#if CCTKCFG_REMOTE_USER_CONTROL
   struct MSENATSESHDL
   {
      BOS_UINT32 uNatSesHdl;
      struct MSENATSESHDL *pNext;
   };
#endif
   
   struct MSECALLPTINFO
   {
      BOS_UINT32 uCId;
      BOS_UINT32 uUsrId;
      BOS_UINT16 uPort[eMSEPTTYPE_MAX];
   };
   
   struct MSEPTINFO
   {
      BOS_UINT16 uLastAllocatedPort[eMSEPTTYPE_MAX * CCTKCFG_MAX_CALL];
      MSECALLPTINFO ptInfo[CCTKCFG_MAX_CALL];
   };

   /* Helper functions */
   void addBandwidthAttrib( CSdpCapabilitiesMgr *pCapsMgr );
   void addBandwidthAttrib( CSdpLevelMedia &rStream,
                            BOS_UINT32 uMaxPrate,
                            BOS_UINT32 uBTias,
                            BOS_UINT32 uBAs );
   void addBandwidthAttrib( CSdpLevelMedia &rStream,
                            BOS_UINT32 uBRs,
                            BOS_UINT32 uBRr );
   BOS_UINT32 addCodec( CSdpCapabilitiesMgr *pCapsMgr,
                        BOS_UINT32 uStreamIdx,
                        CCTKCODEC eCodec,
                        BOS_UINT8 uRtpCode );
   void addCodec( CSdpCapabilitiesMgr *pCapsMgr,
                  BOS_UINT32 uStreamIdx,
                  CSdpParser::ERtpCompressionAlgorithm eMxCodec,
                  BOS_UINT32 uRate,
                  BOS_UINT8 uRtpCode );
#if CCTKCFG_SRTP
   void addCryptoAttrib( CSdpLevelMedia &rStream,
                         CCTKCRYPTOLST *pCryptoLst );
#endif
   void createLocalSdp( CSdpCapabilitiesMgr *pCapsMgr,
                        BOS_BOOL bIsOffer,
                        BOS_BOOL bEarlySession );
   void createLocalAudioSdp( CSdpCapabilitiesMgr *pCapsMgr,
                             CCTKMEDAUD *pAudio,
                             CCTKMEDLST *pMediaLst,
                             BOS_BOOL bIsOffer,
                             BOS_BOOL bSecured,
                             BOS_BOOL bEarlySession );
#if CCTKCFG_VIDEO
   void createLocalVideoSdp( CSdpCapabilitiesMgr *pCapsMgr,
                             CCTKMEDVID *pVideo,
                             CCTKMEDLST *pMediaLst,
                             BOS_BOOL bIsOffer,
                             BOS_BOOL bSecured,
                             BOS_BOOL bEarlySession );
#endif
   void createLocalT38Sdp( CSdpCapabilitiesMgr *pCapsMgr,
                           CCTKMEDFAX *pFax,
                           CCTKMEDLST *pMediaLst,
                           BOS_BOOL bEarlySession );
                           
   CBlob *createSdpBlob( CSdpCapabilitiesMgr *pCapsMgr,
                         BOS_BOOL bUseQuadZero = BOS_FALSE );
   void convertSdpFromMx( CSdpCapabilitiesMgr *pCapsMgr,
                          CCTKSTRMINFO *pStreamInfo,
                          BOS_BOOL bIngress,
                          BOS_BOOL bUnsecuredOnly);
   void convertIpFromMx( CSdpCapabilitiesMgr *pCapsMgr, BOS_IP_ADDRESS *pIpAddr );
   void filterSdp( CCTKSTRMINFO *pIn, CCTKSTRMINFO *pFilter, CCTKSTRMINFO *pOut );
   CCTKSILSUPP filterSilSuppSdp( CCTKSILSUPP eIn, CCTKSILSUPP eFilter );
   CCTKILBCMODE filterIlbcMode( CCTKILBCMODE eIn, CCTKILBCMODE eFilter );
   CCTKG7231VAD filterG7231Vad( BOS_BOOL bIn, CCTKG7231VAD eFilter );
   CCTKG729VAD filterG729Vad( BOS_BOOL bIn, CCTKG729VAD eFilter );
   void filterAmr( CCTKAMRPARM* pIn, CCTKAMRPARM* pOut, CCTKAMRPARM* pFilter );
#if CCTKCFG_RED
   void filterRED( CCTKREDPARM* pIn, CCTKREDPARM* pOut, CCTKREDPARM* pFilter );
#endif
#if CCTKCFG_SRTP
   const BOS_UINT8 *findCryptoText( const CCTKCRYPTOMAP *pMap,
                                    CCTKCSUITE eCryptoSuite );
   CCTKCSUITE findCryptoType( const CCTKCRYPTOMAP *pMap,
                              const BOS_UINT8 *pText );
   BOS_UINT32 findKeyLen( const CCTKKEYLENMAP *pMap,
                          const CCTKCSUITE eCryptoSuite );
#endif
   BOS_BOOL fsmProcess( MSEEVT eEvt, void *pParm );
#if CCTKCFG_SRTP
   void generateKeySalt( const CCTKCSUITE eCryptoSuite,
                         const BOS_UINT32 uKeySaltLength,
                         BOS_UINT8 *pKeySalt );
#endif
   const MSECODECMAP *getCodecMap( CSdpParser::ERtpCompressionAlgorithm eMxCodec,
                                   BOS_UINT32 uRate );
#if CCTKCFG_SRTP
   void getCrypto( CSdpLevelMedia &rStream, CCTKCRYPTOINFO *pCrypto );
#endif
   BOS_BOOL getLoopbackMode( CSdpLevelMedia &rStream,
                             BOS_BOOL &rbIsSrc,
                             CString &rLbFmt );
   BOS_UINT32 getLoopbackType( CSdpLevelMedia &rStream );
   BOS_UINT32 getPtimeFromMx( CSdpLevelMedia &rStream );
   BOS_UINT32 getMaxPtimeFromMx( CSdpLevelMedia &rStream );
   BOS_UINT32 getMptimeFromMx( CSdpLevelMedia &rStream, BOS_UINT32 uRtpIdx );
   CCTKSILSUPP getSilSuppFromMx( CSdpLevelMedia &rStream );
   void getAmrCfgFromMx( CSdpFieldAttributeFmtp* pFmtp,
                         CCTKAMRPARM* pAmrCfg,
                         BOS_BOOL bWideband );
   BOS_BOOL isMptimeAvailableMx( CSdpLevelMedia &rStream );
   BOS_BOOL isRtcpXrMetricsValidMx( CSdpLevelMedia &rStream );
   BOS_BOOL isSdpPrecondApproved( CSdpCapabilitiesMgr *pCapsMgr );
   BOS_BOOL isSdpSupported( CSdpCapabilitiesMgr *pAnsCapsMgr,
                            CSdpCapabilitiesMgr *pOffCapsMgr = NULL );
   BOS_BOOL isTagRequire( const CSipHeader *pHdr, const char* const pcOptionTag );
   BOS_BOOL isTagSupported( const CSipHeader *pHdr, const char* const pcOptionTag );
   BOS_BOOL static isRtcpType( MSEPTTYPE eType );
   BOS_BOOL isWidebandCodec( CCTKCODEC eCodec );
   BOS_BOOL isSdpHoldAcceptable( CSdpCapabilitiesMgr *pCapsMgr,
                                 BOS_BOOL bUseQuadZeroes,
                                 BOS_BOOL bUseInactive );
   void processAnswerSdp( CSdpCapabilitiesMgr *pAnsCapsMgr,
                          BOS_BOOL &bUseQuadZero,
                          CSdpCapabilitiesMgr *pOffCapsMgr,
                          CSdpCapabilitiesMgr *pLocCapsMgr );
   void sendSdpOffer();
   void setMediaMode( CSdpLevelMedia &rStream );
   void setSessionName( CSdpCapabilitiesMgr *pCapsMgr, BOS_UINT8 *pcSesName );
   void setSilSupp( CSdpLevelMedia &rStream, CCTKSILSUPP eSilSupp );
   void undoLocalCallState();
   void updateMediaMode( CSdpCapabilitiesMgr *pCapsMgr );
   BOS_BOOL updateRemHoldState( CSdpCapabilitiesMgr *pRemCapsMgr );
   BOS_UINT16 validateRemoteSdpT38( CSdpCapabilitiesMgr *pRemCapsMgr );
   BOS_UINT16 validateRemoteSdp( const CBlob &rReceivedSdp,
                                 CSdpCapabilitiesMgr *&rpRemoteSdp );
   static BOS_UINT16 allocatePort( BOS_UINT32 uCId, MSEPTTYPE eType );
   static BOS_UINT16 findFreePort( BOS_UINT32 uUsrId,
                                   MSEPTTYPE eType,
                                   BOS_BOOL bUseEveryOtherPort );
   static void freePorts( BOS_UINT32 uCId );
   static void ceNotifyCb( BOS_UINT32 uCId, void *pData );
   static void tkmTimerCb( BOS_UINT32 uId, void *pData );

#if CCTKCFG_REMOTE_USER_CONTROL
   BOS_BOOL natUpdateProxyUsrSdp( CBlob &rProxySdp );
#endif
   void constructAmrFmtp( CSdpFieldAttributeFmtp &fmtpAmr,
                          CCTKAMRPARM* pAmrCfg,
                          BOS_BOOL bWideband );
#if CCTKCFG_ICE_LITE_SUPPORT
   /* private helper function */
   void UpdateMediaInfoWithIceParm( MSEMEDIATYPE eMediaType,
      CCTKMEDINFO *pMedInfo, ICE_MEDIA_TRANSPORT* pTransports );
   void ICEReOffer( void );
   void ICEReject( void );

   BOS_BOOL ICENegotiation ( CSdpCapabilitiesMgr *pRemCapsMgr,
      CSdpCapabilitiesMgr *pLocCapsMgr,
      BOS_BOOL bEarlySession );
#endif /* CCTKCFG_ICE_LITE_SUPPORT */

   /* Inline functions */
   inline BOS_BOOL isHold( MSEHOLDST eHoldState )
   {
      return ( eHoldState == eMSEHOLDST_HOLD ||
               eHoldState == eMSEHOLDST_HOLD_PENDING ) ? BOS_TRUE : BOS_FALSE;
   };

   /* ECOM mechanism */
   static mxt_result createInstance( IEComUnknown *pOuterIEComUnknown,
                                     CEComUnknown **ppCEComUnknown );

private:
   BOS_BOOL mbIsCaller;                /* Indicates if we are the caller / callee */
   BOS_BOOL mbNoQuadZeros;             /* Use "Quad-Zero" (0.0.0.0) for hold when
                                       ** set to false
                                       */
   BOS_BOOL mbNotifyUser;              /* Indicates if we have notified user yet */
   BOS_BOOL mbEmergencyMode;           /* Emergency mode, see MseSdpSession::hold()
                                       ** for detailed description.
                                       */
   BOS_BOOL mb100RelUsed;              /* Indicates if reliable provisional response
                                       ** is used.
                                       */
   BOS_BOOL mbPrecondRequired;         /* Indicates if precondition is required by
                                       ** peer.
                                       */
   BOS_BOOL mbDlgEstablished;          /* Indicates if dialog is established */
   BOS_UINT32 muCid;                   /* CCTK reference id */
   BOS_UINT32 muSdpVer;                /* SDP version */
   MSEHOLDST meLocHoldSt;              /* Local hold state */
   MSEHOLDST meRemHoldSt;              /* Remote hold state */
   MSESTATE meState;                   /* Current SDP state */
   IMspOfferAnswerSessionMgr *mpMgr;   /* IMspOfferAnswerSessionMgr manager */
   CSdpCapabilitiesMgr *mpLocCapsMgr;  /* Cached local caps manager */
   CSdpCapabilitiesMgr *mpRemCapsMgr;  /* Cached remote caps manager */
   CCTKMEDINFO *mpEarlyMedInfo;        /* Cached early media info for the app */
   CCTKMEDINFO *mpMedInfo;             /* Cached media info for the app */
   CSdpPacket *mpPrevSdpPacket;        /* Cached previous SDP packet sent */
   CBlob *mpEarlySdpBlob;              /* Cached early media blob */
   CBlob *mpSdpSession;                /* Cached remote SDP blob (for delayed
                                       ** processing)
                                       */
#if CCTKCFG_REMOTE_USER_CONTROL
   CBlob *mpOffSdpBlob;                /* Offer blob to send */
   CBlob *mpAnsSdpBlob;                /* Answer blob to send */
   MSENATSESHDL *mpNatSesHdl;          /* NAT Session Handles list */
#endif
   MSEEMFLOW meEarlyMediaFlow;         /* Early media flow authorization */
   BOS_UINT32 muMedUpdateRetryTmr;     /* Media update retry timer */
   static MSEPTINFO mPtInfo;           /* Port allocation info */
   
#if CCTKCFG_ICE_LITE_SUPPORT
   MSEICEMODE meIceRmtMode;
   CString mStrIceUfrag;               /* ufrag for ICE */
   CString mStrIcePwd;                 /* pwd for ICE */   
#endif /* CCTKCFG_ICE_LITE_SUPPORT */
};

inline BOS_BOOL MseSdpSession::isRtcpType( MSEPTTYPE eType )
{
   return ( eType == eMSEPTTYPE_AUD_RTCP  || eType == eMSEPTTYPE_AUD_SRTCP ||
            eType == eMSEPTTYPE_VID_RTCP  || eType == eMSEPTTYPE_VID_SRTCP );
}

MX_NAMESPACE_END( MXD_GNS )

#endif /* __MSESDPSESSION__H__INCLUDED__ */

