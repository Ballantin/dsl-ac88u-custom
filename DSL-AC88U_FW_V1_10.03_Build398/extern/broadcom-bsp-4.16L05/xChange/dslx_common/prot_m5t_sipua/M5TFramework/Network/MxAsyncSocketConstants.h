//==============================================================================
//==============================================================================
//
//              Copyright(c) 2007 Media5 Corporation ("Media5")
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
#ifndef MXG_MXASYNCSOCKETCONSTANTS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXASYNCSOCKETCONSTANTS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_NETWORK_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN

// This file defines messages constants for all methods of all ECOM interfaces
// related to Asynchronous sockets. Sockets class should use this file instead
// of redefining each of these message constants.
//
// A class requiring to define a new message can use the
// uMSG_PRIVATE_MSG_FIRST_VALUE + 0 value to start its private messages
// numbering in sequence with the shared constants.

// IAsyncSocket
static const unsigned int uMSG_CLOSE_A = 0;
static const unsigned int uMSG_GET_HANDLE = 1;
static const unsigned int uMSG_GET_LOCAL_ADDRESS = 2;
static const unsigned int uMSG_GET_OPAQUE = 3;
static const unsigned int uMSG_GET_SOCKET_TYPE = 4;
static const unsigned int uMSG_SET_ASYNC_SOCKET_MGR = 5;
static const unsigned int uMSG_SET_OPAQUE = 6;
static const unsigned int uMSG_SET_SOCKET_TYPE = 7;
static const unsigned int uMSG_ERASE_ALL_USER_INFO = 8;
static const unsigned int uMSG_ERASE_USER_INFO = 9;
static const unsigned int uMSG_GET_USER_INFO = 10;
static const unsigned int uMSG_INSERT_USER_INFO = 11;

// IAsyncIoSocket
static const unsigned int uMSG_GET_PEER_ADDRESS = 12;
static const unsigned int uMSG_RECV = 13;
static const unsigned int uMSG_SEND = 14;
static const unsigned int uMSG_SET_ASYNC_IO_SOCKET_MGR = 15;

// IAsyncUnconnectedIoSocket.
static const unsigned int uMSG_RECV_FROM = 16;
static const unsigned int uMSG_SEND_TO = 17;
static const unsigned int uMSG_SET_ASYNC_UNCONNECTED_IO_SOCKET_MGR = 18;

// IAsyncClientSocket
static const unsigned int uMSG_BIND_A = 19;
static const unsigned int uMSG_CONNECT_A = 20;
static const unsigned int uMSG_SET_ASYNC_CLIENT_SOCKET_MGR = 21;

// IAsyncServerSocket
static const unsigned int uMSG_ACCEPT_A = 22;
static const unsigned int uMSG_LISTEN = 23;
static const unsigned int uMSG_SET_ASYNC_SERVER_SOCKET_MGR = 24;

// IAsyncSocketBufferSizeOptions.
static const unsigned int uMSG_SET_RECEIVE_BUFFER_SIZE = 25;
static const unsigned int uMSG_SET_TRANSMIT_BUFFER_SIZE = 26;

// IAsyncSocketQualityOfServiceOptions.
static const unsigned int uMSG_SET_8021Q_USER_PRIORITY = 27;
static const unsigned int uMSG_SET_TOS = 28;

// IAsyncSocketWindowsGqosOptions
static const unsigned int uMSG_SET_WINDOWS_QOS_RECEIVING_FLOWSPEC = 29;
static const unsigned int uMSG_SET_WINDOWS_QOS_SENDING_FLOWSPEC = 30;

// IAsyncSocketUdpOptions.
static const unsigned int uMSG_SET_BROADCAST = 31;

// IAsyncSocketTcpOptions.
static const unsigned int uMSG_SET_KEEP_ALIVE = 32;
static const unsigned int uMSG_SET_NAGLE = 33;
static const unsigned int uMSG_SET_CONNECT_TIMEOUT  = 34;
static const unsigned int uMSG_SET_BACKGROUNDVOIPSOCKET  = 35;

// Asynchronous event notifications
static const unsigned int uMSG_NOTIFY_READY_TO_SEND_A = 36;
static const unsigned int uMSG_NOTIFY_READY_TO_RECV_A = 37;
static const unsigned int uMSG_NOTIFY_UNCONNECTED_READY_TO_RECV_A = 38;
static const unsigned int uMSG_NOTIFY_CLOSED_BY_PEER_A = 39;
static const unsigned int uMSG_NOTIFY_ERROR_DETECTED_A = 40;

// Added as a fix to speed up CAsyncUdpSocket::ConnectA.  This is related to
// IAsyncClientSocket.
static const unsigned int uMSG_CONNECT_A_JUST_NOTIFY_A = 41;

// This constants allows a socket to define its own messages with numbers that
// follows the last number defined in this file.
static const unsigned int uMSG_PRIVATE_MSG_FIRST_VALUE = uMSG_CONNECT_A_JUST_NOTIFY_A + 1;

// Timers.
static const unsigned int uTIMER_CONNECT_TIMEOUT = 0;

// The following constants are the default socket types supported by the framework.
extern const char gs_szUDP[4];
extern const char gs_szTCP[4];
extern const char gs_szOPTION_CLIENT[9];
extern const char gs_szOPTION_ACCEPTED[11];
extern const char gs_szOPTION_SERVER[9];

static const char* const gs_apszSOCKET_TYPE_UDP[] = { gs_szUDP };
static const char* const gs_apszSOCKET_TYPE_TCP_CLIENT[] = { "TCP, m=client" };
static const char* const gs_apszSOCKET_TYPE_TCP_ACCEPTED[] = { "TCP, m=accepted" };
static const char* const gs_apszSOCKET_TYPE_TCP_SERVER[] = { "TCP, m=server" };

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //--     defined(MXD_NETWORK_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_MXASYNCSOCKETCONSTANTS_H
