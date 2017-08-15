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
#ifndef MXG_CSUBJECTALTERNATENAME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSUBJECTALTERNATENAME_H
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
    defined(MXD_STUB_CSUBJECTALTERNATENAME)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CSUBJECTALTERNATENAMESTUB_H
    #include "PkiStubs/CSubjectAlternateNameStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CSubjectAlternateName
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class that manages the subjectAlternateName extension.
//
// Description:
//   CSubjectAlternateName is the class that manages the subjectAlternateName
//   extension.
//
// Location:
//   Pki/CSubjectAlternateName.h
//
//==EDOC========================================================================
class CSubjectAlternateName : public CAlternateName
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
    CSubjectAlternateName();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectAlternateName(IN const CSubjectAlternateName* pSubjectAlternateName);

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectAlternateName(IN const CSubjectAlternateName& rSubjectAlternateName);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSubjectAlternateName();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSubjectAlternateName& operator=(IN const CSubjectAlternateName& rSubjectAlternateName);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CSubjectAlternateName& rSubjectAlternateName) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CSubjectAlternateName& rSubjectAlternateName) const;

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
//==    CSubjectAlternateName
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CSubjectAlternateName::CSubjectAlternateName()
: CAlternateName()
{
}

//==SDOC========================================================================
//==
//==    CSubjectAlternateName
//==
//==============================================================================
//
//  Parameters:
//      pSubjectAlternateName:
//          Pointer to the CSubjectAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CSubjectAlternateName::CSubjectAlternateName(IN const CSubjectAlternateName* pSubjectAlternateName)
: CAlternateName(pSubjectAlternateName)
{
}

//==SDOC========================================================================
//==
//==    CSubjectAlternateName
//==
//==============================================================================
//
//  Parameters:
//      rSubjectAlternateName:
//          Reference to the CSubjectAlternateName to copy.
//
//  Description:
//      Copy constructor.
//
//==EDOC========================================================================
inline
CSubjectAlternateName::CSubjectAlternateName(IN const CSubjectAlternateName& rSubjectAlternateName)
: CAlternateName(&rSubjectAlternateName)
{
}

//==SDOC========================================================================
//==
//==    ~CSubjectAlternateName
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CSubjectAlternateName::~CSubjectAlternateName()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rSubjectAlternateName:
//          Reference to the CSubjectAlternateName to assign.
//
//  Returns:
//      A reference to the assigned object.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==EDOC========================================================================
inline
CSubjectAlternateName& CSubjectAlternateName::operator=(IN const CSubjectAlternateName& rSubjectAlternateName)
{
    *static_cast<CAlternateName*>(this) = rSubjectAlternateName;

    return *this;
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rSubjectAlternateName:
//          Reference to the CSubjectAlternateName to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CSubjectAlternateName::operator==(IN const CSubjectAlternateName& rSubjectAlternateName) const
{
    return CAlternateName::operator==(rSubjectAlternateName);
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSubjectAlternateName:
//          Reference to the CSubjectAlternateName to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CSubjectAlternateName::operator!=(IN const CSubjectAlternateName& rSubjectAlternateName) const
{
    return CAlternateName::operator!=(rSubjectAlternateName);
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CSUBJECTALTERNATENAME)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_PKI_NONE)

#endif // MXG_CSUBJECTALTERNATENAME_H

