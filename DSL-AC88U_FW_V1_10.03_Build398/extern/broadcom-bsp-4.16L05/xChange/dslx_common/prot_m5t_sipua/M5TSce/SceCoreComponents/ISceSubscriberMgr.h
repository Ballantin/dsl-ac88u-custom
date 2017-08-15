//==============================================================================
//==============================================================================
//
//          Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCESUBSCRIBERMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCESUBSCRIBERMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CSipMessageBody;
class CXmlElement;
class ISceSubscriber;
class ISipServerEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceSubscriberMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This is the interface through which the Subscriber component reports
//   its events.
//
// Description:
//  This is the interface through which the Subscriber component reports
//  its events about the state of a resource.
//
// Location:
//   SceCoreComponents/ISceSubscriberMgr.h
//
//==============================================================================
class ISceSubscriberMgr
{
//-- Friend Declarations.

//-- Published Interface.
public:


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The possible subscription statuses that can be reported to the manager.
    //
    // Description:
    //  This enumeration defines the possible subscription statuses that can be
    //  reported to the manager.
    //
    //==========================================================================
    enum ESubscriptionStatus
    {
        // Description:
        // The user is unsubscribed.
        eUNSUBSCRIBED,

        // Description:
        // The user is unsubscribed and the component is trying to subscribe.
        eUNSUBSCRIBED_TRYING,

        // Description:
        // The user is subscribed but in a pending state.
        eSUBSCRIBED_PENDING,

        // Description:
        // The user is subscribed.
        eSUBSCRIBED,

        // Description:
        // The user is subscribed, and the subscription is being refreshed.
        eSUBSCRIBED_REFRESHING,

        // Description:
        // The user is unsubscribing and waiting for an unsubscribed confirmation.
        eUNSUBSCRIBING,

        // Description:
        // The subscription has failed and cannot be retried successfully. This
        // usually denotes configuration problems either on the local device or
        // on the server. The subscription should not be retried with the
        // current parameters. This value is also returned when a received
        // NOTIFY is rejected without a Retry-After header.
        eSUBSCRIPTION_FAILED,

        // Description:
        // The subscription was terminated because the current user has been
        // terminated. The subscription should not be retried.
        eUSER_TERMINATED,

        // Description:
        // The subscription was terminated as a result of calling
        // ISceSubscriber::Unsubscribe or ISceSubscriber::Terminate.
        eTERMINATED
    };

    //==========================================================================
    //==
    //==  EvSubscriptionStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the subscription status.
    //
    //  Parameters:
    //      pSubscriber:
    //       The component reporting this event.
    //
    //      eStatus:
    //       The status of the subscription.
    //
    //  Description:
    //      Reports the subscription status of the user. This event can be
    //      reported even if the subscription status does not change. As an
    //      example, the eSUBSCRIBED status can be reported once the first
    //      subscription succeeds and each time the subscription is successfully
    //      refreshed.
    //
    //      When eStatus is eUNSUBSCRIBED, the component will try to
    //      re-subscribe the user at the specified interval.
    //
    //      When eStatus is either eSUBSCRIPTION_FAILED or eTERMINATED or
    //      eUSER_TERMINATED, the component will not attempt periodic
    //      subscription retries.
    //
    //      It is possible for a subscription to move directly from the state
    //      eSUBSCRIBED to eUNSUBSCRIBED or eSUBSCRIPTION_FAILED without
    //      reporting any other events.
    //
    //  See Also:
    //      ESubscriptionStatus
    //
    //==========================================================================
    virtual void EvSubscriptionStatus(IN ISceSubscriber* pSubscriber,
                                      IN ESubscriptionStatus eStatus) = 0;

    //==========================================================================
    //==
    //==  EvResourceStateUpdate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the state of the resource subscribed to was changed.
    //
    //  Parameters:
    //    pSubscriber:
    //      The subscriber object reporting this event.
    //
    //    pNotifyServerEventCtrl:
    //      The ISipServerEventControl to send the NOTIFY responses. It cannot
    //      be NULL.
    //
    //      The application MUST call SendResponse with a final response on it.
    //
    //    rPayload:
    //      The payload that was received.
    //
    //    bReceivedFinalNotify:
    //      This flag is set when a Final Notify was received. This information
    //      allows the manager to perform the process in the correct sequence.
    //
    //  Description:
    //      Reports that the state of the resource subscribed to was changed.
    //      This corresponds to the reception of a NOTIFY request with a
    //      payload.
    //
    //      The implementation must at least send a final response on
    //      pNotifyServerEventCtrl.
    //
    //      Note that RFC 3265 (section 3.2.4) is mandating for fast response:
    //
    //          "In no case should a NOTIFY transaction extend for any
    //          longer than the time necessary for automated processing.  In
    //          particular, subscribers MUST NOT wait for a user response before
    //          returning a final response to a NOTIFY request."
    //
    //      Until a final response is sent on this ISipServerEventControl, all
    //      incoming NOTIFY requests on this dialog will get rejected.
    //
    //      When sending a failure response, the implementation SHOULD consider
    //      that the payload was not received.
    //
    //      Sending a failure response without a Retry-After header will
    //      terminate the subscription (EvSubscriptionStatus will be called with
    //      eSUBSCRIPTION_FAILED), as stated by RFC 3265, section 3.2.2:
    //
    //          "A NOTIFY request is considered failed if the response times
    //          out, or a non-200 class response code is received which has no
    //          "Retry-After" header and no implied further action which can be
    //          taken to retry the request (e.g., "401 Authorization Required".)
    //
    //          [...]
    //
    //          If the NOTIFY request fails (as defined above) due to an error
    //          response, and the subscription was installed using a soft-state
    //          mechanism, the notifier MUST remove the corresponding
    //          subscription."
    //
    //      When a failure response with a Retry-After is sent, the component
    //      acts as if no NOTIFY was received.
    //
    //==========================================================================
    virtual void EvResourceStateUpdate(IN ISceSubscriber* pSubscriber,
                                       IN ISipServerEventControl* pNotifyServerEventCtrl,
                                       IN const CSipMessageBody& rPayload,
                                       IN bool bReceivedFinalNotify = false) = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  The statuses of a watcher.
    //
    // Description:
    //  This enumeration defines the statuses that can be associated with a
    //  watcher.
    //
    //==========================================================================
    enum EWinfoStatus
    {
        // Description:
        //  The watcher has an active subscription, but the subscription is
        //  pending for authorization.
        eWINFO_PENDING,

        // Description:
        //  The watcher is actively subscribed and has access to the resource
        //  state.
        eWINFO_ACTIVE,

        // Description:
        //  The watcher is waiting for authorization, however it does not have
        //  any active subscription at this time.
        eWINFO_WAITING,

        // Description:
        //  The watcher is no longer subscribed. The event gives more
        //  information as to why it transited into this state.
        eWINFO_TERMINATED,

        // Description:
        //  The status reported by watcher is not valid. This is an
        //  error condition.
        eWINFO_INVALID_STATE
    };

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  The events of a watcher.
    //
    // Description:
    //  This enumeration defines the events that make the watcher status
    //  transit from one state to another. Both the watcher info status and
    //  events are further explained in RFC 3857, 3858 and 3265.
    //
    //==========================================================================
    enum EWinfoEvent
    {
        // Description:
        //  The watcher has just created a new subscription.
        eWINFO_EVSUBSCRIBE,

        // Description:
        //  The authorization policies were updated in a way that now allows
        //  the watcher to access the resource state. If an active subscription
        //  exists, it transits to eWINFO_ACTIVE, otherwise it transits to
        //  eWINFO_TERMINATED and the watcher will be allowed to access the
        //  resource the next time he subscribes.
        eWINFO_EVAPPROVED,

        // Description:
        //  The server has deactivated the subscription, but this does not
        //  imply any changes in authorization. The watcher will usually
        //  immediatly retry to subscribe. This can be used by a server to
        //  force the subscriber to refresh its subscription.
        eWINFO_EVDEACTIVATED,

        // Description:
        //  The subscription was terminated by the server and the watcher
        //  should retry to subscribe some later time.
        eWINFO_EVPROBATION,

        // Description:
        //  The subscription was terminated because of change in authorization
        //  policies.
        eWINFO_EVREJECTED,

        // Description:
        //  This event has two different meaning depending on the state the
        //  watcher is now in. If in state eWINFO_TERMINATED, this means that
        //  the subscriber has either unsubscribed or has failed to refresh its
        //  subscription. When in eWINFO_WAITING, this means that the server
        //  has moved a pending subscription to the waiting state, meaning that
        //  it will reject subscriptions from that watcher until the
        //  authorization policies are updated to allow this watcher access to
        //  the resource state.
        eWINFO_EVTIMEOUT,

        // Description:
        //  The server decided to terminate a pending or waiting subscription
        //  in order to free system resources.
        eWINFO_EVGIVEUP,

        // Description:
        //  The server terminated the subscription because the resource being
        //  watched no longer exists.
        eWINFO_EVNORESOURCE,

        // Description:
        //  The event reported by watcher is not valid. This is an
        //  error condition.
        eWINFO_EVINVALID
    };

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  The informations of a watcher.
    //
    // Description:
    //  This structure holds the information about a single watcher, which is
    //  reported through the EvWatcherInfo event.
    //
    //==========================================================================
    struct SWatcherInfo
    {
        // Description:
        //  Watcher's URI. This is never NULL.
        const char*     m_pszUri;

        // Description:
        //  The watcher ID, it is a unique identifier for the watcher list.
        const char*     m_pszWatcherId;

        // Description:
        //  Watcher's display name. Can be NULL.
        const char*     m_pszDisplayName;

        // Description:
        //  The optionnal display name language tag. Can be NULL.
        const char*     m_pszDisplayNameLang;

        // Description:
        //  The subscription status of the watcher
        EWinfoStatus    m_eStatus;

        // Description:
        //  The event that induced the subscription status described in
        //  m_eStatus.
        EWinfoEvent     m_eEvent;

        // Description:
        //  The number of seconds since the subscription was created.
        unsigned int    m_uDurationSubscribed;

        // Description:
        //  The number of seconds from the current time where the subscription
        //  is due to expire.
        unsigned int    m_uExpiration;

        // Description:
        //  The version of the document which include this watcher state.
        unsigned int    m_uDocVersion;
    };


    //==========================================================================
    //==
    //==  EvWatcherInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that information about a watcher was changed.
    //
    //  Parameters:
    //    pSubscriber:
    //      The subscriber component that is reporting this event.
    //
    //    rWatcherInfo:
    //      Reference to a SWatcherInfo structure that contains information
    //      about the watcher.
    //
    //    bFlushWatcherInfoTable:
    //      A new full watcher information table has been received, so the old
    //      watcher information associated with this subscription must be
    //      flushed.
    //
    //  Description:
    //      Reports that information about a watcher was changed.
    //
    //      This event is reported only when the
    //      ISceSubscriber::SubscribeWatcherInfo was called to create a
    //      subscription to the state of the watchers of a resource.
    //
    //  See Also:
    //      ISceSubscriber::SubscribeWatcherInfo
    //
    //==========================================================================
    virtual void EvWatcherInfo(IN ISceSubscriber* pSubscriber,
                               IN const SWatcherInfo& rWatcherInfo,
                               IN bool bFlushWatcherInfoTable) = 0;


    //==========================================================================
    //==
    //==  EvWatcherInfoUpdateDone
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the manager that there is no more items to report for the
    //      watcher info document.
    //
    //  Description:
    //      Informs the manager that the process of the watcher info document
    //      has been completed and there are no more items to report for the
    //      currently processed NOTIFY request.
    //      This event will be reported only if at least one watcher information
    //      has been reported to the manager.
    //
    //  See Also:
    //      EvWatcherInfo
    //
    //==========================================================================
    virtual void EvWatcherInfoUpdateDone() = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Description:
    //  This structure is used to hold the information about a specific instance
    //  of a device to which the RLS service is subscribed.
    //
    //==========================================================================
    struct SResourceListInstanceInfo
    {
        // Description:
        //  Instance id. All event for this instance will be reported with this
        //  same instance id.
        const char* m_pszInstanceId;

        // Description:
        //  The subscription status.
        ESubscriptionStatus m_eRemoteSubscriptionStatus;

        // Description:
        //  The resource state. Can be NULL if no state information was
        //  received or if this instance points to an other, embedded, RLMI
        //  list.
        const CSipMessageBody* m_pPayload;

    };

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Description:
    //  This structure defines the information that can be reported by the
    //  subscriber when it has subscribed to an URI that is managed by a
    //  resource list server (RLS). This structure is used when reporting
    //  EvResourceListItemUpdate.
    //
    //==========================================================================
    struct SResourceListResourceInfo
    {
        // Description:
        //  The URI of the resource for which this event is reported.
        const char* m_pszResourceUri;

        // Description:
        //  The URI associated with this resource's list. This is usually the
        //  URI used to subscribe to the RLS server.
        const char* m_pszResourceListUri;

        // Description:
        //  A list of XML elements used to represent the display name(s) of the
        //  resource. Use <C>CXmlElement::GetValue()</C> to access the display
        //  name. Multiple display name can be presented in different
        //  languages. In such case, use
        //  <C>CXmlElement::GetAttribute(g_pszXML_NAMESPACE_XML, "lang", &pValue)</C> to
        //  find out the display language used. g_pszXML_NAMESPACE_XML is defined in
        //  SceXml/UaImppXmlDictionary.h.
        CVector<CXmlElement*>   m_vecpDisplayNames;

        // Description:
        //  Vector of instance information. Since a single subscription request
        //  to a resource can result in multiple subscriptions to different
        //  endpoints because of forking, this vector is used to represent all
        //  resulting subscriptions to each end devices. This vector can hold
        //  zero element, in which case it means that the RLS server has not
        //  yet received any information about the resource.
        CVector<SResourceListInstanceInfo*> m_vecInstanceInfo;
    };

    //==========================================================================
    //==
    //==  EvResourceListUpdate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the user that the items of a resource list have their state
    //      updated.
    //
    //  Parameters:
    //    pszResourceListUri:
    //      Pointer to the resource list URI. This URI is used to identify the
    //      list and the items that belong to that list.
    //
    //    rvecpListDisplayName:
    //      The display name associated with this list, found within the list
    //      itself.
    //
    //    bFullState:
    //      Whether or not the list will contain full state information or
    //      partial state information. For full state information, the user must
    //      discard all state it has accumulated for all resources associated
    //      with this list and re-populate its state information from the
    //      subsequent EvResourceListItemUpdate events.
    //
    //  Description:
    //      Informs the user that the items of a resource list have their state
    //      updated.
    //
    //      The state of the resources found on the resource list is provided to
    //      the user in subsequent calls to EvResourceListItemUpdate. All items
    //      that belong on this list will have their
    //      SResourceListResourceInfo::m_pszResourceListUri member of the
    //      pResourceInfo parameter set to the same URI as the
    //      pszResourceListUri parameter of this event.
    //
    //      The example section of EvResourceListItemUpdate details how and when
    //      EvResourceListUpdate is reported in relation with
    //      EvResourceListItemUpdate.
    //
    //  See Also:
    //      EvResourceListItemUpdate
    //
    //==========================================================================
    virtual void EvResourceListUpdate(IN const char* pszResourceListUri,
                                      IN CVector<CXmlElement*>& rvecpListDisplayName,
                                      IN bool bFullState) = 0;

    //==========================================================================
    //==
    //==  EvResourceListUpdateDone
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the user that there is no more item to report for the list.
    //
    //  Description:
    //      Informs the manager that that the process of the root "rlmi" list
    //      has been completed and there are no more items to report for the
    //      currently processed NOTIFY request.
    //
    //  See Also:
    //      EvResourceListUpdate, EvResourceListItemUpdate
    //
    //==========================================================================
    virtual void EvResourceListUpdateDone() = 0;


    //==========================================================================
    //==
    //==  EvResourceListItemUpdate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the state of a resource found on a resource list was
    //      changed.
    //
    //  Parameters:
    //    pSubscriber:
    //      The subscriber object reporting this event.
    //
    //    pResourceInfo:
    //      Pointer to the resource information.
    //
    //  Description:
    //      Reports that the subscription state or the resource state of a
    //      resource to which a Resource List Server (RLS) was subscribed to was
    //      changed. This corresponds to the reception of a NOTIFY request with
    //      a RLMI payload.
    //
    //  Example:
    //      This event is used to report the state of items that are part of a
    //      hierarchical structure. Lets inspect an example to further explain
    //      how and when this event is reported. Assume the user subscribes to
    //      "sip:alice-buddies@example.com", which is a resource managed by a
    //      RLS server. The following RLS document describes what resource list
    //      is associated with this service URI.
    //
    //      <CODE>
    //        \<?xml version="1.0" encoding="UTF-8"?\>
    //        \<rls-services xmlns="urn:ietf:params:xml:ns:rls-services"
    //           xmlns:rl="urn:ietf:params:xml:ns:resource-lists"
    //           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"\>
    //         \<service uri="sip:alice-buddies@example.com"\>
    //          \<resource-list\>http://xcap.example.com/resource-lists/user
    //           s/sip:alice@example.com/index/~~/resource-lists/list%5b@nam
    //           e=%22my-buddies%22%5d\</resource-list\>
    //          \<packages\>
    //           \<package\>presence\</package\>
    //          \</packages\>
    //         \</service\>
    //        \</rls-services\>
    //      </CODE>
    //
    //      Assume the XCAP URL points to the following resource list:
    //
    //      <CODE>
    //         \<list name="my-buddies"\>
    //          \<display-name\>Friends List\</display-name\>
    //
    //          \<entry uri="sip:joe@example.com"\>
    //           \<display-name\>Joe Smith\</display-name\>
    //          \</entry\>
    //
    //          \<entry uri="sip:techsupport@internal.example.com"\>
    //           \<display-name\>Tech Support List\</display-name\>
    //          \</entry\>
    //
    //          \<list name="close-buddies"\>
    //           \<entry uri="sip:will@example.com"\>
    //            \<display-name\>William Roderick\</display-name\>
    //           \</entry\>
    //           \<entry uri="sip:frank@example.com"\>
    //            \<display-name\>Frank Filler\</display-name\>
    //           \</entry\>
    //          \</list\>
    //
    //         \</list\>
    //      </CODE>
    //
    //      Assume that "sip:techsupport@example.com" points to another resource
    //      on a RLS server, which in turn points to the following resource
    //      list.
    //
    //      <CODE>
    //         \<list name="techsupport-team"\>
    //          \<display-name\>Your Friendly Tech Support Team\</display-name\>
    //
    //          \<entry uri="sip:bob@example.com"\>
    //           \<display-name\>Robert Smith\</display-name\>
    //          \</entry\>
    //
    //          \<entry uri="sip:frank@example.com"\>
    //           \<display-name\>Frank Filler\</display-name\>
    //          \</entry\>
    //      </CODE>
    //
    //      A user (probably Alice, but anyone authorized could do so)
    //      subscribes to "sip:alice-buddies@example.com". The RLS server does
    //      its magic and subscribes to the four resources enumerated in the
    //      list named "my-buddies". However, one of this resource is a
    //      subscription to another RLS server found at
    //      "sip:techsupport@internal.example.com" which presumably will
    //      subscribe to an other list of resources.
    //
    //      Assuming the RLS server sends a single notification once it has
    //      gathered all of the resource states, the subscriber component, upon
    //      receiving this notification would report the following (only
    //      presenting relevant parameters).
    //
    //      <CODE>
    //
    //      EvResourceListUpdate(
    //           pszResourceListUri = "sip:alice-buddies@example.com"
    //           rvpListDisplayName[0].GetValue() = "Friends List"
    //           bFullState = true)
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:joe@example.com"
    //           m_pszResourceListUri = "sip:alice-buddies@example.com"
    //           m_vecpDisplayNames[0].GetValue() = "Joe Smith")
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:techsupport@internal.example.com"
    //           m_pszResourceListUri = "sip:alice-buddies@example.com"
    //           m_vecpDisplayNames[0].GetValue() = "Tech Support List"
    //           pPayload = NULL - never a payload for a list subscription)
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:will@example.com"
    //           m_pszResourceListUri = "sip:alice-buddies@example.com"
    //           m_vecpDisplayNames[0].GetValue() = "William Roderick")
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:frank@example.com"
    //           m_pszResourceListUri = "sip:alice-buddies@example.com"
    //           m_vecpDisplayNames[0].GetValue() = "Frank Filler")
    //
    //      EvResourceListUpdate(
    //           pszResourceListUri = "sip:techsupport@internal.example.com"
    //           rvpListDisplayName[0].GetValue() = "Your Friendly Tech Support Team"
    //           bFullState = true)
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:bob@example.com"
    //           m_pszResourceListUri = "sip:techsupport@internal.example.com"
    //           m_vecpDisplayNames[0].GetValue() = "Robert Smith")
    //
    //      EvResourceStateUpdate(
    //           m_pszResourceUri = "sip:frank@example.com"
    //           m_pszResourceListUri = "sip:techsupport@internal.example.com"
    //           m_vecpDisplayNames[0].GetValue() = "Frank Filler")
    //      </CODE>
    //
    //      The subscriber component will report its event in the following way:
    //
    //    - It first report EvResourceListUpdate to inform the user of which
    //      list is currently being updated, and whether or not the previous
    //      state of the resource on the list should be flushed.
    //    - It then reports EvResourceListItemUpdate for <B>ALL</B> items in
    //      the list, including subscriptions to other lists. It is useful to
    //      report this event for subscriptions to other list as it provides the
    //      subscription state to this list, along with its display name as
    //      found on the list that references it.
    //    - If there were any subscription to other lists in the reported
    //      resources, and if there was state associated with these list, then
    //      an EvResourceListUpdate event is reported for this list, and again,
    //      EvResourceListItemUpdate is called for <B>ALL</B> elements on this
    //      list.
    //
    //==========================================================================
    virtual void EvResourceListItemUpdate(IN ISceSubscriber* pSubscriber,
                                          IN const SResourceListResourceInfo* pResourceInfo) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceSubscriberMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceSubscriberMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceSubscriberMgr(IN const ISceSubscriberMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceSubscriberMgr& operator=(IN const ISceSubscriberMgr& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCESUBSCRIBERMGR_H

