//==============================================================================
//==============================================================================
//
//          Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_MXTRACEUAIMPP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXTRACEUAIMPP_H

#ifndef MXG_SCEUAIMPPCFG_H
#include "Config/SceUaImppCfg.h"
#endif


// UA-IMPP package node.
extern STraceNode g_stSceUaImpp;
extern STraceNode g_stSceUaImppPageReceiver;
extern STraceNode g_stSceUaImppPageReceiverEcom;
extern STraceNode g_stSceUaImppPageSender;
extern STraceNode g_stSceUaImppPageSenderEcom;
extern STraceNode g_stSceUaImppPresencePublisher;
extern STraceNode g_stSceUaImppPresencePublisherEcom;
extern STraceNode g_stSceUaImppPresenceSubscriber;
extern STraceNode g_stSceUaImppPresenceSubscriberEcom;
extern STraceNode g_stSceUaImppXmlPatchOperations;

#if defined(MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT)
extern STraceNode g_stSceUaImppPidf;
extern STraceNode g_stSceUaImppPidfEcom;
extern STraceNode g_stSceUaImppPresenceAuthorization;
extern STraceNode g_stSceUaImppPresenceAuthorizationEcom;
extern STraceNode g_stSceUaImppResourceLists;
extern STraceNode g_stSceUaImppResourceListsEcom;
extern STraceNode g_stSceUaImppRlsServices;
extern STraceNode g_stSceUaImppRlsServicesEcom;
extern STraceNode g_stSceUaImppXcapDocumentSubscriber;
extern STraceNode g_stSceUaImppXcapDocumentSubscriberEcom;
extern STraceNode g_stSceUaImppXcapPatchOperations;
extern STraceNode g_stSceUaImppXmlDocumentControl;
extern STraceNode g_stSceUaImppXmlDocumentControlEcom;
#endif


//M5T_INTERNAL_USE_END
#endif  // #ifndef MXG_MXTRACEUAIMPP_H
