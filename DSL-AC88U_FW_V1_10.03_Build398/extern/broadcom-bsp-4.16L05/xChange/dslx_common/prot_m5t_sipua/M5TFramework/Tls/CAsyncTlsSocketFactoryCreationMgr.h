//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CASYNCTLSSOCKETFACTORYCREATIONMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSOCKETFACTORYCREATIONMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

// M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    !defined (MXD_TLS_NONE)

// Interface
#ifndef MXG_IASYNCSOCKETFACTORYCREATIONMGR_H
#include "Network/IAsyncSocketFactoryCreationMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

// Forward declarations
class IAsyncSocket;
class IEComUnknown;

//==============================================================================
//== Class: CAsyncTlsSocketFactoryCreationMgr
//==============================================================================
//<GROUP TLS_CLASSES>
//
// Summary:
//    This class implements a TLS Socket Factory creation manager.
//
// Description:
//    This class implements a TLS Socket Factory creation manager. It
//    implements the IAsyncSocketFactoryCreationMgr interface, and is
//    automatically registered to the socket factory when
//    MXD_ECOM_ENABLE_SUPPORT, MXD_SERVICING_THREAD_ENABLE_SUPPORT and
//    MXD_TLS_OPENSSL are enabled.
//
//    The IAsyncSocketFactoryCreationMgr::EvCreationRequested method creates a
//    TLS asynchronous socket, and MUST only be called by the Socket Factory.
//    The type of the asynchronous TLS socket to be created is provided as a
//    sequence of network protocols starting from the highest level down to the
//    lowest level. Each level is possibly followed by arguments.
//
//    The only type of socket handled by this creation handler are TLS
//    over TCP sockets. Moreover, the TLS token MUST have a "m=" option
//    specifying whether a client or server socket is to be created. The TCP
//    token can have an "m=" option in which case it must be the same as that
//    of the TLS token. The "m=" option in the TCP token can be omitted.
//
//    If the type of the socket to be created is not handled by this creation
//    manager, the method returns resSI_FALSE, so that the socket creation
//    can be handled by another creation manager.
//
//    Examples of sockets handled by the TLS creation manager:
//
//    <CODE>
//      { "SIP", "TLS, m=client", "TCP" }, 3
//      { "SIP", "TLS, m=client", "TCP, m=client" }, 3
//      { "SIP", "TLS, m=server", "TCP" }, 3
//      { "SIP", "TLS, m=server", "TCP, m=server" }, 3
//    </CODE>
//
// Location:
//   Tls/CAsyncTlsSocketFactoryCreationMgr.h
//
// See Also:
//   IAsyncSocketFactoryCreationMgr, CAsyncTlsSocket, CAsyncTlsServerSocket
//
//==============================================================================
class CAsyncTlsSocketFactoryCreationMgr : public IAsyncSocketFactoryCreationMgr
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CAsyncTlsSocketFactoryCreationMgr();

    // Summary:
    //  Destructor.
    virtual ~CAsyncTlsSocketFactoryCreationMgr();

    //-- Inherited from IAsyncSocketFactoryCreationMgr.

    virtual mxt_result EvCreationRequested(IN IEComUnknown* pServicingThread,
                                           IN const char* const* apszType,
                                           IN unsigned int uTypeSize,
                                           OUT IAsyncSocket** ppAsyncSocket);

//M5T_INTERNAL_USE_BEGIN
private:
    // Friends declarations
    friend mxt_result InitializeCAsyncTlsSocketFactoryCreationMgr();
    friend void FinalizeCAsyncTlsSocketFactoryCreationMgr();

    static mxt_result InitializeCAsyncTlsSocketFactoryCreationMgr();
    static void FinalizeCAsyncTlsSocketFactoryCreationMgr();
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     !defined (MXD_TLS_NONE)

#endif // #ifndef MXG_CASYNCTLSSOCKETFACTORYCREATIONMGR_H


