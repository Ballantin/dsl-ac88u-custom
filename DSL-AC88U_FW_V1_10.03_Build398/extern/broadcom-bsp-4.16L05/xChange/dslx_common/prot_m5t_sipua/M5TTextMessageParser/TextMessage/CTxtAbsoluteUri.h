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
#ifndef MXG_CTXTABSOLUTEURI_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTABSOLUTEURI_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members

//-- Interface Realized and/or Parent
#ifndef MXG_ITXTURI_H
#include "TextMessage/ITxtUri.h"
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
//== Class: CTxtAbsoluteUri
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 4>
//
// Summary:
//   This class abstracts an absolute URI.
//
// Description:
//   This class abstracts an absolute URI as inspired by RFC 3986, Appendix A
//   ABNF definition.
//
//  <CODE>
//  Inspired by RFC 3986 ABNF:
//    absoluteURI         = [scheme ":"] absoluteURI-content
//    scheme              = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
//
//    absoluteURI-content = *( ALPHA / DIGIT / gen-delims / sub-delims /
//                             "-" / "." / "_" / "~" / "%" )
//
//    gen-delims          = ":" / "/" / "?" / "#" / "[" / "]" / "@"
//    sub-delims          = "!" / "$" / "&" / "'" / "(" / ")" / "*" / "+" /
//                          "," / ";" / "="
//
//    ALPHA               =  %x41-5A / %x61-7A   ; A-Z / a-z
//    DIGIT               =  %x30-39             ; 0-9
//  </CODE>
//
//  The CTxtAbsoluteUri is used as a last resort when parsing URIs. When all
//  URIs that are natively supported by the Text Message Parser have been tried,
//  the unknown URI is parsed as a CTxtAbsoluteUri. Internally a token that uses
//  the absoluteURI-content accepted characters as its grammar is used to keep
//  the body of the URI. Also note that the CTxtAbsoluteUri::Parse method
//  considers the scheme name and following colon as optional. This allows
//  parsing versatility when the URI factory is used.
//
// Location:
//   TextMessage/CTxtAbsoluteUri.h
//
// See Also:
//      ITxtUri, CTxtUriFactory
//
//==============================================================================
class CTxtAbsoluteUri : public ITxtUri
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CTxtAbsoluteUri(IN IAllocator* pAllocator = NULL);

    // Summary:
    //  Copy Constructor.
    CTxtAbsoluteUri(IN const CTxtAbsoluteUri& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtAbsoluteUri();

    // Summary:
    //  Assignment operator.
    CTxtAbsoluteUri& operator=(IN const CTxtAbsoluteUri& rSrc);

    //-- << Accessors / ITxtUri::Accessors >>

    // Summary:
    //  Provides access to the body of the URI as a string.
    const char* GetBodyString() const;

    // Summary:
    //  Provides access to the body of the URI as a free-string CTxtToken.
    const CTxtToken& GetBodyToken() const;

    //<COMBINE CTxtAbsoluteUri::GetBodyToken@const>
    CTxtToken& GetBodyToken();

    // Summary:
    //  Returns the URI's scheme.
    virtual const char* GetSchemeText() const;

    // Summary:
    //  Returns the URI type, ITxtUri::eABSOLUTE in this case.
    virtual EUriType GetUriType() const;

    //-- << Modifiers / ITxtUri::Modifiers >>

    // Summary:
    //  Sets the scheme part of the URI.
    void SetScheme(IN const char* pszScheme);

    // Summary:
    //  Reinitializes the instance.
    virtual void Reset();

    //-- << ITxtUri::Comparators >>

    // Summary:
    //  Compares the given URI with this instance using applicable RFC rules.
    //  For this URI type, the scheme is compared case-insensitively and the
    //  body byte-per-byte.
    virtual bool IsEquivalent(IN const ITxtUri& rSrc) const;

    //-- << ITxtUri::Parsing >>

    // Summary:
    //  Parses the provided string to gather data that can be interpreted as a
    //  URI.
    virtual mxt_result Parse(INOUT const char*& rpszUriText);

    //-- << ITxtUri::Serialization >>

    // Summary:
    //  Outputs the URI into the provided blob.
    virtual void Serialize(INOUT CBlob& rBlob) const;

//-- Hidden Methods
protected:
private:

//-- Hidden Data Members
protected:
private:

    // Name of the URI's scheme.
    CTxtToken m_tokUriScheme;

    // Body of the URI, in the form that it was received in. The body is not
    // unescaped.
    CTxtToken m_tokUriBody;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  ACCESSORS / ITXTURI::ACCESSORS METHODS  ================================
//==============================================================================

//==============================================================================
//==
//==  GetBodyString
//==
//==============================================================================
//
//  Returns:
//      Body of the URI as a string.
//
//  Description:
//      Provides access to the unparsed body of the URI.
//
//==============================================================================
inline const char* CTxtAbsoluteUri::GetBodyString() const
{
    return m_tokUriBody.GetString();
}

//==============================================================================
//==
//==  GetBodyString
//==
//==============================================================================
//
//  Returns:
//      Body of the URI as a free-string CTxtToken.
//
//  Description:
//      Provides access to the unparsed body of the URI.
//
//==============================================================================
inline const CTxtToken& CTxtAbsoluteUri::GetBodyToken() const
{
    return m_tokUriBody;
}

//--<COMBINE CTxtAbsoluteUri::GetBodyToken@const>
inline CTxtToken& CTxtAbsoluteUri::GetBodyToken()
{
    return m_tokUriBody;
}

//==============================================================================
//====  MODIFIERS / ITXTURI::MODIFIERS METHODS  ================================
//==============================================================================

//==============================================================================
//==
//==  SetScheme
//==
//==============================================================================
//
//  Parameters:
//    rstrScheme:
//      Scheme to set. MUST NOT be NULL.
//
//  Description:
//      Sets the scheme to use.
//
//  See Also:
//      GetScheme
//
//==============================================================================
inline void CTxtAbsoluteUri::SetScheme(IN const char* pszScheme)
{
    if (pszScheme != NULL)
    {
        m_tokUriScheme = pszScheme;
    }
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTABSOLUTEURI_H
