//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
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
#ifndef MXG_CSUBJECTKEYIDENTIFIER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSUBJECTKEYIDENTIFIER_H
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

//-- Interface Realized and/or Parent
//-------------------------------------
#if defined(MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE)
    #include MXD_PKI_CSUBJECTKEYIDENTIFIER_INCLUDE
#else
    #error "No implementation defined for CSubjectKeyIdentifier!!!"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CString;
class CBlob;
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CSUBJECTKEYIDENTIFIER)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CSUBJECTKEYIDENTIFIERSTUB_H
    #include "PkiStubs/CSubjectKeyIdentifierStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CSubjectKeyIdentifier
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class used to handle the subject key identifier extensions.
//
// Description:
//   CSubjectKeyIdentifier is the class to handle the subject key identifier
//   certificates extensions. These extensions contain information to identify a
//   certificate that contains the public key used for this certificate.
//
// Location:
//   Pki/CSubjectKeyIdentifier.h
//
// See Also:
//   CCertificateExtension
//
//==EDOC========================================================================
class CSubjectKeyIdentifier : public MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSubjectKeyIdentifier();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectKeyIdentifier(IN const CSubjectKeyIdentifier* pSubjectKeyIdentifier);

    // Summary:
    //  Copy constructor.
    //--------------------
    CSubjectKeyIdentifier(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSubjectKeyIdentifier();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSubjectKeyIdentifier& operator=(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier) const;

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

//==SDOC========================================================================
//==
//==    CSubjectKeyIdentifier
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CSubjectKeyIdentifier::CSubjectKeyIdentifier()
: MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME()
{
}

//==SDOC========================================================================
//==
//==    CSubjectKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      pSubjectKeyIdentifier:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CSubjectKeyIdentifier::CSubjectKeyIdentifier(IN const CSubjectKeyIdentifier* pSubjectKeyIdentifier)
: MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME(pSubjectKeyIdentifier)
{
}

//==SDOC========================================================================
//==
//==    CSubjectKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      rSubjectKeyIdentifier:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CSubjectKeyIdentifier::CSubjectKeyIdentifier(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier)
: MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME(&rSubjectKeyIdentifier)
{
}

//==SDOC========================================================================
//==
//==    ~CSubjectKeyIdentifier
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CSubjectKeyIdentifier::~CSubjectKeyIdentifier()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rSubjectKeyIdentifier:
//          Reference to the object to assign.
//
//  Returns:
//      A reference to the assigned object.
//
//  Description:
//      Assigns the right hand object to the left hand one.
//
//==EDOC========================================================================
inline
CSubjectKeyIdentifier& CSubjectKeyIdentifier::operator=(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier)
{
    MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME::operator=(rSubjectKeyIdentifier);
    return *this;
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSubjectKeyIdentifier:
//          Reference to the CSubjectKeyIdentifier to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CSubjectKeyIdentifier::operator!=(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier) const
{
    return MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME::operator!=(rSubjectKeyIdentifier);
}

//==SDOC========================================================================
//==
//==    operator==
//==
//==============================================================================
//
//  Parameters:
//      rSubjectKeyIdentifier:
//          Reference to the CSubjectKeyIdentifier to compare.
//
//  Returns:
//      True if both objects are equal, false otherwise.
//
//  Description:
//      Verifies if both objects are equal.
//
//==EDOC========================================================================
inline
bool CSubjectKeyIdentifier::operator==(IN const CSubjectKeyIdentifier& rSubjectKeyIdentifier) const
{
    return MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME::operator==(rSubjectKeyIdentifier);
}


//==SDOC========================================================================
//==
//==    GetSubjectKeyIdentifier
//==
//==============================================================================
//
//  Parameters:
//      pblobIdentifier:
//          Pointer to the CBlob object to contain the return value.
//
//  Returns:
//      - resS_OK
//      - resFE_INVALID_ARGUMENT
//
//  Description:
//      Gets the Subject Key Identifier from the extension.
//
//==EDOC========================================================================
inline
mxt_result CSubjectKeyIdentifier::GetSubjectKeyIdentifier(OUT CBlob* pblobIdentifier) const
{
    return MXD_PKI_CSUBJECTKEYIDENTIFIER_CLASSNAME::GetSubjectKeyIdentifier(OUT pblobIdentifier);
}
#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CSubjectKeyIdentifier)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CSUBJECTKEYIDENTIFIER_H

