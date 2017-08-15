//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPREFEREEMGR_H
#define MXG_ISIPREFEREEMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT to be able \
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
class CNameAddr;
class CSipPacket;
class ISipClientEventControl;
class ISipRefereeSvc;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipRefereeMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The interface through which the ISipRefereeSvc reports events to the
//   application.
//
// Location:
//   SipUserAgent/ISipRefereeMgr.h
//
// See Also:
//   ISipRefereeSvc
//
//==EDOC========================================================================
class ISipRefereeMgr
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
    //==  EvReferred
    //==
    //==========================================================================
    //
    //  Summary:
    //      A REFER request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the REFER request has been received.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final 
    //      response on it.
    //
    //    opqReferId:
    //      The opaque id that the application must use to send NOTIFY for that
    //      REFER. It is also put in the events issued by this service that
    //      relate to this REFER.
    //
    //    rReferToAddr:
    //      The address found in the Refer-To header of the received REFER
    //      request.
    //
    //    rRefer:
    //      The received REFER request.
    //
    //  Description:
    //      Receives a REFER request. This request is guaranteed to contain a
    //      valid Refer-To header. When receiving this event, the manager can
    //      send any final response through the pServerEventControl interface.
    //
    //      If a success response (2xx) is sent, the manager should immediately
    //      call SendReferralStatus to send a NOTIFY to establish the
    //      subscription. When the manager does not intend to continue the
    //      subscription, it can call SendFinalReferralStatus.
    //
    //      Care should be taken when calling SendReferralStatus to use a long
    //      enough expiration value because it will not be able to increase this
    //      value later. Only the sender of the REFER can extend the
    //      subscription by sending a SUBSCRIBE request.
    //
    //  See Also:
    //      ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvReferred(IN ISipRefereeSvc* pSvc,
                            IN ISipServerEventControl* pServerEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CNameAddr& rReferToAddr,
                            IN const CSipPacket& rRefer) = 0;

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
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //    uExpirationSec:
    //      The expiration time suggested in the SUBSCRIBE request. If the
    //      request does not suggest an expiration time, the default expiration
    //      time of 60 seconds is used.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      This method is called when a valid SUBSCRIBE request is received for
    //      a subscription that already exists. Accepting this request with a
    //      success response extends the subscription on which the application
    //      can call SendReferralStatus or SendFinalReferralStatus.
    //
    //      The application must respond quickly to that request.
    //
    //  See Also:
    //      ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvRefreshed(IN ISipRefereeSvc* pSvc,
                             IN ISipServerEventControl* pServerEventControl,
                             IN mxt_opaque opqReferId,
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
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //    rSubscribe:
    //      The SUBSCRIBE request received.
    //
    //  Description:
    //      This method is called when a SUBSCRIBE request with "Expires" header
    //      set to 0 is received for an active or pending subscription.
    //
    //      If the application accepts this request, it should then call
    //      SendFinalReferralStatus to send a final NOTIFY.
    //
    //  See Also:
    //      ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvTerminated(IN ISipRefereeSvc* pSvc,
                              IN ISipServerEventControl* pServerEventControl,
                              IN mxt_opaque opqReferId,
                              IN const CSipPacket& rSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      A request has been automatically rejected by this service.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the request has been received.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rRequest:
    //      The request received. It can be either a SUBSCRIBE or a REFER.
    //
    //    reason:
    //      - resFE_MISSING_HEADER: The REFER request is missing a valid
    //      Refer-To header. A "400 Bad Request" response has already been sent.
    //
    //      - resFE_UNKNOWN_EVENT: The SUBSCRIBE request is missing an Event
    //      header or the event type is not "refer". A "489 Bad Event" response
    //      has already been sent.
    //
    //      - resFE_UNKNOWN_SUBSCRIPTION: The SUBSCRIBE request did not
    //      correspond to any REFER sent by this service. A "481 Subscription
    //      does not exist" response has already been sent.
    //
    //      - resFE_EXPIRED_SUBSCRIPTION: The SUBSCRIBE has been received after
    //      a NOTIFY with "subscription-state" "terminated" was sent or after the
    //      expiration time. A "481 Subscription does not exist" response has
    //      already been sent.
    //
    //  Description:
    //      Notifies the application that an invalid SUBSCRIBE or REFER has been
    //      received. Usually, the application has no action to take other than
    //      reporting this invalid packet if wanted. The ISipRefereeSvc that
    //      reported the event already took care of sending the appropriate
    //      response.
    //
    //      One action that can be taken is to destroy the context if it was
    //      created only to handle this SUBSCRIBE or REFER as it will not create
    //      a dialog.
    //
    //==EDOC====================================================================
    virtual void EvInvalidRequest(IN ISipRefereeSvc* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result reason) = 0;

    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) has been received for a NOTIFY request
    //      sent by the ISipRefereeSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request has been sent. It cannot
    //      be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it.
    //
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //    rResponse:
    //      The provisional response received.
    //
    //  Description:
    //      Notifies the application that a provisional response has been 
    //      received for a NOTIFY request that it sent. Note that this NOTIFY
    //      request could result from a call to SendReferralStatus or 
    //      SendFinalReferralStatus.
    //
    //  See Also:
    //      ISipClientEventControl, ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipRefereeSvc* pSvc,
                            IN ISipClientEventControl* pClientEventControl,
                            IN mxt_opaque opqReferId,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response (2xx) has been received for a NOTIFY request sent
    //      by the ISipRefereeSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request has been sent. It cannot
    //      be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones.
    //
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Notifies the application that a success response has been received
    //      for a NOTIFY request that it sent. If that NOTIFY request was sent 
    //      through SendReferralStatus, the application should consider that the
    //      subscription is still active. If the NOTIFY request was sent through
    //      SendFinalReferralStatus, the application should consider that this
    //      subscription is terminated. It should not call
    //      SendFinalReferralStatus nor SendReferralStatus again on that
    //      subscription.
    //
    //  See Also:
    //      ISipClientEventControl, ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure (<= 3xx) has been received for a NOTIFY request sent by 
    //      the ISipRefereeSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the NOTIFY request has been sent. It cannot
    //      be NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it.
    //
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //    rResponse:
    //      The failure response received.
    //
    //  Description:
    //      Notifies the application that a failure response (<=300) has been
    //      received for a NOTIFY request that it sent. Note that this NOTIFY
    //      request could result from a call to SendReferralStatus or
    //      SendFinalReferralStatus.
    //
    //      When the NOTIFY request was sent through the SendReferralStatus
    //      method and the application does not know how to fix the error
    //      condition, it should call SendFinalReferralStatus.
    //
    //  See Also:
    //      ISipClientEventControl, ISipRefereeSvc
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipRefereeSvc* pSvc,
                           IN ISipClientEventControl* pClientEventControl,
                           IN mxt_opaque opqReferId,
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
    //    opqReferId:
    //      The opaque id that accompanied the corresponding REFER request when
    //      EvReferred was called.
    //
    //  Description:
    //      Notifies the application that a subscription is expired. This only
    //      happens for subscriptions created by accepting a REFER request
    //      through EvReferred and not call SendFinalReferralStatus for that
    //      subscription.
    //
    //      When this event occurs, the application should call
    //      SendFinalReferralStatus.
    //
    //==EDOC====================================================================
    virtual void EvExpired(IN ISipRefereeSvc* pSvc,
                           IN mxt_opaque opqReferId) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipRefereeMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipRefereeMgr() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipRefereeMgr(IN const ISipRefereeMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipRefereeMgr& operator=(IN const ISipRefereeMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREFEREEMGR_H

