//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CCERTIFICATEOPENSSL_H
#define MXG_CCERTIFICATEOPENSSL_H

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
    #include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_PKI_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CPUBLICKEY_H
#include "Crypto/CPublicKey.h"
#endif
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif
//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEBASE_H
#include "Pki/CCertificateBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CCertificateIssuer;
class CCertificateSubject;
class CPublicKey;
class CTime;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATE_CLASSNAME
    #define MXD_PKI_CCERTIFICATE_CLASSNAME CCertificateOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//==SDOC========================================================================
//== Class: CCertificateOpenSsl
//========================================
//
// Summary:
//   Class that manages and abstracts an X.509 certificate using open ssl.
//
// Description:
//   CCertificateOpenSsl is the class that manages and abstracts an X.509 certificate using open ssl.
//
//   An X.509 certificate is used to certify the identity of an entity.
//
// Location:
//   Pki/CCertificateOpenSsl.h
//
// See Also:
//  CCertificate
//
//==EDOC========================================================================
class CCertificateOpenSsl : public CCertificateBase
{
// Published Enum
//----------------
public:

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    // Summary:
    // Constructor.
    //--------------
    CCertificateOpenSsl();

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateOpenSsl(IN const CCertificateOpenSsl* pCertificate);

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateOpenSsl(IN const CCertificateOpenSsl& rCertificate);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CCertificateOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateOpenSsl& operator=(IN const CCertificateOpenSsl& rCertificate);    

    // Summary:
    //  Reads a DER-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestoreDer(IN const CBlob* pX509);

    // Summary:
    //  Reads a PEM-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestorePem(IN const CBlob* pX509);

    // Summary:
    //  Stores the content of this certificate in DER format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StoreDer(OUT CBlob* pX509) const;

    // Summary:
    //  Stores the content of this certificate in PEM format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StorePem(OUT CBlob* pX509) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the certificate version.
    //--------------------------------
    virtual mxt_result GetVersion(OUT CCertificateOpenSsl::EVersion* peVersion) const;
    // Summary:
    //  Gets the certificate serial number.
    //--------------------------------------
    virtual mxt_result GetSerialNumber(OUT CBlob* pSerialNumber) const;
    // Summary:
    //  Gets the time after which the certificate is considered as invalid.
    //----------------------------------------------------------------------
    virtual mxt_result GetNotAfterTime(OUT CTime* pNotAfter) const;
    // Summary:
    //  Gets the time before which the certificate is considered as invalid.
    //-----------------------------------------------------------------------
    virtual mxt_result GetNotBeforeTime(OUT CTime* pNotBefore) const;

    // Summary:
    //  Gets the certificate issuer.
    //-------------------------------
    virtual mxt_result GetIssuer(OUT CCertificateIssuer* pIssuer) const;

    // Summary:
    //  Gets the certificate subject.
    //--------------------------------
    virtual mxt_result GetSubject(OUT CCertificateSubject* pSubject) const;

    // Summary:
    //  Gets the public key associated with this certificate.
    //--------------------------------------------------------
    virtual mxt_result GetPublicKey(OUT CPublicKey* pPublicKey) const;

    // Summary:
    //  Gets the signature and signature algorithm for this certificate.
    //-------------------------------------------------------------------
    virtual mxt_result GetSignature(OUT CPublicKey::EAlgorithm* peSignatureAlgorithm,
                                    OUT CBlob* pSignature) const;

    // Only V3.

    // Summary:
    //  Gets the certificate extension information by index.
    //-------------------------------------------------------
    virtual mxt_result GetExtension(IN unsigned int uExtension,
                                    OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the certificate extension corresponding to a specified type.
    //--------------------------------------------------------------------
    virtual mxt_result GetExtensionByType(IN CCertificateExtension::EType eType,
                                          OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the number of extensions added to this certificate.
    //-----------------------------------------------------------
    virtual mxt_result GetExtensionCount(OUT unsigned int* puExtensionCount) const;

    // << Helpers >>
    //---------------
    // Summary:
    //  Verifies whether a certificate is self-issued.
    //-------------------------------------------------
    mxt_result IsSelfIssued() const;

    // Summary:
    //  Verifies whether a certificate has been issued by another one.
    //-----------------------------------------------------------------
    mxt_result IsIssuedBy(IN const CCertificateOpenSsl* pCertificate) const;

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    virtual mxt_result VerifySignature(IN const CPublicKey* pIssuerPublicKey) const;

    // Summary:
    // Initialize the object with an X509.
    //-------------------------------------
    void SetX509(IN X509* pEvpX509);

// Hidden Methods
//----------------
protected:
private:

    bool ConvertAsn1TimeToCTime(IN ASN1_TIME* pAsn1Time, OUT CTime* pTime) const;

// Hidden Data Members
//---------------------
protected:

    X509* m_pEvpX509;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
mxt_result CCertificateOpenSsl::IsSelfIssued() const
{
    return IsIssuedBy(this);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_OPENSSL)

#endif // MXG_CCERTIFICATEOPENSSL_H
//M5T_INTERNAL_USE_END

