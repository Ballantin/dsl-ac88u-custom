//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make  changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_MXTRACESIPSTACK_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXTRACESIPSTACK_H
//M5T_INTERNAL_USE_END

// SipUserAgent package node
extern STraceNode g_stSipStackSipUserAgent;
extern STraceNode g_stSipStackSipUserAgentCSipAutomaticAnswererReqCtxSvc;
extern STraceNode g_stSipStackSipUserAgentCSipNotifierSvc;
extern STraceNode g_stSipStackSipUserAgentCSipNotifierSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipClientSvc;
extern STraceNode g_stSipStackSipUserAgentCSipClientSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipPrivacySvc;
extern STraceNode g_stSipStackSipUserAgentCSipPrivacySvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipDigestClientAuthSvc;
extern STraceNode g_stSipStackSipUserAgentCSipDigestClientAuthSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipDiversionSvc;
extern STraceNode g_stSipStackSipUserAgentCSipDiversionSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipGenericSvc;
extern STraceNode g_stSipStackSipUserAgentCSipGenericSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipGenericReqCtxCoreSvc;
extern STraceNode g_stSipStackSipUserAgentCSipGenericReqCtxCoreSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipGlareSvc;
extern STraceNode g_stSipStackSipUserAgentCSipGlareSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipHistoryInfoSvc;
extern STraceNode g_stSipStackSipUserAgentCSipHistoryInfoSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipOptionTagsSvc;
extern STraceNode g_stSipStackSipUserAgentCSipOptionTagsSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipPublishSvc;
extern STraceNode g_stSipStackSipUserAgentCSipPublishSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipRedirectionSvc;
extern STraceNode g_stSipStackSipUserAgentCSipRedirectionSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipRefereeSvc;
extern STraceNode g_stSipStackSipUserAgentCSipRefereeSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSubscriberSvc;
extern STraceNode g_stSipStackSipUserAgentCSipSubscriberSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipReferrerSvc;
extern STraceNode g_stSipStackSipUserAgentCSipReferrerSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipMwiSvc;
extern STraceNode g_stSipStackSipUserAgentCSipMwiSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipRegistrationSvc;
extern STraceNode g_stSipStackSipUserAgentCSipRegistrationSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipTransferSvc07;
extern STraceNode g_stSipStackSipUserAgentCSipTransferSvc07FeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipReliableProvisionalResponseSvc;
extern STraceNode g_stSipStackSipUserAgentCSipReliableProvisionalResponseSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipUaAssertedIdentitySvc;
extern STraceNode g_stSipStackSipUserAgentCSipUaAssertedIdentitySvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipReplacesSvc;
extern STraceNode g_stSipStackSipUserAgentCSipReplacesSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipUpdateSvc;
extern STraceNode g_stSipStackSipUserAgentCSipUpdateSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionSvc;
extern STraceNode g_stSipStackSipUserAgentCSipSessionSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransaction;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUacBye;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUacByeFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUacInvite;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUacInviteFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUasBye;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUasByeFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUasInvite;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTransactionUasInviteFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipUserAgentSvc;
extern STraceNode g_stSipStackSipUserAgentCSipUserAgentSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTimerSvc;
extern STraceNode g_stSipStackSipUserAgentCSipSessionTimerSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentResultIdSipUserAgent;
extern STraceNode g_stSipStackSipUserAgentCSipTransactionCompletionSvc;
extern STraceNode g_stSipStackSipUserAgentCSipTransactionCompletionSvcFeatureECOM;
extern STraceNode g_stSipStackSipUserAgentCSipTransactionCompletionReqCtxConSvc;
extern STraceNode g_stSipStackSipUserAgentCSipUaHelpers;
extern STraceNode g_stSipStackSipUserAgentCSipContextTerminator;
extern STraceNode g_stSipStackSipUserAgentCSipTransactionTimerSvc;

#endif  // #ifndef MXG_MXTRACESIPSTACK_H
