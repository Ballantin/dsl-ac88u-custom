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
#ifndef MXG_CBASICCONSTRAINTSMOCANASS_H
#define MXG_CBASICCONSTRAINTSMOCANASS_H

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
#include "Config/FrameworkCfg.h" // MXD_PKI_MOCANA_SS
#endif

#if defined(MXD_PKI_MOCANA_SS)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif // #ifndef MXG_CPKI_H

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATECHAIN_CLASSNAME
    #define MXD_PKI_CBASICCONSTRAINTS_CLASSNAME CBasicConstraintsMocanaSs
#endif
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
//== Class: CBasicConstraintsMocanaSs
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the Basic Constraints extensions inside certificates.
//
// Description:
//   CBasicConstraintsMocanaSs is the class to handle the Basic Constraints 
//   extensions inside certificates using the Mocana SS functionalities.
//
// Location:
//   Pki/CBasicConstraintsMocanaSs.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CBasicConstraintsMocanaSs : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CBasicConstraintsMocanaSs();

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraintsMocanaSs(IN const CBasicConstraintsMocanaSs* pBasicConstraints);

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraintsMocanaSs(IN const CBasicConstraintsMocanaSs& rBasicConstraints);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CBasicConstraintsMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CBasicConstraintsMocanaSs& operator=(IN const CBasicConstraintsMocanaSs& rBasicConstraints);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CBasicConstraintsMocanaSs& rBasicConstraints) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CBasicConstraintsMocanaSs& rBasicConstraints) const;

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

inline
CBasicConstraintsMocanaSs::CBasicConstraintsMocanaSs()
: CCertificateExtension()
{
}

inline
CBasicConstraintsMocanaSs::CBasicConstraintsMocanaSs(IN const CBasicConstraintsMocanaSs* pBasicConstraints)
: CCertificateExtension(pBasicConstraints)
{
}

inline
CBasicConstraintsMocanaSs::CBasicConstraintsMocanaSs(IN const CBasicConstraintsMocanaSs& rBasicConstraints)
: CCertificateExtension(&rBasicConstraints)
{
}

inline
CBasicConstraintsMocanaSs::~CBasicConstraintsMocanaSs()
{
}

inline
CBasicConstraintsMocanaSs& CBasicConstraintsMocanaSs::operator=(IN const CBasicConstraintsMocanaSs& rBasicConstraints)
{
    *static_cast<CCertificateExtension*>(this) = rBasicConstraints;

    return *this;
}

inline
bool CBasicConstraintsMocanaSs::operator!=(IN const CBasicConstraintsMocanaSs& rBasicConstraints) const
{
    return !(operator==(rBasicConstraints));
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CBASICCONSTRAINTS)

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

#endif // MXG_CBASICCONSTRAINTSMOCANASS_H
//M5T_INTERNAL_USE_END
