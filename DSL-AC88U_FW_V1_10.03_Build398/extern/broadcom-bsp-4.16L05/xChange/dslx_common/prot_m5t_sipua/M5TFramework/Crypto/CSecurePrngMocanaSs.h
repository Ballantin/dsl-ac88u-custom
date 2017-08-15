//==============================================================================
//==============================================================================
//
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSECUREPRNGMOCANASS_H
#define MXG_CSECUREPRNGMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS)

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
#if defined(MXD_CAP_ENABLE_SUPPORT)
class CBlob;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_SECUREPRNG_CLASSNAME
    #define MXD_CRYPTO_SECUREPRNG_CLASSNAME CSecurePrngMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
class CSecurePrngMocanaSs
{
// Published Interface.
public:
    // Summary
    //  Generates uNumberOfBytes random bytes.
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT uint8_t* puRandomBytes);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    // Summary
    //  Generates uNumberOfBytes random bytes.
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT CBlob* pblobRandomBytes);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    // Summary:
    //  Sets the bytes to seed the generator.
    static mxt_result SetSeed(IN unsigned int uSeedSize,
                              IN uint8_t* puSeedData);

// Hidden Methods.
protected:
    // << Constructors / Destructors >>.
    CSecurePrngMocanaSs();
    virtual ~CSecurePrngMocanaSs();

private:
    CSecurePrngMocanaSs(IN const CSecurePrngMocanaSs* pSecurePrng);
    CSecurePrngMocanaSs(IN const CSecurePrngMocanaSs& rSecurePrng);

    CSecurePrngMocanaSs& operator=(IN const CSecurePrngMocanaSs& rSecurePrng);

    // Summary:
    //  Locks the crypto Mutex.
    static void Lock();

    // Summary:
    //  Unlocks the crypto Mutex.
    static void Unlock();
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SECUREPRNG_MOCANA_SS)

#endif // MXG_CSECUREPRNGMOCANASS_H
//M5T_INTERNAL_USE_END

