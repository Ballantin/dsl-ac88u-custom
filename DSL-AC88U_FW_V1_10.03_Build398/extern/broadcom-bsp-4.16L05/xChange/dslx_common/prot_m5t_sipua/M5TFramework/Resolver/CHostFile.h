//==============================================================================
//==============================================================================
//
//   Copyright(c) 2009 Media5 Corporation ("Media5")
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
#ifndef MXG_CHOSTFILE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CHOSTFILE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if defined (MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CAATREE_H
#include "Cap/CAATree.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CDNSPACKET_H
#include "Resolver/CDnsPacket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CAsyncResolver;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CHostFile
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   This class servers as a local host file.
//
// Description:
//   Implements a local host file. This host file is used to configure DNS 
//   records locally when it is impossible to do so on the DNS server.
//
//   This is different from the resolver cache as this is static. Once an item
//   is added / removed it is done so for the lifetime of the 
//   resolver. The TTL value of all host file records is hardcoded to 
//   0xFFFFFFFF.
//
//   The host file record has precedence over the DNS cache and the resolver.
//   All types of queries can be configured into the host file. Queries are
//   configured step by step.
//
// Example:
//      To configure a NAPTR record containing 2 SRV records with each a single 
//      address:
//
//  <CODE>
//
//    CAsyncResolver* pResolver = CAsyncResolver::GetInstance();
//    CHostFile* pHostFile = pResolver->GetHostFile();
//
//    MX_TFW_ASSERT(pHostFile != NULL);
//
//    // Add NAPTR records.
//    CString strNaptrQuestion = "www.private.mediacorp.com";
//    CVector<CDnsPacket::SNaptrRecordData> vecstNaptrRecord;
//    CDnsPacket::SNaptrRecordData stNaptrRecordDataTcp;
//    stNaptrRecordDataTcp.m_uOrder = 1;
//    stNaptrRecordDataTcp.m_uPreference = 1;
//    stNaptrRecordDataTcp.m_strServices = "SIP+D2T";
//    stNaptrRecordDataTcp.m_strFlags = "S";
//    stNaptrRecordDataTcp.m_strReplacement = "_sip._tcp.private.mediacorp.com";
//    vecstNaptrRecord.Append(stNaptrRecordDataTcp);
//
//    // Build 2nd NAPTR record.
//    CDnsPacket::SNaptrRecordData stNaptrRecordDataUdp;
//    stNaptrRecordDataUdp.m_uOrder = 2;
//    stNaptrRecordDataUdp.m_uPreference = 1;
//    stNaptrRecordDataUdp.m_strServices = SIP+D2U;
//    stNaptrRecordDataUdp.m_strFlags = "S";
//    stNaptrRecordDataUdp.m_strReplacement = "_sip._udp.private.mediacorp.com";
//    vecstNaptrRecord.Append(stNaptrRecordDataUdp);
//
//    mxt_result res = pHostFile->AddNamingAuthorityPointer(strNaptrQuestion, 
//                                                          vecstNaptrRecord);
//    CDnsPacket::SSrvRecordData stTcpRecordData;
//    stTcpRecordData.m_uPort = 5060;
//    stTcpRecordData.m_uPriority = 1;
//    stTcpRecordData.m_uWeight = 10;
//    stTcpRecordData.m_strTarget = "name.tcp.private.mediacorp.com";
//
//    res = pHostFile->AddService("_sip._tcp.private.mediacorp.com", 
//                                stTcpRecordData);
//    
//    CDnsPacket::SSrvRecordData stUdpRecordData;
//    stUdpRecordData.m_uPort = 5060;
//    stUdpRecordData.m_uPriority = 2;
//    stUdpRecordData.m_uWeight = 10;
//    stUdpRecordData.m_strTarget = "name.udp.private.mediacorp.com";
//
//    res = pHostFile->AddService("_sip._udp.private.mediacorp.com", 
//                                stUdpRecordData);
//
//    CVector<CString> vecstrAddresses;
//
//    vecstrAddresses.Append("127.0.0.1");
//    res = pHostFile->AddHostName("name.udp.private.mediacorp.com", 
//                                 vecstrAddresses, 
//                                 CSocketAddr::eINET);
//    vecstrAddresses.EraseAll();
//
//    vecstrAddresses.Append("127.0.0.2");
//    vecstrAddresses.Append("127.0.0.22");
//    res = pHostFile->AddHostName("_sip._tcp.private.mediacorp.com", 
//                                 vecstrAddresses, 
//                                 CSocketAddr::eINET);
//    vecstrAddresses.EraseAll();
//  </CODE>
//
// Location:
//   Resolver/CHostFile.h
//
// See Also:
//   CAsyncResolver
//
//==============================================================================
class CHostFile
{
//-- Friend Declarations
friend class CAsyncResolver;

//-- New types within class namespace
public:
protected:
private:

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    //-- << Stereotype >>

    // Summary:
    //  Adds a NAPTR record to the host file.
    mxt_result AddNamingAuthorityPointer(IN const CString& rstrDomain, 
                                         IN CVector<CDnsPacket::SNaptrRecordData>& rvecstAnswerServices);

    // Summary:
    //  Adds a SRV record to the host file.
    mxt_result AddService(IN const CString& rstrService, 
                          IN CDnsPacket::SSrvRecordData& rstSrvAnswer);

    // Summary:
    //  Adds a A / AAAA record to the host file.
    mxt_result AddHostName(IN const CString& rstrHost, 
                           IN CVector<CString>& rvecstrAnswerAddresses, 
                           IN CSocketAddr::EAddressFamily eFamily);

    // Summary:
    //  Removes a NAPTR record from the host file.
    mxt_result RemoveNamingAuthorityPointer(IN const CString& rstrDomain);

    // Summary:
    //  Removes a SRV record from the host file.
    mxt_result RemoveService(IN const CString& rstrService);

    // Summary:
    //  Removes a A / AAAA record from the host file.
    mxt_result RemoveHostName(IN const CString& rstrHost, IN CSocketAddr::EAddressFamily eFamily);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    CHostFile();

    // Summary:
    //  Destructor.
    virtual ~CHostFile();

    // Summary:
    //  Erases a DNS packet from the host file.
    mxt_result EraseDnsPacket(CDnsPacket& rPacketToErase);

    // Summary:
    //   Host file sorting method.
    static int Compare(IN const CUncmp<CDnsPacket>& rLhs, IN const CUncmp<CDnsPacket>& rRhs, IN mxt_opaque opq);

    // Summary:
    //   Queries the host file for a response.
    mxt_result Query(IN CDnsPacket::SQuestion& rstQuestion, OUT CDnsPacket*& rpResponse);

    // Summary:
    //  Updates the DNS packet.
    void UpdateDnsPacket(IN const CString& rstrQuestion,
                         IN CDnsPacket::EType eType,
                         IN unsigned int uAnswerCount,
                         OUT CDnsPacket& rPacket);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CHostFile(IN const CHostFile& rSrc);

    // Summary:
    //  Assignment Operator.
    CHostFile& operator=(IN const CHostFile& rSrc);


//-- Hidden Data Members
protected:
    CAATree<CUncmp<CDnsPacket> > m_hostFile;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT)
       //       defined(MXD_NETWORK_ENABLE_SUPPORT) && 
       //       defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && 
       //       defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CHOSTFILE_H
