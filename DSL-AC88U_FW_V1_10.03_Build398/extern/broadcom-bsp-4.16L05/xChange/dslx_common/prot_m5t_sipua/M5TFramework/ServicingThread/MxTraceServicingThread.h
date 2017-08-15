//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_MXTRACESERVICINGTHREAD_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXTRACESERVICINGTHREAD_H
//M5T_INTERNAL_USE_END

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

// ServicingThread package node
extern STraceNode g_stFrameworkServicingThread;
extern STraceNode g_stFrameworkServicingThreadCEventDriven;
extern STraceNode g_stFrameworkServicingThreadCServicingThread;
extern STraceNode g_stFrameworkServicingThreadCServicingThreadActivate;
extern STraceNode g_stFrameworkServicingThreadCServicingThreadMessageService;
extern STraceNode g_stFrameworkServicingThreadCServicingThreadSocketService;
extern STraceNode g_stFrameworkServicingThreadCServicingThreadTimerService;
extern STraceNode g_stFrameworkServicingThreadCServicingThreadWatchdogService;
extern STraceNode g_stFrameworkServicingThreadCWatchdogMonitor;

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif  // MXG_MXTRACESERVICINGTHREAD_H
