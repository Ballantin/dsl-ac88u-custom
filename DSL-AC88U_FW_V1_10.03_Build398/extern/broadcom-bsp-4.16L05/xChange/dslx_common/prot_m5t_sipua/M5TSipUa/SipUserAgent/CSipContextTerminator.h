//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CSIPCONTEXTTERMINATOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPCONTEXTTERMINATOR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT to be able to use this class.
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_ISIPCONTEXT_H
#include "SipCore/ISipContext.h"
#endif

#ifndef MXG_ISIPREQUESTCONTEXTTERMINATIONMGR_H
#include "SipCore/ISipRequestContextTerminationMgr.h"
#endif

#ifndef MXG_CHEADERLIST_H
#include "SipParser/CHeaderList.h"
#endif

#ifndef MXG_CSIPMESSAGEBODY_H
#include "SipParser/CSipMessageBody.h"
#endif

#if !defined(MXG_METHOD_H)
#include "SipParser/Method.h"
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
    #ifndef MXG_ISIPDIGESTCLIENTAUTHMGR_H
    #include "SipUserAgent/ISipDigestClientAuthMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
    #ifndef MXG_ISIPGENERICMGR_H
    #include "SipUserAgent/ISipGenericMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)
    #ifndef MXG_ISIPGLAREMGR_H
    #include "SipUserAgent/ISipGlareMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
    #ifndef MXG_ISIPNOTIFIERMGR_H
    #include "SipUserAgent/ISipNotifierMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)
    #ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSEMGR_H
    #include "SipUserAgent/ISipReliableProvisionalResponseMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
    #ifndef MXG_ISIPREDIRECTIONMGR_H
    #include "SipUserAgent/ISipRedirectionMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
    #ifndef MXG_ISIPREPLACESMGR_H
    #include "SipUserAgent/ISipReplacesMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
    #ifndef MXG_ISIPSESSIONMGR_H
    #include "SipUserAgent/ISipSessionMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)
    #ifndef MXG_ISIPSESSIONTIMERMGR_H
    #include "SipUserAgent/ISipSessionTimerMgr.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)
    #ifndef MXG_ISIPTRANSFERMGR07_H
    #include "SipUserAgent/ISipTransferMgr07.h"
    #endif
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
    #ifndef MXG_ISIPUPDATEMGR_H
    #include "SipUserAgent/ISipUpdateMgr.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipStackMonitor;
class IEComUnknown;
class ISipRequestContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSipContextTerminator
//==============================================================================
//<GROUP SIPUSERAGENT_CLASSES>
//
// Summary:
//   This class terminates a context as cleanly and rapidly as it can.
//
// Description:
//   This class terminates a context as cleanly and rapidly as it can. It does 
//   so on behalf of the application as a result of a call to
//   ISipUaHelpers::TerminateContext. 
//
//   It will replace all service managers of the context, abort any on-going
//   transactions and tear-down all dialogs. Once all this is done, the context
//   is cleared and released.
//
// Location:
//   SipUserAgent/CSipContextTerminator.h
//
// See Also:
//   ISipUaHelpers
//
// MXI_DESIGN_NOTE:
//    Most of the manager interfaces implemented by this class could be set
//    to NULL without hindering on the useability of the feature. However, 
//    it would introduce an unwanted API behaviour change. The SIP Stack services
//    do not allow setting their managers to NULL and it has always been this
//    way. So in order to prevent the introduction of a behaviour change all
//    managers are implemented, with empty or basic event implements when
//    required.
//
//==============================================================================
class CSipContextTerminator :
#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
                              private ISipDigestClientAuthMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
                              private ISipGenericMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)
                              private ISipGlareMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
                              private ISipNotifierMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)
                              private ISipReliableProvisionalResponseMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
                              private ISipRedirectionMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
                              private ISipReplacesMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
                              private ISipSessionMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)
                              private ISipSessionTimerMgr,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)
                              private ISipTransferMgr07,
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
                              private ISipUpdateMgr,
#endif
                              private ISipRequestContextTerminationMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipContextTerminator();

    // Summary:
    //  Destructor.
    ~CSipContextTerminator();

    // Summary:
    //  Terminates a context immediately but as cleanly as possible.
    mxt_result TerminateContext(IN ISipContext* pContext);

    // Summary:
    //  Adds a SIP method to a list for which the transaction will be terminated
    //  gracefully.
    mxt_result AddGracefullyTerminatedClientRequest(IN ESipMethod eRequestMethod);

    // Summary:
    //  Removes a SIP method from a list for which the transaction will be
    //  terminated gracefully.
    mxt_result RemoveGracefullyTerminatedClientRequest(IN ESipMethod eRequestMethod);

    // Summary:
    //  Returns the current list of SIP methods for which existing client
    //  transactions will be terminated gracefully.
    const CVector<ESipMethod>& GetGracefullyTerminatedClientRequests() const;

    // Summary:
    //  Customize the headers and body of potential request sent to terminate the context.
    mxt_result CustomizePacketForContextTermination(IN ISipContext* pContext,
                                                    IN ESipMethod eMethod,
                                                    IN TOA CHeaderList* pExtraHeaders,
                                                    IN TOA CSipMessageBody* pBody);

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipContextTerminator(IN const CSipContextTerminator& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipContextTerminator& operator=(IN const CSipContextTerminator& rSrc);

    //-- << ISipRequestContextTerminationMgr >>
    virtual void EvAllRequestContextTerminatedForContext(IN ISipContext* pContext);

#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)

    //-- << ISipDigestClientAuthMgr >>
    virtual void EvCredentialsRequired(IN ISipDigestClientAuthSvc* pSvc,
                                       IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);

    virtual void EvCredentialsExist(IN ISipDigestClientAuthSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse);

    virtual void EvInvalidCredentials(IN ISipDigestClientAuthSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CString& rstrRealm,
                                      IN const CSipPacket& rResponse);

    virtual void EvAuthLoop(IN ISipDigestClientAuthSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvInvalidResponseCredentials(IN ISipDigestClientAuthSvc* pSvc,
                                              IN ISipClientEventControl*  pClientEventCtrl,
                                              IN const CVector<mxt_result>& rvecresReason,
                                              IN const CSipPacket&        rResponse);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)

    //-- << ISipGenericMgr >>
    virtual void EvRequest(IN ISipGenericSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest);

    virtual void EvProgress(IN ISipGenericSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipGenericSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipGenericSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)

    //-- << ISipGlareMgr >>
    virtual void EvReadyToRetry(IN ISipGlareSvc* pGlareSvc,
                                IN bool bOtherRequestSeenSinceGlare);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)

    //-- << ISipNotifierMgr >>
    virtual void EvSubscribed(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN unsigned int uExpirationSec,
                              IN const CSipPacket& rSubscribe);

    virtual void EvFetched(IN ISipNotifierSvc* pSvc,
                           IN ISipServerEventControl* pServerEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rSubscribe);

    virtual void EvRefreshed(IN ISipNotifierSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN const CString& rstrEvent,
                             IN const CString& rstrId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe);

    virtual void EvTerminated(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rSubscribe);

    virtual void EvInvalidSubscribe(IN ISipNotifierSvc* pSvc,
                                    IN mxt_opaque opqApplicationData,
                                    IN const CSipPacket& rSubscribe,
                                    IN mxt_result reason);

    virtual void EvProgress(IN ISipNotifierSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvExpired(IN ISipNotifierSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)

    //-- << ISipRedirectionMgr >>
    virtual void EvRedirected(IN ISipRedirectionSvc* pSvc,
                              IN ISipClientEventControl* pClientEventCtrl,
                              IN const CSipPacket& rPacket );

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)

    //-- << ISipReliableProvisionalResponseMgr >>
    virtual void EvReliableResponseReceived(IN ISipReliableProvisionalResponseSvc* pSvc,
                                            IN ISipClientEventControl* pClientEventCtrl,
                                            IN const CSipPacket& rPacket);

    virtual void EvResponseAcknowledged(IN ISipReliableProvisionalResponseSvc* pSvc,
                                        IN ISipServerEventControl* pServerEventCtrl,
                                        IN const CSipPacket& rPacket);

    virtual void EvInvalidPrack(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rPacket,
                                IN mxt_result resReason);

    virtual void EvReliableProvisionalResponseTimeout(IN ISipReliableProvisionalResponseSvc* pSvc,
                                                      IN mxt_opaque opqTransaction);

    virtual void EvPrackProgress(IN ISipReliableProvisionalResponseSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rPacket);

    virtual void EvPrackSuccess(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket);

    virtual void EvPrackFailure(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)

    //-- << ISipReplacesMgr >>
    virtual void EvReplaces(IN ISipReplacesSvc* pSvc,
                            IN ISipUserAgentSvc* pReplacedUaSvc,
                            IN bool bEarlyOnly,
                            IN const CSipPacket& rRequest,
                            INOUT mxt_opaque& rApplicationData);

    virtual void EvInvalidReplaces(IN ISipReplacesSvc* pSvc,
                                   IN mxt_opaque opqApplicationData,
                                   IN const CSipPacket& rRequest,
                                   IN mxt_result res);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)

    //-- << ISipSessionMgr >>
    virtual void EvInvited(IN ISipSessionSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest);

    virtual void EvReInvited(IN ISipSessionSvc* pSvc,
                             IN ISipServerEventControl* pServerEventCtrl,
                             IN const CSipPacket& rRequest);

    virtual void EvAcknowledged(IN ISipSessionSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rRequest);

    virtual void EvTerminated(IN ISipSessionSvc* pSvc,
                              IN ISipServerEventControl* pServerEventCtrl,
                              IN const CSipPacket& rRequest);

    virtual void EvInviteCancelled(IN ISipSessionSvc* pSvc,
                                   IN ISipServerEventControl* pServerEventCtrl,
                                   IN const CSipPacket& rRequest );

    virtual void EvReInviteCancelled(IN ISipSessionSvc* pSvc,
                                     IN ISipServerEventControl* pServerEventCtrl,
                                     IN const CSipPacket& rRequest );

    virtual void EvInvalidInvite(IN ISipSessionSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rRequest,
                                 IN mxt_result resReason);

    virtual void EvProgress(IN ISipSessionSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvTerminationProgress(IN ISipSessionSvc* pSvc,
                                       IN ISipClientEventControl* pClientEventCtrl,
                                       IN const CSipPacket& rResponse);

    virtual void EvTerminationSuccess(IN ISipSessionSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CSipPacket& rResponse);

    virtual void EvTerminationFailure(IN ISipSessionSvc* pSvc,
                                      IN ISipClientEventControl* pClientEventCtrl,
                                      IN const CSipPacket& rResponse);

    virtual void EvNewSessionNeededForOriginalInviteResponse(IN ISipSessionSvc* pSvc,
                                                             IN ISipClientEventControl* pClientEventCtrl,
                                                             IN const CSipPacket& rPacket);

    virtual void EvInviteSuccessResponseTimeout(IN ISipSessionSvc* pSvc,
                                                IN mxt_opaque opq,
                                                IN const CSipPacket& rRequest);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)

    //-- << ISipSessionTimerMgr Interface >>
    virtual void EvSessionIntervalTooShortSent(IN ISipSessionTimerSvc* pSvc,
                                               IN mxt_opaque opqApplicationData,
                                               IN const CSipPacket& rRequest);

    virtual void EvSessionIntervalTooShortRecv(IN ISipSessionTimerSvc* pSvc,
                                               IN ISipClientEventControl* pClientEventCtrl,
                                               IN const CSipPacket& rRequest);

    virtual void EvSessionMustRefresh(IN ISipSessionTimerSvc* pSvc);

    virtual void EvSessionExpired(IN ISipSessionTimerSvc* pSvc);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)

    //-- << ISipTransferMgr07 Interface >>
    virtual void EvReferProgress(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvReferSuccess(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse);

    virtual void EvReferFailure(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse);

    virtual void EvProgressReport(IN ISipTransferSvc07* pSvc,
                                  IN ISipServerEventControl* pServerEventControl,
                                  IN ISipReferrerMgr::EState eState,
                                  IN const CSipStatusLine& rStatus,
                                  IN const CHeaderList* pContentHeaders,
                                  IN const CSipPacket& rNotifyRequest);

    virtual void EvFinalReport(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
                               IN const CSipPacket& rNotifyRequest);

    virtual void EvReportingExpired(IN ISipTransferSvc07* pSvc);

    virtual void EvTransferred(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CNameAddr& rTarget,
                               IN const CSipPacket& rReferRequest);

    virtual void EvNotifyProgress(IN ISipTransferSvc07* pSvc,
                                  IN ISipClientEventControl* pClientEventControl,
                                  IN const CSipPacket& rResponse);

    virtual void EvNotifySuccess(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvNotifyFailure(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse);

    virtual void EvFinalStatusRequired(IN ISipTransferSvc07* pSvc,
                                       IN const CSipPacket* pSubscribe);

    virtual void EvInvalidRequest(IN ISipTransferSvc07* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result resError);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)

    //-- << ISipUpdateMgr Interface >>
    virtual void EvUpdated(IN ISipUpdateSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rUpdateRequest);

    virtual void EvInvalidUpdate(IN ISipUpdateSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rRequest,
                                 IN mxt_result resReason);

    virtual void EvProgress(IN ISipUpdateSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse);

#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)

    //-- << Helpers >>
    void ClearContextIfNotUsed(IN unsigned int uCtxIndex);
    unsigned int FindContext(IN ISipContext* pContext) const;
    bool FindSupplementalContext(IN ISipContext* pContext, OUT unsigned int& ruIndex) const;
    void HandleTermination(IN IEComUnknown* pSvc);
    void HandleSupplementalContextTermination(IN IEComUnknown* pSvc);
    void TerminateSupplementalContext(IN IEComUnknown* pSvc);
    void SendByeRequest(IN ISipSessionSvc* pSvc);
    bool ShouldTerminateTransaction(IN ISipRequestContext* pRequestContext);

//-- Hidden Data Members
protected:
private:

    enum ETerminationState
    {
        eTERMINATION_IDLE,
        eTERMINATION_TRANSACTIONS,
        eTERMINATION_DIALOGS
    };

    struct SPacketCustomization
    {
        SPacketCustomization()
        :   m_pExtraHeaders(NULL),
            m_pBody(NULL)
        {
        }

        SPacketCustomization(IN const SPacketCustomization& rPacketCustom)
        :   m_pExtraHeaders(NULL),
            m_pBody(NULL)
        {
            if (rPacketCustom.m_pExtraHeaders != NULL)
            {
                m_pExtraHeaders = MX_NEW(CHeaderList)(*rPacketCustom.m_pExtraHeaders);
            }
            if (rPacketCustom.m_pBody != NULL)
            {
                m_pBody = MX_NEW(CSipMessageBody)(*rPacketCustom.m_pBody);
            }
        }

        ~SPacketCustomization()
        {
            MX_DELETE(m_pExtraHeaders);
            m_pExtraHeaders = NULL;

            MX_DELETE(m_pBody);
            m_pBody = NULL;
        }

        CHeaderList* m_pExtraHeaders;
        CSipMessageBody* m_pBody;
    };

    struct SContextToTerminate
    {
        ISipContext* m_pContext;
        ETerminationState m_eTerminationState;
        unsigned int m_uNbRemainingDialogUsersToTerminate;
        CList<ISipContext*> m_lstpSupplementalContexts;

#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 does not support default parameters in templates,
        // in which case all the parameters must be specified.
        CMap<ESipMethod, SPacketCustomization,
            CAATree<CMapPair<ESipMethod, SPacketCustomization> > > m_mapCustomization;
#else
        CMap<ESipMethod, SPacketCustomization> m_mapCustomization;
#endif

        // This flag indicates if the context received a 2XX response. It is used
        // only in cases of forking to know if a BYE request is required on 
        // the originator when all supplemental context are terminated.
        bool m_bContextReceivedSuccessResponse;

        // This flag indicates if a BYE request should be sent when terminating
        // dialogs. It will be true unless all of these conditions are met:
        //  - The application registered eSIP_METHOD_BYE with AddGracefullyTerminatedClientRequest
        //  - The application sent a BYE that had not received a response when it called TerminateContext.
        //  - The request receives a failure response while it is terminating existing transactions.
        bool m_bDialogTerminationSendsByeRequest;

        SContextToTerminate()
        :   m_pContext(NULL),
            m_eTerminationState(eTERMINATION_IDLE),
            m_uNbRemainingDialogUsersToTerminate(0),
            m_bContextReceivedSuccessResponse(false),
            m_bDialogTerminationSendsByeRequest(true)
        {
        }

        void AddSupplementalContext(IN ISipContext* pContext)
        {
            MX_ASSERT(pContext != NULL);
            m_lstpSupplementalContexts.Append(pContext);
            pContext->AddIfRef();
            m_uNbRemainingDialogUsersToTerminate++;
        }

        void RemoveSupplementalContext(IN ISipContext* pContext)
        {
            MX_ASSERT(pContext != NULL);
            unsigned int uIndex = m_lstpSupplementalContexts.Find(0, pContext);
            if (uIndex != m_lstpSupplementalContexts.GetEndIndex())
            {
                m_lstpSupplementalContexts[uIndex]->Clear();
                m_lstpSupplementalContexts[uIndex]->ReleaseIfRef();
                m_lstpSupplementalContexts.Erase(uIndex);
                m_uNbRemainingDialogUsersToTerminate--;
            }
        }

        void SetContext(IN ISipContext* pContext)
        {
            MX_ASSERT(pContext != NULL);
            MX_ASSERT(m_pContext == NULL);
            m_pContext = pContext;
            m_pContext->AddIfRef();
        }

        ~SContextToTerminate()
        {
            // At this point, all supplemental context MUST have been released
            // since the context terminator is supposed to wait for all of them 
            // to be released before releasing the originator.
            MX_ASSERT(m_lstpSupplementalContexts.IsEmpty());

            m_mapCustomization.EraseAll();

            MX_ASSERT(m_pContext != NULL);
            m_pContext->ReleaseIfRef();
            m_pContext = NULL;
        }
    };

#if MXD_COMPILER_MS_VC == MXD_COMPILER_MS_VC_6
    // MXI_DESIGN_NOTE:
    // This friendship declaration is required to get msvc6 to accept the usage
    // of the private ETerminationState enumeration inside the 
    // SContextToTerminate structure.
    friend struct SContextToTerminate;
#endif

    // Contexts currently in termination process.
    CList<SContextToTerminate> m_lststContextsToTerminate;

    // Convenience pointer on the stack monitor.
    CSipStackMonitor* m_pStackMonitor;

    // The list of SIP methods for which the existing client transactions MUST
    // not be terminated.
    CVector<ESipMethod> m_vecSipMethods;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPCONTEXTTERMINATOR_H

//M5T_INTERNAL_USE_END
