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
#ifndef MXG_CPUBLICKEYOPENSSL_H
#define MXG_CPUBLICKEYOPENSSL_H

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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PUBLICKEY_OPENSSL
#endif

#if defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)
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

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_PUBLICKEY_CLASSNAME
    #define MXD_CRYPTO_PUBLICKEY_CLASSNAME CPublicKeyOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CPublicKeyOpenSsl
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
    CPublicKeyOpenSsl();
    CPublicKeyOpenSsl(IN const CPublicKeyOpenSsl* pKey);
    CPublicKeyOpenSsl(IN const CPublicKeyOpenSsl& rKey);
    virtual ~CPublicKeyOpenSsl();

    CPublicKeyOpenSsl& operator=(IN const CPublicKeyOpenSsl& rKey);
    bool operator==(IN const CPublicKeyOpenSsl& rPublicKey) const;
    bool operator!=(IN const CPublicKeyOpenSsl& rPublicKey) const;

    mxt_result Restore(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm);
    mxt_result RestoreDer(IN const CBlob* pblobKey, IN EAlgorithm eAlgorithm);
    mxt_result RestorePem(IN const CBlob* pblobKey);
    mxt_result Store(OUT CBlob* pblobKey, IN EEncoding eEncoding) const;
    mxt_result StoreDer(OUT CBlob* pblobKey) const;
    mxt_result StorePem(OUT CBlob* pblobKey) const;
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

// Hidden Data Members
//---------------------
protected:
    CCrypto* m_pCrypto;

    EVP_PKEY* m_pEvpPKey;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
bool CPublicKeyOpenSsl::operator!=(IN const CPublicKeyOpenSsl& rKey) const
{
    return !operator==(rKey);
}

inline
void CPublicKeyOpenSsl::Enter() const
{
    m_pCrypto->Enter();
}

inline
void CPublicKeyOpenSsl::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_CRYPTO_PUBLICKEY_OPENSSL)

#endif // MXG_CPUBLICKEYOPENSSL_H
//M5T_INTERNAL_USE_END
