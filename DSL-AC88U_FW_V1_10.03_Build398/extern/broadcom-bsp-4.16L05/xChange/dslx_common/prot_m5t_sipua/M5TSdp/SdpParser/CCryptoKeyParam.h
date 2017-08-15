//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CCRYPTOKEYPARAM_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CCRYPTOKEYPARAM_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const unsigned int uMAX_MKI_LENGTH = 128;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CCryptoKeyParam
//=========================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//      This class implements key parameters.
//
// Description:
//      This class implements key parameters, separated by character specified
//      in the draft-ietf-mmusic-sdescriptions-09.
//
//  <CODE>
//    draft-ietf-mmusic-sdescriptions-09 ABNF:
//
//        key-param       = key-method ":" key-info
//        key-method      = srtp-key-method
//        srtp-key-method = "inline"
//        key-info        = srtp-key-info
//        srtp-key-info   = key-salt ["|" lifetime] ["|" mki]
//
//        key-salt        = 1*(base64)   ; binary key and salt values
//                                       ; concatenated together, and then
//                                       ; base64 encoded [section 6.8 of
//                                       ; RFC2046]
//
//        lifetime      = ["2^"] 1*(DIGIT)   ; see section 5.1 for "2^"
//        mki           = mki-value ":" mki-length
//        mki-value     = 1*DIGIT
//        mki-length    = 1*3DIGIT   ; range 1..128.
//
//        base64        = ALPHA / DIGIT / "+" / "/" / "="
//  </CODE>
//
// Location:
//   SdpParser/CCryptoKeyParam.h
//
//==============================================================================
class CCryptoKeyParam : public CSdpParser
{
//-- Published Interface

public:

    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CCryptoKeyParam();
    // Summary: Copy constructor
    CCryptoKeyParam(IN const CCryptoKeyParam& rCryptoKeyParam);
    // Summary: Destructor.
    virtual ~CCryptoKeyParam();

    //  Summary:
    //   Assignment operator.
    CCryptoKeyParam& operator=(IN const CCryptoKeyParam& rFrom);

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CCryptoKeyParam& rFrom) const;

    //-- << Public methods >>

    //-- << Stereotype >>

    // Summary:
    //  Gets the key method.
    const char* GetKeyMethod() const;
    // Summary:
    //  Sets the key method.
    void SetKeyMethod(IN const char* pszKeyMethod);

    // Summary:
    //  Gets the crypto key.
    const char* GetKey() const;
    // Summary:
    //  Sets the crypto key.
    void SetKey(IN const char* pszKey);

    // Summary:
    //  Gets the lifetime.
    const uint64_t GetLifeTime() const;
    // Summary:
    //  Sets the lifetime.
    void SetLifeTime(IN uint64_t uLifeTime);

    // Summary:
    //  Gets the mki value.
    int GetMkiValue() const;
    // Summary:
    //  Sets the mki value.
    void SetMkiValue(IN int nMkiValue);

    // Summary:
    //  Gets the mki value of the key as a byte array.
    void GetMkiValue(INOUT uint8_t* puMkiValue, IN unsigned int uLenght) const;
    // Summary:
    //  Sets the mki value of the key and its length.
    void SetMki(IN uint8_t* puMkiValue, IN unsigned int uLenght);

    // Summary:
    //  Gets mki lenght.
    int GetMkiLength() const;
    // Summary:
    //  Sets the mki length.
    void SetMkiLength(IN int nMkiLength);

    // Summary:
    //  Parses the parameters list beginning at rpszStartPosition.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //  Inserts cSeparator after each parameter, except for last parameter.
    void Serialize(INOUT CBlob& rBlob);

    // Summary:
    //  Return true if data members are valid
    bool Validate();

    // Summary:
    //  Resets this object.
    void Reset();
    
//-- Helpers
private:
    // Summary:
    //  Computes the binary logarithm of a power of two number.
    uint8_t LogBaseTwo(uint64_t uPowerOfTwoNumber);

    // Summary:
    //  Computes whether or not a number is a power of two.
    bool IsPowerOfTwo(int64_t nNumber);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Data Members

private:
    CString m_strKeyMethod;
    CString m_strKeySalt;
    uint64_t m_uLifeTime;
    uint8_t m_auMkiValue[uMAX_MKI_LENGTH];
    unsigned int m_uMkiLength;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetKeyMethod
//==
//==============================================================================
//
//  Returns:
//      NULL-terminated string of the key method.
//
//  Description:
//      Returns the key method.
//
//==============================================================================
inline const char* CCryptoKeyParam::GetKeyMethod() const
{
    return m_strKeyMethod.CStr();
}

//==============================================================================
//==
//==  SetKeyMethod
//==
//==============================================================================
//
//  Parameters:
//    pszKeyMethod:
//      Key method to set
//
//  Description:
//      This method sets the key method to use
//
//==============================================================================
inline void CCryptoKeyParam::SetKeyMethod(IN const char* pszKeyMethod)
{
    m_strKeyMethod = pszKeyMethod;
}

//==============================================================================
//==
//==  GetKey
//==
//==============================================================================
//
//  Returns:
//      A char* with the key in it.
//
//  Description:
//      Returns the key and salt key in base64 format.
//
//==============================================================================
inline const char* CCryptoKeyParam::GetKey() const
{
    return m_strKeySalt.CStr();
}

//==============================================================================
//==
//==  SetKey
//==
//==============================================================================
//
//  Parameters:
//    pszKey:
//      Key to set. The key and salt key should be concatenated and base64
//      encoded
//
//  Description:
//      This method sets the key(s) to use.
//
//==============================================================================
inline void CCryptoKeyParam::SetKey(IN const char* pszKey)
{
    m_strKeySalt = pszKey;
}

//==============================================================================
//==
//==  GetLifeTime
//==
//==============================================================================
//
//  Returns:
//      The life time of the key.
//
//  Description:
//      Returns the life time of the key.
//
//==============================================================================
inline const uint64_t CCryptoKeyParam::GetLifeTime() const
{
    return m_uLifeTime;
}

//==============================================================================
//==
//==  SetLifeTime
//==
//==============================================================================
//
//  Parameters:
//    uLifeTime:
//      Life time of the key.
//
//  Description:
//      This method sets the life time of the key.
//
//==============================================================================
inline void CCryptoKeyParam::SetLifeTime(IN uint64_t uLifeTime)
{
    m_uLifeTime = uLifeTime;
}

//==============================================================================
//==
//==  GetMkiValue
//==
//==============================================================================
//
//    puMkiValue:
//      Array of bytes that contains the Mki value encoded as a positive
//      decimal integer.
//
//    uLength:
//      Length of the puMkiValue array. It should be in the range 0..128. It
//      should be the one returned by GetMkiLength.
//
//  Description:
//      Returns the Mki value of the key as a byte array. If the uLength byte
//      array is not large enough, only the first nLength bytes will be copied.
//
//==============================================================================
inline void CCryptoKeyParam::GetMkiValue(INOUT uint8_t* puMkiValue, IN unsigned int uLength) const
{
    MX_ASSERT(uLength <= uMAX_MKI_LENGTH);

    // Bytes are aligned to the right of the array in MSB order to simplify
    // the manipulations.
    memcpy(puMkiValue, m_auMkiValue + uMAX_MKI_LENGTH - uLength, uLength);
}

//==============================================================================
//==
//==  SetMki
//==
//==============================================================================
//
//  Parameters:
//    puMkiValue:
//      Array of bytes that contains the Mki value encoded as a positive decimal
//      integer.
//
//    uLength:
//      Length of the array that contains the Mki length of the key. It should
//      be in the range 0..128.
//
//  Description:
//      This method sets the Mki value of the key and its length. The values
//      contained in the array MUST be in network byte order.
//
//==============================================================================
inline void CCryptoKeyParam::SetMki(IN uint8_t* puMkiValue, IN unsigned int uLength)
{
    MX_ASSERT(uLength <= uMAX_MKI_LENGTH);
    m_uMkiLength = uLength;

    memset(m_auMkiValue, 0, uMAX_MKI_LENGTH);
    // Bytes are aligned to the right of the array in MSB order to simplify
    // the manipulations.
    memcpy(m_auMkiValue + uMAX_MKI_LENGTH - uLength, puMkiValue, uLength);
}

//==============================================================================
//==
//==  GetLifeTime
//==
//==============================================================================
//
//  Returns:
//      The Mki length of the key.
//
//  Description:
//      Returns the Mki length of the key.
//
//==============================================================================
inline int CCryptoKeyParam::GetMkiLength() const
{
    return static_cast<int>(m_uMkiLength);
}

//==============================================================================
//==
//==  SetMkiLength
//==
//==============================================================================
//
//  Parameters:
//    nMkiLength:
//      The Mki length of the key.
//
//  Description:
//      This method sets the Mki length of the key. It should be in the range
//      0..128.
//
//==============================================================================
inline void CCryptoKeyParam::SetMkiLength(IN int nMkiLength)
{
    m_uMkiLength = static_cast<unsigned int>(nMkiLength);
}

//==============================================================================
//==
//==  IsPowerOfTwo
//==
//==============================================================================
//
//  Parameters:
//    nNumber:
//      The number to evaluate.
//
//  Returns:
//      True if the given number is a power of two.
//
//  Description: Computes whether or not a number is a power of two.
//
//==============================================================================
inline bool CCryptoKeyParam::IsPowerOfTwo(int64_t nNumber)
{
    // A binary AND of a number and the opposite of that number gives back the 
    // number if it is a power of two :
    //
    // Ex : 64 (2^6)             Ex : 7 
    //      64 : 0100 0000            7 : 0000 0111
    //   & -64 : 1100 0000         & -7 : 0000 1001
    //           ---------                ---------
    //           0100 0000 -> 64          0000 0001 -> 1
    //
    return ((nNumber & -nNumber) == nNumber);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CCRYPTOKEYPARAM_H

