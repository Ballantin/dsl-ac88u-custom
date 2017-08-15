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
#ifndef MXG_CSDPFIELDCONNECTIONDATA_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDCONNECTIONDATA_H

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
//==  Class: CSdpFieldConnectionData
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//   This class implements an abstraction of a connection-field.
//
// Description:
//   This class is an abstraction of a connection-field in a SDP packet.
//
//   The "c=" field contains connection data.  It follows the BNF described in
//   RFC 2327.
//
//  RFC 2327 BNF:
//  <PRE>
//      connection-field    =    ["c=" nettype space addrtype space
//                               connection-address CRLF]
//      nettype             =    "IN"
//      addrtype            =    "IP4" | "IP6"
//      connection-address  =    multicast-address
//                               | addr
//      addr                =    FQDN | unicast-address
//      FQDN                =    4*(alpha-numeric|"-"|".")
//      unicast-address     =    IP4-address | IP6-address
//      IP4-address         =    b1 "." decimal-uchar "." decimal-uchar "." b4
//      b1                  =    decimal-uchar
//      b4                  =    decimal-uchar
//      multicast-address   =    3*(decimal-uchar ".") decimal-uchar "/" ttl
//                               [ "/" integer ]
//      ttl                 =    decimal-uchar
//      space               =    " "
//      alpha-numeric       =    ALPHA | DIGIT
//      decimal-uchar       =    DIGIT
//                               | POS-DIGIT DIGIT
//                               | ("1" 2*(DIGIT))
//                               | ("2" ("0"|"1"|"2"|"3"|"4") DIGIT)
//                               | ("2" "5" ("0"|"1"|"2"|"3"|"4"|"5"))
//      integer             =    POS-DIGIT *(DIGIT)
//      POS-DIGIT           =    "1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"
//
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldConnectionData.h
//
// See Also:
//
//==============================================================================
class CSdpFieldConnectionData : public CSdpParser
{
//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldConnectionData();

    // Summary: Copy constructor.
    CSdpFieldConnectionData(IN const CSdpFieldConnectionData& rFrom);

    // Summary: Destructor.
    virtual ~CSdpFieldConnectionData();

    // Summary: Comparison operator.
    bool operator==(IN const CSdpFieldConnectionData& rFrom) const;

    // Summary: Comparison operator.
    bool operator!=(IN const CSdpFieldConnectionData& rFrom) const;

    // Summary:
    //   Assignment operator.
    CSdpFieldConnectionData& operator=(IN const CSdpFieldConnectionData& rFrom);

    //-- Get functions

    // Summary:
    //   Gets the network type ID.
    ENetworkType    GetNetworkTypeId() const;

    // Summary:
    //   Gets the network type string.
    const char*     GetNetworkTypeString() const;

    // Summary:
    //   Gets the address type ID.
    EAddressType    GetAddressTypeId() const;

    // Summary:
    //   Gets the address type string.
    const char*     GetAddressTypeString() const;

    // Summary:
    //   Gets the address.
    const char*     GetAddress() const;

    // Summary:
    //   Gets TTL.
    int16_t         GetTtl() const;

    // Summary:
    //   Gets the number of addresses.
    int16_t         GetNbAddresses() const;

    //Summary:
    //   Generates the data blob from the data members.
    void            Serialize(INOUT CBlob& rBlob) const;

    //-- Set functions

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

    // Summary:
    //   Sets TTL.
    void SetTtl(IN int16_t nTtl);

    // Summary:
    //   Sets the number of addresses.
    void SetNbAddresses(IN int16_t nNbAddresses);

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
    ENetworkType    m_eNetworkType;
    CString         m_strNetworkType;
    EAddressType    m_eAddressType;
    CString         m_strAddressType;
    CString         m_strAddress;
    int16_t         m_nTtl;
    int16_t         m_nNbAddresses;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//    rFrom:
//      The CSdpFieldConnectionData to be compared.
//
//  Returns:
//      true if both are not equal, false otherwise.
//
//  Description:
//      Comparison operator.
//
//==============================================================================
inline bool CSdpFieldConnectionData::operator!=(
                                IN const CSdpFieldConnectionData& rFrom) const
{
    return !(*this == rFrom);
}

//==============================================================================
//==
//==  GetNetworkTypeId
//==
//==============================================================================
//
//  Returns:
//      The field connection data CSdpParser::ENetworkType.
//
//  Description:
//      Returns the network type ID.
//
//==============================================================================
inline CSdpParser::ENetworkType CSdpFieldConnectionData::GetNetworkTypeId() const
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
//      The network type string.
//
//  Description:
//      Returns the network type string.
//
//==============================================================================
inline const char* CSdpFieldConnectionData::GetNetworkTypeString() const
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
//      The field connection data CSdpParser::EAddressType.
//
//  Description:
//      Returns the address type ID.
//
//==============================================================================
inline CSdpParser::EAddressType CSdpFieldConnectionData::GetAddressTypeId() const
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
//      The address type string.
//
//  Description:
//      Returns the address type string.
//
//==============================================================================
inline const char* CSdpFieldConnectionData::GetAddressTypeString() const
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
//      The field connection data address.
//
//  Description:
//      Returns the field connection data address.
//
//==============================================================================
inline const char* CSdpFieldConnectionData::GetAddress() const
{
    return m_strAddress.CStr();
}

//==============================================================================
//==
//==  GetTtl
//==
//==============================================================================
//
//  Returns:
//      The field connection data ttl.
//
//  Description:
//      Returns the field connection data ttl.
//
//==============================================================================
inline int16_t CSdpFieldConnectionData::GetTtl() const
{
    return m_nTtl;
}

//==============================================================================
//==
//==  GetNbAddresses
//==
//==============================================================================
//
//  Returns:
//      The field connection data number of addresses.
//
//  Description:
//      Returns the field connection data number of addresses.
//
//==============================================================================
inline int16_t CSdpFieldConnectionData::GetNbAddresses() const
{
    return m_nNbAddresses;
}

//==============================================================================
//==
//==  SetNetworkTypeId
//==
//==============================================================================
//
//  Parameters:
//      eNetworkType:
//       The ENetworkType to set.
//
//  Description:
//      Sets the network type ID.
//
//==============================================================================
inline void CSdpFieldConnectionData::SetNetworkTypeId(IN ENetworkType eNetworkType)
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
//      pszNetworkType:
//       The network type string to set.
//
//  Description:
//      Sets the network type string.
//
//==============================================================================
inline void
CSdpFieldConnectionData::SetNetworkTypeString(IN const char* pszNetworkType)
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
//      eAddressType:
//       The EAddressType to set.
//
//  Description:
//      Sets the address type ID.
//
//==============================================================================
inline void
CSdpFieldConnectionData::SetAddressTypeId(IN EAddressType eAddressType)
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
//      eAddressType:
//       The address type string to set.
//
//  Description:
//      Sets the address type string.
//
//==============================================================================
inline void
CSdpFieldConnectionData::SetAddressTypeString(IN const char* pszAddressType)
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
//      pszAddress:
//       The address string to set.
//
//  Description:
//      Sets the address string.
//
//==============================================================================
inline void CSdpFieldConnectionData::SetAddress(IN const char* pszAddress)
{
    m_strAddress = pszAddress;
}

//==============================================================================
//==
//==  SetTtl
//==
//==============================================================================
//
//  Parameters:
//      nTtl:
//       The TTL value to set.
//
//  Description:
//      Sets the TTL value.
//
//==============================================================================
inline void CSdpFieldConnectionData::SetTtl(IN int16_t nTtl)
{
    m_nTtl = nTtl;
}

//==============================================================================
//==
//==  SetNbAddresses
//==
//==============================================================================
//
//  Parameters:
//      nNbAddresses:
//       The number of addresses to set.
//
//  Description:
//      Sets the number of addresses.
//
//==============================================================================
inline void CSdpFieldConnectionData::SetNbAddresses(IN int16_t nNbAddresses)
{
    m_nNbAddresses = nNbAddresses;
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #ifndef MXG_CSDPFIELDCONNECTIONDATA_H
