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
#ifndef MXG_CASYNCRESOLVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CASYNCRESOLVER_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_CAP_ENABLE_SUPPORT
                                 // MXD_ENUM_ENABLE_SUPPORT
                                 // MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
                                 // MXD_IPV6_ENABLE_SUPPORT
                                 // MXD_RESOLVER_CACHE_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CDNSPACKET_H
#include "Resolver/CDnsPacket.h"
#endif

#if defined MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
    #ifndef MXG_CRESOLVER_H
    #include "Resolver/CResolver.h"
    #endif
    #ifndef MXG_CPORTABLERESOLVER_H
    #include "Resolver/CPortableResolver.h"
    #endif
#elif defined (MXD_OS_LINUX) || defined (MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN)
    #ifndef MXG_CUNIXRESOLVER_H
    #include "Resolver/CUnixResolver.h"
    #endif
#elif defined (MXD_OS_WINDOWS)
    #ifndef MXG_CWINDOWSRESOLVER_H
    #include "Resolver/CWindowsResolver.h"
    #endif
#elif defined (MXD_OS_VXWORKS)
    #ifndef MXG_CVXWORKSRESOLVER_H
    #include "Resolver/CVxWorksResolver.h"
    #endif
// BRCM_CUSTOM - [add] Add eCos support
#elif defined (MXD_OS_BOS_ECOS)
    #ifndef MXG_CBOSECOSRESOLVER_H
    #include "Custom/Resolver/CBosECosResolver.h"
    #endif
// BRCM_CUSTOM - [end] Add eCos support
#endif

#if defined MXD_RESOLVER_CACHE_ENABLE_SUPPORT
    #ifndef MXG_CRESOLVERCACHE_H
    #include "Resolver/CResolverCache.h"
    #endif
#endif

#if defined MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT
    #ifndef MXG_CHOSTFILE_H
    #include "Resolver/CHostFile.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class CResolverCore;
class CString;
class IAsyncResolverUser;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//== Class: CAsyncResolver
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   Singleton asynchronous interface for DNS queries.
//
// Description:
//   This class provides a singleton asynchronous interface for DNS queries. It
//   also provides query recursion. Notifications are sent on the
//   IAsyncResolverUser interface.
//
//   This class performs the generic operations of a DNS resolver and interacts
//   with implementations specific to either the operating system's DNS API or
//   the portable resolver, which uses the Network socket API to perform
//   queries. It uses its own thread, so requests do not block the user's
//   thread. It also launches the core resolver in its own separated thread when
//   it uses a synchronous API (e.g. Operating system's API). This way, queries
//   can be queued while waiting for an answer from the nameserver. Another way
//   to improve the resolver's efficiency is to piggyback queries to others
//   which have the same question. For example, if two equal queries are
//   performed for the same domain name, the first query is sent on the network
//   and the second is only queued waiting for the answer to the first one. Once
//   the answer is received for the query, both queries are resolved at the same
//   time.
//
//   <CODE>
//
//   +----------------+                        +---------------+
//   |                |-- Query -------------->|               |
//   | CAsyncResolver |                        | CResolverCore |
//   |                |<- EvResponseReceived --|               |
//   +----------------+                        +---------------+
//      |        |                                   ^  +- virtual Query() = 0;
//      |        |                                   |
//      Query/Update                  +--------------+--------------+
//      |        |                    |                             |
//      |        v                    |                             |
//      |  +---------------+    +-----------------+           +----------------+
//      |  | CResolverCache|    |CPortableResolver|    ...    |CWindowsResolver|
//      |  +---------------+    +-----------------+           +----------------+
//      |    +- Query()            +- Query()                    +- Query()
//      |    +- Update()
//      v
// +---------------+
// |   CHostFile   |
// +---------------+
//      +- Query()
//      +- AddNamingAuthorityPointer()
//      +- AddService()
//      +- AddHostName()
//      +- RemoveNamingAuthrityPointer()
//      +- RemoveService()
//      +- RemoveHostName()
//
//   </CODE>
//
//  Note on support for recursion:
//      The recursion is handled by making successive queries to the DNS and
//      merging the records back in the original response. The latter response
//      is searched every time until no recusions are necessary and therefore
//      all the data necessary to answer the DNS question have been queried.
//      For example:
//
//      - let's say an A query returns a single CNAME record. The response is
//        searched for the A record associated with the query name and only a
//        CNAME will be found. The response is saved as the original response
//        and a recursive A query is made to resolve the alias.
//
//      - now let's say the A query for the alias has returned an A record
//        (obviously containing an IPv4 address).
//
//      - the A record for the alias is merged into the original response
//        (so far only containing a CNAME record).
//
//      - the response is then searched for the A record of the original query
//        name and the answer will be found.
//
// Location:
//   Resolver/CAsyncResolver.h
//
// See also:
//  IAsyncResolverUser, CResolverCore, CResolverCache
//
//==============================================================================
class CAsyncResolver : protected CEventDriven
{
//M5T_INTERNAL_USE_BEGIN
    //-- Friend Declarations
    friend mxt_result InitializeCAsyncResolver();
    friend void FinalizeCAsyncResolver();

//-- New types within class namespace
protected:
    enum EMessageId
    {
        eCANCEL,
        eCLEAR,
        eEVQUERY_CANCELED,
        eEVRESPONSE_RECEIVED,
        eHOST_BY_ADDRESS,
        eHOST_BY_NAME,
        eSERVICES,
        eNAPTRS,

        //-- << Portable resolver >>
        eGET_NAMESERVERS,
        eSET_NAMESERVERS
    };

// BRCM_CUSTOM - [add] Add per-user DNS lookup support
#if defined(MXD_OS_BOS_ECOS)
public:
#endif
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
    struct SQuery
    {
        // This ID is not the DNS query ID, but only
        // an ID to differentiate among queries that
        // are in progress.
        unsigned int m_uId;
        CDnsPacket::SQuestion m_stQuestion;
        CVector<CString> m_vecstrServices;
        CSocketAddr::EAddressFamily m_eFamily;
        unsigned int m_uRecursionsLeft;
        CDnsPacket* m_pResponse;
        IAsyncResolverUser* m_pUser;
        mxt_opaque m_opqUserParam;
        CDnsPacket::SQuestion m_stRecursiveQuestion;
#if defined (MXD_ENUM_ENABLE_SUPPORT)
        CString m_strAus;
        CList<CDnsPacket::SEnumServiceData> m_lststSupportedServices;
#endif
        // This flag is needed in order not to notify the user of a response, if
        // it arrives prior to finishing up the cancellation process.
        bool m_bIsCanceled;
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
        mxt_opaque m_opqUserId;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
    };
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
#if defined(MXD_OS_BOS_ECOS)
protected:
#endif
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
//M5T_INTERNAL_USE_END

//-- Published Interface
public:
    // Summary:
    //   Synchronously cancels a query.
    mxt_result Cancel(IN CDnsPacket::SQuestion& rstQuestion,
                      IN IAsyncResolverUser* pUser,
                      IN mxt_opaque opq);

    // Summary:
    //   Clears the resolver cache synchronously.
    void ClearCache();

    //-- << Singleton >>.

    // Summary:
    //   Returns the unique instance of this class.
    static CAsyncResolver* GetInstance();

#if defined MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
    // <COMBINE CResolver::GetNameServers@OUT CList<SNameServer\>&>
    void GetNameServers(OUT CList<CResolver::SNameServer>& rlstNameServers);
    // <COMBINE CResolver::SetNameServers@IN CList<SNameServer\>&>
    mxt_result SetNameServers(IN CList<CResolver::SNameServer>& rlstNameServers);
#endif

#if defined MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT
    // Summary:
    //  Gets the host file so it can be configured.
    CHostFile* GetHostFile();
#endif

    // Summary:
    //   Allows to store non-authoritative responses in cache.
    void EnableCacheNonAuthoritativeResponse();

    // Summary:
    //   Disallows to store non-authoritative responses in cache.
    void DisableCacheNonAuthoritativeResponse();

    // Summary:
    //   Notifies that the query has been canceled.
    void EvQueryCanceledA(IN mxt_opaque opq);

    // Summary:
    //   Called by the resolver implementation when a query is answered.
    void EvResponseReceivedA(IN CDnsPacket& rResponse, IN mxt_opaque opq);

    //-- << DNS >>.

#if defined (MXD_ENUM_ENABLE_SUPPORT)
    //  Summary:
    //      Generates an ENUM query for the specified services and E.164
    //      number.
    mxt_result GetEnumUrisA(IN const CString& rstrNumber,
                            IN const CList<CDnsPacket::SEnumServiceData>& rlstSupportedServices,
                            IN CString& rstrZone,
                            IN IAsyncResolverUser* pUser,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                            IN mxt_opaque opq,
                            IN mxt_opaque opqId);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
#endif
    //  Summary:
    //      Gets the hosts name using its address.
    mxt_result GetHostByAddressA(IN const CSocketAddr& rAddress,
                                 IN IAsyncResolverUser* pUser,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                                 IN mxt_opaque opq,
                                 IN mxt_opaque opqId);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
    //  Summary:
    //      Gets the hosts address using its name.
    mxt_result GetHostByNameA(IN const CString& rstrName,
                              IN IAsyncResolverUser* pUser,
                              IN mxt_opaque opq,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                              IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                              IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET);
    //  Summary:
    //      Queries the domain for a list of naming authority pointers.
    mxt_result GetNamingAuthorityPointersA(IN const CString& rstrDomain,
                                           IN IAsyncResolverUser* pUser,
                                           IN mxt_opaque opq,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                                           IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                                           IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                                           IN const CVector<CString>* pvecstrServices = NULL);
    //  Summary:
    //      Queries hosts providing the desired service.
    mxt_result GetServicesA(IN const CString& rstrService,
                            IN IAsyncResolverUser* pUser,
                            IN mxt_opaque opq,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                            IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                            IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET);

    // Summary:
    //   Returns a pointer to the resolver servicing thread.
    mxt_result GetResolverThread(OUT IEComUnknown** ppEComUnknown) const;

    //  Summary:
    //   Returns a pointer to the resolver core servicing thread.
    mxt_result GetResolverCoreThread(OUT IEComUnknown** ppEComUnknown) const;

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    CAsyncResolver();

    // Summary:
    //  Destructor.
    virtual ~CAsyncResolver();

    mxt_result Activate();

    //-- << Resolver core interface >>

    // Summary:
    //   Prepares the result container and notifies the user of the query
    //   completion.
    bool Notify(IN CDnsPacket* pResponse, IN mxt_opaque opq);

    // Summary:
    //   Sends a query to the resolver core.
    void Query(IN TO SQuery* pstQuery);

    //-- << DNS helpers >>

    // Summary:
    //   Sorts SRV records according to the priority and weight.
    void SortSrvRecords(IN CList<CDnsPacket::SSrvRecordData>& rlststSrvRecord,
                        OUT CList<CDnsPacket::SSrvRecordData>& rlststSrvRecordSorted);

    // Summary:
    //   Sorts SRV records resulting from a NAPTR query according to the priority and weight.
    void SortSrvRecordsInNaptrResults(INOUT CList<CDnsPacket::SNaptrRecordData>& rlststPointers);


    // Summary:
    //   Converts the address to a PTR query name.
    mxt_result ConvertAddress(IN const CSocketAddr& rAddress, OUT CString& rstrQuery);

    // Summary:
    //   Searches the response for associated addresses.
    mxt_result RetrieveAddresses(IN CString& rstrName,
                                 IN CDnsPacket* pResponse,
                                 IN CSocketAddr::EAddressFamily eFamily,
                                 INOUT CList<CString>& rlstResolvedNames,
                                 INOUT CDnsPacket::SQuestion& rstRecursiveQuestions,
                                 OUT CList<CSocketAddr>& rlstAddresses);
    // Summary:
    //   Searches the response for associated names.
    mxt_result RetrieveNames(IN CDnsPacket* pResponse,
                             OUT CList<CString>& rlststrNames);

    // Summary:
    //   Searches the response for assoicated naming authority pointers.
    mxt_result RetrievePointers(IN CString& rstrDomain,
                                IN unsigned int uQuestionIndex,
                                INOUT SQuery* pstQuery,
                                INOUT CList<CString>& rlstResolvedReplacements,
                                OUT CList<CDnsPacket::SNaptrRecordData>& rlststPointers);

    // Summary:
    //   Searches the response for services records associated to the given
    //   service.
    mxt_result RetrieveServices(IN CString& rstrService,
                                IN CDnsPacket* pResponse,
                                IN CSocketAddr::EAddressFamily eFamily,
                                INOUT CDnsPacket::SQuestion& rstRecursiveQuestions,
                                OUT CList<CDnsPacket::SSrvRecordData>& rlststServices);

    //-- << ENUM helpers >>.
#if defined (MXD_ENUM_ENABLE_SUPPORT)
    // Summary:
    //   Converts an E.164 number.
    mxt_result ConvertNumber(IN const CString& rstrE164Number,
                             IN CString& rstrZone,
                             OUT CString& rstrAus,
                             OUT CString& rstrFqdn);

    // Summary:
    //   Parse the NAPTR service field.
    mxt_result GetNaptrServices(IN const CDnsPacket::SNaptrRecordData& rstNaptrRecord,
                                OUT CList<CDnsPacket::SEnumServiceData>& rlststEnumServices);

    // Summary:
    //   Substitutes the AUS with the regular expression in the replacement
    //   field.
    mxt_result ParseSubstitutionExp(IN const CString& rstrAus,
                                    IN const CString& rstrSubstitutionExp,
                                    OUT CString& rstrResult);

    // Summary:
    //   Searches the response for associated naming authority pointers..
    mxt_result RetrieveNumbers(INOUT SQuery* pstQuery,
                               INOUT CList<CString>& rlstResolvedNumbers,
                               OUT CList<CDnsPacket::SEnumUriData>& rlststNumbers);

    // Summary:
    //   Validate that the client supports the services specified in the
    //   response.
    mxt_result ValidateServices(IN const CList<CDnsPacket::SEnumServiceData>& rlststClientSupportedServices,
                                IN const CList<CDnsPacket::SEnumServiceData>& rlststNaptrSupportedServices,
                                OUT CList<CDnsPacket::SEnumServiceData>& rlststSupportedServices);
#endif

    //-- << CEventDriven >>.

    void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                   IN unsigned int uMessage,
                                   IN CMarshaler* pParameter);

    void InternalClear(IN CMarshaler* pParams);
    void InternalCancel(IN CMarshaler* pParams);
    void InternalEvQueryCanceledA(IN CMarshaler* pParams);
    void InternalEvResponseReceivedA(IN CMarshaler* pParams);
    void InternalGetNamingAuthorityPointersA(IN CMarshaler* pParams);
    void InternalGetHostByAddressA(IN CMarshaler* pParams);
    void InternalGetHostByNameA(IN CMarshaler* pParams);
    void InternalGetServicesA(IN CMarshaler* pParams);
#if defined MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT
    void InternalGetNameServers(IN CMarshaler* pParams);
    void InternalSetNameServers(IN CMarshaler* pParams);
#endif // #if defined MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT

    // Summary:
    //   Releases internal resources.
    void Release();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CAsyncResolver(IN const CAsyncResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CAsyncResolver& operator=(IN const CAsyncResolver& rSrc);

    //-- << Startup mechanism >>

    // Summary:
    //   Releases the singleton instance.
    static void Finalize();

//-- Hidden Data Members
protected:
    // The singleton instance.
    static CAsyncResolver* ms_pInstance;

    // Resolver core to use to perform queries.
    CResolverCore* m_pResolverCore;

    // List of active queries, that can be canceled.
    CVector<SQuery*> m_vecpstActiveQueries;

    // This is an ID only used to identify concurrent (active) queries. This
    // limits the number of concurrent queries to the size of a unsigned
    // integer.
    unsigned int m_uCurrentQueryId;

#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    // Resolver cache.
    CResolverCache m_cache;
#endif

#if defined (MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT)
    CHostFile m_hostFile;
#endif

    static const unsigned int ms_uMAX_RECURSIVE_QUERIES;
    static const char ms_cNAPTR_FLAG_SRV_RULE;

#if defined (MXD_ENUM_ENABLE_SUPPORT)
    static const char* const ms_szENUM_NAPTR_RESOLUTION_SERVICE;
    static const unsigned int ms_uE164_MAX_DIGITS;
    static const unsigned int ms_uENUM_NAPTR_MAX_NON_TERMINAL;
    static const char ms_cNAPTR_FLAG_ENUM_TERMINAL_RULE;
    static const char ms_cENUM_SERVICE_DELIMITER;
    static const char ms_cENUM_SUBTYPE_DELIMITER;
    static const char ms_cDDDS_FLAG_CASE_SENSITIVITY;
#endif

//M5T_INTERNAL_USE_END
};

#if defined MXD_RESOLVER_HOST_FILE_ENABLE_SUPPORT

//==============================================================================
//==
//==  GetHostFile
//==
//==============================================================================
//
//  Returns:
//      A pointer to the CHostFile of the resolver.
//
//  Description:
//      Gets the CHostFile of the resolver. 
//
//==============================================================================
inline CHostFile* CAsyncResolver::GetHostFile()
{
    return &m_hostFile;
}
#endif

//==============================================================================
//==
//==  EnableCacheNonAuthoritativeResponse
//==
//==============================================================================
//
//  Description:
//      Allows to store non-authoritative responses in cache. By default,
//      non-authoritative responses are cached.
//      If MXD_RESOLVER_CACHE_ENABLE_SUPPORT is not defined, calling this
//      method has no effect.
//
//==============================================================================
inline void CAsyncResolver::EnableCacheNonAuthoritativeResponse()
{
#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    m_cache.EnableCacheNonAuthoritativeResponse();
#endif
}

//==============================================================================
//==
//==  DisableCacheNonAuthoritativeResponse
//==
//==============================================================================
//
//  Description:
//      Disallows to store non-authoritative responses in cache. By default,
//      non-authoritative responses are cached.
//      If MXD_RESOLVER_CACHE_ENABLE_SUPPORT is not defined, calling this
//      method has no effect.
//
//==============================================================================
inline void CAsyncResolver::DisableCacheNonAuthoritativeResponse()
{
#if defined (MXD_RESOLVER_CACHE_ENABLE_SUPPORT)
    m_cache.DisableCacheNonAuthoritativeResponse();
#endif
}

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CASYNCRESOLVER_H


