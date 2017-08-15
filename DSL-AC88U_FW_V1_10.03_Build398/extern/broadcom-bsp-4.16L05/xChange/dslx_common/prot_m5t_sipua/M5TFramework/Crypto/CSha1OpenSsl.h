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
#ifndef MXG_CSHA1OPENSSL_H
#define MXG_CSHA1OPENSSL_H

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
    #include "Config/FrameworkCfg.h" // MXD_CRYPTO_SHA1_XXX
#endif

#if defined(MXD_CRYPTO_SHA1_OPENSSL)

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
#ifndef MXD_CRYPTO_SHA1_CLASSNAME
    #define MXD_CRYPTO_SHA1_CLASSNAME CSha1OpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha1OpenSsl
{
// Published Interface
//---------------------
public:

    //==========================================================================
    //====  OPTIMIZED VERSON BASED ON THE RFC3174 IMPLEMENTATION  ==============
    //==========================================================================

    // << Constructors / Destructors >>
    //----------------------------------
    CSha1OpenSsl();
    virtual ~CSha1OpenSsl();

    mxt_result Begin(IN CHash::EAlgorithm eAlgorithm);
    mxt_result Update(IN const uint8_t* puData, unsigned int uDataSize);
    mxt_result End(OUT uint8_t* puHash);

    mxt_result Update(IN const CBlob* pData);
    mxt_result End(OUT CBlob* pHash);

    CHash::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();
    mxt_result SetState(IN const CSha1OpenSsl* pHash);

private:
    SHA_CTX m_shaCtx;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha1OpenSsl::CSha1OpenSsl()
{
}

inline
CSha1OpenSsl::~CSha1OpenSsl()
{
}

inline
mxt_result CSha1OpenSsl::Begin(IN CHash::EAlgorithm eAlgorithm)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(eAlgorithm);

    SHA1_Init(&m_shaCtx);

    return resS_OK;
}

inline
mxt_result CSha1OpenSsl::Update(IN const uint8_t* puData, unsigned int uDataSize)
{
    mxt_result res = resS_OK;

    if (puData == NULL)
    {
        res = resFE_INVALID_ARGUMENT;
    }
    else
    {
        SHA1_Update(&m_shaCtx, puData, static_cast<unsigned long>(uDataSize));
    }

    return res;
}

inline
mxt_result CSha1OpenSsl::End(OUT uint8_t* puHash)
{
    mxt_result res = resS_OK;

    if (puHash == NULL)
    {
        res = resFE_INVALID_ARGUMENT;
    }
    else
    {
        SHA1_Final(puHash, &m_shaCtx);
    }

    return res;
}

inline
mxt_result CSha1OpenSsl::End(OUT CBlob* pHash)
{
    mxt_result res;

    if (pHash == NULL)
    {
        res = resFE_INVALID_ARGUMENT;
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
mxt_result CSha1OpenSsl::Update(IN const CBlob* pData)
{
    mxt_result res = resS_OK;

    if (pData == NULL)
    {
        res = resFE_INVALID_ARGUMENT;
    }
    else
    {
        res = Update(pData->GetFirstIndexPtr(), pData->GetSize());
    }

    return res;
}

inline
CHash::EAlgorithm CSha1OpenSsl::GetAlgorithm()
{
    return CHash::eALGORITHM_SHA1;
}

inline
unsigned int CSha1OpenSsl::GetSizeInBits()
{
    return uSHA1_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha1OpenSsl::GetSizeInBytes()
{
    return uSHA1_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha1OpenSsl::SetState(IN const CSha1OpenSsl* pHash)
{
    if (pHash != NULL)
    {
        m_shaCtx = pHash->m_shaCtx;
    }
    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA1_OPENSSL)

#endif // #ifndef MXG_CSHA1OPENSSL_H
//M5T_INTERNAL_USE_END

