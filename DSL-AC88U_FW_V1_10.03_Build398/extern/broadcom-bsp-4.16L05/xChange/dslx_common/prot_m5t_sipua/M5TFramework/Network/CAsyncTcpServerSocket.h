//==============================================================================
//==============================================================================
//
//         Copyright(c) 2003 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_CASYNCTCPSERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTCPSERVERSOCKET_H

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
                                 // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Data Members
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

#ifndef MXG_CMAP_H
#include "Cap/CMap.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CASYNCTCPSOCKET_H
#include "Network/CAsyncTcpSocket.h"
#endif

#ifndef MXG_IASYNCSOCKETTCPOPTIONS_H
#include "Network/IAsyncSocketTcpOptions.h"
#endif

#ifndef MXG_ITCPSOCKETOPTIONSCONFIGURE_H
#include "Network/ITcpSocketOptionsConfigure.h"
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
#ifndef MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
#include "Network/IAsyncSocketQualityOfServiceOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
#include "Network/IAsyncSocketWindowsGqosOptions.h"
#endif
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CAsyncTcpSocket;
class CSocketAddr;
#if !defined(MXD_OS_SYMBIAN)
class CTcpServerSocket;
#endif
class IAsyncServerSocketMgr;
class IAsyncSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAsyncTcpServerSocket
//==============================================================================
//
//  Summary:
//    Class that implements an asynchronous TCP server socket.
//
//  Description:
//    The CAsyncTcpServerSocket class is an ECOM implementation of an
//    asynchronous TCP server socket which offers numerous ECOM interfaces
//    related to said sockets.
//
//  Location:
//    Network/CAsyncTcpServerSocket.h
//
//  See Also:
//    CTcpServerSocket, CASyncTcpSocket
//
//==============================================================================
class CAsyncTcpServerSocket : protected CEComDelegatingUnknown,
                              protected CEventDriven,
                              protected IAsyncServerSocket,
                              public IAsyncSocket,
                              protected IAsyncSocketQualityOfServiceOptions,
                              protected IAsyncSocketTcpOptions
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
                              ,
                              protected IAsyncSocketWindowsGqosOptions
#endif
{
//-- Published interface
public:
    // The following statement is essential, it provide the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Published methods through inherited interfaces
protected:
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    // Inherited from CEComDelegatingUnknown.

    virtual mxt_result InitializeInstance();
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IAsyncServerSocket.

    virtual mxt_result AcceptA(IN IEComUnknown* pServicingThread);
    virtual mxt_result BindA(IN const CSocketAddr* pLocalAddress);
    virtual mxt_result Listen(IN unsigned int uMaxPendingConnection = 5);
    virtual mxt_result QueryAcceptedOptionsIf(IN mxt_iid iidRequested,
                                              OUT void** ppInterface);
    virtual mxt_result SetAsyncServerSocketMgr(IN IAsyncServerSocketMgr* pMgr);

    // Inherited from IAsyncSocket.

    virtual mxt_result Activate(IN IEComUnknown* pIEComUnknown);
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

    // Inherited from IAsyncSocketQualityOfServiceOptions.

    virtual mxt_result Set8021QUserPriority(IN bool bEnable,
                                            IN uint8_t uUserPriority);
    virtual mxt_result SetTos(IN uint8_t uTos);

    // Inherited from IAsyncSocketWindowsGqosOptions.

#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    virtual mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);
    virtual mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);
#endif

    // Inherited from IAsyncSocketTcpOptions

    virtual mxt_result SetKeepAlive(IN bool bEnable);
    virtual mxt_result SetNagle(IN bool bEnable);
    virtual void SetConnectTimeoutMs(IN uint64_t uConnectTimeoutMs);
    virtual mxt_result SetBackgroundVoipSocket(IN bool bEnable);

//-- Hidden Methods
private:
    friend mxt_result InitializeCAsyncTcpServerSocket();
    friend void FinalizeCAsyncTcpServerSocket();

    CAsyncTcpServerSocket(IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CAsyncTcpServerSocket();
    
// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncTcpServerSocket(const CAsyncTcpServerSocket& from);
    CAsyncTcpServerSocket& operator=(const CAsyncTcpServerSocket& from);
#endif

    void ReleaseResources(IN bool bClose);

    // Inherited from CEComDelegatingUnknown
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);
    virtual unsigned int NonDelegatingReleaseIfRef();

    // Support for IAsyncServerSocket.

    void InternalAcceptA(IN CMarshaler* pParameter);
    void InternalBindA(IN CMarshaler* pParameter);
    void InternalListen(IN CMarshaler* pParameter);
    void InternalSetAsyncServerSocketMgr(IN CMarshaler* pParameter);

    // Support for IAsyncSocket.

    void InternalCloseA(IN CMarshaler* pParameter);
    void InternalGetHandle(IN CMarshaler* pParameter);
    void InternalGetLocalAddress(IN CMarshaler* pParameter);
    void InternalGetOpaque(IN CMarshaler* pParameter);
    void InternalGetSocketType(IN CMarshaler* pParameter);
    void InternalSetAsyncSocketMgr(IN CMarshaler* pParameter);
    void InternalSetOpaque(IN CMarshaler* pParameter);
    void InternalSetSocketType(IN CMarshaler* pParameter);
    void InternalEraseAllUserInfo(IN CMarshaler* pParameter);
    void InternalEraseUserInfo(IN CMarshaler* pParameter);
    void InternalGetUserInfo(IN CMarshaler* pParameter);
    void InternalInsertUserInfo(IN CMarshaler* pParameter);
// BRCM_CUSTOM - [begin] Add user opaque to socket.
    void InternalGetUserOpaque(IN CMarshaler* pParameter);
    void InternalAddUserOpaque(IN CMarshaler* pParameter);
    void InternalRemoveUserOpaque(IN CMarshaler* pParameter);
// BRCM_CUSTOM - [end] Add user opaque to socket.

    // Support for IAsyncSocketQualityOfServiceOptions.

    void ApplyAsyncSocketQualityOfServiceOptionsCache();
    void InternalSet8021QUserPriority(IN CMarshaler* pParameter);
    void InternalSetTos(IN CMarshaler* pParameter);

    // Support for IAsyncSocketWindowsGqosOptions.
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    void InternalSetWindowsReceivingFlowspec(IN CMarshaler* pParameter);
    void InternalSetWindowsSendingFlowspec(IN CMarshaler* pParameter);
#endif

    // Support for IAsyncSocketTcpOptions.

    void ApplyAsyncSocketTcpOptionsCache();
    void InternalSetBackgroundVoipSocket(IN CMarshaler* pParameter);

    // Inherited from CEventDriven.

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);
#if defined(MXD_OS_SYMBIAN)
    virtual void EvRequestStatusServiceMgrAwaken(IN TRequestStatus* pRequestStatus,
                                                 IN mxt_opaque opq);
#else
    virtual void EvSocketServiceMgrAwaken(IN mxt_hSocket hSocket,
                                          IN unsigned int uEvents,
                                          IN mxt_opaque opq);
#endif

//-- Hidden Data Members
private:
    mxt_opaque m_opq;
    ITcpSocketOptionsConfigure* m_pAcceptedTcpSocketOptionsConfigure;
    IAsyncServerSocketMgr* m_pAsyncServerSocketMgr;
    IAsyncSocketMgr* m_pAsyncSocketMgr;

    bool m_bBound;

#if defined(MXD_OS_SYMBIAN)
    RSocket* m_pTcpServerSocket;
    RSocket* m_pAcceptedSocket;
    TRequestStatus m_acceptRequestStatus;
    unsigned int m_uMaxPendingConnection;
#else
    CTcpServerSocket* m_pTcpServerSocket;
#endif

    // Cache for IAsyncSocketQualityOfServiceOptions.
    bool m_b8021QUserPriorityCache;
    bool m_b8021QUserPriorityEnableCache;
    uint8_t m_u8021QUserPriorityUserPriorityCache;
    bool m_bTosCache;
    unsigned int m_uTosTosCache;

    // Cache for IAsyncSocketWindowsGqosOptions.
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    bool m_bWindowsReceivingFlowspecCache;
    FLOWSPEC* m_pWindowsReceivingFlowspecReceivingFlowspecCache;

    bool m_bWindowsSendingFlowspecCache;
    FLOWSPEC* m_pWindowsSendingFlowspecSendingFlowspecCache;
#endif

    // Cache for IAsyncSocketTcpOptions.
    bool m_bBackgroundVoipSocketCache;
    bool m_bBackgroundVoipSocketEnableCache;

    // Socket Type
    char** m_ppszType;
    unsigned int m_uTypeSize;

    // User info
    // GCC2.7 does not support default arguments in template definition.
#if defined(MXD_COMPILER_GNU_GCC) && (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
    CMap<CString, CBlob, CAATree<CMapPair<CString, CBlob> > > m_mapUserInfo;
#else
    CMap<CString, CBlob> m_mapUserInfo;
#endif
// BRCM_CUSTOM - [begin] Add user opaque to socket.
    CVector<mxt_opaque> m_vecOpqUser;
// BRCM_CUSTOM - [end] Add user opaque to socket.
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_NETWORK_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTCPSERVERSOCKET_H
