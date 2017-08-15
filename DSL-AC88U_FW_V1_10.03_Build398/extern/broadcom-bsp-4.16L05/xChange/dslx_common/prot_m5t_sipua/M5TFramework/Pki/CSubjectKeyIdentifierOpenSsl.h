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
#ifndef MXG_CSUBJECTKEYIDENTIFIEROPENSSL_H
#define MXG_CSUBJECTKEYIDENTIFIEROPENSSL_H

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

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CString;
class CBlob;
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME
    #define MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME CSubjectKeyIdentifierOpenSsl
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSubjectKeyIdentifierOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class used to handle the subject key identifier extensions.
//
// Description:
//   CSubjectKeyIdentifierOpenSsl is the class to handle the subject key 
//   identifier certificates extensions. These extensions contain information 
//   to identify a certificate that contains the public key used for this 
//   certificate.
//
// Location:
//   Pki/CSubjectKeyIdentifierOpenSsl.h
//
// See Also:
//   CCertificateExtension
//
//==EDOC========================================================================
class CSubjectKeyIdentifierOpenSsl : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSubjectKeyIdentifierOpenSsl();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectKeyIdentifierOpenSsl(IN const CSubjectKeyIdentifierOpenSsl* pSubjectKeyIdentifier);

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectKeyIdentifierOpenSsl(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSubjectKeyIdentifierOpenSsl();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSubjectKeyIdentifierOpenSsl& operator=(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the subject key identifier.
    //-----------------------------------
    mxt_result GetSubjectKeyIdentifier(OUT CBlob* pblobIdentifier) const;

// Hidden Methods
//----------------
protected:
private:

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


inline
CSubjectKeyIdentifierOpenSsl::CSubjectKeyIdentifierOpenSsl()
: CCertificateExtension()
{
}


inline
CSubjectKeyIdentifierOpenSsl::CSubjectKeyIdentifierOpenSsl(IN const CSubjectKeyIdentifierOpenSsl* pSubjectKeyIdentifier)
: CCertificateExtension(pSubjectKeyIdentifier)
{
}


inline
CSubjectKeyIdentifierOpenSsl::CSubjectKeyIdentifierOpenSsl(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier)
: CCertificateExtension(&rSubjectKeyIdentifier)
{
}

inline
CSubjectKeyIdentifierOpenSsl::~CSubjectKeyIdentifierOpenSsl()
{
}

inline
CSubjectKeyIdentifierOpenSsl& CSubjectKeyIdentifierOpenSsl::operator=(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier)
{
    *static_cast<CCertificateExtension*>(this) = rSubjectKeyIdentifier;

    return *this;
}

inline
bool CSubjectKeyIdentifierOpenSsl::operator!=(IN const CSubjectKeyIdentifierOpenSsl& rSubjectKeyIdentifier) const
{
    return !(operator==(rSubjectKeyIdentifier));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CSUBJECTKEYIDENTIFIEROPENSSL_H
//M5T_INTERNAL_USE_END
