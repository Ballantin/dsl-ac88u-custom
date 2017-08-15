//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CTOKEN_H
#define MXG_CTOKEN_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRINGHELPER_H
#include "SipParser/CStringHelper.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// BRCM_CUSTOM [begin] - Escape the star ('*') character in a SIP URI.
// This variable can be used to escape the star ('*') character in a SIP URI.
//
// IMPORTANT : Escaping '*' character in SIP URI is not RFC 2396 compliant.
//
// Setting this variable to true will escape the star character with the %2A
// sequence in SIP URI.
//
//------------------------------------------------------------------------------
extern bool g_bESCAPE_STAR_CHARACTER_IN_SIP_URI;
// BRCM_CUSTOM [end] - Escape the star ('*') character in a SIP URI.

// BRCM_CUSTOM [begin] - Escape the star ('#') character in a SIP URI.
// This variable can be used to escape the pound ('#') character in an URI.
//
// IMPORTANT : *NOT* escaping '#' character in URI is not RFC 2396 compliant.
//
// Setting this variable to true will escape the pound character with the %23
// sequence in SIP URI.
//
//------------------------------------------------------------------------------
extern bool g_bESCAPE_POUND_CHARACTER_IN_URI;
// BRCM_CUSTOM [end] - Escape the star ('#') character in a SIP URI.


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================




//==SDOC========================================================================
//== Class: CToken
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//  This class handles parsing and storage for the basic token structure. The
//  token is the building block of almost all RFC 3261 ABNF. A token is like a
//  word. Spaces delimit tokens.
//
//  RFC 3261 declares a few tokens. One is used inside headers, others are used
//  within the SipUri. Each token has a different character set, meaning that
//  some characters are valid and others are not in the context of a token.
//  Refer to the ms_aTokenData array for a list of each token's character set.
//
//  Some tokens are case sensitive, and some can contain escaped values (such as
//  an escaped 'space' character: %20). This is also hardcoded in the
//  ms_aTokenData table.
//
//  This class implements all token types. The ECharSet tells CToken how to
//  handle parsing and serialization by giving it access to the correct
//  structure in the ms_aTokenData. Through it, the CToken knows which
//  characters are legal, which should be escaped, and how comparison should be
//  made (case sensitive or not). The charset is set at the CToken construction,
//  and will never change.
//
//  Special note: Upon Serialization, CToken automatically handles escaping.
//  Unescapable tokens are serialized as they are -- with no further validation.
//  It is thus possible for the user to set a token string that does not follow
//  RFC 3261 syntax for tokens that are not escapable.
//
// Location:
//   SipParser/CToken.h
//
//==EDOC========================================================================
class CToken : protected CString
{
// New types definition
//---------------------
public:
    // Supported charsets
    //-------------------
    enum ECharSet
    {
        // Tokens within a Sip Header context.
        //-------------------------------------
        eCS_SIP_HEADER = 0,

        // Tokens within a SipUri user name context.
        //-------------------------------------------
        eCS_SIPURI_USER,

        // Tokens within a SipUri password context.
        //------------------------------------------
        eCS_SIPURI_PASSWORD,

        // Tokens within a SipUri parameter context.
        //-------------------------------------------
        eCS_SIPURI_PARAM,

        // Tokens within a SipUri header context.
        //----------------------------------------
        eCS_SIPURI_HEADER,

        // Tokens within a TelUri telephone subscriber context.
        //------------------------------------------------------
        eCS_TELURI_TELEPHONE_SUBSCRIBER,

        // Tokens within a TelUri parameter context.
        //-------------------------------------------
        eCS_TELURI_PARAM,

        // Tokens within a SIP Header Parameter.
        //---------------------------------------
        eCS_SIPHEADER_PARAM,

        // Keep last.
        //------------
        eCS_LAST,
    };
protected:
    // This is the definition of one token context.
    //----------------------------------------------
    struct STokenCharacteristics
    {
        // Pointer to an array of 128 bools indexed by the character value.
        // This table says if a character is legal or not within a context.
        //------------------------------------------------------------------
        bool* m_pbGrammar;

        // Sets the token comparison case-sensitivity rule for this context.
        //-------------------------------------------------------------------
        bool m_bCaseSensitive;

        // Sets parsing/serializing escaping for illegal characters for this
        // context.
        //-------------------------------------------------------------------
        bool m_bCanBeEscaped;
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------


    // Summary:
    //  Constructor from a c-style string.
    //-------------------------------------
    CToken(IN ECharSet eCharSet, IN const char* szSrc = NULL);

    // Summary:
    //  Constructor from a c-style string and length.
    //------------------------------------------------
    CToken(IN ECharSet eCharSet, IN const char* szSrc, IN uint16_t uCount);

    // Summary:
    //  Constructor from a CString.
    //------------------------------
    CToken(IN ECharSet eCharSet, IN const CString& rstrSrc);

    // Summary:
    //  Copy constructor.
    //--------------------
    CToken(IN const CToken& rSrc);


    // Summary:
    //  Destructor.
    //--------------
    virtual ~CToken();

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN const CToken& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN const CString& rstrSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN const char* szSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN int32_t nValue);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN uint32_t uValue);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CToken& operator=(IN uint64_t uValue);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CToken& rSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CString& rstrSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const char* szSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator!=(IN const CToken& rSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator!=(IN const CString& rstrSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator!=(IN const char* szSrc) const;

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator<(IN const CToken& rSrc) const;

    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Updates the grammar of a given character set.
    static mxt_result SetParserGrammar(IN const unsigned int uIndex,
                                       IN const bool bValue,
                                       IN const ECharSet eCharSet);

    // Summary:
    //  Gets whether or not the specified element in the given character set
    //  is allowed.
    static mxt_result GetParserGrammar(IN const unsigned int uIndex,
                                       IN const ECharSet eCharSet,
                                       OUT bool& rbValue);

    // Summary:
    //  Returns the charset for which this token is configured.
    //----------------------------------------------------------
    ECharSet GetCharSet() const;

    // Summary:
    //  Returns the token's length.
    //------------------------------
    uint16_t Length() const;

    // Summary:
    //  Provides access to the token data.
    //-------------------------------------
    CString& GetString();

    // <COMBINE CToken::GetString>
    const CString& GetString() const;

    // Summary:
    //  Provides conversion from string to number.
    //---------------------------------------------
    mxt_result GetInt32(OUT int32_t& rnValue) const;

    // <COMBINE CToken::GetInt32@OUT int32_t&@const>
    mxt_result GetUint32(OUT uint32_t& ruValue) const;

    // Summary:
    //  Provides conversion from string to number.
    //---------------------------------------------
    mxt_result GetUint64(OUT uint64_t& ruValue) const;

    // <COMBINE CToken::GetUint16@OUT uint16_t&@const>
    mxt_result GetUint16(OUT uint16_t& ruValue) const;

    // Summary:
    //  Provides access to conversion of a float / double string to
    //  an unsigned integer and a multiplier.
    //-------------------------------------------------------------
    mxt_result GetFloat(OUT int& ruValue,
                        OUT unsigned int& ruMultiplicator) const;

    // Summary:
    //  Sets a float value into the token.
    //-------------------------------------
    mxt_result SetFloat(IN int nFloatValue, IN unsigned int uFloatMultiplier);

    // Summary:
    //  Returns true if the token is empty.
    //--------------------------------------
    bool IsEmpty() const;

    // Summary:
    //  Returns true if the token is alphabetic only.
    //------------------------------------------------
    bool IsAlpha() const;

    // Summary:
    //  Returns true if the token is hexadecimal only.
    //-------------------------------------------------
    bool IsHexadecimal() const;

    // Summary:
    //  Returns true if the token is numeric only.
    //---------------------------------------------
    bool IsNumeric() const;

    // Summary:
    //  Returns true if the token is a floating-point only.
    //------------------------------------------------------
    bool IsFloat() const;

    // Summary:
    //  Returns true if the token's context (based on the charset) allows
    //  whitespace.
    //--------------------------------------------------------------------
    static CStringHelper::EAllowLws IsLwsAllowed(IN ECharSet eCharSet);

    // Summary:
    //  Returns true if the token's context (based on the charset) allows
    //  whitespace.
    //--------------------------------------------------------------------
    CStringHelper::EAllowLws IsLwsAllowed() const;

    // Summary:
    //  Returns true if the token is an unsigned integer.
    //----------------------------------------------------
    bool IsUnsignedInteger() const;

    // Summary:
    //  Returns true if the token is an unsigned integer.
    //----------------------------------------------------
    bool IsUnsignedInteger(IN const char* szString) const;

    // Summary:
    //  Returns true if the token is a signed integer.
    //-------------------------------------------------
    bool IsSignedInteger() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from 0 to 65535.
    //--------------------------------------------------------------------
    bool IsUint16() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from 0 to
    //  4,294,967,295.
    //-------------------------------------------------------------
    bool IsUint32() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from -2,147,483,647 to
    //  +2,147,483,647. The sign is optional.
    //--------------------------------------------------------------------------
    bool IsInt32() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from 0 to
    //  +18,446,744,073,709,551,615.
    //--------------------------------------------------------------------------
    bool IsUint64() const;

    // Summary:
    //  Clears the token.
    //--------------------
    void Reset();

    // Summary:
    //  Returns true if 'ch' is part of the charset supported by this type of
    //  token.
    //------------------------------------------------------------------------
    bool IsValidChar(IN char c) const;

    // Summary:
    //  Returns a pointer to the first character that is not part of the
    //  charset.
    //-------------------------------------------------------------------
    const char* FindTokenEnd(IN const char* pcStart) const;

    // Summary:
    //  Parses a token up to the first character that is not part of the
    //  charset. Also skips LWS.
    //-------------------------------------------------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Outputs the token. Outputs escaped characters if the token type
    //  supports it.
    //-------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;


//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Verifies that the internal string is smaller or equal to the reference.
    //--------------------------------------------------------------------------
    bool IsUnsignedIntegerSmallerOrEqual(IN const char* szReference) const;

    // Summary:
    //  Verifies that the internal string is smaller or equal to the reference.
    //--------------------------------------------------------------------------
    bool IsSignedIntegerSmallerOrEqual(IN const char* szReference) const;

private:
    // Summary:
    //  Default Constructor.
    //-----------------------
    CToken();

//-- Hidden Data Members
//------------------------
protected:

    // Indicates which character set is supported by this token. This affects
    // the legal characters, escaping, and comparison.
    //------------------------------------------------------------------------
    ECharSet m_eCharSet;

    // This array contains the definition for all token supported character
    // sets. It is indexed by ECharSet.
    //----------------------------------------------------------------------
    static STokenCharacteristics ms_astTokenData[eCS_LAST];

    static bool ms_abCS_SIP_HEADER[128];
    static bool ms_abCS_SIPURI_USER[128];
    static bool ms_abCS_SIPURI_PASSWORD[128];
    static bool ms_abCS_SIPURI_PARAM[128];
    static bool ms_abCS_SIPURI_HEADER[128];
    static bool ms_abCS_TELURI_TELEPHONE_SUBSCRIBER[128];
    static bool ms_abCS_SIP_HEADER_PARAM[128];
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetCharSet
//==
//==============================================================================
//
//  Returns:
//      The character set for which this token has been created.
//
//  Description:
//      Provides access to the character set that this token implements. The
//      charset is specified at construction time, and will never change
//      throughout the life of the token.
//
//==EDOC========================================================================
inline CToken::ECharSet CToken::GetCharSet() const
{
    return m_eCharSet;
}

//==SDOC========================================================================
//==
//==  Length
//==
//==============================================================================
//
//  Returns:
//      Returns the length of the contained token.
//
//  Description:
//      Provides the length of the contained token.
//
//==EDOC========================================================================
inline uint16_t CToken::Length() const
{
    return GetSize();
}

//==SDOC========================================================================
//==
//==  GetString
//==
//==============================================================================
//
//  Returns:
//      The contained token.
//
//  Description:
//      Provides access to the token data.
//
//==EDOC========================================================================
inline CString& CToken::GetString()
{
    return *this;
}

//==SDOC========================================================================
//==
//==  GetString
//==
//==============================================================================
//
//  Returns:
//      The contained token.
//
//  Description:
//      Provides access to the token data.
//
//==EDOC========================================================================
inline const CString& CToken::GetString() const
{
    return *this;
}

//==SDOC========================================================================
//==
//==  IsValidChar
//==
//==============================================================================
//
//  Parameters:
//    ch:
//      Character to validate.
//
//  Returns:
//      True if 'ch' is part of the character set supported by this token.
//
//  Description:
//      This method provides a way to verify if a certain character is legal for
//      the token charset.
//
//==EDOC========================================================================
inline bool CToken::IsValidChar(IN char c) const
{
    if( g_bESCAPE_STAR_CHARACTER_IN_SIP_URI &&
        (c == '*') && (m_eCharSet == eCS_SIPURI_USER) )
    {
        return false;
    }
    else if( !g_bESCAPE_POUND_CHARACTER_IN_URI &&
        (c == '#') && (m_eCharSet == eCS_SIPURI_USER) )
    {
        return true;
    }
    else
    {
        return ms_astTokenData[m_eCharSet].m_pbGrammar[static_cast<unsigned char>(c)];
    }
}

//==SDOC========================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      True if the token is empty.
//
//  Description:
//      Returns true if the token is empty.
//
//==EDOC========================================================================
inline bool CToken::IsEmpty() const
{
    return CString::IsEmpty();
}

//==SDOC========================================================================
//==
//==  IsLwsAllowed
//==
//==============================================================================
//
//  Returns:
//      Returns CStringHelper::eALLOW_LWS if the token's context (based on the
//      charset) allows whitespace.
//
//  Description:
//      LWS is allowed only within SIP Headers and SIP Header Parameters.
//
//==EDOC========================================================================
inline CStringHelper::EAllowLws CToken::IsLwsAllowed() const
{
    // LWS is allowed only within SIP Headers and SIP Header Parameters.
    //-------------------------------------------------------------------
    return IsLwsAllowed(m_eCharSet);
}

//==SDOC========================================================================
//==
//==  IsLwsAllowed
//==
//==============================================================================
//
//  Parameters:
//    eCharSet:
//      Character set to check.
//
//  Returns:
//      Returns CStringHelper::eALLOW_LWS if the token's context (based on the
//      charset) allows whitespace.
//
//  Description:
//      LWS is allowed only within SIP Headers and SIP Header Parameters.
//
//==EDOC========================================================================
inline CStringHelper::EAllowLws CToken::IsLwsAllowed(IN ECharSet eCharSet)
{
    // LWS is allowed only within SIP Headers and SIP Header Parameters.
    //-------------------------------------------------------------------
    return eCharSet == eCS_SIP_HEADER || eCharSet == eCS_SIPHEADER_PARAM ?
                                                    CStringHelper::eALLOW_LWS :
                                                    CStringHelper::eNO_LWS;
}

//==SDOC========================================================================
//==
//==  Reset
//==
//==============================================================================
//
//  Description:
//      Clears the internal data (the token string). Does not affect the
//      character set.
//
//==EDOC========================================================================
inline void CToken::Reset()
{
    EraseAll();
}


//==SDOC========================================================================
//==
//==  IsAlpha
//==
//==============================================================================
//
//  Returns:
//      Returns true if the token is alphabetic only. An empty string is
//      considered as not being alphabetic.
//
//  Description:
//      Checks whether or not the token is made up exclusively of alphabetic
//      characters.
//
//  See Also:
//      CStringHelper::IsAlpha
//
//  Example:
//  <CODE>
//    "abcdeFRD" is an alphabetic string.
//    "abc3deFRD" is not an alphabetic string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsAlpha() const
{
    return CStringHelper::IsAlpha(CStr());
}

//==SDOC========================================================================
//==
//==  IsHexadecimal
//==
//==============================================================================
//
//  Returns:
//      Returns true if the token is hexadecimal only. An empty string is
//      considered as not being hexadecimal.
//
//  Description:
//      Checks whether or not the token is made up exclusively of hexadecimal
//      characters.
//
//  See Also:
//      CStringHelper::IsHexadecimal
//
//  Example:
//  <CODE>
//    "abcdeF1234567890" is an hexadecimal string.
//    "abcdeFG1234567890" is not an hexadecimal string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsHexadecimal() const
{
    return CStringHelper::IsHexadecimal(CStr());
}

//==SDOC========================================================================
//==
//==  IsNumeric
//==
//==============================================================================
//
//  Returns:
//      Returns true if the token is numeric. An empty string is
//      considered as not being numeric.
//
//  Description:
//      Checks whether or not the token is made up exclusively of numeric
//      characters.
//
//  See Also:
//      CStringHelper::IsNumeric
//
//  Example:
//  <CODE>
//    "1234567890" is a numeric string.
//    "1234567890A" is not a numeric string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsNumeric() const
{
    return CStringHelper::IsNumeric(CStr());
}

//==SDOC========================================================================
//==
//==  IsFloat
//==
//==============================================================================
//
//  Returns:
//      Returns true if the token is a floating-point. An empty string is
//      considered as not being floating-point.
//
//  Description:
//      Checks whether or not the token is made up exclusively of floating-point
//      characters.
//
//  See Also:
//      CStringHelper::IsFloat
//
//  Example:
//  <CODE>
//    "1234567890" is NOT a floating-point string.
//    "1234567890.1" is a floating-point string.
//    "1234567890A.1" is NOT a floating-point string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsFloat() const
{
    return CStringHelper::IsFloat(CStr());
}

//==SDOC========================================================================
//==
//==  IsUnsignedInteger
//==
//==============================================================================
//
//  Returns:
//      Returns true if the token is an unsigned integer. An empty string is
//      considered as not being an unsigned integer.
//
//  Description:
//      Checks whether or not the token is made up exclusively of unsigned
//      integer characters (digits).
//
//  See Also:
//      CStringHelper::IsDigit
//
//  Example:
//  <CODE>
//    "1234567890" is an unsigned integer string.
//    "-1234567890" is not an unsigned integer string.
//    "+1234567890" is not an unsigned integer string.
//    "1234567890A" is not an unsigned integer string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsUnsignedInteger() const
{
    return IsUnsignedInteger(CStr());
}


//==SDOC========================================================================
//==
//==  IsUnsignedInteger
//==
//==============================================================================
//
//  Parameters:
//    szString:
//      String to check.
//
//  Returns:
//      Returns true if the token is an unsigned integer. An empty string is
//      considered as not being an unsigned integer.
//
//  Description:
//      Checks whether or not the token is made up exclusively of unsigned
//      integer characters (digits).
//
//  See Also:
//      CStringHelper::IsDigit
//
//  Example:
//  <CODE>
//    "1234567890" is an unsigned integer string.
//    "-1234567890" is not an unsigned integer string.
//    "+1234567890" is not an unsigned integer string.
//    "1234567890A" is not an unsigned integer string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsUnsignedInteger(IN const char* szString) const
{
    return CStringHelper::IsDigit(szString);
}


//==SDOC========================================================================
//==
//==  IsInt32
//==
//==============================================================================
//
//  Returns:
//      True if the token is a signed integer between -2,147,483,647 and
//      +2,147,483,647 inclusively.
//
//  Description:
//      Checks whether or not the token is a 4 byte signed int.
//
//  See Also:
//      IsInt32, IsSignedInteger
//
//  Example:
//  <CODE>
//    "-2147483647" is a 32 bit signed integer string.
//    "+2147483647" is a 32 bit signed integer string.
//    "2147483647" is a 32 bit signed integer string.
//    "+-2147483647" is not a 32 bit signed integer string.
//    "12345678900000546547987" is not a 32 bit signed integer string.
//    "AAA123" is not a 32 bit signed integer string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsInt32() const
{
    // Maximum 32 bit unsigned value.
    //--------------------------------
    const char* const szMAX_32_BIT_SIGNED_VAL = "2147483647";

    // Int32_t. -2,147,483,647 to +2,147,483,647 inclusively.
    //--------------------------------------------------------
    return IsSignedInteger() &&
           IsSignedIntegerSmallerOrEqual(szMAX_32_BIT_SIGNED_VAL);
}


//==SDOC========================================================================
//==
//==  IsUint16
//==
//==============================================================================
//
//  Returns:
//      True if the token is an unsigned between 0 and 65535 inclusively.
//
//  Description:
//      Checks whether or not the token is a 2 byte unsigned int.
//
//  See Also:
//      IsUint32, IsUnsignedInteger
//
//  Example:
//  <CODE>
//    "65535" is a 16 bit unsigned integer string.
//    "-4000" is not a 16 bit unsigned integer string.
//    "+4000" is not a 16 bit unsigned integer string.
//    "1234567890" is not a 16 bit unsigned integer string.
//  </CODE>
//
//==EDOC========================================================================
//==EDOC========================================================================
inline bool CToken::IsUint16() const
{
    // Maximum 16 bit unsigned value.
    //--------------------------------
    const char* const szMAX_16_BIT_UNSIGNED_VAL = "65535";

    // uint16_t. 0 To 65,535 inclusively.
    //------------------------------------
    return IsUnsignedInteger() &&
           IsUnsignedIntegerSmallerOrEqual(szMAX_16_BIT_UNSIGNED_VAL);
}

//==SDOC========================================================================
//==
//==  IsUint32
//==
//==============================================================================
//
//  Returns:
//      True if the token is an unsigned between 0 and 4,294,967,295
//      inclusively.
//
//  Description:
//      Checks whether or not the token is a 4 byte unsigned int.
//
//  See Also:
//      IsUint16, IsUnsignedInteger
//
//  Example:
//  <CODE>
//    "4294967295" is a 32 bit unsigned integer string.
//    "-4000" is not a 32 bit unsigned integer string.
//    "+4000" is not a 32 bit unsigned integer string.
//    "12345678900000546547987" is not a 32 bit unsigned integer string.
//  </CODE>
//
//==EDOC========================================================================
inline bool CToken::IsUint32() const
{
    // Maximum 32 bit unsigned value.
    //--------------------------------
    const char* const szMAX_32_BIT_UNSIGNED_VAL = "4294967295";

    // uint32_t. 0 to 4,294,967,295 inclusively.
    //-------------------------------------------
    return IsUnsignedInteger() &&
           IsUnsignedIntegerSmallerOrEqual(szMAX_32_BIT_UNSIGNED_VAL);
}


//==SDOC========================================================================
//==
//==  IsUint64
//==
//==============================================================================
//
//  Returns:
//      True if the token is a unsigned integer between 0 to
//      +18,446,744,073,709,551,615 inclusively.
//
//  Description:
//      Checks if the token is a 8 byte unsigned int.
//
//==EDOC========================================================================
inline bool CToken::IsUint64() const
{
    // Maximum 64 bit unsigned value.
    //--------------------------------
    const char* const szMAX_64_BIT_UNSIGNED_VAL = "18446744073709551616";

    // uint64_t. 0 to +18,446,744,073,709,551,615 inclusively.
    //--------------------------------------------------------
    return IsUnsignedInteger() &&
           IsSignedIntegerSmallerOrEqual(szMAX_64_BIT_UNSIGNED_VAL);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTOKEN_H

