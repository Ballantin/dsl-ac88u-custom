//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CKEYUSAGE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CKEYUSAGE_H
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
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Interface Realized and/or Parent
//-------------------------------------
#if defined(MXD_PKI_CKEYUSAGE_INCLUDE)
    #include MXD_PKI_CKEYUSAGE_INCLUDE
#else
    #error "No implementation defined for CKeyUsage!!!"
#endif

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
    defined(MXD_STUB_CKEYUSAGE)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CKEYUSAGESTUB_H
    #include "PkiStubs/CKeyUsageStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CKeyUsage
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the key usage extensions inside certificates.
//
// Description:
//   CKeyUsage is the class to handle the key usage extensions inside
//   certificates.
//
// Location:
//   Pki/CKeyUsage.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CKeyUsage : public MXD_PKI_CKEYUSAGE_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CKeyUsage();

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsage(IN const CKeyUsage* pKeyUsage);

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsage(IN const CKeyUsage& rKeyUsage);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CKeyUsage();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CKeyUsage& operator=(IN const CKeyUsage& rKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CKeyUsage& rKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CKeyUsage& rKeyUsage) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Verifies if the certificate signing flag is set.
    //---------------------------------------------------
    mxt_result IsCertificateSigningSet() const;

    // Summary:
    //  Verifies if the CRL signed flag is set.
    //------------------------------------------
    mxt_result IsCrlSignedSet() const;

    // Summary:
    //  Verifies if the data encipherment flag is set.
    //-------------------------------------------------
    mxt_result IsDataEnciphermentSet() const;

    // Summary:
    //  Verifies if the decipher only flag is set.
    //---------------------------------------------
    mxt_result IsDecipherOnlySet() const;

    // Summary:
    //  Verifies if the digital signature flag is set.
    //-------------------------------------------------
    mxt_result IsDigitalSignatureSet() const;

    // Summary:
    //  Verifies if the encipher only flag is set.
    //---------------------------------------------
    mxt_result IsEncipherOnlySet() const;

    // Summary:
    //  Verifies if the key agreement flag is set.
    //---------------------------------------------
    mxt_result IsKeyAgreementSet() const;

    // Summary:
    //  Verifies if the key encipherment flag is set.
    //------------------------------------------------
    mxt_result IsKeyEnciphermentSet() const;

    // Summary:
    //  Verifies if the non repudiation flag is set.
    //-----------------------------------------------
    mxt_result IsNonRepudiationSet() const;

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
//==    CKeyUsage
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CKeyUsage::CKeyUsage()
:   MXD_PKI_CKEYUSAGE_CLASSNAME()
{
}

//==============================================================================
//==
//==    CKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      pKeyUsage:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CKeyUsage::CKeyUsage(IN const CKeyUsage* pKeyUsage)
:   MXD_PKI_CKEYUSAGE_CLASSNAME(pKeyUsage)
{
}

//==============================================================================
//==
//==    CKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      rKeyUsage:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CKeyUsage::CKeyUsage(IN const CKeyUsage& rKeyUsage)
: MXD_PKI_CKEYUSAGE_CLASSNAME(&rKeyUsage)
{
}

//==============================================================================
//==
//==    ~CKeyUsage
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CKeyUsage::~CKeyUsage()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rKeyUsage:
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
CKeyUsage& CKeyUsage::operator=(IN const CKeyUsage& rKeyUsage)
{
    MXD_PKI_CKEYUSAGE_CLASSNAME::operator=(rKeyUsage);
    return *this;
}

//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rKeyUsage:
//          Reference to the CKeyUsage to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline
bool CKeyUsage::operator!=(IN const CKeyUsage& rKeyUsage) const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::operator!=(rKeyUsage);
}

//==============================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rKeyUsage:
//          Reference to the CKeyUsage to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline
bool CKeyUsage::operator==(IN const CKeyUsage& rKeyUsage) const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::operator==(rKeyUsage);
}
//==============================================================================
//==
//==    IsCertificateSigningSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the certificate signing flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsCertificateSigningSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsCertificateSigningSet();
}

//==============================================================================
//==
//==    IsCrlSignedSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the CRL signed flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsCrlSignedSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsCrlSignedSet();    
}

//==============================================================================
//==
//==    IsDataEnciphermentSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the data encipherment flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsDataEnciphermentSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsDataEnciphermentSet();
}

//==============================================================================
//==
//==    IsDecipherOnlySet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the decipher only flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsDecipherOnlySet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsDecipherOnlySet();
}

//==============================================================================
//==
//==    IsDigitalSignatureSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the digital signature flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsDigitalSignatureSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsDigitalSignatureSet();
}

//==============================================================================
//==
//==    IsEncipherOnlySet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the encipher only flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsEncipherOnlySet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsEncipherOnlySet();
}

//==============================================================================
//==
//==    IsKeyAgreementSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the key agreement flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsKeyAgreementSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsKeyAgreementSet();
}

//==============================================================================
//==
//==    IsKeyEnciphermentSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the key encipherment flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsKeyEnciphermentSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsKeyEnciphermentSet();
}

//==============================================================================
//==
//==    IsNonRepudiationSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the non repudiation flag is set.
//
//==============================================================================
inline
mxt_result CKeyUsage::IsNonRepudiationSet() const
{
    return MXD_PKI_CKEYUSAGE_CLASSNAME::IsNonRepudiationSet();
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CKEYUSAGE)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CKEYUSAGE_H

