//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_CSOCKETADDR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSOCKETADDR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_IPV6_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

#if defined (MXD_OS_LINUX) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <netinet/in.h>                 // for sockaddr struct declaration
    #include <arpa/inet.h>                  // for inet_addr & inet_pton
    #include <errno.h>
    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        #include <netdb.h>                  // for getaddrinfo
    #endif

#elif defined(MXD_OS_SOLARIS)
    #include <sys/socket.h>
    #include <netinet/in.h>                 // for sockaddr struct declaration
    #include <arpa/inet.h>                  // for inet_addr
    #include <errno.h>

    #ifndef INADDR_NONE                     // This is not defined on solaris
        #define INADDR_NONE ((unsigned long) -1)
    #endif

    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        #include <netdb.h>                  // for getaddrinfo
    #endif

#elif defined (MXD_OS_VXWORKS)
    #include <socket.h>                     // for AF_INET
    #include <netinet/in.h>                 // for INADDR_ANY
    #include <inetLib.h>
    #include <errnoLib.h>
    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        #include <hostLib.h>                // for getaddrinfo
    #endif
// BRCM_CUSTOM - [begin] Add ECOS support.
#elif defined MXD_OS_BOS_ECOS
    #include <bosSocket.h>
    #include <netStr.h>                     // for netStrIpAddrStrToNum and netStrIpAddrNumToStr
// BRCM_CUSTOM - [end] Add ECOS support.
#elif defined (MXD_OS_WINDOWS)
    #include <errno.h>
#elif defined MXD_OS_WINDOWS_CE
#elif defined MXD_OS_NUCLEUS
    #ifndef MXG_SOCKETDEFINES_H
        #include "Network/SocketDefines.h"
    #endif
#elif defined (MXD_OS_SYMBIAN)
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <in_sock.h>
#else
    #error "Targeted operating system undefined. Please define MXD_OS_LINUX, " \
           "MXD_OS_ANDROID, MXD_OS_SOLARIS, MXD_OS_DARWIN, MXD_OS_VXWORKS, " \
		   "MXD_OS_WINDOWS, MXD_OS_WINDOWS_CE, MXD_OS_SYMBIAN or MXD_OS_NUCLEUS"
#endif

#ifndef MXG_CMARSHALER_H
#include "Cap/CMarshaler.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

// Data Member

// Interface Realized & Parent

// Forward Declarations Outside of the Namespace

//M5T_INTERNAL_USE_END

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSocketAddr;
class CString;

//M5T_INTERNAL_USE_BEGIN
CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CSocketAddr& rData);
CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CSocketAddr& rData);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const unsigned int g_uSIZE_OF_IPV6_ADDRESS = 16;
const unsigned int g_uSIZE_OF_IPV4_ADDRESS = 4;

// In network byte order (big endian)
static const uint8_t gs_uINET_ADDR_LOOPBACK[] = {0x7F, 0x00, 0x00, 0x01};
static const uint8_t gs_uINET_ADDR_ANY[] = {0x00, 0x00, 0x00, 0x00};
static const uint8_t gs_uINET_ADDR_BROADCAST[] = {0xFF, 0xFF, 0xFF, 0xFF};
static const uint8_t gs_uINET_ADDR_NONE[] = {0xFF, 0xFF, 0xFF, 0xFF};

static const uint8_t gs_uINET6_ADDR_UNSPECIFIED[] = {0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00,
                                                     0x00, 0x00, 0x00, 0x00};

static const uint8_t gs_uINET6_ADDR_LOOPBACK[] = {0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x01};

const char g_pszINET6_ADDR_LOOPBACK[] = "::1";
const char g_pszINET6_ADDR_ANY[] = "::";
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Class: CSocketAddr
//==
//==============================================================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Class providing a common interface for storing socket addresses.
//
//  Description:
//    The CSocketAddr class is used to provide a common interface to store
//    socket addresses independently from the operating system and socket
//    family used. This way all code used for sockets is the same for any
//    architecture.
//
//==EDOC========================================================================
class CSocketAddr
{
//M5T_INTERNAL_USE_BEGIN
// Friend Declaration
//--------------------
    friend CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CSocketAddr& rData);
    friend CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CSocketAddr& rData);

    // Socket classes need to call ConvertFromOsSpecific.
    friend class CUdpSocket;
    friend class CAsyncUdpSocket;
    friend class CTcpSocket;
    friend class CAsyncTcpSocket;
    friend class CAsyncTlsSocket;

    friend class CTcpServerSocket;
    friend class CAsyncTcpServerSocket;
    friend class CAsyncTlsServerSocket;

    // This is not documented so far. It is used by ICE to access CSocketAddr private methods.
    friend class COtherSocket;

//M5T_INTERNAL_USE_END

// Enum Type Definition
//----------------------
public:
    //<GROUP NETWORK_ENUMS>
    //
    // Summary:
    //  Address families supported.
    //-----------------------------
    enum EAddressFamily
    {
        // Description:
        // IPv4 address family.
        eINET,
        // Description:
        // IPv6 address family.
        eINET6,
    };

    //<GROUP NETWORK_ENUMS>
    //
    // Summary:
    //  Standard IP addresses.
    //-------------------------
    enum EStandardAddress
    {
        // Description:
        // IPv4 any address (0.0.0.0).
        eINET_ANY,
        // Description:
        // IPv4 broadcast address (255.255.255.255).
        eINET_BROADCAST,
        // Description:
        // IPv4 loopback address (127.0.0.1).
        eINET_LOOPBACK,
        // Description:
        // IPv4 none address (255.255.255.255).
        eINET_NONE,
        // Description:
        // IPv6 any address (::). MXD_IPV6_ENABLE_SUPPORT must be defined to
        // access this value.
        eINET6_ANY,
        // Description:
        // IPv6 loopback address (::1). MXD_IPV6_ENABLE_SUPPORT must be defined
        // to access this value.
        eINET6_LOOPBACK
    };

// Published Interface
//---------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSocketAddr();

    // Summary:
    //  Constructor. Builds address using a char array and port number.
    //------------------------------------------------------------------
    CSocketAddr(IN const char* pszAddress,
                IN uint16_t uPort = 0,
                IN EMxBase eBase = eBASE_AUTO,
                IN bool bForceFourParts = false);

    // Summary:
    //  Constructor. Builds address using a byte array, a family and a port
    //  number.
    //----------------------------------------------------------------------
    CSocketAddr(IN const uint8_t* puAddress,
                IN unsigned int uAddressSize,
                IN EAddressFamily eFamily,
                IN uint16_t uPort = 0);

    // Summary:
    //  Constructor. Builds address using an address type and port number.
    //---------------------------------------------------------------------
    CSocketAddr(IN EStandardAddress eType,
                IN uint16_t uPort = 0);

    // Summary:
    //  Copy constructor.
    //--------------------
    CSocketAddr(IN const CSocketAddr& rFrom);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSocketAddr();

    // Overloadded operator
    //----------------------

    // Summary:
    //  Assignment operator.
    //-----------------------
    CSocketAddr& operator=(IN const CSocketAddr& rFrom);

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CSocketAddr& rFrom) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CSocketAddr& rFrom) const;

// BRCM_CUSTOM - [begin] Add ECOS support.
#if defined MXD_OS_BOS_ECOS
    // Summary:
    //  Cast operator.
    //-----------------
    operator BOS_SOCKET_ADDR() const;

    // Summary:
    //  Cast operator.
    //-----------------
    operator BOS_SOCKET_ADDR*();

    // Summary:
    //  Cast operator.
    //-----------------
    operator const BOS_SOCKET_ADDR*();
#endif
// BRCM_CUSTOM - [end] Add ECOS support.

#if defined(MXD_OS_SYMBIAN)
    //<FLAG Symbian OS only>
    //
    // Summary:
    //  Cast operator.
    //-----------------
    operator TInetAddr*() const;
#else
    // Summary:
    //  Cast operator.
    //-----------------
    operator sockaddr() const;

    // Summary:
    //  Cast operator.
    //-----------------
    operator sockaddr*() const;

    // Summary:
    //  Cast operator.
    //-----------------
    operator const sockaddr*() const;
#endif // #if defined(MXD_OS_SYMBIAN)

    // General Address Methods
    //-------------------------

    // Summary:
    //  Verifies that the addresses are equal.
    //-----------------------------------------
    bool IsEqualAddress(IN const CSocketAddr& rFrom) const;

    // Summary:
    //  Verifies that the families are equal.
    //----------------------------------------
    bool IsEqualFamily(IN const CSocketAddr& rFrom) const;

    // Summary:
    //  Verifies that the port addresses are equal.
    //----------------------------------------------
    bool IsEqualPort(IN const CSocketAddr& rFrom) const;

    // Summary:
    //  Verifies that the port is the IPV4 loopback address.
    //-------------------------------------------------------
    bool IsInetAddressLoopback() const;

    // Summary:
    //  Verifies that the scope ID are equal.
    //----------------------------------------
    bool IsEqualScopeId(IN const CSocketAddr& rFrom) const;

    // Summary:
    //  Verifies that the address and port in the object are valid.
    //--------------------------------------------------------------
    bool IsValid() const;

    // Summary:
    //  Verifies that the address is valid.
    //--------------------------------------
    bool IsValidAddress() const;

    // Summary:
    //  Verifies that the address family is valid.
    //---------------------------------------------
    bool IsValidFamily() const;

    // Summary:
    //  Verifies that the port is valid.
    //-----------------------------------
    bool IsValidPort() const;

    // Summary:
    //  Verifies if the address is an IPv4 multicast address.
    bool IsInetAddressMulticast() const;

    // Summary:
    //  Verifies that the address is the IPv6 unspecifed address.
    //------------------------------------------------------------
    bool IsInet6AddressUnspecified() const;

    // Summary:
    //  Verifies that the address is the IPv6 loopback address.
    //----------------------------------------------------------
    bool IsInet6AddressLoopback() const;

    // Summary:
    //  Verifies that the address is an IPv6 multicast address.
    //----------------------------------------------------------
    bool IsInet6AddressMulticast() const;

    // Summary:
    //  Verifies that the address is an IPv6 LinkLocal address.
    //----------------------------------------------------------
    bool IsInet6AddressLinkLocal() const;

    // Summary:
    //  Verifies that the address is an IPv6 SiteLocal address.
    //----------------------------------------------------------
    bool IsInet6AddressSiteLocal() const;

    // Summary:
    //  Verifies that the address is a V4-mapped IPv6 address.
    //---------------------------------------------------------
    bool IsInet6AddressV4Mapped() const;

    // Summary:
    //  Verifies that the address is an IPv6 global address.
    //-------------------------------------------------------
    bool IsInet6AddressGlobal() const;

    // Summary:
    //  Verifies that the address is an IPv6 unique local address (RFC4193).
    //-----------------------------------------------------------------------
    bool IsInet6AddressUniqueLocal() const;

    // Summary:
    //  Verifies that the addresses are of the same scope.
    //-----------------------------------------------------
    bool IsEqualScope(IN CSocketAddr rFrom) const;

    // Summary:
    //  Resets the object's data.
    //----------------------------
    void Reset();

    // Summary:
    //  Gets the family.
    //-------------------
    EAddressFamily GetFamily() const;

    // Summary:
    //  Sets the port.
    //-----------------
    void SetPort(IN uint16_t uPort);

    // Summary:
    //  Gets the port.
    //-----------------
    uint16_t GetPort() const;

    // Summary:
    //  Gets the size.
    //-----------------
    unsigned int GetSize() const;

    // Internet Address Methods
    //--------------------------

    // Summary:
    //  Sets a CSocketAddr in the current object.
    //--------------------------------------------
    mxt_result SetAddress(IN const CSocketAddr& rAddress);

    // For the following SetAddress(), the param IN uint16_t uPort
    // default to 0 for a specific reason: When uPort is 0, the port
    // will remain unchanged. If a parameter is specified, then the
    // provided port number will be set.

    // Summary:
    //  Sets the address and port.
    //-----------------------------
    mxt_result SetAddress(IN const char* pszAddress,
                          IN uint16_t uPort = 0,
                          IN EMxBase eBase = eBASE_AUTO,
                          IN bool bForceFourParts = false);

    // Summary:
    //  Sets the address and port.
    //-----------------------------
    mxt_result SetAddress(IN EStandardAddress eType,
                          IN uint16_t uPort = 0);

    // Summary:
    //  Sets the address and port.
    //-----------------------------
    mxt_result SetAddress(IN const uint8_t* puAddress,
                          IN unsigned int uAddressSize,
                          IN EAddressFamily eFamily,
                          IN uint16_t uPort = 0);

    // Summary:
    //  Gets the address.
    //--------------------
    mxt_result GetAddress(IN unsigned int uAddressSize,
                          OUT char* pszAddress,
                          IN bool bShowScope = true) const;

    // Summary:
    //  Gets the address.
    //--------------------
    mxt_result GetAddress(IN const unsigned int uAddressCapacity,
                          OUT uint8_t* puAddress,
                          OUT unsigned int* puAddressSize) const;

    // Summary:
    //  Gets the address.
    //--------------------
    mxt_result GetAddress(OUT CString& rstrAddress,
                          IN bool bShowScope = true) const;

    // Summary:
    //  Gets the address.
    //--------------------
    CString GetAddress(IN bool bShowScope = true) const;

    // Summary:
    //  Sets the scope ID.
    //--------------------
    mxt_result SetScopeId(IN uint32_t uScopeId);

    // Summary:
    //  Gets the scope ID.
    //--------------------
    uint32_t GetScopeId() const;

    // Constant values
    //-----------------

    // Summary:
    //  Returns a INET CSocketAddr of type any.
    //------------------------------------------
    static const CSocketAddr& InetAnyAddress();

    // Summary:
    //  Returns a INET6 CSocketAddr of type any.
    //------------------------------------------.
    static const CSocketAddr& Inet6AnyAddress();

    // Summary:
    //  Returns a INET CSocketAddr of type broadcast.
    //------------------------------------------------
    static const CSocketAddr& InetBroadcastAddress();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
private:
    void SetSinPort(IN uint16_t uPort);
    void SetSinFamily(IN EAddressFamily eFamily);
    mxt_result SetSinAddress(IN const char* pszAddress,
                             IN EMxBase eBase,
                             IN bool bForceFourParts);
    mxt_result SetSinAddress(IN const uint8_t* puAddress);
    mxt_result SetSinAddress(IN EStandardAddress eType);
// BRCM_CUSTOM - [mod] Add ECOS support.
#if defined(MXD_OS_VXWORKS) || defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] Add ECOS support.
    void SetSinLength();
#endif

    uint32_t GetSinAddress() const;
    const uint8_t* GetSinAddressByteArray(OUT unsigned int* puAddressSize) const;
    mxt_result GetSinAddressAscii(IN unsigned int uAddressSize,
                                  OUT char* pszAddress,
                                  IN bool bShowScope = true) const;

    CSocketAddr::EAddressFamily DetectFamilyFromString(IN const char* pszAddress) const;

    static mxt_result InitializeCSocketAddr();
    static void FinalizeCSocketAddr();

    mxt_result NtoA(IN unsigned int uBufferSize,
                    OUT char* pszAddress) const;

    mxt_result AtoN(IN const char* pszAddress,
                    IN EMxBase eBase,
                    IN bool bForceFourParts,
                    OUT uint8_t* pauAddress);

    bool ValidateIpV6Str(IN const char* pszAddress,
                         IN unsigned int uSize) const;

    CString ExtractScopeId(IN const char* pszAddress,
                           IN unsigned int uSize) const;

    void ConvertFromOsSpecific();

    void ConvertToOsSpecific();

// Friends declarations
//----------------------
    friend mxt_result InitializeCSocketAddr();
    friend void FinalizeCSocketAddr();

// Hidden Data Members
//---------------------
private:
    uint16_t m_uPort;
    // IPv4 and IPv6 address are stored in this array. IPv4 are stored in the
    // first 4 bytes of the array.
    uint8_t  m_auAddr[g_uSIZE_OF_IPV6_ADDRESS];
    uint32_t m_uScopeId;
    EAddressFamily m_eFamily;

#if defined(MXD_OS_SYMBIAN)
    TInetAddr m_stSockAddr;
#else
    #if defined(MXD_IPV6_ENABLE_SUPPORT)
        sockaddr_storage m_stSockAddr;
    #else
        sockaddr m_stSockAddr;
    #endif
#endif

    static const CSocketAddr* ms_pSockInet6AnyAddress;
    static const CSocketAddr* ms_pSockInetAnyAddress;
    static const CSocketAddr* ms_pSockInetBroadcastAddress;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

// BRCM_CUSTOM - [begin] Add ECOS support.
#if defined MXD_OS_BOS_ECOS
//==SDOC========================================================================
//==
//==  operator BOS_SOCKET_ADDR() const
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a const BOS_SOCKET_ADDR.
//
//==EDOC========================================================================
inline
CSocketAddr::operator BOS_SOCKET_ADDR() const
{
    return *(reinterpret_cast<const BOS_SOCKET_ADDR*>(&m_stSockAddr));
}

//==SDOC========================================================================
//==
//==  operator BOS_SOCKET_ADDR()*
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a BOS_SOCKET_ADDR pointer.
//
//==EDOC========================================================================
inline
CSocketAddr::operator BOS_SOCKET_ADDR*()
{
    return reinterpret_cast<BOS_SOCKET_ADDR*>(&m_stSockAddr);
}

//==SDOC========================================================================
//==
//==  operator BOS_SOCKET_ADDR()*
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a const BOS_SOCKET_ADDR pointer.
//
//==EDOC========================================================================
inline
CSocketAddr::operator const BOS_SOCKET_ADDR*()
{
    return reinterpret_cast<const BOS_SOCKET_ADDR*>(&m_stSockAddr);
}
#endif
// BRCM_CUSTOM - [end] Add ECOS support.

#if defined(MXD_OS_SYMBIAN)
inline CSocketAddr::operator TInetAddr*() const
{
    // IPv6 address should never be used by Symbian OS methods.
    // It's not supported.
    MX_ASSERT(m_eFamily == eINET);
    return const_cast<TInetAddr*>(&m_stSockAddr);
}

#else
//==SDOC========================================================================
//==
//==  operator sockaddr() const
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a const sockaddr.
//
//==EDOC========================================================================
inline CSocketAddr::operator sockaddr() const
{
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    return *reinterpret_cast<const sockaddr*>(&m_stSockAddr);
#else
    // IPv6 address should never be used by OS methods.
    // It's not supported.
    MX_ASSERT(m_eFamily == eINET);
    return m_stSockAddr;
#endif
}

//==SDOC========================================================================
//==
//==  operator sockaddr*()
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a sockaddr pointer.
//
//==EDOC========================================================================
inline CSocketAddr::operator sockaddr*() const
{
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    return reinterpret_cast<sockaddr*>(const_cast<sockaddr_storage*>(&m_stSockAddr));
#else
    // IPv6 address should never be used by OS methods.
    // It's not supported.
    MX_ASSERT(m_eFamily == eINET);
    return const_cast<sockaddr*>(&m_stSockAddr);
#endif
}

//==SDOC========================================================================
//==
//==  operator sockaddr*() const
//==
//==============================================================================
//
//  Description:
//      Casts the current object into a const sockaddr pointer.
//
//==EDOC========================================================================
inline CSocketAddr::operator const sockaddr*() const
{
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    return reinterpret_cast<const sockaddr*>(&m_stSockAddr);
#else
    // IPv6 address should never be used by OS methods.
    // It's not supported.
    MX_ASSERT(m_eFamily == eINET);
    return &m_stSockAddr;
#endif
}
#endif// defined(MXD_OS_SYMBIAN)

//==SDOC========================================================================
//==
//==  GetSize
//==
//==============================================================================
//
//  Returns:
//      Size of the socket address structure.
//
//  Description:
//      Gets the size of the socket address structure. Note that if
//      MXD_IPV6_ENABLE_SUPPORT is not defined, GetSize will always return the
//      size of an IPv4 address even if it contains an IPv6 address.
//
//==EDOC========================================================================
inline unsigned int CSocketAddr::GetSize() const
{
#if defined(MXD_OS_SYMBIAN)
    return sizeof(SSockAddr) + const_cast<CSocketAddr*>(this)->m_stSockAddr.GetUserLen();
#else
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    if (GetFamily() == eINET6)
    {
        return sizeof(sockaddr_in6);
    }
    else
#endif
    {
        return sizeof(sockaddr_in);
    }
#endif //defined(MXD_OS_SYMBIAN)
}

//==SDOC========================================================================
//==
//==  GetFamily
//==
//==============================================================================
//
//  Returns:
//      The family type of the socket address.
//
//  Description:
//      Gets the family type of the socket address.
//
//==EDOC========================================================================
inline CSocketAddr::EAddressFamily CSocketAddr::GetFamily() const
{
    return m_eFamily;
}

//==SDOC========================================================================
//==
//==  SetPort
//==
//==============================================================================
//
//  Parameters:
//      uPort:
//          Port number.
//
//  Description:
//      Sets the port.
//
//==EDOC========================================================================
inline void CSocketAddr::SetPort(IN uint16_t uPort)
{
    SetSinPort(uPort);
}

//==SDOC========================================================================
//==
//==  GetPort
//==
//==============================================================================
//
//  Returns:
//      The port associated to this socket address.
//
//  Description:
//      Gets the port associate to this socket address.
//
//==EDOC========================================================================
inline uint16_t CSocketAddr::GetPort() const
{
    return m_uPort;
}

//==SDOC========================================================================
//==
//==  operator==
//==
//==============================================================================
//
//  Parameters:
//      rFrom:
//          Reference to the address to compare.
//
//  Returns:
//      True if both addresses are equal, false otherwise.
//
//  Description:
//      Verifies is both addresses are equal.
//
//==EDOC========================================================================
inline bool CSocketAddr::operator==(IN const CSocketAddr& rFrom) const
{
    return IsEqualAddress(rFrom) && IsEqualPort(rFrom) && IsEqualScopeId(rFrom);
}

//==SDOC========================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Parameters:
//      rFrom:
//          Reference to the socket address to compare with.
//
//  Returns:
//      True if the two socket addresses are different, false otherwise.
//
//  Description:
//      Verifies if the two socket addresses are different.
//
//==EDOC========================================================================
inline bool CSocketAddr::operator!=(IN const CSocketAddr& rFrom) const
{
    return !(rFrom == *this);
}

//==SDOC========================================================================
//==
//==  IsEqualFamily
//==
//==============================================================================
//
//  Parameters:
//      rFrom:
//          Reference to the socket address to compare with.
//
//  Returns:
//      True if the two families are different, false otherwise.
//
//  Description:
//      Verifies if the two socket address families are different.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsEqualFamily(IN const CSocketAddr& rFrom) const
{
    return (GetFamily() == rFrom.GetFamily());
}

//==SDOC========================================================================
//==
//==  IsEqualPort
//==
//==============================================================================
//
//  Parameters:
//      rFrom:
//          Reference to the socket address to compare with.
//
//  Returns:
//      True if the two ports are equal, false otherwise.
//
//  Description:
//      Verifies if the two ports are equal.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsEqualPort(IN const CSocketAddr& rFrom) const
{
    return (GetPort() == rFrom.GetPort());
}

//==SDOC========================================================================
//==
//==  IsEqualScopeId
//==
//==============================================================================
//
//  Parameters:
//      rFrom:
//          Reference to the socket address to compare with.
//
//  Returns:
//      True if the two scope ID are equal, false otherwise.
//
//  Description:
//      Verifies if the two scope ID are equal.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsEqualScopeId(IN const CSocketAddr& rFrom) const
{
    return (GetScopeId() == rFrom.GetScopeId());
}

//==SDOC========================================================================
//==
//==  IsValid
//==
//==============================================================================
//
//  Returns:
//      True if the socket address,port and family are valid, false otherwise.
//
//  Description:
//      Verifies if the IP address, port and family are valid.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsValid() const
{
    return (IsValidFamily() && IsValidAddress() && IsValidPort());
}

//==SDOC========================================================================
//==
//==  IsValidPort
//==
//==============================================================================
//
//  Returns:
//      True if the IP port is valid, false otherwise.
//
//  Description:
//      Verifies if the IP port is valid.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsValidPort() const
{
    return (GetPort() != 0);
}

//==SDOC========================================================================
//==
//==  IsValidFamily
//==
//==============================================================================
//
//  Returns:
//      True if the address family valid, false otherwise.
//
//  Description:
//      Verifies if the address family is valid.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsValidFamily() const
{
    if (GetFamily() == eINET6 ||
        GetFamily() == eINET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//==SDOC========================================================================
//==
//==  IsInetAddressLoopback
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv4 loopback, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv4 loopback.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInetAddressLoopback() const
{
    return (GetFamily() == eINET &&
            memcmp(&gs_uINET_ADDR_LOOPBACK,
                   &m_auAddr[0],
                   g_uSIZE_OF_IPV4_ADDRESS) == 0);
}

//==SDOC========================================================================
//==
//==  IsInet6AddressUnspecified
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 unspecified, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv6 unspecified.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressUnspecified() const
{
    return (GetFamily() == eINET6 &&
            memcmp(gs_uINET6_ADDR_UNSPECIFIED, m_auAddr, g_uSIZE_OF_IPV6_ADDRESS) == 0);
}

//==SDOC========================================================================
//==
//==  IsInet6AddressLoopback
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 loopback, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv6 loopback.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressLoopback() const
{
    return (GetFamily() == eINET6 &&
            memcmp(gs_uINET6_ADDR_LOOPBACK, m_auAddr, g_uSIZE_OF_IPV6_ADDRESS) == 0);
}

//==SDOC========================================================================
//==
//==  IsInet6AddressMulticast
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 multicast, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv6 multicast.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressMulticast() const
{
    return (GetFamily() == eINET6) &&
           m_auAddr[0] == 0xFF;
}

//==SDOC========================================================================
//==
//==  IsInet6AddressLinkLocal
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 link-local, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv6 link-local.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressLinkLocal() const
{
    return (GetFamily() == eINET6) &&
           m_auAddr[0] == 0xFE &&
           (m_auAddr[1] & 0xC0) == 0x80;
}

//==SDOC========================================================================
//==
//==  IsInet6AddressSiteLocal
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 site-local, false otherwise.
//
//  Description:
//      Verifies if the address is of type IPv6 site-local.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressSiteLocal() const
{
    return (GetFamily() == eINET6) &&
           m_auAddr[0] == 0xFE &&
           (m_auAddr[1] & 0xC0) == 0xC0;
}

//==SDOC========================================================================
//==
//==  IsInet6AddressV4Mapped
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type V4-mapped IPv6 , false otherwise.
//
//  Description:
//      Verifies if the address is of type V4-mapped IPv6.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressV4Mapped() const
{
    return (GetFamily() == eINET6) &&
           m_auAddr[0] == 0 &&
           m_auAddr[1] == 0 &&
           m_auAddr[2] == 0 &&
           m_auAddr[3] == 0 &&
           m_auAddr[4] == 0 &&
           m_auAddr[5] == 0 &&
           m_auAddr[6] == 0 &&
           m_auAddr[7] == 0 &&
           m_auAddr[8] == 0 &&
           m_auAddr[9] == 0 &&
           m_auAddr[10] == 0xFF &&
           m_auAddr[11] == 0xFF;
}

//==SDOC========================================================================
//==
//==  IsInet6AddressGlobal
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 global , false otherwise.
//
//  Description:
//      Verifies if the address is an IPv6 global address.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressGlobal() const
{
    return (GetFamily() == eINET6) &&
           (!IsInet6AddressUnspecified() && !IsInet6AddressLoopback()  &&
            !IsInet6AddressMulticast()   && !IsInet6AddressLinkLocal() &&
            !IsInet6AddressSiteLocal()   && !IsInet6AddressV4Mapped());
}

//==SDOC========================================================================
//==
//==  IsInet6AddressUniqueLocal
//==
//==============================================================================
//
//  Returns:
//      True if the address is of type IPv6 unique local , false otherwise.
//
//  Description:
//      Verifies if the address is an IPv6 unique local address, as defined in
//  RFC4193. IPv6 unique local addresses have the prefix FC00::/7.
//
//==EDOC========================================================================
inline bool CSocketAddr::IsInet6AddressUniqueLocal() const
{
    return (GetFamily() == eINET6) &&
           (m_auAddr[0] & 0xFE) == 0xFC;
}

//==SDOC========================================================================
//==
//==  SetAddress
//==
//==============================================================================
//
//  Parameters:
//      rAddress:
//          Reference to the socket address to set.
//
//  Description:
//      Sets a socket address to this one.
//
//==EDOC========================================================================
inline mxt_result CSocketAddr::SetAddress(IN const CSocketAddr& rAddress)
{
    *this = rAddress;
    return resS_OK;
}

//==SDOC========================================================================
//==
//==  InetAnyAddress
//==
//==============================================================================
//
//  Returns:
//      A socket address of the INET family and type any.
//
//  Description:
//      Creates a socket address of the INET family of the any type.
//
//==EDOC========================================================================
inline const CSocketAddr& CSocketAddr::InetAnyAddress()
{
    return *ms_pSockInetAnyAddress;
}

//==SDOC========================================================================
//==
//==  InetBroadcastAddress
//==
//==============================================================================
//
//  Returns:
//      A socket address of the INET family and type broadcast.
//
//  Description:
//      Creates a socket address of the INET family of the broadcast type.
//
//==EDOC========================================================================
inline const CSocketAddr& CSocketAddr::InetBroadcastAddress()
{
    return *ms_pSockInetBroadcastAddress;
}

//==SDOC========================================================================
//==
//==  Inet6AnyAddress
//==
//==============================================================================
//
//  Returns:
//      A socket address of the INET6 family and type any.
//
//  Description:
//      Creates a socket address of the INET6 family of the any type.
//
//==EDOC========================================================================
inline const CSocketAddr& CSocketAddr::Inet6AnyAddress()
{
    return *ms_pSockInet6AnyAddress;
}

//==============================================================================
//====  PRIVATE FUNCTIONS  =====================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
inline void CSocketAddr::SetSinPort(IN uint16_t uPort)
{
    m_uPort = uPort;
    ConvertToOsSpecific();
}

inline void CSocketAddr::SetSinFamily(IN EAddressFamily eFamily)
{
    m_eFamily = eFamily;
    ConvertToOsSpecific();
}

// BRCM_CUSTOM - [mod] Add ECOS support.
#if defined(MXD_OS_VXWORKS) || defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] Add ECOS support.
inline void CSocketAddr::SetSinLength()
{
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    if (GetFamily() == eINET6)
    {
// BRCM_CUSTOM - [mod] change to sockaddr_in6 see BCR21235
        //reinterpret_cast<sockaddr_in6*>(&m_stSockAddr)->sin6_len = sizeof(m_stSockAddr);
        reinterpret_cast<sockaddr_in6*>(&m_stSockAddr)->sin6_len = sizeof(sockaddr_in6);
// BRCM_CUSTOM - [end] change to sockaddr_in6 see BCR21235
    }
    else
#endif
    {
        // We must explicitely put the value of sin_len to sizeof(sockaddr_in),
        // even if we really use sockaddr_storage. Otherwise, getnameinfo
        // complains that it does not recognise the family of the address.
        //----------------------------------------------------------------------
        reinterpret_cast<sockaddr_in*>(&m_stSockAddr)->sin_len = sizeof(sockaddr_in);
    }
}
// BRCM_CUSTOM - [mod] Add ECOS support.
#endif //defined(MXD_OS_VXWORKS) || defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] Add ECOS support.

inline uint32_t CSocketAddr::GetSinAddress() const
{
    return *(reinterpret_cast<const mxt_UNALIGNED_UINT32*>(&m_auAddr[0]));
}

inline const uint8_t* CSocketAddr::GetSinAddressByteArray(OUT unsigned int* puAddressSize) const
{
    if (GetFamily() == eINET6)
    {
        *puAddressSize = g_uSIZE_OF_IPV6_ADDRESS;
    }
    else
    {
        *puAddressSize = g_uSIZE_OF_IPV4_ADDRESS;
    }
    return &m_auAddr[0];
}

inline CSocketAddr::EAddressFamily CSocketAddr::DetectFamilyFromString(IN const char* pszAddress) const
{
    // We search for a ':' in pszAddress. If one is found, the address is
    // considered to be in IPv6 format. Otherwise, the address is is considered
    // to be in IPv4 format.
    if (strchr(pszAddress, ':') != NULL)
    {
        return eINET6;
    }
    else
    {
        return eINET;
    }
}

//==============================================================================
//==
//==  IsInetAddressMulticast
//==
//==============================================================================
//
//  Returns:
//      True if the address is an IPv4 multicast address, false otherwise.
//
//  Description:
//      Verifies if the address is an IPv4 multicast address.
//
//==============================================================================
inline bool CSocketAddr::IsInetAddressMulticast() const
{
    return (m_eFamily == eINET && m_auAddr[0] >= 224 && m_auAddr[0] <= 239);
}

//==============================================================================
//====  MARSHALING METHODS  ====================================================
//==============================================================================

inline CMarshaler& operator<< (IN CMarshaler& rMarshaler, IN const CSocketAddr& rData)
{
    rMarshaler.Store(&rData.m_auAddr, sizeof(rData.m_auAddr));
    rMarshaler.Store(&rData.m_eFamily, sizeof(rData.m_eFamily));
    rMarshaler.Store(&rData.m_uPort, sizeof(rData.m_uPort));
    rMarshaler.Store(&rData.m_uScopeId, sizeof(rData.m_uScopeId));
    return rMarshaler;
}

inline CMarshaler& operator>> (IN CMarshaler& rMarshaler, IN CSocketAddr& rData)
{
    rMarshaler.Load(&rData.m_auAddr, sizeof(rData.m_auAddr));
    rMarshaler.Load(&rData.m_eFamily, sizeof(rData.m_eFamily));
    rMarshaler.Load(&rData.m_uPort, sizeof(rData.m_uPort));
    rMarshaler.Load(&rData.m_uScopeId, sizeof(rData.m_uScopeId));
    rData.ConvertToOsSpecific();
    return rMarshaler;
}

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CSOCKETADDR_H
