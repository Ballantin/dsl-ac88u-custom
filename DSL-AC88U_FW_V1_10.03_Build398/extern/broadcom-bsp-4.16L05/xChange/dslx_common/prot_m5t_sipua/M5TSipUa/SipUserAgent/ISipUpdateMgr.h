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
#ifndef MXG_ISIPUPDATEMGR_H
#define MXG_ISIPUPDATEMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT to be able \
to use this class.
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipClientEventControl;
class ISipServerEventControl;
class ISipUpdateSvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipUpdateMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The update manager is the interface through which the update service talks
//  to the application. It is through this interface that M5T SIP UA informs the
//  application of incoming UPDATE requests and of incoming responses to UPDATE
//  requests sent to the remote party.
//
//  It is important to note that a special process is required by the
//  application for the event EvUpdated when there is a pending offer to which
//  no answer has been sent or received. Under some circumstances, the
//  application has to answer a 491 or 500 response when there is a pending 
//  offer. Except for UPDATE request process, M5T SIP UA does not send such 
//  responses because it is the application's responsibility to look at the 
//  offers and to know whether or not there is a pending offer. M5T SIP UA 
//  automatically answers a 500 response to an UPDATE if it has received an 
//  incoming UPDATE to which no final answer has been sent. M5T SIP UA also 
//  automatically answers a 491 response to an UPDATE if it did not received a 
//  final response to a previous outgoing UPDATE request. For more details on 
//  this, please see the anotated RFC 3311 Section 5.2 Conformance Items 12 to 
//  19.
//
//  Upon reception of a 2xx response to a sent UPDATE request, the application
//  MUST change the session description parameters according to what is found in
//  the 2xx response. If a non-2xx response is received, the session description
//  parameters MUST remain unchanged.
//
// Location:
//   SipUserAgent/ISipUpdateMgr.h
//
// See Also:
//   ISipUpdateSvc, RFC 3311
//
//==EDOC========================================================================
class ISipUpdateMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvUpdated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that an UPDATE request has been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the request.
    //
    //    pServerEventCtrl:
    //      The interface through which the application must return at least one
    //      response.
    //
    //    rUpdateRequest:
    //      The UPDATE request.
    //
    //  Description:
    //      Notifies the application that a remote user is trying to refresh the
    //      target by sending an UPDATE. The application should send responses
    //      through ISipServerEventControl::SendResponse. The UPDATE MUST be
    //      promptly responded to. This means that the user MUST NOT be prompted
    //      to approve the session changes.
    //
    //      If the packet contains a payload, the application MUST inspect the
    //      offer contained within. If the application already has a pending
    //      generated or received offer (in an INVITE request or in a PRACK), it
    //      MUST reject the UPDATE with a 491 if the offer was generated by the
    //      local UA or with a 500 response if the offer was generated by the
    //      peer UA. The 500 response MUST have a Retry-After header with a
    //      random value between 0 and 10. If the application can generate an
    //      answer, it must put it in the 2xx response to the UPDATE. If the
    //      offer cannot be answered without prompting the user, the application
    //      SHOULD send a 504 response. If the offer is not acceptable, the
    //      application MUST answer with a 488 response. For more information on
    //      what response should be and what it must contain, refer to RFC 3311 
    //      in section 5.2.
    //
    //      Take note that if a 2xx response to the request is sent in response
    //      to this request, the Request-URI in the ISipUserAgentSvc is
    //      automatically updated to the value in the contact header of the
    //      request.
    //
    //      In some cases, an UPDATE request is automatically responded by the
    //      service. See EvInvalidUpdate for a list of these cases.
    //
    //  See Also:
    //      ISipUpdateSvc, EvProgress, EvSuccess, EvFailure,
    //      EvErroneousRequestPacket, RFC 3311
    //==EDOC====================================================================
    virtual void EvUpdated( IN ISipUpdateSvc* pSvc,
                            IN ISipServerEventControl* pServerEventCtrl,
                            IN const CSipPacket& rUpdateRequest ) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidUpdate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Warns the application that an invalid UPDATE request packet has been
    //      received.
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
    //      The packet containing the invalid UPDATE request.
    //
    //    resReason:
    //      resFE_INVALID_CONTACT_HEADER if the received UPDATE request did not
    //          contain a Contact header, contained an invalid Contact header, 
    //          or contained more than one Contact header. In this case, an
    //          automatic 400 "Missing or Invalid Contact header field" response
    //          has been sent to the peer UAC.
    //
    //      resFE_NO_ISIPUSERAGENTSVC_ATTACHED if an UPDATE request has been
    //          received while there was no ISipUserAgentSvc attached to the 
    //          context. In this case, an automatic 500 response has been sent
    //          to the peer UAC.
    //
    //      resFE_CALL_LEG_TRANSACTION_DOES_NOT_EXIST if an UPDATE request has
    //          been received while there was no dialog established. In this 
    //          case, an automatic 481 response has been sent to the peer UAC.
    //
    //      resFE_REQUEST_PENDING if an UPDATE request has been received while 
    //          there was already an UPDATE request in progress. In this case, 
    //          an automatic response has been sent to the peer UAC. If the 
    //          UPDATE request in progress has been sent by the local UA, a 491 
    //          response is sent. If the request in progress has been sent by 
    //          the peer UAC, a 500 response with a Retry-After header is sent.
    //
    //  Description:
    //      This method is called when an UPDATE request is received while the
    //      service cannot accept it.
    //
    //      Take note that if the manager of the service is not set, all the
    //      UPDATE requests are responded with a 500 response.
    //
    //==EDOC====================================================================
    virtual void EvInvalidUpdate(IN ISipUpdateSvc* pSvc,
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
    //      Notifies the application that a provisional response has been 
    //      received for the sent UPDATE request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response.
    //
    //  Description:
    //      Notifies the application that a provisional response has been
    //      received for the sent UPDATE request.
    //
    //      Upon reception of a provisional response, the session description
    //      parameters MUST remain unchanged.
    //
    //  See Also:
    //      ISipUpdateSvc
    //==EDOC====================================================================
    virtual void EvProgress(IN ISipUpdateSvc* pSvc,
                            IN ISipClientEventControl* pClientEventCtrl,
                            IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a final successful response has been
    //      received for the sent UPDATE request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response.
    //
    //  Description:
    //      Informs the application that a response has been received for the
    //      sent UPDATE request.
    //
    //      Upon reception of this event, the application MUST update the
    //      session description parameters to the value received in the packet.
    //
    //      Take note that the Request-Uri in the ISipUserAgentSvc is
    //      automatically updated to the value found in the Contact header of
    //      the response.
    //
    //  See Also:
    //      ISipUpdateSvc
    //==EDOC====================================================================
    virtual void EvSuccess(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

    //==SDOC====================================================================
    //==
    //==  EvFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the application that a final failure response has been
    //      received for the sent UPDATE request.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the response.
    //
    //    pClientEventCtrl:
    //      The interface to access client event related functionality.
    //
    //    rPacket:
    //      The response.
    //
    //  Description:
    //      Notifies the application that a final negative response has been
    //      received for the sent UPDATE request.
    //
    //      Depending on the status code of the response, different actions
    //      might be taken by the application.
    //
    //      In case the response is a 491, the application can start a timer
    //      with a randomly chosen value (See RFC 3311 section 3.2 for more
    //      information on how the value should be chosen). When the timer
    //      fires, the application can once more try to issue the UPDATE via
    //      ISipClientEventControl::ReIssueRequest.
    //
    //      In case the response is 481 or 408, the application MUST terminate
    //      the dialog (by calling ISipSessionSvc::Bye for example). Note that
    //      the M5T SIP stack reports a timeout by making a 408 response and
    //      reporting it to the application (through this method if the request
    //      sent was sent by the ISipUpdateSvc).
    //
    //      In all cases, the session description parameters MUST remain
    //      unchanged.
    //
    //  See Also:
    //      ISipUpdateSvc
    //==EDOC====================================================================
    virtual void EvFailure(IN ISipUpdateSvc* pSvc,
                           IN ISipClientEventControl* pClientEventCtrl,
                           IN const CSipPacket& rResponse) = 0;

//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipUpdateMgr() {}

    // Summary:
    //  Default Destructor.
    //----------------------
    virtual ~ISipUpdateMgr() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipUpdateMgr(IN const ISipUpdateMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipUpdateMgr& operator=(IN const ISipUpdateMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPUPDATEMGR_H

