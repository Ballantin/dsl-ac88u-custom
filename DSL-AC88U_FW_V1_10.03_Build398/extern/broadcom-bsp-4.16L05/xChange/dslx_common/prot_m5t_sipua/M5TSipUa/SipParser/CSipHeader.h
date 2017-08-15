//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSIPHEADER_H
#define MXG_CSIPHEADER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
// For MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
#include "Config/SipStackCfg.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CDATE_H
#include "SipParser/CDate.h"
#endif

#ifndef MXG_CGENERICPARAM_H
#include "SipParser/CGenericParam.h"
#endif

#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif

#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif


//-- Interface Realized and/or Parent
//-------------------------------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h" // ESipHeaderType
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CGenParamList;
class CRawHeader;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//@@CSIPHEADERVAR
//==SDOC========================================================================
//== File: CSipHeader.h
//==========================
// <TITLE CSipHeader Constants>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains some constants that are used by CSipHeader.
//
// Location:
//   SipParser/CSipHeader.h
//
// See Also:
//   CSipHeader
//
//==EDOC========================================================================

// <GROUP CSIPHEADERVAR>
//
// Description:
//     Specifying this value for the uMaxHeaders parameter of Parse() will parse
//     all available headers.
//-------------------------------------------------------------------------------
const uint8_t uHEADERS_PARSE_ALL = 0;

#if defined(MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR)
// <GROUP CSIPHEADERVAR>
//
// Description:
//     Define the name of the differentiator header.
//
// See also:
//     MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
//--------------------------------------------------------------------------
const char* const pszX_M5T_ORIGIN_HEADER_NAME  = "X-M5T-Origin";

// <GROUP CSIPHEADERVAR>
//
// Description:
//     Define the value for the differentiator header.
//
// See also:
//     MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
//--------------------------------------------------------------------------
const char* const pszX_M5T_ORIGIN_HEADER_VALUE = "locally generated";
#endif


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
// Class: CSipHeader
//==================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//      The CSipHeader class is primarily responsible for parsing, serializing,
//      and containing the header body. Each CSipHeader instance has a type
//      (ESipHeaderType) that gives it enough information (by accessing the
//      header definition table) to handle parsing and serialization for any
//      header type.
//
//      The CSipHeader class applies very generic parsing rules. In particular,
//      there is no semantic validation of the headers whatsoever. The only
//      validation that takes place is the syntactic validation, i.e., parsing
//      errors are flagged.
//
//      The CSipHeader class is also a single-linked chain of CSipHeaders. This
//      functionality, referred to as the header chain, is used for storing
//      multiple instances of the same header type. For example, two Contacts
//      would be available through one CSipHeader. To access the second contact,
//      the GetNextHeader() method would be used on the first contact. This
//      functionality works equally for multi header that can be comma-separated
//      or not (for example, many Www-Authenticate headers can be found in a
//      single packet, but cannot be combined on a single comma-separated line).
//
//      The CSipHeader class also contains the controls for header generation
//      such as comma separation configuration and short/long header name form
//      configuration. See CSipCoreConfig::SetCommaSeparatedHeader and
//      CSipCoreConfig::SetHeaderFormPreference for more details.
//
//  #Raw versus Parsed header#
//      The CSipHeader class can contain two versions of the same data. The raw
//      data is the data received from the network. The parsed data is the raw
//      data after it has been processed.
//
//      A CSipHeader is considered as "raw header" when its data has not been
//      parsed.
//
//      If the header type can be comma-separated, a single raw header can
//      contain multiple headers. For example, a raw "Allow" header's data could
//      look like this: "allow1, allow2". This raw header would be counted as
//      one header, because until the raw data is parsed, the CSipHeader cannot
//      know that the raw data actually contains two headers.
//
//      The header chain can be made up of parsed and raw headers, in any
//      order. The basic use case consists in calling the Parse() method, after
//      which the first headers of the chain are parsed, and the end
//      of the chain (if not entirely parsed) will contain raw headers.
//
//      Refer to CSipPacketParser for more details on the parsing strategy.
//
//  #Generic parsing structure#
//      As stated before, the CSipHeader class generically handles any SIP
//      header. At its core, a SIP header is made up of a name, a separator
//      (such as ':'), a header-specific body, and an optional parameter list.
//
//      Since headers are contextual (can be found in the context of a SIP
//      packet or SIP URI), the header name, name-body separator, and
//      the terminating separator (CRLF for SIP packet, '&' for SIP URIs) are
//      not handled in this class. As such, the CSipHeader is context-less.
//
//      CSipHeader handles the body of the header, which includes the
//      header-specific data and the optional parameter list. It also handles
//      comma-separated instances, since these do not have a header name
//      in-between.
//
//      To take up the least space possible in RAM, the parsed data is stored
//      in a generic byte array. Users of the CSipHeader don't actually need to
//      know the details of this implementation, but do need to be warned that
//      improper use of the CSipHeader interface (particularly any of the
//      helpers for header body access such as GetAllow, SetContactWildcard,
//      etc.) can lead to very strange problems. For example, using the SetAllow
//      method when the header type is not eHDR_ALLOW can and will probably
//      cause invalid data accesses.
//
//      With that said, the byte array is allocated when the CSipHeader is
//      created. Using a byte array minimises memory fragmentation. When a new
//      header is to be created, the easiest way to do so is by using the access
//      helpers. These automatically create the correct data structures. In this
//      case, the CSipHeader only contains parsed data, and no raw data.
//
//  #Serialization and RAM use#
//      When the CSipHeader contains raw data, it takes up this space in RAM. If
//      that same header is parsed, the raw data still takes up the same RAM as
//      before, and now the parsed version of the data also uses new RAM. For
//      this reason, it is best to avoid parsing headers that are not really
//      required by the application.
//
//      At serialization time, the CSipHeader first checks if it contains raw
//      data. Regardless of the parsed data, if raw data is available, it is
//      output into the buffer instead of the parsed data. The parsed data is
//      serialized only if no raw data is available.
//
//      The reasons for this behaviour are twofold. When raw data is available,
//      it is assumed that this raw data comes from the network. The first
//      reason is that serializing the raw data is much less costly in
//      execution, because it consists in a single memcpy. Serializing the
//      parsed data is much longer because it breaks down into many function
//      calls and memcpys. The second reason is that parsing the data may have
//      altered it. For example, when parsing "allow\r\n ,allow2", the extra
//      "\r\n " is lost. Serializing the raw data thus sends the header as it
//      was received, which should help with interoperation scenarios.
//
//      The problem with serializing the raw data is that any change made by the
//      user to the parsed data is ignored at Serialization time. It is thus
//      UP TO THE USER to nullify the raw data if he wishes to use an existing
//      header and modify it.
//
//  Warning:
//      The user of the CSipHeader is required to verify the header type before
//      using any of the Set/Get methods. In the interest of keeping the code
//      size small, no verifications are made versus the header type or the
//      current real use of the byte array data structure in these methods. It
//      is thus VERY IMPORTANT that CSipHeader users make sure that they are
//      using the right kind of interface!
//
//  #Header accessor nomenclature#
//
//      Access to each header's internal data is supplied through simple
//      methods. Each header has a method to get and set each of its internal
//      data constructs as per the appropriate ABNF. Note that there are no
//      specific Get/Set methods for the parameter lists (usually described as
//      *(SEMI param) in the ABNF ) as this structure is generically handled by
//      the CSipHeader's m_pParamList data member (through the
//      CSipHeader::GetParamList() methods).
//
//      The header access methods are grouped by RFC/draft document.
//
//      Get and Set methods are formed as such:
//          Get(HeaderName)(DataItem)
//      where:
//          - (HeaderName) is the header name, such as Allow or Reply-To;
//          - (DataItem) is the name of the item to access. If there is only one
//            item in the header's ABNF, then the (DataItem) part is omitted in
//            the method name.
//
//      Example:
//          <CODE>
//           RFC 3261 ABNF:
//            Allow  =  "Allow" HCOLON [Method *(COMMA Method)]
//          </CODE>
//
//          The Allow header only has one data item (Method). As such, its
//          accessors are:
//              - GetAllow, SetAllow
//
//      Example:
//       <CODE>
//         RFC 3262 ABNF:
//           RAck          =  "RAck" HCOLON response-num LWS CSeq-num LWS Method
//           response-num  =  1*DIGIT
//           CSeq-num      =  1*DIGIT
//       </CODE>
//
//          The RAck header has three data items. As such, its
//          accessors are:
//              - GetRackCSeq, GetRackMethod, GetRackResponseNum
//
//      Example:
//          <CODE>
//            RFC 3261 ABNF:
//              Reply-To      =  "Reply-To" HCOLON rplyto-spec
//              rplyto-spec   =  ( name-addr / addr-spec )
//                             *( SEMI rplyto-param )
//              rplyto-param  =  generic-param
//          </CODE>
//
//          The Reply-To header has one data item, plus a parameter list. As
//          such, its accessors are:
//              - GetReplyTo (const and non-const)
//              These both access the ( name-addr / addr-spec ) construct (not
//              the rplyto-spec construct), since the *( SEMI rplyto-param )
//              construct is handled by the generic parameter list m_pParamList.
//
//      Example:
//      <CODE>
//        RFC 3261 ABNF:
//          Proxy-Authenticate  =  "Proxy-Authenticate" HCOLON challenge
//          challenge           =  ("Digest" LWS digest-cln *(COMMA digest-cln))
//                                 / other-challenge
//          other-challenge     =  auth-scheme LWS auth-param
//                                 *(COMMA auth-param)
//      </CODE>
//
//          The authentication headers are special cases, as they all contain an
//          auth-scheme, followed by a parameter list that does not start with a
//          SEMI. In those cases, all parameters have been put into the
//          parameter list (m_pParamList), as usual. To avoid any confusion, the
//          accessors for these headers specify that the auth-scheme is fetched:
//              - GetAuthorizationScheme
//              - GetWwwAuthenticateScheme
//              - GetProxyAuthenticateScheme
//              - GetProxyAuthorizationScheme
//
//      Example:
//      <CODE>
//      Allow: INVITE, ACK, OPTIONS, CANCEL, BYE
//      </CODE>
//
//      After this header is parsed, it contains a chain of headers. The first
//      contains a CToken with "INVITE" for data. The second CSipHeader contains
//      a CToken with "ACK" for data, and the third a CToken with "OPTIONS" for
//      data, etc. Calling GetAllow right after the header is parsed returns a
//      reference to the CToken with "INVITE" for data. To have access to the
//      following CSipHeader in the chain, it is necessary to use GetNextHeader
//      followed by GetAllow to access the CTokens.
//
//      Example:
//      <CODE>
//      Accept-Language: da, en-gb;q=0.8, en;q=0.7
//      </CODE>
//
//      After this header is parsed, it contains a chain of headers. The first
//      contains a CToken with "da" for data. The second CSipHeader contains a
//      CToken with "en-gb" for data and a parameter list with a parameter "q"
//      set to "0.8", and the third a CToken with "en" for data and a parameter
//      "q" set to "0.7". Calling GetAcceptLanguage right after the header is
//      parsed returns a reference to the CToken with "da" for data. To have
//      access to the following CSipHeader in the chain, it is necessary to use
//      GetNextHeader followed by GetAcceptLanguage to access the CTokens or
//      GetParamList to access the parameter list.
//
//      Example:
//      <CODE>
//      Call-Info: <http://wwww.example.com/alice/photo.jpg> ;purpose=icon,
//       <http://www.example.com/alice/> ;purpose=info
//      </CODE>
//
//      After this header is parsed, it contains a chain of headers. The first
//      contains a CNameAddr with URI "http://wwww.example.com/alice/photo.jpg"
//      and a parameter list with the parameter purpose set to "icon". The
//      second CSipHeader contains a CNameAddr with URI
//      "http://www.example.com/alice/" and a parameter list with the parameter
//      purpose set to "info". Calling GetCallId right after the header is
//      parsed returns a reference to the CNameAddr with
//      "http://wwww.example.com/alice/photo.jpg" for URI. To have access to the
//      following CSipHeader in the chain, it is necessary to use GetNextHeader
//      followed by GetCallId to access the second CNameAddress or GetParamList
//      to access the parameter list.
//
// Location:
//   SipParser/CSipHeader.h
//
//==============================================================================
class CSipHeader
{
//-- Friend Declarations
//------------------------

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END


//-- New types definition
//-------------------------
public:

    // Description:
    //  Indicate what to reset to the Reset method.
    enum EResetLevel
    {
        // Description:
        //  Resets the parameter list and the header values.
        eBASIC_RESET = 0x1000,
        // Description:
        //  Does basic reset in addition of resetting the m_pNextHeader member.
        eNEXT_HDRS_RESET = ((int)eBASIC_RESET | 0x1),
        // Description:
        //  Does basic reset in addition of resetting the m_pRawHeader member.
        eRAW_HDR_RESET = ((int)eBASIC_RESET | 0x2),
        // Description:
        //  Resets all
        eCOMPLETE_RESET =
            ((int)eBASIC_RESET | eNEXT_HDRS_RESET | eRAW_HDR_RESET)
    };

    // Description:
    //  Indicates to use the short or long header form.
    enum EHeaderNameForm
    {
        // Description:
        //  Use the short header form.
        eSHORT_FORM_NAME,
        // Description:
        //  Use the long header form.
        eLONG_FORM_NAME,
    };

    // Description:
    //  Indicates whether or not the parameter must be created when calling
    //  GetParam.
    enum EParameterCreationBehavior
    {
        // Description:
        //  Dont' create the parameter.
        ePARAM_DONT_CREATE,
        // Description:
        //  Create the parameter.
        ePARAM_CREATE_NEW
    };
protected:
    // This union encapsulates all possible parsed data structures. All header
    // type containment are represented by either of the structures that
    // compose this union. The important thing to remember is that a union
    // takes up the memory of its largest data structure.
    //-------------------------------------------------------------------------

    // eDT_NAMEADDR.
    // eDT_CONTACT.
    //--------------
    struct SNameAddrForm
    {
        CNameAddr m_nameAddr;
        bool m_bIsWildCard;

        SNameAddrForm()
        :   m_bIsWildCard(false)
        {}
    };

    // eDT_PARAMETER.
    //----------------
    struct SParamForm
    {
        CGenericParam m_param;
        SParamForm()
        :   m_param(CGenericParam::eCS_SIP_HEADER)
        {}
    };

    // eDT_TOKEN
    // eDT_TOKEN_SLASH_TOKEN
    // eDT_UINT32
    // eDT_FLOAT
    // eDT_UINT32_TOKEN
    // eDT_TOKEN_AUTH
    // eDT_UINT32_UINT32_TOKEN
    // eDT_STRING
    // eDT_VIA
    struct STokenForm
    {
        CToken m_tokFirst;
        CToken m_tokSecond;
        CToken m_tokThird;
        CHostPort m_host;

        STokenForm()
        :   m_tokFirst(CToken::eCS_SIP_HEADER),
            m_tokSecond(CToken::eCS_SIP_HEADER),
            m_tokThird(CToken::eCS_SIP_HEADER)
        {}
    };

    // eDT_DATE.
    //-----------
    struct SDateForm
    {
        CDate m_date;
    };

    // eDT_MAILBOX_URI
    struct SMailboxUriForm
    {
        CMailboxUri m_mailboxUri;
    };

    // eDT_PARAMETER_COMMA_SEP has no data form. It only contains parameters.

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor. Takes any type of ESipHeaderType.
    //-------------------------------------------------
    CSipHeader(IN ESipHeaderType eHeader);

    // Summary:
    //  Copy constructor.
    //--------------------
    CSipHeader(IN const CSipHeader& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSipHeader();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSipHeader& operator=(IN const CSipHeader& rSrc);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(INOUT CSipHeader& rSrc);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator!=(INOUT CSipHeader& rSrc);

    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Retrieves the long or short form following ShortNameForm configuration.
    //--------------------------------------------------------------------------
    const char* GetHeaderName() const;

    // Summary:
    //  Returns the header type.
    //---------------------------
    ESipHeaderType GetHeaderType() const;

    // Summary:
    //  Retrieves the long form header name for this header.
    //-------------------------------------------------------
    const char* GetLongHeaderName() const;

    // Summary:
    //  Retrieves the short form header name for this header, if available.
    //----------------------------------------------------------------------
    const char* GetShortHeaderName() const;

    // Summary:
    //  Provides access to the parameter list.
    //-----------------------------------------
    inline const CGenParamList* GetParamList(INOUT mxt_result* pres = NULL) const;

    // <COMBINE CSipHeader::GetParamList@INOUT mxt_result*@const>
    //-----------------------------------------
    inline CGenParamList* GetParamList(INOUT mxt_result* pres = NULL);

    // Summary:
    //  Provides access to the parameter list.
    //-----------------------------------------
    inline const CGenParamList* GetParamList(OUT mxt_result& rres) const;

    // <COMBINE CSipHeader::GetParamList@OUT mxt_result&@const>
    //-----------------------------------------
    inline CGenParamList* GetParamList(OUT mxt_result& rres);


    // Summary:
    //  Sets the parameter list.
    //---------------------------
    mxt_result SetParamList(IN TO CGenParamList* pList);

    // Summary:
    //  Inserts a header at the end of the chain.
    //--------------------------------------------
    mxt_result AppendNextHeader(IN TO CSipHeader* pNewHeader,
                                IN bool bTakeOwnershipOnSuccess = false);

    // Summary:
    //  Inserts a header at the given 0-based index in the chain. Index 0
    //  refers to this CSipHeader's first next header.
    //--------------------------------------------------------------------
    mxt_result InsertNextHeader(IN unsigned int uIndex,
                                IN TO CSipHeader* pNewHeader,
                                IN bool bTakeOwnershipOnSuccess = false);

    // Summary:
    //  Returns the number of headers in the chain, but omits the current
    //  header.
    //--------------------------------------------------------------------
    unsigned int GetNbNextHeaders() const;

    // Summary:
    //  Returns the number of parsed headers in the chain, including the
    //  current header.
    //-------------------------------------------------------------------
    unsigned int GetNbParsedHeaders() const;

    // Summary:
    //  Returns a pointer to the chained header at the given 0-based index.
    //----------------------------------------------------------------------
    inline const CSipHeader* GetNextHeader(IN unsigned int uIndex = 0) const;

    // <COMBINE CSipHeader::GetNextHeader@IN unsigned int@const>
    //------------------------------------------
    inline CSipHeader* GetNextHeader(IN unsigned int uIndex = 0);

    // Summary:
    //  Replaces the next headers with the headers in parameter.
    //-----------------------------------------------------------
    mxt_result ReplaceNextHeader(IN TO CSipHeader* pNewHeader);

    // Summary:
    //  Removes a header from the chain, at the given 0-based index. Index 0
    //  refers to this CSipHeader's first next header.
    //-----------------------------------------------------------------------
    mxt_result RemoveNextHeader(IN unsigned int uIndex);

    // Summary:
    //  Removes a header from the chain and returns it, at the given 0-based
    //  index. Index 0 refers to this CSipHeader's first next header.
    //-----------------------------------------------------------------------
    GO CSipHeader* UnlinkNextHeader(IN unsigned int uIndex);

    // Summary:
    //  Unlinks the top header from the chain.  The returned header is the
    //  chain excluding the current header.
    //---------------------------------------------------------------------
    GO CSipHeader* UnlinkTopHeader();


    // Summary:
    //  Returns true if this header is empty, and can be empty as per the
    //  header definition table.
    //--------------------------------------------------------------------
    bool IsEmptyHeader() const;

    // Summary:
    //  Returns true if parsed data (parsed from raw or set by application) is
    //  available.
    //-------------------------------------------------------------------------
    bool IsParsedDataAvailable() const;

    // Summary:
    //  Returns true if the current instance is equivalent to the cited
    //  instance, without regard to the next headers.
    //------------------------------------------------------------------
    bool IsSingleHdrEquivalent(IN const CSipHeader& rSrc) const;

    // Summary:
    //  Copies this header without its next headers.
    void CopySingleHeader(IN const CSipHeader& rSrc);

    // Summary:
    //  Provides access to the raw header.
    //-------------------------------------
    inline const CRawHeader* GetRawHeader() const;

    // Provides access to the raw header.
    //--------------------------------------
    inline CRawHeader* GetRawHeader();

    // Summary:
    //  Sets the raw header. Ownership is taken.
    //-------------------------------------------
    mxt_result SetRawHeader(IN TO CRawHeader* pRawHeader);

    // Summary:
    //  Parses the header chain for up to uMaxHeaders.
    //------------------------------------------------
    mxt_result Parse( IN unsigned int uMaxHeaders = uHEADERS_PARSE_ALL );

    // Summary:
    //  Clears all parsed/Set data.
    //------------------------------
    void Reset(IN EResetLevel = eCOMPLETE_RESET);

    // Summary:
    //  Outputs the body of the header into the blob.
    //------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;


    //-- << Global settings >>
    //--------------------------------------------

    // Summary:
    //  Sets the form of header name to use for all header types.
    //------------------------------------------------------------
    static void SetNameForm(IN EHeaderNameForm eHeaderNameForm);


    //-- << Header accessors >>
    //--------------------------------

    // This section contains helper methods that relate to specific headers
    // (e.g. Via, Contact, Accept, etc). Since all header types were abstracted
    // in this basic class, access to the parsed contents of the headers will
    // be facilitated through small methods in this section.
    //--------------------------------------------------------------------------

    // RFC 3261 headers.
    //-------------------

    // Summary:
    //  Provides access to the Extension-header's header-name construct.
    //-------------------------------------------------------------------
    const CToken& GetExtensionHeaderName() const;

    // <COMBINE CSipHeader::GetExtensionHeaderName@const>
    //---------------------------------------------------
    CToken& GetExtensionHeaderName();

    // Summary:
    //  Provides access to the Extension-header's header-value construct.
    //--------------------------------------------------------------------
    const CToken& GetExtensionHeaderValue() const;

    // <COMBINE CSipHeader::GetExtensionHeaderValue@const>
    //----------------------------------------------------
    CToken& GetExtensionHeaderValue();

    // Summary:
    //  Provides access to the Accept header's m-type construct.
    //-----------------------------------------------------------
    const CToken& GetAcceptMType() const;

    // <COMBINE CSipHeader::GetAcceptMType@const>
    //-------------------------------------------
    CToken& GetAcceptMType();

    // Summary:
    //  Provides access to the Accept header's m-subtype construct.
    //--------------------------------------------------------------
    const CToken& GetAcceptMSubType() const;

    // <COMBINE CSipHeader::GetAcceptMSubType@const>
    //----------------------------------------------
    CToken& GetAcceptMSubType();

    // Summary:
    //  Provides access to the Accept-Encoding header's content-coding
    //  construct.
    //-----------------------------------------------------------------
    const CToken& GetAcceptEncoding() const;

    // <COMBINE CSipHeader::GetAcceptEncoding@const>
    //----------------------------------------------
    CToken& GetAcceptEncoding();

    // Summary:
    //  Provides access to the Accept-Language header's language-range
    //  construct.
    //-----------------------------------------------------------------
    const CToken& GetAcceptLanguage() const;

    // <COMBINE CSipHeader::GetAcceptLanguage@const>
    //----------------------------------------------
    CToken& GetAcceptLanguage();

    // Summary:
    //  Provides access to the Alert-Info header's absoluteURI construct.
    //--------------------------------------------------------------------
    const CNameAddr& GetAlertInfo() const;

    // <COMBINE CSipHeader::GetAlertInfo@const>
    //-----------------------------------------
    CNameAddr& GetAlertInfo();

    // Summary:
    //  Provides access to the Allow header's Method construct.
    //----------------------------------------------------------
    const CToken& GetAllow() const;

    // <COMBINE CSipHeader::GetAllow@const>
    //-------------------------------------
    CToken& GetAllow();

    // Summary:
    //  Provides access to the Authorization header's auth-scheme construct.
    //-----------------------------------------------------------------------
    const CToken& GetAuthorizationScheme() const;

    // <COMBINE CSipHeader::GetAuthorizationScheme@const>
    //---------------------------------------------------
    CToken& GetAuthorizationScheme();

    // Summary:
    //  Provides access to the Call-Id header's callid construct.
    //------------------------------------------------------------
    const CToken& GetCallId() const;

    // <COMBINE CSipHeader::GetCallId@const>
    //--------------------------------------
    CToken& GetCallId();

    // Summary:
    //  Provides access to the Call-Info header's absoluteURI construct.
    //-------------------------------------------------------------------
    const CNameAddr& GetCallInfo() const;

    // <COMBINE CSipHeader::GetCallInfo@const>
    //----------------------------------------
    CNameAddr& GetCallInfo();

    // Summary:
    //  Provides access to the Contact header's (name-addr / addr-spec)
    //  construct.
    //------------------------------------------------------------------
    const CNameAddr& GetContact() const;

    // <COMBINE CSipHeader::GetContact@const>
    //---------------------------------------
    CNameAddr& GetContact();

    // Summary:
    //  Returns true if the Contact header's value is STAR.
    //------------------------------------------------------
    bool IsContactWildcard() const;

    // Summary:
    //  Sets the value of the STAR for the Contact header's
    //  (name-addr / addr-spec) construct.
    //------------------------------------------------------
    void SetContactWildcard(IN bool bIsWildcard);

    // Summary:
    //  Provides access to the Content-Description header's text construct.
    //----------------------------------------------------------------------
    const CToken& GetContentDescription() const;

    // <COMBINE CSipHeader::GetContentDescription@const>
    //---------------------------------------------------
    CToken& GetContentDescription();

    // Summary:
    //  Provides access to the Content-Disposition header's disp-type
    //  construct.
    //----------------------------------------------------------------
    const CToken& GetContentDisposition() const;

    // <COMBINE CSipHeader::GetContentDisposition@const>
    //--------------------------------------------------
    CToken& GetContentDisposition();

    // Summary:
    //  Provides access to the Content-Encoding header's content-coding
    //  construct.
    //------------------------------------------------------------------
    const CToken& GetContentEncoding() const;

    // <COMBINE CSipHeader::GetContentEncoding@const>
    //-----------------------------------------------
    CToken& GetContentEncoding();

    // Summary:
    //  Provides access to the Content-ID header's addr-spec (RFC 2822) construct.
    const CMailboxUri& GetContentId() const;

    // <COMBINE CSipHeader::GetContentId@const>
    CMailboxUri& GetContentId();

    // Summary:
    //  Provides access to the Content-Language header's language-tag
    //  construct.
    //----------------------------------------------------------------
    const CToken& GetContentLanguage() const;

    // <COMBINE CSipHeader::GetContentLanguage@const>
    //-----------------------------------------------
    CToken& GetContentLanguage();

    // Summary:
    //  Provides access to the Content-Length header's 1*DIGIT construct.
    //--------------------------------------------------------------------
    const CToken& GetContentLength() const;

    // <COMBINE CSipHeader::GetContentLength@const>
    //---------------------------------------------
    CToken& GetContentLength();

    // Summary:
    //  Provides access to the Content-Language header's mechanism construct.
    //------------------------------------------------------------------------
    const CToken& GetContentTransferEncoding() const;

    // <COMBINE CSipHeader::GetContentTransferEncoding@const>
    //--------------------------------------------------------
    CToken& GetContentTransferEncoding();

    // Summary:
    //  Provides access to the Content-Type header's m-type construct.
    //-----------------------------------------------------------------
    const CToken& GetContentTypeMType() const;

    // <COMBINE CSipHeader::GetContentTypeMType@const>
    //------------------------------------------------
    CToken& GetContentTypeMType();

    // Summary:
    //  Provides access to the Content-Type header's m-subtype construct.
    //--------------------------------------------------------------------
    const CToken& GetContentTypeMSubType() const;

    // <COMBINE CSipHeader::GetContentTypeMSubType@const>
    //---------------------------------------------------
    CToken& GetContentTypeMSubType();

    // Summary:
    //  Provides access to the CSeq header's 1*DIGIT construct.
    //----------------------------------------------------------
    const CToken& GetCSeqNumber() const;

    // <COMBINE CSipHeader::GetCSeqNumber@const>
    //------------------------------------------
    CToken& GetCSeqNumber();

    // Summary:
    //  Provides access to the CSeq header's Method construct.
    //---------------------------------------------------------
    const CToken& GetCSeqMethod() const;

    // <COMBINE CSipHeader::GetCSeqMethod@const>
    //------------------------------------------
    CToken& GetCSeqMethod();

    // Summary:
    //  Provides access to the Date header's SIP-date construct.
    //-----------------------------------------------------------
    const CDate& GetDate() const;

    // <COMBINE CSipHeader::GetDate@const>
    //------------------------------------------
    CDate& GetDate();

    // Summary:
    //  Provides access to the Error-Info header's error-uri construct.
    //------------------------------------------------------------------
    const CNameAddr& GetErrorInfo() const;

    // <COMBINE CSipHeader::GetDate@const>
    //------------------------------------
    CNameAddr& GetErrorInfo();

    // Summary:
    //  Provides access to the Expires header's delta-seconds construct.
    //-------------------------------------------------------------------
    const CToken& GetExpires() const;

    // <COMBINE CSipHeader::GetExpires@const>
    //---------------------------------------
    CToken& GetExpires();

    // Summary:
    //  Provides access to the Flow Timer header's delta-seconds construct.
    //-------------------------------------------------------------------
    const CToken& GetFlowTimer() const;

    // <COMBINE CSipHeader::GetExpires@const>
    //---------------------------------------
    CToken& GetFlowTimer();

    // Summary:
    //  Provides access to the From header's from-spec construct.
    //------------------------------------------------------------
    const CNameAddr& GetFrom() const;

    // <COMBINE CSipHeader::GetFrom@const>
    //------------------------------------
    CNameAddr& GetFrom();

    // Summary:
    //  Provides access to the In-Reply-To header's callid construct.
    //----------------------------------------------------------------
    const CToken& GetInReplyTo() const;

    // <COMBINE CSipHeader::GetInReplyTo@const>
    //-----------------------------------------
    CToken& GetInReplyTo();

    // Summary:
    //  Provides access to the Max-Forwards header's 1*DIGIT construct.
    //------------------------------------------------------------------
    const CToken& GetMaxForwards() const;

    // <COMBINE CSipHeader::GetMaxForwards@const>
    //-------------------------------------------
    CToken& GetMaxForwards();

    // Summary:
    //  Provides access to the MIME-Version header's "1*DIGIT "." 1*DIGIT"
    //  construct.
    //---------------------------------------------------------------------
    const CToken& GetMimeVersion() const;

    // <COMBINE CSipHeader::GetMimeVersion@const>
    //-------------------------------------------
    CToken& GetMimeVersion();

    // Summary:
    //  Provides access to the Min-Expires header's delta-seconds construct.
    //-----------------------------------------------------------------------
    const CToken& GetMinExpires() const;

    // <COMBINE CSipHeader::GetMinExpires@const>
    //------------------------------------------
    CToken& GetMinExpires();

    // Summary:
    //  Provides access to the Organization header's [TEXT-UTF8-TRIM]
    //  construct.
    //----------------------------------------------------------------
    const CToken& GetOrganization() const;

    // <COMBINE CSipHeader::GetOrganization@const>
    //--------------------------------------------
    CToken& GetOrganization();

    // Summary:
    //  Provides access to the Priority header's priority-value construct.
    //---------------------------------------------------------------------
    const CToken& GetPriority() const;

    // <COMBINE CSipHeader::GetPriority@const>
    //----------------------------------------
    CToken& GetPriority();

    // Summary:
    //  Provides access to the Proxy-Authenticate header's auth-scheme
    //  construct.
    //-----------------------------------------------------------------
    const CToken& GetProxyAuthenticateScheme() const;

    // <COMBINE CSipHeader::GetProxyAuthenticateScheme@const>
    //-------------------------------------------------------
    CToken& GetProxyAuthenticateScheme();

    // Summary:
    //  Provides access to the Proxy-Authorization header's auth-scheme
    //  construct.
    //------------------------------------------------------------------
    const CToken& GetProxyAuthorizationScheme() const;

    // <COMBINE CSipHeader::GetProxyAuthorizationScheme@const>
    //--------------------------------------------------------
    CToken& GetProxyAuthorizationScheme();

    // Summary:
    //  Provides access to the Proxy-Require header's option-tag construct.
    //----------------------------------------------------------------------
    const CToken& GetProxyRequire() const;

    // <COMBINE CSipHeader::GetProxyRequire@const>
    //--------------------------------------------
    CToken& GetProxyRequire();

    // Summary:
    //  Provides access to the Record-Route header's name-addr construct.
    //--------------------------------------------------------------------
    const CNameAddr& GetRecordRoute() const;

    // <COMBINE CSipHeader::GetRecordRoute@const>
    //-------------------------------------------
    CNameAddr& GetRecordRoute();

    // Summary:
    //  Provides access to the Reply-To header's ( name-addr / addr-spec )
    //  construct.
    //---------------------------------------------------------------------
    const CNameAddr& GetReplyTo() const;

    // <COMBINE CSipHeader::GetReplyTo@const>
    //---------------------------------------
    CNameAddr& GetReplyTo();

    // Summary:
    //  Provides access to the Require header's option-tag construct.
    //----------------------------------------------------------------
    const CToken& GetRequire() const;

    // <COMBINE CSipHeader::GetRequire@const>
    //---------------------------------------
    CToken& GetRequire();

    // Summary:
    //  Provides access to the Retry-After header's delta-seconds construct.
    //-----------------------------------------------------------------------
    const CToken& GetRetryAfter() const;

    // <COMBINE CSipHeader::GetRetryAfter@const>
    //------------------------------------------
    CToken& GetRetryAfter();

    // Summary:
    //  Provides access to the Retry-After header's comment construct.
    const CToken& GetRetryAfterComment() const;

    // <COMBINE CSipHeader::GetRetryAfterComment@const>
    CToken& GetRetryAfterComment();

    // Summary:
    //  Provides access to the Route header's name-addr construct.
    //-------------------------------------------------------------
    const CNameAddr& GetRoute() const;

    // <COMBINE CSipHeader::GetRoute@const>
    //-------------------------------------
    CNameAddr& GetRoute();

    // Summary:
    //  Provides access to the Server header's server-val construct.
    //---------------------------------------------------------------
    const CToken& GetServer() const;

    // <COMBINE CSipHeader::GetServer@const>
    //--------------------------------------
    CToken& GetServer();

    // Summary:
    //  Provides access to the Subject header's [TEXT-UTF8-TRIM] construct.
    //----------------------------------------------------------------------
    const CToken& GetSubject() const;

    // <COMBINE CSipHeader::GetSubject@const>
    //---------------------------------------
    CToken& GetSubject();

    // Summary:
    //  Provides access to the Supported header's [option-tag] construct.
    //--------------------------------------------------------------------
    const CToken& GetSupported() const;

    // <COMBINE CSipHeader::GetSupported@const>
    //-----------------------------------------
    CToken& GetSupported();

    // Summary:
    //  Provides access to the Timestamp header's 1*(DIGIT) [ "." *(DIGIT) ]
    //  [ LWS delay ] construct.
    //-----------------------------------------------------------------------
    const CToken& GetTimestamp() const;

    // <COMBINE CSipHeader::GetTimestamp@const>
    //-----------------------------------------
    CToken& GetTimestamp();

    // Summary:
    //  Provides access to the To header's ( name-addr / addr-spec ) construct.
    //--------------------------------------------------------------------------
    const CNameAddr& GetTo() const;

    // <COMBINE CSipHeader::GetTo@const>
    //----------------------------------
    CNameAddr& GetTo();

    // Summary:
    //  Provides access to the Unsupported header's option-tag construct.
    //--------------------------------------------------------------------
    const CToken& GetUnsupported() const;

    // <COMBINE CSipHeader::GetUnsupported@const>
    //-------------------------------------------
    CToken& GetUnsupported();

    // Summary:
    //  Provides access to the User-Agent header's server-val construct.
    //-------------------------------------------------------------------
    const CToken& GetUserAgent() const;

    // <COMBINE CSipHeader::GetUserAgent@const>
    //-----------------------------------------
    CToken& GetUserAgent();

    // Summary:
    //  Provides access to the Via header's protocol-name construct.
    //---------------------------------------------------------------
    const CToken& GetViaProtocolName() const;

    // <COMBINE CSipHeader::GetViaProtocolName@const>
    //-----------------------------------------------
    CToken& GetViaProtocolName();

    // Summary:
    //  Provides access to the Via header's protocol-version construct.
    //------------------------------------------------------------------
    const CToken& GetViaProtocolVersion() const;

    // <COMBINE CSipHeader::GetViaProtocolVersion@const>
    //--------------------------------------------------
    CToken& GetViaProtocolVersion();

    // Summary:
    //  Provides access to the Via header's transport construct.
    //-----------------------------------------------------------
    const CToken& GetViaTransport() const;

    // <COMBINE CSipHeader::GetViaTransport@const>
    //--------------------------------------------
    CToken& GetViaTransport();

    // Summary:
    //  Provides access to the Via header's sent-by construct.
    //---------------------------------------------------------
    const CHostPort& GetViaSentBy() const;

    // <COMBINE CSipHeader::GetViaSentBy@const>
    //-----------------------------------------
    CHostPort& GetViaSentBy();

    // Summary:
    //  Provides access to the Warning header's warning-value construct.
    //-------------------------------------------------------------------
    const CToken& GetWarning() const;

    // <COMBINE CSipHeader::GetWarning@const>
    //---------------------------------------
    CToken& GetWarning();

    // Summary:
    //  Provides access to the Www-Authenticate header's auth-scheme construct.
    //--------------------------------------------------------------------------
    const CToken& GetWwwAuthenticateScheme() const;

    // <COMBINE CSipHeader::GetWwwAuthenticateScheme@const>
    //-----------------------------------------------------
    CToken& GetWwwAuthenticateScheme();


    // RFC 3262 headers.
    //-------------------

    // Summary:
    //  Provides access to the RAck header's response-num construct.
    //---------------------------------------------------------------
    const CToken& GetRackResponseNum() const;

    // <COMBINE CSipHeader::GetRackResponseNum@const>
    //-----------------------------------------------
    CToken& GetRackResponseNum();

    // Summary:
    //  Provides access to the RAck header's CSeq-num construct.
    //-----------------------------------------------------------
    const CToken& GetRackCSeq() const;

    // <COMBINE CSipHeader::GetRackCSeq@const>
    //----------------------------------------
    CToken& GetRackCSeq();

    // Summary:
    //  Provides access to the RAck header's Method construct.
    //---------------------------------------------------------
    const CToken& GetRackMethod() const;

    // <COMBINE CSipHeader::GetRackMethod@const>
    //------------------------------------------
    CToken& GetRackMethod();

    // Summary:
    //  Provides access to the RSeq header's response-num construct.
    //---------------------------------------------------------------
    const CToken& GetRSeq() const;

    // <COMBINE CSipHeader::GetRSeq@const>
    //------------------------------------
    CToken& GetRSeq();


    // RFC 3265 headers.
    //-------------------

    // Summary:
    //  Provides access to the Allow-Events header's event-type construct.
    //---------------------------------------------------------------------
    const CToken& GetAllowEvents() const;

    // <COMBINE CSipHeader::GetAllowEvents@const>
    //-------------------------------------------
    CToken& GetAllowEvents();

    // Summary:
    //  Provides access to the Event header's event-type construct.
    //--------------------------------------------------------------
    const CToken& GetEvent() const;

    // <COMBINE CSipHeader::GetEvent@const>
    //-------------------------------------
    CToken& GetEvent();

    // Summary:
    //  Provides access to the Subscription-State header's substate-value
    //  construct.
    //-------------------------------------------------------------------
    const CToken& GetSubscriptionState() const;

    // <COMBINE CSipHeader::GetSubscriptionState@const>
    //-------------------------------------------------
    CToken& GetSubscriptionState();


    // RFC 3313 header.
    //------------------

    // Summary:
    //  Provides access to the P-Media-Authorization header's
    //  P-Media-Authorization-Token construct.
    //--------------------------------------------------------
    const CToken& GetPMediaAuthorization() const;

    // <COMBINE CSipHeader::GetPMediaAuthorization@const>
    //---------------------------------------------------
    CToken& GetPMediaAuthorization();


    // RFC 3323 header.
    //------------------

    // Summary:
    //  Provides access to the Privacy header's priv-value construct.
    //----------------------------------------------------------------
    const CToken& GetPrivacy() const;

    // <COMBINE CSipHeader::GetPrivacy@const>
    //---------------------------------------
    CToken& GetPrivacy();


    // RFC 3325 headers.
    //-------------------

    // Summary:
    //  Provides access to the P-Asserted-Identity header's PAssertedID-value
    //  construct.
    //------------------------------------------------------------------------
    const CNameAddr& GetPAssertedIdentity() const;

    // <COMBINE CSipHeader::GetPAssertedIdentity@const>
    //-------------------------------------------------
    CNameAddr& GetPAssertedIdentity();

    // Summary:
    //  Provides access to the P-Preferred-Identity header's PPreferredID-value
    //  construct.
    //--------------------------------------------------------------------------
    const CNameAddr& GetPPreferredIdentity() const;

    // <COMBINE CSipHeader::GetPPreferredIdentity@const>
    //--------------------------------------------------
    CNameAddr& GetPPreferredIdentity();


    // RFC 3326 header.
    //------------------

    // Summary:
    //  Provides access to the Reason header's protocol construct.
    //-------------------------------------------------------------
    const CToken& GetReason() const;

    // <COMBINE CSipHeader::GetReason@const>
    //--------------------------------------
    CToken& GetReason();


    // RFC 3327 header.
    //------------------

    // Summary:
    //  Provides access to the Path header's name-addr construct.
    //------------------------------------------------------------
    const CNameAddr& GetPath() const;

    // <COMBINE CSipHeader::GetPath@const>
    //------------------------------------
    CNameAddr& GetPath();


    // RFC 3455 headers.
    //-------------------

    // Summary:
    //  Provides access to the P-Associated-URI header's name-addr construct.
    //------------------------------------------------------------------------
    const CNameAddr& GetPAssociatedUri() const;

    // <COMBINE CSipHeader::GetPAssociatedUri@const>
    //----------------------------------------------
    CNameAddr& GetPAssociatedUri();

    // Summary:
    //  Provides access to the P-Called-Party-ID header's name-addr construct.
    //-------------------------------------------------------------------------
    const CNameAddr& GetPCalledPartyId() const;

    // <COMBINE CSipHeader::GetPCalledPartyId@const>
    //----------------------------------------------
    CNameAddr& GetPCalledPartyId();

    // Summary:
    //  Provides access to the P-Visited-Network-ID header's
    //  (token / quoted-string) construct.
    //--------------------------------------------------------
    const CToken& GetPVisitedNetworkId() const;

    // <COMBINE CSipHeader::GetPVisitedNetworkId@const>
    //-------------------------------------------------
    CToken& GetPVisitedNetworkId();

    // Summary:
    //  Provides access to the P-Access-Network-Info header's access-type
    //  construct.
    //--------------------------------------------------------------------
    const CToken& GetPAccessNetworkInfo() const;

    // <COMBINE CSipHeader::GetPAccessNetworkInfo@const>
    //--------------------------------------------------
    CToken& GetPAccessNetworkInfo();

    // Summary:
    //  Provides access to the P-Charging-Function-Addresses header's mandatory
    //  first charge-addr-params construct. Other charge-addr-params are
    //  accessible through the parameter list.
    //--------------------------------------------------------------------------
    const CGenericParam& GetPChargingFunctionAddresses() const;

    // <COMBINE CSipHeader::GetPChargingFunctionAddresses@const>
    //----------------------------------------------------------
    CGenericParam& GetPChargingFunctionAddresses();

    // Summary:
    //  Provides access to the P-Charging-Vector header's icid-value construct.
    //--------------------------------------------------------------------------
    const CGenericParam& GetPChargingVector() const;

    // <COMBINE CSipHeader::GetPChargingVector@const>
    //-----------------------------------------------
    CGenericParam& GetPChargingVector();


    // RFC 3515 Header.
    //------------------

    // Summary:
    //  Provides access to the Refer-To header's a( name-addr / addr-spec )
    //  construct.
    //----------------------------------------------------------------------
    const CNameAddr& GetReferTo() const;

    // <COMBINE CSipHeader::GetReferTo@const>
    //---------------------------------------
    CNameAddr& GetReferTo();


    // RFC 3608 header.
    //------------------

    // Summary:
    //  Provides access to the Service-Route header's name-addr construct.
    //---------------------------------------------------------------------
    const CNameAddr& GetServiceRoute() const;

    // <COMBINE CSipHeader::GetServiceRoute@const>
    //--------------------------------------------
    CNameAddr& GetServiceRoute();


    // RFC 3891 header.
    //------------------

    // Summary:
    //  Provides access to the Replaces header's callid construct.
    //-------------------------------------------------------------
    const CToken& GetReplaces() const;

    // <COMBINE CSipHeader::GetReplaces@const>
    //----------------------------------------
    CToken& GetReplaces();


    // Draft-ietf-sip-session-timer-14.txt headers.
    //----------------------------------------------

    // Summary:
    //  Provides access to the Min-SE header's delta-seconds construct.
    //------------------------------------------------------------------
    const CToken& GetMinSe() const;

    // <COMBINE CSipHeader::GetMinSe@const>
    //-------------------------------------
    CToken& GetMinSe();

    // Summary:
    //  Provides access to the Session-Expires header's delta-seconds
    //  construct.
    //----------------------------------------------------------------
    const CToken& GetSessionExpires() const;

    // <COMBINE CSipHeader::GetSessionExpires@const>
    //----------------------------------------------
    CToken& GetSessionExpires();


    // Draft-ietf-sip-referredby-05.txt header.
    //------------------------------------------

    // Summary:
    //  Provides access to the Referred-By header's referrer-uri construct.
    //----------------------------------------------------------------------
    const CNameAddr& GetReferredBy() const;

    // <COMBINE CSipHeader::GetReferredBy@const>
    //------------------------------------------
    CNameAddr& GetReferredBy();


    // RFC 3841 headers.
    //------------------

    // Summary:
    //  Provides access to the Accept-Contact header's "*" construct.
    //----------------------------------------------------------------
    const CToken& GetAcceptContact() const;

    // <COMBINE CSipHeader::GetAcceptContact@const>
    //---------------------------------------------
    CToken& GetAcceptContact();

    // Summary:
    //  Provides access to the Reject-Contact header's "*" construct.
    //----------------------------------------------------------------
    const CToken& GetRejectContact() const;

    // <COMBINE CSipHeader::GetRejectContact@const>
    //---------------------------------------------
    CToken& GetRejectContact();

    // Summary:
    //  Provides access to the Request-Disposition header's directive
    //  construct.
    //----------------------------------------------------------------
    const CToken& GetRequestDisposition() const;

    // <COMBINE CSipHeader::GetRequestDisposition@const>
    //--------------------------------------------------
    CToken& GetRequestDisposition();


    // Draft-ietf-sip-join-03.txt header.
    //------------------------------------

    // Summary:
    //  Provides access to the Join header's callid construct.
    //---------------------------------------------------------
    const CToken& GetJoin() const;

    // <COMBINE CSipHeader::GetJoin@const>
    //------------------------------------
    CToken& GetJoin();


    // Draft-ietf-sip-publish-04.txt headers.
    //----------------------------------------

    // Summary:
    //  Provides access to the SIP-ETag header's entity-tag construct.
    //-----------------------------------------------------------------
    const CToken& GetSipETag() const;

    // <COMBINE CSipHeader::GetSipETag@const>
    //---------------------------------------
    CToken& GetSipETag();

    // Summary:
    //  Provides access to the SIP-If-Match header's entity-tag construct.
    //---------------------------------------------------------------------
    const CToken& GetSipIfMatch() const;

    // <COMBINE CSipHeader::GetSipIfMatch@const>
    //------------------------------------------
    CToken& GetSipIfMatch();


    // Draft-ietf-sip-history-info-02.txt header.
    //--------------------------------------------

    // Summary:
    //  Provides access to the History-Info header's hi-targeted-to-uri
    //  construct.
    //------------------------------------------------------------------
    const CNameAddr& GetHistoryInfo() const;

    // <COMBINE CSipHeader::GetHistoryInfo@const>
    //-------------------------------------------
    CNameAddr& GetHistoryInfo();


    // Draft-ietf-sip-identity-02.txt headers.
    //-----------------------------------------

    // Summary:
    //  Provides access to the Identity header's signed-identity-digest
    //  construct.
    //------------------------------------------------------------------
    const CToken& GetIdentity() const;

    // <COMBINE CSipHeader::GetIdentity@const>
    //----------------------------------------
    CToken& GetIdentity();

    // Summary:
    //  Provides access to the Identity-Info header's ident-info construct.
    //----------------------------------------------------------------------
    const CNameAddr& GetIdentityInfo() const;

    // <COMBINE CSipHeader::GetIdentityInfo@const>
    //--------------------------------------------
    CNameAddr& GetIdentityInfo();


    // Draft-ietf-sip-resource-priority-03.txt headers.
    //--------------------------------------------------

    // Summary:
    //  Provides access to the Accept-Resource-Priority header's
    //  [Resource-value] construct.
    //-----------------------------------------------------------
    const CToken& GetAcceptResourcePriority() const;

    // <COMBINE CSipHeader::GetAcceptResourcePriority@const>
    //------------------------------------------------------
    CToken& GetAcceptResourcePriority();

    // Summary:
    //  Provides access to the Resource-Priority header's Resource-value
    //  construct.
    //-------------------------------------------------------------------
    const CToken& GetResourcePriority() const;

    // <COMBINE CSipHeader::GetResourcePriority@const>
    //------------------------------------------------
    CToken& GetResourcePriority();

    // Draft-levy-sip-diversion-08 header.
    //-------------------------------------

    // Summary:
    //  Provides access to the Diversion header's name-addr.
    //-------------------------------------------------------
    const CNameAddr& GetDiversion() const;

    // <COMBINE CSipHeader::GetDiversion@const>
    //------------------------------------------
    CNameAddr& GetDiversion();

    // RFC 4538 header.
    //------------------

    // Summary:
    //  Provides access to the Target-Dialog header's callid construct.
    //------------------------------------------------------------------
    const CToken& GetTargetDialog() const;

    // <COMBINE CSipHeader::GetTargetDialog@const>
    //---------------------------------------------
    CToken& GetTargetDialog();

    // RFC 5373 headers.

    // Summary:
    //  Provides access to the Answer-Mode header's answer-mode-value construct.
    const CToken& GetAnswerMode() const;

    // <COMBINE CSipHeader::GetAnswerMode@const>
    CToken& GetAnswerMode();

    // Summary:
    //  Provides access to the Priv-Answer-Mode header's answer-mode-value
    //  construct.
    const CToken& GetPrivAnswerMode() const;

    // <COMBINE CSipHeader::GetPrivAnswerMode@const>
    CToken& GetPrivAnswerMode();

    // Summary:
    //  Gets a header parameter value from the parameter list.
    //---------------------------------------------------------
    CToken* GetParam(
                IN const char* szName,
                IN const EParameterCreationBehavior eParameterCreationBehavior =
                    ePARAM_DONT_CREATE,
                INOUT mxt_result* pRes = NULL);

    // Summary:
    //  Gets a header parameter value from the parameter list.
    //---------------------------------------------------------
    inline const CToken* GetParam(IN const char* szName) const;

    // Summary:
    //  Sets a header parameter value to the parameter list from a CString.
    //----------------------------------------------------------------------
    mxt_result SetParam(IN const char* szName, IN const CString& rstrValue);

    // Summary:
    //  Sets a header parameter value to the parameter list from a CToken.
    //---------------------------------------------------------------------
    mxt_result SetParam(IN const char* szName, IN const CToken& rValue);

//-- Hidden Methods
//-------------------
protected:
    //-- << Helpers >>
    //-------------------------

    // Summary:
    //  Returns the m_auVal.SParamForm.m_param member.
    //-----------------------------------------------
    const CGenericParam& GetInternalParam() const;

    // Summary:
    //  Returns the m_auVal.SParamForm.m_param member.
    //-----------------------------------------------
    CGenericParam& GetInternalParam();

    // Summary:
    //  Returns the m_auVal.STokenForm.m_host member.
    //----------------------------------------------
    const CHostPort& GetInternalHost() const;

    // Summary:
    //  Returns the m_auVal.STokenForm.m_host member.
    //----------------------------------------------
    CHostPort& GetInternalHost();

    // Summary:
    //  Returns the m_auVal.SNameAddrForm.m_nameAddr member.
    //-----------------------------------------------------
    const CNameAddr& GetInternalNameAddr() const;

    // Summary:
    //  Returns the m_auVal.SNameAddrForm.m_nameAddr member.
    //-----------------------------------------------------
    CNameAddr& GetInternalNameAddr();

    // Summary:
    //  Returns the m_auVal.SNameAddrForm.m_bIsWildCard member.
    //--------------------------------------------------------
    bool GetInternalWildCard() const;

    // Summary:
    //  Returns the m_auVal.SNameAddrForm.m_bIsWildCard member.
    //--------------------------------------------------------
    bool& GetInternalWildCard();

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokFirst member.
    //--------------------------------------------------
    const CToken& GetInternalFirstToken() const;

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokFirst member.
    //--------------------------------------------------
    CToken& GetInternalFirstToken();

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokSecond member.
    //---------------------------------------------------
    const CToken& GetInternalSecondToken() const;

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokSecond member.
    //---------------------------------------------------
    CToken& GetInternalSecondToken();

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokThird member.
    //--------------------------------------------------
    const CToken& GetInternalThirdToken() const;

    // Summary:
    //  Returns the m_auVal.STokenForm.m_tokThird member.
    //--------------------------------------------------
    CToken& GetInternalThirdToken();

    // Summary:
    //  Returns the m_auVal.SDateForm.m_date member.
    //---------------------------------------------
    const CDate& GetInternalDate() const;

    // Summary:
    //  Returns the m_auVal.SDateForm.m_date member.
    //---------------------------------------------
    CDate& GetInternalDate();

    // Summary:
    //  Returns the m_auVal.SMailboxUriForm.m_mailboxUri member.
    const CMailboxUri& GetInternalMailboxUri() const;

    // Summary:
    //  Returns the m_auVal.SMailboxUriForm.m_mailboxUri member.
    CMailboxUri& GetInternalMailboxUri();

    // Summary:
    //  Returns true if the one and only data item that this header contains is
    //  a token.
    //--------------------------------------------------------------------------
    bool HasOneStringTokenOnly(IN EDataType eConfig) const;

    // Summary:
    //  Returns true if the parse type contains one or more tokens.
    //--------------------------------------------------------------
    bool HasOneToken(IN EDataType eConfig) const;

    // Summary:
    //  Returns true if the parse type contains one or more slashes.
    //---------------------------------------------------------------
    bool HasSlash(IN EDataType eConfig) const;

    // Summary:
    //  Returns true if the parse type contains three tokens.
    //--------------------------------------------------------
    bool HasThreeTokens(IN EDataType eConfig) const;

    // Summary:
    //  Returns true if the parse type contains two or more tokens.
    //--------------------------------------------------------------
    bool HasTwoTokens(IN EDataType eConfig) const;

    // Summary:
    //  Allocates the m_auVal structure.
    //---------------------------------
    void InitHeaderVal(IN EDataType eDataType);
    
    // Summary:
    //  Deallocates the m_auVal structure.
    void FinalizeHeaderVal(IN EDataType eDataType);

    // Summary:
    //  Returns true if the pointer points to empty data.
    //----------------------------------------------------
    bool IsEmptyBody(IN const char* pcPos) const;

    // Summary:
    //  Returns true for header types for which the name-addr construct MUST
    //  have the angle quotes when parsing an URI.
    //-----------------------------------------------------------------------
    bool IsNameAddrForcedHeader() const;

    // Summary:
    //  Parses the header-specific contents of the header. This excludes the
    //  parameter list.
    //-----------------------------------------------------------------------
    mxt_result ParseBody(INOUT const char*& rpcPos);

    // Summary:
    //  Parses one complete header and stops after that.
    //---------------------------------------------------
    mxt_result ParseSingleHeader( INOUT const char*& rpcPos );

    // Summary:
    //  Properly resets all parsed information.
    //------------------------------------------
    void ResetHeaderVal();

    // Summary:
    //  Outputs a separator with regards to the given token configuration.
    //---------------------------------------------------------------------
    void SerializeTokenSeparator(IN EDataType eConfig,
                                 INOUT CBlob& rBlob ) const;

    // Summary:
    //  Sees if a list of parameters is present, and if so, parses it and sets
    //  it in the m_pParamList member.
    //-------------------------------------------------------------------------
    mxt_result TryToParseParams(IN const char*& rpcPos);

    // Summary:
    //  Compares two header chains.
    //------------------------------
    bool CompareHeaderChain( IN const CSipHeader& rC1,
                             IN const CSipHeader& rC2 ) const;

private:
    //-- << Startup mechanism >>
    //--------------------------------------------

    static mxt_result InitializeCSipHeader();


    //-- << Constructor >>
    //--------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSipHeader();

    // Summary:
    //  This is a helper for the GetNextHeader method. It returns a pointer to the
    //  chained header at the given 0-based index.
    //----------------------------------------------------------------------
    CSipHeader* InternalGetNextHeader(IN unsigned int uIndex = 0) const;

    // Summary:
    //  This is a helper for the GetNextHeader method. It provides access to the
    //  raw header.
    //---------------------------------------------------------------------------
    inline CRawHeader* InternalGetRawHeader() const;

    // Summary:
    //  This is a helper for the GetParamList method. It provides access to the
    //  parameter list.
    //-----------------------------------------------------------------------
    CGenParamList* InternalGetParamList(INOUT mxt_result* pres = NULL) const;


//-- Hidden Data Members
//------------------------
protected:
    // The type of header.
    //---------------------
    ESipHeaderType m_eHeader;

    // Possible parameter list associated to this header.
    //----------------------------------------------------
    CGenParamList* m_pParamList;

    // Next header for multi-headers.
    //--------------------------------
    CSipHeader* m_pNextHeader;

    // True if the short header name form is to be used when GetName() is
    // called.
    //--------------------------------------------------------------------
    static bool ms_bUseShortName;

    // Parsed contents of this header. We allocated the maximum space that any
    // header can take up.
    //-------------------------------------------------------------------------
    uint8_t m_auVal[ MX_MAX(
                        MX_MAX(
                            MX_MAX(
                                MX_MAX( sizeof(STokenForm),
                                    sizeof(SParamForm) ),
                                sizeof(SNameAddrForm) ),
                            sizeof(SDateForm) ),
                        sizeof(SMailboxUriForm) )];

    // The unparsed, received form of the header.
    //--------------------------------------------
    CRawHeader* m_pRawHeader;


private:
    // Summary:
    //  Typecasts the m_auVal member into the SNameAddrForm.
    //-----------------------------------------------------
    SNameAddrForm& GetNameAddrForm();

    // Summary:
    //  Typecasts the m_auVal member into the SNameAddrForm.
    //-----------------------------------------------------
    const SNameAddrForm& GetNameAddrForm() const;

    // Summary:
    //  Typecasts the m_auVal member into the STokenForm.
    //--------------------------------------------------
    STokenForm& GetTokenForm();

    // Summary:
    //  Typecasts the m_auVal member into the STokenForm.
    //--------------------------------------------------
    const STokenForm& GetTokenForm() const;

    // Summary:
    //  Typecasts the m_auVal member into the SParamForm.
    //--------------------------------------------------
    SParamForm& GetParamForm();

    // Summary:
    //  Typecasts the m_auVal member into the SParamForm.
    //--------------------------------------------------
    const SParamForm& GetParamForm() const;

    // Summary:
    //  Typecasts the m_auVal member into the SDateForm.
    //-------------------------------------------------
    SDateForm& GetDateForm();

    // Summary:
    //  Typecasts the m_auVal member into the SDateForm.
    //-------------------------------------------------
    const SDateForm& GetDateForm() const;

    // Summary:
    //  Typecasts the m_auVal member into the SMailboxUriForm.
    SMailboxUriForm& GetMailboxUriForm();

    // Summary:
    //  Typecasts the m_auVal member into the SMailboxUriForm.
    const SMailboxUriForm& GetMailboxUriForm() const;
};


//==============================================================================
//====  PUBLIC INLINE METHODS  =================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  SetNameForm
//==
//==============================================================================
//
//  Parameters:
//    bEnableShortForm:
//      Setting for the short name form. If true, short names are preferred when
//      GetName() is called.
//
//  Description:
//      Tells the M5T SIP stack parser to prefer short names when serializing
//      headers for output to the network.
//
//  See Also:
//      GetName, HeaderDefinition
//
//==EDOC========================================================================
inline void CSipHeader::SetNameForm(IN EHeaderNameForm eHeaderNameForm)
{
    ms_bUseShortName = (eHeaderNameForm == eSHORT_FORM_NAME);
}

//==SDOC========================================================================
//==
//==  GetHeaderType
//==
//==============================================================================
//
//  Returns:
//      The header's type.
//
//  Description:
//      Returns the header's type.
//
//==EDOC========================================================================
inline ESipHeaderType CSipHeader::GetHeaderType() const
{
    return m_eHeader;
}


//==SDOC========================================================================
//==
//==  GetNextHeader
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      0-based index of the header to find in the chain.
//
//  Returns:
//      Pointer to the header at the specified index, or NULL if out of range.
//
//  Description:
//      Finds the header at the specified index in the chain and returns it.
//      This method accesses the list of next headers, which does not include
//      the current instance of CSipHeader.
//
//  See Also:
//      AppendNextHeader
//
//==EDOC========================================================================
inline const CSipHeader* CSipHeader::GetNextHeader(IN unsigned int uIndex) const
{
    return InternalGetNextHeader( uIndex );
}


//==SDOC========================================================================
//==
//==  GetNextHeader
//==
//==============================================================================
//
//  Parameters:
//    uIndex:
//      0-based index of the header to find in the chain.
//
//  Returns:
//      Pointer to the header at the specified index, or NULL if out of range.
//
//  Description:
//      Finds the header at the specified index in the chain and returns it.
//      This method accesses the list of next headers, which does not include
//      the current instance of CSipHeader.
//
//  See Also:
//      AppendNextHeader
//
//==EDOC========================================================================
inline CSipHeader* CSipHeader::GetNextHeader(IN unsigned int uIndex)
{
    return InternalGetNextHeader( uIndex );
}

//==SDOC========================================================================
//==
//==  GetRawHeader
//==
//==============================================================================
//
//  Returns:
//      Pointer to the raw header, or NULL if unavailable.
//
//  Description:
//      Provides access to the raw header.
//
//  See Also:
//      SetRawHeader
//
//==EDOC========================================================================
inline const CRawHeader* CSipHeader::GetRawHeader() const
{
    return InternalGetRawHeader();
}

//==SDOC========================================================================
//==
//==  GetRawHeader
//==
//==============================================================================
//
//  Returns:
//      Pointer to the raw header, or NULL if unavailable.
//
//  Description:
//      Provides access to the raw header.
//
//  See Also:
//      SetRawHeader
//
//==EDOC========================================================================
inline CRawHeader* CSipHeader::GetRawHeader()
{
    return InternalGetRawHeader();
}

//==SDOC========================================================================
//==
//==  InternalGetRawHeader
//==
//==============================================================================
//
//  Returns:
//      Pointer to the raw header, or NULL if unavailable.
//
//  Description:
//      Provides access to the raw header.
//
//      Warning: This method should remain private. The return value is not
//      const whereas the method has a const modifier. This can be confusing
//      for clients because the returned value can be used to modify the
//      object, despite the method being const. Therefore it should only
//      be accessible internally.
//
//  See Also:
//      SetRawHeader
//
//==EDOC========================================================================
inline CRawHeader* CSipHeader::InternalGetRawHeader() const
{
    return m_pRawHeader;
}

//==SDOC========================================================================
//==
//==  GetAcceptMType
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the Accept header field.
//
//  Description:
//      Indicates the type of message body that the UA wants to receive in
//      response. For example, in the header:
//
//      <CODE>
//      Accept: application/sdp
//      </CODE>
//
//      this is the token containing "application".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptMType() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAcceptMType()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetAcceptMSubType
//==
//==============================================================================
//
//  Returns:
//      The second token (CToken) in the Accept header field..
//
//  Description:
//      Indicates the sub-type of message body that the UA wants to receive in
//      response. For example, in the header:
//
//      <CODE>
//      Accept: application/sdp
//      </CODE>
//
//      this is the token containing "sdp".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptMSubType() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetAcceptMSubType()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetAcceptEncoding
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Accept-Encoding header field.
//
//  Description:
//      Restricts the content-codings that are acceptable in the response. For
//      example, in the header:
//
//      <CODE>
//      Accept-Encoding: gzip
//      </CODE>
//
//      this is the token containing "gzip".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptEncoding() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAcceptEncoding()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetAcceptLanguage
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Accept-Language header field.
//
//  Description:
//      Indicates the preferred languages for reason phrases, session
//      descriptions, or status responses carried as message bodies in the
//      response. For example, in the header:
//
//      <CODE>
//      Accept-Language: da, en-gb;q=0.8, en;q=0.7
//      </CODE>
//
//      this is the token containing "da".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptLanguage() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAcceptLanguage()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetAlertInfo
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Alert-Info header field.
//
//  Description:
//      Specifies an alternative ring or ringback tone. For example, in the
//      header:
//
//      <CODE>
//      Alert-Info: <http://www.example.com/sounds/moo.wav>
//
//      this is the token containing
//      "<http://www.example.com/sounds/moo.wav>".
//      </CODE>
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetAlertInfo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetAlertInfo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetAllow
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Allow header field.
//
//  Description:
//      Lists the set of methods supported by the UA generating the message. For
//      example, in the header:
//
//      <CODE>
//      Allow: INVITE, ACK, OPTIONS, CANCEL, BYE
//      </CODE>
//
//      this is the token containing "INVITE".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAllow() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAllow()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetAllowEvents
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Allow-Events header field.
//
//  Description:
//      Indicates the event packages supported by the UA. A UA sending an
//      "Allow-Events" header is advertising that it can process SUBSCRIBE
//      requests and generate NOTIFY requests for all of the event packages
//      listed in that header. For example, in the header:
//
//      <CODE>
//      Allow-Events: spirits-INDPs
//      </CODE>
//
//      this is the token containing "spirits-INDPs".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAllowEvents() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAllowEvents()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetAuthorizationScheme
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Authorization header field.
//
//  Description:
//      Contains the authentication credentials of a UA. For example, in the
//      header:
//
//      <CODE>
//      Authorization: Digest username="Alice", realm="atlanta.com",
//       nonce="84a4cc6f3082121f32b42a2187831a9e",
//       response="7587245234b3434cc3412213e5f113a5432"
//      </CODE>
//
//      this is the token containing "Digest".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAuthorizationScheme() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAuthorizationScheme()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetCallId
//==
//==============================================================================
//
//  Returns:
//      The token(CToken) in the Call-ID header field.
//
//  Description:
//      Uniquely identifies a particular invitation or all registrations of a
//      particular client. For example, in the header:
//
//      <CODE>
//      Call-ID: f81d4fae-7dec-11d0-a765-00a0c91e6bf6@biloxi.com
//      </CODE>
//
//      this is the token containing
//      "f81d4fae-7dec-11d0-a765-00a0c91e6bf6@biloxi.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetCallId() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetCallId()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetCallId
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Call-Info header field.
//
//  Description:
//      Provides additional information about the caller or callee, depending on
//      whether it is found in a request or response. For example, in the
//      header:
//
//      <CODE>
//      Call-Info: <http://wwww.example.com/alice/photo.jpg> ;purpose=icon,
//       <http://www.example.com/alice/> ;purpose=info
//      </CODE>
//
//      this is the name address containing the URI
//      "http://wwww.example.com/alice/photo.jpg".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetCallInfo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetCallInfo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetContact
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Contact header field.
//
//  Description:
//      Provides a URI whose meaning depends on the type of request or response
//      in which it is. It has a role similar to the Location header field in
//      HTTP. For example, in the header:
//
//      <CODE>
//      Contact: "Mr. Watson" <sip:watson@worcester.bell-telephone.com>
//         ;q=0.7; expires=3600,
//         "Mr. Watson" <mailto:watson@bell-telephone.com> ;q=0.1
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:watson@worcester.bell-telephone.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetContact() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetContact()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  IsContactWildcard
//==
//==============================================================================
//
//  Returns:
//      True if the Contact header is a wildcard, false otherwise.
//
//  Description:
//      A Contact header can be a wildcard if it has the value "*". This is used
//      only in REGISTER requests with an Expires header set to "0" to remove
//      all registrations.
//
//==EDOC========================================================================
inline bool CSipHeader::IsContactWildcard() const
{
    return GetInternalWildCard();
}

//==SDOC========================================================================
//==
//==  SetContactWildcard
//==
//==============================================================================
//
//  Parameters:
//    bIsWildcard:
//      Set to true to indicate the wildcard.
//
//  Description:
//      Uses the parsed data structure as if it was of the SNameAddrForm form.
//      Sets the wildcard boolean. Note that the name-addr member of the
//      SNameAddrForm form and the wildcard are mutually exclusive.
//
//  See Also:
//      IsContactWildcard
//
//  Warning:
//      The user of the CSipHeader is required to verify the header type before
//      calling this helper. In the interest of keeping the code size small, no
//      verifications are made versus m_eHeader or the current real use of the
//      data structure union in these methods. It is thus VERY IMPORTANT that
//      CSipHeader users make sure that they are using the right type of
//      interface!
//
//==EDOC========================================================================
inline void CSipHeader::SetContactWildcard(IN bool bIsWildcard)
{
    MX_ASSERT(GetInternalNameAddr().GetUri() == NULL);
    GetInternalWildCard() = bIsWildcard;
}

//==SDOC========================================================================
//==
//==  GetContentDisposition
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Disposition header field.
//
//  Description:
//      Describes how the message body or, for multipart messages, a message
//      body part, is to be interpreted by the UAC or UAS. This SIP header field
//      extends the MIME Content-Type (RFC 2183 [18]). For example, in the
//      header:
//
//      <CODE>
//      Content-Disposition: session
//      </CODE>
//
//      this is the token containing "session".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentDisposition() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentDisposition()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetContentEncoding
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Encoding header field.
//
//  Description:
//      Used as a modifier to the "media-type".  When present, its value
//      indicates what additional content encodings have been applied to the
//      entity-body, and thus what decoding mechanisms MUST be applied in order
//      to obtain the media-type referenced by the Content-Type header field.
//      For example, in the header:
//
//      <CODE>
//      Content-Encoding: gzip
//      </CODE>
//
//      this is the token containing "gzip".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentEncoding() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentEncoding()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetContentLanguage
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Language header field.
//
//  Description:
//      Describes the natural language(s) of the intended audience for the
//      enclosed entity. For example, in the header:
//
//      <CODE>
//      Content-Language: fr
//      </CODE>
//
//      this is the token containing "fr".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentLanguage() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentLanguage()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetContentLength
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Length header field.
//
//  Description:
//      Indicates the size of the message-body, in decimal number of octets,
//      sent to the recipient. For example, in the header:
//
//      <CODE>
//      Content-Length: 349
//      </CODE>
//
//      this is the token containing "349".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentLength() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentLength()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetContentTypeMType
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the Content-Type header field.
//
//  Description:
//      Indicates the media type of the message-body sent to the recipient.
//      For example in the header:
//
//      <CODE>
//      Content-Type: application/sdp
//      </CODE>
//
//      this is the token containing "application".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentTypeMType() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentTypeMType()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetContentTypeMSubType
//==
//==============================================================================
//
//  Returns:
//      The second token (CToken) in the Content-Type header field.
//
//  Description:
//      Indicates the media sub-type of the message-body sent to the recipient.
//      For example, in the header:
//
//      <CODE>
//      Content-Type: application/sdp
//      </CODE>
//
//      this is the token containing "sdp".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentTypeMSubType() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetContentTypeMSubType()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetCSeqNumber
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the CSeq header field.
//
//  Description:
//      Contains a single decimal sequence number. For example, in the header:
//
//      <CODE>
//      CSeq: 4711 INVITE
//      </CODE>
//
//      this is the token containing "4711".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetCSeqNumber() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetCSeqNumber()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetCSeqMethod
//==
//==============================================================================
//
//  Returns:
//      The second token (CToken) in the CSeq header field.
//
//  Description:
//      Contains the request method. For example, in the header:
//
//      <CODE>
//      CSeq: 4711 INVITE
//      </CODE>
//
//      this is the token containing "INVITE".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetCSeqMethod() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetCSeqMethod()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetDate
//==
//==============================================================================
//
//  Returns:
//      The date (CDate) in the Date header field.
//
//  Description:
//      Contains the date and time. For example, in the header:
//
//      <CODE>
//      Date: Sat, 13 Nov 2010 23:29:00 GMT
//      </CODE>
//
//      this is the date containing "Sat, 13 Nov 2010 23:29:00 GMT".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CDate& CSipHeader::GetDate() const
{
    return GetInternalDate();
}

inline CDate& CSipHeader::GetDate()
{
    return GetInternalDate();
}

//==SDOC========================================================================
//==
//==  GetErrorInfo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Error-Info header field.
//
//  Description:
//      Provides a pointer to additional information about the error
//      status response. For example, in the header:
//
//      <CODE>
//      Error-Info: <sip:not-in-service-recording@atlanta.com>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:not-in-service-recording@atlanta.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetErrorInfo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetErrorInfo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetEvent
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Event header field.
//
//  Description:
//      Used to indicate which event or class of events the message contains
//      or subscribes. For example, in the header:
//
//      <CODE>
//      Event: spirits-INDPs
//      </CODE>
//
//      this is the token containing "spirits-INDPs".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetEvent() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetEvent()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetExpires
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Expires header field.
//
//  Description:
//      Gives the relative time after which the message (or content) expires.
//      The value of this field is an integral number of seconds (in decimal)
//      between 0 and (2**32)-1, measured from the receipt of the request.
//      For example, in the header:
//
//      <CODE>
//      Expires: 5
//      </CODE>
//
//      this is the token containing "5".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetExpires() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetExpires()
{
    return GetInternalFirstToken();
}

//==============================================================================
//==
//==  GetFlowTimer
//==
//==============================================================================
//
//  Returns:
//      The token(CToken) in the Flow-Timer header field.
//
//  Description:
//      Gives the number of seconds the server is prepared to wait without
//      seeing keep alives before it considers the corresponding flow dead.
//      For example in the header:
//
//      <CODE>
//      Flow-timer: 120
//      </CODE>
//
//      this will be the token containing "120".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==============================================================================
inline const CToken& CSipHeader::GetFlowTimer() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetFlowTimer()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetFrom
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the From header field.
//
//  Description:
//      Indicates the initiator of the request. This may be different from the
//      initiator of the dialog. Requests sent by the callee to the caller use
//      the callee's address in the From header field. For example, in the
//      header:
//
//      <CODE>
//      From: "A. G. Bell" <sip:agb@bell-telephone.com> ;tag=a48s
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:agb@bell-telephone.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetFrom() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetFrom()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetInReplyTo
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the In-Reply-To header field.
//
//  Description:
//      Enumerates the Call-IDs that this call references or returns.
//      For example, in the header:
//
//      <CODE>
//      In-Reply-To: 70710@saturn.bell-tel.com, 17320@saturn.bell-tel.com
//      </CODE>
//
//      this is the token containing "70710@saturn.bell-tel.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetInReplyTo() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetInReplyTo()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetMaxForwards
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Max-Forwards header field.
//
//  Description:
//      Limits the number of proxies or gateways that can forward the request
//      to the next downstream server. For example, in the header:
//
//      <CODE>
//      Max-Forwards: 6
//      </CODE>
//
//      this is the token containing "6".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetMaxForwards() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetMaxForwards()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetMimeVersion
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the MIME-Version header field.
//
//  Description:
//      Indicates the version of the MIME protocol used to construct the
//      message. For example, in the header:
//
//      <CODE>
//      MIME-Version: 1.0
//      </CODE>
//
//      this is the token containing "1.0".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetMimeVersion() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetMimeVersion()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetMinExpires
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Min-Expires header field.
//
//  Description:
//      Conveys the minimum refresh interval supported for soft-state elements
//      managed by that server. For example, in the header:
//
//      <CODE>
//      Min-Expires: 60
//      </CODE>
//
//      this is the token containing "60".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetMinExpires() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetMinExpires()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetMinSe
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Min-SE header field.
//
//  Description:
//      Indicates the minimum value for the session interval in seconds.
//      For example, in the header:
//
//      <CODE>
//      Min-SE: 210
//      </CODE>
//
//      this is the token containing "210".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetSessionExpires
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetMinSe() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetMinSe()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetOrganization
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Organization header field.
//
//  Description:
//      Conveys the name of the organization to which the SIP element issuing
//      the request or response belongs. For example, in the header:
//
//      <CODE>
//      Organization: Boxes by Bob
//      </CODE>
//
//      this is the token containing "Boxes by Bob".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetOrganization() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetOrganization()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetPath
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Path header field.
//
//  Description:
//      The Path header is a SIP extension header field with a syntax very
//      similar to the Record-Route header field.  It is used in conjunction
//      with SIP REGISTER requests and 200 class messages in response to
//      REGISTER (REGISTER responses). For example, in the header:
//
//      <CODE>
//      Path: <sip:P3.EXAMPLEHOME.COM;lr>,<sip:P1.EXAMPLEVISITED.COM;lr>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:P3.EXAMPLEHOME.COM".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetPath() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetPath()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetPriority
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Priority header field.
//
//  Description:
//      Indicates the urgency of the request as perceived by the client.
//      For example, in the header:
//
//      <CODE>
//      Priority: emergency
//      </CODE>
//
//      this is the token containing "emergency".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetPriority() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPriority()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetPrivacy
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Privacy header field.
//
//  Description:
//      Used by User-Agents to request privacy services from the network.
//      For example, in the header:
//
//      <CODE>
//      Privacy: none
//      </CODE>
//
//      this is the token containing "none".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetPrivacy() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPrivacy()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetProxyAuthenticateScheme
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Proxy-Authenticate header field.
//
//  Description:
//      Contains an authentication challenge. For example, in the header:
//
//      <CODE>
//      Proxy-Authenticate: Digest realm="atlanta.com",
//       domain="sip:ss1.carrier.com", qop="auth",
//       nonce="f84f1cec41e6cbe5aea9c8e88d359",
//       opaque="", stale=FALSE, algorithm=MD5
//      </CODE>
//
//      this is the token containing "Digest".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetProxyAuthorizationScheme
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetProxyAuthenticateScheme() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetProxyAuthenticateScheme()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetProxyAuthorizationScheme
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Proxy-Authorization header field.
//
//  Description:
//      Allows the client to identify itself (or its user) to a proxy that
//      requires authentication. A Proxy-Authorization field value consists of
//      credentials containing the authentication information of the user agent
//      for the proxy and/or realm of the resource being requested. For example,
//      in the header:
//
//      <CODE>
//      Proxy-Authorization: Digest username="Alice", realm="atlanta.com",
//       nonce="c60f3082ee1212b402a21831ae",
//       response="245f23415f11432b3434341c022"
//      </CODE>
//
//      this is the token containing "Digest".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetProxyAuthenticateScheme
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetProxyAuthorizationScheme() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetProxyAuthorizationScheme()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetProxyRequire
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Proxy-Require header field.
//
//  Description:
//      Indicates proxy-sensitive features that must be supported by the proxy.
//      For example, in the header:
//
//      <CODE>
//      Proxy-Require: foo
//      </CODE>
//
//      this is the token containing "foo".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetProxyRequire() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetProxyRequire()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetPAssertedIdentity
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the P-Asserted-Identity header field.
//
//  Description:
//      The P-Asserted-Identity header field is used among trusted SIP entities
//      (typically intermediaries) to carry the identity of the user sending a
//      SIP message as it was verified by authentication. For example, in the
//      header:
//
//      <CODE>
//      P-Asserted-Identity: "Cullen Jennings" <sip:fluffy@domain.com>
//      </CODE>
//
//      this is the name address containing the URI "sip:fluffy@domain.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetPPreferredIdentity
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetPAssertedIdentity() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetPAssertedIdentity()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetPPreferredIdentity
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the P-Preferred-Identity header field.
//
//  Description:
//      The P-Preferred-Identity header field is used from a user agent to a
//      trusted proxy to carry the identity that the user sending the SIP
//      message wants to be used as the P-Asserted-Header field value, which the
//      trusted element inserts. For example, in the header:
//
//      <CODE>
//      P-Preferred-Identity: "Cullen Jennings" <sip:fluffy@domain.com>
//      </CODE>
//
//      this is the name address containing the URI "sip:fluffy@domain.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetPAssertedIdentity
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetPPreferredIdentity() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetPPreferredIdentity()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetPMediaAuthorization
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the P-Media-Authorization header field.
//
//  Description:
//      Contains one or more media authorization tokens to be included in
//      subsequent resource reservations for the media flows associated with
//      the session, that is, passed to an independent resource reservation
//      mechanism, which is not specified here. For example, in the header:
//
//      <CODE>
//      P-Media-Authorization: 123456789abcdef
//      </CODE>
//
//      this is the token containing "123456789abcdef".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetPMediaAuthorization() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPMediaAuthorization()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRackResponseNum
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the RAck header field.
//
//  Description:
//      Indicates the value from the RSeq header in the provisional response
//      that is being acknowledged. For example, in the header:
//
//      <CODE>
//      RAck: 776655 1 INVITE
//      </CODE>
//
//      this is the token containing "776655".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRackResponseNum() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRackResponseNum()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRackCSeq
//==
//==============================================================================
//
//  Returns:
//      The second token (CToken) in the RAck header field.
//
//  Description:
//      Indicates the value of the CSeq in the response that is being
//      acknowledged. For example, in the header:
//
//      <CODE>
//      RAck: 776655 1 INVITE
//      </CODE>
//
//      this is the token containing "1".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRackCSeq() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetRackCSeq()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetRackMethod
//==
//==============================================================================
//
//  Returns:
//      The third token (CToken) in the RAck header field.
//
//  Description:
//      Indicates the value of the method in the response that is being
//      acknowledged. For example, in the header:
//
//      <CODE>
//      RAck: 776655 1 INVITE
//      </CODE>
//
//      this is the token containing "INVITE".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRackMethod() const
{
    return GetInternalThirdToken();
}

inline CToken& CSipHeader::GetRackMethod()
{
    return GetInternalThirdToken();
}


//==SDOC========================================================================
//==
//==  GetReason
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Reason header field.
//
//  Description:
//      Indicates why a SIP request has been issued or why a provisional
//      response has been sent. For example, in the header:
//
//      <CODE>
//      Reason: preemption ;cause=1 ;text="UA Preemption"
//      </CODE>
//
//      this is the token containing "preemption".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetReason() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetReason()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRecordRoute
//==
//==============================================================================
//
//  Returns:
//      The name address(CNameAddr) in the Record-Route header field.
//
//  Description:
//      The Record-Route header field is inserted by proxies in a request to
//      force future requests in the dialog to be routed through the proxy.
//      For example, in the header:
//
//      <CODE>
//      Record-Route: <sip:server10.biloxi.com;lr>,
//                    <sip:bigbox3.site3.atlanta.com;lr>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:server10.biloxi.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetRecordRoute() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetRecordRoute()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetReferredBy
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Referred-By header field.
//
//  Description:
//      Carries a SIP URI representing the identity of the referrer and,
//      optionally, the Content-ID of a body part that provides a more secure
//      statement of that identity. For example, in the header:
//
//      <CODE>
//      Referred-By: <sip:referrer@referrer.example>
//          ;cid="20398823.2UWQFN309shb3@referrer.example"
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:referrer@referrer.example".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetReferredBy() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetReferredBy()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetReferTo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Refer-To header field.
//
//  Description:
//      Provides a URI to reference. For example, in the header:
//
//      <CODE>
//      Refer-To: <sip:refertarget@target.example>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:refertarget@target.example".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetReferTo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetReferTo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetReplaces
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Replaces header field.
//
//  Description:
//      Indicates that a single dialog identified by the header field is to be
//      shut down and logically replaced by the incoming INVITE in which it is
//      contained. For example, in the header:
//
//      <CODE>
//      Replaces: 98732@sip.example.com
//                ;from-tag=r33th4x0r
//                ;to-tag=ff87ff
//      </CODE>
//
//      this is the token containing "98732@sip.example.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetReplaces() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetReplaces()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetReplyTo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Reply-To header field.
//
//  Description:
//      Contains a logical return URI that may be different from the From header
//      field. For example, in the header:
//
//      <CODE>
//      Reply-To: Bob <sip:bob@biloxi.com>
//      </CODE>
//
//      this is the name address containing the URI "sip:bob@biloxi.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetReplyTo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetReplyTo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetRequire
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Require header field.
//
//  Description:
//      Tells UASs about options that the UAC expects the UAS to support in
//      order to process the request. For example, in the header:
//
//      <CODE>
//      Require: 100rel
//      </CODE>
//
//      this is the token containing "100rel".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRequire() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRequire()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRetryAfter
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the Retry-After header field.
//
//  Description:
//      This header can be used with a 500 (Server Internal Error) or 503
//      (Service Unavailable) response to indicate how long the service is
//      expected to be unavailable to the requesting client. It can also be used
//      with a 404 (Not Found), 413 (Request Entity Too Large), 480 (Temporarily
//      Unavailable), 486 (Busy Here), 600 (Busy), or 603 (Decline) response to
//      indicate when the called party anticipates being available again. The
//      value of this field is a positive integer number of seconds (in decimal)
//      after the time of the response. An optional "duration" parameter
//      indicates how long the called party can be reached, starting at the
//      initial time of availability. For example, in the header:
//
//      <CODE>
//      Retry-After: 18000;duration=3600
//      </CODE>
//
//      this is the token containing "18000".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRetryAfter() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRetryAfter()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRetryAfterComment
//==
//==============================================================================
//
//  Returns:
//      The second token(CToken) in the Retry-After header field.
//
//  Description:
//      The comment in a Retry-After header. For example in the header:
//
//      <CODE>
//      Retry-After: 120 (I'm in a meeting)
//      </CODE>
//
//      this will be the token containing "I'm in a meeting".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRetryAfterComment() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetRetryAfterComment()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetRoute
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Route header field.
//
//  Description:
//      Forces routing for a request through the listed set of proxies. For
//      example, in the header:
//
//      <CODE>
//      Route: <sip:bigbox3.site3.atlanta.com;lr>,
//             <sip:server10.biloxi.com;lr>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:bigbox3.site3.atlanta.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetRoute() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetRoute()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetRSeq
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the RSeq header field.
//
//  Description:
//      Identifies provisional responses within a transaction. For example, in
//      the header:
//
//      <CODE>
//      CSeq: 4711
//      </CODE>
//
//      this is the token containing "4711".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRSeq() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRSeq()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetServer
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Server header field.
//
//  Description:
//      Contains information about the software used by the UAS to handle the
//      request. For example, in the header:
//
//      <CODE>
//      Server: HomeServer v2
//      </CODE>
//
//      this is the token containing "HomeServer v2".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetServer() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetServer()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetServiceRoute
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Service-Route header field.
//
//  Description:
//      Contains a route vector that directs requests through a specific
//      sequence of proxies. For example, in the header:
//
//      <CODE>
//      Service-Route: <sip:P2.HOME.EXAMPLE.COM;lr>,
//                <sip:HSP.HOME.EXAMPLE.COM;lr>
//      </CODE>
//
//      this is the name address containing the URI "sip:P2.HOME.EXAMPLE.COM".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetServiceRoute() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetServiceRoute()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetSessionExpires
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Session-Expires header field.
//
//  Description:
//      Conveys the session interval for a SIP session. For example, in the
//      header:
//
//      <CODE>
//      Session-Expires: 50
//      </CODE>
//
//      this is the token containing "50".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetMinSe
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSessionExpires() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSessionExpires()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetSubject
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Subject header field.
//
//  Description:
//      Provides a summary or indicates the nature of the call, allowing call
//      filtering without having to parse the session description. For example,
//      in the header:
//
//      <CODE>
//      Subject: Need more boxes
//      </CODE>
//
//      this is the token containing "Need more boxes".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSubject() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSubject()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetSubscriptionState
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Subscription-State header field.
//
//  Description:
//      Indicates in which state a subscription is. For example, in the header:
//
//      <CODE>
//      Subscription-State: active;expires=(depends on Refer-To URI)
//      </CODE>
//
//      this is the token containing "active".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSubscriptionState() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSubscriptionState()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetSupported
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Supported header field.
//
//  Description:
//      Enumerates all the extensions supported by the UAC or UAS. For example,
//      in the header:
//
//      <CODE>
//      Supported: 100rel
//      </CODE>
//
//      this is the token containing "100rel".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSupported() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSupported()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetTimestamp
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Timestamp header field.
//
//  Description:
//      Describes when the UAC sent the request to the UAS. For example, in the
//      header:
//
//      <CODE>
//      Timestamp: 54
//      </CODE>
//
//      this is the token containing "54".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetTimestamp() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetTimestamp()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetTo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the To header field.
//
//  Description:
//      Specifies the logical recipient of the request. For example, in the
//      header:
//
//      <CODE>
//      To: The Operator <sip:operator@cs.columbia.edu>;tag=287447
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:operator@cs.columbia.edu".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetTo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetTo()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetExtensionHeaderName
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) containing the name of a generic header field.
//
//  Description:
//      The name of an extension header. Extension headers are special since
//      their name is not parsed by this class, but by the user of the
//      CSipHeader.
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetExtensionHeaderName() const
{
    // The extension header is a special case of eDT_STRING. It is parsed as a
    // string, and the string is kept in the first token (this way it also
    // beneficiates from the HasOneStringTokenOnly() optimization). The
    // special thing about extension headers is that their name is not parsed
    // by this class, but by the user of the CSipHeader, and its name is kept
    // in the second token. Throughout the code, the name is treated as a
    // special case.
    //-------------------------------------------------------------------------
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetExtensionHeaderName()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetExtensionHeaderValue
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) containing the value of a generic header field.
//
//  Description:
//      The value of an extension header.
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetExtensionHeaderValue() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetExtensionHeaderValue()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetUnsupported
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Unsupported header field.
//
//  Description:
//      Lists the features not supported by the UAs. For example, in the header:
//
//      <CODE>
//      Unsupported: foo
//      </CODE>
//
//      this is the token containing "foo".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetUnsupported() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetUnsupported()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetUserAgent
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the User-Agent header field.
//
//  Description:
//      Contains information about the UAC originating the request. For example,
//      in the header:
//
//      <CODE>
//      User-Agent: Softphone Beta1.5
//      </CODE>
//
//      this is the token containing "Softphone Beta1.5".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetUserAgent() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetUserAgent()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetViaProtocolName
//==
//==============================================================================
//
//  Returns:
//      The first token (CToken) in the Via header field.
//
//  Description:
//      Contains the name of the protocol used to send the message. For example,
//      in the header:
//
//      <CODE>
//      Via: SIP/2.0/UDP erlang.bell-telephone.com:5060;branch=z9hG4bK87asdks7
//      </CODE>
//
//      this is the token containing "SIP".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetViaProtocolName() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetViaProtocolName()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetViaProtocolVersion
//==
//==============================================================================
//
//  Returns:
//      The second token (CToken) in the Via header field.
//
//  Description:
//      Contains the version of the protocol used to send the message.
//      For example, in the header:
//
//      <CODE>
//      Via: SIP/2.0/UDP erlang.bell-telephone.com:5060;branch=z9hG4bK87asdks7
//      </CODE>
//
//      this is the token containing "2.0".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetViaProtocolVersion() const
{
    return GetInternalSecondToken();
}

inline CToken& CSipHeader::GetViaProtocolVersion()
{
    return GetInternalSecondToken();
}

//==SDOC========================================================================
//==
//==  GetViaTransport
//==
//==============================================================================
//
//  Returns:
//      The third token (CToken) in the Via header field.
//
//  Description:
//      Contains the name of the transport protocol used to send the message.
//      For example, in the header:
//
//      <CODE>
//      Via: SIP/2.0/UDP erlang.bell-telephone.com:5060;branch=z9hG4bK87asdks7
//      </CODE>
//
//      this is the token containing "UDP".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetViaTransport() const
{
    return GetInternalThirdToken();
}

inline CToken& CSipHeader::GetViaTransport()
{
    return GetInternalThirdToken();
}

//==SDOC========================================================================
//==
//==  GetViaSentBy
//==
//==============================================================================
//
//  Returns:
//      The host name and port (CHostPort) in the Via header field.
//
//  Description:
//      Contains the client's host name or network address, and possibly the
//      port number at which it wants to receive responses. For example, in the
//      header:
//
//      <CODE>
//      Via: SIP/2.0/UDP erlang.bell-telephone.com:5060;branch=z9hG4bK87asdks7
//      </CODE>
//
//      this is the host name and port containing
//      "erlang.bell-telephone.com:5060".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CHostPort& CSipHeader::GetViaSentBy() const
{
    return GetInternalHost();
}

inline CHostPort& CSipHeader::GetViaSentBy()
{
    return GetInternalHost();
}

//==SDOC========================================================================
//==
//==  GetWarning
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Warning header field.
//
//  Description:
//      Carries additional information about the status of a response.
//      For example, in the header:
//
//      <CODE>
//      Warning: 307 isi.edu "Session parameter 'foo' not understood"
//      </CODE>
//
//      this is the token containing '307 isi.edu
//      "Session parameter 'foo' not understood" '.
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetWarning() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetWarning()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetWwwAuthenticateScheme
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the WWW-Authenticate header field.
//
//  Description:
//      Contains an authentication challenge. For example, in the header:
//
//      <CODE>
//      WWW-Authenticate: Digest realm="atlanta.com",
//        domain="sip:boxesbybob.com", qop="auth",
//        nonce="f84f1cec41e6cbe5aea9c8e88d359",
//        opaque="", stale=FALSE, algorithm=MD5
//      </CODE>
//
//      this is the token containing "Digest".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetWwwAuthenticateScheme() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetWwwAuthenticateScheme()
{
    return GetInternalFirstToken();
}

// RFC 3455 Headers.
//-------------------

//==SDOC========================================================================
//==
//==  GetPAssociatedUri
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the P-Associated-URI header field.
//
//  Description:
//      Allows a registrar to return a set of associated URIs for a registered
//      address-of-record. For example, in the header:
//
//      <CODE>
//      P-Associated-URI: <sip:alice-family@home1.net>,
//       <sip:alice-business@home1.net>,
//       <sip:+46-8-123-4567>@home1.net;user=phone>
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:alice-family@home1.net".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetPAssociatedUri() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetPAssociatedUri()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetPCalledPartyId
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the P-Called-Party-ID header field.
//
//  Description:
//      Identifies the address-of-record to which this session is destined. For
//      example, in the header:
//
//      <CODE>
//      P-Called-Party-ID: sip:user1-business@example.com
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:user1-business@example.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetPCalledPartyId() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetPCalledPartyId()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetPVisitedNetworkId
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the P-Visited-Network-ID header field.
//
//  Description:
//      Conveys to the registrar or home proxy in the home network the
//      identifier of a visited network.  The identifier is a text string or
//      token that is known by both the registrar or the home proxy at the home
//      network and the proxies in the visited network. For example, in the
//      header:
//
//      <CODE>
//      P-Visited-Network-ID: "Visited network number 1"
//      </CODE>
//
//      this is the token containing "Visited network number 1".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetPVisitedNetworkId() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPVisitedNetworkId()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetPAccessNetworkInfo
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the P-Access-Network-Info header field.
//
//  Description:
//      The P-Access-Network-Info header is useful in SIP-based networks that
//      also provide layer 2/layer 3 connectivity through different access
//      technologies. SIP User Agents may use this header to relay information
//      about the access technology to proxies that are providing services.
//      For example, in the header:
//
//      <CODE>
//      P-Access-Network-Info: 3GPP-UTRAN-TDD; utran-cell-id-3gpp=C359A3913B20E
//      </CODE>
//
//      this is the token containing "3GPP-UTRAN-TDD".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetPAccessNetworkInfo() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPAccessNetworkInfo()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetPChargingFunctionAddresses
//==
//==============================================================================
//
//  Returns:
//      The first parameter (CGenericParam) in the P-Charging-Function-Addresses
//      header field.
//
//  Description:
//      Contains one or more parameter that contains the hostnames or IP
//      addresses of the nodes willing to receive charging information.
//      For example, in the header:
//
//      <CODE>
//      P-Charging-Function-Addresses: ccf=[5555::b99:c88:d77:e66];
//      ccf=[5555::a55:b44:c33:d22]; ecf=[5555::1ff:2ee:3dd:4cc];
//      ecf=[5555::6aa:7bb:8cc:9dd]
//      </CODE>
//
//      this is the parameter ccf set to "[5555::b99:c88:d77:e66]"
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CGenericParam& CSipHeader::GetPChargingFunctionAddresses() const
{
    return GetInternalParam();
}

inline CGenericParam& CSipHeader::GetPChargingFunctionAddresses()
{
    return GetInternalParam();
}

//==SDOC========================================================================
//==
//==  GetPChargingVector
//==
//==============================================================================
//
//  Returns:
//      The first parameter (CGenericParam) in the P-Charging-Vector header
//      field.
//
//  Description:
//      Conveys charging related information, such as the globally unique IMS
//      charging identifier (ICID) value. For example, in the header:
//
//      <CODE>
//      P-Charging-Vector: icid-value="AyretyU0dm+6O2IrT5tAFrbHLso=023551024";
//      orig-ioi=home1.net; term-ioi=home2.net
//      </CODE>
//
//      this is the parameter icid-value set
//      to "AyretyU0dm+6O2IrT5tAFrbHLso=023551024".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CGenericParam& CSipHeader::GetPChargingVector() const
{
    return GetInternalParam();
}

inline CGenericParam& CSipHeader::GetPChargingVector()
{
    return GetInternalParam();
}

// RFC 3841 headers.
//------------------

//==SDOC========================================================================
//==
//==  GetAcceptContact
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Accept-Contact header field.
//
//  Description:
//      Contains feature sets which, if matched by a UA, imply that the request
//      should be routed to that UA. For example, in the header:
//
//      <CODE>
//      Accept-Contact: *;audio;require
//      </CODE>
//
//      this is the token containing "*".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptContact() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAcceptContact()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRejectContact
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Reject-Contact header field.
//
//  Description:
//      Contains feature sets which, if matched by a UA, imply that the request
//      should not be routed to that UA. For example, in the header:
//
//      <CODE>
//      Reject-Contact: *;actor="msg-taker";video
//      </CODE>
//
//      this is the token containing "*".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRejectContact() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRejectContact()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetRequestDisposition
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Request-Disposition header field.
//
//  Description:
//      Specifies caller preferences for how a server should process a request.
//      Its value is a list of tokens, each of which specifies a particular
//      processing directive. For example, in the header:
//
//      <CODE>
//      Request-Disposition: proxy, recurse, parallel
//      </CODE>
//
//      this is the token containing "proxy".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetRequestDisposition() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetRequestDisposition()
{
    return GetInternalFirstToken();
}

// Draft-ietf-sip-join-03.txt headers.
//-------------------------------------

//==SDOC========================================================================
//==
//==  GetJoin
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Join header field.
//
//  Description:
//      Indicates that a new dialog (created by the INVITE in which the Join
//      header field is contained) should be joined with a dialog identified by
//      the header field, and any associated dialogs or conferences. For
//      example, in the header:
//
//      <CODE>
//      Join: 98732@sip.example.com
//             ;from-tag=r33th4x0r
//             ;to-tag=ff87ff
//      </CODE>
//
//      this is the token containing "98732@sip.example.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetJoin() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetJoin()
{
    return GetInternalFirstToken();
}

// Draft-ietf-sip-publish-04.txt headers.
//----------------------------------------

//==SDOC========================================================================
//==
//==  GetSipETag
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the SIP-ETag header field.
//
//  Description:
//      The SIP-ETag header is present in the response sent by Event State
//      Compositor (ESC) and contains a single entity-tag identifying the
//      publication. For example, in the header:
//
//      <CODE>
//      SIP-ETag: dx200xyz
//      </CODE>
//
//      this is the token containing "dx200xyz".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetSipIfMatch
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSipETag() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSipETag()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetSipIfMatch
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the SIP-If-Match header field.
//
//  Description:
//      Identifies the specific event state that the request is refreshing,
//      modifying, or removing. This header field MUST contain a single
//      entity-tag that was returned by the ESC in the SIP-ETag header field of
//      the response to a previous publication. For example, in the header:
//
//      <CODE>
//      SIP-If-Match: dx200xyz
//      </CODE>
//
//      this is the token containing "dx200xyz".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetSipETag
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetSipIfMatch() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetSipIfMatch()
{
    return GetInternalFirstToken();
}

// Draft-ietf-sip-history-info-02.txt headers.
//---------------------------------------------

//==SDOC========================================================================
//==
//==  GetHistoryInfo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the History-Info header field.
//
//  Description:
//      Provides a standard mechanism for capturing the request history
//      information. For example, in the header:
//
//      <CODE>
//       History-Info: <sip:UserA@examplenetwork.com?Reason=SIP;
//         cause=302; text="Moved Temporarily">; index=1.1,
//         <sip:UserB@examplenetwork.com? Reason=SIP;cause=486;
//         text="Busy Here">;index=1.2,
//         <sip:45432@vm.examplenetwork.com> ; index=1.3
//      </CODE>
//
//      this is the name address containing the URI
//      "sip:UserA@examplenetwork.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetHistoryInfo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetHistoryInfo()
{
    return GetInternalNameAddr();
}

// Draft-ietf-sip-identity-02.txt headers.
//-----------------------------------------

//==SDOC========================================================================
//==
//==  GetIdentity
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Identity header field.
//
//  Description:
//      Contains a signature using the domain’s private key. For example, in the
//      header:
//
//      <CODE>
//      Identity: "dKJ97..."
//      </CODE>
//
//      this is the token containing "dKJ97...".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetIdentityInfo
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetIdentity() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetIdentity()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetIdentityInfo
//==
//==============================================================================
//
//  Returns:
//      The name address (CNameAddr) in the Identity-Info header field.
//
//  Description:
//      Contains the corresponding certificate to the Identity header. For
//      example, in the header:
//
//      <CODE>
//       Identity-Info: <https://example.com/cert>;alg=rsa-sha1
//      </CODE>
//
//      this is the name address containing the URI "https://example.com/cert".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//      GetIdentity
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetIdentityInfo() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetIdentityInfo()
{
    return GetInternalNameAddr();
}


// Draft-ietf-sip-resource-priority-03.txt headers.
//--------------------------------------------------

//==SDOC========================================================================
//==
//==  GetAcceptResourcePriority
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Accept-Resource-Priority header field.
//
//  Description:
//      Enumerates the resource values (r-values) a SIP user agent server is
//      willing to process. For example, in the header:
//
//      <CODE>
//      Accept-Resource-Priority: dsn.flash-override,
//        dsn.flash, dsn.immediate, dsn.priority, dsn.routine
//      </CODE>
//
//      this is the token containing "dsn.flash-override".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetAcceptResourcePriority() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAcceptResourcePriority()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetResourcePriority
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Resource-Priority header field.
//
//  Description:
//      Marks a SIP request as wanting prioritized access to resources. For
//      example, in the header:
//
//      <CODE>
//      Resource-Priority: dsn.flash, wps.3
//      </CODE>
//
//      this is the token containing "dsn.flash".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetResourcePriority() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetResourcePriority()
{
    return GetInternalFirstToken();
}

// RFC 2045.
//-----------

//==SDOC========================================================================
//==
//==  GetContentDescription
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Description header field.
//
//  Description:
//      Provides the ability to associate some descriptive information with a
//      given body. For example, in the header:
//
//      <CODE>
//      Content-Description: This is the content description
//      </CODE>
//
//      this is the token containing "This is the content description".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentDescription() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentDescription()
{
    return GetInternalFirstToken();
}

//==============================================================================
//==
//==  GetContentId
//==
//==============================================================================
//
//  Returns:
//      The mailbox URI (CMailboxUri) in the Content-ID header field.
//
//  Description:
//      Provides a URI whose meaning depends on the type of the body in which it
//      is. For example, in the header.:
//
//      <CODE>
//      Content-ID: <foo@bar.net>
//      </CODE>
//
//      This is the address containing the URI "foo@bar.net.
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==============================================================================
inline const CMailboxUri& CSipHeader::GetContentId() const
{
    return GetInternalMailboxUri();
}

inline CMailboxUri& CSipHeader::GetContentId()
{
    return GetInternalMailboxUri();
}

//==SDOC========================================================================
//==
//==  GetContentTransferEncoding
//==
//==============================================================================
//
//  Returns:
//      The token (CToken) in the Content-Transfer-Encoding header field.
//
//  Description:
//      Specifies the encoding transformation that was applied to the body
//      For example, in the header:
//
//      <CODE>
//      Content-Transfer-Encoding: gzip
//      </CODE>
//
//      this is the token containing "gzip".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetContentTransferEncoding() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetContentTransferEncoding()
{
    return GetInternalFirstToken();
}

//==SDOC========================================================================
//==
//==  GetDiversion
//==
//==============================================================================
// <FLAG New in 4.1.4>
//
//  Returns:
//      The name address(CNameAddr) in the Diversion header field.
//
//  Description:
//      Provides an URI indicating from whom the call was diverted.
//      For example in the header:
//
//      <CODE>
//      Diversion: <sip:Bob@uas1.isp.com>
//           ;reason=do-not-disturb
//      </CODE>
//
//      this will be the name address containing the URI
//      "sip:Bob@uas1.isp.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetDiversion() const
{
    return GetInternalNameAddr();
}

inline CNameAddr& CSipHeader::GetDiversion()
{
    return GetInternalNameAddr();
}

//==SDOC========================================================================
//==
//==  GetTargetDialog
//==
//==============================================================================
//
//  Returns:
//      The callid construct in the Target-Dialog header field.
//
//  Description:
//      Provides access to the Target-Dialog header's callid construct.
//
//      For example, with the following Target-Dialog header:
//
//      <CODE>
//      Target-Dialog: jazz@festival.com
//                     ;local-tag=kkaz
//                     ;remote-tag=6544
//      </CODE>
//
//      the returned token contains "jazz@festival.com".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetTargetDialog() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetTargetDialog()
{
    return GetInternalFirstToken();
}

//==============================================================================
//==
//==  GetAnswerMode
//==
//==============================================================================
//
//  Returns:
//      The answer-mode-value construct in the Answer-Mode header field.
//
//  Description:
//      Provides access to the Answer-Mode header's answer-mode-value construct.
//
//      Answer-Mode is defined in RFC 5373.
//
//      For example, with the following Answer-Mode header:
//
//      <CODE>
//      Answer-Mode: Manual;require
//      </CODE>
//
//      the returned token contains "Manual".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==============================================================================
inline const CToken& CSipHeader::GetAnswerMode() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetAnswerMode()
{
    return GetInternalFirstToken();
}

//==============================================================================
//==
//==  GetPrivAnswerMode
//==
//==============================================================================
//
//  Returns:
//      The answer-mode-value construct in the Priv-Answer-Mode header field.
//
//  Description:
//      Provides access to the Priv-Answer-Mode header's answer-mode-value
//      construct.
//
//      Priv-Answer-Mode is defined in RFC 5373.
//
//      For example, with the following Priv-Answer-Mode header:
//
//      <CODE>
//      Priv-Answer-Mode: Manual;require
//      </CODE>
//
//      the returned token contains "Manual".
//
//  See Also:
//      Header accessor nomenclature in CSipHeader.
//
//==============================================================================
inline const CToken& CSipHeader::GetPrivAnswerMode() const
{
    return GetInternalFirstToken();
}

inline CToken& CSipHeader::GetPrivAnswerMode()
{
    return GetInternalFirstToken();
}


//==============================================================================
//====  PRIVATE INLINE METHODS  ================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetInternalFirstToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the first token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the first token.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetInternalFirstToken() const
{
    return GetTokenForm().m_tokFirst;
}

//==SDOC========================================================================
//==
//==  GetInternalFirstToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the first token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the first token.
//
//==EDOC========================================================================
inline CToken& CSipHeader::GetInternalFirstToken()
{
    return GetTokenForm().m_tokFirst;
}

//==SDOC========================================================================
//==
//==  GetInternalSecondToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the second token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the second
//      token.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetInternalSecondToken() const
{
    return GetTokenForm().m_tokSecond;
}

//==SDOC========================================================================
//==
//==  GetInternalSecondToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the second token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the second
//      token.
//
//==EDOC========================================================================
inline CToken& CSipHeader::GetInternalSecondToken()
{
    return GetTokenForm().m_tokSecond;
}

//==SDOC========================================================================
//==
//==  GetInternalThirdToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the third token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the third token.
//
//==EDOC========================================================================
inline const CToken& CSipHeader::GetInternalThirdToken() const
{
    return GetTokenForm().m_tokThird;
}

//==SDOC========================================================================
//==
//==  GetInternalThirdToken
//==
//==============================================================================
//
//  Returns:
//      Reference to the third token.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the third token.
//
//==EDOC========================================================================
inline CToken& CSipHeader::GetInternalThirdToken()
{
    return GetTokenForm().m_tokThird;
}

//==SDOC========================================================================
//==
//==  GetInternalDate
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SDateForm. Retrieves the CDate member.
//
//==EDOC========================================================================
inline const CDate& CSipHeader::GetInternalDate() const
{
    return GetDateForm().m_date;
}

//==SDOC========================================================================
//==
//==  GetInternalDate
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SDateForm. Retrieves the CDate member.
//
//==EDOC========================================================================
inline CDate& CSipHeader::GetInternalDate()
{
    return GetDateForm().m_date;
}

//==============================================================================
//==
//==  GetInternalMailboxUri
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SMailboxUriForm. Retrieves the
//      CMailboxUri member.
//
//==============================================================================
inline const CMailboxUri& CSipHeader::GetInternalMailboxUri() const
{
    return GetMailboxUriForm().m_mailboxUri;
}

//==============================================================================
//==
//==  GetInternalMailboxUri
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SMailboxUriForm. Retrieves the
//      CMailboxUri member.
//
//==============================================================================
inline CMailboxUri& CSipHeader::GetInternalMailboxUri()
{
    return GetMailboxUriForm().m_mailboxUri;
}

//==SDOC========================================================================
//==
//==  GetParam
//==
//==============================================================================
//
//  Parameters:
//    szName:
//      Name of the parameter for which to look. A pre-defined list of
//      parameters exists in SipParser/HeaderParameter.h.
//
//  Returns:
//      A pointer to the value of the requested parameter. It is NULL if it did
//      not exist.
//
//  Description:
//      Returns a header paramater value or NULL if the parameter name is not
//      found in the list.
//
//  See Also:
//      GetParamList, SetParamList
//
//==EDOC========================================================================
inline
const CToken* CSipHeader::GetParam(IN const char* szName) const
{
    // The const_cast modifier is acceptable here, because of
    // ePARAM_DONT_CREATE.
    return const_cast<CSipHeader*>(this)->GetParam(szName);
}

//==SDOC========================================================================
//==
//==  SetParam
//==
//==============================================================================
//
//  Parameters:
//    szName:
//      Name of the parameter to set. A pre-defined list of parameters exists in
//      SipParser/HeaderParameter.h.
//
//    rValue:
//      Value of the parameter to set.
//
//  Returns:
//      resS_OK :
//          The parameter is properly set in the parameter list.
//
//      resFE_UNEXPECTED :
//          This header cannot have parameters.
//
//  Description:
//      Sets this header parameter list with the <C>szName</C> parameter. The
//      parameter is set with the value held by <C>rValue</C>.
//
//      This method takes care of the following:
//      - If this header has no parameter list yet, one is created.
//      - If a <C>szName</C> parameter already exists, its value is replaced
//        with the value held by <C>rValue</C>.
//      - If no <C>szName</C> parameter already exists, it is created and
//        appended to the parameter list.
//
//  See Also:
//      GetParamList, SetParamList
//
//==EDOC========================================================================
inline
mxt_result CSipHeader::SetParam(IN const char* szName, IN const CToken& rValue)
{
    return SetParam(szName, rValue.GetString());
}

//==SDOC========================================================================
//==
//==  GetInternalParam
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SParamForm. Retrieves the
//      CGenericParam member.
//
//==EDOC========================================================================
inline const CGenericParam& CSipHeader::GetInternalParam() const
{
    return GetParamForm().m_param;
}

//==SDOC========================================================================
//==
//==  GetInternalParam
//==
//==============================================================================
//
//  Returns:
//      Reference to the parameter.
//
//  Description:
//      Uses the parsed data structure as SParamForm. Retrieves the
//      CGenericParam member.
//
//==EDOC========================================================================
inline CGenericParam& CSipHeader::GetInternalParam()
{
    return GetParamForm().m_param;
}

//==SDOC========================================================================
//==
//==  GetInternalHost
//==
//==============================================================================
//
//  Returns:
//      Reference to the host.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the CHostPort
//      member.
//
//==EDOC========================================================================
inline const CHostPort& CSipHeader::GetInternalHost() const
{
    return GetTokenForm().m_host;
}

//==SDOC========================================================================
//==
//==  GetInternalHost
//==
//==============================================================================
//
//  Returns:
//      Reference to the host.
//
//  Description:
//      Uses the parsed data structure as STokenForm. Retrieves the CHostPort
//      member.
//
//==EDOC========================================================================
inline CHostPort& CSipHeader::GetInternalHost()
{
    return GetTokenForm().m_host;
}

//==SDOC========================================================================
//==
//==  GetInternalNameAddr
//==
//==============================================================================
//
//  Returns:
//      Reference to the name-addr.
//
//  Description:
//      Uses the parsed data structure as SNameAddrForm. Retrieves the CNameAddr
//      member.
//
//==EDOC========================================================================
inline const CNameAddr& CSipHeader::GetInternalNameAddr() const
{
    return GetNameAddrForm().m_nameAddr;
}

//==SDOC========================================================================
//==
//==  GetInternalNameAddr
//==
//==============================================================================
//
//  Returns:
//      Reference to the name-addr.
//
//  Description:
//      Uses the parsed data structure as a SNameAddrForm. Retrieves the
//      CNameAddr member.
//
//==EDOC========================================================================
inline CNameAddr& CSipHeader::GetInternalNameAddr()
{
    return GetNameAddrForm().m_nameAddr;
}

//==SDOC========================================================================
//==
//==  GetInternalWildCard
//==
//==============================================================================
//
//  Returns:
//      Reference to the name-addr.
//
//  Description:
//      Uses the parsed data structure as a SNameAddrForm. Retrieves the
//      m_bIsWildCard member.
//
//==EDOC========================================================================
inline bool CSipHeader::GetInternalWildCard() const
{
    return GetNameAddrForm().m_bIsWildCard;
}

//==SDOC========================================================================
//==
//==  GetInternalWildCard
//==
//==============================================================================
//
//  Returns:
//      Reference to the name-addr.
//
//  Description:
//      Uses the parsed data structure as a SNameAddrForm. Retrieves the
//      m_bIsWildCard member.
//
//==EDOC========================================================================
inline bool& CSipHeader::GetInternalWildCard()
{
    return GetNameAddrForm().m_bIsWildCard;
}

inline CSipHeader::SNameAddrForm& CSipHeader::GetNameAddrForm()
{
    return *reinterpret_cast<SNameAddrForm*>(m_auVal);
}

inline const CSipHeader::SNameAddrForm& CSipHeader::GetNameAddrForm() const
{
    return *reinterpret_cast<const SNameAddrForm*>(m_auVal);
}

inline CSipHeader::STokenForm& CSipHeader::GetTokenForm()
{
    return *reinterpret_cast<STokenForm*>(m_auVal);
}

inline const CSipHeader::STokenForm& CSipHeader::GetTokenForm() const
{
    return *reinterpret_cast<const STokenForm*>(m_auVal);
}

inline CSipHeader::SParamForm& CSipHeader::GetParamForm()
{
    return *reinterpret_cast<SParamForm*>(m_auVal);
}

inline const CSipHeader::SParamForm& CSipHeader::GetParamForm() const
{
    return *reinterpret_cast<const SParamForm*>(m_auVal);
}

inline CSipHeader::SDateForm& CSipHeader::GetDateForm()
{
    return *reinterpret_cast<SDateForm*>(m_auVal);
}

inline const CSipHeader::SDateForm& CSipHeader::GetDateForm() const
{
    return *reinterpret_cast<const SDateForm*>(m_auVal);
}

inline CSipHeader::SMailboxUriForm& CSipHeader::GetMailboxUriForm()
{
    return *reinterpret_cast<SMailboxUriForm*>(m_auVal);
}

inline const CSipHeader::SMailboxUriForm& CSipHeader::GetMailboxUriForm() const
{
    return *reinterpret_cast<const SMailboxUriForm*>(m_auVal);
}

inline bool CSipHeader::HasOneStringTokenOnly(IN EDataType eConfig) const
{
    return eConfig == eDT_TOKEN ||
           eConfig == eDT_TOKEN_AUTH ||
           eConfig == eDT_STRING ||
           eConfig == eDT_FREE_STRING;
}

inline bool CSipHeader::HasOneToken(IN EDataType eConfig) const
{
   return eConfig == eDT_TOKEN ||
          eConfig == eDT_TOKEN_AUTH ||
          HasTwoTokens(eConfig);
}

inline bool CSipHeader::HasSlash(IN EDataType eConfig) const
{
   return eConfig == eDT_TOKEN_SLASH_TOKEN || eConfig == eDT_VIA;
}

inline bool CSipHeader::HasThreeTokens(IN EDataType eConfig) const
{
   return eConfig == eDT_THREE_TOKENS || eConfig == eDT_VIA;
}

inline bool CSipHeader::HasTwoTokens(IN EDataType eConfig) const
{
   return eConfig == eDT_TOKEN_SLASH_TOKEN ||
          eConfig == eDT_TWO_TOKENS ||
          HasThreeTokens(eConfig);
}

inline bool CSipHeader::IsNameAddrForcedHeader() const
{
    return m_eHeader == eHDR_ALERT_INFO ||
           m_eHeader == eHDR_CALL_INFO ||
           m_eHeader == eHDR_ERROR_INFO ||
           m_eHeader == eHDR_RECORD_ROUTE ||
           m_eHeader == eHDR_ROUTE ||
           m_eHeader == eHDR_PATH ||
           m_eHeader == eHDR_SERVICE_ROUTE ||
           m_eHeader == eHDR_P_ASSOCIATED_URI ||
           m_eHeader == eHDR_P_CALLED_PARTY_ID ||
           m_eHeader == eHDR_HISTORY_INFO ||
           m_eHeader == eHDR_IDENTITY_INFO;
}

//==SDOC========================================================================
//==
//==  UnlinkTopHeader
//==
//==============================================================================
//
//  Parameters:
//    None.
//
//  Returns:
//      A CSipHeader*, the value previously stored in m_pNextHeader.
//
//      The ownership of the object is GIVEN to the caller.
//
//  Description:
//      Sets the m_pNextHeader pointer to NULL. The m_pNextHeader is not deleted
//      so the caller must be carefull not to lose the return value.
//
//  See Also:
//      RemoveNextHeader, GetNextHeader
//
//==EDOC========================================================================
inline
GO CSipHeader* CSipHeader::UnlinkTopHeader()
{
    CSipHeader* pNextHeader = m_pNextHeader;
    m_pNextHeader = NULL;
    return pNextHeader;
}

//==SDOC========================================================================
//==
//==  GetParamList
//==
//==============================================================================
//
//  Parameters:
//    rres:
//      Reference to a mxt_result where the result is set.
//
//      resS_OK
//          Pointer set in the OUT parameter.
//
//      resFE_UNEXPECTED
//          This header cannot have parameters as per the header definition
//          table.
//
//      resSW_SIPPARSER_NULL_PTR
//          Warning, the value is NULL.
//
//  Returns:
//      Pointer to the internal parameter list. It can be NULL
//
//  Description:
//      Provides access to the optional parameter list.
//
//  See Also:
//      SetParamList
//
//==EDOC========================================================================
inline const CGenParamList* CSipHeader::GetParamList(OUT mxt_result& rres) const
{
    return InternalGetParamList(&rres);
}

inline CGenParamList* CSipHeader::GetParamList(OUT mxt_result& rres)
{
    return InternalGetParamList(&rres);
}

inline const CGenParamList* CSipHeader::GetParamList(INOUT mxt_result* pres) const
{
    return InternalGetParamList(pres);
}

inline CGenParamList* CSipHeader::GetParamList(INOUT mxt_result* pres)
{
    return InternalGetParamList(pres);
}

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CSIPHEADER_H
