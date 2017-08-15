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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_CRSAOPENSSL_H
#define MXG_CRSAOPENSSL_H

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

#if defined(MXD_CRYPTO_RSA_OPENSSL)

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
#ifndef MXD_CRYPTO_RSA_CLASSNAME
    #define MXD_CRYPTO_RSA_CLASSNAME CRsaOpenSsl
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

class CRsaOpenSsl
{
// Enum Type Definition
//----------------------
public:
    // Type of padding used for the encryption
    enum EEncryptionPaddingAlgorithm
    {
        ePKCS1,
        ePKCS1_OAEP
    };

    // Type of padding used for the signature.
    enum ESignaturePaddingAlgorithm
    {
        eSIG_PKCS1,
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //-----------------------
    CRsaOpenSsl();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CRsaOpenSsl();

    // Summary:
    //  Generates the RSA private key and the Modulus.
    //-------------------------------------------------
    mxt_result GenerateKey(IN unsigned int uExponent, IN unsigned int uKeySizeInBits);

    // Summary:
    //  Encrypts the message with the public key.
    //--------------------------------------------
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

    // Summary:
    //  Decrypts the message with the public key.
    //--------------------------------------------
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

    // Summary:
    //  Encrypts the message with the private key.
    //---------------------------------------------
    mxt_result PrivateKeyEncrypt(IN const uint8_t* puMessage,
                                 IN unsigned int uMessageSize,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 IN unsigned int uEncryptedMessageCapacity,
                                 OUT uint8_t* puEncryptedMessage,
                                 OUT unsigned int* puEncryptedMessageSize);

    mxt_result PrivateKeyEncrypt(IN const CBlob* pblobMessage,
                                 IN ESignaturePaddingAlgorithm ePadding,
                                 OUT CBlob* pblobEncryptedMessage);

    // Summary:
    //  Decrypts the message with the private key.
    //---------------------------------------------
    mxt_result PrivateKeyDecrypt(IN const uint8_t* puEncryptedMessage,
                                 IN unsigned int uEncryptedMessageSize,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 IN unsigned int uMessageCapacity,
                                 OUT uint8_t* puMessage,
                                 OUT unsigned int* puMessageSize);

    mxt_result PrivateKeyDecrypt(IN const CBlob* pblobEncryptedMessage,
                                 IN EEncryptionPaddingAlgorithm ePadding,
                                 OUT CBlob* pblobMessage);

    // Summary:
    //  Signs a message.
    //-------------------
    mxt_result Sign(IN const uint8_t* puHash,
                    IN unsigned int uHashSize,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    IN unsigned int uSignatureCapacity,
                    OUT uint8_t* puSignature,
                    OUT unsigned int* puSignatureSize);

    mxt_result Sign(IN const CBlob* pblobHash,
                    IN CHash::EAlgorithm eHashAlgorithm,
                    OUT CBlob* pblobSignature);

    // Summary:
    //  Verifies a signature.
    //------------------------
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

    // Summary:
    //  Gets the public key.
    //-----------------------
    mxt_result GetPublicKey(OUT CPublicKey* pPublicKey);

    // Summary:
    //  Gets the private key.
    //------------------------
    mxt_result GetPrivateKey(OUT CPrivateKey* pPrivateKey);

    // Summary:
    //  Sets the keys for RSA.
    //-------------------------
    mxt_result SetKey(IN const CPrivateKey* pPrivateKey);



//-- Hidden Methods
//------------------
protected:

    void Lock() const;
    void Unlock() const;
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CRsaOpenSsl(IN const CRsaOpenSsl& rRsa);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CRsaOpenSsl& operator=(IN const CRsaOpenSsl& rRsa);

//-- Hidden Data Members
//------------------------
protected:
private:
    RSA* m_pRsa;
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================
inline
void CRsaOpenSsl::Lock() const
{
    CCrypto::Instance()->Enter();
}

inline
void CRsaOpenSsl::Unlock() const
{
    CCrypto::Instance()->Exit();
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_CRYPTO_RSA_OPENSSL)

#endif // #ifndef MXG_CRSAOPENSSL_H
//M5T_INTERNAL_USE_END


