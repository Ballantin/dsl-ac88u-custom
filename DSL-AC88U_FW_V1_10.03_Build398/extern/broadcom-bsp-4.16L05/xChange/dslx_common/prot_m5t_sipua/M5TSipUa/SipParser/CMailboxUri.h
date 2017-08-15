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
#ifndef MXG_CMAILBOXURI_H
#define MXG_CMAILBOXURI_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

//-- Data Members.
#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h"
#endif

//-- Interface Realized and/or Parent.
#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CBlob;
class CHeaderList;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CMailboxUri
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CMailboxUri class is used to store, parse, and serialize Mailbox URIs.
//   The supported URI types are IUri::eMAILBOX, IUri::eIM and IUri::ePRES.
//
//   A Mailbox URI is made up of a name-addr or addr-spec, and an optional
//   header list.
//
//   Mailbox URIs can be escaped. There are four charsets (see CToken) contexts
//   associated with CMailboxUri. The user, and headers each have different lists
//   characters that are legal and can be escaped. CMailboxUri handles all manner
//   of escaping automatically.
//
//  <CODE>
//    RFC 2822 ABNF:
//       mailbox        =  name-addr / addr-spec
//       name-addr      =  [display-name] angle-addr
//       angle-addr     =  [CFWS] "<" addr-spec ">" [CFWS]
//       display-name   =  phrase
//       addr-spec      =  local-part "@" domain
//       local-part     =  dot-atom / quoted-string
//       domain         =  dot-atom / domain-literal
//       domain-literal =  [CFWS] "[" *([FWS] dcontent) [FWS] "]" [CFWS]
//       dcontent       =  dtext / quoted-pair
//       dtext          =  NO-WS-CTL /     ; Non white space controls
//                         %d33-90 /       ; The rest of the US-ASCII
//                         %d94-126        ;  characters not including "[",
//                                         ;  "]", or "\"
//       dot-atom       =  [CFWS] dot-atom-text [CFWS]
//       dot-atom-text  =  1*atext *("." 1*atext)
//       atext          =  ALPHA / DIGIT / ; Any character except controls,
//                         "!" / "#" /     ;  SP, and specials.
//                         "$" / "%" /     ;  Used for atoms
//                         "&" / "'" /
//                         "*" / "+" /
//                         "-" / "/" /
//                         "=" / "?" /
//                         "^" / "_" /
//                         "`" / "{" /
//                         "|" / "}" /
//                         "~"
//
//       phrase         =  1*word 
//       word           =  atom / quoted-string
//       atom           =  [CFWS] 1*atext [CFWS]
//       quoted-string  =  [CFWS]
//                         DQUOTE *([FWS] qcontent) [FWS] DQUOTE
//                         [CFWS]
//       qcontent       =  qtext / quoted-pair
//       qtext          =  NO-WS-CTL /     ; Non white space controls
//                         %d33 /          ; The rest of the US-ASCII
//                         %d35-91 /       ;  characters not including "\"
//                         %d93-126        ;  or the quote character
//
//       CFWS           =  *([FWS] comment) (([FWS] comment) / FWS)
//       comment        =  "(" *([FWS] ccontent) [FWS] ")"
//       ccontent       =  ctext / quoted-pair / comment
//       ctext          =  NO-WS-CTL /     ; Non white space controls
//                         %d33-39 /       ; The rest of the US-ASCII
//                         %d42-91 /       ;  characters not including "(",
//                         %d93-126        ;  ")", or "\"
//       FWS            =  ([*WSP CRLF] 1*WSP) /   ; Folding white space
//       quoted-pair    =  ("\" text)
//       text           =  %d1-9 /         ; Characters excluding CR and LF
//                         %d11 /
//                         %d12 /
//                         %d14-127 /
//  </CODE>
//
// Location:
//   SipParser/CMailboxUri.h
//
// See Also:
//   CNameAddr, IUri, CHeaderList, CSipHeader
//
//==============================================================================
class CMailboxUri : public IUri
{
//-- New types definition.
public:

//-- Published Interface.
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CMailboxUri();

    // Summary:
    //  Constructor.
    CMailboxUri(IN bool bUseDisplayName);

    // Summary:
    //  Copy constructor.
    CMailboxUri(IN const CMailboxUri& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CMailboxUri();

    // Summary:
    //  Assignment operator.
    CMailboxUri& operator=(IN const CMailboxUri& rSrc);

    // Summary:
    //  Comparison operator. Compares as per RFC 3261 sect 19.1.4.
    bool operator==(IN const CMailboxUri& rSrc) const;

    // Summary:
    //  Comparison operator. Compares as per RFC 3261 sect 19.1.4.
    bool operator!=(IN const CMailboxUri& rSrc) const;

    //-- << Public Methods >>

    // Summary:
    //  Gets the addr-spec host contained in this URI.
    const CHostPort& GetHostPort() const;

    // <COMBINE CMailboxUri::GetHostPort@const>
    CHostPort& GetHostPort();

    // Summary:
    //  Gets the addr-spec local-part contained in this URI.
    const CString& GetUser() const;

    // <COMBINE CSipUri::GetUser@const>
    CString& GetUser();

    // Summary:
    //  Gets the display name if present in this URI.
    const CString* GetDisplayName() const;

    // <COMBINE CSipUri::GetDisplayName@const>
    CString* GetDisplayName();

    // Summary:
    //  Sets the display name.
    void SetDisplayName(IN const CString& rstrDisplayName);

    // Summary:
    //  Sets this URI with specified hostname, user, and display name.
    void Set(IN const CString& rstrHost,
             IN const CString& rstrUser,
             IN const CString* pstrDisplayName = NULL);

    // Summary:
    //  Provides access to the optional header list.
    const CHeaderList* GetHeaderList() const;

    // <COMBINE CMailboxUri::GetHeaderList@const>
    CHeaderList* GetHeaderList();

    // Summary:
    //  Sets the header list.
    void SetHeaderList(IN TO CHeaderList* pHeaderList);

    //-- << IUri >>

    // Summary:
    //  Generates a copy of this URI.
    virtual GO IUri* GenerateCopy() const;

    // Summary:
    //  Returns the URI's scheme.
    virtual const char* GetScheme() const;

    // Summary:
    //  Returns the URI type.
    virtual EUriType GetUriType() const;

    // Summary:
    //  Compares the given URI with this instance by using applicable RFC rules.
    //  For this URI type, compares by using RFC 3261 sect 19.1.4 rules.
    virtual bool IsEquivalent(IN const IUri& rSrc) const;

    // Summary:
    //  Parses a byte string into useable data.
    virtual mxt_result Parse(IN ESpecialCharactersAllowed eAllowSpecials,
                             INOUT const char*& rpcPos);

    // Summary:
    //  Reinitializes the instance.
    virtual void Reset();

    // Summary:
    //  Outputs the data member in a format that is ready to send on the
    //  network.
    virtual void Serialize(INOUT CBlob& rBlob) const;


//-- Hidden Methods.
protected:
    //-- << Helpers >>

    // Summary:
    //  Parses the display name if found.
    mxt_result ParseDisplayName(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the addr-spec if found.
    mxt_result ParseAddrSpec(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the list of headers if found.
    mxt_result ParseHeaders(INOUT const char*& rpcPos);

private:


//-- Hidden Data Members.
protected:

    // Display name of the 'name-addr' construct.
    CString* m_pstrName;

    // local part of the 'addr-spec' construct.
    CString m_strUser;

    // domain part of the 'addr-spec' construct.
    CHostPort m_host;

    // 'headers' construct.
    CHeaderList* m_pHeaderList;

    bool m_bUseDisplayName;

private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      Source against which to compare.
//
//  Returns:
//      True if the URIs are equivalent.
//
//  Description:
//      Comparison operator. Compares the URIs following RFC 3261 section 19.1.4
//      rules.
//
//  <CODE>
//  Extract from RFC 3261:
//   19.1.4 URI Comparison
//
//    Some operations in this specification require determining whether two
//    URIs are equivalent.  In this specification, registrars need to compare
//    bindings in Contact URIs in REGISTER requests (see Section 10.3.).
//    Mailbox URIs are compared for equality according to the following rules:
//
//       o  The mailbox URIs type must first match.
//
//       o  Comparison of the mailbox URIs user is case-sensitive.
//          Comparison of all other components of the URI is case-insensitive
//          unless explicitly defined otherwise.
//
//       o  The ordering of the header fields is not significant in comparing
//          mailbox URIs.
//
//       o  An IP address that is the result of a DNS lookup of a host name
//          does not match that host name.
//
//       o  For two URIs to be equal, the name-addr or addr-spec must match.
//
//          A URI omitting the display name component will not match a URI
//          that includes one. Thus a name-addr component cannot match an
//          addr-spec one.
//
//          A URI omitting any component with a default value will not
//          match a URI explicitly containing that component with its
//          default value.
//
//       o  URI header components are never ignored.  Any present header
//          component MUST be present in both URIs and match for the URIs
//          to match.  The matching rules are defined for each header field
//          in Section 20.
//  </CODE>
//
//  See Also:
//      IsEquivalent
//
//==============================================================================
inline bool CMailboxUri::operator==(IN const CMailboxUri& rSrc) const
{
    return IsEquivalent(rSrc);
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rSrc:
//      Source against which to compare.
//
//  Returns:
//      True if the URIs are not equivalent.
//
//  Description:
//      Comparison operator. Compares the URIs following RFC 3261 section 19.1.4
//      rules.
//
//  <CODE>
//  Extract from RFC 3261:
//   19.1.4 URI Comparison
//
//    Some operations in this specification require determining whether two
//    URIs are equivalent.  In this specification, registrars need to compare
//    bindings in Contact URIs in REGISTER requests (see Section 10.3.).
//    Mailbox URIs are compared for equality according to the following rules:
//
//       o  The mailbox URIs type must first match.
//
//       o  Comparison of the display name of mailbox URIs is case-sensitive.
//          Comparison of all other components of the URI is case-insensitive
//          unless explicitly defined otherwise.
//
//       o  The ordering of the header fields is not significant in comparing
//          mailbox URIs.
//
//       o  An IP address that is the result of a DNS lookup of a host name
//          does not match that host name.
//
//       o  For two URIs to be equal, the name-addr or addr-spec must match.
//
//          A URI omitting the display name component will not match a URI
//          that includes one. Thus a name-addr component cannot match an
//          addr-spec one.
//
//          A URI omitting any component with a default value will not
//          match a URI explicitly containing that component with its
//          default value.
//
//       o  URI header components are never ignored.  Any present header
//          component MUST be present in both URIs and match for the URIs
//          to match.  The matching rules are defined for each header field
//          in Section 20.
//  </CODE>
//
//  See Also:
//      IsEquivalent
//
//==============================================================================
inline bool CMailboxUri::operator!=(IN const CMailboxUri& rSrc) const
{
    return !IsEquivalent(rSrc);
}

//==============================================================================
//==
//==  GetScheme
//==
//==============================================================================
//
//  Returns:
//      URI Scheme.
//
//      Can be NULL if the URI type is eMAILBOX.
//
//  Description:
//      Returns the scheme.
//
//==============================================================================
inline const char* CMailboxUri::GetScheme() const
{
    return NULL;
}

//==============================================================================
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
//==============================================================================
inline IUri::EUriType CMailboxUri::GetUriType() const
{
    return IUri::eMAILBOX;
}

//==============================================================================
//==
//==  GetHostPort
//==
//==============================================================================
//
//  Returns:
//      The addr-spec host element.
//
//  Description:
//      Provides access to the addr-spec host element.
//
//==============================================================================
inline CHostPort& CMailboxUri::GetHostPort()
{
    return m_host;
}

//==============================================================================
//==
//==  GetHostPort
//==
//==============================================================================
//
//  Returns:
//      The addr-spec host element.
//
//  Description:
//      Provides access to the addr-spec host element.
//
//==============================================================================
inline const CHostPort& CMailboxUri::GetHostPort() const
{
    return m_host;
}

//==============================================================================
//==
//==  GetUser
//==
//==============================================================================
//
//  Returns:
//      The addr-spec's local-part element.
//
//  Description:
//      Provides access to the addr-spec's local-part element.
//
//==============================================================================
inline CString& CMailboxUri::GetUser()
{
    return m_strUser;
}

//==============================================================================
//==
//==  GetUser
//==
//==============================================================================
//
//  Returns:
//      The addr-spec's local-part element.
//
//  Description:
//      Provides access to the addr-spec's local-part element.
//
//==============================================================================
inline const CString& CMailboxUri::GetUser() const
{
    return m_strUser;
}

//==============================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Returns:
//      The display name element.
//
//      NULL if there is none.
//
//  Description:
//      Provides access to the display name element.
//
//==============================================================================
inline CString* CMailboxUri::GetDisplayName()
{
    return m_pstrName;
}

//==============================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Returns:
//      The display name element.
//
//      NULL if there is none.
//
//  Description:
//      Provides access to the display name element.
//
//==============================================================================
inline const CString* CMailboxUri::GetDisplayName() const
{
    return m_pstrName;
}

//==============================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the header list. It may be NULL.
//
//  Description:
//      Provides access to the header list.
//
//==============================================================================
inline const CHeaderList* CMailboxUri::GetHeaderList() const
{
    return m_pHeaderList;
}

//==============================================================================
//==
//==  GetHeaderList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the header list. It may be NULL.
//
//  Description:
//      Provides access to the header list.
//
//==============================================================================
inline CHeaderList* CMailboxUri::GetHeaderList()
{
    return m_pHeaderList;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CMAILBOXURI_H
