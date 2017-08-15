//==============================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CMD5MAC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMD5MAC_H
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

#if !defined(MXD_CRYPTO_MD5MAC_NONE)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//  We need to declare these const here so that they are available from
//  MXD_CRYPTO_MD5_INCLUDE.
//==============================================================================

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// Size of the HMAC in bits.
const unsigned int uMD5_MAC_SIZE_IN_BITS = 128;

//<GROUP CRYPTO_VARIABLES>
//
// Description:
// Size of the HMAC in bytes.
const unsigned int uMD5_MAC_SIZE_IN_BYTES = (uMD5_MAC_SIZE_IN_BITS / 8);

//M5T_INTERNAL_USE_BEGIN
const unsigned int uMD5_MAC_BLOCK_SIZE = 64;
//M5T_INTERNAL_USE_END

#if defined(MXD_CRYPTO_MD5MAC_INCLUDE)
    #include MXD_CRYPTO_MD5MAC_INCLUDE
#else
    #error "No implementation defined for CMd5Mac!!!"
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
#ifndef MXG_CHMAC_H
#include "Crypto/CHMac.h"
#endif // #ifndef MXG_CHMAC_H

#ifndef MXG_CMD5_H
#include "Crypto/CMd5.h"
#endif // MXG_CMD5_H

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
//== Class: CMd5Mac
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the MD5 message authentication code algorithm.
//
// Description:
//   CMd5Mac is the class that manages the MD5 message authentication code
//   algorithm.
//
// Location:
//   Crypto/CMd5Mac.h
//
// See Also:
//
//==EDOC========================================================================
class CMd5Mac : public CHMac,
                public MXD_CRYPTO_MD5MAC_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CMd5Mac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Basic constructor.
    //
    //  Description:
    //      Basic constructor.
    //
    //==EDOC====================================================================
    CMd5Mac();

    //==SDOC====================================================================
    //==
    //==  ~CMd5Mac
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
    virtual ~CMd5Mac();

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
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CMd5Mac::CMd5Mac()
:   MXD_CRYPTO_MD5MAC_CLASSNAME()
{
}

inline
CMd5Mac::~CMd5Mac()
{
}

inline
mxt_result CMd5Mac::Begin(IN const uint8_t* puKey,
                          IN unsigned int uKeySize,
                          IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHMac::eALGORITHM_DEFAULT &&
        eAlgorithm != CHMac::eALGORITHM_MD5)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_MD5MAC_CLASSNAME::Begin(puKey, uKeySize, eAlgorithm);
}

inline
mxt_result CMd5Mac::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
   return MXD_CRYPTO_MD5MAC_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CMd5Mac::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CMd5Mac::Begin(IN const CBlob* pKey,
                          IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHMac::eALGORITHM_DEFAULT &&
        eAlgorithm != CHMac::eALGORITHM_MD5)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_MD5MAC_CLASSNAME::Begin(pKey, eAlgorithm);
}

inline
mxt_result CMd5Mac::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::Update(pData);
}

inline
mxt_result CMd5Mac::End(OUT CBlob* pHash)
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CMd5Mac::GetAlgorithm()
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CMd5Mac::GetSizeInBits()
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CMd5Mac::GetSizeInBytes()
{
    return MXD_CRYPTO_MD5MAC_CLASSNAME::GetSizeInBytes();
}

inline
mxt_result CMd5Mac::SetState(IN const CHMac* pHMac)
{
    if (pHMac != NULL && const_cast<CHMac*>(pHMac)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of HMAC-MD5 is permitted at a time).
    return MXD_CRYPTO_MD5MAC_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_MD5MAC_CLASSNAME*>(static_cast<const CMd5Mac*>(pHMac)));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_MD5MAC_NONE)

#endif // MXG_CMD5MAC_H


