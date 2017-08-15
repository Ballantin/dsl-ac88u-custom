//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_CWINDOWSRESOLVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CWINDOWSRESOLVER_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif

#if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    (defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE))

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "dnsapi.lib")
#include <windns.h>

#ifndef MXG_CRESOLVERCORE_H
#include "Resolver/CResolverCore.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CWindowsResolver
//==============================================================================
//
// Summary:
//   This is the Windows resolver core.
//
// Description:
//   This class implements the resolver core using the Windows DNS API.
//
// Location:
//   Resolver/CWindowsResolver.h
//
//==============================================================================
class CWindowsResolver : public CResolverCore
{
    //-- Friend Declarations
    friend class CAsyncResolver;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    CWindowsResolver();

    // Summary:
    //  Destructor.
    virtual ~CWindowsResolver();

    // -- << Windows API Resolver Core >>.
    mxt_result Parse(IN void* pBuffer, OUT CDnsPacket& rResponse);
    TO CSocketAddr* ParseARecord(IN void* pBuffer);
    TO CSocketAddr* ParseAAAARecord(IN void* pBuffer);
    TO CString* ParseCnameRecord(IN void* pBuffer);
    TO CDnsPacket::SNaptrRecordData* ParseNaptrRecord(IN void* pBuffer);
    TO CString* ParseNsRecord(IN void* pBuffer);
    TO CString* ParsePtrRecord(IN void* pBuffer);
    TO CDnsPacket::SSoaRecordData* ParseSoaRecord(IN void* pBuffer);
    TO CDnsPacket::SSrvRecordData* ParseSrvRecord(IN void* pBuffer);
    void Send(IN mxt_opaque opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CWindowsResolver(IN const CWindowsResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CWindowsResolver& operator=(IN const CWindowsResolver& rSrc);
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

inline
TO CSocketAddr* CWindowsResolver::ParseARecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    return MX_NEW(CSocketAddr)(reinterpret_cast<uint8_t*>(&pCurrentRecord->Data.A.IpAddress),
                               g_uSIZE_OF_IPV4_ADDRESS,
                               CSocketAddr::eINET);
}

inline
TO CSocketAddr* CWindowsResolver::ParseAAAARecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    return MX_NEW(CSocketAddr)(reinterpret_cast<uint8_t*>(&pCurrentRecord->Data.AAAA.Ip6Address),
                               g_uSIZE_OF_IPV6_ADDRESS,
                               CSocketAddr::eINET6,
                               0);
}

inline
TO CString* CWindowsResolver::ParseCnameRecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    return MX_NEW(CString)(pCurrentRecord->Data.CNAME.pNameHost);
}


inline
TO CString* CWindowsResolver::ParseNsRecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    return  MX_NEW(CString)(pCurrentRecord->Data.NS.pNameHost);
}

inline
TO CString* CWindowsResolver::ParsePtrRecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    return  MX_NEW(CString)(pCurrentRecord->Data.PTR.pNameHost);
}

inline
TO CDnsPacket::SSoaRecordData* CWindowsResolver::ParseSoaRecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    CDnsPacket::SSoaRecordData* pstSoaRecordData = MX_NEW(CDnsPacket::SSoaRecordData);

    pstSoaRecordData->m_strMname = pCurrentRecord->Data.SOA.pNamePrimaryServer;
    pstSoaRecordData->m_strRname = pCurrentRecord->Data.SOA.pNameAdministrator;
    pstSoaRecordData->m_uSerial = pCurrentRecord->Data.SOA.dwSerialNo;
    pstSoaRecordData->m_nRefresh = pCurrentRecord->Data.SOA.dwRefresh;
    pstSoaRecordData->m_nRetry = pCurrentRecord->Data.SOA.dwRetry;
    pstSoaRecordData->m_nExpire = pCurrentRecord->Data.SOA.dwExpire;
    pstSoaRecordData->m_uMin = pCurrentRecord->Data.SOA.dwDefaultTtl;

    return pstSoaRecordData;
}

inline
TO CDnsPacket::SSrvRecordData* CWindowsResolver::ParseSrvRecord(IN void* pBuffer)
{
    PDNS_RECORD pCurrentRecord = reinterpret_cast<PDNS_RECORD>(pBuffer);

    CDnsPacket::SSrvRecordData* pstSrvRecordData = MX_NEW(CDnsPacket::SSrvRecordData);

    pstSrvRecordData->m_uPriority = pCurrentRecord->Data.Srv.wPriority;
    pstSrvRecordData->m_uWeight = pCurrentRecord->Data.Srv.wWeight;
    pstSrvRecordData->m_uPort = pCurrentRecord->Data.Srv.wPort;
    pstSrvRecordData->m_strTarget = pCurrentRecord->Data.Srv.pNameTarget;

    // Addresses and filled in the async resolver.
    return pstSrvRecordData;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if !defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) &&
       //--     defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //--     defined(MXD_CAP_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //--     (defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE))

//M5T_INTERNAL_USE_END
#endif //-- #ifndef MXG_CWINDOWSRESOLVER_H


