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
#ifndef MXG_ISIPNOTIFIERMGR_H
#define MXG_ISIPNOTIFIERMGR_H

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


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CString;
class ISipNotifierSvc;
class ISipClientEventControl;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipNotifierMgr
//========================================
// <FLAG New in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The interface through which the ISipNotifierSvc reports events to the
//   application.
//
// Warning:
//    This class replaces the old class of the same name,
//    which has been deprecated and renamed ISipNotifierBaseMgr
//    for clarity.
//
// Location:
//   SipUserAgent/ISipNotifierMgr.h
//
// See Also:
//   ISipNotifierSvc
//
//==EDOC========================================================================
class ISipNotifierMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  EvSubscribed
    //==
    //==========================================================================
    //
    //  Summary:
    //      A SUBSCRIBE request for a new subscription has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been received.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final
    //      response on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID of the subscription. It can be an empty string, which means
    //      that there is no ID.
    //
    //    uExpirationSec:
    //      The expiration time suggested in the SUBSCRIBE request. If the
    //      request does not suggest an expiration time, it is the default
    //      expiration time for that event type.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      This method is called when a valid new SUBSCRIBE request is
    //      received. Accepting this request with a success response creates
    //      a subscription on which the application can call Notify and
    //      Terminate.
    //
    //      The application must respond quickly to that request. If time is
    //      needed to authorize a user to access that particular subscription
    //      type, a success response should be returned and Notify should be
    //      called immediately with the eState parameter set to ePENDING. If the
    //      result of the verification is to refuse the subscription, the
    //      application would then call Terminate possibly specifying a reason
    //      and/or a retry-after value. If the result of the verification is to
    //      accept the subscription, the application would then call Notify
    //      again but with the eState parameter set to eACTIVE.
    //
    //      Note that if the application knows quickly enough that the
    //      subscription cannot be accepted, it can reject it with a failure
    //      response.
    //
    //  See Also:
    //      ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvSubscribed(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN unsigned int uExpirationSec,
                              IN const CSipPacket& rSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFetched
    //==
    //==========================================================================
    //
    //  Summary:
    //      A SUBSCRIBE request to fetch the current state has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been received.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final
    //      response on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID of the subscription. It can be an empty string, which means
    //      that there is no ID.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      Similar to EvSubscribed but with the "Expires" parameter set to 0.
    //      If the application sends a success response for that request (2xx),
    //      it must call Terminate. If the authorization process allows to give
    //      the state information to that user, it should put it in the content
    //      part of that NOTIFY.
    //
    //      If the application rejects the NOTIFY with a final negative response
    //      (>=300), it should not call Terminate.
    //
    //      The application should never call Notify on this subscription.
    //
    //  See Also:
    //      ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvFetched(IN ISipNotifierSvc* pSvc,
                           IN ISipServerEventControl* pServerEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvRefreshed
    //==
    //==========================================================================
    //
    //  Summary:
    //      A SUBSCRIBE request to refresh an active subscription has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been received.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final
    //      response on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID of the subscription. It can be an empty string, which means
    //      that there is no ID.
    //
    //    uExpirationSec:
    //      The expiration time suggested in the SUBSCRIBE request. If the
    //      request does not suggest an expiration time, it is the default
    //      expiration time for that event type.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      This method is called when a valid SUBSCRIBE request is received for
    //      a subscription that already exists. Accepting this request with a
    //      success response extends the subscription on which the
    //      application can call Notify and Terminate.
    //
    //      The application must respond quickly to that request.
    //
    //  See Also:
    //      ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvRefreshed(IN ISipNotifierSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN const CString& rstrEvent,
                             IN const CString& rstrId,
                             IN unsigned int uExpirationSec,
                             IN const CSipPacket& rSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      A SUBSCRIBE request to terminate an active/pending subscription has
    //      been received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been received.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final
    //      response on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID of the subscription. It can be an empty string, which means
    //      that there is no ID.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      This method is called when a SUBSCRIBE request with an "Expires"
    //      header set to 0 is received for an active or pending subscription.
    //
    //      If the application accepts this request, it should then call
    //      Terminate to send a final NOTIFY.
    //
    //  See Also:
    //      ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvTerminated(IN ISipNotifierSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidSubscribe
    //==
    //==========================================================================
    //
    //  Summary:
    //      A SUBSCRIBE request has been automatically rejected by this service.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been received.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    reason:
    //      - resFE_UNKNOWN_EVENT: The Event header was missing or for an
    //      unknown event type. A "489 Bad Event" response has already been sent.
    //
    //      - resFE_EXPIRED_SUBSCRIPTION: The SUBSCRIBE has been received after
    //      a NOTIFY with "subscription-state" "terminated" was sent or after
    //      the expiration time. A "481 Subscription does not exist" response
    //      has already been sent.
    //
    //      - resFE_INTERVALL_TOO_BRIEF: The received SUBSCRIBE had a non 0
    //      expiration time smaller than the minimum configured through
    //      SetMinimumExpiration. A "423 Interval too brief" response has
    //      already been sent.
    //
    //  Description:
    //      Informs the application that an invalid SUBSCRIBE has been received.
    //      Usually, the application has no action to take other than reporting
    //      this invalid packet if wanted. The ISipNotifierMgr that reported
    //      the event already took care of sending the appropriate response.
    //
    //      One action that can be taken is to destroy the context if it was
    //      created only to handle this SUBSCRIBE as it will not create a
    //      dialog.
    //
    //==EDOC====================================================================
    virtual void EvInvalidSubscribe(IN ISipNotifierSvc* pSvc,
                                    IN mxt_opaque opqApplicationData,
                                    IN const CSipPacket& rSubscribe,
                                    IN mxt_result reason) = 0;

    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) has been received for a NOTIFY request
    //      sent by the ISipNotifierSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request was sent. It cannot be
    //      NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must either call CallNextEvent or
    //      ClearClientEvents on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID parameter of the Event header of the NOTIFY request.
    //
    //    rResponse:
    //      The provisional response received.
    //
    //  Description:
    //      Informs the application that a provisional response has been
    //      received for a NOTIFY request that it sent. Note that this NOTIFY
    //      request could result of a call to Notify or Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipSubscriberSvc
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipNotifierSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response (2xx) has been received for a NOTIFY request sent
    //      by the ISipNotifierSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request was sent. It cannot be
    //      NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must either call CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The Event header's ID parameter of the NOTIFY request.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Informs the application that a success response has been received
    //      for a NOTIFY request that it sent. If that NOTIFY request was sent
    //      through NOTIFY, the application should consider that the
    //      subscription is still active. If the NOTIFY request was sent through
    //      a Terminate, the application should consider that this subscription
    //      is terminated. It should not call Terminate nor Notify again on that
    //      subscription.
    //
    //  See Also:
    //      ISipClientEventControl, ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure (<= 3xx) has been received for a NOTIFY request sent by
    //      ISipNotifierSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request has been sent. It cannot
    //      be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must either call CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The Event header's ID parameter of the NOTIFY request.
    //
    //    rResponse:
    //      The failure response received.
    //
    //  Description:
    //      Informs the application that a failure response (<=300) has been
    //      received for a NOTIFY request that it sent. Note that this NOTIFY
    //      request could be the result of a call to Notify or Terminate.
    //
    //      When the NOTIFY request was sent through the Notify method and the
    //      application does not know how to fix the error condition, it should
    //      call Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipNotifierSvc
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipNotifierSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription is expired.
    //
    //  Parameters:
    //    pSvc:
    //      The interface that has an expired subscription.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The ID of the expired subscription.
    //
    //  Description:
    //      Informs the application that a subscription is expired. This only
    //      happens for subscriptions created through CreateSubscription or
    //      by accepting a SUBSCRIBE request through EvSubscribed and not
    //      calling Terminate for that subscription.
    //
    //      When this event occurs, the application should call Terminate unless
    //      the subscription was implicit, in which case it may call Terminate.
    //
    //==EDOC====================================================================
    virtual void EvExpired(IN ISipNotifierSvc* pSvc,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipNotifierMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipNotifierMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipNotifierMgr(IN const ISipNotifierMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipNotifierMgr& operator=(IN const ISipNotifierMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPNOTIFIERMGR_H
