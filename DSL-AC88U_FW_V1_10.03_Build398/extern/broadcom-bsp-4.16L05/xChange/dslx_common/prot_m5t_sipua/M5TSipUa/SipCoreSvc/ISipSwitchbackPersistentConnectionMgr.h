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
#ifndef MXG_ISIPSWITCHBACKPERSISTENTCONNECTIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPSWITCHBACKPERSISTENTCONNECTIONMGR_H
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

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipSwitchbackPersistentConnection;
class CSipPacket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipSwitchbackPersistentConnectionMgr
//==============================================================================
//<GROUP SipCoreSvc>
//
// Summary:
//   This is the manager of the switchback mechanism.
//
// Description:
//   This is the manager through which the stack reports persistent connection
//   events. This manager is mandatory to be set, it is specified when the
//   application calls the Establish method to setup persistent connections for
//   the user-supplied hostname.
//
// Location:
//   SipCoreSvc/ISipSwitchbackPersistentConnectionMgr.h
//
// See Also:
//   ISipSwitchbackPersistentConnection
//
//==============================================================================
class ISipSwitchbackPersistentConnectionMgr
{
//-- Friend Declarations

//-- New types within class namespace
public:

    // Description:
    //  Contains the data for a resolved address.
    struct SResolvedTarget
    {
        // Description:
        //  The hostname that was resolved.
        CString m_strHostName;

        // Description:
        //  All the addresses that m_strHostName resolved to.
        CVector<CSocketAddr> m_vecResolvedAddresses;
    };

protected:
private:

//-- Published Interface
public:
    //-- << Stereotype >>

    //==========================================================================
    //==
    //==  EvDnsResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells the number of targets returned by the DNS.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //       CANNOT be NULL.
    //
    //      eTransport:
    //       The transport given in
    //       ISipSwitchbackPersistentConnection::Establish.
    //
    //      opqLocalAddr:
    //       The local address given in
    //       ISipSwitchbackPersistentConnection::Establish. This opaque is the
    //       one returned by ISipCoreConfig::AddLocalAddress.
    //
    //      uLocalPort:
    //       The local port given in
    //       ISipSwitchbackPersistentConnection::Establish.
    //
    //      rstrPeerHost:
    //       The hostname/FQDN of the peer given in
    //       ISipSwitchbackPersistentConnection::Establish.
    //
    //      uPeerPort:
    //       The port of the peer given in
    //       ISipSwitchbackPersistentConnection::Establish.
    //
    //      rvecstTargets:
    //       The resolved targets that were given for rstrPeerHost.
    //
    //  Description:
    //      This gives the result of a resolving.
    //
    //      If no targets are returned, the FQDN cannot be resolved or there are
    //      entries missing on the DNS server as it could be misconfigured.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::Establish,
    //      ISipSwitchbackPersistentConnection::Terminate
    //
    //==========================================================================
    virtual void EvDnsResult(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                             IN ESipTransport eTransport,
                             IN mxt_opaque opqLocalAddr,
                             IN uint16_t uLocalPort,
                             IN const CString& rstrPeerHost,
                             IN uint16_t uPeerPort,
                             IN const CVector<SResolvedTarget>& rvecstTargets) = 0;

    //==========================================================================
    //==
    //==  EvConnectionCreated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that a persistent connection has been created.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //       CANNOT be NULL.
    //
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Description:
    //      This event is called when a persistent connection is created. It is
    //      created when the stack attempts to connect to it the first time.
    //
    //      Every connection on which EvConnectionCreated is reported MUST be
    //      terminated using Terminate.
    //
    //  NOTES:
    //      The stack automatically terminates an existing persistent connection
    //      created by the switchback interface that is not connected if its
    //      remote IP address is not found in the DNS records.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::Establish,
    //      ISipSwitchbackPersistentConnection::Terminate
    //
    //==========================================================================
    virtual void EvConnectionCreated(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                                     IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  EvEstablished
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs that the connection was successful.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //       CANNOT be NULL.
    //
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Description:
    //      Tells that the persistent connection was established (connection was
    //      successful). This event can be called many times during the lifetime
    //      of the persistent connection as it could be disconnected and then
    //      re-connected when the error timeout elapses.
    //
    //      If the application wants the stack to use this persistent connection
    //      as the default highest priority connection it must verify its
    //      connectivity first. Verifying the connection is done by calling the
    //      ConnectivityVerified method.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::ConnectivityVerified
    //
    //==========================================================================
    virtual void EvEstablished(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                               IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  EvTerminated
    //==
    //==========================================================================
    //
    //  Summary:
    //      Tells that the persistent connection is terminated.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //       CANNOT be NULL.
    //
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //  Description:
    //      Tells that the persistent connection is terminated. This is reported
    //      when the termination process of the connection has completed.
    //
    //      This event can be reported in two cases:
    //      - Terminate was called by the application.
    //      - A persistent connection to remote IP1 was created because the DNS
    //        returned IP1 as a record. A subsequent DNS request did not return
    //        IP1. In such cases, EvTerminated is automatically reported by the
    //        stack if the state of the persistent connection was not connected.
    //
    //      This is the final event, opqConnection cannot be used after this
    //      event has been called.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::Terminate
    //
    //==========================================================================
    virtual void EvTerminated(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                              IN mxt_opaque opqConnection) = 0;

    //==========================================================================
    //==
    //==  EvError
    //==
    //==========================================================================
    //
    //  Summary:
    //      Informs that a failure occurred on the persistent connection.
    //
    //  Parameters:
    //      pSwitchbackPersistentConnection:
    //       The switchback persistent connection.
    //
    //       CANNOT be NULL.
    //
    //      opqConnection:
    //       The persistent connection opaque identifier.
    //
    //      res:
    //       The failure result.
    //
    //  Description:
    //      Reported when a failure occurred on the persistent connection. The
    //      failure could have occurred while the connect was in progress or when
    //      the connection was established.
    //
    //      As for EvEstablished, this EvError event can be called many times on
    //      the persistent connection during its lifetime. Upon reporting this
    //      event the stack starts an error timeout timer if none is already
    //      started.
    //
    //      Calling GenerateError will result in this event being reported.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::GenerateError
    //
    //==========================================================================
    virtual void EvError(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                         IN mxt_opaque opqConnection,
                         IN mxt_result res) = 0;

    //==========================================================================
    //==
    //==  EvSipErrorOnConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports a SIP failure on a persistent connection.
    //
    //  Parameters:
    //    pSwitchbackPersistentConnection:
    //      The switchback persistent connection. CANNOT be NULL.
    //
    //    opqConnection:
    //      The connection handle received in the call to Establish.
    //
    //    rSentPacket:
    //      The packet that failed to be sent.
    //
    //    resConnectionError:
    //      The error that occurred at the SIP level on the persistent 
    //      connection :
    //        resFE_SIPTRANSPORT_MTU_SIZE_ERROR:
    //          Indicates the packet could not be sent because its serialized
    //          size exceeds its maximum allowed size.
    //
    //        resFE_SIPTRANSACTION_SHUTDOWN :
    //          The shutdown process was initiated.
    //
    //        resFE_ABORT 
    //          Transport error (e.g. ICMP error, timeout, etc.)
    //
    //        Other failures
    //          See errors code that can be reported through EvCommandResult.
    //
    //  Description:
    //      Informs that the last packet has not been successfully sent. It can
    //      be because the connection does not exist, an ICMP error was received
    //      or a timeout occurred. 
    //
    //  Note:
    //      Can be safely ignored.
    //
    //  See Also:
    //      ISipSwitchbackPersistentConnection::Establish,
    //      ISipSwitchbackPersistentConnection::GenerateError,
    //      ISipCoreUser::EvCommandResult
    //
    //==========================================================================
    virtual void EvSipErrorOnConnection(IN ISipSwitchbackPersistentConnection* pSwitchbackPersistentConnection,
                                        IN mxt_opaque opqConnection,
                                        IN const CSipPacket& rSentPacket,
                                        IN mxt_result resConnectionError) = 0;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipSwitchbackPersistentConnectionMgr(){}

    // Summary:
    //  Destructor.
    virtual ~ISipSwitchbackPersistentConnectionMgr(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipSwitchbackPersistentConnectionMgr(IN const ISipSwitchbackPersistentConnectionMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipSwitchbackPersistentConnectionMgr& operator=(IN const ISipSwitchbackPersistentConnectionMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPSWITCHBACKPERSISTENTCONNECTIONMGR_H

