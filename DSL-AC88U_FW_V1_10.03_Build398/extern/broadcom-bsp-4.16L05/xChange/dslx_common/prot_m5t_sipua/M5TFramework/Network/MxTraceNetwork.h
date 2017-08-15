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
#ifndef MXG_MXTRACENETWORK_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXTRACENETWORK_H
//M5T_INTERNAL_USE_END

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)

// Network package nodes
extern STraceNode g_stFrameworkNetwork;
extern STraceNode g_stFrameworkNetworkCAsyncSocketFactory;
extern STraceNode g_stFrameworkNetworkCAsyncTcpServerSocket;
extern STraceNode g_stFrameworkNetworkCAsyncTcpSocket;
extern STraceNode g_stFrameworkNetworkCAsyncUdpSocket;
extern STraceNode g_stFrameworkNetworkCPollRequestStatus;
extern STraceNode g_stFrameworkNetworkCPollRequestStatusPoll;
extern STraceNode g_stFrameworkNetworkCPollSocket;
extern STraceNode g_stFrameworkNetworkCPollSocketPoll;
extern STraceNode g_stFrameworkNetworkCTcpServerSocket;
extern STraceNode g_stFrameworkNetworkCTcpSocket;
extern STraceNode g_stFrameworkNetworkCTcpSocketSendRecv;
extern STraceNode g_stFrameworkNetworkCUdpSocket;
extern STraceNode g_stFrameworkNetworkSocketErrors;

#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif  // MXG_MXTRACENETWORK_H
