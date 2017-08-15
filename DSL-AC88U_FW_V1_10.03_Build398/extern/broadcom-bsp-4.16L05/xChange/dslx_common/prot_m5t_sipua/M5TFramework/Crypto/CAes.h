//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
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
#ifndef MXG_CAES_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CAES_H
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

#if !defined(MXD_CRYPTO_AES_NONE)

#if defined(MXD_CRYPTO_AES_OPENSSL) || defined (MXD_CRYPTO_AES_MOCANA_SS)
    #ifndef MXG_CCRYPTO_H
        #include "Crypto/CCrypto.h" // AES_BLOCK_SIZE
    #endif
#endif

#if defined(MXD_CRYPTO_AES_MITOSFW) || defined (MXD_CRYPTO_AES_MOCANA_SS)
    #ifndef HEADER_AES_H
        #include "Crypto/Aes/aes.h" // AES_BLOCK_SIZE
    #endif
#endif

//M5T_INTERNAL_USE_BEGIN
// We must define AES_BLOCK_SIZE if it is not.
// This can happen when we override the AES implementation.
#if !defined(AES_BLOCK_SIZE)
    #define AES_BLOCK_SIZE 16
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//  We need to declare these const here so that they are available from
//  MXD_CRYPTO_AES_INCLUDE.
//==============================================================================
const unsigned int uAES_BLOCK_SIZE = AES_BLOCK_SIZE;
const unsigned int uAES_BLOCK_SIZE_IN_BITS = AES_BLOCK_SIZE * 8;
//M5T_INTERNAL_USE_END

#if defined(MXD_CRYPTO_AES_INCLUDE)
    #include MXD_CRYPTO_AES_INCLUDE
#else
    #error "No implementation defined for CAes!!!"
#endif

//-- Data Members
//-----------------
#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
        #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CCIPHER_H
    #include "Crypto/CCipher.h"
#endif // #ifndef MXG_CCIPHER_H

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CAes
//========================================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages the AES algorithm.
//
// Description:
//   CAes is the class that manages the AES algorithm. The algorithm
//   decrypts/encrypts data by blocks of 16 bytes. If a block of less than 16
//   bytes is passed to the Update method, this block is temporarily stored
//   internally until new data is added and the block is completed or the End
//   method is called.
//
// Location:
//   Crypto/CAes.h
//
// See Also:
//
//==EDOC========================================================================
class CAes : public CCipher,
             public MXD_CRYPTO_AES_CLASSNAME
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CAes();

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CAes();

    //  Summary:
    //      Sets the default action used.
    //------------------------------------
    mxt_result SetDefaultAction(IN EAction eAction);

    //  Summary:
    //      Sets the default mode used.
    //------------------------------------
    mxt_result SetDefaultMode(IN EMode eMode);

    //  Summary:
    //      Sets the default IV.
    //---------------------------
    mxt_result SetDefaultIV(IN const uint8_t* puIV, IN unsigned int uIVSize);

    //  Summary:
    //      Sets the default key.
    //----------------------------
    mxt_result SetDefaultKey(IN const uint8_t* puKey, IN unsigned int uKeySize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    //==SDOC====================================================================
    //==
    //==   SetDefaultIV
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default initialization vector.
    //
    //  Parameters:
    //      pIV:
    //          Pointer to the blob containing the initialization vector.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Sets the default initialization vector. This parameter is used when
    //      the initialization vector is not specified in the Begin method.
    //
    //  See Also:
    //      Begin
    //
    //==EDOC====================================================================
    mxt_result SetDefaultIV(IN const CBlob* pIV);

    //==SDOC====================================================================
    //==
    //==   SetDefaultKey
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default key.
    //
    //  Parameters:
    //      pKeyV:
    //          Pointer to the blob containing the key.
    //
    //  Returns:
    //      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
    //      otherwise.
    //
    //  Description:
    //      Sets the default key. This parameter is used when the key is not
    //      specified in the Begin method.
    //
    //  See Also:
    //      Begin
    //
    //==EDOC====================================================================
    mxt_result SetDefaultKey(IN const CBlob* pKey);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const uint8_t* puKey,
                             IN unsigned int uKeySize,
                             IN const uint8_t* puIV = NULL,
                             IN unsigned int uIVSize = 0);
    virtual mxt_result Update(IN const uint8_t* puInData,
                              IN unsigned int uInDataSize,
                              OUT uint8_t* puOutData,
                              OUT unsigned int* puOutDataSize);
    virtual mxt_result End(OUT uint8_t* puOutData, OUT unsigned int* puOutDataSize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const CBlob* pKey,
                             IN const CBlob* pIV = NULL);
    virtual mxt_result Update(IN const CBlob* pInData,
                              OUT CBlob* pOutData,
                              IN bool bAppend = true);
    virtual mxt_result End(OUT CBlob* pOutData, IN bool bAppend = true);
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

    virtual EAlgorithm GetAlgorithm();
    virtual unsigned int GetBlockSizeInBits();
    virtual unsigned int GetBlockSizeInByte();

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual const CVector<EMode>* GetSupportedModes();
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC====================================================================
//==
//==   CAes
//==
//==========================================================================
//
//  Description:
//      Basic constructor for the object.
//
//==EDOC====================================================================
inline
CAes::CAes()
 : MXD_CRYPTO_AES_CLASSNAME()
{
}

//==SDOC====================================================================
//==
//==   ~CAes
//==
//==========================================================================
//
//  Description:
//      Destructor for the object.
//
//==EDOC====================================================================
inline
CAes::~CAes()
{
}

//==SDOC====================================================================
//==
//==   SetDefaultAction
//==
//==========================================================================
//
//  Parameters:
//      eAction:
//          Action to use.
//
//  Returns:
//      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
//      otherwise.
//
//  Description:
//      Sets the default action to use when eACTION_DEFAULT is passed to the
//      Begin method.
//
//  See Also:
//      Begin
//
//==EDOC====================================================================
inline
mxt_result CAes::SetDefaultAction(IN EAction eAction)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultAction(eAction);
}

//==SDOC====================================================================
//==
//==   SetDefaultMode
//==
//==========================================================================
//
//  Parameters:
//      eMode:
//          Mode to use
//
//  Returns:
//      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
//      otherwise.
//
//  Description:
//      Sets the default mode to use when eMODE_DEFAULT is passed to the Begin
//      method.
//
//  See Also:
//      Begin
//
//==EDOC====================================================================
inline
mxt_result CAes::SetDefaultMode(IN EMode eMode)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultMode(eMode);
}

//==SDOC====================================================================
//==
//==   SetDefaultIV
//==
//==========================================================================
//
//  Parameters:
//      puIV:
//          Pointer to the initialization vector.
//
//      uIVSize:
//          Size of the initialization vector.
//
//  Returns:
//      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
//      otherwise.
//
//  Description:
//      Sets the default initialization vector. This parameter is used when
//      the initialization vector is not specified in the Begin method.
//
//  See Also:
//      Begin
//
//==EDOC====================================================================
inline
mxt_result CAes::SetDefaultIV(IN const uint8_t* puIV, IN unsigned int uIVSize)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultIV(puIV, uIVSize);
}

//==SDOC====================================================================
//==
//==   SetDefaultKey
//==
//==========================================================================
//
//  Parameters:
//      puKeyV:
//          Pointer to the key.
//
//      uIVSize:
//          Size of the key.
//
//  Returns:
//      resS_OK if successful, resFE_FAIL or resFE_INVALID_ARGUMENT
//      otherwise.
//
//  Description:
//      Sets the default key. This parameter is used when the key is not
//      specified (= NULL) in the Begin method. When used, it increases the
//      performance of the "Begin" method because the key is not set in the
//      AES algorithm each time it is called. The key is rather set only 
//      once until a new key is set or a key is used in the "Begin" method.
//
//  See Also:
//      Begin
//
//==EDOC====================================================================
inline
mxt_result CAes::SetDefaultKey(IN const uint8_t* puKey, IN unsigned int uKeySize)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultKey(puKey, uKeySize);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CAes::SetDefaultIV(IN const CBlob* pIV)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultIV(pIV);
}

inline
mxt_result CAes::SetDefaultKey(IN const CBlob* pKey)
{
    return MXD_CRYPTO_AES_CLASSNAME::SetDefaultKey(pKey);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
mxt_result CAes::Begin(IN EAction eAction,
                       IN EMode eMode,
                       IN const uint8_t* puKey,
                       IN unsigned int uKeySize,
                       IN const uint8_t* puIV,
                       IN unsigned int uIVSize)
{
    return MXD_CRYPTO_AES_CLASSNAME::Begin(eAction,
                                           eMode,
                                           puKey,
                                           uKeySize,
                                           puIV,
                                           uIVSize);
}

inline
mxt_result CAes::Update(IN const uint8_t* puInData,
                        IN unsigned int uInDataSize,
                        OUT uint8_t* puOutData,
                        OUT unsigned int* puOutDataSize)
{
    return MXD_CRYPTO_AES_CLASSNAME::Update(puInData,
                                            uInDataSize,
                                            puOutData,
                                            puOutDataSize);
}

inline
mxt_result CAes::End(OUT uint8_t* puOutData, OUT unsigned int* puOutDataSize)
{
    return MXD_CRYPTO_AES_CLASSNAME::End(puOutData, puOutDataSize);
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
mxt_result CAes::Begin(IN EAction eAction,
                       IN EMode eMode,
                       IN const CBlob* pKey,
                       IN const CBlob* pIV)
{
    return MXD_CRYPTO_AES_CLASSNAME::Begin(eAction,
                                           eMode,
                                           pKey,
                                           pIV);
}

inline
mxt_result CAes::Update(IN const CBlob* pInData,
                        OUT CBlob* pOutData,
                        IN bool bAppend)
{
    return MXD_CRYPTO_AES_CLASSNAME::Update(pInData,
                                            pOutData,
                                            bAppend);
}

inline
mxt_result CAes::End(OUT CBlob* pOutData, IN bool bAppend)
{
    return MXD_CRYPTO_AES_CLASSNAME::End(pOutData, bAppend);
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

inline
CCipher::EAlgorithm CAes::GetAlgorithm()
{
    return MXD_CRYPTO_AES_CLASSNAME::GetAlgorithm();
}

inline
unsigned int CAes::GetBlockSizeInBits()
{
    return MXD_CRYPTO_AES_CLASSNAME::GetBlockSizeInBits();
}

inline
unsigned int CAes::GetBlockSizeInByte()
{
    return MXD_CRYPTO_AES_CLASSNAME::GetBlockSizeInByte();
}

#if defined(MXD_CAP_ENABLE_SUPPORT)
inline
const CVector<CCipher::EMode>* CAes::GetSupportedModes()
{
    return MXD_CRYPTO_AES_CLASSNAME::GetSupportedModes();
}
#endif // #if defined(MXD_CAP_ENABLE_SUPPORT)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_CRYPTO_AES_NONE)

#endif // MXG_CAES_H


