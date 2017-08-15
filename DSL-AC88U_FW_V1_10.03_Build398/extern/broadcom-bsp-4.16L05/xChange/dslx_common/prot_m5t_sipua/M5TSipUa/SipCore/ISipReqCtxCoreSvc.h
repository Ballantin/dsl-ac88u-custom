//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPREQCTXCORESVC_H
#define MXG_ISIPREQCTXCORESVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
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
class CSipCoreEventList;
class CSipMessageBody;
class ISipRequestContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReqCtxCoreSvc
//========================================
//
// Description:
//   This interface is used by the ISipRequestContext to inform the interested
//   services of packets sent and received. These services help build outgoing
//   packets and warn the application of events upon reception of incoming
//   packets. They can also update their internal state when seeing packets 
//   either sent or received.
//
//   This interface is obtained through the ISipCoreSvc interface. It can be
//   implemented either by the same object as the ISipCoreSvc or by a different
//   object. Usually, the implementation should use a different class if there 
//   is a transactional state associated with the service.
//
// Location:
//   SipCore/ISipReqCtxCoreSvc.h
//
// See Also:
//   ISipRequestContext, ISipCoreSvc
//
//==EDOC========================================================================
class ISipReqCtxCoreSvc
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
    //==  HandlePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Manages a packet not associated with any ISipRequestContext yet.
    //
    //  Parameters:
    //    rPacket:
    //      The received packet creating a request context.
    //
    //  Returns:
    //      resS_OK: The packet has been handled.
    //
    //      resFE_DUPLICATE: A duplicated transaction has been found. Resources
    //      created for this packet should be released and the packet should be
    //      considered as a retransmission.
    //
    //      resFE_FAIL: Some other error occurred. It could be that the
    //      implementor is not supposed to be the owner for any packet, that the
    //      request context to handle the packet could not be created, or that
    //      the transaction manager is overloaded and could not create the
    //      server transaction. Resources created for this packet should be
    //      released.
    //
    //  Description:
    //      This method is called on the request context service obtained
    //      through the GetOwnerInstance method of the ISipCoreSvc interface
    //      when a received packet needs a new request context to handle it.
    //
    //      The implementation should create a ISipRequestContext, obtain the
    //      requests context services interested in following this request
    //      context (both core services and connection services), and give the
    //      packet to the request context for processing.
    //
    //      When this method fails, this object MUST be released as it cannot
    //      handle another packet.
    //
    //  See Also:
    //      ISipRequestContext, ISipCoreSvc
    //
    //==EDOC====================================================================
    virtual mxt_result HandlePacket(IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  OnPacketReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the request context core service of a received packet.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet received.
    //
    //    rEventList:
    //      The container in which the services put the event they need to
    //      report to the application.
    //
    //  Description:
    //      This method is called by the ISipRequestContext each time a packet
    //      is received on it. The implementer then analyzes the packet and
    //      possibly updates its internal state. If the service has an event to
    //      report to the application regarding this packet, it adds it to the
    //      event list in parameter. When OnPacketReceived has been called on
    //      every interested services by the ISipRequestContext, the request
    //      context calls back each service that added an event to the event
    //      list and lets it call its event to the application.
    //
    //      Note that, if the packet is a response, after the first event, the
    //      application has the ability to tell the request context (through
    //      the ISipClientEventControl interface) whether or not it wants to see
    //      the other events. If the application does not want to see the other
    //      events, the request context calls ClearEvent for each event still in
    //      the event list.
    //
    //      If the packet is a request, the application sees only the first
    //      event from the list and sends a response through the
    //      ISipServerEventControl interface of the request context.
    //
    //  See Also:
    //      ISipRequestContext, CSipCoreEventList, ClearEvent,
    //      ISipClientEventControl, ISipServerEventControl
    //
    //==EDOC====================================================================
    virtual void OnPacketReceived(IN ISipRequestContext& rRequestContext,
                                  IN const CSipPacket& rPacket,
                                  INOUT CSipCoreEventList& rEventList) = 0;

    //==SDOC====================================================================
    //==
    //==  UpdatePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Modifies a packet about to be sent.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet to be sent.
    //
    //  Returns:
    //      resS_OK:
    //          The packet has been successfully updated.
    //
    //      resSW_SIPCORE_WAIT_ASYNC_RESPONSE:
    //          Asynchronous processing is needed to query the application for
    //          packet modification.
    //
    //  Description:
    //      Notifies an interested service that a packet is to be sent. The
    //      service can then update its internal state and even modify the
    //      packet according to its configuration. For instance, the
    //      authentication request context service could add its current
    //      credentials to an outgoing request.
    //
    //      Note that UpdatePacket is not called on the owner of a request
    //      context because the owner initiates the packet to send. It can 
    //      update its state and modify the packet before and after the packet 
    //      is updated by other request context core services.
    //
    //      This method can asynchronously consult the application to further
    //      update the packet. In this case, an information result is returned 
    //      by this method and the packet is then returned by the application
    //      through the OnPacketUpdated method.
    //
    //  See Also:
    //      ISipRequestContext, OnPacketUpdated
    //
    //==EDOC====================================================================
    virtual mxt_result UpdatePacket(IN ISipRequestContext& rRequestContext,
                                    INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  OnPacketUpdated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the service that the application is done updating the 
    //      packet.
    //
    //  Returns:
    //      resS_OK:
    //          The packet has been successfully updated.
    //
    //      resFE_FAIL:
    //          The packet could not be sent. When the ISipRequestContext
    //          receives this status, it acts as if a failure response was
    //          received for requests.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet to send.
    //
    //  Description:
    //      This method is called by the request context on an owner service
    //      after it tried to update a SIP packet but asynchronous processing
    //      was required. This means that ISipRequestContext::UpdatePacket
    //      returned resSW_SIPCORE_WAIT_ASYNC_RESPONSE.
    //
    //      This is called after ALL services have successfully updated the
    //      packet, asynchronously or not.
    //
    //      The only processing left for the owner service when this method is
    //      called is to possibly further update the packet and then send it
    //      through the transaction manager or the transport manager.
    //
    //  See Also:
    //      UpdatePacket
    //
    //==EDOC====================================================================
    virtual mxt_result OnPacketUpdated(IN ISipRequestContext& rRequestContext,
                                       INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  SendResponse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a response to a received request.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet from which the response must be built.
    //
    //    uCode:
    //      The response code to send.
    //
    //    szReason:
    //      The reason phrase to send.
    //
    //    pExtraHeaders:
    //      Additional headers for the response to send. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      Message-body for the response to send. Ownership is TAKEN.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The attached ISipUserAgentSvc is incorrectly
    //      configured. For instance, the contact list could be empty.
    //
    //      - resS_OK: The response is being sent.
    //
    //  Description:
    //      This method is called by the ISipRequestContext on its owner service
    //      when the application calls the ISipServerEventControl::SendResponse
    //      method. It builds the CSipPacket and sends it through the
    //      ISipRequestContext::SendResponse method (which takes a CSipPacket in
    //      parameter instead of the code and reason phrase).
    //
    //      This method is guaranteed to be called by the request context only
    //      for server transactions and for at most one final response.
    //
    //  See Also:
    //      ISipRequestContext
    //
    //==EDOC====================================================================
    virtual mxt_result SendResponse(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket,
                                    IN unsigned int        uCode,
                                    IN const char*         szReason,
                                    IN TO CHeaderList*     pExtraHeaders,
                                    IN TO CSipMessageBody* pMessageBody) = 0;

    //==SDOC====================================================================
    //==
    //==  OnRequestCancelled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies that a CANCEL has been received for a pending request.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet received.
    //
    //  Description:
    //      This method is called by the ISipRequestContext on its owner service
    //      when the transaction manager notifies it of a received CANCEL
    //      matching its transaction. The request context service should notify
    //      the application and send a 487 response if it did not already sent a
    //      final status for that request.
    //
    //      Note that the response to the CANCEL request is sent directly by the
    //      ISipRequestContext itself.
    //
    //  See Also:
    //      ISipRequestContext
    //
    //==EDOC====================================================================
    virtual void OnRequestCancelled(IN ISipRequestContext& rRequestContext,
                                    IN const CSipPacket&   rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  OnPacketReadyToSend
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the core service that a packet is ready to send.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    rPacket:
    //      The packet received.
    //
    //  Description:
    //      This method is called by the ISipRequestContext on its owner
    //      service when a packet is ready to be sent. After this method
    //      is called, the request context is done for this request or
    //      response.
    //
    //      This method is the last chance a service has of updating the
    //      outgoing packet. However, this should be done such that the
    //      routing of the packet doesn't change. For instance, modifying
    //      the Request-URI is not allowed. Note that this is not
    //      enforced and the packet will not be validated in any way.
    //
    //  See Also:
    //      ISipRequestContext
    //
    //==EDOC====================================================================
    virtual void OnPacketReadyToSend(IN ISipRequestContext& rRequestContext,
                                     INOUT CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  CallEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Lets the service call an event on the application.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    opqEvent:
    //      Data stored by this object when it added the event to the event
    //      list.
    //
    //    rPacket:
    //      The received packet that triggered the event.
    //
    //  Description:
    //      This method is called by the ISipRequestContext when the packet has
    //      been shown to every services interested in the request context
    //      (through the OnPacketReceived method). The services that have an
    //      event to report to the application added the event to the
    //      CSipCoreEventList in parameter. The ISipRequestContext then calls
    //      CallEvent for each event added to the list as long as the
    //      application calls CallNextEvent (on its ISipClientEventControl
    //      interface).
    //
    //      When the service adds an event to the event list, this method is
    //      always called for that event unless ClearEvent is called instead.
    //
    //  See Also:
    //      ISipRequestContext, ISipClientEventControl::CallNextEvent,
    //      ClearEvent
    //
    //==EDOC====================================================================
    virtual void CallEvent(IN ISipRequestContext& rRequestContext,
                           IN mxt_opaque opqEvent,
                           IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  ClearEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Lets the service clear the state associated with an event in which 
    //      the application has no interest.
    //
    //  Parameters:
    //    rRequestContext:
    //      The parent request context.
    //
    //    opqEvent:
    //      Data stored by this object when it added the event to the event
    //      list.
    //
    //  Description:
    //      This method is called by the ISipRequestContext when the packet has
    //      been shown to every services interested in the request context
    //      (through the OnPacketReceived method). Another service has reported
    //      an event to the application and this event was sufficient for the
    //      application to take appropriate action. The application then
    //      informed the request context to ignore remaining events (through the
    //      SendResponse, ReIssueRequest, or Clear methods of the
    //      ISipServerEventControl interface). The ISipRequestContext then calls
    //      ClearEvent for each event still remaining in the event list.
    //
    //      When the service adds an event to the event list, this method is
    //      always called for that event unless CallEvent is called instead.
    //
    //      When this method is called, the service should act exactly as when
    //      CallEvent is called except that it should not call an event on the
    //      application.
    //
    //  See Also:
    //      ISipRequestContext, ISipServerEventControl::SendResponse,
    //      ISipClientEventControl::ReIssueRequest, IAppTransaction::Clear,
    //      CallEvent.
    //
    //==EDOC====================================================================
    virtual void ClearEvent(IN ISipRequestContext& rRequestContext,
                            IN mxt_opaque opqEvent) = 0;

    //==SDOC====================================================================
    //==
    //==  AddRcsRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a reference on this request context core service.
    //
    //  Returns:
    //      The number of references.
    //
    //  Description:
    //      Adds a reference on this object.
    //
    //  See Also:
    //      ReleaseRcsRef
    //
    //==EDOC====================================================================
    virtual unsigned int AddRcsRef() = 0;

    //==SDOC====================================================================
    //==
    //==  ReleaseRcsRef
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a reference on this request context core service.
    //
    //  Returns:
    //      The number of remaining references.
    //
    //  Description:
    //      Removes a reference on this object. When the reference count reaches
    //      0, the object removes itself from memory.
    //
    //  See Also:
    //      AddRcsRef
    //
    //==EDOC====================================================================
    virtual unsigned int ReleaseRcsRef() = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReqCtxCoreSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReqCtxCoreSvc() {}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReqCtxCoreSvc(IN const ISipReqCtxCoreSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReqCtxCoreSvc& operator=(IN const ISipReqCtxCoreSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREQCTXCORESVC_H

