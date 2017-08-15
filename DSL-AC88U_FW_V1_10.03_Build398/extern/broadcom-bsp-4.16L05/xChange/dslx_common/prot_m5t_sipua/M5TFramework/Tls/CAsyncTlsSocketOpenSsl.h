//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CASYNCTLSSOCKETOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCTLSSOCKETOPENSSL_H

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
    defined(MXD_TLS_OPENSSL)

//-- Data members
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_CASYNCTLSSOCKETBASE_H
#include "Tls/CAsyncTlsSocketBase.h"
#endif

#ifndef MXG_IASYNCTLSOPENSSL_H
#include "Tls/IAsyncTlsOpenSsl.h"
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
//== Class: CAsyncTlsSocket
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
//   Tls/CAsyncTlsSocket.h
//
//==EDOC========================================================================
class CAsyncTlsSocket : protected CAsyncTlsSocketBase,
                        protected IAsyncTlsOpenSsl
{
    friend class CAsyncTlsServerSocket;

protected:
    // This is an extension to the OpenSsl BIO_METHOD structure. The purpose
    // of this extension is to add the object instance pointer to the
    // structure.
    struct STlsSocketBioMethod : public BIO_METHOD
    {
        // Add an objet pointer to the OpenSsl Bio Method structure.
        CAsyncTlsSocket* m_pThis;
    };

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

    // Inherited from CEComDelegatingUnknown.
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Inherited from IAsyncTlsSocket.
    virtual mxt_result SetTlsContext(IN const CTlsContext* pTlsContext);
    virtual mxt_result GetUsedCipher(OUT CString& rstrCipherName) const;
    virtual mxt_result GetUsedCipherStrength(OUT unsigned int& ruCipherStrengthBits) const;

    // Inherited from IAsyncTlsOpenSsl.
    virtual mxt_result GetSsl(OUT const SSL** ppSsl) const;

// Hidden Methods
//----------------
private:
    friend mxt_result InitializeCAsyncTlsSocket();
    friend void FinalizeCAsyncTlsSocket();

    CAsyncTlsSocket(IN IEComUnknown* pOuterIEComUnknown = NULL);
    virtual ~CAsyncTlsSocket();

// GCC 2.7.2 does not like empty private copy constructors (when building in debug).
#if !defined(MXD_COMPILER_GNU_GCC) || \
    (defined(MXD_COMPILER_GNU_GCC) && MXD_COMPILER_GNU_GCC > MXD_COMPILER_GNU_GCC_2_7)
    CAsyncTlsSocket(IN const CAsyncTlsSocket& rFrom);
    CAsyncTlsSocket& operator=(IN const CAsyncTlsSocket& rFrom);
#endif

    // Inherited from IPrivateAsyncTlsSocket
    virtual mxt_result PerformTlsHandshakingA(IN CTlsContext* pTlsContext,
                                              IN mxt_opaque opq);

    // Inherited from CEventDriven.
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    // Asynchronism support methods.

    // Support for IPrivateAsyncTlsSocket
    void InternalPerformTlsHandshakingA(IN CMarshaler* pParameter);

    // Support for IAsyncTlsOpenSsl.
    void InternalGetSsl(IN CMarshaler* pParameter);

    // Dummy callback for SSL_CTX_set_verify.
    static int VerifyCallback(IN int nOk, IN X509_STORE_CTX* pStore);
    static void TlsMessageCallback(IN int nSent,
                                   IN int nVersion,
                                   IN int nContentType,
                                   IN const void* pvoidBuffer,
                                   IN size_t uLength,
                                   IN SSL* pSsl,
                                   IN void* pvoidArgument);

    // Helper methods.

    // Summary:
    //  Resets the socket members to their default values, releasing the
    //  internal TCP socket if it exists.
    mxt_result CreateTls();

    void ConnectingTls(IN bool bTimerExpired);

    void SendingTls();

    void ReceivingTls();

    void ClosingTls(IN bool bTimerExpired);

    void RenegotiatingTls(IN bool bTimerExpired);

    void DisplayErrors();

    CCertificateChain GO * GetPeerCertificateChain();

    virtual void ReleaseResources();

    virtual void TryToStartAutoRenegotiation(IN bool bTimerExpired);

    // Summary
    //  This method calls EvAsyncTlsSocketMgrNewTlsSession if the CTlsSession
    //  has changed.
    void NotifyOnNewTlsSession();

    // OpenSSL BIO methods override.

    // Summary:
    //  Initializes the BIO METHOD structure.
    void InitBioMethod();

    // Summary:
    //  Initializes the BIO stucture.
    static int BioNew(BIO *b);

    // Summary:
    //  Write bytes to a socket.
    static int BioWrite(BIO *b, const char *in, int inl);

    // Summary:
    //  Read bytes from a socket.
    static int BioRead(BIO *b, char *out, int outl);

    // Summary:
    //  Performs some controls on a BIO.
    static long BioCtrl(BIO *b, int cmd, long num, void *ptr);

    // Summary:
    //  Not used.
    static int BioPuts(BIO *b, const char *str);

    // Summary:
    //  Uninitializes the BIO stucture.
    static int BioFree(BIO *b);

// Hidden Data Members
//---------------------
private:

    STlsSocketBioMethod m_stTlsSocketBioMethod;
    BIO* m_pBio;
    SSL* m_pSsl;
    SSL_CTX* m_pSslCtx;

    // Summary:
    //  Specifies that a SSL_read operation is currently in progress and that no
    //  SSL_write must be attempted until it is complete. The SSL_read is
    //  waiting for the socket to become readable or writable.
    bool m_bSslReadInProgress;

    // Summary:
    //  Specifies that a SSL_write operation is currently in progress and that
    //  no SSL_read must be attempted until it is complete. The SSL_read is
    //  waiting for the socket to become readable or writable.
    bool m_bSslWriteInProgress;

    // Indicates if the last BioRead() returned WOULD_BLOCK.
    bool m_bLastBioReadReturnedWouldBlock;

    // Summary:
    //  Specifies if a closed by peer event was received on the socket.
    bool m_bClosedByPeer;

    // Results used to report TLS alerts.
    mxt_result m_resTlsAlertError;
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_TLS_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CASYNCTLSSOCKETOPENSSL_H

