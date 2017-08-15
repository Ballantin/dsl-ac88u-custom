//==============================================================================
//==============================================================================
//
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
#ifndef MXG_CASYNCTLSSOCKETMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSOCKETMOCANASS_H

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
    defined(MXD_TLS_MOCANA_SS)

#ifndef MXG_CASYNCTLSSOCKETBASE_H
#include "Tls/CAsyncTlsSocketBase.h"
#endif

#ifndef MXG_IPRIVATEASYNCTLSSOCKET_H
#include "Tls/IPrivateAsyncTlsSocket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CAsyncTlsServerSocket;
class CCertificateChain;
class CTlsContext;
class CTlsMocanaSs;
class CCertificate;

//==============================================================================
//== Class: CAsyncTlsSocket
//========================================
//
// Summary:
//     Implementation of a TLS client socket.
//
// Description:
//     The class CAsyncTlsSocket is an ECOM implementation of a TLS socket that
//     offers numerous ECOM interfaces related to said sockets. It implements
//     both the client and server (accepted) side of the TLS connection.
//
// Location:
//   Tls/CAsyncTlsSocket.h
//
//==============================================================================
class CAsyncTlsSocket : public CAsyncTlsSocketBase
{
//-- Friend Declarations

    friend class CAsyncTlsServerSocket;

    //-- This class must be friend so it can acces the InitializeEventHandlers
    //-- method.
    friend class CTlsMocanaSs;

//-- New types within class namespace
protected:
    // This is the repository for MocanaSS stack callback dispatch.
    struct SSocket
    {
        // This holds either a pointer to either a TCP socket or the TLS socket.
        union USocket
        {
            IAsyncSocket* m_pTcpSocket;
            CAsyncTlsSocket* m_pTlsSocket;
        } m_stuSocket;

        // Either the socket handle (for TCP sockets) or the Mocana connection
        // handle (for TLS sockets). Since the handle is an int (on platforms
        // running Mocana SS), we can use it to store the connection handle.
        mxt_hSocket m_hSocket;
    };

    // This is the structure of the socket repository.
    struct SSocketRepository
    {
        // This is the list of sockets.
        CVector<SSocket> m_vecstSockets;

        // This is the mutex protecting the access to the repository.
        CMutex m_mutexRepositoryAccess;
    };

//-- Published Interface
public:

    //-- THESE METHODS ARE MEANT FOR INTERNAL USE ONLY.
    //-- These methods are public only because they are required by C helper
    //-- functions which cannot be made friends.

    // Summary:
    //  Notifies the socket that application data has been received.
    virtual void EvReceivedApplicationData(uint8_t* puData, unsigned int uSize);

    // Summary:
    //   This is to notify that the TLS handshake succeeded.
    virtual void EvTlsHandshakingSucceeded();

    // Summary:
    //   Finds a socket in the TCP socket repository.
    static IAsyncSocket* FindTcpSocket(mxt_hSocket hSocket);

    // Summary:
    //   Finds a socket in the TLS socket repository.
    static CAsyncTlsSocket* FindTlsSocket(int nIndex);

    // Summary:
    //   Retrieves a certificate from the list of trusted certifricatss in the
    //   context.
    virtual mxt_result GetTrustedCertificate(IN struct certDistinguishedName* pstSubjectName,
                                             OUT struct certDescriptor* pstCertificate);

    // Summary:
    //   Retrieves the end entity certificate from the chain set in the context.
    virtual mxt_result GetEntityCertificate(OUT struct certDescriptor* pstCertificate);

    // Summary:
    //   Verifies if the certificate is trusted.
    virtual bool IsTrustedCertificate(IN CCertificate* pCertificate) const;

    //-- << CAsyncTlsSocketBase>>.
    virtual CCertificateChain GO * GetPeerCertificateChain();

    //-- << ECOM >>.
    MX_DECLARE_DELEGATING_IECOMUNKNOWN

//-- Published methods through inherited interfaces
protected:
    // Summary:
    //   Destructor.
    virtual ~CAsyncTlsSocket();

    // Summary:
    //   Sets the callbacks for the Mocana stack.
    static void InitializeEventHandlers();

    // Summary:
    //   Registers the TLS socket and its associated TCP socket in the
    //   repositories.
    static void RegisterSocket(IN CAsyncTlsSocket* pTlsSocket);

    //-- << ECOM >>.
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    //-- << IAsyncTlsSocket >>.
    virtual mxt_result SetTlsContext(IN const CTlsContext* pTlsContext);

    //-- << IPrivateAsyncTlsSocket >>.
    virtual mxt_result PerformTlsHandshakingA(IN CTlsContext* pTlsContext,
                                              IN mxt_opaque opq);

    //-- << IAsyncIoSocketMgr >>.
    virtual void EvAsyncIoSocketMgrReadyToRecv(IN mxt_opaque opq);
    virtual void EvAsyncIoSocketMgrReadyToSend(IN mxt_opaque opq);

// Hidden Methods.
protected:
    // Summary:
    //   Closes the TLS connection.
    void ClosingTls(IN bool bTimerExpired);

    // Summary:
    //   Closes the socket at the network level.
    void Close(IN ISocket::ECloseBehavior eBehavior);

    // Summary:
    //   Processes TLS handshake messages.
    void ConnectingTls(IN bool bTimerExpired);

    // Summary:
    //   Starts the TLS handshake.
    mxt_result CreateTls();

    // Summary:
    //   Returns the result associated with the given status.
    mxt_result GetError(IN MSTATUS eStatus);

    // Summary:
    //   Returns the result associated with the given status.
    mxt_result GetError(IN int nStatus);

    // Summary:
    //   Returns the connection index.
    const int GetIndex();

    // Summary:
    //   Notifies the manager of a new session.
    void NotifyOnNewTlsSession();

    // Summary:
    //   Receives application data.
    void ReceivingTls();

    // Summary:
    //   Sends application data.
    void SendingTls();

    // Summary:
    //   Constructor.
    CAsyncTlsSocket(IN IEComUnknown* pOuterIEComUnknown = NULL);

    //-- << IPrivateAsyncTlsSocket >>.
    void InternalPerformTlsHandshakingA(IN CMarshaler* pParameter);

    //-- << CEventDriven >>.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    //-- << IAsyncTlsSocket >>.
    void InternalTlsHandshakingApprovalCompletedA(IN CMarshaler* pParameter);

private:
    //-- << static members initializers >>.
    friend mxt_result InitializeCAsyncTlsSocket();
    friend void FinalizeCAsyncTlsSocket();

    // Summary:
    //   Copy constructor.
    CAsyncTlsSocket(IN const CAsyncTlsSocket& rFrom);

    // Summary:
    //   Equality operator.
    CAsyncTlsSocket& operator=(IN const CAsyncTlsSocket& rFrom);

// Hidden Data Members.
protected:
    // The index in the Mocana SS stack repository.
    int m_nIndex;

private:
    // The TCP socket repository.
    static SSocketRepository* m_pstTcpSocketRepository;

    // The TLS socket repository.
    static SSocketRepository* m_pstTlsSocketRepository;
};

//==============================================================================
//====  INLINE METHODS  =======================================================
//==============================================================================

inline CCertificateChain GO * CAsyncTlsSocket::GetPeerCertificateChain()
{
    return m_pPeerCertificateChain;
}

//==============================================================================
//==
//==  GetIndex
//==
//==============================================================================
//
//  Summary:
//      Returns the connection index.
//
//  Returns:
//      The connection index.
//
//  Description:
//      Returns the connection index.
//
//==============================================================================
inline const int CAsyncTlsSocket::GetIndex()
{
    return m_nIndex;
}

//==============================================================================
//==
//==  GetError
//==
//==============================================================================
//
//  Summary:
//      Returns the result associated with the given status.
//
//  Parameters:
//      nStatus:
//       The status.
//
//  Returns:
//      The result.
//
//  Description:
//      Returns the result associated with the given status.
//
//==============================================================================
inline mxt_result CAsyncTlsSocket::GetError(IN int nStatus)
{
    return GetError(static_cast<MSTATUS>(nStatus));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_TLS_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTLSSOCKETMOCANASS_H

