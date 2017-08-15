//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_NETWORKTOOLS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_NETWORKTOOLS_H
//M5T_INTERNAL_USE_END

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
    #include "Config/FrameworkCfg.h" // MXD_IPV6_ENABLE_SUPPORT
                                     // MXD_ENUM_ENABLE_SUPPORT
                                     // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Data Members
//-----------------
#ifndef MXG_CLIST_H
    #include "Cap/CList.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_SOCKETDEFINES_H
    #include "Network/SocketDefines.h"
#endif

#ifndef MXG_CSOCKETADDR_H
    #include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CRESOLVER_H
#include "Resolver/CResolver.h"
#endif

// Forward Declarations Inside of the Namespace

MX_NAMESPACE_START(MXD_GNS)
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
// The structure reprensenting a DNS packet header.
struct SResolverStandardHeader
{
    uint16_t id;           //-- 16 bit ID assigned by the query generator.
#if defined(MXD_BIG_ENDIAN)
    uint16_t qr     : 1,   //-- Query(0) - Response(1).
             opcode : 4,   //-- Kind of query.  Only 0 supported for this version.
             aa     : 1,   //-- Authorative Answer bit.
             tc     : 1,   //-- Truncation bit.
             rd     : 1,   //-- Recursion desired bit.
             ra     : 1,   //-- Recursion available bit.
             z      : 3,   //-- Reserved. MUST be 0.
             rcode  : 4;   //-- Response code.
#else
    uint16_t rd     : 1,   //-- Recursion desired bit.
             tc     : 1,   //-- Truncation bit.
             aa     : 1,   //-- Authorative Answer bit.
             opcode : 4,   //-- Kind of query.  Only 0 supported for this version.
             qr     : 1,   //-- Query(0) - Response(1).
             rcode  : 4,   //-- Response code.
             z      : 3,   //-- Reserved. MUST be 0.
             ra     : 1;   //-- Recursion available bit.
#endif
    uint16_t qdcount;      //-- Number of entries in the query section.
    uint16_t ancount;      //-- Number of RR in the answer section.
    uint16_t nscount;      //-- Number of NS-RR in the authority records section.
    uint16_t arcount;      //-- Number of RR in the additionnal records section.
};

// The structure representing an SRV record.
struct SSrvRecord
{
    uint16_t uPriority;
    uint16_t uWeight;
    uint16_t uPort;

    // The target for this request.
    CString strTarget;

    // The address records found in the Additional Data section of the SRV
    // response.
    CList<CSocketAddr> lstSocketAddr;
};

// The structure representing a NAPTR record.
struct SNaptrRecord
{
    // The order, priority, flags and services for this record.
    uint16_t uOrder;

    // Equivalent to Priority in DDDS algorithm.
    uint16_t uPreference;

    // A single character string.
    CString strFlags;
    CString strServices;

    // Regular expression to be used in DDDS
    CString strRegexp;

    // Domain name to be used in next request: ex SRV record query string.
    CString strReplacement;

    // The SRV records found in the Additional Data section of the NAPTR
    // response.
    CList<SSrvRecord> lstSrvRecord;
};

// The structure representing an SOA record.
struct SSoaRecord
{
    CString  strMname;
    CString  strRname;
    uint32_t uSerial;
    int32_t  nRefresh;
    int32_t  nRetry;
    int32_t  nExpire;
    uint32_t uMin;
};

// The structure representing any RR.
struct SRecordCommon
{
    char     szName[256];
    int16_t  nNameLength;
    uint16_t uType;
    uint16_t uClass;
    int32_t  nTtl;
    uint16_t uDataLength;
};

// The structure representing a question.
struct SQuestion
{
    char     szName[256];
    uint16_t uType;
    uint16_t uClass;
};
//M5T_INTERNAL_USE_END

//<GROUP NETWORK_STRUCTS>
//
// Summary:
//  Structure representing an enumservice and its supported sub-services.
struct SEnumService
{
    // Description:
    // The enumservice.
    CString m_strEnumService;
    // Description:
    // Sub-services supported by the enumservice.
    CList<CString> m_lststrEnumSubTypes;
};

//<GROUP NETWORK_STRUCTS>
//
// Summary:
//  Structure representing an URI returned by an ENUM request.
struct SEnumUri
{
    // Description:
    // CString representing the URI.
    CString m_strUri;
    // Description:
    // List of enumservices supported by the URI.
    CList<SEnumService> m_lststEnumServices;
};

//==SDOC====================================================================
//==
//==  SLocalIpAddress
//==
//==========================================================================
//<GROUP NETWORK_STRUCTS>
//
//  Summary:
//      Structure to store all the local IP addresses and their associated
//      interface name for the local machine.
//
//  Description:
//      This structure is used to store all the local IP addresses and their
//      associated interface name for the local machine.
//
//==EDOC====================================================================
struct SLocalIpAddress
{
    // Description:
    // A CSocketAddr to keep the IP address.
    CSocketAddr m_addrLocal;
    // Description:
    // A CString to store the interface name.
    CString m_strInterfaceName;
};

// Mechanism to permit the stubbing of NetworkTools.
//---------------------------------------------------
#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_NETWORKTOOLS)
MX_NAMESPACE_END(MXD_GNS)
    #ifndef MXG_NETWORKTOOLSSTUB_H
        #include "NetworkStubs/NetworkToolsStub.h"
    #endif
#else

//==============================================================================
//====  NAME RESOLUTION  =======================================================
//==============================================================================

#if defined(MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
//M5T_INTERNAL_USE_BEGIN
int ResolverSetServerList(IN const CList<CSocketAddr>& rlstServers);
//M5T_INTERNAL_USE_END
#endif

// Summary:
//  Retrieves all local IPv4 and IPv6 addresses.
//----------------------------------------------
mxt_result GetAllLocalIpAddresses(OUT CVector<SLocalIpAddress>* pvecAllLocalIpAddresses);

// Summary:
//  Retrieves the local address used to connect to the specied remote peer.
//--------------------------------------------------------------------------
CSocketAddr GetLocalIPAddr(IN const CSocketAddr& rPeerAddress = "0.0.0.0");

// Summary:
//  Retrieves the local IPv4 address used to connect to the specied remote peer.
//------------------------------------------------------------------------------
CSocketAddr GetLocalIPv4Addr(IN const CSocketAddr& rPeerAddress = "0.0.0.0");

#if defined(MXD_IPV6_ENABLE_SUPPORT)
// Summary:
//  Retrieves the local IPv6 address used to connect to the specied remote peer.
//------------------------------------------------------------------------------
CSocketAddr GetLocalIPv6Addr(IN const CSocketAddr& rPeerAddress = CSocketAddr("::"));
#endif // #if defined(MXD_IPV6_ENABLE_SUPPORT)

// Summary:
//  Get the hosts address using its name.
//----------------------------------------
bool GetHostByName(IN const char* pszHostName,
                   OUT CSocketAddr& rHostAddr,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                   IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                   IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// Summary:
//  Get the hosts address using its name.
//----------------------------------------
bool GetHostByName(IN const char* pszHostName,
                   OUT CList<CSocketAddr>& rlstHostAddr,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                   IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                   IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// Summary:
//  Get the hosts name using its address.
//----------------------------------------
bool GetHostByAddr(IN const CSocketAddr& rHostAddr,
                   IN unsigned int uBufLen,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                   OUT char* pszHostName,
                   IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// Summary:
//  Get the name of the local host.
//----------------------------------
bool GetLocalHostName(IN unsigned int nNameLen,
                      OUT char* pszName);

// Service and proto will not be prefixed by the _ that must be sent to the DNS

// Summary:
//  Queries hosts providing the desired service.
//-----------------------------------------------
bool GetSrvRecord(IN const char*  pszService,
                  IN const char*  pszProtocol,
                  IN const char*  pszDomainName,
                  OUT CList<SSrvRecord>& rlstSrvRecord,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                  IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                  IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// The name will be queried directely, without modification.  This is the case
// when using the replacement string in a NAPTR record, for example.

// Summary:
//  Queries the domain for a list of service records.
//----------------------------------------------------
bool GetSrvRecord(IN const char*  pszSrvQueryName,
                  OUT CList<SSrvRecord>& rlstSrvRecord,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                  IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                  IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

// Summary:
//  Queries the domain for a list of NAPTR records.
//----------------------------------------------------
bool GetNaptrRecord(IN const char* pszDomainName,
                    OUT CList<SNaptrRecord>& rlstNaptrRecord,
                    IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                    IN const CVector<CString>* pvecstrServices = NULL,
                    IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support

#if defined(MXD_ENUM_ENABLE_SUPPORT)
// Summary:
//  Generates an ENUM request for the specified services and E.164 number.
//-------------------------------------------------------------------------
mxt_result GetEnumUri(IN const char* pszE164Number,
                      IN const CList<SEnumService>& rlstSupportedServices,
                      OUT CList<SEnumUri>& rlstEnumUri,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                      IN const char* pszZone = NULL,
                      IN mxt_opaque opqId = 0);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
#endif

//==============================================================================
//====  PROTOCOL AND SERVICE RESOLUTION  =======================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // defined(MXD_ENABLE_STUBBING) &&
       // defined(MXD_STUB_NETWORKTOOLS)


#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_NETWORKTOOLS_H
