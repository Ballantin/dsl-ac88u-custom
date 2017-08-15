//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPSESSIONMGR_H
#define MXG_ISIPSESSIONMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT to be able \
to use this class.
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


// Forward Declarations.
//-----------------------
class CSipPacket;
class ISipServerEventControl;
class ISipClientEventControl;
class ISipSessionSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipSessionMgr
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The session manager is the interface through which the session service
//  reports events to the application. The SIP Stack informs the application
//  through this interface of the progress for INVITE requests, that an
//  INVITE request has been received, that an ACK for a successful final
//  response (2xx) to an INVITE has been received, and that the session has been
//  terminated by the remote user.
//
//  It is also through this interface that the SIP Stack asks the application to
//  create a new context when an outgoing INVITE response requires such action.
//
// Updated Behaviour:
//
//  Some behaviours of the session service manager have changed from M5T SIP UA
//  3.x versions.
//
//  - EvTerminated is not called for the reception of a CANCEL request. It is
//  only called for the reception of a BYE request. The reception of a CANCEL
//  request that triggers the termination of a session is signaled through the
//  new event EvInviteCancelled.
//
//  - EvTimeout no longer exists. Failure to receive a final response to
//  an outgoing INVITE and BYE request are reported through the event EvFailure
//  and EvTerminationFailure respectively. Failure to receive an ACK request
//  following the sending of a final positive response is reported through a new
//  event called EvInviteSuccessResponseTimeout.
//
// Location:
//   SipUserAgent/ISipSessionMgr.h
//
// See Also:
//  ISipSessionSvc
//
//==============================================================================
class ISipSessionMgr
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvInvited
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an INVITE request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a remote user is trying to establish a
    //      dialog with the local user by sending an INVITE request. The
    //      application should send responses through the
    //      <C>ISipServerEventControl::SendResponse</C> method. If the
    //      application has not sent any response within 200 ms, the SIP Stack
    //      sends a 100 "Trying" response in order to stop retransmission.
    //      If the application releases the context without sending any final
    //      response, a 487 "Request Terminated" is sent. If the application
    //      sends a successful final response (2xx), the SIP Stack issues the
    //      <C>EvAcknowledged</C> event upon receiving the ACK request from the
    //      remote server.
    //
    //  See Also:
    //      ISipSessionSvc, ISipServerEventControl, EvAcknowledged
    //
    //==EDOC====================================================================
    virtual void EvInvited(IN ISipSessionSvc* pSvc,
                           IN ISipServerEventControl* pServerEventCtrl,
                           IN const CSipPacket& rRequest) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReInvited
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a re-INVITE request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Informs the application that the already connected remote user is
    //      sending an INVITE request in order to change the agreed session
    //      description or simply refresh the current session (as part of a keep
    //      alive mechanism). The application should send responses through the
    //      <C>ISipServerEventControl::SendResponse</C> method. If the
    //      application has not sent any response within 200 ms, the SIP Stack
    //      sends a 100 "Trying" response in order to stop retransmission.
    //
    //      If the application sends a successful final response (2xx), the SIP
    //      Stack issues the <C>EvAcknowledged</C> event upon receiving the ACK
    //      request from the remote server.
    //
    //  See Also:
    //      ISipSessionSvc, ISipServerEventControl, EvAcknowledged
    //
    //==EDOC====================================================================
    virtual void EvReInvited(IN ISipSessionSvc* pSvc,
                             IN ISipServerEventControl* pServerEventCtrl,
                             IN const CSipPacket& rRequest) = 0;

    //==SDOC====================================================================
    //==
    //==  EvAcknowledged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an ACK request has been received for
    //      its successful final response (2xx).
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request
    //      or the corresponding INVITE request.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Informs the application that the ACK for a successful final response
    //      (2xx) sent on this session service has been received. ACK for
    //      failure final response are automatically managed by the SIP Stack
    //      and are NOT notified through this method.
    //
    //      Only the ACK requests to 2xx responses are reported to the
    //      application because they can contain a content in the payload or
    //      they are end-to-end instead of hop-by-hop.
    //
    //  See Also:
    //      ISipSessionSvc, EvInvited, EvReInvited
    //
    //==EDOC====================================================================
    virtual void EvAcknowledged(IN ISipSessionSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rRequest) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the remote party wants to terminate
    //      the session by using a BYE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Informs the application that it should no longer process the session
    //      associated with this context. This method is triggered by the
    //      reception of a BYE request.
    //
    //      The application has to respond a final response to any pending BYE
    //      request. It MUST also respond a 487 "Request Terminated" to any
    //      pending server INVITE transaction. A server transaction is
    //      considered to be pending if no final response (>= 2xx) has been sent
    //      yet.
    //
    //  See Also:
    //      ISipSessionSvc.
    //
    //==EDOC====================================================================
    virtual void EvTerminated(IN ISipSessionSvc* pSvc,
                              IN ISipServerEventControl* pServerEventCtrl,
                              IN const CSipPacket& rRequest) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInviteCancelled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the last INVITE request it received
    //      has been cancelled.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The CANCEL SIP request.
    //
    //  Description:
    //      Notifies the application that the last INVITE request it received
    //      has been cancelled. Before issuing this method, the SIP Stack
    //      automatically sends a 200 "OK" in response to the CANCEL request.
    //
    //      The application MUST send a 487 "Request Cancelled" to the INVITE
    //      request by using the pServerEventCtrl given in parameter of this
    //      method.
    //
    //  See Also:
    //      ISessionSvcMgr, EvInvited
    //
    //==EDOC====================================================================
    virtual void EvInviteCancelled(IN ISipSessionSvc* pSvc,
                                   IN ISipServerEventControl* pServerEventCtrl,
                                   IN const CSipPacket& rRequest ) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReInviteCancelled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that the last reINVITE request it received
    //      has been cancelled.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    pServerEventCtrl:
    //      Interface to the server event control for this server transaction.
    //
    //    rRequest:
    //      The CANCEL SIP request.
    //
    //  Description:
    //      Notifies the application that the last reINVITE request it received
    //      has been cancelled. Before issuing this method, the SIP Stack
    //      automatically sends a 200 "OK" in response to the CANCEL request.
    //
    //      The application MUST send a 487 "Request Cancelled" to the INVITE
    //      request by using the pServerEventCtrl given in parameter of this
    //      method.
    //
    //      The application should stop every processing associated with this
    //      reINVITE and fall back to the previously agreed media description
    //      (SDP).
    //
    //  See Also:
    //      ISessionSvcMgr, EvReInvited
    //
    //==EDOC====================================================================
    virtual void EvReInviteCancelled(
                                    IN ISipSessionSvc* pSvc,
                                    IN ISipServerEventControl* pServerEventCtrl,
                                    IN const CSipPacket& rRequest ) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidInvite
    //==
    //==========================================================================
    //
    //  Summary:
    //      Warns the application that an INVITE or BYE request has been
    //      declined with a final negative response.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing this request.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rRequest:
    //      The request automatically declined by the SIP Stack.
    //
    //    resReason:
    //      - resFE_INVALID_CONTACT_HEADER if the received INVITE request did
    //        not contain a Contact header, contained an invalid Contact header,
    //        or contained more than one Contact headers. In these cases, an
    //        automatic 400 "Missing, Erroneous or Multiple Contact header
    //        field(s)" response has been sent.
    //
    //      - resFE_NO_ISIPUSERAGENTSVC_ATTACHED:
    //          - An INVITE or BYE request has been received while there was no
    //            ISipUserAgentSvc attached to the context. In this case, an
    //            automatic 500 response has been sent.
    //          - An INVITE or BYE request has been received while there was no
    //            manager set on this object. In this case, an automatic 500
    //            response has been sent.
    //
    //      - resFE_CALL_LEG_TRANSACTION_DOES_NOT_EXIST: If a BYE request has
    //        been received while there was no dialog established or an INVITE
    //        request has been received after the dialog has been terminated. In
    //        this case, an automatic 481 response has been sent.
    //
    //      - resFE_REQUEST_PENDING:
    //          - An INVITE or BYE request has been received while the same
    //            request is proceeding as a UAC. In this case, an automatic 491
    //            response has been sent.
    //          - An INVITE or BYE request has been received while the same
    //            request is proceeding as a UAS. In this case, an automatic 500
    //            response has been sent.
    //          - An INVITE has been received while a BYE is processing (either
    //            as a UAC or UAS). In this case, an automatic 500 response has
    //            been sent.
    //
    //  Description:
    //      This method is called when an INVITE or BYE request is received
    //      while the service cannot accept it.
    //
    //      Please note that if the manager of the service is not set, all the
    //      requests are responded with a 500 response.
    //
    //      Warning: The name of this method can be misleading. Indeed this
    //      event reports both invalid INVITE and invalid BYE requests.
    //
    //==EDOC====================================================================
    virtual void EvInvalidInvite(IN ISipSessionSvc* pSvc,
                                 IN mxt_opaque opqApplicationData,
                                 IN const CSipPacket& rRequest,
                                 IN mxt_result resReason) = 0;

    //==SDOC====================================================================
    //==
    //==  EvProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 1xx response has been received for
    //      the last outgoing INVITE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a provisional response has been
    //      received. It could be any response with a 1xx code. 100 Trying and
    //      180 Ringing responses are reported through this event.
    //
    //      This event can be called on a new context created because of a call
    //      to the event <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //      The INVITE request associated with this event has been sent on this
    //      context or on the context that reported the event
    //      <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //  See Also:
    //      ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipSessionSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 2xx response has been received for
    //      the last outgoing INVITE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a 2xx response has been received.
    //
    //      This event can be called on a new context created because of a call
    //      to the event <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //      The INVITE request associated with this event has been sent on this
    //      context or on the context that reported the event
    //      <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //      The application must send the ACK through the session service
    //      <C>Ack</C> method.
    //
    //  See Also:
    //      ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a final negative response has been
    //      received for the last outgoing INVITE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a final negative response has been
    //      received. Before reporting the event, the SIP Stack automatically
    //      sends the ACK request.
    //
    //      This event can be called on a new context created because of a call
    //      to the event <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //      The INVITE request associated with this event has been sent on this
    //      context or on the context that reported the event
    //      <C>EvNewSessionNeededForOriginalInviteResponse</C>.
    //
    //  See Also:
    //      ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipSessionSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminationProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a provisionnal response has been
    //      received for the last outgoing BYE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a provisionnal response has been
    //      received for the last BYE sent.
    //
    //      Note that although it is recommended to not send provisional
    //      responses (1xx) to requests other than INVITE, it is possible that
    //      this event may be called. The application should not release the
    //      context until reception of a final response.
    //
    //  See Also:
    //      ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual void EvTerminationProgress(
                                    IN ISipSessionSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminationSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a 2xx response has been received for
    //      the last outgoing BYE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Notifies the application that a 2xx response has been received for
    //      the last BYE sent.
    //
    //      This event indicates that the session has been successfully
    //      terminated and that the session service does not need to be kept.
    //      However, the application should verify that no other dialog is
    //      active on this context through the ISipUserAgentSvc interface before
    //      releasing all its references to the context. For instance, it could
    //      be the case that the subscription to a transfer call result is not
    //      terminated yet.
    //
    //  See Also:
    //      ISipSessionSvc, ISipUserAgentSvc
    //
    //==EDOC====================================================================
    virtual
        void EvTerminationSuccess(IN ISipSessionSvc* pSvc,
                                  IN ISipClientEventControl* pClientEventCtrl,
                                  IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTerminationFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a final negative response has been
    //      received for the last outgoing BYE request.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the outgoing request.
    //
    //    pClientEventCtrl:
    //      Interface to the client event control for this client transaction.
    //
    //    rRequest:
    //      The SIP request.
    //
    //  Description:
    //      Informs the application that a >= 3xx response has been received for
    //      the last BYE sent.
    //
    //      The application should try to fix the situation and reissue the BYE
    //      request. If it cannot fix the situation, it should consider the
    //      session terminated as if EvTerminationSuccess was received.
    //
    //  See Also:
    //      ISipSessionSvc
    //
    //==EDOC====================================================================
    virtual
        void EvTerminationFailure(IN ISipSessionSvc* pSvc,
                                  IN ISipClientEventControl* pClientEventCtrl,
                                  IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvNewSessionNeededForOriginalInviteResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a new context is needed to handle the
    //      given original INVITE response.
    //
    //  Parameters:
    //      pSvc:
    //       The session service.
    //
    //      pClientEventCtrl:
    //       The client event control.
    //
    //       Do not use. This parameter is always NULL.
    //
    //      rPacket:
    //       The response that requires a new context.
    //
    //  Description:
    //      Informs the application that a new context is needed to handle the
    //      given original INVITE response. This event is reported to the
    //      application when a response to an initial INVITE has a To tag
    //      different from the To tag of every other response received before
    //      for that INVITE.
    //
    //      This can be the case when an outgoing INVITE request is forked by a
    //      Proxy. The first response received for the original INVITE request
    //      is associated with the session created to send the INVITE. All
    //      other INVITE responses that have a different To tag need a new
    //      session to be handled properly.
    //
    //      The manager that implements this method should create a new session
    //      , attach services to it, configure it the same way the the original
    //      session was, and give it back to the session service that issued the
    //      event. The application SHOULD attach all the same services that are
    //      attached to the original INVITE context.
    //
    //  See Also:
    //      ISipSessionSvc::HandleOriginalInviteResponseNewSession,
    //      ISipContext::AttachService
    //
    //==EDOC====================================================================
    virtual void EvNewSessionNeededForOriginalInviteResponse(
                        IN ISipSessionSvc* pSvc,
                        IN ISipClientEventControl* pClientEventCtrl,
                        IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInviteSuccessResponseTimeout
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an INVITE server transaction timed out.
    //
    //  Parameters:
    //    pSvc:
    //      The ISipSessionSvc managing the incoming request.
    //
    //    opq:
    //      The opaque parameter associated with this server transaction.
    //
    //    rResponse:
    //      The SIP 2xx response.
    //
    //  Description:
    //      Informs the application that a timeout occurred when sending a final
    //      2xx response to an INVITE and failing to receive an ACK. The
    //      application can then call the <C>Bye</C> method on the
    //      session service.
    //
    //  See Also:
    //      ISipSessionSvc::Bye
    //
    //==EDOC====================================================================
    virtual void EvInviteSuccessResponseTimeout(
                        IN ISipSessionSvc* pSvc,
                        IN mxt_opaque opq,
                        IN const CSipPacket& rResponse) = 0;


// Hidden Methods
//-------------------------
protected:
    // << Constructors / Destructors >>
    //----------------------------------
    ISipSessionMgr() {}

    virtual ~ISipSessionMgr() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipSessionMgr( const ISipSessionMgr& rFrom );
    ISipSessionMgr& operator=( const ISipSessionMgr& rFrom );

// Hidden Data Members
//-------------------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPSESSIONMGR_H

