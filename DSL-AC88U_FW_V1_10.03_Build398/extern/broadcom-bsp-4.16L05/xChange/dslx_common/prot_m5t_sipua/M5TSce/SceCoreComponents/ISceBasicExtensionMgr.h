//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISCEBASICEXTENSIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEBASICEXTENSIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSipPacket;
class IEComUnknown;
class ISceBasicExtensionControl;
class ISceUserConfig;
class ISipContext;
class ISipServerEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceBasicExtensionMgr
//==============================================================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the event reported by the basic extension control
//   when an unhandled request is received on a component.
//
// Description:
//   This interface defines the event reported by the basic extension control
//   when an unhandled request is received on a component. It also defines the
//   event reported when a response is received for a request sent at the
//   application's demand (through the ISceBasicExtensionControl
//   interface).
//
// Location:
//   SceCoreComponents/ISceBasicExtensionMgr.h
//
//==============================================================================
class ISceBasicExtensionMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvUnhandledRequest
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a request unhandled by the component was
    //      received.
    //
    //  Parameters:
    //    pComponent:
    //      The component that received the request. This is never NULL.
    //
    //    pUserConfig:
    //      The user configuration of the user that received the request. It may
    //      be NULL if none of the registered users accepted to manage the
    //      request.
    //
    //    pServerEventCtrl:
    //      The server event control on which to send the response. This is
    //      never NULL.
    //
    //    rRequest:
    //      The unhandled request.
    //
    //    pAdditionalParameters:
    //      Additional parameters in relation to the unhandled request. Can be
    //      NULL if no additional parameters are present.
    //
    //      This returned parameter is an ECOM object that can be queried for
    //      all the interfaces that need to be watched or processed by the
    //      application from the Base Component or from specific component
    //      interfaces.
    //
    //      If it is NULL, it means no additional parameters could be returned
    //      to the application, either because there was a processing problem
    //      or that no server event yielded information worth returning.
    //
    //  Description:
    //      This method is called by a component when it receives a request that
    //      it does not know how to handle and an unhandled request manager is
    //      configured. If no unhandled request manager is configured, the
    //      component automatically rejects the request with a response
    //      computed according to the user to which the request is destined, its
    //      capabilities and the request itself. If this computation does not
    //      suit the application, it can override that behaviour by implementing
    //      this method.
    //
    //      The implementation should first determine whether or not the request
    //      is really one for which the default behaviour is unacceptable. If it
    //      is not the case, it can directly call RejectRequest on the pComponent
    //      in parameter, passing the other received parameters. If the default
    //      behaviour is unacceptable, the application must create the response
    //      and send it with the pServerEventCtrl in parameter.
    //
    //      The response can be any response that the application sees fit.
    //      Sending provisional responses through this mean is not recommended
    //      though. However, the application decide to send provisional
    //      responses, it must take care of sending a final response later or to
    //      call RejectRequest to have the component send the final response.
    //      There is however an important restriction on responses that can be
    //      sent through this mean: They must never create a dialog. At the
    //      moment of writing this documentation, this includes provisional and
    //      success responses to INVITE requests, success responses to SUBSCRIBE
    //      requests and success responses to NOTIFY requests. Dialog management
    //      must be done through a component. Hence, this method should only be
    //      used to reject requests or to send success responses to requests
    //      other than INVITE, SUBSCRIBE or NOTIFY.
    //
    //      All the parameters of this method are reference counted objects.
    //      They are guaranteed to be valid until EvUnhandledRequest returns. If
    //      the manager wants to process this event asynchronously, it must add
    //      reference for every object it will need later and release them when
    //      it is done processing the request.
    //
    //  See Also:
    //      ISceBasicExtensionControl
    //
    //==========================================================================
    virtual void EvUnhandledRequest(IN ISceBasicExtensionControl* pComponent,
                                    IN ISceUserConfig* pUserConfig,
                                    IN ISipServerEventControl* pServerEventCtrl,
                                    IN const CSipPacket& rRequest,
                                    IN IEComUnknown* pAdditionalParameters) = 0;

    //==========================================================================
    //==
    //==  EvResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a response for a request it sent was
    //      received.
    //
    //  Parameters:
    //    pComponent:
    //      The component on which the request was sent.
    //
    //    pUserConfig:
    //      The user configuration used to send the request. Cannot be NULL.
    //
    //    opqTransaction:
    //      The opaque identifier that the application passed to SendRequest.
    //
    //    rResponse:
    //      The received response.
    //
    //  Description:
    //      This method is called by a component when a response for a request
    //      sent through ISceBasicExtensionControl::SendRequest is
    //      received.
    //
    //      If the response in parameter is provisional (< 200), the manager
    //      must be prepared to receive this event again for the same request
    //      (that is with the same opqTransaction parameter).
    //
    //      If the response in parameter is final (>= 200), the manager will not
    //      receive this event again for this opqTransaction.
    //
    //  See Also:
    //      ISceBasicExtensionControl
    //
    //==========================================================================
    virtual void EvResponse(IN ISceBasicExtensionControl* pComponent,
                            IN ISceUserConfig* pUserConfig,
                            IN mxt_opaque opqTransaction,
                            IN const CSipPacket& rResponse) = 0;

    //==========================================================================
    //==
    //==  EvChallenged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Event reported when the component is challenged for an
    //      authentication mechanism it cannot handle.
    //
    //  Parameters:
    //    pComponent:
    //      The component reporting the event.
    //
    //    rChallenge:
    //      The SIP packet, which is either a 401 or 407 response that should
    //      contain all necessary challenge information.
    //
    //    opqChallenge:
    //      An opaque value that the application must provide when it calls
    //      ISceBasicExtensionControl::AcceptChallenge or
    //      ISceBasicExtensionControl::RejectChallenge.
    //
    //  Description:
    //      This event is reported only when the application instructs the basic
    //      extension control to report it (see
    //      ISceBasicExtensionControl::EnableAuthenticationCallback).
    //
    //      Moreover, this event is reported only when receiving a 401/407
    //      challenge for an authentication mechanism not supported by the
    //      component. Note that the call to EnableAuthenticationCallback may be
    //      used to disable some authentication mechanisms supported by the SIP
    //      Client Engine in order to receive this event even for these types of
    //      challenges.
    //
    //      If the implementation accepts to handle the challenge and calls
    //      ISceBasicExtensionControl::AcceptChallenge, then it must monitor
    //      outgoing SIP requests from this component, using
    //      ISceSipPacketObserver::EvSendingRequest, and add the necessary
    //      credentials to each outgoing request.
    //
    // Warning:
    //      It is important that the implementation of this event provides the
    //      opaque value, provided in this event, to the
    //      ISceBasicExtensionControl::AcceptChallenge or
    //      ISceBasicExtensionControl::RejectChallenge method to ensure proper
    //      operation of the extension.
    //
    //  See Also:
    //      ISceBasicExtensionControl::EnableAuthenticationCallback,
    //      ISceBasicExtensionControl::AcceptChallenge,
    //      ISceBasicExtensionControl::RejectChallenge
    //
    //==========================================================================
    virtual void EvChallenged(IN ISceBasicExtensionControl* pComponent,
                              IN const CSipPacket& rChallenge,
                              IN mxt_opaque opqChallenge) = 0;

    //==========================================================================
    //==
    //==  EvSipContextCreated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Event reported when the component creates its main ISipContext.
    //
    //  Parameters:
    //    pComponent:
    //      The component reporting the event.
    //
    //    pCreatedContext:
    //      The ISipContext that just have been created and configured.
    //
    //  Description:
    //      This event is reported only when the component successfully created
    //      and configured its ISipContext.
    //
    //      Note that this event can be called often for the same ISipContext
    //      when the associated component is handling out-of-dialog requests.
    //      Each call means that the configuration may have changed.
    //
    // Warning:
    //      Accessing and using the ISipContext of a component is very sensitive
    //      and care must be taken in order to avoid creating synchronization
    //      problems between the application and the component. Please contact
    //      M5T and explain what you want to achieve with the ISipContext before
    //      using pCreatedContext.
    //
    //  See Also:
    //      ISceBasicExtensionControl::GetSipContext
    //
    //==========================================================================
    virtual void EvSipContextCreated(IN ISceBasicExtensionControl* pComponent,
                                     IN ISipContext* pCreatedContext) = 0;


//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceBasicExtensionMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ISceBasicExtensionMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceBasicExtensionMgr(IN const ISceBasicExtensionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceBasicExtensionMgr& operator=(IN const ISceBasicExtensionMgr& rSrc);


//-- Hidden Data Members
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEBASICEXTENSIONMGR_H

