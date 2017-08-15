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
#ifndef MXG_CSDPFIELDPROTOCOLVERSION_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDPROTOCOLVERSION_H

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
//==  Class: CSdpFieldProtocolVersion
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a proto-version field.
//
// Description:
//   This class is an abstraction of a proto-version field in a SDP packet.
//
//   The "v=" field gives the version of the Session Description Protocol.  It
//   follows what is described in the BNF of RFC 2327.
//
//  RFC 2327 BNF:
//   <PRE>
//      proto-version =       "v=" 1*DIGIT CRLF
//
//   </PRE>
//
// Location:
//   SdpParser/CSdpFieldProtocolVersion.h
//
// See Also:
//
//==============================================================================
class CSdpFieldProtocolVersion : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldProtocolVersion();

    // Summary: Copy constructor.
    CSdpFieldProtocolVersion(IN const CSdpFieldProtocolVersion& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldProtocolVersion();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldProtocolVersion& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldProtocolVersion& operator=(IN const CSdpFieldProtocolVersion& rFrom);

    // Summary:
    //   Gets the protocol version.
    int16_t GetProtocolVersion() const;

    // Summary:
    //   Generates the data blob from the data members.
    void    Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the protocol version.
    void SetProtocolVersion(IN int16_t nProtocolVersion);

    //-- Virtual functions

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
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    int32_t m_nProtocolVersion;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetProtocolVersion
//==
//==============================================================================
//
//  Returns:
//      The proto-version.
//
//  Description:
//      Returns the proto-version (i.e. the v= field).
//
//==============================================================================
inline int16_t CSdpFieldProtocolVersion::GetProtocolVersion() const
{
    // BRCM_CUSTOM [mod] - Suppress warning
    return (static_cast<int16_t> (m_nProtocolVersion));
    // BRCM_CUSTOM [end] - Suppress warning
}

//==============================================================================
//==
//==  SetProtocolVersion
//==
//==============================================================================
//
//  Parameters:
//    nProtocolVersion:
//      The proto-version.
//
//  Description:
//      Sets the proto-version (i.e. the v= field).
//
//==============================================================================
inline void
CSdpFieldProtocolVersion::SetProtocolVersion(IN int16_t nProtocolVersion)
{
    m_nProtocolVersion = nProtocolVersion;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDPROTOCOLVERSION_H
