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
#ifndef MXG_IASYNCIOSOCKETMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCIOSOCKETMGR_H
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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncIoSocketMgr
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    This is the interface through which the asynchronous I/O sockets report
//    their events.
//
//  Description:
//    This is the interface through which the asynchronous I/O sockets report
//    their events. All events are reported asynchronously with respect to the
//    manager's execution context.
//
//  Location:
//    Network/IAsyncIoSocketMgr.h
//
//  See Also:
//    IAsyncIoSocket
//
//==============================================================================
class IAsyncIoSocketMgr
{
//-- Published Interface
public:
    //==========================================================================
    //==
    //==  EvAsyncIoSocketMgrReadyToRecv
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the socket user that there is data available to be received
    //    on the socket.
    //
    //  Parameters:
    //    opq:
    //      Opaque parameter associated to the socket.
    //
    //  Description:
    //    This event is reported by a socket when there is data available on
    //    the socket for reading. This means that the socket user can call
    //    IAsyncIoSocket::Recv or IAsyncUnconnectedIoSocket::RecvFrom at least
    //    once without blocking on the socket.
    //
    //    Note that there might be some instances of sockets (depending on the
    //    protocol) that do generate this event with a call to Recv or
    //    RecvFrom yielding zero bytes read. Thus, the socket user should be
    //    ready to handle this case.
    //
    //  See Also:
    //    IAsyncIoSocket::Recv@CBlob*,
    //    IAsyncIoSocket::Recv@uint8_t*,unsigned int, unsigned int*,
    //    IAsyncIoSocket::RecvFrom@CBlob*,CSocketAddr*,
    //    IAsyncIoSocket::RecvFrom@uint8_t*,unsigned int, unsigned int*,CSocketAddr*,
    //
    //==========================================================================
    virtual void EvAsyncIoSocketMgrReadyToRecv(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncIoSocketMgrReadyToSend
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies the socket user that the socket is ready to send data on
    //    the network.
    //
    //  Parameters:
    //    opq:
    //      Opaque parameter associated to the socket.
    //
    //  Description:
    //    This event is reported by a socket when it is possible for its user
    //    to send data on the network. Thus, after receiving this, the socket
    //    user should be able to use the various flavors of IAsyncIoSocket::Send
    //    and IAsyncUnconnectedIoSocket::SendTo to send the data on the network.
    //
    //    Note that there might be some instances of sockets (depending on the
    //    protocol) that do generate this event even if a call to
    //    IAsyncIoSocket::Send and IAsyncUnconnectedIoSocket::SendTo
    //    will fail with eFE_SOCK_WOULDBLOCK and zero bytes written. The
    //    socket user must be able to handle this case.
    //
    //  See Also:
    //    IAsyncIoSocket::Send@CBlob*
    //    IAsyncIoSocket::Send@uint8_t*, unsigned int
    //    IAsyncIoSocket::SendTo@CBlob*, CSocketAddr*
    //    IAsyncIoSocket::SendTo@uint8_t*, unsigned int, CSocketAddr*
    //
    //==========================================================================
    virtual void EvAsyncIoSocketMgrReadyToSend(IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncIoSocketMgr
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
    IAsyncIoSocketMgr(){};

    //==========================================================================
    //==
    //==  ~IAsyncIoSocketMgr
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
    virtual ~IAsyncIoSocketMgr(){};

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncIoSocketMgr(const IAsyncIoSocketMgr& from);
    IAsyncIoSocketMgr& operator=(const IAsyncIoSocketMgr& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

#endif // MXG_IASYNCIOSOCKETMGR_H
