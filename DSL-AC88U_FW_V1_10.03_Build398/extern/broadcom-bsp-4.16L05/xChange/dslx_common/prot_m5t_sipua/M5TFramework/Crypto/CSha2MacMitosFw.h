//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CSHA2MACMITOSFW_H
#define MXG_CSHA2MACMITOSFW_H

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

#if defined(MXD_CRYPTO_SHA2MAC_MITOSFW)

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

#ifndef MXG_CSHA2_H
#include "Crypto/CSha2.h"
#endif // MXG_CSHA2_H

#ifndef MXG_CHMACMITOSFW_H
#include "Crypto/CHMacMitosFw.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SHA2MAC_CLASSNAME
    #define MXD_CRYPTO_SHA2MAC_CLASSNAME CSha2MacMitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha2MacMitosFw : public CHMacMitosFw
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CSha2MacMitosFw();
    virtual ~CSha2MacMitosFw();

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
    mxt_result SetState(IN const CSha2MacMitosFw* pHmac);

    mxt_result SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm);

// Hidden methods
//----------------
protected:

// Hidden Data Members
//---------------------
protected:
    CSha2 m_sha2Ipad;
    CSha2 m_sha2Opad;

    CHMac::EAlgorithm m_eDefaultAlgorithm;
    CHMac::EAlgorithm m_eCurrentAlgorithm;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha2MacMitosFw::CSha2MacMitosFw()
:   m_eDefaultAlgorithm(CHMac::eALGORITHM_SHA2_256),
    m_eCurrentAlgorithm(m_eDefaultAlgorithm)
{
}

inline
CSha2MacMitosFw::~CSha2MacMitosFw()
{
}

inline
mxt_result CSha2MacMitosFw::Update(IN const uint8_t* puData, IN unsigned int uDataSize)
{
    if (puData == NULL)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return m_sha2Ipad.Update(puData, uDataSize);
}

inline
mxt_result CSha2MacMitosFw::End(OUT uint8_t* puHash)
{
    if (puHash == NULL)
    {
        return resFE_INVALID_ARGUMENT;
    }

    // 32 bits aligned for better performance.
    uint32_t auTempBuf[uSHA256_HASH_SIZE_IN_BYTES/sizeof(uint32_t)];

    return HMacResult(&m_sha2Ipad,
                      &m_sha2Opad,
                      reinterpret_cast<uint8_t*>(auTempBuf),
                      OUT puHash);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha2MacMitosFw::Begin(IN const CBlob* pKey,
                                  IN CHMac::EAlgorithm eAlgorithm)
{
    if (pKey == NULL)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return Begin(pKey->GetFirstIndexPtr(), pKey->GetSize(), eAlgorithm);
}

inline
mxt_result CSha2MacMitosFw::Update(IN const CBlob* pData)
{
    if (pData == NULL)
    {
        return resFE_INVALID_ARGUMENT;
    }

    return Update(pData->GetFirstIndexPtr(), pData->GetSize());
}
inline
mxt_result CSha2MacMitosFw::End(OUT CBlob* pHash)
{
    if (pHash == NULL)
    {
        return resFE_INVALID_ARGUMENT;
    }

    pHash->ReserveCapacity(GetSizeInBytes());
    pHash->Resize(GetSizeInBytes());

    return End(pHash->GetFirstIndexPtr());
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHMac::EAlgorithm CSha2MacMitosFw::GetAlgorithm()
{
    return m_eCurrentAlgorithm;
}

inline
unsigned int CSha2MacMitosFw::GetSizeInBits()
{
    return uSHA256_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha2MacMitosFw::GetSizeInBytes()
{
    return uSHA256_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha2MacMitosFw::SetState(IN const CSha2MacMitosFw* pHmac)
{
    mxt_result res = resS_OK;
    if (pHmac == NULL)
    {
        // Validates if SetState is implemented.
        res = m_sha2Ipad.SetState(NULL);
    }
    else
    {
        m_eDefaultAlgorithm = pHmac->m_eDefaultAlgorithm;
        m_eCurrentAlgorithm = pHmac->m_eCurrentAlgorithm;

        res = m_sha2Ipad.SetState(&pHmac->m_sha2Ipad);
        res = MX_RGET_WORST_OF(res, m_sha2Opad.SetState(&pHmac->m_sha2Opad));
    }

    return res;
}

inline mxt_result CSha2MacMitosFw::SetDefaultAlgorithm(IN CHMac::EAlgorithm eAlgorithm)
{
    if (eAlgorithm != CHMac::eALGORITHM_SHA2_256)
    {
        return resFE_INVALID_ARGUMENT;
    }

    m_eDefaultAlgorithm = eAlgorithm;

    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_SHA2_NONE)

#endif // MXG_CSHA2MACMITOSFW_H

//M5T_INTERNAL_USE_END
