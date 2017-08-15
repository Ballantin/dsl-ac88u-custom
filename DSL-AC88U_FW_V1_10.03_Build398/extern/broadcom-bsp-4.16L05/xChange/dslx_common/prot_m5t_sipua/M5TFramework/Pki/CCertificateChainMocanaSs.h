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
#ifndef MXG_CCERTIFICATECHAINMOCANASS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCERTIFICATECHAINMOCANASS_H

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
    #define MXD_PKI_CCERTIFICATECHAIN_CLASSNAME CCertificateChainMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CCertificateChainMocanaSs
//========================================
//
// Summary:
//   Class that manages an ordered list of certificates.
//
// Description:
//   CCertificateChain implementation using Mocana SS.
//
// Location:
//   Pki/CCertificateChainMocanaSs.h
//
// See Also:
//  CCertificateChain
//
//==============================================================================
class CCertificateChainMocanaSs : public CCertificateChainBase
{

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //  Summary:
    //   Constructor.
    //----------------
    CCertificateChainMocanaSs();

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainMocanaSs(IN const CCertificateChainMocanaSs* pCertificateChain);

    //  Summary:
    //   Copy constructor.
    //---------------------
    CCertificateChainMocanaSs(IN const CCertificateChainMocanaSs& rCertificateChain);

    //  Summary:
    //   Destructor.
    //---------------------
    virtual ~CCertificateChainMocanaSs();

    //  Summary:
    //   Assignment operator.
    //------------------------
    CCertificateChainMocanaSs& operator=(IN const CCertificateChainMocanaSs& rCertificateChain);

    // Summary:
    //  Returns certificate in chain that corresponds to the provided distinguished name.
    //------------------------------------------------------------------------------------
    mxt_result GetCertificate(IN struct certDistinguishedName* pCertificateDistinguishedName,
                              OUT CCertificate* pCertificate);

//-- Hidden Methods
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_PKI_MOCANA_SS)

//M5T_INTERNAL_USE_END

#endif // MXG_CCERTIFICATECHAINMOCANASS_H

