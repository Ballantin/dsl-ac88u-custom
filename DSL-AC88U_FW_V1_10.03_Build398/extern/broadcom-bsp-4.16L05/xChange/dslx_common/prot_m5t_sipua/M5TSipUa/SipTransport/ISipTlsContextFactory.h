//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_ISIPTLSCONTEXTFACTORY_H
#define MXG_ISIPTLSCONTEXTFACTORY_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CTlsContext;
class CString;
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipTlsContextFactory);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTlsContextFactory
//==============================================================================
// <GROUP SIPTRANSPORT_CLASSES>
// <FLAG New in 4.1.4>
//
// Description:
//
//   This class represents a factory for the client and server TLS contexts. It
//   keeps a default client and a default server context. These default
//   contexts are used by the SIP stack when there are no contexts that map to
//   a host name or an IP address. This class also keeps a list of client
//   contexts grouped by peer host name and a list of server contexts grouped
//   by local listening addresses.
//
//   The following are the responsibilities of the application when configuring
//   TLS contexts:
//
//   - Set a default client TLS context with authentication enabled, the
//     trusted certificates, and possibly the chain containing a personal
//     certificate. The authentication must be enabled on client connections
//     because this is the only way to identify the remote peer. The personal
//     certificate is not mandatory for User-Agents as they usually do not
//     possess such certificate.
//
//   - Could also optionally set a client TLS context per remote peer host name.
//
//   - Set a default server TLS context with, most of time, no authentication
//     and a personal certificate in the chain.
//
//   - Set a list of server TLS contexts associated with a local listening
//     address and asking for mutual authentication. When an application wants
//     to do mutual authentication on TLS server connections, it needs to set a
//     specific context per local listening address. In order for a server
//     socket to authenticate a remote peer, the server MUST know the list of
//     clients it is allowed to accept connections from. This is done by
//     specifying a list of Subject Alternate Names that are to be accepted.
//     Note that when the TLS peer authentication is not used on incoming
//     server connections, the application SHOULD challenge the request by
//     using the Digest Server Authentication to identify the peer.
//
//   The class <C>CTlsContext</C> allows to set all information required by
//   client and server contexts.
//
//   The ISipTlsContextFactory is an ECOM interface obtained through the
//   ISipCoreConfig::GetTlsContextFactory method. Hence, it is reference
//   counted.
//
// Location:
//   SipTransport/ISipTlsContextFactory.h
//
// See Also:
//   CTlsContext, ISipCoreConfig
//
//==============================================================================
class ISipTlsContextFactory : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipTlsContextFactory);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetDefaultTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default TLS client context.
    //
    //  Parameters:
    //    rTlsContext:
    //      The default TLS client context.
    //
    //  Returns:
    //      - resS_OK: The default TLS server context is returned in the
    //                 rTlsContext parameter.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Sets the default TLS client context. This is the
    //      generic context that applies to an outgoing TLS connection request
    //      when there is no specific context for a remote peer host name.
    //      If there are existing TLS connections that are using the default
    //      TLS client context, these connections will be modified with the new
    //      context. It is then up to the application to initiate a
    //      renegotiation if desired.
    //
    //  See Also:
    //      GetDefaultTlsClientContextS
    //
    //==========================================================================
    virtual mxt_result SetDefaultTlsClientContextS(IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  SetDefautlTlsServerContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default TLS server context.
    //
    //  Parameters:
    //    rTlsContext:
    //      The default TLS server context.
    //
    //  Returns:
    //      - resS_OK: The default TLS server context is returned in the
    //                 rTlsContext parameter.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Sets the default TLS server context. This is the
    //      generic context that applies to an incoming TLS connection request
    //      when there is no specific context for a local listening address.
    //      If there are existing TLS connections that are using the default
    //      TLS server context, these connections will be modified with the new
    //      context. It is then up to the application to initiate a
    //      renegotiation if desired.
    //
    //  See Also:
    //      GetDefaultTlsServerContextS
    //
    //==========================================================================
    virtual mxt_result SetDefaultTlsServerContextS(IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  AddTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a TLS client context identified by the remote peer host name.
    //
    //  Parameters:
    //    rstrHostname:
    //      The remote peer host name.
    //    rTlsContext:
    //      The TLS client context.
    //
    //  Returns:
    //      - resS_OK: The TLS client context is properly added.
    //      - resFE_INVALID_STATE: A TLS client context already exists for the
    //                             specified host name. Use
    //                             UpdateTlsClientContextS to modify the TLS
    //                             client context for that host name.
    //      - resFE_FAIL: Unable to process message.
    //
    //  Description:
    //      Adds a TLS client context in the map with the remote peer host name
    //      as the key.
    //      If there are existing client TLS connections to the specified
    //      host name, these connections will be modified with the new context.
    //      It is then up to the application to initiate a renegotiation if
    //      desired.
    //
    //  See Also:
    //      UpdateTlsClientContextS, RemoveTlsClientContextS,
    //      GetTlsClientContextS.
    //
    //==========================================================================
    virtual mxt_result AddTlsClientContextS(IN const CString& rstrHostname,
                                            IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  AddTlsServerContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a TLS server context identified by local listening address.
    //
    //  Parameters:
    //    rLocalAddr:
    //      The local listening address.
    //    rTlsContext:
    //      The TLS server context.
    //
    //  Returns:
    //      - resS_OK: The TLS server context is properly added.
    //      - resFE_INVALID_STATE: A TLS server context already exists for the
    //                             specified local listening address. Use
    //                             UpdateTlsClientContextS to modify the
    //                             TLS server context for that local listening
    //                             address.
    //      - resFE_FAIL: Unable to process message.
    //
    //  Description:
    //      Adds a TLS server context in the map with the local listening
    //      address as the key.
    //      If there are existing accepted TLS connections using the specified
    //      local listening address, these connections will be modified with
    //      the new context. It is then up to the application to initiate a
    //      renegotiation if desired.
    //
    //  See Also:
    //      UpdateTlsServerContextS, RemoveTlsServerContextS,
    //      GetTlsServerContextS.
    //
    //==========================================================================
    virtual mxt_result AddTlsServerContextS(IN const CSocketAddr& rLocalAddr,
                                            IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  UpdateTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the TLS client context identified by the remote peer host
    //      name.
    //
    //  Parameters:
    //    rstrHostname:
    //      The remote peer host name.
    //    rTlsContext:
    //      The TLS client context.
    //
    //  Returns:
    //      - resS_OK: The TLS client context is properly updated.
    //      - resFE_INVALID_STATE: No TLS client context exists for the
    //                             specified host name. Use
    //                             AddTlsClientContextS to add a TLS client
    //                             context for a new host name.
    //      - resFE_FAIL: Unable to process message.
    //
    //  Description:
    //      Updates the TLS client context corresponding to the remote peer host
    //      name.
    //      If there are existing client TLS connections to the specified
    //      host name, these connections will be modified with the new context.
    //      It is then up to the application to initiate a renegotiation if
    //      desired.
    //
    //  See Also:
    //      AddTlsClientContextS, RemoveTlsClientContextS, GetTlsClientContextS
    //
    //==========================================================================
    virtual mxt_result UpdateTlsClientContextS(IN const CString& rstrHostname,
                                               IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  UpdateTlsServerContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the TLS server context identified by the remote peer socket
    //      address.
    //
    //  Parameters:
    //    rLocalAddr:
    //      The local listening address.
    //    rTlsContext:
    //      The TLS server context.
    //
    //  Returns:
    //      - resS_OK: The TLS server context is properly updated.
    //      - resFE_INVALID_STATE: No TLS server context exists for the
    //                             specified socket address. Use
    //                             AddTlsServerContextS to add a TLS
    //                             server context for a new socket address.
    //      - resFE_FAIL: unable to process message.
    //
    //  Description:
    //      Updates the TLS server context corresponding to the local listening
    //      address.
    //      If there are existing accepted TLS connections using the specified
    //      local listening address, these connections will be modified with
    //      the new context. It is then up to the application to initiate a
    //      renegotiation if desired.
    //
    //  See Also:
    //      AddTlsServerContextS, RemoveTlsServerContextS, GetTlsServerContextS
    //
    //==========================================================================
    virtual mxt_result UpdateTlsServerContextS(IN const CSocketAddr& rLocalAddr,
                                               IN const CTlsContext& rTlsContext) = 0;

    //==========================================================================
    //==
    //==  RemoveTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the TLS client context identified by the remote peer host
    //      name.
    //
    //  Parameters:
    //    rstrHostname:
    //      The remote peer host name.
    //
    //  Returns:
    //      - resS_OK: The TLS client context is properly removed.
    //      - resFE_INVALID_STATE: No TLS client context exists for the
    //                             specified host name.
    //      - resFE_FAIL: Unable to process message.
    //
    //  Description:
    //      Removes the TLS client context corresponding to the remote peer host
    //      name.
    //      If there are existing client TLS connections to the specified
    //      host name, these connections will be modified to use the default
    //      client context. It is then up to the application to initiate a
    //      renegotiation if desired.
    //
    //  See Also:
    //      AddTlsClientContextS, UpdateTlsClientContextS, GetTlsClientContextS
    //
    //==========================================================================
    virtual mxt_result RemoveTlsClientContextS(IN const CString& rstrHostname) = 0;

    //==========================================================================
    //==
    //==  RemoveTlsServerContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the TLS server context identified by the remote peer socket
    //      address.
    //
    //  Parameters:
    //    rLocalAddr:
    //      The local listening address.
    //
    //  Returns:
    //      - resS_OK: The TLS server context is properly removed.
    //      - resFE_INVALID_STATE: No TLS server context exists for the
    //                             specified local listening address.
    //      - resFE_FAIL: Unable to process message.
    //
    //  Description:
    //      Removes the TLS server context corresponding to the remote peer
    //      socket address.
    //      If there are existing accepted TLS connections using the specified
    //      local listening address, these connections will be modified to use
    //      the default server context. It is then up to the application to
    //      initiate a renegotiation if desired.
    //
    //  See Also:
    //      AddTlsServerContextS, UpdateTlsServerContextS, GetTlsServerContextS
    //
    //==========================================================================
    virtual mxt_result RemoveTlsServerContextS(IN const CSocketAddr& rLocalAddr) = 0;

    //==========================================================================
    //==
    //==  GetDefaultTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the default TLS client context.
    //
    //  Parameters:
    //    rTlsContext:
    //      The default TLS client context.
    //
    //  Returns:
    //      - resS_OK: The default TLS client context is returned in the
    //                 rTlsContext parameter.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Gets the default TLS client context set by the application. This is
    //      the generic context that applies to an outgoing TLS connection
    //      request when there is no specific context for a remote peer host
    //      name.
    //
    //  See Also:
    //      SetDefaultTlsClientContextS
    //
    //==========================================================================
    virtual mxt_result GetDefaultTlsClientContextS(OUT CTlsContext& rTlsContext) const = 0;

    //==========================================================================
    //==
    //==  GetDefaultTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the default TLS server context.
    //
    //  Parameters:
    //    rTlsContext:
    //      The default TLS server context.
    //
    //  Returns:
    //      - resS_OK: The default TLS server context is returned in the
    //                 rTlsContext parameter.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Gets the default TLS server context set by the application. This is
    //      the generic context that applies to an incoming TLS connection
    //      request when there is no specific context for the local listening
    //      address.
    //
    //  See Also:
    //      SetDefaultTlsServerContextS
    //
    //==========================================================================
    virtual mxt_result GetDefaultTlsServerContextS(OUT CTlsContext& rTlsContext) const = 0;

    //==========================================================================
    //==
    //==  GetTlsClientContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the TLS client context corresponding to the remote peer host
    //      name.
    //
    //  Parameters:
    //    rstrHostname:
    //      The remote peer host name.
    //    rTlsContext:
    //      The TLS client context.
    //
    //  Returns:
    //      - resS_OK: The TLS client context has been found and returned in
    //                 rTlsContext.
    //      - resFE_INVALID_STATE: No TLS client context has been found for the
    //                             specified remote peer host name.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Gets the TLS client context corresponding to the remote peer host
    //      name.
    //
    //  See Also:
    //      AddTlsClientContextS, UpdateTlsClientContextS,
    //      RemoveTlsClientContextS.
    //
    //==========================================================================
    virtual mxt_result GetTlsClientContextS(IN const CString& rstrHostname,
                                            OUT CTlsContext& rTlsContext) const = 0;

    //==========================================================================
    //==
    //==  GetTlsServerContextS
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the TLS server context corresponding to the local listening
    //      address.
    //
    //  Parameters:
    //    rLocalAddr:
    //      The local listening address.
    //    rTlsContext:
    //      The TLS server context.
    //
    //  Returns:
    //      - resS_OK: The TLS server context has been found and returned in
    //                 rTlsContext.
    //      - resFE_INVALID_STATE: No TLS server context has been found for the
    //                             specified local listening address.
    //      - resFE_FAIL: Unable to process the message.
    //
    //  Description:
    //      Gets the TLS server context corresponding to the local listening
    //      address.
    //
    //  See Also:
    //      AddTlsServerContextS, UpdateTlsServerContextS,
    //      RemoveTlsServerContextS.
    //
    //==========================================================================
    virtual mxt_result GetTlsServerContextS(IN const CSocketAddr& rAddr,
                                            OUT CTlsContext& rTlsContext) const = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTlsContextFactory(){}

    // Summary:
    //  Destructor.
    virtual ~ISipTlsContextFactory(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTlsContextFactory(IN const ISipTlsContextFactory& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTlsContextFactory& operator=(IN const ISipTlsContextFactory& rSrc);
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTLSCONTEXTFACTORY_H
