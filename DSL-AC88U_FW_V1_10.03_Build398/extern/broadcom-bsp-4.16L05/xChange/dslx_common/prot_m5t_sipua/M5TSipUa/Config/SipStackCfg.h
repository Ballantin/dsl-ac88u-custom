//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_SIPSTACKCFG_H
#define MXG_SIPSTACKCFG_H


//@@PreSipStackCfg.h
// <GROUP CONFIG>
// <TOPICORDER 0>
// <TITLE Configuring the SIP Stack with "PreSipStackCfg.h">
//
// The SIP stack comes with the file "Config/SipStackCfg.h", which defines many
// compilation configuration options and values used throughout the source code.
// Generally, these values need updating for the specific application being
// developed with the SIP stack.
//
// To update these default values, you must create the "PreSipStackCfg.h" file
// with the updated configuration options for your application.
// "PreSipStackCfg.h" is always included first by "SipStackCfg.h" to retrieve
// application specific configurations, and then the default configuration
// options found in "Config/SipStackCfg.h" are applied for all items that have
// not been configured by the application.
//
// "PreSipStackCfg.h" is not packaged with the SIP stack and must be created for
// the specific application being developed. This file must be placed somewhere
// in the compiler search path to permit the retrieval of the application
// specific configuration options by the SIP stack.
//
//==============================================================================
//==============================================================================


// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSipStackCfg.h".
//
//     2 : You have created "PreSipStackCfg.h" but the compiler cannot find it.
//         This file must be located in a directory found within the include
//         path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename
//         (e.g.: "SipCore/ISipContext.h"), which permits setting the include
//         path to the "Source" directory of the package only, while
//         "PreSipStackCfg.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreSipStackCfg.h"


MX_NAMESPACE_START(MXD_GNS)


//@@MXD_POST_SIPSTACKCFG
// <GROUP PreSipStackCfg.h>
// Source Code:
//    #define MXD_POST_SIPSTACKCFG
//
// Description:
//  Enables the inclusion of "PostSipStackCfg.h" right at the end of
//  SipStackCfg.h. "PostSipStackCfg.h" is an application provided file that can
//  contain additional configuration options to possibly override the
//  configuration found in PreSipStackCfg.h and SipStackCfg.h.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_TLS
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_TLS
//
// Description:
//  Enables the inclusion of the TLS transport into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SEQUENTIAL_TRANSACTION_TABLE
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SEQUENTIAL_TRANSACTION_TABLE
//
// Description:
//  The MXD_SEQUENTIAL_TRANSACTION_TABLE macro makes the internal
//  transaction table use a CVector instead of a CMap. This is allowed
//  for memory footprint reasons. Indeed, enabling this macro and thus
//  using a CVector will lead to slightly less memory usage. Care must
//  be taken however because it will also cause loss of performance,
//  especially if the number of simultaneous transactions is large.
//
//  In general, applications should not use this macro unless memory
//  constraints are very severe. However, if the number of simultaneous
//  transactions is large (above 10, for instance), loss of performance
//  will occur that may be unacceptable.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================


//==============================================================================
//==  SERVICE ENABLING  ========================================================
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_TARGET_REUSE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipTargetReuseSvc into the build.
//
//  Note that the ISipServerLocationSvc must be enabled to enable this service
//  because this service uses some ISipServerLocationSvc functionalities.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipTargetReuseSvc,
//  MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT
//
// Description:
//  Enables the inclusion of the CTargetManagementModule into the build.
//
// This module, that is used with TargetReuseSvc and ConnectionBlacklistSvc
// services, allows to associate targets with context and generate events
// when these targets become available or unavailable.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CTargetManagementModule,
//  MXD_SIPSTACK_ENABLE_TARGET_MANAGEMENT_MODULE_SUPPORT, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_KEEP_ALIVE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipKeepAliveSvc into the build.
//
//  Enables the compilation of the Keep-Alive Transport Service and its
//  features. This define does not activate the Keep-Alive features, it simply
//  enables the code that permits their use.
//
// Location:
//  Define this in PreSipStackCfg.h.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_NETWORK_RECEPTION_CONTROL_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipNetworkReceptionControlSvc into the build.
//
//  Enables the compilation of the Network Reception Control Transport Service
//  and its features. This define does not activate the Network Reception
//  Control features, it simply enables the code that permits their use.
//
// Location:
//  Define this in PreSipStackCfg.h.
//
// See Also:
//  PreSipStackCfg.h
//  ISipCoreConfig::SetNetworkReceptionControlMgrA
//  ISipCoreConfig::SetPeerFloodBlockingA
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_PACKET_RECEPTION_CONTROL_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipPacketReceptionControlSvc into the build.
//
//  Enables the compilation of the Packet Reception Control Transport Service
//  and its features. This define does not activate the Packet Reception
//  Control features, it simply enables the code that permits their use.
//
// Location:
//  Define this in PreSipStackCfg.h.
//
// See Also:
//  PreSipStackCfg.h
//  ISipCoreConfig::SetPacketReceptionControlMgrA
//  ISipCoreConfig::EnablePeerFilteringA
//  ISipCoreConfig::AddApprovedPeerA
//  ISipCoreConfig::RemoveApprovedPeerA
//  ISipCoreConfig::RemoveAllApprovedPeersA
//  ISipCoreConfig::AddAllowedMethodA
//  ISipCoreConfig::RemoveAllowedMethodA
//  ISipCoreConfig::RemoveAllAllowedMethodsA
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipConnectionBlacklistSvc into the build.
//
//  Note that the ISipServerLocationSvc must be enabled to enable this service
//  because this service uses the ISipServerLocationSvc.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipConnectionBlacklistSvc,
//  MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_CORE_OUTPUT_CONTROLLING_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_CORE_OUTPUT_CONTROLLING_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipCoreOutputControllingSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipCoreOutputControllingSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipPersistentConnectionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipPersistentConnectionSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_CONNECTION_REUSE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipConnectionReuseSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipConnectionReuseSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_PERSISTENT_CONNECTION_CURRENT_SRV_FQDN_RETRIEVAL
//
// Description:
//  Enables the retrieval of the current SRV target for a given persistent
//  connection. This enables the application to set a callback inside the
//  PersistentConnectionList to get the current SRV FQDN and peer address used
//  to connect.
//
//  For example, this new information could be used during the TLS handshaking
//  process to match against a certificate alternate name if the said TLS
//  certificate included SRV FQDN alternate names.
//
//  In order to match the retrieved SRV and the TLS handhsaking callback, the
//  TLS callback now returns the peer address in order to match it to the
//  SRV callback peer address.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipPersistentConnectionList, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_OUTBOUND_CONNECTION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipOutboundConnectionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipOutboundConnectionSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SERVER_LOCATION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipServerLocationSvc into the build.
//
//  Note that when this service is not enabled, the ISipConnectionBlacklistSvc
//  cannot be enabled.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipServerLocationSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SPIRALLING_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipSpirallingSvc into the build.
//
//  Enables the Spiralling Service of the SIP stack. This define enables the
//  code that permits to use the Spiralling Service. By default, the Spiralling
//  Service is activated, the application may make a call to
//  ISipCoreConfig::SetSpirallingSvcState. See the ISipCoreConfig documentation
//  for more information.
//
//  Note: The Spiralling Service is useful for an application that implements
//  SIP server services that may need to send SIP packets to themselves, i.e.,
//  in loopback.
//
// Location:
//  Define this in PreSipStackCfg.h.
//
// See Also:
//  CSipSpirallingSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SYMMETRIC_UDP_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipSymmetricSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSymmetricSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_STATELESS_DIGEST_SERVER_AUTH_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_STATELESS_DIGEST_SERVER_AUTH_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipStatelessDigestServerAuthSvc into the
//  build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipStatelessDigestServerAuthSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_STATEFUL_PROXY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_STATEFUL_PROXY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipTransactionStatefulProxySvc into the
//  build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipTransactionStatefulProxySvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_SESSION_STATEFUL_PROXY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SESSION_STATEFUL_PROXY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipSessionStatefulProxySvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSessionStatefulProxySvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_STATELESS_PROXY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_STATELESS_PROXY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipStatelessProxySvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipStatelessProxySvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_DIGEST_CLIENT_AUTH_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipDigestClientAuthSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipStatelessProxySvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_SIP_DIGEST_CLIENT_AUTH_RESET_LOOP_COUNTER_WITH_NEW_NONCE
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_SIP_DIGEST_CLIENT_AUTH_RESET_LOOP_COUNTER_WITH_NEW_NONCE
//
// Description:
//  Resets the authentication loop counter when the nonce value changes.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_GENERIC_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipGenericSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipGenericSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipGlareSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipGlareSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_MWI_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipMwiSvc into the build.
//
//  To enable the ISipMwiSvc, ISipSubscriberSvc must also be
//  enabled with MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipMwiSvc, MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT,
//  PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG Updated behavior in 4.1.4>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipNotifierSvc into the build.
//
// Updated behaviour:
//  The previous ISipNotifierSvc interface has been deprecated and replaced by
//  this one. The interfaces ISipNotifier[PresenceEventName]Svc where
//  PresenceEventName can stand for "MessageSummary", "Reg", etc. are not
//  supported anymore. To support a presence event using the notifier you
//  simply have to call the AddEvent method on the ISipNotifierSvc interface.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSubscriberSvc, ISipMwiSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_OPTION_TAGS_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipOptionTagsSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipOptionTagsSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_PRIVACY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipPrivacySvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipPrivacySvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_PUBLISH_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipPublishSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipPublishSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_REDIRECTION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipRedirectionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipRedirectionSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipRefereeSvc into the build.
//
//  This service needs to be enabled to enable the ISipTransferSvc07.
//
//  To enable the ISipRefereeSvc, ISipNotifierSvc must also be enabled with
//  MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipRefereeSvc, MXD_SIPSTACK_ENABLE_SIP_NOTIFIER_SVC_SUPPORT,
//  PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipReferrerSvc into the build.
//
//  This service needs to be enabled to enable the ISipTransferSvc07.
//
//  To enable the ISipReferrerSvc, ISipSubscriberSvc must also be enabled
//  with MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipReferrerSvc, MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT,
//  PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_REGISTRATION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipRegistrationSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipRegistrationSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_RELIABLE_PROVISIONAL_RESPONSE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipReliableProvisionalResponseSvc into the
//  build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipReliableProvisionalResponseSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipReplacesSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipReplacesSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SESSION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipSessionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSessionSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SESSION_TIMER_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipSessionTimerSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSessionTimerSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG Updated behavior in 4.1.4>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SUBSCRIBER_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipSubscriberSvc into the build.
//
// Updated behaviour:
//  The previous ISipSubscriberSvc interface has been deprecated and replaced by
//  this one. The interfaces ISipSubscriber[PresenceEventName]Svc where
//  PresenceEventName can stand for "MessageSummary", "Reg", etc. are not
//  supported anymore. To support a presence event using the subscriber you
//  simply have to call the AddEvent method on the ISipNotifierSvc interface.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipSubscriberSvc, ISipMwiSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_TRANSFER_SVC_07_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipTransferSvc07 into the build.
//
//  To enable the ISipTransferSvc07, ISipRefereeSvc and ISipReferrerSvc must
//  also be enabled with MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT and
//  MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipTransferSvc07, MXD_SIPSTACK_ENABLE_SIP_REFEREE_SVC_SUPPORT,
//  MXD_SIPSTACK_ENABLE_SIP_REFERRER_SVC_SUPPORT, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_UA_ASSERTED_IDENTITY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipUaAssertedIdentitySvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipUaAssertedIdentitySvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_UPDATE_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipUpdateSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipUpdateSvc, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_USER_AGENT_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipUserAgentSvc into the build. The
//  ISipUserAgentSvc MUST be enabled to use any class from the SipUserAgent
//  folder.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipUserAgentSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_TRANSACTION_COMPLETION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipTransactionCompletionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipTransactionCompletionSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_DIVERSION_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipDiversionSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipDiversionSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_HISTORY_INFO_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_HISTORY_INFO_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipHistoryInfoSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipHistoryInfoSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_SERVER_MONITOR_SVC_SUPPORT
//
// Description:
//  Enables the SIP Server Monitoring feature allowing usage of the
//  ISipServerMonitor and ISipServerMonitorSvc interfaces.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipServerMonitorSvc, ISipServerMonitor, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_DEFAULT_NUMBER_OF_HEADER_PER_PACKET
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_DEFAULT_NUMBER_OF_HEADER_PER_PACKET 20
//
// Description:
//  Defines the initial capacity of the containers used to hold CSipHeader
//  objects. This value is used to avoid unnecessary memory reallocations while
//  adding headers in SIP packets or during SIP packet parsing.
//
//  However, note that SIP packets support holding more headers than this
//  defined value.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile to override the
//  default value.
//
//==============================================================================
#ifndef MXD_SIPSTACK_DEFAULT_NUMBER_OF_HEADER_PER_PACKET
    #define MXD_SIPSTACK_DEFAULT_NUMBER_OF_HEADER_PER_PACKET 20
#endif

//@@MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SERVER_INVITE_SAME_BRANCH_ACK
//
// Description:
//  Enables the proper handling of an ACK request to an INVITE with the
//  same branch value in the topmost via as the INVITE. This behaviour is
//  contrary to RFC 3261, which states that the branch in the topmost via of the
//  ACK request MUST be different than that of the INVITE.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_IPV6_ENABLE_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_IPV6_ENABLE_SUPPORT
//
// Description:
//  Enables the use of IP version 6 in the SIP stack. IPV6 must be enabled
//  in the framework in order to use it in the SIP stack.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipCoreConfig, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_MAILBOX_URI_SPECIFIC_IMPLEMENTATIONS_ENABLE_SUPPORT
//
// Description:
//  Enables the use of specific mailbox URI implementations in the SIP stack.
//
//  This option includes the implementation and resolving code for CPresUri and
//  CImUri.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_STATISTICS_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipStatisticsSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_ENUM_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the ISipEnumSvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENUM_DEFAULT_ZONE
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENUM_DEFAULT_ZONE "e164.arpa"
//
// Description:
//  Sets the default ENUM zone that is used to perform ENUM queries. When not
//  defined, the "e164.arpa" zone is used as per RFC 3761.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_MAX_DUMP_LENGTH
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_MAX_DUMP_LENGTH
//
// Description:
//  The maximum length in bytes of a dump buffer when using CInstanceTracker.
//  the default length is 8192.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetContextDump, PreSipStackCfg.h
//
//==============================================================================

#ifndef MXD_SIPSTACK_MAX_DUMP_LENGTH
    #define MXD_SIPSTACK_MAX_DUMP_LENGTH 8192
#endif

//@@MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEFAULT_DATA_LOGGER
//
// Description:
//  Enables the default data logger, which traces the sent and received SIP
//  packets as a string of ASCII characters.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipDefaultDataLogger, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
// <GROUP PreSipStackCfg.h>
// <FLAG Deprecated>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_LOCALLY_GENERATED_ERROR_RESPONSE_DIFFERENTIATOR
//
// Description:
//  Enables the addition of the "X-M5T-Origin: locally generated" custom header
//  to locally-generated final negative reponses (>= 3XX). This header can be
//  used to differentiate a locally-generated response from a remote one.
//
//  Global variables pszX_M5T_ORIGIN_HEADER_NAME and
//  pszX_M5T_ORIGIN_HEADER_VALUE are defined in CSipHeader.h to help in handling
//  this custom header.
//
// Note:
//  This macro is DEPRECATED. Applications MUST now use the new
//  CSipPacket::IsLocallyGenerated method.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h, CSipHeader.h, CSipPacketParser
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_REGINFO_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.3>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_REGINFO_SUPPORT
//
// Description:
//  Enables the XML parser for reginfo and GRUU.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIPCONTEXT_INSTANCE_TRACKING
//
// Description:
//  Enables the tracking of ISipContext objects, which may be used to facilitate
//  the debugging of memory leaks. When this macro is enabled, the application
//  may call the following methods:
//  CInstanceTracker::DebugGetContextTable,
//  CInstanceTracker::DebugGetNumContextsLeft,
//  CInstanceTracker::DebugGetContextDump.
//
// Note:
//  This macro should be enabled only in a debugging process since it activates
//  code that requires considerable CPU processing. Using it may therefore
//  significantly decrease the system performance.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetContextTable,
//  CInstanceTracker::DebugGetNumContextsLeft,
//  CInstanceTracker::DebugGetContextDump,
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_SIP_TRANSACTION_PEER_ADDRESS_MATCH_BYPASS_ENABLE_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.4>
//
// Source Code:
//    #define MXD_SIPSTACK_SIP_TRANSACTION_PEER_ADDRESS_MATCH_BYPASS_ENABLE_SUPPORT
//
// Description:
//  Enables the peer IP address matching bypass, so that the peer IP address
//  will not be used to match the packet to an existing transaction.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipTransaction, PreSipStackCfg.h
//
//==============================================================================


//@@MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.6>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_DESTINATION_SELECTION_SVC_SUPPORT
//
// Description:
//  Enables the destination selection service.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipDestinationSelectionSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.6>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_VIA_MANAGEMENT_SVC_SUPPORT
//
// Description:
//  Enables the Via management service.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipViaManagementSvc, PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_SIP_CONNECTION_RETRY_SVC_SUPPORT
// <GROUP PreSipStackCfg.h>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_CONNECTION_RETRY_SVC_SUPPORT
//
// Description:
//  Enables the inclusion of the CSipConnectionRetrySvc into the build.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipConnectionRetrySvc, PreSipStackCfg.h
//
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
//@@MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.6>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEPRECATED_PERSISTENT_CONNECTION_SWITCHBACK
//
// Description:
//  Enables the switchback feature for persistent connections.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipPersistentConnectionList, PreSipStackCfg.h
//
//==============================================================================
//M5T_INTERNAL_USE_END

//@@MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.7>
//
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
//
// Description:
//  Enables the tracking of ISipContext objects, which may be used to facilitate
//  the debugging of memory leaks. When this macro is enabled, the application
//  may call the following methods:
//  CInstanceTracker::DebugGetContextTable,
//  CInstanceTracker::DebugGetNumContextsLeft,
//  CInstanceTracker::DebugGetContextDump.
//
// Note:
//  This macro should be enabled only in a debugging process since it activates
//  code that requires considerable CPU processing. Using it may therefore
//  significantly decrease the system performance.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetContextTable,
//  CInstanceTracker::DebugGetNumContextsLeft,
//  CInstanceTracker::DebugGetContextDump,
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_DEPRECATED_SIPPACKET_INSTANCE_TRACKING
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.7>
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEPRECATED_SIPCONTEXT_INSTANCE_TRACKING
//
// Description:
//  Enables the tracking of CSipPacket objects, which may be used to facilitate
//  the debugging of memory leaks. When this macro is enabled, the application
//  may call the following methods:
//  CInstanceTracker::DebugGetPacketTable,
//  CInstanceTracker::DebugGetNumPacketsLeft,
//  CInstanceTracker::DebugGetPacketDump.
//
// Note:
//  This macro should be enabled only in a debugging process since it activates
//  code that requires considerable CPU processing. Using it may therefore
//  significantly decrease the system performance.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetPacketTable,
//  CInstanceTracker::DebugGetNumPacketsLeft,
//  CInstanceTracker::DebugGetPacketDump,
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.7>
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEPRECATED_SIPREQUEST_CONTEXT_INSTANCE_TRACKING
//
// Description:
//  Enables the tracking of ISipRequestContext objects, which may be used to
//  facilitate the debugging of memory leaks. When this macro is enabled,
//  the application may call the following methods:
//  CInstanceTracker::DebugGetRequestContextTable,
//  CInstanceTracker::DebugGetNumRequestContextsLeft,
//  CInstanceTracker::DebugGetRequestContextDump.
//
// Note:
//  This macro should be enabled only in a debugging process since it activates
//  code that requires considerable CPU processing. Using it may therefore
//  significantly decrease the system performance.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetRequestContextTable,
//  CInstanceTracker::DebugGetNumRequestContextsLeft,
//  CInstanceTracker::DebugGetRequestContextDump.
//  PreSipStackCfg.h
//
//==============================================================================

//@@MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.7>
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_DEPRECATED_SIPTRANSACTION_INSTANCE_TRACKING
//
// Description:
//  Enables the tracking of CSipTransaction objects, which may be used to
//  facilitate the debugging of memory leaks. When this macro is enabled,
//  the application may call the following methods:
//  CInstanceTracker::DebugGetTransactionTable,
//  CInstanceTracker::DebugGetNumTransactionsLeft,
//  CInstanceTracker::DebugGetTransactionDump.
//
// Note:
//  This macro should be enabled only in a debugging process since it activates
//  code that requires considerable CPU processing. Using it may therefore
//  significantly decrease the system performance.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CInstanceTracker::DebugGetTransactionTable,
//  CInstanceTracker::DebugGetNumTransactionsLeft,
//  CInstanceTracker::DebugGetTransactionDump.
//  PreSipStackCfg.h
//
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
//@@MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.9>
//
// Source Code:
//    #define MXD_SIPSTACK_SWITCHBACK_PERSISTENT_CONNECTION_MANAGEMENT_ENABLE_SUPPORT
//
// Description:
//  Enables the switchback feature for persistent connections.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  CSipPersistentConnectionList, PreSipStackCfg.h,
//  ISipSwitchbackPersistentConnection
//
//==============================================================================
//M5T_INTERNAL_USE_END

//@@MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.7>
// Source Code:
//    #define MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT
//
// Description:
//  Enables helper methods for a user agent.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
// See Also:
//  ISipUaHelpers::Terminate
//  PreSipStackCfg.h
//
//==============================================================================
#if defined(MXD_SIPSTACK_ENABLE_SIP_UA_HELPERS_SUPPORT)
    // Define a private macro for the monitor feature needed in the SIP core package.
    #define MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT
#endif

#if defined (MXD_SIPSTACK_ENABLE_MONITOR_SUPPORT)
    // The monitor uses the request context manager feature.
    #define MXD_SIPSTACK_ENABLE_SIP_REQUEST_CONTEXT_MGR_SUPPORT
#endif

//M5T_INTERNAL_USE_BEGIN
//@@MXD_SIPSTACK_SIP_PERSISTENT_CONNECTION_ENABLE_ON_LISTENING_SOCKET
// <GROUP PreSipStackCfg.h>
// <FLAG New in 4.1.10>
//
// Source Code:
//    #define MXD_SIPSTACK_SIP_PERSISTENT_CONNECTION_ENABLE_ON_LISTENING_SOCKET
//
// Description:
//  Enables the creation of UDP persistent connection on a listening socket.
//  Note that when this feature is enabled the persistent connection MUST be
//  terminated before calling StopListen on the listening socket.
//
// Location:
//  Define this in PreSipStackCfg.h or in your makefile.
//
//==============================================================================
//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#if defined(MXD_POST_SIPSTACKCFG)
#include "PostSipStackCfg.h"
#endif

#endif // #ifndef MXG_SIPSTACKCFG_H
