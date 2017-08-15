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
#ifndef MXG_CMD5MITOSFW_H
#define MXG_CMD5MITOSFW_H

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
    #include "Config/FrameworkCfg.h" // MXD_CRYPTO_MD5_XXX
#endif

#if !defined(MXD_MD5_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
#endif // #ifndef MXG_CBLOB_H

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
#ifndef MXD_CRYPTO_MD5_CLASSNAME
    #define MXD_CRYPTO_MD5_CLASSNAME CMd5MitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CMd5MitosFw
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CMd5MitosFw();
    virtual ~CMd5MitosFw();

    mxt_result Begin(IN CHash::EAlgorithm eAlgorithm);
    mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize);
    mxt_result End(OUT uint8_t* puHash);
#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result Update(IN const CBlob* pData);
    mxt_result End(OUT CBlob* pHash);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    CHash::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();
    mxt_result SetState(IN const CMd5MitosFw* pHash);

// Hidden Data Members
//---------------------
protected:
    typedef struct {
      uint32_t auDigest[4];         // State (ABCD)
      uint32_t auCount[2];          // 64-bit bit count
      uint8_t auBuffer[64];         // Input buffer
    } mxt_md5ctx;

    mxt_md5ctx m_md5Ctx;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CMd5MitosFw::CMd5MitosFw()
{
}

inline
CMd5MitosFw::~CMd5MitosFw()
{
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CMd5MitosFw::Update(IN const CBlob* pData)
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
mxt_result CMd5MitosFw::End(OUT CBlob* pHash)
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
unsigned int CMd5MitosFw::GetSizeInBits()
{
    return uMD5_HASH_SIZE_IN_BITS;
}

inline
unsigned int CMd5MitosFw::GetSizeInBytes()
{
    return uMD5_HASH_SIZE_IN_BYTES;
}

inline
CHash::EAlgorithm CMd5MitosFw::GetAlgorithm()
{
    return CHash::eALGORITHM_MD5;
}

inline
mxt_result CMd5MitosFw::SetState(IN const CMd5MitosFw* pHash)
{
    if (pHash != NULL)
    {
        m_md5Ctx = pHash->m_md5Ctx;
    }
    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_MD5_NONE)

#endif // #ifndef MXG_CMD5MITOSFW_H
//M5T_INTERNAL_USE_END

