//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CSDPFMTPG7221_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFMTPG7221_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_SDP_G7221_FMTP_ATTRIBUTE_ENABLE_SUPPORT
#endif

#ifdef MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE

#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
#include "SdpParser/CSdpFieldAttributeFmtp.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSdpFmtpG7221
//==============================================================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Description:
//   This class is the implementation of a G.722.1 payload format for the
//   G.722.1 audio streams. It is used to parse the fmtp attribute in SDP.
//
//   The format of this fmtp attribute is based on the format of
//   CSdpFieldAttributeFmtp, but it is more specific in its
//   format-specific-parameters.
//
//  Derived from RFC3047:
//  <PRE>
//      G.722.1-fmtp-attribute     = "fmtp:" format G.722.1-parameters
//      format                     = token
//      G.722.1-parameters         = G.722.1-param *( ";" G.722.1-param )
//      G.722.1-param              = token "=" token
//  </PRE>
//
// Location:
//   SdpParser/CSdpFmtpG7221.h
//
// See Also:
//   CSdpFieldAttributeFmtp
//
//==============================================================================
class CSdpFmtpG7221: public CSdpFieldAttributeFmtp
{
//-- Friend Declarations

//-- New types within class namespace
public:
    //  Description:
    // Contains the possible parameter types in a G722.1 FMTP attribute.
    enum EVideoRenderingSize
    {
        eBIT_RATE = 0,
        eUNKNOWN
    };
protected:
private:

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSdpFmtpG7221();

    // Summary:
    //  Destructor.
    virtual ~CSdpFmtpG7221();

    // Summary:
    //  Copy Constructor.
    CSdpFmtpG7221(IN const CSdpFmtpG7221& rSrc);

    // Summary:
    //  Assignment Operator.
    CSdpFmtpG7221& operator=(IN const CSdpFmtpG7221& rSrc);

    // Summary:
    //  Gets the audio bitrate.
    unsigned int GetBitRate() const;

    // Summary:
    //  Sets the audio bitrate.
    void SetBitRate(unsigned int uBitRate);

    //-- << CSdpFieldAttributeFmtp >>

    // Summary:
    //  Serializes the Fmtp field value in a string and returns its value.
    //  Also used by Serialize to add the format-specific-parameters to the
    //  blob.
    virtual const char* GetValue() const;

    //-- << CSdpParser >>

    // Summary:
    //  Parses the data. Can return any type of EParserResult.
    virtual EParserResult Parse(INOUT const char*& rpszStartPosition,
                                OUT mxt_result& rres);

    // Summary:
    //  Validates and checks the validity of the parsed data.
    virtual bool Validate();

    // Summary:
    //  Resets the data in the parser.
    virtual void Reset();

    // Summary:
    //  Generates a copy of the current object.
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;


//-- Hidden Methods
protected:
private:
    // Summary:
    //  Gets the corresponding rendering size from the string.
    unsigned int GetParameterTypeFromStr(IN const char* pszParameterType);

//-- Hidden Data Members
protected:
private:
    unsigned int m_uBitRate;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//
//   GetBitRate
//
//==============================================================================
//
//  Returns:
//      The rendering size supported.
//
//  Description:
//      Gets the audio bitrate.
//
//==============================================================================
inline unsigned int CSdpFmtpG7221::GetBitRate() const
{
    return m_uBitRate;
}

//==============================================================================
//
//   SetBitrate
//
//==============================================================================
//
//  Parameter:
//      uBitrate:
//          The bit rate supported.
//
//  Description:
//      Sets the audio bitrate.
//
//==============================================================================
inline void CSdpFmtpG7221::SetBitRate(IN unsigned int uBitrate)
{
    m_uBitRate = uBitrate;
}

//==============================================================================
//
//   GenerateCopy
//
//==============================================================================
//
//  Returns:
//      A copy of the current object. Ownership is given.
//
//  Description:
//      Generates a copy of the current object.
//
//==============================================================================
inline GO CSdpFieldAttributeFmtp* CSdpFmtpG7221::GenerateCopy() const
{
    return MX_NEW(CSdpFmtpG7221)(*this);
}

MX_NAMESPACE_END(MXD_GNS)
#endif //-- #ifdef MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE
#endif //-- #ifndef MXG_CSDPFMTPG7221_H
