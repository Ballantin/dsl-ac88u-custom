//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ISIPUAASSERTEDIDENTITYSVC_H
#define MXG_ISIPUAASSERTEDIDENTITYSVC_H

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
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif


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
class CNameAddr;
class CSipUri;
class ISipUaAssertedIdentityMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipUaAssertedIdentitySvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipUaAssertedIdentitySvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The ISipUaAssertedIdentitySvc service implements Network Asserted Identity
//  capabilities for a user agent.
//
//  This service allows the configuration of trusted proxies to which requests
//  should be sent and from which they should be received. It will report an
//  event when a request or a response is not received from a trusted proxy,
//  in which case the application should ignore the P-Asserted-Identity header
//  in the packet, if any. This service will also report an event when a request
//  or a response is received from a trusted proxy with a P-Asserted-Identity
//  header, in which case the application should use this asserted identity
//  information instead of the From header of the request, or the To header of
//  the response. Moreover, this service can be configured with a preferred
//  identity, which will be placed into outgoing requests and responses.
//
//  It is the application's responsibility to ensure that the packet is sent to
//  a trusted proxy when P-Preferred-Identity header(s) are added to the packet.
//  Doing otherwise may compromise privacy. RFC 3325 states that "user agents
//  MUST NOT populate the P-Preferred-Identity header field in a message that is
//  not sent directly to a proxy that is trusted by the user agent."
//
//  To ensure that requests are sent to the trusted proxy server, the
//  service user must configure a pre-loaded route, with the address of the
//  trusted proxy as the first item in this pre-loaded route. If a response
//  is received from another entity than one of the configured trusted proxies,
//  the service will report the EvUntrustedProxy event, which means that the
//  P-Asserted-Identity header that may be contained in the packet MUST NOT be
//  used.
//
//  When an incoming request is not received from the trusted proxies,
//  EvUntrustedProxy is reported to the manager. This event means that the
//  service user must not consider the P-Asserted-Identity header found in this
//  request.
//
//  The service allows the configuration of two identities (one SIP or SIPS URI
//  and/or one telephone URI) to be added into packets as P-Preferred-Identity
//  headers. These headers may be used by the trusted proxy to select the
//  Network Asserted Identity to specify in the P-Asserted-Identity header it
//  will generate. Note that nothing guaranties that the preferred identity will
//  be used by the proxy. If the preferred identity is not known by the proxy,
//  it can use its known Network Asserted Identity or it can reject the request
//  with a 403 Forbidden.
//
//  When packets are received from a trusted proxy with the P-Asserted-Identity
//  header, the service will warn the application through the
//  ISipUaAssertedIdentityMgr interface. If the use of the P-Asserted-Identity
//  is invalid (too many headers or invalid headers), the application will be
//  notified by one of the EvInvalidAssertedIdentity events (depending on
//  whether the packet is a request or a response). Otherwise, one of the
//  EvAssertedIdentity events will be reported.
//
//  To enable Network Asserted Identity privacy, use the ISipPrivacySvc. This
//  service allows to request for Network Asserted Identity privacy, which
//  mandates the SIP elements in the Trust Domain to keep the user's information
//  confidential.
//
//  This service notifies the application of events through the
//  ISipUaAssertedIdentityMgr interface.
//
//#The ISipUaAssertedIdentitySvc is an ECOM object#
//
//  The ISipUaAssertedIdentitySvc is an ECOM object that is created and accessed
//  through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipUaAssertedIdentitySvc</B>
//
//  <B>Interface Id: IID_ISipUaAssertedIdentitySvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgent/ISipUaAssertedIdentitySvc.h
//
// See Also:
//   ISipUaAssertedIdentityMgr, ISipUserAgentSvc::SetPreloadedRoute
//
//==EDOC========================================================================
class ISipUaAssertedIdentitySvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipUaAssertedIdentitySvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------


    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the manager associated with this service instance.
    //
    //  Parameters:
    //    pMgr:
    //      The manager. It must not be NULL.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT:
    //          pMgr is NULL. The manager is not changed in this case.
    //
    //      resS_OK:
    //          Otherwise.
    //
    //  Description:
    //      Configures the manager that will receive the events reported by
    //      this service.
    //
    //      Note that a manager MUST be associated with this service before it
    //      is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager( IN ISipUaAssertedIdentityMgr* pMgr ) = 0;


    //==SDOC====================================================================
    //==
    //==  SetSharedTrustedProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address of the trusted proxy server that will be used by
    //      all instances of this service.
    //
    //  Parameters:
    //    rTrustedProxy:
    //      The SIP / SIPS URI of the trusted proxy.
    //
    //  Returns:
    //    - resFE_INVALID_STATE: This service has no manager. Maybe it was not
    //                           set or Clear was called on the context.
    //
    //                           OR
    //
    //                           This service already has its own instance list
    //                           so modifying the shared list would not affect
    //                           the current service behavior. Updating was not
    //                           done.
    //
    //                           OR
    //
    //                           The trusted proxy IP address list is currently
    //                           used. The main cause for that would be that
    //                           DNS resolution is currently processed for the
    //                           service list.
    //
    //    - resS_OK: DNS resolution has begun for the trusted proxy URI passed
    //               to the method.
    //
    //  Description:
    //      Sets the address of the trusted proxy. It is through this proxy
    //      that the user's identity is asserted.
    //
    //      This method configures all existing and future instances of this
    //      service with rTrustedProxy, but does not affect the instances that
    //      had their trusted proxy address configured through
    //      SetInstanceTrustedProxy.
    //
    //      If the system only uses a single trusted proxy for all Sip Context,
    //      you should use SetSharedTrustedProxy once in the system as DNS
    //      resolution for the server will be done only once. If the system
    //      simultaneously uses more than one trusted proxy server, you will
    //      have to use SetInstanceTrustedProxy each time this service is
    //      attached to a Sip Context.
    //
    //      This method will resolve the SIP-URI / SIPS-URI to one or more IP
    //      addresses.
    //
    //      If a request is received from another proxy than the trusted proxy,
    //      the service will generate the EvUntrustedProxy event on its
    //      manager.
    //
    //      A pre-loaded route SHOULD be set by the application, with the first
    //      route pointing to the trusted proxy server. This is to guarantee
    //      that requests are always sent first to the trusted proxy server,
    //      which can then verify the user's identity before forwarding the
    //      request.
    //
    //  Note:
    //      This method is kept for backward compatibility reasons. The overload
    //      that receives a <CODE>CVector<CSipUri></CODE> parameter SHOULD be 
    //      used instead.
    //
    //  See Also:
    //      SetInstanceTrustedProxy
    //
    //==EDOC====================================================================
    virtual mxt_result SetSharedTrustedProxy(
                                        IN const CSipUri& rTrustedProxy) = 0;
                                        
    //==SDOC====================================================================
    //==
    //==  SetSharedTrustedProxy
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Sets the address of the trusted proxy servers that will be used by
    //      all instances of this service.
    //
    //  Parameters:
    //    rvecTrustedProxies:
    //      The SIP / SIPS URIs of the trusted proxies.
    //
    //  Returns:
    //    - resFE_INVALID_STATE: This service has no manager. Maybe it was not
    //                           set or Clear was called on the context.
    //
    //                           OR
    //
    //                           This service already has its own instance list
    //                           so modifying the shared list would not affect
    //                           the current service behaviour. Updating was not
    //                           done.
    //
    //                           OR
    //
    //                           The trusted proxy IP address list is currently
    //                           used. The main cause for that would be that
    //                           DNS resolution is currently processed for the
    //                           service list.
    //
    //   - resS_OK: DNS resolution has begun for the list of trusted proxy URI
    //              passed to the method.
    //
    //  Description:
    //      Sets the address of the trusted proxy servers. It is through these
    //      proxies that the user's identity is asserted.
    //
    //      This method configures all existing and future instances of this
    //      service with rvecTrustedProxies, but does not affect the instances
    //      that had their trusted proxy addresses configured through
    //      SetInstanceTrustedProxy.
    //
    //      If the system only uses a single trusted proxy list for all Sip
    //      Contexts, you should use SetSharedTrustedProxy once in the system as
    //      DNS resolution for the servers will be done only once. If the system
    //      simultaneously uses more than one trusted proxy server lists, you
    //      will need to use SetInstanceTrustedProxy each time this service is
    //      attached to a Sip Context.
    //
    //      This method will resolve the SIP-URI / SIPS-URI list to one or more
    //      IP addresses for each URI.
    //
    //      If a request is received from another proxy than one of the trusted
    //      proxies, the service will generate the EvUntrustedProxy event on its
    //      manager.
    //
    //      A pre-loaded route SHOULD be set by the application, with the first
    //      route pointing to one of the trusted proxy servers. This is to
    //      guarantee that requests are always sent first to a trusted proxy
    //      server, which can then verify the user's identity before forwarding
    //      the request.
    //
    //  Warning:
    //      The vector must not be empty or the shared trusted proxy list will 
    //      be erased for all current and future instances.
    //
    //  See Also:
    //      SetInstanceTrustedProxy
    //
    //==EDOC====================================================================
    virtual mxt_result SetSharedTrustedProxy(IN const CVector<CSipUri>& rvecTrustedProxies) = 0;                                        

    
    //==SDOC====================================================================
    //==
    //==  SetInstanceTrustedProxy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the address of the trusted proxy server that will be used by
    //      this instance of this service.
    //
    //  Parameters:
    //    rTrustedProxy:
    //      The SIP / SIPS URI of the trusted proxy.
    //
    //  Returns:
    //    - resFE_INVALID_STATE: This service has no manager. Maybe it was not
    //                           set or Clear was called on the context.
    //
    //                           OR
    //
    //                           The trusted proxy IP address list is currently
    //                           used. The main cause for that would be that DNS
    //                           resolution is currently processed for the
    //                           service list.
    //
    //    - resS_OK: DNS resolution has begun for the trusted proxy URI passed
    //               to the method.
    //
    //  Description:
    //      Sets the address of the trusted proxy. It is through this proxy
    //      that the user's identity is asserted.
    //
    //      This method overrides, for this instance only, all previous and
    //      future configuration done through SetSharedTrustedProxy. That is,
    //      this service will always use its instance configuration first, and
    //      if none is set, then it will use the shared configuration.
    //
    //      If the system only uses a single trusted proxy for all Sip Contexts,
    //      you should use SetSharedTrustedProxy once in the system as DNS
    //      resolution for the server will be done only once. If the system
    //      simultaneously uses more than one trusted proxy server, you will
    //      need to call SetInstanceTrustedProxy each time this service is
    //      attached to a Sip Context.
    //
    //      This method will resolve the SIP-URI / SIPS-URI to one or more IP
    //      addresses.
    //
    //      If a request is received from another proxy than the trusted proxy,
    //      the service will generate the EvUntrustedProxy event on its
    //      manager.
    //
    //      A pre-loaded route SHOULD be set by the application, with the first
    //      route pointing to the trusted proxy server. This is to guarantee
    //      that requests are always sent first to the trusted proxy server,
    //      which can then verify the user's identity before forwarding the
    //      request.
    //
    //  Note:
    //      This method is kept for backward compatibility reasons. The overload
    //      that receives a <CODE>CVector<CSipUri></CODE> parameter SHOULD be
    //      used instead.
    //
    //  See Also:
    //      SetSharedTrustedProxy
    //
    //==EDOC====================================================================
    virtual mxt_result SetInstanceTrustedProxy(
                                        IN const CSipUri& rTrustedProxy) = 0;
                                        
    //==SDOC====================================================================
    //==
    //==  SetInstanceTrustedProxy
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Sets the address of the trusted proxy servers that will be used by
    //      this instance of this service.
    //
    //  Parameters:
    //    rvecTrustedProxies:
    //      The SIP / SIPS URI of the trusted proxies.
    //
    //  Returns:
    //    - resFE_INVALID_STATE: This service has no manager. Maybe it was not
    //                           set or Clear was called on the context.
    //
    //                           OR
    //
    //                           The trusted proxy IP address list is currently
    //                           used. The main cause for that would be that DNS
    //                           resolution is currently processed for the
    //                           service list.
    //
    //    - resS_OK: DNS resolution has begun for the list of trusted proxy URI
    //               passed to the method.
    //
    //  Description:
    //      Sets the address of the trusted proxy servers. It is through these
    //      proxies that the user's identity is asserted.
    //
    //      This method overrides, for this instance only, all previous and
    //      future configuration done through SetSharedTrustedProxy. That is,
    //      this service will always use its instance configuration first, and
    //      if none is set, then it will use the shared configuration.
    //
    //      If the system only uses a single trusted proxy list for all Sip
    //      Contexts, you should use SetSharedTrustedProxy once in the system as
    //      DNS resolution for the server will be done only once. If the system
    //      simultaneously uses more than one trusted proxy server list, you
    //      will need to call SetInstanceTrustedProxy each time this service is
    //      attached to a Sip Context.
    //
    //      This method will resolve the SIP-URI / SIPS-URI list to one or more
    //      IP addresses for each URI.
    //
    //      If a request is received from another proxy than one of the trusted
    //      proxies, the service will generate the EvUntrustedProxy event on its
    //      manager.
    //
    //      A pre-loaded route SHOULD be set by the application, with the first
    //      route pointing to one of the trusted proxy server. This is to
    //      guarantee that requests are always sent first to a trusted proxy
    //      server, which can then verify the user's identity before forwarding
    //      the request.
    //
    //  See Also:
    //      SetSharedTrustedProxy
    //
    //==EDOC====================================================================
    virtual mxt_result SetInstanceTrustedProxy(IN const CVector<CSipUri>& rvecTrustedProxies) = 0;                                        


    //==SDOC====================================================================
    //==
    //==  SetPreferredIdentities
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the identities added to the P-Preferred-Identity of the 
    //      outgoing requests.
    //
    //  Parameters:
    //    pSipIdentity:
    //      The SIP or SIPS URI identity to be added in a P-Preferred-Identity 
    //      header.
    //
    //      NULL if no SIP or SIPS URI identity should be sent in a
    //      P-Preferred-Identity header.
    //
    //      By default, this parameter is NULL.
    //
    //    pTelIdentity:
    //      The telephone URI identity to be added in a P-Preferred-Identity 
    //      header.
    //
    //      NULL if no telephone URI identity should be sent in a
    //      P-Preferred-Identity header.
    //
    //      By default, this parameter is NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      The manager is NULL. This could happen when the manager was not set
    //      or when Clear was called on the ISipContext.
    //
    //    resFE_INVALID_ARGUMENT:
    //      pSipIdentity is not NULL or does not contain a SIP or SIPS URI.
    //
    //    resFE_INVALID_ARGUMENT:
    //      pTelIdentity is not NULL or does not contain a telephone URI.
    //
    //    resS_OK:
    //      The configuration has been updated.
    //
    //  Description:
    //      Permits to configure the identities that the application prefers to
    //      be used as their Network Asserted Identity. These identities are
    //      added to P-Preferred-Identity headers.
    //
    //      To be RFC 3325 compliant, this method only permits to add a maximum
    //      of 2 P-Preferred-Identity headers: one with a SIP or SIPS URI and
    //      one with a telephone URI.
    //
    //      By default, no P-Preferred-Identity header is added to the outgoing
    //      packets.
    //
    //      Because this service must be used to manage P-Preferred-Identity
    //      headers, it removes all P-Preferred-Identity headers it finds in
    //      outgoing packets and then adds the one it creates following its
    //      configuration.
    //
    //      RFC 3325 states that a "user agents MUST NOT populate the
    //      P-Preferred-Identity header field in a message that is not sent
    //      directly to a proxy that is trusted by the user agent." It is the
    //      application's responsibility to properly route the requests directly
    //      to a trusted proxy.
    //
    //      To ask for privacy on the Network Asserted Identity, see
    //      ISipPrivacySvc::SetPrivacyType.
    //
    //==EDOC====================================================================
    virtual
    mxt_result SetPreferredIdentities( IN const CNameAddr* pSipIdentity,
                                       IN const CNameAddr* pTelIdentity ) = 0;

    //==============================================================================
    //==
    //==  IgnoreUnknownUri
    //==
    //==============================================================================
    //
    //  Summary:
    //      Ignores unknown URI in P-Asserted-Identity header.
    //
    //  Parameters:
    //      bEnable:
    //       true to ignore unknown URI, false otherwise.
    //
    //  Description:
    //      By default, unknown URI are refused as per RFC3325. Only SIP, SIPS and
    //      TEL uri are accepted. The RFC5876 updates this behavior. When enabled,
    //      the service ignores any URI with an unexpected URI scheme.
    //
    //==============================================================================
    virtual void IgnoreUnknownUri(IN bool bEnable) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipUaAssertedIdentitySvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipUaAssertedIdentitySvc(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipUaAssertedIdentitySvc(IN const ISipUaAssertedIdentitySvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipUaAssertedIdentitySvc& operator=(IN const ISipUaAssertedIdentitySvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPUAASSERTEDIDENTITYSVC_H

