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
#ifndef MXG_CPORTABLERESOLVER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CPORTABLERESOLVER_H
//M5T_INTERNAL_USE_END

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

#if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_CAP_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

#ifndef MXG_IASYNCCLIENTSOCKETMGR_H
#include "Network/IAsyncClientSocketMgr.h"
#endif

#ifndef MXG_IASYNCIOSOCKETMGR_H
#include "Network/IAsyncIoSocketMgr.h"
#endif

#ifndef MXG_IASYNCSOCKETMGR_H
#include "Network/IAsyncSocketMgr.h"
#endif

#ifndef MXG_CRESOLVER_H
#include "Resolver/CResolver.h"
#endif

#ifndef MXG_CRESOLVERCORE_H
#include "Resolver/CResolverCore.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncClientSocket;
class IAsyncIoSocket;
class IAsyncSocket;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CPortableResolver
//==============================================================================
//<GROUP RESOLVER_CLASSES>
//
// Summary:
//   This is the portable implementation of the resolver core.
//
// Description:
//   This class implements a portable resolver, which uses the Framework
//   asynchronous socket API to send and receive DNS requests. It queues queries
//   as they come in and sends them when the socket is ready. If a query comes in
//   and there is an identical one waiting for a response, it just bundles itself
//   with the latter so it saves on sending multiple identical queries on the
//   network. As recommended in RFC 1035, if a query fails (including timing
//   out), it sends it to the next nameserver, prior to retrying on the
//   nameserver that caused the failure to occur.
//
//  The portable resolver uses a list of nameservers where to send queries. This
//  list is considered ordered by decreasing order of priority. If a query to a
//  nameserver fails (times out after
//  MXD_PORTABLE_RESOLVER_RETRANSMISSION_TIMEOUT_MS), it switches to the
//  subsequent nameserver and so on.
//
//  If all the nameservers fail, it then wraps around and goes through the list
//  again. This is done MXD_PORTABLE_RESOLVER_MAX_RETRANSMISSIONS times. This is
//  done independently for each query (transaction). This means that every
//  transaction will begin by trying the top-priority nameserver first. This
//  encourages using the primary nameserver.
//
//  Finally, if an ICMP error occurs while either sending or receiving on a
//  nameserver for a given transaction, all ongoing transactions that use this
//  nameserver switch.
//
// Location:
//   Resolver/CPortableResolver.h
//
//==============================================================================
class CPortableResolver : public CResolverCore,
                          public IAsyncSocketMgr,
                          public IAsyncClientSocketMgr,
                          public IAsyncIoSocketMgr
{
    //-- Friend Declarations
    friend class CAsyncResolver;

//-- New types within class namespace
protected:
//M5T_INTERNAL_USE_BEGIN
    enum EMessageId
    {
        // We need to set a value, since we inherit from a class that posts messages
        eGET_NAMESERVERS = CResolverCore::eLAST,
        eSET_NAMESERVERS
    };

    // This structure holds the necessary information about the connections to a
    // nameserver.
    struct SNameServer
    {
        // Socket interfaces.
        IAsyncSocket* m_pSocket;
        IAsyncClientSocket* m_pClientSocket;
        IAsyncIoSocket* m_pIoSocket;
        // Socket state;
        bool m_bConnected;
        // Nameserver address.
        CSocketAddr m_serverAddress;
        CSocketAddr m_localAddress;
    };
//M5T_INTERNAL_USE_END

//-- Published Interface
public:
    //-- << CResolverCore >>

    // <COMBINE CResolverCore::QueryA@IN CDnsPacket::SQuestion& rstQuestion, IN mxt_opaque opq>
    mxt_result QueryA(IN CDnsPacket::SQuestion& rstQuestion, IN mxt_opaque opq);

    // BRCM_CUSTOM - [add] Add Android support.
    //-- << CEventDriven >>
    mxt_result Activate(IN IEComUnknown* pPortableResolverThread);
    // BRCM_CUSTOM - [end] Add Android support.

    //-- << IAsyncSocketMgr >>.
    void EvAsyncSocketMgrClosed(IN mxt_opaque opq);
    void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq);
    void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq, IN mxt_result res);

    //-- << IAsyncClientSocketMgr >>.
    void EvAsyncClientSocketMgrBound(IN mxt_opaque opq, IN CSocketAddr* pEffectiveLocalAddress);
    void EvAsyncClientSocketMgrConnected(IN mxt_opaque opq);

    //-- << IAsyncIoSocketMgr >>.
    void EvAsyncIoSocketMgrReadyToRecv(IN mxt_opaque opq);
    void EvAsyncIoSocketMgrReadyToSend(IN mxt_opaque opq);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    // Summary:
    //  Default Constructor.
    CPortableResolver();

    // Summary:
    //  Destructor.
    virtual ~CPortableResolver();

    //-- << Portable API core >>

    // Summary:
    //   Overrides CResolverCore message to send a packet on the network.
    void InternalQueryA(IN CMarshaler* pParams);

    // Summary:
    //   Sends a query on the network.
    void Send(IN mxt_opaque opq);

    //-- << Portable resolver implementation >>.

    // Summary:
    //   Connects socket to the nameservers.
    mxt_result Connect();

    // Summary:
    //   Parses the packet and notifies the user of a response.
    mxt_result HandleResponse(IN CDnsPacket& rResponse);

    // Summary:
    //   Notifies the asynchronous resolver of transaction completions.
    void Notify(IN TO STransaction* pstTransaction, IN CDnsPacket& rResponse);

    //  Summary:
    //      Gets the list of nameservers used by the portable resolver.
    void GetNameServers(OUT CList<CResolver::SNameServer>& rlstNameServers);

    //  Summary:
    //      Sets the list of nameservers used by the portable resolver.
    mxt_result SetNameServers(IN CList<CResolver::SNameServer>& rlstNameServers);

    void ReleaseNameServer(IN SNameServer* pstNameServer);

    // Summary:
    //   Switches the current nameserver.
    bool SwitchNameServer(IN TO STransaction* pstTransaction, IN bool bReset = false);

    //  Summary:
    //   Resets the list of nameservers used by the portable resolver.
    mxt_result ResetNameServers();

    //-- << CEventDriven >>

    // Summary:
    //   Closes the sockets.
    void ReleaseInstance();

    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimerId,
                                 IN mxt_opaque opq);

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CPortableResolver(IN const CPortableResolver& rSrc);

    // Summary:
    //  Assignment Operator.
    CPortableResolver& operator=(IN const CPortableResolver& rSrc);

    //-- << CEventDriven >>.

    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

    void InternalGetNameServers(IN CMarshaler* pParams);
    void InternalSetNameServers(IN CMarshaler* pParams);

//-- Hidden Data Members
protected:
    //-- << DNS configuration >>

    // List of nameservers.
    CList<SNameServer*> m_lstpstNameServers;

    // Default RFC 1035 nameserver port.
    static const uint16_t ms_uDEFAULT_DNS_PORT;

// BRCM_CUSTOM - [add] Add DNS resolver support for Android.
#if defined( MXD_OS_ANDROID )
    // Maximum number of Android DNS property entries.
    static const unsigned int ms_uMAX_DNS_PROPERTIES;
    // DNS property prefix.
    static const char* const ms_szDNS_PROP_NAME_PREFIX;
#endif
// BRCM_CUSTOM - [end] Add DNS resolver support for Android.
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined (MXD_PORTABLE_RESOLVER_ENABLE_SUPPORT) &&
       //--     defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //--     defined(MXD_CAP_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CPORTABLERESOLVER_H


