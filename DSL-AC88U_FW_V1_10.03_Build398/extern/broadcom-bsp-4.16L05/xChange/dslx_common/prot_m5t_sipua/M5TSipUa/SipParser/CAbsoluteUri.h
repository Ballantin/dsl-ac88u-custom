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
#ifndef MXG_CABSOLUTEURI_H
#define MXG_CABSOLUTEURI_H

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


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif



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
//== Class: CAbsoluteUri
//========================================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   This class abstracts a basic URI as per RFC 3261 ABNF (derived from RFC 
//   2396)
//
//  <CODE>
//      RFC 3261 ABNF
//          absoluteURI   =  scheme ":" ( hier-part / opaque-part )
//          hier-part     =  ( net-path / abs-path ) [ "?" query ]
//          net-path      =  "//" authority [ abs-path ]
//          abs-path      =  "/" path-segments
//          opaque-part   =  uric-no-slash *uric
//          uric          =  reserved / unreserved / escaped
//          uric-no-slash =  unreserved / escaped / ";" / "?" / ":" / "@"
//                             / "&" / "=" / "+" / "$" / ","
//          path-segments =  segment *( "/" segment )
//          segment       =  *pchar *( ";" param )
//          param         =  *pchar
//          pchar         =  unreserved / escaped /
//                             ":" / "@" / "&" / "=" / "+" / "$" / ","
//          scheme        =  ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
//          authority     =  srvr / reg-name
//          srvr          =  [ [ userinfo "@" ] hostport ]
//          reg-name      =  1*( unreserved / escaped / "$" / ","
//                             / ";" / ":" / "@" / "&" / "=" / "+" )
//          query         =  *uric
//  </CODE>
//
//  The CAbsoluteUri is used as a last resort when parsing URIs. When all URIs
//  that are natively supported by the SIP stack have been tried, the unknown
//  URI is parsed as a CAbsoluteUri.
//
//  No processing is made on the CAbsoluteUri when it is parsed; it is kept in
//  escaped form, as was received.
//
//  Note that there is no specific parsing of the (hier-part / opaque part)
//  ABNF. Anything that is found after the colon trailing the scheme is
//  considered as the CAbsoluteUri's body.
//
//  To parse CAbsoluteUri, users should use the CUriFactory to generate and
//  parse.
//
// Location:
//   SipParser/CAbsoluteUri.h
//
// See Also:
//      IUri, CSipUri, CUriFactory
//
//==EDOC========================================================================
class CAbsoluteUri : public IUri
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CAbsoluteUri();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CAbsoluteUri(IN const CAbsoluteUri& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CAbsoluteUri();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CAbsoluteUri& operator=(IN const CAbsoluteUri& rSrc);

    //-- << Public methods >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the body of the URI, in the form in which it was
    //  received from the network.
    //----------------------------------------------------------------------
    const CString& GetBody() const;

    //<COMBINE CAbsoluteUri::GetBody@const>
    CString& GetBody();

    // Summary:
    //  Sets the scheme part of the URI.
    //-----------------------------------
    void SetScheme(IN const CString& rstrScheme);


    //-- << IUri >>
    //--------------------------------------------

    // Summary:
    //  Generates a copy of this URI.
    //--------------------------------
    virtual GO IUri* GenerateCopy() const;

    // Summary:
    //  Returns the URI's scheme. In this case, the m_strScheme data member.
    //-----------------------------------------------------------------------
    virtual const char* GetScheme() const;

    // Summary:
    //  Returns URI type, SIP or SIPS.
    //---------------------------------
    virtual EUriType GetUriType() const;

    // Summary:
    //  Compares the given URI with this instance using applicable RFC rules.
    //  For this URI type, the scheme is compared case-insensitively and the
    //  body byte-per-byte.
    //------------------------------------------------------------------------
    virtual bool IsEquivalent(IN const IUri& rSrc) const;

    // Summary:
    //  Parses a byte string into usable data.
    //-----------------------------------------
    virtual mxt_result Parse( IN ESpecialCharactersAllowed eAllowSpecials,
                              INOUT const char*& rpcPos );

    // Summary:
    //  Reinitializes the instance.
    //------------------------------
    virtual void Reset();

    // Summary:
    //  Outputs the data member in a format that is ready to be sent on the
    //  network.
    //----------------------------------------------------------------------
    virtual void Serialize( INOUT CBlob& rBlob ) const;



//-- Hidden Methods
//-------------------
protected:
    // Summary:
    //  Returns the number of characters that are legal in a URI.
    //------------------------------------------------------------
    static unsigned int GetUriLength(
                                  INOUT const char* pcPos,
                                  IN ESpecialCharactersAllowed eAllowSpecials );

    // Summary:
    //  True if the character is one of the "mark" RFC 3261 ABNF construct.
    //----------------------------------------------------------------------
    static bool IsMark(IN char c);

    // Summary:
    //  True if the character is one of the "reserved" RFC 3261 ABNF construct.
    //--------------------------------------------------------------------------
    static bool IsReserved(IN char c);


private:

//-- Hidden Data Members
//------------------------
protected:
    // Name of the URI's scheme.
    //---------------------------
    CString m_strScheme;

    // Body of the URI, in the form that it was received in. The body is not
    // unescaped.
    //-----------------------------------------------------------------------
    CString m_strBody;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsMark
//==
//==============================================================================
//
//  Parameters:
//    c:
//      Character to verify.
//
//  Returns:
//      True if the character is one of the "mark" RFC 3261 ABNF construct.
//
//  Description:
//      Checks if a character is one of the "mark" RFC 3261 ABNF construct.
//
//  <CODE>
//      RFC 3261 ABNF:
//          mark        =  "-" / "_" / "." / "!" / "~" / "*" / "'" / "(" / ")"
//  </CODE>
//
//==EDOC========================================================================
inline bool CAbsoluteUri::IsMark(IN char c)
{
    return c == '-' || c == '_' || c == '.' || c == '!' || c == '~' ||
           c == '*' || c == '\'' || c == '(' || c == ')';
}

//==SDOC========================================================================
//==
//==  IsReserved
//==
//==============================================================================
//
//  Parameters:
//    c:
//      Character to verify.
//
//  Returns:
//      True if the character is one of the "reserved" RFC 3261 ABNF construct.
//
//  Description:
//      Checks if a character is one of the "reserved" RFC 3261 ABNF construct.
//
//  <CODE>
//      RFC 3261 ABNF:
//         reserved =  ";" / "/" / "?" / ":" / "@" / "&" / "=" / "+" / "$" / ","
//  </CODE>
//
//==EDOC========================================================================
inline bool CAbsoluteUri::IsReserved(IN char c)
{
    return c == ';' || c == '/' || c == '?' || c == ':' || c == '@' ||
        c == '&' || c == '=' || c == '+' || c == '$' || c == ',';
}

//==SDOC========================================================================
//==
//==  GetScheme
//==
//==============================================================================
//
//  Returns:
//      Returns the m_strScheme member.
//
//  Description:
//      Provides access to the m_strScheme member.
//
//  See Also:
//      SetScheme
//
//==EDOC========================================================================
inline const char* CAbsoluteUri::GetScheme() const
{
    return m_strScheme.CStr();
}

//==SDOC========================================================================
//==
//==  GetBody
//==
//==============================================================================
//
//  Returns:
//      Body of the URI.
//
//  Description:
//      Provides access to the as-received-from network version of the the URI's
//      body.
//
//==EDOC========================================================================
inline const CString& CAbsoluteUri::GetBody() const
{
    return m_strBody;
}

//==SDOC========================================================================
//==
//==  GetBody
//==
//==============================================================================
//
//  Returns:
//      Body of the URI.
//
//  Description:
//      Provides access to the as-received-from network version of the the URI's
//      body.
//
//==EDOC========================================================================
inline CString& CAbsoluteUri::GetBody()
{
    return m_strBody;
}

//==SDOC========================================================================
//==
//==  GetUriType
//==
//==============================================================================
//
//  Returns:
//      URI type.
//
//  Description:
//      Returns the URI type. This method is required because of the
//      IsEquivalent method.
//
//==EDOC========================================================================
inline IUri::EUriType CAbsoluteUri::GetUriType() const
{
    return IUri::eABSOLUTE;
}

//==SDOC========================================================================
//==
//==  SetScheme
//==
//==============================================================================
//
//  Parameters:
//    rstrScheme:
//      Scheme to set.
//
//  Description:
//      Sets the scheme to use.
//
//  See Also:
//      GetScheme
//
//==EDOC========================================================================
inline void CAbsoluteUri::SetScheme(IN const CString& rstrScheme)
{
    m_strScheme = rstrScheme;
}


MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CABSOLUTEURI_H
