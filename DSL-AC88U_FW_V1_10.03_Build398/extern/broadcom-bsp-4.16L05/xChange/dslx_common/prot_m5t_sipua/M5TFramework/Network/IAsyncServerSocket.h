//==============================================================================
//==============================================================================
//
//         Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_IASYNCSERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSERVERSOCKET_H
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

#ifdef MXD_ECOM_ENABLE_SUPPORT

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncServerSocketMgr;
class CSocketAddr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncServerSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncServerSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    The IServerSocket interface defines the methods needed for a server type
//    of socket.
//
//  Description:
//    The IAsyncServerSocket interface defines the methods needed for a server
//    type of socket. Note that this interface is available only to
//    connection-oriented sockets types.
//
//    Events related to this interface are reported through the
//    IAsyncServerSocketMgr interface.
//
//  Location:
//    Network/IAsyncServerSocket.h
//
//  See Also:
//    IAsyncClientSocket
//    IAsyncIoSocket
//    IAsyncServerSocketMgr
//    IAsyncSocket
//    IAsyncUnconnectedIoSocket
//
//==============================================================================
class IAsyncServerSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncServerSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AcceptA
    //==
    //==========================================================================
    //
    //  Summary:
    //    Accepts an incoming connection attempt.
    //
    //  Parameters:
    //    pServicingThread:
    //      Pointer to the Servicing Thread to which the socket should be
    //      attached. It can be NULL, in which case the server socket's internal
    //      servicing thread will be used.
    //
    //  Returns:
    //    Always returns resS_OK. Errors will be reported through the
    //    IAsynSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  Description:
    //    Accepts an incoming connection attempt on a listening
    //    connection-oriented socket.
    //
    //    Creating multiple sockets attached to the same Servicing Thread
    //    is a way to have multiple sockets running in a single thread.
    //
    //    The method is asynchronous because under some conditions, the
    //    process of accepting a new connection may take some time. The user
    //    will be advised when the process is completed by the
    //    IAsyncServerSocketMgr::EvAsyncServerSocketMgrConnectionAccepted event.
    //
    //    If an important error is detected late in the accepting process,
    //    the error will be forwarded to the caller by means of an
    //    IAsynSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  #Deprecation Note:#
    //    The opaque parameter that used to be a parameter to this method is now
    //    deprecated. To set the opaque value of an accepted socket, the
    //    application must now wait for the
    //    IAsyncServerSocketMgr::EvAsyncServerSocketMgrConnectionAccepted event
    //    and use the IAsyncSocket::SetOpaque method. An IAsyncIoSocket
    //    interface is received as a parameter to the event on which the
    //    IAsyncSocket interface can be queried.
    //
    //    Furthermore, the application can also use the socket factory's
    //    IAsyncSocketFactoryConfigurationMgr interface instead to set the
    //    opaque value. See the interface's documentation for more details.
    //
    //  See Also:
    //    Listen
    //
    //==========================================================================
    virtual mxt_result AcceptA(IN IEComUnknown* pServicingThread) = 0;

    //==========================================================================
    //==
    //==  BindA
    //==
    //==========================================================================
    //
    //  Summary:
    //    Binds the socket to a local address.
    //
    //  Parameters:
    //    pLocalAddress:
    //      Contains the local address where to bind.  May be NULL if the user
    //      does not care about the interface or the local port. If the
    //      parameter is not NULL and its port is set to 0, the automatic port
    //      allocation feature will also be used.
    //
    //  Returns:
    //    Always return resS_OK. Errors will be reported through the
    //    IAsynSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  Description:
    //    Used to associate a local address to the socket. The user MUST
    //    call this method following a call to
    //    CAsyncSocketFactory::CreateAsyncSocket().
    //
    //    Upon a successful bind, the socket will generate the
    //    IAsyncServerSocketMgr::EvAsyncClientSocketMgrBound event. You will
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
    //==  Listen
    //==
    //==========================================================================
    //
    //  Summary:
    //    Listens on the socket for incoming connection attempts.
    //
    //  Parameters:
    //    uMaxPendingConnection:
    //      Maximum number of pending connections accepted.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not correctly initialized.
    //    See GetSocketErrorId for possible return values
    //
    //  Description:
    //    Sets the socket to listen for incoming connection attempts. Once a
    //    connection attempt is detected, IAsyncServerSocket::AcceptA must be
    //    called in order to fully establish the connection.
    //
    //    Once the socket detects an incoming connection attempt, the
    //    IAsyncServerSocketMgr::EvAsyncServerSocketMgrConnectionRequested
    //    event is reported. The socket user must then call
    //    IAsyncServerSocket::AcceptA in order to establish the connection.
    //
    //  See Also:
    //    AcceptA,
    //    IAsyncServerSocketMgr::EvAsyncServerSocketMgrConnectionRequested
    //
    //==========================================================================
    virtual mxt_result Listen(IN unsigned int uMaxPendingConnection = 5) = 0;

    //==========================================================================
    //==
    //== QueryAcceptedOptionsIf
    //==
    //==========================================================================
    //
    //  Summary:
    //    Provides ECOM interfaces to set options on the accepted TCP socket.
    //
    //  Parameters:
    //    iidRequested:
    //      The id of the ECOM interface requested for the accepted TCP socket.
    //
    //    ppInterface:
    //      The returned interface. The caller is responsible of calling
    //      ReleaseIfRef on the returned interface. It must not be NULL.
    //
    //  Returns:
    //    resFE_MITOSFW_ECOM_NOINTERFACE: The queried interface has not been
    //                                    found.
    //    resS_OK: The interface has been retrieved.
    //
    //  Description:
    //    This method provides ECOM interfaces to set options on the accepted
    //    TCP socket.
    //
    //  WARNING:
    //     Proper reference count handling is required when using this method:
    //         - All interfaces returned by this method MUST be released EXACTLY
    //           ONCE using ReleaseIfRef.
    //         - All copies of the returned pointer MUST add a reference using
    //           AddIfRef and use ReleaseIfRef on it when the copy is no longer
    //           needed
    //
    //     These statements are part of the basic ECOM rules. They are reminded
    //     here to stress out their importance regarding this method.
    //
    //     Following these rules will prevent memory leaks and crashes inside
    //     the asynchronous server socket.
    //
    //==========================================================================
    virtual mxt_result QueryAcceptedOptionsIf(IN mxt_iid iidRequested,
                                              OUT void** ppInterface) = 0;

// GCC 2.7.2 does not support the following templated single-parameter method.
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7))
    //==========================================================================
    //==
    //==  QueryAcceptedOptionsIf
    //==
    //==========================================================================
    //
    //  Summary:
    //    Provides ECOM interfaces to set options on the accepted TCP socket.
    //
    //  Parameters:
    //    ppInterface:
    //      The returned interface. The caller is responsible of calling
    //      ReleaseIfRef on the returned interface. It must not be NULL.
    //
    //  Returns:
    //    resFE_MITOSFW_ECOM_NOINTERFACE: The queried interface has not been
    //                                    found.
    //    resS_OK: The interface has been retrieved.
    //
    //  Description:
    //    This is a templated version of the original QueryAcceptedOptionsIf
    //    method. It is designed to ease code readability.
    //
    //    For this method to compile, the **ppInterface referenced class must
    //    contain MX_DECLARE_ECOM_GETIID.
    //
    //    This method provides ECOM interfaces to set options on the accepted
    //    TCP socket.
    //
    //  WARNING:
    //     Proper reference count handling is required when using this method:
    //         - All interfaces returned by this method MUST be released EXACTLY
    //           ONCE using ReleaseIfRef.
    //         - All copies of the returned pointer MUST add a reference using
    //           AddIfRef and use ReleaseIfRef on it when the copy is no longer
    //           needed
    //
    //     These statements are part of the basic ECOM rules. They are reminded
    //     here to stress out their importance regarding this method.
    //
    //     Following these rules will prevent memory leaks and crashes inside
    //     the asynchronous server socket.
    //
    //==========================================================================
    template<class _Type> mxt_result QueryAcceptedOptionsIf(OUT _Type** ppInterface)
    {
        // Has to be inlined here because Visual C++ 6.0 does not like out of class
        // inlined specialized template method.
        if (ppInterface != NULL)
        {
            return QueryAcceptedOptionsIf((*ppInterface)->GetIID(),
                                          reinterpret_cast<void**>(ppInterface));
        }
        return resFE_INVALID_ARGUMENT;
    }
#endif //- #if (MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)


    //==========================================================================
    //==
    //== SetAsyncServerSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the manager associated with the interface IAsyncServerSocket
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to a IAsyncServerSocketMgr interface. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The manager has probably already been set.
    //    resFE_INVALID_ARGUMENT: pMgr is NULL.
    //    resS_OK: The manager has been correctly set.
    //
    //  Description:
    //    This method sets the manager associated with the IAsyncServerSocket
    //    interface.
    //
    //==========================================================================
    virtual mxt_result SetAsyncServerSocketMgr(IN IAsyncServerSocketMgr* pMgr) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncServerSocket
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
    IAsyncServerSocket() {}

    //==========================================================================
    //==
    //==  ~IAsyncServerSocket
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
    virtual ~IAsyncServerSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncServerSocket(const IAsyncServerSocket& from);
    IAsyncServerSocket& operator=(const IAsyncServerSocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //#ifdef MXD_ECOM_ENABLE_SUPPORT

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IASYNCSERVERSOCKET_H
