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
#ifndef MXG_ISIPSUBSCRIBERMGR_H
#define MXG_ISIPSUBSCRIBERMGR_H

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


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CString;
class ISipClientEventControl;
class ISipServerEventControl;
class ISipSubscriberSvc;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipSubscriberMgr
//========================================
// <FLAG New in 4.1.4>
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The interface through which the ISipSubscriberSvc reports events to the
//   application.
//
// Warning:
//    This class replaces the old class of the same name,
//    which has been deprecated and renamed ISipSubscriberBaseMgr
//    for clarity.
//
// Location:
//   SipUserAgent/ISipSubscriberMgr.h
//
// See Also:
//   ISipSubscriberSvc
//
//==EDOC========================================================================
class ISipSubscriberMgr
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
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) has been received for a SUBSCRIBE
    //      request sent by the ISipSubscriberSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been sent. It
    //      cannot be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The id parameter of the Event header of the SUBSCRIBE request.
    //
    //    rResponse:
    //      The provisional response received.
    //
    //  Description:
    //      Notifies the application that a provisional response has been
    //      received for a SUBSCRIBE request that it sent. Note that this
    //      SUBSCRIBE request could result from a call to Subscribe, Fetch,
    //      Refresh, or Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipSubscriberSvc
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipSubscriberSvc* pSvc,
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
    //      A success response (2xx) has been received for a SUBSCRIBE request
    //      sent by the ISipSubscriberSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been sent. It
    //      cannot be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The id parameter of the Event header of the SUBSCRIBE request.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Informs the application that a success response has been received
    //      for a SUBSCRIBE request that it sent. If that SUBSCRIBE request was
    //      sent through Subscribe or Refresh, the application should consider
    //      that the subscription is active and be ready to receive EvNotified
    //      events. If the SUBSCRIBE request was sent through a Fetch or a
    //      Terminate, the application should expect to receive the event
    //      EvTerminated or EvExpired. Note that the EvTerminated or EvExpired
    //      can still occur unrequested if the notifier decides to terminate the
    //      subscription or if the subscription expires.
    //
    //  See Also:
    //      ISipClientEventControl, ISipSubscriberSvc
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvIntervalTooSmall
    //==
    //==========================================================================
    //
    //  Summary:
    //      An "Interval Too Small" (423) has been received for a SUBSCRIBE
    //      request sent by the ISipSubscriberSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been sent. It
    //      cannot be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest would not correct the problem because the SUBSCRIBE
    //      request would be retried with the same expiration time.
    //
    //    uMinExpirationSec:
    //      The value of the Min-Expires header found in the 423 response.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The id parameter of the Event header of the SUBSCRIBE request.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Informs the application that a 423 Interval Too Small response has
    //      been received for a SUBSCRIBE request that it sent. This SUBSCRIBE
    //      was sent through Subscribe or Refresh. The application should call
    //      the same method but with an expiration time greater than or equal to
    //      uMinExpirationSec.
    //
    //  See Also:
    //      ISipClientEventControl, ISipSubscriberSvc
    //
    //==EDOC====================================================================
    virtual void EvIntervalTooSmall(
                                 IN ISipSubscriberSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN unsigned int uMinExpirationSec,
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
    //      A failure (>= 3xx) has been received for a SUBSCRIBE request sent by
    //      the ISipSubscriberSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the SUBSCRIBE request has been sent. It
    //      cannot be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The id parameter of the Event header of the SUBSCRIBE request.
    //
    //    rResponse:
    //      The failure response received.
    //
    //  Description:
    //      Notifies the application that a failure response (>=300) has been
    //      received for a SUBSCRIBE request that it sent. Note that this
    //      SUBSCRIBE request could result from a call to Subscribe, Fetch,
    //      Refresh, or Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipSubscriberSvc
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipSubscriberSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN const CString& rstrEvent,
                           IN const CString& rstrId,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvNotified
    //==
    //==========================================================================
    //
    //  Summary:
    //      A NOTIFY request has been received for a valid active subscription.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the subscription has been created.
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
    //      The id of the subscription. It can be an empty string, which means
    //      that there is no id.
    //
    //    rNotify:
    //      The NOTIFY request received.
    //
    //  Description:
    //      Notifies the application that a valid NOTIFY request has been
    //      received. The ISipSubscriberSvc has already validated that the
    //      NOTIFY is for an active subscription and that it is valid. However,
    //      the ISipSubscriberSvc does not automatically accept this NOTIFY
    //      request with a 200 OK response because it does not verify the
    //      content and the content could be unacceptable to the application.
    //
    //      Note that this NOTIFY request is guaranteed to contain a
    //      "subscription-state" header and its value is guaranteed not to be
    //      "terminated" either.
    //
    //      When a NOTIFY with a "subscription-state" header with value
    //      "terminated" is received, it is presented to the application through
    //      the EvTerminated method.
    //
    //      The first NOTIFY sent after a SUBSCRIBE has to contain a message-
    //      summary. The draft does not specify that the following NOTIFYs have
    //      to contain a message-summary.  If a message-summary is wrong or not
    //      present when it should be, a 400 response is sent back automatically.
    //      If the content-type is wrong, a 415 response is sent back
    //      automatically.
    //
    //
    //  See Also:
    //      EvTerminated
    //
    //==EDOC====================================================================
    virtual void EvNotified(IN ISipSubscriberSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId,
                            IN const CSipPacket& rNotify) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      A final NOTIFY has been received for a valid active subscription.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the subscription has been created.
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
    //      The id of the subscription. It can be an empty string, which means
    //      that there is no id.
    //
    //    rNotify:
    //      The NOTIFY request received.
    //
    //  Description:
    //      Informs the application that a valid NOTIFY request has been
    //      received. The ISipSubscriberSvc has already validated that the
    //      NOTIFY is for an active subscription and that it is valid. However,
    //      the ISipSubscriberSvc does not automatically accept this NOTIFY
    //      request with a 200 OK response because it does not verify the
    //      content and the content could be unacceptable to the application.
    //
    //      If accepted, this NOTIFY terminates the subscription. It is
    //      guaranteed to contain a "subscription-state" header with the value
    //      "terminated".
    //
    //      The first NOTIFY sent after a SUBSCRIBE has to contain a message-
    //      summary. The draft does not specify that the following NOTIFYs have
    //      to contain a message-summary.  If a message-summary is wrong or not
    //      present when it should be, a 400 response is sent back automatically.
    //      If the content-type is wrong, a 415 response is sent back
    //      automatically.
    //
    //  See Also:
    //      EvNotified
    //
    //==EDOC====================================================================
    virtual void EvTerminated(IN ISipSubscriberSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN const CString& rstrEvent,
                              IN const CString& rstrId,
                              IN const CSipPacket& rNotify) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidNotify
    //==
    //==========================================================================
    //
    //  Summary:
    //      An invalid NOTIFY has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface that handled the notify.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rNotify:
    //      The NOTIFY request received.
    //
    //    reason:
    //      - resFE_UNKNOWN_EVENT: The Event header was missing or for an
    //      unknown event type. A "489 Bad Event" response has already been sent.
    //
    //      - resFE_UNKNOWN_SUBSCRIPTION: The NOTIFY contains an id unknown to
    //      the service. A "481 Subscription does not exist" response has
    //      already been sent.
    //
    //      - resFE_EXPIRED_SUBSCRIPTION: The NOTIFY has been received after
    //      another NOTIFY with "subscription-state" "terminated" or after the
    //      expiration time. A "481 Subscription does not exist" response has
    //      already been sent.
    //
    //      - resFE_MISSING_HEADER: The received NOTIFY did not contain the
    //      mandatory "Subscription-State" header.
    //
    //  Description:
    //      Notifies the application that an invalid NOTIFY has been received.
    //      Usually, the application has no action to take other than reporting
    //      this invalid packet if wanted. The ISipSubscriberSvc that reported
    //      the event already took care of sending the appropriate response.
    //
    //      One action that can be taken is to destroy the context if it was
    //      created only to handle this NOTIFY as it will not create a dialog.
    //
    //  See Also:
    //      EvNotified, EvTerminated
    //
    //==EDOC====================================================================
    virtual void EvInvalidNotify(IN ISipSubscriberSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rNotify,
                                 IN mxt_result reason) = 0;

    //==SDOC====================================================================
    //==
    //==  EvExpiring
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription is about to expire.
    //
    //  Parameters:
    //    pSvc:
    //      The interface that has a subscription about to expire.
    //
    //    rstrEvent:
    //      The event type, e.g., "message-summary".
    //
    //    rstrId:
    //      The id of the subscription about to expire.
    //
    //  Description:
    //      Notifies the application that a subscription will expire in the
    //      amount of time configured with the
    //      ISipSubscriberSvc::SetExpiringThreshold method. This event does not
    //      occur when a subscription is set for a time smaller than or equal to
    //      the threshold.
    //
    //      The application should then either call Refresh to keep the
    //      subscription active or Terminate to terminate it.
    //
    //  See Also:
    //      EvExpired
    //
    //==EDOC====================================================================
    virtual void EvExpiring(IN ISipSubscriberSvc* pSvc,
                            IN const CString& rstrEvent,
                            IN const CString& rstrId) = 0;

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
    //      The id of the expired subscription.
    //
    //  Description:
    //      Notifies the application that a subscription is expired. There is no
    //      action to take by the application as far as the ISipSubscriberSvc is
    //      concerned.
    //
    //      Note that this event can happen for a subscription created with
    //      CreateSubscription, Subscribe and will happen if Fetch does not
    //      receive a NOTIFY with "subscription-state" "terminated" within 32
    //      seconds.
    //
    //  See Also:
    //      EvExpiring
    //
    //==EDOC====================================================================
    virtual void EvExpired(IN ISipSubscriberSvc* pSvc,
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
    ISipSubscriberMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipSubscriberMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipSubscriberMgr(IN const ISipSubscriberMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipSubscriberMgr& operator=(IN const ISipSubscriberMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSUBSCRIBERMGR_H
