//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_ISIPTRANSPORTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISIPTRANSPORTMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SIP Stack Global Configuration
#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

//-- Data Members

//-- Parameters used by Value
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" // ESipTransport
#endif

#ifndef MXG_ISIPTRANSPORTOBSERVER_H
#include "SipTransport/ISipTransportObserver.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

//-- Interface Realized and/or Parent


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
class ISipTransportUser;
class IEComUnknown;
class CSipPacket;
class CSocketAddr;
class CRawHeader;
class ISipKeepAliveMgr;
class ISipKeepAliveExtensionMgr;
#if defined(MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT)
class ISipNetworkReceptionControlMgr;
#endif
#if defined(MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT)
class ISipPacketReceptionControlMgr;
#endif


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISipTransportMgr
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
// Description:
//  This interface defines the services offered by the transport manager. The
//  transport manager is the object that is responsible for sending SIP packets
//  to fully resolved destinations with a specified transport.
//
//  If required, the transport manager establishes a connection to the remote
//  host before actually sending the packet.
//
//  The transport manager is responsible only for sending packets, not for their
//  retransmissions.
//
//  This is also the interface through which the core layer can communicate with
//  the keep alive service used for managing sip-outbound keep alives.
//
//  This interface is internal to the stack and should not be accessed directly
//  by the application.
//
// Location:
//   SipTransport/ISipTransportMgr.h
//
//==============================================================================
class ISipTransportMgr
{
//-- Friend Declarations


//-- Published Interface
public:
    //-- << Initialization >>

    //==========================================================================
    //==
    //==  Initialize
    //==
    //==========================================================================
    //
    //  Summary:
    //    Initializes the transport manager.
    //
    //  Description:
    //      Initializes the SIP transport module.
    //
    //==========================================================================
    virtual mxt_result Initialize() = 0;

    //==========================================================================
    //==
    //==  AddObserverA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds an observer for handling specific SIP transport events.
    //
    //  Parameters:
    //    pObserver:
    //      Pointer to the observer to add.
    //
    //    ePriority:
    //      Observer priority. eLOW_PRIORITY_OBSERVER is default.
    //
    //  Description:
    //      This method is used to append a new observer of activities on
    //      sockets.
    //
    //      eHIGH_PRIORITY_OBSERVER observers handle transport events before
    //      eLOW_PRIORITY_OBSERVER observers.
    //
    //      When two transport observers are added with the same priority, the
    //      last added observer handles transport events first.
    //
    //  See Also:
    //      ISipTransportObserver
    //      RemoveObserverA
    //
    //==========================================================================
    virtual void AddObserverA(
        IN ISipTransportObserver*                         pObserver,
        IN ISipTransportObserver::EInsertObserverPriority ePriority =
            ISipTransportObserver::eLOW_PRIORITY_OBSERVER) = 0;

    //==========================================================================
    //==
    //==  RemoveObserverA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes an observer from the observer list
    //
    //  Parameters:
    //    pObserver:
    //      Pointer to the observer to remove from the list.
    //
    //  Description:
    //      The method is called to remove an observer of activities sockets
    //      from the observer list.
    //
    //  See Also:
    //      ISipTransportObserver
    //      AddObserverA
    //
    //==========================================================================
    virtual void RemoveObserverA(IN ISipTransportObserver* pObserver) = 0;

    //-- << Persistent connections >>

    //==========================================================================
    //==
    //==  ConnectA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a new persistent connection to the connection list or
    //      reestablishes a lost connection.
    //
    //  Parameters:
    //    rSource:
    //      The connection's local IP address and port. The port can be 0,
    //       in which case a random available port is chosen. Note that
    //       a single persistent connection can be created with a local
    //       port of 0. Otherwise, an error is returned.
    //
    //    rDestination:
    //      The connection's peer IP address and port.
    //
    //    eTransport:
    //      The connection's transport ID.
    //
    //    rstrPeerHostName:
    //      The peer host name (for TLS handshaking purposes).
    //
    //    pUser:
    //      Notification interface where the result of the method is issued.
    //      Nothing is reported if NULL.
    //
    //    opqUserData:
    //      Opaque data associated with this connection request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //    uDestinationId:
    //      The destination ID to set in the socket. Only the contexts with
    //      the same destination ID will be able to use the connection.
    //      If there is no need to restrict the persistent connection to a context,
    //      0 is set by default on the persistent connection. By default,
    //      contexts have a destination ID of 0  and will be able to use any
    //      of the persistent connection with a destination ID of 0.
    //
    //  Description:
    //      This method is used to add a new persistent connection to the
    //      connection list or to reestablish a lost connection. If pUser is not
    //      NULL, the result of the command is reported through EvCommandResult
    //      on this interface.
    //
    //      If resS_TRANSPORT_TRYING is reported,
    //      EvConnectionEstablished or EvTransportError will eventually be
    //      reported on the transport observer interfaces that are registered
    //      with the transport manager. If resS_OK is reported, the connection
    //      already existed and has been successfully converted to a persistent
    //      connection. Neither EvConnectionEstablished nor EvTransportError are
    //      called in this case and the connection can be considered ready to
    //      use immediately.
    //
    //      For other possible error codes, please consult the
    //      documentation of CSipConnectionSvc::Connect.
    //
    //  See Also:
    //      CSipPersistentConnectionList, DisconnectA, CSipConnectionSvc
    //
    //==========================================================================
    virtual void ConnectA(IN const CSocketAddr& rLocalAddr,
                          IN const CSocketAddr& rPeerAddr,
                          IN ESipTransport eTransport,
                          IN const CString& rstrPeerHostName,
                          IN ISipTransportUser* pUser,
                          IN mxt_opaque opqUserData,
// BRCM_CUSTOM - [mod] Add user opaque to persistent connection.
                          IN unsigned int uDestinationId,
                          IN mxt_opaque opqUser = 0,
                          IN bool bCreateCnx = true) = 0;
// BRCM_CUSTOM - [end] Add user opaque to persistent connection.

    //==========================================================================
    //==
    //==  DisconnectA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes a persistent connection from the connection list.
    //
    //  Parameters:
    //    rLocalAddr:
    //      The connection's local IP address and port.
    //
    //    rPeerAddr:
    //      The connection's peer IP address and port.
    //
    //    eTransport:
    //      The connection's transport ID.
    //
    //    pUser:
    //      Notification interface where result of the method are issued.
    //      Nothing is reported if NULL.
    //
    //    opqUserData:
    //      Opaque data associated with this disconnection request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //    uDestinationId:
    //      Specify the destination ID assigned to the connection.
    //
    //  Description:
    //      This method is used to remove a persistent connection from the
    //      connection list. The result of this method is reported through the
    //      EvCommandResult method on the pUser interface passed in parameter,
    //      if any. A success indicates that the connection has been properly
    //      terminated. A failure indicates that the connection did not exist.
    //
    //  See Also:
    //      CSipPersistentConnectionList, ConnectA
    //
    //==========================================================================
    virtual void DisconnectA(IN const CSocketAddr& rLocalAddr,
                             IN const CSocketAddr& rPeerAddr,
                             IN ESipTransport eTransport,
                             IN ISipTransportUser* pUser,
                             IN mxt_opaque opqUserData,
                             IN unsigned int uDestinationId) = 0;

    //==============================================================================
    //==
    //==  GetLocalPortS
    //==
    //==============================================================================
    //
    //  Parameters:
    //    rLocalAddr:
    //      The local IP address. The port is not used as an input
    //      but will be set after this method returns.
    //
    //    rPeerAddr:
    //      The peer IP address and port.
    //
    //    eTransport:
    //      The transport.
    //
    //    uDestinationId:
    //      Specify the destination ID assigned to the connection.
    //
    //  Returns:
    //    resS_OK:
    //      Success.
    //
    //    resFE_FAIL:
    //      No such connections exist.
    //
    //  Description:
    //    This method takes a local address (no port), a peer address
    //    (including port) and a transport. If a connection exists that
    //    has the same parameters, the local port of this connection
    //    is set in rLocalAddr upon return.
    //
    //    If multiple connections with the same parameters exist, one
    //    is chosen at random and the local port of this connection is returned.
    //
    //==============================================================================
    virtual mxt_result GetLocalPortS(INOUT CSocketAddr& rLocalAddr,
                                     IN const CSocketAddr& rPeerAddr,
                                     IN ESipTransport eTransport,
                                     IN unsigned int uDestinationId) = 0;

    //-- << Listening >>
    //--------------------------------------------

    //==========================================================================
    //==
    //==  ListenA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Creates a server socket and listens on it (either for data or
    //      connection requests).
    //
    //  Parameters:
    //    transport:
    //      Transport to use.
    //
    //    rLocalAddress:
    //      Local IP address and port to listen on.
    //
    //    pUser:
    //      Notification interface to report failures and successes. Nothing is
    //      reported if NULL.
    //
    //    opq:
    //      Opaque data associated with this transmission request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //  Returns:
    //    resS_OK:
    //      The listen process has been properly started. If pUser is not NULL,
    //      the result of the command is reported through EvCommandResult on
    //      the ISipTransportUser interface.
    //
    //    resFE_INVALID_ARGUMENT:
    //      The rLocalAddress parameter is invalid. Even if pUser is not NULL,
    //      nothing is reported through EvCommandResult on the
    //      ISipTransportUser interface.
    //
    //    resFE_INVALID_STATE:
    //      The stack is in the shutdown process. If pUser is not NULL, a
    //      resFE_SIPTRANSPORT_SHUTDOWN result is reported through
    //      EvCommandResult on the ISipTransportUser interface.
    //
    //  Description:
    //      Opens a socket with the corresponding characteristics and listens on
    //      it.
    //
    //      For connection-oriented transports, a socket is created and bound to
    //      the specified local address. The socket then waits for incoming
    //      connection requests.
    //
    //      For connection-less transports, a socket is created and bound to the
    //      specified local address, but it is not connected to any remote
    //      address. The stack simply waits for incoming data on the socket.
    //
    //  See Also:
    //      SendA
    //
    //==========================================================================
    virtual mxt_result ListenA(IN ESipTransport      eTransport,
                               IN const CSocketAddr& rLocalAddress,
                               IN ISipTransportUser* pUser,
                               IN mxt_opaque         opq) = 0;

    //==========================================================================
    //==
    //==  StopListeningA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Closes a previously listening socket.
    //
    //  Parameters:
    //    eTransport:
    //      Param Description.
    //
    //    rLocalAddress:
    //      Local IP address and port to which stop listening.
    //
    //    pUser:
    //      Notification interface to report failures and successes. Nothing is
    //      reported if NULL.
    //
    //    opq:
    //      Opaque data associated with this transmission request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //  Description:
    //      Deletes the server socket listening to the specified port and
    //      transport.
    //
    //      Note that calling this method when there are ongoing transactions
    //      can have strange side effects, particularly when the socket is used
    //      for sending data.
    //
    //  See Also:
    //      ListenA
    //
    //==========================================================================
    virtual void StopListeningA(IN ESipTransport      eTransport,
                                IN const CSocketAddr& rLocalAddress,
                                IN ISipTransportUser* pUser,
                                IN mxt_opaque         opq) = 0;

    //-- << Sending >>

    //==========================================================================
    //==
    //==  SendA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends a packet through the transport layer.
    //
    //  Parameters:
    //    rPacketOut:
    //      The SIP packet to send.
    //
    //    pUser:
    //      Notification interface to report failures and successes. Nothing is
    //      reported if NULL.
    //
    //    opq:
    //      Opaque data associated with this transmission request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //  Description:
    //      This method must be used when trying to send a packet to a resolved
    //      IP address and port using a specific transport. Information about
    //      where and how to send the packet must be further specified before
    //      calling SendA through the pPacketOut parameter.
    //
    //      The local address and port to use for sending the request must be
    //      specified through <C>CSipPacket::SetLocalAddr()</C>. This address
    //      may be invalid.
    //
    //      The peer address and port where the packet is to be sent must be
    //      specified in <C>CSipPacket::SetPeerAddr()</C>. This address must
    //      always be valid.
    //
    //      The transport to use for sending the packet must be specified in
    //      <C>CSipPacket::SetTransport()</C>.
    //
    //      To specify whether or not a new connection can be created if none
    //      exists to the specified peer, use <C>CSipPacket::AllowNewConnection()</C>. A
    //      connection is a socket connected to a single specific peer.
    //
    //      If the local address is invalid, the processing consists of checking
    //      for existing sockets connected to the peer address using the
    //      specified transport. If such a socket exists, it is used to send the
    //      packet. If no such socket is found and a new connection is allowed,
    //      a new socket is created and then connected to the remote host with
    //      the specified transport.
    //
    //      If the local address is valid, the transport layer first searches
    //      for a socket locally bound to that address and connected to the peer
    //      address with the specified transport. If no such socket is found,
    //      then the transport layer searches for an unconnected socket bound
    //      to the specified local address. If such a socket is found, the
    //      transport layer performs a SendTo() operation to send the packet.
    //      If no socket is found and a new connection is allowed, the
    //      transport layer creates a new socket bound to the specified local
    //      address and connects it to the specified peer address.
    //
    //      Whenever creating new connections is disallowed and an existing
    //      connected socket is not found, the SIP transport reports an error
    //      instead of creating a new connected socket.
    //
    //      An unconnected socket is created when ListenA() is called for a
    //      connection-less transport. This "listening" socket is in fact a
    //      socket waiting to receive data from any host instead of waiting for
    //      connections, as would be done with TCP. Such unconnected sockets can
    //      be used for sending data when pSource corresponds to the local
    //      address to which the socket is bound.
    //
    //      Note that when using an unconnected socket for sending data, the IP
    //      stack does not report errors such as ICMP errors. The socket has to
    //      be connected in order to allow the stack to report this kind of
    //      error.
    //
    //  See Also:
    //      ListenA, ISipTransportUser
    //
    //==========================================================================
    virtual void SendA(IN const CSipPacket&  rPacketOut,
                       IN ISipTransportUser* pUser,
                       IN mxt_opaque         opq) = 0;

    //==========================================================================
    //==
    //==  ShutdownA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Shuts down the service chain.
    //
    //  Parameters:
    //    rUser:
    //      Notification interface to report failures and successes.
    //
    //    opq:
    //      Opaque data associated with this transmission request, used as a
    //      parameter by the transport manager when reporting back events
    //      through the ISipTransportUser interface.
    //
    //  Description:
    //      Shuts down the transport manager. After calling this, all calls to
    //      SendA will fail.
    //
    //      pUser->EvCommandResult() with resS_OK is called when the shutdown
    //      process is finished. If already in the shutdown process,
    //      pUser->EvCommandResult() with resFE_FAIL is called.
    //
    //==========================================================================
    virtual void ShutdownA(IN ISipTransportUser& rUser,
                           IN mxt_opaque         opq) = 0;

    //==========================================================================
    //==
    //==  ReHandleEvPacketReceivedA
    //==
    //==========================================================================
    // <FLAG New in 4.1.4>
    //
    //  Summary:
    //      Handles a packet on every transport observers just as if it was just
    //      received from the network.
    //
    //  Parameters:
    //      rPacket:
    //       The SIP packet.
    //
    //  Description:
    //      Handles a packet by giving it back to the list of transport
    //      observers. This is useful when the packet received from the network
    //      was shown to its observers and handled by one of them. For whatever
    //      reason, the observer could determine later on that the packet cannot
    //      be handled as it thought.
    //
    //      Such an example is the reception of a supplemental 2xx to an INVITE
    //      when forking occurs. If a supplemental 2xx is received too quickly
    //      (just before the original 2xx is processed from the transaction
    //      manager message queue), then the 2xx was dropped by the transaction
    //      that was supposed to handle it. This method allows the transaction
    //      layer (or any other transport observer) to inform the transport
    //      layer that the packet could not be handled.
    //
    //      This method SHOULD NOT be called from the application. It is only
    //      meant to be used internally by the SIP stack.
    //
    //==========================================================================
    virtual void ReHandleEvPacketReceivedA(IN const CSipPacket& rPacket) = 0;

    //==============================================================================
    //==
    //==  AddListenAddressInParserSvcA
    //==
    //==============================================================================
    //
    //  Parameters:
    //      rstrAddress:
    //       Listening address to add.
    //
    //      uPort:
    //       Port of the listening address to add.
    //
    //      pvecFqdn:
    //       Pointer to a list of FQDN associated with the listening address.
    //
    //  Description:
    //      This method is used by CSipNetworkInterfaceList to signal to
    //      CSipParserSvc that a new listening address needs to be added.
    //      CSipParserSvc maintains an internal list of addresses to validate the
    //      via sent by in received response. CSipParserSvc needs to maintain
    //      its own list since it is running in the transport thread.
    //      CSipNetworkInterfaceList is running in the core thread.
    //
    //  See Also:
    //      RemoveListenAddressInParserSvcA
    //
    //==============================================================================
    virtual void AddListenAddressInParserSvcA(IN const CString& rstrAddress,
                                              IN uint16_t uPort,
                                              IN const CVector<CString>* pvecFqdn) = 0;

    //==============================================================================
    //==
    //==  RemoveListenAddressInParserSvcA
    //==
    //==============================================================================
    //
    //  Parameters:
    //      rstrAddress:
    //       Listening address to remove.
    //
    //      uPort:
    //       Port of the listening address to remove.
    //
    //      pvecFqdn:
    //       Pointer to a list of FQDN associated with the listening address.
    //
    //  Description:
    //      This method is used by CSipNetworkInterfaceList to signal to
    //      CSipParserSvc that a listening address is no longer valid.
    //      CSipParserSvc maintains an internal list of addresses to validate the
    //      via sent by in received response. CSipParserSvc needs to maintain
    //      its own list since it is running in the transport thread.
    //      CSipNetworkInterfaceList is running in the core thread.
    //
    //  See Also:
    //      AddListenAddressInParserSvcA
    //
    //==============================================================================
    virtual void RemoveListenAddressInParserSvcA(IN const CString& rstrAddress,
                                                 IN uint16_t uPort,
                                                 IN const CVector<CString>* pvecFqdn) = 0;

#if defined(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

    //==========================================================================
    //==
    //==  SetKeepAliveMgrA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a keep alive manager.
    //
    //  Parameters:
    //      pMgr:
    //       The manager.
    //
    //  Description:
    //      Sets the manager that is responsible to handle events related to
    //      the flow failures and successes.
    //
    //==========================================================================
    virtual void SetKeepAliveMgrA(IN ISipKeepAliveMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SetKeepAliveExtensionMgrA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sets a keep alive extension manager.
    //
    //  Parameters:
    //      pMgr:
    //       The manager.
    //
    //  Description:
    //      The application can register this manager when it wants to be
    //      informed of non SIP data reception. This is useful when an
    //      application wants to send keep alives using STUN requests for
    //      instance. When the application is responsible to send STUN requests,
    //      it is also responsible to receive and interpret the STUN responses.
    //      This is currently possible by configuring this manager.
    //
    //==========================================================================
    virtual void SetKeepAliveExtensionMgrA(IN ISipKeepAliveExtensionMgr* pMgr) = 0;

    //==========================================================================
    //==
    //==  SendA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Sends the data on the specified socket.
    //
    //  Parameters:
    //      eTransport:
    //       The connection's transport ID.
    //
    //      rLocalAddr:
    //       The connection's local IP address and port.
    //
    //      rPeerAddr:
    //       The connection's peer IP address and port.
    //
    //      puData:
    //       A pointer to the data to send.
    //
    //      uSize:
    //       The size of the data to send
    //
    //      pUser:
    //       Notification interface to report failures and successes. Nothing is
    //       reported if NULL.
    //
    //      opq:
    //       Opaque data associated with this data sending. It is used as a
    //       parameter by the transport manager when reporting back events
    //       through the ISipTransportUser interface.
    //
    //      uDestinationId:
    //        The destination ID of the packet to send. The packets with
    //        a destination ID will be sent through a socket
    //        that has the same destination ID.
    //
    //  Description:
    //      Sends a flow of bytes that can be non SIP data. This method can be
    //      useful to send STUN binding requests over a connection that the SIP
    //      stack has created.
    //
    //      The responses to this flow of bytes are reported through the
    //      ISipKeepAliveExtensionMgr manager. Currently the SIP stack only
    //      reports packets that correspond to STUN over UDP transport.
    //
    //==========================================================================
    virtual void SendA(IN ESipTransport eTransport,
                       IN const CSocketAddr& rLocalAddr,
                       IN const CSocketAddr& rPeerAddr,
                       IN const uint8_t* puData,
                       IN unsigned int uSize,
                       IN ISipTransportUser* pUser,
                       IN mxt_opaque opq,
                       IN unsigned int uDestinationId) = 0;

    //==========================================================================
    //==
    //==  EnableOutboundCrlfKeepAliveA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables the CRLF keep alive for reliable transports.
    //
    //  Parameters:
    //      bEnable:
    //       True to enable the CRLF keep alive. False indicates that there will
    //       be no keep alives using CRLF.
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
    //      CRLFs on each flow selected by the application. The application
    //      decides when it wants to start sending CRLF keep alives to the
    //      server. It tells the stack to do so when the method
    //      StartOutboundCrlfKeepAliveA is called on the transport manager.
    //      Usually the application starts issuing keep alives when it receives
    //      a response to a REGISTER request that contains the Require header
    //      with outbound value.
    //
    //      When the CRLF keep alive is enabled, the stack is responsible to
    //      send the keep alives and receive the responses. The application does
    //      not need to interact with the stack for the CRLF keep alives.
    //
    //==========================================================================
    virtual void EnableOutboundCrlfKeepAliveA(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  StartOutboundCrlfKeepAliveA
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts the CRLF keep alive on the specified connection.
    //
    //  Parameters:
    //      eTransport:
    //       The connection's transport ID.
    //
    //      rLocalAddr:
    //       The connection's local IP address and port.
    //
    //      rPeerAddr:
    //       The connection's peer IP address and port.
    //
    //      uIntervalS:
    //       The interval (in seconds) at which CRLF keep alives are sent to the
    //       server. This defaults to 120 seconds. The stack chooses a random
    //       value for each keep alive sent between 80% and 100% of the
    //       specified value. If the default is used, then the CRLF keep alive
    //       will be sent at a value chosen randomly between 95 and 120 seconds.
    //       It cannot be zero or a value less than uServerResponseTimeS.
    //
    //      uServerResponseTimeS:
    //       This is the maximum time for which the client waits for the server
    //       response when it has sent a keep alive. The default waiting time is
    //       10 seconds as described in the draft. It cannot be zero or a value
    //       higher than uIntervalS.
    //
    //      pUser:
    //       Notification interface to report failures and successes. Nothing is
    //       reported if NULL.
    //
    //      opq:
    //       Opaque data associated with this data sending. It is used as a
    //       parameter by the transport manager when reporting back events
    //       through the ISipTransportUser interface.
    //
    //      uDestinationId:
    //       Specify the destination ID assigned to the connection.
    //
    //  Description:
    //      Starts the CRLF keep alive mechanism on the specified
    //      connection. This is usually called when a response to a REGISTER
    //      request containing a Require header with the "outbound" value. This
    //      needs to be called every time that a connection is re-established.
    //
    //      When called, the stack starts an internal timer that will trigger a
    //      CRLF keep alive to be sent on the connection after X seconds. This
    //      timer chooses a random value between 80% and 100% of the time
    //      configured on the transport manager.
    //
    //      If a CRLF keep alive response fails to arrive within the configured
    //      server response time, the connection is closed.
    //
    //      Closing a flow (persistent connection) triggers the event
    //      EvErrorOnConnection on the persistent connection list manager. The
    //      persistent connection will try automatically to re-establish itself.
    //      When re-established, the event EvEstablished is called again and
    //      StartOutboundCrlfKeepAliveA needs to be reinvoked by the application
    //      when if wants to restart the CRLF keep alives.
    //
    //==========================================================================
    virtual void StartOutboundCrlfKeepAliveA(IN ESipTransport eTransport,
                                             IN const CSocketAddr& rLocalAddr,
                                             IN const CSocketAddr& rPeerAddr,
                                             IN unsigned int uIntervalS,
                                             IN unsigned int uServerResponseTimeS,
                                             IN ISipTransportUser* pUser,
                                             IN mxt_opaque opq,
                                             IN unsigned int uDestinationId) = 0;

#endif // #if defided(MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT)

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
    //  Description:
    //      Sets the network reception control service manager. This allows the
    //      application to be notified of the network reception control service
    //      events.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void SetNetworkReceptionControlMgrA(IN ISipNetworkReceptionControlMgr* pMgr) = 0;

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
    //  See Also:
    //      ISipCoreConfig::SetPeerFloodBlockingA
    //
    //==========================================================================
    virtual void SetPeerFloodBlockingA(IN uint32_t uRawPacketCountLimit,
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
    //  Description:
    //      Sets the packet reception control service manager. This allows the
    //      application to be notified of the packet reception control service
    //      events.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void SetPacketReceptionControlMgrA(IN ISipPacketReceptionControlMgr* pMgr) = 0;

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
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //    #More on Peer Filtering Option#
    //      This option maintains two separate lists, one of approved peers and
    //      another of allowed SIP methods. Both lists' content are populated
    //      by the application.
    //
    //      When a packet is first received, after being parsed, the Packet
    //      Reception Control service checks if Peer Filtering is enabled.
    //      If so, it then checks if the packet is coming from an approved
    //      peer. If not, it finally checks if the packet method is allowed
    //      to go through regardless.
    //
    //      At any point during that time, if the packet is found to be from an
    //      unapproved peer and/or an disallowed method, the packet is
    //      automatically discarded. If the packet either matches an approved
    //      peer or an allowed method, it is forwarded up the transport service
    //      chain, eventually reaching the application.
    //
    //      The approved peer list contains the list of peers the application
    //      always wishes to receive all packets from. A peer is typically added
    //      in that list once approved by the application, one way or another.
    //
    //      The allowed method list contains the list of SIP methods the
    //      application always wishes to receive packets from. A method is
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
    virtual void EnablePeerFilteringA(IN bool bEnable) = 0;

    //==========================================================================
    //==
    //==  ModifyApprovedPeerListA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Modifies the Peer Filtering option approved peers list.
    //
    //  Parameters:
    //    rPeerAddr:
    //      The peer address to modify the peer list with.
    //
    //    bIsApproved:
    //      Tells if the peer is approved or not.
    //
    //  Description:
    //      It allows the user to modify at any time the Peer Filtering option
    //      approved peers list. If approved, the peer is added to the
    //      list, if not already present. If not approved, the peer is
    //      removed from the list if present.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void ModifyApprovedPeerListA(IN const CSocketAddr& rPeerAddr, IN bool bIsApproved) = 0;

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
    //  Description:
    //      It allows the user to easily removes all entries in the Peer
    //      Filtering option approved peers list.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void RemoveAllApprovedPeersA() = 0;

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
    //==  ModifyAllowedMethodListA
    //==
    //==========================================================================
    // <FLAG New in 4.1.8>
    //
    //  Summary:
    //      Modifies the Peer Filtering option allowed methods list.
    //
    //  Parameters:
    //    pszMethod:
    //      The SIP method to modify the methods list with.
    //
    //    bIsAllowed:
    //      Tells if the SIP method is allowed or not.
    //
    //  Description:
    //      It allows the user to modify at any time the Peer Filtering option
    //      allowed methods list. If allowed, the method is added to the
    //      list, if not already present. If not allowed, the method is
    //      removed from the list if present.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void ModifyAllowedMethodListA(IN const char* pszMethod, IN bool bIsAllowed) = 0;

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
    //  Description:
    //      It allows the user to easily removes all entries in the Peer
    //      Filtering option allowed methods list.
    //
    //      This method is internal to the stack and must not be called by the
    //      application.
    //
    //==========================================================================
    virtual void RemoveAllAllowedMethodsA() = 0;

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

//-- Hidden Methods
protected:

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISipTransportMgr(){};

    // Summary:
    //  Destructor. Use ReleaseService instead.
    virtual ~ISipTransportMgr(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISipTransportMgr(IN const ISipTransportMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISipTransportMgr& operator=(IN const ISipTransportMgr& rSrc);


//-- Hidden Data Members
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_ISIPTRANSPORTMGR_H

