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
#ifndef MXG_IASYNCUNCONNECTEDIOSOCKETMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCUNCONNECTEDIOSOCKETMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncUnconnectedIoSocketMgr
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    This is the interface through which the unconnected asynchronous socket
//    implementations report their events.
//
//  Description:
//    This is the interface through which the unconnected asynchronous socket
//    implementations report their events. All events are reported
//    asynchronously with respect to the manager's execution context.
//
//  Location:
//    Network/IAsyncUnconnectedIoSocketMgr.h
//
//  See Also:
//    IAsyncUnconnectedIoSocket
//
//==============================================================================
class IAsyncUnconnectedIoSocketMgr
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvAsyncUnconnectedIoSocketMgrReadyToRecv
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the socket user that there is data available to be received
    //    on the socket.
    //
    //  Parameters:
    //    opq:
    //      Opaque parameter associated with the socket.
    //
    //  Description:
    //    This event is reported by a socket when there is data available on
    //    the socket for reading.
    //
    //    Note that there might be some implementations of
    //    IAsyncUnconnectedIoSocket that do generate this event with a call to
    //    IAsyncUnconnectedIoSocket::RecvFrom yielding zero bytes received.
    //    Thus, the socket user should be ready to handle this case.
    //
    //  See Also:
    //    IAsyncIoSocket::Recv
    //    IAsyncUnconnectedIoSocket::RecvFrom
    //
    //==========================================================================
    virtual void EvAsyncUnconnectedIoSocketMgrReadyToRecv(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncUnconnectedIoSocketMgrReadyToSend
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the socket user that the socket is ready to send data on
    //    the network.
    //
    //  Parameters:
    //    opq:
    //      Opaque parameter associated with the socket.
    //
    //  Description:
    //    This event is reported by a socket when it is possible for its user
    //    to send data on the network. Thus, after receiving this, the socket
    //    user should be able to call IAsyncUnconnectedIoSocket::SendTo.
    //
    //    Note that there might be some implementations of
    //    IAsyncUnconnectedIoSocket that do generate this event even if a call
    //    to IAsyncUnconnectedIoSocket::SendTo returns with non zero bytes
    //    sent. The socket user must be able to handle this case.
    //
    //  See Also:
    //    IAsyncIoSocket::Send
    //    IAsyncUnconnectedIoSocket::SendTo
    //
    //==========================================================================
    virtual void EvAsyncUnconnectedIoSocketMgrReadyToSend(IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncUnconnectedIoSocketMgr
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
    IAsyncUnconnectedIoSocketMgr() {}

    //==========================================================================
    //==
    //==  ~IAsyncUnconnectedIoSocketMgr
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
    virtual ~IAsyncUnconnectedIoSocketMgr() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncUnconnectedIoSocketMgr(const IAsyncUnconnectedIoSocketMgr& from);
    IAsyncUnconnectedIoSocketMgr& operator=(const IAsyncUnconnectedIoSocketMgr& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCUNCONNECTEDIOSOCKETMGR_H
