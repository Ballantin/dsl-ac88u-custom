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
#ifndef MXG_CKEYUSAGEOPENSSL_H
#define MXG_CKEYUSAGEOPENSSL_H

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
#ifndef MXD_PKI_CKEYUSAGE_CLASSNAME
    #define MXD_PKI_CKEYUSAGE_CLASSNAME CKeyUsageOpenSsl
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CKeyUsageOpenSsl
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the key usage extensions inside certificates.
//
// Description:
//   CKeyUsageOpenSsl is the class to handle the key usage extensions inside
//   certificates.
//
// Location:
//   Pki/CKeyUsageOpenSsl.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CKeyUsageOpenSsl : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CKeyUsageOpenSsl();

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsageOpenSsl(IN const CKeyUsageOpenSsl* pKeyUsage);

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsageOpenSsl(IN const CKeyUsageOpenSsl& rKeyUsage);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CKeyUsageOpenSsl();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CKeyUsageOpenSsl& operator=(IN const CKeyUsageOpenSsl& rKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CKeyUsageOpenSsl& rKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CKeyUsageOpenSsl& rKeyUsage) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Verifies if the certificate signing flag is set.
    //---------------------------------------------------
    mxt_result IsCertificateSigningSet() const;

    // Summary:
    //  Verifies if the CRL signed flag is set.
    //------------------------------------------
    mxt_result IsCrlSignedSet() const;

    // Summary:
    //  Verifies if the data encipherment flag is set.
    //-------------------------------------------------
    mxt_result IsDataEnciphermentSet() const;

    // Summary:
    //  Verifies if the decipher only flag is set.
    //---------------------------------------------
    mxt_result IsDecipherOnlySet() const;

    // Summary:
    //  Verifies if the digital signature flag is set.
    //-------------------------------------------------
    mxt_result IsDigitalSignatureSet() const;

    // Summary:
    //  Verifies if the encipher only flag is set.
    //---------------------------------------------
    mxt_result IsEncipherOnlySet() const;

    // Summary:
    //  Verifies if the key agreement flag is set.
    //---------------------------------------------
    mxt_result IsKeyAgreementSet() const;

    // Summary:
    //  Verifies if the key encipherment flag is set.
    //------------------------------------------------
    mxt_result IsKeyEnciphermentSet() const;

    // Summary:
    //  Verifies if the non repudiation flag is set.
    //-----------------------------------------------
    mxt_result IsNonRepudiationSet() const;

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
CKeyUsageOpenSsl::CKeyUsageOpenSsl()
: CCertificateExtension()
{
}


inline
CKeyUsageOpenSsl::CKeyUsageOpenSsl(IN const CKeyUsageOpenSsl* pKeyUsage)
: CCertificateExtension(pKeyUsage)
{
}

inline
CKeyUsageOpenSsl::CKeyUsageOpenSsl(IN const CKeyUsageOpenSsl& rKeyUsage)
: CCertificateExtension(&rKeyUsage)
{
}

inline
CKeyUsageOpenSsl::~CKeyUsageOpenSsl()
{
}

inline
CKeyUsageOpenSsl& CKeyUsageOpenSsl::operator=(IN const CKeyUsageOpenSsl& rKeyUsage)
{
    *static_cast<CCertificateExtension*>(this) = rKeyUsage;

    return *this;
}

inline
bool CKeyUsageOpenSsl::operator!=(IN const CKeyUsageOpenSsl& rKeyUsage) const
{
    return !(operator==(rKeyUsage));
}

inline
mxt_result CKeyUsageOpenSsl::IsCertificateSigningSet() const
{
    return GetFlag(KU_KEY_CERT_SIGN);
}

inline
mxt_result CKeyUsageOpenSsl::IsCrlSignedSet() const
{
    return GetFlag(KU_CRL_SIGN);
}

inline
mxt_result CKeyUsageOpenSsl::IsDataEnciphermentSet() const
{
    return GetFlag(KU_DATA_ENCIPHERMENT);
}

inline
mxt_result CKeyUsageOpenSsl::IsDecipherOnlySet() const
{
    return GetFlag(KU_DECIPHER_ONLY);
}

inline
mxt_result CKeyUsageOpenSsl::IsDigitalSignatureSet() const
{
    return GetFlag(KU_DIGITAL_SIGNATURE);
}

inline
mxt_result CKeyUsageOpenSsl::IsEncipherOnlySet() const
{
    return GetFlag(KU_ENCIPHER_ONLY);
}

inline
mxt_result CKeyUsageOpenSsl::IsKeyAgreementSet() const
{
    return GetFlag(KU_KEY_AGREEMENT);
}

inline
mxt_result CKeyUsageOpenSsl::IsKeyEnciphermentSet() const
{
    return GetFlag(KU_KEY_ENCIPHERMENT);
}

inline
mxt_result CKeyUsageOpenSsl::IsNonRepudiationSet() const
{
    return GetFlag(KU_NON_REPUDIATION);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

#endif // MXG_CKEYUSAGEOPENSSL_H
//M5T_INTERNAL_USE_END
