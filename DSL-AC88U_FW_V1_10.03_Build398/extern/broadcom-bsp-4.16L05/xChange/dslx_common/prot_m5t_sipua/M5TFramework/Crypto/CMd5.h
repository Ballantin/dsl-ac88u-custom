//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_CMD5_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMD5_H
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

#if !defined(MXD_CRYPTO_MD5_NONE)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//  We need to declare these const here so that they are available from
//  MXD_CRYPTO_MD5_INCLUDE.
//----------------------------------------------------------------------

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// Size of the MD5 hash in bits.
const unsigned int uMD5_HASH_SIZE_IN_BITS = 128;

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// Size of the MD5 hash in bytes.
const unsigned int uMD5_HASH_SIZE_IN_BYTES = (uMD5_HASH_SIZE_IN_BITS / 8);

#if defined(MXD_CRYPTO_MD5_INCLUDE)
    #include MXD_CRYPTO_MD5_INCLUDE
#else
    #error "No implementation defined for CMd5!!!"
#endif

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CHASH_H
#include "Crypto/CHash.h"
#endif // #ifndef MXG_CHASH_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CMd5
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the MD5 algorithm.
//
// Description:
//   CMd5 is the class that manages the MD5 algorithm.
//
// Location:
//   Crypto/CMd5.h
//
// See Also:
//
//==EDOC========================================================================
class CMd5 : public CHash,
             public MXD_CRYPTO_MD5_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CMd5
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Constructor.
    //
    //==EDOC====================================================================
    CMd5();

    //==SDOC====================================================================
    //==
    //==  ~CMd5
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC====================================================================
    virtual ~CMd5();

    // Summary:
    // See CHash::Begin.
    virtual mxt_result Begin(IN CHash::EAlgorithm eAlgorithm = CHash::eALGORITHM_DEFAULT);

    virtual mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize);

    virtual mxt_result End(OUT uint8_t* puHash);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual mxt_result Update(IN const CBlob* pData);

    virtual mxt_result End(OUT CBlob* pHash);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    virtual EAlgorithm GetAlgorithm();

    virtual unsigned int GetSizeInBits();

    virtual unsigned int GetSizeInBytes();

    virtual mxt_result SetState(IN const CHash* pHash);
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CMd5::CMd5()
 : MXD_CRYPTO_MD5_CLASSNAME()
{
}

inline
CMd5::~CMd5()
{
}

inline
mxt_result CMd5::Begin(IN CHash::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHash::eALGORITHM_DEFAULT &&
        eAlgorithm != CHash::eALGORITHM_MD5)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_MD5_CLASSNAME::Begin(eAlgorithm);
}

inline
mxt_result CMd5::Update(IN const uint8_t* puData, IN unsigned int uDataSize)
{
    return MXD_CRYPTO_MD5_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CMd5::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_MD5_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CMd5::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_MD5_CLASSNAME::Update(pData);
}

inline
mxt_result CMd5::End(OUT CBlob* pHash)
{
    return MXD_CRYPTO_MD5_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHash::EAlgorithm CMd5::GetAlgorithm()
{
    return MXD_CRYPTO_MD5_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CMd5::GetSizeInBits()
{
    return MXD_CRYPTO_MD5_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CMd5::GetSizeInBytes()
{
    return MXD_CRYPTO_MD5_CLASSNAME::GetSizeInBytes();
}

inline
mxt_result CMd5::SetState(IN const CHash* pHash)
{
    if (pHash != NULL && const_cast<CHash*>(pHash)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of MD5 is permitted at a time).
    return MXD_CRYPTO_MD5_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_MD5_CLASSNAME*>(static_cast<const CMd5*>(pHash)));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_MD5_NONE)

#endif // MXG_CMD5_H


