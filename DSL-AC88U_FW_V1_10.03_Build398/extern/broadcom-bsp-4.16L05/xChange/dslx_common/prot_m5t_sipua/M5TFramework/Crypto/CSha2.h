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
#ifndef MXG_CSHA2_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSHA2_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
    #include "Config/MxConfig.h"
#endif

//-- M5T Framework configuration
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h"
#endif

#if !defined(MXD_CRYPTO_SHA2_NONE)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// The size of a SHA-256 hash in bits.
const unsigned int uSHA256_HASH_SIZE_IN_BITS = 256;

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// The size of a SHA-256 hash in bytes.
const unsigned int uSHA256_HASH_SIZE_IN_BYTES = (uSHA256_HASH_SIZE_IN_BITS / 8);

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// The size of a SHA-256 block in bytes.
const unsigned int uSHA256_BLOCK_SIZE = 64;

#if defined(MXD_CRYPTO_SHA2_INCLUDE)
    #include MXD_CRYPTO_SHA2_INCLUDE
#else
    #error "No implementation defined for CSha2!!!"
#endif

#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
        #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CHASH_H
    #include "Crypto/CHash.h"
#endif // MXG_CHASH_H

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSha2
//==============================================================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the SHA-2 algorithm.
//
// Description:
//   CSha2 is the class that manages the SHA-2 algorithm. The algorithm digest
//   length is specified by the begin parameter. Only SHA-256 is supported for
//   now.
//
// Example:
//   Here is a typical example of code used to do SHA-2 on the value puInData.
//   The resulting digest value is returned in puHash.
//
//  <CODE>
//      void PerformHash(IN const uint8_t* puInData,
//                       IN unsigned int uInDataSize,
//                       OUT uint8_t* puHash)
//      {
//          CSha2 sha2;
//          sha2.Begin(CHash::eALGORITHM_SHA2_256);
//          sha2.Update(puInData, uInDataSize);
//          sha2.End(puHash);
//      }
//  </CODE>
//
// Location:
//   Crypto/CSha2.h
//
// See Also:
//   CHash, CSha1
//
//==============================================================================
class CSha2 : public CHash,
              public MXD_CRYPTO_SHA2_CLASSNAME
{
public:
    // Summary:
    //  Default Constructor.
    CSha2();

    // Summary:
    //  Destructor.
    virtual ~CSha2();

    // Summary:
    // See CHash::Begin.
    virtual mxt_result Begin(IN CHash::EAlgorithm eAlgorithm = CHash::eALGORITHM_DEFAULT);
    // Summary:
    // See CHash::Update.
    virtual mxt_result Update(IN const uint8_t* puData, unsigned int uDataSize);
    virtual mxt_result End(OUT uint8_t* puHash);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual mxt_result Update(IN const CBlob* pData);
    virtual mxt_result End(OUT CBlob* pHash);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    virtual EAlgorithm GetAlgorithm();
    virtual unsigned int GetSizeInBits();
    virtual unsigned int GetSizeInBytes();
    virtual mxt_result SetState(IN const CHash* pHash);

    //  Summary:
    //      Sets the default hashing algorithm to use when the eAlgorithm parameter
    //      of the Begin method is CHash::eALGORITHM_DEFAULT.
    virtual mxt_result SetDefaultAlgorithm(IN CHash::EAlgorithm eAlgorithm);

protected:
private:
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline
CSha2::CSha2()
:   MXD_CRYPTO_SHA2_CLASSNAME()
{
}

inline
CSha2::~CSha2()
{
}

inline
mxt_result CSha2::Begin(IN CHash::EAlgorithm eAlgorithm)
{
    if (eAlgorithm > CHash::eALGORITHM_DEFAULT &&
        (eAlgorithm < CHash::eALGORITHM_SHA2_224 ||
         eAlgorithm > CHash::eALGORITHM_SHA2_512))
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA2_CLASSNAME::Begin(eAlgorithm);
}

inline
mxt_result CSha2::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    return MXD_CRYPTO_SHA2_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CSha2::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_SHA2_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha2::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_SHA2_CLASSNAME::Update(pData);
}

inline
mxt_result CSha2::End(OUT CBlob* pHash)
{
    return MXD_CRYPTO_SHA2_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHash::EAlgorithm CSha2::GetAlgorithm()
{
    return MXD_CRYPTO_SHA2_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CSha2::GetSizeInBits()
{
    return MXD_CRYPTO_SHA2_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CSha2::GetSizeInBytes()
{
    return MXD_CRYPTO_SHA2_CLASSNAME::GetSizeInBytes();
}

inline
mxt_result CSha2::SetState(IN const CHash* pHash)
{
    if (pHash != NULL && const_cast<CHash*>(pHash)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of SHA-2 is permitted at a time).
    return MXD_CRYPTO_SHA2_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_SHA2_CLASSNAME*>(static_cast<const CSha2*>(pHash)));
}

//==============================================================================
//==
//==  SetDefaultAlgorithm
//==
//==============================================================================
//
//  Parameters:
//   eAlgorithm:
//      One of: CHash::eALGORITHM_SHA2_224,CHash:eALGORITHM_SHA2_256,
//              CHash::eALGORITHM_SHA2_384, CHash::eALGORITHM_SHA2_512.
//
//  Returns:
//      resS_OK: Algorithm accepted.
//      resFE_INVALID_ARGUMENT: eAlgorithm is not one of:
//              CHash::eALGORITHM_SHA2_224, CHash:eALGORITHM_SHA2_256,
//              CHash::eALGORITHM_SHA2_384, CHash::eALGORITHM_SHA2_512.
//
//  Description:
//      Sets the default hashing algorithm to use when the eAlgorithm parameter
//      of the Begin method is CHash::eALGORITHM_DEFAULT.
//
//==============================================================================
inline mxt_result CSha2::SetDefaultAlgorithm(IN CHash::EAlgorithm eAlgorithm)
{
    if (eAlgorithm > CHash::eALGORITHM_DEFAULT &&
        (eAlgorithm < CHash::eALGORITHM_SHA2_224 ||
         eAlgorithm > CHash::eALGORITHM_SHA2_512))
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA2_CLASSNAME::SetDefaultAlgorithm(eAlgorithm);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA2_NONE)

#endif // #define MXG_CSHA2_H

