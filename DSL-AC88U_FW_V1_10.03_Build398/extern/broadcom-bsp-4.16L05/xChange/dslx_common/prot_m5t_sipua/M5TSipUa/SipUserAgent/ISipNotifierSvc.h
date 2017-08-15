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
#ifndef MXG_ISIPNOTIFIERSVC_H
#define MXG_ISIPNOTIFIERSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT to be able \
to use this class.
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CGenParamList;
class CHeaderList;
class CSipMessageBody;
class ISipClientTransaction;
class ISipNotifierMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipNotifierSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipNotifierSvc
//========================================
// <FLAG New in 4.1.4>
// <FLAG Updated behavior in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   This interface is used to act as a notifier as defined in RFC 3265. It lets
//   the application receive SUBSCRIBE and send NOTIFY messages.
//
// Description:
//   This interface is used to act as a notifier as defined in RFC 3265. It
//   lets the application receive SUBSCRIBE and send NOTIFY for multiple Events.
//   By default, the notifier service does not handle any event types
//   and will reject all subscription requests.
//   Supported events are added through the AddEvent method. Once event types
//   are thus added they are correctly handled.
//
//   This service reports events received to the application through the
//   ISipNotifierMgr interface.
//
// #The ISipNotifierSvc is an ECOM object#
//
//  The ISipNotifierSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipNotifierSvc</B>
//
//  <B>Interface Id: IID_ISipNotifierSvc</B>
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
//   event types unnecessarily complex.
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
//   SipUserAgent/ISipNotifierSvc.h
//
// See Also:
//   ISipNotifierMgr
//
//==EDOC========================================================================
class ISipNotifierSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipNotifierSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------

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
    virtual mxt_result SetManager(IN ISipNotifierMgr* pMgr) = 0;

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
    //==  SetMinimumExpiration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the minimum acceptable expiration time.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The event type is unknown.
    //
    //  Parameters:
    //    rstrEvent:
    //      Event type, e.g., "message-summary".
    //
    //    uMinExpirationSec:
    //      The minimum acceptable expiration time. When 0, it means that the
    //      expiration time can be as low as the subscriber wants. The
    //      application should not set this value higher than 1 hour (3600
    //      seconds) in order to be compliant with RFC 3265.
    //
    //  Description:
    //      Sets the minimum acceptable expiration time for a subscription
    //      of the given event type. When receiving a SUBSCRIBE request with
    //      this event type and with a value lower than this value but
    //      higher than 0, a "423 Interval too small" error is automatically
    //      sent.
    //
    //==EDOC====================================================================
    virtual mxt_result SetMinimumExpiration(IN const CString& rstrEvent,
                                            IN unsigned int uMinExpirationSec) = 0;

    // Description:
    //  Indicate how the subscription is created. See CreateSubscription for
    //  more information.
    enum EType
    {
        // Description:
        //  The subscription is established by other means than SIP
        eIMPLICIT,
        // Description:
        //  The subscription has been established by SIP signalling but not via
        //  a SUBSCRIBE request
        eEXPLICIT,
        // Description:
        //  Same as eIMPLICIT but the expiration time will be ignored
        eIMPLICIT_NOEXPIRES
    };

    //==SDOC====================================================================
    //==
    //==  CreateSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates a subscription without receiving a SUBSCRIBE request.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter in the Event header field of the SUBSCRIBE
    //      received for that subscription. When using the empty string, the
    //      "id" parameter must not be in the SUBSCRIBE message to match that
    //      subscription.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      for that event type is used. This argument is ignored when eType is
    //      eIMPLICIT_NOEXPIRES.
    //
    //    eType:
    //      Can be eEXPLICIT, which means that the subscription has been
    //      established by SIP signalling but not via a SUBSCRIBE request. When
    //      the subscription is established by other means than SIP, the value
    //      of this parameter should be eIMPLICIT or eIMPLICIT_NOEXPIRES to
    //      ignore uExpirationSec.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The manager has not been set.
    //
    //      - resFE_INVALID_ARGUMENT: The id is already used by another
    //      subscription in this object or the event is unknown.
    //
    //      - resS_OK: The subscription has been successfully created and ready
    //      to send NOTIFY requests.
    //
    //  Description:
    //      Creates a subscription without receiving a SUBSCRIBE request. Set
    //      the eType parameter to eEXPLICIT when the subscription was created
    //      by some other SIP means (eg. Receiving a REFER request). Set the
    //      eType parameter to eIMPLICIT when the subscription is created by
    //      some other non SIP means like a common agreement. Set the eType
    //      parameter to eIMPLICIT_NOEXPIRES when you want an implicit
    //      subscription that will never expire. The value of this parameter
    //      will dictate what is to be done when the subscription is terminated
    //      or refreshed. When the subscription is explicit, calling Refresh
    //      will continue the implicit subscription. When the subscription is
    //      explicit, it must be refreshed by the remote peer sending a
    //      SUBSCRIBE request in order to stay active.
    //
    //      When establishing an explicit subscription, because of the reception
    //      of a non-SUBSCRIBE request, the application should wait to send a
    //      success response to that request to call CreateSubscription. It is
    //      then able to send a NOTIFY immediately.
    //
    //      Otherwise, the subscription created by this method works exactly the
    //      same way as the one created by sending a success response to a
    //      received SUBSCRIBE request.
    //
    //  See Also:
    //      Terminate
    //
    //==EDOC====================================================================
    virtual mxt_result CreateSubscription(IN const CString& rstrEvent,
                                          IN const CString& rstrId,
                                          IN unsigned int uExpirationSec,
                                          IN EType eType) = 0;

    //==SDOC====================================================================
    //==
    //==  ExtendImplicitSubscription
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the expiration time for an implicit subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter in the Event header field of the SUBSCRIBE
    //      received for that subscription. When using the empty string,
    //      the "id" parameter must not be in the SUBSCRIBE message to match
    //      that subscription.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the default expiration
    //      for that event type is used.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The id corresponds to an explicit
    //      subscription.
    //
    //      - resS_OK: The implicit subscription expiration is updated.
    //
    //  Description:
    //      Updates the expiration time for an implicit subscription. It lets
    //      the application continue to call Notify for an implicit subscription
    //      after the original amount of time that was configured through
    //      CreateSubscription.
    //
    //      This method should be called when EvExpired is called for an
    //      implicit subscription and the application wants to continue the
    //      subscription.
    //
    //  See Also:
    //      ISipNotifierMgr::EvExpired, CreateSubscription, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result ExtendImplicitSubscription(
                                            IN const CString& rstrEvent,
                                            IN const CString& rstrId,
                                            IN unsigned int uExpirationSec) = 0;

    // Description:
    //  Indicates the state of the subscription. See Notify for more information.
    enum EState
    {
        // Description:
        //  The subscription is active.
        eACTIVE,
        // Description:
        //  The subscription is pending.
        ePENDING
    };

    //==SDOC====================================================================
    //==
    //==  Notify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a NOTIFY request for the specified subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter of the subscription. It is put in Event header of
    //      the NOTIFY request sent.
    //
    //    eSubscriptionState:
    //      The state to put in that subscription. Values can be ePENDING, which
    //      usually means it is the first NOTIFY and that the application is
    //      verifying if the user has the right to subscribe to this resource.
    //      A "Subscription-State" header with the value "pending" is added to
    //      the NOTIFY request. Otherwise, the value is eACTIVE, which means
    //      that the subscription is authorized and active. A
    //      "Subscription-State" header with the value "active" is added to the
    //      NOTIFY request.
    //
    //    uExpirationSec:
    //      The maximum time, in seconds, for which the subscription is
    //      considered active without refreshing. When 0, the current remaining
    //      expiration for that subscription is used. This value is put in the
    //      "Subscription-State" header's "expires" parameter of the NOTIFY
    //      request sent.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the sent NOTIFY request. It can be NULL, which means that
    //      no message-body is added to the request. Ownership of this parameter
    //      is TAKEN.
    //
    //    pCustomParameters:
    //      A pointer to the parameters added to the Event header of the sent
    //      NOTIFY request. It can be NULL, which means that no parameter is
    //      added to the request. Ownership of this parameter is TAKEN. This
    //      list should not contain the "id" parameter since it will be added
    //      from the strId parameter. If the "id" parameter is present, its
    //      value will be replaced with strId.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The id does not correspond to any
    //      subscription in this object, the expiration time is higher than
    //      the remaining time for that subscription or the event is unknown.
    //
    //      - resFE_INVALID_STATE: The id corresponds to a subscription that is
    //      expired. Use Terminate instead to send a final NOTIFY. Note that
    //      this is the case when receiving a SUBSCRIBE with a "Expires" header
    //      value of 0
    //      OR the attached ISipUserAgentSvc is incorrectly configured.
    //      For instance, the contact list could be empty.
    //
    //      - resFE_FAIL: The NOTIFY request could not be sent, for instance
    //      because there is an outstanding NOTIFY which didn't yet
    //      receive a response.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //      - resS_OK: The NOTIFY request has been successfully sent.
    //
    //  Description:
    //      Sends a NOTIFY request for an active or pending subscription. The
    //      application may add the proper content for that event type.
    //
    //      When the application lowers the expiration time by using a non NULL
    //      value in the uExpirationSec parameter, the expiration time is
    //      lowered if the NOTIFY request receives a success response.
    //
    //  See Also:
    //      Terminate, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Notify(IN    const CString& rstrEvent,
                              IN    const CString& rstrId,
                              IN    EState eSubscriptionState,
                              IN    unsigned int uExpirationSec,
                              IN    mxt_opaque opqTransaction,
                              IN TO CHeaderList* pExtraHeaders,
                              IN TO CSipMessageBody* pMessageBody,
                              IN TO CGenParamList* pCustomParameters,
                              OUT   ISipClientTransaction*& rpTransaction) = 0;

    // Description:
    //  Indicate why the subscription if terminating. See Terminate.
    enum EReason
    {
        // Description:
        //  Reason to terminate the subscription.
        eDEACTIVATED,
        //<COMBINE eDEACTIVATED>
        ePROBATION,
        //<COMBINE eDEACTIVATED>
        eREJECTED,
        //<COMBINE eDEACTIVATED>
        eTIMEOUT,
        //<COMBINE eDEACTIVATED>
        eGIVEUP,
        //<COMBINE eDEACTIVATED>
        eNO_RESOURCE,

        // M5T_INTERNAL_USE_BEGIN
        // Must stay the last in the enum.
        // M5T_INTERNAL_USE_END
        // Description:
        //  No reason parameter will be added to the "Subscription-State" header.
        eNO_REASON
    };

    //==SDOC====================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a final NOTIFY for that subscription.
    //
    //  Parameters:
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    strId:
    //      The "id" parameter of the subscription. It is put in Event header
    //      of the NOTIFY request sent.
    //
    //    eReason:
    //      The "reason" parameter to put in the "Subscription-State" header.
    //      When the value is eNO_REASON, the parameter is not put in the
    //      "Subscription-State" header.
    //
    //    uRetryAfterSec:
    //      The number of seconds to put in the "retry-after" parameter of the
    //      "Subscription-State" header. When the value is 0, no "retry-after"
    //      parameter is put. Note that RFC 3265 defines semantics for this
    //      parameter only if the eReason parameter has the value eNO_REASON,
    //      ePROBATION, or eGIVEUP.
    //
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL, which
    //      means that no extra headers are added. Ownership of this parameter
    //      is TAKEN.
    //
    //    pMessageBody:
    //      A pointer to the description of the message-body that should
    //      accompany the sent NOTIFY request. It can be NULL, which means that
    //      no message-body is added to the request. Ownership of this parameter
    //      is TAKEN.
    //
    //    pCustomParameters:
    //      A pointer to the parameters added to the Event header of the sent
    //      NOTIFY request. It can be NULL, which means that no parameter is
    //      added to the request. Ownership of this parameter is TAKEN. This
    //      list should not contain the "id" parameter since it will be added
    //      from the strId parameter. If "id" parameter is present, its value
    //      will be replaced with strId.
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
    //      - resFE_FAIL: The NOTIFY request could not be sent, for instance
    //      because there is an outstanding NOTIFY which didn't yet
    //      receive a response.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //      - resS_OK: The NOTIFY request has been successfully sent.
    //
    //  Description:
    //      Sends a NOTIFY with the "Subscription-State" header set to
    //      "terminated". The application may add the proper content for that
    //      event type.
    //
    //      This method should be called when the event EvExpired is called by
    //      the ISipNotifierMgr.
    //
    //      It should also be called when accepting a SUBSCRIBE received through
    //      the EvFetched event or the EvTerminated event of the
    //      ISipNotifierMgr.
    //
    //  See Also:
    //      Notify, EvExpired, EvFetched, EvTerminated, AddEvent
    //
    //==EDOC====================================================================
    virtual mxt_result Terminate(IN    const CString& rstrEvent,
                                 IN    const CString& rstrId,
                                 IN    EReason eReason,
                                 IN    unsigned int uRetryAfterSec,
                                 IN    mxt_opaque opqTransaction,
                                 IN TO CHeaderList* pExtraHeaders,
                                 IN TO CSipMessageBody* pMessageBody,
                                 IN TO CGenParamList* pCustomParameters,
                                 OUT   ISipClientTransaction*& rpTransaction) = 0;

    // Description:
    //  Information that uniquely identify a subscription.
    struct SSubscriptionInfo
    {
        // Description:
        //  The event type, e.g., "message-summary".
        CString m_strEvent;

        // Description:
        //  The "id" parameter of the subscription.
        CString m_strId;
    };

    //==========================================================================
    //==
    //==  GetCurrentSubscriptions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the currently active subscriptions.
    //
    //  Parameters:
    //    rvecstSubscriptions:
    //      A vector to hold the currently active subscriptions.
    //
    //  Description:
    //      Returns in a vector the information about each currently active
    //      subscription.
    //
    //==========================================================================
    virtual void GetCurrentSubscriptions(OUT CVector<SSubscriptionInfo>& rvecstSubscriptions) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipNotifierSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipNotifierSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipNotifierSvc(IN const ISipNotifierSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipNotifierSvc& operator=(IN const ISipNotifierSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPNOTIFIERSVC_H

