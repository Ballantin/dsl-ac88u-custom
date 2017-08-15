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
#ifndef MXG_IPRIVATEASYNCTLSSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IPRIVATEASYNCTLSSOCKET_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

//-- Parameters used by Value

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CTlsContext;
class IAsyncIoSocket;
class IAsyncTlsSocketAcceptMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

MX_DECLARE_ECOM_IID(IPrivateAsyncTlsSocket);

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IPrivateAsyncTlsSocket
//==============================================================================
//
//  Description:
//    This private ECOM interface is necessary for the asynchronous TLS socket
//    and asynchronous TLS server socket to communicate with each other to
//    complete the connection acceptation process.
//
//  Location:
//    Tls/IPrivateAsyncTlsSocket.h
//
//  See Also:
//   AcceptA
//
//==============================================================================
class IPrivateAsyncTlsSocket : public IEComUnknown
{
//-- Published Interface
public:
    MX_DECLARE_ECOM_GETIID(IPrivateAsyncTlsSocket);

    //==========================================================================
    //==
    //== SetTcpSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //    Sets the underlying TCP socket for an accepted TLS socket.
    //
    //  Parameters:
    //    pAsyncIoSocket:
    //      The IO socket interface of the accepted TCP socket. It must not be
    //      NULL.
    //
    //  Returns:
    //      - resFE_INVALID_STATE: The internal TCP socket is not correctly set.
    //      - resFE_INVALID_ARGUMENT: The pAsyncTcpSocket parameter is NULL.
    //      - resS_OK: Method processed successfully.
    //
    //  Description:
    //      This method is called by CAsyncTlsServerSocket when it needs to
    //      provide the accepted TCP socket to the accepted TLS socket.
    //
    //==========================================================================
    virtual mxt_result SetTcpSocket(IN IAsyncIoSocket* pAsyncIoSocket) = 0;

    //==========================================================================
    //==
    //== PerformTlsHandshakingA
    //==
    //==========================================================================
    //
    //  Summary:
    //    Performs the TLS handshaking.
    //
    //  Parameters:
    //    pTlsContext:
    //      The TLS context used for this TLS connection.
    //
    //    opq:
    //      In the OpenSSL version of the implementation this is the SSL_CTX*.
    //      In the MocanaSS version of the implementation this is unused.
    //
    //  Returns:
    //      - resS_OK: Method processed successfully. All errors are reported
    //                 through the accepted socket's manager.
    //
    //  Description:
    //      This method is called by CAsyncTlsServerSocket to instruct the
    //      accepted TLS socket to perform the TLS handshaking.
    //
    //==========================================================================
    virtual mxt_result PerformTlsHandshakingA(IN CTlsContext* pTlsContext,
                                              IN mxt_opaque opq) = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    //==========================================================================
    //==
    //==  IPrivateAsyncTlsSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //   Constructor
    //
    //  Description:
    //   Constructor
    //
    //==========================================================================
    IPrivateAsyncTlsSocket() {}

    //==========================================================================
    //==
    //==  ~IPrivateAsyncTlsSocket
    //==
    //==========================================================================
    //
    //  Summary:
    //   Destructor
    //
    //  Description:
    //   Destructor
    //
    //==========================================================================
    virtual ~IPrivateAsyncTlsSocket() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    IPrivateAsyncTlsSocket(const IPrivateAsyncTlsSocket& from);
    IPrivateAsyncTlsSocket& operator=(const IPrivateAsyncTlsSocket& from);
};

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //#ifndef MXG_IPRIVATEASYNCTLSSOCKET_H

