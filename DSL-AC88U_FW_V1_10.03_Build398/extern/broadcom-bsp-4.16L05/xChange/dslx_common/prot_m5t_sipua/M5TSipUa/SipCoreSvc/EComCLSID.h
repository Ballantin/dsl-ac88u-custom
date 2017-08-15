//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//M5T_INTERNAL_USE_BEGIN

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

// MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
//------------------------------------------------
#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  STATIC MEMBERS INITIALIZATION  =========================================
//==============================================================================
#if defined (MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipStatisticsContainer);
MX_DECLARE_ECOM_CLSID(CSipStatisticsSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipConnectionBlacklistSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_CORE_OUTPUT_CONTROLLING_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipCoreOutputControllingSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipPersistentConnectionSvc);
#endif

#if defined (MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipPersistentConnectionManagement);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipDestinationSelectionSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipServerLocationSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipTargetReuseSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipViaManagementSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_STATELESS_DIGEST_SERVER_AUTH_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipStatelessDigestServerAuthSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipSymmetricUdpSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipEnumSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_CONNECTION_RETRY_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipConnectionRetrySvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipOutboundConnectionSvc);
#endif

#if defined (MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipConnectionReuseSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipServerMonitor);
MX_DECLARE_ECOM_CLSID(CSipServerMonitorSvc);
#endif

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END


