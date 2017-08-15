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
#ifndef MXG_CTXTQUOTEDSTRING_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTQUOTEDSTRING_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
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
//== Class: CTxtQuotedString
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 2>
//
// Summary:
//   This class handles parsing and storage of a quoted string construct.
//
// Description:
//   This object abstracts a quoted string. The parsing mechanism is very much
//   simplified. This object does not do any character escaping or UTF8
//   conversion. It only finds the matching ending quote. Users of this class
//   can set a string that omits the quotes since the Serialization process adds
//   them automatically. When parsing, the stored data does not include the
//   enclosing quotes.
//
// Location:
//   TextMessage/CTxtQuotedString.h
//
//==============================================================================
class CTxtQuotedString
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CTxtQuotedString(IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CTxtQuotedString(IN const CTxtQuotedString& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtQuotedString();

    // Summary:
    //  Assignment Operator.
    CTxtQuotedString& operator=(IN const CTxtQuotedString& rSrc);

    //<COMBINE CTxtQuotedString::=@IN const CTxtQuotedString&>
    CTxtQuotedString& operator=(IN const char* pszSrc);

    //-- << Accessors >>

    // Summary:
    //  Provides access to the internal string.
    const char* GetString() const;

    // Summary:
    //  Provides access to the string length.
    unsigned int GetLength() const;

    //-- << Modifiers >>

    // Summary:
    //  Sets the string value.
    void SetString(IN const char* pszStringWithoutQuotes, IN unsigned long uStringLength = 0);

    // Summary:
    //  Clears the quoted string.
    void Reset();

    //-- << Parsing >>

    // Summary:
    //  Parses a quoted string structure. Simply finds the matching quote.
    mxt_result Parse(INOUT const char*& rpszQuotedStringText);

    //-- << Serialization >>

    // Summary:
    //  Outputs the quoted string to the blob. The output always contains
    //  quotes.
    void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
protected:

    //-- << Helpers >>

    // Summary:
    //  Allocates memory for the quoted string.
    char* GO AllocateString(IN unsigned int uLength);

    // Summary:
    //  Releases memory used by the quoted string allocated with AllocateString.
    void ReleaseString(IN TOA char* pszString);

private:

//-- Hidden Data Members
protected:

    // The Allocator
    IAllocator* m_pAllocator;

private:

    // The unquoted string.
    char* m_pszUnquotedString;

    // The unquoted string length.
    unsigned int m_uLength;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetString
//==
//==============================================================================
//
//  Returns:
//      The contained quoted string. Can be NULL.
//
//  Description:
//      Provides access to the quoted string data.
//
//==============================================================================
inline const char* CTxtQuotedString::GetString() const
{
    return m_pszUnquotedString;
}

//==============================================================================
//==
//==  GetLength
//==
//==============================================================================
//
//  Returns:
//      The quoted string length without the quotes and ending '\0'.
//
//  Description:
//      Provides access to the quoted string length.
//
//==============================================================================
inline unsigned int CTxtQuotedString::GetLength() const
{
    return m_uLength;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTQUOTEDSTRING_H

