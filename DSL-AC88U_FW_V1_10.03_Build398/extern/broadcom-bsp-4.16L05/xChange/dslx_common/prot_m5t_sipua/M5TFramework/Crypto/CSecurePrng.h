//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
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
//==============================================================================
#ifndef MXG_CSECUREPRNG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSECUREPRNG_H
//M5T_INTERNAL_USE_END

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

#if !defined(MXD_CRYPTO_SECUREPRNG_NONE)

#if defined(MXD_CRYPTO_SECUREPRNG_INCLUDE)
    #include MXD_CRYPTO_SECUREPRNG_INCLUDE
#else
    #error "No implementation defined for CSecurePrng!!!"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.

#if defined(MXD_CAP_ENABLE_SUPPORT)
class CBlob;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSecurePrng
//==============================================================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class implementing a secure pseudo-random generator.
//
// Description:
//   CPrng is the class that manages a secure pseudo-random number generator.
//
// Location:
//   Crypto/CSecurePrng.h
//
//==============================================================================
class CSecurePrng : public MXD_CRYPTO_SECUREPRNG_CLASSNAME
{
// Published Interface.
public:

    //==========================================================================
    //==
    //==   Generate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets x random bytes.
    //
    //  Parameters:
    //      uNumberOfBytes:
    //          The number of random bytes to get.
    //
    //      puRandomBytes:
    //          The byte array to contain the random data. This pointer cannot
    //          be NULL.
    //
    //  Returns:
    //      - resS_OK: the number has been set successfully
    //      - resFE_FAIL: the generation failed.
    //      - resFE_INVALID_ARGUMENT: puRandomBytes is NULL.
    //      - resFE_NOT_IMPLEMENTED: the PRNG is not implemented.
    //
    //  Description:
    //      Generates pseudo-random bytes for use in cryptography (seeded at
    //      startup). Bytes are generated 20 at a time from the seed by using a
    //      Sha1Mac hash of the last 20 bytes, an internal counter, and the
    //      generate seed as the key. These bytes are regenerated every time the
    //      20 previous bytes have been used by calls to GetRandomBytes.
    //
    //==========================================================================
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT uint8_t* puRandomBytes);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    //==========================================================================
    //==
    //==   Generate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets x random bytes.
    //
    //  Parameters:
    //      uNumberOfBytes:
    //          The number of random bytes to get.
    //
    //      pblobRandomBytes:
    //          The blob to contain the random data.
    //
    //  Returns:
    //      - resS_OK: the number has been set successfully
    //      - resFE_FAIL: the generation failed.
    //      - resFE_INVALID_ARGUMENT: pblobRandomBytes is NULL.
    //      - resFE_NOT_IMPLEMENTED: the PRNG is not implemented.
    //
    //  Description:
    //      Generates pseudo-random bytes for use in cryptography (seeded at
    //      startup). Bytes are generated 20 at a time from the seed by using a
    //      SHA-1 hash of the last 20 bytes used and an internal counter. These
    //      bytes are regenerated every time the 20 previous bytes have been
    //      used by calls to GetRandomBytes.
    //
    //==========================================================================
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT CBlob* pblobRandomBytes);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    //==========================================================================
    //==
    //==   SetSeed
    //==
    //==========================================================================
    //
    //  Summary:
    //      Seeds the generator.
    //
    //  Parameters:
    //      uSeedSize:
    //          Number of bytes to seed the generator with.
    //
    //      puSeedData:
    //          The data to seed the generator.
    //
    //  Returns:
    //      - resS_OK: the number has been set successfully
    //      - resFE_FAIL: the generation failed.
    //      - resFE_INVALID_ARGUMENT: puSeedData is NULL.
    //      - resFE_NOT_IMPLEMENTED: the PRNG is not implemented.
    //
    //  Description:
    //      Seeds the generator with x random bytes. These are meant to add
    //      additional entropy to the one already gathered to create the seed.
    //
    //==========================================================================
    static mxt_result SetSeed(IN unsigned int uSeedSize,
                              IN uint8_t* puSeedData);

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods.
private:
#if defined(MXD_COMPILER_GNU_GCC)
    // Must be enclosed in the #if defined(MXD_COMPILER_GNU_GCC) to avoid
    // waring or error with other compiler where MXD_COMPILER_GNU_GCC has no
    // value while it is required for the comparison.
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_7)
        // GCC 2.7.2 complains about private destructor with no friends.
        friend class Foo;
    #endif
#endif

    // << Constructors / Destructors >>.
    CSecurePrng();
    virtual ~CSecurePrng();

    CSecurePrng(IN const CSecurePrng* pSecurePrng);
    CSecurePrng(IN const CSecurePrng& rSecurePrng);

    CSecurePrng& operator=(IN const CSecurePrng& rSecurePrng);
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline
CSecurePrng::CSecurePrng()
 : MXD_CRYPTO_SECUREPRNG_CLASSNAME()
{
}

inline
CSecurePrng::~CSecurePrng()
{
}

inline
mxt_result CSecurePrng::Generate(IN unsigned int uNumberOfBytes,
                                 OUT uint8_t* puRandomBytes)
{
    return MXD_CRYPTO_SECUREPRNG_CLASSNAME::Generate(uNumberOfBytes,
                                                     puRandomBytes);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSecurePrng::Generate(IN unsigned int uNumberOfBytes,
                                 OUT CBlob* pblobRandomBytes)
{
    return MXD_CRYPTO_SECUREPRNG_CLASSNAME::Generate(uNumberOfBytes,
                                                     pblobRandomBytes);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
mxt_result CSecurePrng::SetSeed(IN unsigned int uSeedSize,
                                IN uint8_t* puSeedData)
{
    return MXD_CRYPTO_SECUREPRNG_CLASSNAME::SetSeed(uSeedSize, puSeedData);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SECUREPRNG_NONE)

#endif // MXG_CSECUREPRNG_H


