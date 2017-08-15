//==SDOC========================================================================
//==============================================================================
//
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
#ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSESVC_H
#define MXG_ISIPRELIABLEPROVISIONALRESPONSESVC_H

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
class ISipReliableProvisionalResponseMgr;
class ISipServerEventControl;
class CSipMessageBody;
class CHeaderList;
class ISipClientTransaction;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipReliableProvisionalResponseSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReliableProvisionalResponseSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The reliable provisional response service allows the application to support
//  the 100rel extension as defined in RFC 3262. This service is responsible for
//  sending and receiving reliable provisional responses for INVITE transaction.
//  When sending a reliable provisional response, it automatically takes
//  care of retransmitting the response until a corresponding PRACK is received
//  and responded 2xx or a timeout occurs. When receiving a reliable provisional
//  response, it notifies the application of only one of the retransmitted
//  responses and silently catches the retransmissions. This service also
//  handles the extension tag '100rel' that needs to be put in Supported or
//  Require header fields.
//
//  This service permits to send or receive reliable provisional responses to an
//  INVITE request one at a time. This means that the application using this
//  service as UAS can only send one reliable provisional response at a time,
//  and also that an application using this service as UAC can only receive one
//  reliable provisional response at a time (the others are ignored). The
//  reliable provisional response process is considered over when a 2xx response
//  was sent or received for a PRACK request corresponding to the provisional
//  response.
//
//  RFC 3262 states that a 2xx response MUST NOT be sent until all reliable
//  provisional responses are acknowledged if any of the reliable provisional
//  responses contained a session description. To be compliant with this
//  condition, the reliable provisional service delays a 2xx response to an
//  INVITE if there is still an unacknowledged reliable provisional response
//  until a corresponding PRACK is received and responded successfully or the
//  reliable provisional response times out.
//
//  Note that the usage of reliable provisional responses has implications
//  where SDP offer and answer can appear. In particular, RFC 3264 states that
//  when a SDP offer appears in an INVITE request, the answer may appear in any
//  reliable response to the INVITE. When the 100rel extension is used, the
//  responses sent through the reliable provisional response service are
//  considered reliable responses and can hence contain the answer as well as
//  the 2xx response sent by the session service. Also, if the INVITE did not
//  contain an offer, RFC 3261 mandates that the offer MUST be sent in the first
//  reliable response to that INVITE.
//
//  For more information on SDP offers and reliable provisional responses,
//  please see section 5 in RFC 3262.
//
//  This service generates events to the application through the
//  ISipReliableProvisionalResponseMgr interface.
//
//#The ISipReliableProvisionalResponseSvc is an ECOM object#
//
//  The ISipReliableProvisionalResponseSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipReliableProvisionalResponseSvc</B>
//
//  <B>Interface Id: IID_ISipReliableProvisionalResponseSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipReliableProvisionalResponseSvc.h
//
// See Also:
//   ISipReliableProvisionalResponseMgr
//
//==EDOC========================================================================
class ISipReliableProvisionalResponseSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipReliableProvisionalResponseSvc);
    //M5T_INTERNAL_USE_END

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
    //      resFE_INVALID_ARGUMENT if pMgr is NULL. The manager is not
    //          changed in this case.
    //
    //      resS_OK otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events generated by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual
        mxt_result SetManager(IN ISipReliableProvisionalResponseMgr* pMgr) = 0;


    //==SDOC====================================================================
    //==
    //==  LocalRequestsRequireReliability
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures whether or not reliable provisional responses are
    //      required for local INVITE requests.
    //
    //  Parameters:
    //    bRequire:
    //      Requires the usage of reliable provisional responses when true.
    //      Otherwise, lets the remote endpoint decide whether or not reliable
    //      responses are used.
    //
    //  Returns:
    //      resFE_INVALID_STATE if this service has no manager. This could
    //          happen when the manager was not set or when Clear was called on
    //          the context.
    //
    //      resS_OK if the method succeeded
    //
    //  Description:
    //      Use this method to require usage of reliable provisional response
    //      from the remote party.
    //
    //      This method has an impact only when this service is acting as a UAC.
    //      When setting the bRequire to true it puts a "Require: 100rel"
    //      header in every INVITE request sent. The remote party must then only
    //      send reliable provisional responses (except for the 100 Trying which
    //      cannot be sent reliably since it is processed hop by hop) or reject
    //      the call. If bRequire is set to false, the "Supported: 100rel"
    //      header is put in every INVITE instead. The remote party can then
    //      decide whether or not to send its provisional responses reliably.
    //
    //      Reliable provisional responses are received through the
    //      ISipReliableProvisionalResponseMgr::EvReliableResponseReceived
    //      method.
    //
    //      When the service is created, the default value is false.
    //
    //  See Also:
    //      ISipReliableProvisionalResponseMgr
    //
    //==EDOC====================================================================
    virtual mxt_result LocalRequestsRequireReliability(IN bool bRequire) = 0;

    //==SDOC====================================================================
    //==
    //==  MakeReliableServerEventControl
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gives an ISipServerEventControl interface through which provisional
    //      responses are sent reliably.
    //
    //  Parameters:
    //    pServerEventCtrl:
    //      The ISipServerEventControl interface given in the event signaling
    //      the reception of the INVITE request (normally
    //      ISipSessionMgr::EvInvited or ISipSessionMgr::EvReInvited). This
    //      object is used by rpRelServerEventCtrl to send requests and to
    //      manage opaque values. It MUST NOT be NULL.
    //
    //      IMPORTANT: If this method succeeds, this interface must no longer be
    //      used. The application should release it and use rpRelServerEventCtrl
    //      instead. Doing otherwise may cause problems in the reliable
    //      provisional response service.
    //
    //    rpRelServerEventCtrl:
    //      The reliable ISipServerEventControl interface created by using
    //      pServerEventCtrl. Reliable provisional responses are sent through
    //      rpRelServerEventCtrl. rpRelServerEventCtrl must be used to send all
    //      types of responses (it must replace pServerEventCtrl). The opaque
    //      value of this ISipServerEventControl object is the same as
    //      pServerEventCtrl.
    //
    //      It is NULL if there was an error while calling this method.
    //
    //  Returns:
    //      resFE_INVALID_STATE if this service has no manager. This could
    //                             happen when the manager was not set or when
    //                             Clear was called on the context.
    //                          OR if there is no INVITE request pending that
    //                             supports reliability of provisional
    //                             responses.
    //
    //      resFE_INVALID_ARGUMENT if pServerEventCtrl is not the interface
    //                                managing the INVITE request in which a
    //                                reliable provisional response can be sent.
    //
    //      resS_OK if all is correct, in this case, rpRelServerEventCtrl takes
    //                 the value of a reliable ISipServerEventControl interface.
    //
    //  Description:
    //      This method gives an ISipServerEventControl interface that must be
    //      used to send all the remaining responses to the INVITE request.
    //      Non-100 provisional responses are automatically sent reliably
    //      with this ISipServerEventControl interface.
    //
    //      To succeed, this method needs the ISipServerEventControl given in
    //      the event signaling the reception of the INVITE request.
    //
    //      Note that the application must not send reliable provisional
    //      responses if it is not supported by the peer user agent. To know it,
    //      the application should call IsReliabilitySupportedByPeer. If the
    //      application calls this method when reliability is not supported by
    //      the peer user agent, it fails.
    //
    //  See Also:
    //      IsReliabilitySupportedByPeer, IsReliabilityRequiredByPeer
    //
    //==EDOC====================================================================
    virtual mxt_result MakeReliableServerEventControl(
                        IN ISipServerEventControl* pServerEventCtrl,
                        OUT ISipServerEventControl*& rpRelServerEventCtrl ) = 0;

    //==SDOC====================================================================
    //==
    //==  Prack
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a PRACK request to acknowledge a reliable provisional response
    //      received.
    //
    //  Parameters:
    //    transactionOpaque:
    //      Application data to associate with this transaction. This is opaque
    //      to the service.
    //
    //    pExtraHeaders:
    //      Extra SIP headers to send with the request. Adding a RAck header in
    //      this header list is useless since this method replaces it with
    //      the RAck header it generates. It can be NULL. Ownership is TAKEN.
    //
    //    pMessageBody:
    //      Message-body to include with the PRACK. It can be NULL. Ownership is
    //      TAKEN.
    //
    //    rpTransaction:
    //      Interface to the created client transaction.
    //
    //  Returns:
    //      resFE_INVALID_STATE:
    //          When the manager of this service has not been set or Clear was
    //          called on the context.
    //
    //      resFE_INVALID_STATE:
    //          If no reliable response has been received.
    //
    //      resFE_FAIL:
    //          When a PRACK request has already been sent for the reliable
    //          provisional response.
    //
    //      resFE_SIPCORE_PACKET_BLOCKED:
    //          One of the service synchronously blocked the packet.
    //          No additional event will be reported.
    //
    //      resS_OK:
    //          If the request has been sent.
    //
    //  Description:
    //      Upon reception of a reliable provisional response (through the
    //      ISipReliableProvisionalResponseMgr::EvReliableResponseReceived()
    //      event), a PRACK request MUST be sent to acknowledge reception of the
    //      response. This method should be used to send this PRACK. It returns
    //      an error if there is no reliable provisional response to acknowledge
    //      or if there was already a PRACK request sent. This method
    //      automatically takes care of adding the RAck header that is mandatory
    //      in PRACK requests.
    //
    //      RFC 3262 describes when a content should accompany the PRACK request.
    //      When needed, such a content can be set through the pMessageBody
    //      parameter.
    //
    //      Extra headers can be added to the PRACK request through the
    //      pExtraHeaders. If pExtraHeaders already contains a RAck header, the
    //      header is removed and replaced by the RAck header that the service
    //      has generated.
    //
    //  See Also:
    //      ISipReliableProvisionalResponseMgr
    //
    //==EDOC====================================================================
    virtual mxt_result Prack(IN mxt_opaque transactionOpaque,
                             IN TO CHeaderList* pExtraHeaders,
                             IN TO CSipMessageBody*  pMessageBody,
                             OUT ISipClientTransaction*& rpTransaction) = 0;

    //==SDOC====================================================================
    //==
    //==  IsReliabilitySupportedByPeer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Indicates whether or not the remote endpoint supports the 100rel
    //      extension.
    //
    //  Returns:
    //      true when the remote endpoint supports the 100rel extension
    //
    //      false otherwise.
    //
    //  Description:
    //      Returns true if the last INVITE received in the parent context
    //      while there was no other pending INVITE request contained either a
    //      Require or a Supported with the '100rel' option-tag. If no such
    //      header was present, or if no INVITE has been received yet, it
    //      returns false.
    //
    //      When false is returned, the peer UA does not support reliability in
    //      the provisional responses so reliable provisional responses MUST NOT
    //      be sent.
    //
    //==EDOC====================================================================
    virtual bool IsReliabilitySupportedByPeer() = 0;

    //==SDOC====================================================================
    //==
    //==  IsReliabilityRequiredByPeer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies whether or not the remote endpoint insists on reliable
    //      delivery of provisional response.
    //
    //  Returns:
    //      true when the remote endpoint insists on reliable delivery of
    //      provisional response.
    //
    //      false otherwise.
    //
    //  Description:
    //      Returns true if the last INVITE received in the parent context
    //      while there was no other INVITE pending contained a Require header
    //      with the '100rel' option-tag. If no such header was present, or if
    //      no INVITE has been received yet, it returns false.
    //
    //      When true is returned, the application MUST send non-100 provisional
    //      responses reliably. The application must call
    //      MakeReliableServerEventControl to get a reliable
    //      ISipServerEventControl interface through which non-100 provisional
    //      responses are sent reliably.
    //
    //==EDOC====================================================================
    virtual bool IsReliabilityRequiredByPeer() = 0;


//-- Hidden Methods
//-------------------
protected:
    // << Constructors / Destructors >>
    //--------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReliableProvisionalResponseSvc() {}

    // Summary:
    //  Default Destructor.
    //----------------------
    virtual ~ISipReliableProvisionalResponseSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReliableProvisionalResponseSvc(
                             IN const ISipReliableProvisionalResponseSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReliableProvisionalResponseSvc&
    operator=(IN const ISipReliableProvisionalResponseSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPRELIABLEPROVISIONALRESPONSESVC_H

