//==SDOC========================================================================
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
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CMD5MACOPENSSL_H
#define MXG_CMD5MACOPENSSL_H

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

#if defined(MXD_CRYPTO_MD5MAC_OPENSSL)

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

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_MD5MAC_CLASSNAME
    #define MXD_CRYPTO_MD5MAC_CLASSNAME CMd5MacOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CMd5MacOpenSsl
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CMd5MacOpenSsl();
    virtual ~CMd5MacOpenSsl();

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
    mxt_result SetState(IN const CMd5MacOpenSsl* pHmac);

// Hidden Data Members
//---------------------
protected:
    HMAC_CTX m_macCtx;
    
    EVP_MD m_md5;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CMd5MacOpenSsl::CMd5MacOpenSsl()
{
    HMAC_CTX_init(&m_macCtx);

#if defined MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT
    // OpenSSL MD5 algorithm is not FIPS compliant, but nevertheless, this flag
    // allows its use by HMAC algorithm.
    HMAC_CTX_set_flags(&m_macCtx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
#endif
}

inline
CMd5MacOpenSsl::~CMd5MacOpenSsl()
{
    HMAC_CTX_cleanup(&m_macCtx);
}

inline
mxt_result CMd5MacOpenSsl::Begin(IN const uint8_t* puKey,
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
        // Initialize the message digest structure with the MD5 parameters.
        memcpy(&m_md5, EVP_md5(), sizeof(EVP_MD));

#if defined MXD_OPENSSL_FIPS_140_2_ENABLE_SUPPORT
        // Set this flag to allow the use of the MD5 algorithm in the HMAC
        // algorithm.
        m_md5.flags |= EVP_MD_FLAG_FIPS;
#endif

        // Copy the key to the key block along with the message digest
        // parameters structure, while making sure not to use the (deprecated)
        // HMAC_Init() function, since it causes a memory leak.
        HMAC_Init_ex(&m_macCtx,
                     puKey,
                     static_cast<int>(uKeySize),
                     &m_md5,
                     NULL);
    }

    return res;
}

inline
mxt_result CMd5MacOpenSsl::Update(IN const uint8_t* puData, unsigned int uDataSize)
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
mxt_result CMd5MacOpenSsl::End(OUT uint8_t* puHash)
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
mxt_result CMd5MacOpenSsl::Begin(IN const CBlob* pKey,
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
mxt_result CMd5MacOpenSsl::Update(IN const CBlob* pData)
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
mxt_result CMd5MacOpenSsl::End(OUT CBlob* pHash)
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
CHMac::EAlgorithm CMd5MacOpenSsl::GetAlgorithm()
{
    return CHMac::eALGORITHM_MD5;
}

inline
unsigned int CMd5MacOpenSsl::GetSizeInBits()
{
    return uMD5_MAC_SIZE_IN_BITS;
}

inline
unsigned int CMd5MacOpenSsl::GetSizeInBytes()
{
    return uMD5_MAC_SIZE_IN_BYTES;
}

inline
mxt_result CMd5MacOpenSsl::SetState(IN const CMd5MacOpenSsl* pHmac)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pHmac);
    return resFE_NOT_IMPLEMENTED;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_MD5MAC_OPENSSL)

#endif // MXG_CMD5MACOPENSLL_H
//M5T_INTERNAL_USE_END

