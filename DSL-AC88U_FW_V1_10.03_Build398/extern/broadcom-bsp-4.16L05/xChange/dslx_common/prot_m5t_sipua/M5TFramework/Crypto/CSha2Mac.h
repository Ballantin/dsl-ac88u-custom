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
#ifndef MXG_CSHA2MAC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSHA2MAC_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if !defined(MXD_CRYPTO_SHA2MAC_NONE)

#if defined(MXD_CRYPTO_SHA2MAC_INCLUDE)
    #include MXD_CRYPTO_SHA2MAC_INCLUDE
#else
    #error "No implementation defined for CSha2Mac!!!"
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CHMAC_H
#include "Crypto/CHMac.h"
#endif // #ifndef MXG_CHMAC_H

#ifndef MXG_CSHA2_H
#include "Crypto/CSha2.h"
#endif // MXG_CSHA2_H

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSha2Mac
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the SHA-2 Message Authentication Code algorithm.
//
// Description:
//   CSha2Mac is the class that manages the SHA-2 Message Authentication
//   Code algorithm.
//
// Location:
//   Crypto/CSha2Mac.h
//
// See Also:
//   CSha2, CHMac
//
//==EDOC========================================================================
class CSha2Mac : public CHMac,
                 public MXD_CRYPTO_SHA2MAC_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CSha2Mac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Basic constructor.
    //==EDOC====================================================================
    CSha2Mac();

    //==SDOC====================================================================
    //==
    //==  ~CSha2Mac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //==EDOC====================================================================
    virtual ~CSha2Mac();

    // Summary:
    // See CHMac::Begin.
    virtual mxt_result Begin(IN const uint8_t* puKey,
                             IN unsigned int uKeySize,
                             IN CHMac::EAlgorithm eAlgorithm = CHMac::eALGORITHM_DEFAULT);
    virtual mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize);
    virtual mxt_result End(OUT uint8_t* puHash);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    // Summary:
    // See CHMac::Begin.
    virtual mxt_result Begin(IN const CBlob* pKey,
                             IN CHMac::EAlgorithm eAlgorithm = CHMac::eALGORITHM_DEFAULT);
    virtual mxt_result Update(IN const CBlob* pData);
    virtual mxt_result End(OUT CBlob* pHash);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    virtual EAlgorithm GetAlgorithm();
    virtual unsigned int GetSizeInBits();
    virtual unsigned int GetSizeInBytes();
    virtual mxt_result SetState(IN const CHMac* pHMac);

    // Summary:
    // Sets the default digest bit length.
    virtual mxt_result SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm);
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha2Mac::CSha2Mac()
:   MXD_CRYPTO_SHA2MAC_CLASSNAME()
{
}

inline
CSha2Mac::~CSha2Mac()
{
}

inline
mxt_result CSha2Mac::Begin(IN const uint8_t* puKey,
                           IN unsigned int uKeySize,
                           IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm > CHMac::eALGORITHM_DEFAULT &&
        (eAlgorithm < CHMac::eALGORITHM_SHA2_224 ||
         eAlgorithm > CHMac::eALGORITHM_SHA2_512))
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA2MAC_CLASSNAME::Begin(puKey, uKeySize, eAlgorithm);
}

inline
mxt_result CSha2Mac::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CSha2Mac::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha2Mac::Begin(IN const CBlob* pKey,
                           IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm > CHMac::eALGORITHM_DEFAULT &&
        (eAlgorithm < CHMac::eALGORITHM_SHA2_224 ||
         eAlgorithm > CHMac::eALGORITHM_SHA2_512))
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA2MAC_CLASSNAME::Begin(pKey, eAlgorithm);
}
inline
mxt_result CSha2Mac::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::Update(pData);
}
inline
mxt_result CSha2Mac::End(OUT CBlob* pHash)
{
   return MXD_CRYPTO_SHA2MAC_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CSha2Mac::GetAlgorithm()
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CSha2Mac::GetSizeInBits()
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CSha2Mac::GetSizeInBytes()
{
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::GetSizeInBytes();
}

//==============================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Summary:
//      Sets the internal state from another instance.
//
//  Returns:
//      resS_OK: Method success.
//      Other: Method failure.
//
//  Description:
//      Sets the internal state from another instance.
//
//==============================================================================
inline
mxt_result CSha2Mac::SetState(IN const CHMac* pHmac)
{
    if (pHmac != NULL && const_cast<CHMac*>(pHmac)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of HMAC-SHA-2 is permitted at a time).
    return MXD_CRYPTO_SHA2MAC_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_SHA2MAC_CLASSNAME*>(static_cast<const CSha2Mac*>(pHmac)));
}

//==============================================================================
//==
//==  SetDefaultAlgorithm
//==
//==============================================================================
//
//  Parameters:
//      eAlgorithm:
//           The algorithm to use.
//
//  Returns:
//      - resS_OK, the algorithm has correctly been set.
//      - Failure, the provided algorithm is unsupported.
//
//  Description:
//      Sets the default digest bit length for the HMAC-SHA-2 algorithm.
//
//==============================================================================
inline mxt_result CSha2Mac::SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm > CHMac::eALGORITHM_DEFAULT &&
        (eAlgorithm < CHMac::eALGORITHM_SHA2_224 ||
         eAlgorithm > CHMac::eALGORITHM_SHA2_512))
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA2MAC_CLASSNAME::SetDefaultAlgorithm(eAlgorithm);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA2MAC_NONE)

#endif // MXG_CSHA2MAC_H

