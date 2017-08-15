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
#ifndef MXG_CBASICCONSTRAINTSOPENSSL_H
#define MXG_CBASICCONSTRAINTSOPENSSL_H

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
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CBASICCONSTRAINTS_CLASSNAME
    #define MXD_PKI_CBASICCONSTRAINTS_CLASSNAME CBasicConstraintsOpenSsl
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CBasicConstraintsOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the Basic Constraints extensions inside certificates.
//
// Description:
//   CBasicConstraintsOpenSsl is the class to handle the Basic Constraints 
//   extensions inside certificates using the Open SSL functionalities.
//
// Notes:
//   It may occur that some older certificates encountered have the obsolete ID
//   for the basic constraints extension 2.5.29.10 instead of 2.5.29.19. If this
//   is the case, the extension is considered as an unknown extension.
//
// Location:
//   Pki/CBasicConstraintsOpenSsl.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CBasicConstraintsOpenSsl : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CBasicConstraintsOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraintsOpenSsl(IN const CBasicConstraintsOpenSsl* pBasicConstraints);

    // Summary:
    // Copy constructor.
    //-------------------
    CBasicConstraintsOpenSsl(IN const CBasicConstraintsOpenSsl& rBasicConstraints);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CBasicConstraintsOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CBasicConstraintsOpenSsl& operator=(IN const CBasicConstraintsOpenSsl& rBasicConstraints);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CBasicConstraintsOpenSsl& rBasicConstraints) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CBasicConstraintsOpenSsl& rBasicConstraints) const;

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
CBasicConstraintsOpenSsl::CBasicConstraintsOpenSsl()
: CCertificateExtension()
{
}

inline
CBasicConstraintsOpenSsl::CBasicConstraintsOpenSsl(IN const CBasicConstraintsOpenSsl* pBasicConstraints)
: CCertificateExtension(pBasicConstraints)
{
}

inline
CBasicConstraintsOpenSsl::CBasicConstraintsOpenSsl(IN const CBasicConstraintsOpenSsl& rBasicConstraints)
: CCertificateExtension(&rBasicConstraints)
{
}

inline
CBasicConstraintsOpenSsl::~CBasicConstraintsOpenSsl()
{
}

inline
CBasicConstraintsOpenSsl& CBasicConstraintsOpenSsl::operator=(IN const CBasicConstraintsOpenSsl& rBasicConstraints)
{
    *static_cast<CCertificateExtension*>(this) = rBasicConstraints;

    return *this;
}

inline
bool CBasicConstraintsOpenSsl::operator!=(IN const CBasicConstraintsOpenSsl& rBasicConstraints) const
{
    return !(operator==(rBasicConstraints));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CBASICCONSTRAINTSOPENSSL_H
//M5T_INTERNAL_USE_END
