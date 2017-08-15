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
#ifndef MXG_CCERTIFICATEEXTENSIONOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEEXTENSIONOPENSSL_H

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
#include "Config/FrameworkCfg.h" // MXD_PKI_OPENSSL
#endif

#if defined(MXD_PKI_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBasicConstraints;
class CBlob;
class CExtendedKeyUsage;
class CIssuerAlternateName;
class CKeyUsage;
class CNetscapeCertificateType;
class CSubjectAlternateName;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME
    #define MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME CCertificateExtensionOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateExtensionOpenSsl
//========================================
//
// Summary:
//   Class managing certificate extensions using Open SSL.
//
// Description:
//   CCertificateExtension is the class that manages the certificate extensions.
//
// Location:
//   Pki/CCertificateExtension.h
//
// See Also:
//
//==EDOC========================================================================
class CCertificateExtensionOpenSsl
{
    friend class CCertificateOpenSsl;

// Published Enum
//----------------
public:
    // Summary:
    //  Defines the possible extension types.
    enum EType
    {
        // Subject key identifier extension type.
        eTYPE_SUBJECT_KEY_IDENTIFIER = NID_subject_key_identifier,
        // Key usage extension type.
        eTYPE_KEY_USAGE = NID_key_usage,
        // Subject alternative name extension type.
        eTYPE_SUBJECT_ALTERNATIVE_NAME = NID_subject_alt_name,
        // Issuer alternative name extension type.
        eTYPE_ISSUER_ALTERNATIVE_NAME = NID_issuer_alt_name,
        // Basic constraints extension type.
        eTYPE_BASIC_CONSTRAINTS = NID_basic_constraints,
        // Certificate policies extension type.
        eTYPE_CERTIFICATE_POLICIES = NID_certificate_policies,
        // Authority key identifier extension type.
        eTYPE_AUTHORITY_KEY_IDENTIFIER = NID_authority_key_identifier,
        // Extended key usage extension type.
        eTYPE_EXTENDED_KEY_USAGE = NID_ext_key_usage,
        // Netscape certificate extension type.
        eTYPE_NETSCAPE_CERTIFICATE_TYPE = NID_netscape_cert_type,
        // Unkown extension type.
        eTYPE_UNKNOWN = NID_undef
    };


    // Summary:
    //  The ENameType enum specifies the type of the alternate name.
    //  See RFC 3280 section 4.2.1.7 for more information.
    enum ENameType
    {
        // DNS name (RFC 1034).
        eNAME_TYPE_DNS_NAME,
        // IP address name (RFC 791 or RFC 1883).
        eNAME_TYPE_IP_ADDRESS,
        // RFC 822.
        eNAME_TYPE_RFC_822_NAME,
        // Uniform resource identifier name (RFC 1738).
        eNAME_TYPE_UNIFORM_RESOURCE_IDENTIFIER,
        // Unknown name type.
        eNAME_TYPE_UNKNOWN
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CCertificateExtensionOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtensionOpenSsl(IN const CCertificateExtensionOpenSsl* pCertificateExtension);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtensionOpenSsl(IN const CCertificateExtensionOpenSsl& rCertificateExtension);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateExtensionOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateExtensionOpenSsl& operator=(IN const CCertificateExtensionOpenSsl& rCertificateExtension);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateExtensionOpenSsl& rCertificateExtension) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificateExtensionOpenSsl& rCertificateExtension) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the Basic Constraints extension.
    //----------------------------------------
    mxt_result GetBasicConstraints(OUT CBasicConstraints* pBasicConstraints) const;

    // Summary:
    //  Gets the Extended key usage.
    //-------------------------------
    mxt_result GetExtendedKeyUsage(OUT CExtendedKeyUsage* pExtendedKeyUsage) const;

    // Summary:
    //  Gets the Issuer Alternate Name extension.
    //--------------------------------------------
    mxt_result GetIssuerAlternateName(OUT CIssuerAlternateName* pIssuerAlternateName) const;

    // Summary:
    //  Gets the Key usage extension.
    //--------------------------------
    mxt_result GetKeyUsage(OUT CKeyUsage* pKeyUsage) const;

    // Summary:
    //  Gets the Netscape certificate extensions.
    //--------------------------------------------
    mxt_result GetNetscapeCertificateType(OUT CNetscapeCertificateType* pNetscapeCertificateType) const;

    // Summary:
    //  Gets the Subject Alternate Name extension.
    //---------------------------------------------
    mxt_result GetSubjectAlternateName(OUT CSubjectAlternateName* pSubjectAlternateName) const;

    // Summary:
    // Gets the type of extension.
    //-----------------------------
    mxt_result GetType(OUT EType* peType) const;

    // Summary:
    //  Verifies if an extension is critical.
    //----------------------------------------
    mxt_result IsCritical() const;

    mxt_result Initialize(IN X509* pEvpX509, IN unsigned int uExtension);

// Hidden Methods
//----------------
protected:

    void Enter() const;
    void Exit() const;

private:

// Hidden Data Members
//---------------------
protected:

    CCrypto* m_pCrypto;

    X509* m_pEvpX509;
    unsigned int m_uExtension;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CCertificateExtensionOpenSsl::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CCertificateExtensionOpenSsl::Exit() const
{
    m_pCrypto->Exit();
}


MX_NAMESPACE_END(MXD_GNS)


#endif // #if defined(MXD_PKI_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATEEXTENSIONOPENSSL_H

