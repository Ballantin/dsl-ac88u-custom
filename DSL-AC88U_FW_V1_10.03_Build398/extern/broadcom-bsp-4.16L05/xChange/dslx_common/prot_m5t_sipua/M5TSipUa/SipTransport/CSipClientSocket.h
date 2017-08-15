//==============================================================================
//==============================================================================
//
//     Copyright(c) 2004-2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity
//   of such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_CSIPCLIENTSOCKET_H
#define MXG_CSIPCLIENTSOCKET_H

//M5T_INTERNAL_USE_BEGIN

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- SIP Stack Global Configuration
#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h" // MXD_SIPSTACK_ENABLE_TLS
#endif

//-- Data Members
#ifndef MXG_CBLOB_H
#include "Cap/CBlob.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSIPPACKET_H
#include "SipTransport/CSipPacket.h"
#endif

//-- Parameters used by Value
#ifndef MXG_CSIPTRANSPORTTOOLS_H
#include "SipTransport/CSipTransportTools.h" //-- ESipTransport
#endif

//-- Interface Realized and/or Parent
#ifndef MXG_IASYNCSOCKETMGR_H
#include "Network/IAsyncSocketMgr.h"
#endif

#ifndef MXG_IASYNCCLIENTSOCKETMGR_H
#include "Network/IAsyncClientSocketMgr.h"
#endif

#ifndef MXG_IASYNCIOSOCKETMGR_H
#include "Network/IAsyncIoSocketMgr.h"
#endif

#ifndef MXG_IASYNCUNCONNECTEDIOSOCKETMGR_H
#include "Network/IAsyncUnconnectedIoSocketMgr.h"
#endif

#ifdef MXD_SIPSTACK_ENABLE_TLS
    #ifndef MXG_IASYNCTLSSOCKETMGR_H
    #include "Tls/IAsyncTlsSocketMgr.h"
    #endif

    #ifndef MXG_CTLSSESSION_H
    #include "Tls/CTlsSession.h"
    #endif

    #ifndef MXG_CLIST_H
    #include "Cap/CList.h"
    #endif
#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS


#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class IAsyncSocket;
class IAsyncClientSocket;
class IAsyncIoSocket;
class IAsyncUnconnectedIoSocket;
class ISipTransportUser;
class CSipTransportSvc;
class CSipTransportMgr;
class CCertificateChain;
#if defined(MXD_SIPSTACK_ENABLE_TLS)
    class IAsyncTlsSocket;
    class ISipTlsContextFactory;
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// As per RFC 3261, conformance item 724.
const unsigned int uMX_SIPTRANSPORT_MAX_BUFFER_SIZE = 0xFFFF; // max. 64K

extern const CString* g_pstrEMPTY;
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

typedef
mxt_result (*PFNHandshakeValidator)(bool,
                                    bool,
                                    const CCertificateChain&,
                                    CString&
#if defined(MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL)
                                    , const CSocketAddr&
#endif
                                    );

#if defined(MXD_SIPSTACK_ENABLE_TLS)
// The m_localAddress is the "effective local address" because it is possible to
// have two connection to the same peer at the same time using port 0, and in
// that case the two connections would fetch the same session in the cache.
//
// A Tls session in the cache is considered "active" (in used) when m_localAddress
// is different than "0.0.0.0:0"; otherwise it is considered "inactive" and it is
// kept in the cache so the Tls session (in m_tlsSession) can be used later for
// another connection to the same peer.
struct STlsSessionCache
{
    CTlsSession m_tlsSession;
    CSocketAddr m_peerAddress;
    CSocketAddr m_localAddress;
    bool bTlsSessionSet;
};
#endif

//==============================================================================
//== Class: CSipClientSocket
//========================================
//
// Description:
//   Encapsulates a socket to asynchronously send and/or receive SIP packets.
//
// Location:
//   SipTransport/CSipClientSocket.h
//
//==============================================================================
class CSipClientSocket : public IAsyncSocketMgr,
                         public IAsyncClientSocketMgr,
                         public IAsyncIoSocketMgr,
                         public IAsyncUnconnectedIoSocketMgr
#ifdef MXD_SIPSTACK_ENABLE_TLS
                         ,public IAsyncTlsSocketMgr
#endif
{
//-- Friend Declarations

    //M5T_INTERNAL_USE_BEGIN
    friend class CSipStackInitializer;
    //M5T_INTERNAL_USE_END

//-- Published Interface
public:
    // Description:
    //  EServiceId allows to reserve a specific array position to each element
    //  of the service chain.
    enum EServiceId
    {
        eTRANSPORTMGR,
        eCONNECTIONSVC,
        ePARSERSVC,
        eLOGGERSVC,
        eALL
    };

    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Constructor.
    CSipClientSocket(IN  CSipTransportSvc&  rNetworkSideSvc,
                     IN  ESipTransport      eTransport,
                     IN  const CSocketAddr& rLocalAddr,
                     IN  const CSocketAddr& rPeerAddr,
                     IN  ISipTransportUser* pTransportUser,
                     IN  mxt_opaque         opq,
                     IN unsigned int        uDestinationId,
#ifdef MXD_SIPSTACK_ENABLE_TLS
                     IN  const CString&     rstrPeerHostname,
#endif
                     OUT mxt_result&        rResult);


    // Summary:
    //  Constructor for an incoming connection socket.
    CSipClientSocket(IN CSipTransportSvc& rNetworkSideSvc,
                     IN ESipTransport     eTransport,
                     IN TO IAsyncIoSocket* pAsyncIoSocket);

    // Summary:
    //  Destructor.
    virtual ~CSipClientSocket();

    //-- << CSipClientSocket defined interface >>

    // Summary:
    //  Sets the maximum total size allowed for buffered data.
    static void SetMaxBufferSize(IN unsigned int nMaxBufferSize);

    // Summary:
    //  Returns the maximum total size allowed for buffered data.
    static unsigned int GetMaxBufferSize();

// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
    // Summary:
    //  Sets the SubjectAltName extension check option.
    static void SetBypassSubjectAltNameCheck(IN bool bBypass);
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check

    // Summary:
    //  Sends the specified SIP packet on the network or buffers it for
    //  sending when the network is ready.
    mxt_result Send(IN const CSipPacket&  rPacket,
                    IN ISipTransportUser* pUser,
                    IN mxt_opaque         opq);

#ifdef MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
    //  Summary
    //  Sends the specified data on the network.
    mxt_result Send(IN TOA const CBlob* pData,
                    IN ISipTransportUser* pUser,
                    IN mxt_opaque         opq);

    //  Summary
    //  Sends the specified data to the specified destination on the network.
    mxt_result SendTo(IN TOA const CBlob* pData,
                      IN const CSocketAddr& rPeerAddr,
                      IN ISipTransportUser* pUser,
                      IN mxt_opaque opq);
#endif

    // Summary:
    //  Forces to close the embedded asynchronous socket.
    mxt_result Close();

    // Summary:
    //  Returns whether or not the socket is available (i.e., not closing).
    bool IsAvailable() const;

    //  Summary:
    //  Returns the local IP and port address.
    const CSocketAddr& GetLocalAddr() const;

    // Summary:
    //  Returns the peer IP and port address.
    const CSocketAddr& GetPeerAddr() const;

    // Summary:
    //  Returns the current peer IP and port address.
    const CSocketAddr& GetCurrentPeerAddr() const;

    // Summary:
    //  Returns the service information (generally a pointer) for the specified
    //  service.
    virtual mxt_opaque GetSvcInfo(IN EServiceId eId) const;

    // Summary:
    //  Sets the service information (generally a pointer) for the specified
    //  service.
    virtual void SetSvcInfo(IN EServiceId eId, IN mxt_opaque opq);

// BRCM_CUSTOM - [begin] Add user opaque to socket.
    // Summary:
    //  Returns the user opaque value associated to the socket.
    virtual void GetUserOpaque(OUT CVector<mxt_opaque>* pvecOpq) const;

    // Summary:
    //  Associates the socket with a user opaque value.
    virtual void AddUserOpaque(IN mxt_opaque opq);

    // Summary:
    //  Remove a user opaque from the socket
    virtual void RemoveUserOpaque(IN mxt_opaque opq);
// BRCM_CUSTOM - [end] Add user opaque to socket.

    //  Summary:
    //   Tells if the socket is currently accumulating a fragmented SIP packet.
    bool IsAccumulatingSipPacket() const;

    // Summary:
    //  Returns the transport.
    ESipTransport GetTransport() const;

    // Summary:
    //  Gets the destination ID of the socket.
    unsigned int GetDestinationId() const;

    // Summary:
    //  Tells if the socket is currently binding or not.
    bool IsSocketBinding() const;

    //-- << IAsyncSocketMgr >>

    // Summary:
    //  Used to notify the socket that it has been closed locally.
    void EvAsyncSocketMgrClosed(IN mxt_opaque opq);

    // Summary:
    //  Used to notify the socket that it has been closed remotely.
    void EvAsyncSocketMgrClosedByPeer(IN mxt_opaque opq);

    // Summary:
    //  Used to notify the socket that an error occurred.
    void EvAsyncSocketMgrErrorDetected(IN mxt_opaque opq, IN mxt_result res);

    //-- << IAsyncIoSocketMgr Interface >>

    // Summary:
    //  Notifies the socket user that there is data available to be received
    //  on the socket.
    void EvAsyncIoSocketMgrReadyToRecv(IN mxt_opaque opq);

    // Summary:
    //  Notifies the socket user that the socket is ready to send data on the
    //  network.
    void EvAsyncIoSocketMgrReadyToSend(IN mxt_opaque opq);


    //-- << IAsyncClientSocketMgr Interface >>

    // Summary:
    //  Notifies that the client socket has been bound.
    void EvAsyncClientSocketMgrBound(IN mxt_opaque opq,
                                     IN CSocketAddr* pEffectiveLocalAddress);

    // Summary:
    //  Notifies of the successful connection to a remote server.
    void EvAsyncClientSocketMgrConnected(IN mxt_opaque opq);

    //-- << IAsyncUnconnectedIoSocketMgr Interface >>

    // Summary:
    //  Notifies the socket user that there is data available to be received
    //  on the socket.
    void EvAsyncUnconnectedIoSocketMgrReadyToRecv(IN mxt_opaque opq);

    // Summary:
    //  Notifies the socket user that the socket is ready to send data on
    //  the network.
    void EvAsyncUnconnectedIoSocketMgrReadyToSend(IN mxt_opaque opq);

    // Summary:
    // Gets the remote peer hostname.
    const CString& GetPeerHostname() const;

#ifdef MXD_SIPSTACK_ENABLE_TLS
    // <COMBINE CSipClientSocket::GetPeerHostname@const>
    CString& GetPeerHostname();
#endif

    // Summary:
    //  Tells if the remote peer is authenticated for this TLS connection.
    bool IsAuthenticatedConnection();

#ifdef MXD_SIPSTACK_ENABLE_TLS
    //-- << IAsyncTlsSocketMgr Interface >>

    // Summary:
    //  Notifies that the TLS handshaking is completed and gives the manager
    //  the opportunity to do some more validation prior to Connected or
    //  SwitchedInTls notification.
    void EvAsyncTlsSocketMgrTlsHandshakingCompletedAwaitingApproval(
                             IN mxt_opaque               opq,
                             IN const CCertificateChain* pPeerCertificateChain,
                             IN IAsyncTlsSocket*         pAsyncTlsSocket);

    // Summary:
    //  Notifies of the chosen session parameters.
    void EvAsyncTlsSocketMgrNewTlsSession(IN mxt_opaque         opq,
                                          IN const CTlsSession* pTlsSession);

#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

    //-- << Application Callbacks >>.

    // Summary:
    //  Sets the handshake validator method (callback).
    static void SetHandshakeValidatorCB(
                        PFNHandshakeValidator pfnHandshakeValidator,
                        bool bOverrideDefaultBehaviour);

    // Summary:
    //  Returns true when the embedded socket is connected, false otherwise.
    bool IsConnected();

#if defined(MXD_SIPSTACK_ENABLE_TLS)

    // Summary:
    //  Adds a new element to the list of cached Tls session.
    mxt_result AddTlsSessionCache(IN const CSocketAddr& rPeerAddress,
                                  IN const CSocketAddr& rLocalAddress,
                                  OUT mxt_opaque& ropq);

    // Summary:
    //  Clears the cached TLS session.
    static void ClearSessionCache();

    // Summary:
    //  Modifies a cached Tls session.
    void ModifyTlsSessionCache(IN mxt_opaque opq,
                               IN const CTlsSession& rSession);

    // Summary:
    //  Gets the Tls session associated with the address.
    mxt_result FetchTlsSessionCache(IN const CSocketAddr& rPeerAddr,
                                    IN const CSocketAddr& rLocalAddr,
                                    OUT CTlsSession& rSession,
                                    OUT mxt_opaque& ropq);

    // Summary:
    //  Sets the maximum size of the cached Tls session list.
    static mxt_result SetTlsSessionCacheMaxSize(IN unsigned int uMaxSize);

    // Summary:
    //  Returns a const pointer to the list of cached TLS session.
    static const CList<STlsSessionCache*>* GetCachedTlsSessionList();

#endif

//-- Hidden Methods
protected:

    // Summary:
    //  Sends the first SIP packet of the remaining buffered data when the
    //  network is ready.
    mxt_result SendBuffered();

    // Summary:
    //  Processes all buffered data.
    void OnReadyToSend();

    // Summary:
    //  Receives a packet from the network.
    void OnReadyToRecv();

    // Summary:
    //  Notifies the service chain that an error occurred.
    void OnErrorDetected(IN mxt_result res);

    // Summary:
    //  Flushes the buffer vector and its contents.
    void DeleteBuffered();

    // Summary:
    //  Helper sent method used to send Sip packets or non-Sip data.
    void SendHelper(IN const CSipPacket* pPacket,
                    IN TOA const CBlob* pData,
                    IN ISipTransportUser* pUser,
                    IN mxt_opaque opq,
                    IN const CSocketAddr& rPeerAddr = CSocketAddr());

    // Summary:
    //  Sets the DSCP value on the socket.
    void SetSocketDscp(uint32_t uDscp);

#ifdef MXD_SIPSTACK_ENABLE_TLS
    // Summary:
    //  Validates the TLS certificate against the remote peer hostname.
    static mxt_result ValidateTlsPersonalCertificateHelper(
                            IN const CCertificateChain& rPeerCertificateChain,
                            IN const CString& rstrPeerHostname);

    // Summary:
    // Gets the TLS context factory from the core config.
    void GetTlsContextFactoryHelper(OUT ISipTlsContextFactory*& rpTlsContextFactory);

    // Summary:
    //  Compares host names.
    static bool AreHostnamesEquivalent(IN const CString& rstrHost1,
                                       IN const CString& rstrHost2);

    // Summary:
    //  Set a Tls session to "inactive" in the cache.
    void SetInactiveTlsSession(IN const CSocketAddr& rPeerAddress,
                               IN const CSocketAddr& rLocalAddress);

#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

private:
    //-- << Startup mechanism >>

    static mxt_result InitializeCSipClientSocket();

    static void FinalizeCSipClientSocket();


    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    CSipClientSocket();

    // Summary:
    //  Copy Constructor.
    CSipClientSocket(IN const CSipClientSocket& rSrc);

    // Summary:
    //  Assignment Operator.
    CSipClientSocket& operator=(IN const CSipClientSocket& rSrc);

//-- Hidden Data Members
protected:
private:

    // Allows to discriminate which socket can be used and which cannot.
    bool m_bClosing;

    // Differentiates if the socket was closed locally or by a peer.
    bool m_bClosedByPeer;

    // The maximum total size for the buffered data.
    static unsigned int ms_uMaxBufferSize;

    // Structure used to store a serialized SIP packet and its info.
    struct SPacketData
    {
        CBlob              m_blob;
        const CSipPacket*  m_pPacket;
        ISipTransportUser* m_pPacketUser;
        mxt_opaque         m_opq;

        SPacketData(IN const CSipPacket&  rPacket,
                    IN ISipTransportUser* pUser,
                    IN mxt_opaque         opq)
                    : m_pPacket(&rPacket),
                      m_pPacketUser(pUser),
                      m_opq(opq)
        {
            m_pPacket->AddRef();
        }

        SPacketData(IN const CBlob&       rBlob,
                    IN const CSipPacket*  pPacket,
                    IN ISipTransportUser* pUser,
                    IN mxt_opaque         opq)
                    : m_blob(rBlob.GetSize() + 1), // GetSize() + 1 to allow
                                                   // null terminator.
                      m_pPacket(pPacket),
                      m_pPacketUser(pUser),
                      m_opq(opq)
        {
            m_blob = rBlob;
            if (m_pPacket != NULL)
            {
                m_pPacket->AddRef();
            }
        }

        ~SPacketData()
        {
            if (m_pPacket != NULL)
            {
                m_pPacket->Release();
            }
        }

    private:
        SPacketData(IN const SPacketData& rstSrc);
        SPacketData();
        SPacketData& operator=(IN const SPacketData& rstSrc);
    };

    // Used to store the remaining data to send and the info pertaining to
    // them.
    CVector<SPacketData*> m_vecpstPacketData;

    // The total size reserved for the buffered blobs.
    unsigned int m_uTotalReservedSize;

    // True when the embedded socket is connected, false otherwise.
    bool m_bConnected;

    // The socket's interfaces.
    IAsyncSocket* m_pAsyncSocket;
    IAsyncClientSocket* m_pAsyncClientSocket;
    IAsyncIoSocket* m_pAsyncIoSocket;
    IAsyncUnconnectedIoSocket* m_pAsyncUnconnectedIoSocket;

    // The local IP and port address.
    CSocketAddr m_localAddr;

    // The peer IP and port address. When the socket is not connected (like
    // listening on UDP), then this address is invalid.  Used only in client
    // sockets but kept here for interface purposes.
    CSocketAddr m_peerAddr;

    // The current peer IP and port address (i.e. the peer address when
    // valid or the address returned by the last call of RecvFrom on the
    // async socket).  Used only in client sockets but kept here for
    // interface purposes.
    CSocketAddr m_currentPeerAddr;

    // The transport protocol.
    ESipTransport m_eTransport;

    // Transport user.
    ISipTransportUser* m_pTransportUser;

    // Opaque to pass in the EvCommandResult to the transport user.
    mxt_opaque m_opqTransportUser;

    // Destination ID of the socket
    unsigned int m_uDestinationId;

#ifdef MXD_SIPSTACK_ENABLE_TLS
    // The TLS specific socket interfaces.
    IAsyncTlsSocket* m_pAsyncTlsSocket;

    // The remote peer hostname associated to this socket.
    CString m_strPeerHostname;

    // Tells if this socket was created as a TLS client or server socket. True
    // means that it was created as a client, false as a server.
    bool m_bIsTlsClient;

    //  The handshake validator method pointer.
    static PFNHandshakeValidator ms_pfnHandshakeValidator;

    // List of cached TLS session.
    static CList<STlsSessionCache*>* ms_plstTlsSession;

    //  Indicates if the handshake validator method pointer overrides the
    //  default behavior.
    static bool ms_bOverrideDefaultBehaviour;

// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
    // This variable can be used to bypass the SubjectAltName check with the
    // Request-URI host.
    static bool ms_bBypassSubjectAltNameCheck;
// BRCM_CUSTOM [end] - Bypass SubjectAltName check

#endif // #ifdef MXD_SIPSTACK_ENABLE_TLS

    // The service to which the socket reports network events.
    static CSipTransportSvc* ms_pNetworkSideSvc;

    // Used to store service information
    mxt_opaque m_aopqSvcInfo[eALL];

    // The socket is being bound. This is important to know if an error
    // happens on a UDP listening socket so that the CSipConnectionSvc will close it.
    bool m_bBinding;
};

//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==========================================================================
//==
//==  IsAccumulatingSipPacket
//==
//==========================================================================
//
//  Returns:
//      True when the socket is handling a packet that was fragmented,
//      otherwise false.
//
//  Description:
//      Tells if the socket is accumulating a fragmented packet. It can only
//      happen over reliable transports. This method is used by the
//      CSipKeepAliveSvc for CRLF keep alives. When true is returned, it
//      means that the keep alive service must let the packet reach the
//      parser service as it must process the data to continue accumulating
//      the SIP packet.
//
//==========================================================================
inline
bool CSipClientSocket::IsAccumulatingSipPacket() const
{
    return (m_aopqSvcInfo[ePARSERSVC] != 0);
}

//==============================================================================
//==
//==  GetSvcInfo
//==
//==============================================================================
//
//  Parameters:
//      eId:
//       The service id.
//
//  Returns:
//      The stored information to be retrieved.
//
//  Description:
//      Returns the service information (generally a pointer) for the specified
//      service.
//==============================================================================
inline
mxt_opaque CSipClientSocket::GetSvcInfo(IN EServiceId eId) const
{
    MX_ASSERT(eId >= 0 && eId < eALL);

    return m_aopqSvcInfo[eId];
}


//==============================================================================
//==
//==  SetMaxBufferSize
//==
//==============================================================================
//
//  Parameters:
//    nMaxBufferSize
//      The maximum buffer size.
//
//  Description:
//      Sets the maximum total size allowed for buffered data.
//=============================================================================
inline
void CSipClientSocket::SetMaxBufferSize(IN unsigned int nMaxBufferSize)
{
    ms_uMaxBufferSize = nMaxBufferSize;
}

//==============================================================================
//==
//==  GetMaxBufferSize
//==
//==============================================================================
//
//  Returns:
//      The maximum buffer size.
//
//  Description:
//      Gets the maximum total size allowed for buffered data.
//=============================================================================
inline
unsigned int CSipClientSocket::GetMaxBufferSize()
{
    return ms_uMaxBufferSize;
}

//==============================================================================
//==
//==  GetPeerHostname
//==
//==============================================================================
//
//  Returns:
//      The constant remote hostname.
//
//  Description:
//      Gets the remote peer hostname.
//=============================================================================
inline
const CString& CSipClientSocket::GetPeerHostname() const
{
#ifdef MXD_SIPSTACK_ENABLE_TLS
    return m_strPeerHostname;
#else
    return *g_pstrEMPTY;
#endif
}

//==============================================================================
//==
//==  GetPeerHostname
//==
//==============================================================================
//
//  Returns:
//      The remote hostname.
//
//  Description:
//      Gets the remote peer hostname.
//=============================================================================
#ifdef MXD_SIPSTACK_ENABLE_TLS
inline
CString& CSipClientSocket::GetPeerHostname()
{
    return m_strPeerHostname;
}
#endif

// BRCM_CUSTOM [begin] - Bypass SubjectAltName check
#ifdef MXD_SIPSTACK_ENABLE_TLS
inline
void CSipClientSocket::SetBypassSubjectAltNameCheck(IN bool bBypass)
{
    ms_bBypassSubjectAltNameCheck = bBypass;
}
#endif
// BRCM_CUSTOM [end] - Bypass SubjectAltName check

//==============================================================================
//==
//==  GetLocalAddr
//==
//==============================================================================
//
//  Returns:
//      The local address.
//
//  Description:
//      Returns the local address.
//=============================================================================
inline
const CSocketAddr& CSipClientSocket::GetLocalAddr() const
{
    return m_localAddr;
}

//==============================================================================
//==
//==  GetPeerAddr
//==
//==============================================================================
//
//  Returns:
//      The peer address.
//
//  Description:
//      Returns the peer address.
//=============================================================================
inline
const CSocketAddr& CSipClientSocket::GetPeerAddr() const
{
    return m_peerAddr;
}

//==============================================================================
//==
//==  GetCurrentPeerAddr
//==
//==============================================================================
//
//  Returns:
//      The actual peer address.
//
//  Description:
//      Returns the actual peer address.
//
//==============================================================================
inline
const CSocketAddr& CSipClientSocket::GetCurrentPeerAddr() const
{
    return m_currentPeerAddr;
}

//==============================================================================
//==
//==  GetTransport
//==
//==============================================================================
//
//  Returns:
//    The transport protocol.
//
//  Description:
//    Returns the transport protocol.
//=============================================================================
inline
ESipTransport CSipClientSocket::GetTransport() const
{
    return m_eTransport;
}

//==============================================================================
//==
//==  IsAvailable
//==
//==============================================================================
//
//  Returns:
//    True if the socket is available, false otherwise.
//
//  Description:
//    Returns the state of the socket (either available or closing).
//=============================================================================
inline
bool CSipClientSocket::IsAvailable() const
{
    return (m_bClosing == false);
}

//==============================================================================
//==
//==  SetHandshakeValidatorCB
//==
//==============================================================================
//
//  Parameters:
//      pfnHandshakeValidator:
//       The handshake validator method (pointer).
//      bOverrideDefaultBehaviour:
//       True when pfnHandshakeValidator will override the default behaviour of
//       the handshake validator. When false pfnHandshakeValidator will be
//       invoked prior to the default handshake validator.
//
//  Description:
//      Sets the handshake validator method (callback).
//
//==============================================================================
inline void CSipClientSocket::SetHandshakeValidatorCB(
                                 IN PFNHandshakeValidator pfnHandshakeValidator,
                                 IN bool bOverrideDefaultBehaviour)
{
#ifdef MXD_SIPSTACK_ENABLE_TLS
    ms_pfnHandshakeValidator = pfnHandshakeValidator;
    ms_bOverrideDefaultBehaviour = bOverrideDefaultBehaviour;
#else
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pfnHandshakeValidator);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(bOverrideDefaultBehaviour);
#endif
}

//==============================================================================
//==
//==  IsConnected
//==
//==============================================================================
//
//  Description:
//      Tells whether or not the embedded socket is connected.
//
//==============================================================================
inline bool CSipClientSocket::IsConnected()
{
    return m_bConnected;
}

//==============================================================================
//==
//==  GetDestinationId
//==
//==============================================================================
//
//
//  Returns:
//      The destination ID of the socket.
//
//  Description:
//      Returns the destination ID of the socket.
//
//==============================================================================
inline unsigned int CSipClientSocket::GetDestinationId() const
{
    return m_uDestinationId;
}

//==============================================================================
//==
//==  IsSocketBinding
//==
//==============================================================================
//
//  Description:
//      Tells whether or not the embedded socket is binding.
//
//==============================================================================
inline bool CSipClientSocket::IsSocketBinding() const
{
    return m_bBinding;
}


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_CSIPCLIENTSOCKET_H

