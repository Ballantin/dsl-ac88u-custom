//==============================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSIPPERSISTENTCONNECTIONLIST_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSIPPERSISTENTCONNECTIONLIST_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT) && \
    !defined(MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT or \
MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT to be able to use \
this class.
#endif

//-- Data Members
#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_ISIPPERSISTENTCONNECTIONMGR_H
#include "SipCoreSvc/ISipPersistentConnectionMgr.h"
#endif

//-- Parameters used by Value
#ifndef MXG_CSIPURI_H
#include "SipParser/CSipUri.h" // CSipUri::ESecurityFlag
#endif

//-- Interface Realized and/or Parent
#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
  #ifndef MXG_ISIPKEEPALIVEMGR_H
  #include "SipTransport/ISipKeepAliveMgr.h"
  #endif

  #ifndef MXG_ISIPKEEPALIVEEXTENSIONMGR_H
  #include "SipTransport/ISipKeepAliveExtensionMgr.h"
  #endif
#endif

#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#include "SipTransport/ISipTransportObserver.h"
#endif

#ifndef MXG_ISIPTRANSPORTUSER_H
#include "SipTransport/ISipTransportUser.h"
#endif

#ifndef MXG_CSERVERLOCATOR_H
#include "SipCoreSvc/CServerLocator.h" // ISipServerLocatorMgr
#endif

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class ISipPersistentConnectionMgr;
class ISipTransportMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
//==============================================================================
//==
//==  PFNSrvRetrievalCallback
//==
//==============================================================================
//
//  Parameters:
//    const CString*:
//      The DNS SRV target name. It can be NULL if no SRV query has been done.
//
//    const CSocketAddr*:
//      The peer address to which it will be connected.
//
//  Description:
//      DNS SRV target name retrieval callback.
//
//==============================================================================
typedef void (*PFNSrvRetrievalCallback)(const CString*, const CSocketAddr*);
#endif

//==============================================================================
//== Class: CSipPersistentConnectionList
//========================================
//
// Description:
//  The persistent connection list offers the interfaces for the application
//  to create and manage its own connections.
//
//  A connection must be configured with a manager, to which events about the
//  connection state are reported.
//
//  The persistent connection list periodically tries to re-establish
//  connections when it is not able to connect to the peer or when an existing
//  connection has been closed by the peer.
//
//  The persistent connection list is automatically created by the stack. It can
//  be accessed through ISipCoreConfig::GetPersistentConnectionList once the
//  stack is initialized.
//
//#More about connections#
//  The M5T SIP stack creates two types of connections: The automatic
//  connections and the persistent connections.
//
//  The automatic connections are created and deleted automatically by the stack
//  when required. That is, a new connection to a peer is established for
//  sending a packet if no connections to that peer already exist, and this
//  connection may be deleted when the M5T SIP stack needs resources, for
//  instance, to establish another connection to a different peer. The
//  configuration for how many automatic connections can be created, and when
//  and how they are closed, is done through
//  ISipCoreConfig::SetConnectionsParameters.
//
//  The M5T SIP stack offers the possibility to the application to create and
//  manage the lifetime of its own connections. This is done through the
//  persistent connections mechanism. Persistent connections are never
//  automatically closed by the M5T SIP stack, unless the application requests
//  it.
//
//  Another difference between automatic and persistent connections is the
//  local port used for the connection. With automatic connections, the
//  M5T SIP stack uses an ephemeral port, which is automatically assigned
//  by the system. When creating a persistent connection, the application
//  can specify the local address and port to use for creating the connection.
//  Note however that it's also possible to let the stack chose an ephemeral
//  port with persistent connections.
//
//  Until connection-reuse is properly defined by the IETF and supported by the
//  industry, the persistent connection mechanism is often useful when managing
//  TLS connections from a UA to a server. Because they generally do not have
//  certificates they can present to a server, endpoints must establish
//  a long-lived TLS connection to their server in order to be able to receive
//  requests on it. Moreover, in order to force the reuse of the
//  client-established connection by the server, the connection is usually
//  established from the port 5061 on the UA to the port 5061 on the server.
//
//M5T_INTERNAL_USE_BEGIN
//#More about switchbacked connections#
//  When creating a switchbacked connection using the EstablishSwitchback()
//  method, more than one connection to the same hostname may be established.
//  The actual addresses to which the different connection part of the
//  switchbacked connection are determined either by a list of user-supplied
//  addresses or the DNS records returned for the user-specified hostname.
//  Should a connection of a switchbacked connection go down, the next highest
//  priority connection will be automatically connected. The disconnected
//  connection will then follow the standard retry procedure. Once a connection
//  becomes connected again, it is not used until the application marks it as
//  verified by calling the SwitchbackConnectivityVerified() method.
//  When a switchbacked connection is no longer needed, all connections part of
//  the switchbacked connection must be terminated via a call to Terminate().
//  Once all connections have received their ExConnectionTerminated callback,
//  the switchbacked connection MUST be terminated by calling
//  TerminateSwitchback().
//M5T_INTERNAL_USE_END
//
// Location:
//   SipCoreSvc/CSipPersistentConnectionList.h
//
// See Also:
//   ISipPersistentConnectionMgr, ISipPersistentConnectionSvc
//
//==============================================================================
class CSipPersistentConnectionList : private CEventDriven,
#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
                                     public ISipKeepAliveMgr,
                                     public ISipKeepAliveExtensionMgr,
#endif
                                     private ISipTransportObserver,
                                     private IServerLocatorMgr,
                                     private ISipTransportUser
{
//-- Friend Declarations
    //M5T_INTERNAL_USE_BEGIN
    friend class CSipPersistentConnectionSvc;
    // Outbound service needs to access UpdatePacket which is private.
    friend class CSipOutboundConnectionSvc;
    friend class CSipCoreConfig;
    friend class CSipStackInitializer;
    friend class CCoreSvcPersistentConnectionSvcTest;
    friend class CSipNetworkInterfaceList;

    //M5T_INTERNAL_USE_END

//-- Published Interface
public:

    // Constant used to have the local address chosen
    // automatically in Establish.
    static const mxt_opaque opqLOCAL_ADDRESS_ANY;

    //-- << Stereotype >>

    // Enumeration describing how persistent connections are to be
    // automatically re-established.
    enum ERetryMode
    {
        // Uses the currently configured retry set by SetRetryConfig.
        eDEFAULT,
        // Periodically retry to establish the connection, each uTimeout1
        // seconds.
        eRETRY_PERIODIC,

        // Retry to establish the connection with exponential backoff,
        // starting at uTimeout1 up to uTimeout2, and then restarting
        // exponential backoff at uTimeout1.
        eRETRY_EXPONENTIAL,

        // Retry to establish the connection using the formula as specified in
        // the outbound draft: W = min( max-time, (base-time * (2 ^
        // consecutive-failures))). See SetRetryConfig for more details.
        eRETRY_OUTBOUND,

        // No retry is done for persistent connections. Should only be used for
        // switch back connections.
        eNONE
    };

    // Summary:
    //  Establishes a persistent connection.
    mxt_result Establish(IN  ESipTransport                eTransport,
                         IN  const mxt_opaque             opqLocalAddress,
                         IN  uint16_t                     uLocalPort,
                         IN  const CString&               rstrPeerHost,
                         IN  uint16_t                     uPeerPort,
                         IN  ISipPersistentConnectionMgr* pMgr,
                         OUT mxt_opaque&                  ropqConnection,
                         IN  ERetryMode                   eRetryMode = eDEFAULT,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection / Add per-user DNS lookup support
                         IN  unsigned int                 uDestinationId = 0,
                         IN  mxt_opaque                   opqUser = 0,
                         IN  mxt_opaque                   opqDnsUser = 0);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection / Add per-user DNS lookup support

    // Summary:
    //  Establishes a persistent connection.
    mxt_result Establish(IN  ESipTransport                eTransport,
                         IN  const mxt_opaque             opqLocalAddress,
                         IN  uint16_t                     uLocalPort,
                         IN  const CSocketAddr&           rPeerAddr,
                         IN  ISipPersistentConnectionMgr* pMgr,
                         OUT mxt_opaque&                  ropqConnection,
                         IN const CString*                pstrPeerHost = NULL,
                         IN  ERetryMode                   eRetryMode = eDEFAULT,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection / Add per-user DNS lookup support
                         IN  unsigned int                 uDestinationId = 0,
                         IN  mxt_opaque                   opqUser = 0,
                         IN  mxt_opaque                   opqDnsUser = 0);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection / Add per-user DNS lookup support

    // Summary:
    //  Attempts to Re-establish a persistent connection that is in error
    //  or has been disconnected.
    mxt_result ReEstablish(IN mxt_opaque opqConnection);

    // Summary:
    //  Disconnects a connected connection. This can happen when a SIP request
    //  has timed out for instance.
    mxt_result Disconnect(IN mxt_opaque opqConnection);

#if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK)
//M5T_INTERNAL_USE_BEGIN
    // Summary:
    //  Establishes a persistent connection with switchback support enabled.
    mxt_result EstablishSwitchback(IN ESipTransport eTransport,
                                   IN const mxt_opaque,
                                   IN uint16_t uLocalPort,
                                   IN const CString& rstrPeerHost,
                                   IN uint16_t uPeerPort,
                                   IN ISipPersistentConnectionMgr* pMgr,
                                   IN unsigned int uMaxNbConnections,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection / Add per-user DNS lookup support
                                   OUT CVector<mxt_opaque>& rvecopqConnection,
                                   IN mxt_opaque opqUser = 0,
                                   IN mxt_opaque opqDnsUser = 0);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection / Add per-user DNS lookup support

    // Summary:
    //  Establishes a persistent connection with switchback support enabled.
    mxt_result EstablishSwitchback(IN ESipTransport eTransport,
                                   IN const mxt_opaque opqLocalAddress,
                                   IN uint16_t uLocalPort,
                                   IN const CList<CSocketAddr>& rlstPeerAddr,
                                   IN ISipPersistentConnectionMgr* pMgr,
                                   IN unsigned int uMaxNbConnections,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection / Add per-user DNS lookup support
                                   OUT CVector<mxt_opaque>& rvecopqConnection,
                                   IN mxt_opaque opqUser = 0,
                                   IN mxt_opaque opqDnsUser = 0);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection / Add per-user DNS lookup support

    // Summary:
    //  Terminates a persistent connection with switchback support enabled.
    mxt_result TerminateSwitchbackConnection(IN mxt_opaque opqConnection);

    // Summary:
    //  Modifies the parameters of a persistent connection with switchback support enabled.
    mxt_result ModifySwitchback(IN mxt_opaque opqConnection,
                                IN unsigned int uMaxNbConnections,
                                OUT CVector<mxt_opaque>& rvecopqConnection,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection / Add per-user DNS lookup support
                                IN CList<CSocketAddr>* plstPeerAddr = NULL,
                                IN mxt_opaque opqUser = 0,
                                IN mxt_opaque opqDnsUser = 0);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection / Add per-user DNS lookup support

    // Summary:
    //  Notifies the persistent connection list that a switchbacked connection has been verified.
    mxt_result SwitchbackConnectivityVerified(IN mxt_opaque opqConnection);

    // Summary:
    //  Checks if a switchbacked persistent connection is active.
    bool IsSwitchbackActive(IN mxt_opaque opqConnection) const;

    // Summary:
    //  Gets the number of addresses returned by the DNS for a switchbacked connection.
    mxt_result GetSwitchbackNbAddresses(IN mxt_opaque opqConnection, OUT unsigned int& ruNbAddresses) const;

    // Summary:
    //  Configures the failure retry timer.
    void SetSwitchbackListFailureRetryTimer(IN uint32_t uTimeoutMs);
//M5T_INTERNAL_USE_END
#endif //-- #if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK)

    // Summary:
    //  Terminates a persistent connection.
    mxt_result Terminate(IN mxt_opaque opqConnection);

    // Summary:
    //  Allows or prevents DNS SRV failover for persistent connections.
    void SetAllowFailover(IN bool bEnable);

    // Summary:
    //  Configures how to retry to establish persistent connections.
    mxt_result SetRetryConfig(IN ERetryMode eMode,
                              IN uint32_t uTimeout1Ms,
                              IN uint32_t uTimeout2Ms,
                              IN uint32_t uTimeout3Ms = 90000);

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
    // Summary:
    //  Sets the keep alive manager of a connection.
    mxt_result SetKeepAliveMgr(IN ISipKeepAliveMgr* pMgr,
                               IN mxt_opaque opqConnection);

    // Summary:
    //  Sets the keep alive manager of a connection.
    mxt_result SetKeepAliveExtensionMgr(IN ISipKeepAliveExtensionMgr* pMgr,
                                        IN mxt_opaque opqConnection);

    //  Summary:
    //      Starts the CRLF keep alive on the specified flow.
    mxt_result StartKeepAlive(IN mxt_opaque opqConnection,
                              IN unsigned int uIntervalS = 120,
                              IN unsigned int uServerResponseTimeS = 10);

    //  Summary:
    //   Sends the data using the specified flow.
    mxt_result Send(IN mxt_opaque opqConnection,
                    IN mxt_opaque opq,
                    IN const uint8_t* puData,
                    IN unsigned int uSize);

    //-- << ISipKeepAliveMgr Interface >>

    //  Summary:
    //      Tells that the first keep alive has succeeded.
    virtual void EvFlowSucceeded(IN ESipTransport eTransport,
                                 IN const CSocketAddr& rLocalAddr,
                                 IN const CSocketAddr& rPeerAddr);

    //  Summary:
    //      Reports a flow failure.
    virtual void EvFlowFailure(IN ESipTransport eTransport,
                               IN const CSocketAddr& rLocalAddr,
                               IN const CSocketAddr& rPeerAddr);


    //-- << ISipKellAliveExtensionMgr >>
    virtual void EvDataReceived(IN ESipTransport eTransport,
                                IN const CSocketAddr& rLocalAddr,
                                IN const CSocketAddr& rPeerAddr,
                                IN uint8_t* puData,
                                IN unsigned int uSize);

#endif // -- #ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT


    //  Summary:
    //  Tells whether or not the connection is connected. Returns true when
    //  connected is the only state.
    bool IsConnected(IN mxt_opaque opqConnection);

    //  Summary:
    //      Gets the persistent connection opaque identifier.
    mxt_result GetConnection(IN ESipTransport eTransport,
                             IN const CSocketAddr& rLocalAddr,
                             IN const CSocketAddr& rPeerAddr,
                             OUT mxt_opaque& ropqConnection);

    //  Summary:
    //      Gets the persistent connection attributes.
    mxt_result GetConnection(IN mxt_opaque opqConnection,
                             OUT ESipTransport& reTransport,
                             OUT const CSocketAddr*& rpLocalAddr,
                             OUT const CSocketAddr*& rpPeerAddr);

    // Summary:
    //  Creates the SIP URI that needs to be queried to obtain the connection
    //  to the specified transport, host and port.
    mxt_result CreateUriToResolve(IN ESipTransport eTransport,
                                  IN const CString& rstrPeerHost,
                                  IN uint16_t uPeerPort,
                                  OUT GO CSipUri*& rpUri);

    // Summary:
    //  Updates the packet if it can be sent using a persistent connection.
    mxt_result UpdateVia(INOUT CSipPacket& rPacket,
                         IN bool bUpdateSentBy = true);

// BRCM_CUSTOM - [add] Add user opaque to persistent connection.
    // Summary:
    //  Retrieve the duplicated connection opaque at EvErrorOnConnection() with resFE_DUPLICATE.
    //-------------------------------------------------------------------------------------------
    mxt_opaque GetDuplicatedConnection(IN mxt_opaque opqConnection);

    // Summary:
    //  Add a user opaque to a persistent connection.
    //------------------------------------------------
    void AddUserOpaque(IN mxt_opaque opqConnection,
                       IN mxt_opaque opqUser);
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.

#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
    //  Summary:
    //      Configures a callback for the retrieval of the current SRV target name.
    void SetPersistentConnectionSrvRetrievalCallback(IN PFNSrvRetrievalCallback pfnSrvCallback);
#endif

    // Summary:
    //   Notifies the persistent connection list that a packet
    //   failed to be sent.
    void SipErrorOnConnection(IN mxt_opaque opqConnectionHandle,
                              IN const CSipPacket& rSentPacket,
                              IN mxt_result resConnectionError);

//-- Hidden Methods
protected:
private:
    //-- << New types within class namespace >>

    // These state are bitmaskable because we can be in more than one at the
    // same time.
    enum EConnectionState
    {
        // Connection has not yet been started.
        eNOT_CONNECTED = 0x00,
        // Waiting for dns results.
        eRESOLVING = 0x01,
        // Trying to connect or never succeeded to connect. This state is not
        // reset when an error happens on a connection.
        eTRYING = 0x02,
        eCONNECTED = 0x04,
         // Trying to close a connection. Only used when Terminate is called
         // or during the shutdown.
        eCLOSING = 0x08,
        // The connection failed and the timer is running to wait to retry.
        eWAITING_TO_RETRY = 0x10,
        // The connection has failed, and it will not be reestablished unless
        // forced by the ReEstablish method. Disconnect method sets this state
        eIDLE = 0x20,
        // The connection is terminated.
        eTERMINATED = 0x40
    };

    struct SConnectionData
    {
        ISipPersistentConnectionMgr* const m_pPersistentConnectionMgr;
        // Must store the local address opaque passed in Establish
        // because if opqLOCAL_ADDRESS_ANY is specified,
        // the local address can change from one attempt to the next
        // depending on the peer address.
        const mxt_opaque         m_opqLocalAddress;
// BRCM_CUSTOM - [add] Add user opaque to persistent connection.
        const mxt_opaque         m_opqUser;
        // The duplicated connection opaque used to store the duplicated connection ID in
        // EvErrorOnConnection() with resFE_DUPLICATE. This value can be retrieved by calling
        // GetDuplicatedConnection().
        // NOTE: This value is temporary and can be changed while establishing multiple (> 2)
        //       simultaneous persistent connections. It is recommended to call
        //       GetDuplicatedConnection() immediately when EvErrorOnConnection() is called.
        mxt_opaque               m_opqDupCnxTemp;
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
        mxt_opaque               m_opqDnsUser;
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
        CSocketAddr              m_localAddr;
        CSocketAddr              m_peerAddr;
        const ESipTransport      m_eTransport;

        // The peer hostname to resolve using the server locator. Can be empty
        // if no DNS request are necessary (Establish method taken an IP address
        // as the peer was called).
        CString                  m_strPeerHostnameToResolve;

        // The peer hostname or IP address to pass to the transport layer when
        // calling ConnectA. This member is used by the socket to compare
        // against the value of the Common Name (CN) in a TLS certificate.
        CString                  m_strPeer;
#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
        CString*                 m_pstrCurrentSrvTarget;
#endif
        uint16_t                 m_uPeerPort;
        uint16_t                 m_uRequestedLocalPort;
        CList<SNaptrRecord>*     m_plststRecords;
        // Is a bitset of the connection states.
        int                      m_nState;
        unsigned int             m_uNextTimeoutFactor;
        mxt_result               m_resConnectionError;
        // True if CSipTransportMgr has been called for this connection
        // and we are expecting a result.
        bool                     m_bConnecting;
        // True if the connection is switchbacked.
        bool                     m_bSwitchbackEnabled;
        // True if the switchbacked connection is verified.
        bool                     m_bSwitchbackVerified;
        // The switchbacked connection's priority. The smaller number has the
        // greater priority.
        unsigned int             m_uSwitchbackPriority;
        // Used by Outbound service:
        //  true if at least 1 response to  keep-alive has been received from the server.
        //  false otherwise.
        bool                     m_bFlowSucceeded;
        // Used by Outbound service:
        //  Number of consecutive failures trying to setup an outbound flow with a server.
        unsigned int             m_uFlowFailureCounter;
        // List containing a pointer to all other connections part of the switchbacked connection.
        CList<SConnectionData*>* m_plstSwitchbackConnections;
        // List of NAPTR records returned by the DNS for the switchbacked connection.
        CList<SNaptrRecord>*     m_plststSwitchbackRecords;
        // Timer ID linked to the connection data.
        unsigned int             m_uTimerId;
        ERetryMode m_eRetryMode;
        // The destination Id of the connection
        unsigned int m_uDestinationId;
#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
        // Pointer to the keep alive manager.
        ISipKeepAliveMgr* m_pKeepAliveMgr;
        // Pointer to the keep alive extension manager.
        ISipKeepAliveExtensionMgr* m_pKeepAliveExtensionMgr;
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

        SConnectionData(IN ISipPersistentConnectionMgr* pPersistentConnectionListMgr,
                        IN mxt_opaque opqLocalAddress,
// BRCM_CUSTOM - [begin] Add user opaque to persistent connection.
                        mxt_opaque opqUser,
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
                        mxt_opaque opqDnsUser,
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
                        IN uint16_t uLocalPort,
                        IN ESipTransport eTransport,
                        IN const CString& strPeerHostnameToResolve,
                        IN uint16_t uPeerPort,
                        IN const CString& rstrPeer,
                        IN unsigned int uTimerId,
                        IN ERetryMode eRetryMode,
                        IN unsigned int uDestinationId)
        :   m_pPersistentConnectionMgr(pPersistentConnectionListMgr),
            m_opqLocalAddress(opqLocalAddress),
// BRCM_CUSTOM - [begin] Add user opaque to persistent connection.
            m_opqUser(opqUser),
            m_opqDupCnxTemp(NULL),
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.
// BRCM_CUSTOM - [add] Add per-user DNS lookup support
            m_opqDnsUser(opqDnsUser),
// BRCM_CUSTOM - [end] Add per-user DNS lookup support
            m_localAddr(),
            m_peerAddr(),
            m_eTransport(eTransport),
            m_strPeerHostnameToResolve(strPeerHostnameToResolve),
            m_strPeer(rstrPeer),
#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
            m_pstrCurrentSrvTarget(NULL),
#endif
            m_uPeerPort(uPeerPort),
            m_uRequestedLocalPort(uLocalPort),
            m_plststRecords(NULL),
            // We cannot set this directly to eNOT_CONNECTED as MSVC 6
            // does not compile.
            m_nState(static_cast<EConnectionState>(0)),
            m_uNextTimeoutFactor(1),
            m_resConnectionError(resS_OK),
            m_bConnecting(false),
            m_bSwitchbackEnabled(false),
            m_bSwitchbackVerified(true),
            m_uSwitchbackPriority(0),
            m_bFlowSucceeded(false),
            m_uFlowFailureCounter(0),
            m_plstSwitchbackConnections(NULL),
            m_plststSwitchbackRecords(NULL),
            m_uTimerId(uTimerId),
            m_eRetryMode(eRetryMode),
            m_uDestinationId(uDestinationId)
#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
            ,
            m_pKeepAliveMgr(NULL),
            m_pKeepAliveExtensionMgr(NULL)
#endif // #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
        {
            m_localAddr.SetPort(uLocalPort);
        }

// BRCM_CUSTOM - [begin] Suppress VC8 L4 warnings
        SConnectionData& operator=( IN const SConnectionData& );
// BRCM_CUSTOM - [end] Suppress VC8 L4 warnings
    };

    //-- << Startup mechanism >>

    static mxt_result InitializeCSipPersistentConnectionList();

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipPersistentConnectionList();

    // Summary:
    //  Destructor.
    virtual ~CSipPersistentConnectionList();

    //-- << Helper Methods >>

    // Summary:
    //  Releases the persistent connection list objects.
    void Release();

    // Summary:
    //  Initializes the transport manager used by all instances of this class.
    static void Initialize(IN ISipTransportMgr& rTransportMgr);

    // Summary:
    //  Terminates and deletes this object.
    void Shutdown();

    // Summary:
    //   Helper for Establish.
    mxt_result EstablishHelper(IN TO SConnectionData* pstData,
                               IN bool bResolvePeerAddr,
                               OUT mxt_opaque& ropqConnection);

    // Summary:
    //  Checks if the packet must use a persistent connection and updates the
    //  packet accordingly.
    bool UpdatePacket(INOUT CSipPacket& rPacket, INOUT mxt_opaque& ropqConnection) const;

    // Summary:
    //  Removes the corresponding connection from the connection list.
    void ConnectionTerminatedHelper(IN unsigned int uIndex);

    // Summary:
    //  Warns the manager that the connection failed and starts the appropriate
    //  retry timer.
    void ConnectionFailedHelper(IN unsigned int uConnectionIndex);

    // Summary:
    //  The transport manager has notified that the connection is connected and
    //  ready to be used so the manager is warned.
    void ConnectionConnectedHelper(IN unsigned int uIndex);

    // Summary:
    //  Terminates every persistent connections related to the provided local
    //  address.
    void RemoveLocalAddress(IN const CSocketAddr& rLocalAddress);

    // Summary:
    //  Initiates the persistent connection.
    mxt_result Initiate(IN SConnectionData& rstConnection);

    // Summary:
    //  Selects a NAPTR target and attempts to connect to it.
    void Connect(INOUT SConnectionData& rstConnection);

    // Summary:
    //  Finds whether or not a persistent connection is already present.
    mxt_result FindConnection(IN const CSocketAddr& rLocalAddr,
                              IN const CSocketAddr& rPeerAddr,
                              IN ESipTransport      eTransport,
                              OUT unsigned int&      ruIndex,
                              IN bool bMustBeConnectedOrConnecting = false,
                              IN bool bUpdatePort = false);

    // Summary:
    //  Finds whether or not a persistent connection is already present.
    mxt_result FindConnection(IN const CSocketAddr& rLocalAddr,
                              IN const CString& rstrPeerHost,
                              IN const CSocketAddr& rPeerAddr,
                              IN uint16_t uPeerPort,
                              IN ESipTransport eTransport,
                              OUT unsigned int& ruIndex,
                              IN bool bMustBeConnectedOrConnecting = false);

    // Finds a persistent connection that match the packet transport, local and peer addresses.
    SConnectionData* FindConnection(IN CSipPacket& rPacket, IN bool bIsUpdatingPacket) const;

    // Summary:
    //  Converts the transport to a URI transport parameter value and a boolean
    //  indicating whether or not the URI is secured.
    mxt_result GetUriParamsFromTransport(
                            IN  ESipTransport           eTransport,
                            OUT CString&                rstrTransportParamValue,
                            OUT CSipUri::ESecurityFlag& reSecurityFlag);

    // Summary:
    //  Notifies the Manager that resolution is complete.
    void OnTargetResolvedNaptr(IN TOA CList<SNaptrRecord>* plststNaptrRecords,
                               IN SConnectionData& rstConnectionData);

    // Summary:
    //  Verifies that the connection is in the state in parameter.
    static bool IsInState(IN const SConnectionData& rstConnection,
                          IN EConnectionState eState);

    // Summary:
    //  Sets the specified state for the connection.
    static void SetState(INOUT SConnectionData& rstConnection,
                         IN EConnectionState eState);

    // Summary:
    //  Resets the specified state for the connection.
    static void ResetState(INOUT SConnectionData& rstConnection,
                           IN EConnectionState eState);

    // Summary:
    //  Resets all states correctly.
    void ResetAllStates(INOUT SConnectionData& rstConnectionData,
                        IN EConnectionState eNewState = eNOT_CONNECTED);

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)
    // Summary:
    //      Checks if there are succeeded flows.
    bool CheckForSucceededFlows();
#endif

#if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK)
//M5T_INTERNAL_USE_BEGIN
    // Summary:
    // Sets the addresses of switchbacked connections from the NAPTR records returned by the DNS.
    unsigned int SwitchbackNaptrHelper(IN unsigned int uPriority,
                                       IN SConnectionData* pConnectionData,
                                       IN CList<SNaptrRecord>* plststNaptrRecords,
                                       IN bool bReportErrors);

    // Summary:
    //  Deletes a switchbacked connection's data structure.
    void DeleteSwitchback(IN SConnectionData* pstData);

    // Summary:
    //  Finds the highest priority connection that is part of a switchbacked connection.
    SConnectionData* FindHighestPrioritySwitchback(IN SConnectionData* pstData) const;

    // Summary:
    //  Associates switchbacked connections.
    void SwitchbackAssociateConnections(IN const unsigned int uStartIndex, IN CVector<mxt_opaque>& rvecopqConnections);
    //M5T_INTERNAL_USE_END
#endif //-- #if defined(MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK)

    //-- << Messaging methods >>

    void InternalOnTargetResolvedNaptr(IN CMarshaler* pParams);

    void InternalEvConnectionClosed(IN CMarshaler* pParams);

    void InternalEvConnectionEstablished(IN CMarshaler* pParams);

    void InternalEvCommandResult(IN CMarshaler* pParams);

    void InternalEvTransportError(IN CMarshaler* pParams);

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    void InternalEvFlowSucceeded(IN CMarshaler* pParams);

    void InternalEvFlowFailure(IN CMarshaler* pParams);

    void InternalEvStartKeepAliveResult(IN CMarshaler* pParams);

    void InternalEvDataReceived(IN CMarshaler* pParams);

#endif  // #if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    //-- << CEventDriven Interface >>

    virtual void EvMessageServiceMgrAwaken(IN bool         bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler*  pParameter);

    virtual void EvTimerServiceMgrAwaken(IN bool         bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque   opq);

    //-- << ISipTransportObserver Interface >>

    virtual mxt_result EvPacketReceived(IN const CSipPacket& rPacket,
                                        IN bool              bHandled);

    virtual void EvTransportError(IN mxt_result         res,
                                  IN const CSocketAddr& rSource,
                                  IN const CSocketAddr& rDestination,
                                  IN ESipTransport      eTransport);

    virtual void EvConnectionClosed(IN const CSocketAddr& rSource,
                                    IN const CSocketAddr& rDestination,
                                    IN ESipTransport      eTransport,
                                    IN EClosureType       eClosureType);

    virtual void EvConnectionEstablished(IN const CSocketAddr& rSource,
                                         IN const CSocketAddr& rDestination,
                                         IN ESipTransport      eTransport);

    virtual void EvObserverRemoved();

    //-- << IServerLocatorMgr Interface >>

    virtual void OnAllTargetsResolved(IN TO CList<SNaptrRecord>* plst,
                                      IN    mxt_opaque           opq);

    virtual void OnTargetResolved(IN TO CList<SNaptrRecord>* plststNaptrRecords,
                                  IN    mxt_opaque           opq);

    virtual void OnTargetResolved(IN TO CList<SSrvRecord>* plststSrvRecords,
                                  IN    mxt_opaque         opq);

    virtual void OnTargetResolved(IN TO CList<CSocketAddr>* plstSocketAddr,
                                  IN    mxt_opaque          opq);

    //-- << ISipTransportUser Interface >>

    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opq);

//-- Hidden Methods
protected:
private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    CSipPersistentConnectionList(IN const CSipPersistentConnectionList& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipPersistentConnectionList& operator=(
                                   IN const CSipPersistentConnectionList& rSrc);


//-- Hidden Data Members
protected:
    // Messages.
    enum
    {
        eONTARGETRESOLVEDNAPTR,
        eEVCONNECTIONCLOSED,
        eEVCONNECTIONESTABLISHED,
        eEVCOMMANDRESULT,
        eEVTRANSPORTERROR,
        eEVFLOWSUCCEEDED,
        eEVFLOWFAILURE,
        eEVDATARECEIVED
    };

private:

    // Structure used to pass as an opaque when calling a method on the
    // ISipTransportMgr.
    struct STransportMgrData
    {
        enum ECommand
        {
            eSEND,
            eSTART_KEEP_ALIVE,
            eCONNECT_DISCONNECT
        };

        STransportMgrData(IN ECommand eCommand,
                          IN SConnectionData* pstConnectionData,
                          IN mxt_opaque opq = 0)
         : m_eCommand(eCommand),
           m_pstConnectionData(pstConnectionData),
           m_opq(opq)
        {};

        // The command passed to the transport manager.
        ECommand m_eCommand;
        // Used to retrieve the connection parameters.
        SConnectionData* m_pstConnectionData;
        // Could be the application opaque data, only used in case of Send.
        mxt_opaque m_opq;
    };

    CVector<SConnectionData*> m_vecpstConnections;

    CServerLocator* m_pServerLocator;

    static ISipTransportMgr* ms_pTransportMgr;

    bool m_bShutdownCalled;

    bool m_bFailoverAllowed;

    unsigned int m_uDnsCounter;

    unsigned int m_uStartingTimerMs;

    unsigned int m_uTimerCapMs;

#ifdef MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK
    uint32_t m_uSwitchbackListFailureRetryTimeoutMs;
#endif

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
    // Used by the Outbound service only.
    // This is the retry base time used when at least one flow has succeeded.
    unsigned int m_uStartingTimerSucceededFlowsFMs;
#endif
    ERetryMode m_eRetryMode;

#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
    // The persistent connection current SRV retrieval callback.
    PFNSrvRetrievalCallback m_pfnSrvCallback;
#endif
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//==============================================================================
//==
//==  SetAllowFailover
//==
//==============================================================================
//
//  Parameters:
//    bEnable:
//      Whether or not the SRV failover should be used when a connection attempt
//      fails.
//
//  Description:
//      Sets the behaviour when a connection attempt using a domain name fails.
//      When attempting a connection with a domain name, the name is resolved
//      according to the rules of RFC 3263. This means that if the port is
//      unspecified, the DNS is queried for NAPTR, SRV, and A records. The list
//      then randomly selects a target in these records. These steps are
//      performed regardless of the value of this parameter. If the connection
//      attempt fails, the behaviour of the list depends on the value of this
//      parameter. When true, the list selects another target in the set of
//      records previously received and attempts to connect with that target.
//      When false, it starts a timer (determined by the SetRetryConfig
//      configuration) and, when this timer fires, it requeries the DNS for
//      complete records and reselects a record randomly.
//
//      In other words, when this parameter is true, the list attempts all
//      targets returned by the DNS query before querying the DNS again. When it
//      is false, the list queries the DNS each time it attempts to connect
//      to the target.
//
//      Note that once the connection is successfully established, the DNS
//      results are cleared and if the connection should be closed or encounter
//      an error, the DNS is queried again before reconnecting.
//
//      By default, this parameter is true.
//
//      This parameter affects both existing and future connections.
//
//  See Also:
//      SetRetryConfig
//
//==============================================================================
inline void CSipPersistentConnectionList::SetAllowFailover(IN bool bEnable)
{
    m_bFailoverAllowed = bEnable;
}

#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
//==========================================================================
//==
//==  SetPersistentConnectionSrvRetrievalCallback
//==
//==========================================================================
//
//  Parameters:
//    pfnSrvCallback:
//      Method used to retrieve and store the current SRV in use.
//
//  Description:
//      Configures a callback function that allows the user of the stack to
//      retrieve the current persistent connection SRV. It can then be used
//      to match against a TLS certificate for example.
//
//  #Matching a SRV retrieval callback with a TLS callback#
//      In order to successfully match a SRV retrieval callback with the
//      proper TLS callback, the application first needs to set both
//      callbacks.
//
//      The SRV retrieval callback can be set after getting the
//      CSipPersistentConnectionList instance from the ISipCoreConfig
//      interface and calling this method.
//
//      The TLS callback can be set by calling SetHandshakeValidatorCallback
//      directly on the ISipCoreConfig interface.
//
//      After setting both callbacks, the application needs to store the
//      reported information of the SRV callback in order to use it in the
//      TLS callback later on. This can be done a number of ways, but the
//      important thing is to make sure to synchronize access to the data
//      container since both callbacks are reported on different threads.
//
//      When the TLS callback happens, a new parameter, the socket peer
//      address, can be used to match to the one passed in the SRV callback
//      and use the correct SRV target name to do additional/different
//      validation on the TLS certificate.
//
//==========================================================================
inline void CSipPersistentConnectionList::SetPersistentConnectionSrvRetrievalCallback(IN PFNSrvRetrievalCallback pfnSrvCallback)
{
    m_pfnSrvCallback = pfnSrvCallback;
}
#endif

//==============================================================================
//==
//==  IsInState
//==
//==============================================================================
//
//  Parameters:
//    rstConnection:
//      The connection data for which to verify the state.
//
//    eState:
//      The state to verify. This parameter can be a concatenation of multiple
//      states using the bitwise "OR" operator.
//
//  Returns:
//      True when all the states in parameter are set in the connection in
//      parameter. False otherwise.
//
//  Description:
//      Uses the bitwise "AND" operator to verify that the states in parameter
//      are set.
//
//==============================================================================
inline bool CSipPersistentConnectionList::IsInState(
                        IN const SConnectionData& rstConnection,
                        IN EConnectionState eState)
{
    return (rstConnection.m_nState & eState) == eState;
}

//==============================================================================
//==
//==  SetState
//==
//==============================================================================
//
//  Parameters:
//    rstConnection:
//      The connection data to set the state in.
//
//    eState:
//      The state to set.
//
//  Description:
//      Sets the specified state for the connection.
//
//==============================================================================
inline void CSipPersistentConnectionList::SetState(INOUT SConnectionData& rstConnection,
                                                   IN EConnectionState eState)
{
    rstConnection.m_nState = rstConnection.m_nState | eState;
}


//==============================================================================
//==
//==  ResetState
//==
//==============================================================================
//
//  Parameters:
//    rstConnection:
//      The connection data to reset the state from.
//
//    eState:
//      The state to reset.
//
//  Description:
//      Resets the specified state for the connection.
//
//==============================================================================
inline void CSipPersistentConnectionList::ResetState(INOUT SConnectionData& rstConnection,
                                                     IN EConnectionState eState)
{
    rstConnection.m_nState = rstConnection.m_nState & ~eState;
}

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_CSIPPERSISTENTCONNECTIONLIST_H

