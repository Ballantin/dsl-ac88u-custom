//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSCESUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSCESUBSCRIBER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSCEBASECOMPONENT_H
#include "SceCoreComponents/CSceBaseComponent.h"
#endif

#ifndef MXG_ISCESUBSCRIBER_H
#include "SceCoreComponents/ISceSubscriber.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif

#ifndef MXG_ISIPSERVEREVENTCONTROL_H
#include "SipCore/ISipServerEventControl.h"
#endif

#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SipTypes/SipEventTypes.h"
#endif

#ifndef MXG_ISIPSUBSCRIBERMGR_H
#include "SipUserAgent/ISipSubscriberMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CSceCallerPreferences;
class CXmlElement;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSceSubscriber
//========================================
//
// Description:
//   This interface can be used to subscribe to any event type.
//
// Description:
//  The subscriber component can be used to subscribe to any type of resources
//  in the following ways:
//    - The user can use the Subscribe() API to subscribe to a single resource
//      as defined in RFC 3265. The subscriber component will use
//      EvSubscriptionStatus to report the state of the subscription, while it
//      will be using EvResourceStateUpdate to report the state of the resource
//      subscribed to.
//    - The user can use the Subscribe() API to subscribe to a list of
//      resources as defined in RFC 4662. The subscriber component will use
//      EvSubscriptionStatus to report the state of the subscription. It will
//      also be reporting two more event types. One is EvResourceListUpdate to
//      report information about the resource list being processed, as it is
//      possible for a resource list to contain or point to an other resource
//      list. The other is EvResourceListItemUpdate that is used to report both
//      the subscription state and resource state of the subscriptions resulting
//      from the subscription to the list. This component does not have to know
//      in advance whether it is subscribing to a single resource or to a list;
//      the NOTIFY sent by the server provides enough information for the
//      subscriber component to adapt its behavior to both scenarios.
//    - The user can use the SubscribeWatcherInfo() API to subscribe to the
//      watcher information state of a resource as defined in RFCs 3857 and
//      3858. In other words, this allows the user to monitor the subscriptions
//      done to a specific resource, or watch the watchers. When using
//      SubscribeWatcherInfo, the component will use EvSubscriptionStatus to
//      report the subscription state while it will be using EvWatcherInfo to
//      report the information about the subscriptions done to the resource.
//
//  Subscribe() and SubscribeWatcherInfo() are mutually exclusive and both
//  cannot be used at the same time on a component. Subscribe() can also be used
//  to monitor watchers of a resource. The difference between using Subscribe()
//  and SubscribeWatcherInfo() to monitor watchers of a resource reside in the
//  event being reported.
//     - When subscribing using SubscribeWatcherInfo, the component interprets
//       the XML received in the NOTIFY and uses this to build the
//       ISceSubscriberMgr::SWatcherInfo information that is passed to
//       EvWatcherInfo.
//     - When subscribing using Subscribe to a watcher information event, the
//       component does not interpret the XML payload received in the NOTIFY and
//       passes it transparently to the user in the EvResourceStateUpdate event.
//
//  The subscriber component will automatically refresh and resubscribe upon
//  failure unless it returns eSUBSCRIPTION_FAILED (unrecoverable error),
//  eUSER_TERMINATED (all dialogs from this user are being terminated) or
//  eTERMINATED (user initiated unsubscribe) in
//  ISceSubscriberMgr::EvSubscriptionStatus, in which case the component should
//  be released.
//
//  Note that this SUBSCRIBER component is triggered from the reception of
//  NOTIFY. As a result, receiving the 200-OK before or after the NOTIFY does
//  not impact the subscriber's state.
//
// Location:
//   SceCoreComponents/CSceSubscriber.h
//
// See Also:
//   ISceSubscriber, ISceSubscriberMgr
//
//==============================================================================
class CSceSubscriber : private CSceBaseComponent,
                       private ISceSubscriber,
                       private ISipSubscriberMgr,
                       private ISipServerEventControl
{
//-- Friend Declarations.
    friend class CSceCoreComponentsInitializer;

//-- New types within class namespace
public:
    // Subscription state. Refer to GetSubscriptionStateHelper() for states information.
    enum ESubscriptionState
    {
        eACTIVE,
        ePENDING,
        eTERMINATED,
        eINVALID,
        eNONE
    };

//-- Hidden Methods.
private:

    //-- << Initialization mechanism >>
    static mxt_result InitializeCSceSubscriber();

    static void FinalizeCSceSubscriber();

    //-- << ECOM mechanism >>
    static mxt_result CreateInstance(IN  IEComUnknown*  pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSceSubscriber(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    virtual ~CSceSubscriber();

    //-- << CEComDelegatingUnknown >>
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    virtual mxt_result NonDelegatingQueryIf(IN  mxt_iid iidRequested,
                                            OUT void**  ppInterface);

    //-- << ISceSubscriber >>
    virtual mxt_result SetManager(IN ISceSubscriberMgr* pMgr);

    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig);

    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig);

    virtual ISceSubscriberMgr::ESubscriptionStatus GetSubscriptionState() const;

    virtual void SetOpaque(IN mxt_opaque opq);

    virtual mxt_opaque GetOpaque();

    virtual void SetSubscriptionRetryDelay(IN uint32_t uRetryDelayS);

    virtual uint32_t GetSubscriptionRetryDelay() const;

// BRCM_CUSTOM - [add] Add dynamic threshold support to reg-event subscription
    virtual void SetSubscriptionExpiringThreshold(IN uint32_t uNbSeconds);
// BRCM_CUSTOM - [end] Add dynamic threshold support to reg-event subscription

    virtual mxt_result EnableNotificationParsing(IN bool bEnable);

    virtual mxt_result Subscribe(IN TOA CNameAddr* pResourceAor,
                                 IN TOA IUri* pNotifierRequestUri,
                                 IN ESipEventType eEventType,
                                 IN uint32_t uSubscriptionPeriodS,
                                 IN TOA CSceCallerPreferences* pCallerPreferences,
                                 IN TOA CGenParamList* pCustomParameters,
                                 IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result Refresh(IN TOA CGenParamList* pCustomParameters,
                               IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result SubscribeWatcherInfo(IN TOA CNameAddr* pResourceAor,
                                            IN TOA IUri* pNotifierRequestUri,
                                            IN ESipEventType eEventType,
                                            IN uint32_t uSubscriptionPeriodS,
                                            IN TOA CSceCallerPreferences* pCallerPreferences,
                                            IN TOA CSipMessageBody* pMessageBody);

    virtual mxt_result Unsubscribe();

// BRCM_CUSTOM - [add] Change the subscriber's state to handle NOTIFY rejected
    virtual void UnsubscribeSilently();
// BRCM_CUSTOM - [end] Change the subscriber's state to handle NOTIFY rejected

    virtual void Terminate();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void TerminateImmediately();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void SetSendOption(IN bool bToNetwork);
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    virtual void OverrideTracingNodes(IN STraceNode& rNewComponentNode,
                                      IN STraceNode& rNewEComNode);

    //-- << ISipSubscriberMgr >>
    virtual void EvProgress(IN ISipSubscriberSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse);

    virtual void EvSuccess(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvIntervalTooSmall(IN ISipSubscriberSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventControl,
                                    IN unsigned int uMinExpirationSec,
                                    IN const CString& rstrEvent,
                                    IN const CString& rstrId,
                                    IN const CSipPacket& rResponse);

    virtual void EvFailure(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse);

    virtual void EvNotified(IN ISipSubscriberSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rNotify);

    virtual void EvTerminated(IN ISipSubscriberSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rNotify);

    virtual void EvInvalidNotify(IN ISipSubscriberSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rNotify,
                                 IN mxt_result reason);

    virtual void EvExpiring(IN ISipSubscriberSvc* pSvc,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId);

    virtual void EvExpired(IN ISipSubscriberSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId);

    //-- << CEventDriven >>

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    //-- << ISipServerEventControl >>
    virtual mxt_result SendResponse(IN unsigned int uCode,
                                    IN const char* szReason,
                                    IN TOA CHeaderList* pExtraHeaders,
                                    IN TOA CSipMessageBody* pMessageBody);

    //-- << Helper methods >>

    // Summary:
    // Gets the subscription states from the Sip packet.
    const ESubscriptionState GetSubscriptionStateHelper(IN const CSipPacket& rNotify);

    // Summary:
    // Converts rstrString to an ESubscriptionStatus.
    const ISceSubscriberMgr::ESubscriptionStatus GetSubscriptionStatusHelper(IN const char* pszSubStatus);

    // Summary:
    // Sends a refresh SUBSCRIBE request.
    mxt_result RefreshHelper(IN bool bSentNULLBody = false);

    // Summary:
    // Sends the subscribe and update internal states.
    mxt_result SubscribeHelper();

    // Summary:
    // Sends the subscribe and update internal states.
    mxt_result CompleteSubscription();

    // Summary:
    // Sends the unsubscribe and update internal states.
    mxt_result UnsubscribeHelper();

    // Summary:
    // Creates and configures a new context.
    mxt_result CreateAndConfigNewContext();

    // Summary:
    // Changes the internal subscriber states.
    void ChangeState(ISceSubscriberMgr::ESubscriptionStatus eNewState,
                     uint64_t uSubscriptionRetryMs = 0);

    // Summary:
    // Releases the current Sip context resources.
    void ReleaseSipContext();

    // Summary:
    // Handles events while in eUNSUBSCRIBING state.
    void HandleEventsWhileInUnsubscribingState();

    // Summary:
    // Releases all resources.
    void ReleaseResources();

    // Summary:
    // Releases the current subscription resources.
    void ReleaseCurrentSubscriptionResources();

    // Summary:
    // Handles any pending request.
    bool HandlePendingRequest();

    // Summary:
    // Processes a resource list document contained in a message body.
    mxt_result ProcessResourceList(IN const CSipMessageBody* pMessageBody);

    // Summary:
    // Processes the rlmi xml contained in a message body.
    mxt_result ProcessRlmiHelper(IN const CSipMessageBody* pMessageBody,
                                 IN const CSipMessageBody* pParentMessageBody);

    // Summary:
    // Processes the resource in the rlmi xml contained in a message body.
    mxt_result ProcessRlmiResourceElementHelper(IN const CSipMessageBody* pParentMessageBod,
                                                INOUT CXmlElement* pListElement,
                                                INOUT CVector<const CSipMessageBody*>& rvecpListToProcess);

    // Summary:
    // Processes the instance in the rlmi xml.
    void ProcessRlmiInstanceElementHelper(IN const CSipMessageBody* pParentMessageBody,
                                          INOUT CXmlElement* pResourceElement,
                                          INOUT ISceSubscriberMgr::SResourceListResourceInfo& rstResource,
                                          INOUT CVector<const CSipMessageBody*>& rvecpListToProcess);

    // Summary:
    // Find name element in pElement and appends them in rvecpDisplayNames.
    void FindNameElementHelper(IN CXmlElement* pElement,
                               INOUT CVector<CXmlElement*>& rvecpDisplayNames);

    // Summary:
    // Processes a watcherinfo list document contained in a message body.
    mxt_result ProcessWatcherInfo(IN const CSipMessageBody* pMessageBody);

    // Summary:
    //  Process the watcher entries of a watcher list.
    mxt_result ProcessWatcherHelper(IN const CXmlElement* pWatcherList,
                                    IN unsigned uCurrentVersion,
                                    INOUT bool& bFlushWatcherInfo,
                                    OUT bool& bManagerWasNotified);

    // Summary:
    //  Returns the Status Code associated with the pszWatcherStatus string.
    ISceSubscriberMgr::EWinfoStatus GetWatcherStatusCode(IN const char* pszWatcherStatus);

    // Summary:
    //  Returns the Event Code associated with the pszEvent string.
    ISceSubscriberMgr::EWinfoEvent GetWatcherEventCode(IN const char* pszEvent);

    // Summary:
    //  Check if the strings are equal.
    bool StringsAreEqual(IN const char* pszString1,
                         IN const char* pszString2);
    // Summary:
    // Searches for a body with a Content-ID header matching pTokenStart in the
    // child bodies of pParentMessageBody.
    const CSipMessageBody* FindBodyByCid(IN const CSipMessageBody* pMessageBody,
                                         IN const char* pszStar);

    // Summary:
    //  Tells if the component is ready to send a new request.
    bool SubscriberIsReadyToSend();

    // << CSceBaseComponent >>
    virtual void OnUserAgentServiceConfigured(IN mxt_result res,
                                              IN TO CTelUri* pTelUri,
                                              IN TO CList<CSipUri>* plstSipUris);

    virtual void OnUaCapabilitiesChanged(IN const CSceSipFeatureSet* pNewCapabilities);

    virtual uint32_t GetSupportedServicesBitset() const;

    virtual void OnUserEventTerminateUser();

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    virtual void OnUserEventTerminateUserSilent();
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSceSubscriber(IN const CSceSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    CSceSubscriber& operator=(IN const CSceSubscriber& rSrc);


//-- Hidden Data Members.
private:
    // The subscriber service.
    ISipSubscriberSvc*                      m_pSubscriberSvc;

    // The subscriber retry delay in msec.
    uint64_t                                m_uSubscriptionRetryDelayMs;

// BRCM_CUSTOM - [add] Add dynamic threshold support to reg-event subscription
    uint32_t                                m_uExpiringThreshold;
// BRCM_CUSTOM - [end] Add dynamic threshold support to reg-event subscription

    // The subscriber manager.
    ISceSubscriberMgr*                      m_pMgr;

    // The subscriber status.
    ISceSubscriberMgr::ESubscriptionStatus  m_eSubscriptionStatus;

    // The subscriber user configuration.
    ISceUserConfig*                         m_pUserConfig;

    // The address of the resource to which the component must subscribe.
    // Will be used in the To header. This member is always kept in case we have
    // to start over with a new ISipContext.
    CNameAddr* m_pResourceAor;

    // The address of the resource to which the component must subscribe.
    // Will be used in the request-URI. Can be NULL, in which case the URI
    // found in pResourceAor will be used instead. This member is always kept in
    // case we have to start over with a new ISipContext.
    IUri* m_pNotifierRequestUri;

    // The caller preferences to apply to the current request. This member is
    // always kept in case we have to start over with a new ISipContext.
    CSceCallerPreferences* m_pCallerPrefs;

    // The custom parameters to add in the Event header of the SUBSCRIBE
    // requests.
    CGenParamList* m_pSubscribeEventParametersToSend;

    // The message-body to put in the SUBSCRIBE requests. Note that this body is
    // always kept because if we have to start the subscription over again, we
    // can use the latest version we got.
    CSipMessageBody* m_pSubscribeBody;

    // The buffered message-body to be sent.
    CSipMessageBody* m_pSubscribeBodyToSend;

    // The subscriber event type.
    ESipEventType                           m_eEventType;

    // The Event Type.
    CString                                 m_strEvent;

    // The Id of our subscription.
    CString                                 m_strId;

    // The subscription period in second.
    uint32_t                                m_uSubscriptionPeriodS;

    struct SNotifyTransaction
    {
        // The ISipServerEventControl object on which the response to the NOTIFY
        // must be sent.
        ISipServerEventControl* m_pNotifyServerEventControl;

        // The Subscription-State value.
        ESubscriptionState m_eNotifySubscriptionState;

        // The NOTIFY request.
        const CSipPacket* m_pNotify;

        // State of the subscriber before the NOTIFY was processed.
        ISceSubscriberMgr::ESubscriptionStatus m_ePreviousSubscriberStatus;

        SNotifyTransaction(IN ISipServerEventControl* pNotifyServerEventControl,
                           IN const CSipPacket& rNotify,
                           IN ISceSubscriberMgr::ESubscriptionStatus eStatusBeforeInvite);

        ~SNotifyTransaction();

    private:

        //-- << Deactivated Constructors / Destructors / Operators >>

        // Summary:
        //  Default constructor.
        SNotifyTransaction();

        // Summary:
        //  Copy Constructor.
        SNotifyTransaction(IN const SNotifyTransaction& rSrc);

        // Summary:
        //  Assignment Operator.
        SNotifyTransaction& operator=(IN const SNotifyTransaction& rSrc);
    };

    // The current NOTIFY server transaction.
    SNotifyTransaction* m_pstCurrentServerNotify;

    // Indicates if notification payloads must be parsed.
    bool m_bEnableNotificationParsing;

    // Indicates that at least one 200-OK has been received.
    bool m_bEvSuccess;

    // Indicates that the final NOTIFY has been received.
    bool m_bEvFinalNotifyReceived;

    // Indicates that at least one 4XX or 5XX has been received.
    bool m_bEvFailure;

    // Tells if there currently is an ongoing client SUBSCRIBE request.
    bool m_bOngoingClientRequest;

    // Tells if we must terminate when it is possible.
    bool m_bTerminateWhenPossible;

    // Tells if the user has been terminated or not.
    bool m_bUserTerminated;

    // Tells if EvResourceListUpdate was reported to the manager
    // for the current NOTIFY
    bool m_bResourceListUpdateWasReported;

    // Pending request possibilities.
    enum EPendingRequest
    {
        // No request is pending.
        ePENDING_REQUEST_NONE,

        // A subscribe request is pending.
        ePENDING_REQUEST_SUBSCRIBE,

        // A refresh SUBSCRIBE request is pending.
        ePENDING_REQUEST_REFRESH,

        // An unsubscribe request is pending.
        ePENDING_REQUEST_UNSUBSCRIBE
    };

    // Pending request. Only one pending request is supported at a time.
    EPendingRequest m_ePendingRequest;

    // Map of the rlmi list and their corresponding version.
    CMap<CString, unsigned int, CVector<CMapPair<CString, unsigned int> > > m_mapResourceListVersion;

    // The component tracing node to use.
    STraceNode* m_pstComponentNode;

    // The ECOM tracing node to use.
    STraceNode* m_pstEComNode;

    // Tells if the subscriptions is a watcher information.
    bool m_bWinfoSubscription;

    // Next expected document version, only for watcher Info.
    uint32_t  m_uNextExpectedDocVersion;

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline void CSceSubscriber::SetSubscriptionRetryDelay(IN uint32_t uRetryDelayS)
{
    m_uSubscriptionRetryDelayMs = static_cast<uint64_t>(uRetryDelayS) * 1000;
}


inline uint32_t CSceSubscriber::GetSubscriptionRetryDelay() const
{
    return(static_cast<uint32_t>(m_uSubscriptionRetryDelayMs / 1000));
}

// BRCM_CUSTOM - [add] Add dynamic threshold support to reg-event subscription
inline void CSceSubscriber::SetSubscriptionExpiringThreshold(IN uint32_t uNbSeconds)
{
    m_uExpiringThreshold = uNbSeconds;
}
// BRCM_CUSTOM - [end] Add dynamic threshold support to reg-event subscription

//==============================================================================
//==
//==  SubscriberIsReadyToSend
//==
//==============================================================================
//
//  Description:
//      Tell if the component is ready to send a new request.
//
//==============================================================================
inline bool CSceSubscriber::SubscriberIsReadyToSend()
{
    return m_bOngoingClientRequest == false;
}

//==============================================================================
//====  HELPER METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSCESUBSCRIBER_H

