//==SDOC========================================================================
//==============================================================================
//
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


#ifndef MXG_CSNTPCLIENT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSNTPCLIENT_H
//M5T_INTERNAL_USE_END
//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif //#ifndef MXG_MXCONFIG_H

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SNTP_CLIENT_ENABLE_SUPPORT
#endif

#if defined(MXD_SNTP_CLIENT_ENABLE_SUPPORT)
// Data Member
//-----------------------------------------
#ifndef MXG_CTHREAD_H
#include "Kernel/CThread.h"
#endif // #ifndef MXG_CTHREAD_H
#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif // MXG_CSEMAPHORE_H
#ifndef MXG_CFQDN_H
#include "Network/CFqdn.h"
#endif // MXG_CFQDN_H
#ifndef MXG_NETWORKTOOLS_H
#include "Network/NetworkTools.h"
#endif // MXG_NETWORKTOOLS_H
#ifndef MXG_CUDPSOCKET_H
#include "Network/CUdpSocket.h"
#endif // #ifndef MXG_CUDPSOCKET_H
#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif // #ifndef MXG_CEVENTDRIVEN_H

// Interface Realized & Parent
//--------------------------------------
#ifndef MXG_IEXTERNALTIMESOURCE_H
#include "Time/IExternalTimeSource.h"
#endif // #ifndef MXG_IEXTERNALTIMESOURCE_H

//===========================================================================
//====  MEDIATRIX NAME SPACE  ===============================================
//===========================================================================

MX_NAMESPACE_START(MXD_GNS)

// Forward Declarations Inside of the Namespace
//-----------------------------------------------
class CBlob;
class CTime;

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================
//M5T_INTERNAL_USE_BEGIN
const uint32_t uSNTP_INTER_PACKET_WAIT_MS = 1000;
const uint16_t uSNTP_PACKET_SENT_TO_SERVER = 2;
const uint16_t uSNTP_PORT = 123;
const uint32_t uSNTP_TIMEOUT_MS = 15000;
const uint64_t uUSE_STATIC_CONFIGURATION = MX_MAKEUINT64(0xFFFFFFFF, 0xFFFFFFFF);

// SNTP poll interval, 6^2 = 64 seconds.
const uint8_t  uSNTP_POLLING_INTERVAL = 6;

#define SNTP_LI_NO_WARNING 0x00
#define SNTP_LI_LAST_MINUTE_HAS_61_SECONDS 0x40
#define SNTP_LI_LAST_MINUTE_HAS_59_SECONDS 0x80
#define SNTP_LI_ALARM_CONDITION 0xC0
#define SNTP_LI_MASK 0xC0

#define SNTP_VN_VERSION_2 0x10
#define SNTP_VN_VERSION_3 0x18
#define SNTP_VN_VERSION_4 0x20
#define SNTP_VN_MASK 0x38

#define SNTP_M_RESERVED 0x00
#define SNTP_M_SYMETRIC_ACTIVE 0x01
#define SNTP_M_SYMETRIC_PASSIVE 0x02
#define SNTP_M_CLIENT 0x03
#define SNTP_M_SERVER 0x04
#define SNTP_M_RESERVED_FOR_NTP_CONTROL_MESSAGE 0x05
#define SNTP_M_RESERVED_FOR_PRIVATE_USE 0x06
#define SNTP_M_MASK 0x07
//M5T_INTERNAL_USE_END
//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================

//==SDOC=====================================================================
//==
//==  Class: CSntpClient
//==
//===========================================================================
//<GROUP TIME_CLASSES>
//
//  Summary:
//      Implements a SNTP client to connect to a SNTP or NTP server.
//
//  Description:
//      CSntpClient implements a SNTP client that connects to a SNTP or NTP
//      server and gets the new time. It does not keep the time, it only
//      retrieves the time from the server. CSntpClient follows RFC 1769.
//
//      There are a number of settings that can statically change.
//
//      The global timeout, named only timeout, represents the maximum time
//      that the client is allowed to wait for an answer when it executes
//      a query.
//
//      The SntpPacketSentToServer is the number of packets that the client
//      sends to the server. This might prove useful if there are a lot of
//      packets lost on the network.
//
//==EDOC=====================================================================
class CSntpClient : public CEventDriven,
                    public IExternalTimeSource
{
private:
//M5T_INTERNAL_USE_BEGIN
    #include "Basic/MxPack.h"

    struct SSntpTimestamp
    {
        uint32_t m_uInteger MX_PACK_MEMBER;
        uint32_t m_uFraction MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    struct SSntpBasicInfo
    {
        uint8_t  m_uLIVNM MX_PACK_MEMBER; // Leap Indicator, Version Number, Mode
        uint8_t  m_uStratum MX_PACK_MEMBER;
        uint8_t  m_uPoolInterval MX_PACK_MEMBER;
        uint8_t  m_uPrecision MX_PACK_MEMBER;
        uint32_t m_uRootDelay MX_PACK_MEMBER;
        uint32_t m_uRootDispersion MX_PACK_MEMBER;
        uint32_t m_uReferenceIdentifier MX_PACK_MEMBER;
        SSntpTimestamp m_stReferenceTimestamp MX_PACK_MEMBER;
        SSntpTimestamp m_stOriginateTimestamp MX_PACK_MEMBER;
        SSntpTimestamp m_stReceiveTimestamp MX_PACK_MEMBER;
        SSntpTimestamp m_stTransmitTimestamp MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    struct SSntpAuthenticationInfo
    {
        uint32_t m_uKedIdentifier MX_PACK_MEMBER;
        uint8_t  m_auMessageDigest[16] MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    struct SSntpPacket
    {
        SSntpBasicInfo m_stSntpBasicInfo MX_PACK_MEMBER;
        SSntpAuthenticationInfo m_stSntpAuthenticationInfo MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    #include "Basic/MxPack.h"
//M5T_INTERNAL_USE_END

// Enumeration
//-------------
public:

// Published Interface
//-------------------------
public:
    // << Constructors / Destructors >>
    //---------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSntpClient();

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CSntpClient();

    // Summary:
    //  Activates the thread mechanism of the SNTP client.
    //-----------------------------------------------------
    mxt_result Activate(IN uint32_t uStackSize = 0,
                        IN CThread::EPriority ePriority = CThread::eNORMAL);
    // Summary:
    //  Terminates the thread mechanism of the SNTP client.
    //------------------------------------------------------
    void Terminate();

    //-----------------------------
    //  Accessor methods
    //-----------------------------

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(OUT CTime& rTime,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(OUT uint32_t& ruJulianDateDay,
                           OUT uint32_t& ruJulianTimeMs,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(OUT uint16_t& ruYear,
                           OUT uint16_t& ruMonth,
                           OUT uint16_t& ruDay,
                           OUT uint16_t& ruHour,
                           OUT uint16_t& ruMinute,
                           OUT uint16_t& ruSecond,
                           OUT uint16_t& ruMs,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(IN const CFqdn& rServer,
                           OUT CTime& rTime,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(IN const CFqdn& rServer,
                           OUT uint32_t& ruJulianDateDay,
                           OUT uint32_t& ruJulianTimeMs,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Requests the current date and time from the registered servers.
    //------------------------------------------------------------------
    mxt_result QueryServer(IN const CFqdn& rServer,
                           OUT uint16_t& ruYear,
                           OUT uint16_t& ruMonth,
                           OUT uint16_t& ruDay,
                           OUT uint16_t& ruHour,
                           OUT uint16_t& ruMinute,
                           OUT uint16_t& ruSecond,
                           OUT uint16_t& ruMs,
                           IN uint64_t uTimeoutMs = uUSE_STATIC_CONFIGURATION);

    // Summary:
    //  Clears the SNTP servers list.
    //--------------------------------
    void ClearServerList();

    // Summary:
    //  Inserts a new server into the servers list.
    //----------------------------------------------
    void InsertServer(IN const CFqdn& rServer);

    // Summary:
    //  Sets the default connection timeout.
    //---------------------------------------
    void SetTimeoutMs(IN uint64_t uTimeoutMs);

    // Summary:
    //  Sets the number of packets sent to a server at each request.
    //---------------------------------------------------------------
    void SetSntpPacketSentToServer(IN uint16_t uSntpPacketSentToServer);

    // IExternalTimeSource
    //-----------------------------------------------------------------------

    virtual mxt_result GetTimeS(OUT CTime& rTime);

//M5T_INTERNAL_USE_BEGIN
// Internal Methods Definition
//-----------------------------
protected:

    mxt_result ProtectedQueryServer(IN const CFqdn& rServer,
                                    OUT CTime& rTime,
                                    IN uint64_t uTimeoutMs);

    mxt_result ProtectedQueryServer(IN const CFqdn& rServer,
                                    OUT uint32_t& ruJulianDateDay,
                                    OUT uint32_t& ruJulianTimeMs,
                                    IN uint64_t uTimeoutMs);

    mxt_result ProtectedQueryServer(IN const CFqdn& rServer,
                                    OUT uint16_t& ruYear,
                                    OUT uint16_t& ruMonth,
                                    OUT uint16_t& ruDay,
                                    OUT uint16_t& ruHour,
                                    OUT uint16_t& ruMinute,
                                    OUT uint16_t& ruSecond,
                                    OUT uint16_t& ruMs,
                                    IN uint64_t uTimeoutMs);

    bool ParsePacket(IN const CBlob* pPacket,
                     OUT uint32_t& ruJulianDateDay,
                     OUT uint32_t& ruJulianTimeMs);

    // Inherited from CEventDriven.
    virtual void ReleaseInstance();
    virtual void EvSocketServiceMgrAwaken(IN mxt_hSocket hSocket,
                                          IN unsigned int uEvents,
                                          IN mxt_opaque opq);

private:

    // Deactivated Constructors / Destructors / Operators
    //----------------------------------------------------
    CSntpClient(IN const CSntpClient& rFrom);
    CSntpClient operator=(IN const CSntpClient& rFrom);

// Private Data Members
//---------------------
private:

    CBinarySemaphore m_semTermination;
    CUdpSocket* m_pSocket;
    CBlob* m_pSocketReceptionBlob;

    CFqdn* m_pServer;

    uint64_t m_uTimeoutMs;
    uint16_t m_uSntpPacketSentToServer;

    CBinarySemaphore m_semInternalAccessProtection;
    CBinarySemaphore m_semQueryServerExclusivity;
    CBinarySemaphore m_semWaitSntpPacket;

    uint32_t m_uSntpQueryServerId;
//M5T_INTERNAL_USE_BEGIN
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

#endif // MXD_SNTP_CLIENT_ENABLE_SUPPORT
#endif // MXG_CSNTPCLIENT_H

