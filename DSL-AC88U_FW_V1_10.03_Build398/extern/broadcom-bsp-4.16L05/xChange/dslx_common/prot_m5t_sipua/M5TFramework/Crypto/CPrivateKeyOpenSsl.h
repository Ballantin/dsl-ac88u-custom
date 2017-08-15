//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CPRIVATEKEYOPENSSL_H
#define MXG_CPRIVATEKEYOPENSSL_H

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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PRIVATEKEY_OPENSSL
#endif

#if defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL)
//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class IPassPhrase;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_PRIVATEKEY_CLASSNAME
    #define MXD_CRYPTO_PRIVATEKEY_CLASSNAME CPrivateKeyOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CPrivateKeyOpenSsl
{

// Published Enum
//----------------
public:

    // Defines the various algorithms a key can be used for.
    enum EAlgorithm
    {
        eALGORITHM_DSA,
        eALGORITHM_RSA,
        eALGORITHM_UNKNOWN
    };

    // Defines the types of encoding the key can be in.
    enum EEncoding
    {
        eENCODING_DER,
        eENCODING_PEM
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    CPrivateKeyOpenSsl();
    CPrivateKeyOpenSsl(IN const CPrivateKeyOpenSsl* pKey);
    CPrivateKeyOpenSsl(IN const CPrivateKeyOpenSsl& rKey);
    virtual ~CPrivateKeyOpenSsl();

    CPrivateKeyOpenSsl& operator=(IN const CPrivateKeyOpenSsl& rKey);
    bool operator==(IN const CPrivateKeyOpenSsl& rKey) const;
    bool operator!=(IN const CPrivateKeyOpenSsl& rKey) const;

    mxt_result Restore(IN const CBlob* pblobKey,
                       IN EAlgorithm eAlgorithm,
                       IN IPassPhrase* pPassPhrase,
                       IN mxt_opaque opqPassPhraseParameter);
    mxt_result RestoreDer(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm);
    mxt_result RestorePem(IN const CBlob* pblobKey, IN const char* pszPassPhrase);
    mxt_result RestorePem(IN const CBlob* pblobKey,
                          IN IPassPhrase* pPassPhrase,
                          IN mxt_opaque opqPassPhraseParameter);
    mxt_result Store(OUT CBlob* pblobKey, IN EEncoding eEncoding, IN const char* pszPassPhrase) const;
    mxt_result StoreDer(OUT CBlob* pblobKey) const;
    mxt_result StorePem(OUT CBlob* pblobKey, IN const char* pszPassPhrase) const;
    mxt_result GetAlgorithm(OUT EAlgorithm* peAlgorithm) const;

    // These methods allow optimisation for other algorithms when openssl is used.
    mxt_result SetKey(IN EVP_PKEY* pstEvpKey);
    mxt_result GetKey(OUT EVP_PKEY** ppstEvpKey) const;

// Hidden Methods
//----------------
private:
protected:

    void Enter() const;
    void Exit() const;

    static int pem_password_cb(char *buf, int size, int rwflag, void *userdata);

// Hidden Data Members
//---------------------
protected:

    CCrypto* m_pCrypto;

    EVP_PKEY* m_pEvpPKey;

    IPassPhrase* m_pPassPhrase;

    mxt_opaque m_opqPassPhraseParameter;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
bool CPrivateKeyOpenSsl::operator!=(IN const CPrivateKeyOpenSsl& rKey) const
{
    return !operator==(rKey);
}

inline
void CPrivateKeyOpenSsl::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CPrivateKeyOpenSsl::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_CRYPTO_PRIVATEKEY_OPENSSL)

#endif // MXG_CPRIVATEKEYOPENSSL_H
//M5T_INTERNAL_USE_END
