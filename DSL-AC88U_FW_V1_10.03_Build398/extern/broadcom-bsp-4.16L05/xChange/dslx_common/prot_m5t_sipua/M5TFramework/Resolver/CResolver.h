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
#ifndef MXG_CRESOLVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CRESOLVER_H
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
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT)

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_IASYNCRESOLVERUSER_H
#include "Resolver/IAsyncResolverUser.h"
#endif

#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CResolver
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//  Synchronisation wrapper for DNS requests.
//
// Description:
//   This class is a wrapper around the asynchronous resolver to block on
//   asynchrounous requests. It blocks on a different semaphore for every DNS
//   request. Therefore only the current thread is blocked on its request. Other
//   threads may issue requests and will block on different semaphores.
//
//   All methods are static, meaning there is no such thing as a 'synchronous
//   resolver' instance.
//
// Location:
//   Resolver/CResolver.h
//
//==============================================================================
class CResolver
{
//-- New types within class namespace
public:
#if defined(MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
    // Summary:
    //  Structure that holds name server configuration.
    // Description:
    //  This structure is used to configure the name servers to be used by the
    //  portable resolver.
    struct SNameServer
    {
        // Summary:
        //  Default constructor.
        // Description:
        //  Default constructor of the SNameServer structure. By default, all
        //  member socket addresses are set to the "any" address.
        SNameServer()
        :   m_nameServerAddress(CSocketAddr::eINET_ANY),
            m_localAddress(CSocketAddr::eINET_ANY)
        {
        }

        // Summary:
        //  Constructor.
        // Description:
        //  Constructor of the SNameServer structure.
        SNameServer(IN const CSocketAddr& rNameServerAddress, IN const CSocketAddr& rLocalAddress)
        :   m_nameServerAddress(rNameServerAddress),
            m_localAddress(rLocalAddress)
        {
        }

        // Summary:
        //  The address and port of the name server.
        // Description:
        //  The address and port of the remote name server. If the port is set
        //  to 0, the default DNS port (53) will be used.
        CSocketAddr m_nameServerAddress;

        // Summary:
        //  The local address tu use when contacting the name server.
        // Description:
        //  The local address from which the portable resolver must send its
        //  requests to the remote name server. If the local address is set to
        //  the "any" address (0.0.0.0 for IPv4 or :: for IPv6), the portable
        //  resolver will let the IP stack determine which local address to use.
        CSocketAddr m_localAddress;
    };
#endif

//M5T_INTERNAL_USE_BEGIN
protected:
    //==========================================================================
    //== Class: CRequestSynchronizer
    //==========================================================================
    //<GROUP Resolver>
    //
    // Summary:
    //   This inner class serves an asynchronous request synchronizer.
    //
    // Description:
    //   It signals its internal semaphore when a notification is received.
    //   Users of this class passes an instance to the asynchronous resolver and
    //   call Wait() in order to block until a response is received.
    //
    //   The core goal of this class is to provide a synchronization wrapper
    //   around the asynchronous resolver.
    //
    // Location:
    //   Resolver/CResolver.h
    //
    //==========================================================================
    class CRequestSynchronizer : public IAsyncResolverUser
    {
    //-- Published Interface
    public:
        // Summary:
        //   Default constructor.
        CRequestSynchronizer();

        // Summary:
        //   Destructor.
        ~CRequestSynchronizer();

        // Summary:
        //   Blocks on the internal semaphore.
        mxt_result Wait();

        //-- << IAsyncResolverUser >>.

        // Summary:
        //   Asynchronous resolver notification for a A or AAAA response.
        void EvAsyncResolverUserResponseReceived(IN CList<CSocketAddr>& rlstAddresses, IN mxt_opaque opq);
        // Summary:
        //   Asynchronous resolver notification for a PTR response.
        void EvAsyncResolverUserResponseReceived(IN CList<CString>& rlstpstrNames, IN mxt_opaque opq);
        // Summary:
        //   Asynchronous resolver notification for an SRV response.
        void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SSrvRecordData>& rlstSrvRecord,
                                                 IN mxt_opaque opq);
        // Summary:
        //   Asynchronous resolver notification for an NAPTR response.
        void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SNaptrRecordData>& rlstNaptrRecord,
                                                 IN mxt_opaque opq);
#if defined(MXD_ENUM_ENABLE_SUPPORT)
        // Summary:
        //   Asynchronous resolver notification for an NAPTR (ENUM) response.
        void EvAsyncResolverUserResponseReceived(IN CList<CDnsPacket::SEnumUriData>& rlstEnumUri, IN mxt_opaque opq);
#endif
    //-- Hidden Data Members.
    protected:
        // The result returned by Wait()
        mxt_result m_resAnswer;
        // The semaphore on which blocks Wait() and signaled by the notifications.
        CBinarySemaphore m_semSynchronizer;
    };
//M5T_INTERNAL_USE_END

//-- Published Interface
public:
#if defined (MXD_ENUM_ENABLE_SUPPORT)
    //  Summary:
    //      Generates an ENUM query for the specified services and E.164
    //      number.
    static mxt_result GetEnumUris(IN const CString& rstrE164Number,
                                  IN const CList<CDnsPacket::SEnumServiceData>& rlstSupportedServices,
                                  OUT CList<CDnsPacket::SEnumUriData>& rlstUris,
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
                                  IN CString& rstrZone,
                                  IN mxt_opaque opqId);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
#endif // #if defined (MXD_ENUM_ENABLE_SUPPORT)

    //  Summary:
    //      Gets the hosts name using its address.
// BRCM_CUSTOM - [mod] Add per-user DNS lookup support
    static mxt_result GetHostByAddress(IN const CSocketAddr& rAddress, OUT CList<CString>& rlststrNames, IN mxt_opaque opqId);
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
    //  Summary:
    //      Gets the hosts address using its name.
    static mxt_result GetHostByName(IN const CString& rstrName,
                                    OUT CList<CSocketAddr>& rlstAddresses,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                                    IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                                    IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET);
    //  Summary:
    //      Queries the domain for a list of naming authority pointers.
    static mxt_result GetNamingAuthorityPointers(IN const CString& rstrDomain,
                                                 OUT CList<CDnsPacket::SNaptrRecordData>& rlstPointers,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                                                 IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                                                 IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET,
                                                 IN const CVector<CString>* pvecstrServices = NULL);
    //  Summary:
    //      Queries hosts providing the desired service.
    static mxt_result GetServices(IN const CString& rstrService,
                                  OUT CList<CDnsPacket::SSrvRecordData>& rlstServices,
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                                  IN mxt_opaque opqId,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                                  IN CSocketAddr::EAddressFamily eFamily = CSocketAddr::eINET);

#if defined(MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT)
    //  Summary:
    //      Gets the list of nameservers used by the portable resolver.
    static void GetNameServers(OUT CList<CSocketAddr>& rlstServers);

    //  Summary:
    //      Sets the list of nameservers used by the portable resolver.
    static mxt_result SetNameServers(IN CList<CSocketAddr>& rlstServers);

    //  Summary:
    //      Gets the list of nameservers used by the portable resolver.
    static void GetNameServers(OUT CList<SNameServer>& rlstNameServers);

    //  Summary:
    //      Sets the list of nameservers used by the portable resolver.
    static mxt_result SetNameServers(IN CList<SNameServer>& rlstNameServers);
#endif

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:

    // Summary:
    //  Default Constructor.
    CResolver();

    // Summary:
    //  Destructor.
    virtual ~CResolver();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CResolver(IN const CResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CResolver& operator=(IN const CResolver& rSrc);

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
       //     defined(MXD_CAP_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CRESOLVER_H


