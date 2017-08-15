//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation (“Media5”)
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CASYNCTLSSERVERSOCKETBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSERVERSOCKETBASE_H

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
    !defined(MXD_TLS_NONE)

//-- Data Members
#ifndef MXG_CTLS_H
#include "Tls/CTls.h"
#endif // #ifndef MXG_CTLS_H

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif // #ifndef MXG_CVECTOR_H

//-- Parameters used by Value
// For mxt_hSocket.
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

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
//== Class: CAsyncTlsServerSocketBase
//========================================
//
// Summary:
//   Implementation of a TLS asynchronous server socket.
//
// Description:
//   The CAsyncTlsServerSocketBase class is an ECOM implementation of an
//   asynchronous TLS socket in server mode that offers numerous ECOM interfaces
//   related to manipulating TLS sockets.
//
// Location:
//   Tls/CAsyncTlsServerSocketBase.h
//
// See Also:
//   CAsyncTcpServerSocket
//
//==============================================================================
class CAsyncTlsServerSocketBase :  protected CEComDelegatingUnknown,
                                   protected CEventDriven,
                                   protected IAsyncTlsServerSocket,
                                   protected IAsyncServerSocket,
                                   protected IAsyncSocket,
                                   protected IAsyncServerSocketMgr,
                                   protected IAsyncSocketMgr
{
// Published Interface
public:
    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN


//-- Published methods through inherited interfaces
protected:
    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result InitializeInstance();
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IAsyncTlsServerSocket.
    virtual mxt_result GetAcceptedTlsContext(OUT CTlsContext* pAcceptedTlsContext) const;
    virtual mxt_result SetAcceptedTlsContext(IN const CTlsContext* pTlsContext);

    // Inherited from IAsyncServerSocket.
    virtual mxt_result AcceptA(IN IEComUnknown* pServicingThread);
    virtual mxt_result BindA(IN const CSocketAddr* pLocalAddress);
    virtual mxt_result Listen(IN unsigned int uMaxPendingConnection = 5);
    virtual mxt_result QueryAcceptedOptionsIf(IN mxt_iid iidRequested,
                                              OUT void** ppInterface);
    virtual mxt_result SetAsyncServerSocketMgr(IN IAsyncServerSocketMgr* pMgr);

    // Inherited from IAsyncSocket.
    virtual mxt_result Activate(IN IEComUnknown* pServicingThread);
    virtual mxt_result CloseA(IN ISocket::ECloseBehavior eCloseBehavior);
    virtual mxt_result GetHandle(OUT mxt_hSocket* phSocket) const;
    virtual mxt_result GetLocalAddress(OUT CSocketAddr* pLocalAddress) const;
    virtual mxt_result GetLocalInterfaceAddress(OUT CSocketAddr* pLocalInterfaceAddress) const;
    virtual mxt_result GetOpaque(OUT mxt_opaque* popq) const;
    virtual mxt_result GetServicingThreadIEcomUnknown(OUT IEComUnknown** ppIEComUnknown) const;
    virtual mxt_result GetSocketType(OUT ISocket::ESocketType* peSocketType) const;
    virtual mxt_result SetAsyncSocketMgr(IN IAsyncSocketMgr* pMgr);
    virtual mxt_result SetOpaque(IN mxt_opaque opq);
    virtual const char* const* GetSocketType(OUT unsigned int* puSize) const;
    virtual mxt_result SetSocketType(IN const char* const* ppszType,
                                     IN unsigned int uTypeSize);
    virtual mxt_result EraseAllUserInfo();
    virtual mxt_result EraseUserInfo(IN const char* pszUserInfo);
    virtual mxt_result GetUserInfo(IN const char* pszUserInfo,
                                   OUT CBlob* pblob) const;
    virtual mxt_result InsertUserInfo(IN const char* pszUserInfo,
                                      IN const CBlob* pblob);
// BRCM_CUSTOM - [begin] Add user opaque to socket.
    virtual void GetUserOpaque(OUT CVector<mxt_opaque>* pvecOpq );
    virtual void AddUserOpaque(IN mxt_opaque opq);
    virtual void RemoveUserOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add user opaque to socket.

//-- Hidden Methods
protected:
    CAsyncTlsServerSocketBase(IN IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CAsyncTlsServerSocketBase();

// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncTlsServerSocketBase(const CAsyncTlsServerSocketBase& rFrom);
    CAsyncTlsServerSocketBase& operator=(const CAsyncTlsServerSocketBase& rFrom);
#endif

    // Inherited from CEComDelegatingUnknown.
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);
    virtual unsigned int NonDelegatingReleaseIfRef();

    // Inherited from CEventDriven.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    // Inherited from IAsyncSocketMgr.
    virtual void EvAsyncSocketMgrClosed(IN mxt_opaque opq);
    virtual void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq);
    virtual void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq,
                                               IN mxt_result res);

    // Inherited from IAsyncServerSocketMgr.
    virtual void EvAsyncServerSocketMgrBound(IN mxt_opaque opq,
                                             IN CSocketAddr* pEffectiveLocalAddress);
    virtual void EvAsyncServerSocketMgrConnectionAccepted(IN mxt_opaque opqServerSocketOpaque,
                                                          IN IAsyncIoSocket* pAsyncIoSocket);

    virtual void EvAsyncServerSocketMgrConnectionFailed(IN mxt_opaque opqServerSocketOpaque,
                                                        IN mxt_result res);

    virtual void EvAsyncServerSocketMgrConnectionRequested(IN mxt_opaque opqServerSocketOpaque);

    // Support for IAsyncTlsServerSocket
    virtual void InternalGetAcceptedTlsContext(IN CMarshaler* pParameter);
    virtual void InternalSetAcceptedTlsContext(IN CMarshaler* pParameter);

    // Support for IAsyncServerSocket
    virtual void InternalAcceptA(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncServerSocketMgr(IN CMarshaler* pParameter);

    // Support for IAsyncSocket
    virtual void InternalGetOpaque(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncSocketMgr(IN CMarshaler* pParameter);
    virtual void InternalSetOpaque(IN CMarshaler* pParameter);

    // Helper methods.
    virtual void ReleaseResources(IN bool bClose);

    virtual mxt_result PerformTlsHandShake(IN IAsyncSocket* pAsyncAcceptedTlsSocket,
                                           IN IAsyncIoSocket* pAsyncTcpIoSocket) = 0;

// Hidden Data Members
//---------------------
protected:
    mxt_opaque m_opq;

    // Summary:
    //   This is the default TLS context that is passed to the accepted socket
    //   when it is accepted.
    CTlsContext* m_pAcceptedTlsContext;

    IEComUnknown* m_pTcpAsyncServerSocketEComUnknown;
    IAsyncServerSocket* m_pTcpAsyncServerSocket;
    IAsyncSocket* m_pTcpAsyncSocket;

    IAsyncServerSocketMgr* m_pTlsAsyncServerSocketMgr;
    IAsyncSocketMgr* m_pTlsAsyncSocketMgr;
    IAsyncTlsServerSocketMgr* m_pAsyncTlsServerSocketMgr;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     !defined(MXD_TLS_NONE)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTLSSERVERSOCKETBASE_H

