//==============================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_IUASSPCALL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IUASSPCALL_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_IUASSPCALLMGR_H
#include "SceUaSsp/IUaSspCallMgr.h"
#endif

#ifndef MXG_STATUSCODE_H
#include "SipParser/StatusCode.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CNameAddr;
class CSceCallerPreferences;
class CSipPacket;
class CSipStatusLine;
class IMspOfferAnswerSession;
class ISceUserConfig;
class ISipUserAgentSvc;
class CHeaderList;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IUaSspCall);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IUaSspCall
//========================================
//<GROUP TID_PACKAGE_UASSP_CLASSES>
//
// Summary:
//   This interface describes the operations that can be performed on a
//   call.
//
// Description:
//  This interface describes the operations that can be performed on a
//  call.
//
//  The call is configured with the information found in the configuration
//  object passed to SetConfiguration.
//
// Location:
//   UaSsp/IUaSspCall.h
//
// See Also:
//   IUaSspCallMgr
//
//==============================================================================
class IUaSspCall : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//-- New public types
//------------------------
public:


//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IUaSspCall);
//M5T_INTERNAL_USE_END


    //==========================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager to which events are reported for this call.
    //
    //  Parameters:
    //      pMgr:
    //       Pointer to the manager. It must not be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pMgr is NULL.
    //      - resS_OK:
    //         The manager is set.
    //
    //  Description:
    //      Configures the manager to which events are reported for this call.
    //
    //      IMPORTANT: This method MUST be called before a call is initiated.
    //
    //==========================================================================
    virtual mxt_result SetManager(IN IUaSspCallMgr* pMgr) = 0;


    //==========================================================================
    //==
    //==  SetConfiguration
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the call with a configuration object.
    //
    //  Parameters:
    //      pConfig:
    //       Pointer to the ECOM configuration object for this user.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pConfig is NULL or the ISceUserConfig interface cannot be queried
    //         from pConfig.
    //      - resFE_INVALID_STATE:
    //         A configuration object has already been set.
    //      - resS_OK:
    //         The configuration object has been set.
    //
    //  Description:
    //      Configures the call with a configuration object.
    //
    //      IMPORTANT: This method MUST be called before a call is initiated.
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
    //      Gets the call's configuration object.
    //
    //  Parameters:
    //    rpConfig:
    //      Pointer to the ECOM configuration object for this user. If the
    //      pointer returned is not NULL, its reference will have been increased
    //      by one. The caller must release this reference when it is done with
    //      the object.
    //
    //  Description:
    //      Returns the configuration object associated with the call.
    //
    //==========================================================================
    virtual void GetConfiguration(OUT ISceUserConfig*& rpConfig) = 0;


    //==========================================================================
    //==
    //==  SetMspSession
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the MSP Session to use with this call.
    //
    //  Parameters:
    //      pMspSession:
    //       Pointer to an ECOM object that implements the various MSP Session
    //       interfaces. MUST not be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         pMspSession is NULL or the IMspOfferAnswerSession interface
    //         cannot be queried from pMspSession.
    //      - resFE_INVALID_STATE:
    //         A MSP session object has already been set.
    //      - resS_OK:
    //         The MSP session has been set.
    //
    //  Description:
    //      This method configures the MSP Session this call object must use to
    //      access the media stream information.
    //
    //      For client scenarios, the MSP Session must be configured before
    //      MakeCall is called.
    //
    //      For server scenarios, the MSP Session must be configured before
    //      AcceptCall is called. It is possible to call ForwardCall or
    //      RejectCall without associating an MSP Session with the call.
    //
    //==========================================================================
    virtual mxt_result SetMspSession(IN IEComUnknown* pMspSession) = 0;


    //==========================================================================
    //==
    //==  GetMspSession
    //==
    //==========================================================================
    //
    //  Summary:
    //      Provides access to the configured MSP session.
    //
    //  Parameters:
    //      ppMspSession:
    //       Pointer to a pointer to an MSP session. This pointer is set to the
    //       value of the configured MSP session. Can return NULL if no MSP
    //       session was configured. If it returns non-NULL, a reference will be
    //       counted, as per the ECOM rules.
    //
    //  Description:
    //      Provides access to the configured MSP session.
    //
    //  See Also:
    //      SetMspSession
    //
    //==========================================================================
    virtual void GetMspSession(OUT IEComUnknown** ppMspSession) = 0;


    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets an opaque paramerer to the call.
    //
    //  Parameters:
    //      opq:
    //       An opaque parameter.
    //
    //  Description:
    //      It sets an opaque parameter to the call. It can be used by the
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
    //      Gets the opaque parameter to the call.
    //
    //  Returns:
    //      The opaque value.
    //
    //  Description:
    //      It gets the opaque parameter to the call. It can be used by the
    //      application to store any information.
    //
    //==========================================================================
    virtual mxt_opaque GetOpaque() = 0;


    //==========================================================================
    //==
    //==  MakeCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Makes a call.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The peer address to contact. It is used as part of the "To" SIP
    //       header and for building the SIP request-URI.
    //
    //      pCallerPreferences:
    //       The caller preferences to add to the outgoing request. Ownership is
    //       is taken. May be NULL.
    //
    //  Returns:
    //      - resS_OK:
    //         The initial INVITE is sent.
    //      - resFE_FAIL:
    //         The initial INVITE could not be sent due to a configuration
    //         problem or any other failure.
    //      - resFE_INVALID_STATE:
    //         An initial INVITE cannot be sent at this time.
    //      - resFE_INVALID_ARGUMENT:
    //         rPeerAddr does not contain any URI.
    //
    //  Description:
    //      Makes a call and establishes a session with another peer.
    //
    //      The IUaSspUserConfiguration associated with this call must have a
    //      pre-loaded route if rPeerAddr does not specify a SIP address.
    //
    //      The call object may report IUaSspCallMgr::EvProgress zero or more
    //      times before reporting a final event. The final events are one of
    //      IUaSspCallMgr::EvAnswered or IUaSspCallMgr::EvFailed.
    //
    //      Note that to be able to make a call, it must have been properly
    //      configured. This means that SetManager, SetConfiguration and
    //      SetMspSession must have been called, otherwise this function will
    //      return resFE_INVALID_STATE.
    //
    //==========================================================================
    virtual mxt_result MakeCall(IN const CNameAddr& rPeerAddr,
                                IN TO CSceCallerPreferences* pCallerPreferences) = 0;


    //==========================================================================
    //==
    //==  TerminateCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the call.
    //
    //  Description:
    //      Terminates a call that is established or being established.
    //
    //      When this method is called, the call object reports the
    //      IUaSspCallMgr::EvTerminated event once the call has been terminated.
    //      Note that IUaSspCallMgr::EvTerminated gets reported only once
    //      per IUaSspCall object.
    //
    //      For a call that is currently fully established, a BYE request is
    //      sent to terminate it.
    //
    //      For a call that is currently being setup and this object is the
    //      initiator, a CANCEL request is sent and the call component makes
    //      sure that the call is properly terminated.
    //
    //      For a call that forked and that is still waiting for a final
    //      response on its original INVITE, an action will be taken only when
    //      the final response is received. Note that a final response will
    //      ALWAYS be received on a forked call as reception of no final
    //      response triggers a timeout, which generates a final response.
    //
    //      For incoming calls that have not yet been accepted, the user should
    //      use RejectCall instead of using TerminateCall. Calling TerminateCall
    //      under such condition sends a "487 Request Terminated" response to
    //      the INVITE.
    //
    //  Warning:
    //      This method sets the call object state to terminated and this change
    //      cannot be reverted. This means that the call object is no longer
    //      able to establish a call, the only thing left to do is to release
    //      the object, once IUaSspCallMgr::EvTerminated has been reported.
    //
    //  See Also:
    //      MakeCall, RejectCall, AcceptCall
    //
    //==========================================================================
    virtual void TerminateCall() = 0;

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
    //==========================================================================
    //==
    //==  TerminateImmediately
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminate this call component immediately and silently.
    //
    //==========================================================================
    virtual void TerminateImmediately() = 0;
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

    //==========================================================================
    //==
    //==  Abort
    //==
    //==========================================================================
    //
    //  Summary:
    //      Aborts the call.
    //
    //  Description:
    //      Aborts a call abruptly.
    //
    //  Warning:
    //      This method aborts the call immediately. If a call is in progress,
    //      no CANCEL or BYE request will be sent, it will simply be dropped.
    //
    //      Note that if there is a server request (such as an INVITE) in
    //      progress while Abort is called then it may not terminate immediately
    //      if there is a DNS fallback while sending the final SIP response.
    //      This can occur if the first attempt to send the final response
    //      fails.
    //
    //  See Also:
    //      MakeCall, RejectCall, AcceptCall, TerminateCall
    //
    //==========================================================================
    virtual void Abort() = 0;


    //==========================================================================
    //==
    //==  CallUserNotified
    //==
    //==========================================================================
    //
    //  Summary:
    //      The user has been notified about the incoming call.
    //
    //  Returns:
    //      - resS_OK:
    //         The 180 Ringing response is sent.
    //      - resFE_FAIL:
    //         The response could not be sent due to a configuration problem or
    //         any other failure.
    //      - resFE_INVALID_STATE:
    //         - The response cannot be sent because there is no initial
    //           incoming INVITE underway or a 180 already has been sent.
    //
    //         - There is a state error. A possible error would be that
    //           SetMspSession was not successfully called.
    //
    //  Description:
    //      Tells the peer user that the local user has been notified about the
    //      incoming call. This has the effect of sending a 180 Ringing
    //      response to the incoming INVITE. This method MUST NOT be called
    //      before SetMspSession has been called on this instance.
    //
    //      If IUaSspCallMgr::EvCalled was reported for an OPTIONS request (i.e.
    //      non-NULL pAdditionalParameters from which the IUaSspCallEventInfo
    //      interface can be queried and IsOptionsRequest returns true), this
    //      method will fail.
    //
    //  See Also:
    //      AcceptCall, RejectCall, TerminateCall
    //
    //==========================================================================
    virtual mxt_result CallUserNotified() = 0;


    //==========================================================================
    //==
    //==  AcceptCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Accepts the incoming call.
    //
    //  Returns:
    //      - resS_OK:
    //         The 200 OK response is sent.
    //      - resFE_FAIL:
    //         The response could not be sent due to a configuration problem or
    //         any other failure.
    //      - resFE_INVALID_STATE:
    //         The response cannot be sent because there is no initial incoming
    //         INVITE underway.
    //
    //         There is a state error. A possible error would be that
    //         SetMspSession was not successfully called.
    //
    //  Description:
    //      Accepts the incoming call. This has the effect of sending a 200 OK
    //      response to the incoming INVITE and starting the negotiated media
    //      stream with the peer.
    //
    //      If IUaSspCallMgr::EvCalled was reported with a replaced call (i.e.
    //      non-NULL pAdditionalParameters from which the IUaSspCallEventInfo
    //      interface can be queried and GetReplacedCall returns a non-NULL
    //      pointer), the replaced call will be terminated when this method
    //      succeeds.
    //
    //  See Also:
    //      RejectCall, TerminateCall
    //
    //==========================================================================
    virtual mxt_result AcceptCall() = 0;


    //==========================================================================
    //==
    //==  ForwardCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Forwards an incoming call to another destination.
    //
    //  Parameters:
    //      rRemoteUri:
    //       The address where the call must be forwarded.
    //
    //  Returns:
    //      - resS_OK:
    //         The call is forwarded.
    //      - resFE_INVALID_STATE:
    //         The call cannot be forwarded because there is no incoming initial
    //         INVITE underway.
    //      - resFE_INVALID_ARGUMENT:
    //         rRemoteUri does not contain any URI.
    //
    //  Description:
    //      Forwards an incoming call to the address specified in rRemoteUri.
    //
    //      At the SIP level, this sends back a "302 Moved Temporarily"
    //      response, with rRemoteUri as a Contact header.
    //
    //      When this method succeeds, the call object reports the
    //      IUaSspCallMgr::EvTerminated event once the call has been terminated.
    //
    //  See Also:
    //      RejectCall, AcceptCall
    //
    //==========================================================================
    virtual mxt_result ForwardCall(IN const CNameAddr& rRemoteUri) = 0;


    //==========================================================================
    //==
    //==  RejectCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Rejects an incoming call.
    //
    //  Parameters:
    //      uStatusCode:
    //       The status code to send. One of the following codes defined in
    //       SipParser/StatusCode.h must be used:
    //          - uTEMPORARILY_NOT_AVAILABLE:
    //            The user is temporarily unavailable on this system, but may be
    //            present on some other system.
    //          - uBUSY_HERE:
    //            The user is currently busy on this system, but may be reached
    //            through other means, like through an alternate system or a
    //            voicemail server.
    //          - uBUSY_EVERYWHERE:
    //            The user is currently busy on this system and has authoritative
    //            information that the user is not available on any other system.
    //          - uDECLINE:
    //            The user explicitly does not want to communicate at this time
    //            and does not want any other system to be tried for
    //            communication.
    //
    //      pszReasonPhrase:
    //       The reason phrase to send. If NULL, the default reason phrase
    //       defined in RFC 3261 is used.
    //
    //      uRetryAfter:
    //       The number of seconds the caller should wait before trying to
    //       contact the local user again. Zero means no specific value is
    //       indicated in the response.
    //
    //  Returns:
    //      - resS_OK:
    //         The call is rejected.
    //      - resFE_INVALID_STATE:
    //         The call cannot be rejected because there is no incoming initial
    //         INVITE underway.
    //      - resFE_INVALID_ARGUMENT:
    //         uStatusCode was not a failure code greater than or equal to 400.
    //         For 3xx, use ForwardCall.
    //
    //  Description:
    //      Rejects the incoming call. The uStatusCode parameter influences how
    //      the network behaves after receiving the response.
    //
    //      When rejecting a call with uTEMPORARILY_NOT_AVAILABLE or uBUSY_HERE,
    //      the server contacting this endpoint keeps on searching for other
    //      endpoints to contact. Depending on the network setup, the server may
    //      end up contacting a voice mail server that records a message on
    //      behalf of the called user.
    //
    //      When rejecting a call with uBUSY_EVERYWHERE or uDECLINE, the server
    //      contacting this endpoint immediately stops searching for other
    //      endpoints to contact. This effectively bypasses forwarding to a
    //      voice mail system. The caller is unable to establish a call.
    //
    //      When this method succeeds, the call object reports the
    //      IUaSspCallMgr::EvTerminated event once the call has been terminated.
    //
    //  See Also:
    //      AcceptCall
    //
    //==========================================================================
    virtual mxt_result RejectCall(IN uint16_t uStatusCode = uBUSY_HERE,
                                  IN const char* pszReasonPhrase = NULL,
                                  IN unsigned int uRetryAfter = 0) = 0;


    //==========================================================================
    //==
    //==  Transfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Transfers the current call to another specified target.
    //
    //  Parameters:
    //      rTransferTarget:
    //       The name-addr where the peer should make the call.
    //
    //  Returns:
    //      - resS_OK:
    //         The transfer request is being sent on the network.
    //      - resFE_FAIL:
    //         The call cannot be transferred due to a configuration problem or
    //         any other failure.
    //      - resFE_INVALID_STATE:
    //         The call cannot be transferred because there is no active call at
    //         the moment.
    //
    //  Description:
    //      Transfers the current call to another specified target.
    //
    //      This method initiates an unattended transfer.
    //
    //      The status of the transfer operation is reported to the user of the
    //      call through IUaSspCallMgr::EvTransferorTransferResult.
    //
    //      When calling this method, this object acts as the transferor in
    //      the transfer.
    //
    //==========================================================================
    virtual mxt_result Transfer(IN const CNameAddr& rTransferTarget) = 0;


    //==========================================================================
    //==
    //==  Transfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Transfers the current call to another previously contacted target.
    //
    //  Parameters:
    //      pCallWithTransferTarget:
    //       The call with the peer to which the current call should be
    //       transferred. The call on pCallWithTransferTarget must either:
    //          - Have been established.
    //          - Be an outgoing call currently being established. This means
    //            that it is a call for which the last reported
    //            IUaSspCallMgr::EvProgress was for a non-100 1xx value.
    //
    //       MUST NOT be NULL.
    //
    //  Returns:
    //      - resS_OK:
    //         The transfer request is being sent on the network.
    //      - resFE_INVALID_ARGUMENT:
    //         pCallWithTransferTarget is NULL.
    //      - resFE_FAIL:
    //         The call cannot be transferred due to a configuration problem or
    //         any other failure.
    //     -  resFE_INVALID_STATE:
    //         The call cannot be transferred because there is no active call at
    //         the moment.
    //
    //  Description:
    //      Transfers the current call to another previously contacted target.
    //
    //      The status of the transfer operation is reported to the user of the
    //      call through IUaSspCallMgr::EvTransferorTransferResult.
    //
    //      This method initiates an attended or semi-attended transfer
    //      (depending on the state of pCallWithTransferTarget).
    //
    //      If the transfer fails on first side (i.e. transferring this call to
    //      pCallWithTransferTarget) and pCallWithTransferTarget is an accepted
    //      call, the transfer can be tried the other way (i.e. calling
    //      IUaSspCall::Transfer on pCallWithTransferTarget with this call as
    //      parameter).
    //
    //      When calling this method, this user agent acts as the transferor
    //      in the transfer.
    //
    //==========================================================================
    virtual mxt_result Transfer(IN IUaSspCall* pCallWithTransferTarget) = 0;


    //==========================================================================
    //==
    //==  DeclineTransfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Rejects an incoming transfer request.
    //
    //  Returns:
    //      - resS_OK:
    //         The transfer is declined.
    //      - resFE_FAIL:
    //         The transfer cannot be declined due to a configuration problem or
    //         any other failure.
    //      - resFE_INVALID_STATE:
    //         The call cannot be declined because there is no pending transfer
    //         at the moment or the call is terminating.
    //
    //  Description:
    //      This method rejects an incoming transfer request.
    //
    //      When a remote party wants to transfer the local party to another
    //      destination, IUaSspCallMgr::EvTransfereeTransferRequested is
    //      reported. The user then has the choice to accept the transfer
    //      request by calling AcceptTransfer or to reject the request by
    //      calling DeclineTransfer.
    //
    //      Note that if there is already a client NOTIFY pending, this method
    //      will still succeed. In this case, the final notification to decline
    //      the transfer will be sent when the NOTIFY transaction ends.
    //
    //      When calling this method, this object must be acting as the
    //      transferee in the transfer.
    //
    //  See Also:
    //      AcceptTransfer
    //
    //==========================================================================
    virtual mxt_result DeclineTransfer() = 0;


    //==========================================================================
    //==
    //==  AcceptTransfer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows an incoming transfer request to proceed.
    //
    //  Parameters:
    //      pCreatedCall:
    //       The call on which this user agent is transferred. This call should
    //       be a new call on which no action (e.g.: MakeCall) must have been
    //       made. The IUaSspCall should be in the same state as before
    //       calling MakeCall.
    //
    //       MakeCall will automatically be called on pCreatedCall by this
    //       method.
    //
    //       MUST NOT be NULL.
    //
    //  Returns:
    //      - resS_OK:
    //         The transfer is accepted.
    //      - resFE_INVALID_ARGUMENT:
    //         pCreatedCall is NULL.
    //      - resFE_FAIL:
    //         The transfer cannot be accepted due to a configuration problem or
    //         any other failure.
    //      - resFE_INVALID_STATE:
    //         The call cannot be accepted because there is no pending transfer
    //         at the moment or the call is terminating.
    //
    //  Description:
    //      This method allows a transfer request to proceed.
    //
    //      When a remote party wants to transfer the local party to another
    //      destination, IUaSspCallMgr::EvTransfereeTransferRequested is
    //      reported. The user then has the choice to accept the transfer
    //      request by calling AcceptTransfer or reject the request by calling
    //      DeclineTransfer.
    //
    //      This method automatically establishes the call on pCreatedCall by
    //      calling MakeCall on it.
    //
    //      When this method succeeds,
    //      IUaSspCallMgr::EvTransfereeTransferResult will be called when the
    //      result of the transfer is known.
    //
    //      When calling this method, this object must be acting as the
    //      transferee in the transfer. This method must only be called on a
    //      IUaSspCall received through
    //      IUaSspCallMgr::EvTransfereeTransferRequested.
    //
    //  See Also:
    //      DeclineTransfer
    //
    //==========================================================================
    virtual mxt_result AcceptTransfer(IN IUaSspCall* pCreatedCall) = 0;


//M5T_INTERNAL_USE_BEGIN
    //-- << Internal methods >>
    //-------------------------------------------------

    //==========================================================================
    //==
    //==  HandleIncomingCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that informs the IUaSspCall to proceed with an
    //      incoming call.
    //
    //  Parameters:
    //      rRequest:
    //       The request to handle.
    //
    //  Returns:
    //      - resFE_FAIL:
    //         rRequest is not handled.
    //
    //      - resS_OK:
    //         rRequest is being handled.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method must be called after the call has been configured with
    //      its user configuration object (see SetConfiguration) and with its
    //      manager (see SetManager).
    //
    //      It informs the call that it can proceed with the incoming call
    //      request.
    //
    //      This method does not have to be called by the application. It is
    //      called automatically by the UA SSP after reporting the
    //      IUaSspNewCallMgr::EvNewCall.
    //
    //  See Also:
    //      SetConfiguration, SetManager
    //
    //==========================================================================
    virtual mxt_result HandleIncomingCall(IN const CSipPacket& rRequest) = 0;


    //==========================================================================
    //==
    //==  HandleForkedResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that informs the IUaSspCall to proceed with an
    //      incoming INVITE response.
    //
    //  Parameters:
    //      rResponse:
    //       The response to handle.
    //
    //      pOriginalUserAgentSvc:
    //       The ISipUserAgentSvc on the context of the originator IUaSspCall.
    //
    //       Must NOT be NULL.
    //
    //      pOriginalMsp:
    //       The IMspOfferAnswerSession of the originator IUaSspCall.
    //
    //       IMspOfferAnswerSession::Fork will be called on this parameter to
    //       generate the IMspOfferAnswerSession used by the IUaSspCall.
    //
    //       Must NOT be NULL.
    //
    //      bSessionOfferInInvite:
    //       true:
    //          The sent INVITE contained a session offer.
    //
    //       false:
    //          The sent INVITE did not contain a session offer.
    //
    //      bEarlySessionOfferInInvite:
    //       true:
    //          The sent INVITE contained an early-session offer.
    //
    //       false:
    //          The sent INVITE did not contain an early-session offer.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT:
    //         rResponse is not a response.
    //
    //         pOriginalUserAgentSvc is NULL.
    //
    //         pOriginalMsp is NULL.
    //      - resFE_INVALID_STATE:
    //         Cannot call HandleForkedResponse at this time. This instance
    //         might be incorrectly initialized.
    //
    //      - resS_OK:
    //         rResponse is being handled.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method must be called on the new IUaSspCall after
    //      ISipSessionMgr::EvNewSessionNeededForOriginalInviteResponse is
    //      reported on the originator IUaSspCall.
    //
    //      This method will generate an IMspOfferAnswerSession fork and handle
    //      the incoming response.
    //
    //==========================================================================
    virtual mxt_result HandleForkedResponse(IN const CSipPacket& rResponse,
                                            IN ISipUserAgentSvc* pOriginalUserAgentSvc,
                                            IN IMspOfferAnswerSession* pOriginalMsp,
                                            IN bool bSessionOfferInInvite,
                                            IN bool bEarlySessionOfferInInvite) = 0;


    //==========================================================================
    //==
    //==  GetUserAgentSvc
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that returns the ISipUserAgentSvc.
    //
    //  Parameters:
    //      rpUaSvc:
    //       The pointer to the ISipUserAgentSvc of the IUaSspCall.
    //
    //       NULL if there was a problem.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Returns the ISipUserAgentSvc on the ISipContext of this object.
    //
    //==========================================================================
    virtual void GetUserAgentSvc(OUT ISipUserAgentSvc*& rpUaSvc) = 0;


    //==========================================================================
    //==
    //==  ReportTransferProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that sends a NOTIFY to the transferor.
    //
    //  Parameters:
    //      rContentStatusLine:
    //       The Status-Line that is included in the NOTIFY payload.
    //
    //  Returns:
    //      - resS_OK:
    //         The NOTIFY was sent.
    //
    //         If rContentStatusLine contains a final Status-Code,
    //         IUaSspCallMgr::EvTransfereeTransferRequested will be called on
    //         the manager.
    //      - resFE_FAIL:
    //         The NOTIFY cannot be sent due to a configuration problem or any
    //         other failure.
    //
    //         If rContentStatusLine contains a final Status-Code,
    //         IUaSspCallMgr::EvTransfereeTransferRequested will be called on
    //         the manager.
    //      - resFE_INVALID_STATE:
    //         The NOTIFY cannot be sent because there is no pending transfer at
    //         the moment.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      This method notifies the transferor about the transfer progress.
    //
    //      This will send a NOTIFY request with the passed status line as
    //      payload. The NOTIFY will be final if rContentStatusLine has a final
    //      Status-Code.
    //
    //      When a remote party wants to transfer the local party to another
    //      destination, IUaSspCallMgr::EvTransfereeTransferRequested is
    //      reported. The user then has the choice to reject the request by
    //      calling DeclineTransfer or to process the transfer by calling
    //      AcceptTransfer. If the transfer is processed, the first call (i.e.
    //      with the transferor) should be notified of any progress on the new
    //      call (i.e. the call with the transfer target) using this method. This
    //      method should be called once for a final notification.
    //
    //      When rContentStatusLine contains a final Status-Code,
    //      EvTransfereeTransferResult is reported on the manager. The result
    //      given by EvTransfereeTransferResult depends on the response code in
    //      rContentStatusLine. A final success response will give a success and
    //      a final failure response will give a failure.
    //
    //      This method must only be called on the call with the transferor when
    //      we are acting as the transferee in a transfer scenario.
    //
    //==========================================================================
    virtual mxt_result ReportTransferProgress(IN const CSipStatusLine& rContentStatusLine) = 0;


    //==========================================================================
    //==
    //==  SetTransfereeNotifier
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that sets the call on which notifications will be
    //      sent to the transferor.
    //
    //  Parameters:
    //      pTransfereeNotifier:
    //       The call on which the NOTIFY requests for the progress reports of
    //       the call to the transfer target will be sent.
    //
    //       This is the call on which the REFER was received from the
    //       transferor.
    //
    //       Can be NULL.
    //
    //  Returns:
    //      - resS_OK:
    //         The IUaSspCall was set and ReportTransferProgress will be called
    //         on it when there is transfer progress.
    //      - resFE_INVALID_STATE:
    //         The IUaSspCall is in a call or is terminating.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Sets the call on which notifications about the transfer progress
    //      will be sent to the transferor.
    //
    //      This method must be called before the call is established on the
    //      IUaSspCall.
    //
    //      This method must only be called on the IUaSspCall for the call still
    //      to establish with the transfer target when we are acting as the
    //      transferee in a transfer scenario.
    //
    //==========================================================================
    virtual mxt_result SetTransfereeNotifier(IN IUaSspCall* pTransfereeNotifier) = 0;


    //==========================================================================
    //==
    //==  InternalTerminateCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that terminates the call.
    //
    //  Parameters:
    //      eTerminationReason:
    //       The reason why the call is being terminated.
    //
    //       This reason will be reported to the manager using
    //       IUaSspCallMgr::EvTerminated.
    //
    //  Returns:
    //      - resFE_INVALID_STATE:
    //         The object is already terminating.
    //      - resSW_FIRST:
    //         Nothing had to be done to shutdown.
    //      - resS_OK:
    //         Calling this method terminated something. For example, the BYE or
    //         CANCEL request is sent, or a server INVITE transaction was
    //         terminated.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Terminates the call internally.
    //
    //      This means that the application will get notified when the
    //      termination is over and that the bIsApplicationRequested parameter
    //      of IUaSspCallMgr::EvTerminated will be false.
    //
    //      IUaSspCallMgr::EvTerminated is ALWAYS called when this method is
    //      called.
    //
    //==========================================================================
    virtual mxt_result InternalTerminateCall(IN IUaSspCallMgr::ETerminationReason eTerminationReason) = 0;


    //==========================================================================
    //==
    //==  IsTerminating
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that tells if the IUaSspCall is terminating.
    //
    //  Returns:
    //      - true:
    //         The IUaSspCall is terminating.
    //      - false:
    //         The IUaSspCall is not terminating.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Tells if the IUaSspCall is terminating.
    //
    //==========================================================================
    virtual bool IsTerminating() = 0;


    //==========================================================================
    //==
    //==  IsInEarlyDialog
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that tells if the IUaSspCall is in an early dialog.
    //
    //  Returns:
    //      - true:
    //         The IUaSspCall is in an early dialog.
    //      - false:
    //         The IUaSspCall is in a call or is terminating.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Tells if the IUaSspCall is in an early dialog.
    //
    //==========================================================================
    virtual bool IsInEarlyDialog() = 0;


    //==========================================================================
    //==
    //==  InternalMakeCall
    //==
    //==========================================================================
    //
    //  Summary:
    //      Internal method that makes a call.
    //
    //  Parameters:
    //      rPeerAddr:
    //       The peer address to contact. It is used as part of the "To" SIP
    //       header and for building the SIP request-URI.
    //
    //      pCallerPreferences:
    //       The caller preferences to add to the outgoing request. Ownership is
    //       taken. May be NULL.
    //
    //      pExtraHeaders:
    //       List of extra headers to add to the INVITE request. For example,
    //       when a call is transferred, the Referred-By header must be copied
    //       from the REFER request to the INVITE request.
    //
    //       NULL for none.  Ownership is always taken.
    //
    //  Returns:
    //      - resS_OK:
    //         The initial INVITE is sent.
    //      - resFE_FAIL:
    //         The initial INVITE could not be sent due to a configuration
    //         problem or any other failure.
    //      - resFE_INVALID_STATE:
    //         An initial INVITE cannot be sent at this time.
    //      - resFE_INVALID_ARGUMENT:
    //         rPeerAddr does not contain any URI.
    //
    //  Description:
    //      IMPORTANT: This is an INTERNAL method that should only be used by
    //      the UA SSP implementation.
    //
    //      Makes a call and establishes a session with another peer.
    //
    //      The generated INVITE will contain the Resource-Priority header
    //      depending on the support level set using
    //      ISceUserResourcePriorityConfig::SetResourcePrioritySupportLevel.
    //
    //      The IUaSspUserConfiguration associated with this call must have a
    //      pre-loaded route if rPeerAddr does not specify a SIP address.
    //
    //      The call object may report IUaSspCallMgr::EvProgress zero or more
    //      times before reporting a final event. The final events are one of
    //      IUaSspCallMgr::EvAnswered or IUaSspCallMgr::EvFailed.
    //
    //      Note that to be able to make a call, it must have been properly
    //      configured. This means that SetManager, SetConfiguration and
    //      SetMspSession must have been called, otherwise this function will
    //      return resFE_INVALID_STATE.
    //
    //==========================================================================
    virtual mxt_result InternalMakeCall(IN const CNameAddr& rPeerAddr,
                                        IN TO CSceCallerPreferences* pCallerPreferences,
                                        IN TOA CHeaderList* pExtraHeaders = NULL) = 0;

//M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    IUaSspCall(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~IUaSspCall(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    IUaSspCall(IN const IUaSspCall& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    IUaSspCall& operator=(IN const IUaSspCall& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_IUASSPCALL_H
