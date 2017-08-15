//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPURI_H
#define MXG_CSIPURI_H

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
#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IURI_H
#include "SipParser/IUri.h"
#endif

//-- For helper methods
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CGenParamList;
class CHeaderList;
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CSipUri
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CSipUri class is used to store, parse, and serialize SIP and SIPS URIs.
//   The SIPS URI form can be set by using the SetSecured() method.
//
//   A SIPURI is made up of an optional userinfo section (user and password), a
//   hostport, a parameter list, and an optional header list.
//
//   SIPURIs can be escaped. There are four charsets (see CToken) contexts
//   associated with CSipUri. The user, password, parameters, and headers each
//   have different lists characters that are legal and can be escaped. CSipUri
//   handles all manner of escaping automatically.
//
//  <CODE>
//    RFC 3261 ABNF:
//       SIP-URI          =  "sip:" [ userinfo ] hostport
//                           uri-parameters [ headers ]
//       SIPS-URI         =  "sips:" [ userinfo ] hostport
//                           uri-parameters [ headers ]
//       userinfo         =  ( user / telephone-subscriber ) [ ":" password ] "@"
//       user             =  1*( unreserved / escaped / user-unreserved )
//       user-unreserved  =  "&" / "=" / "+" / "$" / "," / ";" / "?" / "/"
//       password         =  *( unreserved / escaped /
//                           "&" / "=" / "+" / "$" / "," )
//       hostport         =  host [ ":" port ]
//       host             =  hostname / IPv4address / IPv6reference
//       hostname         =  *( domainlabel "." ) toplabel [ "." ]
//       domainlabel      =  alphanum
//                           / alphanum *( alphanum / "-" ) alphanum
//       toplabel         =  ALPHA / ALPHA *( alphanum / "-" ) alphanum
//
//       IPv4address    =  1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
//       IPv6reference  =  "[" IPv6address "]"
//       IPv6address    =  hexpart [ ":" IPv4address ]
//       hexpart        =  hexseq / hexseq "::" [ hexseq ] / "::" [ hexseq ]
//       hexseq         =  hex4 *( ":" hex4)
//       hex4           =  1*4HEXDIG
//       port           =  1*DIGIT
//
//       uri-parameters    =  *( ";" uri-parameter)
//       uri-parameter     =  transport-param / user-param / method-param
//                            / ttl-param / maddr-param / lr-param / other-param
//       transport-param   =  "transport="
//                            ( "udp" / "tcp" / "sctp" / "tls"
//                            / other-transport)
//       other-transport   =  token
//       user-param        =  "user=" ( "phone" / "ip" / other-user)
//       other-user        =  token
//       method-param      =  "method=" Method
//       ttl-param         =  "ttl=" ttl
//       maddr-param       =  "maddr=" host
//       lr-param          =  "lr"
//       other-param       =  pname [ "=" pvalue ]
//       pname             =  1*paramchar
//       pvalue            =  1*paramchar
//       paramchar         =  param-unreserved / unreserved / escaped
//       param-unreserved  =  "[" / "]" / "/" / ":" / "&" / "+" / "$"
//
//       headers         =  "?" header *( "&" header )
//       header          =  hname "=" hvalue
//       hname           =  1*( hnv-unreserved / unreserved / escaped )
//       hvalue          =  *( hnv-unreserved / unreserved / escaped )
//       hnv-unreserved  =  "[" / "]" / "/" / "?" / ":" / "+" / "$"
//  </CODE>
//
// Location:
//   SipParser/CSipUri.h
//
// See Also:
//   CToken, CHostPort, IUri, CGenParamList, CGenericParam, CHeaderList,
//   CSipHeader
//
//==EDOC========================================================================
class CSipUri : public IUri
{
//-- New types definition
//-----------------------
public:
    // Description:
    //  Indicates whether or not this URI is "sips".
    enum ESecurityFlag
    {
        // Description:
        //  The URI is not secure: "sip"
        eUNSECURE,
        // Description:
        //  The URI is secure: "sips"
        eSECURE
    };

    // Description:
    //  Indicates whether or not the parameter must be created when calling GetParam.
    enum EParameterCreationBehavior
    {
        // Description:
        //  Dont' create the parameter.
        ePARAM_DONT_CREATE,
        // Description:
        //  Create the parameter.
         ePARAM_CREATE_NEW
    };

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSipUri();

    // Summary:
    //  Copy constructor.
    //--------------------
    CSipUri(IN const CSipUri& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipUri();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSipUri& operator=(IN const CSipUri& rSrc);

    // Summary:
    //  Comparison operator. Compares as per RFC 3261 sect 19.1.4.
    //-------------------------------------------------------------
    bool operator==(IN const CSipUri& rSrc) const;

    // Summary:
    //  Comparison operator. Compares as per RFC 3261 sect 19.1.4.
    //-------------------------------------------------------------
    bool operator!=(IN const CSipUri& rSrc) const;

    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Gets the hostport contained into this URI.
    //---------------------------------------------
    const CHostPort& GetHostPort() const;

    // <COMBINE CSipUri::GetHostPort@const>
    //---------------------------------
    CHostPort& GetHostPort();

    // Summary:
    //  Gets the user if present in this URI.
    //----------------------------------------
    const CToken& GetUser() const;

    // <COMBINE CSipUri::GetUser@const>
    //---------------------------------
    CToken& GetUser();

    // Summary:
    //  Gets the password if present in this URI.
    //--------------------------------------------
    const CToken* GetPassword() const;

    // Summary:
    //  Sets a password.
    //-------------------
    void SetPassword(IN const char* szPassword);

    // Summary:
    //  Returns true if this URI is a "sips".
    //----------------------------------------
    bool IsSecured() const;

    // Summary:
    //  Sets whether or not this URI is "sips".
    //------------------------------------------
    void SetSecured(IN ESecurityFlag eSecured);

    // Summary:
    //  Sets this URI with specified hostname, port, and user. By default, the
    //  port is not sent.
    //-------------------------------------------------------------------------
    void Set( IN const CString& rstrHost,
              IN uint16_t uPort = CHostPort::ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT,
              IN const CString& rstrUser = CString(),
              IN IN ESecurityFlag eSecured = eUNSECURE );

    // Summary:
    //  Provides access to the optional header list.
    //-----------------------------------------------
    const CHeaderList* GetHeaderList() const;

    // <COMBINE CSipUri::GetHeaderList@const>
    //---------------------------------------
    CHeaderList* GetHeaderList();

    // Summary:
    //  Sets the header list.
    //------------------------
    void SetHeaderList(IN TO CHeaderList* pHeaderList);

    // Summary:
    //  Provides access to the optional parameter list.
    //--------------------------------------------------
    const CGenParamList* GetParamList() const;

    // <COMBINE CSipUri::GetParamList@const>
    //--------------------------------------
    CGenParamList* GetParamList();

    // Summary:
    //  Sets the optional parameter list.
    //------------------------------------
    void SetParamList(IN TO CGenParamList* pParamList);

    // Summary:
    //  Gets a header parameter value from the parameter list.
    //---------------------------------------------------------
    CToken* GetParam(
                IN const char* pszName,
                IN const EParameterCreationBehavior eParameterCreationBehavior =
                    ePARAM_DONT_CREATE);


    // Summary:
    //  Gets a header parameter value from the parameter list.
    //---------------------------------------------------------
    const CToken* GetParam(IN const char* pszName) const;

    // Summary:
    //  Sets a header parameter value to the parameter list from a CString.
    //----------------------------------------------------------------------
    void SetParam(IN const char* pszName, IN const CString& rstrValue);

    // Summary:
    //  Sets a header parameter value to the parameter list from a CToken.
    //---------------------------------------------------------------------
    void SetParam(IN const char* pszName, IN const CToken& rValue);

    // Summary:
    //  Sets the missing default port behavior. This behavior is used only
    //  when comparing two CSipUri using the IsEquivalent method or the
    //  == operator.
    //---------------------------------------------------------------------
    static void SetMissingPortBehavior(bool bMissingPortBehavior);

    // Summary:
    //  Gets the missing default port behavior. This behavior is used only
    //  when comparing two CSipUri using the IsEquivalent method or the
    //  == operator.
    //---------------------------------------------------------------------
    static bool GetMissingPortBehavior();


    //-- << Helper methods >>

    // Summary:
    //  Returns the transport specified in a "transport" parameter.
    ESipTransport GetParamTransport() const;


    //-- << IUri >>
    //--------------------------------------------

    // Summary:
    //  Generates a copy of this URI.
    //--------------------------------
    virtual GO IUri* GenerateCopy() const;

    // Summary:
    //  Returns the URI's scheme. In this case, either SIP or SIPS.
    //--------------------------------------------------------------
    virtual const char* GetScheme() const;

    // Summary:
    //  Returns the URI type, SIP or SIPS.
    //-------------------------------------
    virtual EUriType GetUriType() const;

    // Summary:
    //  Compares the given URI with this instance by using applicable RFC rules.
    //  For this URI type, compares by using RFC 3261 sect 19.1.4 rules.
    //--------------------------------------------------------------------------
    virtual bool IsEquivalent(IN const IUri& rSrc) const;

    // Summary:
    //  Parses a byte string into useable data.
    //------------------------------------------
    virtual mxt_result Parse( IN ESpecialCharactersAllowed eAllowSpecials,
                              INOUT const char*& rpcPos );

    // Summary:
    //  Reinitializes the instance.
    //------------------------------
    virtual void Reset();

    // Summary:
    //  Outputs the data member in a format that is ready to send on the
    //  network.
    //-------------------------------------------------------------------
    virtual void Serialize( INOUT CBlob& rBlob ) const;


//-- Hidden Methods
//-------------------
protected:
    //-- << Helpers >>
    //--------------------------------------------

    // Summary:
    //  Parses the list of headers if found.
    //---------------------------------------
    mxt_result ParseHeaders(INOUT const char*& rpcPos);

    // Summary:
    //  Parses the optional user information structure.
    //--------------------------------------------------
    mxt_result ParseUserInfo(INOUT const char*& rpcPos);


private:


//-- Hidden Data Members
//------------------------
protected:
    // 'hostport' construct.
    //-----------------------
    CHostPort m_host;

    // Userinfo 'user' construct.
    //----------------------------
    CToken m_tokUser;

    // Userinfo 'password' construct.
    //--------------------------------
    CToken* m_pTokPassword;

    // True if this instance is a "sips" uri.
    //----------------------------------------
    bool m_bSecured;

    // 'uri-parameters' construct.
    //-----------------------------
    CGenParamList* m_pParamList;

    // 'headers' construct.
    //----------------------
    CHeaderList* m_pHeaderList;

    static bool ms_bMissingPortBehavior;
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
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
//    SIP or SIPS URIs are equivalent.  In this specification, registrars
//    need to compare bindings in Contact URIs in REGISTER requests (see
//    Section 10.3.).  SIP and SIPS URIs are compared for equality
//    according to the following rules:
//
//       o  A SIP and SIPS URI are never equivalent.
//
//       o  Comparison of the userinfo of SIP and SIPS URIs is case-
//          sensitive.  This includes userinfo containing passwords or
//          formatted as telephone-subscribers.  Comparison of all other
//          components of the URI is case-insensitive unless explicitly
//          defined otherwise.
//
//       o  The ordering of parameters and header fields is not significant
//          in comparing SIP and SIPS URIs.
//
//       o  Characters other than those in the "reserved" set (see RFC 2396
//          [5]) are equivalent to their ""%" HEX HEX" encoding.
//
//       o  An IP address that is the result of a DNS lookup of a host name
//          does not match that host name.
//
//       o  For two URIs to be equal, the user, password, host, and port
//          components must match.
//
//          A URI omitting the user component will not match a URI that
//          includes one.  A URI omitting the password component will not
//          match a URI that includes one.
//
//          A URI omitting any component with a default value will not
//          match a URI explicitly containing that component with its
//          default value.  For instance, a URI omitting the optional port
//          component will not match a URI explicitly declaring port 5060.
//          The same is true for the transport-parameter, ttl-parameter,
//          user-parameter, and method components.
//
//             Defining sip:user@host to not be equivalent to
//             sip:user@host:5060 is a change from RFC 2543.  When deriving
//             addresses from URIs, equivalent addresses are expected from
//             equivalent URIs.  The URI sip:user@host:5060 will always
//             resolve to port 5060.  The URI sip:user@host may resolve to
//             other ports through the DNS SRV mechanisms detailed in [4].
//
//       o  URI uri-parameter components are compared as follows:
//
//          -  Any uri-parameter appearing in both URIs must match.
//
//          -  A user, ttl, or method uri-parameter appearing in only one
//             URI never matches, even if it contains the default value.
//
//          -  A URI that includes an maddr parameter will not match a URI
//             that contains no maddr parameter.
//
//          -  All other uri-parameters appearing in only one URI are
//             ignored when comparing the URIs.
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
//==EDOC========================================================================
inline bool CSipUri::operator==(IN const CSipUri& rSrc) const
{
    return IsEquivalent(rSrc);
}

//==SDOC========================================================================
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
//    SIP or SIPS URIs are equivalent.  In this specification, registrars
//    need to compare bindings in Contact URIs in REGISTER requests (see
//    Section 10.3.).  SIP and SIPS URIs are compared for equality
//    according to the following rules:
//
//       o  A SIP and SIPS URI are never equivalent.
//
//       o  Comparison of the userinfo of SIP and SIPS URIs is case-
//          sensitive.  This includes userinfo containing passwords or
//          formatted as telephone-subscribers.  Comparison of all other
//          components of the URI is case-insensitive unless explicitly
//          defined otherwise.
//
//       o  The ordering of parameters and header fields is not significant
//          in comparing SIP and SIPS URIs.
//
//       o  Characters other than those in the "reserved" set (see RFC 2396
//          [5]) are equivalent to their ""%" HEX HEX" encoding.
//
//       o  An IP address that is the result of a DNS lookup of a host name
//          does not match that host name.
//
//       o  For two URIs to be equal, the user, password, host, and port
//          components must match.
//
//          A URI omitting the user component will not match a URI that
//          includes one.  A URI omitting the password component will not
//          match a URI that includes one.
//
//          A URI omitting any component with a default value will not
//          match a URI explicitly containing that component with its
//          default value.  For instance, a URI omitting the optional port
//          component will not match a URI explicitly declaring port 5060.
//          The same is true for the transport-parameter, ttl-parameter,
//          user-parameter, and method components.
//
//             Defining sip:user@host to not be equivalent to
//             sip:user@host:5060 is a change from RFC 2543.  When deriving
//             addresses from URIs, equivalent addresses are expected from
//             equivalent URIs.  The URI sip:user@host:5060 will always
//             resolve to port 5060.  The URI sip:user@host may resolve to
//             other ports through the DNS SRV mechanisms detailed in [4].
//
//       o  URI uri-parameter components are compared as follows:
//
//          -  Any uri-parameter appearing in both URIs must match.
//
//          -  A user, ttl, or method uri-parameter appearing in only one
//             URI never matches, even if it contains the default value.
//
//          -  A URI that includes an maddr parameter will not match a URI
//             that contains no maddr parameter.
//
//          -  All other uri-parameters appearing in only one URI are
//             ignored when comparing the URIs.
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
//==EDOC========================================================================
inline bool CSipUri::operator!=(IN const CSipUri& rSrc) const
{
    return !IsEquivalent(rSrc);
}

//==SDOC========================================================================
//==
//==  GetScheme
//==
//==============================================================================
//
//  Returns:
//      URI Scheme.
//
//  Description:
//      Returns the scheme, either SIP or SIPS depending on the secured status.
//
//  See Also:
//      SetSecured
//
//==EDOC========================================================================
inline const char* CSipUri::GetScheme() const
{
    return m_bSecured ? szURISCHEME_SIPS : szURISCHEME_SIP;
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
inline IUri::EUriType CSipUri::GetUriType() const
{
    return m_bSecured ? IUri::eSIPS : IUri::eSIP;
}

//==SDOC========================================================================
//==
//==  GetHostPort
//==
//==============================================================================
//
//  Returns:
//      The host.
//
//  Description:
//      Provides access to the host element.
//
//==EDOC========================================================================
inline CHostPort& CSipUri::GetHostPort()
{
    return m_host;
}

//==SDOC========================================================================
//==
//==  GetHostPort
//==
//==============================================================================
//
//  Returns:
//      The host.
//
//  Description:
//      Provides access to the host element.
//
//==EDOC========================================================================
inline const CHostPort& CSipUri::GetHostPort() const
{
    return m_host;
}

//==SDOC========================================================================
//==
//==  GetParamList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the parameter list. It may be NULL.
//
//  Description:
//      Provides access to the parameter list.
//
//==EDOC========================================================================
inline CGenParamList* CSipUri::GetParamList()
{
    return m_pParamList;
}

//==SDOC========================================================================
//==
//==  GetParamList
//==
//==============================================================================
//
//  Returns:
//      Pointer to the parameter list. It may be NULL.
//
//  Description:
//      Provides access to the parameter list.
//
//==EDOC========================================================================
inline const CGenParamList* CSipUri::GetParamList() const
{
    return m_pParamList;
}

//==SDOC========================================================================
//==
//==  GetParam
//==
//==============================================================================
//
//  Parameters:
//    pszName:
//      Name of the parameter for which to look. A pre-defined list of
//      parameters exists in SipParser/UriParameter.h.
//
//  Returns:
//      A pointer to the value of the requested parameter. It is NULL if it does
//      not exist.
//
//  Description:
//      Returns a header paramater value. It can return NULL when the parameter
//      name is not found from the list.
//
//  See Also:
//      GetParamList, SetParamList
//
//==EDOC========================================================================
inline
const CToken* CSipUri::GetParam(IN const char* pszName) const
{
    // The const_cast modifier is acceptable here, because of
    // ePARAM_DONT_CREATE.
    return const_cast<CSipUri*>(this)->GetParam(pszName, ePARAM_DONT_CREATE);
}

//==SDOC========================================================================
//==
//==  SetParam
//==
//==============================================================================
//
//  Parameters:
//    pszName:
//      Name of the parameter to set. A pre-defined list of parameters exists in
//      SipParser/UriParameter.h.
//
//    rValue:
//      Value of the parameter to set.
//
//  Description:
//      Sets this URI parameter list with the <C>pszName</C> parameter. The
//      parameter is set with the value held by <C>rValue</C>.
//
//      This method takes care of the following:
//      - If this URI has no parameter list yet, one is created.
//      - If a <C>pszName</C> parameter already exists, its value is replaced
//        with the value held by <C>rValue</C>.
//      - If no <C>pszName</C> parameter already exists, it is created and
//        appended to the parameter list.
//
//  See Also:
//      GetParamList, SetParamList
//
//==EDOC========================================================================
inline
void CSipUri::SetParam(IN const char* pszName, IN const CToken& rValue)
{
    SetParam(pszName, rValue.GetString());
}

//==SDOC========================================================================
//==
//==  SetMissingPortBehavior
//==
//==============================================================================
//
//  Parameters:
//    bMissingPortBehavior:
//      True if the missing port must be considered equal to the default
//      port. False if the missing port must not be considered equal to
//      the default port.
//
//  Description:
//      Sets the missing default port behavior. This behavior is used only
//      when comparing two CSipUri using the IsEquivalent method or the
//      == operator.
//
//      When set to true, if the hostport construct of the URI contains no
//      port, the missing port is considered to be equal to the default
//      port (5060 for a SIP URI and 5061 for a SIPS URI).
//
//      When set to false, if the hostport construct of the URI contains no
//      port, no value is assumed for the missing port.
//
//      The value of the port (in the hostport construct) is not changed.
//
//  See Also:
//      GetMissingPortBehavior, IsEquivalent, operator==
//
//==EDOC========================================================================
inline void CSipUri::SetMissingPortBehavior(bool bMissingPortBehavior)
{
    ms_bMissingPortBehavior = bMissingPortBehavior;
}

//==SDOC========================================================================
//==
//==  GetMissingPortBehavior
//==
//==============================================================================
//
//  Description:
//      Gets the missing default port behavior. This behavior is used only
//      when comparing two CSipUri using the IsEquivalent method or the
//      == operator.
//
//      True means that if the hostport construct of the URI contains no
//      port, the missing port is considered to be equal to the default
//      port (5060 for a SIP URI and 5061 for a SIPS URI).
//
//      False means that if the hostport construct of the URI contains no
//      port, no value is assumed for the missing port.
//
//  See Also:
//      SetMissingPortBehavior, IsEquivalent, operator==
//
//==EDOC========================================================================
inline bool CSipUri::GetMissingPortBehavior()
{
    return ms_bMissingPortBehavior;
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline const CHeaderList* CSipUri::GetHeaderList() const
{
    return m_pHeaderList;
}

//==SDOC========================================================================
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
//==EDOC========================================================================
inline CHeaderList* CSipUri::GetHeaderList()
{
    return m_pHeaderList;
}

//==SDOC========================================================================
//==
//==  GetPassword
//==
//==============================================================================
//
//  Returns:
//      Userinfo's password. It may be NULL.
//
//  Description:
//      Provides access to the password.
//
//==EDOC========================================================================
inline const CToken* CSipUri::GetPassword() const
{
    return m_pTokPassword;
}


//==SDOC========================================================================
//==
//==  GetUser
//==
//==============================================================================
//
//  Returns:
//      Userinfo's user part.
//
//  Description:
//      Provides access to the username.
//
//==EDOC========================================================================
inline const CToken& CSipUri::GetUser() const
{
    return m_tokUser;
}

//==SDOC========================================================================
//==
//==  GetUser
//==
//==============================================================================
//
//  Returns:
//      Userinfo's user part.
//
//  Description:
//      Provides access to the username.
//
//==EDOC========================================================================
inline CToken& CSipUri::GetUser()
{
    return m_tokUser;
}

//==SDOC========================================================================
//==
//==  SetSecured
//==
//==============================================================================
//
//  Parameters:
//    eSecured:
//      If eSECURED, this CSipUri instance is a "sips", otherwise it is a "sip".
//
//  Description:
//      Sets the secured state for this URI. Secured means that SIPS URI are
//      used.
//
//  See Also:
//      IsSecured
//
//==EDOC========================================================================
inline void CSipUri::SetSecured(IN ESecurityFlag eSecured)
{
    m_bSecured = (eSecured == eSECURE);
}

//==SDOC========================================================================
//==
//==  IsSecured
//==
//==============================================================================
//
//  Returns:
//      True if the URI is SIPS.
//
//  Description:
//      Returns the secured status of this URI.
//
//  See Also:
//      SetSecured
//
//==EDOC========================================================================
inline bool CSipUri::IsSecured() const
{
    return m_bSecured;
}


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPURI_H
