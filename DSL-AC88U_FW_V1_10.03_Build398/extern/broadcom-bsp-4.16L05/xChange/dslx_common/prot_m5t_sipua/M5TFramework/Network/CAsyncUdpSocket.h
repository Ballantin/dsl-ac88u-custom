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
#ifndef MXG_CASYNCUDPSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCUDPSOCKET_H

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
                                 // MXD_ECOM_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_ECOM_ENABLE_SUPPORT)

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

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif
#ifndef MXG_IASYNCCLIENTSOCKET_H
#include "Network/IAsyncClientSocket.h"
#endif
#ifndef MXG_IASYNCIOSOCKET_H
#include "Network/IAsyncIoSocket.h"
#endif
#ifndef MXG_IASYNCSOCKET_H
#include "Network/IAsyncSocket.h"
#endif
#ifndef MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
#include "Network/IAsyncSocketBufferSizeOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
#include "Network/IAsyncSocketQualityOfServiceOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETUDPOPTIONS_H
#include "Network/IAsyncSocketUdpOptions.h"
#endif
#ifndef MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
#include "Network/IAsyncSocketWindowsGqosOptions.h"
#endif
#ifndef MXG_IASYNCUNCONNECTEDIOSOCKET_H
#include "Network/IAsyncUnconnectedIoSocket.h"
#endif
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CSocketAddr;
class CUdpSocket;
class IAsyncClientSocketMgr;
class IAsyncIoSocketMgr;
class IAsyncSocketMgr;
class IAsyncUnconnectedIoSocketMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAsyncUdpSocket
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Class that implements an asynchronous UDP socket.
//
//  Description:
//    The CAsyncUdpSocket class is an ECOM implementation of an asynchronous UDP
//    socket which offers numerous ECOM interfaces related to said sockets.
//
//  Location:
//    Network/CAsyncUdpSocket.h
//
//  See Also:
//    CUdpSocket
//
//==============================================================================
class CAsyncUdpSocket : protected CEComDelegatingUnknown,
                        protected CEventDriven,
                        protected IAsyncClientSocket,
                        protected IAsyncIoSocket,
                        protected IAsyncSocket,
                        protected IAsyncSocketBufferSizeOptions,
                        protected IAsyncSocketQualityOfServiceOptions,
                        protected IAsyncSocketUdpOptions,
                        protected IAsyncUnconnectedIoSocket
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

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IAsyncClientSocket.

    virtual mxt_result BindA(IN const CSocketAddr* pLocalAddress);
    virtual mxt_result ConnectA(IN const CSocketAddr* pPeerAddress);
    virtual mxt_result SetAsyncClientSocketMgr(IN IAsyncClientSocketMgr* pMgr);

    // Inherited from IAsyncIoSocket.

    virtual mxt_result GetPeerAddress(OUT CSocketAddr* pPeerAddress) const;
    virtual mxt_result Recv(OUT CBlob* pData);
    virtual mxt_result Recv(OUT uint8_t* puData,
                            IN unsigned int uCapacity,
                            OUT unsigned int* puSize);
    virtual mxt_result Send(IN const CBlob* pData,
                            OUT unsigned int* puSizeSent);
    virtual mxt_result Send(IN const uint8_t* puData,
                            IN unsigned int uSize,
                            OUT unsigned int* puSizeSent);
    virtual mxt_result SetAsyncIoSocketMgr(IN IAsyncIoSocketMgr* pMgr);

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

    // Inherited from IAsyncSocketBufferSizeOptions.

    virtual mxt_result SetReceiveBufferSize(IN unsigned int uSize);
    virtual mxt_result SetTransmitBufferSize(IN unsigned int uSize);

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

    // Inherited from IAsyncSocketUdpOptions.

    virtual mxt_result SetBroadcast(IN bool bEnable);

    // Inherited from IAsyncUnconnectedIoSocket.

    virtual mxt_result RecvFrom(OUT CBlob* pData,
                                OUT CSocketAddr* pPeerAddress);
    virtual mxt_result RecvFrom(OUT uint8_t* puData,
                                IN unsigned int uCapacity,
                                OUT unsigned int* puSize,
                                OUT CSocketAddr* pPeerAddress);
    virtual mxt_result SendTo(IN const CBlob* pData,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress);
    virtual mxt_result SendTo(IN const uint8_t* puData,
                              IN unsigned int uSize,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress);
    virtual mxt_result SetAsyncUnconnectedIoSocketMgr(IN IAsyncUnconnectedIoSocketMgr* pMgr);

//-- Hidden Methods
private:
    friend mxt_result InitializeCAsyncUdpSocket();
    friend void FinalizeCAsyncUdpSocket();

    CAsyncUdpSocket(IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CAsyncUdpSocket();

// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncUdpSocket(const CAsyncUdpSocket& from);
    CAsyncUdpSocket& operator=(const CAsyncUdpSocket& from);
#endif

    mxt_result DisableEventsDetection(IN unsigned int uEvents);
    mxt_result EnableEventsDetection(IN unsigned int uEvents);
    void ReleaseResources(IN bool bClose);

    // Inherited from CEComDelegatingUnknown.
    virtual unsigned int NonDelegatingReleaseIfRef();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    // Support for IAsyncClientSocket.

    void InternalBindA(IN CMarshaler* pParameter);
    void InternalConnectA(IN CMarshaler* pParameter);
    void InternalConnectAJustNotify(IN CMarshaler* pParameter);
    void InternalSetAsyncClientSocketMgr(IN CMarshaler* pParameter);

    // Support for IAsyncIoSocket.

    void InternalGetPeerAddress(IN CMarshaler* pParameter);
    void InternalRecv(IN CMarshaler* pParameter);
    void InternalSend(IN CMarshaler* pParameter);
    void InternalSetAsyncIoSocketMgr(IN CMarshaler* pParameter);

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


    // Support for IAsyncSocketBufferSizeOptions.

    void ApplyAsyncSocketBufferSizeOptionsCache();
    void InternalSetReceiveBufferSize(IN CMarshaler* pParameter);
    void InternalSetTransmitBufferSize(IN CMarshaler* pParameter);

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

    // Support for IAsyncSocketUdpOptions.

    void ApplyAsyncSocketUdpOptionsCache();
    void InternalSetBroadcast(IN CMarshaler* pParameter);

    // Support for IAsyncUnconnectedIoSocket.

    void InternalRecvFrom(IN CMarshaler* pParameter);
    void InternalSendTo(IN CMarshaler* pParameter);
    void InternalSetAsyncUnconnectedIoSocketMgr(IN CMarshaler* pParameter);

    // Asynchronous event notifications.

    void InternalNotifyReadyToRecvA();
    void InternalNotifyUnconnectedReadyToRecvA();

    // Inherited from CEventDriven.

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);
#ifdef MXD_OS_SYMBIAN
    virtual void EvRequestStatusServiceMgrAwaken(IN TRequestStatus* pRequestStatus,
                                                 IN mxt_opaque opq);
#else
    virtual void EvSocketServiceMgrAwaken(IN mxt_hSocket hSocket,
                                          IN unsigned int uEvents,
                                          IN mxt_opaque opq);
#endif //#ifdef MXD_OS_SYMBIAN

//-- Hidden Data Members
private:
    mxt_opaque m_opq;
    IAsyncClientSocketMgr* m_pAsyncClientSocketMgr;
    IAsyncIoSocketMgr* m_pAsyncIoSocketMgr;
    IAsyncSocketMgr* m_pAsyncSocketMgr;
    IAsyncUnconnectedIoSocketMgr* m_pAsyncUnconnectedIoSocketMgr;
    unsigned int m_uEvents;
    bool m_bBound;
    bool m_bConnected;

#ifdef MXD_OS_SYMBIAN
    TRequestStatus m_receiveRequestStatus;
    TRequestStatus m_receiveFromRequestStatus;
    TRequestStatus m_sendRequestStatus;
    TRequestStatus m_connectRequestStatus;
    bool m_bSending;
    bool m_bReceiving;
    TPtr8 m_sendingData;
    TPtr8 m_receivingData;
    bool m_bUserKnowsSendIsComplete;
    bool m_bGotReadyToRecvEventBeforeUserKnowsSendIsComplete;
    TInetAddr m_receiveFromPeerAddr;
    CSocketAddr m_peerAddress;
    RSocket* m_pUdpSocket;
#else
    CUdpSocket* m_pUdpSocket;
#endif //#ifdef MXD_OS_SYMBIAN

    // Cache for IAsyncSocketBufferSizeOptions.
    bool m_bReceiveBufferSizeCache;
    unsigned int m_uReceiveBufferSizeSizeCache;
    bool m_bTransmitBufferSizeCache;
    unsigned int m_uTransmitBufferSizeSizeCache;

    // Cache for IAsyncSocketQualityOfServiceOptions.
    bool m_b8021QUserPriorityCache;
    bool m_b8021QUserPriorityEnableCache;
    uint8_t m_u8021QUserPriorityUserPriorityCache;
    bool m_bTosCache;
    unsigned int m_uTosTosCache;

#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    bool m_bWindowsReceivingFlowspecCache;
    FLOWSPEC* m_pWindowsReceivingFlowspecReceivingFlowspecCache;

    bool m_bWindowsSendingFlowspecCache;
    FLOWSPEC* m_pWindowsSendingFlowspecSendingFlowspecCache;
#endif

    // Cache for IAsyncSocketUdpOptions.
    bool m_bBroadcastCache;
    bool m_bBroadcastEnableCache;

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

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_ECOM_ENABLE_SUPPORT)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCUDPSOCKET_H
