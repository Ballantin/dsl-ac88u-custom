//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CSOCKET_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSOCKET_H
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
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

//-- Data Members
//-----------------
#ifndef MXG_SOCKETDEFINES_H
#include "Network/SocketDefines.h"
#endif


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_ISOCKET_H
#include "Network/ISocket.h"
#endif

#ifndef MXG_CSOCKETADDR_H
#include "Network/CSocketAddr.h"
#endif

#if defined(MXD_OS_DARWIN)
#include "CoreFoundation/CFSocket.h"
#include "CoreFoundation/CFStream.h"
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

//==SDOC========================================================================
//== Class: CSocket
//========================================
//<GROUP NETWORK_CLASSES>
//
//  Summary:
//    Base socket class.
//
// Description:
//   This is the base socket class. It implements sockets options and
//   Windows Qos mechanism.
//
// Location:
//   Network/CSocket.h
//
//==EDOC========================================================================
class CSocket
{
//M5T_INTERNAL_USE_BEGIN
    // CAsyncTlsSocket must be able to call ApplyFlowspecs.
    friend class CAsyncTlsSocket;

// BRCM_CUSTOM - [begin] Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
    // CTcpServerSocket must be able to access m_socket.
    friend class CTcpServerSocket;
#endif
// BRCM_CUSTOM - [end] Add ECOS support.
//M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
//M5T_INTERNAL_USE_BEGIN
    static bool InitializeSockets(IN const char* pszInitString = NULL);
    static void FinalizeSockets();

// Friends declarations
//----------------------
    friend mxt_result InitializeCSocket();
    friend void FinalizeCSocket();
//M5T_INTERNAL_USE_END

protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    CSocket(IN unsigned int uIpProto);

    // Summary:
    //  Destructor.
    //----------------------
    virtual ~CSocket();

    // Summary:
    //  Retrieve the socket handle.
    //------------------------------
    mxt_hSocket GetHandle() const;

    //  Summary:
    //    Binds the socket to a local address.
    mxt_result Bind(IN const CSocketAddr* pLocalAddress,
                    OUT CSocketAddr* pEffectiveLocalAddress);

    // Summary:
    //  Set 8021Q user priority option.
    //----------------------------------
    mxt_result Set8021QUserPriority(IN bool bEnable,
                                    IN uint8_t uUserPriority);

    // Summary:
    //  Set allow any source option.
    //----------------------------------
    mxt_result SetAllowAnySource(IN bool bEnable);

    // Summary:
    //  Set socket operation to blocking/non-blocking
    //------------------------------------------------
    mxt_result SetBlocking(IN bool bEnable);

    // Summary:
    //  Set socket broadcast option
    //-------------------------------
    mxt_result SetBroadcast(IN bool bEnable);

    // Summary:
    //   Enable/disable the keep-alive option.
    //-----------------------------------------
    mxt_result SetKeepAlive(IN bool bEnable);

    // Summary:
    //  Enable/disable the Nagle algorithm.
    //----------------------------------------------------
    mxt_result SetNagle(IN bool bEnable);

    // Summary:
    //  Set socket receive buffer size option
    //------------------------------------------------
    mxt_result SetReceiveBufferSize(IN unsigned int uSize);

    // Summary:
    //  Enable/disable the possibility to reuse a local address.
    //----------------------------------------------------------
    mxt_result SetReuseAddress(IN bool bEnable);

    // Summary:
    //  Set socket type of service ( TOS byte ) option
    //--------------------------------------------------
    mxt_result SetTos(IN uint8_t uTos);

    // Summary:
    //  Set socket transmit buffer size option
    //------------------------------------------------
    mxt_result SetTransmitBufferSize(IN unsigned int uSize);

    // Summary:
    //  Enable / Disable computation of UDP checksum
    //------------------------------------------------
    mxt_result SetUdpChecksum(IN bool bEnable);

#if defined(MXD_IPV6_ENABLE_SUPPORT)
    // Summary:
    //  Sets the IPv6 outgoing unicast hop limit.
    //--------------------------------------------
    mxt_result SetIpv6UnicastHops(IN int nHops);
#endif

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    // Summary:
    //  Sets the receiving flowspec in windows only.
    //----------------------------------------------
    mxt_result SetWindowsReceivingFlowspec(IN FLOWSPEC* pReceivingFlowspec);

    // Summary:
    //  Sets the sending flowspec in windows only.
    //----------------------------------------------
    mxt_result SetWindowsSendingFlowspec(IN FLOWSPEC* pSendingFlowspec);

    // Summary:
    //  Applies the flowspecs to the socket.
    //---------------------------------------
    mxt_result ApplyFlowspecs(CSocketAddr* pPeerAddress = NULL);
#endif

    // Summary:
    //  Sets the background VOIP socket support.
    mxt_result SetBackgroundVoipSocket(IN bool bEnable);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
protected:
    void Reset();
private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    CSocket(IN const CSocket& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    CSocket& operator=(IN const CSocket& rSrc);

    // Summary:
    //  Comparison Operator.
    //-----------------------
    CSocket& operator==(IN const CSocket& rSrc);

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_TC_QOS)
    static unsigned int AppendObjectTrafficClass(IN unsigned char* pucDestination,
                                                 IN unsigned int uOffset,
                                                 IN uint8_t uPriority);

    static unsigned int AppendObjectEndOfList(IN unsigned char* pucDestination,
                                              IN unsigned int uOffset);

    static unsigned int AppendObjectDsClass(IN unsigned char* pucDestination,
                                            IN unsigned int uOffset,
                                            IN uint8_t uDSCP);

    static void CALLBACK ClNotifyHandler(HANDLE ClRegCtx,
                                         HANDLE ClIfcCtx,
                                         ULONG Event,
                                         HANDLE SubCode,
                                         ULONG BufSize,
                                         PVOID Buffer );
    mxt_result UpdateQoSSettings();
#endif

//-- Hidden Data Members
//------------------------
protected:
    // The socket handle used to issue socket calls.
    mxt_hSocket     m_hSocket;
// BRCM_CUSTOM - [begin] Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
    BOS_SOCKET      m_socket;
#endif
// BRCM_CUSTOM - [end] Add ECOS support.

    // Current state of socket options.
    bool            m_b8021QEnable;
    uint8_t         m_u8021QUserPriority;
    bool            m_bBroadcastEnable; //UDP Only
    bool            m_bOptAllowAnySourceEnable;
    bool            m_bOptBlockingEnable;
    bool            m_bOptKeepAliveEnable; // TCP only
    bool            m_bOptNagleEnable; //TCP only
    bool            m_bOptReuseAddressEnable; //TCP only
    bool            m_bOptBackgroundVoipSocketEnabled; //TCP only
    bool            m_bOptUdpChecksumEnable; //UDP Only
    unsigned int    m_uReceiveBufferSize;
    uint8_t         m_uTos;
    unsigned int    m_uTransmitBufferSize;
#if defined(MXD_IPV6_ENABLE_SUPPORT)
    int             m_nOptIpv6UnicastHops;
#endif

    ISocket::EProtocolFamily m_eProtocolFamily;

#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
    QOS m_stQos;
    bool m_bFlowspecChanged;
#endif

private:
    unsigned int m_uIpProto;
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_TC_QOS)
    static HANDLE ms_hIfcHandle;
    static HANDLE ms_hDefaultFilterHandle;
    static HANDLE ms_hDefaultFlowHandle;
    HANDLE m_hSocketFilterHandle;
    HANDLE m_hSocketFlowHandle;
#endif

#if defined(MXD_OS_DARWIN)
    CFSocketRef m_NativeSocket;
    CFReadStreamRef m_ReadStream;
    CFWriteStreamRef m_WriteStream;
#endif

//M5T_INTERNAL_USE_END
};

inline
CSocket::CSocket(IN unsigned int uIpProto)
// BRCM_CUSTOM - [mod] Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
: m_hSocket(NULL),
#else
: m_hSocket(nINVALID_SOCKET),
#endif
// BRCM_CUSTOM - [end] Add ECOS support.
  m_uIpProto(uIpProto)
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_TC_QOS)
, m_hSocketFilterHandle(0),
  m_hSocketFlowHandle(0)
#endif
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
, m_bFlowspecChanged(false)
#endif
#if defined(MXD_OS_DARWIN)
, m_NativeSocket(NULL),
  m_ReadStream(NULL),
  m_WriteStream(NULL)
#endif
{
    #if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_GQOS_QOS)
        memset ( &m_stQos, QOS_NOT_SPECIFIED, sizeof(QOS) );
        m_stQos.ReceivingFlowspec.ServiceType = SERVICETYPE_NOTRAFFIC;
        m_stQos.SendingFlowspec.ServiceType = SERVICETYPE_NOTRAFFIC;
        m_stQos.ProviderSpecific.len = 0;
        m_stQos.ProviderSpecific.buf = NULL;
    #endif
}

inline
CSocket::~CSocket()
{
    Reset();
}

//==SDOC========================================================================
//==
//==  GetHandle
//==
//==============================================================================
//
//  Returns:
//      The socket's handle.
//
//  Description:
//      This method is used to retrieve the socket handle.
//
//==EDOC========================================================================
inline
mxt_hSocket CSocket::GetHandle() const
{
    return m_hSocket;
}

//==========================================================================
//==
//==  Bind
//==
//==========================================================================
//
//  Parameters:
//    pLocalAddress:
//      Contains the local address where to bind.  May be NULL if the user
//      does not care about the interface or the local port. If the
//      parameter is not NULL and its port is set to 0, the automatic port
//      allocation feature will also be used.
//
//     pEffectiveLocalAddress:
//      Upon return, contains the address to which the socket has effectively
//      been bound. It may be NULL if the effective binding is not important
//      to the caller.
//
//  Returns:
//    - resS_OK: Socket has been bound.
//    - Failure codes: Socket has not been bound.
//
//  Description:
//    Used to associate a local address to the socket.
//
//==========================================================================
inline
mxt_result CSocket::Bind(IN const CSocketAddr* /*pLocalAddress*/,
                     OUT CSocketAddr* /*pEffectiveLocalAddress*/)
{
    mxt_result res = resS_OK;
#if defined(MXD_OS_WINDOWS) && defined(MXD_OS_WINDOWS_ENABLE_TC_QOS)
    res = UpdateQoSSettings();
#endif
    return res;
}
MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_CSOCKET_H
