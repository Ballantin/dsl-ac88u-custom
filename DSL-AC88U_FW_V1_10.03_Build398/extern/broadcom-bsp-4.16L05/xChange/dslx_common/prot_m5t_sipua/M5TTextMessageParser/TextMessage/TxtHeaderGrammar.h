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
#ifndef MXG_TXTHEADERGRAMMAR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_TXTHEADERGRAMMAR_H
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

//==============================================================================
//====  EXTERNAL PACKAGE DATA  =================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//====  METHOD PROTOTYPES  =====================================================
//==============================================================================

//==============================================================================
//====  INTERNAL PACKAGE DATA  =================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//<GROUP TEXT_MESSAGE_ENUMS>
//
// Summary:
//  This enum defines the basic data types used in headers.
//
// Description:
//  This enum is used by the CTxtHeader class to apply the data parsing and
//  serializing recipe for any header type.
//
//  The format of each of the following header types MUST be as the example
//  in double-quotes.
enum ETxtHeaderGrammar
{
    //-- The enums are used in a bitset. Currently only 5 bits are required to
    //-- store all values up to 12 (enum value of eTHDT_URL_LIST) and provide
    //-- for future extensions.

    // Description:
    // This data type is of the form: "token". Note that in all cases, tokens
    // are also used to represent integer and unsigned integer values,
    // floats, and any non-space containing items.
    eTHDT_TOKEN               = 0,

    // Description:
    // This data type is of the form: "token ' ' token".
    eTHDT_TWO_TOKENS          = 1,

    // Description:
    // This data type is of the form: "token ' ' token ' ' token".
    eTHDT_THREE_TOKENS        = 2,

    // Description:
    // This data type is of the form: "token '/' token".
    eTHDT_TOKEN_SLASH_TOKEN   = 3,

    // Description:
    // This data type is of the form: "token '/' token '/' token ' ' hostport".
    eTHDT_TOKEN_SLASH_TOKEN_SLASH_TOKEN_HOSTPORT = 4,

    // Description:
    // This data type is of the form:
    //     name-addr    = ([ display-name ] ["<"] addr-spec [">"]) / addr-spec
    //     display-name = *(token LWS) / quoted-string
    //     addr-spec    =  URI
    //
    // When there is a display-name, the angle brackets are NOT optional.
    eTHDT_NAMEADDR            = 5,

    // Description:
    // This data type is of the form: Same as eTHDT_NAMEADDR or a wildcard
    // character ('*').
    eTHDT_NAMEADDR_WILDCARD   = 6,

    // Description:
    // This data type is a raw unparsed header body. It contains everything up
    // to the CRLF, the separating comma or the parameter delimiter ';'
    // (excluding those).
    eTHDT_STRING              = 7,

    // Description:
    // This data type is a raw unparsed header body. It contains everything
    // including the CRLF or the separating comma.
    eTHDT_FREE_STRING         = 8,

    // Description:
    // This data type is of the form "token". The difference with eTHDT_TOKEN is
    // that the following parameter list is separated with COMMA instead of
    // SEMI.
    eTHDT_TOKEN_AUTH          = 9,

    // Description:
    // This data type is of the form "gen-param".
    //  gen-param = [';'] param-name '=' param-value.
    eTHDT_PARAMETER           = 10,

    // Description:
    // This data type is a Date as per RFC 3261 sec 20.17.
    eTHDT_DATE                = 11,

    // Description:
    // This data type is a space separated URL list in the form:
    // URL [1*(WSP URL)]". As defined in RFC 4975, section 9.
    eTHDT_URL_LIST            = 12,

    // Description:
    // This is an error value returned when the txt header is ill-configured.
    eTHDT_UNKNOWN             = 0x1F,
};


//<GROUP TEXT_MESSAGE_STRUCTS>
//
// Summary:
//  This structure contains all the information associated with one header.
//
// Description:
//  This structure contains all the information associated with one header.
//  This information is used in parsing and serializing.
//
//  Each protocol/datatype parser is responsible to define its own array of
//  STxtHeaderDescriptor, where each header is properly defined and
//  alphabetically sorted. Whenever a new header for the protocol/datatype is
//  created, it also creates a CTxtHeader, which takes as a parameter to its
//  contructor a pointer to this structure definition in the array.
//
//  The automatic extension header is of type eTHDT_FREE_STRING, can appear
//  multiple times but not combined, can be empty and cannot have parameters.
//  In fact, parameters, if any, will be consumed and kept in the header's
//  free string value by the parsing. Extension headers are always serialized
//  last in the header list.
struct STxtHeaderDescriptor
{
    // Description:
    // Full header name. For supported headers, it MUST NOT be NULL. An
    // extension header will have this member set to NULL and is automatically
    // provided by the Text Message Parser. Thus, protocol implementations need
    // not and MUST NOT define their own extension headers.
    const char* m_pszLongName;

    // Description:
    // Short header name, or NULL if no short name is available.
    const char* m_pszShortName;

    // Description:
    // Indicates in which order the header types must be serialized in the
    // packet.
    unsigned int m_uSerializationOrder;

    // Description:
    // This bitset contains many informations pertinent to the parsing and
    // serialization of a header.
    //
    //  <CODE>
    //  MSB                                                          LSB
    //   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
    //   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    //  +-----------------------------------------+-+-+-+-+-+-+---------+
    //  |   Unused                                |L|H|G|C|M|E|   DT    |
    //  +-----------------------------------------+-+-+-+-+-+-+---------+
    //
    //  Bits
    //  31-11: Reserved
    //         Reserved for future use (Shall always be 0)
    //
    //  10:   Parameter Location (L)
    //        If true (1), parameters MUST be placed before the header value
    //        (e.g. CPIM). If false (0), parameters are placed after the header
    //        value (e.g. SIP).
    //        Use CTxtHeader::ms_uPARAMS_LOCATION_BEFORE_VALUE_SET to set this
    //        bit.
    //
    //  9:    HasParams (H)
    //        If true (1), the header can have an optional list of semi-colon
    //        separated parameters. Use CTxtHeader::ms_uHAS_PARAMS_SET to set
    //        this bit.
    //
    //  8:    GenCombined (G)
    //        If true (1), multiple instances of this header will be serialized
    //        on one CRLF-terminated line. The default rule is that headers that
    //        CAN be combined WILL be combined. The CanBeCombined bit contains
    //        this information. This value cannot be true if CanBeCombined is
    //        false. Use CTxtHeader::ms_uGENERATE_COMBINED_SET to set this bit.
    //
    //  7:    CanBeCombined (C)
    //        If true (1), the header type is a comma-separated header. Multiple
    //        instances can be combined on a single CRLF-terminated line. Use
    //        CTxtHeader::ms_uCAN_BE_COMBINED_SET to set this bit.
    //
    //  6:    IsMulti (M)
    //        If true (1), multiple instances of the header can be found in a
    //        single packet. Use CTxtHeader::ms_uMULTIPLE_INSTANCE_SET to set
    //        this bit.
    //
    //  5:    CanBeEmpty (E)
    //        If true (1), the header type's value can be empty. Use
    //        CTxtHeader::ms_uCAN_BE_EMPTY_SET to set this bit.
    //
    //  4-0:  ETxtHeaderGrammar. The type of data to parse in the header's
    //        value. Maps to the ETxtHeaderGrammar enum.
    //
    //          Bits 4 3 2 1 0 - Definition:
    //               0 0 0 0 0 - eTHDT_TOKEN
    //               0 0 0 0 1 - eTHDT_TWO_TOKENS
    //               0 0 0 1 0 - eTHDT_THREE_TOKENS
    //               0 0 0 1 1 - eTHDT_TOKEN_SLASH_TOKEN
    //               0 0 1 0 0 - eTHDT_TOKEN_SLASH_TOKEN_SLASH_TOKEN_HOSTPORT
    //               0 0 1 0 1 - eTHDT_NAMEADDR
    //               0 0 1 1 0 - eTHDT_NAMEADDR_WILDCARD
    //               0 0 1 1 1 - eTHDT_STRING
    //               0 1 0 0 0 - eTHDT_FREE_STRING
    //               0 1 0 0 1 - eTHDT_TOKEN_AUTH
    //               0 1 0 1 0 - eTHDT_PARAMETER
    //               0 1 0 1 1 - eTHDT_DATE
    //               0 1 1 0 0 - eTHDT_URL_LIST
    //               0 1 1 0 1 to
    //               1 1 1 1 0 - Reserved for future use.
    //               1 1 1 1 1 - eTHDT_UNKNOWN, this indicates a configuration
    //                           error.
    //  </CODE>
    uint32_t m_uBitset;
};

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
extern const STxtHeaderDescriptor g_stDefaultHeaderDescriptor;
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_TXTHEADERGRAMMAR_H

