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
#ifndef MXG_IASYNCSOCKETMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IASYNCSOCKETMGR_H
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

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IAsyncSocketMgr
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Defines basic events that can be reported by all types of asynchronous
//    sockets.
//
//  Description:
//    This is the basic asynchronous socket manager interface that must be
//    inherited by all users of asynchronous sockets.
//
//    This interface defines basic events that can be reported by all types of
//    asynchronous sockets.
//
//  Location:
//    Network/IAsyncSocketMgr.h
//
//  See Also:
//   IAsyncClientSocket
//   IAsyncIoSocket
//   IAsyncServerSocket
//   IAsyncSocket
//   IAsyncUnconnectedIoSocket
//
//==============================================================================
class IAsyncSocketMgr
{
public:
    //==========================================================================
    //==
    //==  EvAsyncSocketMgrClosed
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies of the closure of the socket.
    //
    //  Parameters:
    //    opq:
    //      Opaque value associated with the socket that has been closed.
    //
    //  Description:
    //    This event is reported by the socket after it has been locally
    //    closed.
    //
    //  See Also:
    //    IAsyncSocket::CloseA
    //
    //==========================================================================
    virtual void EvAsyncSocketMgrClosed(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncSocketMgrClosedByPeer
    //==
    //==========================================================================
    //
    //  Summary:
    //    Notifies of the closure of the socket by the peer.
    //
    //  Parameters:
    //    opq:
    //      Opaque value associated with this socket that has been closed.
    //
    //  Description:
    //    This event is reported by the socket after it has been closed by the
    //    peer to which it was connected.
    //
    //    Note that this can be reported for sockets of type eSTREAM and
    //    eSEQPACKET.
    //
    //  See Also:
    //    IAsyncSocket::CloseA
    //
    //==========================================================================
    virtual void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq) = 0;

    //==========================================================================
    //==
    //==  EvAsyncSocketMgrErrorDetected
    //==
    //==========================================================================
    //
    //  Summary:
    //    Reports an error detected on the socket.
    //
    //  Parameters:
    //    opq:
    //      Opaque value associated with the socket in error.
    //
    //    res:
    //      The error detected on the socket.
    //
    //  Description:
    //    This event can be generated by a socket when it detects an error
    //    condition for an asynchronous call, like CloseA, ConnectA, AcceptA.
    //
    //==========================================================================
    virtual void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq,
                                               IN mxt_result res) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //==
    //==  IAsyncSocketMgr
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
    IAsyncSocketMgr(){};

    //==========================================================================
    //==
    //==  ~IAsyncSocketMgr
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
    virtual ~IAsyncSocketMgr(){};

private:
    // Deactivated Constructors / Destructors / Operators
    IAsyncSocketMgr(const IAsyncSocketMgr& from);
    IAsyncSocketMgr& operator=(const IAsyncSocketMgr& from);
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IASYNCSOCKETMGR_H
