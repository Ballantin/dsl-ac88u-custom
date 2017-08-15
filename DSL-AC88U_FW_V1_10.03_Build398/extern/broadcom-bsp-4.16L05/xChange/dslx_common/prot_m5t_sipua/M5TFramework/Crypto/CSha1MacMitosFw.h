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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSHA1MACMITOSFW_H
#define MXG_CSHA1MACMITOSFW_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif

#if defined(MXD_CRYPTO_SHA1MAC_MITOSFW)

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

#ifndef MXG_CHMACMITOSFW_H
#include "Crypto/CHMacMitosFw.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SHA1MAC_CLASSNAME
    #define MXD_CRYPTO_SHA1MAC_CLASSNAME CSha1MacMitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha1MacMitosFw : public CHMacMitosFw
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CSha1MacMitosFw();
    virtual ~CSha1MacMitosFw();

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
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    CHMac::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();

    mxt_result SetState(IN const CSha1MacMitosFw* pHmac);

// Hidden Data Members
//---------------------
protected:
    CSha1 m_sha1Ipad;
    CSha1 m_sha1Opad;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha1MacMitosFw::CSha1MacMitosFw()
{
}

inline
CSha1MacMitosFw::~CSha1MacMitosFw()
{
}

inline
mxt_result CSha1MacMitosFw::Begin(IN const uint8_t* puKey,
                                  IN unsigned int uKeySize,
                                  IN CHMac::EAlgorithm eAlgorithm)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eAlgorithm);

    if (puKey == NULL)
    {
        return resFE_FAIL;
    }

    // The following arrays must be 32 bits aligned.
    uint32_t auTempBuf1[uSHA1_BLOCK_SIZE/sizeof(uint32_t)];
    uint32_t auTempBuf2[uSHA1_BLOCK_SIZE/sizeof(uint32_t)];

    return HMacReset(puKey,
                     uKeySize,
                     &m_sha1Ipad,
                     &m_sha1Opad,
                     uSHA1_BLOCK_SIZE,
                     reinterpret_cast<uint8_t*>(auTempBuf1),
                     reinterpret_cast<uint8_t*>(auTempBuf2));
}

inline
mxt_result CSha1MacMitosFw::Update(IN const uint8_t* puData, IN unsigned int uDataSize)
{
    if (puData == NULL)
    {
        return resFE_FAIL;
    }

    return m_sha1Ipad.Update(puData, uDataSize);
}

inline
mxt_result CSha1MacMitosFw::End(OUT uint8_t* puHash)
{
    if (puHash == NULL)
    {
        return resFE_FAIL;
    }

    // 32 bits aligned for better performance.
    uint32_t auTempBuf[uSHA1_HASH_SIZE_IN_BYTES/sizeof(uint32_t)];

    return HMacResult(&m_sha1Ipad,
                      &m_sha1Opad,
                      reinterpret_cast<uint8_t*>(auTempBuf),
                      OUT puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha1MacMitosFw::Begin(IN const CBlob* pKey,
                                  IN CHMac::EAlgorithm eAlgorithm)
{
    if (pKey == NULL)
    {
        return resFE_FAIL;
    }

    return Begin(pKey->GetFirstIndexPtr(), pKey->GetSize(), eAlgorithm);
}

inline
mxt_result CSha1MacMitosFw::Update(IN const CBlob* pData)
{
    if (pData == NULL)
    {
        return resFE_FAIL;
    }

    return Update(pData->GetFirstIndexPtr(), pData->GetSize());
}

inline
mxt_result CSha1MacMitosFw::End(OUT CBlob* pHash)
{
    if (pHash == NULL)
    {
        return resFE_FAIL;
    }

    pHash->ReserveCapacity(GetSizeInBytes());
    pHash->Resize(GetSizeInBytes());

    return End(pHash->GetFirstIndexPtr());
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CSha1MacMitosFw::GetAlgorithm()
{
    return CHMac::eALGORITHM_SHA1;
}

inline
unsigned int CSha1MacMitosFw::GetSizeInBits()
{
    return uSHA1_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha1MacMitosFw::GetSizeInBytes()
{
    return uSHA1_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha1MacMitosFw::SetState(IN const CSha1MacMitosFw* pHmac)
{
    mxt_result res = resS_OK;
    if (pHmac == NULL)
    {
        // Validates if SetState is implemented.
        res = m_sha1Ipad.SetState(NULL);
    }
    else
    {
        res = m_sha1Ipad.SetState(&pHmac->m_sha1Ipad);
        res = MX_RGET_WORST_OF(res, m_sha1Opad.SetState(&pHmac->m_sha1Opad));
    }

    return res;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA1_NONE)

#endif // MXG_CSHA1MACMITOSFW_H
//M5T_INTERNAL_USE_END
