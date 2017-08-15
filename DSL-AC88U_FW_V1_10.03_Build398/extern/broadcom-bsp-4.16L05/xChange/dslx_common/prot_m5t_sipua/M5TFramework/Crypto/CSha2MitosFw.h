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
#ifndef MXG_CSHA2MITOSFW_H
#define MXG_CSHA2MITOSFW_H

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
    #include "Config/FrameworkCfg.h" // MXD_CRYPTO_SHA2_XXX
#endif

#if defined(MXD_CRYPTO_SHA2_MITOSFW)

#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
        #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CHASH_H
    #include "Crypto/CHash.h"
#endif // MXG_CHASH_H

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_SHA2_CLASSNAME
    #define MXD_CRYPTO_SHA2_CLASSNAME CSha2MitosFw
#endif

//==SDOC========================================================================
//== Class: CSha2MitosFw
//========================================
//
// Description:
//   Class that manages the SHA-2 algorithm. This class only supports the 256
//   bits flavor of the algorithm. This implementation is a version
//   of the code found in RFC4634 (ftp://ftp.rfc-editor.org/in-notes/rfc4634.txt)
//   used by written permission from the authors.
//
// Location:
//   Crypto/CSha2MitosFw.h
//
//==EDOC========================================================================
class CSha2MitosFw
{
public:
    // Summary:
    //  Default Constructor.
    CSha2MitosFw();

    // Summary:
    //  Destructor.
    virtual ~CSha2MitosFw();

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
    mxt_result SetState(IN const CSha2MitosFw* pHash);

    virtual mxt_result SetDefaultAlgorithm(IN CHash::EAlgorithm eAlgorithm);

//-- Hidden Methods
protected:
private:
    bool AddLength(unsigned int uLength);
    mxt_result Input(IN const uint8_t *puMsgBlock, unsigned int uLength);
    void Reset(IN CHash::EAlgorithm eAlgorithm);
    mxt_result Result(OUT uint8_t auMsgDigest[uSHA256_HASH_SIZE_IN_BYTES]);
    void PadMsg(uint8_t uPadByte);
    void ProcessMsgBlock();
    void Finalize(uint8_t uPadByte);

//-- Hidden Data Members
protected:
private:
    bool m_bComputed;
    bool m_bCorrupted;
    uint8_t m_uMsgBlockIndex;
    uint8_t m_auMsgBlock[uSHA256_BLOCK_SIZE];
    uint32_t m_uLengthHigh;
    uint32_t m_uLengthLow;
    uint32_t m_auIntermediateHash[uSHA256_HASH_SIZE_IN_BYTES / sizeof(uint32_t)];

    CHash::EAlgorithm m_eDefaultAlgorithm;
    CHash::EAlgorithm m_eCurrentAlgorithm;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================
inline
CSha2MitosFw::CSha2MitosFw()
:   m_eDefaultAlgorithm(CHash::eALGORITHM_SHA2_256),
    m_eCurrentAlgorithm(m_eDefaultAlgorithm)
{
}

inline
CSha2MitosFw::~CSha2MitosFw()
{
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CSha2MitosFw::End(OUT CBlob* pHash)
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
mxt_result CSha2MitosFw::Update(IN const CBlob* pData)
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
CHash::EAlgorithm CSha2MitosFw::GetAlgorithm()
{
    return m_eCurrentAlgorithm;
}

inline
unsigned int CSha2MitosFw::GetSizeInBits()
{
    return uSHA256_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha2MitosFw::GetSizeInBytes()
{
    return uSHA256_HASH_SIZE_IN_BYTES;
}

//==============================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Summary:
//      Sets the internal state from another instance.
//
//  Returns:
//      resS_OK: Method success.
//      Other: Method failure.
//
//  Description:
//      Sets the internal state from another instance.
//
//==============================================================================
inline
mxt_result CSha2MitosFw::SetState(IN const CSha2MitosFw* pHash)
{
    if (pHash != NULL)
    {
        m_bComputed = pHash->m_bComputed;
        m_bCorrupted = pHash->m_bCorrupted;
        m_uMsgBlockIndex = pHash->m_uMsgBlockIndex;
        memcpy(m_auMsgBlock, pHash->m_auMsgBlock, sizeof(m_auMsgBlock));
        m_uLengthHigh = pHash->m_uLengthHigh;
        m_uLengthLow = pHash->m_uLengthLow;
        memcpy(m_auIntermediateHash, pHash->m_auIntermediateHash, sizeof(m_auIntermediateHash));
        m_eDefaultAlgorithm = pHash->m_eDefaultAlgorithm;
        m_eCurrentAlgorithm = pHash->m_eCurrentAlgorithm;
    }
    return resS_OK;
}

//==SDOC========================================================================
//==
//==  AddLength
//==
//==============================================================================
//
//  Parameters:
//      uLength:
//          The length of the final bits to add.
//
//  Returns:
//      False if it is possible to add the desired length, true otherwise.
//
//  Description:
//      This function adds the desired length to the message.
//
//==EDOC========================================================================
inline
bool CSha2MitosFw::AddLength(unsigned int uLength)
{
    unsigned int uAddTemp = m_uLengthLow;
    m_bCorrupted = ((m_uLengthLow += (uLength)) < uAddTemp) && (++m_uLengthHigh == 0) ? true : false;

    return m_bCorrupted;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA2_MITOSFW)

#endif // #define MXG_CSHA2MITOSFW_H

//M5T_INTERNAL_USE_END
