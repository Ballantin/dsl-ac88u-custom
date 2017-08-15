//==============================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CEXTENDEDKEYUSAGE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CEXTENDEDKEYUSAGE_H
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
#if defined(MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE)
    #include MXD_PKI_CEXTENDEDKEYUSAGE_INCLUDE
#else
    #error "No implementation defined for CExtendedKeyUsage!!!"
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
    defined(MXD_STUB_CEXTENDEDKEYUSAGE)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CEXTENDEDKEYUSAGESTUB_H
    #include "PkiStubs/CExtendedKeyUsageStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CExtendedKeyUsage
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//    Class to handle the extended key usage extensions.
//
// Description:
//   CExtendedKeyUsage is the class to handle the extended key usage extensions
//   inside certificates.
//
// Location:
//   Pki/CExtendedKeyUsage.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CExtendedKeyUsage : public MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CExtendedKeyUsage();

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsage(IN const CExtendedKeyUsage* pExtendedKeyUsage);

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsage(IN const CExtendedKeyUsage& rExtendedKeyUsage);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CExtendedKeyUsage();

    // Summary:
    // Assignment operator.
    //----------------------
    CExtendedKeyUsage& operator=(IN const CExtendedKeyUsage& rExtendedKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CExtendedKeyUsage& rExtendedKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CExtendedKeyUsage& rExtendedKeyUsage) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Verifies if the client authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsClientAuthenticationSet() const;

    // Summary:
    //  Verifies if the code signed flag is set.
    //-------------------------------------------
    mxt_result IsCodeSignedSet() const;

    // Summary:
    //  Verifies if the OSCP signed flag is set.
    //-------------------------------------------
    mxt_result IsOcspSignedSet() const;

    // Summary:
    //  Verifies if the server authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsServerAuthenticationSet() const;

    // Summary:
    //  Verifies if the SMIME flag is set.
    //-------------------------------------
    mxt_result IsSmimeSet() const;

    // Summary:
    //  Verifies if the timestamping flag is set.
    //--------------------------------------------
    mxt_result IsTimestampingSet() const;

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
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rExtendedKeyUsage:
//          Reference to the CExtendedKeyUsage to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline
bool CExtendedKeyUsage::operator==(IN const CExtendedKeyUsage& rExtendedKeyUsage) const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::operator==(rExtendedKeyUsage);
}
//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rExtendedKeyUsage:
//          Reference to the CExtendedKeyUsage to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline
bool CExtendedKeyUsage::operator!=(IN const CExtendedKeyUsage& rExtendedKeyUsage) const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::operator!=(rExtendedKeyUsage);
}

//==============================================================================
//==
//==    CExtendedKeyUsage
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CExtendedKeyUsage::CExtendedKeyUsage()
: MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME()
{
}

//==============================================================================
//==
//==    CExtendedKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      pExtendedKeyUsage:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CExtendedKeyUsage::CExtendedKeyUsage(IN const CExtendedKeyUsage* pExtendedKeyUsage)
: MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME(pExtendedKeyUsage)
{
}

//==============================================================================
//==
//==    CExtendedKeyUsage
//==
//==============================================================================
//
//  Parameters:
//      rExtendedKeyUsage:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CExtendedKeyUsage::CExtendedKeyUsage(IN const CExtendedKeyUsage& pExtendedKeyUsage)
: MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME(&pExtendedKeyUsage)
{
}

//==============================================================================
//==
//==    ~CExtendedKeyUsage
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CExtendedKeyUsage::~CExtendedKeyUsage()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rExtendedKeyUsage:
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
CExtendedKeyUsage& CExtendedKeyUsage::operator=(IN const CExtendedKeyUsage& rExtendedKeyUsage)
{
    MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::operator=(rExtendedKeyUsage);
    return *this;
}

//==============================================================================
//==
//==    IsClientAuthenticationSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the client authentication flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsClientAuthenticationSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsClientAuthenticationSet();
}

//==============================================================================
//==
//==    IsCodeSignedSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the code signed flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsCodeSignedSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsCodeSignedSet();
}

//==============================================================================
//==
//==    IsOcspSignedSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the OSCP signed flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsOcspSignedSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsOcspSignedSet();
}

//==============================================================================
//==
//==    IsServerAuthenticationSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the server authentication flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsServerAuthenticationSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsServerAuthenticationSet();
}

//==============================================================================
//==
//==    IsSmimeSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the SMIME flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsSmimeSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsSmimeSet();
}

//==============================================================================
//==
//==    IsTimestampingSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the timestamping flag is set.
//
//==============================================================================
inline
mxt_result CExtendedKeyUsage::IsTimestampingSet() const
{
    return MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME::IsTimestampingSet();
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CEXTENDEDKEYUSAGE)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CEXTENDEDKEYUSAGE_H

