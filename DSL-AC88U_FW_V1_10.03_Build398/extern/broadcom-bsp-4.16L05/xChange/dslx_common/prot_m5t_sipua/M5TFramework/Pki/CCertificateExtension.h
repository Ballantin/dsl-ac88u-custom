//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CCERTIFICATEEXTENSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATEEXTENSION_H
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

#if defined(MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE)
    #include MXD_PKI_CCERTIFICATEEXTENSION_INCLUDE
#else
    #error "No implementation defined for CCertificateExtension!!!"
#endif
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
class CBlob;
class CBasicConstraints;
class CExtendedKeyUsage;
class CIssuerAlternateName;
class CKeyUsage;
class CNetscapeCertificateType;
class CSubjectAlternateName;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CCERTIFICATEEXTENSION)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CCERTIFICATEEXTENSIONSTUB_H
    #include "PkiStubs/CCertificateExtensionStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CCertificateExtension
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class managing certificate extensions.
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
class CCertificateExtension : public MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME
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
    CCertificateExtension();

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtension(IN const CCertificateExtension* pCertificateExtension);

    // Summary:
    // Copy constructor.
    //-------------------
    CCertificateExtension(IN const CCertificateExtension& rCertificateExtension);

    // Summary:
    // Destructor.
    //-------------
    virtual ~CCertificateExtension();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateExtension& operator=(IN const CCertificateExtension& rCertificateExtension);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CCertificateExtension& rCertificateExtension) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CCertificateExtension& rCertificateExtension) const;

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

//==============================================================================
//==
//==    CCertificateExtension
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CCertificateExtension::CCertificateExtension()
:   MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME()
{
}

//==============================================================================
//==
//==    CCertificateExtension
//==
//==============================================================================
//
//  Parameters:
//      pCertificateExtension:
//          Pointer to the CCertificateExtension to copy.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline
CCertificateExtension::CCertificateExtension(IN const CCertificateExtension* pCertificateExtension)
:   MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME(pCertificateExtension)
{
}

//==============================================================================
//==
//==    CCertificateExtension
//==
//==============================================================================
//
//  Parameters:
//      rCertificateExtension:
//          Reference to the CCertificateExtension to copy.
//
//  Description:
//      Copy constructor.
//
//==============================================================================
inline
CCertificateExtension::CCertificateExtension(IN const CCertificateExtension& rCertificateExtension)
:   MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME(rCertificateExtension)
{
}

//==============================================================================
//==
//==    ~CCertificateExtension
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CCertificateExtension::~CCertificateExtension()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateExtension:
//          Reference to the CCertificateExtension to assign.
//
//  Returns:
//      A reference to the assigned extension.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==============================================================================
inline
CCertificateExtension& CCertificateExtension::operator=(IN const CCertificateExtension& rCertificateExtension)
{
    MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::operator=(rCertificateExtension);
    return *this;
}

//==============================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rCertificateExtension:
//          Reference to the CCertificateExtension to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline
bool CCertificateExtension::operator==(IN const CCertificateExtension& rCertificateExtension) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::operator==(rCertificateExtension);
}

//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rCertificateExtension:
//          Reference to the CCertificateExtension to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline
bool CCertificateExtension::operator!=(IN const CCertificateExtension& rCertificateExtension) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::operator!=(rCertificateExtension);
}

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==    GetBasicConstraints
//==
//==============================================================================
//
//  Parameters:
//      pBasicConstraints:
//          Pointer to contain the Basic Constraints extension.
//
//  Returns:
//      resFE_FAIL
//      resS_OK.
//
//  Description:
//      This method gets the Basic Constraints extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetBasicConstraints(OUT CBasicConstraints* pBasicConstraints) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetBasicConstraints(OUT pBasicConstraints);
}

//==============================================================================
//==
//==    GetExtendedKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      pExtendedKeyUsage:
//          Pointer to contain the Extended Key Usage extension.
//
//  Returns:
//      resFE_FAIL
//      resS_OK.
//
//  Description:
//      This method gets the Extended Key Usage extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetExtendedKeyUsage(OUT CExtendedKeyUsage* pExtendedKeyUsage) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetExtendedKeyUsage(OUT pExtendedKeyUsage);
}

//==============================================================================
//==
//==    GetIssuerAlternateName
//==
//==============================================================================
//
//  Parameters:
//      pIssuerAlternateName:
//          Pointer to contain the Issuer Alternate Name extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      This method gets the Issuer Alternate Name extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetIssuerAlternateName(OUT CIssuerAlternateName* pIssuerAlternateName) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetIssuerAlternateName(OUT pIssuerAlternateName);
}

//==============================================================================
//==
//==    GetKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      pKeyUsage:
//          Pointer to contain the Key Usage extension.
//
//  Returns:
//      resFE_FAIL
//      resS_OK.
//
//  Description:
//      This method gets the Key Usage extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetKeyUsage(OUT CKeyUsage* pKeyUsage) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetKeyUsage(OUT pKeyUsage);
}

//==============================================================================
//==
//==    CNetscapeCertificateType
//==
//==============================================================================
//
//  Parameters:
//      pNetscapeCertificateType:
//          Pointer to contain the Netscape Certificate Type extension.
//
//  Returns:
//      resFE_FAIL
//      resS_OK.
//
//  Description:
//      This method gets the Netscape Certificate Type extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetNetscapeCertificateType(OUT CNetscapeCertificateType* pNetscapeCertificateType) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetNetscapeCertificateType(OUT pNetscapeCertificateType);
}

//==============================================================================
//==
//==    GetSubjectAlternateName
//==
//==============================================================================
//
//  Parameters:
//      pSubjectAlternateName:
//          Pointer to contain the Subject Alternate Name extension.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      This method gets the Subject Alternate Name extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetSubjectAlternateName(OUT CSubjectAlternateName* pSubjectAlternateName) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetSubjectAlternateName(OUT pSubjectAlternateName);
}

//==============================================================================
//==
//==    GetType
//==
//==============================================================================
//
//  Parameters:
//      peType:
//          Pointer to contain the extension type.
//
//  Returns:
//      - resFE_INVALID_ARGUMENT
//      - resFE_FAIL
//      - resS_OK
//
//  Description:
//      Gets the type of certificate extension.
//
//==============================================================================
inline
mxt_result CCertificateExtension::GetType(OUT EType* peType) const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::GetType(OUT peType);
}

//==============================================================================
//==
//==    IsCritical
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resFE_INVALID_STATE
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if an extension is critical.
//
//  NOTES:
//      Unrecognized extensions may be ignored if IsCritical() returns
//      resSI_FALSE. If IsCritical() returns resSI_TRUE, then the extension is
//      considered invalid and the certificate must also be considered invalid.
//
//==============================================================================
inline
mxt_result CCertificateExtension::IsCritical() const
{
    return MXD_PKI_CCERTIFICATEEXTENSION_CLASSNAME::IsCritical();
}


#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CCERTIFICATEEXTENSION)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATEEXTENSION_H

