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
#ifndef MXG_IASYNCSOCKETFACTORYCREATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETFACTORYCREATIONMGR_H
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

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncSocket;
class IEComUnknown;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketFactoryCreationMgr
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    This interface is used to receive asynchronous sockets creation requests
//    from the socket factory.
//
//  Description:
//    This interface is a manager which handles the creation of asynchronous
//    sockets. The manager is called each time a new asynchronous socket
//    needs to be created. To be called, the manager must be registered using
//    CAsyncSocketFactory::RegisterCreationMgr. Created sockets must be ECOM
//    objects that implement the follonwing required interfaces and possibly
//    one or more optional interfaces.
//
//    #Required interfaces for all sockets:#
//    - IAsyncSocket.
//
//    #Required interfaces based on the socket's usage:#
//    - Client sockets:
//        - IAsyncClientSocket
//        - IAsyncIoSocket
//    - Client sockets implementing the TLS protocol:
//        - All client sockets interfaces plus:
//        - IAsyncTlsSocket
//    - Server sockets:
//        - IAsyncServerSocket
//    - Server sockets implementing the TLS protocol:
//        - All server sockets interfaces plus:
//        - IAsyncTlsServerSocket
//
//    #Optional interfaces:#
//    - IAsyncSocketUdpOptions
//    - IAsyncSocketTcpOptions
//    - IAsyncSocketBufferSizeOptions
//    - IAsyncSocketQualityOfServiceOptions
//    - IAsyncSocketWindowsGqosOptions
//
//    #Optional interfaces for sockets implementing the TLS protocol:#
//    - IAsyncSocketTlsRenegotiation
//
//  Location:
//    Network/IAsyncSocketFactoryCreationMgr.h
//
//  See Also:
//    CAsyncSocketFactory, IAsyncSocketFactoryConfigurationMgr
//
//==============================================================================
class IAsyncSocketFactoryCreationMgr
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvCreationRequested
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the manager that a new asynchronous socket must be created.
    //
    //  Parameters:
    //    pServicingThread:
    //      The servicing thread used to activate the asynchronous
    //      socket. If NULL is passed, the asynchronous socket
    //      automatically creates its own servicing thread with default
    //      parameters.
    //
    //    apszType:
    //      An array of strings representing the type of the socket.
    //
    //    uTypeSize:
    //      The size of the array of string.
    //
    //    ppAsyncSocket:
    //      The location where the new asynchronous socket is returned.
    //
    //  Returns:
    //    resSI_FALSE:
    //      The asynchronous socket has not been created.
    //    resSI_TRUE:
    //      The asynchronous socket has been created.
    //    resFE_FAIL:
    //      An error has been detected while creating the asynchronous socket.
    //
    //  Description:
    //    This method notifies the manager that a new asynchronous socket must
    //    be created. Created sockets must be ECOM objects. The type of the
    //    asynchronous socket to be created is provided as a sequence of network
    //    protocols starting from the highest level down to the lowest level.
    //    Each level is possibly followed by arguments.
    //
    //    Examples:
    //
    //    <CODE>
    //      { "RTCP", "UDP" }, 2
    //      { "RTP", "UDP" }, 2
    //      { "RTP_RTCP", "UDP" }, 2
    //      { "SIP", "TCP, m=client" }, 2
    //      { "SIP", "TCP, m=server" }, 2
    //      { "SIP", "TLS, m=client", "TCP, m=client" }, 3
    //      { "SIP", "TLS, m=server", "TCP, m=server" }, 3
    //      { "SIP", "UDP" }, 2
    //      { "STUN", "TCP, m=client" }, 2
    //      { "STUN", "TCP, m=server" }, 2
    //      { "STUN", "TLS, m=client", "TCP" }, 3
    //      { "STUN", "TLS, m=server", "TCP" }, 3
    //      { "STUN", "UDP" }, 2
    //    </CODE>
    //
    //==========================================================================
    virtual mxt_result EvCreationRequested(IN IEComUnknown* pServicingThread,
                                           IN const char* const* apszType,
                                           IN unsigned int uTypeSize,
                                           OUT IAsyncSocket** ppAsyncSocket) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketFactoryCreationMgr
    //==
    //==========================================================================
    //
    // Summary:
    //   Constructor.
    //
    //  Description:
    //   Constructor.
    //
    //==========================================================================
    IAsyncSocketFactoryCreationMgr() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketFactoryCreationMgr
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
    virtual ~IAsyncSocketFactoryCreationMgr() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketFactoryCreationMgr(const IAsyncSocketFactoryCreationMgr& from);
    IAsyncSocketFactoryCreationMgr& operator=(const IAsyncSocketFactoryCreationMgr& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETFACTORYCREATIONMGR_H
