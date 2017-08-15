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
#ifndef MXG_CCERTIFICATECHAINVALIDATIONBASE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINVALIDATIONBASE_H

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
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
#ifndef MXG_CCERTIFICATE_H
#include "Pki/CCertificate.h"
#endif
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CCertificateChain;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateChainValidationBase
//========================================
//
// Summary:
//   Performs certificate chain validation.
//
// Description:
//   CCertificateChainValidation is the class that validates a chain of
//   certificates.
//
//   The validation process checks the integrity and authenticity of all
//   certificates within a certificate chain against a vector of trusted and
//   untrusted certificates. The process may take a long time, depending on the
//   length of the chain, the algorithms used to sign the certificates, and the
//   length of their keys.
//
// Location:
//   Pki/CCertificateChainValidationBase.h
//
// See Also:
//  CCertificate, CCertificateChain, CTime, CCertificateChainValidation
//
//==EDOC========================================================================
class CCertificateChainValidationBase
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CCertificateChainValidationBase();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CCertificateChainValidationBase();

    // Summary:
    //  Sets the trusted certificates.
    //---------------------------------
    mxt_result SetTrustedCertificates(IN const CVector<CCertificate>* pvecTrustedCertificates);

    // Summary:
    //  Sets the untrusted certificates.
    //-----------------------------------
    mxt_result SetUntrustedCertificates(IN const CVector<CCertificate>* pvecUntrustedCertificates);

    // Summary:
    //  Performs the certificate chain's validation.
    //-----------------------------------------------
    mxt_result Validate(IN const CCertificateChain* pCertificateChain,
                        IN const CVector<CCertificate>* pvecTrustedCertificates,
                        IN const CVector<CCertificate>* pvecUntrustedCertificates,
                        IN const CTime* pTime);

// Hidden Methods
//----------------
protected:
    // Summary:
    //  Finds the issuer certificate within a list of certificates.
    //--------------------------------------------------------------
    mxt_result FindIssuerCertificateInVector(IN const CVector<CCertificate>* pvecCertificates,
                                             IN const CCertificate* pSubjectCertificate,
                                             OUT CCertificate* pIssuerCertificate) const;

    // Summary:
    //  Checks whether or not a certificate is within a list of certificates.
    //------------------------------------------------------------------------
    mxt_result IsCertificateInVector(IN const CVector<CCertificate>* pvecCertificates,
                                     IN const CCertificate* pCertificate) const;

    // Summary:
    //  Checks whether or not the certificate Time period is consistent with
    //  the requested time.
    //-----------------------------------------------------------------------
    mxt_result IsCertificateTimeValid(IN const CTime* pTime,
                                      IN const CCertificate* pCertificate) const;

    // Summary:
    //  Locks the CCrypto Mutex.
    //---------------------------
    void Lock() const;

    // Summary:
    //  Unlocks the CCrypto Mutex.
    //-----------------------------
    void Unlock() const;
private:

// Hidden Data Members
//---------------------
protected:

    // Vector of trusted certificates.
    CVector<CCertificate> m_vecTrustedCertificates;
    // Vector of untrusted certificates.
    CVector<CCertificate> m_vecUntrustedCertificates;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
void CCertificateChainValidationBase::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CCertificateChainValidationBase::Unlock() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATECHAINVALIDATIONBASE_H

