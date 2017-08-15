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
#ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEFMTP_H

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

#ifndef MXG_CSDPFIELDMEDIAANNOUNCEMENT_H
#include "SdpParser/CSdpFieldMediaAnnouncement.h"
#endif

#ifndef MXG_MXSTRINGFORMAT_H
#include "Basic/MxStringFormat.h" // MxSnprintf
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
//==  Class: CSdpFieldAttributeFmtp
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-fmtp.
//
// Description:
//   This class is an abstraction of an attribute-fmtp in a SDP packet.
//
//   The parsing of this attribute-fmtp does not follow the general BMF
//   described for an attribute in RFC 2327. A restriction is put in the
//   RFC on the format. It must be a format found in the media description.
//   Hence, the format must be a token since the format in the media description
//   are tokens.
//
//  <PRE>
//      attribute-fmtp             = "fmtp:" format format-specific-parameters
//      format                     = token
//      format-specific-parameters = *(byte-string)
//      byte-string                = 1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeFmtp.h
//
//==============================================================================
class CSdpFieldAttributeFmtp : public CSdpParser
{
//-- Published Interface
public:
    // Invalid media format returned value in case of error in GetMediaFormat.
    static const uint32_t uINVALID_MEDIA_FORMAT;

    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CSdpFieldAttributeFmtp();

    // Summary: Copy constructor.
    CSdpFieldAttributeFmtp(IN const CSdpFieldAttributeFmtp& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeFmtp();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributeFmtp& rFrom) const;

    // Summary:
    //   Assignment Operator.
    CSdpFieldAttributeFmtp& operator=(IN const CSdpFieldAttributeFmtp& rFrom);

    // Description:
    //   Fmtp attribute types (format-specific-parameters pattern implemented
    //   by the object).
    enum EFmtpType
    {
        // Description:
        //   Telephone event pattern.
        eFMTP_TYPE_TEL_EVENT,

        // Description:
        //   Redundancy pattern.
        eFMTP_TYPE_RED,


#if defined (MXD_SDP_ENABLE_G7221_FMTP_ATTRIBUTE)
        // Description:
        //   G.722.1 pattern.
        eFMTP_TYPE_G7221,
#endif

#if defined (MXD_SDP_ENABLE_AMR_FMTP_ATTRIBUTE)
        // Description:
        //   AMR pattern.
        eFMTP_TYPE_AMR,
        // Description:
        //   AMR Wideband pattern.
        eFMTP_TYPE_AMR_WB,
#endif

#if defined(MXD_SDP_ENABLE_ISAC_FMTP_ATTRIBUTE)
        // Description:
        //   iSAC pattern.
        eFMTP_TYPE_ISAC,
#endif

#if defined(MXD_SDP_ENABLE_ILBC_FMTP_ATTRIBUTE)
        // Description:
        //   iLBC pattern.
        eFMTP_TYPE_ILBC,
#endif

#if defined (MXD_SDP_H264_FMTP_ATTRIBUTE_ENABLE_SUPPORT)
        // Description:
        //   H.264 pattern.
        eFTMP_TYPE_H264,
#endif
#if defined (MXD_SDP_H263_FMTP_ATTRIBUTE_ENABLE_SUPPORT)
        // Description:
        //   H.263 pattern.
        eFTMP_TYPE_H263,
#endif
#if defined (MXD_SDP_MP4V_ES_FMTP_ATTRIBUTE_ENABLE_SUPPORT)
        // Description:
        //   MPEG-4 visual attributes pattern.
        eFTMP_TYPE_MP4V_ES,
#endif

// BRCM_CUSTOM - [add] Video support
#if defined(MXD_BRCM_SDP_ENABLE_VIDEO_SUPPORT)
        eFMTP_TYPE_H263,
        eFMTP_TYPE_H264,
#endif
// BRCM_CUSTOM - [end] Video support

        // Description:
        //   Unknown pattern.
        eFMTP_TYPE_UNKNOWN
    };

    // Summary:
    //  Generates a copy of the current object.
    virtual GO CSdpFieldAttributeFmtp* GenerateCopy() const;

    //-- Get functions

    // Summary:
    //  Returns the media format of the Fmtp field attribute as a string.
    const char* GetFormat() const;

    // Summary:
    //  Returns the media format of the Fmtp field attribute.
    uint32_t GetMediaFormat() const;

    // Summary:
    //  Serializes the Fmtp field value in m_strValue and returns its value.
    //  Also used by Serialize to add the format-specific-parameters to the
    //  blob.
    virtual const char* GetValue() const;

    // Summary:
    //  Gives the Fmtp type for which the format-specific-parameters pattern
    //  was implemented by the object. For the CSdpFieldAttributeFmtp,
    //  eFMTP_TYPE_UNKNOWN is returned.
    EFmtpType GetFmtpType() const;

    // Summary:
    //  Appends this object into the blob. Also adds a CRLF.
    void        Serialize(INOUT CBlob& rBlob) const;

    //-- Set functions
    // Summary:
    //  Sets the media format of the Fmtp field attribute.
    void SetFormat(IN const char* pszFormat);

    // Summary:
    //  Sets the media format of the Fmtp field attribute.
    void SetMediaFormat( IN uint32_t uFormat );

    // Summary:
    //  Sets the value of the Fmtp field attribute to the string.
    virtual void SetValue(IN const char* pszValue);

    //-- Virtual functions

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

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
    // Summary:
    //  Validate all the specific information to this class, used as a base
    //  class. This method should be used in overriden Validate method in
    //  child classes. This method does NOT set m_bIsValid.
    bool ValidateWithoutValue() const;
private:

// Hidden Data Members
protected:
    // This is the serialized format-specific-parameters of the fmtp field
    // attribute. The value is mutable because it must be updated in GetValue
    // (to reduce processing).
    mutable CString m_strValue;

    // This is the object type. The child classes must initiate the value in
    // their constructor to the compression algorithm they were implemented
    // for.
    CSdpFieldAttributeFmtp::EFmtpType m_eFmtpType;

    // This is the media format of the fmtp field attribute.
    CString m_strMediaFormat;
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetValue
//==
//==============================================================================
//
//  Returns:
//      The format-specific-parameters of the fmtp field attribute.
//
//  Description:
//      Returns the format-specific-parameters into a string.
//
//==============================================================================
inline const char* CSdpFieldAttributeFmtp::GetValue() const
{
    return m_strValue.CStr();
}

//==============================================================================
//==
//==  GetFmtpType
//==
//==============================================================================
//
//  Returns:
//      The compression algorithm for this class.
//
//      eFMTP_TYPE_UNKNOWN if the class is the basic class.
//
//  Description:
//      Returns the compression algorithm for which the
//      format-specific-parameters pattern was implemented by the class.
//
//==============================================================================
inline CSdpFieldAttributeFmtp::EFmtpType
    CSdpFieldAttributeFmtp::GetFmtpType() const
{
    return m_eFmtpType;
}

//==============================================================================
//==
//==  SetFormat
//==
//==============================================================================
//
//  Parameters:
//    pszFormat:
//      The media format of the fmtp field attribute.
//
//  Description:
//      Sets the media format of the fmtp field attribute from a string.
//
//      WARNING : this method does NOT send an error result when the media
//      format does not have the correct syntax.
//
//==============================================================================
inline void CSdpFieldAttributeFmtp::SetFormat(IN const char* pszFormat)
{
    m_strMediaFormat = pszFormat;
}

//==============================================================================
//==
//==  SetMediaFormat
//==
//==============================================================================
//
//  Parameters:
//    uFormat:
//      The media format of the fmtp field attribute.
//
//  Description:
//      Sets the media format of the fmtp field attribute from a number.
//
//==============================================================================
inline void CSdpFieldAttributeFmtp::SetMediaFormat( IN uint32_t uFormat )
{
    char acBuf[20] = {0};
    MxSnprintf(OUT acBuf, 20, "%u", uFormat);
    m_strMediaFormat = acBuf;
}

//==============================================================================
//==
//==  SetValue
//==
//==============================================================================
//
//  Parameters:
//    pszFormat:
//      The format-specific-parameters of the fmtp field attribute.
//
//  Description:
//      Sets the format-specific-parameters of the fmtp field attribute from a
//      string.
//
//      WARNING : do not use this method in the child classes since they may
//      have implemented specific set methods.
//
//==============================================================================
inline void CSdpFieldAttributeFmtp::SetValue(IN const char* pszValue)
{
    m_strValue = pszValue;
}


//==============================================================================
//==
//==  ValidateWithoutValue
//==
//==============================================================================
//
//  Returns:
//      true if all the information but m_strValue is valid.
//
//      false otherwise.
//
//  Description:
//      Validates all the specific information to this class, used as a base
//      class. Only the format-specific-parameters of the fmtp field attribute
//      are not validated.
//
//      This method should be called by the overriden Validate method in child
//      classes because the m_strValue may not be updated when the method is
//      called.
//
//      This method does NOT update the m_bIsValidValue.
//
//==============================================================================
inline bool CSdpFieldAttributeFmtp::ValidateWithoutValue() const
{
    return !m_strMediaFormat.IsEmpty();
}

//==============================================================================
//==
//==  GetFormat
//==
//==============================================================================
//
//  Returns:
//      A string containing the value of the media format or an empty string if
//      the media format is invalid.
//
//  Description:
//      Returns the media format into a string.
//
//==============================================================================
inline const char* CSdpFieldAttributeFmtp::GetFormat() const
{
    return m_strMediaFormat.CStr();
}

//==============================================================================
//==
//==  GenerateCopy
//==
//==============================================================================
//
//  Returns:
//      A copy of the current CSdpFieldAttributeFmtp.
//
//  Description:
//      Generates a copy of the current object.
//
//  Warning:
//      This methods gives ownership of the new object.
//
//==============================================================================
inline GO CSdpFieldAttributeFmtp* CSdpFieldAttributeFmtp::GenerateCopy() const
{
    return MX_NEW(CSdpFieldAttributeFmtp)(*this);
}


MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEFMTP_H
