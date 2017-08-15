//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CTXTTOKEN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTTOKEN_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_TXTMESSAGECONSTANTS_H
#include "TextMessage/TxtMessageConstants.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
// The grammar is a table of 129 boolean entries in which each index represents
// one of the US-ASCII character and the 129th indicates acceptance of non
// US-ASCII UTF-8 characters within the token value.
const unsigned int g_uGRAMMAR_SIZE = 129;
//M5T_INTERNAL_USE_END

//<GROUP TEXT_MESSAGE_VARIABLES>
//
// Summary:
// These two constants provide the values to use in the grammar array.
//
// Description:
// These two constants provides the values to use in the grammar array.
const uint8_t g_uGRAMMAR_ILLEGAL_VALUE = 0x00;

//<GROUP TEXT_MESSAGE_VARIABLES>
//<COMBINE g_uGRAMMAR_ILLEGAL_VALUE>
const uint8_t g_uGRAMMAR_LEGAL_VALUE   = 0x01;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtToken
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 0>
//
// Summary:
//  This class handles parsing and storage of a basic token structure.
//
// Description:
//  This class handles parsing and storage for a basic token structure. A token
//  is the building block of almost all text-based IETF protocols. A token is
//  like a word. Spaces or special characters delimit tokens and are not allowed
//  within it.
//
//  In order to support a wide range of grammar, this class must be configured
//  with a structure that defines the grammar of the token. The grammar is a
//  table of g_uGRAMMAR_SIZE (129) byte entries in which each index
//  represents one of the US-ASCII character and the 129th indicates acceptance
//  of non US-ASCII UTF-8 characters within the token value. Each entry is
//  either 0x00 or 0x01, 0x00 when the character is illegal and 0x01 is legal. A
//  token also has some particular characteristics that depends on the protocol
//  context, those and the grammar are described in the STokenCharacteristics
//  structure. The following example shows how one would configure the CTxtToken
//  class to handle tokens that can be found in SIP header values.
//
//  Note that the values 0x00 and 0x01 can be replaced with the named constants:
//  g_uGRAMMAR_ILLEGAL_VALUE and g_uGRAMMAR_LEGAL_VALUE respectively.
//
//  <CODE>
//  // ABNF from RFC 3261:
//  // token     =  1*(alphanum / "-" / "." / "!" / "%" / "*" / "_" / "+" /
//  //              "`" / "'" / "~")
//  // alphanum  =  ALPHA / DIGIT
//  // ALPHA     =  %x41-5A / %x61-7A   ; A-Z / a-z
//  // DIGIT     =  %x30-39             ; 0-9
//  const uint8_t g_auCS_SIP_HEADER_VALUE_TOKEN[g_uGRAMMAR_SIZE] =
//  {
//  // NUL,  SOH,  STX,  ETX,  EOT,  ENQ,  ACK,  BEL,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  // BS,   HT,   LF,   VT,   FF,   CR,   SO,   SI,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  // DLE,  DC1   DC2,  DC3,  DC4,  NAK,  SYN,  ETB,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  // CAN,  EM,   SUB,  ESC,  FS,   GS,   RS,   US,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  // SP    !,    ",    #,    $,    %,    &,    ',
//     0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
//  // (,    ),    *,    +,    ,     -,    .,    /,
//     0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00,
//  // 0,    1,    2,    3,    4,    5,    6,    7,    8,    9,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  // :,    ;,    <,    =,    >,    ?,    @,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  // A,    B,    C,    D,    E,    F,    G,    H,    I,    J,    K,    L,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  // M,    N,    O,    P,    Q,    R,    S,    T,    U,    V,    W,    X,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  // Y,    Z,    [,    \,    ],    ^,    _,    `,
//     0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
//  // a,    b,    c,    d,    e,    f,    g,    h,    i,    j,    k,    l,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  // m,    n,    o,    p,    q,    r,    s,    t,    u,    v,    w,    x,
//     0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
//  // y,    z,    {,    |,    },    ~,    DEL,  UTF-8
//     0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00
//  };
//
//  const CTxtToken::STokenCharacteristics g_stSIP_HEADER_TOKEN =
//  {
//     //Grammar
//     g_abCS_SIP_HEADER_VALUE_TOKEN,
//
//     //Can be escaped
//     false,
//
//     //Case Sensitive
//     false,
//
//     //Escape Mechanism
//     CTxtToken::eESCAPE_NONE,
//
//     //Allow LWS
//     true
//  };
//  </CODE>
//
//  #Note:#
//     - Upon Serialization, CTxtToken automatically handles escaping.
//       Unescapable tokens are serialized as they are -- with no further
//       validation. It is thus possible for the user to set a token string that
//       does not follow the established grammar for tokens that are not
//       escapable.
//     - Currently a token CANNOT contain the '\\0' character followed by other
//       characters and a token grammar SHOULD NEVER set this character as a
//       valid one.
//     - When a token is escapable the character that starts the escape sequence
//       MUST NOT be a valid character in the grammar.
//
// Location:
//   TextMessage/CTxtToken.h
//
//==============================================================================
class CTxtToken
{
//-- Published data types
public:

    // Summary:
    //  This enumerates the escaping mechanisms supported by the token.
    //
    // Description:
    //  This enumerates the escaping mechanisms supported by the token.
    enum EEscapeMechanism
    {
        // Description:
        // No escaping is performed.
        eESCAPE_NONE,

        // Description:
        // Escaping is performed using the "%XX" form, where X is an hexadecimal
        // digit (0-9, A-F) and XX is the hexadecimal value of the byte being
        // escaped. For example, this is the scheme used in SIP URIs.
        eESCAPE_PERCENT_HEX,

        // Description:
        // Escaping is performed using the "\C" and "\uXXXX" form, where C is
        // one of \\, ', ", b, t, n, r and X is an hexadecimal digit and XXXX is
        // the UCS-2 encoding of the character being escaped. For example, this
        // is the scheme used in CPIM.
        //
        // #Notes:#
        // The token does not directly interpret or generate UCS-2 values. It
        // can only handle the US-ASCII range of the UCS-2 encoding.
        eESCAPE_BACKSLASH_CPIM
    };

    // Summary:
    //  This defines the characteristics for one type of token.
    //
    // Description:
    //  This defines the characteristics for one type of token.
    struct STokenCharacteristics
    {
        // Description:
        // Pointer to an array of g_uGRAMMAR_SIZE byte values indexed by the
        // character value. This table says if a character is legal (0x01) or
        // not (0x00) within a token context.
        //
        // The last entry of this table specifies if the grammar allows for
        // non US-ASCII UTF-8 characters.
        const uint8_t* m_puGrammar;

        // Description:
        // Sets the token comparison case-sensitivity rule for this context.
        bool m_bCaseSensitive;

        // Description:
        // Sets parsing/serializing escaping for illegal characters for this
        // context.
        bool m_bCanBeEscaped;

        // Description:
        // Sets the escape mechanism for illegal characters. This member is
        // ignored when m_bCanBeEscaped is false.
        EEscapeMechanism m_eEscapeMechanism;

        // Description:
        // Configures whether or not it is possible to have whitespace (LWS)
        // after the token. Used only when parsing.
        bool m_bAllowWhitespace;
    };

//-- Published Interface
public:
    //-- << Constructors / Destructors >>

    // Summary:
    //  Constructor from a c-style string.
    CTxtToken(IN const STokenCharacteristics* pTokenCfg,
              IN const char* pszSrc,
              IN IAllocator* pAllocator = NULL);

    //<COMBINE CTxtToken::CTxtToken@IN const STokenCharacteristics*@IN const char*@IN IAllocator*>
    CTxtToken(IN const STokenCharacteristics* pTokenCfg,
              IN const char* pszSrc,
              IN uint16_t uCount,
              IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy constructor.
    CTxtToken(IN const CTxtToken& rSrc);

    // Summary:
    //  Default constructor. Token MUST later be initialized with Initialize.
    CTxtToken(IN const STokenCharacteristics* pTokenCfg = NULL,
              IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Destructor.
    virtual ~CTxtToken();

    //-- << Initializer >>

    // Summary:
    //  Initializes a token that was created through the default constructor.
    mxt_result Initialize(IN const STokenCharacteristics* pTokenCfg, IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Sets an allocator on the token.
    mxt_result SetAllocator(IN IAllocator* pAllocator);

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CTxtToken& operator=(IN const CTxtToken& rSrc);

    // Summary:
    //  Assignment Operator.
    CTxtToken& operator=(IN const char* pszSrc);

    //<COMBINE CTxtToken::=@IN const char*>
    CTxtToken& operator=(IN int32_t nValue);

    //<COMBINE CTxtToken::=@IN const char*>
    CTxtToken& operator=(IN uint32_t uValue);

    // Summary:
    //  Comparison operator.
    bool operator==(IN const CTxtToken& rSrc) const;

    //<COMBINE CTxtToken::==@IN const CTxtToken&@const>
    bool operator==(IN const char* pszSrc) const;

    // Summary:
    //  Comparison operator.
    bool operator!=(IN const CTxtToken& rSrc) const;

    //<COMBINE CTxtToken::!=@IN const CTxtToken&@const>
    bool operator!=(IN const char* pszSrc) const;

    // Summary:
    //  Comparison operator.
    bool operator<(IN const CTxtToken& rSrc) const;

    //-- << Accessors >>

    // Summary:
    //  Returns the token's length.
    uint16_t GetLength() const;

    // Summary:
    //  Provides conversion from string to number.
    mxt_result GetInt32(OUT int32_t& rnValue) const;

    //<COMBINE CTxtToken::GetInt32@OUT int32_t&@const>
    mxt_result GetUint32(OUT uint32_t& ruValue) const;

    //<COMBINE CTxtToken::GetInt32@OUT int32_t&@const>
    mxt_result GetUint16(OUT uint16_t& ruValue) const;

    //<COMBINE CTxtToken::GetInt32@OUT int32_t&@const>
    mxt_result GetFloat(OUT int& rnValue,
                        OUT unsigned int& ruMultiplicator) const;

    // Summary:
    //  Returns true if the token is empty.
    bool IsEmpty() const;

    // Summary:
    //  Returns true if the token is alphabetic only.
    bool IsAlpha() const;

    // Summary:
    //  Returns true if the token only contains alphabetic or numeric characters.
    bool IsAlphaNum() const;

    // Summary:
    //  Returns true if the token is hexadecimal only.
    bool IsHexadecimal() const;

    // Summary:
    //  Returns true if the token is numeric only.
    bool IsNumeric() const;

    // Summary:
    //  Returns true if the token is a floating-point only.
    bool IsFloat() const;

    // Summary:
    //  Returns true if the token only contains numeric characters (0-9) and
    //  possibly a leading sign ('-' or '+').
    bool IsDigit(IN bool bSkipLeadingPlus = true, IN bool bSkipLeadingMinus = true) const;

    // Summary:
    //  Returns true if the token's context (based on the charset) allows
    //  whitespace.
    bool IsLwsAllowed() const;

    // Summary:
    //  Returns true if the token is an unsigned integer.
    bool IsUnsignedInteger() const;

    // Summary:
    //  Returns true if the token is a signed integer.
    bool IsSignedInteger() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from 0 to 65535.
    bool IsUint16() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from 0 to
    //  4,294,967,295.
    bool IsUint32() const;

    // Summary:
    //  Returns true if the token is an unsigned integer from -2,147,483,647 to
    //  +2,147,483,647. The sign is optional.
    bool IsInt32() const;

    // Summary:
    //  Returns true if 'c' is part of the charset supported by this type of
    //  token.
    bool IsValidCharacter(IN char c) const;

    // Summary:
    //  Returns a pointer to the first character that is not part of the
    //  charset.
    const char* FindTokenEnd(IN const char* pcStart) const;

    // Summary:
    //  Provides access to the token data.
    const char* GetString() const;

    //-- << Accessors-Modifiers >>

    // Summary:
    //  Assigns the internal string directly.
    void SetString(IN const char* pszString);

    // Summary:
    //  Assigns the internal string directly.
    void SetString(IN const char* pszString, IN uint16_t uCount);

    // Summary:
    //  Sets a float value into the token.
    mxt_result SetFloat(IN int nFloatValue, IN unsigned int uFloatMultiplier);

    // Summary:
    //  Clears the token.
    void Reset();

    // << Parsing >>

    // Summary:
    //  Parses a token up to the first character that is not part of the
    //  charset. Also skips LWS if allowed by the grammar.
    mxt_result Parse(INOUT const char*& rpcPos);

    // << Serialization >>

    // Summary:
    //  Appends the token's data in the provided blob. Illegal characters are
    //  escaped if the token supports it.
    void Serialize(INOUT CBlob& rBlob) const;

//M5T_INTERNAL_USE_BEGIN
    // << Public Helpers >>

    // Summary:
    //  Skips over WSP constructs.
    static unsigned int SkipWspHelper(INOUT const char*& rpcPos);

    // Summary:
    //  Indicates if the parameter points to a LWS construct.
    static unsigned int IsLwsHelper(IN const char* pcPos);

    // Summary:
    //  Skips over LWS constructs.
    static mxt_result SkipLwsHelper(INOUT const char*& rpcPos);
//M5T_INTERNAL_USE_END

//-- Hidden Methods
protected:
private:
    //-- << Helpers >>

    // Summary:
    //  Calls isdigit on the given string until is fails.
    void IsDigitHelper(INOUT const char*& rpszString) const;

    // Summary:
    //  Compares the unsigned integer value of two strings.
    bool IsUnsignedIntegerSmallerOrEqualHelper(IN const char* pszReference) const;

    // Summary:
    //  Compares the signed integer value of two strings.
    bool IsSignedIntegerSmallerOrEqualHelper(IN const char* pszReference) const;

    // Summary:
    //  Indicates if the provided character is UTF-8 non US-ASCII.
    bool IsUtf8NonAsciiHelper(IN const char c) const;

    // Summary:
    //  Indicates if the provided character is escaped.
    bool IsEscapedHelper(IN const char* pcPos) const;

    // Summary:
    //  Returns the unescaped character corresponding to the escape sequence
    //  pointed by rpcPos.
    char GetEscapedCharacterHelper(INOUT const char*& rpcPos) const;

    // Summary:
    //  Converts a character from escaped to unescaped form.
    char UnescapeCharacterHelper(INOUT const char*& rpcPos);

    // Summary:
    //  Converts a character from unescaped to escaped form.
    void EscapeCharacterHelper(IN char c, INOUT CBlob& rBlob) const;

    // Summary:
    //  Allocates memory for a C-style string.
    char* GO AllocateTokenString(IN uint16_t uLength);

    // Summary:
    //  Releases memory for the C-style string.
    void ReleaseTokenString(IN TOA char* pszString);

//-- Hidden Data Members
protected:
private:

    // This is the allocator that the token must use to allocate memory. If it
    // is NULL, the token falls back to MX_NEW/MX_DELETE.
    IAllocator* m_pAllocator;

    // This is the token text.
    char* m_pszToken;

    // This is the token text length.
    uint16_t m_uTextLength;

    // This is the caracteristics of the tokens handled by the current instance.
    const STokenCharacteristics* m_pstTokenCharacteristics;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
// Parameters:
//     rSrc:
//         The token against which equality is tested. Only the content is
//         compared.
//
//     pszSrc:
//         The value against which the token's content is tested for equality.
//         Note that NULL strings are considered equal to empty strings.
//
// Returns:
//     - true: the tokens are identical.
//     - false: the tokens are different.
//
// Description:
//     This method tests the parameter and the token's content for equality.
//
// Warning:
//     The token characteristics MUST have been assigned (through construction
//     or the Initialize method). If it is not the case, an assertion will fail
//     and false will always be returned.
//
//==============================================================================
inline bool CTxtToken::operator==(IN const CTxtToken& rSrc) const
{
    return *this == rSrc.m_pszToken;
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
// Parameters:
//     rSrc:
//         The token against which inequality is tested. Only the content is
//         compared.
//
//     pszSrc:
//         The value against which the token's content is tested for inequality.
//
// Returns:
//     - true: the tokens are different.
//     - false: the tokens are identical.
//
// Description:
//     This method tests the parameter and the token's content for inequality.
//
// Warning:
//     The token characteristics MUST have been assigned (through construction
//     or the Initialize method). If it is not the case, an assertion will fail
//     and true will always be returned.
//
//==============================================================================
inline bool CTxtToken::operator!=(IN const CTxtToken& rSrc) const
{
    return !(*this == rSrc.m_pszToken);
}

//--<COMBINE CTxtToken::!=@IN const CTxtToken&@const>
inline bool CTxtToken::operator!=(IN const char* pszSrc) const
{
    return !(*this == pszSrc);
}

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetLength
//==
//==============================================================================
//
// Returns:
//     The token's length.
//
// Description:
//     Returns the token's length.
//
//==============================================================================
inline uint16_t CTxtToken::GetLength() const
{
    return m_uTextLength;
}

//==============================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
// Returns:
//     - true: the token is empty.
//     - false: the token is not empty.
//
// Description:
//     This method indicates whether or not the token is empty.
//
//==============================================================================
inline bool CTxtToken::IsEmpty() const
{
    return m_pszToken == NULL ||
           strcmp(m_pszToken, g_pszEMPTY) == 0;
}

//==============================================================================
//==
//==  IsNumeric
//==
//==============================================================================
//
// Returns:
//     - true: the token only contains a numeric value.
//     - false: otherwise.
//
// Description:
//     This method checks whether or not the token contains a numeric value.
//
//==============================================================================
inline bool CTxtToken::IsNumeric() const
{
    return IsDigit() || IsFloat();
}

//==============================================================================
//==
//==  IsUnsignedInteger
//==
//==============================================================================
//
// Returns:
//     - true: the token is an unsigned integer.
//     - false: the token is not an unsigned integer, it might not be a number
//              at all.
//
// Description:
//     This method indicates if the token is an unsigned integer.
//
//==============================================================================
inline bool CTxtToken::IsUnsignedInteger() const
{
    // Skip a leading '+' but not a leading '-'.
    return IsDigit(true, false);
}

//==============================================================================
//==
//==  IsSignedInteger
//==
//==============================================================================
//
// Returns:
//     - true: the token is a signed integer.
//     - false: the token is not a signed integer, it might not be a number
//              at all.
//
// Description:
//     This method indicates if the token is a signed integer.
//
//==============================================================================
inline bool CTxtToken::IsSignedInteger() const
{
    return IsDigit();
}

//==============================================================================
//==
//==  GetString
//==
//==============================================================================
//
// Returns:
//     A pointer on the internal C-style string of the token. Can be NULL.
//
// Description:
//     This method allows access to the internal C-style string of the token.
//
//==============================================================================
inline const char* CTxtToken::GetString() const
{
    return m_pszToken;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTTOKEN_H

