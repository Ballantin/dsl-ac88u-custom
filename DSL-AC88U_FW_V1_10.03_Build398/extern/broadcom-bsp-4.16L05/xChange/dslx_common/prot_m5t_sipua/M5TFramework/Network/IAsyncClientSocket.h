//==============================================================================
//==============================================================================
//
//         Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//         Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//         Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_IASYNCCLIENTSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCCLIENTSOCKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSocketAddr;
class IAsyncClientSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncClientSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncClientSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the methods associated with a client socket.
//
//  Description:
//    The IAsyncClientSocket interface defines the methods associated with a
//    client socket. A client socket can be used to send and receive data, as
//    defined in the IAsyncIoSocket interface, and it can also connect to a
//    remote address.
//
//    This interface defines the BindA() and ConnectA() methods which are used
//    to bind and connect a socket to a remote address, respectively. Note that
//    some types of sockets must be connected in order to send and receive data
//    (connection type eSTREAM and eSEQPACKET), while others can be used
//    unconnected (connection type eDATAGRAM).
//
//    Events related to this interface are reported through the
//    IAsyncClientSocketMgr interface.
//
//  Location:
//    Network/IAsyncClientSocket.h
//
//  See Also:
//    IAsyncClientSocketMgr
//    IAsyncIoSocket
//    IAsyncServerSocket
//    IAsyncSocket
//    IAsyncUnconnectedIoSocket
//
//==============================================================================
class IAsyncClientSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncClientSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  BindA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Binds the socket to a local address.
    //
    //  Parameters:
    //    pLocalAddress:
    //      Contains the local address where to bind.  May be NULL if the user
    //      does not care about the interface or the local port. If the
    //      parameter is not NULL and its port is set to 0, the automatic port
    //      allocation feature will also be used. Do not use NULL for IPv6.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not in a valid state to bind on
    //                         a specific address.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    Used to associate a local address to the socket.  The user MUST call
    //    this method following a call to
    //    CAsyncSocketFactory::CreateAsyncSocket().
    //
    //    Upon a successful bind, the socket will generate the
    //    IAsyncClientSocketMgr::EvAsyncClientSocketMgrBound event. You will
    //    then be able to query the real address and port used by the socket by
    //    calling method IAsyncSocket::GetLocalAddress on it.
    //
    //    Upon a binding error, the socket will generate the
    //    IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  See Also:
    //    IAsyncClientSocketMgr.h
    //
    //==========================================================================
    virtual mxt_result BindA(IN const CSocketAddr* pLocalAddress) = 0;

    //==========================================================================
    //==
    //==  ConnectA
    //==
    //==========================================================================
    //
    //  Summary:
    //    Connects the socket to a remote address.
    //
    //  Parameters:
    //    pPeerAddress:
    //      Remote address where the socket is to be connected. It cannot be
    //      NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not in a valid state to connect
    //                         to a remote host.
    //    resFE_INVALID_ARGUMENT: pPeerAddress is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This asynchronous method is used to connect the socket to a remote
    //    address. Once a socket is connected, IAsyncIoSocket::Send and
    //    IAsyncIoSocket::Recv can be used to exchange data with the remote
    //    peer.
    //
    //    Upon a successful connection, the socket will generate the
    //    IAsyncClientSocketMgr::EvAsyncClientSocketMgrConnected event.
    //
    //    Upon a connection error, the socket will generate the
    //    IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  See Also:
    //    IAsyncClientSocketMgr.h
    //
    //==========================================================================
    virtual mxt_result ConnectA(IN const CSocketAddr* pPeerAddress) = 0;

    //==========================================================================
    //==
    //== mxt_result SetAsyncClientSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the manager associated with the interface IAsyncClientSocket
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The manager is probably already set.
    //    resFE_INVALID_ARGUMENT: pMgr is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets the manager associated with the interface
    //    IAsyncClientSocket.
    //
    //==========================================================================
    virtual mxt_result SetAsyncClientSocketMgr(IN IAsyncClientSocketMgr* pMgr) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncClientSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //    Constructor.
    //
    //  Description:
    //    Default Constructor.
    //
    //==========================================================================
    IAsyncClientSocket() {}

    //==========================================================================
    //==
    //==  ~IAsyncClientSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==========================================================================
    virtual ~IAsyncClientSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncClientSocket(const IAsyncClientSocket& from);
    IAsyncClientSocket& operator=(const IAsyncClientSocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)


#endif // MXG_IASYNCCLIENTSOCKET_H
