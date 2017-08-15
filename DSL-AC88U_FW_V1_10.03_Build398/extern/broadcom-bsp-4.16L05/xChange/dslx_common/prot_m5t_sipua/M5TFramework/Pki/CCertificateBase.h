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
#ifndef MXG_CCERTIFICATEBASE_H
#define MXG_CCERTIFICATEBASE_H

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
    #include "Config/FrameworkCfg.h" // MXD_PKI_NONE
#endif

#if !defined(MXD_PKI_NONE)

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
#ifndef MXG_CAUTHORITYKEYIDENTIFIER_H
#include "Pki/CAuthorityKeyIdentifier.h"
#endif
#ifndef MXG_CBASICCONSTRAINTS_H
#include "Pki/CBasicConstraints.h"
#endif
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif
#ifndef MXG_CEXTENDEDKEYUSAGE_H
#include "Pki/CExtendedKeyUsage.h"
#endif
#ifndef MXG_CISSUERALTERNATENAME_H
#include "Pki/CIssuerAlternateName.h"
#endif
#ifndef MXG_CKEYUSAGE_H
#include "Pki/CKeyUsage.h"
#endif
#ifndef MXG_CNETSCAPECERTIFICATETYPE_H
#include "Pki/CNetscapeCertificateType.h"
#endif
#ifndef MXG_CSUBJECTALTERNATENAME_H
#include "Pki/CSubjectAlternateName.h"
#endif
#ifndef MXG_CSUBJECTKEYIDENTIFIER_H
#include "Pki/CSubjectKeyIdentifier.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CCertificateIssuer;
class CCertificateSubject;
class CMarshaler;
class CPrivateKey;
class CPublicKey;
class CTime;
class CBasicConstraints;
class CExtendedKeyUsage;
class CIssuerAlternateName;
class CKeyUsage;
class CNetscapeCertificateType;
class CSubjectAlternateName;
class CAuthorityKeyIdentifier;
class CSubjectKeyIdentifier;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//==SDOC========================================================================
//== Class: CCertificateBase
//========================================
//
// Summary:
//   Class that manages and abstracts an X.509 certificate.
//
// Description:
//   CCertificateBase is the class that manages and abstracts an X.509 
//   certificate.
//
//   An X.509 certificate is used to certify the identity of an entity.
//
// Location:
//   Pki/CCertificateBase.h
//
// See Also:
//  CCertificate
//
//==EDOC========================================================================
class CCertificateBase
{
// Published Enum
//----------------
public:
    // Summary:
    //  Defines the various ways a certificate can be encoded.
    //---------------------------------------------------------
    enum EEncoding
    {
        // A DER encoded certificate. This is a binary encoding.
        //-------------------------------------------------------
        eENCODING_DER,

        // A PEM encoded certificate. This is a base64 (email safe) encoding of
        // a certificate.
        //----------------------------------------------------------------------
        eENCODING_PEM
    };

    // Summary:
    //  Defines the various versions of certificate.
    enum EVersion
    {
        // Version 1.
        eVERSION_V1 = 0,
        // Version 2.
        eVERSION_V2 = 1,
        // Version 3.
        eVERSION_V3 = 2,
        // Unkown version.
        eVERSION_UNKNOWN
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    // Summary:
    // Constructor.
    //--------------
    CCertificateBase();

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateBase(IN const CCertificateBase* pCertificate);

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateBase(IN const CCertificateBase& rCertificate);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CCertificateBase();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateBase& operator=(IN const CCertificateBase& rCertificate);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateBase& rCertificate) const;

    // Summary:
    //  Reads a certificate (DER- or PEM-encoded) from a blob.
    //---------------------------------------------------------
    mxt_result Restore(IN const CBlob* pX509);

    // Summary:
    //  Reads a DER-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestoreDer(IN const CBlob* pX509) = 0;

    // Summary:
    //  Reads a PEM-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestorePem(IN const CBlob* pX509) = 0;


    // Summary:
    //  Stores the content of this CCertificateBase into a blob, with a 
    //  specified encoding.
    //------------------------------------------------------------------
    mxt_result Store(OUT CBlob* pX509,
                     IN EEncoding eEncoding = eENCODING_DER) const;
    // Summary:
    //  Stores the content of this certificate in DER format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StoreDer(OUT CBlob* pX509) const = 0;

    // Summary:
    //  Stores the content of this certificate in PEM format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StorePem(OUT CBlob* pX509) const = 0;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the certificate version.
    //--------------------------------
    virtual mxt_result GetVersion(OUT CCertificateBase::EVersion* peVersion) const = 0;
    // Summary:
    //  Gets the certificate serial number.
    //--------------------------------------
    virtual mxt_result GetSerialNumber(OUT CBlob* pblobSerialNumber) const = 0;
    // Summary:
    //  Gets the time after which the certificate is considered as invalid.
    //----------------------------------------------------------------------
    virtual mxt_result GetNotAfterTime(OUT CTime* pNotAfter) const = 0;
    // Summary:
    //  Gets the time before which the certificate is considered as invalid.
    //-----------------------------------------------------------------------
    virtual mxt_result GetNotBeforeTime(OUT CTime* pNotBefore) const = 0;

    // Summary:
    //  Gets the certificate issuer.
    //-------------------------------
    virtual mxt_result GetIssuer(OUT CCertificateIssuer* pIssuer) const = 0;

    // Summary:
    //  Gets the certificate subject.
    //--------------------------------
    virtual mxt_result GetSubject(OUT CCertificateSubject* pSubject) const = 0;

    // Summary:
    //  Gets the public key associated with this certificate.
    //--------------------------------------------------------
    virtual mxt_result GetPublicKey(OUT CPublicKey* pPublicKey) const = 0;

    // Summary:
    //  Gets the signature and signature algorithm for this certificate.
    //-------------------------------------------------------------------
    virtual mxt_result GetSignature(OUT CPublicKey::EAlgorithm* peSignatureAlgorithm,
                                    OUT CBlob* pblobSignature) const = 0;

    // Only V3.

    // Summary:
    //  Gets the certificate extension information by index.
    //-------------------------------------------------------
    virtual mxt_result GetExtension(IN unsigned int uExtension,
                                    OUT CCertificateExtension* pExtension) const = 0;

    // Summary:
    //  Gets the certificate extension corresponding to a specified type.
    //--------------------------------------------------------------------
    virtual mxt_result GetExtensionByType(IN CCertificateExtension::EType eType,
                                          OUT CCertificateExtension* pExtension) const = 0;

    // Summary:
    //  Gets the number of extensions added to this certificate.
    //-----------------------------------------------------------
    virtual mxt_result GetExtensionCount(OUT unsigned int* puExtensionCount) const = 0;

    // Summary:
    //  Gets the Basic Constraints extension.
    //----------------------------------------
    mxt_result GetBasicConstraintsExtension(OUT CBasicConstraints* pBasicConstraints) const;

    // Summary:
    //  Gets the Extended Key Usage certificate extension.
    //-----------------------------------------------------
    mxt_result GetExtendedKeyUsageExtension(OUT CExtendedKeyUsage* pExtendedKeyUsage) const;

    // Summary:
    //  Gets the Issuer Alternate Name extension.
    //--------------------------------------------
    mxt_result GetIssuerAlternateNameExtension(OUT CIssuerAlternateName* pIssuerAlternateName) const;

    // Summary:
    //  Gets the Key Usage certificate extension.
    //--------------------------------------------
    mxt_result GetKeyUsageExtension(OUT CKeyUsage* pKeyUsage) const;

    // Summary:
    //  Gets the Netscape Certificate Type extension.
    //------------------------------------------------
    mxt_result GetNetscapeCertificateTypeExtension(OUT CNetscapeCertificateType* pNetscapeCertificateType) const;

    // Summary:
    //  Gets the Subject Alternate Name extension.
    //---------------------------------------------
    mxt_result GetSubjectAlternateNameExtension(OUT CSubjectAlternateName* pSubjectAlternateName) const;

    // Summary:
    //  Gets the Authority Key Identifier extension.
    //-----------------------------------------------
    mxt_result GetAuthorityKeyIdentifierExtension(OUT CAuthorityKeyIdentifier* pAuthorityKeyIdentifier) const;

    // Summary:
    //  Gets the Subject Key Identifier extension.
    //---------------------------------------------
    mxt_result GetSubjectKeyIdentifierExtension(OUT CSubjectKeyIdentifier* pSubjectKeyIdentifier) const;

    // << Helpers >>
    //---------------
    // Summary:
    //  Gets a private key associated with this X.509 certificate.
    //-------------------------------------------------------------
    mxt_result GetPrivateKey(OUT CPrivateKey* pPrivateKey) const;

    // Summary:
    //  Verifies whether a certificate is self-issued.
    //-------------------------------------------------
    mxt_result IsSelfIssued() const;

    // Summary:
    //  Associates a private key with this X.509 certificate.
    //--------------------------------------------------------
    virtual mxt_result SetPrivateKey(IN const CPrivateKey* pPrivateKey);

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    virtual mxt_result VerifySignature(IN const CPublicKey* pIssuerPublicKey) const = 0;

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    mxt_result VerifySignature(IN const CCertificateBase* pIssuerCertificate) const;

    // Summary:
    //  Displays certificate information.
    //------------------------------------
    mxt_result DisplayCertificate() const;

// Hidden Methods
//----------------
protected:
    void Enter() const;
    void Exit() const;

    void PrintNameHelper(IN const char* pszCertificateType,
                         IN const char* pszName,
                         IN CVector<CString>* pvecstrNames) const;

private:

// Hidden Data Members
//---------------------
protected:

    // Required for critical sections.
    CCrypto* m_pCrypto;

    // Private key used with this certificate.
    CPrivateKey* m_pPrivateKey;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
void CCertificateBase::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CCertificateBase::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATEBASE_H
//M5T_INTERNAL_USE_END

