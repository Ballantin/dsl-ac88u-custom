//==============================================================================
//==============================================================================
//
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation (“Media5”)
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CASYNCTLSSERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSERVERSOCKET_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_TLS_OPENSSL)

//-- Data Members
#ifndef MXG_CTLS_H
#include "Tls/CTls.h"
#endif // #ifndef MXG_CTLS_H

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif // #ifndef MXG_CVECTOR_H

#ifndef MXG_CASYNCTLSSOCKETOPENSSL_H
#include "Tls/CAsyncTlsSocketOpenSsl.h"
#endif // #ifndef MXG_CASYNCTLSSOCKETOPENSSL_H

//-- Parameters used by Value

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IASYNCSERVERSOCKET_H
#include "Network/IAsyncServerSocket.h"
#endif

#ifndef MXG_IASYNCSOCKET_H
#include "Network/IAsyncSocket.h"
#endif

#ifndef MXG_IASYNCSERVERSOCKETMGR_H
#include "Network/IAsyncServerSocketMgr.h"
#endif

#ifndef MXG_IASYNCSOCKETMGR_H
#include "Network/IAsyncSocketMgr.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_IASYNCTLSSERVERSOCKET_H
#include "Tls/IAsyncTlsServerSocket.h"
#endif

#ifndef MXG_CASYNCTLSSERVERSOCKETBASE_H
#include "Tls/CAsyncTlsServerSocketBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSocketAddr;
class IAsyncIoSocket;
class IAsyncTlsServerSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAsyncTlsServerSocket
//========================================
//
// Summary:
//   Implementation of a TLS asynchronous server socket.
//
// Description:
//   The CAsyncTlsServerSocket class is an ECOM implementation of an
//   asynchronous TLS socket in server mode that offers numerous ECOM interfaces
//   related to manipulating TLS sockets.
//
// Location:
//   Tls/CAsyncTlsServerSocket.h
//
// See Also:
//   CAsyncTcpServerSocket
//
//==============================================================================
class CAsyncTlsServerSocket : protected CAsyncTlsServerSocketBase
{
// Published Interface
public:
    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Published methods through inherited interfaces
protected:
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

//-- Hidden Methods
private:
    // Inherited from IAsyncServerSocketMgr.

    friend mxt_result InitializeCAsyncTlsServerSocket();
    friend void FinalizeCAsyncTlsServerSocket();

    CAsyncTlsServerSocket(IN IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CAsyncTlsServerSocket();

// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncTlsServerSocket(const CAsyncTlsServerSocket& rFrom);
    CAsyncTlsServerSocket& operator=(const CAsyncTlsServerSocket& rFrom);
#endif

    // Helper methods.
    void ReleaseResources(IN bool bClose);
    virtual mxt_result PerformTlsHandShake(IN IAsyncSocket* pAsyncAcceptedTlsSocket,
                                           IN IAsyncIoSocket* pAsyncTcpIoSocket);

// Hidden Data Members
//---------------------
protected:
    // Server SSL context, mainly used for session caching.
    SSL_CTX* m_pSslCtx;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_TLS_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTLSSERVERSOCKET_H

