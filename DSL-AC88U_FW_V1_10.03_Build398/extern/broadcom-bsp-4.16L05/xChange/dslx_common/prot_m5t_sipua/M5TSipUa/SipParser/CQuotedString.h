//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CQUOTEDSTRING_H
#define MXG_CQUOTEDSTRING_H

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


//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CQuotedString
//=======================
// <GROUP SIPPARSER_CLASSES>
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
//   SipParser/CQuotedString.h
//
// See Also:
//
//
//==EDOC========================================================================
class CQuotedString
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CQuotedString();

    // Summary:
    //  Constructor.
    //---------------
    CQuotedString(IN const CQuotedString& rSrc);

    // Summary:
    //  Constructor.
    //---------------
    CQuotedString(IN const CString& rstr);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CQuotedString();

    // Summary:
    //  Assignment operators.
    //------------------------
    CQuotedString& operator=(IN const CQuotedString& rSrc);

    // <COMBINE CQuotedString::=@IN const CQuotedString&>
    //---------------------------------------------------
    CQuotedString& operator=(IN const CString& rstrSrc);


    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Parses a quoted string structure. Simply finds the matching quote.
    //---------------------------------------------------------------------
    mxt_result Parse(INOUT const char*& rpcPos);

    // Summary:
    //  Outputs the quoted string to the blob. The output always contains
    //  quotes.
    //--------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Provides access to the internal string. The set data should omit the
    //  quotes since they are automatically added when Serializing.
    //  Otherwise, another set of quotes are added.
    //-----------------------------------------------------------------------
    const CString& GetString() const;

    // <COMBINE CQuotedString::GetString@const>
    //-----------------------------------------
    CString& GetString();

    // Summary:
    //  Clears the quoted string.
    //----------------------------
    void Reset();


//-- Hidden Methods
//-------------------
protected:

private:


//-- Hidden Data Members
//------------------------
protected:

    // The quoted string itself. Will not include the enclosing
    //  quotes, except if they are forced by the caller.
    //---------------------------------------------------------
    CString m_strQuoted;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetString
//==
//==============================================================================
//
//  Returns:
//      The contained quoted string.
//
//  Description:
//      Provides access to the quoted string data.
//
//==EDOC========================================================================
inline const CString& CQuotedString::GetString() const
{
    return m_strQuoted;
}

//==SDOC========================================================================
//==
//==  GetString
//==
//==============================================================================
//
//  Returns:
//      The contained quoted string.
//
//  Description:
//      Provides access to the quoted string data.
//
//==EDOC========================================================================
inline CString& CQuotedString::GetString()
{
    return m_strQuoted;
}

//==SDOC========================================================================
//==
//==  Reset
//==
//==============================================================================
//
//  Description:
//      Clears the internal data (the quoted string).
//
//==EDOC========================================================================
inline void CQuotedString::Reset()
{
    m_strQuoted.EraseAll();
}


MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CQUOTEDSTRING_H
