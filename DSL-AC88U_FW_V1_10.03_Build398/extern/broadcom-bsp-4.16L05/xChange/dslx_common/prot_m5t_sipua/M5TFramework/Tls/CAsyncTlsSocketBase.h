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
#ifndef MXG_CASYNCTLSSOCKETBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSOCKETBASE_H

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

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    !defined(MXD_TLS_NONE)

//-- Data members
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

// For mxt_hSocket.
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_IASYNCCLIENTSOCKET_H
#include "Network/IAsyncClientSocket.h"
#endif

#ifndef MXG_IASYNCCLIENTSOCKETMGR_H
#include "Network/IAsyncClientSocketMgr.h"
#endif

#ifndef MXG_IASYNCIOSOCKET_H
#include "Network/IAsyncIoSocket.h"
#endif

#ifndef MXG_IASYNCIOSOCKETMGR_H
#include "Network/IAsyncIoSocketMgr.h"
#endif

#ifndef MXG_IASYNCSOCKET_H
#include "Network/IAsyncSocket.h"
#endif

#ifndef MXG_IASYNCSOCKETBUFFERSIZEOPTIONS_H
#include "Network/IAsyncSocketBufferSizeOptions.h"
#endif

#ifndef MXG_IASYNCSOCKETMGR_H
#include "Network/IAsyncSocketMgr.h"
#endif

#ifndef MXG_IASYNCSOCKETQUALITYOFSERVICEOPTIONS_H
#include "Network/IAsyncSocketQualityOfServiceOptions.h"
#endif

#ifndef MXG_IASYNCSOCKETTCPOPTIONS_H
#include "Network/IAsyncSocketTcpOptions.h"
#endif

#ifndef MXG_IASYNCSOCKETWINDOWSGQOSOPTIONS_H
#include "Network/IAsyncSocketWindowsGqosOptions.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CTLS_H
#include "Tls/CTls.h"
#endif

#ifndef MXG_IASYNCTLSRENEGOTIATION_H
#include "Tls/IAsyncTlsRenegotiation.h"
#endif

#ifndef MXG_IASYNCTLSSOCKET_H
#include "Tls/IAsyncTlsSocket.h"
#endif

#ifndef MXG_IPRIVATEASYNCTLSSOCKET_H
#include "Tls/IPrivateAsyncTlsSocket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CCertificateChain;
class CSocketAddr;
class CTlsContext;
class CTlsSession;
class IAsyncTlsSocketMgr;
class IAsyncTlsRenegotiationMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// Internal enum used to identify if the entity is server or client related to
// the TCP connection.
enum ETcpIdentity
{
    eTCP_IDENTITY_CLIENT,
    eTCP_IDENTITY_SERVER,
    eTCP_IDENTITY_UNKNOWN
};

//==============================================================================
//== Class: CAsyncTlsSocketBase
//========================================
//
// Summary:
//     Implementation of a TLS client socket.
//
// Description:
//     The class CAsyncTlsServerSocket is an ECOM implementation of a TLS socket
//     that offers numerous ECOM interfaces related to said sockets.
//
// Location:
//   Tls/CAsyncTlsSocketBase.h
//
//==EDOC========================================================================
class CAsyncTlsSocketBase : protected CEComDelegatingUnknown,
                            protected CEventDriven,
                            protected IAsyncClientSocket,
                            protected IAsyncIoSocket,
                            protected IAsyncSocket,
                            protected IAsyncSocketBufferSizeOptions,
                            protected IAsyncSocketQualityOfServiceOptions,
                            protected IAsyncSocketTcpOptions,
                            protected IAsyncTlsSocket,
                            protected IAsyncTlsRenegotiation,
                            protected IAsyncClientSocketMgr,
                            protected IAsyncIoSocketMgr,
                            protected IAsyncSocketMgr,
                            protected IPrivateAsyncTlsSocket
#if defined(MXD_OS_WINDOWS) && \
    defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
                            ,
                            protected IAsyncSocketWindowsGqosOptions
#endif
{
    friend class CAsyncTlsServerSocket;

protected:
    // Enumeration containing the possible TLS socket states.
    enum EAsyncTlsSocketState
    {
        // The TCP socket is bound to a local interface and a local port.
        eBOUND,

        // The TCP socket is closed and cannot be used again.
        eCLOSED,

        // The TLS handshaking has been correctly teard down by the peer. The
        // peer may also have closed the TCP socket. However, normally, we
        // should try to close the TLS connection with the Closure Alert too.
        eCLOSED_BY_PEER_TLS,

        // The TLS shutdown has been completed, the underlying TCP socket is now
        // being closed.
        eCLOSING_TCP,
        
        // The TLS connection was established. The TLS connection is now
        // being teard down and will drop back to TCP and then close itself.
        eCLOSING_TLS,

        // The state was eCONNECTING_TCP if the user called ConnectA otherwise
        // the socket has been accepted by the TCP server socket has started
        // with the state eCONNECTED_TCP.
        eCONNECTED_TCP,

        // The state was eCONNECTING_TLS_AWAITING_APPROVAL.
        // TLS handshaking is completed and the user has autorized the peer
        // credentials. The TLS connection is now fully established.
        eCONNECTED_TLS,

        // ConnectA was called and the TCP socket is now awaiting connection
        // establishment.
        eCONNECTING_TCP,

        // The socket was eCONNECTED_TCP.
        // The TLS connection is now being established and awaits TLS
        // handshaking completion.
        eCONNECTING_TLS,

        // The state was eCONNECTING_TLS.
        // The TLS connection handshaking has been performed and the manager
        // has been asked to approve the new credential The TLS connection
        // handshaking will be completed once the manager has called
        // TlsHandshakingApprovalCompletedA. The state will then become
        // eCONNECTED_TLS and the event EvAsyncClientSocketMgrConnected will
        // be notified before setting the state to eCONNECTED_TLS.
        eCONNECTING_TLS_AWAITING_APPROVAL,

        // The socket is in an unstable state and the user has been notified
        // about it. The socket should be released.
        eERRORED,

        // The state was eCONNECTED_TLS.
        // The TLS connection security parameters are now being renegotiated.
        // Note that this state does not account for peer initiated
        // renegotiations which is processed transparently.
        eRENEGOTIATING_AUTO,

        // The state was eRENEGOTIATING_AUTO.
        // The TLS connection security parameters have been renegotiated and
        // the manager has been asked to approve the new credentials. The
        // renegotiation will be completed once the manager has called
        // TlsHandshakingApprovalCompletedA. The state will then become
        // eCONNECTED_TLS.
        eRENEGOTIATING_AUTO_AWAITING_APPROVAL,

        // The state was eCONNECTED_TLS.
        // The manager explicitely requested renegotiation. The TLS connection
        // security parameters are now being renegotiated. Note that this state
        // does not account for peer initiated renegotiations which is processed
        // transparently.
        eRENEGOTIATING_EXPLICIT,

        // The state was eRENEGOTIATING_EXPLICIT.
        // The TLS connection security parameters have been renegotiated and
        // the manager has been asked to approve the new credentials. The
        // renegotiation will be completed once the manager has called
        // TlsHandshakingApprovalCompletedA. The event
        // EvAsyncTlsSocketMgrRenegotiated will be notified. The state will
        // then become eCONNECTED_TLS.
        eRENEGOTIATING_EXPLICIT_AWAITING_APPROVAL,

        // The state was eCONNECTED_TLS.
        // The TLS connection security parameters are now being renegotiated.
        // The renegotiation has been initiated by the peer.
        eRENEGOTIATING_INITIATED_BY_PEER,

        // The state was eRENEGOTIATING_INITIATED_BY_PEER.
        // The TLS connection security parameters have been renegotiated and
        // the manager has been asked to approve the new credentials. The
        // renegotiation will be completed once the manager has called
        // TlsHandshakingApprovalCompletedA. The state will then become
        // eCONNECTED_TLS.
        eRENEGOTIATING_INITIATED_BY_PEER_AWAITING_APPROVAL
    };

    enum ESocketMode
    {
        eSM_ACCEPTED,
        eSM_CLIENT
    };

// Published Interface
public:
    // The following statement is essential, it provides the default and unique
    // implementation of the IEComUnknown interface that every other interfaces
    // inherit from.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN


//-- Published methods through inherited interfaces
protected:

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

    // Inherited from IAsyncSocketTcpOptions.
    virtual mxt_result SetKeepAlive(IN bool bEnable);
    virtual mxt_result SetNagle(IN bool bEnable);
    virtual void SetConnectTimeoutMs(IN uint64_t uConnectTimeoutMs);
    virtual mxt_result SetBackgroundVoipSocket(IN bool bEnable);

    // Inherited from IAsyncTlsSocket.
    virtual mxt_result GetTlsContext(OUT CTlsContext* pTlsContext) const;
    virtual mxt_result SetTlsContext(IN const CTlsContext* pTlsContext);
    virtual mxt_result SetTlsSession(IN const CTlsSession* pTlsSession);
    virtual mxt_result SetAsyncTlsSocketMgr(IN IAsyncTlsSocketMgr* pAsyncTlsSocketMgr);
    virtual mxt_result TlsHandshakingApprovalCompletedA(IN mxt_result resApproval);
    virtual mxt_result GetUsedCipher(OUT CString& rstrCipherName) const;
    virtual mxt_result GetUsedCipherStrength(OUT unsigned int& ruCipherStrengthBits) const;

    // Inherited from IAsyncTlsRenegotiation.
    virtual mxt_result RenegotiateA(IN uint64_t uTimeoutMs);
    virtual mxt_result SetAutoRenegotiationThresholdInByte(IN uint32_t uByte);
    virtual mxt_result SetAutoRenegotiationThresholdInTimeMs(IN uint64_t uTimeMs);
    virtual mxt_result SetAutoRenegotiationTimeoutMs(IN uint64_t uTimeoutMs);
    virtual mxt_result SetAsyncTlsRenegotiationMgr(IN IAsyncTlsRenegotiationMgr* pAsynTlsRenegotiationMgr);
    virtual mxt_result EnableAllRenegotiationNotifications(IN bool bEnable);

    // Inherited from IPrivateAsyncTlsSocket.
    virtual mxt_result SetTcpSocket(IN IAsyncIoSocket* pAsyncIoSocket);

protected:
    virtual ~CAsyncTlsSocketBase();
// Hidden Methods
//----------------
protected:
    CAsyncTlsSocketBase(IN IEComUnknown* pOuterIEComUnknown = NULL);

// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncTlsSocketBase(IN const CAsyncTlsSocketBase& rFrom);
    CAsyncTlsSocketBase& operator=(IN const CAsyncTlsSocketBase& rFrom);
#endif

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result InitializeInstance();
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);
    virtual unsigned int NonDelegatingReleaseIfRef();

    // Inherited from IAsyncClientSocketMgr.
    virtual void EvAsyncClientSocketMgrBound(IN mxt_opaque opq,
                                             IN CSocketAddr* pEffectiveLocalAddress);
    virtual void EvAsyncClientSocketMgrConnected(IN mxt_opaque opq);

    // Inherited from IAsyncIoSocketMgr.
    virtual void EvAsyncIoSocketMgrReadyToRecv(IN mxt_opaque opq);
    virtual void EvAsyncIoSocketMgrReadyToSend(IN mxt_opaque opq);

    //Inherited from IAsyncSocketMgr
    virtual void EvAsyncSocketMgrClosed(IN mxt_opaque opq);
    virtual void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq);
    virtual void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq,
                                               IN mxt_result res);

    // Inherited from CEventDriven.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

    // Asynchronism support methods.

    // Support for IAsyncClientSocket.
    virtual void InternalConnectA(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncClientSocketMgr(IN CMarshaler* pParameter);

    // Support for IAsyncIoSocket.
    virtual void InternalRecv(IN CMarshaler* pParameter);
    virtual void InternalSend(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncIoSocketMgr(IN CMarshaler* pParameter);

    // Support for IAsyncSocket.
    virtual void InternalCloseA(IN CMarshaler* pParameter);
    virtual void InternalGetOpaque(IN CMarshaler* pParameter);
    virtual void InternalGetSocketType(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncSocketMgr(IN CMarshaler* pParameter);
    virtual void InternalSetOpaque(IN CMarshaler* pParameter);
    virtual void InternalSetSocketType(IN CMarshaler* pParameter);

    // Support for IAsyncTlsSocket.
    virtual void InternalGetTlsContext(IN CMarshaler* pParameter);
    virtual void InternalSetTlsContext(IN CMarshaler* pParameter);
    virtual void InternalSetTlsSession(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncTlsSocketMgr(IN CMarshaler* pParameter);
    virtual void InternalTlsHandshakingApprovalCompletedA(IN CMarshaler* pParameter);

    // Support for IAsyncTlsRenegotiation.
    virtual void InternalRenegotiateA(IN CMarshaler* pParameter);
    virtual void InternalSetAutoRenegotiationThresholdInByte(IN CMarshaler* pParameter);
    virtual void InternalSetAutoRenegotiationThresholdInTimeMs(IN CMarshaler* pParameter);
    virtual void InternalSetAutoRenegotiationTimeoutMs(IN CMarshaler* pParameter);
    virtual void InternalSetAsyncTlsRenegotiationMgr(IN CMarshaler* pParameter);
    virtual void InternalEnableAllRenegotiationNotifications(IN CMarshaler* pParameter);

    // Support for IPrivateAsyncTlsSocket.
    virtual void InternalSetTcpSocket(IN CMarshaler* pParameter);

    // Helper methods.

    // Summary:
    //  Resets the socket members to their default values, releasing the
    //  internal TCP socket if it exists.
    virtual mxt_result ResetToDefault(IN ESocketMode eSocketMode,
                                      IN IAsyncIoSocket* pAsyncTcpIoSocket = NULL);
    virtual mxt_result QueryTcpSocketInterfaces();
    virtual void ReleaseTcpSocketInterfaces();

    virtual void Close(IN ISocket::ECloseBehavior eBehavior);

    virtual mxt_result CreateTls();

    virtual void ConnectingTls(IN bool bTimerExpired);

    virtual void ReceivingTlsA();
    virtual void ReceivingTls();

    virtual void SendingTlsA();
    virtual void SendingTls();

    virtual void ClosingTlsA();
    virtual void ClosingTls(IN bool bTimerExpired);

    virtual void RenegotiatingTlsA();
    virtual void RenegotiatingTls(IN bool bTimerExpired);

    virtual void TryToStartAutoRenegotiation(IN bool bTimerExpired);

    virtual CCertificateChain GO * GetPeerCertificateChain();

    virtual void ReleaseResources();

    virtual void SetState(IN EAsyncTlsSocketState eState);

    // Support for helper methods.
    virtual void InternalReceivingTlsA(IN CMarshaler* pParameter);
    virtual void InternalSendingTlsA(IN CMarshaler* pParameter);
    virtual void InternalRenegotiatingTlsA(IN CMarshaler* pParameter);
    virtual void InternalClosingTlsA(IN CMarshaler* pParameter);

    virtual void InternalNotifyReadyToSendA(IN CMarshaler* pParameter);
    virtual void InternalNotifyClosedByPeerA(IN CMarshaler* pParameter);
    virtual void InternalNotifyErrorDetectedA(IN CMarshaler* pParameter);

    // Summary
    //  This method calls EvAsyncTlsSocketMgrNewTlsSession if the CTlsSession
    //  has changed.
    virtual void NotifyOnNewTlsSession();

// Hidden Data Members
//---------------------
protected:

    ESocketMode m_eSocketMode;

    mxt_opaque m_opq;

    CCertificateChain* m_pPeerCertificateChain;

    uint32_t m_uByteReceived;
    uint32_t m_uByteTransmitted;

    // Summary:
    //  A CloseA was requested when the socket was not in a state to close
    //  itself. This flag will be evaluated when the state changes to see
    //  if we can finally close the socket.
    bool m_bCloseRequested;

    // Summary:
    //  Specifies if the user called Send when m_blobSendBuffer was not empty.
    //  If this is true, then EvAsyncIoSocketMgrReadyToSend must be called.
    bool m_bUserSentWhenSendBufferWasNotEmpty;

    // Summary:
    //  Counts the number of times that CloseA has been called to report the
    //  appropriate number of EvAsyncSocketMgrClosed events to the manager.
    unsigned int m_uCloseACallsCount;
    
    // Summary:
    //  Specifies whether the socket is acting as client or server side of
    //  the TCP connection.
    ETcpIdentity m_eTcpIdentity;

    // Summary:
    //  Specifies whether the socket is acting as client or server side in
    //  respect with TLS.
    ETlsIdentity m_eTlsIdentity;

    // Summary:
    //  Used to buffer data that was received because of a call to SSL_read
    //  during renegotiation.
    CBlob m_blobRecvBuffer;

    // Summary:
    //  Used to buffer data that was not sent because of a SSL_ERROR_WANT_READ
    //  or SSL_ERROR_WANT_WRITE.
    CBlob m_blobSendBuffer;

    // Summary:
    //  Contains a copy of the TLS context used for the current connection.
    CTlsContext* m_pTlsContext;

    // Summary:
    //  Contains a fixed copy of the user passed session. The handskaking tries
    //  to use the session's identifier.
    CTlsSession* m_pTlsSession;

    // Summary:
    //  This variable is used in the context of the automatic renegotiation.
    //  The maximum number of bytes that may be exchanged in one direction
    //  before automatic renegotiation starts. Setting it to 0 disables it.
    uint32_t m_uThresholdInByte;

    // Summary:
    //  This variable is used in the context of the automatic renegotiation.
    //  The maximum time that is allowed between renegotiations. Setting it to 0
    //  disables it.
    uint64_t m_uThresholdInTimeMs;

    // Summary:
    //  This variable is used in the context of the automatic renegotiation.
    //  The maximum number of ms to wait until the renegotiation fails.
    //  Setting it to 0 disables the timeout.
    uint64_t m_uThresholdTimeoutMs;

    // Summary:
    //  This variable is used in the context of the explit renegotiation.
    //  The maximum number of ms to wait until the renegotiation fails.
    //  The timeout is stored into this variable until the explicit
    //  renegotiation can take place. If its value is 0, this mean that no
    //  explicit should be started when the currently renegotiation ends.
    uint64_t m_uExplicitRenegotiationTimeoutMs;

    // Summary:
    //  This variable is used to indicate that the automatic renegotiation
    //  timer elapsed and we were not able to start renegotiation at that
    //  time because of the current state of the socket. For instante, the
    //  socket could have been m_bSslWriteInProgress == true.
    bool m_bTryToStartAutoRenegotiationTimerExpired;

    // Summary:
    //  This variable is used to enable all renegotiation events through the
    //  IAsyncTlsRenegotiationMgr::EvAsyncTlsRenegotiationMgrRenegotiated
    //  manager when renegotiation succeeds. When false, only
    //  the manual renegotiation (RenegotiateA) event is reported.
    bool m_bEnableAllRenegotiationNotifications;

    // Interfaces on the internal TCP socket.
    IAsyncClientSocket* m_pTcpAsyncClientSocket;
    IAsyncIoSocket* m_pTcpAsyncIoSocket;
    IAsyncSocket* m_pTcpAsyncSocket;
    IAsyncSocketBufferSizeOptions* m_pTcpAsyncSocketBufferSizeOptions;
    IAsyncSocketQualityOfServiceOptions* m_pTcpAsyncSocketQosOptions;
    IAsyncSocketWindowsGqosOptions* m_pTcpAsyncWindowsGqosOptions;
    IAsyncSocketTcpOptions* m_pTcpAsyncSocketTcpOptions;

    IAsyncClientSocketMgr* m_pTlsAsyncClientSocketMgr;
    IAsyncIoSocketMgr* m_pTlsAsyncIoSocketMgr;
    IAsyncSocketMgr* m_pTlsAsyncSocketMgr;
    IAsyncTlsSocketMgr* m_pTlsAsyncTlsSocketMgr;
    IAsyncTlsRenegotiationMgr* m_pTlsAsyncTlsRenegotiationMgr;

    EAsyncTlsSocketState m_eState;

    // Socket Type
    char** m_ppszType;
    unsigned int m_uTypeSize;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     !defined(MXD_TLS_NONE)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTLSSOCKETBASE_H

