//==SDOC========================================================================
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
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CAESMITOSFW_H
#define MXG_CAESMITOSFW_H

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

#if defined(MXD_CRYPTO_AES_MITOSFW) || \
    defined(MXD_CRYPTO_AES_OPENSSL) || \
    defined(MXD_CRYPTO_AES_MOCANA_SS) 

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
        #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H

    #ifndef MXG_CALIGNEDVARIABLESTORAGE_H
        #include "Basic/CAlignedVariableStorage.h"
    #endif
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

#if defined(MXD_CRYPTO_AES_MITOSFW) || defined (MXD_CRYPTO_AES_MOCANA_SS)
    #ifndef HEADER_AES_H
        #include "Crypto/Aes/aes.h"
    #endif // #ifndef HEADER_AES_H

#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCIPHER_H
    #include "Crypto/CCipher.h"
#endif // #ifndef MXG_CCIPHER_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_AES_CLASSNAME
    #define MXD_CRYPTO_AES_CLASSNAME CAesMitosFw
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CAesMitosFw
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------
    CAesMitosFw();
    virtual ~CAesMitosFw();

    mxt_result SetDefaultAction(IN CCipher::EAction eAction);
    mxt_result SetDefaultMode(IN CCipher::EMode eMode);
    mxt_result SetDefaultIV(IN const uint8_t* puIV, IN unsigned int uIVSize);
    mxt_result SetDefaultKey(IN const uint8_t* puKey, IN unsigned int uKeySize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result SetDefaultIV(IN const CBlob* pIV);
    mxt_result SetDefaultKey(IN const CBlob* pKey);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    mxt_result Begin(IN CCipher::EAction eAction,
                     IN CCipher::EMode eMode,
                     IN const uint8_t* puKey,
                     IN unsigned int uKeySize,
                     IN const uint8_t* puIV = NULL,
                     IN unsigned int uIVSize = 0);
    mxt_result Update(IN const uint8_t* puInData,
                      IN unsigned int uInDataSize,
                      OUT uint8_t* puOutData,
                      OUT unsigned int* puOutDataSize);
    mxt_result End(OUT uint8_t* puOutData, OUT unsigned int* puOutDataSize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    mxt_result Begin(IN CCipher::EAction eAction,
                             IN CCipher::EMode eMode,
                             IN const CBlob* pKey,
                             IN const CBlob* pIV = NULL);
    mxt_result Update(IN const CBlob* pInData,
                              OUT CBlob* pOutData,
                              IN bool bAppend = true);
    mxt_result End(OUT CBlob* pOutData, IN bool bAppend = true);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    CCipher::EAlgorithm GetAlgorithm();
    unsigned int GetBlockSizeInBits();
    unsigned int GetBlockSizeInByte();

#if defined(MXD_CAP_ENABLE_SUPPORT)
    const CVector<CCipher::EMode>* GetSupportedModes();
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

private:
    static mxt_result InitializeCAesMitosFw();
    static void FinalizeCAesMitosFw();

// Friends declarations
//----------------------
    friend mxt_result InitializeCAesMitosFw();
    friend void FinalizeCAesMitosFw();

// Hidden Data Members
//---------------------
protected:

    CCipher::EAction m_eDefaultAction;
    CCipher::EMode m_eDefaultMode;
    uint8_t m_auDefaultKey[256 / 8];
    unsigned int m_uDefaultKeySize;

    // Indicates if key is initialized in the cipher.
    bool m_bEncryptKeyInitialized;
    bool m_bDecryptKeyInitialized;

    uint8_t m_auDefaultIV[uAES_BLOCK_SIZE];

    CCipher::EAction m_eAction;
    CCipher::EMode m_eMode;
    AES_KEY m_aesKey;

    uint8_t m_auIV[uAES_BLOCK_SIZE];

    uint8_t m_auUnused[uAES_BLOCK_SIZE];
    unsigned int m_uUnusedSize;

    // eCTR/CFB mode
    unsigned int m_uOffset;

    // eCTR mode
    uint8_t m_auEncryptedCounter[uAES_BLOCK_SIZE];

#if defined(MXD_CAP_ENABLE_SUPPORT)
    static CAlignedVariableStorage<CVector<CCipher::EMode> > ms_modes;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CAesMitosFw::~CAesMitosFw()
{
}

inline
mxt_result CAesMitosFw::SetDefaultAction(IN CCipher::EAction eAction)
{
    m_eDefaultAction = eAction;
    return resS_OK;
}

inline
mxt_result CAesMitosFw::SetDefaultMode(IN CCipher::EMode eMode)
{
    m_eDefaultMode = eMode;
    return resS_OK;
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CAesMitosFw::SetDefaultIV(IN const CBlob* pIV)
{
    return SetDefaultIV(pIV ? pIV->GetFirstIndexPtr() : NULL, pIV ? pIV->GetSize() : 0);
}

inline
mxt_result CAesMitosFw::SetDefaultKey(IN const CBlob* pKey)
{
    return SetDefaultKey(pKey ? pKey->GetFirstIndexPtr() : NULL, pKey ? pKey->GetSize() : 0);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CCipher::EAlgorithm CAesMitosFw::GetAlgorithm()
{
    return CCipher::eALGORITHM_AES;
}

inline
unsigned int CAesMitosFw::GetBlockSizeInBits()
{
    return uAES_BLOCK_SIZE * 8;
}

inline
unsigned int CAesMitosFw::GetBlockSizeInByte()
{
    return uAES_BLOCK_SIZE;
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
const CVector<CCipher::EMode>* CAesMitosFw::GetSupportedModes()
{
    return ms_modes.Instance();
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_AES_MITOSFW) ||
       //     defined(MXD_CRYPTO_AES_OPENSSL) ||
       //     defined(MXD_CRYPTO_AES_MOCANA_SS)

#endif // MXG_CAESMITOSFW_H
//M5T_INTERNAL_USE_END

