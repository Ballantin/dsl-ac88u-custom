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
#ifndef MXG_CEXTENDEDKEYUSAGEOPENSSL_H
#define MXG_CEXTENDEDKEYUSAGEOPENSSL_H

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
#ifndef MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME
    #define MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME CExtendedKeyUsageOpenSsl
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CExtendedKeyUsageOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//    Class to handle the extended key usage extensions.
//
// Description:
//   CExtendedKeyUsageOpenSsl is the class to handle the extended key usage extensions
//   inside certificates.
//
// Location:
//   Pki/CExtendedKeyUsageOpenSsl.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CExtendedKeyUsageOpenSsl : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CExtendedKeyUsageOpenSsl();

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsageOpenSsl(IN const CExtendedKeyUsageOpenSsl* pExtendedKeyUsage);

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsageOpenSsl(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CExtendedKeyUsageOpenSsl();

    // Summary:
    // Assignment operator.
    //----------------------
    CExtendedKeyUsageOpenSsl& operator=(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Verifies if the client authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsClientAuthenticationSet() const;

    // Summary:
    //  Verifies if the code signed flag is set.
    //-------------------------------------------
    mxt_result IsCodeSignedSet() const;

    // Summary:
    //  Verifies if the OSCP signed flag is set.
    //-------------------------------------------
    mxt_result IsOcspSignedSet() const;

    // Summary:
    //  Verifies if the server authentication flag is set.
    //-----------------------------------------------------
    mxt_result IsServerAuthenticationSet() const;

    // Summary:
    //  Verifies if the SMIME flag is set.
    //-------------------------------------
    mxt_result IsSmimeSet() const;

    // Summary:
    //  Verifies if the timestamping flag is set.
    //--------------------------------------------
    mxt_result IsTimestampingSet() const;

// Hidden Methods
//----------------
protected:
private:

    mxt_result GetFlag(IN unsigned int uValueToVerify) const;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


inline
bool CExtendedKeyUsageOpenSsl::operator!=(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage) const
{
    return !(operator==(rExtendedKeyUsage));
}

inline
CExtendedKeyUsageOpenSsl::CExtendedKeyUsageOpenSsl()
: CCertificateExtension()
{
}

inline
CExtendedKeyUsageOpenSsl::CExtendedKeyUsageOpenSsl(IN const CExtendedKeyUsageOpenSsl* pExtendedKeyUsage)
: CCertificateExtension(pExtendedKeyUsage)
{
}

inline
CExtendedKeyUsageOpenSsl::CExtendedKeyUsageOpenSsl(IN const CExtendedKeyUsageOpenSsl& pExtendedKeyUsage)
: CCertificateExtension(&pExtendedKeyUsage)
{
}

inline
CExtendedKeyUsageOpenSsl::~CExtendedKeyUsageOpenSsl()
{
}

inline
CExtendedKeyUsageOpenSsl& CExtendedKeyUsageOpenSsl::operator=(IN const CExtendedKeyUsageOpenSsl& rExtendedKeyUsage)
{
    *static_cast<CCertificateExtension*>(this) = rExtendedKeyUsage;

    return *this;
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsClientAuthenticationSet() const
{
    return GetFlag(XKU_SSL_CLIENT);
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsCodeSignedSet() const
{
    return GetFlag(XKU_CODE_SIGN);
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsOcspSignedSet() const
{
    return GetFlag(XKU_OCSP_SIGN);
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsServerAuthenticationSet() const
{
    return GetFlag(XKU_SSL_SERVER);
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsSmimeSet() const
{
    return GetFlag(XKU_SMIME);
}

inline
mxt_result CExtendedKeyUsageOpenSsl::IsTimestampingSet() const
{
    return GetFlag(XKU_TIMESTAMP);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CEXTENDEDKEYUSAGEOPENSSL_H
//M5T_INTERNAL_USE_END
