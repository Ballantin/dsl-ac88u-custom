//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CAUTHORITYKEYIDENTIFIEROPENSSL_H
#define MXG_CAUTHORITYKEYIDENTIFIEROPENSSL_H

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

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CString;
class CBlob;
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME
    #define MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME CAuthorityKeyIdentifierOpenSsl
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAuthorityKeyIdentifierOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the authority key identifier extensions certificates.
//
// Description:
//   CAuthorityKeyIdentifierOpenSsl is the class to handle the authority key 
//   identifier extensions 2.5.29.35. They are used to identify the public key 
//   related to the private key used to sign a certificate.
//
//  NOTES:
//   It may occur that, in older certificates, the Authority Key Identifier uses
//   the deprecated extension 2.5.29.1. If this ID is encountered, the
//   extension is not extracted. It is considered an unknown extension.
//
// Location:
//   Pki/CAuthorityKeyIdentifierOpenSsl.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CAuthorityKeyIdentifierOpenSsl : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CAuthorityKeyIdentifierOpenSsl();

    // Summary:
    //  Copy constructor.
    //--------------------
    CAuthorityKeyIdentifierOpenSsl(IN const CAuthorityKeyIdentifierOpenSsl* pAuthorityKeyIdentifier);

    // Summary:
    //  Copy constructor.
    //--------------------
    CAuthorityKeyIdentifierOpenSsl(IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CAuthorityKeyIdentifierOpenSsl();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CAuthorityKeyIdentifierOpenSsl& operator=(IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the count of certificate issuers contained within this extension.
    //-------------------------------------------------------------------------
    mxt_result GetCertificateIssuerCount(OUT unsigned int* puNameCount) const;

    // Summary:
    //  Gets the key identifier of the certificate.
    //---------------------------------------------.
    mxt_result GetKeyIdentifier(OUT CBlob* pblobIdentifier) const;

    // Summary:
    //  Gets the serial number of the certificate.
    //---------------------------------------------
    mxt_result GetCertificateSerialNumber(OUT CBlob* pSerialNumber)const ;

    // Summary:
    //  Gets the type of issuer data.
    //--------------------------------
    mxt_result GetIssuerType(IN unsigned int uName, OUT ENameType* peNameType) const;

    // Summary:
    //  Gets the value of an issuer of type eNAME_TYPE_DNS_NAME.
    //  An error is returned if there is a type mismatch.
    //-----------------------------------------------------------
    mxt_result GetDnsIssuer(IN unsigned int uName, OUT CString* pstrDnsName) const;

    // Summary:
    //  Gets the value of an issuer of type eNAME_TYPE_IP_ADDRESS.
    //  An error is returned if there is a type mismatch.
    //-------------------------------------------------------------
    mxt_result GetIpAddressIssuer(IN unsigned int uName, OUT CString* pstrIpAddress) const;

    // Summary:
    //  Gets the value of an issuer of type eNAME_TYPE_RFC_822_NAME (aka email).
    //  An error is returned if there is a type mismatch.
    //--------------------------------------------------------------------------
    mxt_result GetRfc822NameIssuer(IN unsigned int uName, OUT CString* pstrRfc822Name) const;

    // Summary:
    //  Gets the value of an issuer of type
    //  eNAME_TYPE_UNIFORM_RESOURCE_IDENTIFIER. An error is returned if
    //  there is a type mismatch.
    //------------------------------------------------------------------
    mxt_result GetUniformResourceIdentifierIssuer(IN unsigned int uName,
                                                  OUT CString* pstrUniformResourceIdentifier) const;

// Hidden Methods
//----------------
protected:

mxt_result GetCertificateIssuer(IN unsigned int uName,
                                IN ENameType eIssuerType,
                                OUT CString* pstrName) const;
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CAuthorityKeyIdentifierOpenSsl::CAuthorityKeyIdentifierOpenSsl()
: CCertificateExtension()
{
}

inline
CAuthorityKeyIdentifierOpenSsl::CAuthorityKeyIdentifierOpenSsl(
                                                      IN const CAuthorityKeyIdentifierOpenSsl* pAuthorityKeyIdentifier)
: CCertificateExtension(pAuthorityKeyIdentifier)
{
}

inline
CAuthorityKeyIdentifierOpenSsl::CAuthorityKeyIdentifierOpenSsl(
                                                      IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier)
: CCertificateExtension(&rAuthorityKeyIdentifier)
{
}

inline
CAuthorityKeyIdentifierOpenSsl::~CAuthorityKeyIdentifierOpenSsl()
{
}

inline
CAuthorityKeyIdentifierOpenSsl& CAuthorityKeyIdentifierOpenSsl::operator=(
                                                      IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier)
{
    *static_cast<CCertificateExtension*>(this) = rAuthorityKeyIdentifier;

    return *this;
}

inline
bool CAuthorityKeyIdentifierOpenSsl::operator!=(IN const CAuthorityKeyIdentifierOpenSsl& rAuthorityKeyIdentifier) const
{
    return !(operator==(rAuthorityKeyIdentifier));
}

inline
mxt_result CAuthorityKeyIdentifierOpenSsl::GetDnsIssuer(IN unsigned int uName,
                                                 OUT CString* pstrDnsName) const
{
    return GetCertificateIssuer(uName,
                                eNAME_TYPE_DNS_NAME,
                                pstrDnsName);
}

inline
mxt_result CAuthorityKeyIdentifierOpenSsl::GetIpAddressIssuer(IN unsigned int uName,
                                                       OUT CString* pstrIpAddress) const
{
    return GetCertificateIssuer(uName,
                                eNAME_TYPE_IP_ADDRESS,
                                pstrIpAddress);
}

inline
mxt_result CAuthorityKeyIdentifierOpenSsl::GetRfc822NameIssuer(IN unsigned int uName,
                                                        OUT CString* pstrRfc822Name) const
{
    return GetCertificateIssuer(uName,
                                eNAME_TYPE_RFC_822_NAME,
                                pstrRfc822Name);
}

inline
mxt_result CAuthorityKeyIdentifierOpenSsl::GetUniformResourceIdentifierIssuer(IN unsigned int uName,
                                                                       OUT CString* pstrUniformResourceIdentifier) const
{
    return GetCertificateIssuer(uName,
                                eNAME_TYPE_UNIFORM_RESOURCE_IDENTIFIER,
                                pstrUniformResourceIdentifier);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CAUTHORITYKEYIDENTIFIEROPENSSL_H
//M5T_INTERNAL_USE_END
