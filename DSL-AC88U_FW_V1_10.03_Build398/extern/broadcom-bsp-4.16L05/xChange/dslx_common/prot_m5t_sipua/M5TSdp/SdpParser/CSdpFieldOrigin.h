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
#ifndef MXG_CSDPFIELDORIGIN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDORIGIN_H

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
//==  Class: CSdpFieldOrigin
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of an origin-field.
//
// Description:
//   This class is an abstraction of an origin-field in a SDP packet.
//
//   The "o=" field gives the originator of the session (their username
//   and the address of the user's host) plus a session ID and session
//   version number.
//
//   If follows the BNF described in RFC 2327.
//
//  RFC 2327 BNF:
//  <PRE>
//      origin-field    =    "o=" username space
//                           sess-id space sess-version space
//                           nettype space addrtype space
//                           addr CRLF
//      username        =    safe
//      safe            =    alpha-numeric |
//                           "'" | "'" | "-" | "." | "/" | ":" | "?" | """ |
//                           "#" | "$" | "&" | "*" | ";" | "=" | "@" | "[" |
//                           "]" | "^" | "_" | "`" | "{" | "|" | "}" | "+" |
//                           "~" | "
//      sess-id         =    1*(DIGIT)
//      sess-version    =    1*(DIGIT)
//      nettype         =    "IN"
//      addrtype        =    "IP4" | "IP6"
//      addr            =    FQDN  | unicast-address
//      FQDN            =    4*(alpha-numeric|"-"|".")
//      unicast-address =    IP4-address
//      IP4-address     =    b1 "." decimal-uchar "." decimal-uchar "." b4
//      b1              =    decimal-uchar
//      b4              =    decimal-uchar
//      alpha-numeric   =    ALPHA | DIGIT
//      decimal-uchar   =    DIGIT
//                           | POS-DIGIT DIGIT
//                           | ("1" 2*(DIGIT))
//                           | ("2" ("0"|"1"|"2"|"3"|"4") DIGIT)
//                           | ("2" "5" ("0"|"1"|"2"|"3"|"4"|"5"))
//      POS-DIGIT       =    "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldOrigin.h
//
// See Also:
//
//==============================================================================
class CSdpFieldOrigin : public CSdpParser
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldOrigin();

    // Summary: Copy constructor.
    CSdpFieldOrigin(IN const CSdpFieldOrigin& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldOrigin();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldOrigin& rFrom) const;

    // Summary:
    //  Assignment operator.
    CSdpFieldOrigin& operator=(IN const CSdpFieldOrigin& rFrom);

    //-- << Stereotype >>

    // Summary:
    //   Gets the username.
    const char*  GetUsername() const;

    // Summary:
    //   Gets the session ID.
    const char*  GetSessionId() const;

    // Summary:
    //   Gets the version.
    const char*  GetVersion() const;

    // Summary:
    //   Gets the network type ID.
    ENetworkType GetNetworkTypeId() const;

    // Summary:
    //   Gets the network type string.
    const char*  GetNetworkTypeString() const;

    // Summary:
    //   Gets the address type ID.
    EAddressType GetAddressTypeId() const;

    // Summary:
    //   Gets the address type string.
    const char*  GetAddressTypeString() const;

    // Summary:
    //   Gets the address.
    const char*  GetAddress() const;

    // Summary:
    //   Generates the data blob from the data members.
    void         Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Sets the username.
    void SetUsername(IN const char* pszUsername);

    // Summary:
    //   Sets the session ID.
    void SetSessionId(IN const char* pszSessionId);

    // Summary:
    //   Sets the version.
    void SetVersion(IN const char* pszVersion);

    // Summary:
    //   Sets the network type ID.
    void SetNetworkTypeId(IN ENetworkType eNetworkType);

    // Summary:
    //   Sets the network type string.
    void SetNetworkTypeString(IN const char* pszNetworkType);

    // Summary:
    //   Sets the address type ID.
    void SetAddressTypeId(IN EAddressType eAddressType);

    // Summary:
    //   Sets the address type string.
    void SetAddressTypeString(IN const char* pszAddressType);

    // Summary:
    //   Sets the address.
    void SetAddress(IN const char* pszAddress);

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
    CString          m_strUsername;
    CString          m_strSessionId;
    CString          m_strVersion;
    ENetworkType    m_eNetworkType;
    CString          m_strNetworkType;
    EAddressType    m_eAddressType;
    CString          m_strAddressType;
    CString          m_strAddress;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  GetUsername
//==
//==============================================================================
//
//  Returns:
//      The origin field username.
//
//  Description:
//      Returns the username.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetUsername() const
{
    return m_strUsername.CStr();
}

//==============================================================================
//==
//==  GetSessionId
//==
//==============================================================================
//
//  Returns:
//      The origin field session ID.
//
//  Description:
//      Returns the session ID.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetSessionId() const
{
    return m_strSessionId.CStr();
}

//==============================================================================
//==
//==  GetVersion
//==
//==============================================================================
//
//  Returns:
//      The origin field version.
//
//  Description:
//      Returns the version.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetVersion() const
{
    return m_strVersion.CStr();
}

//==============================================================================
//==
//==  GetNetworkTypeId
//==
//==============================================================================
//
//  Returns:
//      The origin field network type ID.
//
//  Description:
//      Returns the network type ID.
//
//==============================================================================
inline CSdpParser::ENetworkType CSdpFieldOrigin::GetNetworkTypeId() const
{
    return m_eNetworkType;
}

//==============================================================================
//==
//==  GetNetworkTypeString
//==
//==============================================================================
//
//  Returns:
//      The origin field network type string.
//
//  Description:
//      Returns the network type string.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetNetworkTypeString() const
{
    return m_strNetworkType.CStr();
}

//==============================================================================
//==
//==  GetAddressTypeId
//==
//==============================================================================
//
//  Returns:
//      The origin field address type ID.
//
//  Description:
//      Returns the address type ID.
//
//==============================================================================
inline CSdpParser::EAddressType CSdpFieldOrigin::GetAddressTypeId() const
{
    return m_eAddressType;
}

//==============================================================================
//==
//==  GetAddressTypeString
//==
//==============================================================================
//
//  Returns:
//      The origin field address type string.
//
//  Description:
//      Returns the address type string.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetAddressTypeString() const
{
    return m_strAddressType.CStr();
}

//==============================================================================
//==
//==  GetAddress
//==
//==============================================================================
//
//  Returns:
//      The origin field address.
//
//  Description:
//      Returns the address.
//
//==============================================================================
inline const char* CSdpFieldOrigin::GetAddress() const
{
    return m_strAddress.CStr();
}

//==============================================================================
//==
//==  SetUsername
//==
//==============================================================================
//
//  Parameters:
//    pszUsername:
//      The username to set.
//
//  Description:
//      Sets the username in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetUsername(IN const char* pszUsername)
{
    m_strUsername = pszUsername;
}

//==============================================================================
//==
//==  SetSessionId
//==
//==============================================================================
//
//  Parameters:
//    pszSessionId:
//      The session ID to set.
//
//  Description:
//      Sets the session ID in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetSessionId(IN const char* pszSessionId)
{
    m_strSessionId = pszSessionId;
}

//==============================================================================
//==
//==  SetVersion
//==
//==============================================================================
//
//  Parameters:
//    pszVersion:
//      The version to set.
//
//  Description:
//      Sets the version in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetVersion(IN const char* pszVersion)
{
    m_strVersion = pszVersion;
}

//==============================================================================
//==
//==  SetNetworkTypeId
//==
//==============================================================================
//
//  Parameters:
//    eNetworkType:
//      The network type ID to set.
//
//  Description:
//      Sets the network type ID in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetNetworkTypeId(IN ENetworkType eNetworkType)
{
    m_eNetworkType = eNetworkType;
    m_strNetworkType = GetNetworkTypeStrFromId(eNetworkType);
}

//==============================================================================
//==
//==  SetNetworkTypeString
//==
//==============================================================================
//
//  Parameters:
//    pszNetworkType:
//      The network type string to set.
//
//  Description:
//      Sets the network type string in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetNetworkTypeString(IN const char* pszNetworkType)
{
    m_strNetworkType = pszNetworkType;
    m_eNetworkType = GetNetworkTypeIdFromStr(pszNetworkType);
}

//==============================================================================
//==
//==  SetAddressTypeId
//==
//==============================================================================
//
//  Parameters:
//    pszNetworkType:
//      The address type ID to set.
//
//  Description:
//      Sets the address type ID in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetAddressTypeId(IN EAddressType eAddressType)
{
    m_eAddressType = eAddressType;
    m_strAddressType = GetAddressTypeStrFromId(eAddressType);
}

//==============================================================================
//==
//==  SetAddressTypeString
//==
//==============================================================================
//
//  Parameters:
//    pszNetworkType:
//      The address type string to set.
//
//  Description:
//      Sets the address type string in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetAddressTypeString(IN const char* pszAddressType)
{
    m_strAddressType = pszAddressType;
    m_eAddressType = GetAddressTypeIdFromStr(pszAddressType);
}

//==============================================================================
//==
//==  SetAddress
//==
//==============================================================================
//
//  Parameters:
//    pszAddress:
//      The address to set.
//
//  Description:
//      Sets the address in the origin field.
//
//==============================================================================
inline void CSdpFieldOrigin::SetAddress(IN const char* pszAddress)
{
    m_strAddress = pszAddress;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDORIGIN_H
