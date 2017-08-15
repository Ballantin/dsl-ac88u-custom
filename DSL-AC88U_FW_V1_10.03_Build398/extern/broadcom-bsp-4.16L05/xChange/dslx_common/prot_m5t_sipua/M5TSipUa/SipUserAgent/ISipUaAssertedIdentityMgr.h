//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
#ifndef MXG_ISIPUAASSERTEDIDENTITYMGR_H
#define MXG_ISIPUAASSERTEDIDENTITYMGR_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT to be able to use \
this class.
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
class CNameAddr;
class CSipPacket;
class ISipClientEventControl;
class ISipUaAssertedIdentitySvc;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipUaAssertedIdentityMgr
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  ISipUaAssertedIdentityMgr is the interface through which the
//  ISipUaAssertedIdentitySvc reports events to the application.
//
//  The ISipUaAssertedIdentitySvc reports an event when a packet is received
//  from an untrusted proxy or with at least a P-Asserted-Identity header.
//
//  If the packet has been received from an untrusted proxy, EvUntrustedProxy is
//  called.
//
//  If the P-Asserted-Identity header(s) are valid, one of the
//  EvAssertedIdentity events is called (the one called depends on whether
//  the packet received is a request or a response).
//
//  If the P-Asserted-Identity header(s) are invalid, one of the
//  EvInvalidAssertedIdentity events is called (still depending on whether the
//  packet being a request or a response).
//
//  If FQDNs are used, the application SHOULD consider the EvUntrustedProxy,
//  EvAssertedIdentity and EvInvalidAssertedIdentity events of this manager
//  with caution until the EvTrustedProxyDnsResolutionCompleted event has been
//  reported at least once. As soon as the EvTrustedProxyDnsResolutionCompleted
//  event is reported once, the service is in a reliable state. Before that
//  event is reported, the DNS resolution is still in progress and the events
//  might not be reliable.
//
//  In the case where ONLY IP addresses are used, the 
//  EvTrustedProxyDnsResolutionCompleted event will not be reported as no DNS
//  resolution will be necessary. The service is thus in a reliable state
//  right after the trusted proxy servers are set.
//
// Location:
//   SipUserAgent/ISipUaAssertedIdentityMgr.h
//
// See Also:
//   ISipUaAssertedIdentitySvc
//
//==EDOC========================================================================
class ISipUaAssertedIdentityMgr
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
    //==  EvUntrustedProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      A request has been received from an untrusted proxy.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identity UA side on this
    //      context.
    //
    //    rRequest:
    //      The request received from a proxy other than the trusted proxy.
    //
    //    rApplicationData:
    //      Application data opaque to the service. This parameter is an INOUT
    //      parameter. It is used to correlate the events reported by multiple
    //      services for a unique received request. If the application already
    //      received an event for that request through another manager
    //      interface, rApplicationData equals the value stored in it by the
    //      application. Otherwise, if it is the first event reported for this
    //      received request, rApplicationData is set to 0. The value of
    //      rApplicationData when EvUntrustedProxy returns is accessible
    //      through the GetOpaque() method of the ISipServerEventControl
    //      interface that accompanies the request when the owner service
    //      issues its event. This opaque data should be used to store the state
    //      indicating that the request contained has been received from a proxy
    //      it does not trust and be able to act accordingly when the session
    //      manager receives the event containing the ISipServerEventControl. 
    //      Note that this event cannot be processed asynchronously since the 
    //      opaque application data is passed by value to the 
    //      ISipServerEventControl interface.
    //
    //  Description:
    //      This event is reported when a request has been received from an 
    //      untrusted entity.
    //
    //      The request may or may not contain one or more P-Asserted-Identity
    //      headers. If such headers are present, they must be ignored.
    //
    //      Before reporting this event, the service will have cleared any
    //      preferred identity previously configured. This is to prevent having
    //      the preferred identity information used in future requests and
    //      responses issued by a service on the associated SIP Context. The
    //      preferred identity information may be sensitive and is thus kept
    //      private. An application that still wants to present its preferred
    //      identity under this condition has to reconfigure its preferred
    //      identity.
    //
    //      The address of the entity from which the packet has been received 
    //      can be retreived by calling rRequest.GetPeerAddr().
    //
    //==EDOC====================================================================
    virtual void EvUntrustedProxy( IN ISipUaAssertedIdentitySvc* pSvc,
                                   IN const CSipPacket& rRequest,
                                   INOUT mxt_opaque& rApplicationData ) = 0;


    //==SDOC====================================================================
    //==
    //==  EvUntrustedProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      A response has been received from an untrusted proxy.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing Network Asserted Identity UA side on this
    //      context.
    //
    //    pClientEventCtrl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it. If one of these methods is not called
    //      directly from the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The response received from a proxy other than the trusted proxy.
    //
    //  Description:
    //      This event is reported when a response has been received from an
    //      untrusted entity.
    //
    //      The response may or may not contain one or more P-Asserted-Identity
    //      headers. If such headers are present, they must be ignored.
    //
    //      Before reporting this event, the service will have cleared any
    //      preferred identity previously configured. This is to prevent having
    //      the preferred identity information used in future requests and
    //      responses issued by a service on the associated SIP Context. The
    //      preferred identity information may be sensitive and is thus kept
    //      private. An application that still wants to present its preferred
    //      identity under this condition has to reconfigure its preferred
    //      identity.
    //
    //      The address of the entity from which the packet has been received 
    //      can be retrieved by rRequest.GetPeerAddr().
    //
    //      Note that this method should never be called when a request is sent
    //      directly to a trusted proxy.
    //
    //==EDOC====================================================================
    virtual void EvUntrustedProxy( IN ISipUaAssertedIdentitySvc* pSvc,
                                   IN ISipClientEventControl* pClientEventCtrl,
                                   IN const CSipPacket& rResponse ) = 0;


    //==SDOC====================================================================
    //==
    //==  EvAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      A request has been received from the trusted proxy server with at 
    //      least one P-Asserted-Identity header.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identities UA side on this
    //      context.
    //
    //    pSipIdentity:
    //      The SIP or SIPS identity in a P-Asserted-Identity header present in
    //      the request.
    //
    //      It MAY be NULL.
    //
    //    pTelIdentity:
    //      The telephone identity in a P-Asserted-Identity header present in
    //      the request.
    //
    //      It MAY be NULL.
    //
    //    bPrivacyRequested:
    //      true if privacy was requested by the user for the Network Asserted
    //      Identity. In this case, the application MUST ensure that it does not
    //      forward the Network Asserted Identity outside the Trust Domain.
    //
    //      false if privacy was not request by the user for the Network
    //      Asserted Identity.
    //
    //    rRequest:
    //      The request containing the P-Asserted-Identity header(s).
    //
    //    rApplicationData:
    //      Application data opaque to the service. This parameter is an INOUT
    //      parameter. It is used to correlate the events reported by multiple
    //      services for a unique received request. If the application has 
    //      already received an event for that request through another manager
    //      interface, rApplicationData equals the value stored in it by the
    //      application. Otherwise, if it is the first event reported for this
    //      received request, rApplicationData is set to 0. The value of
    //      rApplicationData when EvAssertedIdentity returns is accessible
    //      through the GetOpaque() method of the ISipServerEventControl
    //      interface that accompanies the request when the owner service issues
    //      its event. This opaque data should be used to store the state
    //      indicating that the request contained P-Asserted-Identity header(s)
    //      and be able to act accordingly when the session manager receives the
    //      event containing the ISipServerEventControl. Note that this event
    //      cannot be processed asynchronously since the opaque application
    //      data is passed by value to the ISipServerEventControl interface.
    //
    //  Description:
    //      This event is reported when a request is received from the trusted
    //      proxy server with valid P-Asserted-Identity header(s).
    //
    //      RFC 3325 states that a maximum of two headers can be present in a
    //      packet: one header with a SIP or SIPS URI and the other with
    //      a telephone URI. There may be one of the headers, both, or none. 
    //      This event is not reported when there are none.
    //
    //      The identity contained in the P-Asserted-Identity header(s) may be
    //      considered "privileged, or intrinsically more trustworthy than the
    //      From header field of a request" (RFC 3325, section 8, first
    //      paragraph).
    //
    //      Also, RFC 3325 states that "If a UA is part of the Trust Domain from
    //      which it received a message containing a P-Asserted-Identity header
    //      field, then it can use the value freely but it MUST ensure that it
    //      does not forward the information to any element that is not part of
    //      the Trust Domain, if the user has requested that asserted identity
    //      information be kept private." A user requests for asserted identity
    //      information to be kept private by inserting the 'id' privacy type
    //      into the Privacy header. If the privacy type is present, the
    //      bPrivacyRequested parameter is true.
    //
    //      Finally, "If a UA is not part of the Trust Domain from which it
    //      received a message containing a P-Asserted-Identity header field,
    //      then it can assume this information does not need to be kept
    //      private."
    //
    //==EDOC====================================================================
    virtual void EvAssertedIdentity( IN ISipUaAssertedIdentitySvc* pSvc,
                                     IN const CNameAddr* pSipIdentity,
                                     IN const CNameAddr* pTelIdentity,
                                     IN bool bPrivacyRequested,
                                     IN const CSipPacket& rRequest,
                                     INOUT mxt_opaque& rApplicationData ) = 0;


    //==SDOC====================================================================
    //==
    //==  EvAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      A response has been received from the trusted proxy server with at 
    //      least one P-Asserted-Identity header.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identities UA side on this
    //      context.
    //
    //    pSipIdentity:
    //      The SIP or SIPS identity in a P-Asserted-Identity header present in
    //      the request.
    //
    //      It MAY be NULL.
    //
    //    pTelIdentity:
    //      The telephone identity in a P-Asserted-Identity header present in
    //      the request.
    //
    //      It MAY be NULL.
    //
    //    bPrivacyRequested:
    //      true if privacy was requested by the user for the Network Asserted
    //      Identity. In this case, the application MUST ensure that it does not
    //      forward the Network Asserted Identity outside the Trust Domain.
    //
    //      false if privacy was not request by the user for the Network
    //      Asserted Identity.
    //
    //    pClientEventCtrl:
    //      The client event control interface for this transaction. It cannot
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it. If one of these methods is not called
    //      directly from the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The response containing the P-Asserted-Identity header(s).
    //
    //  Description:
    //      This event is called when a response is received from the trusted
    //      proxy server with valid P-Asserted-Identity header(s).
    //
    //      RFC 3325 states that a maximum of two headers can be present in a
    //      packet. One header contains a SIP or SIPS URI and the other contains
    //      a telephone URI. There may be one of the headers, both, or none. 
    //      This event is not called if there is none.
    //
    //      The identity contained in the P-Asserted-Identity header(s) may be
    //      considered "privileged, or intrinsically more trustworthy than the
    //      From header field of a request" (RFC 3325, section 8, first
    //      paragraph).
    //
    //      Also, RFC 3325 states that "If a UA is part of the Trust Domain from
    //      which it received a message containing a P-Asserted-Identity header
    //      field, then it can use the value freely but it MUST ensure that it
    //      does not forward the information to any element that is not part of
    //      the Trust Domain, if the user has requested that asserted identity
    //      information be kept private." A user requests for asserted identity
    //      information to be kept private by inserting the 'id' privacy type
    //      into the Privacy header. If the privacy type is present, the
    //      bPrivacyRequested parameter is true.
    //
    //      Finally, "If a UA is not part of the Trust Domain from which it
    //      received a message containing a P-Asserted-Identity header field,
    //      then it can assume this information does not need to be kept
    //      private."
    //
    //==EDOC====================================================================
    virtual
        void EvAssertedIdentity( IN ISipUaAssertedIdentitySvc* pSvc,
                                 IN const CNameAddr* pSipIdentity,
                                 IN const CNameAddr* pTelIdentity,
                                 IN bool bPrivacyRequested,
                                 IN ISipClientEventControl* pClientEventCtrl,
                                 IN const CSipPacket& rResponse ) = 0;


    //==SDOC====================================================================
    //==
    //==  EvInvalidAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      A request containing invalid P-Asserted-Identity has been received 
    //      from the trusted proxy server.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identities UA side on this
    //      context.
    //
    //    rRequest:
    //      The request containing the invalid P-Asserted-Identity header(s).
    //
    //    opqApplicationData:
    //      Data set by the application in another event for this same request.
    //      When this is the first event for this request, it is set to 0.
    //
    //    resReason:
    //      resFE_UNSUPPORTED_URI_SCHEME:
    //        A name-addr received in the P-Asserted-Identity header is not a
    //        valid name-addr for the header.  It is neither a SIP, SIPS, or
    //        telephone URI
    //
    //      resFE_FAIL:
    //        There were too many of a URI type in the packet.
    //
    //  Description:
    //      This method is called when an invalid request is received.
    //
    //      The request may be invalid because a P-Asserted-Identity header did
    //      not contain a valid URI (neither SIP, SIPS, or telephone), because
    //      the request contained too many P-Asserted-Identity headers for the
    //      same URI type (for example, the request contained two
    //      P-Asserted-Identity headers with a SIP or SIPS URI) or because the
    //      usage of the P-Asserted-Identity header is not applicable for the
    //      request.
    //
    //      Before this event is reported, a 500 response is sent with the
    //      following reason phrase "Too many or erroneous P-Asserted-Identity
    //      header(s)".
    //
    //==EDOC====================================================================
    virtual void EvInvalidAssertedIdentity( IN ISipUaAssertedIdentitySvc* pSvc,
                                            IN const CSipPacket& rRequest,
                                            IN mxt_opaque opqApplicationData,
                                            IN mxt_result resReason ) = 0;


    //==SDOC====================================================================
    //==
    //==  EvInvalidAssertedIdentity
    //==
    //==========================================================================
    //
    //  Summary:
    //      A response containing invalid P-Asserted-Identity has been received 
    //      from the trusted proxy server.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identities UA side on this
    //      context.
    //
    //    pClientEventCtrl:
    //      The client event control interface for this transaction. It cannot 
    //      be NULL. The application must call either CallNextEvent or
    //      ClearClientEvents on it. If one of these methods is not called
    //      directly from the present method, a reference must be added on
    //      pClientEventControl. The added reference must be released after one
    //      of the methods is called.
    //
    //    rResponse:
    //      The response containing the invalid P-Asserted-Identity header(s).
    //
    //    resReason:
    //      resFE_UNSUPPORTED_URI_SCHEME:
    //        A name-addr received in the P-Asserted-Identity header is not a
    //        valid name-addr for the header.  It is neither a SIP, SIPS, or
    //        telephone URI
    //
    //      resFE_FAIL:
    //        There were too many of a URI type in the packet.
    //
    //  Description:
    //      This method is called when an invalid response is received from the
    //      trusted proxy server.
    //
    //      The response may be invalid because a P-Asserted-Identity header did
    //      not contain a valid URI (neither SIP, SIPS, or telephone), because
    //      the response contained too many P-Asserted-Identity headers for the
    //      same URI type (for example, the response contained two
    //      P-Asserted-Identity headers with a telephone URI) or because the
    //      P-Asserted-Identity header is not applicable in a response to the
    //      associated request.
    //
    //      The application can ignore this event by calling
    //      pClientEventCtrl->CallNextClientEvent. Note that reissuing the
    //      request should be useless to fix this problem since it must come
    //      from a SIP entity between this UA and the server UA.
    //
    //==EDOC====================================================================
    virtual void EvInvalidAssertedIdentity(
                                    IN ISipUaAssertedIdentitySvc* pSvc,
                                    IN ISipClientEventControl* pClientEventCtrl,
                                    IN const CSipPacket& rResponse,
                                    IN mxt_result resReason ) = 0;

    //==SDOC====================================================================
    //==
    //==  EvTrustedProxyDnsResolutionCompleted
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      The DNS resolution of the trusted proxy URI list is completed.
    //
    //  Parameters:
    //    pSvc:
    //      The service managing the Network Asserted Identities UA side on this
    //      context.
    //
    //    bSharedTrustedProxy:
    //      - true: this event is for a SetSharedTrustedProxy call.
    //
    //      - false: this event is for a SetInstanceTrustedProxy call.
    //
    //  Description:
    //      This method is called when the DNS resolution of the trusted proxy
    //      URI list is completed. It will be called for either the shared or
    //      instance trusted proxies as indicated by the bSharedTrustedProxy
    //      parameter.
    //
    //      Before this event is reported to the application, all other events
    //      of this manager SHOULD be considered unreliable. Once this
    //      event has been reported at least once for either the shared or
    //      instance trusted proxy list, all events are reliable.
    //
    //      This means that when the shared trusted proxies are set and the
    //      EvTrustedProxyDnsResolutionCompleted event has been reported once,
    //      changing the instance trusted proxy will NOT render the events
    //      unreliable. Those events could be reported while the DNS resolution
    //      for that new list is in progress and they will be based on the
    //      shared list until the DNS resolution is completed. 
    //
    //      The events reliability is based on the fact that there is at least
    //      one trusted proxy IP address list set. It does not matter if it is
    //      the shared or the instance one. As long as there is a trusted proxy
    //      IP address list set, the events are reliable according to that list.
    //
    //      The rule of thumb is: once the EvTrustedProxyDnsResolutionCompleted
    //      event has been reported, the service's events are reliable.
    //
    //==EDOC====================================================================
    virtual void EvTrustedProxyDnsResolutionCompleted(IN ISipUaAssertedIdentitySvc* pSvc,
                                                      IN bool bSharedTrustedProxy) = 0;
                                                      
//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipUaAssertedIdentityMgr(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipUaAssertedIdentityMgr(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipUaAssertedIdentityMgr(IN const ISipUaAssertedIdentityMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipUaAssertedIdentityMgr&
                            operator=(IN const ISipUaAssertedIdentityMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPUAASSERTEDIDENTITYMGR_H

