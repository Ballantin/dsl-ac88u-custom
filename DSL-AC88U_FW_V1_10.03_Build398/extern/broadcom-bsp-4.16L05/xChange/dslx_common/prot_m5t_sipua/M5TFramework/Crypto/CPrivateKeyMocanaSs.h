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
#ifndef MXG_CPRIVATEKEYMOCANASS_H
#define MXG_CPRIVATEKEYMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_PRIVATEKEY_MOCANA_SS
#endif

#if defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)

#ifndef MXG_CASYMMETRICKEYMOCANASS_H
#include "Crypto/CAsymmetricKeyMocanaSs.h"
#endif

#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif

extern "C"
{
    #include "crypto/pubcrypto.h"
}

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IPassPhrase;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_PRIVATEKEY_CLASSNAME
    #define MXD_CRYPTO_PRIVATEKEY_CLASSNAME CPrivateKeyMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CPrivateKeyMocanaSs : public CAsymmetricKeyMocanaSs
{
//-- Friend Declarations
friend class CRsaMocanaSs;

// Published Interface
public:

    // << Constructors / Destructors / Operators >>.

    CPrivateKeyMocanaSs();
    CPrivateKeyMocanaSs(IN const CPrivateKeyMocanaSs* pKey);
    CPrivateKeyMocanaSs(IN const CPrivateKeyMocanaSs& rKey);
    virtual ~CPrivateKeyMocanaSs();

    mxt_result Restore(IN const CBlob* pblobKey,
                       IN EAlgorithm eAlgorithm,
                       IN IPassPhrase* pPassPhrase,
                       IN mxt_opaque opqPassPhraseParameter);
    mxt_result RestorePem(IN const CBlob* pblobKey, IN const char* pszPassPhrase);
    mxt_result RestorePem(IN const CBlob* pblobKey,
                          IN IPassPhrase* pPassPhrase,
                          IN mxt_opaque opqPassPhraseParameter);

    mxt_result Store(OUT CBlob* pblobKey, IN EEncoding eEncoding, IN const char* pszPassPhrase) const;
    mxt_result StorePem(OUT CBlob* pblobKey, IN const char* pszPassPhrase) const;

// Hidden Methods.
protected:

    // Summary:
    //   Acquire the access to the global crypto mutex.
    void Enter() const;

    // Summary:
    //   Release the access to the global crypto mutex.
    void Exit() const;

    // Summary:
    //   Generates a new RSA key.
    mxt_result GenerateKey(IN EAlgorithm eAlgorithm,
                           IN unsigned int uExponent,
                           IN unsigned int uKeySizeInBits);

// Hidden Data Members
protected:

    // Pointer to the CCrypto instance.
    CCrypto* m_pCrypto;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline
mxt_result CPrivateKeyMocanaSs::Restore(IN const CBlob* pblobKey,
                                        IN EAlgorithm eAlgorithm,
                                        IN IPassPhrase* pPassPhrase,
                                        IN mxt_opaque opqPassPhraseParameter)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pPassPhrase);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(opqPassPhraseParameter);
    return CAsymmetricKeyMocanaSs::Restore(pblobKey, eAlgorithm);
}

inline
mxt_result CPrivateKeyMocanaSs::RestorePem(IN const CBlob* pblobKey, IN const char* pszPassPhrase)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszPassPhrase);
    return CAsymmetricKeyMocanaSs::RestorePem(pblobKey);
}

inline
mxt_result CPrivateKeyMocanaSs::RestorePem(IN const CBlob* pblobKey,
                                           IN IPassPhrase* pPassPhrase,
                                           IN mxt_opaque opqPassPhraseParameter)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pPassPhrase);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(opqPassPhraseParameter);
    return CAsymmetricKeyMocanaSs::RestorePem(pblobKey);
}

inline
mxt_result CPrivateKeyMocanaSs::Store(OUT CBlob* pblobKey,
                                      IN EEncoding eEncoding,
                                      IN const char* pszPassPhrase) const
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszPassPhrase);
    return CAsymmetricKeyMocanaSs::Store(pblobKey, eEncoding);
}

inline
mxt_result CPrivateKeyMocanaSs::StorePem(OUT CBlob* pblobKey, IN const char* pszPassPhrase) const
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pszPassPhrase);
    return CAsymmetricKeyMocanaSs::StorePem(pblobKey);
}

//==============================================================================
//==
//==  Enter
//==
//==============================================================================
//
//  Description:
//      Acquire the access to the global crypto mutex.
//
//==============================================================================
inline
void CPrivateKeyMocanaSs::Enter() const
{
    m_pCrypto->Enter();
}

//==============================================================================
//==
//==  Exit
//==
//==============================================================================
//
//  Description:
//      Release the access to the global crypto mutex.
//
//==============================================================================
inline
void CPrivateKeyMocanaSs::Exit() const
{
    m_pCrypto->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_CRYPTO_PRIVATEKEY_MOCANA_SS)

#endif // MXG_CPRIVATEKEYMOCANASS_H
//M5T_INTERNAL_USE_END
