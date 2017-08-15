//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CTLSCONTEXT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTLSCONTEXT_H
//M5T_INTERNAL_USE_END

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
#include "Config/FrameworkCfg.h"
#endif

#if !defined(MXD_TLS_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CTLS_H
#include "Tls/CTls.h"
#endif // #ifndef MXG_CTLS_H

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif // #ifndef MXG_CVECTOR_H

#ifndef MXG_CCERTIFICATION_H
#include "Pki/CCertificate.h"
#endif // #ifndef MXG_CCERTIFICATION_H

#ifndef MXG_CCERTIFICATECHAIN_H
#include "Pki/CCertificateChain.h"
#endif // #ifndef MXG_CCERTIFICATECHAIN_H

#ifndef MXG_CDIFFIEHELLMAN_H
#include "Crypto/CDiffieHellman.h"
#endif // #ifndef MXG_CDIFFIEHELLMAN_H

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif // #ifndef MXG_CMUTEX_H

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif // #ifndef MXG_CSTRING_H

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CTLSCONTEXT)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CTLSCONTEXTSTUB_H
    #include "TlsStubs/CTlsContextStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CTlsContext
//========================================
//<GROUP TLS_CLASSES>
//
// Summary:
//  The CTlsContext is the class that holds and manages various TLS handshaking
//  parameters.
//
// Description:
//  The CTlsContext is the class that holds and manages various TLS handshaking
//  parameters.
//
//  The following parameters can be configured:
//
//  - <B>Certificate Chain</B>:
//      The ordered list of certificates that is sent to the peer. See
//      CCertificateChain for more information.
//
//  - <B>Cipher</B>:
//      The Cipher is a string that specifies which cipher algorithm suites must
//      be used for the connection. The default is "ALL", which specifies that
//      all supported cipher suites are allowed.
//
//  - <B>Peer Authentication</B>:
//      Specifies whether or not the peer must be authenticated. If true, a
//      certificate request is sent to the peer. The received certificate is
//      then evaluated for trust. The default is peer authenticated.
//
//  - <B>Protocol Versions</B>:
//      Specifies which protocol versions are allowed. This could be SSL 3.0 or
//      TLS 1.0.
//
//  - <B>Trusted Certificates</B>:
//      Specifies a list of root certificates that are trusted. If the peer
//      presents a certificate that does inherit from one of them, the
//      connection handshaking is accepted.
//
//  - <B>Compression</B>:
//      Specifies whether or not compression methods are proposed in the
//      handshake. Supported only on some TLS implementations.
//
// Location:
//   Tls/CTlsContext.h
//
// See Also:
//   CCertificate, CCertificateChain
//
//
//==EDOC========================================================================
class CTlsContext
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CTlsContext();

    // Summary:
    //  Copy constructor. Pointer version.
    //-------------------------------------
    CTlsContext(IN const CTlsContext* pTlsContext);

    // Summary:
    //  Copy constructor. Reference version.
    //---------------------------------------
    CTlsContext(IN const CTlsContext& rTlsContext);

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CTlsContext();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CTlsContext& operator=(IN const CTlsContext& rTlsContext);

    // Summary:
    //  Returns the certificates chain configured for this context.
    //--------------------------------------------------------------------------
    void GetCertificateChain(OUT CCertificateChain* pCertificateChain) const;

    // Summary:
    //  Sets the current certificates chain.
    //--------------------------------------------------------------------------
    void SetCertificateChain(IN CCertificateChain* pCertificateChain);

    // Summary:
    //  Returns the allowed ciphers.
    //--------------------------------------------------------------------------
    void GetCiphers(OUT CString* pstrCiphers) const;

    // Summary:
    //  Configures the allowed ciphers that may be used in the capability
    //  exchange. Defaults to "ALL"
    //--------------------------------------------------------------------------
    mxt_result SetCiphers(IN const CString& rstrCiphers);

    // Summary:
    //  Returns the current Diffie-Hellman configuration.
    //--------------------------------------------------------------------------
    void GetEphemeralDiffieHellman(OUT CDiffieHellman* pEphemeralDiffieHellman) const;

    // Summary:
    //  Sets the current Diffie-Hellman configuration.
    //--------------------------------------------------------------------------
    void SetEphemeralDiffieHellman(IN const CDiffieHellman* pEphemeralDiffieHellman);

    // Summary:
    //  Returns true if the peer must be authenticated, false otherwise.
    //-------------------------------------------------------------------
    void GetPeerAuthentication(OUT bool* pbAuthenticatePeer) const;

    // Summary:
    //  Configures if the peer must be authenticated. Defaults to true.
    //------------------------------------------------------------------
    void SetPeerAuthentication(IN bool bAuthenticatePeer);

    // Summary:
    //  Returns the protocol versions configured within the context.
    //--------------------------------------------------------------------------
    void GetProtocolVersions(OUT CVector<ETlsVersion>* pveceProtocolVersions) const;

    // Summary:
    //  Sets the current protocol versions.
    //  Defaults to eTLS_VERSION_SSL_3_0 and eTLS_VERSION_TLS_1_0.
    //--------------------------------------------------------------------------
    void SetProtocolVersions(IN const CVector<ETlsVersion>* pveceProtocolVersions);

    // Summary:
    //  Returns the trusted certificates configured within the context.
    //--------------------------------------------------------------------------
    void GetTrustedCertificates(OUT CVector<CCertificate>* pvecTrustedCertificates) const;

    // Summary:
    //  Sets the current trusted certificates.
    //--------------------------------------------------------------------------
    void SetTrustedCertificates(IN const CVector<CCertificate>* pvecTrustedCertificates);

    // Summary:
    //  Returns whether or not the compression is enabled.
    //--------------------------------------------------------------------------
    bool IsCompressionEnabled();

    // Summary:
    //  Enables or disables compression.
    //--------------------------------------------------------------------------
    void EnableCompression(IN bool bEnable);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

// Hidden Data Members
//---------------------
protected:

    mutable CMutex m_mutexConcurrencyProtection;

    bool m_bAuthenticatePeer;
    CCertificateChain m_certificateChain;
    CString m_strCiphers;
    CDiffieHellman m_ephemeralDiffieHellman;
    CVector<ETlsVersion> m_veceProtocolVersions;
    CVector<CCertificate> m_vecTrustedCertificates;
    bool m_bCompressionEnabled;
    //M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CTLSCONTEXT)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_TLS_NONE)

#endif // MXG_CTLSCONTEXT_H

