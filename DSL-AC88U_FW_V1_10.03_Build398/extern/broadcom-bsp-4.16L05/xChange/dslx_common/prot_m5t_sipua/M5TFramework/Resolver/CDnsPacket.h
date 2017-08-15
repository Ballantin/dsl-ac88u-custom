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
#ifndef MXG_CDNSPACKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CDNSPACKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_ENUM_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_RESOLVER_CACHE_ENABLE_SUPPORT
#endif

#if defined (MXD_CAP_ENABLE_SUPPORT) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifndef MXG_CPAIR_H
#include "Cap/CPair.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CUNCMP_H
#include "Cap/CUncmp.h"
#endif

#ifndef MXG_CTIMER_H
#include "Kernel/CTimer.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CResolverCache;
class CHostFile;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CDnsPacket
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   This class abstracts a DNS packet.
//
// Description:
//   This class abstracts a DNS packet.
//
// Location:
//   Resolver/CDnsPacket.h
//
//==============================================================================
class CDnsPacket
{
//M5T_INTERNAL_USE_BEGIN
//-- Friend Declarations
friend class CResolverCache;
friend class CHostFile;

//-- New types within class namespace
public:
    // RFC 1035 supported class.
    enum EClass
    {
        eIN = 1
    };

    // RFC 1035 supported types.
    enum EType
    {
        eUNKNOWN = 0,
        eA = 1,
        eNS = 2,
        eCNAME = 5,
        eSOA = 6,
        ePTR = 12,
        eAAAA = 28,
        eSRV = 33,
        eNAPTR = 35
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                                               |
    // /                                               /
    // /                      NAME                     /
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                      TYPE                     |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     CLASS                     |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                      TTL                      |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                   RDLENGTH                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    // /                     RDATA                     /
    // /                                               /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    struct SRecord
    {
        CString m_strName;
        EType m_eType;
        // Even though the TTL is a 32-bit value, we store the uptime when the
        // record will be considered expired. Therefore, we need to store this
        // uptime in a 64-bit variable.
        uint64_t m_uTtl;
        EClass m_eClass;
        uint16_t m_uRecordDataLength;
        void* m_pvoidRecordData;
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                   PRIORITY                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    WEIGHT                     |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     PORT                      |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                                               |
    // /                    TARGET                     /
    // /                                               /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    struct SSrvRecordData
    {
        uint16_t m_uPriority;
        uint16_t m_uWeight;
        uint16_t m_uPort;
        CString m_strTarget;
        CList<CSocketAddr> m_lstAddresses;
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     ORDER                     |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                   PREFERENCE                  |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                     FLAGS                     /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                   SERVICES                    /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                    REGEXP                     /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                  REPLACEMENT                  /
    // /                                               /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    struct SNaptrRecordData
    {
        bool operator!= (IN const SNaptrRecordData& rSrc) const;
        bool operator< (IN const SNaptrRecordData& rSrc) const;

        uint16_t m_uOrder;
        uint16_t m_uPreference;
        CString m_strFlags;
        CString m_strServices;
        CString m_strRegexp;
        CString m_strReplacement;
        CList<SSrvRecordData> m_lstSrvRecord;
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                     MNAME                     /
    // /                                               /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // /                     RNAME                     /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    SERIAL                     |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    REFRESH                    |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     RETRY                     |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    EXPIRE                     |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    MINIMUM                    |
    // |                                               |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    struct SSoaRecordData
    {
        CString m_strMname;
        CString m_strRname;
        uint32_t m_uSerial;
        int32_t m_nRefresh;
        int32_t m_nRetry;
        int32_t m_nExpire;
        uint32_t m_uMin;
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                                               |
    // /                     QNAME                     /
    // /                                               /
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     QTYPE                     |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                     QCLASS                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    struct SQuestion
    {
        CString m_strName;
        EType m_eType;
        EClass m_eClass;
    };

    //                                 1  1  1  1  1  1
    //   0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                      ID                       |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    QDCOUNT                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    ANCOUNT                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    NSCOUNT                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    // |                    ARCOUNT                    |
    // +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

    #include "Basic/MxPack.h"
    struct SHeader
    {
        uint16_t m_uId MX_PACK_MEMBER;

#if defined(MXD_BIG_ENDIAN)
        uint16_t m_uQr     : 1,
                 m_uOpcode : 4,
                 m_uAa     : 1,
                 m_uTc     : 1,
                 m_uRd     : 1,
                 m_uRa     : 1,
                 m_uZ      : 3,
                 m_uRcode  : 4  MX_PACK_MEMBER;
#else
        uint16_t m_uRd     : 1,
                 m_uTc     : 1,
                 m_uAa     : 1,
                 m_uOpcode : 4,
                 m_uQr     : 1,
                 m_uRcode  : 4,
                 m_uZ      : 3,
                 m_uRa     : 1  MX_PACK_MEMBER;
#endif
        uint16_t m_uQdCount  MX_PACK_MEMBER;
        uint16_t m_uAnCount  MX_PACK_MEMBER;
        uint16_t m_uNsCount  MX_PACK_MEMBER;
        uint16_t m_uArCount  MX_PACK_MEMBER;
    } MX_PACK_STRUCT;
    #include "Basic/MxPack.h"
//M5T_INTERNAL_USE_END
#if defined (MXD_ENUM_ENABLE_SUPPORT)
    // Abstraction of an ENUM service and its supported sub-services.
    struct SEnumServiceData
    {
        // The service.
        CString m_strEnumService;
        // The sub-services supported by the service.
        CList<CString> m_lststrEnumSubTypes;
    };

    // Abstraction of an URI returned by an ENUM query.
    struct SEnumUriData
    {
        // The ENUM URI.
        CString m_strUri;
        // The list of ENUM services supported by the URI.
        CList<SEnumServiceData> m_lststEnumServices;
    };
#endif //-- #if defined (MXD_ENUM_ENABLE_SUPPORT)

//-- Published Interface
public:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CDnsPacket();

    // Summary:
    //  Copy Constructor.
    CDnsPacket(IN const CDnsPacket& rSrc);

    // Summary:
    //  Assignment Operator.
    CDnsPacket& operator=(IN const CDnsPacket& rSrc);

    // Summary:
    //  Destructor.
    virtual ~CDnsPacket();

    // Summary:
    //   Copies the records from an existing packet.
    void UpdateRecords(IN const CDnsPacket& rSrc);

    // Summary:
    //  Release the provided record.
    static void ReleaseRecord(INOUT SRecord* pstRecord);

#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    //-- << Cache operations >>.

    // Summary:
    //  Release expired records.
    void ReleaseExpiredRecords();

    // Summary:
    //  Computes the provided record TTL.
    static void ComputeRecordTtl(INOUT SRecord& rstRecord, IN uint64_t uTtlBaselineMs);
#endif

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //   Releases the packet structures.
    void ReleasePacket();

    // Summary:
    //  Removes all the unresolved records data for the provided type.
    void RemoveUnresolvedRecordData(IN EType eRecordType, IN CString& rstrRecordData);

#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    //-- << Cache operations >>.

    // Summary:
    //   Verifies if the questions are the same.
    int CompareQuestions(IN const SQuestion& rstQuery) const;

    // Summary:
    //   Verifies if the packet TTL is expired.
    mxt_result GetExpirationStatus();

    // Summary:
    //  This method returns whether or not the provided record is expired.
    static bool IsRecordExpired(IN const SRecord& rstRecord);

#endif //-- #if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)

//-- Published Data Members
public:
    // The header.
    SHeader m_stHeader;
    // The list of questions.
    CList<SQuestion*> m_lstpstQuestions;
    // The list of answers.
    CList<SRecord*> m_lstpstAnswers;
    // The list of authority records.
    CList<SRecord*> m_lstpstAuthorityRecords;
    // The list of additional records.
    CList<SRecord*> m_lstpstAdditionalRecords;

    //-- << RFC 1035 size limits >>

    // Maximum label length.
    static const unsigned int ms_uMAX_LABEL_SIZE;
    // Maximum domain name length.
    static const unsigned int ms_uMAX_DOMAIN_NAME_SIZE;

    //-- << Handled rcode values (other registered rcodes refer to DNSSEC) >>
    static const uint8_t ms_uNOERROR;  // No Error                           [RFC1035]
    static const uint8_t ms_uFORMERR;  // Format Error                       [RFC1035]
    static const uint8_t ms_uSERVFAIL; // Server Failure                     [RFC1035]
    static const uint8_t ms_uNXDOMAIN; // Non-Existent Domain                [RFC1035]
    static const uint8_t ms_uNOTIMP;   // Not Implemented                    [RFC1035]
    static const uint8_t ms_uREFUSED;  // Query Refused                      [RFC1035]
    static const uint8_t ms_uYXDOMAIN; // Name Exists when it should not     [RFC2136]
    static const uint8_t ms_uYXRRSET;  // RR Set Exists when it should not   [RFC2136]
    static const uint8_t ms_uNXRRSET;  // RR Set that should exist does not  [RFC2136]
    static const uint8_t ms_uNOTAUTH;  // Server Not Authoritative for zone  [RFC2136]
    static const uint8_t ms_uNOTZONE;  // Name not contained in zone         [RFC2136]

#if defined (MXD_ENUM_ENABLE_SUPPORT)
    //-- << ENUM >>.
    CList<SEnumUriData> m_lststEnumUris;
#endif

//-- Hidden Data Members
protected:
#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    //-- << Cache operations >>.
    CUncmp<CDnsPacket>* m_pNewerItem;
    CUncmp<CDnsPacket>* m_pOlderItem;
#endif
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==============================================================================
//==
//==  operator!=
//==
//==============================================================================
//
//  Summary:
//      Operator!=
//
//  Parameters:
//      rLhs:
//       left-hand side of the operator.
//
//      rRhs:
//       right-hand side of the operator.
//
//  Returns:
//      true if different, false otherwise.
//
//  Description:
//      Operator used for sorting as per RFC 3402. Hence, only the priority and
//      weight fields are used for the comparison.
//
//==============================================================================
inline
bool CDnsPacket::SNaptrRecordData::operator!= (IN const SNaptrRecordData& rSrc) const
{
    return m_uOrder != rSrc.m_uOrder || m_uPreference != rSrc.m_uPreference;
}

//==============================================================================
//==
//==  operator<
//==
//==============================================================================
//
//  Summary:
//      Operator<
//
//  Parameters:
//      rLhs:
//       left-hand side of the operator.
//
//      rRhs:
//       right-hand side of the operator.
//
//  Returns:
//      true if smaller, false otherwise.
//
//  Description:
//      Operator used for sorting as per RFC 3402. Hence, only the order and
//      preference fields are used for the comparison.
//
//==============================================================================
inline
bool CDnsPacket::SNaptrRecordData::operator< (IN const SNaptrRecordData& rSrc) const
{
    return m_uOrder < rSrc.m_uOrder || m_uPreference < rSrc.m_uPreference;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_CAP_ENABLE_SUPPORT) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CDNSPACKET_H


