//==============================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==============================================================================
#ifndef MXG_CAUTHORITYKEYIDENTIFIER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CAUTHORITYKEYIDENTIFIER_H
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

//-- Data Members
//-----------------

//-- Interface Realized and/or Parent
//-------------------------------------
#if defined(MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE)
    #include MXD_PKI_CAUTHORITYKEYIDENTIFIER_INCLUDE
#else
    #error "No implementation defined for CAuthorityKeyIdentifier!!!"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CString;
class CBlob;
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CAUTHORITYKEYIDENTIFIER)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CAUTHORITYKEYIDENTIFIERSTUB_H
    #include "PkiStubs/CAuthorityKeyIdentifierStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CAuthorityKeyIdentifier
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the authority key identifier extensions certificates.
//
// Description:
//   CAuthorityKeyIdentifier is the class to handle the authority key identifier
//   extensions 2.5.29.35. They are used to identify the public key related to
//   the private key used to sign a certificate.
//
// Location:
//   Pki/CAuthorityKeyIdentifier.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CAuthorityKeyIdentifier : public MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CAuthorityKeyIdentifier();

    // Summary:
    //  Copy constructor.
    //--------------------
    CAuthorityKeyIdentifier(IN const CAuthorityKeyIdentifier* pAuthorityKeyIdentifier);

    // Summary:
    //  Copy constructor.
    //--------------------
    CAuthorityKeyIdentifier(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CAuthorityKeyIdentifier();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CAuthorityKeyIdentifier& operator=(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier) const;

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

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==    CAuthorityKeyIdentifier
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CAuthorityKeyIdentifier::CAuthorityKeyIdentifier()
: MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME()
{
}

//==============================================================================
//==
//==    CAuthorityKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      pAuthorityKeyIdentifier:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CAuthorityKeyIdentifier::CAuthorityKeyIdentifier(IN const CAuthorityKeyIdentifier* pAuthorityKeyIdentifier)
: MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME(pAuthorityKeyIdentifier)
{
}

//==============================================================================
//==
//==    CAuthorityKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      rAuthorityKeyIdentifier:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CAuthorityKeyIdentifier::CAuthorityKeyIdentifier(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier)
: MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME(&rAuthorityKeyIdentifier)
{
}

//==============================================================================
//==
//==    ~CAuthorityKeyIdentifier
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CAuthorityKeyIdentifier::~CAuthorityKeyIdentifier()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rAuthorityKeyIdentifier:
//          Reference to the object to assign.
//
//  Returns:
//      A reference to the assigned object.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==============================================================================
inline
CAuthorityKeyIdentifier& CAuthorityKeyIdentifier::operator=(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier)
{
    MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::operator=(rAuthorityKeyIdentifier);
    return *this;
}

//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rAuthorityKeyIdentifier:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline
bool CAuthorityKeyIdentifier::operator!=(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::operator!=(rAuthorityKeyIdentifier);
}

//==============================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rAuthorityKeyIdentifier:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline
bool CAuthorityKeyIdentifier::operator==(IN const CAuthorityKeyIdentifier& rAuthorityKeyIdentifier) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::operator==(rAuthorityKeyIdentifier);
}



//==============================================================================
//==
//==    GetDnsIssuer
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//      pstrDnsName:
//          Pointer to the CString object to contain the return value.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the DNS of the issuer at the specified index.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetDnsIssuer(IN unsigned int uName,
                                                 OUT CString* pstrDnsName) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetDnsIssuer(uName, OUT pstrDnsName);    
}

//==============================================================================
//==
//==    GetIpAddressIssuer
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//      pstrIpAddress:
//          Pointer to the CString object to contain the return value.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the IP address of the issuer at the specified index.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetIpAddressIssuer(IN unsigned int uName,
                                                       OUT CString* pstrIpAddress) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetDnsIssuer(uName, OUT pstrIpAddress);
}

//==============================================================================
//==
//==    GetRfc822NameIssuer
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//      pstrRfc822Name:
//          Pointer to the CString object to contain the return value.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the RFC 822 name of the issuer at the specified index.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetRfc822NameIssuer(IN unsigned int uName,
                                                        OUT CString* pstrRfc822Name) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetRfc822NameIssuer(uName, OUT pstrRfc822Name);
}

//==============================================================================
//==
//==    GetUniformResourceIdentifierIssuer
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Reference to the CAuthorityKeyIdentifier to compare.
//
//      pstrUniformResourceIdentifier:
//          Pointer to the CString object to contain the return value.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the URI of the issuer at the specified index in the issuer.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetUniformResourceIdentifierIssuer(IN unsigned int uName,
                                                                       OUT CString* pstrUniformResourceIdentifier) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetUniformResourceIdentifierIssuer(
                                                                                    uName, 
                                                                                    OUT pstrUniformResourceIdentifier);
}


//==============================================================================
//==
//==    GetCertificateIssuerCount
//==
//==============================================================================
//
//  Parameters:
//      puNameCount:
//          Number of issuers.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the number of certificate issuers that are present.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetCertificateIssuerCount(OUT unsigned int* puNameCount) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetCertificateIssuerCount(OUT puNameCount);
}

//==============================================================================
//==
//==    GetKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      pblobIdentifier:
//          Pointer to the blob to hold the key identifier.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the key identifier for this authority.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetKeyIdentifier(OUT CBlob* pblobIdentifier) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetKeyIdentifier(OUT pblobIdentifier);
}

//==============================================================================
//==
//==    GetCertificateSerialNumber
//==
//==============================================================================
//
//  Parameters:
//      pSerialNumber:
//          Pointer to a blob to hold the serial number.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the certificates serial number for this authority.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetCertificateSerialNumber(OUT CBlob* pSerialNumber) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetCertificateSerialNumber(OUT pSerialNumber);
}

//==============================================================================
//==
//==    GetIssuerType
//==
//==============================================================================
//
//  Parameters:
//      uName:
//          Index of the issuer to retrieve.
//
//      peNameType:
//          The type of issuer contained at that index.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the type of issuer data at the specified index.
//
//==============================================================================
inline
mxt_result CAuthorityKeyIdentifier::GetIssuerType(IN unsigned int uName, OUT ENameType* peNameType) const
{
    return MXD_PKI_CAUTHORITYKEYIDENTIFIER_CLASSNAME::GetIssuerType(uName, OUT peNameType);
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CAUTHORITYKEYIDENTIFIER)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CAUTHORITYKEYIDENTIFIER_H

