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
#ifndef MXG_IASYNCSOCKETFACTORYCONFIGURATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETFACTORYCONFIGURATIONMGR_H
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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketFactoryConfigurationMgr
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    This interface is used to receive asynchronous sockets configuration
//    requests from the socket factory.
//
//  Description:
//    This interface is a manager which handles the configuration of
//    asynchronous sockets.  The configuration manager is notified when an
//    asynchronous socket has just been created and must be configured with
//    various socket options. In contrast with the creation managers, all the
//    configuration managers are notified and have the opportunity to
//    configure the asynchrous socket. The configuration managers are called
//    in their registration ordering so that the last registered configuration
//    manager may override configurations previously performed by other
//    managers. To be called, the manager must be registered using
//    CAsyncSocketFactory::RegisterConfigurationMgr.
//
//  Location:
//    Network/IAsyncSocketFactoryConfigurationMgr.h
//
//  See Also:
//    CAsyncSocketFactory, IAsyncSocketFactoryCreationMgr
//
//==============================================================================
class IAsyncSocketFactoryConfigurationMgr
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvConfigurationRequested
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the manager that a newly created asynchronous socket must be
    //    configured.
    //
    //  Parameters:
    //    apszType:
    //      An array of strings representing the type of the socket.
    //
    //    uTypeSize:
    //      The size of the array of strings.
    //
    //    pAsyncSocket:
    //      The newly created asynchronous socket that must be configured.
    //
    //  Returns:
    //    resSI_FALSE:
    //      The asynchronous socket has not been configured.
    //    resSI_TRUE:
    //      The asynchronous socket has been configured.
    //    resFE_FAIL:
    //      An error has been detected while configuring the asynchronous
    //      socket.
    //
    //  Description:
    //    This method notifies the manager that a newly created asynchronous
    //    socket must be configured. The type of the asynchronous socket that
    //    has been created is provided as a sequence of network protocols
    //    starting from the highest level down to the lowest level. Each level
    //    is possibly followed by arguments.
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
    virtual mxt_result EvConfigurationRequested(IN const char* const* apszType,
                                                IN unsigned int uTypeSize,
                                                INOUT IAsyncSocket* pAsyncSocket) = 0;
//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketFactoryConfigurationMgr
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
    IAsyncSocketFactoryConfigurationMgr() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocketFactoryConfigurationMgr
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
    virtual ~IAsyncSocketFactoryConfigurationMgr() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketFactoryConfigurationMgr(const IAsyncSocketFactoryConfigurationMgr& from);
    IAsyncSocketFactoryConfigurationMgr& operator=(const IAsyncSocketFactoryConfigurationMgr& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETFACTORYCONFIGURATIONMGR_H
