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
#ifndef MXG_CMD5MACMITOSFW_H
#define MXG_CMD5MACMITOSFW_H

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

#if defined(MXD_CRYPTO_MD5MAC_MITOSFW)

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

#ifndef MXG_CHMACMITOSFW_H
#include "Crypto/CHMacMitosFw.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_MD5MAC_CLASSNAME
    #define MXD_CRYPTO_MD5MAC_CLASSNAME CMd5MacMitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CMd5MacMitosFw : public CHMacMitosFw
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CMd5MacMitosFw();
    virtual ~CMd5MacMitosFw();

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
    mxt_result SetState(IN const CMd5MacMitosFw* pHmac);

// Hidden Data Members
//---------------------
protected:
    CMd5 m_md5Ipad;
    CMd5 m_md5Opad;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CMd5MacMitosFw::CMd5MacMitosFw()
{
}

inline
CMd5MacMitosFw::~CMd5MacMitosFw()
{
}

inline
mxt_result CMd5MacMitosFw::Begin(IN const uint8_t* puKey,
                                 IN unsigned int uKeySize,
                                 IN CHMac::EAlgorithm eAlgorithm)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eAlgorithm);

    if (puKey == NULL)
    {
        return resFE_FAIL;
    }

    // The following arrays must be 32 bits aligned.
    uint32_t auTempBuf1[uMD5_MAC_BLOCK_SIZE/sizeof(uint32_t)];
    uint32_t auTempBuf2[uMD5_MAC_BLOCK_SIZE/sizeof(uint32_t)];

    return HMacReset(puKey,
                     uKeySize,
                     &m_md5Ipad,
                     &m_md5Opad,
                     uMD5_MAC_BLOCK_SIZE,
                     reinterpret_cast<uint8_t*>(auTempBuf1),
                     reinterpret_cast<uint8_t*>(auTempBuf2));

}

inline
mxt_result CMd5MacMitosFw::Update(IN const uint8_t* puData, IN unsigned int uDataSize)
{
    if (puData == NULL)
    {
        return resFE_FAIL;
    }

    return m_md5Ipad.Update(puData, uDataSize);
}

inline
mxt_result CMd5MacMitosFw::End(OUT uint8_t* puHash)
{
    if (puHash == NULL)
    {
        return resFE_FAIL;
    }

    // 32 bits aligned for better performance.
    uint32_t auTempBuf[uMD5_MAC_BLOCK_SIZE/sizeof(uint32_t)];

    return HMacResult(&m_md5Ipad,
                      &m_md5Opad,
                      reinterpret_cast<uint8_t*>(auTempBuf),
                      OUT puHash);

}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CMd5MacMitosFw::Begin(IN const CBlob* pKey,
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
mxt_result CMd5MacMitosFw::Update(IN const CBlob* pData)
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
mxt_result CMd5MacMitosFw::End(OUT CBlob* pHash)
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
CHMac::EAlgorithm CMd5MacMitosFw::GetAlgorithm()
{
    return CHMac::eALGORITHM_MD5;
}

inline
unsigned int CMd5MacMitosFw::GetSizeInBits()
{
    return uMD5_MAC_SIZE_IN_BITS;
}

inline
unsigned int CMd5MacMitosFw::GetSizeInBytes()
{
    return uMD5_MAC_SIZE_IN_BYTES;
}

inline
mxt_result CMd5MacMitosFw::SetState(IN const CMd5MacMitosFw* pHmac)
{
    mxt_result res = resS_OK;
    if (pHmac == NULL)
    {
        // Validates if SetState is implemented.
        res = m_md5Ipad.SetState(NULL);
    }
    else
    {
        res = m_md5Ipad.SetState(&pHmac->m_md5Ipad);
        res = MX_RGET_WORST_OF(res, m_md5Opad.SetState(&pHmac->m_md5Opad));
    }

    return res;
}



MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_MD5MAC_MITOSFW)

#endif // MXG_CMD5MACMITOSFW_H
//M5T_INTERNAL_USE_END

