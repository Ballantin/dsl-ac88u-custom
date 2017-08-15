//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CTCPSERVERSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTCPSERVERSOCKET_H
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

#ifndef MXG_ISERVERSOCKET_H
#include "Network/IServerSocket.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CTcpSocket;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================
// BRCM_CUSTOM - [add] Callback routines for socket operations
typedef void (*CTCPSERVERSOCKET_BIND)( mxt_hSocket socket, const CSocketAddr *pLocalAddr );
typedef void (*CTCPSERVERSOCKET_ACCEPT)( mxt_hSocket socket, const CSocketAddr *pPeerAddr );
// BRCM_CUSTOM - [end] Callback routines for socket operations

// BRCM_CUSTOM - [begin] Add TCP socket binding support.
#if CCCFG_TCP_SOCK_BIND_LOCAL_ADDRESS_SUPPORT
extern char g_szTCPSERVERSOCKET_LOCAL_INTERFACE[32];
#endif /* CCCFG_TCP_SOCK_BIND_LOCAL_ADDRESS_SUPPORT */
// BRCM_CUSTOM - [end] Add TCP socket binding support.

//==SDOC========================================================================
//== Class: CTcpServerSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
// Summary:
//   Implements a TCP socket in servermode.
//
// Description:
//   The CTcpServerSocket class is an implementation of a TCP socket in
//   server mode which supports the IAServerSocket interface.
//
// Location:
//   Network/CTcpServerSocket.h
//
//==EDOC========================================================================
class CTcpServerSocket : protected CSocket,
                         public IServerSocket
{
//-- Published Interface
//------------------------
public:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CTcpServerSocket();

    //-- << Stereotype >>
    //--------------------

    // Summary:
    //  Accepts the connection request from a client socket.
    //-------------------------------------------------------
    mxt_result Accept(OUT GO CTcpSocket** ppTcpSocket);

    // Summary:
    //  Creates the socket.
    //----------------------
    mxt_result Create(IN CSocketAddr::EAddressFamily eAddressFamily);

    //==========================================================================
    //== Methods inherited from CSocket.
    //==========================================================================

    mxt_hSocket GetHandle() const;
    mxt_result Set8021QUserPriority(IN bool bEnable,
                                    IN uint8_t uUserPriority);
    mxt_result SetBlocking(IN bool bEnable);
    mxt_result SetReuseAddress(IN bool bEnable);
    mxt_result SetTos(IN uint8_t uTos);
    mxt_result SetKeepAlive(IN bool bEnable);
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    mxt_result SetIpv6UnicastHops(IN int nHops);
#endif
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);
    mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);
#endif
    mxt_result SetBackgroundVoipSocket(IN bool bEnable);

    //==========================================================================
    //== Interface IServerSocket methods
    //==========================================================================

    virtual mxt_result Accept(OUT GO IIoSocket** ppIoSocket);

    virtual mxt_result Bind(IN const CSocketAddr* pLocalAddress,
                            OUT CSocketAddr* pEffectiveLocalAddress);

    virtual mxt_result Listen(IN unsigned int uMaxPendingConnection = 5);

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
    virtual ~CTcpServerSocket();

    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CTcpServerSocket(IN const CTcpServerSocket& rSrc);

    void Reset();

//-- Hidden Data Members
//------------------------
protected:
    // The socket state.
    bool        m_bCreated;
    bool        m_bBound;

    // Set options flags.
    bool        m_bBlockingSet;
    bool        m_bKeepAliveSet;
    bool        m_b8021QUserPrioritySet;
    bool        m_bTosSet;
    bool        m_bBackgroundVoipSocketSet;
//M5T_INTERNAL_USE_END
};

inline
mxt_hSocket CTcpServerSocket::GetHandle() const
{
    return CSocket::GetHandle();
}

inline
mxt_result CTcpServerSocket::Set8021QUserPriority(IN bool bEnable,
                                                  IN uint8_t unUserPriority)
{
    mxt_result res = CSocket::Set8021QUserPriority(bEnable, unUserPriority);
    if (MX_RIS_S(res))
    {
        m_b8021QUserPrioritySet = true;
    }
    return res;
}

inline
mxt_result CTcpServerSocket::SetBlocking(IN bool bEnable)
{
    mxt_result res = CSocket::SetBlocking(bEnable);
    if (MX_RIS_S(res))
    {
        m_bBlockingSet = true;
    }
    return res;
}

inline
mxt_result CTcpServerSocket::SetReuseAddress(IN bool bEnable)
{
    return CSocket::SetReuseAddress(bEnable);
}

inline
mxt_result CTcpServerSocket::SetTos(IN uint8_t unTos)
{
    mxt_result res = CSocket::SetTos(unTos);
    if (MX_RIS_S(res))
    {
        m_bTosSet = true;
    }
    return res;
}

inline
mxt_result CTcpServerSocket::SetKeepAlive(IN bool bEnable)
{
    mxt_result res = CSocket::SetKeepAlive(bEnable);
    if (MX_RIS_S(res))
    {
        m_bKeepAliveSet = true;
    }
    return res;
}


#if defined(MXD_IPV6_ENABLE_SUPPORT)
inline
mxt_result CTcpServerSocket::SetIpv6UnicastHops(IN int nHops)
{
    return CSocket::SetIpv6UnicastHops(nHops);
}
#endif // #if defined(MXD_IPV6_ENABLE_SUPPORT)

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
inline
mxt_result CTcpServerSocket::SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec)
{
    return CSocket::SetWindowsReceivingFlowspec(pReceivingFlowspec);
}

inline
mxt_result CTcpServerSocket::SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec)
{
    return CSocket::SetWindowsSendingFlowspec(pSendingFlowspec);
}
#endif

inline
mxt_result CTcpServerSocket::SetBackgroundVoipSocket(IN bool bEnable)
{
    mxt_result res = resS_OK;
    if (m_bBound)
    {
        res = CSocket::SetBackgroundVoipSocket(bEnable);
    }
    else
    {
        // Will be set once the socket is bound.
        m_bOptBackgroundVoipSocketEnabled = bEnable;
    }

    if (MX_RIS_S(res))
    {
        m_bBackgroundVoipSocketSet = true;
    }

    return res;
}

// BRCM_CUSTOM - [add] Callback routines for socket operations
//==============================================================================
//====  EXPORTS  ===============================================================
//==============================================================================
extern CTCPSERVERSOCKET_BIND      g_CTcpServerSocket_BindCB;
extern CTCPSERVERSOCKET_ACCEPT    g_CTcpServerSocket_AcceptCB;
// BRCM_CUSTOM - [end] Callback routines for socket operations

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_SYMBIAN) &&
       //      defined(MXD_NETWORK_ENABLE_SUPPORT)


#endif // MXG_CTCPSERVERSOCKET_H
