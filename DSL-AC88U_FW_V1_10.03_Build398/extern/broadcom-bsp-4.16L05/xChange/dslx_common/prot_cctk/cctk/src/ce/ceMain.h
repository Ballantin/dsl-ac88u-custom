/**********************************************************************************
** Copyright (c) 2010-2013 Broadcom
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
**      This file defines the Call Engine main.
**
***********************************************************************************/
#ifndef __CE_MAIN__H__INCLUDED__
#define __CE_MAIN__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>
#include <ceResBase.h>
#include <ceResUser.h>
#include <ceResCall.h>
#include <ceResSub.h>
#include <ceResNot.h>
#include <ceFsmBase.h>
#include <ceCdrMgr.h>
#include <ceDlg.h>
#include <ceLoc.h>
#include <ceUap.h>


/* ---- Constants and Types ----------------------------------------------------- */
static const BOS_UINT32 guCeMainMaxCallPerUser =
                                 (CCTKCFG_MAX_CALL) / (CCTKCFG_MAX_USR);
static const BOS_UINT32 guCeMainMaxSubsPerUser =
                                 (CCTKCFG_MAX_EVT) / (CCTKCFG_MAX_USR);
static const BOS_UINT32 guCeMainMaxNotsPerUser =
                                 (CCTKCFG_MAX_NOT) / (CCTKCFG_MAX_USR);

#if CCTKCFG_PROXY_FAILOVER
   static const BOS_UINT32 guCeMainMaxUser = (2 * (CCTKCFG_MAX_USR)) +
                                             (CCTKCFG_MAX_VIRTUAL_USR);
#else
   static const BOS_UINT32 guCeMainMaxUser = (CCTKCFG_MAX_USR) +
                                             (CCTKCFG_MAX_VIRTUAL_USR);
#endif

static const BOS_UINT32 guCeMainRstTmr = 2000;

#define CE_MAIN__PRI__EMERGENCY            "emergency"
#define CE_MAIN_SERVICE_SOS                "service:sos"

/* ---- Class Definition -------------------------------------------------------- */

/*
** This class defines the Call Engine main.
*/
class CeMain : public MXD_GNS::IUaSspRegistrationMgr,
               public MXD_GNS::ISceUserAuthenticationMgr,
               public MXD_GNS::IUaSspNewCallMgr,
               public MXD_GNS::IUaSspCallMgr,
               public MXD_GNS::IUaSspMwiMgr,
               public MXD_GNS::ISceOodRequestSenderMgr,
               public MXD_GNS::ISceBasicExtensionMgr,
               public MXD_GNS::ISceSubscriberMgr,
               public MXD_GNS::ISceNewNotifierMgr,
               public MXD_GNS::ISceNotifierMgr,
               public MXD_GNS::ISceRequestHandler,
               public MXD_GNS::ISceReferrerMgr,
               public MXD_GNS::ISceNewReferrerMgr
{
   public:

      friend class CeFsmBase;
      friend class CeResBase;
      friend class CeDlg;

      typedef struct
      {
         BOS_UINT32                uEvt;
         CEEVT                     eEvt;

      } CEMAINEVTMAP;

      typedef struct
      {
         BOS_UINT32                uSize;
         BOS_UINT8                 *puData;
         void                      *pAdditionalNotInfo;

      } CEMAINDATA;

      typedef struct
      {
         BOS_UINT32                uType;
         BOS_UINT32                uTimeOut;
         void                      *pAdditionalSubInfo;

      } CEMAINSUB;

      CeMain( void );
      ~CeMain( void );

      static BOS_BOOL           startUp( void );
      static CCTKSTATUS         shutDown( CeResBase::CERESTYPE eType =
                                             CeResBase::eCERESTYPE_MAX,
                                          BOS_UINT32 uParentGuid =
                                             guCeInvalidGuid );
      static void               show();


      static BOS_UINT32         allocateGuid( void );

      static BOS_UINT32         allocateRes( CeResBase::CERESTYPE eType,
                                             BOS_UINT32 uResGuid,
                                             BOS_UINT32 uParentGuid,
                                             BOS_BOOL bCreate = BOS_TRUE,
                                             BOS_UINT32 uSubType =
                                                            guCeInvalidGuid );
      static BOS_BOOL           allocateReferrer(
                                             BOS_UINT32 uResGuid,
                                             MXD_GNS::ISceReferrer* pReferrer );
      static BOS_BOOL           reparentCallRes( BOS_UINT32 uFromParentGuid,
                                                 BOS_UINT32 uToParentGuid,
                                                 BOS_BOOL bRstrictSig );
      static BOS_BOOL           locateRes( BOS_UINT32 uResGuid,
                                           CeResBase *&prRes );
      static BOS_BOOL           locateCallRes( MXD_GNS::IUaSspCall* pCall,
                                               CeResBase *&prRes,
                                               BOS_BOOL bInternal = BOS_FALSE );
      static BOS_BOOL           locateBrRes( BOS_UINT32 uResGuid,
                                             CeResBase *&prRes );
      static BOS_BOOL           locateReferRes( MXD_GNS::ISceReferrer* pRefer,
                                                CeResBase *&prRes );
      static BOS_BOOL           locateTmrRes( BOS_UINT32 uTmrGuid,
                                              CeResBase *&prRes );
      static BOS_BOOL           locateCallForkRes( MXD_GNS::IUaSspCall* pCall,
                                                   CeResBase *&prRes );
      static BOS_BOOL           locateMediaInfo( BOS_UINT32 uResGuid,
                                                 CCTKMEDINFO *&prMedInfo );
      static BOS_BOOL           locateExpireInfo(
                                             BOS_UINT32 uResGuid,
                                             CCTKEXPIREINFO *&prExpInfo );
      static BOS_BOOL           locateCallInfo( BOS_UINT32 uResGuid,
                                                CeCdr::CECDRTYPE eType,
                                                CCTKCDR *&prCdr );
      static BOS_UINT32         locatePmRes( void *pData,
                                             BOS_BOOL bAssData = BOS_TRUE );
      static BOS_UINT32         locateRegId( BOS_UINT32 uResGuid );
      static BOS_BOOL           locatePrecond( BOS_UINT32 uResGuid );
      static void               locateProfId( BOS_UINT32 uResGuid,
                                              BOS_UINT32 *puParentGuid,
                                              BOS_UINT32 *puProfIx );
      static BOS_BOOL           locateAuthChg( BOS_UINT32 uResGuid );
      static BOS_BOOL           locateKaRes( BOS_UINT32 uResGuid, CeResBase *&prRes );
      static BOS_BOOL           locateFailBkRes( BOS_UINT32 uResGuid, CeResBase *&prRes );
      static BOS_BOOL           locateCallXRtpStat( BOS_UINT32 uResGuid,
                                                       CCTKXRTPSTAT *pStatInfo );
      static void               manageAuthChg( BOS_UINT32 uResGuid,
                                               BOS_BOOL bAuthChg );
      static BOS_UINT8          getResTos( BOS_UINT32 uResGuid );
      static void *             getNetHdl( BOS_UINT32 uResGuid );
      static BOS_UINT32         getUsrGuid( BOS_UINT32 uGuid );
      static MXD_GNS::CSocketAddr *getResRstrictAddr( BOS_UINT32 uResGuid );

      static BOS_BOOL           manageRes( BOS_UINT32 uResGuid,
                                           CEEVT eEvt,
                                           void *pData = NULL );
      static void               terminateAllRes( void );
      static BOS_BOOL           checkFinalRes( BOS_BOOL bIssueEvt = BOS_TRUE );
      static BOS_BOOL           checkCallRes( BOS_UINT32 uParentGuid );

      static void               updateChallenge( BOS_UINT32 uGuidParent );
      static void               CeDispatchRestrictedUser( BOS_UINT32 uResGuid, 
                                                          BOS_BOOL bBackupToPrimary );

      static CCTKSTATUS         fsmProcessEvt( BOS_UINT32 uResGuid,
                                               CEEVT eEvt,
                                               void *pData = NULL );

      static void               fsmProcessFsm( CeResBase &rRes,
                                               CeFsmBase::CEFSMTYPE eType );
      static void               fsmProcessState( CeResBase &rRes,
                                                 CeFsmBase::CEFSMSTATE eState );

      static void               fsmInvokeParent( CeResBase &rRes,
                                                 CEEVT eEvt );
      static void               fsmDistributeChild( BOS_UINT32 uParentGuid,
                                                    CEEVT eEvt );

      static void               fsmTerminated( BOS_UINT32 uResGuid );
      static void               fsmTimer( CeResBase &rRes,
                                          BOS_UINT32 uTimeMs );

      static void               fsmMapEvent( CeFsmBase::CEFSMTYPE eType,
                                             BOS_UINT32 uEvtIn,
                                             CEEVT &reEvtOut );
      static void               fsmMapCctkEvent( CeFsmBase::CEFSMTYPE eType,
                                                 BOS_UINT32 uEvtIn,
                                                 CEEVT &reEvtOut );

      static inline BOS_BOOL    ceReset( void ) { return mbReset; };

      static inline CeCdrMgr    *ceCdrMgr( void ) { return mpCdrMgr; };
      static void               associateLastCall( BOS_UINT32 uResGuid,
                                                   CeCdr *pCdr,
                                                   CeCdr::CECDRTYPE eType );
      static void               associateCid( BOS_UINT32 uResGuid,
                                              const BOS_UINT8 *puCid,
                                              const BOS_UINT8 *puFromTag,
                                              const BOS_UINT8 *puToTag );
      static BOS_BOOL           associateStats( BOS_UINT32 uResGuid,
                                                CCTKCALLMETRICS *pStats );
      static BOS_BOOL           associatedDlg( BOS_UINT32 uResGuid );
      static BOS_UINT32         associateLocInfo( BOS_UINT32 uResGuid,
                                                  BOS_UINT8 *puData,
                                                  BOS_UINT32 uData );

      static BOS_UINT32         countDlg( BOS_UINT32 uResGuid,
                                          BOS_BOOL bOrig );
      static void               refreshDlg( BOS_UINT32 uResGuid );

      static BOS_UINT32         matchCall( BOS_UINT32 uResGuid,
                                           BOS_UINT32 uResParent,
                                           const BOS_UINT8 *puCallCredJoin );

      static void               featInfo( BOS_UINT32 uResGuid,
                                          CCTKFEATINFO *pFeatInfo );

      static BOS_BOOL           remoteRes( BOS_UINT32 uParentGuid );

      static void               setPublishAddr( BOS_UINT32 uResGuid,
                                                MX_NS CSocketAddr *pPubAddr );
      static BOS_BOOL           setPrivInfo( BOS_UINT32 uResGuid, CCTKPRIVCFG *pPrivCfg );

      static BOS_BOOL          parse3gppImsXmlContent(
                                    const MXD_GNS::CSipPacket *pPacket,
                                    MXD_GNS::IXmlDocument *&pDocument,
                                    MXD_GNS::CSubAllocator *&pAllocator,
                                    char *&pType, char *&pReason, char *&pAction );

      /* ---- Callback(s) hook-up from the stack. ---- */

      /* ---- IUaSspRegistrationMgr
      */
      void     EvRegistrationStatus(
                  IN MXD_GNS::IUaSspRegistration* pRegistration,
                  IN EStatus eStatus );
      void     EvExpiring( 
                  IN MXD_GNS::IUaSspRegistration* pRegistration);

      void     EvInternalSubscriberCreated(
                  IN MXD_GNS::IUaSspRegistration *pRegistration,
                  IN MXD_GNS::ISceSubscriber *pSubscriber );

      void     EvPauReceived( IN MXD_GNS::IUaSspRegistration* pRegistration,
                              IN const MXD_GNS::CNameAddr& raddrPau );

      /* ---- IUaSspMwiMgr
      */
      void    EvSubscriptionStatus(
                 IN MXD_GNS::IUaSspMwi* pMwi,
                 IN MXD_GNS::ISceSubscriberMgr::ESubscriptionStatus eStatus );

      void    EvMailboxStatus(
                 IN MXD_GNS::IUaSspMwi* pMwi,
                 IN const MXD_GNS::CMessageSummary& rSummary );

      /* ---- IUaSspNewCallMgr
      */
      void    EvNewCallObject(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN const MXD_GNS::CSipPacket& rPacket );

      /* ---- IUaSspCallMgr
      */
      void    EvCalled(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN const MXD_GNS::CNameAddr& rFromAddr,
                 IN const MXD_GNS::CNameAddr& rToAddr,
                 IN MXD_GNS::IEComUnknown* pAdditionalParameters );

      void    EvTerminated(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::IUaSspCallMgr::ETerminationReason eReason );

      void    EvFailed(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN uint16_t uStatusCode,
                 IN const MXD_GNS::CString& rStrReason,
                 IN unsigned int uRetryAfter );

      void    EvAnswered(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::IEComUnknown* pAdditionalParameters );

      void    EvProgress(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN uint16_t uStatusCode,
                 IN const MXD_GNS::CString& rstrDisplayString,
                 IN const MXD_GNS::CNameAddr* pForwardedAddr );

      void    EvForkedCall(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::IUaSspCall* pNewCall );

      void    EvReplacing(
                 IN MXD_GNS::IUaSspCall* pNewCall,
                 IN MXD_GNS::IUaSspCall* pReplacedCall,
                 IN const MXD_GNS::CNameAddr& rRemoteAddr );

      void    EvTransfereeTransferRequested(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN const MXD_GNS::CNameAddr& rTarget );

      void    EvTransfereeTransferResult(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::IUaSspCallMgr::ETransferReport eTransferResult );

      void    EvTransferorTransferResult(
                 IN MXD_GNS::IUaSspCall* pCall,
                 IN MXD_GNS::IUaSspCallMgr::ETransferReport eTransferResult );

      void    EvConnectedIdChange( IN MXD_GNS::IUaSspCall* pCall,
                                   IN const MXD_GNS::CNameAddr& rLocalAddr,
                                   IN const MXD_GNS::CNameAddr& rRemoteAddr );

      /* ---- ISceOodRequestSenderMgr
      */
      void    EvOodResponse(
                 IN MXD_GNS::ISceOodRequestSender* pComponent,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN mxt_opaque opqTransaction,
                 IN const MXD_GNS::CSipPacket& rResponse );

      /* ---- ISceBasicExtensionMgr
      */
      void    EvUnhandledRequest(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN MXD_GNS::ISipServerEventControl* pServerEventCtrl,
                 IN const MXD_GNS::CSipPacket& rRequest,
                 IN MXD_GNS::IEComUnknown* pAdditionalParameters );

      void    EvResponse(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN mxt_opaque opqTransaction,
                 IN const MXD_GNS::CSipPacket& rResponse );

      void    EvChallenged(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN const MXD_GNS::CSipPacket& rChallenge,
                 IN mxt_opaque opqChallenge );

      void    EvSipContextCreated(
                 IN MXD_GNS::ISceBasicExtensionControl* pComponent,
                 IN MXD_GNS::ISipContext* pCreatedContext );

      /* ---- ISceUserAuthenticationMgr
      */
      void    EvChallenged(
                 IN MXD_GNS::ISceUserAuthentication* pAuth,
                 IN const MXD_GNS::CString& rstrRealm,
                 IN const MXD_GNS::CString& rstrProtocol );

      void    EvSvrAuthenticationFailed(
                 IN MXD_GNS::ISceUserAuthentication* pAuth,
                 IN const MXD_GNS::CString& rstrProtocol );

      void    EvAuthenticationSuccess(
                 IN MXD_GNS::ISceUserAuthentication* pAuth,
                 IN const MXD_GNS::CString& rstrProtocol,
                 IN mxt_opaque opqChallengeId );

     void    EvNonceEmployed(
                 IN MXD_GNS::ISceUserAuthentication* pAuth,
                 IN mxt_opaque opqChallengeId );

      /* ---- ISceSubscriberMgr
      */
      void    EvSubscriptionStatus(
                 IN MXD_GNS::ISceSubscriber* pSubscriber,
                 IN ESubscriptionStatus eStatus );

      void    EvResourceStateUpdate(
                 IN MXD_GNS::ISceSubscriber* pSubscriber,
                 IN MXD_GNS::ISipServerEventControl* pNotifyServerEventCtrl,
                 IN const MXD_GNS::CSipMessageBody& rPayload,
                 IN bool bReceivedFinalNotify );

      void    EvWatcherInfo(
                 IN MXD_GNS::ISceSubscriber* pSubscriber,
                 IN const MXD_GNS::ISceSubscriberMgr::SWatcherInfo& rWatcherInfo,
                 IN bool bFlushWatcherInfoTable );

      void    EvWatcherInfoUpdateDone();

      void    EvResourceListUpdate(
                 IN const char* pszResourceListUri,
                 IN MXD_GNS::CVector< MXD_GNS::CXmlElement* >& rvecpListDisplayName,
                 IN bool bFullState);

      void    EvResourceListUpdateDone();

      void    EvResourceListItemUpdate(
                 IN MXD_GNS::ISceSubscriber* pSubscriber,
                 IN const MXD_GNS::ISceSubscriberMgr::SResourceListResourceInfo*
                     pResourceInfo);

      /* ---- ISceRequestHandler
      */
      mxt_result HandleRequest(
                    IN const MXD_GNS::CSipPacket& rRequest,
                    IN MXD_GNS::IEComUnknown* pUserConfig );

      /* ---- ISceNewNotifierMgr
      */
      void    EvNewNotifierObject(
                 IN MXD_GNS::ISceNotifier* pNotifier,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN const MXD_GNS::CSipPacket& rSubscribeRequest );

      /* ---- ISceNotifierMgr
      */
      void    EvNewSubscription(
                 IN MXD_GNS::ISceNotifier* pNotifier,
                 IN const MXD_GNS::CNameAddr& rFromAddr,
                 IN const MXD_GNS::CNameAddr& rToAddr,
                 IN MXD_GNS::ESipEventType eEventType,
                 IN uint32_t uRequestedExpirationSec,
                 IN const MXD_GNS::CSipMessageBody* pBody,
                 IN MXD_GNS::IEComUnknown* pAdditionalParameter );

      void    EvSubscriptionTerminated(
                 IN MXD_GNS::ISceNotifier* pNotifier,
                 IN MXD_GNS::ISceNotifierMgr::ETerminationReason eReason );

      void    EvNotifierTerminated(
                 IN MXD_GNS::ISceNotifier* pNotifier );

      /* ---- ISceReferrerMdr
      */
      void    EvRefer(
                 IN MXD_GNS::ISceReferrer* pReferrerComponent,
                 IN const MXD_GNS::CNameAddr& pReferSource,
                 IN MXD_GNS::IEComUnknown* pTargetDialog,
                 IN const MXD_GNS::CNameAddr& rReferTo,
                 IN MXD_GNS::IEComUnknown* pAdditionalParameters );

      void    EvReferProgress(
                 IN MXD_GNS::ISceReferrer* pReferrerComponent,
                 IN uint16_t uStatusCode );

      void    EvReferStatus(
                 IN MXD_GNS::ISceReferrer* pReferrerComponent,
                 IN MXD_GNS::ISceReferrerMgr::EReferStatus eStatus );

      void    EvTerminated(
                 IN MXD_GNS::ISceReferrer* pReferrerComponent );

      /* ---- ISceNewReferrerMgr
      */
      void    EvNewReferrerObject(
                 IN MXD_GNS::ISceReferrer* pReferrer,
                 IN MXD_GNS::ISceUserConfig* pUserConfig,
                 IN const MXD_GNS::CSipPacket& rPacket );

      /* ---- Media Session Callback
      */
      void    ceMseEvent(
                 BOS_UINT32 uMseId,
                 CCTKEVT eEvt );

      /* ---- Internal timer Callback
      */
      static void EvXferNotificationTimeout( BOS_UINT32 uId, void *pData );

      static void CeResetTimeout( BOS_UINT32 uId, void *pData );
      static void CeBackupRecoveryTimeout( BOS_UINT32 uId, void *pData );

      static void CeKeepAliveRecoveryTimeout( BOS_UINT32 uId, void *pData );
      static void CeBackToPrimaryRecoveryTimeout( BOS_UINT32 uId, void *pData );

   private:

      /* Resource pool(s).
      */
      static CeResUser          *mpResUser[guCeMainMaxUser];
      static CeResCall          *mpResCall[CCTKCFG_MAX_CALL];
      static CeResSub           *mpResSub[CCTKCFG_MAX_EVT];
      static CeResNot           *mpResNot[CCTKCFG_MAX_NOT];

      static CeCdrMgr           *mpCdrMgr;

      /* FSM Pool.
      */
      static CeFsmBase          *mpFsm[CeFsmBase::eCEFSMTYPE_MAX];

      static BOS_UINT32         muGuidSeed;
      static BOS_MUTEX          mGuidMutex;

      static BOS_BOOL           mbReset;
      static BOS_BOOL           mbResetNot;
      static BOS_BOOL           mbSuspend;
      static BOS_UINT32         muTmrRst;


      /* Following APIs are required for this framework to work alongside
      ** the M5T stack.
      */
      inline mxt_result QueryIf( IN MXD_GNS::mxt_iid iidRequested,
                                 OUT void** ppInterface )
      {
         MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( iidRequested );
         MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( ppInterface );
         return resFE_FAIL;
      };

      inline unsigned int AddIfRef( void )
      {
         return 0;
      };

      inline unsigned int ReleaseIfRef( void )
      {
         return 0;
      };
};


/* ---- Variable Externs -------------------------------------------------------- */
extern CeMain *gpCeMain;


/* ---- Inline Functions -------------------------------------------------------- */


#endif /* __CE_MAIN__H__INCLUDED__ */
