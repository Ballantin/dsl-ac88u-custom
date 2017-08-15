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
#ifndef MXG_CSDPFIELDATTRIBUTEPTIME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTEPTIME_H

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

// Data Member

// Interface Realized & Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif



// Forward Declarations Outside of the Namespace


MX_NAMESPACE_START(MXD_GNS)

const int32_t g_nINVALID_PTIME = 0;


// Forward Declarations Inside of the Namespace
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributePtime
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an attribute-ptime.
//
// Description:
//   This class is an abstraction of an attribute-ptime in a SDP packet.
//
//   The parsing of this attribute-ptime is a specific case of an attribute.
//   The basic BNF that an attribute can have is described in
//   CSdpFieldAttributeOther.
//
//  <PRE>
//      attribute-ptime     =   "ptime:" byte-string
//      byte-string         =   1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributePtime.h
//
// See Also:
//
//==============================================================================
class CSdpFieldAttributePtime : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributePtime();

    // Summary: Copy constructor.
    CSdpFieldAttributePtime(IN const CSdpFieldAttributePtime& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldAttributePtime();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldAttributePtime& rFrom) const;

    // Summary: Assignment Operator.
    CSdpFieldAttributePtime& operator=(IN const CSdpFieldAttributePtime& rFrom);

    //-- << Stereotype >>

    // Summary:
    //  Gets the packet time.
    int32_t GetPacketTime() const;

    // Summary:
    //  Generates the data blob from the data members.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Sets the packet time.
    void SetPacketTime(IN int32_t nPacketTime);

    //-- Virtual functions

    // Summary:
    //  Parses all the needed information for this field.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //  Returns true if data members are valid.
    bool Validate();

    // Summary:
    //  Resets this object.
    void Reset();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    int32_t m_nPacketTime;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetPacketTime
//==
//==============================================================================
//
//  Returns:
//     The packet time attribute.
//
//  Description:
//    Returns the packet time attribute.
//
//==============================================================================
inline int32_t CSdpFieldAttributePtime::GetPacketTime() const
{
    return m_nPacketTime;
}

//==============================================================================
//==
//==  SetPacketTime
//==
//==============================================================================
//
//  Parameters:
//    nPacketTime:
//      The packet time attribute.
//
//  Description:
//    Sets the packet time attribute.
//
//==============================================================================
inline void CSdpFieldAttributePtime::SetPacketTime(IN int32_t nPacketTime)
{
    m_nPacketTime = nPacketTime;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDATTRIBUTEPTIME_H
