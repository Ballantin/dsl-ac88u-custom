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
#ifndef MXG_ITXTURI_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ITXTURI_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members

//-- Parameters used by Value

//-- Interface Realized and/or Parent

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class CString;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ITxtUri
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 3>
//
// Summary:
//   The ITxtUri is the base interface for all URI types that are supported by
//   the "Text Message" parser.
//
// Description:
//   The ITxtUri is the base interface for all URI types that are supported by
//   the <i>Text Message</i> parser (TMP). It contains basic methods
//   that all URI-abstracting classes implement.
//
//   The ITxtUri interface is used in the TMP everywhere a URI can be used. For
//   example, a header that accepts a "Name Address" construct will use a URI,
//   the "Request Line" of SIP and HTTP packets will also use URIs.
//
//   There are numerous flavors of URIs but all have similar syntaxes. They all
//   start by specifying a scheme which is separated from the URI value by a
//   COLON.
//  <CODE>
//  Generic URI definition (inspired from RFC 3986):
//      gen-uri = scheme ":" uri-definition
//      scheme = "sip" / "sips" / "tel" / "mailbox" / "im" / "pres" / "http"
//               "https" / "mail" / scheme-token
//
//      scheme-token = Any other token that is defined in a protocol's specific
//                     RFC. As an example, RFC 3986 (Base URI RFC) defines this
//                     token as
//                       alpha *( alpha | digit | "+" | "-" | "." )
//
//      uri-definition = Any definition that is suitable for a particular
//                       protocol. This shall be described in that protocol
//                       particular RFCs. As an example, SIP/SIPS URIs shall
//                       look like:
//                       [user [":" password]]@hostport[";" parameters]
//                           ["?" headers]
//
//                       And HTTP URIs shall look like:
//                       "//" hostport "/" path-to-document ["?" parameters]
//  </CODE>
//
// Location:
//   TextMessage/ITxtUri.h
//
// See Also:
//   CTxtUriFactory, CTxtAbsoluteUri
//
//==============================================================================
class ITxtUri
{
//-- New types definition
public:

    // All URI types must be listed in this enum. It is required because of the
    // IsEquivalent method.
    enum EUriType
    {
        // Any RFC 2396 compliant URI that the Text Message Parser does not
        // support explicitly, for example ftp.
        eABSOLUTE,

        // RFC3261 "sip" URI.
        eSIP,

        // RFC3261 "sips" URI.
        eSIPS,

        // RFC3966 "tel" URI.
        eTEL,

        // RFC2822 "mailbox" URI.
        eMAILBOX,

        // RFC3859 "pres" URI.
        ePRES,

        // RFC3860 "im" URI.
        eIM,

        // RFC2616 "http" URI.
        eHTTP,

        // RFC2818 "https" URI.
        eHTTPS
    };

//-- Published Interface
public:

    //-- << Accessors >>

    //==========================================================================
    //==
    //==  GetSchemeText
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the URI's scheme in text form.
    //
    //  Returns:
    //      URI's scheme.
    //
    //  Description:
    //      Returns the URI's scheme in text form.
    //
    //==========================================================================
    virtual const char* GetSchemeText() const = 0;

    //==========================================================================
    //==
    //==  GetUriType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns the URI type.
    //
    //  Returns:
    //      URI type.
    //
    //  Description:
    //      Returns the URI type.
    //
    //==========================================================================
    virtual EUriType GetUriType() const = 0;

    //-- << Comparators >>

    //==========================================================================
    //==
    //==  IsEquivalent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Compares the given URI with this instance by using applicable RFC
    //      rules.
    //
    //  Parameters:
    //    rSrc:
    //      The URI against which to verify the equivalence.
    //
    //  Returns:
    //      - true: rSrc is an equivalent URI according to the applicable RFC's
    //              URI comparison rules.
    //      - false: otherwise.
    //
    //  Description:
    //      Compares the given URI with this instance by using applicable RFC
    //      rules.
    //
    //==========================================================================
    virtual bool IsEquivalent(IN const ITxtUri& rSrc) const = 0;

    //-- << Modifiers >>

    //==========================================================================
    //==
    //==  Reset
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reinitializes the instance.
    //
    //  Description:
    //      Reinitializes the instance.
    //
    //==========================================================================
    virtual void Reset() = 0;

    //-- << Parsing >>

    //==========================================================================
    //==
    //==  Parse
    //==
    //==========================================================================
    //
    //  Summary:
    //      Parses a byte string into usable data.
    //
    //  Parameters:
    //      rpszUriText:
    //          Position from which to start. rpcPos initially points after the
    //          scheme and colon. rpcPos is adjusted as parsing progresses. If
    //          Parse() fails, rpcPos points to the position that caused the
    //          failure. URIs MUST NOT skip trailing LWS.
    //
    //  Returns:
    //      - resS_OK: Parsing succeeded and there is no more data in
    //                 rpszUriText.
    //      - warning: Parsing succeeded and there is still data in rpszUriText.
    //      - failure: Parsing failed.
    //
    //  Description:
    //      Parses a URI.
    //
    //==========================================================================
    virtual mxt_result Parse(INOUT const char*& rpszUriText) = 0;

    //-- << Serialization >>

    //==========================================================================
    //==
    //==  Serialize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Outputs the data members in the provided blob.
    //
    //  Parameters:
    //    rBlob:
    //      CBlob into which the URI is output.
    //
    //  Description:
    //      Outputs the contents of the URI. This method outputs the scheme and
    //      the following COLON, then the body of the URI.
    //
    //==========================================================================
    virtual void Serialize(INOUT CBlob& rBlob) const = 0;

//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ITxtUri() {};

    // Summary:
    //  Destructor.
    virtual ~ITxtUri() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ITxtUri(IN const ITxtUri& rSrc);

    // Summary:
    //  Assignment Operator.
    ITxtUri& operator=(IN const ITxtUri& rSrc);

//-- Hidden Data Members
protected:
private:
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ITXTURI_H

