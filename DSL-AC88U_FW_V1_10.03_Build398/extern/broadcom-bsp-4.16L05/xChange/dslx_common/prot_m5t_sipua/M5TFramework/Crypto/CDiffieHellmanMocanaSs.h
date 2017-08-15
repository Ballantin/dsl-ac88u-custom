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
#ifndef MXG_CDIFFIEHELLMANMOCANASS_H
#define MXG_CDIFFIEHELLMANMOCANASS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS)

#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME
    #define MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME CDiffieHellmanMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CDiffieHellmanMocanaSs
{
// Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    CDiffieHellmanMocanaSs();

    CDiffieHellmanMocanaSs(IN const CDiffieHellmanMocanaSs* pDiffieHellman);

    CDiffieHellmanMocanaSs(IN const CDiffieHellmanMocanaSs& rDiffieHellman);

    // Summary:
    //  Destructor.
    virtual ~CDiffieHellmanMocanaSs();

    // Summary:
    //  Assignment Operator.
    CDiffieHellmanMocanaSs& operator=(IN const CDiffieHellmanMocanaSs& rDiffieHellman);

    mxt_result GeneratePrime(IN unsigned int uPrimeSizeInBits = 512,
                             IN unsigned int uGenerator = 2);

    mxt_result GeneratePublicAndPrivateKeys(IN unsigned int uPrivateKeySizeInBits = 0);

    mxt_result GenerateSharedKey(IN const uint8_t* puRemotePublicKey,
                                 IN unsigned int uRemotePublicKeySize);

    mxt_result GenerateSharedKey(IN const CBlob* pblobRemotePublicKey);

    mxt_result GetGenerator(OUT unsigned int* puGenerator) const;

    mxt_result GetPrime(IN unsigned int uMaxSize,
                        OUT uint8_t* puPrime,
                        OUT unsigned int* puPrimeSize) const;

    mxt_result GetPrime(OUT CBlob* pblobPrime) const;

    mxt_result GetPublicKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puPublicKey,
                            OUT unsigned int* puPublicKeySize) const;

    mxt_result GetPublicKey(OUT CBlob* pblobPublicKey) const;

    mxt_result GetPrivateKey(IN unsigned int uMaxSize,
                             OUT uint8_t* puPrivateKey,
                             OUT unsigned int* puPrivateKeySize) const;

    mxt_result GetPrivateKey(OUT CBlob* pblobPrivateKey) const;

    mxt_result GetSharedKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puSharedKey,
                            OUT unsigned int* puSharedKeySize) const;

    mxt_result GetSharedKey(OUT CBlob* pblobSharedKey) const;

    mxt_result SetParameters(IN const uint8_t* puPrime,
                             IN unsigned int uPrimeSize,
                             IN unsigned int uGenerator,
                             IN const uint8_t* puPublicKey,
                             IN unsigned int uPublicKeySize,
                             IN const uint8_t* puPrivateKey,
                             IN unsigned int uPrivateKeySize);

    mxt_result SetParameters(IN const CBlob* pblobPrime,
                             IN unsigned int uGenerator,
                             IN const CBlob* pblobPublicKey,
                             IN const CBlob* pblobPrivateKey);

// Hidden Methods
protected:

    // Summary:
    //  Gets a specific parameter value in the DH internal structure.
    mxt_result GetParameterValue(IN vlong* pstParameter,
                                 IN unsigned int uMaxSize,
                                 OUT uint8_t* puParameter,
                                 OUT unsigned int* puParamterSize) const;

    // Summary:
    //  Locks the crypto mutex.
    void Lock() const;

    // Summary:
    //  Unlocks the crypto mutex.
    void Unlock() const;

// Hidden Data Members
protected:
    // The DH internal structure. This class uses the structure independently
    // from the Mocana SS library utility functions. In the Mocana context, we
    // always use the structure in a 'client' mode. Hence, here is the mapping
    // of the structure members (used through the macros) in the context of this
    // class:
    //    dh_g: the generator.
    //    dh_p: the prime modulus.
    //    dh_y: the private key.
    //    dh_f: the remote public key.
    //    dh_e: the local public key.
    //    dh_k: the shared key
    //    pHashH: unused.
    //
    diffieHellmanContext* m_pDh;

    // The large integers internal allocation queue.
    vlong* m_pQueue;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


//==============================================================================
//==
//==  CDiffieHellmanMocanaSs
//==
//==============================================================================
//
//  Summary:
//      Default constructor.
//
//  Description:
//      Default constructor.
//
//==============================================================================
inline
CDiffieHellmanMocanaSs::CDiffieHellmanMocanaSs()
{
    m_pDh = reinterpret_cast<diffieHellmanContext*>(MOCANA_MALLOC(sizeof(diffieHellmanContext)));
    memset(m_pDh, 0, sizeof(diffieHellmanContext));
    m_pQueue = reinterpret_cast<vlong*>(MOCANA_MALLOC(sizeof(vlong)));
    memset(m_pQueue, 0, sizeof(vlong));
}

//==============================================================================
//==
//==  CDiffieHellmanMocanaSs
//==
//==============================================================================
//
//  Summary:
//      Copy constructor by using a pointer.
//
//  Parameters:
//      pDiffieHellman:
//       The source of the copy.
//
//  Description:
//      Copy constructor by using a pointer
//
//==============================================================================
inline
CDiffieHellmanMocanaSs::CDiffieHellmanMocanaSs(IN const CDiffieHellmanMocanaSs* pDiffieHellman)
{
    m_pQueue = reinterpret_cast<vlong*>(MOCANA_MALLOC(sizeof(vlong)));
    memset(m_pQueue, 0, sizeof(vlong));

    if (pDiffieHellman)
    {
        *this = *pDiffieHellman;
    }
}

//==============================================================================
//==
//==  CDiffieHellmanMocanaSs
//==
//==============================================================================
//
//  Summary:
//      Copy constructor by using a reference.
//
//  Parameters:
//      pDiffieHellman:
//       The source of the copy.
//
//  Description:
//      Copy constructor by using a reference.
//
//==============================================================================
inline
CDiffieHellmanMocanaSs::CDiffieHellmanMocanaSs(IN const CDiffieHellmanMocanaSs& rDiffieHellman)
{
    m_pQueue = reinterpret_cast<vlong*>(MOCANA_MALLOC(sizeof(vlong)));
    memset(m_pQueue, 0, sizeof(vlong));

    *this = rDiffieHellman;
}

inline
void CDiffieHellmanMocanaSs::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CDiffieHellmanMocanaSs::Unlock() const
{
    CCrypto::Instance()->Exit();
}


MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_DIFFIEHELLMAN_MOCANA_SS)

#endif // MXG_CDIFFIEHELLMANMOCANASS_H
//M5T_INTERNAL_USE_END

