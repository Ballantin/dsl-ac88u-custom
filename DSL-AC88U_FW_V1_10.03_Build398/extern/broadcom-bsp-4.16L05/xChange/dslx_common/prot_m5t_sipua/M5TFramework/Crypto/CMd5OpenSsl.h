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
#ifndef MXG_CMD5OPENSSL_H
#define MXG_CMD5OPENSSL_H

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

#if !defined(MXD_CRYPTO_MD5_NONE)

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
    #include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

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
    #define MXD_CRYPTO_MD5_CLASSNAME CMd5OpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CMd5OpenSsl
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CMd5OpenSsl();
    virtual ~CMd5OpenSsl();

    mxt_result Begin(IN CHash::EAlgorithm eAlgorithm);
    mxt_result Update(IN const uint8_t* puData, IN unsigned int uDataSize);
    mxt_result End(OUT uint8_t* puHash);

    mxt_result Update(IN const CBlob* pData);
    mxt_result End(OUT CBlob* pHash);

    CHash::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();
    mxt_result SetState(IN const CMd5OpenSsl* pHash);

// Hidden Data Members
//---------------------
protected:
    typedef MD5_CTX mxt_md5ctx;
    mxt_md5ctx m_md5Ctx;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CMd5OpenSsl::CMd5OpenSsl()
{
}

inline
CMd5OpenSsl::~CMd5OpenSsl()
{
}

inline
mxt_result CMd5OpenSsl::Begin(IN CHash::EAlgorithm eAlgorithm)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eAlgorithm);

    MD5_Init(&m_md5Ctx);

    return resS_OK;
}

inline
mxt_result CMd5OpenSsl::Update(IN const uint8_t* puData, IN unsigned int uDataSize)
{
    mxt_result res = resS_OK;

    if (puData == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        MD5_Update(&m_md5Ctx, puData, static_cast<unsigned long>(uDataSize));
    }

    return res;
}

inline
mxt_result CMd5OpenSsl::End(OUT uint8_t* puHash)
{
    mxt_result res = resS_OK;

    if (puHash == NULL)
    {
        res = resFE_FAIL;
    }
    else
    {
        MD5_Final(puHash, &m_md5Ctx);
    }

    return res;
}

inline
mxt_result CMd5OpenSsl::Update(IN const CBlob* pData)
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
mxt_result CMd5OpenSsl::End(OUT CBlob* pHash)
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

inline
unsigned int CMd5OpenSsl::GetSizeInBits()
{
    return uMD5_HASH_SIZE_IN_BITS;
}

inline
unsigned int CMd5OpenSsl::GetSizeInBytes()
{
    return uMD5_HASH_SIZE_IN_BYTES;
}

inline
CHash::EAlgorithm CMd5OpenSsl::GetAlgorithm()
{
    return CHash::eALGORITHM_MD5;
}

inline
mxt_result CMd5OpenSsl::SetState(IN const CMd5OpenSsl* pHash)
{
    if (pHash != NULL)
    {
        m_md5Ctx = pHash->m_md5Ctx;
    }
    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_MD5_NONE)

#endif // #ifndef MXG_CMD5OPENSSL_H
//M5T_INTERNAL_USE_END

