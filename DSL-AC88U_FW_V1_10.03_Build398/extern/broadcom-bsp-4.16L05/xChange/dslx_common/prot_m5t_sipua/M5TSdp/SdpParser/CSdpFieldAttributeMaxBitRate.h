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
#ifndef MXG_CSDPFIELDATTRIBUTEMAXBITRATE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEMAXBITRATE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SDP Parser Configuration
#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h" // MXD_SDP_ENABLE_T38_SUPPORT
#endif


// We compile this only if it was requested to include the T.38 classes.
#if defined(MXD_SDP_ENABLE_T38_SUPPORT)


//-- Data Member

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
//==  Class: CSdpFieldAttributeMaxBitRate
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-max-bit-rate.
//
// Description:
//   This class is an abstraction of an attribute-max-bit-rate in a SDP packet.
//
//   The parsing of this attribute-max-bit-rate is a specific case of an attribute.
//   The basic BNF that an attribute can have is described into
//   CSdpFieldAttributeOther.
//
//  <PRE>
//      attribute-max-bit-rate  =   "T38MaxBitRate:" byte-string
//      byte-string             =   1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeMaxBitRate.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributeMaxBitRate : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>
    // Summary: Default constructor.
    CSdpFieldAttributeMaxBitRate();

    // Summary: Copy constructor.
    CSdpFieldAttributeMaxBitRate(IN const CSdpFieldAttributeMaxBitRate& rFrom);

    // Summary: Destructor
    virtual ~CSdpFieldAttributeMaxBitRate();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldAttributeMaxBitRate& rFrom) const;

    // Summary: Assignment operator.
    CSdpFieldAttributeMaxBitRate& operator=(IN const CSdpFieldAttributeMaxBitRate& rFrom);

    //-- << Stereotype >>

    // Summary:
    //  Gets the maximum bitrate value.
    const uint32_t GetMaxBitRate() const;

    // Summary:
    //  Generates the data blob from the data members.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Sets the maximum bitrate value.
    void SetMaxBitRate(IN const uint32_t nMaxBitRate);

    //-- Virtual functions
    // Summary:
    //   Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);
    // Summary:
    //   Checks the validity of the parsed data.
    bool Validate();

    // Summary:
    //   Resets all member variables.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    uint32_t m_nMaxBitRate;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetMaxBitRate
//==
//==============================================================================
//
//  Returns:
//      The maximum bitrate.
//
//  Description:
//      Returns the maximum bitrate.
//
//==============================================================================
inline const uint32_t CSdpFieldAttributeMaxBitRate::GetMaxBitRate() const
{
    return m_nMaxBitRate;
}

//==============================================================================
//==
//==  SetMaxBitRate
//==
//==============================================================================
//
//  Parameters:
//      nMaxBitRate:
//       The maximum bitrate to set.
//
//  Description:
//      Sets the maximum bitrate.
//
//==============================================================================
inline void CSdpFieldAttributeMaxBitRate::SetMaxBitRate(IN const uint32_t nMaxBitRate)
{
    m_nMaxBitRate = nMaxBitRate;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //#if defined(MXD_SDP_ENABLE_T38_SUPPORT)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEMAXBITRATE_H

