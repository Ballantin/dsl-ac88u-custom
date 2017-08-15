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
#ifndef MXG_IASYNCSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKET_H
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

//-- Parameters used by Value
#ifndef MXG_ISOCKET_H
#include "Network/ISocket.h"
#endif
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif
// BRCM_CUSTOM - [begin] Add user opaque to socket.
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
// BRCM_CUSTOM - [end] Add user opaque to socket.

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class IAsyncSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the basic methods accessible on asynchronous sockets.
//
//    Class implementing abstraction of sockets. It is used to encapsulate the
//    socket functionality in an asynchronous manner.
//
//  Description:
//    Interface defining the most basic methods accessible on asynchronous
//    sockets. This means the asynchronous closure of the socket and retrieval
//    of its various associated properties. All asynchronous sockets implement
//    this interface.
//
//    Although it is possible to close an asynchronous socket by releasing all
//    references to it, it is a better practice to explicitly request its closure
//    with a call to CloseA. This also allows the caller to specify the type of
//    closure to perform. Releasing all references to the asynchronous socket
//    will always perform a forced close.
//
//    Events related to this interface are reported through the IAsyncSocketMgr
//    interface.
//
//    This interface MUST be obtained by calling
//    CAsyncSocketFactory::CreateAsyncSocket.
//
//  Location:
//    Network/IAsyncSocket.h
//
//  See Also:
//    IAsyncClientSocket
//    IAsyncIoSocket
//    IAsyncServerSocket
//    IAsyncSocketMgr
//    IAsyncUnconnectedIoSocket
//    CAsyncSocketFactory::CreateAsyncSocket
//
//==============================================================================
class IAsyncSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //== Activate
    //==
    //==========================================================================
    //
    //  Summary:
    //    Activates the socket on the provided ServicingThread.
    //
    //  Parameters:
    //    pIEComUnknown:
    //      A pointer to an interface serviced by a Servicing Thread. It can
    //      be NULL, in which case a new servicing thread will be created for
    //      the socket.
    //
    //  Returns:
    //    resS_OK: The socket activated correctly.
    //    resFE_INVALID_ARGUMENT: pIEComUnknown is NULL.
    //    resFE_INVALID_STATE: The socket has already been activated.
    //
    //  Description:
    //    This method must be the first method to be called on a socket. It is
    //    used to associate the socket with a servicing thread. You will have
    //    to call SetAsyncSocketMgr and SetOpaque before you try to use the
    //    socket but after you called this method.
    //
    //    When this interface is properly obtained by calling
    //    CAsyncSocketFactory::CreateAsyncSocket, calling this method is
    //    unnecessary because the servicing thread is already activated.
    //
    //  See Also:
    //    SetAsyncSocketMgr, SetOpaque, CAsyncSocketFactory::CreateAsyncSocket
    //
    //==========================================================================
    virtual mxt_result Activate(IN IEComUnknown* pIEComUnknown) = 0;

    //==========================================================================
    //==
    //==  CloseA
    //==
    //==========================================================================
    //
    //  Summary:
    //    Closes the asynchronous socket.
    //
    //  Parameters:
    //    eCloseBehavior:
    //      How the socket is to be closed.
    //
    //  Returns:
    //    Always returns resS_OK. Errors are reported through the
    //    IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected event.
    //
    //  Description:
    //    IMPORTANT: This is an asynchronous method!
    //
    //    This is used to asynchronously close the socket. Once the socket is
    //    closed, the IAsyncSocketMgr::EvAsyncSocketMgrClosed event will be
    //    generated back to the socket manager once the asynchronous closure has
    //    completed.
    //
    //    IAsyncSocketMgr::EvAsyncSocketMgrClosed is always reported when the
    //    closure was successful. IAsyncSocketMgr::EvAsyncSocketMgrClosed is not
    //    reported over a reliable transport if the closure has failed.
    //    IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected is reported in case of
    //    a failure over a reliable transport. Over an unreliable transport,
    //    IAsyncSocketMgr::EvAsyncSocketMgrClosed is always reported once the
    //    closure has completed (even if the socket closure has failed).
    //
    //    After IAsyncSocketMgr::EvAsyncSocketMgrClosed is generated, it is
    //    possible for the user of the socket to re-use the asynchronous socket.
    //
    //    Closing the socket has the effect of resetting all the socket's
    //    options to their default values. Thus, if the socket is reused, it
    //    might be necessary to reconfigure its socket options.
    //
    //  See Also:
    //    IAsyncSocketMgr::EvAsyncSocketMgrClosed,
    //    IAsyncSocketMgr::EvAsyncSocketMgrErrorDetected
    //
    //==========================================================================
    virtual mxt_result CloseA(IN ISocket::ECloseBehavior eCloseBehavior) = 0;

    //==========================================================================
    //==
    //==  GetHandle
    //==
    //==========================================================================
    //
    //  Summary:
    //    Returns the socket handle.
    //
    //  Parameters:
    //    phSocket:
    //      A pointer to a socket handle. It cannot be NULL.
    //
    //  Returns:
    //    resS_OK: Handle returned successfully.
    //    resFE_INVALID_ARGUMENT: phSocket is NULL.
    //    resFE_INVALID_STATE: The socket is not bound.
    //
    //  Description:
    //    This method gets the handle of the underlying socket.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetHandle(OUT mxt_hSocket* phSocket) const = 0;

    //==========================================================================
    //==
    //==  GetLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves the local address to which the socket is bound.
    //
    //  Parameters:
    //    pLocalAddress:
    //      OUT parameter that will contain the address and port information. It
    //      cannot be NULL.
    //
    //  Returns:
    //    resS_OK: The local address has been returned.
    //    resFE_INVALID_STATE: The socket is not bound.
    //    resFE_INVALID_ARGUMENT: pLocalAddress is NULL.
    //
    //  Description:
    //    This method is used to retrieve the address and port to which the
    //    socket has been bound. This may be a port located on a remote server
    //    (e.g. Allocate using a TURN server), this may also be a port
    //    located on a NAT public side (e.g. Discovery using a STUN Binding
    //    Request) or a port located on a local interface. The user must be
    //    aware that the returned address and port is not always located on a
    //    local interface and hence, it SHOULD NOT be used in a future call to
    //    IAsyncClientSocket or IAsyncServerSocket::BindA.
    //    GetLocalInterfaceAddress should be called instead to retrieve an
    //    address and port located on a local interface.
    //
    //    It will return an error until the socket is bound to an interface
    //    with the IAsyncClientSocket or IAsyncServerSocket::BindA method.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetLocalAddress(OUT CSocketAddr* pLocalAddress) const = 0;

    //==========================================================================
    //==
    //==  GetLocalInterfaceAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves the local address to which the socket is bound on a local
    //    interface.
    //
    //  Parameters:
    //    pLocalAddress:
    //      OUT parameter that will contain the address and port information. It
    //      cannot be NULL.
    //
    //  Returns:
    //    resS_OK: The local interface address has been returned.
    //    resFE_INVALID_STATE: The socket is not bound.
    //    resFE_INVALID_ARGUMENT: pLocalAddress is NULL.
    //
    //  Description:
    //    This method is used to retrieve the address and port to which the
    //    socket has been bound on a local interface. The method is different
    //    than GetLocalAddress in that the returned local address and port
    //    always reside on a local interface.
    //
    //    The method will return an error until the socket is bound to an
    //    interface with the IAsyncClientSocket or IAsyncServerSocket::BindA
    //    method.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetLocalInterfaceAddress(OUT CSocketAddr* pLocalInterfaceAddress) const = 0;

    //==========================================================================
    //==
    //==  GetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves the opaque identifier.
    //
    //  Parameters:
    //    popq:
    //      OUT parameter that receives the opaque identifier. It cannot be
    //      NULL.
    //
    //  Returns:
    //    resS_OK: The opaque value has been returned.
    //    resFE_INVALID_ARGUMENT: popq is NULL.
    //
    //  Description:
    //    This method is used to retrieve the opaque identifier of the socket.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetOpaque(OUT mxt_opaque* popq) const = 0;

    //==========================================================================
    //==
    //==  GetServicingThreadIEcomUnknown
    //==
    //==========================================================================
    //
    //  Summary:
    //    Returns a pointer to the IEComUnknown interface of the servicing
    //    thread that has been used to activate the socket.
    //
    //  Parameters:
    //    ppIEComUnknown:
    //     A pointer to receive the IEComUnknown interface pointer of the
    //     ServicingThread that has been used to activate the socket. Note that
    //     while the parameter CANNOT be NULL, the value returned in it CAN.
    //     This will happen if the Activate method of this interface has not
    //     been called yet. The caller MUST call ReleaseIfRef on the returned
    //     interface when he is done with it.
    //
    //  Returns:
    //     resS_OK: The interface has been successfully returned.
    //     resFE_INVALID_ARGUMENT: The provided parameter is NULL.
    //     resFE_FAIL: The socket has not been activated. The value of the
    //                 parameter will be NULL in this case.
    //
    //  Description:
    //    Returns a pointer to the IEComUnknown interface of the servicing
    //    thread that has been used to activate the socket.
    //
    //  See Also:
    //    Activate
    //
    //==========================================================================
    virtual mxt_result GetServicingThreadIEcomUnknown(OUT IEComUnknown** ppIEComUnknown) const = 0;

    //==========================================================================
    //==
    //==  GetSocketType
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves the type of the socket.
    //
    //  Parameters:
    //    peSocketType:
    //      OUT parameter that receives the socket type information, as defined
    //      in the ISocket::ESocketType enumeration. It cannot be NULL.
    //
    //  Returns:
    //    resS_OK: The socket type has been returned.
    //    resFE_INVALID_STATE: The socket is not bound.
    //    resFE_INVALID_ARGUMENT: peSocketType is NULL.
    //
    //  Description:
    //    This method is used to retrieve the connection type of the socket.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetSocketType(OUT ISocket::ESocketType* peSocketType) const = 0;

    //==========================================================================
    //==
    //==  SetAsyncSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the manager associated with the interface IAsyncSocket.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to a IAsyncSocketMgr manager. It cannot be NULL.
    //
    //  Returns:
    //    resS_OK: The manager has been set correctly.
    //    resFE_INVALID_STATE: The manager has probably already been set.
    //    resFE_INVALID_ARGUMENT: pMgr is NULL.
    //
    //  Description:
    //    This method sets the manager associated with the IAsyncSocket
    //    interface.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //  See Also:
    //    IAsyncSocketMgr
    //
    //==========================================================================
    virtual mxt_result SetAsyncSocketMgr(IN IAsyncSocketMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetOpaque
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the user opaque value.
    //
    //  Parameters:
    //    opq:
    //      The identifier that must be associated with the socket.
    //
    //  Returns:
    //    Always returns resS_OK.
    //
    //  Description:
    //    This method sets the opaque value that must be associated with the
    //    socket. This opaque value is sent with each event to the managers.
    //    A call to this method will always replace the existing value. This
    //    value only has meaning for the application.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result SetOpaque(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  GetSocketType
    //==
    //==========================================================================
    //
    //  Summary:
    //    Returns the socket type as a string array.
    //
    //  Parameters:
    //    puSize:
    //      The size of the returned string array. It cannot be NULL.
    //
    //  Returns:
    //    An array of strings describing the socket type. The return value can
    //    be NULL, if the socket type is not set. In this case, *puSize will be
    //    zero. The return value will be NULL, if puSize is NULL.
    //
    //    Examples:
    //     - { "TCP, m=accepted" }; *puSize = 1
    //     - { "TLS, m=client", "TCP, m=client" }; *puSize =2
    //     - { "SIP", "TLS, m=server", "TCP, m=server" }; *puSize = 3
    //
    //  Description:
    //    This method returns the socket type as an array of strings. The array
    //    is in fact the same as the one given to the socket factory, except for
    //    accepted sockets on which it will be derived from the server socket
    //    type.
    //
    //  Note:
    //    If the application needs to keep a reference on the returned socket
    //    type array, it must take the precaution to copy the array's content
    //    (the const char* values and the strings themselves). This is because
    //    the socket's internal array might get reallocated at some point if
    //    the socket type is changed.
    //
    //    This method is synchronous. The IAsyncSocket will synchronize the
    //    various threads accessing it, thus there is no need to externally
    //    synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual const char* const* GetSocketType(OUT unsigned int* puSize) const = 0;

    //==========================================================================
    //==
    //==  SetSocketType
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the socket type from a string array.
    //
    //  Parameters:
    //    ppszType:
    //      The array of strings describing the socket type. The syntax of this
    //      parameter is the same as that of the apszType parameter to the
    //      CAsyncSocketFactory::CreateAsyncSocket method. It cannot be NULL.
    //
    //    uTypeSize:
    //      The number of elements in the string arrays passed in the ppszType
    //      parameter. This parameter must not be 0.
    //
    //  Returns:
    //      - resS_OK: The socket type has correctly been set.
    //      - resFE_INVALID_ARGUMENT: ppszType is NULL or uTypeSize is 0.
    //
    //  Description:
    //    This method sets the socket type of the socket. This method is
    //    normally only used by the CAsyncSocketFactory or the server sockets.
    //    It is called as the last step of the asynchronous socket creation
    //    process, that is, after all creation managers have been called and
    //    one of them returned a valid socket.
    //
    //  Note:
    //    Internally the socket allocates a new array and copies the string
    //    content. This internal array will be deleted and reallocated.
    //
    //    This method is synchronous. The IAsyncSocket will synchronize the
    //    various threads accessing it, thus there is no need to externally
    //    synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result SetSocketType(IN const char* const* ppszType,
                                     IN unsigned int uTypeSize) = 0;

    //==========================================================================
    //==
    //==  EraseAllUserInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Erases all user information stored in the socket.
    //
    //  Returns:
    //      - resS_OK: The user information has been correctly erased.
    //
    //  Description:
    //    This method erases all user information currently stored in the socket.
    //    After calling this method, all user information contained in the
    //    socket is no longer be available for retrieval using
    //    IAsyncSocket::GetUserInfo.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result EraseAllUserInfo() = 0;

    //==========================================================================
    //==
    //==  EraseUserInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Erases a specific user information stored in the socket.
    //
    //  Parameters:
    //    pszUserInfo:
    //      The identifier of the user information to remove from the socket. This
    //      This parameter must not be NULL.
    //
    //  Returns:
    //      - resS_OK: The user information has been correctly erased.
    //      - resFE_INVALID_ARGUMENT: pszUserInfo is NULL.
    //
    //  Description:
    //    This method erases the specified user information in the socket. If
    //    the call succeeds, the specified user information is no longer
    //    available for retrieval using IAsyncSocket::GetUserInfo.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result EraseUserInfo(IN const char* pszUserInfo) = 0;

    //==========================================================================
    //==
    //==  GetUserInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves a specific user information stored in the socket.
    //
    //  Parameters:
    //    pszUserInfo:
    //      The identifier of the user information to retrieve from the socket.
    //      This parameter must not be NULL.
    //
    //    pblob:
    //      A blob to receive the user information. This parameter must not be
    //      NULL.
    //
    //  Returns:
    //      - resS_OK: The user information has been correctly retrieved.
    //      - resFE_INVALID_ARGUMENT: pszUserInfo or pblob is NULL.
    //      - resFE_FAIL: The requested user information has not been found.
    //
    //  Description:
    //    This method retrieves the specified user information in the socket and
    //    copies it to the provided blob.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result GetUserInfo(IN const char* pszUserInfo,
                                   OUT CBlob* pblob) const = 0;

    //==========================================================================
    //==
    //==  InsertUserInfo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Inserts a specific user information to be stored in the socket.
    //
    //  Parameters:
    //    pszUserInfo:
    //      The identifier of the user information to insert in the socket. This
    //      parameter must not be NULL.
    //
    //    pblob:
    //      The user information content to insert in the socket. This parameter
    //      must not be NULL.
    //
    //  Returns:
    //      - resS_OK: The user information has been correctly inserted.
    //      - resSI_FW_NET_REPLACED_USER_INFO_ENTRY: The user information that
    //                                               already existed in the
    //                                               sockat has been replaced.
    //      - resFE_INVALID_ARGUMENT: pszUserInfo or pblob is NULL.
    //      - resFE_FAIL: The user information could not be inserted.
    //
    //  Description:
    //    This method inserts the specified user information in the socket by
    //    copying the provided blob to an internal one that is associated with
    //    the provided identifier. If there is already an entry with the same
    //    identifier present in the socket, the existing data associated with it
    //    is replaced by the new provided data and the method returns
    //    resSI_FW_NET_REPLACED_USER_INFO_ENTRY. If a new entry is effectively
    //    inserted, resS_OK is returned.
    //
    //    The user information mechanism is meant for applications to be able to
    //    identify the sockets when they are retrieved through
    //    CAsyncSocketFactory::GetSocketList.
    //
    //    Note that this method is synchronous. The IAsyncSocket will
    //    synchronize the various threads accessing it, thus there is no need
    //    to externally synchronize the access to the IAsyncSocket interface.
    //
    //==========================================================================
    virtual mxt_result InsertUserInfo(IN const char* pszUserInfo,
                                      IN const CBlob* pblob) = 0;

// BRCM_CUSTOM - [begin] Add user opaque to socket.
    virtual void GetUserOpaque(OUT CVector<mxt_opaque>* pvecOpq) = 0;
    virtual void AddUserOpaque(IN mxt_opaque opq) = 0;
    virtual void RemoveUserOpaque(IN mxt_opaque opq) = 0;
// BRCM_CUSTOM - [end] Add user opaque to socket.

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocket
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
    IAsyncSocket() {}

    //==========================================================================
    //==
    //==  ~IAsyncSocket
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
    virtual ~IAsyncSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocket(const IAsyncSocket& from);
    IAsyncSocket& operator=(const IAsyncSocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKET_H
