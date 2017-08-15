//==============================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T and Media5 Corporation.
//
//   M5T and Media5 Corporation reserve all rights to this document as well as
//   to the Intellectual Property of the document and the technology and
//   know-how that it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T and Media5
//   Corporation.
//
//   M5T and Media5 Corporation reserve the right to revise this publication and
//   make changes at any time and without the obligation to notify any person
//   and/or entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_IUASSPCALLMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPCALLMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class CString;
class IUaSspCall;
class IEComUnknown;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspCallMgr
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This is the interface through which all events related to a call are
//   reported to the application.
//
// Description:
//   This is the interface through which all events related to a call are
//   reported to the application.
//
// Location:
//   UaSsp/IUaSspCallMgr.h
//
// See Also:
//   IUaSspCall
//
//==============================================================================
class IUaSspCallMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==========================================================================
    //==
    //==  EvCalled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that there is a new incoming call.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP Call object to which the event applies.
    //
    //       It cannot be NULL.
    //
    //      rFromAddr:
    //       The address of the remote party (From header).
    //
    //      rToAddr:
    //       The address of the target party (To header).
    //
    //      pAdditionalParameters:
    //       Additional parameters in relation to the unhandled request. Can be
    //       NULL if no additional parameters are present.
    //
    //       This returned parameter is an ECOM object that can be queried for
    //       all the interfaces needed to be watched or processed by the
    //       application from the Base Component or from specific component
    //       interfaces.
    //
    //       If it is NULL, it means no additional parameters could be returned
    //       to the application, either because there was a processing problem
    //       or that no server event yielded information worth returning.
    //
    //       Information directly related to the IUaSspCall can be retrieved by
    //       querying the IUaSspCallEventInfo interface on
    //       pAdditionalParameters.
    //
    //       Particularly, there could be a replaced call that could be
    //       retrieved through IUaSspCallEventInfo::GetReplacedCall.
    //
    //       If there is a replaced call and the application wants the new call
    //       (pCall) to further progress, it must create and associate an MSP
    //       offer answer session object with this call object (pCall). The MSP
    //       offer answer session object should be configured to use the same
    //       hardware resources as the ones that were used for the replaced
    //       call. The existing call (IUaSspCallEventInfo::GetReplacedCall) will
    //       automatically be terminated and EvTerminated will be reported when
    //       pCall is accepted by the implementation, using
    //       IUaSspCall::AcceptCall.
    //
    //  Description:
    //      This event is triggered when a remote party is placing a call. After
    //      this event is reported, the implementation can forward or reject the
    //      incoming call. If it wants the call to further progress, it must
    //      create and associate an MSP Offer Answer Session with the Call
    //      object.
    //
    //      The implementation MUST NOT notify the local user of a new incoming
    //      call as a result of this event. The implementation should inform the
    //      user about the new call only when it has knowledge that all
    //      components will accept the call. In particular, the
    //      IMspOfferAnswerSession must have been set on the IUaSspCall and the
    //      resources should have been reserved for it.
    //
    //      When the local user is notified about the incoming call (e.g. the
    //      phone is ringing), IUaSspCall::CallUserNotified must be called.
    //
    //  #OPTIONS request handling:#
    //      This event is reported for an initial INVITE but also for
    //      'out-of-dialog' OPTIONS request. When this occurs, the
    //      pAdditionalParameters indicates this through the
    //      IUaSspCallEventInfo::IsOptionsRequest method. The application should
    //      check whether or not that method returns true. When it returns true,
    //      four operations can be performed: accept the call, reject the call,
    //      forward the call or terminate the call. Each of these operations are
    //      discussed below.
    //
    //      When an 'out-of-dialog' OPTIONS request arrives and the application
    //      is ready to accept a call, it should set a MSP session and be ready
    //      to receive EvTerminated. Setting a MSP session on a call component
    //      that is handling an 'out-of-dialog' OPTIONS request automatically
    //      sends a 200 OK that publishes all the UA capabilities to the peer.
    //
    //      When an 'out-of-dialog' OPTIONS request arrives and the application
    //      is not ready to accept a call and knows of another UA that is or,
    //      for security reasons, would prefer to advertise its capabilities
    //      over a secured channel, it should call IUaSspCall::ForwardCall and
    //      provide the SIP/SIPS URI to contact.
    //
    //      When an 'out-of-dialog' OPTIONS request arrives and the application
    //      is not ready to accept a call but wants to advertise its
    //      capabilities to the peer, it should call IUaSspCall::RejectCall.
    //      Doing so answers a final negative response to the peer but that
    //      response publishes all the UA capabilities.
    //
    //      When an 'out-of-dialog' OPTIONS request arrives and the application
    //      is not ready to accept a call and does not want to advertise its
    //      capabilities to the peer, it should call IUaSspCall::TerminateCall.
    //      Doing so answers a 487 final negative response to the peer and
    //      the response does not publish any capabilities.
    //
    //      In all the above scenarios, the application will receive a
    //      EvTerminated event shortly after it took action on the request.
    //
    //  See Also:
    //      IUaSspCall::CallUserNotified, IUaSspCall::ForwardCall,
    //      IUaSspCall::RejectCall, IUaSspCall::SetMspSession,
    //      IUaSspCallEventInfo::GetReplacedCall
    //
    //==========================================================================
    virtual void EvCalled(IN IUaSspCall* pCall,
                          IN const CNameAddr& rFromAddr,
                          IN const CNameAddr& rToAddr,
                          IN IEComUnknown* pAdditionalParameters) = 0;

    //==========================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the progress of an outgoing call.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP call object to which the event applies.
    //
    //       It cannot be NULL.
    //
    //      uStatusCode:
    //       SIP status code corresponding to the progression state of the call.
    //       It is always within 100 and 199 or within 300 and 599. It can be
    //       between 400 and 599 when an initial INVITE received a 3xx
    //       containing more than one Contact header. If the first Contact that
    //       is tried is responded with a final negative response (>= 4xx and
    //       < 600), then EvProgress is reported with that response code and the
    //       second contact is tried.
    //
    //      rstrDisplayString:
    //       The string associated with the status code, as received from the
    //       peer.
    //
    //      pForwardedAddr:
    //       When the call is forwarded to another peer, it contains the
    //       name-addr of the new peer.
    //
    //       On provisional responses (1xx), this parameter is NULL.
    //
    //  Description:
    //      This event is triggered when the remote party's device is ringing
    //      (i.e. a 180 Ringing has been received) or when any informational
    //      (1xx), redirection (3xx) or < 600 failure responses after a 3xx
    //      status code arrives.
    //
    //      Note that after this event is called for a provisional response
    //      (1xx), the call cannot timeout internally. It is the manager's
    //      responsibility to terminate the call if it feels that it has waited
    //      for enough time before it is established.
    //
    //      This event can be called with a status code within 400 and 599 when
    //      the initial INVITE is redirected with more than one Contact. In this
    //      case, this method can be called with a code between 400 and 599 if
    //      the redirection to a Contact fails with one of these status codes
    //      and there is at least another Contact to try.
    //
    //      The following is a callflow to illustrate the example:
    //
    //  <CODE>
    //   UA SSP
    //      |--INV->
    //      |<-302-- // contains two Contact headers, EvProgress(302)
    //      |--ACK->
    //      |
    //      |--INV-> // INVITE to first Contact
    //      |<-486-- // EvProgress(486)
    //      |--ACK->
    //      |
    //      |--INV-> // INVITE to second Contact
    //      |<-486-- // EvFailed (no more Contact to try)
    //      |--ACK->
    //  </CODE>
    //
    //==========================================================================
    virtual void EvProgress(IN IUaSspCall* pCall,
                            IN uint16_t uStatusCode,
                            IN const CString& rstrDisplayString,
                            IN const CNameAddr* pForwardedAddr) = 0;

    //==========================================================================
    //==
    //==  EvForkedCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that another peer answered the original INVITE.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP call object to which the event applies.
    //
    //       It cannot be NULL.
    //
    //      pNewCall:
    //       The newly created IUaSspCall instance.
    //
    //       It cannot be NULL.
    //
    //  Description:
    //      Reports that another peer (different from the first one) answered
    //      the original INVITE. In this situation, a new IUaSspCall is created
    //      to handle the call with the different peer.
    //
    //      In this method, the only thing that the manager MUST do on pNewCall
    //      is to set a manager on it (i.e. a IUaSspCallMgr on pNewCall). This
    //      must be done SYNCHRONOUSLY.
    //
    //      The application could also keep state relating calls together in
    //      this method.
    //
    //      Note that the ISceUserConfig will automatically be set on pNewCall
    //      by the caller of this method. The configured ISceUserConfig will be
    //      the one of pCall.
    //
    //==========================================================================
    virtual void EvForkedCall(IN IUaSspCall* pCall,
                              IN IUaSspCall* pNewCall) = 0;

    //==========================================================================
    //==
    //==  EvAnswered
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the peer has accepted the outgoing call.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP call object to which the event applies.
    //
    //       It cannot be NULL.
    //
// BRCM_CUSTOM - [add] Add TIP support per 3GPP TS 22.173
    //      pAdditionalParameters:
    //       Additional parameters in relation to the response. Can be
    //       NULL if no additional parameters are present.
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173
    //  Description:
    //      This event is triggered when a remote party accepts (answers) the
    //      outgoing call (i.e. sends a 2xx to the initial INVITE).
    //
    //==========================================================================
// BRCM_CUSTOM - [mod] Add TIP support per 3GPP TS 22.173
    virtual void EvAnswered(IN IUaSspCall* pCall,
                            IN IEComUnknown* pAdditionalParameters = NULL) = 0;
// BRCM_CUSTOM - [end] Add TIP support per 3GPP TS 22.173

    //==========================================================================
    //==
    //==  EvFailed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the outgoing call has failed.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP call object to which the event applies.
    //
    //       It cannot be NULL.
    //
    //      uStatusCode:
    //       SIP status code corresponding to the failure response code that was
    //       received. It is always greater of equal to 300.
    //
    //      rStrReason:
    //       The reason phrase describing why the call has failed.
    //
    //      uRetryAfter:
    //       The number of seconds to wait before trying to call the same
    //       destination. This is a hint provided by the server or contacted
    //       user, telling the caller for how long it thinks the call will fail
    //       for the specified reason. This value is taken from the Retry-After
    //       header.
    //
    //       0 if no Retry-After was present in the SIP packet.
    //
    //  Description:
    //      This event is triggered when the remote party sends a final failure
    //      response to the initial INVITE.
    //
    //      This event is only reported for an initial INVITE. It is not
    //      reported for a re-INVITE that has failed.
    //
    //      The user of the call should not release all references it has on
    //      this call. The user should wait for the EvTerminated event before
    //      doing so.
    //
    //==========================================================================
    virtual void EvFailed(IN IUaSspCall* pCall,
                          IN uint16_t uStatusCode,
                          IN const CString& rStrReason,
                          IN unsigned int uRetryAfter) = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_UASSP_STRUCTSENUMS>
    //
    // Summary:
    //   This enum contains the possible termination reasons.
    //
    // Description:
    //   This enum contains the possible termination reasons. This reason is
    //   provided as a parameter to EvTerminated.
    //
    //   The reasons values are grouped by category:
    //   - User action    (0000 - 0999): Locally initiated action from the user
    //                                   of the call resulted in call
    //                                   termination.
    //   - Peer action    (1000 - 1999): An action initiated by one or more
    //                                   peers resulted in call termination.
    //   - External event (2000 - 2999): An external event to the call has the
    //                                   effect of terminating it.
    //   - Protocol error (3000 - 3999): Protocol error resulted in call
    //                                   termination.
    //   - SCE error      (4000 - 4999): SIP Client Engine error at the call or
    //                                   MSP level.
    //
    //==========================================================================
    enum ETerminationReason
    {
        // User action group:

        // Description:
        //  The user requested the immediate termination of the object using the
        //  Abort method. No packets are sent to the peer.
        eTERMINATION_USER_ABORTED = 0,

        // Description:
        //  The user requested termination using IUaSspCall::TerminateCall.
        eTERMINATION_USER_TERMINATED,

        // Description:
        //  The call was forwarded by the application.
        eTERMINATION_USER_FORWARDED,

        // Description:
        //  The call was rejected by the application.
        eTERMINATION_USER_REJECTED,

        // Description:
        //  The out-of-dialog OPTION was answered by the application
        //  when it set the MSP.
        eTERMINATION_USER_OOD_OPTION_ANSWERED,

        // Peer action group:

        // Description:
        //  The peer requested termination by sending a BYE request.
        eTERMINATION_PEER_TERMINATED = 1000,

        // Description:
        //  The call was replaced by an other call from a peer.
        eTERMINATION_PEER_REPLACED,

        // Description:
        //  The incoming call request was cancelled by the peer.
        eTERMINATION_PEER_INVITE_CANCELLED,

        // Description:
        //  A failure response (>=400) was received for the current call
        //  attempt.
        eTERMINATION_PEER_INVITE_REJECTED,

        // External event group:

        // Description:
        //  The call component was terminated as a result of information from
        //  some other component that all existing dialogs associated with this
        //  user must be terminated.
        eTERMINATION_EXTERNAL_TERMINATE_ALL_USER_DIALOGS = 2000,

        // Protocol error group:

        // Description:
        //  The call (and MSP) failed to negociate media capabilities.
        eTERMINATION_PROTOCOL_ERROR_MEDIA_NEGOCIATION_FAILURE = 3000,

        // Description:
        //  The peer required capability are not supported.
        eTERMINATION_PROTOCOL_ERROR_UNSUPPORTED_SIP_CAPABILITY,

        // Description:
        //  A wrong dialog is trying to be replaced.
        eTERMINATION_PROTOCOL_ERROR_REPLACES,

        // Description:
        //  The session-timer elapsed without refresh.
        eTERMINATION_PROTOCOL_ERROR_SESSION_TIMER_EXPIRED,

        // Description:
        //  The negociated session-timer value is too short.
        eTERMINATION_PROTOCOL_ERROR_SESSION_TIMER_TOO_SHORT,

        // Description:
        //  General protocol error.
        eTERMINATION_PROTOCOL_ERROR_GENERAL,


//M5T_INTERNAL_USE_BEGIN
        // MXI_DESIGN_NOTES (2009-04-06):
        // These error codes reflect other possible termination reason, but
        // since there is nothing that the application can really do about them,
        // they we combined in eTERMINATION_PROTOCOL_ERROR_GENERAL.

        // Description:
        //  A timeout occurred when waiting for the ACK.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_ACK_TIMEOUT,
        // Description:
        //  The payload contained in a request/answer cannot be extracted.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        //  eTERMINATION_PROTOCOL_ERROR_CANNOT_EXTRACT_PAYLOAD,
        // Description:
        //  A failure response (>=400) was received for the current UPDATE transaction.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_UPDATE,
        // Description:
        //  A reliable provisional response timed out.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_RPR_TIMEOUT,
        // Description:
        //  A failure response was received for a PRACK. This should never be.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_PRACK_REJECTED,
        // Description:
        //  An invalid request was received.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_INVALID_REQUEST,
        // Description:
        //  Invalid state to receive an offer or answer.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_INVALID_OA_STATE,
        // Description:
        //  A reliable provisional response is already being handled.
        // Maps to eTERMINATION_PROTOCOL_ERROR_GENERAL
        // eTERMINATION_PROTOCOL_ERROR_PRACK_2ND_RESPONSE,
//M5T_INTERNAL_USE_END

        // SCE error group:

        // Description:
        //  An incoming call could not be accepted as there was no
        //  configuration set on it.
        eTERMINATION_SCE_ERROR_CONFIG,

        // Description:
        //  The MSP encountered a terminal error and must terminate.
        eTERMINATION_SCE_ERROR_MSP_TERMINAL_FAILURE,

        // Description:
        //  A problem has been encountered while trying to handle a forked
        //  response.
        eTERMINATION_SCE_ERROR_FORKING,

        // Description:
        //  General error from the SIP Client Engine.
        eTERMINATION_SCE_ERROR_GENERAL,

//M5T_INTERNAL_USE_BEGIN
        // MXI_DESIGN_NOTES (2009-04-06):
        // These error codes reflect other possible termination reason, but
        // since there is nothing that the application can really do about them,
        // they we combined in eTERMINATION_SCE_ERROR_GENERAL.

        // Description:
        //  The original INVITE cannot be handled
        //  (i.e. the configuration is not set, the context could not be created, etc.).
        // eTERMINATION_SCE_ERROR_SERVER_INVITE,
        // Description:
        //  The current request or response cannot be sent.
        // eTERMINATION_SCE_ERROR_CANNOT_SEND_PACKET,
        // Description:
        //  The context could not be properly configured.
        // eTERMINATION_SCE_ERROR_CONTEXT_CONFIG,
        // Description:
        //  The base component reported an initial transaction error.
        // eTERMINATION_SCE_ERROR_INITIAL_TRANSACTION_ERROR,
        // Description:
        //  The received request was rejected by the base component.
        // eTERMINATION_SCE_ERROR_REJECTED_BY_BASE_COMPONENT,
//M5T_INTERNAL_USE_END

        // Description:
        //  The reason was not set, termination may or may not be caused by an
        //  error. MUST always be overriden to another reason.
        eTERMINATION_REASON_NOT_SET,
    };

    //==========================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the call has been terminated.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP call object to which the event applies.
    //
    //       It cannot be NULL.
    //
    //      bIsApplicationRequested:
    //       - true:
    //          the call ending was initiated by the application through
    //          TerminateCall, RejectCall or ForwardCall.
    //       - false:
    //          the remote peer initiated the call release or a protocol
    //          error forced the call termination.
    //
    //  Description:
    //      This event is triggered when the call is terminated. Five cases
    //      trigger this event:
    //       - The remote party either sends a CANCEL for an initial INVITE or
    //          a BYE request on the present call. A 200 OK response is
    //          automatically sent back.
    //       - The remote party responded a 200 OK to the local party's BYE or
    //          a final negative response (>=300) for a cancelled initial
    //          INVITE.
    //       - IUaSspCall::TerminateCall, IUaSspCall::RejectCall or
    //          IUaSspCall::ForwardCall was called and was successfully
    //          processed.
    //       - There was an error while processing an incoming initial INVITE.
    //       - The initial INVITE was rejected.
    //
    //      The user of the call should release all references it has on this
    //      call.
    //
    //==========================================================================
    virtual void EvTerminated(IN IUaSspCall* pCall,
                              IN ETerminationReason eReason) = 0;

    //==========================================================================
    //==
    //==  EvTransfereeTransferRequested
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the call is being transferred to another party.
    //
    //  Parameters:
    //      pCall:
    //       The IUaSspCall on which the transfer request was received.
    //
    //       Cannot be NULL.
    //
    //      rTarget:
    //       The target address of the transfer to which the call is directed.
    //
    //       Headers were removed from the contained SIP-URI before this event
    //       is called.
    //
    //  Description:
    //      This event is triggered when the remote party requests the call to
    //      be transferred to the target name-addr. At that point, the user
    //      should call either IUaSspCall::AcceptTransfer or
    //      IUaSspCall::DeclineTransfer.
    //
    //      If the user wants to be transferred to the offered destination, it
    //      has to create a new IUaSspCall and set it up correctly (i.e. as
    //      if it would call MakeCall). Then, IUaSspCall::AcceptTransfer should
    //      be called. IUaSspCall::AcceptTransfer will take care of the call
    //      establishment.
    //
    //      When this event is reported, the user agent is acting as the
    //      transferee in the transfer.
    //
    //==========================================================================
    virtual void EvTransfereeTransferRequested(IN IUaSspCall* pCall,
                                               IN const CNameAddr& rTarget) = 0;

    //==========================================================================
    //<GROUP TID_PACKAGE_UASSP_STRUCTSENUMS>
    //
    // Summary:
    //   This enum contains the possible transfer result.
    //
    // Description:
    //  This enum contains the possible transfer result.
    //
    //==========================================================================
    enum ETransferReport
    {
        // Description:
        //  The REFER request initiating the transfer has been answered with a
        //  1xx by the transferee.
        eTRANSFER_RESULT_REFER_IN_PROGRESS,
        // Description:
        //  The REFER request initiating the transfer has been answered with a
        //  2xx by the transferee.
        eTRANSFER_RESULT_REFER_SUCCESS,
        // Description:
        //  The REFER request initiating the transfer has been answered with a
        //  >= 3xx by the transferee.
        eTRANSFER_RESULT_REFER_FAILURE,
        // Description:
        //  The transfer is in progress. This result is reported when a NOTIFY
        //  is received by the transferor. This is NOT a final report.
        eTRANSFER_RESULT_IN_PROGRESS,
        // Description:
        //  The final result of the transfer cannot be known by the call
        //  generating the event. The only thing that is known is that the
        //  transfer is still processing. This is a final result.
        eTRANSFER_RESULT_PROGRESSING,
        // Description:
        //  The transfer succeeded. This is a final result.
        eTRANSFER_RESULT_SUCCESS,
        // Description:
        //  The transfer failed. This is a final result.
        eTRANSFER_RESULT_FAILURE
    };

    //==========================================================================
    //==
    //==  EvTransfereeTransferResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the result of the accepted transfer.
    //
    //  Parameters:
    //      pCall:
    //       The IUaSspCall on which AcceptTransfer has been called. This is the
    //       call with the transferor, on which the REFER request is received.
    //
    //       Cannot be NULL.
    //
    //      eTransferResult:
    //       - eTRANSFER_RESULT_PROGRESSING:
    //          pCall cannot know if the transfer ends to be a success or a
    //          failure. This will happen when pCall is terminated before the
    //          call with the transfer target is accepted or rejected.
    //       - eTRANSFER_RESULT_SUCCESS:
    //          The call with the transfer target is accepted.
    //       - eTRANSFER_RESULT_FAILURE:
    //          The call with the transfer target is rejected.
    //
    //  Description:
    //      This event is reported when an accepted transfer request is
    //      completed. eTransferResult indicates the result of the transfer.
    //
    //      This method is called when the new call is accepted or rejected by
    //      the transfer target.
    //
    //      Normally, if eTransferResult is a success, the new call passed in
    //      IUaSspCall::AcceptTransfer should replace pCall and pCall should be
    //      terminated.
    //
    //      When this event is reported, the user agent is acting as the
    //      transferee in the transfer.
    //
    //  See Also:
    //      ETransferReport
    //
    //==========================================================================
    virtual void EvTransfereeTransferResult(IN IUaSspCall* pCall,
                                            IN ETransferReport eTransferResult) = 0;

    //==========================================================================
    //==
    //==  EvTransferorTransferResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports the status of a locally-initiated transfer.
    //
    //  Parameters:
    //      pCall:
    //       The IUaSspCall on which one of the IUaSspCall::Transfer methods has
    //       been called. This is the call with the transferee.
    //
    //       Cannot be NULL.
    //
    //      eTransferResult:
    //       - eTRANSFER_RESULT_REFER_IN_PROGRESS:
    //          the REFER request has been answered with a 1xx response.
    //       - eTRANSFER_RESULT_REFER_SUCCESS:
    //          the REFER request has been answered with a 2xx response.
    //       - eTRANSFER_RESULT_REFER_FAILURE:
    //          the REFER request has been answered with a >=3xx response.
    //       - eTRANSFER_RESULT_IN_PROGRESS:
    //          The transfer is in progress. This result is reported when a
    //          NOTIFY is received by the transferor.
    //       - eTRANSFER_RESULT_PROGRESSING:
    //          A final NOTIFY containing a provisional Status-Line was received
    //          or no final notification is received in time. When this is
    //          reported, it means that pCall cannot know if the transfer ends
    //          to be a success or a failure.
    //       - eTRANSFER_RESULT_SUCCESS:
    //          A final NOTIFY containing a 2xx Status-Line is received.
    //       - eTRANSFER_RESULT_FAILURE:
    //          A final NOTIFY containing a final failure Status-Line is
    //          received or the REFER is rejected.
    //
    //  Description:
    //      This event is triggered upon the reception of a NOTIFY message or
    //      packets that are considered final from a transferor point of view.
    //
    //      A transfer is considered successful when a successful final report
    //      (i.e. a final NOTIFY with a 2xx sipfrag content) has been received.
    //
    //      A transfer is considered failed if either the REFER request has
    //      received a final negative response or the final report contains
    //      a negative response.
    //
    //      If the transfer failed, was an attended transfer and the call with
    //      the original transfer target is accepted, the transfer can be tried
    //      in the other way. This means switching transferee and transfer
    //      target roles by calling IUaSspCall::Transfer on the call originally
    //      with the transfer target and passing pCall to IUaSspCall::Transfer).
    //
    //      When this event is reported, the user agent is acting as the
    //      transferor in the transfer.
    //
    //  See Also:
    //      ETransferReport
    //
    //==========================================================================
    virtual void EvTransferorTransferResult(IN IUaSspCall* pCall,
                                            IN ETransferReport eTransferResult) = 0;

// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
    //==========================================================================
    //==
    //==  EvConnectedIdChange
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that there is a change in local or remote display name or
    //      address on the call thats is passed to this function.
    //
    //  Parameters:
    //      pCall:
    //       The UA SSP Call object to which the event applies.
    //       It cannot be NULL.
    //
    //      rLocalAddr:
    //       The new name/address of the local party.
    //
    //      rRemoteAddr:
    //       The new name/address of the remote party.
    //
    //  Description:
    //     EvConnectedIdChange is called when an to-from change is made. This event includes
    //      the local and/or remote address that has been changed on this dialog. This 
    //      adds connected ID support per RFC 4916.
    //==========================================================================
    virtual void EvConnectedIdChange(IN IUaSspCall* pCall,
                                     IN const CNameAddr& rLocalAddr,
                                     IN const CNameAddr& rRemoteAddr) = 0;
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaSspCallMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaSspCallMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaSspCallMgr(IN const IUaSspCallMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaSspCallMgr& operator=(IN const IUaSspCallMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPCALLMGR_H

