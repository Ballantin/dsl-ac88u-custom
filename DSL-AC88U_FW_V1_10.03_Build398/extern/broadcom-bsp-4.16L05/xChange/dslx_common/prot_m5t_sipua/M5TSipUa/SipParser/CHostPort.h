//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CHOSTPORT_H
#define MXG_CHOSTPORT_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSTRINGHELPER_H
#include "SipParser/CStringHelper.h" // EAllowLws
#endif

//-- Interface Realized and/or Parent
//-------------------------------------

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
//-------------------------
class CBlob;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CHostPort
//=====================
// <GROUP SIPPARSER_CLASSES>
//
// Description:
//      This class abstracts the RFC 3261 ABNF construct "hostport".
//
//  <CODE>
//      RFC 3261 ABNF
//          hostport        =  host [ ":" port ]
//          host            =  hostname / IPv4address / IPv6reference
//          hostname        =  *( domainlabel "." ) toplabel [ "." ]
//          domainlabel     =  alphanum / alphanum *( alphanum / "-" ) alphanum
//          toplabel        =  ALPHA / ALPHA *( alphanum / "-" ) alphanum
//          IPv4address     =  1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
//          IPv6reference   =  "[" IPv6address "]"
//          IPv6address     =  hexpart [ ":" IPv4address ]
//          hexpart         =  hexseq / hexseq "::" [ hexseq ] / "::" [ hexseq ]
//          hexseq          =  hex4 *( ":" hex4)
//          hex4            =  1*4HEXDIG
//          port            =  1*DIGIT
//
//      Corrected IPv6 ABNF (draft-gurbani-sip-ipv6-abnf-fix-00):
//          IPv4address     =  1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT "." 1*3DIGIT
//          IPv6reference   =  "[" IPv6address "]"
//          IPv6address     =                             6( h16 ":" ) ls32
//                           /                       "::" 5( h16 ":" ) ls32
//                           / [               h16 ] "::" 4( h16 ":" ) ls32
//                           / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//                           / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//                           / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
//                           / [ *4( h16 ":" ) h16 ] "::"              ls32
//                           / [ *5( h16 ":" ) h16 ] "::"              h16
//                           / [ *6( h16 ":" ) h16 ] "::"
//
//          h16             = 1*4HEXDIG
//          ls32            = ( h16 ":" h16 ) / IPv4address
//          IPv4address     = dec-octet "." dec-octet "." dec-octet "."
//                            dec-octet
//          dec-octet       = DIGIT                  ; 0-9
//                           / %x31-39 DIGIT         ; 10-99
//                           / "1" 2DIGIT            ; 100-199
//                           / "2" %x30-34 DIGIT     ; 200-249
//                           / "25" %x30-35          ; 250-255
//  </CODE>
//
// NOTES:
//  This class must not contain the scope ID (%) when the host is an
//  IPv6address.
//
// Location:
//   SipParser/CHostPort.h
//
//==EDOC========================================================================
class CHostPort
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CHostPort();

    // Summary:
    //  Copy Constructor.
    //--------------------
    CHostPort(IN const CHostPort& rSrc);

    // Summary:
    //  Constructor.
    //---------------
    CHostPort( IN const CString& rstrHost,
               IN uint16_t uPort = ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT );

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CHostPort();

    // Summary:
    //  Assignment operator.
    //-----------------------
    CHostPort& operator=(IN const CHostPort& rSrc);

    // Summary:
    //  Assignment operator. Assigns the host and resets the port to
    //  ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT.
    //---------------------------------------------------------------
    CHostPort& operator=(IN const CString& rstrHost);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CHostPort& rSrc) const;

    // Summary:
    //  Inequality operator.
    //-----------------------
    bool operator!=(IN const CHostPort& rSrc) const;


    //-- << Public Methods >>
    //--------------------------------------------

    // Summary:
    //  Provides access to the host.
    //-------------------------------
    const CString& GetHost() const;

    // Summary:
    //  Sets the host.
    void SetHost(IN const CString& rstrHost);

    // Summary:
    //  Provides access to the port.
    //-------------------------------
    uint16_t GetPort() const;

    // <COMBINE CHostPort::GetPort@const>
    //-----------------------------------
    uint16_t& GetPort();

    // Summary:
    //  Checks if m_strHost contains a valid "hostname".
    //---------------------------------------------------
    bool IsHostName() const;

    // Summary:
    //  Checks if rstrData contains a valid "hostname".
    //--------------------------------------------------
    static bool IsHostName(IN const CString& rstrData);

    // Summary:
    //  Checks if m_strHost contains a valid "Ipv4Address".
    //------------------------------------------------------
    bool IsIpv4Address() const;

    // Summary:
    //  Checks if rstrData contains a valid "Ipv4Address".
    //-----------------------------------------------------
    static bool IsIpv4Address(IN const CString& rstrData);

    // Summary:
    //  Checks if m_strHost contains a valid "Ipv6Reference".
    //--------------------------------------------------------
    bool IsIpv6Reference() const;

    // Summary:
    //  Checks if rstrData contains a valid "Ipv6Reference".
    //-------------------------------------------------------
    static bool IsIpv6Reference(IN const CString& rstrData);

    // Summary:
    //  Converts an "Ipv6Reference" to an "Ipv6Address".
    //---------------------------------------------------
    static mxt_result Ipv6ReferenceToIpv6Address(INOUT CString& rstrData);

    // Summary:
    //  Converts an "Ipv6Address" to an "Ipv6Reference".
    //---------------------------------------------------
    static void Ipv6AddressToIpv6Reference(INOUT CString& rstrData);

    // Summary:
    //  Parses the host and optional port.
    //-------------------------------------
    mxt_result Parse( IN CStringHelper::EAllowLws eAllowLws,
                      INOUT const char*& rpcPos );

    // Summary:
    //  Outputs the hostport. The port is output only if different from
    //  ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT.
    //------------------------------------------------------------------
    void Serialize(INOUT CBlob& rBlob) const;

    // Summary:
    //  Resets this hostport.
    //------------------------
    void Reset();

    // Summary:
    //  Allows to configure the hostname to accept an alphanum character as the
    //  first character of the last label.
    static void AllowLastLabelAlphanumFirstCharacter(IN bool bAllow);


//-- Hidden Methods
//-------------------
protected:
    //-- << Helper Methods >>
    //-------------------------

    // Summary:
    //  Finds the length of a hexseq sequence.
    //-----------------------------------------
    static unsigned int GetHexSeqLength(IN const char* pcPos);

    // Summary:
    //  Compares two host names.
    //---------------------------
    static bool CompareHosts(IN const CString& rstrHost1,
                             IN const CString& rstrHost2);


private:

//-- Hidden Data Members
//------------------------
protected:

    // Host.
    //-------
    CString m_strHost;

    // Optional Port.
    //----------------
    uint16_t m_uPort;

    // Flag telling if the last label of a hostname accepts the first character
    // to be an alphanum. True to accept it, otherwise false to only accept
    // alpha as the first character (which is the default behavior).
    static bool ms_bAllowLastLabelAlphanumFirstCharacter;


public:
    // This port value means that the default SIP port will be used. It also
    // means that the SIP port will not be output in serialized packets.
    //-----------------------------------------------------------------------
    static const uint16_t ms_uUSE_SIP_DEFAULT_DONT_OUTPUT_PORT;
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  GetHost
//==
//==============================================================================
//
//  Returns:
//      Hostname.
//
//  Description:
//      Provides constant access to the hostname.
//
//==EDOC========================================================================
inline const CString& CHostPort::GetHost() const
{
    return m_strHost;
}

//==SDOC========================================================================
//==
//==  GetPort
//==
//==============================================================================
//
//  Returns:
//      Port.
//
//  Description:
//      Provides constant access to the port.
//
//==EDOC========================================================================
inline uint16_t CHostPort::GetPort() const
{
    return m_uPort;
}

//==SDOC========================================================================
//==
//==  GetPort
//==
//==============================================================================
//
//  Returns:
//      Port.
//
//  Description:
//      Provides modifiable access to the port.
//
//==EDOC========================================================================
inline uint16_t& CHostPort::GetPort()
{
    return m_uPort;
}

//==============================================================================
//==
//==  AllowLastLabelAlphanumFirstCharacter
//==
//==============================================================================
//
//  Summary:
//      Allows to configure the hostname to accept an alphanum character as the
//      first character of the last label.
//
//  Parameters:
//      bAllow:
//       - true: Accepts the first character of the last label to be alphanum.
//       - false: Strict parsing as described in the RFC as the first character
//                of the last label needs to be an alpha.
//
//  Description:
//      Allows parsing a hostname such as this "foo.com.12345".
//
//      According to the BNF of the RFC such hostname is invalid as the first
//      character of the last label needs to be an alpha.
//
//  <CODE>
//      RFC 3261 ABNF:
//          hostname         =  *(domainlabel ".") toplabel [ "." ]
//          domainlabel      =  alphanum / alphanum *(alphanum / "-") alphanum
//          toplabel         =  ALPHA    / ALPHA    *(alphanum / "-") alphanum
//  </CODE>
//
//      The default behavior of the stack is to not allow the first character to
//      be an alphanum (bAllow parameter set to false).
//
//==============================================================================
inline void CHostPort::AllowLastLabelAlphanumFirstCharacter(IN bool bAllow)
{
    ms_bAllowLastLabelAlphanumFirstCharacter = bAllow;
}

MX_NAMESPACE_END(MXD_GNS)


#endif //-- #ifndef MXG_CHOSTPORT_H
