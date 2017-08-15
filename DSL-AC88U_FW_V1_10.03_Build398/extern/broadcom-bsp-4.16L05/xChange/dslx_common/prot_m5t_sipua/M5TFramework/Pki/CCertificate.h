//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CCERTIFICATE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATE_H
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
#include "Config/FrameworkCfg.h" // MXD_PKI_NONE
#endif

#if !defined(MXD_PKI_NONE)
#if defined(MXD_PKI_CCERTIFICATE_INCLUDE)
    #include MXD_PKI_CCERTIFICATE_INCLUDE
#else
    #error "No implementation defined for CCertificate!!!"
#endif

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CPUBLICKEY_H
#include "Crypto/CPublicKey.h"
#endif

#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif

#ifndef MXG_CCERTIFICATEISSUER_H
#include "Pki/CCertificateIssuer.h"
#endif

#ifndef MXG_CCERTIFICATESUBJECT_H
#include "Pki/CCertificateSubject.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CMarshaler;
class CPrivateKey;
class CTime;
class CBasicConstraints;
class CAuthorityKeyIdentifier;
class CExtendedKeyUsage;
class CIssuerAlternateName;
class CKeyUsage;
class CNetscapeCertificateType;
class CSubjectAlternateName;
class CSubjectKeyIdentifier;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CCERTIFICATE)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CCERTIFICATESTUB_H
    #include "PkiStubs/CCertificateStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CCertificate
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages and abstracts an X.509 certificate.
//
// Description:
//   CCertificate is the class that manages and abstracts an X.509 certificate.
//
//   An X.509 certificate is used to certify the identity of an entity.
//
// Location:
//   Pki/CCertificate.h
//
// See Also:
//  CCertificateChain
//
//==EDOC========================================================================
class CCertificate : public MXD_PKI_CCERTIFICATE_CLASSNAME
{
    //M5T_INTERNAL_USE_BEGIN
    friend class CAsyncTlsSocket;
    friend CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CCertificate& rCertificate);
    friend CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CCertificate& rCertificate);
    //M5T_INTERNAL_USE_END

// Published Enum
//----------------

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CCertificate();

    // Summary:
    // Copy constructor.
    //--------------
    CCertificate(IN const CCertificate* pCertificate);

    // Summary:
    // Copy constructor.
    //--------------
    CCertificate(IN const CCertificate& rCertificate);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CCertificate();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificate& operator=(IN const CCertificate& rCertificate);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificate& rCertificate) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificate& rCertificate) const;

    // Summary:
    //  Reads a certificate (DER- or PEM-encoded) from a blob.
    //---------------------------------------------------------
    mxt_result Restore(IN const CBlob* pX509);

    // Summary:
    //  Reads a DER-encoded certificate from a blob.
    //-----------------------------------------------
    mxt_result RestoreDer(IN const CBlob* pX509);

    // Summary:
    //  Reads a PEM-encoded certificate from a blob.
    //-----------------------------------------------
    mxt_result RestorePem(IN const CBlob* pX509);


    // Summary:
    //  Stores the content of this CCertificate into a blob, with a specified
    //  encoding.
    //------------------------------------------------------------------------
    mxt_result Store(OUT CBlob* pX509,
                     IN EEncoding eEncoding = eENCODING_DER) const;
    // Summary:
    //  Stores the content of this certificate in DER format into a blob.
    //--------------------------------------------------------------------
    mxt_result StoreDer(OUT CBlob* pX509) const;

    // Summary:
    //  Stores the content of this certificate in PEM format into a blob.
    //--------------------------------------------------------------------
    mxt_result StorePem(OUT CBlob* pX509) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the certificate version.
    //--------------------------------
    mxt_result GetVersion(OUT CCertificate::EVersion* peVersion) const;
    // Summary:
    //  Gets the certificate serial number.
    //--------------------------------------
    mxt_result GetSerialNumber(OUT CBlob* pSerialNumber) const;
    // Summary:
    //  Gets the time after which the certificate is considered as invalid.
    //----------------------------------------------------------------------
    mxt_result GetNotAfterTime(OUT CTime* pNotAfter) const;
    // Summary:
    //  Gets the time before which the certificate is considered as invalid.
    //-----------------------------------------------------------------------
    mxt_result GetNotBeforeTime(OUT CTime* pNotBefore) const;

    // Summary:
    //  Gets the certificate issuer.
    //-------------------------------
    mxt_result GetIssuer(OUT CCertificateIssuer* pIssuer) const;

    // Summary:
    //  Gets the certificate subject.
    //--------------------------------
    mxt_result GetSubject(OUT CCertificateSubject* pSubject) const;

    // Summary:
    //  Gets the public key associated with this certificate.
    //--------------------------------------------------------
    mxt_result GetPublicKey(OUT CPublicKey* pPublicKey) const;

    // Summary:
    //  Gets the signature and signature algorithm for this certificate.
    //-------------------------------------------------------------------
    mxt_result GetSignature(OUT CPublicKey::EAlgorithm* peSignatureAlgorithm,
                            OUT CBlob* pSignature) const;

    // Only V3.

    // Summary:
    //  Gets the certificate extension information by index.
    //-------------------------------------------------------
    mxt_result GetExtension(IN unsigned int uExtension,
                            OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the certificate extension corresponding to a specified type.
    //--------------------------------------------------------------------
    mxt_result GetExtensionByType(IN CCertificateExtension::EType eType,
                                  OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the number of extensions added to this certificate.
    //-----------------------------------------------------------
    mxt_result GetExtensionCount(OUT unsigned int* puExtensionCount) const;

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
    //  Verifies whether a certificate has been issued by another one.
    //-----------------------------------------------------------------
    mxt_result IsIssuedBy(IN const CCertificate* pCertificate) const;

    // Summary:
    //  Verifies whether a certificate is self-issued.
    //-------------------------------------------------
    mxt_result IsSelfIssued() const;

    // Summary:
    //  Associates a private key with this X.509 certificate.
    //--------------------------------------------------------
    mxt_result SetPrivateKey(IN const CPrivateKey* pPrivateKey);

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    mxt_result VerifySignature(IN const CPublicKey* pIssuerPublicKey) const;

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    mxt_result VerifySignature(IN const CCertificate* pIssuerCertificate) const;

    // Summary:
    //  Displays certificate information.
    //------------------------------------
    mxt_result DisplayCertificate() const;

// Hidden Methods
//----------------
protected:
private:

// Hidden Data Members
//---------------------
protected:

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==    CCertificate
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CCertificate::CCertificate()
:   MXD_PKI_CCERTIFICATE_CLASSNAME()
{
}

//==SDOC========================================================================
//==
//==    CCertificate
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer to the CCertificate to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificate::CCertificate(IN const CCertificate* pCertificate)
:   MXD_PKI_CCERTIFICATE_CLASSNAME(pCertificate)
{
}

//==SDOC========================================================================
//==
//==    CCertificate
//==
//==============================================================================
//
//  Parameters:
//      rCertificate:
//          Reference to the CCertificate to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CCertificate::CCertificate(IN const CCertificate& rCertificate)
:   MXD_PKI_CCERTIFICATE_CLASSNAME(rCertificate)
{
}

//==SDOC========================================================================
//==
//==    ~CCertificate
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCertificate::~CCertificate()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rCertificate:
//          Reference to the certificate to assign.
//
//  Returns:
//      A reference to the assigned certificate.
//
//  Description:
//      Assigns the right hand certificate to the left hand one.
//
//==EDOC========================================================================
inline
CCertificate& CCertificate::operator=(IN const CCertificate& rCertificate)
{
    MXD_PKI_CCERTIFICATE_CLASSNAME::operator=(rCertificate);
    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rCertificate:
//          Reference to the certificate to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CCertificate::operator==(IN const CCertificate& rCertificate) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::operator==(rCertificate);
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rCertificate:
//          Reference to the CCertificate to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CCertificate::operator!=(IN const CCertificate& rCertificate) const
{
    return !(MXD_PKI_CCERTIFICATE_CLASSNAME::operator==(rCertificate));
}

//==SDOC========================================================================
//==
//==    Restore
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer containing the certificate
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Restores the certificate from a blob.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::Restore(IN const CBlob* pX509)
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::Restore(pX509);
}

//==SDOC========================================================================
//==
//==    RestoreDer
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer to a blob containing the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Restores to the CCertificate object a certificate stored in DER format
//      in the specified blob.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::RestoreDer(IN const CBlob* pX509)
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::RestoreDer(pX509);
}


//==SDOC========================================================================
//==
//==    RestorePem
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer to a blob containing the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Restores to the CCertificate object a certificate stored in PEM format
//      in the specified blob.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::RestorePem(IN const CBlob* pX509)
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::RestorePem(pX509);
}

//==SDOC========================================================================
//==
//==    Store
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer to contain the certificate.
//
//      eEncoding:
//          Type of encoding used to store the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Stores the certificate into a blob in the specified encoding.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::Store(OUT CBlob* pX509, IN EEncoding eEncoding) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::Store(pX509,eEncoding);
}

//==SDOC========================================================================
//==
//==    StoreDer
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer to a blob to contain the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Stores the CCertificate object in DER format in the specified blob.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::StoreDer(OUT CBlob* pX509) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::StoreDer(pX509);
}


//==SDOC========================================================================
//==
//==    StorePem
//==
//==============================================================================
//
//  Parameters:
//      pX509:
//          Pointer to a blob to contain the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Stores the CCertificate object in PEM format in the specified blob.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::StorePem(OUT CBlob* pX509) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::StorePem(pX509);
}

//==SDOC========================================================================
//==
//==    GetVersion
//==
//==============================================================================
//
//  Parameters:
//      peVersion:
//          Pointer to contain the version of the certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the version of the certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetVersion(OUT CCertificate::EVersion* peVersion) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetVersion(peVersion);
}

//==SDOC========================================================================
//==
//==    GetSerialNumber
//==
//==============================================================================
//
//  Parameters:
//      pSerialNumber:
//          Pointer to contain the certificate's serial number.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the certificate's serial number.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetSerialNumber(OUT CBlob* pSerialNumber) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetSerialNumber(pSerialNumber);
}

//==SDOC========================================================================
//==
//==    GetNotAfterTime
//==
//==============================================================================
//
//  Parameters:
//      pNotAfter:
//          Pointer to contain the time after which a certificate is not valid.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the time after which a certificate is considered invalid.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetNotAfterTime(OUT CTime* pNotAfter) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetNotAfterTime(pNotAfter);
}

//==SDOC========================================================================
//==
//==    GetNotBeforeTime
//==
//==============================================================================
//
//  Parameters:
//      pNotBefore:
//          Pointer to contain the time before which a certificate is not valid.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the time before which a certificate is considered invalid.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetNotBeforeTime(OUT CTime* pNotBefore) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetNotBeforeTime(pNotBefore);
}

//==SDOC========================================================================
//==
//==    GetIssuer
//==
//==============================================================================
//
//  Parameters:
//      pIssuer:
//          Pointer to contain the issuer of this certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resS_OK
//
//  Description:
//      Gets the issuer of the certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetIssuer(OUT CCertificateIssuer* pIssuer) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetIssuer(pIssuer);
}

//==SDOC========================================================================
//==
//==    GetSubject
//==
//==============================================================================
//
//  Parameters:
//      pSubject:
//          Pointer to contain the subject of this certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resS_OK
//
//  Description:
//      Gets the subject of the certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetSubject(OUT CCertificateSubject* pSubject) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetSubject(pSubject);
}

//==SDOC========================================================================
//==
//==    GetPublicKey
//==
//==============================================================================
//
//  Parameters:
//      pPublicKey:
//          Pointer to contain the public of this certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the public key of the certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetPublicKey(OUT CPublicKey* pPublicKey) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetPublicKey(pPublicKey);
}

//==SDOC========================================================================
//==
//==    GetSignature
//==
//==============================================================================
//
//  Parameters:
//      peSignatureAlgorithm:
//          Pointer to contain the signature algorithm.
//
//      pSignature:
//          Pointer to contain the signature.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the signature of the certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetSignature(OUT CPublicKey::EAlgorithm* peSignatureAlgorithm,
                                      OUT CBlob* pSignature) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetSignature(peSignatureAlgorithm, pSignature);
}

//==SDOC========================================================================
//==
//==    GetExtension
//==
//==============================================================================
//
//  Parameters:
//      uExtension:
//          Index of the extension to get.
//
//      pExtension:
//          Pointer to contain the extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resS_OK
//
//  Description:
//      Gets the extension at the specified index.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetExtension(IN unsigned int uExtension,
                                      OUT CCertificateExtension* pExtension) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetExtension(uExtension, pExtension);
}

//==SDOC========================================================================
//==
//==    GetExtensionByType
//==
//==============================================================================
//
//  Parameters:
//      eType:
//          Type of the extension to get.
//
//      pExtension:
//          Pointer to contain the extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the certificate extension corresponding to a specified
//      type.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetExtensionByType(IN CCertificateExtension::EType eType,
                                            OUT CCertificateExtension* pExtension) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetExtensionByType(eType, pExtension);
}

//==SDOC========================================================================
//==
//==    GetExtensionCount
//==
//==============================================================================
//
//  Parameters:
//      puExtensionCount:
//          Pointer to contain the number of extensions.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the number of certificate extensions.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetExtensionCount(OUT unsigned int* puExtensionCount) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetExtensionCount(puExtensionCount);
}

//==SDOC========================================================================
//==
//==    GetBasicConstraintsExtension
//==
//==============================================================================
//
//  Parameters:
//      pBasicConstraints:
//          Pointer to the outgoing Basic Constraints extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Basic Constraints extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetBasicConstraintsExtension(OUT CBasicConstraints* pBasicConstraints) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetBasicConstraintsExtension(pBasicConstraints);
}

//==SDOC========================================================================
//==
//==    GetExtendedKeyUsageExtension
//==
//==============================================================================
//
//  Parameters:
//      pExtendedKeyUsage:
//          Pointer to the outgoing Extended Key Usage extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Extended Key Usage extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetExtendedKeyUsageExtension(OUT CExtendedKeyUsage* pExtendedKeyUsage) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetExtendedKeyUsageExtension(pExtendedKeyUsage);
}

//==SDOC========================================================================
//==
//==    GetIssuerAlternateNameExtension
//==
//==============================================================================
//
//  Parameters:
//      pIssuerAlternateName:
//          Pointer to the outgoing Issuer Alternate Name extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Issuer Alternate Name extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetIssuerAlternateNameExtension(OUT CIssuerAlternateName* pIssuerAlternateName) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetIssuerAlternateNameExtension(pIssuerAlternateName);
}

//==SDOC========================================================================
//==
//==    GetKeyUsageExtension
//==
//==============================================================================
//
//  Parameters:
//      pKeyUsage:
//          Pointer to the outgoing Key Usage extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Key Usage extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetKeyUsageExtension(OUT CKeyUsage* pKeyUsage) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetKeyUsageExtension(pKeyUsage);
}

//==SDOC========================================================================
//==
//==    GetNetscapeCertificateTypeExtension
//==
//==============================================================================
//
//  Parameters:
//      pNetscapeCertificateType:
//          Pointer to the outgoing Netscape Certificate Type extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Netscape Certificate Type extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetNetscapeCertificateTypeExtension(OUT CNetscapeCertificateType* pNetscapeCertificateType) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetNetscapeCertificateTypeExtension(pNetscapeCertificateType);
}

//==SDOC========================================================================
//==
//==    GetSubjectAlternateNameExtension
//==
//==============================================================================
//
//  Parameters:
//      pSubjectAlternateName:
//          Pointer to the outgoing Subject Alternate Name extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Subject Alternate Name extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetSubjectAlternateNameExtension(OUT CSubjectAlternateName* pSubjectAlternateName) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetSubjectAlternateNameExtension(pSubjectAlternateName);
}

//==SDOC========================================================================
//==
//==    GetAuthorityKeyIdentifierExtension
//==
//==============================================================================
//
//  Parameters:
//      pAuthorityKeyIdentifier:
//          Pointer to the outgoing authority key identifier extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the authority key identifier extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetAuthorityKeyIdentifierExtension(OUT CAuthorityKeyIdentifier* pAuthorityKeyIdentifier) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetAuthorityKeyIdentifierExtension(pAuthorityKeyIdentifier);
}

//==SDOC========================================================================
//==
//==    GetSubjectKeyIdentifierExtension
//==
//==============================================================================
//
//  Parameters:
//      pSubjectKeyIdentifier:
//          Pointer to the outgoing subject key identifier extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      This method gets the Subject key identifier extension.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetSubjectKeyIdentifierExtension(OUT CSubjectKeyIdentifier* pSubjectKeyIdentifier) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetSubjectKeyIdentifierExtension(pSubjectKeyIdentifier);
}

//==SDOC========================================================================
//==
//==    GetPrivateKey
//==
//==============================================================================
//
//  Parameters:
//      pPrivateKey:
//          Pointer to contain the private key.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resS_OK
//
//  Description:
//      Gets the certificate's private key.
//
//  NOTES:
//      The key MUST have been set with a call to SetPrivateKey before using
//      this method.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::GetPrivateKey(OUT CPrivateKey* pPrivateKey) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::GetPrivateKey(pPrivateKey);
}

//==SDOC========================================================================
//==
//==    IsIssuedBy
//==
//==============================================================================
//
//  Parameters:
//      pCertificate:
//          Pointer that contains the issuing certificate.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_INVALID_STATE
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Checks if the current certificate has been issued by the specified
//      certificate. A certificate can verify that it is self-signed by using
//      this method.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::IsIssuedBy(IN const CCertificate* pCertificate) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::IsIssuedBy(pCertificate);
}

//==SDOC========================================================================
//==
//==    IsSelfIssued
//==
//==============================================================================
//
//  Returns:
//      - resFE_INVALID_STATE
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the certificates is self-signed.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::IsSelfIssued() const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::IsSelfIssued();
}

//==SDOC========================================================================
//==
//==    SetPrivateKey
//==
//==============================================================================
//
//  Parameters:
//      pPrivateKey:
//          Pointer that contains the private key.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resS_OK
//
//  Description:
//      Sets the private key associated with this certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::SetPrivateKey(IN const CPrivateKey* pPrivateKey)
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::SetPrivateKey(pPrivateKey);
}

//==SDOC========================================================================
//==
//==    VerifySignature
//==
//==============================================================================
//
//  Parameters:
//      pIssuerPublicKey:
//          Issuer's public key.
//
//  Returns:
//      resS_OK if successful, res error code otherwise.
//
//  Description:
//      Uses the issuer's public key to validate the signature of the current
//      certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::VerifySignature(IN const CPublicKey* pIssuerPublicKey) const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::VerifySignature(pIssuerPublicKey);
}

//==SDOC========================================================================
//==
//==    VerifySignature
//==
//==============================================================================
//
//  Parameters:
//      pIssuerCertificate:
//          Issuer's certificate.
//
//  Returns:
//      resS_OK if successful, res error code otherwise.
//
//  Description:
//      Extracts the public key of the issuers certificate to validate the
//      signature of the current certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::VerifySignature(IN const CCertificate* pIssuerCertificate) const
{    
    return CCertificateBase::VerifySignature(pIssuerCertificate);
}

//==SDOC========================================================================
//==
//==    DisplayCertificate
//==
//==============================================================================
//
//  Returns:
//      resS_OK if there is a valid certificate to display, an error otherwise.
//
//  Description:
//      Displays the information regarding the current certificate.
//
//==EDOC========================================================================
inline
mxt_result CCertificate::DisplayCertificate() const
{
    return MXD_PKI_CCERTIFICATE_CLASSNAME::DisplayCertificate();
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CCERTIFICATE)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATE_H

