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
#ifndef MXG_CSHA1MITOSFW_H
#define MXG_CSHA1MITOSFW_H

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

#if defined(MXD_CRYPTO_SHA1_MITOSFW)

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
        #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

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
    #define MXD_CRYPTO_SHA1_CLASSNAME CSha1MitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSha1MitosFw
//========================================
//
// Description:
//   Class that manages the SHA-1 algorithm. This implementation is an optimized
//   version of the code found in RFC3174 (ftp:
//   ftp.rfc-editor.org/in-notes/rfc3174.txt), used by written permission from
//   the authors.
//
// Location:
//   Crypto/CSha1MitosFw.h
//
//==EDOC========================================================================
class CSha1MitosFw
{
// Published Interface
//---------------------
public:

    //==========================================================================
    //====  OPTIMIZED VERSON BASED ON THE RFC3174 IMPLEMENTATION  ==============
    //==========================================================================

    // << Constructors / Destructors >>
    //----------------------------------
    CSha1MitosFw();
    virtual ~CSha1MitosFw();

    mxt_result Begin(IN CHash::EAlgorithm eAlgorithm);
    mxt_result Update(IN const uint8_t* puData, unsigned int uDataSize);
    mxt_result End(OUT uint8_t* puHash);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result Update(IN const CBlob* pData);
    mxt_result End(OUT CBlob* pHash);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    CHash::EAlgorithm GetAlgorithm();
    unsigned int GetSizeInBits();
    unsigned int GetSizeInBytes();
    mxt_result SetState(IN const CSha1MitosFw* pHash);

private:
    void Reset();
    void Input(IN const uint8_t *, unsigned int);
    void Result(OUT uint8_t* puMsgDigest);

    void PadMsg();
    void ProcessMsgBlock();

    // These members hold the context information for the SHA-1 hashing
    // operation:
    // Message Digest
    uint32_t m_auIntermediateHash[uSHA1_HASH_SIZE_IN_BYTES / sizeof(uint32_t)];
    // 512-bit message blocks.
    // Must be 32 bits aligned.
    uint32_t m_auMsgBlock[uSHA1_BLOCK_SIZE/sizeof(uint32_t)];
    // Index into message block array
    uint8_t m_uMsgBlockIndex;

    // Message length in bits
#if defined(MXD_CRYPTO_SHA1_DISABLE_LONG_MESSAGES)
    uint32_t m_uLength;
#else
    uint64_t m_uLength;
#endif
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha1MitosFw::CSha1MitosFw()
{
}

inline
CSha1MitosFw::~CSha1MitosFw()
{
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha1MitosFw::End(OUT CBlob* pHash)
{
    mxt_result res;

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
mxt_result CSha1MitosFw::Update(IN const CBlob* pData)
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
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CHash::EAlgorithm CSha1MitosFw::GetAlgorithm()
{
    return CHash::eALGORITHM_SHA1;
}

inline
unsigned int CSha1MitosFw::GetSizeInBits()
{
    return uSHA1_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha1MitosFw::GetSizeInBytes()
{
    return uSHA1_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha1MitosFw::SetState(IN const CSha1MitosFw* pHash)
{
    if (pHash != NULL)
    {
        memcpy(m_auIntermediateHash, pHash->m_auIntermediateHash, sizeof(m_auIntermediateHash));
        memcpy(m_auMsgBlock, pHash->m_auMsgBlock, sizeof(m_auMsgBlock));
        m_uMsgBlockIndex = pHash->m_uMsgBlockIndex;
        m_uLength = pHash->m_uLength;
    }
    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA1_MITOSFW)

#endif // #ifndef MXG_CSHA1MITOSFW_H
//M5T_INTERNAL_USE_END

