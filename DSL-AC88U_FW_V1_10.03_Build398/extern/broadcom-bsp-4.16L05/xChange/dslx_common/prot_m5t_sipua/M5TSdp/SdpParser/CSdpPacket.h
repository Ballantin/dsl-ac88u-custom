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
#ifndef MXG_CSDPPACKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPPACKET_H

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
#ifndef MXG_CSDPLEVELSESSION_H
#include "SdpParser/CSdpLevelSession.h"
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
//==  Class: CSdpPacket
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a SDP packet.
//
// Description:
//   This class is an abstraction of a SDP packet.  The parsing of
//   this class corresponds to the BNF described in RFC 2327.
//
// RFC 2327 BNF:
//   <PRE>
//      sdp-packet = <Link CSdpLevelSession, level-session>
//
//   </PRE>
//
//   How to use this class:
//    * Steps to parse a SDP packet:
//      1) Create a CSdpPacket object
//      2) Call Parse() to read the data from a buffer
//      3) Check that rres == resS_OK to make sure no parsing error has
//         occurred
//      4) Call IsValid() to make sure all the data has been correctly read
//      5) Call all the GetXXX() functions needed to get the desired
//         information
//      6) Repeat steps 2, 3, 4 and 5 as needed, but be aware that it is your
//         responsibility to call Reset between consecutive calls to Parse
//         on the same object
//
//    * Steps to generate a SDP packet:
//      1) Create a CSdpPacket object, a CSdpLevelSession one, and
//         CSdpLevelMedia(s) if needed
//      2) Call all the SetXXX() and AddXXX() functions needed to set the
//         desired information
//      3) Call Validate() to make sure the packet at least contains the
//         minimum fields and that all the fields are valid
//      4) Call Serialize() to put the data in a CBlob object
//
// Location:
//   SdpParser/CSdpPacket.h
//
// See Also:
//   CSdpLevelSession
//
//==============================================================================
class CSdpPacket : public CSdpParser
{
//-- Friend Declaration

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpPacket();

    // Summary: Copy constructor.
    CSdpPacket(IN const CSdpPacket& rFrom);

    // Summary: Destructor.
    virtual ~CSdpPacket();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpPacket& rFrom) const;

    // Summary:
    //  Assignment operator.
    CSdpPacket& operator=(IN const CSdpPacket& rFrom);

    //-- << Get functions >>

    // Summary:
    //  Gets the session.
    CSdpLevelSession&       GetSession();
    const CSdpLevelSession& GetSession() const;

    // Summary:
    //  Generates the data blob from the data members.
    void                    Serialize(INOUT CBlob& rBlob) const;


    //-- << Set functions >>

    // Summary:
    //  Sets the session.
    void SetSession(IN CSdpLevelSession& rSession);


    //-- << Virtual functions >>

    // Summary:
    // Parses all needed information for this packet.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    // Validates the parsed data for this packet.
    bool Validate();

    // Summary:
    // Needed in order to recall Parse.
    void Reset();


//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
protected:
private:

// Hidden Data Members
protected:
private:
    CSdpLevelSession m_session;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetSession
//==
//==============================================================================
//
//  Returns:
//      The session level.
//
//  Description:
//      Returns the session level.
//
//==============================================================================
inline CSdpLevelSession&
CSdpPacket::GetSession()
{
    return m_session;
}

// <COMBINE CSdpPacket::GetSession>
inline const CSdpLevelSession&
CSdpPacket::GetSession() const
{
    return m_session;
}

//==============================================================================
//==
//==  SetSession
//==
//==============================================================================
//
//  Parameters:
//    rSession:
//      The session level to set.
//
//  Description:
//      Sets the session level.
//
//==============================================================================
inline void CSdpPacket::SetSession(IN CSdpLevelSession& rSession)
{
    m_session = rSession;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPPACKET_H
