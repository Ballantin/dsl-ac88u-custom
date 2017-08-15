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
#ifndef MXG_CDIFFIEHELLMANOPENSSL_H
#define MXG_CDIFFIEHELLMANOPENSSL_H

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

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL)

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

//-- Parameters used by Value
//----------------------------

//-- Interface Realized and/or Parent
//------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
#ifndef MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME
    #define MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME CDiffieHellmanOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CDiffieHellmanOpenSsl
{
// Published Interface
//---------------------
public:

    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Constructor.
    //-----------------------
    CDiffieHellmanOpenSsl();

    // Summary:
    //  Copy Constructor by using a pointer.
    //---------------------------------------
    CDiffieHellmanOpenSsl(IN const CDiffieHellmanOpenSsl* pDiffieHellman);

    // Summary:
    //  Copy Constructor by using a reference.
    //-----------------------------------------
    CDiffieHellmanOpenSsl(IN const CDiffieHellmanOpenSsl& rDiffieHellman);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CDiffieHellmanOpenSsl();

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CDiffieHellmanOpenSsl& operator=(IN const CDiffieHellmanOpenSsl& rDiffieHellman);

    // Summary:
    //  Generates a prime number.
    //----------------------------
    mxt_result GeneratePrime(IN unsigned int uPrimeSizeInBits = 1024,
                             IN unsigned int uGenerator = 2);

    // Summary:
    //  Generates a private and public key.
    //--------------------------------------
    mxt_result GeneratePublicAndPrivateKeys(IN unsigned int uPrivateKeySizeInBits = 0);

    // Summary:
    //  Generates the shared key.
    //----------------------------
    mxt_result GenerateSharedKey(IN const uint8_t* puRemotePublicKey,
                                 IN unsigned int uRemotePublicKeySize);

    mxt_result GenerateSharedKey(IN const CBlob* pblobRemotePublicKey);

    // Summary:
    //  Gets the generator.
    //----------------------
    mxt_result GetGenerator(OUT unsigned int* puGenerator) const;

    // Summary:
    //  Gets the prime number.
    //-------------------------
    mxt_result GetPrime(IN unsigned int uMaxSize,
                        OUT uint8_t* puPrime,
                        OUT unsigned int* puPrimeSize) const;

    mxt_result GetPrime(OUT CBlob* pblobPrime) const;

    // Summary:
    //  Gets the Public key.
    //-----------------------
    mxt_result GetPublicKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puPublicKey,
                            OUT unsigned int* puPublicKeySize) const;

    mxt_result GetPublicKey(OUT CBlob* pblobPublicKey) const;

    // Summary:
    //  Gets the Private key.
    //------------------------
    mxt_result GetPrivateKey(IN unsigned int uMaxSize,
                             OUT uint8_t* puPrivateKey,
                             OUT unsigned int* puPrivateKeySize) const;

    mxt_result GetPrivateKey(OUT CBlob* pblobPrivateKey) const;

    // Summary:
    //  Gets the shared key.
    //-----------------------
    mxt_result GetSharedKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puSharedKey,
                            OUT unsigned int* puSharedKeySize) const;

    mxt_result GetSharedKey(OUT CBlob* pblobSharedKey) const;

    // Summary:
    //  Sets the parameters used for generating keys.
    //------------------------------------------------
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
//----------------
protected:
    // Summary:
    //  Locks the crypto mutex.
    void Lock() const;

    // Summary:
    //  Unlocks the crypto mutex.
    void Unlock() const;

private:

// Hidden Data Members
//---------------------
protected:
    DH* m_pDh; // Store all parameters for the Diffie-Hellman algorithm.
    CBlob* m_pblobSharedKey;

};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CDiffieHellmanOpenSsl::CDiffieHellmanOpenSsl()
: m_pDh(NULL),
  m_pblobSharedKey(NULL)
{
}

inline
CDiffieHellmanOpenSsl::CDiffieHellmanOpenSsl(IN const CDiffieHellmanOpenSsl* pDiffieHellman)
: m_pDh(NULL),
  m_pblobSharedKey(NULL)
{
    if (pDiffieHellman)
    {
        *this = *pDiffieHellman;
    }
}

inline
CDiffieHellmanOpenSsl::CDiffieHellmanOpenSsl(IN const CDiffieHellmanOpenSsl& rDiffieHellman)
: m_pDh(NULL),
  m_pblobSharedKey(NULL)
{
    *this = rDiffieHellman;
}

inline
void CDiffieHellmanOpenSsl::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CDiffieHellmanOpenSsl::Unlock() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_DIFFIEHELLMAN_OPENSSL)

#endif // MXG_CDIFFIEHELLMANOPENSSL_H
//M5T_INTERNAL_USE_END

