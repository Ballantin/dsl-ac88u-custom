//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPSWITCHBACKPERSISTENTCONNECTION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSWITCHBACKPERSISTENTCONNECTION_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#if !defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT) || \
    !defined(MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT \
and MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT \
to be able to use this class.
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSocketAddr;
class CString;
class ISipSwitchbackPersistentConnectionMgr;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipSwitchbackPersistentConnection);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipSwitchbackPersistentConnection
//==============================================================================
//<GROUP SipCoreSvc>
//
// Description:
//  This is the interface through which the application can establish persistent
//  connections that are managed according to the switchback feature for the
//  supplied FQDN/hostname.
//
//  Switchback allows having at least one persistent connection created based on
//  the DNS records of the supplied FQDN. The switchback always tries to connect
//  to the highest DNS record. If that record is not reachable or the persistent
//  connection associated with it is in error, it tries to connect to the next
//  highest record and so on until all available targets are tried. The goal is
//  to have backup servers when the primary server is not reachable.
//
//  When an error occurs on a persistent connection, the connection stays in an
//  error state until a certain timeout period occurs. This insures the
//  persistent connection will not reconnect until it goes out of the error
//  state. The timeout for which a connection stays in the error state is
//  configurable.
//
//  The switchback mechanism always performs a DNS request before connecting or
//  re-connecting a connection.
//
//  The switchback mechanism will try to reconnect to connections with higher
//  priority than currently established one but will not disconnect any
//  connection by itself. It is the responsibility of the manager, if it is
//  willing to, to disconnect connections that are no longer used.
//
//  If a DNS request yields records different from the ones that are already
//  cached, then the persistent connections for which there are no records are
//  automatically terminated if not connected or in error. If the connection is
//  connected then it stays up until the application closes it.
//
// Location:
//   SipCoreSvc/ISipSwitchbackPersistentConnection.h
//
//==============================================================================
class ISipSwitchbackPersistentConnection : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipSwitchbackPersistentConnection);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  SetErrorTimeoutMs
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the error timeout.
    //
    //  Parameters:
    //      uTimeoutMs:
    //       The timeout in milliseconds. MUST be higher than or equal to 5000 ms.
    //
    //  Returns:
    //      - resS_OK: Error timeout is set.
    //      - resFE_INVALID_ARGUMENT: uTimeoutMs is less than 5000 ms.
    //
    //  Description:
    //      Sets the error timeout used to determine how long connections in
    //      error are kept in the error state.
    //
    //      The timer is started on a connection error. When it fires it will
    //      reset the error state on all the connections.
    //
    //      Once the timer is started, a new error will not reset the timer. The
    //      timer will fire once the timeout value is reached. While the timer
    //      is running, the switchback mechanism will not attempt to reconnect
    //      any connection previously in error even if no more connections are
    //      available to connect to.
    //
    //      A connection can remain less than the configured timeout in the
    //      error state if it is not the first connection in error.
    //      One such example would be the following:
    //
    //      - Establish made to "domain.com" which resolves to two IP addresses
    //        (IP1 and IP2)
    //      - Connection attempted to IP1 fails (IP1 is put in error state)
    //      - Connection attempted to IP2 succeeds
    //      - An error occurs on the IP2 connection before the error timeout
    //        expires (IP2 is also put in an error state)
    //      - Error timeout expires: a new DNS request is made and now the
    //        order of the targets is switched (IP2 and IP1)
    //      - If connection to IP2 succeeds then it would have been in the
    //        error state for less than the configured time
    //
    //      The default value of the error timeout is 300 000 ms (5
    //      minutes).
    //
    //==========================================================================
    virtual mxt_result SetErrorTimeoutMs(IN unsigned int uTimeoutMs) = 0;

    //==========================================================================
    //==
    //==  Establish
    //==
    //==========================================================================
    //
    //  Summary:
    //      Establishes persistent connections to the supplied hostname.
    //
    //  Parameters:
    //      eTransport:
    //       The transport.
    //
    //      opqLocalAddr:
    //       The local address to use to establish the connection.
    //
    //      uLocalPort:
    //       The local port. It can be zero in which case a random port will be
    //       selected by the OS.
    //
    //      rstrPeerHost:
    //       The hostname/FQDN of the peer.
    //
    //      uPeerPort:
    //       The port of the peer. Can be zero in which case a default port is
    //       selected according to the RFC3263.
    //
    //      pMgr:
    //       The manager.
    //
    //       MUST NOT be NULL.
    //
    //  Returns:
    //      - resS_OK: Establish was successful and a DNS request is underway.
    //      - resFE_INVALID_ARGUMENT: The transport is invalid or the local
    //                                address is invalid or the manager is
    //                                NULL.
    //      - Other: Failure. It could be that Establish was already done and
    //                        there are persistent connections.
    //
    //  Description:
    //      Establishes persistent connections to the supplied hostname. Many
    //      connections can be created for that hostname as there could be one
    //      connection per target returned by the DNS.
    //
    //      An oqaque parameter identifies each persistent connection created by
    //      the switchback mechanism. This parameter is given to the application
    //      through the event EvConnectionCreated.
    //
    //      The result of the DNS request (the number of available targets) is
    //      reported to the application through EvDnsResult.
    //
    //      The stack MUST be started before calling Establish.
    //
    //  See Also:
    //      ISipCoreConfig::Startup,
    //      ISipSwitchbackPersistentConnectionMgr::EvConnectionCreated,.
    //      ISipSwitchbackPersistentConnectionMgr::EvDnsResult
    //
    //==========================================================================
    virtual mxt_result Establish(IN ESipTransport eTransport,
                                 IN mxt_opaque opqLocalAddr,
                                 IN uint16_t uLocalPort,
                                 IN const CString& rstrPeerHost,
                                 IN uint16_t uPeerPort,
                                 IN ISipSwitchbackPersistentConnectionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates the persistent connections.
    //
    //  Description:
    //      Terminates all the persistent connections. If a DNS request is
    //      underway, it waits until the DNS has completed before terminating.
    //
    //      This method could be called when the DNS request yields zero targets
    //      as the provided hostname could not be resolved. Calling Terminate
    //      allows the application to recall Establish with another hostname.
    //
    //      The ISipCoreConfig automatically calls Terminate when it is
    //      shut down. As such, it is not mandatory for the application to call
    //      this method.
    //
    //  See Also:
    //      ISipCoreConfig::ShutdownA,
    //      ISipSwitchbackPersistentConnectionMgr::EvDnsResult,
    //      Establish
    //
    //==========================================================================
    virtual void Terminate() = 0;

    //==========================================================================
    //==
    //==  Terminate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Terminates/closes a persistent connection.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Returns:
    //      - resS_OK: The connection is now terminating.
    //                 EvTerminated will be called synchronously.
    //      - resSW_SIPCORE_WAIT_ASYNC_RESPONSE: EvTerminated will be reported
    //                                           asynchronously.
    //      - Other: Failure. Connection not terminated, EvTerminated will not
    //               be called.
    //
    //  Description:
    //      Terminates a persistent connection.
    //
    //      Terminate MUST be done for each connection provided through the
    //      EvConnectionCreated event.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnectionMgr::EvTerminated.
    //
    //==========================================================================
    virtual mxt_result Terminate(IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  ConnectivityVerified
    //==
    //==========================================================================
    //
    //  Summary:
    //      Marks the connectivity of the persistent connection as verified.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Returns:
    //      - resS_OK: The connection was marked as verified.
    //      - resFE_INVALID_ARGUMENT: The supplied connection does not exist.
    //      - resFE_INVALID_STATE: EvEstablished has not been called.
    //
    //  Description:
    //      Allows the application to inform the switchback mechanism that the
    //      connectivity of a persistent connection has been verified. This
    //      method MUST be called before the switchback mechanism considers
    //      using the persistent connection as the default highest priority
    //      connection.
    //
    //      The application should verify the connectivity of a connection upon
    //      receiving the event EvEstablished. Verifying the connectivity can be
    //      done in many ways: either by issuing SIP requests such as OPTIONS or
    //      by using a keepalive mechanism on the persistent connection.
    //
    //      If the application sends a SIP request to verify the connectivity it
    //      should create an ISipContext, attach an ISipPersistentConnectionSvc
    //      on it and set the persistent connection opaque using
    //      SetConnectionOpaque.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnectionMgr::EvEstablished,.
    //      ISipPersistentConnectionSvc::SetConnectionOpaque.
    //
    //==========================================================================
    virtual mxt_result ConnectivityVerified(IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  GenerateError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates an error on the persistent connection.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Returns:
    //      - resS_OK: An error was generated on the connection.
    //      - resFE_INVALID_ARGUMENT: The supplied connection does not exist.
    //      - Other: Failure. The connection could not be disconnected. It could
    //               be because the connection was already disconnected.
    //
    //  Description:
    //      Generates an error on the persistent connection. This will
    //      disconnect the underlying persistent connection.
    //
    //      For example, the application can generate an error when a SIP
    //      request has timed out when sent over that persistent connection.
    //
    //      EvError will be reported with the error
    //      resFE_SIPCORESVC_PERSISTENT_CONNECTION_USER_DISCONNECTED.
    //
    //
    //  See Also:
    //      SetErrorTimeoutMs
    //      ISipSwitchbackPersistentConnectionMgr::EvError
    //
    //==========================================================================
    virtual mxt_result GenerateError(IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  IsVerified
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells if a persistent connection is verified or not.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Returns:
    //      True when the connection is established (connected) and verified.
    //      Otherwise false.
    //
    //  Description:
    //      Tells if a connection is verified. A connection is considered to be
    //      verified when it is established (EvEstablished has been called) and
    //      verified (ConnectivityVerified made).
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnectionMgr::EvEstablished,
    //      ConnectivityVerified
    //
    //==========================================================================
    virtual bool IsVerified(IN mxt_opaque opqConnection) const = 0;

    //==========================================================================
    //==
    //==  GetPriority
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the priority of the persistent connection.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //      ruPriority:
    //       The priority of the connection. 0 is the highest priority, 1 the
    //       second and so on. Only valid when resS_OK is returned.
    //
    //  Returns:
    //      - resS_OK: The priority was properly set.
    //      - resFE_INVALID_ARGUMENT: The supplied connection does not exist.
    //
    //  Description:
    //      Gets the priority of the connection.
    //
    //==========================================================================
    virtual mxt_result GetPriority(IN mxt_opaque opqConnection,
                                   OUT unsigned int& ruPriority) const = 0;

    //==========================================================================
    //==
    //==  GetConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the persistent connection attributes.
    //
    //  Parameters:
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //      reTransport:
    //       The transport. Only valid when resS_OK is returned.
    //
    //      rLocalAddr:
    //       The local address. Only valid when resS_OK is returned.
    //
    //      rPeerAddr:
    //       The remote address. Only valid when resS_OK is returned.
    //
    //  Returns:
    //      - resS_OK: The connection attributes were properly set.
    //      - resFE_INVALID_ARGUMENT: The supplied connection does not exist.
    //
    //  Description:
    //      Gets the connection transport, local and remote addresses.
    //
    ///==========================================================================
    virtual mxt_result GetConnection(IN mxt_opaque opqConnection,
                                     OUT ESipTransport& reTransport,
                                     OUT const CSocketAddr*& rpLocalAddr,
                                     OUT const CSocketAddr*& rpPeerAddr) const = 0;

    //==========================================================================
    //==
    //==  GetConnectionList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the switchback connection list.
    //
    //  Parameters:
    //      rvecopqConnection:
    //       The list of connection opaque identifier.
    //
    //  Description:
    //      Gets a vector that contains the connection opaque values managed by
    //      this object. The order of the connection opaque values is in
    //      ascending priority (top priority connection opaque is at index 0 in
    //      the vector).
    //
    ///==========================================================================
    virtual void GetConnectionList(OUT CVector<mxt_opaque>& rvecopqConnection) const = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  UpdatePacket
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the packet.
    //
    //  Parameters:
    //      rPacket:
    //       The SIP packet.
    //
    //      ropqConnection:
    //       The connection opaque identifier.
    //
    //  Returns:
    //      - resS_OK: The packet has been successfully updated.
    //      - resFE_SIPCORE_PACKET_BLOCKED: The packet transmission is blocked.
    //
    //  Description:
    //      Updates the packet local and remote addresses if a connection is
    //      available. If ropqConnection is not zero, then the specified
    //      connection is used if it is in an established (connected) state.
    //
    //      This method is internal to the stack, it SHOULD NOT be called by the
    //      application.
    //
    //==========================================================================
    virtual mxt_result UpdatePacket(INOUT CSipPacket& rPacket,
                                    INOUT mxt_opaque& ropqConnection) const = 0;
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    ISipSwitchbackPersistentConnection() {}
    virtual ~ISipSwitchbackPersistentConnection() {}
//M5T_INTERNAL_USE_END
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSWITCHBACKPERSISTENTCONNECTION_H
