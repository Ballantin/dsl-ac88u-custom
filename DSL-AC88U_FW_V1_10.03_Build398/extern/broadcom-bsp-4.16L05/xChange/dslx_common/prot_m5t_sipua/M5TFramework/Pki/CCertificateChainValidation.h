//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CCERTIFICATECHAINVALIDATION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINVALIDATION_H
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
#if defined(MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE)
    #include MXD_PKI_CCERTIFICATECHAINVALIDATION_INCLUDE
#else
    #error "No implementation defined for CCertificateChainValidation!!!"
#endif

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
class CCertificateExtension;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CCERTIFICATECHAINVALIDATION)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CCERTIFICATECHAINVALIDATIONSTUB_H
    #include "PkiStubs/CCertificateChainValidationStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC========================================================================
//== Class: CCertificateChainValidation
//========================================
//<GROUP PKI_CLASSES>
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
//   Pki/CCertificateChainValidation.h
//
// See Also:
//  CCertificate, CCertificateChain, CTime
//
//==EDOC========================================================================
class CCertificateChainValidation : public MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CCertificateChainValidation();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CCertificateChainValidation();

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
private:

// Hidden Data Members
//---------------------
protected:

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


//==SDOC========================================================================
//==
//==    CCertificateChainValidation
//==
//==============================================================================
//
//  Description:
//      Default constructor.
//
//==EDOC========================================================================
inline
CCertificateChainValidation::CCertificateChainValidation()
:   MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME()
{

}

//==SDOC========================================================================
//==
//==    ~CCertificateChainValidation
//==
//==============================================================================
//
//  Description:
//      Basic destructor
//
//==EDOC========================================================================
inline
CCertificateChainValidation::~CCertificateChainValidation()
{

}

//==SDOC========================================================================
//==
//==    SetTrustedCertificates.
//==
//==============================================================================
//
//  Parameters:
//      pvecTrustedCertificates:
//          Pointer to the vector of trusted certificates.
//
//  Returns:
//      resS_OK if successful, mxt_result error code otherwise.
//
//  Description:
//      Sets the vector of trusted certificates. It is used if no trusted
//      certificate is provided to the Validate method.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChainValidation::SetTrustedCertificates(
                        IN const CVector<CCertificate>* pvecTrustedCertificates)
{
    return MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME::SetTrustedCertificates(pvecTrustedCertificates);
}

//==SDOC========================================================================
//==
//==    SetUntrustedCertificates
//==
//==============================================================================
//
//  Parameters:
//      pvecUntrustedCertificates:
//          Pointer to the vector of untrusted certificates.
//
//  Returns:
//      resS_OK if successful, mxt_result error code otherwise.
//
//  Description:
//      Sets the vector of untrusted certificates. It is used if no untrusted
//      certificate is provided to the Validate method.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChainValidation::SetUntrustedCertificates(
                      IN const CVector<CCertificate>* pvecUntrustedCertificates)
{
    return MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME::SetUntrustedCertificates(pvecUntrustedCertificates);
}

//==SDOC========================================================================
//==
//==    Validate
//==
//==============================================================================
//
//  Parameters:
//      pCertificateChain:
//          Pointer to the certificate chain.
//
//      pvecTrustedCertificates:
//          Pointer to the Trusted certificates. If NULL is passed, the internal
//          trusted certificates are used instead.
//
//      pvecUntrustedCertificates:
//          Pointer to the untrusted certificates. If NULL, the internal
//          untrusted certificates are used instead.
//
//      pTime:
//          Pointer to the time against which the Certificates Time validity
//          period is verified. If NULL is passed, the system time is used
//          instead.
//
//  Returns:
//      resSI_FALSE:
//          Certificate chain is invalid.
//
//      resSI_TRUE:
//          Certificate chain is valid.
//
//      resFE_INVALID_ARGUMENT:
//          Invalid argument.
//
//  Description:
//      This method performs the certificate chain's validation.
//
//==EDOC========================================================================
inline
mxt_result CCertificateChainValidation::Validate(
                      IN const CCertificateChain* pCertificateChain,
                      IN const CVector<CCertificate>* pvecTrustedCertificates,
                      IN const CVector<CCertificate>* pvecUntrustedCertificates,
                      IN const CTime* pTime)
{
    return MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME::Validate(pCertificateChain,
                                                                   pvecTrustedCertificates,
                                                                   pvecUntrustedCertificates,
                                                                   pTime);
}


#endif  // #if defined(MXD_ENABLE_STUBBING)
        // defined(MXD_STUB_CCERTIFICATECHAINVALIDATION)

MX_NAMESPACE_END(MXD_GNS)

#endif // !defined(MXD_PKI_NONE)

#endif // MXG_CCERTIFICATECHAINVALIDATION_H

