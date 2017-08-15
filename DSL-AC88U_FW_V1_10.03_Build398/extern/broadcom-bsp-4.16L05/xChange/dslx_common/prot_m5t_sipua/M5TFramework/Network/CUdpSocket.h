//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CUDPSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CUDPSOCKET_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
                                 // MXD_IPV6_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

//-- Data Members
//-----------------
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CSOCKET_H
#include "Network/CSocket.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#ifndef MXG_ICLIENTSOCKET_H
#include "Network/IClientSocket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
// BRCM_CUSTOM - [add] Callback routines for socket operations
typedef void (*CUDPSOCKET_BIND)( mxt_hSocket socket, const CSocketAddr *pLocalAddr );
typedef void (*CUDPSOCKET_CONNECT)( mxt_hSocket socket, const CSocketAddr *pPeerAddr );
typedef void (*CUDPSOCKET_SEND)( mxt_hSocket socket, uint8_t **ppData, unsigned int *puSize );
typedef void (*CUDPSOCKET_SENDTO)( mxt_hSocket socket, const CSocketAddr *pPeerAddr, uint8_t **ppData, unsigned int *puSize );
typedef void (*CUDPSOCKET_RECVFROM)( mxt_hSocket socket, const CSocketAddr *pPeerAddr, const uint8_t *puData, const unsigned int uCapacity, int *nSizeReceived );
typedef void (*CUDPSOCKET_CLOSE)( mxt_hSocket socket );
// BRCM_CUSTOM - [end] Callback routines for socket operations

//==SDOC========================================================================
//== Class: CUdpSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Implementation of a client UDP socket.
//
// Description:
//   The CUDPSocket class is an implementation of a UDP socket in
//   client mode which supports the IClientSocket interface.
//
// Location:
//   Network/CUdpSocket.h
//
// See Also:
//   CSomethingElse, CSomething, CMachin
//
//==EDOC========================================================================
class CUdpSocket : protected CSocket,
                   public IClientSocket
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CUdpSocket();

    //-- << Stereotype >>
    //--------------------

    // Summary:
    //  Create the socket.
    //----------------------
    mxt_result Create(IN CSocketAddr::EAddressFamily eAddressFamily);

    //==========================================================================
    //== Methods inherited from CSocket.
    //==========================================================================

    mxt_hSocket GetHandle() const;
    mxt_result Set8021QUserPriority(IN bool bEnable, IN uint8_t uUserPriority);
    mxt_result SetAllowAnySource(IN bool bEnable);
    mxt_result SetBlocking(IN bool bEnable);
    mxt_result SetBroadcast(IN bool bEnable);
    mxt_result SetReceiveBufferSize(IN unsigned int uSize);
    mxt_result SetTos(IN uint8_t uTos);
    mxt_result SetTransmitBufferSize(IN unsigned int uSize);
    mxt_result SetUdpChecksum(IN bool bEnable);
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    mxt_result SetIpv6UnicastHops(IN int nHops);
#endif
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);
    mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);
#endif

    //==========================================================================
    //== Interface IClientSocket methods
    //==========================================================================

    virtual mxt_result Bind(IN const CSocketAddr* pLocalAddress,
                            OUT CSocketAddr* pEffectiveLocalAddress);

    virtual mxt_result Connect(IN const CSocketAddr* pPeerAddress);

    // =========================================================================
    // Interface IIoSocket methods
    // =========================================================================

    virtual mxt_result GetPeerAddress(OUT CSocketAddr* pPeerAddress) const;

    virtual mxt_result Recv(OUT CBlob* pData);

    virtual mxt_result Recv(OUT uint8_t* puData,
                            IN unsigned int uCapacity,
                            OUT unsigned int* puSize);

    virtual mxt_result RecvFrom(OUT CBlob* pData, OUT CSocketAddr* pPeerAddress);

    virtual mxt_result RecvFrom(OUT uint8_t* puData,
                                IN unsigned int uCapacity,
                                OUT unsigned int* puSize,
                                OUT CSocketAddr* pPeerAddress);

    virtual mxt_result Send(IN const CBlob* pData,
                            OUT unsigned int* puSizeSent);

    virtual mxt_result Send(IN const uint8_t* puData,
                            IN unsigned int uSize,
                            OUT unsigned int* puSizeSent);

    virtual mxt_result SendTo(IN const CBlob* pData,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress);

    virtual mxt_result SendTo(IN const uint8_t* puData,
                              IN unsigned int uSize,
                              OUT unsigned int* puSizeSent,
                              IN const CSocketAddr* pPeerAddress);

    // =========================================================================
    // Interface ISocket methods
    // =========================================================================

    virtual mxt_result Close(IN ECloseBehavior eBehavior);

    virtual mxt_result GetLocalAddress(OUT CSocketAddr* pLocalAddress) const;

    virtual mxt_result GetProtocolFamily(OUT EProtocolFamily* peProtocolFamily) const;
    virtual mxt_result GetAddressFamily(OUT CSocketAddr::EAddressFamily* peAddressFamily) const;

    virtual mxt_result GetSocketType(OUT ESocketType* peSocketType) const;

    virtual uint32_t Release();

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CUdpSocket();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CUdpSocket(IN const CUdpSocket& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CUdpSocket& operator=(IN const CUdpSocket& rSrc);

    // Summary:
    //  Comparison Operator.
    //-----------------------
    CUdpSocket& operator==(IN const CUdpSocket& rSrc);

    void Reset();

#if defined (MXD_OS_WINDOWS)
    mxt_result WSAIoctl_SIO_UDP_CONNRESET(IN bool bState);
#endif

//-- Hidden Data Members
//------------------------
protected:
    // The socket state.
    bool        m_bCreated;
    bool        m_bBound;

    // The peer address. i.e, where packets are sent.
    CSocketAddr m_peerAddress;
    bool        m_bConnected;
//M5T_INTERNAL_USE_END
};

inline
mxt_hSocket CUdpSocket::GetHandle() const
{
    return CSocket::GetHandle();
}

inline
mxt_result CUdpSocket::Set8021QUserPriority(IN bool bEnable,
                                        IN uint8_t unUserPriority)
{
    return CSocket::Set8021QUserPriority(bEnable, unUserPriority);
}

inline
mxt_result CUdpSocket::SetAllowAnySource(IN bool bEnable)
{
    return CSocket::SetAllowAnySource(bEnable);
}

inline
mxt_result CUdpSocket::SetBlocking(IN bool bEnable)
{
    return CSocket::SetBlocking(bEnable);
}

inline
mxt_result CUdpSocket::SetBroadcast(IN bool bEnable)
{
    return CSocket::SetBroadcast(bEnable);
}

inline
mxt_result CUdpSocket::SetReceiveBufferSize(IN unsigned int unSize)
{
    return CSocket::SetReceiveBufferSize(unSize);
}

inline
mxt_result CUdpSocket::SetTos(IN uint8_t unTos)
{
    return CSocket::SetTos(unTos);
}

inline
mxt_result CUdpSocket::SetTransmitBufferSize(IN unsigned int unSize)
{
    return CSocket::SetTransmitBufferSize(unSize);
}

inline
mxt_result CUdpSocket::SetUdpChecksum(IN bool bEnable)
{
    return CSocket::SetUdpChecksum(bEnable);
}

#if defined(MXD_IPV6_ENABLE_SUPPORT)
inline
mxt_result CUdpSocket::SetIpv6UnicastHops(IN int nHops)
{
    return CSocket::SetIpv6UnicastHops(nHops);
}
#endif // #if defined(MXD_IPV6_ENABLE_SUPPORT)

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
inline
mxt_result CUdpSocket::SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec)
{
    return CSocket::SetWindowsReceivingFlowspec(pReceivingFlowspec);
}

inline
mxt_result CUdpSocket::SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec)
{
    return CSocket::SetWindowsSendingFlowspec(pSendingFlowspec);
}

#endif

// BRCM_CUSTOM - [add] Callback routines for socket operations
//==============================================================================
//====  EXPORTS  ===============================================================
//==============================================================================
extern CUDPSOCKET_BIND      g_CUdpSocket_BindCB;
extern CUDPSOCKET_CONNECT   g_CUdpSocket_ConnectCB;
extern CUDPSOCKET_SEND      g_CUdpSocket_SendCB;
extern CUDPSOCKET_SENDTO    g_CUdpSocket_SendToCB;
extern CUDPSOCKET_RECVFROM  g_CUdpSocket_RecvFromCB;
extern CUDPSOCKET_CLOSE     g_CUdpSocket_CloseCB;
// BRCM_CUSTOM - [end] Callback routines for socket operations

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CUDPSOCKET_H
