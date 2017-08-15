//==============================================================================
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPTRANSFERMGR07_H
#define MXG_ISIPTRANSFERMGR07_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT to be \
able to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_ISIPREFERRERMGR_H
#include "SipUserAgent/ISipReferrerMgr.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class CSipPacket;
class CSipStatusLine;
class ISipClientEventControl;
class ISipServerEventControl;
class ISipTransferSvc07;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipTransferMgr07
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The transfer manager 07 is the interface through which the transfer
//  service 07 talks to the application. It is through this interface that
//  the M5T SIP Stack notifies the application of the progress of a transfer.
//
//  The transfer service 07 can manage one transfer as transferee and one
//  transfer as transferor at the same time.
//
//  The transfer service 07 does NOT respond to any valid request that are
//  notified to the application by this manager, so the manager must answer them
//  when it receives a request related event other than EvInvalidRequest. The
//  transfer service 07 does NOT send any request by itself, so the application
//  must send the NOTIFY requests stating its progress to the transferor.
//
// Location:
//   SipUserAgent/ISipTransferMgr07.h
//
// See Also:
//  ISipTransferSvc07
//==============================================================================
class ISipTransferMgr07
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
    //      A provisional response has been received for the REFER request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it. If one of these methods is not called
    //      directly from the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The provisional response to the REFER.
    //
    //  Description:
    //      A provisional response to the REFER has been received.
    //
    //      REFER requests are sent with ISipTransferSvc07::Transfer.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::Transfer.
    //
    //==EDOC====================================================================
    virtual void EvReferProgress(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReferSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response has been received for the REFER request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones. If one of these methods is not called directly from
    //      the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The success response to the REFER.
    //
    //  Description:
    //      A success response to the REFER has been received.
    //
    //      The manager must be prepared to receive EvProgressReport or
    //      EvFinalReport. These events may be received before this one is
    //      received  (RFC 3515, section 2.4.4).
    //
    //      REFER requests are sent with ISipTransferSvc07::Transfer.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::Transfer.
    //
    //==EDOC====================================================================
    virtual void EvReferSuccess(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReferFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure response has been received for the REFER request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. If one of these methods
    //      is not called directly from the present method, a reference must be
    //      added on pClientEventControl. The added reference must be released
    //      after one of the methods is called.
    //
    //    rResponse:
    //      The failure response to the REFER.
    //
    //  Description:
    //      A failure response to the REFER has been received.
    //
    //      The manager must be prepared to receive EvProgressReport or
    //      EvFinalReport before this one is received (RFC 3515, section 2.4.4).
    //
    //      REFER requests are sent with ISipTransferSvc07::Transfer.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::Transfer.
    //
    //==EDOC====================================================================
    virtual void EvReferFailure(IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvProgressReport
    //==
    //==========================================================================
    //
    //  Summary:
    //      A non-final NOTIFY has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pServerEventControl:
    //      The interface through which the application must send at least one
    //      response. If SendResponse is not called directly from the present
    //      method, a reference must be added on pServerEventControl. The added
    //      reference must be released after SendResponse is called.
    //
    //    eState:
    //      The state of the subscription. It is either ePENDING or eACTIVE.
    //      This parameter is calculated from the Subscription-State header of
    //      the NOTIFY request received.
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
    //    rNotifyRequest:
    //      The NOTIFY request.
    //
    //  Description:
    //      Informs the application that a valid non-final NOTIFY request has
    //      been received.
    //
    //      A NOTIFY is final when the value of the Subscription-State header is
    //      "terminated".
    //
    //      The application should accept the NOTIFY request by replying 200 OK
    //      through pServerEventControl.
    //
    //  See Also:
    //      EvFinalReport, ISipTransferSvc07
    //
    //==========================================================================
    virtual void EvProgressReport(
                                IN ISipTransferSvc07* pSvc,
                                IN ISipServerEventControl* pServerEventControl,
                                IN ISipReferrerMgr::EState eState,
                                IN const CSipStatusLine& rStatus,
                                IN const CHeaderList* pContentHeaders,
                                IN const CSipPacket& rNotifyRequest) = 0;

    //==========================================================================
    //==
    //==  EvFinalReport
    //==
    //==========================================================================
    //
    //  Summary:
    //      A final NOTIFY has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pServerEventControl:
    //      The interface through which the application must send at least one
    //      response. If SendResponse is not called directly from the present
    //      method, a reference must be added on pServerEventControl. The added
    //      reference must be released after SendResponse is called.
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
    //    rNotifyRequest:
    //      The NOTIFY request.
    //
    //  Description:
    //      Notifies the application that a valid final NOTIFY request has been
    //      received.
    //
    //      A NOTIFY is final when the value of the Subscription-State header is
    //      "terminated".
    //
    //      The application should accept the NOTIFY request by replying 200 OK
    //      through pServerEventControl.
    //
    //      The transfer is considered terminated only when a final success
    //      response (2xx) is sent to this NOTIFY. If no final NOTIFY is
    //      responded 2xx, then the transfer is considered terminated when
    //      EvReportingExpired is called.
    //
    //      Note that if this final NOTIFY is refused (a failure response is
    //      sent by the application), it is not guaranteed that the transferee
    //      will still send NOTIFY requests. In this case, EvReportingExpired
    //      will eventually be called.
    //
    //  See Also:
    //      EvProgressReport, EvReportingExpired, ISipTransferSvc07
    //
    //==========================================================================
    virtual void EvFinalReport(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CSipStatusLine& rStatus,
                               IN const CHeaderList* pContentHeaders,
                               IN const CSipPacket& rNotifyRequest) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReportingExpired
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription expired before a final NOTIFY has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //  Description:
    //      Notifies the application that a subscription is expired. There is no
    //      action to take by the application.
    //
    //      A NOTIFY request is no longer received so the tranfer is
    //      considered terminated.
    //
    //==EDOC====================================================================
    virtual void EvReportingExpired(IN ISipTransferSvc07* pSvc) = 0;

    //==========================================================================
    //==
    //==  EvTransferred
    //==
    //==========================================================================
    //
    //  Summary:
    //      A valid REFER request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer initiated with the REFER request.
    //
    //    pServerEventControl:
    //      The interface through which the application must send at least one
    //      response. If SendResponse is not called directly from the present
    //      method, a reference must be added on pServerEventControl. The added
    //      reference must be released after SendResponse is called.
    //
    //    rTarget:
    //      The transfer target URL. This is the URL found in the Refer-To
    //      header.
    //
    //    rReferRequest:
    //      The REFER request.
    //
    //  Description:
    //      Notifies the application that the remote user-agent (the transferor)
    //      wants the local user-agent (acting as a transferee) to contact the
    //      agent at the target URL (the transfer target).
    //
    //      When this method is called, the REFER request has a valid Refer-To
    //      header containing a supported URI and the service does not manage 
    //      any transfer as a transferee.
    //
    //      If the REFER request is answered with a 2xx response, the
    //      application MUST immediately send a NOTIFY to the transferor (RFC
    //      3515, section 2.4.4). The first NOTIFY request sent for the REFER
    //      may be a "100 Trying", which may be sent by using
    //      ISipTransferSvc07::ReportPending.
    //
    //      Since the REFER request contains a Refer-To header and the target
    //      URI has a supported scheme, the application should answer a 202
    //      response to the REFER request, send a NOTIFY with a "100 Trying"
    //      payload (by using ISipTransferSvc07::ReportPending), and then see if
    //      it accepts to do the transfer (user input should be used, either
    //      through configuration or in real time). If the user accepts, it
    //      should create a new context to rTarget and call
    //      ISipSessionSvc::Invite method on it.
    //
    //      If, after having sent a 202 response to the REFER request, the
    //      application does not accept the transfer, it should send a final
    //      NOTIFY request containing a payload of "603 Declined" to the
    //      transferor by using ISipTransferSvc07::ReportRefusal.
    //
    //      If the transfer was tried but failed, the application should use
    //      ISipTransferSvc07::ReportFailure to send a final NOTIFY to the
    //      transferor.
    //
    //      Note that a final NOTIFY MUST be sent for every REFER accepted
    //      (draft-ietf-sipping-cc-transfer-03, section 3, requirement 3) so
    //      either ISipTransferSvc07::ReportPending,
    //      ISipTransferSvc07::ReportFailure, or
    //      ISipTransferSvc07::ReportFinalStatus must be called once for every
    //      REFER accepted (responded 2xx).
    //
    //      If the application does not want to continue the subscription (even
    //      before the first NOTIFY is sent), it should call ReportFinalStatus
    //      to send a final NOTIFY.
    //
    //  See Also:
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportFinalStatus, ISipTransferSvc07.
    //
    //==========================================================================
    virtual void EvTransferred(IN ISipTransferSvc07* pSvc,
                               IN ISipServerEventControl* pServerEventControl,
                               IN const CNameAddr& rTarget,
                               IN const CSipPacket& rReferRequest) = 0;

    //==========================================================================
    //==
    //==  EvNotifyProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      A provisional response for a NOTIFY request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it. If one of these methods is not called
    //      directly from the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The provisional response to a NOTIFY.
    //
    //  Description:
    //      A provisional response to a NOTIFY has been received.
    //
    //      NOTIFY requests are sent with ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress, or
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress,
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //==========================================================================
    virtual void EvNotifyProgress(
                                IN ISipTransferSvc07* pSvc,
                                IN ISipClientEventControl* pClientEventControl,
                                IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvNotifySuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      A success response has been received for a NOTIFY request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. Note however that
    //      ReIssueRequest is usually called to retry failed requests instead of
    //      successful ones. If one of these methods is not called directly from
    //      the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The final success response to a NOTIFY.
    //
    //  Description:
    //      A final success response to a NOTIFY has been received.
    //
    //      NOTIFY requests are sent with ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress, or
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress,
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //==========================================================================
    virtual void EvNotifySuccess(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvNotifyFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      A failure response has been received for a NOTIFY request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pClientEventControl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent,
    //      ClearClientEvents, or ReIssueRequest on it. If one of these methods
    //      is not called directly from the present method, a reference must be
    //      added on pClientEventControl. The added reference must be released
    //      after one of the methods is called.
    //
    //    rResponse:
    //      The final failure response to a NOTIFY.
    //
    //  Description:
    //      A final failure response to a NOTIFY has been received.
    //
    //      NOTIFY requests are sent with ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress, or
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //  See Also:
    //      ISipTransferSvc07, ISipTransferSvc07::ReportRefusal,
    //      ISipTransferSvc07::ReportPending, ISipTransferSvc07::ReportFailure,
    //      ISipTransferSvc07::ReportProgress,
    //      ISipTransferSvc07::ReportFinalStatus.
    //
    //==========================================================================
    virtual void EvNotifyFailure(IN ISipTransferSvc07* pSvc,
                                 IN ISipClientEventControl* pClientEventControl,
                                 IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFinalStatusRequired
    //==
    //==========================================================================
    //
    //  Summary:
    //      The subscription is terminated so a final NOTIFY should be sent.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfer.
    //
    //    pSubscribe:
    //      The SUBSCRIBE request received that terminated the subscription.
    //      It is NULL if this event was called for an expiration of the
    //      subscription.
    //
    //  Description:
    //      The subscription created by the REFER request has terminated.
    //
    //      A SUBSCRIBE request may have been received to terminate the
    //      subscription.
    //
    //      The other possibility is that the subscription is expired. This 
    //      happens only for subscriptions created by accepting a REFER request
    //      through EvTransferred for which no final NOTIFY request has been
    //      sent.
    //
    //      In both cases, a final NOTIFY request should be sent to the
    //      transferor (either with ISipTransferSvc07::ReportFinalStatus,
    //      ISipTransferSvc07::ReportFailure, or 
    //      ISipTransferSvc07::ReportRefusal) (RFC 3265, section 3.1.4.3 for the
    //      SUBSCRIBE and section 3.1.6.4 for the expiration).
    //
    //  See Also:
    //      EvTransferred, ISipTransferSvc07::ReportFinalStatus,
    //      ISipTransferSvc07::ReportFailure, ISipTransferSvc07::ReportRefusal
    //
    //==EDOC====================================================================
    virtual void EvFinalStatusRequired(IN ISipTransferSvc07* pSvc,
                                       IN const CSipPacket* pSubscribe) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      An invalid request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the transfers on the context.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rRequest:
    //      The invalid request.
    //
    //    resError:
    //      resFE_REQUEST_PENDING:
    //          The REFER request has been received when the transfer service 07
    //          is already processing a transfer as a transferee. A 491 response
    //          has automatically been sent by the transfer service 07.
    //
    //      resFE_UNSUPPORTED_URI_SCHEME:
    //          The REFER request has been received with an unsupported URI 
    //          scheme. A 488 "Unsupported Refer-To URI Scheme" response has 
    //          automatically been sent by the transfer service 07.
    //
    //      resFE_MISSING_HEADER:
    //          The REFER request is missing a valid Refer-To header. A "400 Bad
    //          Request" response has already been sent.
    //
    //      resFE_UNKNOWN_EVENT:
    //          The SUBSCRIBE request is missing an Event header or the event
    //          type is not "refer". A "489 Bad Event" response has already
    //          been sent.
    //
    //      resFE_UNKNOWN_SUBSCRIPTION:
    //          The SUBSCRIBE request did not correspond to any REFER sent by
    //          this service. A "481 Subscription does not exist" response has
    //          already been sent.
    //
    //      resFE_EXPIRED_SUBSCRIPTION:
    //          The SUBSCRIBE has been received after a NOTIFY with
    //          "subscription-state" "terminated" was sent or after the
    //          expiration time. A "481 Subscription does not exist" response
    //          has already been sent.
    //
    //      resFE_UNKNOWN_EVENT:
    //          The Event header was missing or for an event type other than
    //          "refer". A "489 Bad Event" response has already been sent.
    //
    //      resFE_UNKNOWN_SUBSCRIPTION:
    //          The NOTIFY does not correspond to a REFER sent by this service.
    //          A "481 Subscription does not exist" response has already been 
    //          sent.
    //
    //      resFE_EXPIRED_SUBSCRIPTION:
    //          The NOTIFY has been received after another NOTIFY with
    //          "subscription-state" "terminated" or after the expiration time.
    //          A "481 Subscription does not exist" response has already been
    //          sent.
    //
    //      resFE_MISSING_HEADER:
    //          The received NOTIFY did not contain the mandatory
    //          "Subscription-State" header.
    //
    //      resFE_INVALID_CONTENT:
    //          The received NOTIFY did not contain a valid "message/sipfrag".
    //
    //  Description:
    //      Warns the application that an invalid request has been received. The
    //      request was automatically responded with an error response by the
    //      service. Usually, the application has no action to take other than
    //      reporting this invalid packet if wanted.
    //
    //      Take note that if the manager of the service is not set, all the
    //      requests are responded with a 500 response.
    //
    //==EDOC====================================================================
    virtual void EvInvalidRequest(IN ISipTransferSvc07* pSvc,
                                  IN mxt_opaque opqApplicationData,
                                  IN const CSipPacket& rRequest,
                                  IN mxt_result resError) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransferMgr07(){}

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipTransferMgr07(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransferMgr07(IN const ISipTransferMgr07& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransferMgr07& operator=(IN const ISipTransferMgr07& rSrc);
};


MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_ISIPTRANSFERMGR07_H

