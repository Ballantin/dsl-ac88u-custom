//==============================================================================
//==============================================================================
//
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
#ifndef MXG_CSHA2OPENSSL_H
#define MXG_CSHA2OPENSSL_H

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
    #include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_CRYPTO_SHA2_OPENSSL)

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
#ifndef MXD_CRYPTO_SHA2_CLASSNAME
    #define MXD_CRYPTO_SHA2_CLASSNAME CSha2OpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CSha2OpenSsl
{
// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------
    CSha2OpenSsl();
    virtual ~CSha2OpenSsl();

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
    mxt_result SetState(IN const CSha2OpenSsl* pHash);

    virtual mxt_result SetDefaultAlgorithm(IN CHash::EAlgorithm eAlgorithm);

private:
    SHA256_CTX m_shaCtx;

    CHash::EAlgorithm m_eDefaultAlgorithm;
    CHash::EAlgorithm m_eCurrentAlgorithm;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CSha2OpenSsl::CSha2OpenSsl()
:   m_eDefaultAlgorithm(CHash::eALGORITHM_SHA2_256),
    m_eCurrentAlgorithm(m_eDefaultAlgorithm)
{
}

inline
CSha2OpenSsl::~CSha2OpenSsl()
{
}

inline
CHash::EAlgorithm CSha2OpenSsl::GetAlgorithm()
{
    return m_eCurrentAlgorithm;
}

inline
unsigned int CSha2OpenSsl::GetSizeInBits()
{
    return uSHA256_HASH_SIZE_IN_BITS;
}

inline
unsigned int CSha2OpenSsl::GetSizeInBytes()
{
    return uSHA256_HASH_SIZE_IN_BYTES;
}

inline
mxt_result CSha2OpenSsl::SetState(IN const CSha2OpenSsl* pHash)
{
    if (pHash != NULL)
    {
        m_shaCtx = pHash->m_shaCtx;

        m_eDefaultAlgorithm = pHash->m_eDefaultAlgorithm;
        m_eCurrentAlgorithm = pHash->m_eCurrentAlgorithm;
    }
    return resS_OK;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_SHA2_OPENSSL)

#endif // #ifndef MXG_CSHA2OPENSSL_H
//M5T_INTERNAL_USE_END
