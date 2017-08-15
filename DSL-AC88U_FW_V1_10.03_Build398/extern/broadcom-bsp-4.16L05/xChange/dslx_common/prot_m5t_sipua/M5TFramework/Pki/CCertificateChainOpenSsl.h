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
#ifndef MXG_CCERTIFICATECHAINOPENSSL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINOPENSSL_H

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
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCERTIFICATECHAINBASE_H
#include "Pki/CCertificateChainBase.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_PKI_CCERTIFICATECHAIN_CLASSNAME
    #define MXD_PKI_CCERTIFICATECHAIN_CLASSNAME CCertificateChainOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCertificateChainOpenSsl
//========================================
//
// Summary:
//   Class that manages an ordered list of certificates.
//
// Description:
//   CCertificateChain implementation using Open SSL.
//
// Location:
//   Pki/CCertificateChainOpenSsl.h
//
// See Also:
//  CCertificateChain
//
//==EDOC========================================================================
class CCertificateChainOpenSsl : public CCertificateChainBase
{
    friend class CAsyncTlsSocket;

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //  Summary:
    //   Constructor.
    //----------------
    CCertificateChainOpenSsl();

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainOpenSsl(IN const CCertificateChainOpenSsl* pCertificateChain);

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainOpenSsl(IN const CCertificateChainOpenSsl& rCertificateChain);

    //  Summary:
    //   Destructor.
    //---------------------
    virtual ~CCertificateChainOpenSsl();

    //  Summary:
    //   Assignment operator.
    //------------------------
    CCertificateChainOpenSsl& operator=(IN const CCertificateChainOpenSsl& rCertificateChain);

// Hidden Methods
//----------------
protected:
    mxt_result Set(IN const STACK_OF(X509)* pClientX509Chain);
    mxt_result Set(IN const STACK_OF(X509)* pServerX509Chain, IN X509* pServerX509);
    
    mxt_result OrderCertificateChain(IN const STACK_OF(X509)* pClientX509Chain,
                                     OUT CVector<CCertificate>& rvecCertifiates);

    //  Summary:
    //   Finds a certificate's issuer in the provided list.
    unsigned int FindIssuerCertificate(IN const CCertificate& rCertificateIssued,
                                       IN const CVector<CCertificate>& rvecCertifiates);
    //  Summary:
    //   Finds a certificate issued by the provided certificate authority.
    unsigned int FindCertificateIssuedBy(IN const CCertificate& rCertificateIssuer,
                                         IN const CVector<CCertificate>& rvecCertifiates);

private:

// Hidden Data Members
//---------------------
protected:

};

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_OPENSSL)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATECHAINOPENSSL_H

