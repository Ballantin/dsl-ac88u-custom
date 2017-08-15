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
#ifndef MXG_IASYNCIOSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCIOSOCKET_H
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
//-------------------------
class CBlob;
class CSocketAddr;
class IAsyncIoSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncIoSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncIoSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the methods accessible on connected asynchronous
//    sockets to send and receive data.
//
//  Description:
//    The IAsyncIoSocket interface defines the methods accessible on a connected
//    asynchronous socket that can be used to send and receive data.
//
//    Events related to this interface are reported through the
//    IAsyncIoSocketMgr interface:
//
//    - The event IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToRecv is generated
//    when there is data ready for reception on the socket. This means that
//    calling IAsyncIoSocket::Recv should yield incoming data.
//    IAsyncIoSocket::Recv must be called repeatedly until it returns with
//    zero bytes received. If this condition is not reached, the event will
//    never be generated again.
//
//    - The event IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToSend is generated
//    once the socket becomes writable after a call to IAsyncIoSocket::Send has
//    returned successfully but with less than the provided buffer size bytes
//    sent. This means that sending of buffer of 10 bytes from which 0 to 9
//    bytes were sent will generate the event to notify the application that
//    the socket is ready to send new data. At that point the application can
//    send the remaining 1 byte.
//
//    Although the interface name seems to specify that the IAsyncIoSocket::Recv
//    and IAsyncIoSocket::Send methods are asynchronous, this is not the case.
//    These methods are synchronous because the socket should not queue data.
//    Queueing data in the socket could end up with a non deterministic message
//    queue allocation. The asynchronous part is managed through the
//    asynchronous notification of the
//    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToRecv and
//    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToSend. It is guaranteed that
//    these two events will never be reported to the manager while a send or
//    receive operation is in progress.
//
//  Location:
//    Network/IAsyncIoSocket.h
//
//  See Also:
//    IAsyncClientSocket
//    IAsyncIoSocketMgr
//    IAsyncServerSocket
//    IAsyncSocket
//    IAsyncUnconnectedIoSocket
//
//==============================================================================
class IAsyncIoSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncIoSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //== GetPeerAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //    Retrieves the remote address to which the socket is connected.
    //
    //  Parameters:
    //    pPeerAddress:
    //      OUT parameter used to receive the address. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not connected.
    //    resFE_INVALID_ARGUMENT: pPeerAddress is NULL.
    //    resS_OK: pPeerAddress holds the peer information.
    //
    //  Description:
    //    This method is used to retrieve the address to which the socket is
    //    currently connected. It can return an error for unconnected sockets.
    //
    //==========================================================================
    virtual mxt_result GetPeerAddress(OUT CSocketAddr* pPeerAddress) const = 0;

    //==========================================================================
    //==
    //== Recv
    //==
    //==========================================================================
    //
    //  Summary:
    //    Receives data from a connected socket.
    //
    //  Parameters:
    //    pData:
    //      Blob object that will be filled with the data received on the
    //      socket. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not connected.
    //    resFE_INVALID_ARGUMENT: pData is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to receive data from a connected socket.
    //    Although this method is usually called when the socket user is
    //    notified that there is data available on the socket, it can also be
    //    called without having received such a notification. The
    //    IAsyncIoSocket implementation notifies its user of the presence of
    //    incoming data through the
    //    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToRecv event.
    //
    //    The maximum number of bytes that can be received is defined by the
    //    blob's capacity. The user of the socket might want to resize the
    //    blob to a specific size before calling IAsyncIoSocket::Recv, as its
    //    capacity will not be changed by this call.
    //
    //    Note that this method is synchronous. The IAsyncIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncIoSocket interface.
    //
    //  See Also:
    //    IAsyncUnconnectedIoSocket::RecvFrom
    //
    //==========================================================================
    virtual mxt_result Recv(OUT CBlob* pData) = 0;

    //==========================================================================
    //==
    //== Recv
    //==
    //==========================================================================
    //
    //  Summary:
    //    Receives data from a connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer where the received data is to be stored. It
    //      cannot be NULL.
    //
    //    uCapacity:
    //      The maximum size available in the buffer pointed to by puData.
    //
    //    puSize:
    //      OUT parameter that contains the number of bytes actually written in
    //      the buffer. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not connected.
    //    resFE_INVALID_ARGUMENT: puData or puSize is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to receive data from a connected socket.
    //    Although this method is usually called when the socket user is
    //    notified that there is data available on the socket, it can also be
    //    called without having received such a notification. The
    //    IAsyncIoSocket implementation notifies its user of the presence of
    //    incoming data through the
    //    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToRecv event.
    //
    //    Note that this method is synchronous. The IAsyncIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncIoSocket interface.
    //
    //  See Also:
    //    IAsyncUnconnectedIoSocket::RecvFrom
    //
    //==========================================================================
    virtual mxt_result Recv(OUT uint8_t* puData,
                            IN unsigned int uCapacity,
                            OUT unsigned int* puSize) = 0;

    //==========================================================================
    //==
    //==  Send
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sends data on a connected socket.
    //
    //  Parameters:
    //    pData:
    //      Pointer to a blob object that contains the data to be sent. It
    //      cannot be NULL.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually
    //      sent. It cannot be NULL. The value returned in this parameter is not
    //      reliable if the method returns anything else than resS_OK.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not connected.
    //    resFE_INVALID_ARGUMENT: pData or puSizeSent is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to send data on a connected socket. Note that the
    //    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToSend event will be
    //    reported to the manager if this method succeeds and returns with
    //    *puSizeSent >= 0 and < to the pData size.
    //
    //    Note that this method is synchronous. The IAsyncIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncIoSocket interface.
    //
    //    An application MUST not rely on the value of puSizeSent when this
    //    method returns anything else than resS_OK.
    //
    //  See Also:
    //    IAsyncUnconnectedIoSocket::SendTo
    //
    //==========================================================================
    virtual mxt_result Send(IN const CBlob* pData,
                            OUT unsigned int* puSizeSent) = 0;

    //==========================================================================
    //==
    //==  Send
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sends data on a connected socket.
    //
    //  Parameters:
    //    puData:
    //      Pointer to a buffer containing the data to send. It cannot be NULL.
    //
    //    uSize:
    //      The number of bytes in the buffer that must be sent.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually
    //      sent. It cannot be NULL. The value returned in this parameter is not
    //      reliable if the method returns anything else than resS_OK.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is not connected.
    //    resFE_INVALID_ARGUMENT: puData or puSizeSent is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to send data on a connected socket. Note that the
    //    IAsyncIoSocketMgr::EvAsyncIoSocketMgrReadyToSend event will be
    //    reported to the manager if this method succeeds and returns with
    //    *puSizeSent >= 0 and < to the pData size.
    //
    //    Note that this method is synchronous. The IAsyncIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncIoSocket interface.
    //
    //    An application MUST not rely on the value of puSizeSent when this
    //    method returns anything else than resS_OK.
    //
    //  See Also:
    //    IAsyncUnconnectedIoSocket::SendTo
    //
    //==========================================================================
    virtual mxt_result Send(IN const uint8_t* puData,
                            IN unsigned int uSize,
                            OUT unsigned int* puSizeSent) = 0;

    //==========================================================================
    //==
    //== SetAsyncIoSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the manager associated with the interface IAsyncIoSocket
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
    //    IAsyncIoSocket.
    //
    //==========================================================================
    virtual mxt_result SetAsyncIoSocketMgr(IN IAsyncIoSocketMgr* pMgr) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncIoSocket
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
    IAsyncIoSocket() {}

    //==========================================================================
    //==
    //==  ~IAsyncIoSocket
    //==
    //==========================================================================
    //
    //   Summary:
    //     Destructor.
    //
    //  Description:
    //     Destructor.
    //
    //==========================================================================
    virtual ~IAsyncIoSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncIoSocket(const IAsyncIoSocket& from);
    IAsyncIoSocket& operator=(const IAsyncIoSocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)


#endif // MXG_IASYNCIOSOCKET_H
