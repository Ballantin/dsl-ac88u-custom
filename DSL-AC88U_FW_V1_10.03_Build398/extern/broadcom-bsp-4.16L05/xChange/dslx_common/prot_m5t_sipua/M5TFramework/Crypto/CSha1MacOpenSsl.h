//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSHA1MACOPENSSL_H
#define MXG_CSHA1MACOPENSSL_H

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

#if defined(MXD_CRYPTO_SHA1MAC_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
#endif // #ifndef MXG_CBLOB_H

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
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SHA1MAC_CLASSNAME
    #define MXD_CRYPTO_SHA1MAC_CLASSNAME CSha1MacOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha1MacOpenSsl
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CSha1MacOpenSsl();
    virtual ~CSha1MacOpenSsl();

    mxt_result Begin(IN const uint8_t* puKey,
                     IN unsigned int uKeySize,
                     IN CHMac::EAlgorithm eAlgorithm);
    mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize);
    mxt_result End(OUT uint8_t* puHash);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result Begin(IN const CBlob* pKey,
                     IN CHMac::EAlgorithm eAlgorithm);
    mxt_result Update(IN const CBlob* pData);
    mxt_result End(OUT CBlob* pHash);
#endif

    CHMac::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();
    mxt_result SetState(IN const CSha1MacOpenSsl* pHmac);

// Hidden Data Members
//---------------------
protected:
    HMAC_CTX m_macCtx;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha1MacOpenSsl::CSha1MacOpenSsl()
{
    HMAC_CTX_init(&m_macCtx);
}

inline
CSha1MacOpenSsl::~CSha1MacOpenSsl()
{
    HMAC_CTX_cleanup(&m_macCtx);
}

inline
mxt_result CSha1MacOpenSsl::Begin(IN const uint8_t* puKey,
                                  IN unsigned int uKeySize,
                                  IN CHMac::EAlgorithm eAlgorithm)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eAlgorithm);

    mxt_result res = resS_OK;

    if (puKey == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        // Copy Key to KeyBlock
        // Do not use HMAC_Init(), it causes a memory leak.
        // HMAC_Init() is also deprecated.
        HMAC_Init_ex(&m_macCtx,
                     puKey,
                     static_cast<int>(uKeySize),
                     EVP_sha1(),
                     NULL);
    }

    return res;
}

inline
mxt_result CSha1MacOpenSsl::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    mxt_result res = resS_OK;

    if (puData == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        HMAC_Update(&m_macCtx, puData, static_cast<unsigned long>(uDataSize));
    }

    return res;
}


inline
mxt_result CSha1MacOpenSsl::End(OUT uint8_t* puHash)
{
    mxt_result res = resS_OK;

    if (puHash == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        HMAC_Final(&m_macCtx, puHash, NULL);
    }

    return res;
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha1MacOpenSsl::Begin(IN const CBlob* pKey,
                                  IN CHMac::EAlgorithm eAlgorithm)
{
    mxt_result res = resS_OK;

    if (pKey == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        res = Begin(pKey->GetFirstIndexPtr(), pKey->GetSize(), eAlgorithm);
    }

    return res;
}
inline
mxt_result CSha1MacOpenSsl::Update(IN const CBlob* pData)
{
    mxt_result res = resS_OK;

    if (pData == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        res = Update(pData->GetFirstIndexPtr(), pData->GetSize());
    }

    return res;
}
inline
mxt_result CSha1MacOpenSsl::End(OUT CBlob* pHash)
{
    mxt_result res = resS_OK;

    if (pHash == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        pHash->ReserveCapacity(GetSizeInBytes());
        pHash->Resize(GetSizeInBytes());

        res = End(pHash->GetFirstIndexPtr());
    }

    return res;
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CSha1MacOpenSsl::GetAlgorithm()
{
    return CHMac::eALGORITHM_SHA1;
}

inline
unsigned int CSha1MacOpenSsl::GetSizeInBits()
{
    return uSHA1_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha1MacOpenSsl::GetSizeInBytes()
{
    return uSHA1_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha1MacOpenSsl::SetState(IN const CSha1MacOpenSsl* pHmac)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pHmac);
    return resFE_NOT_IMPLEMENTED;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA1MAC_OPENSSL)

#endif // MXG_CSHA1MACOPENSSL_H
//M5T_INTERNAL_USE_END

