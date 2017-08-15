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
#ifndef MXG_CTXTNAMEADDR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTNAMEADDR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_CNAMEADDR_H
#include "SipParser/CNameAddr.h"
#endif

#ifndef MXG_CTXTABSOLUTEURI_H
#include "TextMessage/CTxtAbsoluteUri.h"
#endif

#ifndef MXG_CTXTTOKEN_H
#include "TextMessage/CTxtToken.h"
#endif

#ifndef MXG_TXTMESSAGECONSTANTS_H
#include "TextMessage/TxtMessageConstants.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAllocator;
class IUri;
class ITxtUri;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtNameAddr
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 5>
//
// Summary:
//   The CTxtNameAddr class handles the storage, parsing, and serialization of
//   the (name-addr | addr-spec) construct.
//
// Description:
//   The CTxtNameAddr class handles the storage, parsing, and serialization of
//   the (name-addr | addr-spec) construct. It is a URI with optional display
//   name and angle quotes. The angle quotes may be mandatory through the use of
//   the bForceNameAddr parameter at parse time. The angle quotes are always
//   output at serialization time.
//
//   The CTxtNameAddr class is a simple container comprised of a token and an
//   URI. Refer to the aggregate class's documentation for further details.
//
//   The display name is handled in a very simplified way. Since the Text
//   Message Parser does not care what the display-name is, it outputs it
//   directly. The user has the responsibility of following his protocol RFC
//   rules regarding the display name, such as having the enclosing quotes when
//   the display name is a quoted string.
//
//   Currently, this object internally uses the CNameAddr object of the SIP UA
//   thus parsing and serialization are managed using the following ABNFs.
//
//  <CODE>
//  RFC 3261 ABNF:
//      name-addr    = [ display-name ] "<" addr-spec ">"
//      display-name = *token | quoted-string
//      addr-spec    =  SIP-URI / SIPS-URI / absoluteURI
//  </CODE>
//
// Location:
//   TextMessage/CTxtNameAddr.h
//
//==============================================================================
class CTxtNameAddr
{

//-- Friend Declarations

//-- Published Interface
public:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CTxtNameAddr(IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CTxtNameAddr(IN const CTxtNameAddr& rSrc);

    // Summary:
    //  Conversion Constructor.
    CTxtNameAddr(IN const CNameAddr& rSrc);

    // Summary:
    //  Extended constructor. Assumes an empty display name.
    CTxtNameAddr(IN const ITxtUri& rSrc);

    // Summary:
    //  Extended constructor. Assumes an empty display name.
    CTxtNameAddr(IN const IUri& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtNameAddr();

    // Summary:
    //  Conversion operator.
    operator const CNameAddr&() const;

    // Summary:
    //  Assignment Operator.
    CTxtNameAddr& operator=(IN const CTxtNameAddr& rSrc);

    // Summary:
    //  Assignment Operator.
    CTxtNameAddr& operator=(IN const CNameAddr& rSrc);

    // Summary:
    //  Assignment operator. Assumes an empty display name.
    CTxtNameAddr& operator=(IN const ITxtUri& rSrc);

    // Summary:
    //  Assignment operator. Assumes an empty display name.
    CTxtNameAddr& operator=(IN const IUri& rSrc);

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator==(IN const CTxtNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator==(IN const CNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator==(IN const ITxtUri* pSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator==(IN const IUri* pSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator!=(IN const CTxtNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator!=(IN const CNameAddr& rSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator!=(IN const ITxtUri* pSrc) const;

    // Summary:
    //  Comparison operator. Compares only the URI.
    bool operator!=(IN const IUri* pSrc) const;

    //-- << Accessors >>

    // Summary:
    //  Provides access to the display name.
    const CToken& GetDisplayNameSipToken() const;

    // Summary:
    //  Provides access to the display name.
    const CTxtToken& GetDisplayName() const;

    //<COMBINE CTxtNameAddr::GetDisplayNameSipToken@const>
    CTxtToken& GetDisplayName();

    // Summary:
    //  Provides access to the addr-spec and casts it to CSipUri if
    //  applicable.
    const CSipUri* GetSipUri() const;

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
    // Summary:
    //  Provides access to the addr-spec and casts it to CMailBoxUri
    //  if applicable. It can then be recast as IM or PRES URIs.
    const CMailboxUri* GetMailboxUri() const;
#endif

    // Summary:
    //  Provides access to the addr-spec.
    const IUri* GetIUri() const;

    //<COMBINE CTxtNameAddr::GetIUri@const>
    const ITxtUri* GetUri() const;

    //<COMBINE CTxtNameAddr::GetIUri@const>
    ITxtUri* GetUri();

    //-- << Modifiers >>

#if defined (MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT)
    // Summary:
    //  Sets the URI to a MAILBOX URI.
    void SetMailboxUri(IN IUri::EUriType eType,
                       IN const char* pszUser,
                       IN const char* pszHost,
                       IN const char* pszDisplayName = g_pszEMPTY);
#endif

    // Summary:
    //  Sets the URI to a SIPURI.
    void SetSipUri(IN const char* pszHost,
                   IN uint16_t uPort = 0,
                   IN const char* pszUser = g_pszEMPTY,
                   IN CSipUri::ESecurityFlag eSecured = CSipUri::eUNSECURE,
                   IN const char* pszDisplayName = g_pszEMPTY);

    // Summary:
    //  Sets the URI value for the addr-spec part.
    void SetUri(IN const IUri* pUri, IN const char* pszDisplayName = g_pszEMPTY);

    // Summary:
    //  Sets the URI value for the addr-spec part.
    void SetUri(IN const ITxtUri* pUri, IN const char* pszDisplayName = g_pszEMPTY);

    // Summary:
    //  Resets data members to initial state.
    void Reset();

    //-- << Parsing >>

    // Summary:
    //  Parses the optional display name and following URI.
    mxt_result Parse(INOUT const char*& rpszNameAddrText);

    //-- << Serialization >>

    // Summary:
    //  Outputs the optional display name and following URI.
    void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
protected:

    //-- << Helpers >>

    // Summary:
    //  Updates internal SIP name addr from internal data.
    void UpdateInternalSipNameAddrHelper() const;

    // Summary:
    //  Updates internal data from the internal SIP name addr.
    void UpdateInternalDataHelper();

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:

    // The allocator to use to allocate memory.
    IAllocator* m_pAllocator;

private:

    mutable CNameAddr m_sipNameAddr;

    // The display name of the name-addr.
    CTxtToken m_tokDisplayName;

    // The URI of the name-addr in absolute form.
    CTxtAbsoluteUri m_uri;
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
//          The CTxtNameAddr to which to compare.
//
//  Returns:
//      - true: This name addr is not equivalent to rSrc.
//      - false: This name addr is equivalent to rSrc.
//
//  Description:
//      Comparison operator. Compares only the URI.
//
//==============================================================================
inline bool CTxtNameAddr::operator!=(IN const CTxtNameAddr& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The CNameAddr to which to compare.
//
//  Returns:
//      - true: This name addr is not equivalent to rSrc.
//      - false: This name addr is equivalent to rSrc.
//
//  Description:
//      Comparison operator. Compares only the URI.
//
//==============================================================================
inline bool CTxtNameAddr::operator!=(IN const CNameAddr& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The ITxtUri to which to compare. MUST be a CTxtAbsoluteUri.
//
//  Returns:
//      - true: This name addr is not equivalent to rSrc.
//      - false: This name addr is equivalent to rSrc.
//
//  Description:
//      Comparison operator. Compares only the URI.
//
//==============================================================================
inline bool CTxtNameAddr::operator!=(IN const ITxtUri* pSrc) const
{
    return !(*this == pSrc);
}

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The IUri to which to compare. MUST be a CTxtAbsoluteUri.
//
//  Returns:
//      - true: This name addr is not equivalent to rSrc.
//      - false: This name addr is equivalent to rSrc.
//
//  Description:
//      Comparison operator. Compares only the URI.
//
//==============================================================================
inline bool CTxtNameAddr::operator!=(IN const IUri* pSrc) const
{
    return !(*this == pSrc);
}

//==============================================================================
//====  PUBLIC METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  ACCESSORS  =============================================================
//==============================================================================

//==============================================================================
//==
//==  GetDisplayName
//==
//==============================================================================
//
//  Returns:
//      The display name as a Text Message Parser token.
//
//  Description:
//      Provides access to the display name as a Text Message Parser token.
//
//==============================================================================
inline const CTxtToken& CTxtNameAddr::GetDisplayName() const
{
    return m_tokDisplayName;
}

//--<COMBINE CTxtNameAddr::GetDisplayName@const>
inline CTxtToken& CTxtNameAddr::GetDisplayName()
{
    return m_tokDisplayName;
}

//--<COMBINE CTxtNameAddr::GetIUri@const>
inline const ITxtUri* CTxtNameAddr::GetUri() const
{
    return &m_uri;
}

//--<COMBINE CTxtNameAddr::GetIUri@const>
inline ITxtUri* CTxtNameAddr::GetUri()
{
    return &m_uri;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTNAMEADDR_H

