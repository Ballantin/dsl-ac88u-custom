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
#ifndef MXG_ISCENOTIFIER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCENOTIFIER_H
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

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class ISceNotifierMgr;
class CHeaderList;
class CSipMessageBody;
class CSipPacket;
class ISceUserConfig;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceNotifier);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceNotifier
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface is used to manage the subscriptions of any event type. 
//
// Description:
//  The notifier can be used to manage the subscriptions of any event type. 
//
//  The notifier does not have its own configuration interface in the user
//  configuration. It must be configured using the proper method in this
//  interface to do so.
//
// Location:
//   SceCoreComponents/ISceNotifier.h
//
// See Also:
//   ISceNotifierHandler, ISceNewNotifierMgr, ISceNotifierMgr
//
//==============================================================================
class ISceNotifier : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceNotifier);
//M5T_INTERNAL_USE_END

    //   << Notifier Management >>

    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this
    //      notifier.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resS_OK: The manager has been set.
    //      - resFE_INVALID_ARGUMENT: Could not set a NULL manager.
    //
    //  Description:
    //      Configures the manager to which events are reported for this
    //      notifier object.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN ISceNotifierMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the notifier object with a user configuration object.
    //
    //  Parameters:
    //    pConfig:
    //      Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resS_OK: Configuration has been correctly set.
    //      - resFE_INVALID_ARGUMENT: Could not use a NULL configuration or
    //            could not query the ISceUserConfig interface.
    //      - others: Could not correctly set the configuration.
    //
    //  Description:
    //      Configures the notifier with a configuration object.
    //      
    //  See Also:
    //      ConfigureComponent
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
    //      Gets the notifier's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the notifier.
    //      
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the notifier.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the notifier. It can be used by the
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
    //      Gets the opaque parameter to the notifier.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the notifier. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;

    //==========================================================================
    //==
    //==  ConfigureComponent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the notifier object internals.
    //
    //  Parameters:
    //    uDefaultExpirationTimeSec:
    //      Time before subscription expires. The default is 3600 seconds.
    //
    //    uMinimumExpiresSec:
    //      Minimum expiration time allowed for a new subscription.
    //      The default is 60 seconds.
    //
    //    uRetryDelaySec:
    //      Delay to wait for between Notify retries. The default is 60 seconds.
    //
    //    uNumberOfRetries:
    //      Number of times to retry a Notify before failing completely.
    //      The default is 5 retries.
    //
    //  Description:
    //      Configures the notifier object internals.
    //      
    //      This method DOES NOT replace SetConfiguration().
    //      
    //  See Also:
    //      SetConfiguration
    //
    //==========================================================================
    virtual void ConfigureComponent(IN uint32_t uDefaultExpirationTimeSec = 3600,
                                    IN uint32_t uMinimumExpiresSec = 60,
                                    IN uint32_t uRetryDelaySec = 60,
                                    IN uint32_t uNumberOfRetries = 5) = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   Possible state of accepted subscriptions.
    //
    // Description:
    //  This enumeration defines the possible states for accepted subscriptions.
    //
    //==========================================================================
    enum ESubscriptionState
    {
        // Description:
        // The subscription is accepted and active.
        eACTIVE,
        
        // Description:
        // The subscription is pending.
        ePENDING
    };



    //==========================================================================
    //==
    //==  AcceptSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Accepts an incoming subscription request.
    //
    //  Parameters:
    //    eState:
    //      This defines how the subscription is accepted.
    //
    //    uExpirationSec:
    //      The number of seconds that the subscription should be valid. This
    //      value is taken and placed within an Expires header. This value can
    //      be lower than or equal to the uRequestedExpirationSec parameter of
    //      ISceNotifierMgr::EvNewSubscription. To negotiate a higher
    //      expiration interval, the subscription request must be rejected with
    //      a "423 Interval Too Small" and a Min-Expires header identifying the
    //      minimal acceptable expiration time.
    //
    //      This value can be zero, which has the effect of accepting and
    //      immediately terminating the subscription. The notifier can use this
    //      to force the subscribers to pool for resource state. A NOTIFY
    //      request must be sent with TerminateSubscription even if
    //      uExpirationSec is set to zero. In this case, the termination reason
    //      should be ETerminatedReason::eTIMEOUT.
    //
    //  Returns:
    //      - resS_OK: The subscription is accepted.
    //      - resFE_INVALID_STATE: Could not accept the subscription.
    //      - others: An error happened while trying to reject the
    //                subscription.
    //
    //  Description:
    //      Accepts an incoming subscription request that was previously
    //      reported through ISceNotifierMgr::EvNewSubscription.
    //
    //      In order to be RFC 3265 compliant, the user of this component must
    //      make sure to send a NOTIFY request (using Notify) immediately after
    //      accepting a subscription. If eState is eACTIVE, the NOTIFY should
    //      contain the state of the resource the subscription is for. A NOTIFY
    //      must be sent even if eState is ePENDING, but such notifications will
    //      usually not include the resource state.
    //
    //      This cannot be called if a subscription was previously rejected with
    //      RejectSubscription.
    //
    //      To terminate a previously accepted subscription, the user must call
    //      TerminateSubscription.
    //
    //  See Also:
    //      RejectSubscription, TerminateSubscription, ESubscriptionState
    //
    //==========================================================================
    virtual mxt_result AcceptSubscription(IN ESubscriptionState eState,
                                          IN uint32_t uExpirationSec) = 0;
    

    //==========================================================================
    //==
    //==  RejectSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Rejects a subscription.
    //
    //  Parameters:
    //    uResponseCode:
    //      The error response code to send.
    //
    //    pExtraHeaders:
    //      Additional SIP headers to send along with the response. NULL for
    //      none.
    //
    //  Returns:
    //      - resS_OK: The subscription is rejected successfully.
    //      - resFE_FAIL: Could not reject the subscription.
    //      - others: An error happened while trying to reject the
    //                subscription.
    //
    //  Description:
    //      Rejects a subscription request after it was previously reported
    //      through ISceNotifierMgr::EvNewSubscription.
    //
    //      This cannot be called if a subscription was previously accepted with
    //      AcceptSubscription.
    //
    //  See Also:
    //      AcceptSubscription
    //
    //==========================================================================
    virtual mxt_result RejectSubscription(IN uint16_t uResponseCode,
                                          IN TO CHeaderList* pExtraHeaders) = 0;

    //==========================================================================
    //==
    //==  Notify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the subscriber.
    //
    //  Parameters:
    //    eState:
    //      The subscription state. Pending subscriptions should eventually
    //      transit to the active state or terminated state. Active
    //      subscriptions can only transit to the terminated state. Transiting
    //      to the terminated state is done by calling TerminateSubscription
    //      only.
    //
    //    pBody:
    //      Payload to send along with the NOTIFY request. Ownership is TAKEN.
    //      Can be NULL, in which case no payload is sent.
    //
    //  Returns:
    //      - resS_OK: The NOTIFY is sent successfully.
    //      - resFE_INVALID_STATE: Could not send the NOTIFY request.
    //      - others: An error happened while trying to send the NOTIFY request.
    //
    //  Description:
    //      Notifies the subscriber. There are three different times when Notify
    //      must be called:
    //
    //      1) Immediately after accepting a subscription with
    //         AcceptSubscription. eState can be ePENDING or eACTIVE.
    //
    //      2) When a pending subscription is transiting from the ePENDING state
    //         to the eACTIVE state.
    //
    //      3) Whenever the state of the subscribed-to resource changes while
    //         the subscription itself is in the eACTIVE state.
    //
    //      In case the last NOTIFY request fails, it will be automatically
    //      retried as per the configured delay until either the request
    //      succeeds or all the number of retries have been tried. If it can no
    //      longer retry, the component fails completely.
    //
    //      The last sent NOTIFY will always be the one that will be retried
    //      until success. In case a NOTIFY is trying to retry when a new one
    //      is sent, no new retry for the old one will be attempted.
    //
    //  See Also:
    //      SetConfiguration, AcceptSubscription, TerminateSubscription
    //
    //==========================================================================
    virtual mxt_result Notify(IN ESubscriptionState eState,
                              IN TO CSipMessageBody* pBody) = 0;


    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   Possible reasons associated with terminated subscriptions.
    //
    // Description:
    //  This enumeration defines the RFC 3265 reasons associated with terminated
    //  subscriptions.
    //
    //==========================================================================
    enum ETerminatedReason
    {
        // Description:
        // From RFC 3265: "The subscription has been terminated, but the
        // subscriber SHOULD retry immediately with a new subscription. One
        // primary use of such a status code is to allow migration of
        // subscriptions between nodes." The "retry-after" parameter has no
        // semantics for "deactivated"."
        eDEACTIVATED,

        // Description:
        // From RFC 3265: "The subscription has been terminated, but the client
        // SHOULD retry at some later time. If a "retry-after" parameter is
        // also present, the client SHOULD wait at least the number of seconds
        // specified by that parameter before attempting to re-subscribe."
        ePROBATION,

        // Description:
        // From RFC 3265: "The subscription has been terminated due to change
        // in authorization policy. Clients SHOULD NOT attempt to re-subscribe.
        // The "retry-after" parameter has no semantics for "rejected"."
        eREJECTED,

        // Description:
        // From RFC 3265: "The subscription has been terminated because it was
        // not refreshed before it expired. Clients MAY re-subscribe
        // immediately. The "retry-after" parameter has no semantics for
        // "timeout"."
        eTIMEOUT,

        // Description:
        // From RFC 3265: "The subscription has been terminated because the
        // notifier could not obtain authorization in a timely fashion. If a
        // "retry- after" parameter is also present, the client SHOULD wait at
        // least the number of seconds specified by that parameter before
        // attempting to re-subscribe; otherwise, the client MAY retry
        // immediately, but will likely get put back into pending state."
        eGIVEUP,

        // Description:
        // From RFC 3265: "The subscription has been terminated because the
        // resource state which was being monitored no longer exists. Clients
        // SHOULD NOT attempt to re-subscribe. The "retry-after" parameter has
        // no semantics for "noresource"."
        eNO_RESOURCE,

        // Description:
        // Sends no reason for subscription termination, in which case the
        // subscriber may retry to subscribe at any time, unless retry-after
        // period is specified.
        eNO_REASON
    };


    //==========================================================================
    //==
    //==  TerminateSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the subscription by sending a final NOTIFY to the
    //      subscriber.
    //
    //  Parameters:
    //    eReason:
    //      The reason why the subscription was terminated.
    //
    //    pBody:
    //      Payload to send along with the final NOTIFY request. Ownership is
    //      TAKEN. Can be NULL, in which case no payload is sent.
    //
    //    puRetryAfterSec:
    //      Pointer to an unsigned integer defining the number of seconds the
    //      subscriber should wait before it can re-subscribe to the resource.
    //      Can be NULL, in which case no Retry-After header will be included in
    //      the NOTIFY.
    //
    //  Returns:
    //      - resS_OK: The subscription is successfully terminated.
    //      - resFE_INVALID_STATE: Could not send the NOTIFY request to
    //           terminate.
    //      - others: An error prevents the successful termination of the
    //           current subscription.
    //
    //  Description:
    //      Terminates the subscripion by sending a final NOTIFY with the
    //      Subscription-State header set to terminated.
    //
    //      This method must always be called when a subscription, whether
    //      pending or accepted, is terminated. 
    //
    //  See Also:
    //      AcceptSubscription
    //
    //==========================================================================
    virtual mxt_result TerminateSubscription(IN ETerminatedReason eReason,
                                             IN TO CSipMessageBody* pBody,
                                             IN unsigned int* puRetryAfterSec) = 0;


// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    //==========================================================================
    //==
    //==  TerminateImmediately
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminate this notifier component immediately and silently.
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
    //      specifically trace the used notifier with different nodes than
    //      the default ones and thus making it easier to trace it later on.
    //
    //==========================================================================
    virtual void OverrideTracingNodes(IN STraceNode& rNewComponentNode,
                                      IN STraceNode& rNewEComNode) = 0;

//M5T_INTERNAL_USE_BEGIN
    //-- << Internal methods >>

    //==========================================================================
    //==
    //==  HandleIncomingSubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that informs the ISceNotifier to proceed with an
    //      incoming SUBCRIBE request.
    //
    //  Parameters:
    //      rRequest:
    //       The request to handle.
    //
    //  Returns:
    //      - resS_OK: The SUBSCRIBE is successfully handled.
    //      - resFE_FAIL: The SUBSCRIBE has not been handled.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method must be called after the notifier has been configured
    //      with its user configuration object (see SetConfiguration) and
    //      with its manager (see SetManager).
    //
    //      It informs the notifier that it can proceed with the incoming
    //      SUBSCRIBE request.
    //
    //      This method does not have to be called by the application. It is
    //      called automatically by the UA SSP after reporting the
    //      ISceNewNotifierMgr::EvNewNotifierObject.
    //
    //  See Also:
    //      SetConfiguration, SetManager
    //
    //==========================================================================
    virtual mxt_result HandleIncomingSubscribe(IN const CSipPacket& rRequest) = 0;


    //==========================================================================
    //==
    //==  TerminateNotifier
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that terminates the notifier.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method will call EvNotifierTerminated on the manager so it is
    //      important that the Notifier is terminated when this function is
    //      called.
    //
    //==========================================================================
    virtual void TerminateNotifier() = 0;
//M5T_INTERNAL_USE_END

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceNotifier() {}

    // Summary:
    //  Destructor.
    virtual ~ISceNotifier() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceNotifier(IN const ISceNotifier& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceNotifier& operator=(IN const ISceNotifier& rSrc);


//-- Hidden Data Members.
protected:
private:

};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCENOTIFIER_H
