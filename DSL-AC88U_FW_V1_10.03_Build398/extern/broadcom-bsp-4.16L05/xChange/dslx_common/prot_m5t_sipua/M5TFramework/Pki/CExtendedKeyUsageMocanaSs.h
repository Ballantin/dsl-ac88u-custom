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
#ifndef MXG_CEXTENDEDKEYUSAGEMOCANASS_H
#define MXG_CEXTENDEDKEYUSAGEMOCANASS_H

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
#ifndef MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME
    #define MXD_PKI_CEXTENDEDKEYUSAGE_CLASSNAME CExtendedKeyUsageMocanaSs
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CExtendedKeyUsageMocanaSs
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//    Class to handle the extended key usage extensions.
//
// Description:
//   CExtendedKeyUsageMocanaSs is the class to handle the extended key usage extensions
//   inside certificates.
//
// Location:
//   Pki/CExtendedKeyUsageMocanaSs.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CExtendedKeyUsageMocanaSs : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    static  uint8_t m_auEXTENDED_KEY_PURPOSE_SERVER_AUTHENTICATION_OID[]; // id-kp 1.
    static  uint8_t m_auEXTENDED_KEY_PURPOSE_CLIENT_AUTHENTICATION_OID[]; // id-kp 2.
    static  uint8_t m_auEXTENDED_KEY_PURPOSE_CODE_SIGNING_OID[];          // id-kp 3.
    static  uint8_t m_auEXTENDED_KEY_PURPOSE_EMAIL_PROTECTION_OID[];      // id-kp 4.
    static  uint8_t m_auEXTENDED_KEY_PURPOSE_TIME_STAMPING_OID[];         // id-kp 5.
    static  uint8_t m_auEXTENDED_KEY_PURPOSE_OCSP_SIGNING_OID[];          // id-kp 6.

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    // Constructor.
    //--------------
    CExtendedKeyUsageMocanaSs();

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsageMocanaSs(IN const CExtendedKeyUsageMocanaSs* pExtendedKeyUsage);

    // Summary:
    // Copy constructor.
    //-------------------
    CExtendedKeyUsageMocanaSs(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CExtendedKeyUsageMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CExtendedKeyUsageMocanaSs& operator=(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage) const;

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

    mxt_result GetFlag(IN const uint8_t* puKeyUsageOid) const;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
bool CExtendedKeyUsageMocanaSs::operator!=(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage) const
{
    return !(operator==(rExtendedKeyUsage));
}

inline
CExtendedKeyUsageMocanaSs::CExtendedKeyUsageMocanaSs()
: CCertificateExtension()
{
}

inline
CExtendedKeyUsageMocanaSs::CExtendedKeyUsageMocanaSs(IN const CExtendedKeyUsageMocanaSs* pExtendedKeyUsage)
: CCertificateExtension(pExtendedKeyUsage)
{
}

inline
CExtendedKeyUsageMocanaSs::CExtendedKeyUsageMocanaSs(IN const CExtendedKeyUsageMocanaSs& pExtendedKeyUsage)
: CCertificateExtension(&pExtendedKeyUsage)
{
}

inline
CExtendedKeyUsageMocanaSs::~CExtendedKeyUsageMocanaSs()
{
}

inline
CExtendedKeyUsageMocanaSs& CExtendedKeyUsageMocanaSs::operator=(IN const CExtendedKeyUsageMocanaSs& rExtendedKeyUsage)
{
    *static_cast<CCertificateExtension*>(this) = rExtendedKeyUsage;

    return *this;
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsClientAuthenticationSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_CLIENT_AUTHENTICATION_OID);
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsCodeSignedSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_CODE_SIGNING_OID);
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsOcspSignedSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_OCSP_SIGNING_OID);
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsServerAuthenticationSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_SERVER_AUTHENTICATION_OID);
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsSmimeSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_EMAIL_PROTECTION_OID);
}

inline
mxt_result CExtendedKeyUsageMocanaSs::IsTimestampingSet() const
{
    return GetFlag(m_auEXTENDED_KEY_PURPOSE_TIME_STAMPING_OID);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

#endif // MXG_CEXTENDEDKEYUSAGEMOCANASS_H
//M5T_INTERNAL_USE_END
