//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CISSUERALTERNATENAME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CISSUERALTERNATENAME_H
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

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CALTERNATENAME_H
#include "Pki/CAlternateName.h"
#endif // #ifndef MXG_CALTERNATENAME_H

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
    defined(MXD_STUB_CISSUERALTERNATENAME)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CISSUERALTERNATENAMESTUB_H
    #include "PkiStubs/CIssuerAlternateNameStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else


//==SDOC========================================================================
//== Class: CIssuerAlternateName
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages the issuerAlternateName extension.
//
// Description:
//   CIssuerAlternateName is the class that manages the issuerAlternateName
//   extension.
//
// Location:
//   Pki/CIssuerAlternateName.h
//
// See Also:
//
//==EDOC========================================================================
class CIssuerAlternateName : public CAlternateName
{
//M5T_INTERNAL_USE_BEGIN
    friend class CCertificateExtension;
//M5T_INTERNAL_USE_END

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CIssuerAlternateName();

    // Summary:
    //  Copy constructor.
    //--------------------
    CIssuerAlternateName(IN const CIssuerAlternateName* pIssuerAlternateName);

    // Summary:
    //  Copy constructor.
    //--------------------
    CIssuerAlternateName(IN const CIssuerAlternateName& rIssuerAlternateName);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CIssuerAlternateName();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CIssuerAlternateName& operator=(IN const CIssuerAlternateName& rIssuerAlternateName);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CIssuerAlternateName& rIssuerAlternateName) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CIssuerAlternateName& rIssuerAlternateName) const;

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

//==SDOC========================================================================
//==
//==    CIssuerAlternateName
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CIssuerAlternateName::CIssuerAlternateName()
: CAlternateName()
{
}

//==SDOC========================================================================
//==
//==    CIssuerAlternateName
//==
//==============================================================================
//
//  Parameters:
//      pIssuerAlternateName:
//          Pointer to the CIssuerAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CIssuerAlternateName::CIssuerAlternateName(IN const CIssuerAlternateName* pIssuerAlternateName)
: CAlternateName(pIssuerAlternateName)
{
}

//==SDOC========================================================================
//==
//==    CIssuerAlternateName
//==
//==============================================================================
//
//  Parameters:
//      rIssuerAlternateName:
//          Reference to the CIssuerAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CIssuerAlternateName::CIssuerAlternateName(IN const CIssuerAlternateName& rIssuerAlternateName)
: CAlternateName(&rIssuerAlternateName)
{
}

//==SDOC========================================================================
//==
//==    ~CIssuerAlternateName
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CIssuerAlternateName::~CIssuerAlternateName()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rIssuerAlternateName:
//          Reference to the CIssuerAlternateName to assign.
//
//  Returns:
//      A reference to the assigned object.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==EDOC========================================================================
inline
CIssuerAlternateName& CIssuerAlternateName::operator=(IN const CIssuerAlternateName& rIssuerAlternateName)
{
    *static_cast<CAlternateName*>(this) = rIssuerAlternateName;

    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rIssuerAlternateName:
//          Reference to the CIssuerAlternateName to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CIssuerAlternateName::operator==(IN const CIssuerAlternateName& rIssuerAlternateName) const
{
    return CAlternateName::operator==(rIssuerAlternateName);
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rIssuerAlternateName:
//          Reference to the CIssuerAlternateName to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CIssuerAlternateName::operator!=(IN const CIssuerAlternateName& rIssuerAlternateName) const
{
    return CAlternateName::operator!=(rIssuerAlternateName);
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CISSUERALTERNATENAME)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_PKI_NONE)

#endif // MXG_CISSUERALTERNATENAME_H

