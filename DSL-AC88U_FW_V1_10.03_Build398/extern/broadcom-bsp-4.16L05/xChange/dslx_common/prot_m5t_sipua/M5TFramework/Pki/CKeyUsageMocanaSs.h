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
#ifndef MXG_CKEYUSAGEMOCANASS_H
#define MXG_CKEYUSAGEMOCANASS_H

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

//-- Parameters used by Value
//-----------------------------
extern "C" {
#ifndef __ABSSTREAM_H__
#include "common/absstream.h"
#endif

#ifndef __TREE_H__
#include "common/tree.h"
#endif

#ifndef __CA_MGMT_HEADER__
#include "crypto/ca_mgmt.h"
#endif

#ifndef __PARSEASN1_H__
#include "asn1/parseasn1.h"
#endif

#ifndef __PARSECERT_HEADER__
#include "asn1/parsecert.h"
#endif
}

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif // #ifndef MXG_CCERTIFICATEEXTENSION_H

#ifndef __PARSECERT_HEADER__
#include "parsecert.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CKEYUSAGE_CLASSNAME
    #define MXD_PKI_CKEYUSAGE_CLASSNAME CKeyUsageMocanaSs
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CKeyUsageMocanaSs
//========================================
//<GROUP PKI_CLASSES>
//
// Summary:
//   Class to handle the key usage extensions inside certificates.
//
// Description:
//   CKeyUsageMocanaSs is the class to handle the key usage extensions inside
//   certificates.
//
// Location:
//   Pki/CKeyUsageMocanaSs.h
//
// See Also:
//   CCertificateExtension
//
//==============================================================================
class CKeyUsageMocanaSs : public CCertificateExtension
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CKeyUsageMocanaSs();

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsageMocanaSs(IN const CKeyUsageMocanaSs* pKeyUsage);

    // Summary:
    //  Copy constructor.
    //--------------------
    CKeyUsageMocanaSs(IN const CKeyUsageMocanaSs& rKeyUsage);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CKeyUsageMocanaSs();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CKeyUsageMocanaSs& operator=(IN const CKeyUsageMocanaSs& rKeyUsage);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CKeyUsageMocanaSs& rKeyUsage) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CKeyUsageMocanaSs& rKeyUsage) const;

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
CKeyUsageMocanaSs::CKeyUsageMocanaSs()
: CCertificateExtension()
{
}

inline
CKeyUsageMocanaSs::CKeyUsageMocanaSs(IN const CKeyUsageMocanaSs* pKeyUsage)
: CCertificateExtension(pKeyUsage)
{
}

inline
CKeyUsageMocanaSs::CKeyUsageMocanaSs(IN const CKeyUsageMocanaSs& rKeyUsage)
: CCertificateExtension(&rKeyUsage)
{
}

inline
CKeyUsageMocanaSs::~CKeyUsageMocanaSs()
{
}

inline
CKeyUsageMocanaSs& CKeyUsageMocanaSs::operator=(IN const CKeyUsageMocanaSs& rKeyUsage)
{
    *static_cast<CCertificateExtension*>(this) = rKeyUsage;

    return *this;
}

inline
bool CKeyUsageMocanaSs::operator!=(IN const CKeyUsageMocanaSs& rKeyUsage) const
{
    return !(operator==(rKeyUsage));
}

inline
mxt_result CKeyUsageMocanaSs::IsCertificateSigningSet() const
{
    return GetFlag(keyCertSign);
}

inline
mxt_result CKeyUsageMocanaSs::IsCrlSignedSet() const
{
    return GetFlag(cRLSign);
}

inline
mxt_result CKeyUsageMocanaSs::IsDataEnciphermentSet() const
{
    return GetFlag(dataEncipherment);
}

inline
mxt_result CKeyUsageMocanaSs::IsDecipherOnlySet() const
{
    return GetFlag(decipherOnly);
}

inline
mxt_result CKeyUsageMocanaSs::IsDigitalSignatureSet() const
{
    return GetFlag(digitalSignature);
}

inline
mxt_result CKeyUsageMocanaSs::IsEncipherOnlySet() const
{
    return GetFlag(encipherOnly);
}

inline
mxt_result CKeyUsageMocanaSs::IsKeyAgreementSet() const
{
    return GetFlag(keyAgreement);
}

inline
mxt_result CKeyUsageMocanaSs::IsKeyEnciphermentSet() const
{
    return GetFlag(keyEncipherment);
}

inline
mxt_result CKeyUsageMocanaSs::IsNonRepudiationSet() const
{
    return GetFlag(nonRepudiation);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

#endif // MXG_CKEYUSAGEMOCANASS_H
//M5T_INTERNAL_USE_END
