//==============================================================================
//==============================================================================
//
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CHMAC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CHMAC_H
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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#if !defined(MXG_CHASH_H)
#include "Crypto/CHash.h"
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

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CHMac
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Base class for all HMAC algorithms.
//
// Description:
//   CHMac is the base class for all HMAC algorithms.
//
// <CODE>
//
//    mxt_result PerformHMac(IN const CBlob* pKey,
//                           IN const CBlob* pInData,
//                           OUT CBlob* pOutHMac,
//                           IN CHMac* pHMac)
//    {
//        mxt_result res = resS_OK;
//
//        res = pHMac->Begin(pKey);
//        if (MX_RIS_S(res))
//        {
//            res = pHMac->Update(pInData);
//        }
//        if (MX_RIS_S(res))
//        {
//            res = pHMac->End(pOutHMac);
//        }
//    }
//
// </CODE>
//
// Location:
//   Crypto/CHMac.h
//
//==============================================================================
class CHMac
{
// Published Enum
//----------------
public:

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    //   Enumerates all the supported HMac algorithms.
    enum EAlgorithm
    {
        // Description:
        // Default algorithm.
        eALGORITHM_DEFAULT = CHash::eALGORITHM_DEFAULT,
        // Description:
        // MD5 algorithm.
        eALGORITHM_MD5 = CHash::eALGORITHM_MD5,
        // Description:
        // SHA-1 algorithm.
        eALGORITHM_SHA1 = CHash::eALGORITHM_SHA1,
        // Description:
        // SHA-2 algorithm (224 bits).
        eALGORITHM_SHA2_224 = CHash::eALGORITHM_SHA2_224,
        // Description:
        // SHA-2 algorithm (256 bits).
        eALGORITHM_SHA2_256 = CHash::eALGORITHM_SHA2_256,
        // Description:
        // SHA-2 algorithm (384 bits).
        eALGORITHM_SHA2_384 = CHash::eALGORITHM_SHA2_384,
        // Description:
        // SHA-2 algorithm (512 bits).
        eALGORITHM_SHA2_512 = CHash::eALGORITHM_SHA2_512,

        //M5T_INTERNAL_USE_BEGIN
        //-- This value specifies the maximum of the enum.
        eALGORITHM_MAX = CHash::eALGORITHM_MAX
        //M5T_INTERNAL_USE_END
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==========================================================================
    //==
    //==  CHMac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==========================================================================
    virtual ~CHMac();

    //==========================================================================
    //==
    //==  Begin
    //==
    //==========================================================================
    //
    //  Summary:
    //      Begins a new hash message authentication code.
    //
    //  Parameters:
    //      puKey:
    //          Key to use to generate the HMAC.
    //
    //      uKeySize:
    //          Size of the key in bytes.
    //
    //      eAlgorithm:
    //          The hashing algorithm to use.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Begins a new hash message authentication code.
    //      This is the first method to call to produce a HMAC.
    //      It must be paired with a single call to End.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result Begin(IN const uint8_t* puKey,
                             IN unsigned int uKeySize,
                             IN EAlgorithm eAlgorithm = eALGORITHM_DEFAULT) = 0;

    //==========================================================================
    //==
    //==  Update
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets data to HMAC.
    //
    //  Parameters:
    //      puData:
    //          Data to use to generate the HMAC.
    //
    //      uDataSize:
    //          Size of the data in bytes.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Sets the specified data to HMAC. Multiple calls to the method result
    //      in the total data being hashed in blocks of the hash input size.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize) = 0;

    //==========================================================================
    //==
    //==  End
    //==
    //==========================================================================
    //
    //  Summary:
    //      Ends the HMAC generation.
    //
    //  Parameters:
    //      puHash:
    //          Pointer to contain the resulting HMAC.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Ends the generation of the HMAC. The generated HMAC is then
    //      returned.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result End(OUT uint8_t* puHash) = 0;

#if defined(MXD_CAP_ENABLE_SUPPORT)

    //==========================================================================
    //==
    //==  Begin
    //==
    //==========================================================================
    //
    //  Summary:
    //      Begins a new hash message authentication code.
    //
    //  Parameters:
    //      pKey:
    //          Pointer to a blob.
    //
    //      eAlgorithm:
    //          The hashing algorithm to use.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Begins a new hash message authentication code.
    //      This is the first method to call to produce a HMAC.
    //      It must be paired with a single call to End.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result Begin(IN const CBlob* pKey,
                             IN EAlgorithm eAlgorithm = eALGORITHM_DEFAULT) = 0;

    //==========================================================================
    //==
    //==  Update
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets data to HMAC.
    //
    //  Parameters:
    //      pData:
    //          Pointer to a blob containing the data.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Sets the specified data to HMAC. Multiple calls to the method result
    //      in the total data being hashed in blocks of the hash input size.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result Update(IN const CBlob* pData) = 0;

    //==========================================================================
    //==
    //==  End
    //==
    //==========================================================================
    //
    //  Summary:
    //      Ends the HMAC generation.
    //
    //  Parameters:
    //      pHash:
    //          Pointer to a blob to contain the resulting HMAC.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Ends the generation of the HMAC. The generated HMAC is then
    //      returned.
    //
    //  See also:
    //      End
    //
    //==========================================================================
    virtual mxt_result End(OUT CBlob* pHash) = 0;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    //==========================================================================
    //==
    //==  GetAlgorithm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the algorithm.
    //
    //  Returns:
    //      The algorithm used.
    //
    //  Description:
    //      Gets the algorithm used to create the hash.
    //
    //==========================================================================
    virtual EAlgorithm GetAlgorithm() = 0;

    //==========================================================================
    //==
    //==  GetSizeInBits
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the size in bits.
    //
    //  Returns:
    //      The size of the HMAC in bits
    //
    //  Description:
    //      Gets the size of the generated HMAC in bits.
    //
    //==========================================================================
    virtual unsigned int GetSizeInBits() = 0;

    //==========================================================================
    //==
    //==  GetSizeInBytes
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the size in bytes.
    //
    //  Returns:
    //      The size of the HMAC in bytes.
    //
    //  Description:
    //      Gets the size of the generated HMAC in bytes.
    //
    //==========================================================================
    virtual unsigned int GetSizeInBytes() = 0;

    //==========================================================================
    //==
    //==  SetState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the internal state from another instance.
    //
    //  Parameters:
    //    pHash:
    //      The other hash-mac objet instance from which to read the state. The
    //      hash-mac algorithm must be the same as the current object. A NULL
    //      parameter can be used to detect whether or not the method is
    //      implemented. If it is implemented, resS_OK is returned. Otherwise,
    //      it returns resFE_NOT_IMPLEMENTED.
    //
    //  Returns:
    //      resS_OK: Method success.
    //      Other: Method failure.
    //
    //  Description:
    //      Sets the internal state from another instance.
    //
    //==========================================================================
    virtual mxt_result SetState(IN const CHMac* pHmac) = 0;

//-- Hidden methods.
protected:
    //==========================================================================
    //==
    //==  CHMac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //
    //==========================================================================
    CHMac();

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CHMac::CHMac()
{
}

inline
CHMac::~CHMac()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_CHMAC_H


