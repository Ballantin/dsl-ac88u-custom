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
#ifndef MXG_CBASICCONSTRAINTS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CBASICCONSTRAINTS_H
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
#if defined(MXD_PKI_CBASICCONSTRAINTS_INCLUDE)
    #include MXD_PKI_CBASICCONSTRAINTS_INCLUDE
#else
    #error "No implementation defined for CBasicConstraints!!!"
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
    defined(MXD_STUB_CBASICCONSTRAINTS)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CBASICCONSTRAINTSSTUB_H
    #include "PkiStubs/CBasicConstraintsStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==============================================================================
//== Class: CBasicConstraints
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the Basic Constraints extensions inside certificates.
//
// Description:
//   CBasicConstraints is the class to handle the Basic Constraints extensions
//   inside certificates.
//
// Notes:
//   It may occur that some older certificates encountered have the obsolete ID
//   for the basic constraints extension 2.5.29.10 instead of 2.5.29.19. If this
//   is the case, the extension is considered as an unknown extension.
//
// Location:
//   Pki/CBasicConstraints.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CBasicConstraints : public MXD_PKI_CBASICCONSTRAINTS_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CBasicConstraints();

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraints(IN const CBasicConstraints* pBasicConstraints);

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraints(IN const CBasicConstraints& rBasicConstraints);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CBasicConstraints();

    // Summary:
    // Assignment operator.
    //----------------------
    CBasicConstraints& operator=(IN const CBasicConstraints& rBasicConstraints);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CBasicConstraints& rBasicConstraints) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CBasicConstraints& rBasicConstraints) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the path length constraint.
    //-----------------------------------
    mxt_result GetPathLengthConstraint(OUT unsigned int* puPathLengthConstraint) const;

    // Summary:
    //  Verifies if it is a certificate authority.
    //---------------------------------------------
    mxt_result IsACertificateAuthority() const;

// Hidden Methods
//----------------
protected:
private:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==    CBasicConstraints
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CBasicConstraints::CBasicConstraints()
:   MXD_PKI_CBASICCONSTRAINTS_CLASSNAME()
{
}

//==============================================================================
//==
//==    CBasicConstraints
//==
//==============================================================================
//
//  Parameters:
//      pBasicConstraints:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CBasicConstraints::CBasicConstraints(IN const CBasicConstraints* pBasicConstraints)
:   MXD_PKI_CBASICCONSTRAINTS_CLASSNAME(pBasicConstraints)
{
}

//==============================================================================
//==
//==    CBasicConstraints
//==
//==============================================================================
//
//  Parameters:
//      rBasicConstraints:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==============================================================================
inline
CBasicConstraints::CBasicConstraints(IN const CBasicConstraints& rBasicConstraints)
:   MXD_PKI_CBASICCONSTRAINTS_CLASSNAME(&rBasicConstraints)
{
}

//==============================================================================
//==
//==    ~CBasicConstraints
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==============================================================================
inline
CBasicConstraints::~CBasicConstraints()
{
}

//==============================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rBasicConstraints:
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
CBasicConstraints& CBasicConstraints::operator=(IN const CBasicConstraints& rBasicConstraints)
{
    MXD_PKI_CBASICCONSTRAINTS_CLASSNAME::operator=(rBasicConstraints);
    return *this;
}

//==============================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rBasicConstraints:
//          Reference to the CBasicConstraints to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==============================================================================
inline
bool CBasicConstraints::operator!=(IN const CBasicConstraints& rBasicConstraints) const
{
    return MXD_PKI_CBASICCONSTRAINTS_CLASSNAME::operator!=(rBasicConstraints);
}

//==============================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rBasicConstraints:
//          Reference to the CBasicConstraints to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==============================================================================
inline
bool CBasicConstraints::operator==(IN const CBasicConstraints& rBasicConstraints) const
{
    return MXD_PKI_CBASICCONSTRAINTS_CLASSNAME::operator==(rBasicConstraints);
}

//==============================================================================
//==
//==    GetPathLengthConstraint
//==
//==============================================================================
//
//  Parameters:
//      puPathLengthConstraint:
//          Pointer to contain the path length constraint.
//
//  Returns:
//      - resFE_FAIL
//      - resFE_INVALID_ARGUMENT
//      - resSI_FALSE: The Path Length Contraint does not exist.
//      - resSI_TRUE: The Path Length Contraint exists.
//
//  Description:
//      Gets the path length constraint. The path length constraint is the
//      maximum number of certificate authorities that can be included above
//      this certificate in the certificate path.
//
//==============================================================================
inline
mxt_result CBasicConstraints::GetPathLengthConstraint(OUT unsigned int* puPathLengthConstraint) const
{
    return MXD_PKI_CBASICCONSTRAINTS_CLASSNAME::GetPathLengthConstraint(OUT puPathLengthConstraint);
}

//==============================================================================
//==
//==    IsACertificateAuthority
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
//      Verifies if the certificate has a certificate authority extension.
//
//==============================================================================
inline
mxt_result CBasicConstraints::IsACertificateAuthority() const
{
    return MXD_PKI_CBASICCONSTRAINTS_CLASSNAME::IsACertificateAuthority();
}
#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CBASICCONSTRAINTS)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CBASICCONSTRAINTS_H

