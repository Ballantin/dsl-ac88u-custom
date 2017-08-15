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
#ifndef MXG_IASYNCUNCONNECTEDIOSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCUNCONNECTEDIOSOCKET_H
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
class CBlob;
class CSocketAddr;
class IAsyncUnconnectedIoSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IAsyncUnconnectedIoSocket);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncUnconnectedIoSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Interface defining the methods accessible on unconnected asynchronous
//    sockets to send and receive data.
//
//  Description:
//    The IAsyncUnconnectedIoSocket interface defines the methods accessible on
//    an unconnected asynchronous socket that can be used to send and receive
//    data.
//
//    Events related to this interface are reported through the
//    IAsyncUnconnectedIoSocketMgr interface:
//
//    - The IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToRecv
//      event is generated when there is data ready for reception on the socket.
//      This means that calling IAsyncUnconnectedIoSocket::RecvFrom should yield
//      incoming data. IAsyncUnconnectedIoSocket::RecvFrom must be called
//      repeatedly until it returns with zero bytes received. If this condition
//      is not reached, the event will never be reported again.
//
//    - The IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToSend
//      event is generated once the socket becomes writable after a call to
//      IAsyncUnconnectedIoSocket::SendTo has returned successfully but with
//      less than the provided buffer size bytes sent.
//
//    Although the interface name seems to specify that the
//    IAsyncUnconnectedIoSocket::RecvFrom and IAsyncUnconnectedIoSocket::SendTo
//    methods are asynchronous, this is not the case.  These methods are
//    synchronous because the socket should not queue data. Queueing data in the
//    socket could end up with a non deterministic message queue allocation. The
//    asynchronous part is managed through the asynchronous notification of the
//    IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToRecv and
//    IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToSend. It
//    is guaranteed that these two events will never be reported to the manager
//    while a send or receive operation is in progress.
//
//  Location:
//    Network/IAsyncUnconnectedIoSocket.h
//
//  See Also:
//    IAsyncClientSocket
//    IAsyncIoSocket
//    IAsyncServerSocket
//    IAsyncSocket
//    IAsyncUnconnectedIoSocketMgr
//
//==============================================================================
class IAsyncUnconnectedIoSocket : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IAsyncUnconnectedIoSocket);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //== RecvFrom
    //==
    //==========================================================================
    //
    //  Summary:
    //    Receives data from an unconnected socket.
    //
    //  Parameters:
    //    pData:
    //      Blob object that will be filled with the data received on the
    //      socket. It cannot be NULL.
    //
    //    pPeerAddress:
    //      Address from which the data has been received. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is either not bound or connected.
    //    resFE_INVALID_ARGUMENT: pData or pPeerAddress is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to receive data from an unconnected socket.
    //    Although this method is usually called when the socket user has been
    //    notified that there is data available on the socket, it can also be
    //    called without having received such a notification. The
    //    IAsyncUnconnectedIoSocket implementation notifies its user of the
    //    presence of incoming data through the
    //    IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToRecv
    //    event.
    //
    //    The maximum number of bytes that can be received is defined by the
    //    blob's capacity. The user of the socket might want to resize the
    //    blob to a specific size before calling
    //    IAsyncUnconnectedIoSocket::RecvFrom, as its capacity will not be
    //    changed by this call.
    //
    //    Note that this method is synchronous. The IAsyncUnconnectedIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncUnconnectedIoSocket interface.
    //
    //  See Also:
    //    IAsyncIoSocket::Recv
    //
    //==========================================================================
    virtual mxt_result RecvFrom(OUT CBlob* pData, OUT CSocketAddr* pPeerAddress) = 0;

    //==========================================================================
    //==
    //==  RecvFrom
    //==
    //==========================================================================
    //
    //  Summary:
    //    Receives data from an unconnected socket.
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
    //    pPeerAddress:
    //      Address from which the data has been received. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is either not bound or connected.
    //    resFE_INVALID_ARGUMENT: puData, puSize or pPeerAddress is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to receive data from an unconnected socket.
    //    Although this method is usually called when the socket user has been
    //    notified that there is data available on the socket, it can also be
    //    called without having received such a notification. The
    //    IAsyncUnconnectedIoSocket implementation notifies its user of the
    //    presence of incoming data through the
    //    IAsyncUnconnectedIoSocketMgr::EvAsyncUnconnectedIoSocketMgrReadyToRecv
    //    event.
    //
    //    Note that this method is synchronous. The IAsyncUnconnectedIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncUnconnectedIoSocket interface.
    //
    //  See Also:
    //    IAsyncIoSocket::Recv
    //
    //==========================================================================
    virtual mxt_result RecvFrom(OUT uint8_t* puData,
                                IN unsigned int uCapacity,
                                OUT unsigned int* puSize,
                                OUT CSocketAddr* pPeerAddress) = 0;

    //==========================================================================
    //==
    //==  SendTo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sends data on an unconnected socket.
    //
    //  Parameters:
    //    pData:
    //      Pointer to a blob containing the data to be sent. It cannot be NULL.
    //
    //    puSizeSent:
    //      On output, will contain the number of bytes that were actually
    //      sent. It cannot be NULL. The value returned in this parameter is
    //      not reliable when the method returns anything other than resS_OK.
    //
    //    pPeerAddress:
    //      Address to which the data must be sent. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is either not bound or connected.
    //    resFE_INVALID_ARGUMENT: pData, puSizeSent or pPeerAddress is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to send data on an unconnected socket.
    //
    //    Note that this method is synchronous. The IAsyncUnconnectedIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncUnconnectedIoSocket interface.
    //
    //    An application MUST not rely on the value of puSizeSent when this
    //    method returns anything other than resS_OK.
    //
    //  See Also:
    //    IAsyncIoSocket::Send
    //
    //==========================================================================
    virtual mxt_result SendTo(IN const CBlob* pData,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress) = 0;

    //==========================================================================
    //==
    //==  SendTo
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sends data on an unconnected socket.
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
    //      sent. It cannot be NULL. The value returned in this parameter is
    //      not reliable when the method returns anything other than resS_OK.
    //
    //    pPeerAddress:
    //      Address to which the data must be sent. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The socket is either not bound or connected.
    //    resFE_INVALID_ARGUMENT: puData, puSizeSent or pPeerAddress is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method is used to send data on an unconnected socket.
    //
    //    Note that this method is synchronous. The IAsyncUnconnectedIoSocket
    //    implementation will synchronize the various threads accessing it,
    //    thus there is no need to externally synchronize the access to the
    //    IAsyncUnconnectedIoSocket interface.
    //
    //    An application MUST not rely on the value of puSizeSent when this
    //    method returns anything else than resS_OK.
    //
    //  See Also:
    //    IAsyncIoSocket::Send
    //
    //==========================================================================
    virtual mxt_result SendTo(IN const uint8_t* puData,
                              IN unsigned int uSize,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress) = 0;

    //==========================================================================
    //==
    //==  SetAsyncUnconnectedIoSocketMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the manager associated with the interface
    //    IAsyncUnconnectedIoSocket.
    //
    //  Parameters:
    //    pMgr:
    //      Pointer to the manager. It cannot be NULL.
    //
    //  Returns:
    //    resFE_INVALID_STATE: The manager has probably already been set.
    //    resFE_INVALID_ARGUMENT: pMgr is NULL.
    //    See GetSocketErrorId for possible return values.
    //
    //  Description:
    //    This method sets the manager associated with the interface
    //    IAsyncUnconnectedIoSocket.
    //
    //==========================================================================
    virtual mxt_result SetAsyncUnconnectedIoSocketMgr(IN IAsyncUnconnectedIoSocketMgr* pMgr) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncUnconnectedIoSocket
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
    IAsyncUnconnectedIoSocket() {}

    //==========================================================================
    //==
    //==  ~IAsyncUnconnectedIoSocket
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
    virtual ~IAsyncUnconnectedIoSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncUnconnectedIoSocket(const IAsyncUnconnectedIoSocket& from);
    IAsyncUnconnectedIoSocket& operator=(const IAsyncUnconnectedIoSocket& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) && 
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCUNCONNECTEDIOSOCKET_H
