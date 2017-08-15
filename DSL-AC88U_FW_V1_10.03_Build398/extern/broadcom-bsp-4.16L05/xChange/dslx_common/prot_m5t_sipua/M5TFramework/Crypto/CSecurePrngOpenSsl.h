//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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

#ifndef MXG_CSECUREPRNGOPENSSL_H
#define MXG_CSECUREPRNGOPENSSL_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_CRYPTO_SECUREPRNG_OPENSSL)

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
#if defined(MXD_CAP_ENABLE_SUPPORT)
class CBlob;
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_SECUREPRNG_CLASSNAME
    #define MXD_CRYPTO_SECUREPRNG_CLASSNAME CSecurePrngOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSecurePrngOpenSsl
//==============================================================================
//<GROUP Crypto>
//
// Summary:
//   This is a secure PRNG implementation using the OpenSSL engine.
//
// Description:
//   This is a secure PRNG implementation using the OpenSSL engine.
//
// Location:
//   Crypto/CSecurePrngOpenSsl.h
//
//==============================================================================
class CSecurePrngOpenSsl
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSecurePrngOpenSsl();

    // Summary:
    //  Destructor.
    virtual ~CSecurePrngOpenSsl();

    //-- << CSecurePrng >>
    static mxt_result Generate(IN unsigned int uNumberOfBytes, OUT uint8_t* puRandomBytes);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    static mxt_result Generate(IN unsigned int uNumberOfBytes, OUT CBlob* pblobRandomBytes);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    static mxt_result SetSeed(IN unsigned int uSeedSize, IN uint8_t* puSeedData);

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSecurePrngOpenSsl(IN const CSecurePrngOpenSsl& rSrc);

    // Summary:
    //  Assignment Operator.
    CSecurePrngOpenSsl& operator=(IN const CSecurePrngOpenSsl& rSrc);

    static mxt_result InitializeCSecurePrngOpenSsl();
    static void FinalizeCSecurePrngOpenSsl();

    //-- << Friends declarations >>
    friend mxt_result InitializeCSecurePrngOpenSsl();
    friend void FinalizeCSecurePrngOpenSsl();


//-- Hidden Data Members
protected:
private:
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SECUREPRNG_OPENSSL)

#endif // #ifndef MXG_CSECUREPRNGOPENSSL_H

//M5T_INTERNAL_USE_END

