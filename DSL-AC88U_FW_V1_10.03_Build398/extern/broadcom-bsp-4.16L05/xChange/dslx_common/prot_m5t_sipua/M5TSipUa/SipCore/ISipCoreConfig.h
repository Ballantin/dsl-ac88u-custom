//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_ISIPCORECONFIG_H
#define MXG_ISIPCORECONFIG_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SIP Stack Global Configuration
//-----------------------------------
#ifndef MXG_SIPSTACKCFG_H
// MXD_SIPSTACK_ENABLE_TLS,
// MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
// MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
// MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT
//-----------------------------------------------------------
#include "Config/SipStackCfg.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------
#ifndef MXG_CSIPPARSERSVC_H
#include "SipTransport/CSipParserSvc.h" // PFNTransportPacketInspector,
#endif                                  // PFNTransportPacketModifier


#ifndef MXG_HEADERDEFINITION_H
#include "SipParser/HeaderDefinition.h" // ESipHeaderType
#endif

#ifdef MXD_SIPSTACK_ENABLE_TLS
#ifndef MXG_CSIPCLIENTSOCKET_H
#include "SipTransport/CSipClientSocket.h" // PFNHandshakeValidator
#endif
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

#ifndef MXG_ISOCKET_H
#include "Network/ISocket.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif

#ifdef MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT
#ifndef MXG_ISIPDIGESTCLIENTAUTHSVC_H
#include "SipUserAgent/ISipDigestClientAuthSvc.h"
#endif
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//------------------------
class CSipConnectionBlacklist;
class CSipDialogMatcherList;
class CSipNetworkInterfaceList;
class CSipPersistentConnectionList;
class CSipTlsContextFactory;
class CSocketAddr;
class ISipCoreUser;
class ISipDataLogger;
#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
class ISipNetworkReceptionControlMgr;
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
class ISipPacketReceptionControlMgr;
#endif
class ISipSwitchbackPersistentConnection;

#ifdef MXD_SIPSTACK_ENABLE_TLS
    class ISipTlsContextFactory;
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
    class ISipKeepAliveExtensionMgr;
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT)
    class CSipUaHelpers;
#endif

#ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
    class CSipStackMonitor;
#endif

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)
    class CTargetManagementModule;
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipCoreConfig);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipCoreConfig
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//   This is the configuration interface of the SIP stack. All dynamic
//   configuration is performed through this interface.
//
//   This interface also offers a unified Startup and Shutdown mechanism to make
//   it easy to start and stop the SIP stack.
//
//   You can safely create an ISipCoreConfig object whenever it is needed, and
//   release it as soon as the configuration has been updated. The SIP stack
//   suffers no side effect if the application releases all refereces it has to
//   an ISipCoreConfig.
//
//   #Listening Address Configuration#
//   RFC 3261 defines three transport protocols for SIP: UDP, TCP and TLS.
//   According to RFC 3261, a device should listen on port 5060 for UDP and TCP,
//   and it should listen on port 5061 for TLS connections.
//
//   When advertising that a device listens on default ports in SIP, whether
//   in Contact or Via headers, a device does not have to specify the port it
//   is listening to. The device only has to advertise the port when
//   listening on something else than the default port(s).
//
//   The following are some examples:
//
//   <CODE>
//       Contact: sip:user@10.12.13.14           <- Listening on default port(s)
//       Contact: sip:user@10.12.13.14:5062      <- Listening on non-default port
//   </CODE>
//
//   It is quite important that devices that want to simultaneously support
//   UDP/TCP and TLS listen on the default ports. Not doing so could cause
//   the following problem.
//
//   Assume a device listening on 5065 in UDP/TCP and on 5070 for TLS. The
//   user makes a call to sip:destination@somewhere.com. Upon resolving, the
//   stack uses TLS to establish the connection to the proxy at
//   somewhere.com. The contact in the outgoing INVITE request is:
//
//   <CODE>
//       Contact: sip:user@10.12.13.14:5070
//   </CODE>
//
//   From here, there are different possible scenarios where using this
//   non-default port would break communications:
//
//   - If the TLS connection goes down and the proxy tries to re-establish it
//     with UDP or TCP, the device is expecting TLS on 5070.
//
//   - If the proxy does not record-route and the peer device does not
//     support TLS, it will try UDP/TCP on 5070, failing again.
//
//   - Other scenarios exist when performing transfer and such...
//
//   Now if the default ports would have been used instead of non-default
//   ones, then the contact would look like:
//
//   <CODE>
//       Contact: sip:user@10.12.13.14
//   </CODE>
//
//   Under such condition, the device sending a request will use the proper
//   destination port based on the transport protocol being used; 5060 for
//   UDP/TCP and 5061 for TLS.
//
//   #Initialization Procedure#
//   - Create an ISipCoreConfig.
//   - Create the servicing threads for the SIP stack.
//   - Configure the SIP stack threads.
//   - Configure the core user.
//   - Configure any other optional parameter.
//   - Call Startup.
//   - Call AddLocalAddress to add a local network interface.
//   - Call ListenA for all transports and network interfaces to which you need
//     to listen.
//   - You can safely release the ISipCoreConfig.
//   - You can now start using the SIP stack and its services.
//
//   #Shutdown Procedure#
//   - Send final responses to all pending server transactions.
//   - Call Clear on all ISipContexts on which the SIP stack user still has a
//     reference.
//   - Release all references to all ISipContexts the user still has.
//   - Call ShutdownA.
//   - Wait for ISipCoreUser::EvShutdownComplete.
//
//
//   #The ISipCoreConfig is an ECOM object#
//   The ISipCoreConfig is an ECOM object that is created and accessed through
//   the following ECOM identifiers:
//
//   <B>Class Id: CLSID_CSipCoreConfig</B>
//
//   <B>Interface Id: IID_ISipCoreConfig</B>
//
// Location:
//   SipCore/ISipCoreConfig.h
//
//==EDOC========================================================================
class ISipCoreConfig : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipCoreConfig);
    //M5T_INTERNAL_USE_END

    // Configuration options for SetHeaderFormPreference.
    //----------------------------------------------------
    enum EHeaderFormPreference
    {
        // Prefer using short header form.
        //---------------------------------
        ePREFER_SHORT,

        // Prefer using long header form.
        //--------------------------------
        ePREFER_LONG
    };

    // Configuration options for SetCommaSeparatedHeader.
    //----------------------------------------------------
    enum ECommaSeparatedHeaderConfig
    {
        // Use comma separated headers for specified header.
        //---------------------------------------------------
        eCOMMA_SEPARATED,

        // Place each header on separated lines.
        //---------------------------------------
        eCRLF_SEPARATED
    };

    // Summary:
    //  Enumeration defining the possible combination of supported versions of
    //  IP.
    //-------------------------------------------------------------------------
    enum EIpVersionConfig
    {
        // The application only supports IPv4.
        eCONFIG_IPV4,

#ifdef MXD_SIPSTACK_IPV6_ENABLE_SUPPORT
        // The application only supports IPv6.
        eCONFIG_IPV6,

        // The application supports both IPv4 and IPv6: IPv4 is preferred.
        eCONFIG_IPV4_AND_IPV6,

        // The application supports both IPv4 and IPv6: IPv6 is preferred.
        eCONFIG_IPV6_AND_IPV4
#endif
    };

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
    // Summary:
    //  Enumeration defining the Spiralling Service state.
    //-----------------------------------------------------
    enum ESpirallingSvcState
    {
        // The Spiralling Service is active.
        //-----------------------------------
        eACTIVE,

        // The Spiralling Service is inactive.
        //-------------------------------------
        eINACTIVE,
    };
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

    // Summary:
    //  Enumerations describing the preference for how addresses are
    //  represented.
    //---------------------------------------------------------------
    enum EAddressTypePreference
    {
        // Addresses are represented as an FQDN whenever possible.
        // This is the default value.
        //---------------------------------------------------------
        ePREFER_FQDN,

        // For the address to an IP address, even if there is an FQDN
        // associated with it.
        //------------------------------------------------------------
        eFORCE_IP_ADDRESS
    };

    // Description:
    //  These are the possible modes for transport failover. The mode is
    //  used to decide what needs to be done when a packet fails to be sent.
    enum EFailoverMode
    {
        // Description:
        //  This is the default value. The default failover is to assume that
        //  TCP is supported by the peer. The default failover will degrade
        //  TLS in TCP if both TLS and UDP are supported. Also, TLS will
        //  always only be considered a target if it is the only transport
        //  supported or if the URI to resolve is a SIPS URI.
        eFAILOVER_DEFAULT,

        // Description:
        //  TCP failover only occurs if the peer supports TCP according to
        //  the information provided by the DNS configuration and the DNS
        //  answer. If the peer does not support TCP, then no failover will
        //  occur from UDP to TCP.
        eFAILOVER_NO_TCP_ASSUMPTION,

        // Description:
        //  Failover will assume all supported transports are supported by
        //  the peer. This means that all possible transports, even TLS,
        //  will be eventually used if supported. Also, no unsupported
        //  transport will ever be assumed. The only exception to that rule
        //  is a SIPS URI will always only return TLS targets, as per
        //  SIPS rules.
        eFAILOVER_ALL_TRANSPORTS_ASSUMPTION
    };

    // Summary:
    //  Struct describing an accessible network.
    //
    // Description:
    //  Configures m_address to hold the address of the network, and m_netmask
    //  to hold the network mask to apply to a destination address. After the
    //  mask has been applied on the destination address, if it equals m_address,
    //  then the local address associated with this network is used to send the
    //  packet.
    //
    // Example:
    //
    //  Assume a device with an interface to 10.1.x.x, it could then configure
    //  m_address to 10.1.0.0 and m_mask to 255.255.0.0.
    //
    //  If the SIP stack has to send a packet to 10.1.2.3, the destination
    //  address would be masked with m_netmask, resulting in 10.1.0.0. Since the
    //  masked destination address and m_address match, the address for which
    //  this SAccessibleNetwork is configured would be used.
    //
    // Example:
    //
    //  Assume a device that can reach the following networks from a single
    //  address, let's say 10.2.5.100:
    //      - 10.0.x.x
    //      - 10.1.x.x
    //      - 10.2.x.x
    //      - 10.3.x.x
    //      - 10.4.x.x
    //      - 10.5.x.x
    //      - 10.6.x.x
    //      - 10.7.x.x
    //
    //  There are multiple ways that the application could configure its list
    //  of accessible networks. One way would be to associate with its
    //  10.2.5.100 local address multiple accessible networks:
    //
    //      - 10.0 network: m_address: 10.0.0.0, m_netmask: 255.255.0.0
    //      - 10.1 network: m_address: 10.1.0.0, m_netmask: 255.255.0.0
    //      - 10.2 network: m_address: 10.2.0.0, m_netmask: 255.255.0.0
    //      - 10.3 network: m_address: 10.3.0.0, m_netmask: 255.255.0.0
    //      - 10.4 network: m_address: 10.4.0.0, m_netmask: 255.255.0.0
    //      - 10.5 network: m_address: 10.5.0.0, m_netmask: 255.255.0.0
    //      - 10.6 network: m_address: 10.6.0.0, m_netmask: 255.255.0.0
    //      - 10.7 network: m_address: 10.7.0.0, m_netmask: 255.255.0.0
    //
    //  Another way would be to tweak m_netmask and add a single entry for all
    //  networks:
    //
    //      - 10.[0-7] network: m_address: 10.0.0.0, m_netmask: 255.248.0.0
    //
    //  When sending to a destination that is part of one of the network, let's
    //  say 10.4.3.65, then the destination is masked with m_netmask. This
    //  results in 10.0.0.0. The masked destination matches m_address, thus the
    //  associated local address would be used to reach this destination.
    //
    //  When sending to a destination that is not part of one of the network,
    //  let's say 10.9.22.98, then the destination is masked with m_netmask.
    //  This results in 10.8.0.0. The masked destination does not match
    //  m_address, thus the associated local address would not be used to reach
    //  this destination.
    //
    //  Notice that the mask is applied in binary. Thus for this example, if
    //  representing only the important part of the address in binary:
    //
    //  <CODE>
    //  Destination     Dest.Binary         Mask                Masked Result
    //  ------------------------------------------------------------------------
    //  10.4.3.65       10.b00000100.3.65   255.b11111000.0.0   10.0.0.0
    //  10.9.22.98      10.b00001001.22.98  255.b11111000.0.0   10.b00001000.0.0
    //  </CODE>
    //--------------------------------------------------------------------------
    struct SAccessibleNetwork
    {
        // The address of the network.
        //-----------------------------
        CSocketAddr     m_address;

        // The address mask to apply to destination addresses.
        //-----------------------------------------------------
        CSocketAddr     m_netmask;
    };

    // Summary:
    //  Struct describing a local network interface address and its respective
    //  listening port. Refer to GetNetworkInterfaceList() for more information.
    //--------------------------------------------------------------------------
    struct SNetworkIf
    {
        // Local network interface address.
        //----------------------------------
        CSocketAddr     m_socketAddr;

        // UDP or TCP listened port. If 0 = none.
        //----------------------------------------
        uint16_t        m_uListeningUdpTcpPort;

        // TLS listened port. If 0 = none.
        //---------------------------------
        uint16_t        m_uListeningTlsPort;

        // Constructor.
        SNetworkIf()
        :   m_uListeningUdpTcpPort(0),
            m_uListeningTlsPort(0)
        {};
    };

    // Description:
    //  These are the possible modes for handling 503 Service Unavailable responses.
    //  The mode is used to decide which 503 Service Unavailable response will trigger
    //  a failover process.
    enum EServiceUnavailableHandling
    {
        // Description:
        //  This is the default value.
        //  This value starts the failover on any 503 Service Unavailable response received
        //  (either locally or from a peer).
        eSERVICE_UNAVAILABLE_HANDLING_FAILOVER_ON_ANY_SOURCE,

        // Description:
        //  This value ignores network received 503 Service Unavailable response and
        //  will only start the failover process upon reception of a locally-generated 503.
        eSERVICE_UNAVAILABLE_HANDLING_FAILOVER_ON_LOCALLY_GENERATED_ONLY,
    };

    //-- << Threading Initialization >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetCoreThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the thread used by the SIP core and transactions modules.
    //
    //  Parameters:
    //    pThread:
    //      An ECOM object supporting the proper activation, timer, and
    //      messaging interfaces. This is usually a CServicingThread object.
    //
    //  Returns:
    //      - resS_OK: Thread configured successfully.
    //
    //      - resFE_INVALID_ARGUMENT: NULL pointer or ECOM instance that does
    //      not support the proper interfaces.
    //
    //      - resFE_UNEXPECTED: Thread already configured.
    //
    //  Description:
    //      This method allows to configure the thread used by the core and
    //      transaction modules of the SIP stack.
    //
    //      This thread MUST be the one that the application uses when accessing
    //      the various User-Agent and Proxy services, and other objects from
    //      the core (namely the ISipContext).
    //
    //      If the application does not use the CServicingThread mechanism for
    //      its threading, it must then do the following:
    //
    //          1. Create a CServicingThread instance (see examples in
    //          CServicingThread).
    //
    //          2. Query the IActivationService interface.
    //
    //          3. Periodically call the overloaded <C>Activate(IN uint64_t, OUT
    //          bool*)</C> function on this interface.
    //
    //          4. Query the IEComUnknown interface on the CServicingThread
    //          instance.
    //
    //          5. Set this IEComUnknown interface as the thread for the core
    //          and transactions by calling SetCoreThread.
    //
    //      This has the effect of periodically providing CPU time from the
    //      application's thread to the servicing thread instance and thus to
    //      the core and transaction modules.
    //
    //      Depending on the application requirements, a single thread can be
    //      used to run more than one of the SIP stack modules (Core, transport,
    //      and DNS).
    //
    //  Warning:
    //      This method must be called successfully exactly once before
    //      starting up the SIP stack. Once set, the thread cannot be changed
    //      without a shutdown of the SIP stack.
    //
    //  See Also:
    //      SetTransportThread, SetDnsResolverThread, CServicingThread
    //
    //==EDOC====================================================================
    virtual mxt_result SetCoreThread(IN IEComUnknown* pThread) = 0;

    //==========================================================================
    //==
    //==  GetCoreThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the thread used by the SIP core and
    //      transactions modules.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the
    //      core servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the SIP core thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //      - resFE_FAIL: core thread is not set.
    //
    //  Description:
    //      This method returns a pointer to the thread used by the SIP core
    //      and transactions modules. Make sure ReleaseIfRef is called upon the
    //      returned pointer, since internally AddIfRef has already been called.
    //
    //  See Also:
    //      SetCoreThread, CServicingThread
    //
    //==========================================================================
    virtual mxt_result GetCoreThread(OUT IEComUnknown** ppThread) const = 0;

    //==SDOC====================================================================
    //==
    //==  SetTransportThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the thread that is used by the SIP transport module and the
    //      sockets it uses.
    //
    //  Parameters:
    //    pThread:
    //      An ECOM object supporting the proper activation, timer, and
    //      messaging interfaces. This is usually a CServicingThread object.
    //
    //  Returns:
    //      - resS_OK: Thread configured successfully.
    //
    //      - resFE_INVALID_ARGUMENT: NULL pointer or ECOM instance that does
    //      not support the proper interfaces.
    //
    //      - resFE_UNEXPECTED: Thread already configured.
    //
    //  Description:
    //      This method allows to configure the thread used by the SIP transport
    //      and the socket it uses.
    //
    //      Depending on the application requirements, a single thread can be
    //      used to run more than one of the SIP stack modules (Core, transport,
    //      and DNS).
    //
    //  Warning:
    //      This method must be called successfully exactly once before
    //      starting up the SIP stack. Once set, the thread cannot be changed
    //      without a shutdown of the SIP stack.
    //
    //  See Also:
    //      SetCoreThread, SetDnsResolverThread, CServicingThread
    //
    //==EDOC====================================================================
    virtual mxt_result SetTransportThread(IN IEComUnknown* pThread) = 0;

    //==========================================================================
    //==
    //==  GetTransportThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the thread used by the SIP transport module
    //      and the sockets it uses.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the
    //      transport servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the SIP transport thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //      - resFE_FAIL: transport thread is not set.
    //
    //  Description:
    //      This method returns a pointer to the thread used by the SIP
    //      transport module and the sockets it uses. Make sure ReleaseIfRef is
    //      called upon the returned pointer, since internally AddIfRef has
    //      already been called.
    //
    //  See Also:
    //      SetTransportThread, CServicingThread
    //
    //==========================================================================
    virtual mxt_result GetTransportThread(OUT IEComUnknown** ppThread) const = 0;

    //==SDOC====================================================================
    //==
    //==  SetDnsResolverThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the thread that is used to serially process DNS requests.
    //
    //  Parameters:
    //    pThread:
    //      An ECOM object supporting the proper activation, timer, and
    //      messaging interfaces. This is usually a CServicingThread object.
    //
    //  Returns:
    //      - resS_OK: Thread configured successfully.
    //
    //      - resFE_INVALID_ARGUMENT: NULL pointer or ECOM instance that does
    //      not support the proper interfaces.
    //
    //      - resFE_UNEXPECTED: Thread already configured.
    //
    //  Description:
    //      This method allows to configure the thread used by the DNS resolving
    //      facilities of the SIP stack.
    //
    //      Due to the possible long access time to the network when resolving
    //      DNS addresses, this process should be run in its own separate
    //      thread, as it blocks the thread it uses while waiting for the DNS
    //      answer.
    //
    //  Warning:
    //      This method must be called successfully exactly once before
    //      starting up the SIP stack. Once set, the thread cannot be changed
    //      without a shutdown of the SIP stack.
    //
    //  See Also:
    //      SetCoreThread, SetTransportThread, CServicingThread
    //
    //==EDOC====================================================================
    virtual mxt_result SetDnsResolverThread(IN IEComUnknown* pThread) = 0;

    //==========================================================================
    //==
    //==  GetDnsResolverThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Returns a pointer to the thread that is used to serially process
    //      DNS requests.
    //
    //  Parameters:
    //    ppThread:
    //      Address of a pointer to the IEComUnknown interface of the
    //      DNS resolver servicing thread.
    //
    //  Returns:
    //      - resS_OK: *ppThread contains a pointer to the DNS resolving thread.
    //      - resFE_INVALID_ARGUMENT: pointer address in parameter is NULL.
    //      - resFE_FAIL: DNS resolver thread is not set.
    //
    //  Description:
    //      This method returns a pointer to the thread that is used to serially
    //      process DNS requests. Make sure ReleaseIfRef is called upon the
    //      returned pointer, since internally AddIfRef has already been called.
    //
    //  See Also:
    //      SetDnsResolverThread, CServicingThread
    //
    //==========================================================================
    virtual mxt_result GetDnsResolverThread(OUT IEComUnknown** ppThread) const = 0;

    //-- << Parser Configuration >>
    //-------------------------------

    //==SDOC====================================================================
    //==
    //==  SetCommaSeparatedHeader
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets comma generation setting for the specified header type.
    //
    //  Parameters:
    //    eHeader:
    //      Type of header for which to change the comma separated generation
    //      behaviour.
    //
    //    eConfig:
    //      - eCOMMA_SEPARATED: Headers of the specified type are output as
    //      comma-separated headers.
    //
    //      - eCRLF_SEPARATED: Headers of the specified type are output as
    //      CRLF-separated headers.
    //
    //  Returns:
    //      resS_OK: Setting has been applied.
    //
    //      resFE_INVALID_ARGUMENT: This type of header cannot be combined, as
    //      per the header definition table.
    //
    //  Description:
    //      This method allows the configuration of the behaviour for combining
    //      headers when serializing.
    //
    //      The default behaviour is that all headers that can be combined are
    //      combined on a single line, with each header separated by a comma
    //      and a space.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetCommaSeparatedHeader(
                                    IN ESipHeaderType eHeader,
                                    IN ECommaSeparatedHeaderConfig eConfig) = 0;


    //==SDOC====================================================================
    //==
    //==  SetHeaderFormPreference
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures how SIP headers are generated.
    //
    //  Parameters:
    //    ePreference:
    //      - ePREFER_SHORT to use short header form for headers that support
    //      it.
    //
    //      - ePREFER_LONG to use long header form for all headers.
    //
    //  Returns:
    //      resS_OK on success,
    //      resFE_FAIL on failure.
    //
    //  Description:
    //      Configures how the headers of the packets generated by the SIP stack
    //      are generated.
    //
    //      When ePREFER_SHORT is used, headers that support it use the short
    //      form. For instance:
    //
    //          - "From:" is output as "f:".
    //
    //          - "Content-Length:" is output as "l:".
    //
    //          - "Supported:" is output as "k:".
    //
    //          - "Max-Forwards:" is output as "Max-Forwards:", as there is
    //          no short form version of this header defined.
    //
    //          - etc.
    //
    //      When ePREFER_LONG is used, all headers are output using their long
    //      name version.
    //
    //      By default, the SIP stack is initialized with ePREFER_LONG.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetHeaderFormPreference(
                                      IN EHeaderFormPreference ePreference) = 0;

    //==SDOC====================================================================
    //==
    //==  SetApplicationId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the application identification to output in User-Agent and
    //      Server headers.
    //
    //  Parameters:
    //    rstrUserAgentId:
    //      The application identification to put in the User-Agent header.
    //
    //    pstrServerId:
    //      The application identification to put in the Server header. If NULL,
    //      rstrUserAgentId is used.
    //
    //  Description:
    //      Sets the application identification to use in the User-Agent and
    //      Server headers. This should follow the ABNF defined in RFC 3261 for
    //      the Server and User-Agent values.
    //
    //      <CODE>
    //      value            =  "Server" HCOLON server-val *(LWS server-val)
    //      server-val       =  product / comment
    //      product          =  token [SLASH product-version]
    //      product-version  =  token
    //      comment          =  LPAREN *(ctext / quoted-pair / comment) RPAREN
    //      ctext            =  %x21-27 / %x2A-5B / %x5D-7E / UTF8-NONASCII /
    //                          LWS
    //      </CODE>
    //
    //      By default, this string is empty.
    //
    //      This method can be called at any time.
    //
    //  See Also:
    //      AddStackVersionToId, AddUserAgentToRequests, AddServerToResponses
    //
    //==EDOC====================================================================
    virtual void SetApplicationId(IN const CString& rstrUserAgentId,
                                  IN const CString* pstrServerId = NULL) = 0;

    //==SDOC====================================================================
    //==
    //==  AddStackVersionToId
    //==
    //==========================================================================
    //
    //  Summary:
    //      Determines whether the SIP stack version is added in the User-Agent
    //      and Server headers.
    //
    //  Parameters:
    //    bAdd:
    //      The SIP stack version is added when true, otherwise it is not added.
    //
    //  Description:
    //      Configures whether the SIP stack version should be added to the
    //      User-Agent and Server headers. By default, it is added in the form
    //      "M5T SIP Stack/major.minor.release.build" at the end of the headers
    //      if they are added. 'major', 'minor', 'release' and 'build' are
    //      obviously replaced by the corresponding numbers.
    //
    //      Note that this option has no impact on whether or not the User-Agent
    //      and Server headers are added to packets sent by the SIP stack.
    //
    //      Note that the application identification set through
    //      SetApplicationId appears in both the User-Agent and Server
    //      headers, before the SIP stack version.
    //
    //      If the bAdd parameter is set to false, the SIP stack version is not
    //      in the User-Agent and Server headers.
    //
    //      By default, the SIP stack version is added to the User-Agent and
    //      Server headers.
    //
    //      This method can be called at any time.
    //
    //  See Also:
    //      SetApplicationId, AddUserAgentToRequests, AddServerToResponses
    //
    //==EDOC====================================================================
    virtual void AddStackVersionToId(IN bool bAdd) = 0;

    //==SDOC====================================================================
    //==
    //==  AddUserAgentToRequests
    //==
    //==========================================================================
    //
    //  Summary:
    //      Determines whether or not the User-Agent header is added to requests
    //      created by the SIP stack.
    //
    //  Parameters:
    //    bAdd:
    //      The User-Agent header is added to requests when true, otherwise it
    //      is not added.
    //
    //  Description:
    //      Configures whether or not to add the User-Agent header to requests
    //      created by the SIP stack. To determine what to put in that header,
    //      refer to SetApplicationId and AddStackVersionToId. Note that if
    //      added, the header is added only to requests created by the SIP stack
    //      and not to requests forwarded by it.
    //
    //      By default, the User-Agent header is added to requests.
    //
    //      This method can be called at any time.
    //
    //  See Also:
    //      SetApplicationId, AddStackVersionToId, AddServerToResponses
    //
    //==EDOC====================================================================
    virtual void AddUserAgentToRequests(IN bool bAdd) = 0;

// BRCM_CUSTOM [add] Add User-Agent header to responses
    //==SDOC====================================================================
    //==
    //==  AddUserAgentToResponses
    //==
    //==========================================================================
    //
    //  Summary:
    //      Determines whether or not the User-Agent header is added to responses
    //      created by the SIP stack.
    //
    //  Parameters:
    //    bAdd:
    //      The User-Agent header is added to responses when true, otherwise it
    //      is not added.
    //
    //  Description:
    //      Configures whether or not to add the User-Agent header to responses
    //      created by the SIP stack. To determine what to put in that header,
    //      refer to SetApplicationId and AddStackVersionToId. Note that if
    //      added, the header is added only to responses created by the SIP stack
    //      and not to responses forwarded by it.
    //
    //      By default, the User-Agent header is added to requests.
    //
    //      This method can be called at any time.
    //
    //  See Also:
    //      SetApplicationId, AddStackVersionToId, AddServerToResponses,
    //      AddUserAgentToRequests
    //
    //==EDOC====================================================================
    virtual void AddUserAgentToResponses(IN bool bAdd) = 0;
// BRCM_CUSTOM [end] Add User-Agent header to responses    

    //==SDOC====================================================================
    //==
    //==  AddServerToResponses
    //==
    //==========================================================================
    //
    //  Summary:
    //      Determines whether or not the Server header is added to responses
    //      created by the SIP stack.
    //
    //  Parameters:
    //    bAdd:
    //      The Server header is added to responses when true, otherwise it is
    //      not added.
    //
    //  Description:
    //      Configures whether or not to add the Server header to responses
    //      created by the SIP stack. To determine what to put in that header,
    //      refer to SetApplicationId and AddStackVersionToId. Note that if
    //      added, the header is added only to responses created by the SIP
    //      stack and not to responses forwarded by it.
    //
    //      By default, the Server header is added to responses.
    //
    //      This method can be called at any time.
    //
    //  See Also:
    //      SetApplicationId, AddStackVersionToId, AddServerToResponses
    //
    //==EDOC====================================================================
    virtual void AddServerToResponses(IN bool bAdd) = 0;

    //==SDOC====================================================================
    //==
    //==  SetMaxForwards
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the value to put in the Max-Forwards header of the requests
    //      created by this class.
    //
    //  Parameters:
    //    uMaxForwards:
    //      The value to put in the Max-Forwards header of the request created
    //      by the CSipPacket class.
    //
    //  Description:
    //      Sets the value to put in the Max-Forwards header of the request
    //      created by the CSipPacket class. By default, this value is 70, the
    //      value recommended by RFC 3261.
    //
    //==EDOC====================================================================
    virtual void SetMaxForwards(IN unsigned int uMaxForwards) = 0;

    //==========================================================================
    //==
    //==  SetUaResponseMultipleViasCheck
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the multiple Via headers check in incoming responses destined
    //      to user-agents.
    //
    //  Parameters:
    //    bCheckMultipleVias:
    //      True if the transport needs to check for multiple Vias in user-agent
    //      responses. If false nothing is done, this is the default.
    //
    //    pvecstrProxyIds:
    //      This needs to be set when the application has at least one
    //      user-agent and one proxy within the same stack instance. If the
    //      application is only a UA, this must be NULL.
    //
    //      This must contain the proxy IDs as set in the CSipProxyConfig::GetId
    //      method. If the application has only one proxy ID, then the vector
    //      must contain only one ID. When there are many proxies, it is
    //      possible that there is one ID per proxy.
    //
    //  Returns:
    //      - resS_OK: success.
    //      - resFE_FAIL: the transport thread is not set or pvecstrProxyIds is
    //      empty.
    //
    //  Description:
    //      This method is useful only when the application is acting as a
    //      user-agent. It is used to drop responses that contain more than one
    //      Via header within an incoming response. When enabled
    //      (bCheckMultipleVias set to true), all responses destined to a
    //      user-agent are dropped (discarded). This is to be compliant to
    //      section 8.1.1.3 of the RFC 3261.
    //
    //      If this setting is not enabled, then the responses containing more
    //      than one Via header are processed as usual: e.g. transport observers
    //      see such responses even if there are more than one Via.
    //
    //      When the application is acting as a proxy, this method must not be
    //      called. If the application is acting both as a proxy and a
    //      user-agent, the method must be called only if the application wants
    //      to discard the responses containing multiple Vias.
    //
    //      When acting as a proxy, the stack can receive responses with more
    //      than one Via. This is because the proxy adds a Via header before
    //      forwarding a request. The proxy encodes specific information when
    //      generating the Via. Hence it is possible to determine if a Via was
    //      generated by the stack or not. This means that when
    //      bCheckMultipleVias is true and pvecstrProxyIds has at least one
    //      proxy ID, the response is discarded only when the Via was not
    //      generated by the stack.
    //
    //      By default, the check for multiple Vias is disabled
    //
    //  See also:
    //      CSipProxyConfig::GetProxyId
    //
    //==============================================================================
    virtual mxt_result SetUaResponseMultipleViasCheck(IN bool bCheckMultipleVias,
                                                      IN TOA CVector<CString>* pvecstrProxyIds = NULL) = 0;

    //==========================================================================
    //==
    //==  IgnoreTopViaSentByInReceivedResponses
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows to ignore the validation of the top Via sent-by of the
    //      received responses.
    //
    //  Parameters:
    //      bIgnore:
    //       True if the transport needs to ignore the sent-by of the top Via
    //       header in received responses. False to force the transport to
    //       validate the top Via sent-by.
    //
    //  Returns:
    //      - resS_OK: success.
    //      - resFE_FAIL: the transport thread is not set
    //
    //  Description:
    //      Some servers change the top Via header sent-by value in responses
    //      they send back to user-agents. The top Via sent-by must not change
    //      in responses according to the SIP standards (it must be the same as
    //      the one found in the originating request). The stack currently
    //      validates these Via sent-by values and ignores (drops) the responses
    //      that contain values that have been changed.
    //
    //      Application whishing that the stack does not drop responses that
    //      contained a modified Via sent-by may use this method and pass a
    //      value of bIgnore set to true.
    //
    //      The default value of bIgnore is false meaning the stack validates
    //      the Via sent-by in responses and drops the responses if the sent-by
    //      value has changed.
    //
    //==========================================================================
    virtual mxt_result IgnoreTopViaSentByInReceivedResponses(IN bool bIgnore) = 0;

#ifdef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
    //==SDOC====================================================================
    //==
    //==  AddViaRportParam
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds the rport parameter to the Via header
    //
    //  Parameters:
    //    bAddRport:
    //      True to add an rport parameter to the Via header in all requests,
    //      false otherwise.
    //
    //  Description:
    //      Adds the rport parameter to the Via header of all outgoing requests.
    //      RFC 3581 permits the addition of the rport parameter to force
    //      UAs to reply on the same UDP address/port on which the request was
    //      received. The local address/port used to receive the packet should
    //      also be used for NAT traversal purposes.
    //
    //==EDOC====================================================================
    virtual void AddViaRportParam(IN bool bAddRport) = 0;
#endif

    //==========================================================================
    //==
    //==  ForceVisibleLocalAddress
    //==
    //==========================================================================
    // <FLAG new in v4.1.8>
    //
    //  Summary:
    //      Allows the application to override the hostport part of the Via and
    //      Record-Route headers in sent packets.
    //
    //  Parameters:
    //      opqAddress:
    //          The opaque that identifies the IP address to be
    //          overridden. This opaque has been received from
    //          ISipCoreConfig::AddLocalAddress.
    //
    //      pVisibleAddress:
    //          The hostport to use in Via and Record-Route headers when the
    //          selected sending address is the one identified by opqAddress.
    //          Ownership of the pointer is ALWAYS taken from the caller.
    //
    //          NULL removes the last set visible address.
    //
    //  Returns:
    //      - Success: The visible address has been successfully set.
    //      - Failure: The visible address has NOT been set. This can occur if
    //                 the opqAddress parameter is invalid.
    //
    //  Description:
    //      This method sets and forces the "visible" address for a given
    //      local interface to be reported as the content of the pVisibleAddress
    //      parameter.
    //
    //      For example, the application has a network interface that uses
    //      address 10.1.1.1 and it would like to report it as 10.2.2.2 for some
    //      reason. The application will:
    //      1. add the local address 10.1.1.1 through
    //         ISipCoreConfig::AddLocalAddress and receive an opaque value to
    //         identify it.
    //      2. create a CHostPort instance and set its host part to 10.2.2.2.
    //      3. call ISipCoreConfig::ForceVisibleLocalAddress with the received
    //         opaque and the host port instance.
    //
    //      From this point, whenever 10.1.1.1 is used to send requests, the Via
    //      will be set to 10.2.2.2.
    //
    //      One scenario where this can be helpful is when a NAT Proxy does not
    //      add a Via header for the public address. If the application knows
    //      its public address, it can set it properly by using this method.
    //
    //      This method causes the following overrides and behaviours on the
    //      SIP-UA config:
    //      - Via and Record-Route headers use the visible address instead
    //        of the interface address.
    //      - CSipNetworkInterfaceList::GetLocalAddress returns the forced
    //        visible address instead of the interface address/FQDN.
    //      - <B>The EAddressTypePreference configuration is overridden.</B>
    //
    //  This method can be called at any time after the threading configuration
    //  of the SIP stack.
    //
    //==========================================================================
    virtual mxt_result ForceVisibleLocalAddress(IN mxt_opaque opqAddress,
                                                IN TOA CHostPort* pVisibleAddress) = 0;

    //==========================================================================
    //==
    //==  UpdateParserGrammar
    //==
    //==========================================================================
    //
    //  Summary:
    //      Updates the grammar of a given character set.
    //
    //  Parameters:
    //    uIndex:
    //      Index of the value to change in eCharSet.
    //
    //    bValue:
    //      Whether or not the character at the specified index in the specified
    //      character set is legal.
    //
    //    eCharSet:
    //      Character set in which to change a grammar value.
    //
    //  Returns:
    //      - resS_OK upon success.
    //      - resFE_INVALID_ARGUMENT if eCharSet or uIndex is invalid.
    //
    //  Description:
    //      Changes whether or not an element is legal in a specified character
    //      set. An element's index is the same as in an ASCII table. For more
    //      information on the current character set, please refer to RFC 3261
    //      section 25.1.
    //
    //  Warning:
    //      Changing the grammar in any way can have important side effects and
    //      may potentially cause crashes. Media5 should be consulted before doing
    //      so.
    //
    //  #Notes:#
    //      Must be called before ISipCoreConfig::Startup.
    //
    //==========================================================================
    virtual mxt_result UpdateParserGrammar(IN const unsigned int uIndex,
                                           IN const bool bValue,
                                           IN const CToken::ECharSet eCharSet) = 0;

    //-- << Transport Configuration >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetPrincipalBufferSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the size of the principal buffer.
    //
    //  Parameters:
    //    uSize:
    //      The size, in bytes, of the memory to allocate for the principal
    //      buffer.
    //
    //  Returns:
    //      - resS_OK: Updated successfully.
    //
    //      - resFE_FAIL: Unable to update buffer size.
    //
    //  Description:
    //      This method is used to reserve a memory space for the allocation of
    //      the principal buffer. This buffer is the region from where raw SIP
    //      packets are sent and where they are received. Only one such buffer
    //      is used for the whole SIP stack.
    //
    //      The buffer size should correspond to the maximum packet size
    //      (including the payload) the application is ready to receive or send
    //      over UDP. The following are specifications from RFC 3261:
    //
    //      <CODE>
    //
    //      However, implementations MUST be able to handle messages up to the
    //      maximum datagram packet size. For UDP, this size is 65 535 bytes
    //      including IP and UDP headers.
    //
    //      </CODE>
    //
    //      By default, the SIP stack is initialized with a principal buffer
    //      size of 65535 bytes.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetPrincipalBufferSize(IN unsigned int uSize) = 0;

    //==SDOC====================================================================
    //==
    //==  SetMaxSendBufferSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the socket's transmission buffer size.
    //
    //  Parameters:
    //    uSize:
    //      The size, in bytes, of the socket's transmission buffer.
    //
    //  Returns:
    //      - resS_OK: If properly configured.
    //
    //      - resFE_FAIL: Unable to update buffer size.
    //
    //  Description:
    //      This method sets the connections' transmission buffer size. This
    //      buffer is used when subsequent SendA are fired on the same
    //      connection, which is not yet established or is congested.
    //
    //      There is not a fixed pre-allocated buffer; rather, each time a
    //      message must be buffered, a buffer is created and enqueued. Thus,
    //      the maximum size corresponds to the maximum accepted for the
    //      cumulated sizes of all buffers present in the queue at a given time.
    //
    //      This buffer size is not shared across multiple connections. Each
    //      connection can use up to uSize bytes as send buffer.
    //
    //      This buffer size should be at least the same size as the principal
    //      buffer, or larger. Otherwise, the SIP stack will not be able to
    //      buffer large SIP packets being sent.
    //
    //      By default, the SIP stack is initialized with a maximum send buffer
    //      size of 65535 bytes.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //  See Also:
    //      SetPrincipalBufferSize
    //
    //==EDOC====================================================================
    virtual mxt_result SetMaxSendBufferSize(IN unsigned int uSize) = 0;


// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
    //==SDOC====================================================================
    //==
    //==  SetBypassSubjectAltNameCheck
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the SubjectAltName extension check option.
    //
    //  Parameters:
    //    bBypass:
    //      True to bypass the SubjectAltName extension check with the
    //      Request-URI host.
    //
    //  Returns:
    //      - resS_OK: If properly configured.
    //
    //      - resFE_FAIL: Unable to configure the option.
    //
    //  Description:
    //      This method sets the SubjectAltName extension check option.
    //
    //      By default, the SIP stack is initialized to enable SubjectAltName
    //      extension check.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //  See Also:
    //      N/A
    //
    //==EDOC====================================================================
    virtual mxt_result SetBypassSubjectAltNameCheck(IN bool bBypass) = 0;
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check


    //==SDOC====================================================================
    //==
    //==  SetMaxPayloadSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum payload size that can be accepted by the SIP
    //      stack.
    //
    //  Parameters:
    //    uSize:
    //      The size, in bytes, of the payload.
    //
    //  Returns:
    //      - resS_OK: Max payload size properly configured.
    //
    //      - resFE_FAIL: Unable to set max payload size.
    //
    //  Description:
    //      When parsing a received SIP packet, the packet is considered
    //      erroneous if the payload is larger than uSize. Therefore it is
    //      rejected and an error is generated. A 413 "Request Entity Too Large"
    //      response is sent to the peer.
    //
    //      By default, the SIP stack is initialized with a maximum receivable
    //      payload size of 1024 bytes.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetMaxPayloadSize(IN unsigned int uSize) = 0;


    //==SDOC====================================================================
    //==
    //==  SetMaxReceivePacketSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum receivable size of SIP packet that can be accepted
    //      by the SIP stack.
    //
    //  Parameters:
    //    uSize:
    //      The size, in bytes, of the receiving packet.
    //
    //  Returns:
    //      - resS_OK: Max receive packet size properly configured.
    //
    //      - resFE_FAIL: Unable to set max receive packet size.
    //
    //  Description:
    //      When parsing a received SIP packet, the packet is considered
    //      erroneous if it is larger than uSize (WITHOUT THE PAYLOAD).
    //      Therefore it is rejected and the connection is closed.
    //
    //      Packets received over UDP are not affected, since the maximum UDP
    //      packet size is configured at the IP stack level.
    //
    //      By default, the SIP stack is initialized with a maximum receivable
    //      packet size of 64K bytes (64 * 1024).
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetMaxReceivePacketSize(IN unsigned uSize) = 0;

#ifdef MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
    //==SDOC====================================================================
    //==
    //==  SetUdpMaxSizeThreshold
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Sets the threshold above which a request is first tried using
    //      TCP when no transport mechanism is specified
    //      (as per RFC 3261 section 18.1.1).
    //
    //  Parameters:
    //    uSize:
    //      The size, in bytes, of the UDP maximum size threshold.
    //      Zero (0) has a special meaning, effectively setting an infinite
    //      size and thus disabling the UDP maximum size threshold.
    //
    //  Description:
    //      RFC 3261 section 18.1.1 specifies that a request within 200 bytes
    //      of the path MTU, or larger than 1300 bytes is the path MTU is
    //      unknown, MUST be sent using a congestion controlled transport
    //      protocol, such as TCP.
    //
    //      This method sets the threshold above which this TCP override
    //      happens. It defaults to 1300 bytes.
    //
    //      As specified in RFC 3261, if the request is sent using
    //      TCP because of this size constraint but would otherwise have
    //      been sent using UDP, and that request subsequently fails, it
    //      is retried using UDP.
    //
    //      The server location service uses the value specified by this
    //      method to determine the maximum size a packet should have,
    //      given its transport protocol. When the server location service
    //      is not enabled, the maximum size defaults to infinite. However,
    //      this should not happen under normal circumstances because all use
    //      cases require this service to be enabled.
    //
    //  Note:
    //      This method can be called only after the SIP core thread
    //      is set.
    //
    //==EDOC====================================================================
    virtual void SetUdpMaxSizeThreshold(IN unsigned uSize) = 0;
#endif

    //==SDOC====================================================================
    //==
    //==  SetConnectionParameters
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures SIP stack connection parameters.
    //
    //  Parameters:
    //    uNumConnectionTarget:
    //      The target number of connections that are to be left open by the
    //      SIP stack. This must be smaller than or equal to
    //      uMaxNumberOfConnections.
    //
    //    uMaxNumberOfConnections:
    //      The maximum number of connections that the SIP stack is allowed to
    //      simultaneously manage. In the event that a new connection is
    //      required and the SIP stack already manages this number of
    //      connections, the least recently used connection is replaced with
    //      the new connection.
    //
    //    uInactivityTimeoutSec:
    //      The inactivity timeout, in seconds, to apply to the connections that
    //      are over the target number of connections. It should be at least 32
    //      seconds.
    //
    //  Returns:
    //      - resS_OK: Parameters properly configured.
    //
    //      - resFE_FAIL: Unable to set configuration parameters.
    //
    //  Description:
    //      This method is used to configure how the SIP stack keeps and
    //      manages its connections with other SIP devices on the network.
    //
    //      A connection for the SIP stack is a connected socket to a peer. The
    //      SIP stack can connect all types of sockets it creates: UDP, TCP, or
    //      TLS. The SIP stack obviously uses connected sockets when sending
    //      SIP packets over TCP or TLS. The SIP stack also "connects" UDP
    //      sockets to a peer when sending a packet over UDP, which allows the
    //      detection of some ICMP errors, such as host/port unreachable. The
    //      detection of these errors allows faster failover.
    //
    //      A connection to a peer is created when the SIP stack has to send a
    //      SIP packet to that peer. Once such a connection is created, the SIP
    //      stack manages it according to the description below.
    //
    //      Internally, the SIP stack keeps an ordered list of connections with
    //      the most recently used connections appearing first in the list and
    //      the least recently used appearing last. Each time a connection is
    //      used, its inactivity counter is reset to zero and the connection is
    //      placed at the head of the list.
    //
    //      The "uNumConnectionTarget" first connections are never terminated
    //      automatically by the SIP stack, regardless of the value of their
    //      inactivity counter.
    //
    //      Each connection over uNumConnectionTarget is watched for inactivity
    //      timeout. When its inactivity counter reaches uInactivityTimeoutSec,
    //      the connection is automatically terminated by the SIP stack.
    //
    //      This has the effect that the SIP stack tends to keep
    //      uNumConnectionTarget connections open when in normal runstate. When
    //      encountering a busy period, the SIP stack can manage up to
    //      uMaxNumberOfConnections, but after this period, the connections over
    //      uNumConnectionTarget are released along with their corresponding
    //      resources.
    //
    //      The following ASCII graphic shows the SIP stack starting at zero
    //      connections, and then building its connection list up to the target
    //      number of connections. A busy peak is then encountered, where
    //      additional connections are required. Eventually, when the traffic
    //      slows down, the number of connections is brought back to the target
    //      number of connections.
    //
    //      <CODE>
    //
    //     Num Connections
    //          ^
    // max----> |                        _______
    //          |                       /       \\_
    //          |                      /          \\_
    //          |                   __/             \\.
    //          |                  /                 \\_           _
    //          |                 /                    \\         / \\.
    // target-> |     ___________/                      \\____/\\_/   \\____
    //          |  __/
    //          | /
    //          +--------------------------------------------------------->
    //                                Time
    //      </CODE>
    //
    //      By default, the SIP stack is initialized with the following values:
    //          - Target number of connections: 5
    //          - Maximum number of connections: 10
    //          - Connection inactivity timeout: 64 seconds
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //      In order to use an non-connected socket for sending SIP packets over
    //      UDP, the application must use the ISipSymmetricUdpSvc. This service
    //      allows using the unconnected listening UDP socket for sending SIP
    //      packets.
    //
    //      To create a connection to a peer that must always remain
    //      established, whether or not there is traffic on it, use the
    //      CSipPersistentConnectionList and the ISipPersistentConnectionSvc.
    //      These services give control to the SIP stack user on the creation
    //      and termination of connections to a peer.
    //
    //==EDOC====================================================================
    virtual mxt_result SetConnectionParameters(
                                     IN unsigned int uNumConnectionTarget,
                                     IN unsigned int uMaxNumberOfConnections,
                                     IN unsigned int uInactivityTimeoutSec) = 0;

    //==SDOC====================================================================
    //==
    //==  SetPacketInspectorCallback
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a callback that the SIP stack calls upon the reception of
    //      a complete SIP packet.
    //
    //  Parameters:
    //    pfnTransportPacketInspector:
    //      Method used to modify the packet. PFNTransportPacketInspector is a
    //      typedef void (CSipPacketParser::SRawData*&)
    //
    //  Returns:
    //      - resS_OK: Parameters properly configured.
    //
    //      - resFE_FAIL: Unable to set configuration parameters.
    //
    //  Description:
    //      Configures a callback that the SIP stack calls upon the reception of
    //      a complete SIP packet.
    //
    //      The SIP stack provides the received data as raw data. The callback
    //      can modify the received data before the data is actually parsed,
    //      when the callback returns.
    //
    //      The SIP stack calls the configured callback within the context
    //      of the SIP transport thread.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetPacketInspectorCallback(
              IN PFNTransportPacketInspector pfnTransportPacketInspector) = 0;

    //==SDOC====================================================================
    //==
    //==  SetPacketModifierCallback
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a callback that the SIP stack calls upon the reception
    //      and parsing of a complete SIP packet.
    //
    //  Parameters:
    //    pfnTransportPacketModifier:
    //      Method used to modify the packet. PFNTransportPacketModifier is a
    //      typedef void (CSipPacket&)
    //
    //  Returns:
    //      - resS_OK: Parameters properly configured.
    //
    //      - resFE_FAIL: Unable to set configuration parameters.
    //
    //  Description:
    //      Configures a callback that the SIP stack calls upon the reception
    //      of a complete SIP packet.
    //
    //      The SIP stack provides a parsed SIP packet. The callback should
    //      modify the headers of the provided SIP packet before the packet is
    //      handled by the SIP stack.
    //
    //      The callback MUST NOT modify the following mandatory header:
    //          - From
    //          - To
    //          - Call-ID
    //          - Via
    //          - CSeq
    //
    //      This callback is not called for packets that were received and
    //      discarded because of invalid above mandatory header.
    //
    //      The SIP stack calls the configured callback within the context
    //      of the SIP transport thread. Any packet modifications MUST be done
    //      synchronously.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetPacketModifierCallback(
                  IN PFNTransportPacketModifier pfnTransportPacketModifier) = 0;

    //==SDOC====================================================================
    //==
    //==  SetSipDataLogger
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the data logger.
    //
    //  Parameters:
    //    pDataLogger:
    //      The reference to the data logger.
    //
    //  Description:
    //      Configures the data logger that is used for logging SIP packets
    //      and raw data sent and received from the network.
    //
    //      The SIP stack does NOT take ownership of the given data logger.
    //      Thus, the application must make sure the pointer stays valid as long
    //      as the stack knows it.
    //
    //      The application is also responsible for freeing the resources of the
    //      given data logger.
    //
    //  Note:
    //      This method can be called only after the SIP transport thread
    //      is set.
    //
    //==EDOC====================================================================
    virtual void SetSipDataLogger(IN ISipDataLogger* pDataLogger) = 0;

    //==SDOC====================================================================
    //==
    //==  SetSupportedSipTransport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the transport protocol the SIP stack may use for sending SIP
    //      packets.
    //
    //  Parameters:
    //    nTransportBitset:
    //      The transport(s) the SIP stack may use. This value can be a
    //      concatenation of various values created by using the bitwise "OR"
    //      operator on the ESipTransport enumeration.
    //
    //  Description:
    //      Sets the SIP transport protocol the SIP stack may use for sending
    //      SIP packets. Multiple transports can be concatenated to set them
    //      all at once. The previously configured value is lost.
    //
    //      By default, eUDP, eTCP, and eTLS are supported.
    //
    //      This method can be called at any time after the DNS resolver thread
    //      is set.
    //
    //      Example:
    //      <code>
    //          ISipCoreConfig* pConfig = NULL;
    //
    //          CreateEComInstance(CLSID_CSipCoreConfig, NULL, OUT &pConfig);
    //
    //          if (pConfig != NULL)
    //          {
    //              pConfig->SetSupportedSipTransport(static_cast<ESipTransport>(eTCP | eUDP));
    //              pConfig->ReleaseIfRef();
    //          }
    //      </code>
    //
    //  See Also:
    //      ESipTransport in CSipTransportTools.
    //
    //==EDOC====================================================================
    virtual void SetSupportedSipTransport(IN int nTransportBitset) = 0;

    //==SDOC====================================================================
    //==
    //==  SetSupportedDnsQueries
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the type(s) of DNS queries supported by the application.
    //
    //  Parameters:
    //    nDnsQueryBitset:
    //      The DNS query type(s) the SIP stack may use. This value can be a
    //      concatenation of various values created by using the bitwise "OR"
    //      operator on the ESipDnsQuery enumeration.
    //
    //  Description:
    //      This method sets the type(s) of DNS queries that the SIP stack
    //      supports and uses. It is possible to disable <C>NAPTR</C> AND
    //      <C>SRV</C> queries or only <C>NAPTR</C> queries. The sample code
    //      below shows how to perform those operations.
    //
    //      By default, NAPTR and SRV records are active.
    //
    //      This method can be called at any time after the DNS resolver thread
    //      is set.
    //
    //  NOTES:
    //      Since <C>NAPTR</C> queries require <C>SRV</C> queries, it is not
    //      possible to enable <C>NAPTR</C> without also enabling <C>SRV</C>.
    //      The SIP stack automatically handles this case.
    //
    //      <B>In all cases, the SIP stack always supports <C>A</C> queries and
    //      uses them as needed.</B>
    //
    //      Example:
    //      <CODE>
    //      ISipCoreConfig* pConfig = NULL;
    //
    //      CreateEComInstance(CLSID_CSipCoreConfig, NULL, OUT &pConfig);
    //
    //      if (pConfig != NULL)
    //      {
    //          // To disable NAPTR and SRV queries at once, call:
    //          //------------------------------------------------
    //          pConfig->SetSupportedDnsQueries(eNONE);
    //
    //          // To enable only NAPTR queries, call:
    //          // (As a side effect, SRV queries are also enabled).
    //          //---------------------------------------------------
    //          pConfig->SetSupportedDnsQueries(eNAPTR);
    //
    //          // To enable only SRV queries, call:
    //          //----------------------------------
    //          pConfig->SetSupportedDnsQueries(eSRV);
    //
    //          // To explicitly enable both types of queries, call:
    //          //---------------------------------------------------
    //          pConfig->SetSupportedDnsQueries(eNAPTR | eSRV);
    //          pConfig->SetSupportedDnsQueries(static_cast<ESipDnsQuery>(eNAPTR | eSRV));
    //
    //          // Release the ISipCoreConfig reference.
    //          //---------------------------------------
    //          pConfig->ReleaseIfRef();
    //      }
    //      </CODE>
    //
    //  See Also:
    //      ESipDnsQuery in CServerLocator.h
    //
    //==EDOC====================================================================
    virtual void SetSupportedDnsQueries(IN int nDnsQueryBitset) = 0;

    //==========================================================================
    //==
    //==  SetFailoverMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the failover mode.
    //
    //  Parameters:
    //    eFailoverMode:
    //      The failover mode.
    //
    //  Description:
    //      Sets the failover mode to use.
    //
    //      The failover mode controls the actions taken when a packet failed
    //      to be sent.
    //
    //      The default mode is eFAILOVER_DEFAULT.
    //
    //      Example:
    //      <code>
    //          ISipCoreConfig* pConfig = NULL;
    //
    //          CreateEComInstance(CLSID_CSipCoreConfig, NULL, OUT &pConfig);
    //
    //          if (pConfig != NULL)
    //          {
    //              pConfig->SetFailoverMode(ISipCoreConfig::eFAILOVER_DEFAULT);
    //
    //              pConfig->ReleaseIfRef();
    //              pConfig = NULL;
    //          }
    //      </code>
    //
    //  Note:
    //      This method can be called only after the DNS resolver thread
    //      is set.
    //
    //==========================================================================
    virtual void SetFailoverMode(IN EFailoverMode eFailoverMode) = 0;

    //==SDOC====================================================================
    //==
    //==  SetCSeq64BitsSupport
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets if a 64 bits sequence number is supported.
    //
    //  Parameters:
    //    bSupported:
    //      True when a 64bits sequence number must be supported. False for a
    //      32 bits.
    //
    //  Description:
    //      Sets if a 64 bits sequence number is supported.
    //
    //  Note:
    //      This method can be called only after the SIP transport thread
    //      is set.
    //
    //==EDOC====================================================================
    virtual void SetCSeq64BitsSupport(IN bool bSupported) = 0;

    //==SDOC====================================================================
    //==
    //==  GetSupportedIpVersion
    //==
    //==========================================================================
    //
    //  Summary:
    //      This gets the currently configured supported IP version.
    //
    //  Returns:
    //      The version(s) that is (are) currently supported.
    //
    //  Description:
    //      This gets the currently configured supported IP version.
    //
    //  See Also:
    //      ISipCoreConfig::EIpVersionConfig
    //
    //==EDOC====================================================================
    virtual EIpVersionConfig GetSupportedIpVersion() = 0;

    //==SDOC====================================================================
    //==
    //==  SetSupportedIpVersion
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures which versions of TCP/IP are supported or enabled by the
    //      application.
    //
    //  Parameters:
    //    eIpVersion:
    //      The version(s) that the application wants to support.
    //
    //  Description:
    //      This configures whether the application wants to support only
    //      IPv4, only IPv6, or both IPv4 and IPv6. This is then used by the SIP
    //      stack to determine if it needs to perform A or AAAA queries when
    //      resolving addresses. Note that MXD_SIPSTACK_IPV6_ENABLE_SUPPORT
    //      must be defined to be able to use IPv6. If
    //      MXD_SIPSTACK_IPV6_ENABLE_SUPPORT is not defined, IPv4 is enabled
    //      by default. If it is defined, IPv4 and IPv6 are enabled by default.
    //
    //      When eCONFIG_IPV4_AND_IPV6 is passed, IPv4 addresses are preferred
    //      over IPv6 addresses. The reverse is true when eCONFIG_IPV4_AND_IPV4
    //      is passed.
    //
    //      Example:
    //      <code>
    //          ISipCoreConfig* pConfig = NULL;
    //
    //          CreateEComInstance(CLSID_CSipCoreConfig, NULL, OUT &pConfig);
    //
    //          if (pConfig != NULL)
    //          {
    //              pSipCoreConfig->SetSupportedIpVersion(ISipCoreConfig::eCONFIG_IPV4);
    //              pConfig->ReleaseIfRef();
    //          }
    //      </code>
    //
    //  Note:
    //      This method can be called only after the DNS resolver thread
    //      is set.
    //
    //  See Also:
    //      ISipCoreConfig::EIpVersionConfig, MXD_SIPSTACK_IPV6_ENABLE_SUPPORT
    //
    //==EDOC====================================================================
    virtual void SetSupportedIpVersion(IN EIpVersionConfig eIpVersion) = 0;

#ifdef MXD_SIPSTACK_ENABLE_TLS
    //==SDOC====================================================================
    //==
    //==  GetTlsContextFactory
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Get the TLS context factory used to create TLS contexts.
    //
    //  Parameters:
    //    rpTlsContextFactory:
    //      Returns a pointer to the TLS context factory. A reference is counted
    //      for this object when mxt_result is a success.
    //
    //  Returns:
    //      - resS_OK: The TLS context factory is returned in the OUT parameter.
    //
    //      - resFE_INVALID_STATE: the transport thread is not initialized.
    //
    //  Description:
    //      Returns the TLS context factory used to create TLS contexts. When
    //      called for the first time, this method creates the factory if
    //      needed. Since the factory is synchronized in the transport thread,
    //      the transport thread must be set before calling this method.
    //
    //      The application should configure the TLS context factory at stack
    //      startup. However, the TLS context factory can be modified while
    //      the stack is running.
    //
    //  See Also:
    //      ISipTlsContextFactory, SetTransportThread
    //
    //==EDOC====================================================================
    virtual mxt_result GetTlsContextFactory(OUT ISipTlsContextFactory*& rpTlsContextFactory) = 0;

    //==SDOC====================================================================
    //==
    //==  SetHandshakeValidatorCallback
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures a callback for the handshake validation.
    //
    //  Parameters:
    //    pfnHandshakeValidator:
    //      Method used to approve or reject the handshake.
    //      PFNHandshakeValidator is a typedef mxt_result (bool bIsClient, bool
    //      bPeerAuth, const CCertifiateChain& rChain, CString&
    //      rstrPeerHostname).
    //
    //    bOverrideDefaultBehaviour:
    //       True when pfnHandshakeValidator will override the default behaviour
    //       of the handshake validator. When false, pfnHandshakeValidator will
    //       be invoked prior to the default handshake validator.
    //
    //  Returns:
    //      - resS_OK: Parameters properly configured.
    //
    //      - resFE_FAIL: Unable to set configuration parameters.
    //
    //  Description:
    //      Configures a callback function that allows the user of the SIP
    //      stack to approve or reject the TLS handshake procedure. The
    //      following is the SIP stack's default behaviour upon handshake
    //      validation:
    //
    //      Handshake approval for TLS server connections:
    //
    //      - Connection is accepted if peer authentication is disabled.
    //
    //      - Connection is dropped if remote peer hostname is not found in the
    //      personal certificate.
    //
    //      - Connection is accepted if remote peer hostname is found in the
    //      personal certificate.
    //
    //      Handshake approval for TLS client connections:
    //
    //      - Connection is dropped if peer authentication is disabled.
    //
    //      - Connection is dropped if remote peer hostname is not found in the
    //      personal certificate.
    //
    //      - Connection is accepted if remote peer hostname is found in the
    //      personal certificate.
    //
    //      The parameters of the callback methods are the following:
    //
    //      bIsClient:
    //       True if the handshake validation is on a TLS client connection.
    //       Otherwise false.
    //
    //      bPeerAuth:
    //       True if the remote peer authenticaion is enabled. Otherwise false.
    //
    //      rChain:
    //       The certificate chain returned by the remote peer. This is where
    //       the personal certificate is stored.
    //
    //      rstrPeerHostname:
    //       The remote peer hostname.
    //
    //      The application can define such a callback to:
    //        - Implement a different logic than the default behaviour described
    //          above by setting bOverrideDefaultBehaviour to true.
    //        - Implement a custom pre-processing logic on top of the default
    //          behaviour described above by setting bOverrideDefaultBehaviour
    //          to false.
    //
    //      The SIP stack calls the configured callback within the context
    //      of the SIP transport thread.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetHandshakeValidatorCallback(
                        IN PFNHandshakeValidator pfnHandshakeValidator,
                        IN bool bOverrideDefaultBehaviour = true) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTlsSessionCacheMaxSize
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the maximum size of the cached TLS session list.
    //
    //  Parameters:
    //    uTlsSessionCacheMaxSize:
    //      Maximum size for the list of cached TLS session.
    //
    //  Returns:
    //      - resS_OK: Cached TLS session list maximum size set.
    //
    //      - resFE_FAIL: Unable to set cached TLS session list maximum size.
    //
    //      - resFE_INVALID_ARGUMENT: Maximum size must be at least 1.
    //
    //  Description:
    //      Sets the maximum size of the cached TLS session list. The maximum
    //      size must be at least 1.
    //
    //==EDOC====================================================================
    virtual mxt_result SetTlsSessionCacheMaxSize(IN uint32_t uTlsSessionCacheMaxSize) = 0;

#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

#if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)
    //==SDOC====================================================================
    //==
    //==  SetSpirallingSvcState
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the Spiralling Service state.
    //
    //  Parameters:
    //    eState:
    //      The new state of the Spiralling Service.
    //
    //  Returns:
    //    resS_OK:
    //      The new Spiralling Service state is configured.
    //
    //    resFE_INVALID_STATE:
    //      The SIP stack has been started.
    //
    //  Description:
    //      Configures whether or not the Spiralling Service is enabled.
    //
    //      This method can be called only before the startup of the SIP stack.
    //
    //==EDOC====================================================================
    virtual mxt_result SetSpirallingSvcState(IN ESpirallingSvcState eState) = 0;
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    //==========================================================================
    //==
    //==  SetKeepAliveExtensionMgrA
    //==
    //==========================================================================
    // <FLAG New in 4.1.6>
    //
    //  Summary:
    //      Sets a keep alive extension manager.
    //
    //  Parameters:
    //      pMgr:
    //       The manager.
    //
    //  Return:
    //      resFE_INVALID_STATE: the transport manager is not set.
    //      resS_OK: otherwise.
    //
    //  Description:
    //      The application should use the
    //      CSipPersistentConnectionList::SetKeepAliveExtensionMgr instead as
    //      it provides more flexibility. An application should never set a
    //      manager using this method if it used the
    //      CSipPersistentConnectionList::SetKeepAliveExtensionMgr. This would
    //      prevent the persistent connection list from working properly and
    //      managers set with
    //      CSipPersistentConnectionList::SetKeepAliveExtensionMgr will never
    //      get notified of events.
    //      The application can register this manager when it wants to be
    //      informed of non SIP data reception. This is useful when an
    //      application wants to send keep alives using STUN requests for
    //      instance. When the application is responsible to send STUN requests,
    //      it is also responsible to receive and interpret the STUN responses.
    //      This is currently possible by configuring this manager.
    //
    //==========================================================================
    virtual mxt_result SetKeepAliveExtensionMgrA(IN ISipKeepAliveExtensionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  EnableOutboundCrlfKeepAliveA
    //==
    //==========================================================================
    // <FLAG New in 4.1.6>
    //
    //  Summary:
    //      Enables the CRLF keep alive for reliable transports.
    //
    //  Parameters:
    //      bEnable:
    //       True to enable the CRLF keep alive. False indicates that there will
    //       be no keep alives using CRLF.
    //
    //  Return:
    //      resFE_INVALID_STATE: the transport manager is not set.
    //      resS_OK: otherwise.
    //
    //  Description:
    //      Enables the CRLF keep alive mechanism for reliable transports as
    //      described in the draft-ietf-sip-outbound-15.
    //
    //      By default, this keep alive mechanism is enabled when the compiling
    //      switch MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT is activated.
    //      The application needs to enable the mechanism in order to achieve
    //      sending keep alives to the server on reliable connections such as
    //      TCP and TLS. When enabled, the client sends keep alives using two
    //      CRLFs on each flow selected by the application.
    //
    //      Usually the application starts issuing keep alives when it receives
    //      a response to a REGISTER request that contains the Require header
    //      with outbound value.
    //
    //      When the CRLF keep alive is enabled, the stack is responsible to
    //      send the keep alives and receive the responses. The application does
    //      not need to interact with the stack for the CRLF keep alives.
    //
    //==========================================================================
    virtual mxt_result EnableOutboundCrlfKeepAliveA(IN bool bEnable) = 0;

#endif //#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
    //==========================================================================
    //==
    //==  SetNetworkReceptionControlMgrA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Sets the network reception control service manager.
    //
    //  Parameters:
    //    pMgr:
    //      The network reception control service manager.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      Sets the network reception control service manager. This allows the
    //      application to be notified of the network reception control service
    //      events.
    //
    //==========================================================================
    virtual mxt_result SetNetworkReceptionControlMgrA(IN ISipNetworkReceptionControlMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetPeerFloodBlockingA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Sets the Peer Flood Blocking option.
    //
    //  Parameters:
    //    uRawPacketCountLimit:
    //      The raw data packet count limit for which the peer will be
    //      blocked if exceeded. A value of 0 deactivates Peer Flood Blocking.
    //
    //    uReceptionWindowSizeMs:
    //      The delay, in milliseconds, to receive uRawPacketCountLimit.
    //      This enables the application to configure the size of the time
    //      window this option will consider to check if the peer needs to be
    //      blocked or not. A value of 0 means that all peers will be blocked
    //      automatically.
    //
    //    uBlockingTimeoutMs:
    //      The time, in milliseconds, for which the peer will be blocked. A value
    //      of 0 means that any peer to be blocked will be unblocked
    //      automatically.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      Sets the Peer Flood Blocking option. It allows the user to enable or
    //      disable the Peer Flood Blocking option of the Network Reception
    //      Control service. Peer Flood Blocking allows the SIP stack to
    //      automatically block a peer that sends too many packets, more than
    //      the application can process.
    //
    //      This option configuration parameters affect all peers globally, i.e.
    //      there is no way to block a peer longer or shorter than another.
    //
    //      This option is disabled by default.
    //
    //    #More on Peer Flood Blocking Option#
    //      This option works as a configurable double intersecting window. Both
    //      windows are as big as uReceptionWindowSizeMs and they both share
    //      one of their half as illustrated below.
    //
    //                      Window 1
    //              -----------------------
    //              |          |          |
    //              |                     |
    //              ==================================
    //                         |                     |
    //                         |          |          |
    //                         -----------------------
    //                                 Window 2
    //
    //      Every time a raw packet is received, it will be added into one of
    //      the three "halves" of those windows. If any of them exceeds
    //      uRawPacketCountLimit, the peer is blocked automatically.
    //      Similarly, if the total count for a window exceeds
    //      uRawPacketCountLimit, the peer is also blocked.
    //
    //      The double intersecting window design allows to trap cases where a
    //      burst would be received at the end of the window and through the
    //      next, which would go undetected in a single window design.
    //
    //    #WARNING#
    //      This option calculates raw data packets, which are not SIP packets.
    //      A SIP packet could be sent in one or more data packets in TCP for
    //      example, so care must be taken when setting the Peer Flood Blocking
    //      threshold to take that fact into consideration.
    //
    //      Also, blocking a peer means that all raw data received from it,
    //      regardless of the actual raw data origin, will be discarded, without
    //      any possibility to recover it. As such, if all data comes through a
    //      single SIP Proxy and that one of its peers sends too much data, the
    //      whole SIP Proxy will be blocked even though the other peers behaved
    //      correctly.
    //
    //==========================================================================
    virtual mxt_result SetPeerFloodBlockingA(IN uint32_t uRawPacketCountLimit,
                                             IN uint32_t uReceptionWindowSizeMs,
                                             IN uint32_t uBlockingTimeoutMs) = 0;
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
    //==========================================================================
    //==
    //==  SetPacketReceptionControlMgrA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Sets the packet reception control service manager.
    //
    //  Parameters:
    //    pMgr:
    //      The packet reception control service manager.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      Sets the packet reception control service manager. This allows the
    //      application to be notified of the packet reception control service
    //      events.
    //
    //==========================================================================
    virtual mxt_result SetPacketReceptionControlMgrA(IN ISipPacketReceptionControlMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  EnablePeerFilteringA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Enables the Peer Filtering option.
    //
    //  Parameters:
    //    bEnable:
    //      Tells if the Peer Filtering option is to be enabled or disabled.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      Enables the Peer Filtering option. It allows the user to enable or
    //      disable the Peer Filtering option of the Packet Reception Control
    //      service. Peer Filtering allows the SIP stack to automatically
    //      block packets from peers that were not explicitly approved by the
    //      application and let pass only the ones that match a peer in the list
    //      or the packets received with a method on the allowed list.
    //
    //      This option is disabled by default.
    //
    //    #More on Peer Filtering Option#
    //      This option maintains two separate lists, one of approved peers and
    //      another of allowed SIP methods. Both lists' content are populated
    //      by the application.
    //
    //      When a packet is first received, after being parsed, the Packet
    //      Reception Control service will check if Peer Filtering is enabled.
    //      If so, it will then check if the packet is coming from an approved
    //      peer. If not, it will finally check if the packet method is allowed
    //      to go through regardless.
    //
    //      At any point during that time, if the packet (request or response)
    //      is found to be from an unapproved peer and a disallowed method,
    //      the packet will be automatically discarded. If the packet either
    //      match an approved peer or an allowed method, it will be forwarded
    //      up the transport service chain, eventually reaching the
    //      application.
    //
    //      The approved peer list contains the list of peers from which the
    //      application always wishes to receive all packets. A peer is typically
    //      added in that list once approved by the application, one way or another.
    //
    //      The allowed method list contains the list of SIP methods from which the
    //      application always wishes to receive packets. A method is
    //      typically added in that list so that peers that are yet to be
    //      approved can contact the application for approbation. Typically, a
    //      method like REGISTER would be in that list.
    //
    //    #WARNING#
    //      This option blocks any SIP packets not matching either the
    //      configured approved peers or the configured allowed methods. As
    //      such, it is the responsibility of the application to make sure those
    //      lists are properly populated if it wants to receive any SIP packets
    //      once this option is enabled.
    //
    //==========================================================================
    virtual mxt_result EnablePeerFilteringA(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  AddApprovedPeerA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Adds a peer in the Peer Filtering option approved peers list.
    //
    //  Parameters:
    //    rPeerAddr:
    //      The peer address to add to the peer list. Port can be 0, in which
    //      case any connection matching the peer's IP address will be approved.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to add a peer at any time in the Peer Filtering
    //      option approved peers list, if not already present.
    //
    //==========================================================================
    virtual mxt_result AddApprovedPeerA(IN const CSocketAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  RemoveApprovedPeerA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Removes a peer from the Peer Filtering option approved peers list.
    //
    //  Parameters:
    //    rPeerAddr:
    //      The peer address to remove from the peer list. Port can be 0, in
    //      which case any approved peer matching the specified IP address will
    //      be removed.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to remove a peer at any time from the Peer
    //      Filtering option approved peers list, if already present.
    //
    //==========================================================================
    virtual mxt_result RemoveApprovedPeerA(IN const CSocketAddr& rPeerAddr) = 0;

    //==========================================================================
    //==
    //==  RemoveAllApprovedPeersA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Removes all entries in the Peer Filtering option approved peers list.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to easily remove all entries in the Peer
    //      Filtering option approved peers list.
    //
    //==========================================================================
    virtual mxt_result RemoveAllApprovedPeersA() = 0;

    //==========================================================================
    //==
    //==  GetApprovedPeerList
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Gets the current approved peer list in use by the Peer Filtering
    //      option.
    //
    //  Parameters:
    //    rvecApprovedPeers:
    //      The approved peer list.
    //
    //  Description:
    //      Gets the current approved peer list in use by the Peer Filtering
    //      option.
    //
    //==========================================================================
    virtual void GetApprovedPeerList(OUT CVector<CSocketAddr>& rvecApprovedPeers) = 0;

    //==========================================================================
    //==
    //==  AddAllowedMethodA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Adds a method in the Peer Filtering option allowed methods list.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method to add to the methods list.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to add a method at any time in the Peer Filtering
    //      option allowed methods list, if not already present. This method
    //      will be checked against any incoming packet (request or response)
    //      and only those that match this method will go through to the
    //      application.
    //
    //  See Also:
    //      MxConvertSipMethod
    //
    //==========================================================================
    virtual mxt_result AddAllowedMethodA(IN const char* pszMethod) = 0;

    //==========================================================================
    //==
    //==  RemoveAllowedMethodA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Removes a method from the Peer Filtering option allowed methods
    //      list.
    //
    //  Parameters:
    //    eMethod:
    //      The SIP method to remove from the methods list.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to remove a method at any time from the Peer
    //      Filtering option allowed methods list, if not already present.
    //
    //  See Also:
    //      MxConvertSipMethod
    //
    //==========================================================================
    virtual mxt_result RemoveAllowedMethodA(IN const char* pszMethod) = 0;

    //==========================================================================
    //==
    //==  RemoveAllAllowedMethodsA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Removes all entries in the Peer Filtering option allowed methods
    //      list.
    //
    //  Returns:
    //      resS_OK when the action will be taken, an error otherwise.
    //
    //  Description:
    //      It allows the user to easily remove all entries in the Peer
    //      Filtering option allowed methods list.
    //
    //==========================================================================
    virtual mxt_result RemoveAllAllowedMethodsA() = 0;

    //==========================================================================
    //==
    //==  GetAllowedMethodList
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Gets the current allowed method list in use by the Peer Filtering
    //      option.
    //
    //  Parameters:
    //    rvecstrAllowedMethods:
    //      The allowed method list.
    //
    //  Description:
    //      Gets the current allowed method list in use by the Peer Filtering
    //      option.
    //
    //==========================================================================
    virtual void GetAllowedMethodList(OUT CVector<CString>& rvecstrAllowedMethods) = 0;
#endif

    //-- << Transaction Configuration >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetT1
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for T1.
    //
    //  Parameters:
    //    uT1Ms:
    //      The value to use for T1 in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for T1. By default, the SIP stack uses a value
    //      of 500 ms, as specified in RFC 3261.
    //
    //      One should modify this value with care as it affects the
    //      retransmission interval of requests and responses on unreliable
    //      transports.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //      Note that calling this method does not change the timeout value. To
    //      change it, SetTimeoutTimer must be called.
    //
    //==EDOC====================================================================
    virtual mxt_result SetT1(IN unsigned int uT1Ms) = 0;

    //==SDOC====================================================================
    //==
    //==  SetT2
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for T2.
    //
    //  Parameters:
    //    uT2Ms:
    //      The value to use for T2 in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for T2. By default, the SIP stack uses a value
    //      of 4000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      retransmission interval of non-INVITE requests and INVITE responses
    //      on unreliable transports.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //==EDOC====================================================================
    virtual mxt_result SetT2(IN unsigned int uT2Ms) = 0;

    //==SDOC====================================================================
    //==
    //==  SetT4
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for T4.
    //
    //  Parameters:
    //    uT4Ms:
    //      The value to use for T4 in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for T4. By default, the SIP stack uses a value
    //      of 5000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      maximum duration a message remains in the network
    //      on unreliable transports.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //==EDOC====================================================================
    virtual mxt_result SetT4(IN unsigned int uT4Ms) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimeoutTimer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the timeout value used by some SIP timers and timeouts.
    //
    //  Parameters:
    //    uTimeoutTimerMs:
    //      The value to use for the timeout timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the timeout value used for the SIP timers and timeouts
    //      described below. Its default value is 32 seconds as per RFC 3261.
    //
    //      - Timer B: INVITE transaction timeout timer (RFC 3261, section
    //                 17.1.1.2).
    //
    //      - Timer F: non-INVITE transaction timeout timer (RFC 3261, section
    //                 17.1.2.2).
    //
    //      - Timer H: Wait time for ACK receipt (RFC 3261, section 17.2.1).
    //
    //      - Timer J: Wait time for non-INVITE request retransmits (RFC 3261,
    //                 section 17.2.2).
    //
    //      - UAS INVITE 2xx retransmission timeout until the ACK is received.
    //        (RFC 3261, section 13.3.1.4).
    //
    //      - UAC INVITE sent ACK buffered timeout. When UAS retransmits 2xx,
    //        the buffered ACK is sent again (13.2.2.4).
    //
    //      - UAC final response waiting timeout after the first 2xx is received
    //        on early dialog.
    //
    //      - PROXY timeout period that final responses to an INVITE
    //        request are forwarded. (RFC 3261, section 16.7, bullet 5).
    //
    //      - PROXY retransmission timeout period to received 2xx, ACK and
    //        1xx reliable (ISipSessionStatefulProxySvc).
    //
    //      - Subscribe final NOTIFY waiting timeout.
    //
    //      When timers B, F, H or J are set with a value > 0 using its specific
    //      setter (with ISipCoreConfig::SetTimerB(value) for timer B), this new
    //      value will be used instead of the SetTimeoutTimer value. The other
    //      timeouts described above (other timers than B, F, H and J), always
    //      use the SetTimeoutTimer value.
    //
    //      One should modify this value with care as it affects the time
    //      needed before a transaction waiting for a response times out.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //==EDOC====================================================================
    virtual mxt_result SetTimeoutTimer(IN unsigned int uTimeoutTimerMs) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimerB
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for the Timer B
    //      (as specified in RFC 3261).
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The value to use for the timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for the timer. By default, the SIP
    //      stack uses a value of 32000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      INVITE transaction timeout.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //  See Also:
    //      SetT1
    //      SetT2
    //      SetT4
    //      SetTimeoutTimer
    //      SetTimerD
    //      SetTimerF
    //      SetTimerH
    //      SetTimerJ
    //==EDOC====================================================================
    virtual mxt_result SetTimerB(IN unsigned int uTimerMs) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimerD
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for the Timer D
    //      (as specified in RFC 3261).
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The value to use for the timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for the timer. By default, the SIP
    //      stack uses a value of 32000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      wait time for response retransmits.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //  See Also:
    //      SetT1
    //      SetT2
    //      SetT4
    //      SetTimeoutTimer
    //      SetTimerB
    //      SetTimerF
    //      SetTimerH
    //      SetTimerJ
    //==EDOC====================================================================
    virtual mxt_result SetTimerD(IN unsigned int uTimerMs) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimerF
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for the Timer F
    //      (as specified in RFC 3261).
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The value to use for the timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for the timer. By default, the SIP
    //      stack uses a value of 32000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      non-INVITE transaction timeout timer.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //  See Also:
    //      SetT1
    //      SetT2
    //      SetT4
    //      SetTimeoutTimer
    //      SetTimerB
    //      SetTimerD
    //      SetTimerH
    //      SetTimerJ
    //==EDOC====================================================================
    virtual mxt_result SetTimerF(IN unsigned int uTimerMs) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimerH
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for the Timer H
    //      (as specified in RFC 3261).
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The value to use for the timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for the timer. By default, the SIP
    //      stack uses a value of 32000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      wait time for ACK receipts.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //  See Also:
    //      SetT1
    //      SetT2
    //      SetT4
    //      SetTimeoutTimer
    //      SetTimerB
    //      SetTimerD
    //      SetTimerF
    //      SetTimerJ
    //==EDOC====================================================================
    virtual mxt_result SetTimerH(IN unsigned int uTimerMs) = 0;

    //==SDOC====================================================================
    //==
    //==  SetTimerJ
    //==
    //==========================================================================
    //
    //  Summary:
    //      Overrides the value to use for the Timer J
    //      (as specified in RFC 3261).
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The value to use for the timer in ms.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the value to use for the timer. By default, the SIP
    //      stack uses a value of 32000 ms (as specified in RFC 3261).
    //
    //      One should modify this value with care as it affects the
    //      wait time for non-INVITE request retransmits
    //      on unreliable transports.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack. Note that the new value does not
    //      affect ongoing transactions.
    //
    //  See Also:
    //      SetT1
    //      SetT2
    //      SetT4
    //      SetTimeoutTimer
    //      SetTimerB
    //      SetTimerD
    //      SetTimerF
    //      SetTimerH
    //==EDOC====================================================================
    virtual mxt_result SetTimerJ(IN unsigned int uTimerMs) = 0;

// BRCM_CUSTOM - [begin] Control branch-id matching choices in ACK
#ifndef MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
    //==SDOC====================================================================
    //==
    //==  SetBypassAckBranchIdCheck
    //==
    //==========================================================================
    //
    //  Summary:
    //      Set the ACK branch-id checking option.
    //
    //  Parameters:
    //    bBypass:
    //      True if bypassing ACK branch-id is enabled.
    //
    //  Description:
    //      Implemented to resolve inter-op issues.  Some proxy/UA send out
    //      INVITE and their corresponding ACK with the same branch-id in their
    //      Via header, which is not compliant to RFC3651.
    //
    //==EDOC====================================================================
    virtual mxt_result SetBypassAckBranchIdCheck(IN bool bBypass) = 0;
#endif
// BRCM_CUSTOM - [end] Control branch-id matching choices in ACK

    //-- << Core Configuration >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  SetCoreUser
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the core user interface, which receives new packets and
    //      chooses how they are handled.
    //
    //  Parameters:
    //    pUser:
    //      Pointer to the core user interface. It can be NULL, in which case no
    //      packets are reported. It must not be NULL in normal SIP stack use.
    //
    //  Returns:
    //      - resS_OK: the value has been correctly updated.
    //
    //      - resFE_FAIL: the value could not be updated.
    //
    //  Description:
    //      Sets the core user interface. The application using the SIP stack
    //      usually implements this interface.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //  See Also:
    //      ISipCoreUser
    //
    //==EDOC====================================================================
    virtual mxt_result SetCoreUser(IN ISipCoreUser* pUser) = 0;

    //-- << ENUM Configuration >>
    //-----------------------------

#if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
    //==SDOC====================================================================
    //==
    //==  SetEnumE164ZoneSuffix
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the E164 zone suffix for ENUM requests.
    //
    //  Parameters:
    //    rstrE164ZoneSuffix:
    //      The zone suffix to append to the FQDN form of the AUS. If the
    //      string is empty, this parameter defaults to the zone defined by
    //      MXD_SIPSTACK_ENUM_DEFAULT_ZONE.
    //
    //  Description:
    //      Configures the ENUM zone suffix.
    //
    //      This method can be called at any time after the threading
    //      configuration of the SIP stack.
    //
    //  See Also:
    //      SipStackCfg.h
    //
    //==EDOC====================================================================
    virtual void SetEnumE164ZoneSuffix(IN const CString& rstrE164ZoneSuffix) = 0;
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)

    //-- << Access to Specific SIP Stack Modules >>
    //------------------------------------------------

    //==SDOC====================================================================
    //==
    //==  AddTransportObserverA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a transport observer for handling specific SIP transport
    //      events.
    //
    //  Parameters:
    //    pObserver:
    //      Pointer to the observer to add.
    //
    //    ePriority:
    //      Observer priority. eLOW_PRIORITY_OBSERVER is default.
    //
    //  Returns:
    //      resS_OK: The transport observer is correctly added.
    //
    //      resFE_INVALID_STATE: The SIP stack in not proprely started.
    //
    //  Description:
    //      This method is used to append a new transport observer of
    //      activities on sockets.
    //
    //      eHIGH_PRIORITY_OBSERVER observers handle transport events before
    //      eLOW_PRIORITY_OBSERVER observers.
    //
    //      When two transport observers are added with the same priority, the
    //      last added observer handles transport events first.
    //
    //  See Also:
    //      ISipTransportObserver, RemoveTransportObserverA
    //
    //==EDOC====================================================================
    virtual mxt_result AddTransportObserverA(
                    IN ISipTransportObserver*                         pObserver,
                    IN ISipTransportObserver::EInsertObserverPriority ePriority =
                            ISipTransportObserver::eLOW_PRIORITY_OBSERVER) = 0;

    //==SDOC====================================================================
    //==
    //==  RemoveTransportObserverA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a transport observer from the observer list
    //
    //  Parameters:
    //    pObserver:
    //      Pointer to the transport observer to remove from the list.
    //
    //  Returns:
    //      resS_OK: The transport observer is correctly removed.
    //
    //      resFE_INVALID_STATE: The SIP stack in not proprely started.
    //
    //  Description:
    //      The method is called to remove a transport observer of activities
    //      on sockets from the observer list.
    //
    //  See Also:
    //      ISipTransportObserver, AddTransportObserverA
    //
    //==EDOC====================================================================
    virtual mxt_result RemoveTransportObserverA(IN ISipTransportObserver* pObserver) = 0;

#ifdef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT

    //==SDOC====================================================================
    //==
    //==  GetPersistentConnectionList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows access to the persistent connection list module.
    //
    //  Returns:
    //      Pointer to the persistent connection list. NULL if the SIP stack is
    //      not initialized yet.
    //
    //  Description:
    //      Provides access to the persistent connection list module.
    //
    //      It is through this class that the application can set the manager
    //      interface to which the persistent connection list reports its
    //      events.
    //
    //      It is also through this class that the application creates and
    //      terminates persistent connections.
    //
    //==EDOC====================================================================
    virtual CSipPersistentConnectionList* GetPersistentConnectionList() = 0;

#endif // #ifdef MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT

    //==SDOC====================================================================
    //==
    //==  SetDefaultDialogMatcherList
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default dialog matcher list.
    //
    //  Parameters:
    //    rDialogMatcherList
    //      A reference to the dialog matcher list.
    //
    //  Description:
    //      Sets the default dialog matcher list that is used when creating a
    //      ISipUserAgentSvc or a ISipSessionStatefulProxySvc.
    //
    //      The CSipDialogMatcherList is used to keep a list of active dialogs
    //      for the above services.
    //
    //      NOTE: The application is responsible to create and release the
    //      CSipDialogMatcherList. The provided reference to the
    //      CSipDialogMatcherList must stay valid until EvShutdownCompleted is
    //      fired through the ISipCoreUser interface and after ShutdownA has
    //      been called by the application.
    //
    //  See Also:
    //      CSipDialogMatcherList, ISipUserAgentSvc,
    //      ISipSessionStatefulProxySvc
    //
    //==EDOC====================================================================
    virtual void SetDefaultDialogMatcherList(IN CSipDialogMatcherList& rDialogMatcherList) = 0;

#ifdef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT

    //==SDOC====================================================================
    //==
    //==  SetConnectionBlacklistInstance
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Allows setting a new persistent connection list module to use,
    //      otherwise the default one is used.
    //
    //  Parameters:
    //    pBlacklist:
    //      Pointer to the persistent connection list. It can be NULL to release
    //      a custom blacklist instance previously set, only if Startup or
    //      Shutdown has never been called.
    //      Ownership is taken.
    //
    //  Description:
    //      Enables the user to set a new connection blacklist instance.
    //
    //      This method is totally optional and does not affect the usage of
    //      the stack. If no blacklist instance has been set prior to the stack
    //      Startup, the stack creates its own default one and manages it.
    //
    //==EDOC====================================================================
    virtual void SetConnectionBlacklistInstance(TO CSipConnectionBlacklist* pBlacklist) = 0;

    //==SDOC====================================================================
    //==
    //==  GetConnectionBlacklistInstance
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Allows access to the persistent connection list module.
    //
    //  Returns:
    //      Pointer to the persistent connection list. NULL if the SIP stack is
    //      not initialized yet.
    //
    //  Description:
    //      Provides access to the persistent connection list module.
    //
    //      It is through this class that the application can set the manager
    //      interface to which the persistent connection list reports its
    //      events.
    //
    //      It is also through this class that the application creates and
    //      terminates persistent connections.
    //
    //      If a new connection blacklist has been set using
    //      SetConnectionBlacklistInstance, it is returned here,
    //      otherwise the default connection blacklist is returned.
    //
    //==EDOC====================================================================
    virtual CSipConnectionBlacklist* GetConnectionBlacklistInstance() = 0;

#endif // #ifdef MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT

#ifdef MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT
    //==========================================================================
    //==
    //==  GetUaHelpers
    //==
    //==========================================================================
    // <FLAG New in 4.1.7>
    //
    //  Summary:
    //      Allows access to the user agent helpers methods.
    //
    //  Returns:
    //      Pointer to the user agent helpers object instance.
    //
    //  Description:
    //      Provides access to the user agent helpers methods.
    //
    //==========================================================================
    virtual CSipUaHelpers* GetUaHelpers() const = 0;
#endif // #ifdef MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT

    //M5T_INTERNAL_USE_BEGIN
#ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
    //==========================================================================
    //==
    //==  GetSipStackMonitor
    //==
    //==========================================================================
    // <FLAG New in 4.1.7>
    //
    //  Summary:
    //      Allows access to the monitor of the stack.
    //
    //  Returns:
    //      Pointer to the monitor of the stack, NULL if the SIP stack
    //      is not initialized yet.
    //
    //  Description:
    //      Provides access to the monitor of the stack.
    //
    //==========================================================================
    virtual CSipStackMonitor* GetSipStackMonitor() const = 0;
#endif // #ifdef MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
    //M5T_INTERNAL_USE_END

#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)

    //==========================================================================
    //==
    //==  GetTargetManagementModuleInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows access to the target management module.
    //
    //  Returns:
    //      Pointer to the target management module. NULL if the stack is not
    //      initialized yet.
    //
    //  Description:
    //      Provides access to the target management module.
    //
    //
    //==========================================================================
    virtual CTargetManagementModule* GetTargetManagementModuleInstance() = 0;

#endif //#if defined(MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT)

    //==SDOC====================================================================
    //==
    //==  GetVersion
    //==
    //==========================================================================
    //
    //  Summary:
    //      Obtains a NULL-terminated string representing the version of the SIP
    //      stack.
    //
    //  Returns:
    //      A NULL-terminated string representing the version of the SIP stack.
    //
    //  Description:
    //      Gets the version of the SIP stack. The version is a NULL-terminated
    //      string that has the following form "major.minor.release.build".
    //      CVersion can be used to interpret this string as the major, minor,
    //      release, and build numbers.
    //
    //      This can be called at any time.
    //
    //  See Also:
    //      CVersion
    //
    //==EDOC====================================================================
    virtual const char* GetVersion() = 0;

    //-- << Startup and Shutdown >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  Startup
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts the initialization procedures of the SIP stack.
    //
    //  Returns:
    //      resS_OK: If it succesfully started.
    //
    //      resFE_FAIL: If it is already started or if it failed to be started.
    //
    //  Description:
    //      Starts the SIP stack by initializing the necessary components.
    //
    //  See Also:
    //      ShutdownA
    //
    //==EDOC====================================================================
    virtual mxt_result Startup() = 0;


    //==SDOC====================================================================
    //==
    //==  ShutdownA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Shuts down the SIP stack asynchronously.
    //
    //  Description:
    //      Shuts down the SIP stack. When shutdown process is completed,
    //      EvShutdownCompleted is called on the ISipCoreUser interface.
    //
    //  See Also:
    //      Startup
    //
    //==EDOC====================================================================
    virtual void ShutdownA() = 0;

    //-- << Network accessibility >>
    //-------------------------------

    //==SDOC====================================================================
    //==
    //==  AddLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //     Adds a local address.
    //
    //  Parameters:
    //    rAddr:
    //      The IP address to configure.
    //
    //    pvecStrFqdn:
    //      The FQDNs associated with this local address. It can be NULL, in
    //      which case the SIP stack assumes there are no FQDNs associated with
    //      this address. Ownership is taken.
    //
    //    pvecDestinations:
    //      The accessible networks from this local IP address. It can be NULL,
    //      in which case the SIP stack assumes all networks are reachable from
    //      this address. Ownership is taken.
    //
    //    ropqAddress:
    //      SIP Stack-specific opaque address identifier. This can be used by
    //      the application to later perform additional operations from this
    //      local address.
    //
    //  Returns:
    //    resFE_INVALID_STATE:
    //      The provided IP address is already present.
    //
    //    resFE_INVALID_ARGUMENT:
    //      The provided IP address is invalid.
    //
    //    resS_OK:
    //      The IP address has been added successfuly.
    //
    //  Description:
    //      This method configures a local address and its parameters. The SIP
    //      stack does not listen to this local address until the user calls
    //      ListenA.
    //
    //      An application that has a single address should not configure any
    //      accessible network. That is, pvDestinations should be NULL.
    //
    //      An application that has multiple addresses to different networks
    //      should add the addresses in order of preference, as the SIP stack
    //      searches a local address to use in order they were added in the
    //      SIP stack.
    //
    //      The last address added should be the default address to use and
    //      should have a NULL pvDestinations. This means that there is at least
    //      a default local address to use when sending a packet to an
    //      unconfigured accessible network.
    //
    //      When pvecStrFqdn contains more than one FQDN and the SIP stack is
    //      configured to use FQDNs in Via headers it created, the first FQDN of
    //      this list is used.
    //
    //  Example:
    //      An application has two local addresses: 10.2.5.100 to access a
    //      private network and 205.237.248.241 to access the public Internet.
    //
    //      From its 10.2.5.100 address, it has access to 8 networks, from
    //      10.0.x.x to 10.7.x.x.
    //
    //      From its 205.237.248.241 address, it has access to the public
    //      network.
    //
    //      The application would then configure its 10.2.5.100 as its first
    //      address with a single SAccessibleNetwork that has the following
    //      parameters:
    //
    //              - m_address = 10.0.0.0
    //              - m_netmask = 255.248.0.0
    //
    //      The application would afterwards configure its 205.237.248.241
    //      address as its second address, but this time, it would specify no
    //      accessible networks, meaning that this is the default address to use
    //      if no previous address can be used.
    //
    //  See Also:
    //      RemoveLocalAddress, SAccessibleNetwork, ListenA, StopListeningA,
    //      SetViaAddressType
    //
    //==EDOC====================================================================
    virtual mxt_result AddLocalAddress(
                    IN    const CSocketAddr&                 rAddr,
                    IN TO const CVector<CString>*            pvecStrFqdn,
                    IN TO const CVector<SAccessibleNetwork>* pvecDestinations,
                    OUT   mxt_opaque&                        ropqAddress) = 0;

    //==SDOC====================================================================
    //==
    //==  RemoveLocalAddress
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a local address.
    //
    //  Parameters:
    //    opqAddress:
    //      SIP Stack-specific opaque address identifier received from
    //      AddLocalAddress.
    //
    //  Returns:
    //    resFE_FAIL:
    //      The provided IP address was not present.
    //
    //    resS_OK:
    //      The IP address has been removed successfuly.
    //
    //  Description:
    //      Removes a previously added local address. Calling this also
    //      automatically stops all listening on this local address.
    //
    //      It is important to never call this method after Shutdown is called,
    //      as Shutdown automatically removes all local addresses and render
    //      all associated opqAddress invalid.
    //
    //  See Also:
    //      AddLocalAddress, StopListeningA
    //
    //==EDOC====================================================================
    virtual mxt_result RemoveLocalAddress(IN mxt_opaque opqAddress) = 0;

    //==SDOC====================================================================
    //==
    //==  ListenA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Listens for incoming SIP packets on an IP address.
    //
    //  Parameters:
    //    opqAddress:
    //      The address on which to listen, received from AddLocalAddress.
    //
    //    uPort:
    //      The port on which to listen.
    //
    //    eTransport:
    //      The transport to which listen.
    //
    //    pCoreUser:
    //      Pointer to the interface that is notified once the operation has
    //      completed. It can be NULL, in which case the SIP stack does not
    //      report any status about the completion of the operation.
    //
    //    opqCallBackParam:
    //      Parameters to report when reporting the completion of the operation.
    //      This parameter is not used by the SIP stack but simply reported to
    //      the pCoreUser upon completion.
    //
    //    ropqListen:
    //      SIP Stack-specific opaque listen identifier received from ListenA.
    //
    //  Returns:
    //    resS_OK:
    //      Starts listening, the result is given through EvCommandResult.
    //
    //    resFE_INVALID_ARGUMENT:
    //      The provided opqAddress or uPort is invalid.
    //
    //    resFE_INVALID_STATE:
    //      The SIP stack is not stated.
    //
    //  Description:
    //      This method allows the SIP stack user to have the stack listen on a
    //      specific address and port for incoming SIP packets over the
    //      specified transport.
    //
    //      Since this is an asynchronous operation performed by the SIP stack,
    //      it reports its success or failure through the
    //      ISipCoreUser::EvCommandResult method implemented by the SIP stack
    //      user. If a failure result code is returned from the method call, no
    //      EvCommandResult will be reported.
    //
    //  See Also:
    //      StopListeningA, AddLocalAddress, RemoveLocalAddress
    //
    //==EDOC====================================================================
    virtual mxt_result ListenA(IN  mxt_opaque    opqAddress,
                               IN  uint16_t      uPort,
                               IN  ESipTransport eTransport,
                               IN  ISipCoreUser* pCoreUser,
                               IN  mxt_opaque    opqCallBackParam,
                               OUT mxt_opaque&   ropqListen) = 0;

    //==SDOC====================================================================
    //==
    //==  StopListeningA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stops listening for incoming SIP packets on an IP address.
    //
    //  Parameters:
    //    opqListen:
    //      The listen identifier on which to stop listening, received from
    //      ListenA.
    //
    //    pCoreUser:
    //      Pointer to the interface that is notified once the operation has
    //      completed. It can be NULL, in which case the SIP stack does not
    //      report any status about the completion of the operation.
    //
    //    opqCallBackParam:
    //      Parameters to report when reporting the completion of the operation.
    //      This parameter is not used by the SIP stack but simply reported to
    //      the pCoreUser upon completion.
    //
    //  Returns:
    //    resS_OK:
    //      Stops listening, the result is given through EvCommandResult.
    //
    //    resFE_INVALID_STATE:
    //      The SIP stack is not stated or this listen identifier is already
    //      proceeding StopListeningA.
    //
    //    resFE_INVALID_ARGUMENT:
    //      The provided opqListen parameter is invalid.
    //
    //  Description:
    //      This method allows the SIP stack user to stop a previous call to
    //      ListenA.
    //
    //      Since this is an asynchronous operation performed by the SIP stack,
    //      it reports its success or failure through the
    //      ISipCoreUser::EvCommandResult method implemented by the SIP stack
    //      user. If a failure result code is returned from the method call, no
    //      EvCommandResult will be reported.
    //
    //  See Also:
    //      ListenA, AddLocalAddress, RemoveLocalAddress
    //
    //==EDOC====================================================================
    virtual mxt_result StopListeningA(IN mxt_opaque    opqListen,
                                      IN ISipCoreUser* pCoreUser,
                                      IN mxt_opaque    opqCallBackParam) = 0;

    //==SDOC========================================================================
    //==
    //==  GetNetworkInterfaceList
    //==
    //==============================================================================
    //
    //  Summary:
    //      Gets the network interfaces address list.
    //
    //  Parameters:
    //    rvecstNetworkIf:
    //      Local network interfaces addresses list.
    //
    //  Returns:
    //      - resS_OK on success.
    //      - resFE_INVALID_STATE: The SIP stack in not proprely started.
    //
    //  Description:
    //      Returns a list of local network interface addresses. This list contains
    //      the local addresses added using the AddLocalAddress() method. For each
    //      added local address, it also returns the listening ports added with
    //      the ListenA method. If many UDP, TCP, or TLS ports are listening at the
    //      same address, it returns only the first registered UDP or TCP port and
    //      the first TLS port.
    //
    //  See Also:
    //      AddLocalAddress, ListenA
    //
    //==EDOC========================================================================
    virtual mxt_result GetNetworkInterfaceList(
                                OUT CVector<SNetworkIf>& rvecstNetworkIf) = 0;

//M5T_INTERNAL_USE_BEGIN
    //==========================================================================
    //==
    //==  GetNetworkInterfaceListInstance
    //==
    //==========================================================================
    //
    //  Summary:
    //      Allows access to the network interface list instance.
    //
    //  Returns:
    //      Pointer to the network interface list. NULL if the SIP stack is
    //      not initialized yet.
    //
    //  Description:
    //      Provides access to the network interface list instance.
    //
    //      This method should only be use within the core thread without any
    //      thread-safety mechanism.
    //
    //==========================================================================
    virtual const CSipNetworkInterfaceList* GetNetworkInterfaceListInstance() = 0;
//M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  SetViaAddressType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures which type of addresses the SIP stack uses in Via
    //      headers.
    //
    //  Parameters:
    //    ePreference:
    //      The configuration for the Via address.
    //
    //  Description:
    //      Configures how the SIP stack builds the Via addresses from the
    //      information configured by AddLocalAddress. ePREFER_FQDN is the
    //      default value.
    //
    //  See Also:
    //      AddLocalAddress
    //
    //==EDOC====================================================================
    virtual void SetViaAddressType(IN EAddressTypePreference ePreference) = 0;

    //==============================================================================
    //==
    //==  SetEntityId
    //==
    //==============================================================================
    //
    //  Parameters:
    //      opqListen:
    //       SIP stack-specific opaque listen identifier received from ListenA.
    //
    //      uEntityId:
    //       The entity ID.
    //
    //  Returns:
    //      - resS_OK: success.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      Associate an entity ID with the specified listening opaque. Only
    //      SIP Context with the same entity ID can send SIP packets
    //      through this interface. This is useful to limit a SIP context to
    //      to a specific interface. If this method is not called for a
    //      listening opaque, 0 is the assumed entity ID.
    //
    //  See Also:
    //      ISipContext::SetEntityId
    //
    //==============================================================================
    virtual mxt_result SetEntityId(IN mxt_opaque opqListen,
                                   IN unsigned int uEntityId) = 0;

    //==============================================================================
    //==
    //==  SetSocketClosureType
    //==
    //==============================================================================
    // <FLAG New in 4.1.5>
    //
    //  Parameters:
    //      eCloseBehavior:
    //       The closing behaviour to use.
    //
    //  Returns:
    //    resS_OK:
    //      Successfully set the closing behaviour.
    //
    //    resFE_INVALID_STATE:
    //      The stack has not been configured properly yet.
    //
    //  Description:
    //      This sets the closing behaviour to use to close connected client sockets
    //      while removing a network interface. This closing behaviour is also used
    //      if ShutdownA is called and some network interfaces are still present.
    //      By default, graceful closes are done.
    //
    //==============================================================================
    virtual mxt_result SetSocketClosureType(IN ISocket::ECloseBehavior eCloseBehavior) = 0;

    //==========================================================================
    //==
    //==  CloseAllConnections
    //==
    //==========================================================================
    //
    //  Summary:
    //      Closes all active connections or only those that match the provided
    //      remote address.
    //
    //  Parameters:
    //    pRemoteAddress:
    //      Specify the remote address of the connections to close. Provide NULL
    //      to close all active connections.
    //
    //  Returns:
    //      - resS_OK: Connections have been successfully closed.
    //      - resFE_FAIL: The operation has not been completed.
    //
    //  Description:
    //      Closes all active connections or only the connections that match the
    //      pRemoteAddr parameter when it is non-NULL.
    //
    //      This method only closes sockets that are connected. This is true
    //      for all transports. This means that listening sockets and sockets
    //      used by the symmetric UDP service are left untouched. It is the
    //      application's responsibility to close those sockets by the
    //      appropriate mechanism.
    //
    //      The sockets are closed by using the closure type set with the
    //      ISipCoreConfig::SetSocketClosureType method.
    //
    //  See also:
    //      SetSocketClosureType
    //
    //==========================================================================
    virtual mxt_result CloseAllConnections(IN CSocketAddr* pRemoteAddress = NULL) = 0;


    //-- << Core Service Configuration >>
    //------------------------------------

#if defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
    //==========================================================================
    //==
    //==  AddSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a globally supported extension.
    //
    //  Parameters:
    //    rstrExtension:
    //      The custom extension to add.
    //
    //  Returns:
    //     resS_OK: The extension was added.
    //     resFE_FAIL: The extension was already present.
    //
    //  Description:
    //      Adds a globally supported extension to the list.
    //
    //==========================================================================
    virtual mxt_result AddSupportedExtension(IN const CString& rstrExtension) = 0;

    //==========================================================================
    //==
    //==  RemoveSupportedExtension
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a globally supported extension.
    //
    //  Parameters:
    //    rstrExtension:
    //      The custom extension to remove.
    //
    //  Returns:
    //     resS_OK: The extension was removed.
    //     resFE_FAIL: The extension was not found.
    //
    //  Description:
    //      Removes a globally supported extension from the list.
    //
    //==========================================================================
    virtual mxt_result RemoveSupportedExtension(IN const CString& rstrExtension) = 0;

    //==========================================================================
    //==
    //==  GetSupportedExtensions
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the globally supported extension list.
    //
    //  Parameters:
    //    rplststrExtension:
    //      The globally supported extension list.
    //
    //  Description:
    //      Gets the globally supported extension list.
    //
    //      The returned list can be NULL, in which case there are no globally
    //      supported extension configured.
    //
    //==========================================================================
    virtual void GetSupportedExtensions(OUT const CList<CString>*& rplststrExtension) const = 0;
#endif

    //-- << User Agent Services Configuration >>

#if defined MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
    //==========================================================================
    //==
    //==  SetDefaultRegistrationContactMatchingType
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the default contacting matching type used in the
    //      ISipRegistrationSvc.
    //
    //  Parameters:
    //    uContactMatchingType:
    //      The type of matching used.
    //      See ISipRegistrationSvc::EContactMatchingType for
    //      more documentation.
    //
    //  Returns:
    //      - resS_OK: Matching value was set.
    //      - other: value was not set.
    //
    //  Description:
    //      Sets the default contacting matching type used in the
    //      ISipRegistrationSvc. This will be used for all contexts that have
    //      the ISipRegistrationSvc set.
    //
    //  See Also:
    //      ISipRegistrationSvc::EContactMatchingType
    //
    //==========================================================================
    virtual mxt_result SetDefaultRegistrationContactMatchingType(IN unsigned int uContactMatchingType) = 0;
#endif //-- #if defined MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT

#if defined MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
    //==========================================================================
    //==
    //==  SetDefaultCallIdLength
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the the default Call-ID length generated by the
    //      ISipUserAgentSvc.
    //
    //  Parameters:
    //    uCallIdlength:
    //      The default length of the Call-ID. The maximum length is currently
    //      limited to 32 characters, even if a higher value is set. Setting 0
    //      will reset the Call-ID length to its default value.
    //
    //  Returns:
    //      - resS_OK: The default length was set.
    //      - other: The default length was not set.
    //
    //  Description:
    //      Sets the the default Call-ID length generated by the
    //      ISipUserAgentSvc. This will be used for all contexts that have the
    //      ISipUserAgentSvc set.
    //
    //      If an application wishes to have a Call-ID longer than 32
    //      characters, it must set an application-generated Call-ID using
    //      ISipUserAgentSvc::SetCallId on the SIP context.
    //
    //      The default value for the Call-ID length is 16 characters.
    //
    //==========================================================================
    virtual mxt_result SetDefaultCallIdLength(IN unsigned int uCallIdlength) = 0;

    //==========================================================================
    //==
    //==  GetDefaultCallIdLength
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the the default Call-ID length generated by the
    //      ISipUserAgentSvc.
    //
    //  Returns:
    //      The default length of the Call-ID or 0 if an error occurred.
    //
    //  Description:
    //      Gets the the default Call-ID length generated by the
    //      ISipUserAgentSvc.
    //
    //==========================================================================
    virtual unsigned int GetDefaultCallIdLength() = 0;
#endif //-- #if defined MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT

    //-- << ISipDigestClientAuthSvc Configuration >>
    //-----------------------------------------------

#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
    //==========================================================================
    //==
    //==  SetClientAuthUnknownQopBehavior
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the behavior of the ISipDigestClientAuthSvc upon reception of
    //      an unknown qop parameter value in the WWW-Authenticate or
    //      Proxy-Authenticate header.
    //
    //  Parameters:
    //    eBehavior:
    //      The behavior of ISipDigestClientAuthSvc to set.
    //
    //  Description:
    //      Sets the behavior of the ISipDigestClientAuthSvc upon reception of
    //      an unknown qop parameter value in the WWW-Authenticate or
    //      Proxy-Authenticate header. A qop value is considered unknown when
    //      it is unsupported by the ISipDigestClientAuthSvc implementation.
    //
    //  See Also:
    //      ISipDigestClientAuthSvc::EUnknownQopBehavior
    //
    //==========================================================================
    virtual void SetClientAuthUnknownQopBehavior(IN ISipDigestClientAuthSvc::EUnknownQopBehavior eBehavior) = 0;
#endif

#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
    //==========================================================================
    //==
    //==  AddSwitchbackPersistentConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a switchback persistent connection object.
    //
    //  Parameters:
    //      ppSwitckBackManagement:
    //        The pointer that will hold the newly created switchback object.
    //        The application is responsible for releasing this object.
    //
    //  Returns:
    //    - resS_OK: Switchback management correctly created.
    //    - Other: Failure, object is not created.
    //
    //  Description:
    //      Adds a switchback persistent connection object to the
    //      ISipCoreConfig. The SIP stack will terminate all connections
    //      when ShutdownA is called.
    //
    //  See also:
    //    RemoveSwitchbackPersistentConnection, ShutdownA,
    //    ISipSwitchbackPersistentConnection
    //
    //==========================================================================
    virtual mxt_result AddSwitchbackPersistentConnection(OUT ISipSwitchbackPersistentConnection*& rpSwitckBackManagement) = 0;

    //==========================================================================
    //==
    //==  RemoveSwitchbackPersistentConnection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a switchback persistent connection object.
    //
    //  Parameters:
    //      pSwitckBackManagement:
    //          The switchback connection to remove.
    //
    //  Returns:
    //    - resS_OK: connection removed from the ISipCoreConfig.
    //    - other: connection does not exists.
    //
    //  Description:
    //      Removes the switchback persistent connection object from the
    //      ISipCoreConfig when it is no longer needed. This object will not be
    //      released when ShutDownA is called.
    //
    //      Calling RemoveSwitchbackPersistentConnection is not mandatory as
    //      ShutdownA terminates the switchback persistent connection if
    //      RemoveSwitchbackPersistentConnection was not done prior to calling
    //      ShutdownA.
    //
    //  See also:
    //    AddSwitchbackPersistentConnection, ShutdownA,
    //    ISipSwitchbackPersistentConnection
    //
    //==========================================================================
    virtual mxt_result RemoveSwitchbackPersistentConnection(IN ISipSwitchbackPersistentConnection* pSwitckBackManagement) = 0;
#endif //-- #if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)

    //-- << Request Handling Configuration >>
    //----------------------------------------

    //==========================================================================
    //==
    //==  SetServiceUnavailableHandlingMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the 503 Service Unavailable response handling mode.
    //
    //  Parameters:
    //    eServiceUnavailableHandlingMode:
    //      The 503 Service Unavailable response handling mode.
    //
    //  Description:
    //      Sets the 503 Service Unavailable response handling mode. This mode
    //      will tell the SIP stack how to process a 503 Service Unavailable
    //      response.
    //
    //      This mode affects the SIP stack globally.
    //
    //==========================================================================
    virtual void SetServiceUnavailableHandlingMode(IN ISipCoreConfig::EServiceUnavailableHandling eServiceUnavailableHandlingMode) = 0;

    //==========================================================================
    //==
    //==  GetServiceUnavailableHandlingMode
    //==
    //==========================================================================
    //
    //  Summary:
    //      Gets the 503 Service Unavailable response handling mode.
    //
    //  Returns:
    //      The 503 Service Unavailable response handling mode.
    //
    //  Description:
    //      Gets the 503 Service Unavailable response handling mode.
    //
    //==========================================================================
    virtual ISipCoreConfig::EServiceUnavailableHandling GetServiceUnavailableHandlingMode() const = 0;

// BRCM_CUSTOM - [add] Configure P-Access-Network-Info header
    //==========================================================================
    //==
    //==  SetPaniHdr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets the P-Access-Network-Info header for all transmitted SIP
    //      messages
    //
    //  Parameters:
    //    rstrPaniHdr:
    //      The P-Access-Network-Info header to add.
    //
    //  Returns:
    //     resS_OK: The header was added.
    //     resFE_FAIL: The header was not added.
    //
    //  Description:
    //      Adds a P-Access-Network-Info header.
    //
    //==========================================================================
    virtual void SetPaniHdr(IN const CString& rstrPaniHdr) = 0;
// BRCM_CUSTOM - [end] Configure P-Access-Network-Info header

//-- Hidden Methods.
//-------------------
protected:

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipCoreConfig(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipCoreConfig(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipCoreConfig(IN const ISipCoreConfig& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipCoreConfig& operator=(IN const ISipCoreConfig& rSrc);
};


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCORECONFIG_H
