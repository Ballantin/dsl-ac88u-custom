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
#ifndef MXG_CASYNCSOCKETFACTORY_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCSOCKETFACTORY_H
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
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Parameters used by value
#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncSocket;
class IAsyncSocketFactoryConfigurationMgr;
class IAsyncSocketFactoryCreationMgr;
class IEComUnknown;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAsyncSocketFactory
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    This class centralizes the creation of asynchronous sockets.
//
//  Description:
//    This class is an asynchronous socket factory that centralizes the creation
//    of asynchronous sockets. The behaviour of the factory may be modified by
//    the registration of managers. Two types of managers exist:
//    creation managers and configuration managers.
//
//    The creation manager will be notified when an asynchronous socket must be
//    created. The manager must then decide, based on the type of socket to be
//    created, if it can handle the request. If it can, it creates the new
//    asynchronous socket and returns it back to the factory. If not, it just
//    indicates to the factory that it cannot fulfill the request. The factory
//    will then try the same process on the next registered manager, if one is
//    available. Creation managers are called in the reverse registration order.
//    This is needed so that the last creation manager to be registered may
//    override the creation process for a specific asynchronous socket type.
//    If no managers are available or if none of them created the asynchronous
//    socket, the socket factory will default to its own internal creation
//    method which supports TCP and UDP asynchronous sockets.
//
//    The configuration manager will be notified when an asynchronous socket has
//    just been created and must be configured with various socket options.
//    In contrast with the creation managers, all the configuration managers
//    will be notified and have the opportunity to configure the asynchronous
//    socket. The configuration managers will be called in their registration
//    ordering so that last registered configuration manager may override
//    configurations previously performed by other managers.
//
//  Location:
//    Network/CAsyncSocketFactory.h
//
//  See Also:
//    IAsyncSocketFactoryConfigurationMgr, IAsyncSocketFactoryCreationMgr
//
//==============================================================================
class CAsyncSocketFactory
{
//-- Published Interface
public:

    //  Summary:
    //    Creates an asynchronous socket.
    static mxt_result CreateAsyncSocket(IN IEComUnknown* pServicingThread,
                                        IN const char* const* apszType,
                                        IN unsigned int uTypeSize,
                                        OUT IAsyncSocket** ppAsyncSocket);

    //  Summary:
    //    Indicates if the asynchronous socket is already in the internal list.
    static bool IsAsyncSocketInList(IN IAsyncSocket* pAsyncSocket);

    //  Summary:
    //    Registers a configuration manager.
    static void RegisterConfigurationMgr(IN IAsyncSocketFactoryConfigurationMgr* pMgr);

    //  Summary:
    //    Registers a creation manager.
    static void RegisterCreationMgr(IN IAsyncSocketFactoryCreationMgr* pMgr);

    //  Summary:
    //    Unregisters a configuration manager.
    static void UnregisterConfigurationMgr(IN IAsyncSocketFactoryConfigurationMgr* pMgr);

    //  Summary:
    //    Unregisters a creation manager.
    static void UnregisterCreationMgr(IN IAsyncSocketFactoryCreationMgr* pMgr);

    //  Summary:
    //    Returns a copy of the list of sockets.
    static mxt_result GetSocketList(OUT CList<IAsyncSocket*>* plistOfSocket);

    //  Summary:
    //    Removes a socket from the internal list.
    static mxt_result RemoveSocketFromFactoryList(IN IAsyncSocket* pAsyncSocket);

//M5T_INTERNAL_USE_BEGIN
    //  Summary:
    //    Calls all the registered configuration managers.
    static mxt_result CallConfigurationMgr(IN IAsyncSocket* pAsyncSocket);

private:
    //  Summary:
    //    Adds a socket to the internal list.
    static void AddSocketToFactoryList(IN IAsyncSocket* pAsyncSocket);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CASYNCSOCKETFACTORY_H
