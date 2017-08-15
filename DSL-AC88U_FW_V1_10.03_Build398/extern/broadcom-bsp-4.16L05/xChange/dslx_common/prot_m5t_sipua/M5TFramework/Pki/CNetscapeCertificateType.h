//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CNETSCAPECERTIFICATETYPE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CNETSCAPECERTIFICATETYPE_H
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
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

MX_NAMESPACE_START(MXD_GNS)

#if defined(MXD_PKI_OPENSSL)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CNETSCAPECERTIFICATETYPE)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CNETSCAPECERTIFICATETYPESTUB_H
    #include "PkiStubs/CNetscapeCertificateTypeStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CNetscapeCertificateType
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the Netscape certificates extensions certificates.
//
// Description:
//   CNetscapeCertificateType is the class to handle the Netscape certificates
//   extensions.
//
// Location:
//   Pki/CNetscapeCertificateType.h
//
// See Also:
//   CCertificateExtension
//
//==EDOC========================================================================
class CNetscapeCertificateType : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CNetscapeCertificateType();

    // Summary:
    //  Copy constructor.
    //--------------------
    CNetscapeCertificateType(IN const CNetscapeCertificateType* pNetscapeCertificateType);

    // Summary:
    //  Copy constructor.
    //--------------------
    CNetscapeCertificateType(IN const CNetscapeCertificateType& rNetscapeCertificateType);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CNetscapeCertificateType();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CNetscapeCertificateType& operator=(IN const CNetscapeCertificateType& rNetscapeCertificateType);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CNetscapeCertificateType& rNetscapeCertificateType) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CNetscapeCertificateType& rNetscapeCertificateType) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Verifies if the client authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsClientAuthenticationSet() const;

    // Summary:
    //  Verifies if the object signed flag is set.
    //---------------------------------------------
    mxt_result IsObjectSignedSet() const;

    // Summary:
    //  Verifies if the object signed certificate authority flag is set.
    //-------------------------------------------------------------------
    mxt_result IsObjectSignedCertificateAuthoritySet() const;

    // Summary:
    //  Verifies if the server authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsServerAuthenticationSet() const;

    // Summary:
    //  Verifies if the SMIME flag is set.
    //-------------------------------------
    mxt_result IsSmimeSet() const;

    // Summary:
    //  Verifies if the SMIME certificate authority flag is set.
    //-----------------------------------------------------------
    mxt_result IsSmimeCertificateAuthoritySet() const;

    // Summary:
    //  Verifies if the SSL certificate authority flag is set.
    //---------------------------------------------------------
    mxt_result IsSslCertificateAuthoritySet() const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:
private:

    mxt_result GetFlag(IN unsigned int uValueToVerify) const;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==    CNetscapeCertificateType
//==
//==============================================================================
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CNetscapeCertificateType::CNetscapeCertificateType()
: CCertificateExtension()
{
}

//==SDOC========================================================================
//==
//==    CNetscapeCertificateType
//==
//==============================================================================
//
//  Parameters:
//      pNetscapeCertificateType:
//          Pointer to the object to copy.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CNetscapeCertificateType::CNetscapeCertificateType(IN const CNetscapeCertificateType* pNetscapeCertificateType)
: CCertificateExtension(pNetscapeCertificateType)
{
}

//==SDOC========================================================================
//==
//==    CNetscapeCertificateType
//==
//==============================================================================
//
//  Parameters:
//      rNetscapeCertificateType:
//          Reference to the object to copy.
//
//  Description:
//      Constructor.
//
//==EDOC========================================================================
inline
CNetscapeCertificateType::CNetscapeCertificateType(IN const CNetscapeCertificateType& rNetscapeCertificateType)
: CCertificateExtension(&rNetscapeCertificateType)
{
}

//==SDOC========================================================================
//==
//==    ~CNetscapeCertificateType
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CNetscapeCertificateType::~CNetscapeCertificateType()
{
}

//==SDOC========================================================================
//==
//==    operator=
//==
//==============================================================================
//
//  Parameters:
//      rNetscapeCertificateType:
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
CNetscapeCertificateType& CNetscapeCertificateType::operator=(IN const CNetscapeCertificateType& rNetscapeCertificateType)
{
    *static_cast<CCertificateExtension*>(this) = rNetscapeCertificateType;

    return *this;
}

//==SDOC========================================================================
//==
//==    operator!=
//==
//==============================================================================
//
//  Parameters:
//      rNetscapeCertificateType:
//          Reference to the CNetscapeCertificateType to compare.
//
//  Returns:
//      True if both objects are different, false otherwise.
//
//  Description:
//      Verifies if both objects are different.
//
//==EDOC========================================================================
inline
bool CNetscapeCertificateType::operator!=(IN const CNetscapeCertificateType& rNetscapeCertificateType) const
{
    return !(operator==(rNetscapeCertificateType));
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsClientAuthenticationSet() const
{
    return GetFlag(NS_SSL_CLIENT);
}

//==SDOC========================================================================
//==
//==    IsObjectSignedSet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the object signed flag is set.
//
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsObjectSignedSet() const
{
    return GetFlag(NS_OBJSIGN);
}

//==SDOC========================================================================
//==
//==    IsObjectSignedCertificateAuthoritySet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the object signed certificate authority flag is set.
//
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsObjectSignedCertificateAuthoritySet() const
{
    return GetFlag(NS_OBJSIGN_CA);
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsServerAuthenticationSet() const
{
    return GetFlag(NS_SSL_SERVER);
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsSmimeSet() const
{
    return GetFlag(NS_SMIME);
}

//==SDOC========================================================================
//==
//==    IsSmimeCertificateAuthoritySet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the SMIME certificate authority flag is set.
//
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsSmimeCertificateAuthoritySet() const
{
    return GetFlag(NS_SMIME_CA);
}

//==SDOC========================================================================
//==
//==    IsSslCertificateAuthoritySet
//==
//==============================================================================
//
//  Returns:
//      - resFE_FAIL
//      - resSI_FALSE
//      - resSI_TRUE
//
//  Description:
//      Verifies if the SSL certificate authority flag is set.
//
//==EDOC========================================================================
inline
mxt_result CNetscapeCertificateType::IsSslCertificateAuthoritySet() const
{
    return GetFlag(NS_SSL_CA);
}

#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CNETSCAPECERTIFICATETYPE)

#else
//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//== Class: CNetscapeCertificateType
//========================================
//
// Summary:
//   Class to handle the Netscape certificates extensions certificates.
//
// Description:
//   CNetscapeCertificateType is the class to handle the Netscape certificates
//   extensions.
//
// Location:
//   Pki/CNetscapeCertificateType.h
//
// See Also:
//   CCertificateExtension
//
//==EDOC========================================================================
class CNetscapeCertificateType : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CNetscapeCertificateType() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CNetscapeCertificateType() {}

    // << Accessors >>
    //-----------------

// Hidden Methods
//----------------
protected:
private:

};
//M5T_INTERNAL_USE_END
#endif // defined(MXD_PKI_OPENSSL)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CNETSCAPECERTIFICATETYPE_H

