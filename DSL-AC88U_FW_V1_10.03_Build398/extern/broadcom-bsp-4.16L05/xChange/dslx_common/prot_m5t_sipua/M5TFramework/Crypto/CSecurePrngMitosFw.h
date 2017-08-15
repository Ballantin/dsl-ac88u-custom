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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSECUREPRNGMITOSFW_H
#define MXG_CSECUREPRNGMITOSFW_H

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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECUREPRNG_MITOSFW)


#ifndef MXG_CSHA1_H
#include "Crypto/CSha1.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
class CBlob;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SECUREPRNG_CLASSNAME
    #define MXD_CRYPTO_SECUREPRNG_CLASSNAME CSecurePrngMitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
class CSecurePrngMitosFw
{
// Published Interface
//---------------------
public:
    // Summary
    //  Generates uNumberOfBytes random bytes.
    //-----------------------------------------
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT uint8_t* puRandomBytes);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    // Summary
    //  Generates uNumberOfBytes random bytes.
    //-----------------------------------------
    static mxt_result Generate(IN unsigned int uNumberOfBytes,
                               OUT CBlob* pblobRandomBytes);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    // Summary:
    //  Sets the bytes to seed the generator.
    //---------------------------------------
    static mxt_result SetSeed(IN unsigned int uSeedSize,
                              IN uint8_t* puSeedData);

// Hidden Methods
//----------------
protected:
    // << Constructors / Destructors >>
    //----------------------------------
    CSecurePrngMitosFw();
    virtual ~CSecurePrngMitosFw();
private:
    CSecurePrngMitosFw(IN const CSecurePrngMitosFw* pSecurePrng);
    CSecurePrngMitosFw(IN const CSecurePrngMitosFw& rSecurePrng);

    CSecurePrngMitosFw& operator=(IN const CSecurePrngMitosFw& rSecurePrng);

    // Summary
    //  Initializes the seed.
    //------------------------
    static void InitializeSeed();

    // Summary:
    //  Locks the crypto Mutex.
    //-------------------------
    static void Lock();

    // Summary:
    //  Unlocks the crypto Mutex.
    //---------------------------
    static void Unlock();

    static mxt_result InitializeCSecurePrngMitosFw();
    static void FinalizeCSecurePrngMitosFw();

// Friends declarations
//----------------------
    friend mxt_result InitializeCSecurePrngMitosFw();
    friend void FinalizeCSecurePrngMitosFw();

// Hidden Data Members
//---------------------
protected:
private:
    static unsigned int ms_auCurrentBytes[];
    static unsigned int ms_uBytesRemaining;
    static bool ms_bInitialized;
    // The following arrays must be 32 bits aligned for better performance.
    static uint32_t ms_auSeed[uSHA1_HASH_SIZE_IN_BYTES/sizeof(uint32_t)];

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSecurePrngMitosFw::CSecurePrngMitosFw()
{
}

inline
CSecurePrngMitosFw::~CSecurePrngMitosFw()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA1_NONE) && defined(MXD_CRYPTO_SECUREPRNG_MITOSFW)

#endif // MXG_CSECUREPRNGMITOSFW_H
//M5T_INTERNAL_USE_END

