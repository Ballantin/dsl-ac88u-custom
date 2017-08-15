//==============================================================================
//==============================================================================
//
//   Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSDPFIELDATTRIBUTERTCP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSDPFIELDATTRIBUTERTCP_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif


//-- Data Members
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif


//-- Parameters used by Value


//-- Interface Realized and/or Parent
#ifndef MXG_CSDPPARSER_H
#include "SdpParser/CSdpParser.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CBlob;
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//==  Class: CSdpFieldAttributeRtcp
//========================================
//<GROUP TID_PACKAGE_SDPPARSER_CLASSES>
//
// Summary:
//  This class implements an abstraction of the rtcp SDP attribute.
//
// Description:
//  This class is an abstraction of the rtcp SDP attribute, as described in
//  RFC 3605. This complies to the following ABNF (using rules in RFC 2327,
//  Appendix A):
//
//  <PRE>
//     rtcp-attribute =      "a=rtcp:" port  [nettype space addrtype space
//                           connection-address] CRLF
//
//     port =                1*(DIGIT)
//                           ;should in the range "1024" to "65535" inclusive
//                           ;for UDP based media
//
//     nettype =             "IN"
//                           ;list to be extended
//
//     addrtype =            "IP4" | "IP6"
//                           ;list to be extended
//
//     connection-address =  multicast-address
//                           | addr
//
//     multicast-address =   3*(decimal-uchar ".") decimal-uchar "/" ttl
//                           [ "/" integer ]
//                           ;multicast addresses may be in the range
//                           ;224.0.0.0 to 239.255.255.255
//
//     addr =                FQDN | unicast-address
//
//     unicast-address =     IP4-address | IP6-address
//
//     IP4-address =         b1 "." decimal-uchar "." decimal-uchar "." b4
//
//     b1 =                  decimal-uchar
//                           ;less than "224"; not "0" or "127"
//     b4 =                  decimal-uchar
//                           ;not "0"
//
//     IP6-address =         ;to be defined
//
//     decimal-uchar =       DIGIT
//                           | POS-DIGIT DIGIT
//                           | ("1" 2*(DIGIT))
//                           | ("2" ("0"|"1"|"2"|"3"|"4") DIGIT)
//                           | ("2" "5" ("0"|"1"|"2"|"3"|"4"|"5"))
//
//     integer =             POS-DIGIT *(DIGIT)
//
//     alpha-numeric =       ALPHA | DIGIT
//
//     space =               %d32
//
//     ttl =                 decimal-uchar
//  </PRE>
//
// Location:
//   SdpParser/CSdpFieldAttributeRtcp.h
//
// See also:
//  CSdpParser
//
//==============================================================================
class CSdpFieldAttributeRtcp : public CSdpParser
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary: Default constructor.
    CSdpFieldAttributeRtcp();

    // Summary: Destructor.
    virtual ~CSdpFieldAttributeRtcp();


    //-- << CSdpParser >>.
    // Summary:
    //   Assignment operator.
    CSdpFieldAttributeRtcp& operator= (IN const CSdpFieldAttributeRtcp& rFrom);

    // Summary:
    //   Parses rpszStartPosition wrt rtcp media attribute syntax.
    EParserResult Parse(INOUT const char*& rpszStartPosition,
                        OUT mxt_result& rres);

    // Summary:
    //   Resets the data in the parser.
    virtual void Reset();

    // Summary:
    //   Serializes the data member into the proper rtcp media attribute syntax.
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //   Validates the parsed data.
    virtual bool Validate();

    //-- << Accessors >>.

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
    //   Gets the RTCP port.
    int32_t      GetPort() const;
    // Summary:
    //   Gets the RTCP address.
    const char*  GetAddress() const;

    //-- << Mutators >>.
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
    //   Sets the RTCP address.
    void SetAddress(IN const char* pszAddress);
    // Summary:
    //   Sets the RTCP port.
    void SetPort(IN uint16_t uPort);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary: Copy Constructor.
    CSdpFieldAttributeRtcp(IN const CSdpFieldAttributeRtcp& rSrc);

    // Summary:
    //   Updates the network type ID and string representation.
    void UpdateNetworkTypeIdHelper(IN ENetworkType eNetworkTypeId);

    // Summary:
    //   Updates the address type ID and string representation.
    void UpdateAddressTypeIdHelper(IN EAddressType eAddressTypeId);


//-- Hidden Data Members
protected:
private:
    // The network type of the optional address.
    ENetworkType m_eNetworkType;

    // The string representation of the network type.
    CString      m_strNetworkType;

    // The address type of the optional address.
    EAddressType m_eAddressType;

    // The string representation of the address type.
    CString      m_strAddressType;

    // The optional address.
    CString      m_strAddress;

    // The RTCP port value.
    int32_t      m_nPort;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  GetNetworkTypeId
//==
//==============================================================================
//
//  Returns:
//      The rtcp attribute CSdpParser::ENetworkType.
//
//  Description:
//      Returns the network type ID.
//
//==============================================================================
inline CSdpParser::ENetworkType CSdpFieldAttributeRtcp::GetNetworkTypeId() const
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
inline const char* CSdpFieldAttributeRtcp::GetNetworkTypeString() const
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
//      The rtcp attribute CSdpParser::EAddressType.
//
//  Description:
//      Returns the address type ID.
//
//==============================================================================
inline CSdpParser::EAddressType CSdpFieldAttributeRtcp::GetAddressTypeId() const
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
inline const char* CSdpFieldAttributeRtcp::GetAddressTypeString() const
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
//      The rtcp attribute address.
//
//  Description:
//      Returns the rtcp attribute address.
//
//==============================================================================
inline const char* CSdpFieldAttributeRtcp::GetAddress() const
{
    return m_strAddress.CStr();
}

//==============================================================================
//==
//==  GetPort
//==
//==============================================================================
//
//  Returns:
//      The rtcp attribute port.
//
//  Description:
//      Returns the rtcp attribute port.
//
//==============================================================================
inline int32_t CSdpFieldAttributeRtcp::GetPort() const
{
    return m_nPort;
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
inline void
CSdpFieldAttributeRtcp::SetNetworkTypeId(IN ENetworkType eNetworkType)
{
    UpdateNetworkTypeIdHelper(eNetworkType);
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
CSdpFieldAttributeRtcp::SetNetworkTypeString(IN const char* pszNetworkType)
{
    UpdateNetworkTypeIdHelper(GetNetworkTypeIdFromStr(pszNetworkType));
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
CSdpFieldAttributeRtcp::SetAddressTypeId(IN EAddressType eAddressType)
{
    UpdateAddressTypeIdHelper(eAddressType);
}

//==============================================================================
//==
//==  SetAddressTypeString
//==
//==============================================================================
//
//  Parameters:
//      pszAddressType:
//       The address type string to set.
//
//  Description:
//      Sets the address type string.
//
//==============================================================================
inline void 
CSdpFieldAttributeRtcp::SetAddressTypeString(IN const char* pszAddressType)
{
    UpdateAddressTypeIdHelper(GetAddressTypeIdFromStr(pszAddressType));
}

//==============================================================================
//==
//==  SetAddress
//==
//==============================================================================
//
//  Parameters:
//      pszAddress:
//       The address to set.
//
//  Description:
//      Sets the address.
//
//==============================================================================
inline void CSdpFieldAttributeRtcp::SetAddress(IN const char* pszAddress)
{
    m_strAddress = pszAddress;
}

//==============================================================================
//==
//==  SetPort
//==
//==============================================================================
//
//  Parameters:
//      uPort:
//       The port to set.
//
//  Description:
//      Sets the port of the rtcp sttribute.
//
//==============================================================================
inline void CSdpFieldAttributeRtcp::SetPort(IN uint16_t uPort)
{
    m_nPort = uPort;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSDPFIELDATTRIBUTERTCP_H


