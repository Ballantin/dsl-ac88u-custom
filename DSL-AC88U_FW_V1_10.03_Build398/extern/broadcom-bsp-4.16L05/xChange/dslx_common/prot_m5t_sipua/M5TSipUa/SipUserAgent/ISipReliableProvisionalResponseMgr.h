//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSEMGR_H
#define MXG_ISIPRELIABLEPROVISIONALRESPONSEMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT to be able \
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
class CSipPacket;
class ISipClientEventControl;
class ISipReliableProvisionalResponseSvc;
class ISipServerEventControl;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReliableProvisionalResponseMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The reliable provisional response manager is the interface through which the
//  corresponding service reports events to the application. The M5T stack 
//  informs the application, through this interface, of the reception of a
//  reliable 1xx response or the reception of a PRACK to a reliable 1xx
//  response. It is also through this interface that the application can have
//  access to the various payloads carried with the SIP packets, which are
//  usually updated SDP offers and answers.
//
// Updated Behaviour:
//
//  The behaviour of the reliable provisional response service has changed from
//  MxSF v3.x.
//
//  - An INVITE request requiring reliability in its provisional responses is
//    NO LONGER reported to the manager. It is now the responsability of the
//    application to verify if the option-tag '100rel' is present in the Require
//    header of the INVITE request. If so, the application MUST send non-100
//    provisional responses reliably. If the application does not want to do so,
//    it MUST reject the INVITE with a 420 response where an Unsupported header
//    contains the '100rel' option-tag. The application can use
//    ISipReliableProvisionalResponseSvc::IsReliabilitySupportedByPeer and
//    ISipReliableProvisionalResponseSvc::IsReliabilityRequiredByPeer to know
//    if the peer UA supports or requires reliability in the provisional
//    responses.
//
// Location:
//   SipUserAgent/ISipReliableProvisionalResponseMgr.h
//
// See Also:
//   ISipReliableProvisionalResponseSvc
//
//==EDOC========================================================================
class ISipReliableProvisionalResponseMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvReliableResponseReceived
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a reliable provisional response has been received.
    //
    //  Parameters:
    //    ISipReliableProvisionalResponseSvc:
    //      The service managing the reliable provisional responses to INVITE
    //      requests.
    //
    //    pClientEventCtrl:
    //      The interface through which access client event related
    //      functionality.
    //
    //    rPacket:
    //      The incoming reliable provisional response.
    //
    //  Description:
    //      This method is called upon receiving a valid reliable provisional
    //      response. When this event occurs, the application must call
    //      ISipReliableProvisionalResponseSvc::Prack() to acknowledge the
    //      response. Note that this method is not called to retransmit the
    //      reliable response. Neither will it be called if the RSeq number in 
    //      the response is more than one higher than the RSeq number in the 
    //      last reliable response received.
    //
    //  See Also:
    //      ISipReliableProvisionalResponseSvc::Prack()
    //
    //==EDOC====================================================================
    virtual void EvReliableResponseReceived(
                                    IN ISipReliableProvisionalResponseSvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvResponseAcknowledged
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a PRACK for the last provisional response sent has
    //      been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the PRACK request.
    //
    //    pServerEventCtrl:
    //      The interface through which the application must reply at least a
    //      final response.
    //
    //    rPacket:
    //      The incoming PRACK.
    //
    //  Description:
    //      This method is called when a PRACK is received and its RAck header
    //      matches the last reliable provisional response that was sent. The
    //      application must use ISipServerEventControl::SendResponse() to send
    //      a final response to this PRACK.
    //
    //      This method is called every time a PRACK request is received.
    //      It can be called more than once per reliable provisional response
    //      sent. The application should accept one PRACK request with a 200 OK
    //      response and reject the others with a proper error status code. When
    //      a failure response is sent to all the PRACK received, the
    //      application should be prepared to receive new PRACK requests for the
    //      reliable provisional response. The reliable provisional response
    //      processing is not terminated until a 2xx response is sent for a
    //      PRACK received for it.
    //
    //  See Also:
    //      ISipServerEventControl::SendResponse
    //
    //==EDOC====================================================================
    virtual void EvResponseAcknowledged(
                                    IN ISipReliableProvisionalResponseSvc* pSvc,
                                    IN ISipServerEventControl* pServerEventCtrl,
                                    IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvInvalidPrack
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs the application that an invalid PRACK request has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the PRACK request.
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    rPacket:
    //      The invalid incoming PRACK.
    //
    //    resReason:
    //      resFE_MISSING_HEADER:
    //          There is no RAck header in the PRACK request. The service has 
    //          automatically replied with a 400 response with "Missing RAck 
    //          header" as reason-phrase.
    //
    //      resFE_CALL_LEG_TRANSACTION_DOES_NOT_EXIST:
    //          The PRACK request does not match any reliable provisional
    //          response sent by the service. The service has automatically 
    //          replied with a 481 response.
    //
    //  Description:
    //      An invalid PRACK request has been received. The request may be
    //      invalid because the service is not ready to receive it.
    //
    //      If the manager of the pSvc was not set, all the PRACK requests are
    //      responded with a 500 response.
    //
    //==EDOC====================================================================
    virtual void EvInvalidPrack(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN mxt_opaque opqApplicationData,
                                IN const CSipPacket& rPacket,
                                IN mxt_result resReason) = 0;

    //==SDOC====================================================================
    //==
    //==  EvReliableProvisionalResponseTimeout
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that no PRACK has been received and answered successfully 
    //      (2xx) for the last provisional response sent.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the retransmission of the responses.
    //
    //    opqTransaction:
    //      The opaque parameter associated with the server transaction.
    //
    //  Description:
    //      This method is called when no PRACK has arrived or was answered
    //      successfully (2xx) 64*T1 seconds after a reliable provisional
    //      response was sent. The application should consider that the reliable
    //      provisional response was not received by the remote party. RFC 3262
    //      recommends that the INVITE transaction SHOULD be rejected with a 5xx
    //      response.
    //
    //      If a PRACK request has been received and not responded with a final
    //      response yet, the application MUST send a final response to the
    //      PRACK request. Until a PRACK request is responded successfully to,
    //      trying to send a reliable provisional response fails.
    //
    //==EDOC====================================================================
    virtual void EvReliableProvisionalResponseTimeout(
                                    IN ISipReliableProvisionalResponseSvc* pSvc,
                                    IN mxt_opaque opqTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  EvPrackProgress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a provisional response to the last sent PRACK has
    //      been received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the PRACK request.
    //
    //    pClientEventCtrl:
    //      The interface through which access client event related
    //      functionality.
    //
    //    rPacket:
    //      The provisional response to the PRACK request.
    //
    //  Description:
    //      This method is called when a provisional response (1xx) to a PRACK
    //      request is received.
    //
    //==EDOC====================================================================
    virtual void EvPrackProgress(IN ISipReliableProvisionalResponseSvc* pSvc,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvPrackSuccess
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a success response to the last PRACK sent has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the PRACK request.
    //
    //    pClientEventCtrl:
    //      The interface through which access client event related
    //      functionality.
    //
    //    rPacket:
    //      The success response to the PRACK request.
    //
    //  Description:
    //      This method is called when a success response (2xx) to a PRACK is
    //      received. Either this method or EvPrackFailure is called once
    //      for every PRACK request sent. When this method is called, the
    //      processing of the reliable provisional response is terminated so the
    //      service is ready to receive a new reliable provisional response.
    //
    //==EDOC====================================================================
    virtual void EvPrackSuccess(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket) = 0;

    //==SDOC====================================================================
    //==
    //==  EvPrackFailure
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates that a failure response to the last PRACK sent has been
    //      received.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the PRACK request.
    //
    //    pClientEventCtrl:
    //      The interface through which access client event related
    //      functionality.
    //
    //    rPacket:
    //      The failure response to the PRACK request.
    //
    //  Description:
    //      This method is called when a failure response (>=300) to a PRACK is
    //      received. Either this method or EvPrackSuccess is called once
    //      for every PRACK request sent. When this method is called, the PRACK
    //      request was denied so the reliable provisional response process is
    //      not terminated. The application could send another PRACK request to
    //      try to finish the reliable provisional reponse process again.
    //
    //      Note that the stack reports a timeout by creating a 408 response and
    //      reporting it to the application (through this method if the request
    //      sent was sent by the ISipReliableProvisionalResponseSvc).
    //
    //==EDOC====================================================================
    virtual void EvPrackFailure(IN ISipReliableProvisionalResponseSvc* pSvc,
                                IN ISipClientEventControl* pClientEventCtrl,
                                IN const CSipPacket& rPacket) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReliableProvisionalResponseMgr() {}

    // Summary:
    //  Default Destructor.
    //----------------------
    virtual ~ISipReliableProvisionalResponseMgr() {}
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReliableProvisionalResponseMgr(
                            IN const ISipReliableProvisionalResponseMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReliableProvisionalResponseMgr& operator=(
                            IN const ISipReliableProvisionalResponseMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSEMGR_H

