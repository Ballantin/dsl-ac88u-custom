//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CNAMEADDR_H
#define MXG_CNAMEADDR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CTOKEN_H
#include "SipParser/CToken.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CHOSTPORT_H
#include "SipParser/CHostPort.h" // ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT
#endif

#ifndef MXG_CMAILBOXURI_H
#include "SipParser/CMailboxUri.h"
#endif

#ifndef MXG_CSIPURI_H
#include "SipParser/CSipUri.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CBlob;
class CString;
class IUri;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CNameAddr
//======================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//   The CNameAddr class handles the storage, parsing, and serialization of the
//   (name-addr | addr-spec) construct. It is a URI with optional display name
//   and angle quotes. The angle quotes may be mandatory through the use of the
//   bForceNameAddr parameter at parse time. The angle quotes are always output
//   at serialization time.
//
//   The CNameAddr class is a simple container comprised of a token and an URI.
//   Refer to the aggregate class's documentation for further details.
//
//   The display name is handled in a very simplified way. Since the stack does
//   not care what the display-name is, it outputs it directly. The user has
//   the responsibility of following RFC 3261 rules regarding the display-name,
//   such as having the enclosing quotes when the display-name is a quoted
//   string.
//
//  <CODE>
//      RFC 3261 ABNF (when forcing the name-addr criterion,
//                    set eAngleBracket to eMANDATORY_ANGLE_BRACKET) :
//          name-addr    = [ display-name ] "<" addr-spec ">"
//          display-name = *token | quoted-string
//          addr-spec    =  SIP-URI / SIPS-URI / absoluteURI
//
//      RFC 3261 extended ABNF (when relaxing the name-addr criterion,
//                             set eAngleBracket to eOPTIONAL_ANGLE_BRACKET) :
//          name-addr    = ([ display-name ] "<" addr-spec ">") / addr-spec
//          display-name = *token | quoted-string
//          addr-spec    =  SIP-URI / SIPS-URI / absoluteURI
//
//      Note that the extended ABNF is more flexible than the "official" 
//      RFC 3261 ABNF.
//  </CODE>
//
// Location:
//   SipParser/CNameAddr.h
//
// See Also:
//   CToken, IUri, CSipUri, CAbsoluteUri, CImUri, CPresUri
//
//==EDOC========================================================================
class CNameAddr
{
//-- New types definition
//-----------------------
public:
    // Description:
    //  Indicates whetever or not angle bracket are mandatory.
    enum EAngleBracket
    {
        // Description:
        //  Angle brackets must be present when parsing the string.
        eMANDATORY_ANGLE_BRACKET,
        // Description:
        //  Angle brackets must optionally be present when parsing the string.
        eOPTIONAL_ANGLE_BRACKET
    };

// Published Interface
//-------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CNameAddr();

    // Summary:
    //  Copy constructor.
    //--------------------
    CNameAddr(IN const CNameAddr& rSrc);

    // Summary:
    //  Extended constructor. Assumes an empty display name.
    //-------------------------------------------------------
    CNameAddr(IN const IUri& rSrc);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CNameAddr();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CNameAddr& operator=(IN const CNameAddr& rSrc);

    // Summary:
    //  Assignment operator. Assumes an empty display name.
    //------------------------------------------------------
    CNameAddr& operator=(IN const IUri& rSrc);

    // Summary:
    //  Comparison operator. Compares only the URI.
    //----------------------------------------------
    bool operator==(IN const CNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    //----------------------------------------------
    bool operator==(IN const IUri* pSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    //----------------------------------------------
    bool operator!=(IN const CNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    //----------------------------------------------
    bool operator!=(IN const IUri* pSrc) const;


    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the display name.
    //---------------------------------------
    const CToken& GetDisplayName() const;

    // <COMBINE CNameAddr::GetDisplayName@const>
    //------------------------------------------
    CToken& GetDisplayName();

    // Summary:
    //  Provides access to the addr-spec and casts it to CSipUri if
    //  applicable.
    //--------------------------------------------------------------
    inline const CSipUri* GetSipUri() const;

    // Summary:
    //  Provides access to the addr-spec and casts it to CSipUri if
    //  applicable.
    //-------------------------------------
    inline CSipUri* GetSipUri();

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
    // Summary:
    //  Provides access to the addr-spec and casts it to CMailboxUri if
    //  applicable. Depending on the type, It may then be recasted as IM or PRES
    //  URIs.
    const CMailboxUri* GetMailboxUri() const;

    // Summary:
    //  Provides access to the addr-spec and casts it to CMailBoxUri
    //  if applicable. It can then be recast as IM or PRES URIs.
    //---------------------------------------------------------------
    CMailboxUri* GetMailboxUri();
#endif //-- #if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

    // Summary:
    //  Provides access to the addr-spec.
    //------------------------------------
    const IUri* GetUri() const;

    // <COMBINE CNameAddr::GetUri@const>
    //----------------------------------
    IUri* GetUri();

    // Summary:
    //  Parses the optional display name and following URI.
    //------------------------------------------------------
    mxt_result Parse(INOUT const char*& rpcPos,
                     IN EAngleBracket eAngleBracket = eOPTIONAL_ANGLE_BRACKET);

    // Summary:
    //  Resets data members to initial state.
    //----------------------------------------
    void Reset();

    // Summary:
    //  Outputs the optional display name and following URI.
    //-------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
    // Summary:
    //  Sets the URI to a pres or im URI.
    void SetMailboxUri(IN IUri::EUriType eType,
                       IN const CString& rstrUser,
                       IN const CString& rstrHost,
                       IN const CString& rstrDisplayName = CString());
#endif //-- #if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

    // Summary:
    //  Sets the URI to a SIPURI.
    //----------------------------
    void SetSipUri( IN const CString& rstrHost,
                    IN uint16_t uPort = CHostPort::ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT,
                    IN const CString& rstrUser = CString(),
                    IN CSipUri::ESecurityFlag eSecured = CSipUri::eUNSECURE,
                    IN const CString& rstrDisplayName = CString() );

    // Summary:
    //  Sets the URI value for the addr-spec part.
    //---------------------------------------------
    void SetUri(IN TO IUri* pUri, IN const CString& rstrDisplayName = CString());

// BRCM_CUSTOM - [begin] Embrace all display names by double quotes
    // Summary:
    //  If parameter is true, DisplayName will always be serialized with quotes.
    //---------------------------------------------------------------------------
    static void SetForceQuotedDisplayName(bool bForceQuotedDisplayName);

    // Summary:
    // Returns true if the DisplayName is always serialized with quotes.
    //-------------------------------------------------------------------
    static bool GetForceQuotedDisplayName();
// BRCM_CUSTOM - [end] Embrace all display names by double quotes

// Hidden Methods
//-------------------------
protected:
    // Summary:
    //  Attempts to parse the optional display name.
    //-----------------------------------------------
    mxt_result ParseDisplayName(INOUT const char*& rpcPos);


private:
    // Summary:
    //  This is a helper for the GetSipUri method. It provides access to the 
    //  addr-spec and casts it to CSipUri if applicable.
    //-------------------------------------------------------------------
    CSipUri* InternalGetSipUri() const;

    // Summary:
    //  This is a helper to copy the internal URI display, if any,
    //  into the NameAddr's display name.
    //--------------------------------------------------------------
    void CopyInternalUriDisplayName();

// Hidden Data Members
//-------------------------
protected:
    // Optional display-name part.
    //-----------------------------
    CToken m_tokDisplayName;

    // addr-spec part.
    //-----------------
    IUri* m_pAddrSpec;

// BRCM_CUSTOM - [begin] Embrace all display names by double quotes
    // If true, display name is always serialized with quotes.
    //---------------------------------------------------------
    static bool ms_bForceQuotedDisplayName;
// BRCM_CUSTOM - [end] Embrace all display names by double quotes

private:
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Returns:
//      Constant reference on the display name. If the display name is a quoted
//      string, the quotes are included in the stored data.
//
//  Description:
//      Provides access to the display name.
//
//==EDOC========================================================================
inline const CToken& CNameAddr::GetDisplayName() const
{
    return m_tokDisplayName;
}

//==SDOC========================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Returns:
//      Modifiable reference on the display name.
//
//  Description:
//      Provides access to the display name. This method is designed to set the
//      display name. It is up to the user to follow RFC 3261 rules such as
//      adding the enclosing quotes if the display-name is a quoted-string.
//
//==EDOC========================================================================
inline CToken& CNameAddr::GetDisplayName()
{
    return m_tokDisplayName;
}

//==SDOC========================================================================
//==
//==  GetUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the addr-spec. It may be NULL.
//
//  Description:
//      Provides access to the addr-spec URI.
//
//  See Also:
//      SetUri, GetSipUri, SetSipUri
//
//==EDOC========================================================================
inline const IUri* CNameAddr::GetUri() const
{
    return m_pAddrSpec;
}

//==SDOC========================================================================
//==
//==  GetUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the addr-spec. It may be NULL.
//
//  Description:
//      Provides access to the addr-spec URI.
//
//  See Also:
//      SetUri, GetSipUri, SetSipUri
//
//==EDOC========================================================================
inline IUri* CNameAddr::GetUri()
{
    return m_pAddrSpec;
}


//==SDOC========================================================================
//==
//==  GetSipUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the addr-spec, if it is a SipUri. It can be NULL if the URI
//      is not SIP or if the URI is simply absent.
//
//  Description:
//      Provides access to the URI in SIP form.
//
//  See Also:
//      SetUri, GetUri, SetSipUri
//
//==EDOC========================================================================
inline const CSipUri* CNameAddr::GetSipUri() const
{
    return InternalGetSipUri();
}

//==SDOC========================================================================
//==
//==  GetSipUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the addr-spec, if it is a SipUri. It can be NULL if the URI
//      is not SIP or if the URI is simply absent.
//
//  Description:
//      Provides access to the URI in SIP form.
//
//  See Also:
//      SetUri, GetUri, SetSipUri
//
//==EDOC========================================================================
inline CSipUri* CNameAddr::GetSipUri()
{
    return InternalGetSipUri();
}

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
//==============================================================================
//==
//==  GetMailboxUri
//==
//==============================================================================
//
//  Returns:
//      Pointer to the addr-spec, if it is a mailbox URI.
//
//      NULL if the URI is not a mailbox URI or if there is no URI.
//
//  Description:
//      Provides access to the URI in mailbox form.
//
//  See Also:
//      SetUri, GetUri, SetMailboxUri
//
//==============================================================================
inline const CMailboxUri* CNameAddr::GetMailboxUri() const
{
    return const_cast<CNameAddr*>(this)->GetMailboxUri();
}
#endif //-- #if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)

// BRCM_CUSTOM - [begin] Embrace all display names by double quotes
//==SDOC========================================================================
//==
//==  SetForceQuotedDisplayName
//==
//==============================================================================
//
//  Parameters:
//    bForceQuotedDisplayName:
//      True - force display name to be serialized with quotes.
//
//  Returns:
//      N/A
//
//  Description:
//      If parameter is true, DisplayName will always be serialized with quotes.
//
//  See Also:
//      GetForceQuotedDisplayName
//
//==EDOC========================================================================
inline void CNameAddr::SetForceQuotedDisplayName(bool bForceQuotedDisplayName)
{
    ms_bForceQuotedDisplayName = bForceQuotedDisplayName;
}

//==SDOC========================================================================
//==
//==  GetForceQuotedDisplayName
//==
//==============================================================================
//
//  Parameters:
//      N/A
//
//  Returns:
//      Option whether display name is serialized with quotes.
//
//  Description:
//      Returns true if the DisplayName is always serialized with quotes.
//
//  See Also:
//      SetForceQuotedDisplayName
//
//==EDOC========================================================================
inline bool CNameAddr::GetForceQuotedDisplayName()
{
    return ms_bForceQuotedDisplayName;
}

// BRCM_CUSTOM - [end] Embrace all display names by double quotes

MX_NAMESPACE_END(MXD_GNS)


#endif // #ifndef MXG_CNAMEADDR_H

