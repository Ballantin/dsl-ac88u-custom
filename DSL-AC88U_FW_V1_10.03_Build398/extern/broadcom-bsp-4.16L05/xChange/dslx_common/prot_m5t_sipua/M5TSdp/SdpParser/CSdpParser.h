//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_CSDPPARSER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPPARSER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_SDP_ENABLE_SRTP_SUPPORT
#endif

// Data Member

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

const char          cSDP_CR             = '\r';
const char          cSDP_LF             = '\n';
const char          cSDP_SPACE          = ' ';
const char          cSDP_SLASH          = '/';
const char          cSDP_NUL            = '\0';
const char          cSDP_HYPHEN         = '-';
const char* const   szSDP_CRLF          = "\r\n";

const int32_t       nMAX_TOKEN_SIZE     = 512;

const char* const pszT38_MEDIA_FORMAT = "t38";
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//==  Class: CSdpParser
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements the abstract base class for all the other SDP Parser
//   classes.
//
// Description:
//   This is the abstract base class for all the other SDP Parser classes.
//   It contains definitions of pure virtual functions to be defined in the
//   concrete classes. It also contains some useful parsing functions.
//
//   For every field, the parser follows what is described in RFC 2327.
//   The enumeration EParserType describes in more details what is parsed.
//
//   SDP session descriptions are entirely textual using the ISO 10646
//   character set in UTF-8 encoding. SDP field names and attribute names
//   use only the US-ASCII subset of UTF-8, but textual fields and
//   attribute values may use the full ISO 10646 character set.
//
//   The fields in a SDP Packet contain one or several tokens which
//   this base class offers methods to parse properly.
//
//   <PRE>
//      CSdpParser::GetLine
//      CSdpParser::GetToken
//      CSdpParser::GetSubToken
//
//   </PRE>
//
//   These methods and the Parse method of every Child return a type of the
//   enumeration EParserResult.
//
// Location:
//   SdpParser/CSdpParser.h
//
// See Also:
//
//==============================================================================
class CSdpParser
{
    // Friend class used by CSdpParser
    friend class CSdpCapabilitiesMgr;

// Enumerations
public:
//M5T_INTERNAL_USE_BEGIN
    // Return type for most parsing methods
    enum EParserResult
    {
        // SDP is based on lines (separated by EOLs), tokens (separated by
        // spaces, which we call EOT - End Of Token) and subtokens (usually
        // separated by slashes).

        eERROR,         // ERROR: Could not parse the requested data.
        eOK_CONTINUE,   // SUCCESS: The data was parsed. 'data pointer' now
                        // points to the next character that needs to be parsed
                        // for this construct, i.e. to the next token or
                        // subtoken.
        eOK_EOT,        // SUCCESS: The data was parsed and an EOT marker was
                        // found after the parsed data. 'data pointer' now
                        // points to the beginning of the new token.
        eOK_EOL,        // SUCCESS: The data was parsed and an EOL marker was
                        // found after the parsed data. 'data pointer' now
                        // points to the beginning of the new line.
        eOK_NULL,       // SUCCESS: The data was parsed and a NULL character
                        // was found after the parsed data. 'data pointer'
                        // points to this NULL character.
    };

    // Parser types, one for each possible type of line ("v=", "o=", etc.)
    enum EParserType
    {
        ePROTOCOL_VERSION,      // "v="
        eORIGIN,                // "o="
        eSESSION_NAME,          // "s="
        eSESSION_MEDIA_INFO,    // "i="
        eURI,                   // "u="
        eEMAIL_ADDRESS,         // "e="
        ePHONE_NUMBER,          // "p="
        eCONNECTION_DATA,       // "c="
        eBANDWIDTH,             // "b="
        eTIME,                  // "t="
        eREPEAT_TIME,           // "r="
        eTIME_ZONE,             // "z="
        eENCRYPTION_KEY,        // "k="
        eATTRIBUTE,             // "a="
        eMEDIA_ANNOUNCEMENT,    // "m="
        eUNKNOWN_PARSER_TYPE
    };

    // Common network types identifiers
    enum ENetworkType
    {
        eIN = 0,
        // Insert new types here, and don't forget to update
        // ms_apszNetworkTypeMap
        eUNKNOWN_NETWORK_TYPE   // NEED to be the last element
    };

    static const char* const ms_apszNetworkTypeMap[];

    // Common address type identifiers
    enum EAddressType
    {
        eIP4 = 0,
        eIP6,
        // Insert new types here, and don't forget to update
        // ms_apszAddressTypeMap
        eUNKNOWN_ADDRESS_TYPE   // NEED to be the last element
    };

    static const char* const ms_apszAddressTypeMap[];

    // Common media type identifiers
    enum EMediaType
    {
        eAUDIO = 0,
        eVIDEO,
        eAPPLICATION,
        eDATA,
        eCONTROL,
        eIMAGE,

        // draft-ietf-simple-message-sessions-10.
        eMESSAGE,
        // Insert new types here, and don't forget to update
        // ms_apszMediaTypeMap
 // BRCM_CUSTOM - [add] Adding media type
        eTEXT,
 // BRCM_CUSTOM - [end] Adding media type
        eUNKNOWN_MEDIA_TYPE     // NEED to be the last element
    };

    static const char* const ms_apszMediaTypeMap[];

    // Common transport protocol identifiers
    enum ETransportProtocol
    {
        eRTPAVP = 0,
        eUDP,
        eUDPTL,

        // "eudptl" maps to upper case "UDPTL", added for interop.
        eudptl,

        eTCP,
        eTCPTL,
        eRTPSAVP,

        // From draft-rosenberg-simple-message-session-00.
        eSIPTCP,
        eSIPUDP,
        eSIPTLS,

        // Insert new protocols here, and don't forget to update
        // ms_apszTransportProtocolMap
        eUNKNOWN_TRANSPORT_PROTOCOL     // NEED to be the last element
    };

    static const char* const ms_apszTransportProtocolMap[];

    // Recognized attribute types identifiers
    enum EAttributeType
    {
        eRTPMAP,
        ePTIME,
        eSENDONLY,
        eRECVONLY,
        eSENDRECV,
        eINACTIVE,
        eT38_FAX_FILL_BIT_REMOVAL,
        eT38_MAX_BIT_RATE,
        eT38_FAX_MAX_DATAGRAM,
        eT38_FAX_UDP_EC,
        eT38_FAX_MAX_BUFFER,
        eT38_FAX_RATE_MANAGEMENT,
        eTRANSCODING,  // TDB:  delete this later on.  No longer part of T38
        eT38_TRANSCODING_MMR,
        eT38_TRANSCODING_JBIG,
        eT38_FAX_VERSION,
        eFMTP,

#if defined (MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT)
        eSILENCESUPP,
#endif

        eCRYPTO,
        eKEYMGMT,
        eRTCP,
        eMID,
        eGROUP,
// BRCM_CUSTOM - [add] Precondition support
 #if defined(MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT)
// BRCM_CUSTOM - [end] Precondition support
        ePRECOND_DES,
        ePRECOND_CURR,
        ePRECOND_CONF,
// BRCM_CUSTOM - [add] Precondition support
#endif // MXD_BRCM_SDP_ENABLE_PRECOND_SUPPORT
// BRCM_CUSTOM - [end] Precondition support
        eRTCP_MUX,
#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
        eICE_LITE,
        eICE_UFRAG,
        eICE_PWD,
        eICE_MISMATCH,
        eICE_OPTIONS,
        eICE_CANDIDATE,
        eICE_REMOTE_CANDIDATES,
#endif
#if defined(MXD_SDP_ENABLE_MPTIME)
        eMPTIME,
#endif
        // Insert new types here, and don't forget to update
        // ms_apszAttributeTypeMap

        eUNKNOWN_ATTRIBUTE_TYPE     // NEED to be the last element
    };

    static const char* const ms_apszAttributeTypeMap[];

    // Recognized compression algorithms. This enum also serves as index for
    // s_aszRtpCompressionAlgorithmMap.
    enum ERtpCompressionAlgorithm
    {
        // These are the static audio payload types defined in the Audio/Video
        // Profile for RTP (draft-ietf-avt-rfc3555bis-00.txt).
        ePCMU,
        eG72632,
        eGSM,
        eG723,
        eDVI4_8000,
        eDVI4_16000,
        eLPC,
        ePCMA,
        eG722,
        eL16_2,
        eL16_1,
        eQCELP,
        eCN,
        eMPA,
        eG728,
        eDVI4_11025,
        eDVI4_22050,
        eG729,
        eG72640,
        eG72624,
        eG72616,
        eG729D,
        eG729E,
        eL8,

        // From RFC2190.
        eRED,

        // From draft-ietf-avt-rfc3555bis-00.txt.
        eVDVI,

// BRCM_CUSTOM - [mod] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
        // From RFC2190
        eH263,
        eH264,
        eMPEG4,
#endif // MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT
// BRCM_CUSTOM - [end] Video support

        // From RFC4040.
        eCLEARMODE,

        // RFC2833.
        eTELEPHONE_EVENT,

        // From draft-ietf-avt-rtp-bv-04.txt.
        eBV16,
        eBV32,

        // From RFC3047.
#if defined(MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE)
        eG7221,
#endif

        // From RFC3952.
        eILBC,

        // From RFC4573.
        eH224,

        // From RFC4867.
        eAMR,
        eAMR_WB,

// BRCM_CUSTOM - [mod] GSM EFR Support
        // From RFC3551.
        eEFR,
// BRCM_CUSTOM - [end] GSM EFR Support
        
        // From unknown RFC but supported by GIPS voice engine.
        eEG711U,
        eEG711A,
        
#if defined(MXD_SDP_ENABLE_ISAC_FMTP_ATTRIBUTE)
        // From http://tools.ietf.org/html/draft-legrand-rtp-isac-02, this is a GIPS
        // proprietary codec.
        eISAC,
#endif

#if defined MXD_SDP_H264_FMTP_ATTRIBUTE_ENABLE_SUPPORT
        // From RFC 3984,
        eH264,
#endif
#if defined MXD_SDP_H263_FMTP_ATTRIBUTE_ENABLE_SUPPORT
        // From draft-ietf-avt-rfc2429-bis-09.txt
        eH263_2000,
        eH263_1998,
#endif
#if defined MXD_SDP_MP4V_ES_FMTP_ATTRIBUTE_ENABLE_SUPPORT
        // RFC 3016
        eMP4V_ES,
#endif
        
        MXD_SDP_USER_EXTEND_COMPRESSION_ALGORITHM_ENUM_PRIVATE

        // Insert new types here and update ms_apszRtpCompressionAlgorithmMap.
        eUNKNOWN_RTP_COMPRESSION_ALGORITHM // NEED to be the last element
    };

    enum ERtpStreamDirection
    {
        eINCOMING,
        eOUTGOING
    };

    static const char* const ms_apszRtpCompressionAlgorithmMap[];

    enum EGroupingMethod
    {
        eANAT
    };

    static const char* const ms_apszGroupingMethodMap[];

    // <TITLE RFC3312 Precondition attributes>
    //
    // Description:
    //  Define which precondition attributes to use.
    enum EPreCondField
    {
        ePRE_COND_FIELD_DES,
        ePRE_COND_FIELD_CURR,
        ePRE_COND_FIELD_CONF
    };

    // <TITLE RFC3312 Precondition tags>
    //
    // Description:
    //  Tags used in precondition attribute field.
    enum EPreCondStrengthTag
    {
        ePRE_COND_STRENGTH_MANDATORY,
        ePRE_COND_STRENGTH_OPTIONAL,
        ePRE_COND_STRENGTH_NONE,
        ePRE_COND_STRENGTH_FAILURE,
        ePRE_COND_STRENGTH_UNKNOWN,
        ePRE_COND_STRENGTH_INVALID
    };
    // <COMBINE EPreCondStrengthTag>
    enum EPreCondStatusTag
    {
        ePRE_COND_STATUS_E2E,
        ePRE_COND_STATUS_LOCAL,
        ePRE_COND_STATUS_REMOTE,
        ePRE_COND_STATUS_INVALID
    };
    // <COMBINE EPreCondStrengthTag>
    enum EPreCondDirectionTag
    {
        ePRE_COND_DIRECTION_NONE,
        ePRE_COND_DIRECTION_SEND,
        ePRE_COND_DIRECTION_RECV,
        ePRE_COND_DIRECTION_SENDRECV,
        ePRE_COND_DIRECTION_INVALID
    };

    static const char* const ms_apszPreCondStrengthTagMap[];
    static const char* const ms_apszPreCondStatusTagMap[];
    static const char* const ms_apszPreCondConditionTagMap[];
//M5T_INTERNAL_USE_END

// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CSdpParser();

    // Summary: Copy constructor.
    CSdpParser(IN const CSdpParser& rFrom);

    // Summary: Destructor.
    virtual ~CSdpParser();

    // Summary: Assignment operator.
    CSdpParser& operator=(IN const CSdpParser& rFrom);

    //-- << Virtual functions >>

    // Summary:
    // Parses the parameters list beginning at rpszStartPosition. Can return any
    // type of EParserResult.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres) = 0;

    // Summary:
    // Validates the parsed data.
    virtual bool Validate() = 0;

    // Summary:
    // Resets the data in the parser.
    virtual void Reset();

    // Summary:
    // Returns true if the data was parsed successfully.
    bool IsValid() const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:

    // Summary:
    // Retrieves the line (delimited by CRLF) found at rpszPosition.
    EParserResult GetLine(IN const char*& rpszPosition,
                          IN uint32_t nBufferSize,
                          OUT char* pszBuffer) const;

    // Summary:
    // Retrieves the line (delimited by CRLF) found at rpszPosition.
    EParserResult GetLine(IN const char*& rpszPosition,
                          OUT CString& rString) const;

    // Summary:
    // Retrieves the token (delimited by SPACE) found at rpszPosition.
    EParserResult GetToken(IN const char*& rpszPosition,
                           IN uint32_t nBufferSize,
                           OUT char* pszBuffer) const;

    // Summary:
    // Retrieves the subtoken (delimited by cSeparator) found at rpszPosition.
    EParserResult GetSubToken(IN const char*& rpszPosition,
                              IN char cSeparator,
                              IN uint32_t nBufferSize,
                              OUT char* pszBuffer) const;

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  Retrieves a token made of ice-char.
    EParserResult GetIceCharToken(INOUT const char*& rpszPosition,
                                  IN uint32_t nBufferSize,
                                  OUT char* pszBuffer) const;

    // Summary:
    //  Checks that the string contains only ice-char.
    bool IsIceCharToken(IN const char* pszToken) const;

#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)


    // Summary:
    // Removes one specified character.
    bool RemoveChar(IN const char*& rpszPosition, IN char ch) const;

    // Summary:
    // Removes all EOL markers.
    bool RemoveEol(IN const char*& rpszPosition) const;

    // Summary:
    //  Skips up to EOL.
    EParserResult SkipToEol(IN const char*& rpszPosition);

    // Summary:
    // Finds the next type of data to be parsed.
    EParserResult GetNextParserType(IN const char*& rpszPosition,
                                    OUT EParserType& eType,
                                    OUT mxt_result& rres) const;

    // Summary:
    // Finds the next type of attribute to be parsed.
    EParserResult GetNextAttributeType(IN const char*& rpszPosition,
                                       OUT EAttributeType& eType,
                                       OUT char* pszAttributeName) const;

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
    // Summary:
    // Finds the next type of fmtp attribute to be parsed.
    //------------------------------------------------
    EParserResult GetNextFmtpAttributeType(IN const char*&  rpszPosition,
                                           IN const char*   pszFmtpParamNames[],
                                           OUT int*         paramIndex,
                                           OUT char*        pszFmtpValue) const;
#endif // MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT
// BRCM_CUSTOM - [end] Video support

    //-- << Character category verification >>
    bool IsByteStringChar(IN char ch) const;
    bool IsNumeric(IN const char* pszToken) const;
    bool IsAlphaNumeric(IN const char* pszToken) const;
    bool IsToken(IN const char* pszToken) const;
#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
    // Summary:
    //  Tells if the character is an ice-char.
    bool IsIceChar(IN char cChar) const;
#endif

public:    
    //-- << Functions to convert a string into a specified enum value >>
    ENetworkType    GetNetworkTypeIdFromStr(IN const char* pszNetworkType) const;
    EAddressType    GetAddressTypeIdFromStr(IN const char* pszAddressType) const;
    EMediaType      GetMediaTypeIdFromStr(IN const char* pszMediaType) const;
    ETransportProtocol GetTransportProtoIdFromStr(IN const char* pszProto) const;
    EAttributeType  GetAttributeTypeIdFromStr(IN const char* pszAttributeType) const;
    ERtpCompressionAlgorithm GetRtpCompressionAlgorithmIdFromStr(IN const char* pszRtpCompressionAlgorithm) const;
    EPreCondStrengthTag GetPreCondStrengthIdFromStr(IN const char* pszStrength) const;
    EPreCondStatusTag GetPreCondStatusIdFromStr(IN const char* pszStatus) const;
    EPreCondDirectionTag GetPreCondDirectionIdFromStr(IN const char* pszDirection) const;

    //-- << Functions to convert an enum value into its corresponding string >>
    // Summary:
    //  Returns the string representing the network type ID.
    static const char* GetNetworkTypeStrFromId(IN ENetworkType eNetworkType);

    // Summary:
    //  Returns the string representing the address type ID.
    static const char* GetAddressTypeStrFromId(IN EAddressType eAddressType);

    // Summary:
    //  Returns the string representing the media type ID.
    static const char* GetMediaTypeStrFromId(IN EMediaType eMediaType);

    // Summary:
    //  Returns the string representing the transport protocol ID.
    static const char* GetTransportProtoStrFromId(IN ETransportProtocol eProto);

    // Summary:
    //  Returns the string representing the attribute type ID.
    static const char* GetAttributeTypeStrFromId(IN EAttributeType eAttributeType);

    // Summary:
    //  Returns the string representing the compression algorithm ID.
    static const char* GetRtpCompressionAlgorithmStrFromId(IN ERtpCompressionAlgorithm eRtpCompressionAlgorithm);

    const char* GetPreCondStrengthStrFromId(IN EPreCondStrengthTag ePreCondStrength) const;
    const char* GetPreCondStatusStrFromId(IN EPreCondStatusTag ePreCondStatus) const;
    const char* GetPreCondDirectionStrFromId(IN EPreCondDirectionTag ePreCondDirection) const;

protected:    
    // Summary:
    // Parses a single token and stores the result in a string.
    EParserResult ParseSingleTokenStr(IN const char*& rpszStartPosition,
                                      OUT mxt_result& rres,
                                      OUT CString& strRes);

    // Summary:
    // Parses a single token and stores the result in a int.
    EParserResult ParseSingleTokenInt(IN const char*& rpszStartPosition,
                                      OUT mxt_result& rres,
                                      OUT int32_t& nRes,
                                      IN bool bMustBeZero = false);

    // Summary:
    // Parses a line and stores the result in a string.
    EParserResult ParseLine(IN const char*& rpszStartPosition,
                            INOUT CString& strOutput,
                            OUT mxt_result& rres);

    // Summary:
    // Parses a line and appends the string in a vector.
    EParserResult ParseLine(IN const char*& rpszStartPosition,
                            INOUT CVector<CString>& strOutput,
                            OUT mxt_result& rres);

private:

// Hidden Data Members
protected:
    // Validity of the parsed data
    bool m_bIsValid;

private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================


//==============================================================================
//==
//==  IsValid
//==
//==============================================================================
//
//  Returns:
//    True if the data was parsed successfully.
//
//  Description:
//    Returns whether or not the parsing succeeded.
//
//==============================================================================
inline bool CSdpParser::IsValid() const
{
    return m_bIsValid;
}

//==============================================================================
//==
//==  IsByteStringChar
//==
//==============================================================================
//
//  Returns:
//    True if the byte is a string character.
//
//  Description:
//    Determines if a specified byte is a string character.
//
//==============================================================================
inline bool CSdpParser::IsByteStringChar(IN char ch) const
{
    return (ch != cSDP_NUL && ch != cSDP_CR && ch != cSDP_LF);
}

#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)
//==============================================================================
//==
//==  IsIceChar
//==
//==============================================================================
//
//  Parameters:
//      cChar:
//       Character to verify.
//
//  Returns:
//      - true if the character is an ice-char.
//      - false otherwise.
//
//  Description:
//      Returns true if the cChar is an ice-char character.
//
//  From draft-ietf-mmusic-ice-19:
//  <PRE>
//      ice-char              = ALPHA / DIGIT / "+" / "/"
//  </PRE>
//
//==============================================================================
inline bool CSdpParser::IsIceChar(IN char cChar) const
{
    return (isalnum(cChar) || cChar == '+' || cChar == '/');
}
#endif //#if defined(MXD_SDP_ICE_ENABLE_SUPPORT)

//==============================================================================
//==
//==  GetNetworkTypeStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eNetworkType:
//      The network type.
//
//  Returns:
//      The string representing the specified network type.
//
//  Description:
//      Returns the string representing the specified network type ID.
//
//==============================================================================
inline const char* CSdpParser::GetNetworkTypeStrFromId(IN ENetworkType eNetworkType)
{
    MX_ASSERT (eNetworkType <= eUNKNOWN_NETWORK_TYPE);
    return ms_apszNetworkTypeMap[eNetworkType];
}

//==============================================================================
//==
//==  GetAddressTypeStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eAddressType:
//      The address type.
//
//  Returns:
//      The string representing the specified address type.
//
//  Description:
//      Returns the string representing the specified address type ID.
//
//==============================================================================
inline const char* CSdpParser::GetAddressTypeStrFromId(IN EAddressType eAddressType)
{
    MX_ASSERT (eAddressType <= eUNKNOWN_ADDRESS_TYPE);
    return ms_apszAddressTypeMap[eAddressType];
}

//==============================================================================
//==
//==  GetMediaTypeStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eMediaType:
//      The media type.
//
//  Returns:
//      The string representing the specified media type.
//
//  Description:
//      Returns the string representing the specified media type ID.
//
//==============================================================================
inline const char* CSdpParser::GetMediaTypeStrFromId(IN EMediaType eMediaType)
{
    MX_ASSERT (eMediaType <= eUNKNOWN_MEDIA_TYPE);
    return ms_apszMediaTypeMap[eMediaType];
}

//==============================================================================
//==
//==  GetTransportProtoStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eProto:
//      The transport protocol.
//
//  Returns:
//      The string representing the specified transport protocol.
//
//  Description:
//      Returns the string representing the specified transport protocol ID.
//
//==============================================================================
inline const char* CSdpParser::GetTransportProtoStrFromId(IN ETransportProtocol eProto)
{
    MX_ASSERT (eProto <= eUNKNOWN_TRANSPORT_PROTOCOL);
    return ms_apszTransportProtocolMap[eProto];
}

//==============================================================================
//==
//==  GetAttributeTypeStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eAttributeType:
//      The attribute type.
//
//  Returns:
//      The string representing the specified attribute type.
//
//  Description:
//      Returns the string representing the specified attribute type ID.
//
//==============================================================================
inline const char* CSdpParser::GetAttributeTypeStrFromId(IN EAttributeType eAttributeType)
{
    MX_ASSERT (eAttributeType <= eUNKNOWN_ATTRIBUTE_TYPE);
    return ms_apszAttributeTypeMap[eAttributeType];
}

//==============================================================================
//==
//==  GetRtpCompressionAlgorithmStrFromId
//==
//==============================================================================
//
//  Parameters:
//    eRtpCompressionAlgorithm:
//      The compression algorithm.
//
//  Returns:
//      The string representing the specified compression algorithm.
//
//  Description:
//      Returns the string representing the specified compression algorithm ID.
//
//==============================================================================
inline const char* CSdpParser::GetRtpCompressionAlgorithmStrFromId(IN ERtpCompressionAlgorithm eRtpCompressionAlgorithm)
{
    MX_ASSERT (eRtpCompressionAlgorithm <= eUNKNOWN_RTP_COMPRESSION_ALGORITHM);

    return ms_apszRtpCompressionAlgorithmMap[eRtpCompressionAlgorithm];
}

//==============================================================================
//==
//==  GetPreCondStrengthStrFromId
//==
//==============================================================================
//
//  Parameters:
//    ePreCondStrength:
//      The precondition Strength tag.
//
//  Returns:
//      The string representing the specified strength ID.
//
//  Description:
//      Returns the string representing the specified strength ID.
//
//==============================================================================
inline const char* CSdpParser::GetPreCondStrengthStrFromId(IN EPreCondStrengthTag ePreCondStrength) const
{
    MX_ASSERT(ePreCondStrength <= ePRE_COND_STRENGTH_INVALID);
    return ms_apszPreCondStrengthTagMap[ePreCondStrength];
}

//==============================================================================
//==
//==  GetPreCondStatusStrFromId
//==
//==============================================================================
//
//  Parameters:
//    ePreCondStatus:
//      The precondition Status tag.
//
//  Returns:
//      The string representing the specified status ID.
//
//  Description:
//      Returns the string representing the specified status ID.
//
//==============================================================================
inline const char* CSdpParser::GetPreCondStatusStrFromId(IN EPreCondStatusTag ePreCondStatus) const
{
    MX_ASSERT(ePreCondStatus <= ePRE_COND_STATUS_INVALID);
    return ms_apszPreCondStatusTagMap[ePreCondStatus];
}

//==============================================================================
//==
//==  GetPreCondDirectionStrFromId
//==
//==============================================================================
//
//  Parameters:
//    ePreCondDirection:
//      The precondition Direction tag.
//
//  Returns:
//      The string representing the specified direction ID.
//
//  Description:
//      Returns the string representing the specified direction ID.
//
//==============================================================================
inline const char* CSdpParser::GetPreCondDirectionStrFromId(IN EPreCondDirectionTag ePreCondDirection) const
{
    MX_ASSERT(ePreCondDirection <= ePRE_COND_DIRECTION_INVALID);
    return ms_apszPreCondConditionTagMap[ePreCondDirection];
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPPARSER_H
