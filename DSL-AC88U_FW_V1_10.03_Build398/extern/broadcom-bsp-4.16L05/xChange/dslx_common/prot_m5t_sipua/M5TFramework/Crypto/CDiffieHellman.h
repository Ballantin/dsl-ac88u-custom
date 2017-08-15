//==============================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CDIFFIEHELLMAN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CDIFFIEHELLMAN_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
    #include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration.
#ifndef MXG_FRAMEWORKCFG_H
    #include "Config/FrameworkCfg.h"
#endif

#if !defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE)

#if defined(MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE)
    #include MXD_CRYPTO_DIFFIEHELLMAN_INCLUDE
#else
    #error "No implementation defined for CDiffieHellman!!!"
#endif

//-- Data Members.
#ifndef MXG_CCRYPTO_H
    #include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CDiffieHellman
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the Diffie-Hellman algorithm.
//
// Description:
//   CDiffieHellman is the class that manages the Diffie-Hellman key exchange
//   algorithm.
//
//   Diffie-Hellman:
//
//     The Diffie-Hellman algorithm is a key exchange protocol that was
//     developed by Bailey Whitfield Diffie and Martin Hellman in 1976. It
//     allows two users to exchange a secret key over an insecure medium without
//     any prior exchange.
//
//     Suppose Alice and Bob want to agree on a shared secret key.
//
//       First, Alice must generate a random private key x and Bob generates
//       a random private key y. Both x and y must meet certain
//       mathematical criteria and be kept secret.
//
//       Second, they compute their public key using two parameters: a large
//       prime p and a generator g, which they have both agreed upon beforehand.
//       Alice computes X = g^x mod p and Bob computes Y = g^y mod p.
//
//       Alice sends Bob her public key X and Bob sends Alice his public key Y.
//
//       Finally, Alice computes k = (Y^x) mod p, and Bob computes
//       k' = (X^y) mod p. Both k and k' are equal to  g^(x * y) mod p.
//       Alice and Bob now both have a shared secret key k (k').
//
//   To successfully use Diffie-Hellman, a few steps must be executed in the
//   correct order.
//
//   First, a generator g must be chosen. It affects the generation of the large
//   prime p. These two parameters are public. They may be chosen by the
//   initiator of the protocol or they may be hardcoded. The only rule is that
//   the same parameters g and p must be used by both sides. To generate the
//   prime p, the method GeneratePrime must be called. To use an already
//   generated prime p, the method SetParameters must be called. It is important
//   to note that the generation of the prime p is a very CPU-intensive task.
//
//   Instead of choosing an abitrary generator and generating a prime,
//   Diffie-Hellman Oakley groups can be used. Groups 1 (768-bit prime), 2
//   (1024-bit prime), 5 (1536-bit prime), 14 (2048-bit prime), 15 (3072-bit
//   prime), 16 (4096-bit prime), 17 (6144-bit prime) and 18 (8192-bit prime)
//   are available in this class.
//
//   Once the parameters are known, the public and private keys must be
//   generated. This is accomplished with a call to the method
//   GeneratePublicAndPrivateKeys.
//
//   Now, the initiator must send his public key. It is possible to also send a
//   generator g and a prime p if required. All these values are public and may
//   be observed without any security breach. Only the private key must remain
//   secret.
//
//   Upon reception of the peer's public key, the initiator must call the method
//   GenerateSharedKey using the peer's public key.
//
//   From now on, both sides have access to the same shared key.
//
//   <CODE>
//
//   Initiator                                    Acceptor
//   =========                                    ========
//   g = generator
//   GeneratePrime(1024, g)
//   GeneratePublicAndPrivateKeys()
//   p = GetPrime()
//   X = GetPublicKey()
//                 ---------- g, p, X ---------->
//                                                SetParameters(p, g)
//                                                GeneratePublicAndPrivateKeys()
//                                                Y = GetPublicKey()
//                 <------------ Y --------------
//   K = GenerateSharedKey(Y)                     K = GenerateSharedKey(X)
//
//   </CODE>
//
//  Note:
//   Keys generated with Diffie-Hellman may not be of the exact length of the
//   specified prime. The reason for this is that leading zeros in the keys are
//   discarded. However, when retrieving the keys, they are appropriately
//   padded.
//
// Location:
//   Crypto/CDiffieHellman.h
//
//==============================================================================
class CDiffieHellman : public MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME
{
//M5T_INTERNAL_USE_BEGIN
    friend class CAsyncTlsSocket;
//M5T_INTERNAL_USE_END

// Published Constants.
public:

    // Summary:
    //  Oakley group 1 768-bit prime from RFC 2409: 2^768 - 2 ^704 - 1 + 2^64 * ((2^638 pi) + 149686).
    static const uint8_t ms_auOAKLEY1_PRIME[96];

    // Summary:
    //  Oakley group 1 generator : 2.
    static const unsigned int ms_uOAKLEY1_GENERATOR;

    // Summary:
    //  Oakley group 2 1024-bit prime from RFC 2409: 2^1024 - 2^960 - 1 + 2^64 * ((2^894 pi) + 129093)
    static const uint8_t ms_auOAKLEY2_PRIME[128];

    // Summary:
    //  Oakley group 2 generator : 2.
    static const unsigned int ms_uOAKLEY2_GENERATOR;

//M5T_INTERNAL_USE_BEGIN
    // Note: DH groups 3 and 4 are for (currently unsupported) EC2N
//M5T_INTERNAL_USE_END

    // Summary:
    //  Oakley group 5 1536-bit prime from RFC 3526:  2^1536 - 2^1472 - 1 + 2^64 * ( (2^1406 pi] + 741804)
    static const uint8_t ms_auOAKLEY5_PRIME[192];

    // Summary:
    //  Oakley group 5 generator: 2.
    static const unsigned int ms_uOAKLEY5_GENERATOR;

//M5T_INTERNAL_USE_BEGIN
    // Note: DH groups 6 to 13 are for (currently unsupported) EC2N
//M5T_INTERNAL_USE_END

    // Summary:
    //  Oakley group 14 2048-bit prime from RFC 3526: 2^2048 - 2^1984 - 1 + 2^64 * ( (2^1918 pi] + 124476)
    static const uint8_t ms_auOAKLEY14_PRIME[256];

    // Summary:
    //  Oakley group 14 generator: 2.
    static const unsigned int ms_uOAKLEY14_GENERATOR;

    // Summary:
    //  Oakley group 15 3072-bit prime number from RFC 3526: 2^3072 - 2^3008 - 1 + 2^64 * ( (2^2942 pi] + 1690314)
    static const uint8_t ms_auOAKLEY15_PRIME[384];

    // Summary:
    //  Oakley group 15 generator: 2.
    static const unsigned int ms_uOAKLEY15_GENERATOR;

    // Summary:
    //  Oakley group 16 4096-bit prime number from RFC 3526: 2^4096 - 2^4032 - 1 + 2^64 * ( (2^3966 pi] + 240904)
    static const uint8_t ms_auOAKLEY16_PRIME[512];

    // Summary:
    //  Oakley group 16 generator: 2.
    static const unsigned int ms_uOAKLEY16_GENERATOR;

    // Summary:
    //  Oakley group 17 6144-bit prime number from RFC 3526: 2^6144 - 2^6080 - 1 + 2^64 * ( (2^6014 pi] + 929484)
    static const uint8_t ms_auOAKLEY17_PRIME[768];

    // Summary:
    //  Oakley group 17 generator: 2.
    static const unsigned int ms_uOAKLEY17_GENERATOR;

    // Summary:
    //  Oakley group 18 8192-bit prime number from RFC 3526: 2^8192 - 2^8128 - 1 + 2^64 * ( (2^8062 pi] + 4743158)
    static const uint8_t ms_auOAKLEY18_PRIME[1024];

    // Summary:
    //  Oakley group 18 generator: 2.
    static const unsigned int ms_uOAKLEY18_GENERATOR;

//M5T_INTERNAL_USE_BEGIN
    // Note: DH groups 19 to 21 are for (currently unsupported) ECP.
//M5T_INTERNAL_USE_END

// Published Interface.
public:

    //-- << Constructors / Destructors / Operators >>.

    //==========================================================================
    //==
    //==    CDiffieHellman
    //==
    //==========================================================================
    //
    //    Summary:
    //        Default constructor.
    //
    //    Description:
    //        Default constructor.
    //
    //==========================================================================
    CDiffieHellman();

    //==========================================================================
    //==
    //==    CDiffieHellman
    //==
    //==========================================================================
    //
    //    Summary:
    //        Copy constructor.
    //
    //  Parameters:
    //      pDiffieHellman:
    //          Pointer to a CDiffieHellman class.
    //
    //    Description:
    //        Copy constructor.
    //
    //==========================================================================
    CDiffieHellman(IN const CDiffieHellman* pDiffieHellman);

    //==========================================================================
    //==
    //==    CDiffieHellman
    //==
    //==========================================================================
    //
    //    Summary:
    //        Copy constructor.
    //
    //  Parameters:
    //      rDiffieHellman:
    //          Reference to a CDiffieHellman class.
    //
    //    Description:
    //        Copy constructor.
    //
    //==========================================================================
    CDiffieHellman(IN const CDiffieHellman& rDiffieHellman);

    //==========================================================================
    //==
    //==    ~CDiffieHellman
    //==
    //==========================================================================
    //
    //    Summary:
    //        Destructor.
    //
    //    Description:
    //        Default destructor.
    //
    //==========================================================================
    virtual ~CDiffieHellman();

    //==========================================================================
    //==
    //==    operator=
    //==
    //==========================================================================
    //
    //  Summary:
    //      Copies the referenced class into this one.
    //
    //  Parameters:
    //      rDiffieHellman:
    //          Reference to a CDiffieHellman class.
    //
    //  Returns:
    //      Copied CDiffieHellman class.
    //
    //  Description:
    //      Copies the reference to this.
    //
    //==========================================================================
    CDiffieHellman& operator=(IN const CDiffieHellman& rDiffieHellman);

    //==========================================================================
    //==
    //==   GeneratePrime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates a prime number.
    //
    //  Parameters:
    //      uPrimeSizeInBits:
    //          Size of the prime number in bits. Default is 1024, in order to
    //          comply with the FIPS standard.
    //
    //      uGenerator:
    //          Value of the generator for the key. Default is 2.
    //
    //  Returns:
    //      - resS_OK: the prime has been successfully generated.
    //
    //      - resFE_FAIL: the prime generation failed.
    //
    //      - resFE_NOT_IMPLEMENTED: the prime generation is not supported in
    //      this configuration.
    //
    //  Description:
    //      Generates a prime p serving as the modulus in the modular
    //      exponentiation.
    //
    //==========================================================================
    mxt_result GeneratePrime(IN unsigned int uPrimeSizeInBits = 1024,
                             IN unsigned int uGenerator = 2);

    //==========================================================================
    //==
    //==   GeneratePublicAndPrivateKeys
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates a private and a public key.
    //
    //  Parameters:
    //      uPrivateKeySizeInBits:
    //          Size of the private key in bits. Default is zero, meaning the
    //          private key will be the same size as the prime p.
    //
    //  Returns:
    //      - resS_OK: the keys have been successfully generated.
    //
    //      - resFE_FAIL: the generation failed or mandatory parameters are
    //      missing for the generation.
    //
    //  Description:
    //      Generates a random private key x, then a public key X, by applying
    //      the formula X = g^x mod p, using the gnerator g and the prime p.
    //
    //      Note: As covered in [RES], section 6.16, smaller private keys may be
    //      used in some scenarios, without threatening the security of the
    //      protocol. Hence, when uPrivateKeySizeInBits is non-zero, it will
    //      dictate the private key size.
    //
    //  See also:
    //      [RES] E. Rescorla, "SSL and TLS: Designing and Building Secure
    //      Systems", Addison-Wesley Professional, 2000, 499 pp.
    //
    //==========================================================================
    mxt_result GeneratePublicAndPrivateKeys(IN unsigned int uPrivateKeySizeInBits = 0);

    //==========================================================================
    //==
    //==   GenerateSharedKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates the shared key.
    //
    //  Parameters:
    //      puRemotePublicKey:
    //          The remote public key (e.g. the key Y).
    //
    //      uRemotePublicKeySize:
    //          The size of the key in bytes (the actual byte array length).
    //
    //  Returns:
    //      - resS_OK: the shared key has been successfully generated.
    //
    //      - resFE_FAIL: the generation failed.
    //
    //      - resFE_INVALID_ARGUMENT: either puRemotePublicKey is NULL or
    //      uRemotePublicKeySize is zero.
    //
    //  Description:
    //      Let x be the private key, p the prime and Y the remote public key.
    //      It generates the shared key K, using the formula K = Y^x mod p.
    //
    //==========================================================================
    mxt_result GenerateSharedKey(IN const uint8_t* puRemotePublicKey,
                                 IN unsigned int uRemotePublicKeySize);

    //==========================================================================
    //==
    //==   GenerateSharedKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates the shared key.
    //
    //  Parameters:
    //      pblobRemotePublicKey:
    //          The remote public key (e.g. the key Y).
    //
    //  Returns:
    //      - resS_OK: the shared key has been successfully generated.
    //
    //      - resFE_FAIL: the generation failed.
    //
    //      - resFE_INVALID_ARGUMENT: either pblobRemotePublicKey is NULL.
    //
    //  Description:
    //      Let x be the private key, p the prime and Y the remote public key.
    //      It generates the shared key K, using the formula K = Y^x mod p.
    //
    //==========================================================================
    mxt_result GenerateSharedKey(IN const CBlob* pblobRemotePublicKey);

    //==========================================================================
    //==
    //==   GetGenerator
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the generator.
    //
    //  Parameters:
    //      puGenerator:
    //          unsigned integer to store the generator value.
    //
    //  Returns:
    //      - resS_OK: the generator has been retrieved.
    //
    //      - resFE_FAIL: the generator has not yet been set.
    //
    //      - resFE_INVALID_ARGUMENT: puGenerator is NULL.
    //
    //  Description:
    //      Retrieves the generator g.
    //
    //==========================================================================
    mxt_result GetGenerator(OUT unsigned int* puGenerator) const;

    //==========================================================================
    //==
    //==   GetPrime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the prime number.
    //
    //  Parameters:
    //      uMaxSize:
    //          The maximum capacity of puPrime.
    //
    //      puPrime:
    //          The array used to store the prime.
    //
    //      puPrimeSize:
    //          The size of the prime in bytes.
    //
    //  Returns:
    //      - resS_OK: the prime has been retrieved.
    //
    //      - resFE_FAIL: the prime has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: puPrimeSize is NULL.
    //
    //  Description:
    //      Retrieves the prime p. If puPrime is NULL, the method returns the
    //      size of the prime in bytes in puPrimeSize.
    //
    //==========================================================================
    mxt_result GetPrime(IN unsigned int uMaxSize,
                        OUT uint8_t* puPrime,
                        OUT unsigned int* puPrimeSize) const;

    //==========================================================================
    //==
    //==   GetPrime
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the prime number.
    //
    //  Parameters:
    //      pblobPrime
    //          The blob used to store the prime.
    //
    //  Returns:
    //      - resS_OK: the prime has been retrieved.
    //
    //      - resFE_FAIL: the prime has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: pblobPrime is NULL.
    //
    //  Description:
    //      Retrieves the prime p.
    //
    //==========================================================================
    mxt_result GetPrime(OUT CBlob* pblobPrime) const;

    //==========================================================================
    //==
    //==   GetPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the public key.
    //
    //  Parameters:
    //      uMaxSize:
    //          The maximum capacity of puPublicKey.
    //
    //      puPublicKey:
    //          The array used to store the public key.
    //
    //      puPublicKeySize:
    //          The size of the public key in bytes.
    //
    //  Returns:
    //      - resS_OK: the public key has been retrieved.
    //
    //      - resFE_FAIL: the public key has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: puPublicKeySize is NULL.
    //
    //  Description:
    //      Retrieves the public key that must be forwarded to the peer.
    //      If puPublicKey is NULL, the method returns the size of the prime
    //      in bytes in puPublicKeySize.
    //
    //==========================================================================
    mxt_result GetPublicKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puPublicKey,
                            OUT unsigned int* puPublicKeySize) const;

    //==========================================================================
    //==
    //==   GetPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the public key.
    //
    //  Parameters:
    //      pblobPublicKey:
    //          The blob used to store the public key.
    //
    //  Returns:
    //      - resS_OK: the public key has been retrieved.
    //
    //      - resFE_FAIL: the public key has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: pblobPublicKey is NULL.
    //
    //  Description:
    //      Retrieves the public key that must be forwarded to the peer.
    //
    //==========================================================================
    mxt_result GetPublicKey(OUT CBlob* pblobPublicKey) const;

    //==========================================================================
    //==
    //==   GetPrivateKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the private key.
    //
    //  Parameters:
    //      uMaxSize:
    //          The maximum capacity of puPublicKey.
    //
    //      puPrivateKey:
    //          The array used to store the public key.
    //
    //      puPrivateKeySize:
    //          The size of the public key in bytes.
    //
    //  Returns:
    //      - resS_OK: the private key has been retrieved.
    //
    //      - resFE_FAIL: the private key has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: puPrivateKeySize is NULL.
    //
    //  Description:
    //      Retrieves the private key used to generate the public key. If
    //      puPrivateKey is NULL, the method returns the size of the prime in
    //      bytes in puPrivateKeySize.
    //
    //      Note: this key must remain secret to ensure the security of the
    //      protocol.
    //
    //==========================================================================
    mxt_result GetPrivateKey(IN unsigned int uMaxSize,
                             OUT uint8_t* puPrivateKey,
                             OUT unsigned int* puPrivateKeySize) const;

    //==========================================================================
    //==
    //==   GetPrivateKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the private key.
    //
    //  Parameters:
    //      pblobPrivateKey:
    //          The blob used to store the public key.
    //
    //  Returns:
    //      - resS_OK: the private key has been retrieved.
    //
    //      - resFE_FAIL: the private key has not yet been set or generated.
    //
    //      - resFE_INVALID_ARGUMENT: pblobPrivateKey is NULL.
    //
    //  Description:
    //      Retrieves the private key used to generate the public key.
    //
    //      Note: this key must remain secret to ensure the security of the
    //      protocol.
    //
    //==========================================================================
    mxt_result GetPrivateKey(OUT CBlob* pblobPrivateKey) const;

    //==========================================================================
    //==
    //==   GetSharedKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the shared key.
    //
    //  Parameters:
    //      uMaxSize:
    //          The maximum capacity of puSharedKey.
    //
    //      puSharedKey:
    //          The array used to store the shared key.
    //
    //      puSharedKeySize:
    //          The size of the shared key in bytes.
    //
    //  Returns:
    //      - resS_OK: the shared key has been retrieved.
    //
    //      - resFE_FAIL: the shared key has not yet been generated.
    //
    //      - resFE_INVALID_ARGUMENT: puSharedKeySize is NULL.
    //
    //  Description:
    //      Retrieves the shared key. If puSharedKey is NULL, the method returns
    //      the size of the prime in bytes in puPrimeSize.
    //
    //==========================================================================
    mxt_result GetSharedKey(IN unsigned int uMaxSize,
                            OUT uint8_t* puSharedKey,
                            OUT unsigned int* puSharedKeySize) const;

    //==========================================================================
    //==
    //==   GetSharedKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the shared key.
    //
    //  Parameters:
    //      pblobSharedKey:
    //          The blob used to store the shared key.
    //
    //  Returns:
    //      - resS_OK: the shared key has been retrieved.
    //
    //      - resFE_FAIL: the shared key has not yet been generated.
    //
    //      - resFE_INVALID_ARGUMENT: pblobSharedKey is NULL.
    //
    //  Description:
    //      Retrieves the shared key.
    //
    //==========================================================================
    mxt_result GetSharedKey(OUT CBlob* pblobSharedKey) const;

    //==========================================================================
    //==
    //==   SetParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the parameters used for generating keys.
    //
    //  Parameters:
    //      puPrime:
    //          The prime modulus.
    //
    //      uPrimeSize:
    //          The size of the prime in bytes.
    //
    //      uGenerator:
    //          The generator. It must be greater or equal to 2.
    //
    //      puPublicKey:
    //          The public key.
    //
    //      uPublicKeySize:
    //          The size of the public key in bytes.
    //
    //      puPrivateKey:
    //          The rivate key.
    //
    //      uPrivateKeySize:
    //          The size of the private key in bytes.
    //
    //  Returns:
    //      - resS_OK: the parameters has successfully been set.
    //
    //      - resFE_FAIL: failed to set the parameters.
    //
    //      - resFE_INVALID_ARGUMENT: there is a missing mandatory parameter
    //      (depending on the context), or their values are invalid.
    //
    //  Description:
    //      Sets the prime, the size of the prime in bytes, the generator, the
    //      public key, the size of the public key, the private key, and the
    //      size of the private key for use in the GeneratePublicAndPrivateKeys
    //      and GenerateSharedKey methods. If the public key or the private key
    //      parameters are set, then the other parameters must also be set for
    //      the shared key generation to work.
    //
    //      Note: GeneratePublicAndPrivateKeys destroys the public and private
    //      keys if they are set.
    //
    //==========================================================================
    mxt_result SetParameters(IN const uint8_t* puPrime,
                             IN unsigned int uPrimeSize,
                             IN unsigned int uGenerator,
                             IN const uint8_t* puPublicKey,
                             IN unsigned int uPublicKeySize,
                             IN const uint8_t* puPrivateKey,
                             IN unsigned int uPrivateKeySize);

    //==========================================================================
    //==
    //==   SetParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the parameters used for generating keys.
    //
    //  Parameters:
    //      pblobPrime:
    //          The prime modulus.
    //
    //      uGenerator:
    //          The generator. It must be greater or equal to 2.
    //
    //      pblobPublicKey:
    //          The public key.
    //
    //      pblobPrivateKey:
    //          The private key.
    //
    //  Returns:
    //      - resS_OK: the parameters has successfully been set.
    //
    //      - resFE_FAIL: failed to set the parameters.
    //
    //      - resFE_INVALID_ARGUMENT: there is a missing mandatory parameter
    //      (depending on the context), or their values are invalid.
    //
    //  Description:
    //      Sets the prime, the generator, the public key, and the private key
    //      for use in the GeneratePublicAndPrivateKeys and GenerateSharedKey
    //      methods. If either the public key or the private key is set, then
    //      the other must also be set for the shared key generation to work.
    //
    //      Note: GeneratePublicAndPrivateKeys destroys the public and private
    //      keys if they are set.
    //
    //==========================================================================
    mxt_result SetParameters(IN const CBlob* pblobPrime,
                             IN unsigned int uGenerator,
                             IN const CBlob* pblobPublicKey,
                             IN const CBlob* pblobPrivateKey);
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
CDiffieHellman::CDiffieHellman()
 : MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME()
{
}

inline
CDiffieHellman::CDiffieHellman(IN const CDiffieHellman* pDiffieHellman)
 : MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME()
{
    if (pDiffieHellman)
    {
        *this = *pDiffieHellman;
    }
}

inline
CDiffieHellman::CDiffieHellman(IN const CDiffieHellman& rDiffieHellman)
 : MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME()
{
    *this = rDiffieHellman;
}

inline
CDiffieHellman::~CDiffieHellman()
{
}

inline
CDiffieHellman& CDiffieHellman::operator=(IN const CDiffieHellman& rDiffieHellman)
{
    return static_cast<CDiffieHellman&>(MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::operator=(rDiffieHellman));
}

inline
mxt_result CDiffieHellman::GeneratePrime(IN unsigned int uPrimeSizeInBits,
                                         IN unsigned int uGenerator)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GeneratePrime(uPrimeSizeInBits,
                                                             uGenerator);
}

inline
mxt_result CDiffieHellman::GeneratePublicAndPrivateKeys(IN unsigned int uPrivateKeyLength)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GeneratePublicAndPrivateKeys(uPrivateKeyLength);
}

inline
mxt_result CDiffieHellman::GenerateSharedKey(IN const uint8_t* puRemotePublicKey,
                                             IN unsigned int uRemotePublicKeySize)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GenerateSharedKey(puRemotePublicKey,
                                                                 uRemotePublicKeySize);
}

inline
mxt_result CDiffieHellman::GenerateSharedKey(IN const CBlob* pblobRemotePublicKey)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GenerateSharedKey(pblobRemotePublicKey);
}

inline
mxt_result CDiffieHellman::GetGenerator(OUT unsigned int* puGenerator) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetGenerator(puGenerator);
}

inline
mxt_result CDiffieHellman::GetPrime(IN unsigned int uMaxSize,
                                    OUT uint8_t* puPrime,
                                    OUT unsigned int* puPrimeSize) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPrime(uMaxSize,
                                                        puPrime,
                                                        puPrimeSize);
}

inline
mxt_result CDiffieHellman::GetPrime(OUT CBlob* pblobPrime) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPrime(pblobPrime);
}

inline
mxt_result CDiffieHellman::GetPublicKey(IN unsigned int uMaxSize,
                                        OUT uint8_t* puPublicKey,
                                        OUT unsigned int* puPublicKeySize) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPublicKey(uMaxSize,
                                                            puPublicKey,
                                                            puPublicKeySize);
}

inline
mxt_result CDiffieHellman::GetPublicKey(OUT CBlob* pblobPublicKey) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPublicKey(pblobPublicKey);
}

inline
mxt_result CDiffieHellman::GetPrivateKey(IN unsigned int uMaxSize,
                                         OUT uint8_t* puPrivateKey,
                                         OUT unsigned int* puPrivateKeySize) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPrivateKey(uMaxSize,
                                                             puPrivateKey,
                                                             puPrivateKeySize);
}

inline
mxt_result CDiffieHellman::GetPrivateKey(OUT CBlob* pblobPrivateKey) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetPrivateKey(pblobPrivateKey);
}

inline
mxt_result CDiffieHellman::GetSharedKey(IN unsigned int uMaxSize,
                                        OUT uint8_t* puSharedKey,
                                        OUT unsigned int* puSharedKeySize) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetSharedKey(uMaxSize,
                                                            puSharedKey,
                                                            puSharedKeySize);
}

inline
mxt_result CDiffieHellman::GetSharedKey(OUT CBlob* pblobSharedKey) const
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::GetSharedKey(pblobSharedKey);
}

inline
mxt_result CDiffieHellman::SetParameters(IN const uint8_t* puPrime,
                                         IN unsigned int uPrimeSize,
                                         IN unsigned int uGenerator,
                                         IN const uint8_t* puPublicKey,
                                         IN unsigned int uPublicKeySize,
                                         IN const uint8_t* puPrivateKey,
                                         IN unsigned int uPrivateKeySize)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::SetParameters(puPrime,
                                                             uPrimeSize,
                                                             uGenerator,
                                                             puPublicKey,
                                                             uPublicKeySize,
                                                             puPrivateKey,
                                                             uPrivateKeySize);
}

inline
mxt_result CDiffieHellman::SetParameters(IN const CBlob* pblobPrime,
                                         IN unsigned int uGenerator,
                                         IN const CBlob* pblobPublicKey,
                                         IN const CBlob* pblobPrivateKey)
{
    return MXD_CRYPTO_DIFFIEHELLMAN_CLASSNAME::SetParameters(pblobPrime,
                                                             uGenerator,
                                                             pblobPublicKey,
                                                             pblobPrivateKey);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_DIFFIEHELLMAN_NONE)

#endif // MXG_CDIFFIEHELLMAN_H


