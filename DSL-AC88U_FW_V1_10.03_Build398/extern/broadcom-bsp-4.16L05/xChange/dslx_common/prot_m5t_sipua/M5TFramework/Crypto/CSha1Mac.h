//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSHA1MAC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSHA1MAC_H
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

#if !defined(MXD_CRYPTO_SHA1MAC_NONE)

#if defined(MXD_CRYPTO_SHA1MAC_INCLUDE)
    #include MXD_CRYPTO_SHA1MAC_INCLUDE
#else
    #error "No implementation defined for CSha1Mac!!!"
#endif

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CHMAC_H
#include "Crypto/CHMac.h"
#endif // #ifndef MXG_CHMAC_H

#ifndef MXG_CSHA1_H
#include "Crypto/CSha1.h"
#endif // MXG_CSHA1_H

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSha1Mac
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the SHA-1 Message Authentication Code algorithm.
//
// Description:
//   CSha1Mac is the class that manages the SHA-1 Message Authentication Code
//   algorithm.
//
// Location:
//   Crypto/CSha1Mac.h
//
// See Also:
//   CSha1, CHMac
//
//==EDOC========================================================================
class CSha1Mac : public CHMac,
                 public MXD_CRYPTO_SHA1MAC_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CSha1Mac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Default constructor.
    //==EDOC====================================================================
    CSha1Mac();

    //==SDOC====================================================================
    //==
    //==  ~CSha1Mac
    //==
    //==========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //==EDOC====================================================================
    virtual ~CSha1Mac();

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
CSha1Mac::CSha1Mac()
 : MXD_CRYPTO_SHA1MAC_CLASSNAME()
{
}

inline
CSha1Mac::~CSha1Mac()
{
}

inline
mxt_result CSha1Mac::Begin(IN const uint8_t* puKey,
                           IN unsigned int uKeySize,
                           IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHMac::eALGORITHM_DEFAULT &&
        eAlgorithm != CHMac::eALGORITHM_SHA1)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA1MAC_CLASSNAME::Begin(puKey, uKeySize, eAlgorithm);
}

inline
mxt_result CSha1Mac::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::Update(puData, uDataSize);
}

inline
mxt_result CSha1Mac::End(OUT uint8_t* puHash)
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::End(puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha1Mac::Begin(IN const CBlob* pKey,
                           IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHMac::eALGORITHM_DEFAULT &&
        eAlgorithm != CHMac::eALGORITHM_SHA1)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return MXD_CRYPTO_SHA1MAC_CLASSNAME::Begin(pKey, eAlgorithm);
}
inline
mxt_result CSha1Mac::Update(IN const CBlob* pData)
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::Update(pData);
}
inline
mxt_result CSha1Mac::End(OUT CBlob* pHash)
{
   return MXD_CRYPTO_SHA1MAC_CLASSNAME::End(pHash);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CSha1Mac::GetAlgorithm()
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CSha1Mac::GetSizeInBits()
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::GetSizeInBits();
}

inline
unsigned int CSha1Mac::GetSizeInBytes()
{
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::GetSizeInBytes();
}

inline
mxt_result CSha1Mac::SetState(IN const CHMac* pHmac)
{
    if (pHmac != NULL && const_cast<CHMac*>(pHmac)->GetAlgorithm() != GetAlgorithm())
    {
        return resFE_INVALID_ARGUMENT;
    }

    // The previous validation ensures that the downcast is valid (only one
    // implementation of HMAC-SHA-1 is permitted at a time).
    return MXD_CRYPTO_SHA1MAC_CLASSNAME::SetState(static_cast<const MXD_CRYPTO_SHA1MAC_CLASSNAME*>(static_cast<const CSha1Mac*>(pHmac)));
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if!defined(MXD_CRYPTO_SHA1MAC_NONE)

#endif // MXG_CSHA1MAC_H


