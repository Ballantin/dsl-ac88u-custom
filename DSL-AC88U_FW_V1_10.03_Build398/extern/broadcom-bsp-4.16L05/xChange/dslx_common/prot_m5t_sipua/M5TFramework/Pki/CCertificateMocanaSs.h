//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CCERTIFICATEMOCANASS_H
#define MXG_CCERTIFICATEMOCANASS_H

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
    #include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_PKI_MOCANA_SS)

//-- Data Members
//-----------------
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif

#ifndef MXG_CCERTIFICATEDESCRIPTORMOCANASS_H
#include "Pki/CCertificateDescriptorMocanaSs.h"
#endif

#ifndef MXG_CTIME_H
#include "Time/CTime.h"
#endif

#ifndef MXG_CCERTIFICATEISSUER_H
#include "Pki/CCertificateIssuer.h"
#endif

#ifndef MXG_CCERTIFICATESUBJECT_H
#include "Pki/CCertificateSubject.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CPUBLICKEY_H
#include "Crypto/CPublicKey.h"
#endif
#ifndef MXG_CCERTIFICATEEXTENSION_H
#include "Pki/CCertificateExtension.h"
#endif

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

extern "C" {
#ifndef __CA_MGMT_HEADER__
#include "crypto/ca_mgmt.h"
#endif

#ifndef __ABSSTREAM_H__
#include "common/absstream.h"
#endif

#ifndef __MEMFILE_H__
#include "common/memfile.h"
#endif

#ifndef __TREE_H__
#include "common/tree.h"
#endif

#ifndef __PARSEASN1_H__
#include "asn1/parseasn1.h"
#endif
}

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATEBASE_H
#include "Pki/CCertificateBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CCertificateIssuer;
class CCertificateSubject;
class CMarshaler;
class CPublicKey;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATE_CLASSNAME
    #define MXD_PKI_CCERTIFICATE_CLASSNAME CCertificateMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateMocanaSs
//========================================
//
// Summary:
//   Class that manages and abstracts an X.509 certificate using mocana ss.
//
// Description:
//   CCertificateMocanaSs is the class that manages and abstracts an X.509
//   certificate.
//
//   An X.509 certificate is used to certify the identity of an entity.
//
// Location:
//   Pki/CCertificateMocanaSs.h
//
// See Also:
//  CCertificate
//
//==EDOC========================================================================
class CCertificateMocanaSs : public CCertificateBase
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
    CCertificateMocanaSs();

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateMocanaSs(IN const CCertificateMocanaSs* pCertificate);

    // Summary:
    // Copy constructor.
    //--------------
    CCertificateMocanaSs(IN const CCertificateMocanaSs& rCertificate);

    // Summary:
    // Destructor.
    //--------------
    virtual ~CCertificateMocanaSs();

    // Summary:
    // Assignment operator.
    //----------------------
    CCertificateMocanaSs& operator=(IN const CCertificateMocanaSs& rCertificate);

    // Summary:
    //  Reads a DER-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestoreDer(IN const CBlob* pX509);

    // Summary:
    //  Reads a PEM-encoded certificate from a blob.
    //-----------------------------------------------
    virtual mxt_result RestorePem(IN const CBlob* pX509);

    // Summary:
    //  Associates a private key with this X.509 certificate.
    //--------------------------------------------------------
    mxt_result SetPrivateKey(IN const CPrivateKey* pPrivateKey);

    // Summary:
    //  Stores the content of this certificate in DER format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StoreDer(OUT CBlob* pX509) const;

    // Summary:
    //  Stores the content of this certificate in PEM format into a blob.
    //--------------------------------------------------------------------
    virtual mxt_result StorePem(OUT CBlob* pX509) const;

    // << Accessors >>
    //-----------------

    // Summary:
    //  Gets the certificate version.
    //--------------------------------
    virtual mxt_result GetVersion(OUT CCertificateMocanaSs::EVersion* peVersion) const;
    // Summary:
    //  Gets the certificate serial number.
    //--------------------------------------
    virtual mxt_result GetSerialNumber(OUT CBlob* pSerialNumber) const;
    // Summary:
    //  Gets the time after which the certificate is considered as invalid.
    //----------------------------------------------------------------------
    virtual mxt_result GetNotAfterTime(OUT CTime* pNotAfter) const;
    // Summary:
    //  Gets the time before which the certificate is considered as invalid.
    //-----------------------------------------------------------------------
    virtual mxt_result GetNotBeforeTime(OUT CTime* pNotBefore) const;

    // Summary:
    //  Gets the certificate issuer.
    //-------------------------------
    virtual mxt_result GetIssuer(OUT CCertificateIssuer* pIssuer) const;

    // Summary:
    //  Gets the certificate subject.
    //--------------------------------
    virtual mxt_result GetSubject(OUT CCertificateSubject* pSubject) const;

    // Summary:
    //  Gets the public key associated with this certificate.
    //--------------------------------------------------------
    virtual mxt_result GetPublicKey(OUT CPublicKey* pPublicKey) const;

    // Summary:
    //  Gets the signature and signature algorithm for this certificate.
    //-------------------------------------------------------------------
    virtual mxt_result GetSignature(OUT CPublicKey::EAlgorithm* peSignatureAlgorithm,
                                    OUT CBlob* pSignature) const;

    // Only V3.

    // Summary:
    //  Gets the certificate extension information by index.
    //-------------------------------------------------------
    virtual mxt_result GetExtension(IN unsigned int uExtension,
                                    OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the certificate extension corresponding to a specified type.
    //--------------------------------------------------------------------
    virtual mxt_result GetExtensionByType(IN CCertificateExtension::EType eType,
                                          OUT CCertificateExtension* pExtension) const;

    // Summary:
    //  Gets the number of extensions added to this certificate.
    //-----------------------------------------------------------
    virtual mxt_result GetExtensionCount(OUT unsigned int* puExtensionCount) const;

    // << Helpers >>
    //---------------
    mxt_result GetCertificateDescriptor(CCertificateDescriptorMocanaSs** ppCertDescriptor) const;

    // Summary:
    //  Returns a copy of the certifcate's internal structure.
    mxt_result GetCertificateDescriptor(INOUT TOA certDescriptor* pstCertDescriptor) const;

    // Summary:
    //  Verifies whether a certificate is self-issued.
    //-------------------------------------------------
    mxt_result IsSelfIssued() const;

    // Summary:
    //  Verifies whether a certificate has been issued by another one.
    //-----------------------------------------------------------------
    mxt_result IsIssuedBy(IN const CCertificateMocanaSs* pCertificate) const;

    // Summary:
    //  Validates if the certificate has been properly signed.
    //---------------------------------------------------------
    virtual mxt_result VerifySignature(IN const CPublicKey* pIssuerPublicKey) const;

// Hidden Methods
//----------------
private:

    // Summary:
    //  Parse a certificate having a DER format and return the base of the resulting ASN1 tree.
    //-----------------------------------------------------------------------------------------
    mxt_result ParseCertificateDescriptor(certDescriptor*, MemFile* mf, CStream* cs, ASN1_ITEM** pAsn1Item) const;

    // Summary:
    //  Find the ASN1 item representing the beginning of the certificate extensions.
    //-------------------------------------------------------------------------------
    mxt_result FindExtensionBeginning(IN ASN1_ITEM* pRootItem, OUT ASN1_ITEM** ppExtensionBeginning) const;

    // Summary:
    //  Find the ASN1 item representing the beginning of the certificate.
    //--------------------------------------------------------------------
    mxt_result FindCertificateBeginning(IN ASN1_ITEM* rootItem, OUT ASN1_ITEM** ppCertificate) const;

// Hidden Data Members
//---------------------
protected:

    // Holder of the related certDescriptor.
    CCertificateDescriptorMocanaSs* m_pCertDescriptorCounted;
    EVersion m_versionParsed;
    CBlob m_SerialVersionParsed;
    CTime m_NotAfterTimeParsed;
    CTime m_NotBeforeTimeParsed;
    bool m_bSelfIssued;
    bool m_bInitialized;
    CCertificateIssuer m_Issuer;
    CCertificateSubject m_Subject;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
mxt_result CCertificateMocanaSs::IsSelfIssued() const
{
    mxt_result res = resS_OK;
    if (m_bInitialized == true)
    {
        if(m_bSelfIssued == true)
        {
            res = resSI_TRUE;
        }
        else
        {
            res = resSI_FALSE;
        }
    }
    else
    {
        res = IsIssuedBy(this);
    }
    return res;
}

inline
mxt_result CCertificateMocanaSs::GetCertificateDescriptor(CCertificateDescriptorMocanaSs** ppCertDescriptor) const
{
    *ppCertDescriptor = m_pCertDescriptorCounted;

    return resS_OK;
}

//==============================================================================
//==
//==  GetCertificateDescriptor
//==
//==============================================================================
//
//  Summary:
//      Returns a copy of the certifcate's internal structure.
//
//  Parameters:
//      ppstCertDescriptor:
//       Description of the parameter.
//
//  Returns:
//      resS_OK: the copy has been made.
//
//      ResFE_INVALID_ARGUMENT: either ppstCertDescriptor is NULL or its value
//      is not.
//
//  Description:
//      Returns a copy of the certifcate's internal structure.
//
//==============================================================================
inline
mxt_result CCertificateMocanaSs::GetCertificateDescriptor(INOUT TOA certDescriptor* pstCertDescriptor) const
{
    return m_pCertDescriptorCounted->Copy(pstCertDescriptor);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_PKI_MOCANA_SS)

#endif // MXG_CCERTIFICATEMOCANASS_H
//M5T_INTERNAL_USE_END

