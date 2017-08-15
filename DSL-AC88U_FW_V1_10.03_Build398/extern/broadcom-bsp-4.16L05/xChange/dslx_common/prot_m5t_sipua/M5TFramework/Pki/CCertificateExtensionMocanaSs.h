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
#ifndef MXG_CCERTIFICATEEXTENSIONMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEEXTENSIONMOCANASS_H

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

#if defined(MXD_PKI_MOCANA_SS)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

#ifndef MXG_CCERTIFICATEDESCRIPTORMOCANASS_H
#include "Pki/CCertificateDescriptorMocanaSs.h"
#endif

extern "C"
{
#ifndef __ASN1TREEWALKER_HEADER__
#include "asn1/ASN1TreeWalker.h"
#endif
#ifndef __CA_MGMT_HEADER__
#include "crypto/ca_mgmt.h"
#endif
}

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CCrypto;
class CString;

class CBasicConstraints;
class CExtendedKeyUsage;
class CIssuerAlternateName;
class CKeyUsage;
class CNetscapeCertificateType;
class CSubjectAlternateName;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME
    #define MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME CCertificateExtensionMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCertificateExtensionMocanaSs
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
//==============================================================================
class CCertificateExtensionMocanaSs
{
    friend class CCertificateMocanaSs;

// Published Enum
//----------------
public:

    static uint8_t m_auEXTENDED_KEY_USAGE_OID[]; // 2.5.29.37.

    // Summary:
    //  Defines the possible extension types.
    enum EType
    {
        // Subject key identifier extension type.
        eTYPE_SUBJECT_KEY_IDENTIFIER,
        // Key usage extension type.
        eTYPE_KEY_USAGE,
        // Subject alternative name extension type.
        eTYPE_SUBJECT_ALTERNATIVE_NAME,
        // Issuer alternative name extension type.
        eTYPE_ISSUER_ALTERNATIVE_NAME,
        // Basic constraints extension type.
        eTYPE_BASIC_CONSTRAINTS,
        // Certificate policies extension type.
        eTYPE_CERTIFICATE_POLICIES,
        // Authority key identifier extension type.
        eTYPE_AUTHORITY_KEY_IDENTIFIER,
        // Extended key usage extension type.
        eTYPE_EXTENDED_KEY_USAGE,
        // Netscape certificate extension type.
        eTYPE_NETSCAPE_CERTIFICATE_TYPE,
        // Unkown extension type.
        eTYPE_UNKNOWN
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
    CCertificateExtensionMocanaSs();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtensionMocanaSs(IN const CCertificateExtensionMocanaSs* pCertificateExtension);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtensionMocanaSs(IN const CCertificateExtensionMocanaSs& rCertificateExtension);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateExtensionMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateExtensionMocanaSs& operator=(IN const CCertificateExtensionMocanaSs& rCertificateExtension);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateExtensionMocanaSs& rCertificateExtension) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificateExtensionMocanaSs& rCertificateExtension) const;

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

    // Summary:
    //  Retreives the leaf corresponding to the beginning of the desired 
    //  extension in the parsed certificate.
    //-------------------------------------------------------------------
    mxt_result FindExtension(IN ASN1_ITEM* pExtensionsBeginning, 
                             IN uint32_t uExtension, 
                             OUT ASN1_ITEM** ppExtension, 
                             OUT ASN1_ITEM** ppExtensionData, 
                             OUT bool* pbCritical) const;

// Hidden Methods
//----------------
protected:

    // Summary:
    //  Initialize this object with counted certDescriptor and an extension 
    //  identifier.
    //----------------------------------------------------------------------
    mxt_result Initialize(IN CCertificateDescriptorMocanaSs* pCertDescriptorCounted, IN unsigned int uExtension);

    mxt_result GetNameCount(IN const WalkerStep* pastWalkerStep, 
                            OUT unsigned int* puNameCount) const;

    mxt_result GetNameType(IN unsigned int uName, 
                           IN const WalkerStep* pastWalkerStep, 
                           OUT ENameType* peNameType) const;

    // Summary:
    //  Verifies if the extension is of the specified type.
    //------------------------------------------------------
    mxt_result IsType(IN EType eType) const;

    mxt_result GetName(IN unsigned int uName,
                       IN uint32_t uNameTag,
                       IN const WalkerStep* pastWalkerStep, 
                       OUT CString* pstrName) const;
    mxt_result GetOctetString(IN const WalkerStep* pastWalkerStep,
                              OUT CBlob* pblobOctetString) const;

    // Summary:
    //  Enters the concurrency protection domain.
    //--------------------------------------------
    void Enter() const;
    // Summary:
    //  Exits the concurrency protection domain.
    //-------------------------------------------
    void Exit() const;

private:

// Hidden Data Members
//---------------------
protected:

    // Use to enter and exit the concurrency protection domain.
    CCrypto* m_pCrypto;
    // Holder of the related certDescriptor.
    CCertificateDescriptorMocanaSs* m_pCertDescriptorCounted;

    // Pointer to the leaf corresponding to the beginning of the extension in 
    // the parsed certificate.
    ASN1_ITEM* m_pExtension;

    // Pointer to the leaf corresponding to the beginning of the extension 
    // data in the parsed certificate.
    ASN1_ITEM* m_pExtensionData;

    // Extension identifier.
    unsigned int m_uExtension;

    // Extension type.
    EType m_eType;

    // Is a crtical extension.
    bool m_bCritical;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==   Enter
//==
//==============================================================================
//
//  Description:
//    Enters the concurrency protection domain. Note that this method reserves
//    the system wide crypto critical section. Exit from it quickly by using the
//    method Exit.
//
//==============================================================================
inline
void CCertificateExtensionMocanaSs::Enter() const
{
    m_pCrypto->Enter();
}

//==============================================================================
//==
//==   Exit
//==
//==============================================================================
//
//  Description:
//      Exits the concurrency protection domain.
//
//==============================================================================
inline
void CCertificateExtensionMocanaSs::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)


#endif // #if defined(MXD_PKI_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATEEXTENSIONMOCANASS_H

