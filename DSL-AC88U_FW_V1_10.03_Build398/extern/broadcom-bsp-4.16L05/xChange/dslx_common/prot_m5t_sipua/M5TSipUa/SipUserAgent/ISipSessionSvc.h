//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2001 Mediatrix Telecom, Inc. ("Mediatrix")
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPSESSIONSVC_H
#define MXG_ISIPSESSIONSVC_H

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
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class CSipMessageBody;
class CHeaderList;
class ISipClientTransaction;
class ISipSessionMgr;
class ISipContext;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipSessionSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Interface: ISipSessionSvc
//=============================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The session service is used to establish a SIP dialog with a remote user
//  agent by sending an INVITE. It is also used in the other direction to send
//  progression status when a remote user agent tries to establish a dialog with
//  the application by sending an INVITE.
//
//  This interface can also be used to send refreshes or modifications to the
//  established dialog (or send the response to the refresh or modification
//  initiated by the remote user agent).
//
// #Sending ACK Requests#
//
//  In RFC 3261, the ACK request is used to stop the retransmission of final
//  responses to an INVITE. An ACK sent to stop the transmission of a final
//  error response (>=300) does not contain a payload while the ACK sent to stop
//  the retransmissions of success responses (2xx) must contain a payload if the
//  initial INVITE did not specify any offer. Thus, when the SIP Stack receives
//  a final error response, it automatically sends an ACK to stop the
//  retransmissions. When receiving a success response, the SIP Stack lets the
//  application issue the ACK, since it may be sent with a content.
//
// #Terminating a Dialog#
//
//  To tear down an established session, the application must call the
//  <C>Bye</C> method.
//
//  This service generates events to the application through the ISipSessionMgr
//  interface.
//
// #The ISipSessionSvc is an ECOM object#
//
//  The ISipSessionSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipSessionSvc</B>
//
//  <B>Interface Id: IID_ISipSessionSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Updated Behaviour:
//
//  Some behaviours of the session service have changed from M5T SIP UA 3.x
//  versions.
//
//  - Releasing the context or setting the manager to NULL does not terminate
//  all pending transactions in the session service. Instead, the application
//  has to properly terminate all pending transactions before being done with
//  the context. The application needs to call the <C>Bye</C> or
//  <C>CancelRequest</C> method to properly terminate a session. When the
//  session is terminated by the remote party, it needs to send a success final
//  response because the SIP Stack does not automatically respond any final
//  response to a BYE request.
//
//  - The responses to incoming requests are not sent through the session
//  service. They are sent through a new method <C>SendResponse</C> in the
//  ISipServerEventControl interface.
//
//  - The <C>BYE</C> method does not send a CANCEL request. It can only be used
//  to send a BYE request. To send a CANCEL request, the <C>CancelRequest</C>
//  method on the interface ISipClientTransaction has to be used.
//
// Location:
//   SipUserAgent/ISipSessionSvc.h
//
// See Also:
//  ISipSessionMgr, ISipContext
//
//==============================================================================
class ISipSessionSvc : public IEComUnknown
{
// Friend Declarations
//-------------------------

// Published Interface
//-------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipSessionSvc);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the manager of the session.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It cannot be NULL.
    //
    //  Returns:
    //      - resFE_INVALID_ARGUMENT: The manager cannot be NULL.
    //
    //      - resS_OK: The manager was set properly.
    //
    //  Description:
    //      Configures the manager that will receive the events from this
    //      service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipSessionMgr* pMgr) = 0;

    //==SDOC====================================================================
    //==
    //==  Invite
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an INVITE request to the target server.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      The SIP headers to add in the packet. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      The packet's body, aka payload. Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The manager of this session service is NULL,
    //      there is an INVITE request underway, the User-Agent service is not
    //      properly configured (e.g.: contact list is empty) or it is impossible
    //      to create the request by using the User-Agent service. The manager of
    //      the service can be NULL after Clear is called on the context, or when
    //      it was not set.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: Other failure such as creating a transaction occurred.
    //
    //      - resS_OK: The INVITE request is going to be sent.
    //
    //  Description:
    //      This method is used to establish or refresh a dialog with a remote
    //      user agent by sending an INVITE. Response to this INVITE is reported
    //      through the ISipSessionMgr interface. Usually the content contains
    //      a description of the session that the application is trying to
    //      establish (an SDP packet).
    //
    //      The reference to ISipClientTransaction allows users to further
    //      control the transaction. Note that it is possible to cancel an
    //      INVITE request by using ISipClientTransaction::CancelRequest.
    //
    //  See Also:
    //      ISipSessionMgr, CHeaderList, CSipMessageBody, ISipClientTransaction
    //
    //==EDOC====================================================================
    virtual mxt_result Invite(IN mxt_opaque                 opqTransaction,
                              IN TOA CHeaderList*           pExtraHeaders,
                              IN TOA CSipMessageBody*       pMessageBody,
                              OUT ISipClientTransaction*&   rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  Ack
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends an ACK request to the target server.
    //
    //  Parameters:
    //    pExtraHeaders:
    //      The SIP headers to add in the packet. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      The packet's body, aka payload. Ownership is TAKEN.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The manager of this session service is NULL,
    //      the session is not in a state where it is possible to send an ACK
    //      request, or it is impossible to create the request by using the
    //      User-Agent service. The manager of the service can be NULL after
    //      Clear is called on the context or when it was not set.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: Other failure that may have occurred.
    //
    //      - resS_OK: The ACK request is going to be sent.
    //
    //  Description:
    //      This method is used to acknowledge success final response (2xx
    //      response). If no success final response was received, the method
    //      returns an error. The payload may contain a session description.
    //
    //  See Also:
    //      ISipSessionMgr, CHeaderList, CSipMessageBody, ISipClientTransaction
    //
    //==EDOC====================================================================
    virtual mxt_result Ack(IN TOA CHeaderList*          pExtraHeaders,
                           IN TOA CSipMessageBody*      pMessageBody) = 0;

    //==SDOC====================================================================
    //==
    //==  Bye
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a BYE request to terminate this dialog.
    //
    //  Parameters:
    //    opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      The SIP headers to add in the packet. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      The packet's body, aka payload. Ownership is TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The manager of this session service is NULL,
    //      the session is not in a state where it is possible to send a BYE
    //      request or it is impossible to create the request using the
    //      User-Agent service. The manager of the service can be NULL after
    //      Clear is called on the context, or when it was not set.
    //
    //      - resFE_SIPCORE_PACKET_BLOCKED: One of the service synchronously
    //      blocked the packet. No additional event will be reported.
    //
    //      - resFE_FAIL: Other failure such as creating a transaction occured.
    //
    //      - resS_OK: The BYE request is going to be sent.
    //
    //  Description:
    //      Use this method to terminate an established dialog by using a BYE
    //      request. It can also be used to terminate an early dialog
    //      established with an INVITE that was forked. Terminating a dialog can
    //      also be done with a CANCEL request by using
    //      ISipClientTransaction::CancelRequest.
    //
    //      This method returns an error if the application tries to send a
    //      BYE while it is not in the proper state.
    //
    //      Upon receiving the event <C>EvTerminationSuccess</C>, it indicates
    //      that the session has been successfully terminated and that the
    //      session service does not need to be kept. However, the application
    //      should verify that no other dialog is active on this context through
    //      the ISipUserAgentSvc interface before releasing all its references
    //      to the context. For instance, it could be the case that the
    //      subscription to a transfer call result is not terminated yet.
    //
    //      Even if the API of ISipClientTransaction allows to cancel a pending
    //      BYE request, it should not be used to do so. RFC 3261 does not
    //      recommend to cancel a BYE request.
    //
    //  See Also:
    //      ISipSessionMgr, CHeaderList, CSipMessageBody, ISipClientTransaction,
    //      ISipUserAgentSvc.
    //
    //==EDOC====================================================================
    virtual mxt_result Bye(IN mxt_opaque                opqTransaction,
                           IN TOA CHeaderList*          pExtraHeaders,
                           IN TOA CSipMessageBody*      pMessageBody,
                           OUT ISipClientTransaction*&  rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  HandleOriginalInviteResponseNewSession
    //==
    //==========================================================================
    //
    //  Summary:
    //      Handles the newly created context for original INVITE response.
    //
    //  Parameters:
    //   opqTransaction:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //   pNewSession:
    //      The newly created session.
    //
    //      MUST NOT be NULL.
    //
    //   rPacket:
    //      The INVITE response packet.
    //
    //  Returns:
    //      - resS_OK: The initial INVITE response has been reported properly to
    //      the application.
    //
    //      - Other return codes: Failures.
    //
    //  Description:
    //      Notifies the session service that the application has created a new
    //      context to handle the given original INVITE response.
    //
    //      The newly created context MUST at least contain the User-Agent and
    //      session services. If one of those services is missing, this method
    //      fails.
    //
    //==EDOC====================================================================
    virtual mxt_result HandleOriginalInviteResponseNewSession(
                            IN mxt_opaque               opqTransaction,
                            IN ISipSessionSvc*          pNewSession,
                            IN const CSipPacket&        rPacket) = 0;


// Hidden Methods
//-------------------------
protected:
    // << Constructors / Destructors >>
    //--------------------------------------------
    ISipSessionSvc() {}

    virtual ~ISipSessionSvc() {}

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    ISipSessionSvc( IN const ISipSessionSvc& rSrc );
    ISipSessionSvc& operator=( IN const ISipSessionSvc& rSrc );
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_ISIPSESSIONSVC_H

