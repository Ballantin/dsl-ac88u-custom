//==============================================================================
//==============================================================================
//
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_SDPPARSERCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SDPPARSERCFG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//@@TID_COMPILE_CONFIG_HOWTO_SCECORE
//<TITLE Configuring SDP SAFE with "PreSdpParserCfg.h">
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 1>
//
// SDP SAFE comes with the file "Config/SdpParserCfg.h" which defines many
// compilation configuration options and values used specifically by the SDP
// Parser. Generally, these values will need updating for the specific
// application being developed with SDP SAFE.
//
// To update these default values, you must create the "PreSdpParserCfg.h" file
// with the updated configuration options for your application.
// "PreSdpParserCfg.h" is always included first by "SdpParserCfg.h" to
// retrieve application specific configurations, and then the default
// configuration options found in "Config/SdpParserCfg.h" are applied for all
// items that were not configured by the application.
//
// "PreSdpParserCfg.h" is not packaged with SDP SAFE and must be created for the
// specific application being developed. This file must simply be placed
// somewhere in the compiler search path to permit the retrieval of the
// application specific configuration options by SDP SAFE.
//
//******************************************************************************
//******************************************************************************


// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSdpParserCfg.h".
//
//     2 : You have created "PreSdpParserCfg.h" but it is not found by the
//         application
#include "PreSdpParserCfg.h"



MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
// The topics for these variables are defined below.

#ifdef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM

    // Since the enums are used to index an array of strings, both arrays must
    // be extended.
    #ifndef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME
        #error Must define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME as well.
    #endif

    #ifndef MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY
        #error Must define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY as well.
    #endif

    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM_PRIVATE MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM
#else
    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM_PRIVATE
#endif

#ifdef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME

    // Since the enums are used to index an array of strings, both arrays must
    // be extended.
    #ifndef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM
        #error Must define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM as well.
    #endif

    #ifndef MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY
        #error Must define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY as well.
    #endif

    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME_PRIVATE MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME
#else
    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME_PRIVATE
#endif

#ifdef MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY

    #ifndef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM
        #error Must define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM as well.
    #endif

    #ifndef MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME
        #error Must define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME as well.
    #endif

    #define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY_PRIVATE MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY
#else
    #define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY_PRIVATE
#endif

#ifdef MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE
    #ifndef MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT
        #error Must define MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT as well
    #endif
#endif

//M5T_INTERNAL_USE_END

// Below this is the documentation of the various configuration macros
// available.
//---------------------------------------------------------------------
#if 0
//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of T.38.
//
// Description:
//  This switch is used to control whether or not the SDP parser recognizes the
//  parameters specific to T.38 when parsing a media line in an SDP packet. You
//  can enable this switch if your application wants to support the T.38 fax
//  protocol. This enables the compilation of a few classes and thus
//  increases the size of the build.
//
// Location:
//  Define this in PreSdpParserCfg.h if T.38 parsing classes are required.
//
//==============================================================================
#define MXD_SDP_ENABLE_T38_SUPPORT


//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of SRTP.
//
// Description:
//  This switch is used to control whether or not the SDP parser recognizes the
//  parameters specific to SRTP when parsing a media line in an SDP packet. You
//  can enable this switch if your application wants to support the SRTP
//  additions to SDP. This enables the compilation of a few classes and thus
//  increases the size of the build.
//
// Location:
//  Define this in PreSdpParserCfg.h if SRTP parsing classes are required.
//
//==============================================================================
#define MXD_SDP_ENABLE_SRTP_SUPPORT


//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of telephone event FMTP attribute.
//
// Description:
//  Enables the support for telephone event FMTP attribute in the SDP
//  capabilities manager.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
// See Also:
//  CSdpCapabilitiesMgr
//
//==============================================================================
#define MXD_SDP_ENABLE_TELEPHONE_EVENT_FMTP_ATTRIBUTE


//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support for redundancy FMTP attribute.
//
// Description:
//  Enables the support for redundancy FMTP attribute in the SDP capabilities
//  manager.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
// See Also:
//  CSdpCapabilitiesMgr
//
//==============================================================================
#define MXD_SDP_ENABLE_REDUNDANCY_FMTP_ATTRIBUTE

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support for key management attribute negotiation.
//
// Description:
//  Enables the support for key management attribute negotiation in the SDP
//  capabilities manager. Negotiation is done according to RFC 4567 by using
//  the first key management attribute supported both locally and by the peer.
//  The local string configured in the CSdpFieldAttributeKeyMgmt is sent
//  to the peer.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
// See Also:
//  CSdpCapabilitiesMgr
//
//==============================================================================
#define MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the integration of M5T MIKEY key management.
//
// Description:
//  Enables the integration of M5T MIKEY key management in SDP. Negotiation is
//  done according to RFC 4567. In this mode, only MIKEY key-mgmt attributes are
//  supported and all other key-mgmt attribute, whether supported or not, are
//  ignored. When using this mode, users must configure key management using
//  the MIKEY class.
//
//  When an application sets up a MIKEY key-mgmt attribute in this mode with
//  the corresponding CSdpKeyManagementParameterMikey, the SDP capabilities
//  manager does the following in client mode:
//  - Generates a MIKEY message according to the configured information for
//    each media, base 64 encodes it, and adds it to the SDP packet.
//  - When the answer is received, it decodes the MIKEY answer, parses the MIKEY
//    message, and then sets the needed data in the corresponding
//    CSdpKeyManagementParameterMikey.
//
//  In server mode:
//  - When generating an answer, it matches the first MIKEY key-mgmt with the
//    first locally supported one. It parses this message and configures the
//    CSdpKeyManagementParameterMikey.
//  - When the answer is returned, it generates a MIKEY response or error
//    message according to the configured parameters.
//
//  MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE MUST be defined for this
//  define to be available.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
// See Also:
//  CSdpCapabilitiesMgr
//
//==============================================================================
#define MXD_SDP_ENABLE_KEY_MANAGEMENT_MIKEY_ATTRIBUTE

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of silenceSupp media level attribute.
//
// Description:
//  When defined, the SDP parser supports the silenceSupp media level
//  attribute. The capabilities manager is also able to negotiate the
//  silenceSupp attribute (fmtp).
//
//  Note that silenceSupp is intepreted as a media level attribute only (not
//  valid at the session level). http: www.iana.org/assignments/sdp-parameters
//  specifies that silenceSupp is not considered in one of three attribute
//  categories:
//  att-field (session level)
//  att-field (both session and media level)
//  att-field (media level only)
//
//  but is considered as unknown:
//  att-field (unknown level)
//
//  The IETF itself does not consider the silenceSupp as a media or session
//  level attribute, and it is a matter of implementation decision.
//
// Location:
//  Define this in PreSdpParserCfg.h.
//
//==============================================================================
#define MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  When defined, indicates that silence suppression is to be considered
//  disabled when the "annex=" fmtp parameter is not specified whithin a session
//  description.
//
// Description:
//  When defined, the capabilities manager will consider that VAD is not
//  enabled if the "annex=" format attribute is absent. The default behaviour
//  is to consider VAD as enabled, as per RFC 3555.
//
//  This macro only impacts the behaviour if the "annex=" format attribute is
//  absent. If the format attribute is present, its value will have precedence.
//  Since this macro also affects the interpretation of the "index=" format
//  attribute absence from the local caps, it is recommended to always
//  explicitely specify if VAD is enabled or not when using this macro.
//
// Location:
//  Define this in PreSdpParserCfg.h.
//
//==============================================================================
#define MXD_SDP_SILENCE_SUPPRESSION_INDICATION_ABSENCE_MEANS_DISABLED

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the extension of the compression algorithm enum array.
//
// Description:
//  This switch enables users to extend the CSdpParser::ERtpCompressionAlgorithm
//  enum to support codecs that are not included in M5T's standard list. Users
//  must define this to the enum values that would be put inside the enum
//  declaration, each additional codec separated with a comma. When defined,
//  this value must be terminated be a comma.
//
// Example:
//  <CODE>
//    Note: To compile this sample you must either add a backslash at the end
//    of the first line or have everything on the same line.
//
//    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM eEXTENDED1,
//                                                           eEXTENDED2,
//  </CODE>
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the extension of the compression algorithm string array.
//
// Description:
//  This switch enables users to extend the
//  CSdpParser::s_aszRtpCompressionAlgorithmMap array of strings to support
//  codecs that are not included in M5T's standard list. Users must define this
//  to the text that would be put inside a string array, each additional codec
//  separated with a comma. When defined, this value must be terminated by a
//  comma.
//
// Example:
//  <CODE>
//    Note: To compile this sample you must either add a backslash at the end
//    of the first line or have everything on the same line.
//
//    #define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME "ExtendedCodec1",
//                                                           "ExtendedCodec2",
//  </CODE>
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_NAME

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the extension of the RTP Map array.
//
// Description:
//  This switch enables users to extend the CSdpCapsManager::s_aRtpAlgorithmMap
//  array, to support codecs that are not included in M5T's standard list. This
//  array holds the default payload number, clock rate, and number of encoding
//  parameters associated with a codec. Users must define this to the definition
//  of a structure instance that would be put inside an array of structures,
//  each additional codec separated with a comma. When defined, this value must
//  be terminated by a comma.
//
// Warning:
//  Care should be taken that the used payload numbers do not clash with those
//  already set by M5T in the CSdpCapsManager::s_aRtpAlgorithmMap array.
//
// Example:
//  { payload number, clock rate, number of encoding parameters }
//
//  Using a number of encoding parameters less than 2 will result in it not
//  being serialized on output as this is the SDP default value.
//
//  <CODE>
//    Note: To compile this sample you must either add a backslash at the end
//    of the first line or have everything on the same line.
//
//      #define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY
//              {109, 5000,   1},
//              {110, 16000,  4},
//  </CODE>
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDPCAPSMGR_USER_EXTEND_RTPMAP_ARRAY

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of missing media lines in SDP answers.
//
// Description:
//  This switch allows the VerifyAnswer method to match media lines even if
//  the peer removed some media lines in the response.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDP_SUPPORT_MISSING_MEDIA_LINE_IN_ANSWER

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support of non-compliant send/receive attribute in SDP answers.
//
// Description:
//  This switch allows the VerifyAnswer method to accept answers containing
//  streams with an attribute of a=sendrecv when the offer specified a=sendonly
//  or a=recvonly.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDP_SUPPORT_NON_COMPLIANT_SENDRECV_ANSWER

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support for the B2BUA connector.
//
// Description:
//  Enables the support for the B2BUA connector. This helper class implements
//  the necessary functionality required to allow connecting at the media level
//  two UAs that previously had their media routed through a third-party entity.
//  This handles only the on / off part. The rest of the parsing is up to the
//  application.
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
// See Also:
//  CSdpB2BUaConnector
//
//==============================================================================
#define MXD_SDP_B2BUA_CONNECTOR_ENABLE_SUPPORT

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the support for the ICE attributes.
//
// Description:
//  Enables the support for the ICE attributes (parsing and negotiation).
//
// Location:
//  Define this in PreSdpParserCfg.h or in your makefile.
//
//==============================================================================
#define MXD_SDP_ICE_ENABLE_SUPPORT

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the address negotiation using ICE lite.
//
// Description:
//   Enables negotiation using ICE candidates without connectivity check.
//
//   The application is responsible to add ICE candidates into each media of the
//   capabilities manager corresponding to the addresses of its RTP and RTCP
//   streams.
//
//   When sending an offer, the capabilities manager automatically takes care of:
//
//   - Choosing one candidate for each component (RTP and RTCP) to be the
//     default destination. If IPv4 is supported, the IPv4 candidates
//     have priority over IPv6.
//   - Setting the IP address of the RTP candidate in the c line and the port of
//     the RTP candidate in the m line.
//   - Setting the IP address of the RTCP candidate in the RTCP attribute.
//   - Adding the ice-lite, ice-ufrag and ice-pwd attributes if not already
//     present.
//
//   When generating an answer, the capabilities manager automatically takes
//   care of:
//
//   - Checking if there is the ice-lite attribute, if there are ICE
//     candidates, and if all default destinations match an ICE candidate.
//     If not, the answer is processed as normal RFC 3264 procedures.
//   - Selecting the default destination for each component based on local
//     priority of the candidates. Only one candidate per component is put
//     into the answer.
//   - Setting the IP address of the RTP candidate in the c line and the port of
//     the RTP candidate in the m line.
//   - Setting the IP address of the RTCP candidate in the RTCP attribute.
//   - Adding the ice-lite, ice-ufrag and ice-pwd attributes if
//     not already present.
//   - If candidate IP versions are not compatible, the GenerateAnswer
//     method fails.
//
//   When validating an answer, the capabilities manager automatically takes
//   care of:
//
//   - Checking if there is the ice-lite attribute, if there are ICE
//     candidates, and if all default destinations match an ICE candidate.
//     If not, the answer is processed as normal RFC 3264 procedures.
//   - Validating if the received ICE candidate IP versions are compatible
//     with the offered ones. If they are not compatible, the VerifyAnswer
//     method fails.
//
// Location:
//  Define this in PreSdpParserCfg.h.
//
//==============================================================================
#define MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
//  Summary:
//      Enables parsing and serialization of the attribute mptime.
//
//  Description:
//      When defined, the attribute a=mptime is parsed. The value can then be
//      used, modified and serialized.
//
// Location:
//  Define this in PreSdpParserCfg.h.
//
//==============================================================================
#define MXD_SDP_ENABLE_MPTIME


#endif // #if 0 for documentation.

#if defined(MXD_POST_SDPPARSERCFG)
#include "PostSdpParserCfg.h"
#endif

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SDPPARSERCFG_H

