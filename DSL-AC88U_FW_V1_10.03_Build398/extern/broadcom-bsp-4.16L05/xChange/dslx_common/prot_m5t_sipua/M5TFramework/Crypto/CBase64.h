//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CBASE64_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CBASE64_H
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

#if defined (MXD_CRYPTO_BASE64_MITOSFW)

//-- Data Members
//-----------------
#ifndef MXG_CCRYPTO_H
#include "Crypto/CCrypto.h"
#endif // #ifndef MXG_CCRYPTO_H

#if defined(MXD_CAP_ENABLE_SUPPORT)
    #ifndef MXG_CBLOB_H
    #include "Cap/CBlob.h"
    #endif // #ifndef MXG_CBLOB_H
#endif //MXD_CAP_ENABLE_SUPPORT

#ifndef MXG_CCIPHER_H
#include "Crypto/CCipher.h"
#endif // #ifndef MXG_CCIPHER_H


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//-- Constants and defines
//-------------------------

//M5T_INTERNAL_USE_BEGIN
const unsigned int uBASE64_ENCRYPT_INPUT_BLOCK = 3;
const unsigned int uBASE64_ENCRYPT_OUTPUT_BLOCK = 4;
const unsigned int uBASE64_DECRYPT_INPUT_BLOCK = 4;
const unsigned int uBASE64_DECRYPT_OUTPUT_BLOCK = 3;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CBase64
//===================
//<GROUP CRYPTO_CLASSES>
//
// Summary:
//   Class that manages Base64 encoding.
//
// Description:
//   CBase64 is the class that manages Base64 encoding. Base64 encoding
//   takes groups of 24 bits with each character encoded in 8 bits and
//   transforms it to 4 characters each encoded in 6 bits. Each of these
//   characters is then mapped to one of 64 possible characters. If there are
//   not enough characters to complete the encoding, a 65th character, '=', is
//   appended at the end.
//
//   To decode, each series of four 6 bit characters is converted back into
//   3 8 bit characters.
//
//   The block size of the input buffer is 3 bytes long and the size of an
//   output block is 4 bytes long.
//
//   To get the size of the output buffer, it is possible to call the update
//   method with the output buffer equal to NULL. The buffer can then be
//   allocated with the appropriate size to contain the output.
//
// Location:
//  Crypto/CBase64.h
//
//==EDOC========================================================================
class CBase64 : public CCipher
{
//-- Published Interface
//------------------------
public:
    //-- New types within class namespace
    //-----------------------------------

    //<GROUP CRYPTO_ENUMS>
    //
    // Summary:
    //  Types of variants.
    enum EVariant
    {
        // Description:
        // Standard variant.
        eSTANDARD,
        // Description:
        // URL variant.
        eURL
    };

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CBase64();

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CBase64();

    //-- << Public Methods >>
    //------------------------

    // Summary:
    //  Gets the variant.
    //--------------------
    EVariant GetVariant() const;

    // Summary:
    //  Sets the variant.
    //--------------------
    void SetVariant(EVariant eVariant);

    // Inherited from CCipher.

    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const uint8_t* puKey = NULL,
                             IN unsigned int uKeySize = 0,
                             IN const uint8_t* puIV = NULL,
                             IN unsigned int uIVSize = 0);
    virtual mxt_result Update(IN const uint8_t* puInData,
                              IN unsigned int uInDataSize,
                              OUT uint8_t* puOutData,
                              OUT unsigned int* puOutDataSize);
    virtual mxt_result End(OUT uint8_t* puOutData,
                           OUT unsigned int* puOutDataSize);

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual mxt_result Begin(IN EAction eAction,
                             IN EMode eMode,
                             IN const CBlob* pblobKey,
                             IN const CBlob* pblobIV = NULL);
    virtual mxt_result Update(IN const CBlob* pblobInData,
                              OUT CBlob* pblobOutData,
                              IN bool bAppend = true);
    virtual mxt_result End(OUT CBlob* pblobOutData, IN bool bAppend = true);
#endif //MXD_CAP_ENABLE_SUPPORT

    virtual EAlgorithm GetAlgorithm();
    virtual unsigned int GetBlockSizeInBits();
    virtual unsigned int GetBlockSizeInByte();

#if defined(MXD_CAP_ENABLE_SUPPORT)
    virtual const CVector<EMode>* GetSupportedModes();
#endif //MXD_CAP_ENABLE_SUPPORT

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:
    //-- << Helper Methods >>
    //-------------------------
    int32_t Position(IN uint8_t u);

    mxt_result Decode(IN const uint8_t* puData,
                      IN unsigned int uInputData,
                      OUT uint8_t* puDecoded,
                      OUT unsigned int* puDecodedSize);

    void Encode(IN const uint8_t* puData,
                IN unsigned int uSize,
                OUT uint8_t* puEncoded);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------
    CBase64(IN const CBase64& rFrom);

    CBase64& operator=(IN const CBase64& rFrom);

//-- Hidden Data Members
//------------------------
protected:
private:
    EAction m_eDefaultAction;
    EMode m_eDefaultMode;

    EAction m_eAction;
    EMode m_eMode;

    uint8_t m_auUnusedData[uBASE64_ENCRYPT_OUTPUT_BLOCK];
    unsigned int m_uUnusedSize;

    EVariant m_eVariant;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline
CCipher::EAlgorithm CBase64::GetAlgorithm()
{
    return CCipher::eALGORITHM_BASE64;
}

inline
unsigned int CBase64::GetBlockSizeInBits()
{
    return 0;
}

inline
unsigned int CBase64::GetBlockSizeInByte()
{
    return 0;
}

#if defined(MXD_CAP_ENABLE_SUPPORT)

inline
const CVector<CCipher::EMode>* CBase64::GetSupportedModes()
{
    return NULL;
}

#endif //MXD_CAP_ENABLE_SUPPORT

//==SDOC========================================================================
//==
//== GetVariant
//==
//==============================================================================
//
//  Returns:
//   The variant of the CBase64 object.
//
//  Description:
//   Gets the variant of a CBase64.
//
//==EDOC========================================================================
inline
CBase64::EVariant CBase64::GetVariant() const
{
    return m_eVariant;
}

//==SDOC========================================================================
//==
//== SetVariant
//==
//==============================================================================
//
//  Parameter:
//   eVariant:
//    The variant to set.
//
//  Description:
//   Sets the variant of a CBase64.
//
//==EDOC========================================================================
inline
void CBase64::SetVariant(CBase64::EVariant eVariant)
{
    m_eVariant = eVariant;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined (MXD_CRYPTO_BASE64_MITOSFW)

#endif //-- #ifndef MXG_CBASE64_H

