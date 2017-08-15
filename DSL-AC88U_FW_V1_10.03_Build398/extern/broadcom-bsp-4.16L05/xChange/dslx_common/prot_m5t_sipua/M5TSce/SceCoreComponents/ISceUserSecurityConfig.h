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
#ifndef MXG_ISCEUSERSECURITYCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSERSECURITYCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUserSecurityConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserSecurityConfig
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines various security related parameters that can be
//   applied to a user.
//
// Description:
//  This interface defines various security related parameters that can be
//  applied to a user and the components associated with it.
//
// Location:
//   SceCoreComponents/ISceUserSecurityConfig.h
//
// See Also:
//  ISceUserIdentityConfig
//
//==============================================================================
class ISceUserSecurityConfig : public IEComUnknown
{
//-- Friend Declarations


//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUserSecurityConfig);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  EnablePersistentTlsConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the use of a TLS persistent connection for this user.
    //
    //  Parameters:
    //    bEnable:
    //      - true: Enables persistent connections.
    //      - false: Disables persistent connections.
    //
    //  Description:
    //      This method enables or disables the establishment of a persistent
    //      connection with the outbound proxy server of the user.
    //
    //#What is a Persistent TLS Connection#
    //      The M5T SIP stack over which the Client Engine is built performs its
    //      own connection management. It performs connection reuse, it keeps
    //      connections open for a while, thus allowing reuse, it closes unused
    //      connections after a time, and can replace an old connection with a
    //      new one in order to preserve system resources.
    //
    //      At the M5T SIP stack level, a persistent connection is a connection
    //      that does not fall under the automatic control of the stack.
    //      Persistent connections cannot be closed by the stack if it sees that
    //      the connection was unused for a time, neither will it be closed if
    //      another connection must be created. In a sense, the persistent
    //      connections are kept outside of the connection pool that is
    //      automatically managed by the stack.
    //
    //      A persistent connection is established by the user of the stack (the
    //      Client Engine in this instance) and is terminated only when the
    //      stack user requests the connection to be terminated. The M5T SIP
    //      stack will periodically retry to establish a persistent connection
    //      that breaks due to network conditions or if the peer closes the
    //      connection. While in this state, the persistent connection mechanism
    //      will prevent standard connections establishment to the outbound
    //      proxy, until the persistent connection is re-established.
    //
    //#Why use Persistent TLS Connections#
    //      The principal use of persistent connections in the SIP Client Engine
    //      is to allow the communication with a SIP server over TLS.
    //
    //      As described in the SIP connecion reuse draft
    //      \<draft-ietf-sip-connect-reuse-xx.txt\>, SIP entities usually create
    //      two connections to talk to each other:
    //
    //<CODE>
    //     The SIP protocol includes mechanisms which insure that responses to a
    //     request reuse the existing connection which is typically still
    //     available, and also includes provisions for reusing existing
    //     connections for other requests sent by the originator of the
    //     connection.  However, new requests sent in the opposite direction --
    //     in the example above, requests from B destined to A -- are unlikely
    //     to reuse the existing connection.  This frequently causes a pair of
    //     SIP entities to use one connection for requests sent in each
    //     direction, as shown below.
    //
    //            +-----------+ 8293              5061 +-----------+
    //            |           |.......................>|           |
    //            |  Entity   |                        |  Entity   |
    //            |     A     | 5061              9741 |     B     |
    //            |           |<-----------------------|           |
    //            +-----------+                        +-----------+
    //
    //</CODE>
    //
    //      When using TCP, this does not create any perticular problem beyond
    //      the use of additional sockets. When using TLS however, a device is
    //      not able to initiate a connection to another device that does not
    //      have a personal certificate to present at the TLS handshake. Since
    //      most client devices do not have personal certificates, it is thus
    //      usually impossible for them to accept incoming TLS connections.
    //
    //      How do we make it possible for client devices without a personal
    //      certificate to receive a request from the server? The SIP working
    //      group is finishing the definition of the outbound mechanism defined
    //      in \<draft-ietf-sip-outbound-xx\>, which provides a solution to this
    //      problem. The connection reuse mechanism, which is being defined in
    //      \<draft-ietf-sip-connect-reuse-xx\>, also offers another solution
    //      to this problem. Until these solutions are finalized by the IETF,
    //      the proprietary persistent TLS connection mechanism allows a client
    //      device to establish a connection to a server and force the server
    //      to reuse this connection to send requests to the client.
    //
    //      The mechanism consists in creating a connection to the server from
    //      the local port that the client advertises as its listening port. By
    //      default, this is port 5061 for TLS. Thus, the client establishes a
    //      connection <I>from</I> port 5061 to port 5061 on the server.
    //
    //<CODE>
    //             +-----------+                        +-----------+
    //             |           |                        |           |
    //             | CLIENT    | 5061              5061 |  SERVER   |
    //             |           |----------------------->|           |
    //             |           |                        |           |
    //             +-----------+                        +-----------+
    //</CODE>
    //
    //      As soon as the client establishes the connection to the server, it
    //      is required to send a request to the server, allowing the server to
    //      challenge the request and authenticate the user of the connection.
    //      This is usually done by sending a REGISTER request, and is
    //      automatically taken care of by the Client Engine when the
    //      registration component is used.
    //
    //      Since the client does not usually have a personal certificate, it is
    //      not possible to perform mutual TLS authentication. When the client
    //      establishes the connection, the client is able to verify that the
    //      certificate presented by the server is trusted. However, since the
    //      client presents no certificate to the server, the server relies on
    //      the challenge of the first incoming SIP request to authenticate the
    //      user that created this connection.
    //
    //      When the server stores the information pertaining to this
    //      connection, it stores it as a connection to the client's IP address,
    //      port 5061. Now, if the server wants to send a request to the client,
    //      it will look to see if it has an exsiting connection to port 5061 on
    //      this client. Since it did establish its connection from 5061, the
    //      client-established connection is reused by the server. This
    //      effectively tricks the server into reusing the connection. There are
    //      however a few limitations and drawbacks to the use of persistent TLS
    //      connections.
    //
    //#Limitations Associated with Persistent TLS Connections#
    //      There are some potential limitations with persistent TLS
    //      connections:
    //
    //      - The server could possibly manage the connection it establishes in
    //        a pool separated from the connection it has accepted. This would
    //        possibly break the connection reuse trick described above.
    //
    //#Persistent TLS Connections in the SIP Client Engine#
    //      Persistent TLS Connections are offered in the following way in the
    //      SIP Client Engine.
    //
    //      - The persistent connection must be enabled before the
    //        ISceUserConfig related to this configuration is associated with
    //        any component.
    //
    //      - A persistent connection is established to the first host in the
    //        pre-loaded Route. Thus a pre-loaded route must be configured for
    //        the user. Furthermore, this first host address must be a "sips" 
    //        address.
    //
    //      - Updates to the pre-loaded Route are ignored once an initial
    //        persistent connection for a user has been established. This also
    //        means that an updated Service-Route is ignored after an initial
    //        persistent connection is established.
    //
    //      - The local port used to establish the connection is allocated among
    //        the system's ephemeral ports. This port will in turn be included in
    //        the Contact of the register request.
    //
    //      - If more than one user on the system share the same first hop in
    //        their pre-loaded route, the same persistent TLS connection will be
    //        used for both users.
    //
    //
    //==========================================================================
    virtual void EnablePersistentTlsConnection(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  IsPersistentTlsConnectionEnabled
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells whether or not the persistent connection is enabled.
    //
    //  Returns:
    //      - true: Persistent connection is enabled.
    //      - false: Persistent connection is disabled.
    //
    //  Description:
    //      This method enables the user to know the state of the persistent
    //      connection for this configuration.
    //
    //  See Also:
    //      EnablePersistentTlsConnection
    //
    //==========================================================================
    virtual bool IsPersistentTlsConnectionEnabled() const = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  SetPersistentConnectionLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the local address of the persistent connection.
    //
    //  Parameters:
    //    pLocalAddr:
    //      The local address of the persistent connection. Can be NULL, which
    //      means that the persistent connection is not established.
    //
    //  Description:
    //      This method is used by the CScePersistentConnectionList to inform
    //      the ISceUserSecurityConfig of the local address effectively used by
    //      the persistent connection. This is an internal method that should
    //      not be used by objects other than the CScePersistentConnectionList.
    //
    //  See Also:
    //      GetPersistentConnectionLocalAddress
    //
    //==========================================================================
    virtual void SetPersistentConnectionLocalAddress(IN const CSocketAddr* pLocalAddr) = 0;

    //==========================================================================
    //==
    //==  GetPersistentConnectionLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the local address of the persistent connection.
    //
    //  Returns:
    //      Returns the local address of the persistent connection. May be NULL,
    //      which means that the persistent connection is currently not
    //      connected either because it is disabled or in the process of being
    //      connected.
    //
    //  Description:
    //      Gets the local address of the persistent connection. This method is
    //      used by BuildContact to properly populate the contact address. The
    //      address returned by this method is set by the
    //      CScePersistentConnectionList through
    //      SetPersistentConnectionLocalAddress.
    //
    //  See Also:
    //      SetPersistentConnectionLocalAddress
    //
    //==========================================================================
    virtual const CSocketAddr* GetPersistentConnectionLocalAddress() const = 0;
//M5T_INTERNAL_USE_END

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //   The possible policies to apply to this user with regards to the usage
    //   of the "sips" URI.
    //
    // Description:
    //  This enumeration defines the possible policies to apply to this user
    //  with regards to the usage of the "sips" URI. The "sips" type of URI is
    //  used to indicate that the resource identified by the URI should be
    //  contacted over TLS. This enumeration is used with
    //  SetSignalingSecurityPolicy.
    //
    //==========================================================================
    enum ESigSecurityPolicy
    {
        // Description:
        // Forces the use of secure signaling for this user.
        //
        //  - All incoming requests for this user that are not identifying a
        //    SIPS request-URI are rejected with a 480 Response and a Warning
        //    header "381 SIPS Required". If the request specifies a SIPS
        //    request-URI but is received over TCP or UDP, the request is also
        //    rejected in the same way.
        //
        //  - All outgoing requests will always be addressed to a SIPS URI,
        //    thus ensuring that TLS is used for sending the request and
        //    requesting a secure transport for hops beyond the first one. All
        //    request-URIs and Route headers are promoted to the SIPS URI.
        //
        //  - If registering the user with the registration component, a SIPS
        //    Contact will be registered.
        //
        eSIGSEC_FORCE,

        // Description:
        // Asks for best-effort secure signaling for this user.
        //
        //  - All incoming requests are accepted, whether or not they are
        //    identifying the SIPS URI of this user.
        //
        //  - All outgoing requests will use the current setting for this
        //    request. For instance, an outgoing call to a SIPS URI will keep
        //    the request-URI as SIPS URI, while an outgoing call to a SIP URI
        //    will keep the request-URI as a SIP URI. The pre-loaded route is
        //    promoted to SIPS only if the resource to which send the request
        //    specifies a SIPS URI.
        //
        //  - If registering the user with the registration component, a SIPS
        //    Contact will be registered.
        eSIGSEC_BESTEFFORT,

        // Description:
        // Minimizes the use of secure signaling for this user.
        //
        //  - All incoming requests are accepted only if they specify the SIP
        //    URI of this user. If a request specifies the SIPS URI of the user,
        //    it is rejected with a 480 response and a Warning header "380 SIPS
        //    Not Allowed". Requests to SIP URI are accepted even if they are
        //    received over TLS.
        //
        //  - All outgoing requests are allowed only to SIP URIs. If the
        //    application asks for sending a request to a SIPS URI, it is
        //    prevented.
        //
        //  - If registering the user with the registration component, a SIP
        //    Contact will be registered.
        eSIGSEC_NONE
    };


    //==========================================================================
    //==
    //==  SetSignalingSecurityPolicy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the signaling security policy for this user.
    //
    //  Parameters:
    //    ePolicy:
    //      The policy to apply. See ESigSecurityPolicy for more information.
    //
    //  Description:
    //      Configures the signaling security policy to apply for this user.
    //
    //      The default policy if this method is not called is eSIGSEC_NONE.
    //
    //  See Also:
    //      ESigSecurityPolicy, GetSignalingSecurityPolicy
    //
    //==========================================================================
    virtual void SetSignalingSecurityPolicy(IN ESigSecurityPolicy ePolicy) = 0;

    //==========================================================================
    //==
    //==  GetSignalingSecurityPolicy
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the current signaling security policy for this user.
    //
    //  Returns:
    //      The current signaling security policy for this user.
    //
    //  Description:
    //      Returns the current signaling security policy for this user.
    //
    //      The default policy if the SetSignalingSecurityPolicy method has not
    //      been called is eSIGSEC_NONE.
    //
    //  See Also:
    //      ESigSecurityPolicy, SetSignalingSecurityPolicy
    //
    //==========================================================================
    virtual ESigSecurityPolicy GetSignalingSecurityPolicy() const = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceUserSecurityConfig() {}

    // Summary:
    //  Destructor.
    virtual ~ISceUserSecurityConfig() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceUserSecurityConfig(IN const ISceUserSecurityConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceUserSecurityConfig& operator=(IN const ISceUserSecurityConfig& rSrc);


//-- Hidden Data Members
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSERSECURITYCONFIG_H

