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
#ifndef MXG_CCERTIFICATECHAINVALIDATIONMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINVALIDATIONMOCANASS_H

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
#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif
#ifndef MXG_CCERTIFICATE_H
#include "Pki/CCertificate.h"
#endif
#ifndef MXG_CPKI_H
#include "Pki/CPki.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATECHAINVALIDATIONBASE_H
#include "Pki/CCertificateChainValidationBase.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CCertificateChain;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME
    #define MXD_PKI_CCERTIFICATECHAINVALIDATION_CLASSNAME CCertificateChainValidationMocanaSs
#endif
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateChainValidationMocanaSs
//========================================
//
// Summary:
//   Performs certificate chain validation using Mocana SS.
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
//   Pki/CCertificateChainValidationMocanaSs.h
//
// See Also:
//  CCertificate, CCertificateChain, CTime, CCertificateChainValidation
//
//==EDOC========================================================================
class CCertificateChainValidationMocanaSs : public CCertificateChainValidationBase
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CCertificateChainValidationMocanaSs();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CCertificateChainValidationMocanaSs();

    // Summary:
    //  Performs the certificate chain's validation.
    //-----------------------------------------------
    mxt_result Validate(IN const CCertificateChain* pCertificateChain,
                        IN const CVector<CCertificate>* pvecTrustedCertificates,
                        IN const CVector<CCertificate>* pvecUntrustedCertificates,
                        IN const CTime* pTime);
};

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATECHAINVALIDATIONMOCANASS_H

