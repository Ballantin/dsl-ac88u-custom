//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_CTCPSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTCPSOCKET_H
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

#if !defined(MXD_OS_SYMBIAN) && \
     defined(MXD_NETWORK_ENABLE_SUPPORT)

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

#ifndef MXG_MXTRACEFRAMEWORK_H
#include "Basic/MxTraceBasic.h"
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
typedef void (*CTCPSOCKET_BIND)( mxt_hSocket socket, const CSocketAddr *pLocalAddr );
typedef void (*CTCPSOCKET_CONNECT)( mxt_hSocket socket, const CSocketAddr *pPeerAddr );
typedef void (*CTCPSOCKET_SEND)( mxt_hSocket socket, const CBlob *pData );
typedef void (*CTCPSOCKET_RECV)( mxt_hSocket socket, const uint8_t *puData, const unsigned int uCapacity, int *nSizeReceived );
typedef void (*CTCPSOCKET_CLOSE)( mxt_hSocket socket );
// BRCM_CUSTOM - [end] Callback routines for socket operations

// BRCM_CUSTOM - [begin] Add TCP socket binding support.
#if CCCFG_TCP_SOCK_BIND_LOCAL_ADDRESS_SUPPORT
extern char g_szTCPSOCKET_LOCAL_INTERFACE[32];
#endif /* CCCFG_TCP_SOCK_BIND_LOCAL_ADDRESS_SUPPORT */
// BRCM_CUSTOM - [end] Add TCP socket binding support.

//==SDOC========================================================================
//== Class: CTcpSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Implementation of a client TCP socket.
//
// Description:
//   The CTcpSocket class is an implementation of a TCP socket in
//   client mode which supports the IClientSocket interface.
//
// Location:
//   Network/CTcpSocket.h
//
//==EDOC========================================================================
// BRCM_CUSTOM - [mod] Add ECOS support.
#if defined( MXD_OS_BOS_ECOS )
class CTcpSocket :  public CSocket,
#else
class CTcpSocket :  protected CSocket,
#endif
// BRCM_CUSTOM - [end] Add ECOS support.
                    public IClientSocket
{
//-- Friend Declarations
//------------------------

//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CTcpSocket();

    //-- << Stereotype >>
    //--------------------

    // Summary:
    //  Creates the socket.
    //----------------------
    mxt_result Create(IN CSocketAddr::EAddressFamily eAddressFamily);

    // Summary:
    //  Also creates the socket but this method is restricted to
    //  CTcpServerSocket::Accept( ) method.
    //------------------------------------------------------------
    mxt_result Create(IN EProtocolFamily eProtocolFamily,
                  IN mxt_hSocket hSocket,
                  IN CSocketAddr* pPeerAddress);


    //==========================================================================
    //== Methods inherited from CSocket.
    //==========================================================================

    mxt_hSocket GetHandle() const;
    mxt_result Set8021QUserPriority(IN bool bEnable, IN uint8_t uUserPriority);
    mxt_result SetBlocking(IN bool bEnable);
    mxt_result SetKeepAlive(IN bool bEnable);
    mxt_result SetNagle(IN bool bEnable);
    mxt_result SetReceiveBufferSize(IN unsigned int uSize);
    mxt_result SetReuseAddress(IN bool bEnable);
    mxt_result SetTos(IN uint8_t uTos);
    mxt_result SetTransmitBufferSize(IN unsigned int uSize);
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    mxt_result SetIpv6UnicastHops(IN int nHops);
#endif
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);
    mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);
#endif
    mxt_result SetBackgroundVoipSocket(IN bool bEnable);

    //==========================================================================
    //== Interface IClientSocket methods
    //==========================================================================

    virtual mxt_result Bind(IN const CSocketAddr* pLocalAddress,
                            OUT CSocketAddr* pEffectiveLocalAddress);

    virtual mxt_result Connect(IN const CSocketAddr* pPeerAddress);

    //==========================================================================
    //== Interface IIoSocket methods
    //==========================================================================

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

    //==========================================================================
    //== Interface ISocket methods
    //==========================================================================

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
    virtual ~CTcpSocket();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CTcpSocket(IN const CTcpSocket& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CTcpSocket& operator=(IN const CTcpSocket& rSrc);

    // Summary:
    //  Comparison Operator.
    //-----------------------
    CTcpSocket& operator==(IN const CTcpSocket& rSrc);

    void Reset();

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
mxt_hSocket CTcpSocket::GetHandle() const
{
    return CSocket::GetHandle();
}

inline
mxt_result CTcpSocket::Set8021QUserPriority(IN bool bEnable,
                                        IN uint8_t unUserPriority)
{
    return CSocket::Set8021QUserPriority(bEnable, unUserPriority);
}

inline
mxt_result CTcpSocket::SetBlocking(IN bool bEnable)
{
    return CSocket::SetBlocking(bEnable);
}

inline
mxt_result CTcpSocket::SetReuseAddress(IN bool bEnable)
{
    return CSocket::SetReuseAddress(bEnable);
}

inline
mxt_result CTcpSocket::SetReceiveBufferSize(IN unsigned int unSize)
{
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(unSize);

    mxt_result res = resS_OK;
#if defined(MXD_OS_WINDOWS_CE)
        MX_TRACE1(0,
                  g_stFramework,
                  "CTcpSocket - ::SetReceiveBufferSize () ERROR: (%x) \"%s\"",
                  static_cast<unsigned int>(resFE_NOT_IMPLEMENTED),
                  MxResultGetMsgStr(resFE_NOT_IMPLEMENTED));
#else
    res = CSocket::SetReceiveBufferSize(unSize);
#endif
    return res;
}

inline
mxt_result CTcpSocket::SetTos(IN uint8_t unTos)
{
    return CSocket::SetTos(unTos);
}

inline
mxt_result CTcpSocket::SetTransmitBufferSize(IN unsigned int unSize)
{
    return CSocket::SetTransmitBufferSize(unSize);
}

inline
mxt_result CTcpSocket::SetNagle(IN bool bEnable)
{
    return CSocket::SetNagle(bEnable);
}

inline
mxt_result CTcpSocket::SetKeepAlive(IN bool bEnable)
{
    return CSocket::SetKeepAlive(bEnable);
}

#if defined(MXD_IPV6_ENABLE_SUPPORT)
inline
mxt_result CTcpSocket::SetIpv6UnicastHops(IN int nHops)
{
    return CSocket::SetIpv6UnicastHops(nHops);
}
#endif // #if defined(MXD_IPV6_ENABLE_SUPPORT)

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)

inline
mxt_result CTcpSocket::SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec)
{
    return CSocket::SetWindowsReceivingFlowspec(pReceivingFlowspec);
}

inline
mxt_result CTcpSocket::SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec)
{
    return CSocket::SetWindowsSendingFlowspec(pSendingFlowspec);
}

#endif

inline
mxt_result CTcpSocket::SetBackgroundVoipSocket(IN bool bEnable)
{
    mxt_result res = resS_OK;
    if (m_bConnected)
    {
        res = CSocket::SetBackgroundVoipSocket(bEnable);
    }
    else
    {
        // Will be set once the socket is connected.
        m_bOptBackgroundVoipSocketEnabled = bEnable;
    }

    return res;
}

// BRCM_CUSTOM - [add] Callback routines for socket operations
//==============================================================================
//====  EXPORTS  ===============================================================
//==============================================================================
extern CTCPSOCKET_BIND     g_CTcpSocket_BindCB;
extern CTCPSOCKET_CONNECT  g_CTcpSocket_ConnectCB;
extern CTCPSOCKET_SEND     g_CTcpSocket_SendCB;
extern CTCPSOCKET_RECV     g_CTcpSocket_RecvCB;
extern CTCPSOCKET_CLOSE    g_CTcpSocket_CloseCB;
// BRCM_CUSTOM - [end] Callback routines for socket operations

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CTCPSOCKET_H
