//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_HEADERDEFINITION_H
#define MXG_HEADERDEFINITION_H

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


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CString;

//==============================================================================
//====  EXTERNAL PACKAGE DATA  =================================================
//==============================================================================

// The following code can be of some use to users of the SipParser package.
//--------------------------------------------------------------------------

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//  This table contains the order of headers. All headers must have a different
//  order number. By default, the header order is that of the ESipHeaderType
//  enum, except for the header types that proxies use.
//
//  The default header order number for each header is its (ESipHeaderType) enum
//  value times 1000. This means that there is a gap of 999 ordering slots
//  between each header type. Doing so gives a lot of room for users to reorder
//  the headers as they see fit. To reorder headers, users can simply use the
//  g_auHeaderOrder table directly and set a value for the wanted ESipHeaderType
//  type. Care must be taken that all header types must have different order
//  values, otherwise the sorting algorithm may produce unexpected results.
//
//  <CODE>
//   RFC 3261 Section 7.3.1
//    The relative order of header fields with different field names is not
//    significant. However, it is RECOMMENDED that header fields which are
//    needed for proxy processing (Via, Route, Record-Route, Proxy-Require,
//    Max-Forwards, and Proxy-Authorization, for example) appear towards the top
//    of the message to facilitate rapid parsing. The relative order of header
//    field rows with the same field name is important.
//  </CODE>
//
// Warning:
//  This table is indexed by the ESipHeaderType enum.
//
// Also see:
//  MxSetDefaultHeaderOrder
//
//------------------------------------------------------------------------------
extern uint32_t g_auHeaderOrder[];

// <GROUP SIPPARSER_ENUMS>
//
// Description:
//  The ESipHeaderType enum is the list of headers that the SIP stack 
//  understands. The eHDR_EXTENSION value is a special case and represents the
//  generic form of header as per RFC 3261, and is used to extend the stack to
//  work with headers that it does not know about or cannot parse.
//
// Warning:
//  This enum is used to index a table of header descriptors.
//
//------------------------------------------------------------------------------
enum ESipHeaderType
{
    eHDR_ACCEPT = 0,                      // RFC 3261 (SIP) headers.
    eHDR_ACCEPT_CONTACT,                  // RFC 3841 (Caller Preferences for the Session Initiation Protocol.)
    eHDR_ACCEPT_ENCODING,                 // RFC 3261 (SIP) headers.
    eHDR_ACCEPT_LANGUAGE,                 // RFC 3261 (SIP) headers.
    eHDR_ACCEPT_RESOURCE_PRIORITY,        // RFC 4412 (Communications Resource Priority for the Session Initiation Protocol (SIP))
    eHDR_ALERT_INFO,                      // RFC 3261 (SIP) headers.
    eHDR_ALLOW,                           // RFC 3261 (SIP) headers.
    eHDR_ALLOW_EVENTS,                    // RFC 3265 (Event notification) headers.
    eHDR_ANSWER_MODE,                     // RFC 5375 (Requesting Answering Modes for the Session Initiation Protocol (SIP)) headers.
    eHDR_AUTHENTICATION_INFO,             // RFC 3261 (SIP) headers.
    eHDR_AUTHORIZATION,                   // RFC 3261 (SIP) headers.
    eHDR_CALL_ID,                         // RFC 3261 (SIP) headers.
    eHDR_CALL_INFO,                       // RFC 3261 (SIP) headers.
    eHDR_CONTACT,                         // RFC 3261 (SIP) headers.
    eHDR_CONTENT_DESCRIPTION,             // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_CONTENT_DISPOSITION,             // RFC 3261 (SIP) headers.
    eHDR_CONTENT_ENCODING,                // RFC 3261 (SIP) headers.
    eHDR_CONTENT_ID,                      // RFC 2111 (Content-ID and Message-ID Uniform Resource Locators) headers.
    eHDR_CONTENT_LANGUAGE,                // RFC 3261 (SIP) headers.
    eHDR_CONTENT_LENGTH,                  // RFC 3261 (SIP) headers.
    eHDR_CONTENT_TRANSFER_ENCODING,       // RFC 2045 (Multipurpose Internet Mail Extensions (MIME) Part One) headers.
    eHDR_CONTENT_TYPE,                    // RFC 3261 (SIP) headers.
    eHDR_CSEQ,                            // RFC 3261 (SIP) headers.
    eHDR_DATE,                            // RFC 3261 (SIP) headers.
    eHDR_DIVERSION,                       // Draft-levy-sip-diversion-08.txt.
    eHDR_ERROR_INFO,                      // RFC 3261 (SIP) headers.
    eHDR_EVENT,                           // RFC 3265 (Event notification) headers.
    eHDR_EXPIRES,                         // RFC 3261 (SIP) headers.
    eHDR_FLOW_TIMER,                      // draft-ietf-sip-outbound-15.txt
    eHDR_FROM,                            // RFC 3261 (SIP) headers.
    eHDR_HISTORY_INFO,                    // RFC 4244 (An Extension to the Session Initiation Protocol (SIP) for Request History Information)
    eHDR_IDENTITY,                        // draft-ietf-sip-identity-06.txt headers.
    eHDR_IDENTITY_INFO,                   // draft-ietf-sip-identity-06.txt headers.
    eHDR_IN_REPLY_TO,                     // RFC 3261 (SIP) headers.
    eHDR_JOIN,                            // RFC 3911 (The Session Initiation Protocol (SIP) "Join" Header)
    eHDR_MAX_FORWARDS,                    // RFC 3261 (SIP) headers.
    eHDR_MIME_VERSION,                    // RFC 3261 (SIP) headers.
    eHDR_MIN_EXPIRES,                     // RFC 3261 (SIP) headers.
    eHDR_MIN_SE,                          // RFC 4028 (Session Timers in the Session Initiation Protocol)
    eHDR_ORGANIZATION,                    // RFC 3261 (SIP) headers.
    eHDR_P_ACCESS_NETWORK_INFO,           // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_P_ASSERTED_IDENTITY,             // RFC 3325 (Private Extensions to the SIP for Asserted Identity within Trusted Networks) headers.
    eHDR_P_ASSOCIATED_URI,                // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_P_CALLED_PARTY_ID,               // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_P_CHARGING_FUNCTION_ADDRESSES,   // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_P_CHARGING_VECTOR,               // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_P_MEDIA_AUTHORIZATION,           // RFC 3313 (Private SIP Extensions for Media Authorization) headers.
    eHDR_P_PREFERRED_IDENTITY,            // RFC 3325 (Private Extensions to the SIP for Asserted Identity within Trusted Networks) headers.
    eHDR_P_VISITED_NETWORK_ID,            // RFC 3455 (P-Header Extensions to the SIP for the 3GPP) headers.
    eHDR_PATH,                            // RFC 3327 (SIP Extension Header Field for Registering Non-Adjacent Contacts) headers.
    eHDR_PRIORITY,                        // RFC 3261 (SIP) headers.
    eHDR_PRIV_ANSWER_MODE,                // RFC 5375 (Requesting Answering Modes for the Session Initiation Protocol (SIP)) headers.
    eHDR_PRIVACY,                         // RFC 3323 (A Privacy Mechanism for the SIP) headers.
    eHDR_PROXY_AUTHENTICATE,              // RFC 3261 (SIP) headers.
    eHDR_PROXY_AUTHENTICATION_INFO,       // RFC 2617 (HTTP Authentication: Basic and Digest Access Authentication) headers.
    eHDR_PROXY_AUTHORIZATION,             // RFC 3261 (SIP) headers.
    eHDR_PROXY_REQUIRE,                   // RFC 3261 (SIP) headers.
    eHDR_RACK,                            // RFC 3262 (PRACK) headers.
    eHDR_REASON,                          // RFC 3326 (The Reason Header Field for the SIP) headers.
    eHDR_RECORD_ROUTE,                    // RFC 3261 (SIP) headers.
    eHDR_REFER_TO,                        // RFC 3515 (The SIP Refer Method) headers.
    eHDR_REFERRED_BY,                     // RFC 3892 (Referred-By Mechanism)
    eHDR_REJECT_CONTACT,                  // RFC 3841 (Caller Preferences for the Session Initiation Protocol.)
    eHDR_REPLACES,                        // RFC 3891 (The SIP "Replaces" Header) headers.
    eHDR_REPLY_TO,                        // RFC 3261 (SIP) headers.
    eHDR_REQUEST_DISPOSITION,             // RFC 3841 (Caller Preferences for the Session Initiation Protocol.)
    eHDR_REQUIRE,                         // RFC 3261 (SIP) headers.
    eHDR_RESOURCE_PRIORITY,               // RFC 4412 (Communications Resource Priority for the Session Initiation Protocol (SIP))
    eHDR_RETRY_AFTER,                     // RFC 3261 (SIP) headers.
    eHDR_ROUTE,                           // RFC 3261 (SIP) headers.
    eHDR_RSEQ,                            // RFC 3262 (PRACK) headers.
    eHDR_SERVER,                          // RFC 3261 (SIP) headers.
    eHDR_SERVICE_ROUTE,                   // RFC 3608 (SIP Extension Header Field for Service Route Discovery During Registration) headers.
    eHDR_SESSION_EXPIRES,                 // RFC 4028 (Session Timers in the Session Initiation Protocol)
    eHDR_SIP_ETAG,                        // RFC 3903 (Session Initiation Protocol (SIP) Extension for Event State Publication)
    eHDR_SIP_IF_MATCH,                    // RFC 3903 (Session Initiation Protocol (SIP) Extension for Event State Publication)
    eHDR_SUBJECT,                         // RFC 3261 (SIP) headers.
    eHDR_SUBSCRIPTION_STATE,              // RFC 3265 (Event notification) headers.
    eHDR_SUPPORTED,                       // RFC 3261 (SIP) headers.
    eHDR_TARGET_DIALOG,                   // RFC 4538 (Request Authorization through Dialog Identification in the Session Initiation Protocol (SIP)).
    eHDR_TIMESTAMP,                       // RFC 3261 (SIP) headers.
    eHDR_TO,                              // RFC 3261 (SIP) headers.
    eHDR_UNSUPPORTED,                     // RFC 3261 (SIP) headers.
    eHDR_USER_AGENT,                      // RFC 3261 (SIP) headers.
    eHDR_VIA,                             // RFC 3261 (SIP) headers.
    eHDR_WARNING,                         // RFC 3261 (SIP) headers.
    eHDR_WWW_AUTHENTICATE,                // RFC 3261 (SIP) headers.

    // Description:
    //  This is an application defined header (not related to any known RFC or
    //  draft). It is used when we don't recognize a header. This enum must be
    //  kept last. It maps to the RFC3261 extension-header construct.
    eHDR_EXTENSION
};

//==============================================================================
//====  METHOD PROTOTYPES  =====================================================
//==============================================================================

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  This method converts a header name to its enum form.
//-------------------------------------------------------
ESipHeaderType MxConvertSipHeader(IN const CString& rstrHeader);

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  Resets the header order to its default.
//------------------------------------------
void MxSetDefaultHeaderOrder();

//==============================================================================
//====  INTERNAL PACKAGE DATA  =================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
// The rest of this file contains SipParser-internal data not required by the
// user.
//----------------------------------------------------------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// Description:
//  This enum is used by the CSipHeader class to apply the data parsing and
//  serializing recipe for any header type.
//--------------------------------------------------------------------------
enum EDataType
{
    // The enums are used in a bitset. Currently only 4 bits are required to
    // store all values.

    // This data type is of the form : "token". Note that in all cases, tokens
    // are also used to represent a integer and unsigned integer values,
    // floats, and any non-space containing items.
    //-------------------------------------------------------------------------
    eDT_TOKEN               = 0,

    // This data type is of the form : "token token".
    //------------------------------------------------
    eDT_TWO_TOKENS          = 1,

    // This data type is of the form : "token token token".
    //------------------------------------------------------
    eDT_THREE_TOKENS        = 2,

    // This data type is of the form : "token '/' token".
    //----------------------------------------------------
    eDT_TOKEN_SLASH_TOKEN   = 3,

    // This data type is of the form : "token token token hostport".
    //---------------------------------------------------------------
    eDT_VIA                 = 4,

    // This data type is of the form : "(name-addr|addr-spec)".
    //----------------------------------------------------------
    eDT_NAMEADDR            = 5,

    // This data type is of the form : "(name-addr|addr-spec) or the Contact's
    // wildcard ('*')".
    //-------------------------------------------------------------------------
    eDT_CONTACT             = 6,

    // This data type is a raw unparsed header body. It contains everything up
    // to the CRLF or the separating comma.
    //-------------------------------------------------------------------------
    eDT_STRING              = 7,

    // This data type is a raw unparsed header body. It contains everything
    // including the CRLF and the separating comma.
    //----------------------------------------------------------------------
    eDT_FREE_STRING         = 8,

    // This data type is of the form "token". The difference with eDT_TOKEN is
    // that the following parameter list is separated with COMMA instead of
    // SEMI.
    //-------------------------------------------------------------------------
    eDT_TOKEN_AUTH          = 9,

    // This data type is of the form "gen-param".
    //--------------------------------------------
    eDT_PARAMETER           = 10,

    // This data type is a Date as per RFC 3261 sec 20.17.
    //-----------------------------------------------------
    eDT_DATE                = 11,

    // This data type is a Retry-After header as per RFC 3261 sec 20.33.
    eDT_RETRY_AFTER         = 12,

    // This data type is of the form: "(RFC2822-name-addr|RFC2822-addr-spec)"
    eDT_MAILBOX_URI         = 13,

    // This data type is of the form: "gen-param *(COMMA gen-param)". The
    // difference with eDT_TOKEN_AUTH is that it contains no token at the
    // beginning (the header value is always empty).
    eDT_PARAMETER_COMMA_SEP = 14
};


// Description:
//  This structure contains all the information associated with one header.
//  This information is used in parsing and serializing.
//--------------------------------------------------------------------------
struct SHeaderDescriptor
{
    // Full header name.
    //-------------------
    const char* m_szLongName;

    // Short header name, or NULL if no short name is available.
    //-----------------------------------------------------------
    const char* m_szShortName;

    // This bitset contains many informations pertinent to the parsing and
    // serialization of a header.
    //
    //  <CODE>
    //     MSB                         LSB
    //     1 1 1 1 1 1
    //     5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    //    +-------------+-+-+-+-+-+------+
    //    |   Unused    |G|E|P|C|M|  PT  |
    //    +-------------+-+-+-+-+-+------+
    //
    //    Bits
    //    15-9: Reserved
    //          Reserved for future use (Shall always be 0)
    //
    //    8:    GenCombined
    //          If true, multiple instances of this header will be serialized on
    //          one CRLF-terminated line. The default rule is that headers that
    //          CAN be combined WILL be combined. The CanBeCombined bit contains
    //          this information. This value cannot be true if CanBeCombined is
    //          false. This value can be changed using the
    //          ISipCoreConfig::SetCommaSeparatedHeader method.
    //
    //    7:    CanBeEmpty
    //          The header type's value can be empty.
    //
    //    6:    HasParams
    //          An optional list of semi-colon separated parameters is allowed
    //          for this header type.
    //
    //    5:    CanBeCombined
    //          The header type is a comma-separated header. Multiple instances
    //          can be combined on a single CRLF-terminated line.
    //
    //    4:    IsMulti
    //          Multiple instances of the header can be found in a single
    //          packet.
    //
    //    3-0:  EDataType. The type of data to parse in the header's value. Maps
    //          to the EDataType enum.
    //
    //            Bits 3 2 1 0 definition:
    //                 0 0 0 0 - eDT_TOKEN
    //                 0 0 0 1 - eDT_TWO_TOKENS
    //                 0 0 1 0 - eDT_THREE_TOKENS
    //                 0 0 1 1 - eDT_TOKEN_SLASH_TOKEN
    //                 0 1 0 0 - eDT_VIA
    //                 0 1 0 1 - eDT_NAMEADDR
    //                 0 1 1 0 - eDT_CONTACT
    //                 0 1 1 1 - eDT_STRING
    //                 1 0 0 0 - eDT_FREE_STRING
    //                 1 0 0 1 - eDT_TOKEN_AUTH
    //                 1 0 1 0 - eDT_PARAMETER
    //                 1 0 1 1 - eDT_DATE
    //                 1 1 0 0 - eDT_RETRY_AFTER
    //                 1 1 0 1 - eDT_MAILBOX_URI
    //                 1 1 1 0 - eDT_PARAMETER_COMMA_SEP
    //                 Other values are unused.
    //  </CODE>
    uint16_t m_uBitset;
};


// Description:
//  This is the table of header descriptions for all known headers.
//
// Warning:
//  This table is indexed by the ESipHeaderType enum.
//
//------------------------------------------------------------------------------
extern SHeaderDescriptor g_astHeaderDescriptor[eHDR_EXTENSION+1];


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

// These variables are the bitmasks used to set and verify the presence of bits
// in the SHeaderDescriptor::m_uBitset bitset. Each bit mask has only one bit
// set to one. This bit is at the index position as specified in the definition
// of SHeaderDescriptor::m_uBitset.
//------------------------------------------------------------------------------
const uint16_t uMULTI_SET = (1 << 5);
const uint16_t uCOMBINE_SET = (1 << 6);
const uint16_t uEMPTY_SET = (1 << 7);
const uint16_t uPARAMS_SET = (1 << 8);
const uint16_t uGENCOMB_SET = (1 << 9);

//==SDOC========================================================================
//==
//==  MxIsSet
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//    uBitMask:
//      Bit mask against which to compare.
//
//  Returns:
//      True if the bits in the mask are also all set in the header descriptor
//      bitset for the header type.
//
//  Description:
//      Verifies if the header's bitset contains the specified bits.
//
//==EDOC========================================================================
inline bool MxIsSet(IN ESipHeaderType eHeaderType, IN uint32_t uBitMask)
{
    return (g_astHeaderDescriptor[eHeaderType].m_uBitset & uBitMask) > 0;
}

//==SDOC========================================================================
//==
//==  MxIsMulti
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      True if the header type can be found multiple times within a packet.
//
//  Description:
//      Fetches the IsMulti field in the header descriptor bitset.
//
//==EDOC========================================================================
inline bool MxIsMulti(IN ESipHeaderType eHeaderType)
{
    return MxIsSet(eHeaderType, uMULTI_SET);
}

//==SDOC========================================================================
//==
//==  MxCanBeCombined
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      True if the header type can be combined on one line.
//
//  Description:
//      Fetches the CanBeCombined field in the header descriptor bitset.
//
//==EDOC========================================================================
inline bool MxCanBeCombined(IN ESipHeaderType eHeaderType)
{
    return MxIsSet(eHeaderType, uCOMBINE_SET);
}

//==SDOC========================================================================
//==
//==  MxCanBeEmpty
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      True if the header type's value can be empty.
//
//  Description:
//      Fetches the CanBeEmpty field in the header descriptor bitset.
//
//==EDOC========================================================================
inline bool MxCanBeEmpty(IN ESipHeaderType eHeaderType)
{
    return MxIsSet(eHeaderType, uEMPTY_SET);
}

//==SDOC========================================================================
//==
//==  MxHasParams
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      True if the header type can have parameters.
//
//  Description:
//      Fetches the HasParams field in the header descriptor bitset.
//
//==EDOC========================================================================
inline bool MxHasParams(IN ESipHeaderType eHeaderType)
{
    return MxIsSet(eHeaderType, uPARAMS_SET);
}

//==SDOC========================================================================
//==
//==  MxGenCombined
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      True if the header type is comma-separated on serialization.
//
//  Description:
//      Fetches the combined generation field in the header descriptor bitset.
//
//  See Also:
//      MxSetGenCombined
//
//==EDOC========================================================================
inline bool MxGenCombined(IN ESipHeaderType eHeaderType)
{
    return MxIsSet(eHeaderType, uGENCOMB_SET);
}

// The first 4 bits are set to 1. Used to verify which parse type the bitset
// hides.
//---------------------------------------------------------------------------
const uint16_t uPARSE_TYPE_SECTION_ONLY_BITSET = 0x0F;

//==SDOC========================================================================
//==
//==  MxParseType
//==
//==============================================================================
//
//  Parameters:
//    eHeaderType:
//      Header to check.
//
//  Returns:
//      Parse type for the specified header.
//
//  Description:
//      Fetches the parse type for the specified header from the header
//      descriptor bitset.
//
//==EDOC========================================================================
inline EDataType MxParseType(IN ESipHeaderType eHeaderType)
{
    return EDataType(g_astHeaderDescriptor[eHeaderType].m_uBitset &
                                            uPARSE_TYPE_SECTION_ONLY_BITSET );
}

// Summary:
//  Sets the comma-separation behaviour in the header bitset.
//------------------------------------------------------------
void MxSetGenCombined(IN ESipHeaderType eHeaderType, IN bool bOneLine);

//M5T_INTERNAL_USE_END
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_HEADERDEFINITION_H

