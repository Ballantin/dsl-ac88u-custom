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
#ifndef MXG_CALTERNATENAMEMOCANASS_H
#define MXG_CALTERNATENAMEMOCANASS_H

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
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif // #ifndef MXG_CSTRING_H

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CALTERNATENAME_CLASSNAME
    #define MXD_PKI_CALTERNATENAME_CLASSNAME CAlternateNameMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAlternateNameMocanaSs
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   CAlternateNameMocanaSs is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions.
//
// Description:
//   CAlternateNameMocanaSs is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions. It is a container of names that may be
//   of multiple types.
//
// Location:
//   Pki/CAlternateNameMocanaSs.h
//
//==============================================================================
class CAlternateNameMocanaSs : public CCertificateExtension
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
    CAlternateNameMocanaSs();

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateNameMocanaSs(IN const CAlternateNameMocanaSs* pAlternateName);

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateNameMocanaSs(IN const CAlternateNameMocanaSs& rAlternateName);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CAlternateNameMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CAlternateNameMocanaSs& operator=(IN const CAlternateNameMocanaSs& rAlternateName);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CAlternateNameMocanaSs& rAlternateName) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CAlternateNameMocanaSs& rAlternateName) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the number of name entries.
    //-----------------------------------
    mxt_result GetNameCount(OUT unsigned int* puNameCount) const;

    // Summary:
    //  Gets the type of a name entry.
    //---------------------------------
    mxt_result GetNameType(IN unsigned int uName, OUT ENameType* peNameType) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_DNS_NAME.
    //  An error is returned if there is a type mismatch.
    //---------------------------------------------------------
    mxt_result GetDnsName(IN unsigned int uName, OUT CString* pstrDnsName) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_IP_ADDRESS.
    //  An error is returned if there is a type mismatch.
    //----------------------------------------------------------
    mxt_result GetIpAddress(IN unsigned int uName, OUT CString* pstrIpAddress) const;

    // Summary:
    //  Gets the value of the name of type eNAME_TYPE_RFC_822_NAME (aka email).
    //  An error is returned if there is a type mismatch.
    //--------------------------------------------------------------------------
    mxt_result GetRfc822Name(IN unsigned int uName, OUT CString* pstrRfc822Name) const;

    // Summary:
    //  Gets the value of a name of type eNAME_TYPE_UNIFORM_RESOURCE_IDENTIFIER.
    //  An error is returned if there is a type mismatch.
    //--------------------------------------------------------------------------
    mxt_result GetUniformResourceIdentifier(IN unsigned int uName,
                                            OUT CString* pstrUniformResourceIdentifier) const;

// Hidden Methods
//----------------
protected:
    mxt_result GetNameValue(IN unsigned int uName,
                            OUT CString* pstrValue,
                            IN ENameType eNameType) const;

private:

// Hidden Data Members
//---------------------
protected:
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline CAlternateNameMocanaSs::CAlternateNameMocanaSs()
:   CCertificateExtension()
{
}

inline CAlternateNameMocanaSs::CAlternateNameMocanaSs(IN const CAlternateNameMocanaSs* pAlternateName)
:   CCertificateExtension(pAlternateName)
{
}

inline CAlternateNameMocanaSs::CAlternateNameMocanaSs(IN const CAlternateNameMocanaSs& rAlternateName)
:   CCertificateExtension(&rAlternateName)
{
}

inline CAlternateNameMocanaSs::~CAlternateNameMocanaSs()
{
}

inline CAlternateNameMocanaSs& CAlternateNameMocanaSs::operator=(IN const CAlternateNameMocanaSs& rAlternateName)
{
    *static_cast<CCertificateExtension*>(this) = rAlternateName;

    return *this;
}

inline bool CAlternateNameMocanaSs::operator!=(IN const CAlternateNameMocanaSs& rAlternateName) const
{
    return !(operator==(rAlternateName));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_MOCANA_SS)

#endif // MXG_CALTERNATENAMEMOCANASS_H
//M5T_INTERNAL_USE_END
