//==============================================================================
//==============================================================================
//
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
#ifndef MXG_IPRIVATEASYNCTCPSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATEASYNCTCPSOCKET_H

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
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

#ifndef MXG_CTCPSOCKET_H
#include "Network/CTcpSocket.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncTcpSocketAcceptMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

MX_DECLARE_ECOM_IID(IPrivateAsyncTcpSocket);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateAsyncTcpSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Description:
//    This private ECOM interface is necessary for the asynchronous TCP socket
//    and asynchronous TCP server socket to communicate with each other to
//    complete the connection acceptation process of the client socket.
//
//  Location:
//    Network/IPrivateAsyncTcpSocket.h
//
//  See Also:
//   AcceptA
//
//==============================================================================
class IPrivateAsyncTcpSocket : public IEComUnknown
{
//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(IPrivateAsyncTcpSocket);

    //==========================================================================
    //==
    //== SetTcpSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //    Provides the accepted synchronous socket to the asynchronous accepted
    //    socket.
    //
    //  Parameters:
    //    pTcpSocket:
    //      The new socket that was created with CTcpServerSocket::Accept.
    //
    //  Returns:
    //      - resFE_INVALID_STATE
    //      - resFE_INVALID_ARGUMENT
    //      - See GetSocketErrorId for possible return values.
    //
    //  Description:
    //      This method is called by CAsyncTcpServerSocket when it needs to create
    //      a new asynchronous TCP socket. Its only purpose is to provide the
    //      underlying synchronous socket to the asynchronous one.
    //
    //==========================================================================
#if defined(MXD_OS_SYMBIAN)
    virtual mxt_result SetTcpSocket(IN TOS RSocket* pTcpSocket) = 0;
#else
    virtual mxt_result SetTcpSocket(IN TOS CTcpSocket* pTcpSocket) = 0;
#endif

protected:
    //==========================================================================
    //==
    //==  IPrivateAsyncTcpSocket
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
    IPrivateAsyncTcpSocket() {}

    //==========================================================================
    //==
    //==  IPrivateAsyncTcpSocket
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
    virtual ~IPrivateAsyncTcpSocket() {}

private:
    // Deactivated Constructors / Destructors / Operators
    IPrivateAsyncTcpSocket(const IPrivateAsyncTcpSocket& from);
    IPrivateAsyncTcpSocket& operator=(const IPrivateAsyncTcpSocket& from);
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END

#endif // MXG_IPRIVATEASYNCTCPSOCKET_H
