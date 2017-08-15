//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CRSA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CRSA_H
//M5T_INTERNAL_USE_END

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

#if !defined(MXD_CRYPTO_RSA_NONE)

#if defined(MXD_CRYPTO_RSA_INCLUDE)
    #include MXD_CRYPTO_RSA_INCLUDE
#else
    #error "No implementation defined for CRsa!!!"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
    #include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#ifndef MXG_CHASH_H
    #include "Crypto/CHash.h"
#endif

//-- Parameters used by Value
//----------------------------

//-- Interface Realized and/or Parent
//------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CBlob;
class CPublicKey;
class CPrivateKey;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CRsa
//==============================================================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//  Class that manages the RSA algorithm.
//
// Description:
//  CRsa is the class that manages RSA encryption, decryption, signature,
//  signature verification, and key management.
//
//  RSA:
//      RSA is a public key algorithm used for data encryption and digital
//      signatures.
//
//      The encryption of a message with RSA is a very costly process and is
//      many orders of magnitude slower than symmetric encryption. For this
//      reason, RSA is used for sharing the keys used in symmetric algorithms
//      as both users must be in possession of a common key to use these
//      algorithms.
//
//      It is also used to digitally sign a message as anybody can authenticate
//      that a user has signed a document using his private key.
//
//      A RSA private/public key pair is generated in the following way:
//          A user chooses an exponent e to be used. This exponent can be
//          anything but some are better suited as they speed up calculations.
//          Two random large primes p and q are then chosen and multiplied to
//          give the modulus n. e and n both form the public key.
//          e and (p - 1)(q - 1) must be relatively prime.
//
//          The private key d is then calculated: d = e^-1 (p - 1)(q - 1).
//
//          The two primes p and q should be discarded at this point as they
//          are of no more use once the public/private key pair has been
//          generated.
//
//      Key sharing:
//          Bob encrypts a key to be used in a symmetric cryptographic algorithm
//          with Alice using Alice's public key and sends it to Alice.
//          Only Alice can decrypt this message since she is the only one with
//          her private key. Once Alice has the shared key, she can communicate
//          with Bob using the new key for the symmetric algorithm. Bob obtains
//          Alice's public key either from a key broker or Alice's certificate.
//
//   <CODE>
//
//   Initiator                                    Acceptor
//   =========                                    ========
//                 <---Get acceptor public key--
//
//   PublicKeyEncrypt(PublicKey,
//                    MessageToEncrypt,
//                    PaddingType,
//                    EncryptedMessage)
//
//                 ---- EncryptedMessage ----->
//                                                PrivateKeyDecrypt(EncryptedMssage,
//                                                                  PaddingType,
//                                                                  DecryptedMessage)
//
//   </CODE>
//
//      Signature:
//          Using her private key, Alice encrypts a hashed message. She can
//          then send the signed hash and the hashed message to Bob, who can
//          verify that this message is really from Alice by decompressing the
//          encrypted message using Alice's public key and comparing it to the
//          hashed message. If both of these are the same, then the message hash
//          can only have been created by Alice. This uniquely identifies Alice
//          as the creator.
//
//   <CODE>
//
//   Initiator                                    Acceptor
//   =========                                    ========
//   Sign(MessageHash,
//        DigestType,
//        SignedMessage)
//
//                 -- EncryptedMessage, SignedMessage ->
//                 ------Get initiator public key ----->
//                                                Verify(PublicKey,
//                                                       EncryptedMssage,
//                                                       DigestType,
//                                                       MessageHash)
//
//   </CODE>
//
//
// Location:
//   Crypto/CRsa.h
//
//==EDOC========================================================================
class CRsa : public MXD_CRYPTO_RSA_CLASSNAME
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    //==SDOC====================================================================
    //==
    //==   CRsa
    //==
    //==========================================================================
    //
    //    Summary:
    //        Constructor.
    //
    //    Description:
    //        Default constructor.
    //
    //==EDOC====================================================================
    CRsa();

    //==SDOC====================================================================
    //==
    //==   ~CRsa
    //==
    //==========================================================================
    //
    //    Summary:
    //        Default destructor.
    //
    //    Description:
    //        Default destructor.
    //
    //==EDOC====================================================================
    virtual ~CRsa();

    //==SDOC====================================================================
    //==
    //==   GenerateKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Generates the private key and the product of two primes.
    //
    //  Parameters:
    //      uExponent:
    //          The exponent of the public key.
    //
    //      uKeySizeInBits:
    //          The size of the key to generate in number of bits.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method generates 2 large prime numbers randomly, p and q. Then,
    //      p and q are used to compute n, the product of 2 primes also referred
    //      to as the modulus. The exponent, e, supplied in uExponent together
    //      with the modulus, form the public key.
    //
    //      The private key, d, is then calculated using the following formula:
    //      d = e^-1 mod(p - 1)(q - 1).
    //
    //  Notes:
    //      The public exponent must be an odd large number as this greatly
    //      increases the chance that e and d are relatively prime. There are
    //      a few common exponents that are recommended by PKCS#1, PEM, and
    //      x.509. These are: 3, 17, and 65537. These numbers speed up the
    //      exponentiation operations.
    //
    //      The recommendation of these exponents explains why uExponent is
    //      defined as an unsigned integer even if it is specified the exponent
    //      may be a large number.
    //
    //==EDOC====================================================================
    mxt_result GenerateKey(IN unsigned int uExponent, IN unsigned int uKeySizeInBits);

    //==SDOC====================================================================
    //==
    //==   PublicKeyEncrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Encrypts the message with the public key.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a CPublicKey class containing the key to be used.
    //
    //      puMessage:
    //          Pointer to a byte array to encrypt using the public key.
    //
    //      uMessageSize:
    //          The size of the byte array to encrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      uEncryptedMessageCapacity:
    //          The capacity of the buffer supplied to store the encrypted
    //          message.
    //
    //      puEncryptedMessage:
    //          Pointer to a byte array to contain the encrypted bytes.
    //
    //      puEncryptedMessageSize:
    //          The size of the encrypted byte array.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method encrypts the message pointed by puMessage using the
    //      public key pointed by the pRemotePublicKey parameter. In the end,
    //      the encrypted message is stored at the location pointed by
    //      puEncryptedMessage. The integer pointed by puEncryptedMessageSize
    //      is updated to reflect the size of the encrypted message. The size
    //      does not exceed the capacity indicated by uEncryptedMessageCapacity.
    //      An error is returned if the capacity is too small.
    //
    //      If puEncryptedMessage is NULL, the encryption process does not occur.
    //      Instead, the value pointed by puEncryptedMessageSize is updated to
    //      reflect the maximum size required to store the encrypted message.
    //      This gives the caller the opportunity to properly allocate the
    //      buffer.
    //
    //      The parameter ePadding configures the type of padding used for the
    //      encryption with respect to EEncryptionPaddingAlgorithm. This padding
    //      reduces the space available to store the encrypted message. For each
    //      padding type, uMessageSize must be less than or equal to a certain
    //      amount:
    //          ePKCS1:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (public key size) minus 11 padding bytes.
    //
    //          ePKCS1_OAEP:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (public key size) minus 41 padding bytes.
    //
    //          eNOPADDING:
    //                  The message size must be equal to the size of the
    //                  modulus (public key size).
    //
    //==EDOC====================================================================
    mxt_result PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const uint8_t* puMessage,
                                IN unsigned int uMessageSize,
                                IN EEncryptionPaddingAlgorithm ePadding,
                                IN unsigned int uEncryptedMessageCapacity,
                                OUT uint8_t* puEncryptedMessage,
                                OUT unsigned int* puEncryptedMessageSize);

    //==SDOC====================================================================
    //==
    //==   PublicKeyEncrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Encrypts the message with the public key.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a CPublicKey class containing the key to be used.
    //
    //      pblobMessage:
    //          Pointer to a blob to encrypt using the public key.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      pblobEncryptedMessage:
    //          Pointer to a blob to contain the encrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method encrypts the message pointed by pblobMessage using the
    //      public key pointed by the pRemotePublicKey parameter. In the end,
    //      the encrypted message and its size are stored inside a CBlob at the
    //      location pointed by pblobEncryptedMessage. If it is required, its
    //      capacity is increased to provide enough space to store the encrypted
    //      message.
    //
    //      The parameter ePadding configures the type of padding used for the
    //      encryption with respect to EEncryptionPaddingAlgorithm. This padding
    //      reduces the space available to store the encrypted message. For each
    //      padding type, uMessageSize must be less than or equal to a certain
    //      amount:
    //          ePKCS1:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (public key size) minus 11 padding bytes.
    //
    //          ePKCS1_OAEP:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (public key size) minus 41 padding bytes.
    //
    //          eNOPADDING:
    //                  The message size must be equal to the size of the
    //                  modulus (public key size).
    //
    //==EDOC====================================================================
    mxt_result PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const CBlob* pblobMessage,
                                IN EEncryptionPaddingAlgorithm ePadding,
                                OUT CBlob* pblobEncryptedMessage);

    //==SDOC====================================================================
    //==
    //==   PublicKeyDecrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Decrypts the message with the public key.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a CPublicKey class containing the key to be used.
    //
    //      puEncryptedMessage:
    //          Pointer to a byte array to decrypt using the public key.
    //
    //      uEncryptedMessageSize:
    //          The size of the byte array to decrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      uMessageCapacity:
    //          The capacity of the buffer supplied to receive the decrypted
    //          message.
    //
    //      puMessage:
    //          Pointer to an array to contain the decrypted bytes.
    //
    //      puMessageSize:
    //          The size of the decrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the message referred by puEncryptedMessage
    //      using the public key supplied with pRemotePublicKey. The successful
    //      decryption of a message with a public key confirms at the same time
    //      the authenticity of the source that encrypted the message.
    //
    //      In the end, the decrypted message is stored at the location pointed
    //      by puMessage. The integer pointed by puMessageSize is updated to
    //      reflect the size of the decrypted message. The size does not exceed
    //      the capacity indicated by uMessageCapacity. An error is returned if
    //      the capacity is too small.
    //
    //      If puMessage is NULL, the decryption process does not occur.
    //      Instead, the value pointed by puMessageSize is updated to reflect
    //      the maximum size required to store the decrypted message. This gives
    //      the caller the opportunity to properly allocate the buffer.
    //
    //      This method is also useful for signature verification.
    //
    //  See Also:
    //      Verify
    //
    //
    //==EDOC====================================================================
    mxt_result PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const uint8_t* puEncryptedMessage,
                                IN unsigned int uEncryptedMessageSize,
                                IN ESignaturePaddingAlgorithm ePadding,
                                IN unsigned int uMessageCapacity,
                                OUT uint8_t* puMessage,
                                OUT unsigned int* puMessageSize);

    //==SDOC====================================================================
    //==
    //==   PublicKeyDecrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Decrypts the message with the public key.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a CPublicKey class containing the key to be used.
    //
    //      pblobEncryptedMessage:
    //          Blob to decrypt using the public key.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      pblobMessage:
    //          Blob to contain the decrypted bytes.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the message referred by pblobEncryptedMessage
    //      using the public key supplied with pRemotePublicKey. The successful
    //      decryption of a message with a public key confirms at the same time
    //      the authenticity of the source that encrypted the message.
    //
    //      In the end, the decrypted message and its size are stored inside a
    //      CBlob at the location pointed by pblobMessage. If it is required,
    //      its capacity is increased to provide enough space to store the
    //      decrypted message.
    //
    //      This method is also useful for signature verification.
    //
    //  See Also:
    //      Verify
    //
    //==EDOC====================================================================
    mxt_result PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const CBlob* pblobEncryptedMessage,
                                IN ESignaturePaddingAlgorithm ePadding,
                                OUT CBlob* pblobMessage);

    //==SDOC====================================================================
    //==
    //==   PrivateKeyEncrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Encrypts the message with the private key.
    //
    //  Parameters:
    //      puMessage:
    //          Pointer to a byte array containing the message to encrypt.
    //
    //      uMessageSize:
    //          The size of the message to encrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      uEncryptedMessageCapacity:
    //          The capacity of the buffer supplied to store encrypted message.
    //
    //      puEncryptedMessage:
    //          Pointer to a byte array to contain the encrypted message.
    //
    //      puEncryptedMessageSize:
    //          The size of the encrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method encrypts the message pointed by puMessage using the
    //      private key stored in this instance of CRsa. The encryption of a
    //      message with a private key confirms at the same time the identity
    //      of the source to anyone who decrypts its message successfully with
    //      the public key.
    //
    //      In the end, the encrypted message is stored at the location pointed
    //      by puEncryptedMessage. The integer pointed by puEncryptedMessageSize
    //      is updated to reflect the size of the encrypted message. The size
    //      does not exceed the capacity indicated by uEncryptedMessageCapacity.
    //      An error is returned if the capacity is too small.
    //
    //      The parameter ePadding configures the type of padding used for the
    //      encryption with respect to ESignaturePaddingAlgorithm. This padding
    //      reduces the space available to store the encrypted message. For each
    //      padding type, uMessageSize must be less than or equal to a certain
    //      amount:
    //          ePKCS1:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (private key size) minus 11 padding bytes.
    //
    //          eNOPADDING:
    //                  The message size must be equal to the size of the
    //                  modulus (private key size).
    //
    //      If puEncryptedMessage is NULL, the encryption process does not occur.
    //      Instead, the value pointed by puEncryptedMessageSize is updated to
    //      reflect the maximum size required to store the encrypted message.
    //      This gives the caller the opportunity to properly allocate the
    //      buffer.
    //
    //      This method is also useful for signing a message.
    //
    //==EDOC====================================================================
    mxt_result PrivateKeyEncrypt(IN const uint8_t* puMessage,
                                 IN unsigned int uMessageSize,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 IN unsigned int uEncryptedMessageCapacity,
                                 OUT uint8_t* puEncryptedMessage,
                                 OUT unsigned int* puEncryptedMessageSize);

    //==SDOC====================================================================
    //==
    //==   PrivateKeyEncrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Encrypts the message with the private key.
    //
    //  Parameters:
    //      pblobMessage:
    //          Pointer to a blob containing the message to encrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      pblobEncryptedMessage:
    //          Pointer to a blob to contain the encrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method encrypts the message pointed by pblobMessage using the
    //      private key stored in this instance of CRsa. The encryption of a
    //      message with a private key confirms at the same time the identity
    //      of the source to anyone who decrypts its message successfully with
    //      the public key.
    //
    //      In the end, the encrypted message and its size are stored inside a
    //      CBlob at the location pointed by pblobEncryptedMessage. If it is
    //      required, its capacity is increased to provide enough space to store
    //      the encrypted message.
    //
    //      The parameter ePadding configures the type of padding used for the
    //      encryption with respect to ESignaturePaddingAlgorithm. This padding
    //      reduces the space available to store the encrypted message. For each
    //      padding type, uMessageSize must be less than or equal to a certain
    //      amount:
    //          ePKCS1:
    //                  The message size must be less than or equal to the size
    //                  of the modulus (private key size) minus 11 padding bytes.
    //
    //          eNOPADDING:
    //                  The message size must be equal to the size of the
    //                  modulus (private key size).
    //
    //      This method is also useful for generating signature.
    //
    //==EDOC====================================================================
    mxt_result PrivateKeyEncrypt(IN const CBlob* pblobMessage,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 OUT CBlob* pblobEncryptedMessage);

    //==SDOC====================================================================
    //==
    //==   PrivateKeyDecrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Decrypts the message with the private key.
    //
    //  Parameters:
    //      puEncryptedMessage:
    //          Pointer to a byte array containing the message to decrypt.
    //
    //      uEncryptedMessageSize:
    //          The size of the message to decrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      uMessageCapacity:
    //          The capacity of the array to receive decrypted message.
    //
    //      puMessage:
    //          Pointer to a byte array to contain the decrypted message.
    //
    //      puMessageSize:
    //          The size of the decrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the message referred by puEncryptedMessage
    //      using the private key stored in this CRsa instance.
    //
    //      In the end, the decrypted message is stored at the location pointed
    //      by puMessage. The integer pointed by puMessageSize is updated to
    //      reflect the size of the decrypted message. The size does not exceed
    //      the capacity indicated by uMessageCapacity. An error is returned if
    //      the capacity is too small.
    //
    //      If puMessage is NULL, the decryption process does not occur.
    //      Instead, the value pointed by puMessageSize is updated to reflect
    //      the maximum size required to store the decrypted message. This gives
    //      the caller the opportunity to properly allocate the buffer.
    //
    //==EDOC====================================================================
    mxt_result PrivateKeyDecrypt(IN const uint8_t* puEncryptedMessage,
                                 IN unsigned int uEncryptedMessageSize,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 IN unsigned int uMessageCapacity,
                                 OUT uint8_t* puMessage,
                                 OUT unsigned int* puMessageSize);

    //==SDOC====================================================================
    //==
    //==   PrivateKeyDecrypt
    //==
    //==========================================================================
    //
    //  Summary:
    //      Decrypts the message with the private key.
    //
    //  Parameters:
    //      pblobEncryptedMessage:
    //          Pointer to a blob containing the message to decrypt.
    //
    //      ePadding:
    //          The type of padding used on the message.
    //
    //      pblobMessage:
    //          Pointer to a blob to contain the decrypted message.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the message referred by pblobEncryptedMessage
    //      using the private key stored inside this CRsa instance.
    //
    //      In the end, the decrypted message and its size are stored inside a
    //      CBlob at the location pointed by pblobMessage. If it is required,
    //      its capacity is increased to provide enough space to store the
    //      decrypted message.
    //
    //==EDOC====================================================================
    mxt_result PrivateKeyDecrypt(IN const CBlob* pblobEncryptedMessage,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 OUT CBlob* pblobMessage);

    //==SDOC====================================================================
    //==
    //==   Sign
    //==
    //==========================================================================
    //
    //  Summary:
    //      Signs the hash of a message.
    //
    //  Parameters:
    //      puHash:
    //          Pointer to a byte array containing the hash to sign.
    //
    //      uHashSize:
    //          The size of the hash to sign.
    //
    //      eAlgorithm:
    //          Hashing algorithm used to generate the hash.
    //
    //      uSignatureCapacity:
    //          The capacity of the array to receive signature.
    //
    //      puSignature:
    //          Pointer to a byte array to contain the signature.
    //
    //      puSignatureSize:
    //          The size of the signature.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method signs the hash pointed by puHash with the private key
    //      stored inside this CRsa instance.
    //
    //      In the end, the hash signature is stored at the location pointed
    //      by puSignature. The integer pointed by puSignatureSize is updated to
    //      reflect the size of the signature. The size does not exceed the
    //      capacity indicated by uSignatureCapacity. An error is returned if
    //      the capacity is too small.
    //
    //      If puSignature is NULL, the signature process does not occur.
    //      Instead, the value pointed by puSignatureSize is updated to reflect
    //      the maximum size required to store the signature. It gives the
    //      caller the opportunity to properly allocate the buffer.
    //
    //      The parameter eHashAlgorithm indicates the hash algorithm used
    //      to generate puHash with respect to CHash::EAlgorithm.
    //
    //==EDOC====================================================================
    mxt_result Sign(IN const uint8_t* puHash,
                    IN unsigned int uHashSize,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    IN unsigned int uSignatureCapacity,
                    OUT uint8_t* puSignature,
                    OUT unsigned int* puSignatureSize);

    //==SDOC====================================================================
    //==
    //==   Sign
    //==
    //==========================================================================
    //
    //  Summary:
    //      Signs the hash of a message.
    //
    //  Parameters:
    //      pblobHash:
    //          Pointer to a blob containing the hash to sign.
    //
    //      eAlgorithm:
    //          Hash algorithm used to generate the hash.
    //
    //      pblobSignature:
    //          Pointer to a blob to contain the signature.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method signs the hash pointed by pblobHash with the private
    //      key stored inside this CRsa instance.
    //
    //      In the end, a CBlob pointed by pblobSignature is updated to contain
    //      the signature and its corresponding size. If required, the capacity
    //      of the object pointed by pblobSignature is increased so there is
    //      enough space to hold the signature.
    //
    //      The parameter eHashAlgorithm indicates the hash algorithm used to
    //      generate pblobHash with respect to CHash::EAlgorithm.
    //
    //==EDOC====================================================================
    mxt_result Sign(IN const CBlob* pblobHash,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    OUT CBlob* pblobSignature);

    //==SDOC====================================================================
    //==
    //==   Verify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies a signature.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a blob containing the remote public key.
    //
    //      puSignature:
    //          Pointer to a byte array containing the signature to verify.
    //
    //      uSignatureSize:
    //          The size of the signature to verify.
    //
    //      puHash:
    //          Pointer to a byte array containing the original hash.
    //
    //      uHashSize:
    //          The size of the hash to compare with the signature.
    //
    //      eAlgorithm:
    //          Hash algorithm used.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the signatures pointed by puSignature with the
    //      public key supplied by pRemotePublicKey and verifies that the
    //      resulting hash corresponds to the hash referred by puHash.
    //
    //      The parameter eHashAlgorithm is also compared against the hash
    //      algorithm used to generate the signature.
    //
    //==EDOC====================================================================
    mxt_result Verify(IN const CPublicKey* pRemotePublicKey,
                      IN const uint8_t* puSignature,
                      IN unsigned int uSignatureSize,
                      IN const uint8_t* puHash,
                      IN unsigned int uHashSize,
                      IN CHash::EAlgorithm eHashAlgorithm);

    //==SDOC====================================================================
    //==
    //==   Verify
    //==
    //==========================================================================
    //
    //  Summary:
    //      Verifies a signature.
    //
    //  Parameters:
    //      pRemotePublicKey:
    //          Pointer to a CPublicKey containing the remote key.
    //
    //      pblobSignature:
    //          Pointer to a blob containing the signed messge.
    //
    //      pblobHash:
    //          Pointer to a blob containing the original hash.
    //
    //      eAlgorithm:
    //          Hash algorithm used.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method decrypts the signatures pointed by pblobSignature with
    //      the public key supplied by pRemotePublicKey and verifies that the
    //      resulting hash corresponds to the hash referred by pblobHash.
    //
    //      The parameter eHashAlgorithm is also compared against the hash
    //      algorithm used to generate the signature.
    //
    //==EDOC====================================================================
    mxt_result Verify(IN const CPublicKey* pRemotePublicKey,
                      IN const CBlob* pblobSignature,
                      IN const CBlob* pblobHash,
                      IN CHash::EAlgorithm eHashAlgorithm);

    //==SDOC====================================================================
    //==
    //==   GetPublicKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the public key.
    //
    //  Parameters:
    //      pPublicKey:
    //          Pointer to a CPublicKey to contain the exponent.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method is used to get the public key currently associated with
    //      this CRsa instance. The key is returned in the form of a CPublicKey
    //      class for abstraction.
    //
    //  Notes:
    //      The key returned in CPublicKey is valid as long as the m_pRsa
    //      is also valid. Changing the public key either in the CPublicKey or
    //      in the CRsa results in an unknown behaviour of both the CPublicKey
    //      and the CRsa.
    //
    //
    //==EDOC====================================================================
    mxt_result GetPublicKey(OUT CPublicKey* pPublicKey);

    //==SDOC====================================================================
    //==
    //==   GetPrivateKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the private key.
    //
    //  Parameters:
    //      pPrivateKey:
    //          Pointer to a CPrivateKey class to receive the private key.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method is used to get the private key currently associated with
    //      this CRsa instance. The key is returned in the form of a CPrivateKey
    //      class for abstraction.
    //
    //==EDOC====================================================================
    mxt_result GetPrivateKey(OUT CPrivateKey* pPrivateKey);

    //==SDOC====================================================================
    //==
    //==   SetKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the keys for RSA.
    //
    //  Parameters:
    //      pPrivateKey:
    //          Pointer to a CPrivateKey.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      This method sets the public and private keys for the current
    //      instance of CRsa. The key information supplied in the parameter
    //      pPrivateKey is used to determine the public key and the private key.
    //
    //      When this method is used, the private key was generated by other
    //      means and the object is configured to use that key.
    //
    //==EDOC====================================================================
    mxt_result SetKey(IN const CPrivateKey* pPrivateKey);
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CRsa::CRsa()
 : MXD_CRYPTO_RSA_CLASSNAME()
{
}

inline
CRsa::~CRsa()
{
}

inline
mxt_result CRsa::GenerateKey(IN unsigned int uExponent, IN unsigned int uKeySizeInBits)
{
    return MXD_CRYPTO_RSA_CLASSNAME::GenerateKey(uExponent, uKeySizeInBits);
}

inline
mxt_result CRsa::PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                  IN const uint8_t* puMessage,
                                  IN unsigned int uMessageSize,
                                  IN EEncryptionPaddingAlgorithm ePadding,
                                  IN unsigned int uEncryptedMessageCapacity,
                                  OUT uint8_t* puEncryptedMessage,
                                  OUT unsigned int* puEncryptedMessageSize)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PublicKeyEncrypt(pRemotePublicKey,
                                                      puMessage,
                                                      uMessageSize,
                                                      ePadding,
                                                      uEncryptedMessageCapacity,
                                                      puEncryptedMessage,
                                                      puEncryptedMessageSize);
}

inline
mxt_result CRsa::PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                  IN const CBlob* pblobMessage,
                                  IN EEncryptionPaddingAlgorithm ePadding,
                                  OUT CBlob* pblobEncryptedMessage)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PublicKeyEncrypt(pRemotePublicKey,
                                                      pblobMessage,
                                                      ePadding,
                                                      pblobEncryptedMessage);
}

inline
mxt_result CRsa::PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                  IN const uint8_t* puEncryptedMessage,
                                  IN unsigned int uEncryptedMessageSize,
                                  IN ESignaturePaddingAlgorithm ePadding,
                                  IN unsigned int uMessageCapacity,
                                  OUT uint8_t* puMessage,
                                  OUT unsigned int* puMessageSize)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PublicKeyDecrypt(pRemotePublicKey,
                                                      puEncryptedMessage,
                                                      uEncryptedMessageSize,
                                                      ePadding,
                                                      uMessageCapacity,
                                                      puMessage,
                                                      puMessageSize);
}

inline
mxt_result CRsa::PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                  IN const CBlob* pblobEncryptedMessage,
                                  IN ESignaturePaddingAlgorithm ePadding,
                                  OUT CBlob* pblobMessage)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PublicKeyDecrypt(pRemotePublicKey,
                                                      pblobEncryptedMessage,
                                                      ePadding,
                                                      pblobMessage);
}

inline
mxt_result CRsa::PrivateKeyEncrypt(IN const uint8_t* puMessage,
                                   IN unsigned int uMessageSize,
                                   IN ESignaturePaddingAlgorithm ePadding,
                                   IN unsigned int uEncryptedMessageCapacity,
                                   OUT uint8_t* puEncryptedMessage,
                                   OUT unsigned int* puEncryptedMessageSize)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PrivateKeyEncrypt(puMessage,
                                                       uMessageSize,
                                                       ePadding,
                                                       uEncryptedMessageCapacity,
                                                       puEncryptedMessage,
                                                       puEncryptedMessageSize);
}

inline
mxt_result CRsa::PrivateKeyEncrypt(IN const CBlob* pblobMessage,
                                   IN ESignaturePaddingAlgorithm ePadding,
                                   OUT CBlob* pblobEncryptedMessage)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PrivateKeyEncrypt(pblobMessage,
                                                       ePadding,
                                                       pblobEncryptedMessage);
}

inline
mxt_result CRsa::PrivateKeyDecrypt(IN const uint8_t* puEncryptedMessage,
                                   IN unsigned int uEncryptedMessageSize,
                                   IN EEncryptionPaddingAlgorithm ePadding,
                                   IN unsigned int uMessageCapacity,
                                   OUT uint8_t* puMessage,
                                   OUT unsigned int* puMessageSize)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PrivateKeyDecrypt(puEncryptedMessage,
                                                       uEncryptedMessageSize,
                                                       ePadding,
                                                       uMessageCapacity,
                                                       puMessage,
                                                       puMessageSize);
}

inline
mxt_result CRsa::PrivateKeyDecrypt(IN const CBlob* pblobEncryptedMessage,
                                   IN EEncryptionPaddingAlgorithm ePadding,
                                   OUT CBlob* pblobMessage)
{
    return MXD_CRYPTO_RSA_CLASSNAME::PrivateKeyDecrypt(pblobEncryptedMessage,
                                                       ePadding,
                                                       pblobMessage);
}

inline
mxt_result CRsa::Sign(IN const uint8_t* puHash,
                      IN unsigned int uHashSize,
                      IN CHash::EAlgorithm eHashAlgorithm,
                      IN unsigned int uSignatureCapacity,
                      OUT uint8_t* puSignature,
                      OUT unsigned int* puSignatureSize)
{
    return MXD_CRYPTO_RSA_CLASSNAME::Sign(puHash,
                                          uHashSize,
                                          eHashAlgorithm,
                                          uSignatureCapacity,
                                          puSignature,
                                          puSignatureSize);
}

inline
mxt_result CRsa::Sign(IN const CBlob* pblobHash,
                      IN CHash::EAlgorithm eHashAlgorithm,
                      OUT CBlob* pblobSignature)
{
    return MXD_CRYPTO_RSA_CLASSNAME::Sign(pblobHash,
                                          eHashAlgorithm,
                                          pblobSignature);
}

inline
mxt_result CRsa::Verify(IN const CPublicKey* pRemotePublicKey,
                        IN const uint8_t* puSignature,
                        IN unsigned int uSignatureSize,
                        IN const uint8_t* puHash,
                        IN unsigned int uHashSize,
                        IN CHash::EAlgorithm eHashAlgorithm)
{
    return MXD_CRYPTO_RSA_CLASSNAME::Verify(pRemotePublicKey,
                                            puSignature,
                                            uSignatureSize,
                                            puHash,
                                            uHashSize,
                                            eHashAlgorithm);
}

inline
mxt_result CRsa::Verify(IN const CPublicKey* pRemotePublicKey,
                        IN const CBlob* pblobSignature,
                        IN const CBlob* pblobHash,
                        IN CHash::EAlgorithm eHashAlgorithm)
{
    return MXD_CRYPTO_RSA_CLASSNAME::Verify(pRemotePublicKey,
                                            pblobSignature,
                                            pblobHash,
                                            eHashAlgorithm);
}

inline
mxt_result CRsa::GetPublicKey(OUT CPublicKey* pPublicKey)
{
    return MXD_CRYPTO_RSA_CLASSNAME::GetPublicKey(pPublicKey);
}

inline
mxt_result CRsa::GetPrivateKey(OUT CPrivateKey* pPrivateKey)
{
    return MXD_CRYPTO_RSA_CLASSNAME::GetPrivateKey(pPrivateKey);
}

inline
mxt_result CRsa::SetKey(IN const CPrivateKey* pPrivateKey)
{
    return MXD_CRYPTO_RSA_CLASSNAME::SetKey(pPrivateKey);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_RSA_NONE)

#endif //-- #ifndef MXG_CLASSNAME_H


