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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_MXTLSSOCKETMESSAGES_H
#define MXG_MXTLSSOCKETMESSAGES_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

// This will pull-in message IDs that are shared with the asynchronous sockets
// defined in the Network's folder. The TLS folder is already dependant on the
// Network folder, thus this is OK.
#ifndef MXG_MXASYNCSOCKETCONSTANTS_H
#include "Network/MxAsyncSocketConstants.h"
#endif

#if defined(MXD_ECOM_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    !defined (MXD_TLS_NONE)

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// This file defines various constants used throughout all asynchronous TLS
// sockets. These constants represent thresholds, timeouts, timer IDs and
// message ids.
//
// A class requiring to define a new message can use the
// uMSG_TLS_PRIVATE_MSG_FIRST_VALUE + 0 value to start its private messages
// numbering in sequence with the shared message constants.

// Various thresholds and timeouts used in TLS sockets.
static const unsigned int uDEFAULT_THRESHOLD_IN_BYTE = 65536;             // 2^16.
static const unsigned int uDEFAULT_THRESHOLD_IN_TIME_MS = 30000;          // 30 seconds.

static const unsigned int uDEFAULT_THRESHOLD_TIMEOUT_MS = 60000;          // 1 minute.

static const unsigned int uTIMER_TRACE_STATE_MS = 1000;                   // 1 second.
static const unsigned int uTIMER_WAITING_FOR_SSL_ACCEPT_MS = 120000;      // 2 minutes.
static const unsigned int uTIMER_WAITING_FOR_SSL_CONNECT_MS = 120000;     // 2 minutes.
static const unsigned int uTIMER_WAITING_FOR_SSL_SHUTDOWN_MS = 120000;    // 2 minutes.
static const unsigned int uTIMER_WAITING_FOR_NEXT_SSL_SHUTDOWN_MS = 1000; // 1 second.

// TLS Socket's internal states names.
// It is defined in CAsyncTlsSocketBase.cpp.
extern const char* const g_aszSTATE_NAMES[];

// Initial receive and send buffer capacity.
static const unsigned int gs_uINITIAL_BUFFER_CAPACITY_IN_BYTES = 1516;

// Timers
#if defined(MXD_TLS_TRACE_STATE)
static const unsigned int uTIMER_TRACE_STATE = 0;
#endif

const unsigned int uTIMER_AUTO_RENEGOTIATION_THRESHOLD_IN_TIME = 1;
const unsigned int uTIMER_WAITING_FOR_SSL_ACCEPT = 2;
const unsigned int uTIMER_WAITING_FOR_SSL_CONNECT = 3;
const unsigned int uTIMER_WAITING_FOR_SSL_RENEGOTIATE = 4;
const unsigned int uTIMER_WAITING_FOR_SSL_SHUTDOWN = 5;

// Messages

// IAsyncTlsSocket
static const unsigned int uMSG_GET_TLS_CONTEXT = uMSG_PRIVATE_MSG_FIRST_VALUE + 0;
static const unsigned int uMSG_SET_TLS_CONTEXT = uMSG_PRIVATE_MSG_FIRST_VALUE + 1;
static const unsigned int uMSG_SET_TLS_SESSION = uMSG_PRIVATE_MSG_FIRST_VALUE + 2;
static const unsigned int uMSG_SET_ASYNC_TLS_SOCKET_MGR = uMSG_PRIVATE_MSG_FIRST_VALUE + 3;
static const unsigned int uMSG_TLS_HANDSHAKING_APPROVAL_COMPLETED_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 4;

// IAsyncTlsRenegotiation
static const unsigned int uMSG_RENEGOTIATE_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 5;
static const unsigned int uMSG_SET_AUTO_RENEGOTIATION_THRESHOLD_IN_BYTE = uMSG_PRIVATE_MSG_FIRST_VALUE + 6;
static const unsigned int uMSG_SET_AUTO_RENEGOTIATION_THRESHOLD_IN_TIME_MS = uMSG_PRIVATE_MSG_FIRST_VALUE + 7;
static const unsigned int uMSG_SET_AUTO_RENEGOTIATION_TIMEOUT_MS = uMSG_PRIVATE_MSG_FIRST_VALUE + 8;
static const unsigned int uMSG_SET_ASYNC_TLS_RENEGOTIATION_MGR = uMSG_PRIVATE_MSG_FIRST_VALUE + 9;
static const unsigned int uMSG_SET_RENEGOTIATION_NOTIFICATION = uMSG_PRIVATE_MSG_FIRST_VALUE + 10;

// IAsyncTlsServerSocket
static const unsigned int uMSG_GET_ACCEPTED_TLS_CONTEXT = uMSG_PRIVATE_MSG_FIRST_VALUE + 11;
static const unsigned int uMSG_SET_ACCEPTED_TLS_CONTEXT = uMSG_PRIVATE_MSG_FIRST_VALUE + 12;

// Internal helpers
static const unsigned int uMSG_RECEIVING_TLS_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 13;
static const unsigned int uMSG_SENDING_TLS_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 14;
static const unsigned int uMSG_RENEGOTIATING_TLS_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 15;
static const unsigned int uMSG_CLOSING_TLS_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 16;

// IPrivateAsyncTlsSocket
static const unsigned int uMSG_SET_TCP_SOCKET = uMSG_PRIVATE_MSG_FIRST_VALUE + 17;
static const unsigned int uMSG_PERFORM_TLS_HANDSHAKING_A = uMSG_PRIVATE_MSG_FIRST_VALUE + 18;

// IAsyncTlsOpenSsl
static const unsigned int uMSG_GET_SSL = uMSG_PRIVATE_MSG_FIRST_VALUE + 19;

// Last message number
static const unsigned int uMSG_LAST = uMSG_GET_SSL;

// This constants allows a socket to define its own messages with numbers that
// follows the last number defined in this file.
static const unsigned int uMSG_TLS_PRIVATE_MSG_FIRST_VALUE = uMSG_LAST + 1;

// The following constants are the default socket types supported by the framework.
extern const char gs_szTLS[4];

static const char* const gs_apszSOCKET_TYPE_TLS_CLIENT[] = { "TLS, m=client", "TCP, m=client" };
static const char* const gs_apszSOCKET_TYPE_TLS_ACCEPTED[] = { "TLS, m=accepted", "TCP, m=accepted" };
static const char* const gs_apszSOCKET_TYPE_TLS_SERVER[] = { "TLS, m=server", "TCP, m=server" };

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #if defined(MXD_ECOM_ENABLE_SUPPORT) &&
       //--     defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && 
       //--     !defined (MXD_TLS_NONE)

#endif //-- #ifndef MXG_MXTLSSOCKETMESSAGES_H
//M5T_INTERNAL_USE_END

