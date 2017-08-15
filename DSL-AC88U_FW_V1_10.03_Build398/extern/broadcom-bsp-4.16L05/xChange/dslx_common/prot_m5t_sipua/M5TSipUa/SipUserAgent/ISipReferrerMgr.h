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
#ifndef MXG_ISIPREFERRERMGR_H
#define MXG_ISIPREFERRERMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT to be able \
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
class CHeaderList;
class CSipPacket;
class CSipStatusLine;
class ISipClientEventControl;
class ISipReferrerSvc;
class ISipServerEventControl;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReferrerMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The interface through which the ISipReferrerSvc reports events to the
//   application.
//
// Location:
//   SipUserAgent/ISipReferrerMgr.h
//
// See Also:
//   ISipReferrerMgr
//
//==EDOC========================================================================
class ISipReferrerMgr
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
    //==  EvReferProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) has been received for a REFER request 
    //      sent by the ISipReferrerSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the REFER request has been sent. It cannot be
    //      NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it.
    //
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The provisional response received.
    //
    //  Description:
    //      Notifies the application that a provisional response has been 
    //      received for a REFER request that it sent. Note that if the REFER 
    //      request resulted from a call to ReIssueRequest on a 
    //      ISipClientEventControl interface, the opqReferId parameter is still the
    //      same as the one returned by the original call to the Refer method.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvReferProgress(IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReferSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response (2xx) has been received for a REFER request sent
    //      by the ISipReferrerSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the REFER request has been sent. It cannot be
    //      NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones.
    //
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Notifies the application that a success response has been received
    //      for a REFER request that it sent. The application should consider 
    //      that the subscription is active and be ready to receive 
    //      EvReferStatus events. Note that EvReferStatus can occur before 
    //      EvReferSuccess when the initial NOTIFY is received before the REFER
    //      success response.
    //
    //      Note that if the REFER request resulted from a call to
    //      ReIssueRequest on a ISipClientEventControl interface, the opqReferId
    //      parameter is still the same as the one returned by the original call
    //      to the Refer method.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvReferSuccess(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReferFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure (<= 3xx) has been received for a REFER request sent by the
    //      ISipReferrerSvc.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the REFER request has been sent. It cannot be
    //      NULL.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it.
    //
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The failure response received.
    //
    //  Description:
    //      Notifies the application that a failure response (<=300) has been
    //      received for a REFER request that it sent. Note that this SUBSCRIBE
    //      request could result from a call to Subscribe, Fetch, Refresh, or
    //      Terminate.
    //
    //      Note that if the REFER request resulted from a call to
    //      ReIssueRequest on a ISipClientEventControl interface, the opqReferId
    //      parameter is still the same as the one returned by the original call
    //      to the Refer method.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvReferFailure(IN ISipReferrerSvc* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN mxt_opaque opqReferId,
                                IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSubscribeProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response (1xx) has been received for a SUBSCRIBE 
    //      request sent by the ISipReferrerSvc.
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
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The provisional response received.
    //
    //  Description:
    //      Notifies the application that a provisional response has been 
    //      received for a SUBSCRIBE request that it sent. Note that this 
    //      SUBSCRIBE request could result from a call to Refresh or Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvSubscribeProgress(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSubscribeSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response (2xx) has been received for a SUBSCRIBE request
    //      sent by the ISipReferrerSvc.
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
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Notifies the application that a success response has been received 
    //      for a SUBSCRIBE request that it sent. If that SUBSCRIBE request was 
    //      sent through Refresh, the application should consider that the
    //      subscription is active and be ready to receive EvReferStatus events.
    //      If the SUBSCRIBE request was sent through Terminate, the application
    //      should expect to receive the event EvTerminated or EvExpired. Note
    //      that the EvTerminated or EvExpired can still occur unrequested if
    //      the referee decides to terminate the subscription or if the
    //      subscription expires.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvSubscribeSuccess(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvIntervalTooSmall
    //==
    //==========================================================================
    //
    //  Summary:
    //      An "Interval Too Small" (423) has been received for a SUBSCRIBE 
    //      request sent by the ISipReferrerSvc.
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
    //   uMinExpirationSec:
    //      The minimum number of seconds acceptable for the subscription
    //      duration according to the remote peer.
    //
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The success response received.
    //
    //  Description:
    //      Notifies the application that a 423 Interval Too Small response has
    //      been received for a SUBSCRIBE request that it sent. This SUBSCRIBE 
    //      was sent through Refresh. The application should call Refresh again 
    //      but with an expiration time greater than or equal to 
    //      uMinExpirationSec.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvIntervalTooSmall(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN unsigned int uMinExpirationSec,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSubscribeFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure (<= 3xx) has been received for a SUBSCRIBE request sent by
    //      the ISipReferrerSvc.
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
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    rResponse:
    //      The failure response received.
    //
    //  Description:
    //      Notifies the application that a failure response (<=300) has been 
    //      received for a SUBSCRIBE request that it sent. Note that this 
    //      SUBSCRIBE request could result from a call to Refresh or Terminate.
    //
    //  See Also:
    //      ISipClientEventControl, ISipReferrerSvc
    //
    //==EDOC====================================================================
    virtual void EvSubscribeFailure(
                                 IN ISipReferrerSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN mxt_opaque opqReferId,
                                 IN const CSipPacket& rResponse) = 0;

    // Description:
    //  Subscription states.
    enum EState
    {
        // A subscription state.
        ePENDING,
        //<COMBINE ePENDING>
        eACTIVE,
        //<COMBINE ePENDING>
        eTERMINATED
    };

    //==SDOC====================================================================
    //==
    //==  EvReferStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a NOTIFY for a REFER sent has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The interface on which the REFER was sent.
    //
    //    pServerEventControl:
    //      The server event control interface for this transaction. It cannot
    //      be NULL. The application must call SendResponse with a final response
    //      on it.
    //
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //    eState:
    //      The state of the subscription. When eTERMINATED and the application
    //      accepts the NOTIFY by sending a success response, the subscription
    //      is terminated. This parameter is calculated from the
    //      Subscription-State header of the NOTIFY request received.
    //
    //    rStatus:
    //      The status line found in the content of the NOTIFY request.
    //
    //    pContentHeaders:
    //      Headers that accompanied the status line in the content of the
    //      NOTIFY. It can be NULL, which means that there were no such headers. 
    //      Note that the application does not have ownership of this pointer 
    //      and the header list must be copied if the application intends to use
    //      it after the method returned.
    //
    //    rNotify:
    //      The NOTIFY request received.
    //
    //  Description:
    //      Notifies the application that a valid NOTIFY request has been 
    //      received. The ISipReferrerSvc has already validated that the NOTIFY 
    //      is for an active subscription and that it is valid. It has also 
    //      verified that the content is valid according to RFC 3515. It means 
    //      that it was of type "message/sipfrag" and contained at least a valid
    //      status-line.
    //
    //      Note that this NOTIFY request is guaranteed to contain a valid
    //      "subscription-state" header.
    //
    //  See Also:
    //      ISipServerEventControl
    //
    //==EDOC====================================================================
    virtual void EvReferStatus(IN ISipReferrerSvc* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN mxt_opaque opqReferId,
                               IN EState eState,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
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
    //      - resFE_UNKNOWN_EVENT: The Event header was missing or for an event
    //      type other than "refer". A "489 Bad Event" response has already been
    //      sent.
    //
    //      - resFE_UNKNOWN_SUBSCRIPTION: The NOTIFY does not correspond to a
    //      REFER sent by this service. A "481 Subscription does not exist"
    //      response has already been sent.
    //
    //      - resFE_EXPIRED_SUBSCRIPTION: The NOTIFY has been received after 
    //      another NOTIFY with "subscription-state" "terminated" or after the
    //      expiration time. A "481 Subscription does not exist" response has
    //      already been sent.
    //
    //      - resFE_MISSING_HEADER: The received NOTIFY did not contain the
    //      mandatory "Subscription-State" header.
    //
    //      - resFE_INVALID_CONTENT: The received NOTIFY did not contain a valid
    //      "message/sipfrag".
    //
    //  Description:
    //      Informs the application that an invalid NOTIFY has been received.
    //      Usually, the application has no action to take other than reporting
    //      this invalid packet if wanted. The ISipReferrerMgr that reported the
    //      event already took care of sending the appropriate response.
    //
    //      One action that can be taken is to destroy the context if it was
    //      created only to handle this NOTIFY as it will not create a dialog.
    //
    //  See Also:
    //      EvNotified, EvTerminated
    //
    //==EDOC====================================================================
    virtual void EvInvalidNotify(IN ISipReferrerSvc* pSvc,
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
    //    opqReferId:
    //      The opqReferId returned by the service when Refer was called.
    //
    //  Description:
    //      Notifies the application that a subscription will expire in the
    //      amount of time configured with the SetExpiringThreshold method. This
    //      event does not occur when a subscription is set for a time smaller
    //      or equal to the threshold.
    //
    //      The application should then either call Refresh to keep the
    //      subscription active or Terminate to terminate it.
    //
    //  See Also:
    //      EvExpired
    //
    //==EDOC====================================================================
    virtual void EvExpiring(IN ISipReferrerSvc* pSvc,
                            IN mxt_opaque opqReferId) = 0;

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
    //      The opqReferId returned by the service when Refer was called.
    //
    //  Description:
    //      Informs the application that a subscription is expired. There is no
    //      action to take by the application as far as the ISipReferrerSvc is
    //      concerned.
    //
    //  See Also:
    //      EvExpiring
    //
    //==EDOC====================================================================
    virtual void EvExpired(IN ISipReferrerSvc* pSvc,
                           IN mxt_opaque opqReferId) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReferrerMgr() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReferrerMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReferrerMgr(IN const ISipReferrerMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReferrerMgr& operator=(IN const ISipReferrerMgr& rSrc);
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREFERRERMGR_H

