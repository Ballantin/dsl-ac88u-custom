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
#ifndef MXG_ISIPTRANSFERSVC07_H
#define MXG_ISIPTRANSFERSVC07_H

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
#ifndef MXG_ISIPNOTIFIERSVC_H
#include "SipUserAgent/ISipNotifierSvc.h"
#endif

#ifndef MXG_ISIPREFEREESVC_H
#include "SipUserAgent/ISipRefereeSvc.h" // EIdParameterUsage enum.
#endif


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CHeaderList;
class CNameAddr;
class CSipMessageBody;
class CSipStatusLine;
class ISipClientTransaction;
class ISipUserAgentSvc;
class ISipTransferMgr07;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipTransferSvc07);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipTransferSvc07
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The transfer service 07 implements the RFC 3515 (REFER Method) and the
//  draft-ietf-sipping-cc-transfer-03 for transfer. It is also based on RFC 3265
//  for event notification.
//
//  The transfer service 07 does NOT manage the Referred-By header and related
//  content.
//
//  The transfer service is used to transfer an existing session to another user
//  using the REFER method. It can be used as a transferor and as a transferee.
//
//  The service can manage one transfer as a transferor and as a transferee at
//  the same time. If a REFER request is received while the service manages a
//  transfer as a transferee, it is automatically refused with a 491 "Request
//  Pending" response.
//
//  It can only be used to transfer to other SIP targets and only to trigger
//  INVITE requests. The ISipReferrerSvc (or ISipGenericSvc) can be used to send
//  a REFER that triggers a request other than an INVITE or that has a target
//  other than a SIP-URL.
//
//  If the Transferee is using a GRUU (Globally Routable User Agent, see
//  draft-ietf-sip-gruu-03), the REFER requests should be sent in a new dialog
//  (other than the INVITE one). If not, the REFER requests should be sent
//  inside the INVITE dialog.
//
//  The REFER request creates a subscription if it is accepted by the
//  transferee. It is currently not possible to resubscribe to or unsubscribe
//  from a subscription by sending a SUBSCRIBE request.
//
//  When the transfer service 07 acts as a transferor and receives a SUBSCRIBE
//  corresponding to the REFER request with the Event value set to "refer", it
//  automatically accepts it with a 200 OK response. If the SUBSCRIBE request
//  is an unsubscribe request, the service calls EvFinalStatusRequired on its
//  manager.
//
//  Note that after creating this service, the application should call
//  SetManager on it because otherwise no request can be sent with it and the
//  service refuses all the requests it receives with a 500 "Internal Server
//  Error" response.
//
//  When acting as a transferee, the length of the subscription must be set in
//  the first NOTIFY sent by using the uExpirationInSec parameter of the choosen
//  method (ReportPending or ReportProgress).
//
//  The transfer service 07 does NOT respond to REFER and NOTIFY requests
//  automatically when they are acceptable. The service does not send NOTIFY
//  requests automatically. All these actions are the application's
//  responsibility.
//
//  The ONLY thing this service does automatically is respond to SUBSCRIBE
//  requests and non-acceptable requests.
//
//  This service generates events to the application through the ISipTransferMgr
//  interface.
//
//#The ISipTransferSvc07 is an ECOM object#
//
//  The ISipTransferSvc07 is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipTransferSvc07</B>
//
//  <B>Interface Id: IID_ISipTransferSvc07</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
//  Note: This class follows the rules of draft-ietf-sipping-cc-transfer-03. The
//  class numeration is incremental. Since the last corresponding version in
//  v3.6 was 06, this one is called 07.
//
// Location:
//  SipUserAgent/ISipTransferSvc07.h
//
// See Also:
//  ISipTransferMgr07
//
//==EDOC========================================================================
class ISipTransferSvc07 : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTransferSvc07);
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
    //      Configures the manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //          pMgr is NULL. The manager is not changed in this case.
    //
    //      resS_OK:
    //          Otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipTransferMgr07* pMgr) = 0;


    //==========================================================================
    //==
    //==  Transfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REFER request to initiate a transfer.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    rTarget:
    //      The URL where the remote user-agent should send the request. It is
    //      put in the Refer-To header of the REFER request.
    //
    //    pMessageBody:
    //      Information about the payload to send with the request. It can be
    //      NULL. Ownership is TAKEN.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - If this service has no manager.
    //
    //      - The service already manages a transfer as a transferor.
    //
    //    resFE_INVALID_ARGUMENT:
    //      - rTarget is not a SIP-URI or SIPS-URI.
    //
    //      - rTarget is a SIP-URI or a SIPS-URI but contains a method-param
    //      with a value other than "INVITE".
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The REFER could not be sent.
    //
    //    resS_OK:
    //      The REFER request has been successfully sent.
    //
    //  Description:
    //      This method is used to send a REFER request to the remote host
    //      connected with the parent context. Notification about the request's
    //      progress is received through the ISipTransferMgr07 interface.
    //
    //      This service can only manage one transfer at a time as a transferor.
    //      Calling this method while a transfer is managed as a transferor
    //      fails. A transfer is managed as a transferor when a REFER has been
    //      sent and not finally answered yet or the response was a 2xx and no
    //      final NOTIFY has been received for that transfer.
    //
    //      Note that draft-ietf-sipping-cc-transfer-04 states that if the
    //      Contact URI given by the transferee in the INVITE is a GRUU, the
    //      transfer should be done on a new dialog. It is the caller's
    //      responsibility to create the new context and to call this method on
    //      the new context.
    //
    //  See Also:
    //      ISipTransferMgr07
    //
    //==========================================================================
    virtual mxt_result Transfer(IN mxt_opaque opqTransaction,
                                IN const CNameAddr& rTarget,
                                IN TO CHeaderList* pExtraHeaders,
                                IN TO CSipMessageBody* pMessageBody,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  Transfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a REFER request to initiate an attended transfer.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pUaDialog:
    //      The ISipUserAgentSvc of context of consultation in case of an
    //      attended transfer.
    //
    //      It cannot be NULL.
    //
    //    pMessageBody:
    //      Information about the payload to send with the request. It can be
    //      NULL. Ownership is TAKEN.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //    bUseRemoteAddrInReferTo:
    //      When true the URL where the transferee should send the request will
    //      be taken from ISipUserAgentSvc::GetRemoteAddr(). It will be put in
    //      the Refer-To header of the REFER request. If the URI contains a
    //      Replaces header, it will be replaced by a Replaces header built by
    //      this method. This is the default behavior of the stack.
    //
    //      When false the URL where the transferee should send the request will
    //      be taken from ISipUserAgentSvc::GetCurrentTarget() only if it is a
    //      SIP URI and put in the Refer-To header of the REFER request.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - If this service has no manager.
    //
    //      - The service already manages a transfer as a transferor.
    //
    //    resFE_INVALID_ARGUMENT:
    //      - pUaDialog is NULL.
    //
    //      - pUaDialog is not in a dialog (nothing to replace).
    //
    //      - the remote address of pUaDialog is neither a SIP-URI or a
    //      SIPS-URI.
    //
    //      - the remote address of pUaDialog is a SIP-URI or SIPS-URI but
    //      contains a method-param with a value other than "INVITE".
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The REFER could not be sent.
    //
    //    resS_OK:
    //      The REFER request has successfully been sent.
    //
    //  Description:
    //      This method is used to send a REFER request to the remote host
    //      connected with the parent context to begin an attended transfer. An
    //      attended transfer can be done by calling the transfer target before
    //      the REFER is sent to the transferee. When the INVITE is accepted by
    //      the transfer target, the call is then put on hold and this method is
    //      called on the context related to the transferee with the
    //      ISipUserAgentSvc related to the call to the transfer target.
    //
    //      Notification about the request's progress is received through
    //      the ISipTransferMgr07 interface.
    //
    //      This service can only manage one transfer at a time as a transferor.
    //      Calling this method while a transfer is managed as a transferor
    //      fails. A transfer is managed as a transferor when a REFER has been
    //      sent and not finally answered yet or the response was a 2xx and no
    //      final NOTIFY has been received for that transfer.
    //
    //      Note that draft-ietf-sipping-cc-transfer-04 states that if the
    //      Contact URI given by the transferee in the INVITE is a GRUU, the
    //      transfer should be done on a new dialog. It is the caller's
    //      responsibility to create the new context and to call this method on
    //      the new context.
    //
    //  See Also:
    //      ISipTransferMgr07
    //
    //==========================================================================
    virtual mxt_result Transfer(IN mxt_opaque opqTransaction,
                                IN const ISipUserAgentSvc* pUaDialog,
                                IN TO CHeaderList* pExtraHeaders,
                                IN TO CSipMessageBody* pMessageBody,
                                OUT ISipClientTransaction*& rpTransaction,
                                IN bool bUseRemoteAddrInReferTo = true) = 0;


    //==========================================================================
    //==
    //==  ReportRefusal
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transferor that the transfer has been declined.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    eReason:
    //      The "reason" parameter to put in the "Subscription-State" header.
    //      When the value is eNO_REASON, the parameter is not put in the
    //      "Subscription-State" header. This reason can be used to tell if the
    //      REFER request can be re-attempted and why it was refused. Take note
    //      that other methods reporting status to the transferor use the
    //      eNO_RESOURCE value.
    //
    //    uRetryAfterSec:
    //      The number of seconds to put in the "retry-after" parameter of the
    //      "Subscription-State" header. When the value is 0, no "retry-after"
    //      parameter is put. Note that RFC 3265 defines semantics for this
    //      parameter only if the eReason parameter has the value eNO_REASON,
    //      ePROBATION, or eGIVEUP.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - No transfer is currently managed as a transferee or the original
    //      REFER request has not been accepted yet.
    //
    //      - A transfer is managed as a transferee but the manager is NULL.
    //      This can happen when Clear was called on the context.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The NOTIFY request could not be sent.
    //
    //    resS_OK:
    //      If the final NOTIFY request has been correctly sent.
    //
    //  Description:
    //      This method is used to send a final NOTIFY request to the remote
    //      host connected with the parent context with a payload of "603
    //      Decline". This method should be called when the application knows
    //      that the transfer must be refused after a 202 response has been sent
    //      to the REFER (e.g.: The user decided not to accept it).
    //
    //      If the transfer was attempted but failed, ReportFailure should be
    //      used instead.
    //
    //      The NOTIFY sent by this method is a final NOTIFY so no other NOTIFY
    //      request may be sent for the transfer. The NOTIFY contains a
    //      Subscription-State header set to "terminated" and a reason parameter
    //      depending on eReason.
    //
    //      Note that a final NOTIFY MUST be sent for every REFER accepted
    //      (draft-ietf-sipping-cc-transfer-03, section 3, requirement 3) so
    //      either ReportFailure, ReportFinalStatus, or this method must be
    //      called once for every REFER accepted (responded 2xx).
    //
    //      IMPORTANT: This method does NOT send a response for the REFER
    //      request method, the application must send it on its own.
    //
    //  See Also:
    //      ISipTransferMgr07
    //
    //==========================================================================
    virtual mxt_result ReportRefusal(
                                IN mxt_opaque opqTransaction,
                                IN ISipNotifierSvc::EReason eReason,
                                IN unsigned int uRetryAfterSec,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  ReportPending
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transferor that the transfer is tried.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    uExpirationInSec:
    //      The expiration time of the subscription initiated with the REFER
    //      request. This expiration should be set to the maximum time expected
    //      to obtain a final status for the INVITE request. The value of
    //      uExpirationInSec cannot be increased after the first NOTIFY request
    //      is sent. Only the sender of the REFER can extend the subscription by
    //      sending a SUBSCRIBE request.
    //
    //      This value is put in the "expires" parameter of the
    //      "Subscription-State" header of the NOTIFY request sent. When 0, the
    //      current remaining expiration for that subscription is used. The
    //      application should not use 0 the first time it calls this method (or
    //      ReportProgress) since the initial expiration is set really high so
    //      the application can decrease it to what it wants.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - No transfer is currently managed as a transferee or the original
    //      REFER request has not been accepted yet.
    //
    //      - A transfer is managed as a transferee but the manager is NULL.
    //       This can happen when Clear was called on the context.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The NOTIFY request could not be sent.
    //
    //    resS_OK:
    //      If the final NOTIFY request has been correctly sent.
    //
    //  Description:
    //      This method is used to send a NOTIFY request to the remote host
    //      connected with the parent context with a payload of "100 Trying".
    //      This method should be called when the a 2xx response has been
    //      responded to the REFER request initiating the transfer.
    //
    //      Note that a NOTIFY request MUST immediately be sent to the
    //      transferor when a REFER request is accepted (2xx) (RFC 3515, section
    //      2.4.4).
    //
    //      The Subscription-State header of the NOTIFY is set to "pending".
    //
    //  See Also:
    //      ISipTransferMgr07
    //
    //==========================================================================
    virtual mxt_result ReportPending(
                                IN mxt_opaque opqTransaction,
                                IN unsigned int uExpirationInSec,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  ReportFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transferor that the transfer failed.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      No transfer is currently managed as a transferee or the original
    //      REFER request has not been accepted yet.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The NOTIFY request could not be sent.
    //
    //    resS_OK:
    //      If the final NOTIFY request has been correctly sent.
    //
    //  Description:
    //      This method is used to send a final NOTIFY request to the remote
    //      host connected with the parent context with a payload of "503
    //      Service Unavailable". This method should be called when the transfer
    //      failed (e.g.: The INVITE was refused by the target, it timed up, or
    //      the user got tired of waiting for a final response).
    //
    //      If the transfer is declined by the application before it is
    //      attempted (e.g.: The user has refused to follow the REFER request it
    //      received) and a 2xx response has been sent for the REFER, the
    //      application should send a payload of "603 Declined" (RFC 3515,
    //      section 2.4.5) by using ReportRefusal.
    //
    //      The NOTIFY sent by this method is a final NOTIFY so no other NOTIFY
    //      request may be sent for the transfer. The NOTIFY contains a
    //      Subscription-State header set to "terminated" and a reason parameter
    //      of "noresource".
    //
    //      Note that a final NOTIFY MUST be sent for every REFER accepted
    //      (draft-ietf-sipping-cc-transfer-03, section 3, requirement 3) so
    //      either ReportRefusal, ReportFinalStatus, or this method must be
    //      called once for every REFER accepted (responded 2xx).
    //
    //      IMPORTANT: This method does NOT send a response for the REFER
    //      request, the application must send it on its own.
    //
    //  See Also:
    //      ISipTransferMgr07
    //
    //==========================================================================
    virtual mxt_result ReportFailure(
                                IN mxt_opaque opqTransaction,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  ReportProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transferor of the transfer progress.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    uExpirationInSec:
    //      The expiration time of the subscription initiated with the REFER
    //      request. This expiration should be set to the maximum time expected
    //      to obtain a final status for the INVITE request. The value of
    //      uExpirationInSec cannot be increased after the first NOTIFY request
    //      is sent. Only the sender of the REFER can extend the subscription by
    //      sending a SUBSCRIBE request.
    //
    //      This value is put in the "expires" parameter of the
    //      "Subscription-State" header of the NOTIFY request sent. When 0, the
    //      current remaining expiration for that subscription is used. The
    //      application should not use 0 the first time it calls this method (or
    //      ReportProgress) since the initial expiration is set really high so
    //      the application can decrease it to what it wants.
    //
    //    rContentStatusLine:
    //      The status-line to put in the content of the NOTIFY request. It is
    //      the actual referral status. Note that it can be a final response
    //      when other responses are still expected (because the application
    //      will retry the referred request for instance). When no other
    //      responses are expected, ReportFinalStatus should be used instead.
    //
    //      When this method is used to report a progress received from the
    //      transfer target, you can take the status line of the packet by using
    //      CSipPacket::GetStatusLine.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - No transfer is currently managed as a transferee or the original
    //      REFER request has not been accepted yet.
    //
    //      - A transfer is managed as a transferee but the manager is NULL. This
    //      can happen when Clear was called on the context.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The NOTIFY request could not be sent.
    //
    //    resS_OK:
    //      If the final NOTIFY request has been correctly sent.
    //
    //  Description:
    //      This method is used to send a NOTIFY request to the remote host
    //      connected with the parent context with a payload corresponding to
    //      the status line in parameter. This method should be called when
    //      responses (provisional or final) are received for the INVITE
    //      triggered by the reception of a REFER, and other responses will
    //      still be received for the INVITE (because the application will retry
    //      the INVITE for instance in case of a final response).
    //
    //      The Subscription-State header is set to "active" with this method,
    //      meaning that the transferor will receive other NOTIFY requests for
    //      further progress.
    //
    //      When no other response is expected from the transfer target,
    //      ReportFinalStatus should be used instead of this method.
    //
    //      IMPORTANT: This method does NOT send a response for the REFER
    //      request, the application must send it on its own.
    //
    //  See Also:
    //      ReportFinalStatus, ISipTransferMgr07, CSipPacket::GetStatusLine
    //
    //==========================================================================
    virtual mxt_result ReportProgress(
                                IN mxt_opaque opqTransaction,
                                IN unsigned int uExpirationInSec,
                                IN const CSipStatusLine& rContentStatusLine,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  ReportFinalStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transferor of the transfer progress and informs it
    //      that this is the last information it will receive on the progress.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    rContentStatusLine:
    //      The status-line to put in the content of the NOTIFY request. It is
    //      the actual referral status. Note that this response can be either
    //      final or provisional. The fact that this NOTIFY is final simply
    //      indicates to the transferor that the transferee will no longer
    //      inform it of the refer request progress.
    //
    //      When this method is used to report a progress received from the
    //      transfer target (on the network), you can take the status line of
    //      the packet by using CSipPacket::GetStatusLine.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. It can be NULL.
    //      Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction. This interface is a
    //      reference counted object and a reference is counted for that
    //      parameter. The application is responsible for that reference.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      - No transfer is currently managed as a transferee or the original
    //      REFER request has not been accepted yet.
    //      - A transfer is managed as a transferee but the manager is NULL.
    //     This can happen when Clear was called on the context.
    //
    //    resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //          blocked the packet. No additional event will be reported.
    //
    //    resFE_FAIL:
    //      The NOTIFY request could not be sent.
    //
    //    resS_OK:
    //      If the final NOTIFY request has been correctly sent.
    //
    //  Description:
    //      This method is used to send a NOTIFY request to the remote host
    //      connected with the parent context with a payload corresponding to
    //      rContentStatusLine.
    //
    //      The Subscription-State header of the NOTIFY sent is set to
    //      "terminated" and a reason parameter of "noresource".
    //
    //      When other responses are expected from the transfer target,
    //      ReportProgress should be used instead of this method.
    //
    //      The NOTIFY sent by this method is a final NOTIFY so no other NOTIFY
    //      request may be sent for the transfer.
    //
    //      Note that a final NOTIFY MUST be sent for every REFER accepted
    //      (draft-ietf-sipping-cc-transfer-03, section 3, requirement 3) so
    //      either ReportRefusal, ReportFailure, or this method must be called
    //      once for every REFER accepted (responded 2xx).
    //
    //      RFC 3515 states that a final NOTIFY MUST indicate the subscription
    //      has been "terminated" with a reason of "noresource", except when the
    //      REFER was accepted with a 202 but approval to follow the reference
    //      was then refused (section 2.4.7). In the second case, ReportRefusal
    //      should be used instead of ReportFinalStatus.
    //
    //      IMPORTANT: This method does NOT send a response for the REFER
    //      request, the application must send it on its own.
    //
    //  See Also:
    //      ISipTransferMgr07, CSipPacket::GetStatusLine
    //
    //==========================================================================
    virtual mxt_result ReportFinalStatus(
                                IN mxt_opaque opqTransaction,
                                IN const CSipStatusLine& rContentStatusLine,
                                IN TO CHeaderList* pExtraHeaders,
                                OUT ISipClientTransaction*& rpTransaction) = 0;


    //==========================================================================
    //==
    //==  ConfigureNotifyIdParameterUsage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures whether or not the stack will use the "id" parameter in
    //      the Event header of NOTIFY requests for first REFER request.
    //
    //  Parameters:
    //    eIdParamUsage:
    //      - eID_PARAM_ALWAYS_PRESENT:
    //          The id parameter is added to all NOTIFYs. This is the default
    //          behaviour.
    //
    //      - eID_PARAM_ABSENT_FOR_FIRST_REFER:
    //          The id parameter in the NOTIFYs to the first REFER are not set.
    //
    //  Description:
    //      Configures whether or not the stack will use the "id" parameter in
    //      the Event header of the NOTIFY it sends after a subscription is
    //      created by receiving a REFER. This is used only for the NOTIFY(s)
    //      associated with the first REFER received by this service.
    //
    //      RFC 3515 states that the NOTIFY(s) associated with the first
    //      REFER sent on a dialog MAY contain an "id" parameter in the Event
    //      header, and the NOTIFY(s) associated with any additional REFER
    //      after the first one MUST contain this parameter. The default
    //      behavior of the stack is to always include the parameter for all
    //      NOTIFY(s) it sends, however this can sometimes cause
    //      interoperability issues. An application can use this method to
    //      configure whether or not the stack will include the "id" parameter
    //      for the NOTIFY(s) associated with the first REFER it has
    //      received. All NOTIFYs associated with subsequent REFERs will always
    //      contain the "id" parameter in the Event header.
    //
    //  See Also:
    //      ISipRefereeSvc
    //
    //==========================================================================
    virtual void ConfigureNotifyIdParameterUsage(IN ISipRefereeSvc::EIdParameterUsage eIdParamUsage) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransferSvc07(){}

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~ISipTransferSvc07(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransferSvc07(IN const ISipTransferSvc07& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransferSvc07& operator=(IN const ISipTransferSvc07& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPTRANSFERSVC07_H

