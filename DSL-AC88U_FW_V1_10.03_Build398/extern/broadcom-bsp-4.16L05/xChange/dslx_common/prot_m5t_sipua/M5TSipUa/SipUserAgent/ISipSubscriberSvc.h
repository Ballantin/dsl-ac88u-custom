//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_ISIPSUBSCRIBERSVC_H
#define MXG_ISIPSUBSCRIBERSVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#include "Cap/CMap.h"

#include "SipParser/CToken.h"

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CGenParamList;
class CHeaderList;
class CSipMessageBody;
class CString;
class ISipClientTransaction;
class ISipSubscriberMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipSubscriberSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipSubscriberSvc
//========================================
// <FLAG New in 4.1.4>
// <FLAG Updated behavior in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This interface is used to act as a subscriber as defined in RFC 3265. It
//   lets the application send SUBSCRIBE and receive NOTIFY for multiple Events.
//   By default, the subscriber service does not handle any event types
//   and will reject all subscrition requests.
//   Supported events are added through the AddEvent method. Once event types
//   are thus added they are correctly handled.
//
//   This service reports events received to the application through the
//   ISipSubscriberMgr interface.
//
// #The ISipSubscriberSvc is an ECOM object#
//
//  The ISipSubscriberSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipSubscriberSvc</B>
//
//  <B>Interface Id: IID_ISipSubscriberSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Updated behavior:
//   Previously, each event type had its own specific subscriber and
//   notifier services, with its own interface and implementation classes.
//   Thus, for a SIP context to support different event types, many
//   subscriber and/or notifier services had to be individually created
//   and attached to the associated ISipContext.
//   Moreover, new event types required the creation of new child classes
//   deriving from base subscriber or notifier classes, along with supporting
//   infrastructure. This scheme was cumbersome and made supporting new
//   event types unecessarily complex.
//
//  It follows from the foregoing that:
//   - Supporting many different event types in the same ISipContext
//     now requires creating a single subscription or notification
//     service, upon which AddEvent() is called many times (once for
//     each event type to support). The service is then added to the context.
//     This previously required many different services to be created
//     and attached.
//   - Existing user-defined subscription or notification
//     services should be removed from the code base and replaced
//     with the new subscriber or notifier service.
//     Of course, appropriate calls to AddEvent() should also be made.
//     For instance, an "ISipSubscriberProprietaryEventSvc" subscription
//     service should be replaced with a plain ISipSubscriberSvc upon which
//     AddEvent("ProprietaryEvent", ...) is called.
//   - Adding new user-defined subscription event types is now trivial.
//
// Warning:
//   Old subscriber and notifier services are deprecated and are now removed. By
//   old services we refer to interfaces that were previously named
//   ISipNotifier[EventName]Svc and ISipSubscriber[EventName]Svc.
//
// Location:
//   SipUserAgent/ISipSubscriberSvc.h
//
// See Also:
//   ISipSubscriberMgr
//
//==EDOC========================================================================
class ISipSubscriberSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipSubscriberSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager of this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT if pMgr is NULL.  The manager is not
    //          changed in this case.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events from this
    //      service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipSubscriberMgr* pMgr) = 0;



    //  Summary:
    //    Possible threshold types.
    //
    //  Description:
    //   These are the possible threshold types that can be used to calculate
    //   the time for EvExpiring.
    enum EThresholdType
    {
        //  Description:
        //   Fixed value threshold.
        eTHRESHOLD_FIXED,

        //  Description:
        //   Dynamic value threshold.
        eTHRESHOLD_DYNAMIC
    };

    //==SDOC====================================================================
    //==
    //==  AddEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds an event type to be supported by this service.
    //
    // Returns:
    //    resS_OK:
    //      Success.
    //
    //  Parameters:
    //    rstrEvent:
    //      Event type to support, e.g., "message-summary".
    //
    //    uDefaultExpiration:
    //      Default expiration for this event type, in seconds.
    //
    //  Description:
    //      Adds an event type to be supported by this service.
    //
    //==EDOC====================================================================
    virtual mxt_result AddEvent(IN const CString& rstrEvent,
                                IN unsigned int uDefaultExpiration) = 0;

     //==SDOC====================================================================
    //==
    //==  SetExpiringThreshold
    //==
    //==========================================================================
    // <FLAG Updated behavior in 4.1.4>
    //
    //  Summary:
    //      Sets the time to warn the application before a subscription expires.
    //
    // Returns:
    //    resS_OK:
    //      Success.
    //    resFE_INVALID_ARGUMENT:
    //      The event type is unknown to this service.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    uThresholdSec:
    //      The time, in seconds, before expiration time that the EvExpiring is
    //      fired.
    //
    //    eThresholdType:
    //      The threshold type to use. Default value is eTHRESHOLD_FIXED.
    //
    //  Description:
    //      Sets the amount of time that should remain to a subscription before
    //      EvExpiring is called for that subscription. The amount of time is fixed 
    //      when eThresholdType is eTHRESHOLD_FIXED.
    //
    //      This method affects only future subscriptions. This means that if
    //      there is an active subscription while this method is called, the
    //      EvExpiring event is called according to the previous threshold
    //      value.
    //
    //      If the expires value is smaller than or equal to the threshold, the
    //      EvExpiring event is never called, EvExpired is instead called
    //      directly.
    //
    //      When eThresholdType is eTHESHOLD_DYNAMIC and when the threshold value 
    //      is smaller than half the expires value, the expiring threshold is  
    //      automatically adjusted to half the amount of time of the received   
    //      expires value. This ensures that EvExpiring event is always reported.
    //
    //  See Also:
    //      Refresh, Terminate, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result SetExpiringThreshold(IN const CString& rstrEvent,
                                            IN unsigned int uThresholdSec,
                                            IN EThresholdType eThresholdType = eTHRESHOLD_FIXED) = 0;

    // Description:
    //  Indicate how the subscription is created.
    //
    // See Also:
    //  CreateSubscription
    enum EType
    {
        // Description:
        //  The subscription is established by other means than SIP
        eIMPLICIT,
        // Description:
        //  The subscription has been established by SIP  signalling but not
        //  via a SUBSCRIBE request
        eEXPLICIT
    };

    //==SDOC====================================================================
    //==
    //==  CreateSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates a subscription without sending a SUBSCRIBE request.
    //
    //  Parameters:
    //    rstrEvent:
    //      Event type for which to create the subscription,
    //      e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter that should be found in the Event header field of
    //      the NOTIFY received for that subscription. When the empty string is
    //      used, the id parameter must not be in the NOTIFY to match that
    //      subscription.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      for that event type is used.
    //
    //    eType:
    //      Can be eEXPLICIT, which means that the subscription has been
    //      established by SIP signaling but by other means than a SUBSCRIBE
    //      request. When the subscription is not established via SIP, the
    //      value of this parameter should be eIMPLICIT.
    //
    //    pCustomParameters:
    //      A pointer to the parameters added to the Event header. The pointer
    //      may be NULL. Ownership is TAKEN. This list should not contain the
    //      parameter "id" since it will be added from the strId parameter.
    //      If the "id" parameter is present, its value is replaced with strId.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id is already used by another
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set or
    //      Clear has been called on the context.
    //
    //      - resS_OK: The subscription is successfully created and ready to
    //      receive NOTIFY requests.
    //
    //  Description:
    //      Creates a subscription without sending a SUBSCRIBE request. Set the
    //      eType parameter to eEXPLICIT when the subscription is created by
    //      some other SIP means (eg. REFER request). Set the eType parameter to
    //      eIMPLICIT when the subscription is not created via SIP, such as a
    //      common agreement. The value of this parameter dictates what is to be
    //      done when the subscription is terminated or refreshed. When the
    //      subscription is explicit, calling Terminate or Refresh triggers a
    //      SUBSCRIBE request to be sent. When the subscription is implicit,
    //      calling those methods only has impact on the internal state and does
    //      not trigger a SUBSCRIBE request to be sent.
    //
    //      When establishing an explicit subscription through a non-SUBSCRIBE
    //      request, the application must not wait for the response to that
    //      request to call CreateSubscription. This is because the first NOTIFY
    //      could arrive before the final response to the request (because of
    //      forking and record routing). However, should the method that was
    //      supposed to create a subscription fails, the application should
    //      call AbortSubscription for this subscription as it will never
    //      receive a NOTIFY.
    //
    //      This method should also be used when the application wants to handle
    //      supplemental branches of a forked SUBSCRIBE. In this case, the
    //      NOTIFY can arrive with a different From tag than the To tag in the
    //      response to the SUBSCRIBE. In order for the subscriber service on
    //      the new context to accept that NOTIFY, the application must call
    //      CreateSubscription with the proper parameters.
    //
    //      Otherwise, the subscription created by this method works exactly the
    //      same way as the one created by the Subscribe method.
    //
    //  See Also:
    //      AbortSubscription, Subscribe, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result CreateSubscription(
                                IN const CString&    rstrEvent,
                                IN const CString&    rstrId,
                                IN unsigned int      uExpirationSec,
                                IN EType             eType,
                                IN TO CGenParamList* pCustomParameters) = 0;

    //==SDOC====================================================================
    //==
    //==  AbortSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates a subscription without sending a SUBSCRIBE request.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter that should be found in the Event header field of
    //      the NOTIFY received for that subscription. When the empty string is
    //      used, the id parameter must not be in the NOTIFY to match that
    //      subscription.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: There is no subscription with this id in
    //      this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The subscription for this id cannot be
    //      aborted because it is already terminated.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set or
    //      Clear has been called on the context.
    //
    //      - resS_OK: The subscription for this id is correctly aborted.
    //
    //  Description:
    //      Terminates a subscription without sending a SUBSCRIBE request with
    //      the Expires header field set to 0. This method should be called only
    //      when an explicit subscription was created with the
    //      CreateSubscription method but the non-SUBSCRIBE request that was
    //      supposed to create the subscription received a final negative
    //      response.
    //
    //      When the method is called for an active implicit subscription
    //      (created through the CreateSubscription method with the eType
    //      parameter set to eIMPLICIT), it has the exact same effect as
    //      calling Terminate for that subscription.
    //
    //      When the method is called for an active subscription created through
    //      the Subscribe method, it has the effect of being non conformant with
    //      RFC 3265 since it terminates the subscription without sending a
    //      SUBSCRIBE request with the Expires header set to 0.
    //
    //      In every other case, the method returns an error.
    //
    //  See Also:
    //      CreateSubscription, Subscribe, Terminate, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result AbortSubscription(IN const CString& rstrEvent,
                                         IN const CString& rstrId) = 0;

    //==SDOC====================================================================
    //==
    //==  Subscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a SUBSCRIBE request to establish a subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      Event type for which to create the subscription,
    //      e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter put in the Event header field of the SUBSCRIBE
    //      request sent. It should also be in the Event header field of the
    //      NOTIFY received for that subscription. When the empty string is
    //      used, the id parameter is not in the SUBSCRIBE sent and it must not
    //      be in the NOTIFY to match that subscription.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      for that event type is used.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      is TAKEN.
    //
    //    pCustomParameters:
    //      A pointer to the parameters to be added to the Event header.
    //      The pointer may be NULL. Ownership is TAKEN. This list should not
    //      contain the parameter "id" since it will be added from the strId
    //      parameter. If the "id" parameter is present, its value is replaced
    //      with strId.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id is already used by another
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set,
    //      Clear has been called on the context or the attached user agent
    //      service is not properly configured (e.g.: contact list is empty).
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request is successfully sent.
    //
    //  Description:
    //      Creates a subscription by sending a SUBSCRIBE request. That
    //      subscription works exactly as an explicit subscription created by
    //      the CreateSubscription method. The status of this request is given
    //      to the application through the ISipSubscriberMgr interface. Note
    //      that the initial NOTIFY message for this subscription can be
    //      received before the response to the SUBSCRIBE request.
    //
    //      The expiration time is a suggestion to the remote peer. The actual
    //      expiration time can be lowered by the peer. If the subscription is
    //      still active at the threshold time, before the expiration arrives,
    //      the EvExpiring event is triggered for that subscription. The
    //      application can then decide to continue the subscription by calling
    //      Refresh() or to terminate it by calling Terminate(). If none of
    //      these actions are taken by the application before the subscription
    //      expires, the EvExpired event is called for that subscription.
    //
    //  See Also:
    //      Refresh, Terminate, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Subscribe(
                            IN    const CString&          rstrEvent,
                            IN    const CString&          rstrId,
                            IN    unsigned int            uExpirationSec,
                            IN    mxt_opaque              opqTransaction,
                            IN TO CHeaderList*            pExtraHeaders,
                            IN TO CSipMessageBody*        pMessageBody,
                            IN TO CGenParamList*          pCustomParameters,
                            OUT   ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Fetch
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a SUBSCRIBE request only to obtain a single NOTIFY.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter to be put in the Event header field of the
    //      SUBSCRIBE request sent. It should also be in the Event header field
    //      of the NOTIFY received for that subscription. When the empty
    //      string is used, the id parameter is not in the SUBSCRIBE sent and it
    //      must not be in the NOTIFY to match that subscription.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      is TAKEN.
    //
    //    pCustomParameters:
    //      A pointer to the parameters added to the Event header. The pointer
    //      may be NULL. Ownership is TAKEN. This list should not contain the
    //      parameter "id" since it will be added from the strId parameter.
    //      If the "id" parameter is present, its value is replaced with strId.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id is already used by another
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set,
    //      Clear has been called on the context or the attached user agent
    //      service is not properly configured (e.g.: contact list is empty).
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request is successfully sent.
    //
    //  Description:
    //      Sends a SUBSCRIBE request but with the Expires header set to 0. If
    //      the request is successful, it makes the notifier send only a single
    //      NOTIFY (that is reported through the ISipSubscriberMgr). This method
    //      does not create a subscription. It cannot be used to terminate an
    //      active subscription neither.
    //
    //  See Also:
    //      AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Fetch(IN     const CString&          rstrEvent,
                             IN     const CString&          rstrId,
                             IN     mxt_opaque              opqTransaction,
                             IN  TO CHeaderList*            pExtraHeaders,
                             IN  TO CSipMessageBody*        pMessageBody,
                             IN  TO CGenParamList*          pCustomParameters,
                             OUT    ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Refresh
    //==
    //==========================================================================
    //
    //  Summary:
    //      Extends an active subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter that is put in the Event header field of the
    //      SUBSCRIBE request sent. It should also be in the Event header field
    //      of the NOTIFY received for that subscription. When the empty
    //      string is used, the id parameter is not in the SUBSCRIBE sent and
    //      it must not be in the NOTIFY to match that subscription.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      for that event type is used.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //    pCustomParameters:
    //      A pointer to the parameters added to the Event header. The pointer
    //      may be NULL, in which case, the previous set of parameters is
    //      used. Otherwise, if this parameter is not NULL, the new set of
    //      parameters is used. In the special case when pCustomParameters
    //      is not NULL but no parameters are specified in pCustomParameters,
    //      all the previous parameters are removed and no parameter is used.
    //      Ownership is TAKEN. This list should not contain the parameter
    //      "id" since it will be added from the strId parameter.
    //      If the "id" parameter is present, its value is replaced with strId.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set or
    //      Clear has been called on the context.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request is successfully sent.
    //
    //  Description:
    //      This method acts exactly as Subscribe except that it can only be
    //      called on an active subscription. If successful, it extends the
    //      amount of time for which the subscription remains active.
    //
    //  See Also:
    //      Subscribe, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Refresh(IN const CString& rstrEvent,
                               IN const CString& rstrId,
                               IN unsigned int uExpirationSec,
                               IN mxt_opaque opqTransaction,
                               IN TO CHeaderList* pExtraHeaders,
                               IN TO CSipMessageBody* pMessageBody,
                               OUT ISipClientTransaction*& rpTransaction,
                               IN TOA CGenParamList* pCustomParameters = NULL) = 0;

    //==SDOC====================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates an active subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter that is put in the Event header field of the
    //      SUBSCRIBE request sent. It should also be in the Event header field
    //      of the NOTIFY received for that subscription. When the empty string
    //      is used, the id parameter is not in the SUBSCRIBE sent and
    //      it must not be in the NOTIFY to match that subscription.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the SUBSCRIBE request to be sent. It can be NULL, which
    //      means that no message-body is added to the request. Ownership
    //      is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The manager of the service was not set or
    //      Clear has been called on the context.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: The SUBSCRIBE request could not be sent.
    //
    //      - resS_OK: The SUBSCRIBE request is successfully sent.
    //
    //  Description:
    //      Terminates the subscription corresponding to the id in parameter. If
    //      the subscription is an implicit subscription, it only updates its
    //      state to automatically refuse NOTIFY for this subscription.
    //      Otherwise, it sends a SUBSCRIBE request with the Expires header set
    //      to 0. The subscription is considered terminated when a NOTIFY
    //      with status terminated is received or 32 seconds after the SUBSCRIBE
    //      receives its final response.
    //
    //  See Also:
    //      Subscribe, CreateSubscription, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Terminate(IN const CString& rstrEvent,
                                 IN const CString& rstrId,
                                 IN mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 OUT ISipClientTransaction*& rpTransaction) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipSubscriberSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipSubscriberSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipSubscriberSvc(IN const ISipSubscriberSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipSubscriberSvc& operator=(IN const ISipSubscriberSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSUBSCRIBERSVC_H
