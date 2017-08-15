//==============================================================================
//==============================================================================
//
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CRSAMOCANASS_H
#define MXG_CRSAMOCANASS_H

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

#if defined(MXD_CRYPTO_RSA_MOCANA_SS)

//-- Data Members.
#ifndef MXG_CCRYPTO_H
    #include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#ifndef MXG_CHASH_H
    #include "Crypto/CHash.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations.
class CBlob;
class CPublicKey;
class CPrivateKey;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_CRYPTO_RSA_CLASSNAME
    #define MXD_CRYPTO_RSA_CLASSNAME CRsaMocanaSs
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CRsaMocanaSs
{
// Enum Type Definition.
public:
    // Type of padding used for the encryption.
    enum EEncryptionPaddingAlgorithm
    {
        ePKCS1
    };

    // Type of padding used for the signature.
    enum ESignaturePaddingAlgorithm
    {
        eSIG_PKCS1
    };

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>.

    // Summary:
    //  Default constructor.
    CRsaMocanaSs();

    // Summary:
    //  Destructor.
    virtual ~CRsaMocanaSs();

    mxt_result GenerateKey(IN unsigned int uExponent, IN unsigned int uKeySizeInBits);

    mxt_result PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const uint8_t* puMessage,
                                IN unsigned int uMessageSize,
                                IN EEncryptionPaddingAlgorithm ePadding,
                                IN unsigned int uEncryptedMessageCapacity,
                                OUT uint8_t* puEncryptedMessage,
                                OUT unsigned int* puEncryptedMessageSize);

    mxt_result PublicKeyEncrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const CBlob* pblobMessage,
                                IN EEncryptionPaddingAlgorithm ePadding,
                                OUT CBlob* pblobEncryptedMessage);

    mxt_result PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const uint8_t* puEncryptedMessage,
                                IN unsigned int uEncryptedMessageSize,
                                IN ESignaturePaddingAlgorithm ePadding,
                                IN unsigned int uMessageCapacity,
                                OUT uint8_t* puMessage,
                                OUT unsigned int* puMessageSize);

    mxt_result PublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                IN const CBlob* pblobEncryptedMessage,
                                IN ESignaturePaddingAlgorithm ePadding,
                                OUT CBlob* pblobMessage);

    mxt_result PrivateKeyEncrypt(IN const uint8_t* puMessage,
                                 IN unsigned int uMessageSize,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 IN unsigned int uEncryptedMessageCapacity,
                                 OUT uint8_t* puEncryptedMessage,
                                 OUT unsigned int* puEncryptedMessageSize);

    mxt_result PrivateKeyEncrypt(IN const CBlob* pblobMessage,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 OUT CBlob* pblobEncryptedMessage);

    mxt_result PrivateKeyDecrypt(IN const uint8_t* puEncryptedMessage,
                                 IN unsigned int uEncryptedMessageSize,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 IN unsigned int uMessageCapacity,
                                 OUT uint8_t* puMessage,
                                 OUT unsigned int* puMessageSize);

    mxt_result PrivateKeyDecrypt(IN const CBlob* pblobEncryptedMessage,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 OUT CBlob* pblobMessage);

    mxt_result Sign(IN const uint8_t* puHash,
                    IN unsigned int uHashSize,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    IN unsigned int uSignatureCapacity,
                    OUT uint8_t* puSignature,
                    OUT unsigned int* puSignatureSize);

    mxt_result Sign(IN const CBlob* pblobHash,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    OUT CBlob* pblobSignature);

    mxt_result Verify(IN const CPublicKey* pRemotePublicKey,
                      IN const uint8_t* puSignature,
                      IN unsigned int uSignatureSize,
                      IN const uint8_t* puHash,
                      IN unsigned int uHashSize,
                      IN CHash::EAlgorithm eHashAlgorithm);

    mxt_result Verify(IN const CPublicKey* pRemotePublicKey,
                      IN const CBlob* pblobSignature,
                      IN const CBlob* pblobHash,
                      IN CHash::EAlgorithm eHashAlgorithm);

    mxt_result GetPublicKey(OUT CPublicKey* pPublicKey);

    mxt_result GetPrivateKey(OUT CPrivateKey* pPrivateKey);

    mxt_result SetKey(IN const CPrivateKey* pPrivateKey);

//-- Hidden Methods
//------------------
protected:

    void Lock() const;
    void Unlock() const;

private:
    //-- << Deactivated Constructors / Destructors / Operators >>.

    // Summary:
    //  Copy Constructor.
    CRsaMocanaSs(IN const CRsaMocanaSs& rRsa);

    // Summary:
    //  Assignment Operator.
    CRsaMocanaSs& operator=(IN const CRsaMocanaSs& rRsa);


    //-- << Helpers >>

    // Summary:
    //  Adds PKCS1 padding to a message.
    mxt_result AddPkcs1Padding(IN unsigned int uMessageSize,
                               IN const uint8_t* puMessage,
                               IN unsigned int uPaddedMessageSize,
                               OUT uint8_t* puPaddedMessage);

    // Summary:
    //  Performs the RSA encryption with a private key.
    mxt_result InternalPrivateKeyEncrypt(IN unsigned int uPaddedMessageSize,
                                         INOUT uint8_t* puPaddedMessage);

    // Summary:
    //  Performs the RSA decryption with a public key.
    mxt_result InternalPublicKeyDecrypt(IN const CPublicKey* pRemotePublicKey,
                                        IN const uint8_t* puEncryptedMessage,
                                        IN unsigned int uEncryptedMessageSize,
                                        IN unsigned int uMessageCapacity,
                                        OUT uint8_t* puMessage,
                                        OUT unsigned int* puMessageSize);

    // Summary:
    //  Removes the PKCS1 padding from a message.
    mxt_result RemovePkcs1Padding(IN unsigned int uExpectedPaddedMessageLength,
                                  INOUT uint8_t* puMessage,
                                  INOUT unsigned int* puMessageSize);

//-- Hidden Data Members.
protected:
    // The RSA private key.
    CPrivateKey* m_pPrivateKey;

    // The RSA public key.
    CPublicKey* m_pPublicKey;

    // The big numbers allocation queue.
    vlong* m_pQueue;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
void CRsaMocanaSs::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CRsaMocanaSs::Unlock() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_RSA_MOCANA_SS)

#endif // #ifndef MXG_CRSAMOCANASS_H
//M5T_INTERNAL_USE_END


