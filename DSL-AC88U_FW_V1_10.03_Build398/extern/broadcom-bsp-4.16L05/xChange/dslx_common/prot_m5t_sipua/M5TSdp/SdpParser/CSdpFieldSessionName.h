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
#ifndef MXG_CSDPFIELDSESSIONNAME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDSESSIONNAME_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif



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
//==  Class: CSdpFieldSessionName
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a session-name-field.
//
// Description:
//   This class is an abstraction of a session-name-field in a SDP packet.
//
//   The "s=" field is the session name. It follows the BNF that is described
//   in RFC 2327.
//
//  RFC 2327 BNF:
//   <PRE>
//      session-name-field  =  "s=" text CRLF
//      text                =   byte-string
//      byte-string         =   1*(0x01..0x09|0x0b|0x0c|0x0e..0xff)
//
//   </PRE>
//
// Location:
//   SdpParser/CSdpFieldSessionName.h
//
// See Also:
//
//==============================================================================
class CSdpFieldSessionName : public CSdpParser
{
// Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldSessionName();

    // Summary: Copy constructor.
    CSdpFieldSessionName(IN const CSdpFieldSessionName& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldSessionName();

    //  Summary:
    //   Comparison operator.
    bool operator==(IN const CSdpFieldSessionName& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldSessionName& operator=(IN const CSdpFieldSessionName& rFrom);

    // Summary:
    //   Gets the session name.
    const char* GetSessionName() const;

    // Summary:
    //   Generates the data blob from the data members.
    void        Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the session name.
    void SetSessionName(IN const char* pszSessionName);

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
    CString m_strSessionName;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetSessionName
//==
//==============================================================================
//
//  Returns:
//      The session-name-field.
//
//  Description:
//      Returns the session-name-field (i.e. the s= field).
//
//==============================================================================
inline const char* CSdpFieldSessionName::GetSessionName() const
{
    return m_strSessionName.CStr();
}

//==============================================================================
//==
//==  SetSessionName
//==
//==============================================================================
//
//  Parameters:
//    pszSessionName:
//      The session-name-field.
//
//  Description:
//      Sets the session-name-field (i.e. the s= field).
//
//==============================================================================
inline void CSdpFieldSessionName::SetSessionName(IN const char* pszSessionName)
{
    m_strSessionName = pszSessionName;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDSESSIONNAME_H
