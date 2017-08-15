//==============================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CSDPFIELDATTRIBUTERTPMAP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTERTPMAP_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

//-- Data Member
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif



//-- Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



//-- Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeRtpmap
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-rtpmap.
//
// Description:
//   This class is an abstraction of an attribute-rtpmap in a SDP packet.
//
//   The parsing of this attribute-rtpmap does not follow the general BMF
//   described for an attribute in RFC 2327.  It follows another BNF
//   described in RFC 2327.
//
//  RFC 2327 BNF:
//  <PRE>
//      attribute-rtpmap    =     "rtpmap:" payload-type encoding-name / clock-rate
//                                [/ encoding-parameters]
//      payload-type        =     space byte-string
//      encoding-name       =     space byte-string
//      clock-rate          =     space byte-string
//      encoding-parameters =     space *(byte-string)
//      byte-string         =     1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
//  Note that the only encoding-parameter accepted right now is an integer.
//
// Location:
//   SdpParser/CSdpFieldAttributeRtpmap.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeRtpmap : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeRtpmap();

    // Summary: Copy Constructor.
    CSdpFieldAttributeRtpmap(IN const CSdpFieldAttributeRtpmap& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeRtpmap();

    // Summary: Comparison Operator.
    bool operator==(IN const CSdpFieldAttributeRtpmap& rFrom) const;

    // Summary: Assignment operator.
    CSdpFieldAttributeRtpmap& operator=(IN const CSdpFieldAttributeRtpmap& rFrom);

    //-- << Accessors >>

    // Summary:
    //   Gets the payload type.
    int32_t     GetPayloadType() const;

    // Summary:
    //   Gets the encoding name.
    const char* GetEncodingName() const;

    // Summary:
    //   Gets the compression algorithm.
    CSdpParser::ERtpCompressionAlgorithm  GetEncoding() const;

    // Summary:
    //   Gets the clock rate.
    int32_t     GetClockRate() const;

    // Summary:
    //   Gets the encoding parameters.
    int32_t     GetEncodingParameters() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    //-- << Mutators >>

    // Summary:
    //   Sets the payload type.
    void SetPayloadType(IN int32_t nPayloadType);

    // Summary:
    //   Sets the encoding name.
    void SetEncodingName(IN const char* pszEncodingName);

    // Summary:
    //   Sets the compression algorithm.
    void SetEncoding(IN CSdpParser::ERtpCompressionAlgorithm eEncoding);

    // Summary:
    //   Sets the clock rate.
    void SetClockRate(IN int32_t nClockRate);

    // Summary:
    //   Sets the encoding parameters.
    void SetEncodingParameters(IN int32_t nEncodingParameters);


    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all the data members.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    void UpdateEncodingFromString();
    void UpdateEncodingFromEnum();

private:

//-- Hidden Data Members
protected:
private:
    //JM 99-05-13: For now, we consider that <encoding parameters> is only
    // one value, and that this value is a integer. But maybe later it should
    // be a list of strings...
    int32_t   m_nPayloadType;
    CString m_strEncodingName;
    int32_t   m_nClockRate;
    int32_t   m_nEncodingParameters;
    CSdpParser::ERtpCompressionAlgorithm m_eEncoding;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetPayloadType
//==
//==============================================================================
//
//  Returns:
//      The payload type
//
//  Description:
//      Returns the rtp map attribute payload type.
//
//==============================================================================
inline int32_t CSdpFieldAttributeRtpmap::GetPayloadType() const
{
    return m_nPayloadType;
}

//==============================================================================
//==
//==  GetEncodingName
//==
//==============================================================================
//
//  Returns:
//      The encoding name.
//
//  Description:
//      Returns the rtp map attribute encoding name.
//
//==============================================================================
inline const char* CSdpFieldAttributeRtpmap::GetEncodingName() const
{
    return m_strEncodingName.CStr();
}

//==============================================================================
//==
//==  GetClockRate
//==
//==============================================================================
//
//  Returns:
//      The clock rate.
//
//  Description:
//      Returns the rtp map attribute clock rate.
//
//==============================================================================
inline int32_t CSdpFieldAttributeRtpmap::GetClockRate() const
{
    return m_nClockRate;
}

//==============================================================================
//==
//==  GetEncodingParameters
//==
//==============================================================================
//
//  Returns:
//      The encoding parameters
//
//  Description:
//      Returns the rtp map attribute encoding parameters.
//
//==============================================================================
inline int32_t CSdpFieldAttributeRtpmap::GetEncodingParameters() const
{
    return m_nEncodingParameters;
}

//==============================================================================
//==
//==  GetEncoding
//==
//==============================================================================
//
//  Returns:
//      The compression algorithm used.
//
//  Description:
//      Returns the rtp map attribute ERtpCompressionAlgorithm used.
//
//==============================================================================
inline CSdpParser::ERtpCompressionAlgorithm CSdpFieldAttributeRtpmap::GetEncoding() const
{
    return(m_eEncoding);
}

//==============================================================================
//==
//==  SetPayloadType
//==
//==============================================================================
//
//  Parameters:
//      nPayloadType:
//       The payload type to set.
//
//  Description:
//      Sets the rtp map attribute payload type.
//
//==============================================================================
inline void CSdpFieldAttributeRtpmap::SetPayloadType(IN int32_t nPayloadType)
{
    m_nPayloadType = nPayloadType;
}

//==============================================================================
//==
//==  SetEncodingName
//==
//==============================================================================
//
//  Parameters:
//      pszEncodingName:
//       The encoding name to set.
//
//  Description:
//      Sets the rtp map attribute encoding name and compression algorithm.
//
//  See also :
//    SetEncoding
//
//==============================================================================
inline void CSdpFieldAttributeRtpmap::SetEncodingName(IN const char* pszEncodingName)
{
    m_strEncodingName = pszEncodingName;
    m_eEncoding = GetRtpCompressionAlgorithmIdFromStr(pszEncodingName);
}

//==============================================================================
//==
//==  SetClockRate
//==
//==============================================================================
//
//  Parameters:
//      nClockRate:
//       The clock rate to set.
//
//  Description:
//      Sets the rtp map attribute clock rate.
//
//==============================================================================
inline void CSdpFieldAttributeRtpmap::SetClockRate(IN int32_t nClockRate)
{
    m_nClockRate = nClockRate;
}

//==============================================================================
//==
//==  SetEncodingParameters
//==
//==============================================================================
//
//  Parameters:
//      nEncodingParameters:
//       The encoding parameters to set.
//
//  Description:
//      Sets the rtp map attribute encoding parameters.
//
//==============================================================================
inline void
CSdpFieldAttributeRtpmap::SetEncodingParameters(IN int32_t nEncodingParameters)
{
    m_nEncodingParameters = nEncodingParameters;
}

//==============================================================================
//==
//==  SetEncoding
//==
//==============================================================================
//
//  Parameters:
//      eEncoding:
//       The compression algorithm to set.
//
//  Description:
//      Sets the rtp map attribute compression algorithm and encoding name.
//
//  See also :
//    SetEncodingName
//
//==============================================================================
inline
void CSdpFieldAttributeRtpmap::SetEncoding(IN CSdpParser::ERtpCompressionAlgorithm eEncoding)
{
    m_eEncoding = eEncoding;
    m_strEncodingName = GetRtpCompressionAlgorithmStrFromId(eEncoding);
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTERTPMAP_H
