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
#ifndef MXG_CTXTPARAMETERLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTPARAMETERLIST_H
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

#ifndef MXG_TXTMESSAGECONSTANTS_H
#include "TextMessage/TxtMessageConstants.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class CTxtParameter;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtParameterList
//================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 7>
//
//  Summary:
//      This class handles parsing and storage of a generic parameter list
//      structure.
//
//  Description:
//      This class handles parsing and storage of a generic parameter list
//      structure. This class can handle lists of parameters that respects the
//      following ABNF which is inspired by RFC 3261.
//
//  <CODE>
//  param-list = 1*( param-sep [LWS] param-name [LWS]
//                   [ "=" [LWS] param-value [LWS] ] )
//
//  param-sep = The separator character provided through the cSeparator
//              parameter of the constructor. Generally ';' but sometimes ','.
//
//  param-name    = token
//  token         = 1*( ALPHA / DIGIT /
//                      "-" / "." / "!" / "%" / "*" / "_" / "+" / "`" / "'" /
//                      "~" )
//
//  param-value   = quoted-string / x-token
//  quoted-string = '"' *( x-token ) '"'
//  x-token       = 1*( ALPHA / DIGIT / escaped /
//                      "-" / "." / "!" / "_" / "+" / "`" / "'" / "~" / "[" /
//                      "]" / "/" / ":" / "&" / "$" / "(" / ")" )
//  escaped       = "%" HEXDIGIT HEXDIGIT
//  </CODE>
//
//  Location:
//      TextMessage/CTxtParameterList.h
//
//==============================================================================
class CTxtParameterList
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CTxtParameterList(IN IAllocator* pAllocator = NULL,
                      IN bool bStartWithSeparator = true,
                      IN char cSeparator = g_cSEMI);

    // Summary:
    //  Copy Constructor.
    CTxtParameterList(IN const CTxtParameterList& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtParameterList();

    // Summary:
    //  Assignment Operator.
    CTxtParameterList& operator=(IN const CTxtParameterList& rSrc);

    // Summary:
    //  Equality Operator.
    virtual bool operator==(IN const CTxtParameterList& rSrc);

    // Summary:
    //  Inequality Operator.
    bool operator!=(IN const CTxtParameterList& rSrc);

    //-- << Accessors >>

    // Summary:
    //  Fetches a parameter in the list by name.
    CTxtParameter* GetParam(IN const char* pszParameterName);

    //<COMBINE CTxtParameterList::GetParam@IN const char*>
    const CTxtParameter* GetParam(IN const char* pszParameterName) const;

    // Summary:
    //  Fetches a parameter in the list by index.
    CTxtParameter* GetParam(IN unsigned int uParameterIndex);

    //<COMBINE CTxtParameterList::GetParam@IN unsigned int>
    const CTxtParameter* GetParam(IN unsigned int uParameterIndex) const;

    // Summary:
    //  Fetches a parameter in the list.
    CTxtToken* GetParamToken(IN const char* pszParameterName);

    //<COMBINE CTxtParameterList::GetParamToken>
    const CTxtToken* GetParamToken(IN const char* pszParameterName) const;

    // Summary:
    //  Fetches a parameter in the list.
    const char* GetParamString(IN const char* pszParameterName);

    //<COMBINE CTxtParameterList::GetParamString>
    const char* GetParamString(IN const char* pszParameterName) const;

    // Summary:
    //  Returns the number of parameters in the list.
    unsigned int GetLength() const;

    // Summary:
    //  Checks whether or not the parameter list is empty.
    bool IsEmpty() const;

    //-- << Modifiers >>

    // Summary:
    //  Sets the token characteristics to use in the parameters used in the
    //  parameter list.
    void SetParametersTokenCharacteristics(IN const CTxtToken::STokenCharacteristics* pstNameCharacteristics,
                                           IN const CTxtToken::STokenCharacteristics* pstValueCharacteristics = NULL);

    // Summary:
    //  Creates and appends a new CTxtParameter.
    virtual CTxtParameter* CreateNewParameter(IN const char* pszNewParamName = NULL,
                                              IN const char* pszNewParamValue = NULL);

    // Summary:
    //  Removes a parameter from the list by its name.
    mxt_result Remove(IN const char* pszParameterName);

    // Summary:
    //  Removes a parameter from the list by its index.
    mxt_result Remove(IN unsigned int uParameterIndex);

    // Summary:
    //  Resets the parameter list.
    void Reset(IN bool bCompleteReset = false,
               IN bool bStartWithSeparator = true,
               IN char cSeparator = g_cSEMI);

    //-- << Parsing >>

    // Summary:
    //  Extracts parameters from the provided text.
    mxt_result Parse(INOUT const char*& rpszParametersText);

    // Summary:
    //  Extracts a single parameter from the provided text.
    mxt_result ParseSingleParameter(INOUT const char*& rpszParameterText);

    //-- << Serialization >>

    // Summary:
    //  Ouputs the parameter list into the provided blob. The parameters are
    //  separated using the provided separator character.
    void Serialize(INOUT CBlob& rBlob);

//-- Hidden Methods
protected:

    //-- << Helpers >>

    // Summary:
    //  Releases the memory used by a parameter.
    void ReleaseParameter(IN TOA CTxtParameter* pParameter) const;

    // Summary:
    //  Allocates memory using the correct method.
    uint8_t* AllocateMemory(IN unsigned int uSize) const;

    // Summary:
    //  Releases memory allocated with AllocateMemory.
    void ReleaseMemory(IN TOA uint8_t* pMemory) const;

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:

    // This is the allocator to use to manage the memory.
    IAllocator* m_pAllocator;

private:

    // Indicates to the parser and serializer whether or not the parameter list
    // starts with its separator character.
    bool m_bStartWithSeparator;

    // Indicates the character to use as the parameter separator when parsing
    // and serializing the list.
    char m_cSeparator;

    // The list of parameters.
    CVector<CTxtParameter*> m_vecpParameters;

    // This is the token characteristics to set in new parameters for the parameter name.
    const CTxtToken::STokenCharacteristics*  m_pstParamNameCharacteristics;

    // This is the token characteristics to set in new parameters for the parameter value.
    const CTxtToken::STokenCharacteristics*  m_pstParamValueCharacteristics;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS  =============================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The parameter list to which this instance is compared.
//
//  Returns:
//      - true: if the parameter lists are not equal.
//      - false: if the parameter lists are equal.
//
//  Descriptions:
//      Inequality Operator. Parameter lists are considered to be not equal if
//      they are of the different size or any of their contained parameters are
//      not equal.
//
//==============================================================================
inline bool CTxtParameterList::operator!=(IN const CTxtParameterList& rSrc)
{
    return !(*this == rSrc);
}

//==============================================================================
//====  ACCESSORS  =============================================================
//==============================================================================

//--<COMBINE CTxtParameterList::GetParam@IN const char*>
inline const CTxtParameter* CTxtParameterList::GetParam(IN const char* pszParameterName) const
{
    return const_cast<CTxtParameterList*>(this)->GetParam(pszParameterName);
}

//--<COMBINE CTxtParameterList::GetParam@IN unsigned int>
inline const CTxtParameter* CTxtParameterList::GetParam(IN unsigned int uParameterIndex) const
{
    return const_cast<CTxtParameterList*>(this)->GetParam(uParameterIndex);
}

//==============================================================================
//==
//==  GetParam
//==
//==============================================================================
//
//  Parameters:
//      uParameterIndex:
//          The 0-based index of the parameter to retrieve. MUST be less than
//          the value returned by GetLength.
//
//  Returns:
//      A pointer to the parameter fetched. MAY be NULL if the parameter could
//      not be found.
//
//  Description:
//      Fetches a parameter in the list.
//
//==============================================================================
inline CTxtParameter* CTxtParameterList::GetParam(IN unsigned int uParameterIndex)
{
    return uParameterIndex < GetLength() ? m_vecpParameters[uParameterIndex] : NULL;
}

//--<COMBINE CTxtParameterList::GetParamToken@IN const char*>
inline const CTxtToken* CTxtParameterList::GetParamToken(IN const char* pszParameterName) const
{
    return const_cast<CTxtParameterList*>(this)->GetParamToken(pszParameterName);
}

//--<COMBINE CTxtParameterList::GetParamString@IN const char*>
inline const char* CTxtParameterList::GetParamString(IN const char* pszParameterName) const
{
    return const_cast<CTxtParameterList*>(this)->GetParamString(pszParameterName);
}

//==============================================================================
//==
//==  GetLength
//==
//==============================================================================
//
//  Returns:
//      The number of parameters available in the parameter list.
//
//  Description:
//      Returns the number of parameters in the list.
//
//==============================================================================
inline unsigned int CTxtParameterList::GetLength() const
{
    return m_vecpParameters.GetSize();
}

//==============================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      - true: There are no parameters in the list.
//      - false: Parameters are available in the list.
//
//  Description:
//      Checks whether or not the parameter list is empty.
//
//==============================================================================
inline bool CTxtParameterList::IsEmpty() const
{
    return m_vecpParameters.IsEmpty();
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTPARAMETERLIST_H

