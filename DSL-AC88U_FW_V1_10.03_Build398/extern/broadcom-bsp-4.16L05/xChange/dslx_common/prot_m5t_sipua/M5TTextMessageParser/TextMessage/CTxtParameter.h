//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CTXTPARAMETER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTPARAMETER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CTXTTOKEN_H
#include "TextMessage/CTxtToken.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtParameter
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 6>
//
// Summary:
//  This class handles parsing and storage of a generic parameter structure.
//
// Description:
//  This class handles parsing and storage of a generic parameter structure.
//  The parameter general format is described in the ABNF below, this ABNF
//  is inspired by RFC 3261.
//
// <CODE>
// param         = param-name [LWS] [ "=" [LWS] param-value [LWS] ]
//
// param-name    = token
// token         = 1*( ALPHA / DIGIT /
//                     "%" / "-" / "." / "!" / "*" / "_" / "+" / "`" / "'" /
//                     "~" )
//
// param-value   = quoted-string / x-token
// quoted-string = '"' *( x-token ) '"'
// x-token       = 1*( ALPHA / DIGIT /
//                     "-" / "." / "!" / "_" / "+" / "`" / "'" / "~" / "[" /
//                     "%" / "]" / "/" / ":" / "&" / "$" / "(" / ")" )
// </CODE>
//
//  The CTxtParameter does not interpret the value, it simply stores it. It is
//  up to the protocol implementation to evaluate if the value is suited for its
//  intended usage. This can be done at parse time by overriding the Parse
//  method or at other time through other means.
//
//  A Parse method override would first call the base class implementation to
//  separate name and value and would then continue parsing the value to
//  validate that it corresponds to what can be expected has a value for the
//  particular parameter at hand.
//
//  Parsing in the base class is very large and permissive to try and accomodate
//  all protocol implementations that might be built over the Text Message
//  Parser. The protocol implementation ought to subclass this class to provide
//  stricter parsing.
//
//  The data is internally stored in tokens that are initialized to generic
//  characteristics large enough to accept most constructs. A subclass can
//  change those characteristics using the SetTokenCharacteristics method of
//  this class.
//
// Location:
//   TextMessage/CTxtParameter.h
//
//==============================================================================
class CTxtParameter
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CTxtParameter(IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CTxtParameter(IN const CTxtParameter& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtParameter();

    // Summary:
    //  Assignment Operator.
    CTxtParameter& operator=(IN const CTxtParameter& rSrc);

    // Summary:
    //  Equality Operator.
    bool operator==(IN const CTxtParameter& rSrc);

    // Summary:
    //  Inequality Operator.
    bool operator!=(IN const CTxtParameter& rSrc);

    //-- << Accessors >>

    // Summary:
    //  Provides access to the parameter's name.
    CTxtToken& GetName();

    //<COMBINE CTxtParameter::GetName>
    const CTxtToken& GetName() const;

    // Summary:
    //  Provides access to the parameter's value.
    CTxtToken& GetValue();

    //<COMBINE CTxtParameter::GetValue>
    const CTxtToken& GetValue() const;

    // Summary:
    //  Indicates if the value of this parameter is a quoted-string.
    bool IsQuotedString() const;

    //-- << Modifiers >>

    // Summary:
    //  Changes the characteristics used in the internal tokens.
    void SetTokenCharacteristics(IN const CTxtToken::STokenCharacteristics* pstNameCharacteristics,
                                 IN const CTxtToken::STokenCharacteristics* pstValueCharacteristics = NULL);

    // Summary:
    //  Sets the parameter.
    void SetParameter(IN CTxtToken& rName, IN CTxtToken& rValue);

    //<COMBINE CTxtParameter::SetParameter@IN CTxtToken&@IN CTxtToken&>
    void SetParameter(IN const char* pszName, IN const char* pszValue);

    // Summary:
    //  Sets the parameter name.
    void SetName(IN CTxtToken& rName);

    //<COMBINE CTxtParameter::SetName@IN CTxtToken&>
    void SetName(IN const char* pszName, IN unsigned int uLength = 0);

    // Summary:
    //  Sets the parameter value.
    void SetValue(IN CTxtToken& rValue);

    //<COMBINE CTxtParameter::SetValue@IN CTxtToken&>
    void SetValue(IN const char* pszValue, IN unsigned int uLength = 0);

    // Summary:
    //  Sets the parameter value as a quoted string.
    void SetQuotedStringValue(IN CTxtToken& rValue);

    //<COMBINE CTxtParameter::SetQuotedStringValue@IN CTxtToken&>
    void SetQuotedStringValue(IN const char* pszValue, IN unsigned int uLength = 0);

    // Summary:
    //  Resets the parameter.
    virtual void Reset();

    //-- << Parsing >>

    // Summary:
    //  Parses the parameter, including name and optional value.
    virtual mxt_result Parse(INOUT const char*& rpszParameterText);

    //-- << Serialization >>

    // Summary:
    //  Outputs the parameter into the provided blob.
    void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:

    // This is the allocator that the parameter must use to allocate memory.
    IAllocator* m_pAllocator;

private:

    // This is the parameter's name.
    CTxtToken m_tokName;

    // This is the parameter's value.
    CTxtToken m_tokValue;

    // This indicates if the value must be serialized has a quoted string.
    bool m_bQuotedValue;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The parameter to which this one is compared.
//
//  Returns:
//      - true: this and rSrc are different.
//      - false: this and rSrc are equivalent.
//
//  Description:
//      Inequality Operator.
//
//==============================================================================
inline bool CTxtParameter::operator!=(IN const CTxtParameter& rSrc)
{
    return !(*this == rSrc);
}

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetName
//==
//==============================================================================
//
//  Returns:
//      The parameter name.
//
//  Description:
//      Returns the name of the parameter.
//
//==============================================================================
inline CTxtToken& CTxtParameter::GetName()
{
    return m_tokName;
}

//--<COMBINE CTxtParameter::GetName>
inline const CTxtToken& CTxtParameter::GetName() const
{
    // Const cast for code reuse.
    return const_cast<CTxtParameter*>(this)->GetName();
}

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      The parameter value.
//
//  Description:
//      Returns the value of the parameter.
//
//==============================================================================
inline CTxtToken& CTxtParameter::GetValue()
{
    return m_tokValue;
}

//--<COMBINE CTxtParameter::GetValue>
inline const CTxtToken& CTxtParameter::GetValue() const
{
    // Const cast for code reuse.
    return const_cast<CTxtParameter*>(this)->GetValue();
}

//==============================================================================
//==
//==  IsQuotedString
//==
//==============================================================================
//
//  Returns:
//      - true: the value is a quoted string.
//      - false: the value is a token.
//
//  Description:
//      Indicates if the value of this parameter is a quoted-string.
//
//==============================================================================
inline bool CTxtParameter::IsQuotedString() const
{
    return m_bQuotedValue;
}

//==============================================================================
//====  MODIFIERS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  SetParameter
//==
//==============================================================================
//
//  Parameters:
//      rName:
//          The name of the parameter.
//
//      rValue:
//          The value of the parameter.
//
//      pszName:
//          See rName.
//
//      pszValue:
//          See rValue.
//
//  Description:
//      Sets the name and value of the parameter.
//
//==============================================================================
inline void CTxtParameter::SetParameter(IN CTxtToken& rName, IN CTxtToken& rValue)
{
    SetParameter(rName.GetString(), rValue.GetString());
}

//--<COMBINE CTxtParameter::SetParameter@IN CTxtToken&@IN CTxtToken&>
inline void CTxtParameter::SetParameter(IN const char* pszName, IN const char* pszValue)
{
    SetName(pszName);
    SetValue(pszValue);
}

//==============================================================================
//==
//==  SetName
//==
//==============================================================================
//
//  Parameters:
//      rName:
//          The name of the parameter.
//
//      pszName:
//          See rName.
//
//      uLength:
//          The length of the string pointed by pszName. When zero pszName is
//          considered NULL terminated and the length is computed with strlen.
//
//  Description:
//      Sets the name of the parameter.
//
//==============================================================================
inline void CTxtParameter::SetName(IN CTxtToken& rName)
{
    SetName(rName.GetString());
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//      rValue:
//          The value of the parameter.
//
//      pszValue:
//          See rValue.
//
//      uLength:
//          The length of the string pointed by pszValue. When zero pszName is
//          considered NULL terminated and the length is computed with strlen.
//
//  Description:
//      Sets the value of the parameter.
//
//==============================================================================
inline void CTxtParameter::SetValue(IN CTxtToken& rValue)
{
    SetValue(rValue.GetString());
}

//==============================================================================
//==
//==  SetQuotedStringValue
//==
//==============================================================================
//
//  Parameters:
//      rValue:
//          The value of the parameter.
//
//      pszValue:
//          See rValue.
//
//      uLength:
//          The length of the string pointed by pszValue. When zero pszName is
//          considered NULL terminated and the length is computed with strlen.
//
//  Description:
//      Sets the parameter value has a quoted string.
//
//==============================================================================
inline void CTxtParameter::SetQuotedStringValue(IN CTxtToken& rValue)
{
    SetQuotedStringValue(rValue.GetString());
}

//--<COMBINE CTxtParameter::SetQuotedStringValue@IN CTxtToken&>
inline void CTxtParameter::SetQuotedStringValue(IN const char* pszValue, IN unsigned int uLength)
{
    SetValue(pszValue, uLength);
    m_bQuotedValue = true;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTPARAMETER_H

