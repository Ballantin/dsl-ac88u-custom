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
#ifndef MXG_ISCESUBSCRIBER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCESUBSCRIBER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISCESUBSCRIBERMGR_H
// ISceSubscriberMgr:: ESubscriptionStatus
#include "SceCoreComponents/ISceSubscriberMgr.h"
#endif

#ifndef MXG_SIPEVENTTYPES_H
#include "SceSipTypes/SipEventTypes.h"
#endif


MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CGenParamList;
class CNameAddr;
class CSceCallerPreferences;
class CSipMessageBody;
class ISceUserConfig;
class IUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceSubscriber);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceSubscriber
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
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
// Location:
//   SceCoreComponents/ISceSubscriber.h
//
// See Also:
//   ISceSubscriberMgr
//
//==============================================================================
class ISceSubscriber : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceSubscriber);
//M5T_INTERNAL_USE_END

    //   << Subscriber Management >>

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      subscriber.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pMgr cannot be NULL.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      subscriber object.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISceSubscriberMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the subscriber object with a user configuration object.
    //
    //  Parameters:
    //    pConfig:
    //      Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pConfig cannot be NULL.
    //      - resFE_INVALID_STATE: Configuration already set.
    //      - Other: Method failure.
    //
    //  Description:
    //      Configures the subscriber with a user configuration object.
    //
    //==========================================================================
    virtual mxt_result SetConfiguration(IN IEComUnknown* pConfig) = 0;

    //==========================================================================
    //==
    //==  GetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the subscriber's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the subscriber.
    //
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  GetSubscriptionState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current state.
    //
    //  Returns:
    //      The current state of the component.
    //
    //  Description:
    //      Returns the current state of the component.
    //
    //==========================================================================
    virtual ISceSubscriberMgr:: ESubscriptionStatus GetSubscriptionState() const = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the subscriber.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the subscriber. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual void SetOpaque(IN mxt_opaque opq) = 0;


    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the opaque parameter to the subscriber.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the subscriber. It can be used by
    //      the application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  SetSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the time to wait after a failed subscription attempt and
    //      the next one.
    //
    //  Parameters:
    //    uRetryDelayS:
    //      The number of seconds to wait.
    //
    //  Description:
    //      Configures the number of seconds to wait after a failed subscription
    //      attempt and the next one.
    //
    //      This defaults to 60 seconds.
    //
    //  See Also:
    //      GetSubscriptionRetryDelay
    //
    //==========================================================================
    virtual void SetSubscriptionRetryDelay(IN uint32_t uRetryDelayS) = 0;


    //==========================================================================
    //==
    //==  GetSubscriptionRetryDelay
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the configured subscription retry delay.
    //
    //  Returns:
    //      The subscription retry delay, in seconds.
    //
    //  Description:
    //      Returns the configured subscription retry delay.
    //
    //  See Also:
    //      SetSubscriptionRetryDelay
    //
    //==========================================================================
    virtual uint32_t GetSubscriptionRetryDelay() const = 0;

// BRCM_CUSTOM - [add] Add dynamic threshold support to reg-event subscription
    //==========================================================================
    //==
    //==  SetSubscriptionExpiringThreshold
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the maximum delay between when the ISceSubscriber
    //      component tries to refresh the subscription and the expiration in
    //      seconds.
    //
    //  Parameters:
    //    uNbSeconds:
    //      The number of seconds to set as expiring threshold time of
    //      subscription. Set to 0 to disable dynamic threshold mechanism and
    //      use the stack's default fixed threshold value.
    //
    //  Description:
    //      Configures the maximum delay between when the ISceSubscriber
    //      component tries to refresh the subscription and the expiration in
    //      seconds.
    //
    //      Note that when this function is called with a non-zero threshold
    //      value, the amount of time before the component refreshes the
    //      subscription is not a fixed amount of time and is calculated as
    //      follow. Assume that S_Expiration is the expiration time returned by
    //      the server. If S_Expiration / 2 is less than the configured
    //      threshold, the component will refresh the subscription
    //      (S_Expiration / 2) seconds before the subscription expires. If
    //      S_Expiration / 2 is greater than the configured threshold, then the
    //      component will refresh the subscription "threshold" seconds before
    //      the subscription expires.
    //
    //      This dynamic threshold mechanism is disabled by default, i.e.
    //      uNbSeconds = 0.
    //
    //      This function must be called before Subscribe() or
    //      SubscribeWatcherInfo() is called.
    //
    //==========================================================================
    virtual void SetSubscriptionExpiringThreshold(IN uint32_t uNbSeconds) = 0;
// BRCM_CUSTOM - [end] Add dynamic threshold support to reg-event subscription

    //==========================================================================
    //==
    //==  EnableNotificationParsing
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables or not the parsing of the payload of the notifications.
    //
    //  Parameters:
    //    bEnable:
    //      Controls whether the notifications that contain RLMI or watcher info
    //      data are parsed or not.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_STATE: A subscription is ongoing.
    //
    //  Description:
    //      This method indicates if the RLMI (application/rlmi+xml) or
    //      watcher info payloads (application/watcherinfo+xml) of the
    //      notifications must be parsed or not.  When they are not parsed only
    //      the events ISceSubscriberMgr::EvSubscriptionStatus and
    //      ISceSubscriberMgr::EvResourceStateUpdate are reported by this
    //      component.
    //
    //  Note:
    //      The default setting is to parse the RLMI and watcher info
    //      notifications.
    //
    //  See Also:
    //      ISceSubscriberMgr
    //
    //==========================================================================
    virtual mxt_result EnableNotificationParsing(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  Subscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Subscribes to receive event notifications from a server.
    //
    //  Parameters:
    //    pResourceAor:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the To header. Ownership is taken. Cannot be NULL.
    //
    //    pNotifierRequestUri:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the request-URI. Can be NULL, in which case the URI
    //      found in pResourceAor will be used instead. Ownership is taken.
    //
    //    eEventType:
    //      The type of event to which subscribe.
    //
    //    uSubscriptionPeriodS:
    //      The subscription refresh period in seconds. Cannot be 0.
    //
    //    pCallerPreferences:
    //      The caller preferences to send in the Accept-Contact, Reject-Contact
    //      and Request-Disposition headers. Ownership is taken. May be NULL, in
    //      which case the caller preferences headers are not included in the
    //      request. Ownership is taken.
    //
    //    pCustomParameters:
    //      A pointer to the parameters to be added to the Event header of the
    //      SUBSCRIBE requests. The pointer may be NULL. Ownership is TAKEN.
    //
    //      The "id" parameter should not be present in the list (it is
    //      automatically added). If the "id" parameter is present, its value
    //      will be replaced.
    //
    //    pMessageBody:
    //      A pointer to the message-body to send in the SUBSCRIBE request to be
    //      sent. It can be NULL, which means that no message-body is added to
    //      the request. Ownership is TAKEN.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pResourceAor cannot be NULL and must
    //                                contain a valid URI.
    //                                uSubscriptionPeriodS cannot be 0.
    //      - resFE_INVALID_STATE: Configuration not completed.
    //      - resFE_FAIL: The subscription has failed and cannot be retried.
    //                    The Terminate method must be called to complete the
    //                    termination of the subscription.
    //
    //  Description:
    //      This method sends a SUBSCRIBE request for the specified event type
    //      to pNotifierUri.
    //
    //      Subscription status are reported through the
    //      ISceSubscriberMgr.
    //
    //  See Also:
    //      Unsubscribe
    //
    //==========================================================================
    virtual mxt_result Subscribe(IN TOA CNameAddr* pResourceAor,
                                 IN TOA IUri* pNotifierRequestUri,
                                 IN ESipEventType eEventType,
                                 IN uint32_t uSubscriptionPeriodS,
                                 IN TOA CSceCallerPreferences* pCallerPreferences,
                                 IN TOA CGenParamList* pCustomParameters,
                                 IN TOA CSipMessageBody* pMessageBody) = 0;

    //==========================================================================
    //==
    //==  SubscribeWatcherInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //      Subscribes to find out the watchers of a specific resource.
    //
    //  Parameters:
    //    pResourceAor:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the To header. Ownership is taken.
    //
    //    pNotifierRequestUri:
    //      The address of the resource to which the component must subscribe.
    //      Will be used in the request-URI. Can be NULL, in which case the URI
    //      found in pResourceAor will be used instead. Ownership is taken.
    //
    //    eEventType:
    //      The type of event to which subscribe. There are no restrictions
    //      about the type of event to subscribe. The suffix ".winfo" is added
    //      to the value of the Event header by the SceSubscriber component.
    //
    //    uSubscriptionPeriodS:
    //      The subscription refresh period in seconds. Cannot be 0.
    //
    //    pCallerPreferences:
    //      The caller preferences to send in the Accept-Contact, Reject-Contact
    //      and Request-Disposition headers. Ownership is taken. May be NULL, in
    //      which case the caller preferences headers are not included in the
    //      request. Ownership is taken.
    //
    //    pMessageBody:
    //      Pointer to the payload to send along with the subscribe request.
    //      This payload can be, but is not limited to, a filter document.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_ARGUMENT: pResourceAor cannot be NULL and must
    //                                contain a valid URI.
    //                                uSubscriptionPeriodS cannot be 0.
    //      - resFE_INVALID_STATE: Configuration not completed.
    //      - resFE_FAIL: The subscription has failed and cannot be retried.
    //                    The Terminate method must be called to complete the
    //                    termination of the subscription.
    //
    //  Description:
    //      This method sends a SUBSCRIBE request in order to detect the current
    //      watchers of a specific resource. The subscriber component will
    //      interpret the XML payload it receives in the NOTIFY request and
    //      report ISceSubscriberMgr::EvWatcherInfo with pre-processed
    //      information about the watchers.
    //
    //      Note that it is possible to subscribe to watcher information using
    //      the Subscribe() API instead. The event that will then be reported
    //      will be EvResourceStateUpdate with the full XML information received
    //      in the NOTIFY request.
    //
    //  See Also:
    //      Subscribe, Unsubscribe
    //
    //==========================================================================
    virtual mxt_result SubscribeWatcherInfo(IN TOA CNameAddr* pResourceAor,
                                            IN TOA IUri* pNotifierRequestUri,
                                            IN ESipEventType eEventType,
                                            IN uint32_t uSubscriptionPeriodS,
                                            IN TOA CSceCallerPreferences* pCallerPreferences,
                                            IN TOA CSipMessageBody* pMessageBody) = 0;

    //==========================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Refreshes the established subscription.
    //
    //  Parameters:
    //    pCustomParameters:
    //      A pointer to the parameters to be added to the Event header of the
    //      SUBSCRIBE requests. The pointer may be NULL. Ownership is TAKEN.
    //
    //      The "id" parameter should not be present in the list (it is
    //      automatically added). If the "id" parameter is present, its value
    //      will be replaced.
    //
    //      Note that this parameter will override the same parameter
    //      previously specified in Subscribe.
    //
    //    pMessageBody:
    //      A pointer to the message-body to send in the SUBSCRIBE request to be
    //      sent. It can be NULL, which means that no message-body is added to
    //      the request. Ownership is TAKEN.
    //
    //      Note that this parameter will override the same parameter
    //      previously specified in Subscribe.
    //
    //  Returns:
    //      - resS_OK: Method success, the refresh has been sent.
    //      - resSW_ASYNC_PROCESSING: The refresh will be sent later.
    //      - resFE_INVALID_STATE: Cannot refresh a subscription in this state,
    //      - Other: Method failure.
    //
    //  Description:
    //      This method sends a SUBSCRIBE request to refresh the established
    //      subscription.
    //
    //      Subscription status are reported through the ISceSubscriberMgr.
    //
    //  See Also:
    //      Subscribe, Unsubscribe
    //
    //==========================================================================
    virtual mxt_result Refresh(IN TOA CGenParamList* pCustomParameters,
                               IN TOA CSipMessageBody* pMessageBody) = 0;

    //==========================================================================
    //==
    //==  Unsubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unsusbcribes from the server.
    //
    //  Returns:
    //      - resS_OK: Method success.
    //      - resFE_INVALID_STATE: Configuration not completed.
    //      - Other: Method failure.
    //
    //  Description:
    //      Unsubscribes from the server or stops trying to subscribe to the
    //      server. This can be called only after Subscribe has been called and
    //      while the subscription state is not in eTERMINATED, eUSER_TERMINATED
    //      or eSUBSCRIPTION_FAILED.
    //
    //      The caller must wait for
    //      ISceSubscriberMgr::EvSubscriptionStatus(eTERMINATED) before
    //      releasing the component implementing this interface.
    //
    //  See Also:
    //      Subscribe
    //
    //==========================================================================
    virtual mxt_result Unsubscribe() = 0;

// BRCM_CUSTOM - [add] Change the subscriber's state to handle NOTIFY rejected
    //==========================================================================
    //==
    //==  UnsubscribeSilently
    //==
    //==========================================================================
    //
    //  Summary:
    //      Changes the state to eUNSUBSCRIBING and set m_bEvSuccess to true.
    //
    //  Description:
    //      Calling this function will prevent the subscriber from sending
    //      UNSUBSCRIBE messages.
    //      When SendResponse() is called later, the subscriber will be
    //      terminated.
    //
    //  See Also:
    //      SendResponse
    //
    //==========================================================================
    virtual void UnsubscribeSilently() = 0;
// BRCM_CUSTOM - [end] Change the subscriber's state to handle NOTIFY rejected

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Immediately terminates the subscriber component.
    //
    //  Description:
    //      Immediately terminates the subscriber component. If this subscriber
    //      component is handling a subscription, it is immediately removed
    //      without sending an unsubscribe request.
    //
    //      The manager must expect a call to EvSubscriptionStatus(eTERMINATED)
    //      while this method is called, at which time the manager can release
    //      the reference it has on the subscriber component.
    //
    //      This method should only be used on abnormal scenarios where the
    //      user somehow has knowledge that the subscription is terminated or
    //      when it knows that an unsubscribe request would fail. The
    //      Unsubscribe method should otherwise be used.
    //
    //  See Also:
    //      Unsubscribe
    //
    //==========================================================================
    virtual void Terminate() = 0;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    //==========================================================================
    //==
    //==  TerminateImmediately
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminate this call component immediately and silently.
    //
    //==========================================================================
    virtual void TerminateImmediately() = 0;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //==========================================================================
    //==
    //==  OverrideTracingNodes
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the default tracing nodes.
    //
    //  Parameters:
    //    rNewComponentNode:
    //      The new component node to use for tracing.
    //
    //    rNewEComNode:
    //      The new ECOM node to use for tracing.
    //
    //  Description:
    //      This method overrides the normal tracing nodes of the component
    //      and its base component with new ones to use. This enables the
    //      component user, either the application or another component, to
    //      specifically trace the used subscriber with different nodes than
    //      the default ones and thus making it easier to trace it later on.
    //
    //==========================================================================
    virtual void OverrideTracingNodes(IN STraceNode& rNewComponentNode,
                                      IN STraceNode& rNewEComNode) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceSubscriber() {}

    // Summary:
    //  Destructor.
    virtual ~ISceSubscriber() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceSubscriber(IN const ISceSubscriber& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceSubscriber& operator=(IN const ISceSubscriber& rSrc);

//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCESUBSCRIBER_H
