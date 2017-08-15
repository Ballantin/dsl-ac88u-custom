//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSHA2MACOPENSSL_H
#define MXG_CSHA2MACOPENSSL_H

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

#if defined(MXD_CRYPTO_SHA2MAC_OPENSSL)

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
#ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
#endif // #ifndef MXG_CBLOB_H
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CHMAC_H
    #include "Crypto/CHMac.h"
#endif // #ifndef MXG_CHMAC_H

#ifndef MXG_CSHA2_H
    #include "Crypto/CSha2.h"
#endif // MXG_CSHA2_H

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SHA2MAC_CLASSNAME
    #define MXD_CRYPTO_SHA2MAC_CLASSNAME CSha2MacOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha2MacOpenSsl
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CSha2MacOpenSsl();
    virtual ~CSha2MacOpenSsl();

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
    mxt_result SetState(IN const CSha2MacOpenSsl* pHmac);

    mxt_result SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm);

// Hidden Data Members
//---------------------
protected:
    HMAC_CTX m_macCtx;

    CHMac::EAlgorithm m_eDefaultAlgorithm;
    CHMac::EAlgorithm m_eCurrentAlgorithm;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha2MacOpenSsl::CSha2MacOpenSsl()
:   m_eDefaultAlgorithm(CHMac::eALGORITHM_SHA2_256),
    m_eCurrentAlgorithm(m_eDefaultAlgorithm)
{
    HMAC_CTX_init(&m_macCtx);
}

inline
CSha2MacOpenSsl::~CSha2MacOpenSsl()
{
    HMAC_CTX_cleanup(&m_macCtx);
}

inline
mxt_result CSha2MacOpenSsl::Update(IN const uint8_t* puData, unsigned int uDataSize)
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
mxt_result CSha2MacOpenSsl::End(OUT uint8_t* puHash)
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
mxt_result CSha2MacOpenSsl::Begin(IN const CBlob* pKey,
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
mxt_result CSha2MacOpenSsl::Update(IN const CBlob* pData)
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
mxt_result CSha2MacOpenSsl::End(OUT CBlob* pHash)
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
CHMac::EAlgorithm CSha2MacOpenSsl::GetAlgorithm()
{
    return m_eCurrentAlgorithm;
}

inline
unsigned int CSha2MacOpenSsl::GetSizeInBits()
{
    return uSHA256_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha2MacOpenSsl::GetSizeInBytes()
{
    return uSHA256_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha2MacOpenSsl::SetState(IN const CSha2MacOpenSsl* pHmac)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pHmac);
    return resFE_NOT_IMPLEMENTED;
}

inline
mxt_result CSha2MacOpenSsl::SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm)
{
    // The only supported algorithm for now in SHA-256.
    if (eAlgorithm != CHMac::eALGORITHM_SHA2_256)
    {
        return resFE_INVALID_ARGUMENT;
    }

    m_eDefaultAlgorithm = eAlgorithm;

    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA2MAC_OPENSSL)

#endif // MXG_CSHA2MACOPENSSL_H
//M5T_INTERNAL_USE_END
