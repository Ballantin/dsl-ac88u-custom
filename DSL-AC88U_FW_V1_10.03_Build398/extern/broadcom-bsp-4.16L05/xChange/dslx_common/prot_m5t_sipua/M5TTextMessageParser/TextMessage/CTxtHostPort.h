//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CTXTHOSTPORT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTXTHOSTPORT_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_TXTMESSAGECONSTANTS_H
#include "TextMessage/TxtMessageConstants.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBlob;
class CTxtToken;
class IAllocator;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CTxtHostPort
//========================================
//<GROUP TEXT_MESSAGE_CLASSES>
//<TOPICORDER 1>
//
// Summary:
//     This class abstracts the host-port construct.
//
// Description:
//     This class abstracts the host-port construct.
//
//     The host-port construct, as it is implemented in this class, has the
//     following ABNF which is inspired by RFC 3261.
// <CODE>
// host-port     = [LWS] host [LWS] [ ":" [LWS] port ] [LWS]
// port          = [0*4(%x30)] 1(%x31-%x39) /                             ; 00001-00009
//                 [0*1(%x30-%x35) 0*2(DIGIT)] 1(%x31-%x39) DIGIT /       ; 00010-59999
//                 1(%x36) 1(%30-%x34) 3(DIGIT) /                         ; 60000-64999
//                 1(%x36) 1(%x35) 1(%x30-%x35) 1(%x30-%x33) 1(%x30-%x37) ; 65000-65537
// host          = hostname / IPv6Reference / IPv4Address
// hostname      = *( domainlabel "." ) toplabel
// domainlabel   = alphanum / alphanum *( alphanum / "-" ) alphanum
// toplabel      = ALPHA / ALPHA *( alphanum / "-" ) alphanum
// IPv6Reference = "[" IPv6Address "]"
// IPv6Address   =                            6( h16 ":" ) ls32 / ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                                       "::" 5( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                               [ h16 ] "::" 4( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; ::xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                 [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; ::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                 [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx OR
//                                                                ; ::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                 [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32 / ; ::xxxx:xxxx:xxxx OR
//                                                                ; ::xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx::xxxx:xxxx:xxxx OR
//                                                                ; xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx
//
//                 [ *4( h16 ":" ) h16 ] "::"              ls32 / ; ::xxxx:xxxx OR
//                                                                ; ::xxx.xxx.xxx.xxx OR
//                                                                ; xxxx::xxxx:xxxx OR
//                                                                ; xxxx::xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx::xxxx:xxxx OR
//                                                                ; xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx
//
//                 [ *5( h16 ":" ) h16 ] "::"              h16  / ; ::xxxx OR
//                                                                ; xxxx::xxxx OR
//                                                                ; xxxx:xxxx::xxxx OR
//                                                                ; xxxx:xxxx:xxxx::xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx::xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx::xxxx OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx::xxxx
//
//                 [ *6( h16 ":" ) h16 ] "::"                     ; :: OR
//                                                                ; xxxx:: OR
//                                                                ; xxxx:xxxx:: OR
//                                                                ; xxxx:xxxx:xxxx:: OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:: OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx:: OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:: OR
//                                                                ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx::
// ls32          = ( h16 ":" h16 ) / IPv4Address
// h16           = 1*4HEXDIGIT
// HEXDIGIT      = DIGIT / %x41-%x46 / %x61-%66 ; 0-9, A-F, a-f
// IPv4Address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
// dec-octet     = [0*2(%x30)] DIGIT        / ; 000-009
//                 [1(%x30)] %x31-39 DIGIT  / ; 010-099
//                 %x31 2DIGIT              / ; 100-199
//                 %x32 %x30-34 DIGIT       / ; 200-249
//                 %x32 %x35 %x30-35          ; 250-255
// alphanum      = ALPHA / DIGIT
// ALPHA         = %x41-5A / %x61-7A          ; A-Z / a-z
// DIGIT         = %x30-39                    ; 0-9
// LWS           = [*WSP CRLF] 1*WSP          ; linear whitespace
// SWS           = [LWS]                      ; seperator whitespace
// WSP           = SP / HTAB                  ; space or tab (\t)
// SP            = %20                        ; space
// HTAB          = %09                        ; tab (\t)
// </CODE>
//
// Location:
//   TextMessage/CTxtHostPort.h
//
//==============================================================================
class CTxtHostPort
{
//-- Friend Declarations

//-- Published Interface
public:
    //-- << Constructors / Destructors >>

    // Summary:
    //  Default Constructor.
    CTxtHostPort(IN IAllocator* pAllocator = NULL,
                 IN const char* pszHost = NULL,
                 IN uint16_t uPort = 0);

    // Summary:
    //  Copy Constructor.
    CTxtHostPort(IN const CTxtHostPort& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CTxtHostPort();

    //-- << Operators >>

    // Summary:
    //  Assignment Operator.
    CTxtHostPort& operator=(IN const CTxtHostPort& rSrc);

    //<COMBINE CTxtHostPort::=@IN const CTxtHostPort&>
    CTxtHostPort& operator=(IN const char* pszHost);

    // Summary:
    //  Equality operator.
    bool operator==(IN const CTxtHostPort& rSrc) const;

    // Summary:
    //  Inequality operator.
    bool operator!=(IN const CTxtHostPort& rSrc) const;

    //-- << Accessors >>

    // Summary:
    //  Gets the current host.
    const char* GetHost() const;

    // Summary:
    //  Gets the current port.
    uint16_t& GetPort();

    //<COMBINE CTxtHostPort::GetPort>
    uint16_t GetPort() const;

    // Summary:
    //  Checks whether or not the host-port is empty.
    bool IsEmpty() const;

    // Summary:
    //  Checks whether or not the host-port contains a valid "hostname".
    bool IsHostName() const;

    // Summary:
    //  Checks whether or not the host-port contains a valid "IPv4Address".
    bool IsIpv4Address() const;

    // Summary:
    //  Checks whether or not the host-port contains a valid "IPv6Address".
    bool IsIpv6Address() const;

    //-- << Modifiers >>

    // Summary:
    //  Sets the host and port.
    mxt_result SetHostPort(IN const char* pszHost, IN uint16_t uPort, IN unsigned int uHostLength = 0);

    // Summary:
    //  Resets the content of the HostPort class.
    void Reset();

    //-- << Parsing >>
    mxt_result Parse(INOUT const char*& rpszHostPortText);

    //-- << Serialization >>

    // Summary:
    //  Appends the hostport's data in the provided blob.
    void Serialize(INOUT CBlob& rBlob, IN CTxtToken* ptokSerializingToken = NULL) const;

    //-- << public helpers >>

    // Summary:
    //  Checks whether or not the parameter contains a valid "hostname".
    static bool IsHostName(IN const char* pszData, IN unsigned int uLength = 0);

    // Summary:
    //  Checks whether or not the parameter contains a valid "IPv4Address".
    static bool IsIpv4Address(IN const char* pszData, IN unsigned int uLength = 0);

    // Summary:
    //  Checks whether or not the parameter contains a valid "IPv6Address".
    static bool IsIpv6Address(IN const char* pszData, IN unsigned int uLength = 0);

    // Summary:
    //  Checks whether or not the parameter contains a valid "Ipv6Reference".
    static bool IsIpv6Reference(IN const char* pszData, IN unsigned int uLength = 0);

//-- Hidden Methods
protected:
    //-- << Helpers >>

    // Summary:
    //  Allocates memory for a C-style string.
    GO char* AllocateString(IN uint16_t uLength);

    // Summary:
    //  Releases memory for the C-style string.
    void ReleaseString(IN TOA char* pszString);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

//-- Hidden Data Members
protected:
private:

    // The allocator to use to allocate new items in memory.
    IAllocator* m_pAllocator;

    // The host part of the hostport.
    char* m_pszHost;

    // The port part of the hostport.
    uint16_t m_uPort;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//====  OPERATORS  =============================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rSrc:
//          The host-port to which this host-port is compared.
//
//  Returns:
//      - true: The two host-ports are NOT equal.
//      - false: The two host-ports are equal.
//
//  Description:
//      Inequality operator. To be equal two host-port constructs MUST have the
//      same port and specify the same host (comparison is case-insensitive).
//
//==============================================================================
inline bool CTxtHostPort::operator!=(IN const CTxtHostPort& rSrc) const
{
    return !(*this == rSrc);
}

//==============================================================================
//====  ACCESSORS METHODS  =====================================================
//==============================================================================

//==============================================================================
//==
//==  GetHost
//==
//==============================================================================
//
//  Returns:
//      The current host string.
//
//  Description:
//      This method returns the current host as a C-Style string.
//
//==============================================================================
inline const char* CTxtHostPort::GetHost() const
{
    return m_pszHost == NULL ? g_pszEMPTY : m_pszHost;
}

//==============================================================================
//==
//==  GetPort
//==
//==============================================================================
//
//  Returns:
//      The current port.
//
//  Description:
//      This method returns the current port. The non-const overload also allows
//      to modify the port directly.
//
//==============================================================================
inline uint16_t& CTxtHostPort::GetPort()
{
    return m_uPort;
}

//--<COMBINE CTxtHostPort::GetPort>
inline uint16_t CTxtHostPort::GetPort() const
{
    return m_uPort;
}

//==============================================================================
//==
//==  IsEmpty
//==
//==============================================================================
//
//  Returns:
//      - true: The host-port is empty.
//      - false: The host-port is NOT empty.
//
//  Description:
//      This method checks whether or not the host-port is empty. An empty
//      host-port is one where the host part is NULL or the empty string, the
//      port is irrelevant as it is optional (See the ABNF of this class).
//
//==============================================================================
inline bool CTxtHostPort::IsEmpty() const
{
    return m_pszHost == NULL ||
           strcmp(m_pszHost, g_pszEMPTY) == 0;
}

//==============================================================================
//==
//==  IsHostName
//==
//==============================================================================
//
//  Returns:
//      - true: The host part is a hostname.
//      - false: The host part is empty or NOT a hostname.
//
//  Description:
//      This method checks whether or not the host-port contains a valid
//      "hostname". A hostname is any value that does not represent an IPv4 nor
//      an IPv6 address and respects this part of this class ABNF:
//
//  <CODE>
//  hostname      = *( domainlabel "." ) toplabel
//  domainlabel   = alphanum / alphanum *( alphanum / "-" ) alphanum
//  toplabel      = ALPHA / ALPHA *( alphanum / "-" ) alphanum
//  </CODE>
//
//==============================================================================
inline bool CTxtHostPort::IsHostName() const
{
    return IsEmpty() == false && IsHostName(m_pszHost);
}

//==============================================================================
//==
//==  IsIpv4Address
//==
//==============================================================================
//
//  Returns:
//      - true: The host part is an IPv4Address.
//      - false: The host part is empty or NOT an IPv4Address.
//
//  Description:
//      This method checks whether or not the host-port contains a valid
//      "IPv4Address". An IPv4Address is any value that respects this part of
//      this class ABNF:
//
//  <CODE>
//  IPv4Address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
//  dec-octet     = DIGIT              / ; 0-9
//                  %x31-39 DIGIT      / ; 10-99
//                  "1" 2DIGIT         / ; 100-199
//                  "2" %x30-34 DIGIT  / ; 200-249
//                  "25" %x30-35         ; 250-255
//  </CODE>
//
//==============================================================================
inline bool CTxtHostPort::IsIpv4Address() const
{
    return IsEmpty() == false && IsIpv4Address(m_pszHost);
}

//==============================================================================
//==
//==  IsIpv6Address
//==
//==============================================================================
//
//  Returns:
//      - true: The host part is an IPv6Address.
//      - false: The host part is empty or NOT an IPv6Address.
//
//  Description:
//      This method checks whether or not the host-port contains a valid
//      "IPv6Address". An IPv6Address is any value that respects this part of
//      this class ABNF:
//
//  <CODE>
//  IPv6Address   =                            6( h16 ":" ) ls32 / ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                                        "::" 5( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                                [ h16 ] "::" 4( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; ::xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                  [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; ::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                  [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32 / ; ::xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; ::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx:xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx:xxxx:xxx.xxx.xxx.xxx
//
//                  [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32 / ; ::xxxx:xxxx:xxxx OR
//                                                                 ; ::xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx::xxxx:xxxx:xxxx OR
//                                                                 ; xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx::xxxx:xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx::xxxx:xxx.xxx.xxx.xxx
//
//                  [ *4( h16 ":" ) h16 ] "::"              ls32 / ; ::xxxx:xxxx OR
//                                                                 ; ::xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx::xxxx:xxxx OR
//                                                                 ; xxxx::xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx::xxxx:xxxx OR
//                                                                 ; xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx::xxxx:xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx::xxx.xxx.xxx.xxx
//
//                  [ *5( h16 ":" ) h16 ] "::"              h16  / ; ::xxxx OR
//                                                                 ; xxxx::xxxx OR
//                                                                 ; xxxx:xxxx::xxxx OR
//                                                                 ; xxxx:xxxx:xxxx::xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx::xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx::xxxx OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx::xxxx
//
//                  [ *6( h16 ":" ) h16 ] "::"                     ; :: OR
//                                                                 ; xxxx:: OR
//                                                                 ; xxxx:xxxx:: OR
//                                                                 ; xxxx:xxxx:xxxx:: OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:: OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx:: OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:: OR
//                                                                 ; xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx::
//
//  h16           = 1*4HEXDIGIT
//  ls32          = ( h16 ":" h16 ) / IPv4Address
//  </CODE>
//
//==============================================================================
inline bool CTxtHostPort::IsIpv6Address() const
{
    return IsEmpty() == false && IsIpv6Address(m_pszHost);
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CTXTHOSTPORT_H

