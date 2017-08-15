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
#ifndef MXG_CSHA1_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSHA1_H
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

#if !defined(MXD_CRYPTO_SHA1_NONE)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//  We need to declare these const here so that they are available from
//  MXD_CRYPTO_SHA1_INCLUDE.
//----------------------------------------------------------------------

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// The size of a SHA-1 hash in bits.
const unsigned int uSHA1_HASH_SIZE_IN_BITS = 160;

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// The size of a SHA-1 hash in bytes.
const unsigned int uSHA1_HASH_SIZE_IN_BYTES = (uSHA1_HASH_SIZE_IN_BITS / 8);

//M5T_INTERNAL_USE_BEGIN
const unsigned int uSHA1_BLOCK_SIZE = 64;
//M5T_INTERNAL_USE_END

#if defined(MXD_CRYPTO_SHA1_INCLUDE)
    #include MXD_CRYPTO_SHA1_INCLUDE
#else
    #error "No implementation defined for CSha1!!!"
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
#ifndef MXG_CHASH_H
    #include "Crypto/CHash.h"
#endif // MXG_CHASH_H

//-- Interface Realized and/or Parent
//-------------------------------------

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
//== Class: CSha1
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the SHA-1 algorithm.
//
// Description:
//   CSha1 is the class that manages the SHA-1 algorithm.
//
// Location:
//   Crypto/CSha1.h
//
// See Also:
//
//==EDOC========================================================================
class CSha1 : public CHash,
              public MXD_CRYPTO_SHA1_CLASSNAME
{
// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CSha1
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //==EDOC====================================================================
    CSha1();

    //==SDOC====================================================================
    //==
    //==  ~CSha1
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //==EDOC====================================================================
    virtual ~CSha1();

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
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha1::CSha1()
 : MXD_CRYPTO_SHA1_CLASSNAME()
{
}

inline
CSha1::~CSha1()
{
}

inline
mxt_result CSha1::Begin(IN CHash::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHash::eALGORITHM_DEFAULT &&
        eAlgorithm != CHash::eALGORITHM_SHA1)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA1_CLASSNAME::Begin(eAlgorithm);
}

inline
mxt_result CSha1::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    return MXD_CRYPTO_SHA1_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CSha1::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_SHA1_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha1::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_SHA1_CLASSNAME::Update(pData);
}

inline
mxt_result CSha1::End(OUT CBlob* pHash)
{
    return MXD_CRYPTO_SHA1_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHash::EAlgorithm CSha1::GetAlgorithm()
{
    return MXD_CRYPTO_SHA1_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CSha1::GetSizeInBits()
{
    return MXD_CRYPTO_SHA1_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CSha1::GetSizeInBytes()
{
    return MXD_CRYPTO_SHA1_CLASSNAME::GetSizeInBytes();
}

inline
mxt_result CSha1::SetState(IN const CHash* pHash)
{
    if (pHash != NULL && const_cast<CHash*>(pHash)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of SHA-1 is permitted at a time).
    return MXD_CRYPTO_SHA1_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_SHA1_CLASSNAME*>(static_cast<const CSha1*>(pHash)));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA1_NONE)

#endif // #ifndef MXG_CSHA1_H


