//==============================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_ECOMDEF_H
#include "ECom/EComDef.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  STATIC MEMBERS INITIALIZATION  =========================================
//==============================================================================
#if defined(MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipDigestClientAuthSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipDiversionSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipGenericSvc);
MX_DECLARE_ECOM_CLSID(CSipGenericReqCtxCoreSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipGlareSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_HISTORY_INFO_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipHistoryInfoSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipMwiSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipNotifierSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipOptionTagsSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipPrivacySvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipRedirectionSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipRefereeSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipReferrerSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipRegistrationSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipReliableProvisionalResponseSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipReplacesSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipSessionSvc);
MX_DECLARE_ECOM_CLSID(CSipSessionTransactionUacBye);
MX_DECLARE_ECOM_CLSID(CSipSessionTransactionUacInvite);
MX_DECLARE_ECOM_CLSID(CSipSessionTransactionUasBye);
MX_DECLARE_ECOM_CLSID(CSipSessionTransactionUasInvite);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipSessionTimerSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipSubscriberSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipTransferSvc07);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipUaAssertedIdentitySvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipUpdateSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipUserAgentSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipPublishSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipTransactionCompletionSvc);
#endif

#if defined(MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_TIMER_SVC_SUPPORT)
MX_DECLARE_ECOM_CLSID(CSipTransactionTimerSvc);
#endif

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END


