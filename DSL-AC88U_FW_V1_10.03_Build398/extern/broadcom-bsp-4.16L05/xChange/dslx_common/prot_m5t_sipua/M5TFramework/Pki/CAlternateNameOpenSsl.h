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
#ifndef MXG_CALTERNATENAMEOPENSSL_H
#define MXG_CALTERNATENAMEOPENSSL_H

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
    #define MXD_PKI_CALTERNATENAME_CLASSNAME CAlternateNameOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CAlternateNameOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   CAlternateNameOpenSsl is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions.
//
// Description:
//   CAlternateNameOpenSsl is the base class for the CIssuerAlternateName and
//   CSubjectAlternateName extensions. It is a container of names that may be
//   of multiple types.
//
// Location:
//   Pki/CAlternateNameOpenSsl.h
//
// See Also:
//   CIssuerAlternateName, CSubjectAlternateName, CCertificateExtension
//
//==============================================================================
class CAlternateNameOpenSsl : public CCertificateExtension
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
    CAlternateNameOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateNameOpenSsl(IN const CAlternateNameOpenSsl* pAlternateName);

    // Summary:
    // Copy constructor.
    //-------------------
    CAlternateNameOpenSsl(IN const CAlternateNameOpenSsl& rAlternateName);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CAlternateNameOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CAlternateNameOpenSsl& operator=(IN const CAlternateNameOpenSsl& rAlternateName);

    // Summary:
    // Comparison operator.
    //----------------------
    bool operator==(IN const CAlternateNameOpenSsl& rAlternateName) const;

    // Summary:
    // Different than operator.
    //--------------------------
    bool operator!=(IN const CAlternateNameOpenSsl& rAlternateName) const;

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

    mxt_result GetStackOfName(OUT STACK_OF(CONF_VALUE)** ppStackOfName) const;
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

inline CAlternateNameOpenSsl::CAlternateNameOpenSsl()
:   CCertificateExtension()
{
}

inline CAlternateNameOpenSsl::CAlternateNameOpenSsl(IN const CAlternateNameOpenSsl* pAlternateName)
:   CCertificateExtension(pAlternateName)
{
}

inline CAlternateNameOpenSsl::CAlternateNameOpenSsl(IN const CAlternateNameOpenSsl& rAlternateName)
:   CCertificateExtension(&rAlternateName)
{
}

inline CAlternateNameOpenSsl::~CAlternateNameOpenSsl()
{
}

inline CAlternateNameOpenSsl& CAlternateNameOpenSsl::operator=(IN const CAlternateNameOpenSsl& rAlternateName)
{
    *static_cast<CCertificateExtension*>(this) = rAlternateName;

    return *this;
}

inline bool CAlternateNameOpenSsl::operator!=(IN const CAlternateNameOpenSsl& rAlternateName) const
{
    return !(operator==(rAlternateName));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_OPENSSL)

#endif // MXG_CALTERNATENAMEOPENSSL_H
//M5T_INTERNAL_USE_END
