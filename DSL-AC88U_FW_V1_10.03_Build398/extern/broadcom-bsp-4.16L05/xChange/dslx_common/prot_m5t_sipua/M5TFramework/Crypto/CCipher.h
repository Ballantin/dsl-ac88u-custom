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
#ifndef MXG_CCIPHER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCIPHER_H
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
#include "Config/FrameworkCfg.h" // MXD_CRYPTO_XXX
#endif


//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#if defined(MXD_CAP_ENABLE_SUPPORT)
    //-- Parameters used by Value
    //-----------------------------
    #ifndef MXG_CVECTOR_H
    #include "Cap/CVector.h"
    #endif // #ifndef MXG_CVECTOR_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    class CBlob;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)
//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CCipher
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Base class for all cipher algorithms.
//
//  Description:
//   CCipher is the base class for all cipher algorithms.
//
//   There are two types of cipher algorithms, stream cipher and block cipher.
//   Both are managed by CCipher. A stream cipher is an algorithm that encrypts
//   data on a byte per byte basis. A block cipher operates on fixed length
//   block size, such as 16 bytes in the case of AES. Encryption is performed
//   on a block by block basis. If longer messages are to be encrypted, several
//   modes of operation may be used.
//
//   The simplest mode of operation is Electronic Cookbook (ECB), in which the
//   message is simply split into blocks and each block is encrypted. This makes
//   identical blocks to give identical cipher blocks after encryption and does
//   not hide data patterns. Padding is required with this mode.
//
//   The Cipher-Block Chain (CBC) mode XORs each block with the previous
//   cipher-text block. Each cipher-text block thus depends on all the plain
//   text blocks up to that point. Padding is required with this mode.
//
//   Counter (CTR) Cipher Feedback (CFB) and Output Feedback (OFB) turn the
//   block cipher into a stream cipher. They do not require padding. They
//   generate key-stream blocks that are then XORed with the plaintext to give
//   the cipher text. CTR generates the next key-stream by encrypting successive
//   values of a counter. CFB generates the next key-stream by encrypting the
//   previous cipher-text block. OFB generates the key-stream by encrypting the
//   previous key-stream block.
//
//   The ECB and CBC modes require the input to be an exact multiple of their
//   block size (sixteen bytes for AES). If the plaintext is not a multiple of
//   block size, it needs to be padded. With AES, the End method returns an
//   error if the leftover number of bytes from the Update method is not a
//   multiple of sixteen. The receiving side needs to know how to remove the
//   padding.
//
// <CODE>
//
//    mxt_result PerformCipher(IN CCipher::EAction eAction,
//                             IN CCipher::EMode eMode,
//                             IN const CBlob* pKey,
//                             IN const CBlob* pIV,
//                             IN const CBlob* pInData,
//                             OUT CBlob* pOutData,
//                             IN CCipher* pCipher)
//    {
//        mxt_result res = resS_OK;
//
//        res = pCipher->Begin(eAction, eMode, pKey, pIV);
//        if (MX_RIS_S(res))
//        {
//            res = pCipher->Update(pInData, pOutData);
//        }
//        if (MX_RIS_S(res))
//        {
//            res = pCipher->End(pOutData, true);
//        }
//
//        return res;
//    }
//
// </CODE>
//
// Location:
//   Crypto/CCipher.h
//
// See Also:
//
//==EDOC========================================================================
class CCipher
{
// Published Enum
//----------------
public:

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    //   The EAction enum specifies whether decryption or encryption should be
    //   performed. eACTION_DEFAULT specifies that the provisioned default
    //   action is to be used.
    enum EAction
    {
        // Description:
        // Decryption.
        eACTION_DECRYPT,
        // Description:
        // Encryption.
        eACTION_ENCRYPT,
        // Description:
        // Default action.
        eACTION_DEFAULT
    };

    //<GROUP CRYPTO_ENUMS>
    // Summary:
    //   Enumerates all the supported cipher algorithms.
    enum EAlgorithm
    {
        // Description:
        // AES algorithm.
        eALGORITHM_AES,
        // Description:
        // Base 64 algorithm.
        eALGORITHM_BASE64
    };

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    //   The EMode enum specifies the utilization mode of the cipher algorithm.
    //   What combination of fundamental cipher algorithm operations should be
    //   performed upon the input stream to generate the output stream.
    //   eMODE_DEFAULT specifies that the provisioned default mode is to be used.
    enum EMode
    {
        // Description:
        // Cipher Block Chaining.
        eMODE_CBC,
        // Description:
        // Cipher Feedback.
        eMODE_CFB,
        // Description:
        // Counter.
        eMODE_CTR,
        // Description:
        // Electronic Code Book.
        eMODE_ECB,
        // Description:
        // Output Feedback.
        eMODE_OFB,
        // Description:
        // Default mode.
        eMODE_DEFAULT
    };

// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC===================================================================
    //==
    //==   CCipher
    //==
    //=========================================================================
    //
    //  Summary:
    //      Constructor.
    //
    //  Description:
    //      Basic constructor.
    //
    //==EDOC===================================================================
    CCipher();

    //==SDOC===================================================================
    //==
    //==   ~CCipher
    //==
    //=========================================================================
    //
    //  Summary:
    //      Destructor.
    //
    //  Description:
    //      Destructor.
    //
    //==EDOC===================================================================
    virtual ~CCipher();

    //==SDOC===================================================================
    //==
    //==   Begin
    //==
    //=========================================================================
    //
    //  Summary:
    //      Initiates the cipher prior to its utilization.
    //
    //  Parameters:
    //      eAction:
    //          Action to perform.
    //
    //      eMode:
    //          The mode in which to use the cipher. If the cipher does not have
    //          multiple modes, then eMODE_DEFAULT MUST be specified.
    //
    //      puKey:
    //          Pointer containing the key to encrypt/decrypt. It may be NULL if
    //          the cipher does not require a key.
    //
    //      uKeySize:
    //          Size of the key in bytes.
    //
    //      puIV:
    //          Pointer containing an initialization vector. It may be NULL if
    //          the cipher does not require an initialization vector.
    //
    //      uIVSize:
    //          Size of the initialization vector.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Initializes the cipher prior to its utilization. Specifies the
    //      action to be performed, the cipher mode, the key, and the
    //      initialization vector.
    //
    //==EDOC===================================================================
    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const uint8_t* puKey = NULL,
                             IN unsigned int uKeySize = 0,
                             IN const uint8_t* puIV = NULL,
                             IN unsigned int uIVSize = 0) = 0;

    //==SDOC===================================================================
    //==
    //==   Update
    //==
    //=========================================================================
    //
    //  Summary:
    //      Decrypts or encrypts input data.
    //
    //  Parameters:
    //      puInData:
    //          Pointer to the byte array to decrypt/encrypt.
    //
    //      uInDataSize:
    //          Size of the byte array to decrypt/encrypt.
    //
    //      puOutData:
    //          Pointer to contain the complete decrypted/encrypted blocks.
    //
    //      puOutDataSize:
    //          Size of the outputted data.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Decrypts or encrypts uInDataSize bytes from the buffer puInData and
    //      writes the result to puOutData. The resulting amount of data
    //      depends on the algorithm used. Sufficient space MUST be available.
    //      The amount of encrypted data returned depends on the number of
    //      complete cipher blocks that are available. Any leftover data
    //      is kept and used in future Update calls or with the End method.
    //
    //  See Also:
    //      End
    //
    //==EDOC===================================================================
    virtual mxt_result Update(IN const uint8_t* puInData,
                              IN unsigned int uInDataSize,
                              OUT uint8_t* puOutData,
                              OUT unsigned int* puOutDataSize) = 0;

    //==SDOC===================================================================
    //==
    //==   End
    //==
    //=========================================================================
    //
    //  Summary:
    //      Decrypts or encrypts the rest of the data.
    //
    //  Parameters:
    //      puOutData:
    //          Pointer to contain the last data block.
    //
    //      puOutDataSize:
    //          Size of the output data.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Decrypts or encrypts the rest of the data that is left within the
    //      cipher and writes it to the output buffer.
    //
    //==EDOC===================================================================
    virtual mxt_result End(OUT uint8_t* puOutData, OUT unsigned int* puOutDataSize) = 0;

#if defined(MXD_CAP_ENABLE_SUPPORT)

    //==SDOC===================================================================
    //==
    //==   Begin
    //==
    //=========================================================================
    //
    //  Summary:
    //      Initiates the cipher prior to its utilization.
    //
    //  Parameters:
    //      eAction:
    //          Action to perform.
    //
    //      eMode:
    //          The mode in which to use the cipher. If the cipher does support
    //          multiple modes, then eMODE_DEFAULT MUST be specified.
    //
    //      pKey:
    //          Pointer containing the key blob to use for
    //          encryption/decryption. It may be NULL if the cipher does not
    //          require a key.
    //
    //      pIV:
    //          Pointer containing an initialization vector blob. It may be NULL
    //          if the cipher does not require an initialization vector.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Initializes the cipher prior to its utilization. Specifies the
    //      action to be performed, the cipher mode, the key, and the
    //      initialization  vector.
    //
    //==EDOC===================================================================
    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const CBlob* pKey,
                             IN const CBlob* pIV) = 0;

    //==SDOC===================================================================
    //==
    //==   Update
    //==
    //=========================================================================
    //
    //  Summary:
    //      Decrypts or encrypts input data.
    //
    //  Parameters:
    //      pInData:
    //          Pointer to the blob to encrypt.
    //
    //      pOutData:
    //          Pointer to a blob to contain the complete decrypted/encrypted
    //          blocks.
    //
    //      bAppend:
    //          True if data is to be appended to the end of the blob, false
    //          otherwise.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Decrypts or encrypts pInData->Size() bytes from pInData and
    //      writes the result to pOutData. The resulting amount of data
    //      depends on the algorithm used. If append is true, the data is
    //      appended to the end of the blob.
    //
    //==EDOC===================================================================
    virtual mxt_result Update(IN const CBlob* pInData,
                              OUT CBlob* pOutData,
                              IN bool bAppend = true) = 0;

    //==SDOC===================================================================
    //==
    //==   End
    //==
    //=========================================================================
    //
    //  Summary:
    //      Decrypts or Encrypts the rest of the data.
    //
    //  Parameters:
    //      puOutData:
    //          Pointer to contain the last data block.
    //
    //      bAppend:
    //          True if data is to be appended to the end of the blob, false
    //          otherwise.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Decrypts or encrypts the rest of the data that is left within the
    //      cipher and writes it to the output buffer. If append is true, the
    //      data is appended to the end of the blob.
    //
    //==EDOC===================================================================
    virtual mxt_result End(OUT CBlob* pOutData, IN bool bAppend = true) = 0;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    //==SDOC===================================================================
    //==
    //==   GetAlgorithm
    //==
    //=========================================================================
    //
    //  Summary:
    //      Returns the underlying algorithm used by the cipher.
    //
    //  Returns:
    //      EAlgorithm.
    //
    //  Description:
    //      Returns the underlying algorithm used by the cipher.
    //
    //==EDOC===================================================================
    virtual EAlgorithm GetAlgorithm() = 0;

    //==SDOC===================================================================
    //==
    //==   GetBlockSizeInBits
    //==
    //=========================================================================
    //
    //  Summary:
    //      Returns the size of each block in bits.
    //
    //  Returns:
    //      Size of a block in bits. Returns 0 if no block size is available
    //      for the specified algorithm.
    //
    //  Description:
    //      Returns the size of each block in bits.
    //
    //==EDOC===================================================================
    virtual unsigned int GetBlockSizeInBits() = 0;

    //==SDOC===================================================================
    //==
    //==   GetBlockSizeInByte
    //==
    //=========================================================================
    //
    //  Summary:
    //      Returns the size of each block in bytes.
    //
    //  Returns:
    //      Size of a block in bytes. Returns 0 if no block size is available
    //      for the specified algorithm.
    //
    //  Description:
    //      Returns the size of each block in bytes.
    //
    //==EDOC===================================================================
    virtual unsigned int GetBlockSizeInByte() = 0;

#if defined(MXD_CAP_ENABLE_SUPPORT)

    //==SDOC===================================================================
    //==
    //==   GetSupportedModes
    //==
    //=========================================================================
    //
    //  Summary:
    //      Returns the modes supported by the cipher.
    //
    //  Returns:
    //      CVector containing EModes.
    //
    //  Description:
    //      Returns the modes supported by the cipher.
    //
    //==EDOC===================================================================
    virtual const CVector<EMode>* GetSupportedModes() = 0;
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CCipher::CCipher()
{
}

inline
CCipher::~CCipher()
{
}

MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_CCIPHER_H

