//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CHASH_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CHASH_H
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

//==SDOC========================================================================
//== Class: CHash
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Base class for all hash algorithms.
//
// Description:
//   CHash is the base class for all hash algorithms.
//
// <CODE>
//
//    void PerformHash(IN CHash* pHash,
//                     IN const uint8_t* puInData,
//                     IN unsigned int uInDataSize,
//                     OUT uint8_t* puHash)
//    {
//        pHash->Begin();
//        pHash->Update(puInData, uInDataSize);
//        pHash->End(puHash);
//    }
//
// </CODE>
//
// Location:
//   Crypto/CHash.h
//
// See Also:
//
//==EDOC========================================================================
class CHash
{
// Published Enum
//----------------
public:

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    //   Enumerates all the supported hash algorithms.
    enum EAlgorithm
    {
        // Description:
        // Default algorithm.
        eALGORITHM_DEFAULT,
        // Description:
        // MD5 algorithm.
        eALGORITHM_MD5,
        // Description:
        // SHA-1 algorithm.
        eALGORITHM_SHA1,
        // Description:
        // SHA-2 algorithm (224 bits).
        eALGORITHM_SHA2_224,
        // Description:
        // SHA-2 algorithm (256 bits).
        eALGORITHM_SHA2_256,
        // Description:
        // SHA-2 algorithm (384 bits).
        eALGORITHM_SHA2_384,
        // Description:
        // SHA-2 algorithm (512 bits).
        eALGORITHM_SHA2_512,

        //M5T_INTERNAL_USE_BEGIN
        //-- This value specifies the maximum of the enum.
        eALGORITHM_MAX
        //M5T_INTERNAL_USE_END
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CHash
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //
    //==EDOC====================================================================
    CHash();

    //==SDOC====================================================================
    //==
    //==  ~CHash
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CHash();

    //==SDOC====================================================================
    //==
    //==  Begin
    //==
    //==========================================================================
    //
    //  Summary:
    //      Begins a new hash.
    //
    //  Parameters:
    //   eAlgorithm:
    //    Algorithm to use in the hash computation.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //
    //  Description:
    //      Begins a new hash.
    //      This is the first method to call to produce a hash.
    //      It must be paired with a single call to End.
    //
    //==EDOC====================================================================
    virtual mxt_result Begin(IN EAlgorithm eAlgorithm = eALGORITHM_DEFAULT) = 0;

    //==SDOC====================================================================
    //==
    //==  Update
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the hash with new data.
    //
    //  Parameters:
    //    puData:
    //      Input data to hash.
    //
    //    puData:
    //      Size of the input data to hash.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Updates the hash with new data.
    //      Begin must have already been called once before Update is
    //      called. Update may be called more than once. Updating from
    //      a buffer one byte at a time gives the same hash as updating from
    //      the same buffer all at once.
    //
    //==EDOC====================================================================
    virtual mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize) = 0;

    //==SDOC====================================================================
    //==
    //==  End
    //==
    //==========================================================================
    //
    //  Summary:
    //      Ends the hash.
    //
    //  Parameters:
    //    puHash:
    //      The output buffer to store the hash.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Ends the hash. End must be called once to finalize the
    //      hash. It must be paired with a single call to Begin.
    //
    //==EDOC====================================================================
    virtual mxt_result End(OUT uint8_t* puHash) = 0;

#if defined(MXD_CAP_ENABLE_SUPPORT)
    //==SDOC====================================================================
    //==
    //==  Update
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the hash with new data.
    //
    //  Parameters:
    //    pData:
    //      Input data to hash.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Updates the hash with new data.
    //      Begin must have already been called once before Update is
    //      called. Update may be called more than once. Updating from
    //      a buffer one byte at a time gives the same hash as updating from
    //      the same buffer all at once.
    //
    //==EDOC====================================================================
    virtual mxt_result Update(IN const CBlob* pData) = 0;

    //==SDOC====================================================================
    //==
    //==  End
    //==
    //==========================================================================
    //
    //  Summary:
    //      Ends the hash.
    //
    //  Parameters:
    //    pHash:
    //      The output buffer to store the hash.
    //
    //  Returns:
    //      - resS_OK
    //      - resFE_FAIL
    //      - resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Ends the hash. End must be called once to finalize the
    //      hash. It must be paired with a single call to Begin.
    //
    //==EDOC====================================================================
    virtual mxt_result End(OUT CBlob* pHash) = 0;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //==  GetAlgorithm
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the algorithm implemented within the inheriting class.
    //
    //  Returns:
    //      One of the CHash::EAlgorithm values.
    //
    //  Description:
    //      Retrieves the algorithm implemented within the inheriting class.
    //
    //==EDOC====================================================================
    virtual EAlgorithm GetAlgorithm() = 0;

    //==SDOC====================================================================
    //==
    //==  GetSizeInBits
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the size in bits of the hash returned by End.
    //
    //  Returns:
    //      The size in bits.
    //
    //  Description:
    //      Retrieves the size in bits of the hash returned by End.
    //
    //==EDOC====================================================================
    virtual unsigned int GetSizeInBits() = 0;

    //==SDOC====================================================================
    //==
    //==  GetSizeInBytes
    //==
    //==========================================================================
    //
    //  Summary:
    //      Retrieves the size in bytes of the hash returned by End.
    //
    //  Returns:
    //      The size in bytes.
    //
    //  Description:
    //      Retrieves the size in bytes of the hash returned by End.
    //
    //==EDOC====================================================================
    virtual unsigned int GetSizeInBytes() = 0;

    //==SDOC====================================================================
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
    //      The other hash objet instance from which to read the state. The hash
    //      algorithm must be the same as the current object. A NULL parameter
    //      can be used to detect whether or not the method is implemented. If
    //      it is implemented, resS_OK is returned. Otherwise, it returns
    //      resFE_NOT_IMPLEMENTED.
    //
    //  Returns:
    //      resS_OK: Method success.
    //      Other: Method failure.
    //
    //  Description:
    //      Sets the internal state from another instance.
    //
    //==EDOC====================================================================
    virtual mxt_result SetState(IN const CHash* pHash) = 0;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CHash::CHash()
{
}

inline
CHash::~CHash()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_CHASH_H
