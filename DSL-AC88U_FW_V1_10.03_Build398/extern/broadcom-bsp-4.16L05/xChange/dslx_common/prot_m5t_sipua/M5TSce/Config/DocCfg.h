//==============================================================================
//==============================================================================
//
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2009 Media5 Corporation. ("Media5")
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

//@@!!WELCOME
//<title M5T SIP Client Engine v2.1 - API Reference>
//
//Summary:
//Proprietary & Confidential
//==============================================================================

//@@!!COPYRIGHT
//<TITLE Legal Information>
//
//* Legal Information *
//
// Copyright(c) 2011 Media5 Corporation. ("Media5")
//
// This document contains information that is confidential and proprietary to
// Media5.
//
// Media5 reserves all rights to this document as well as to the Intellectual
// Property of the document and the technology and know-how that it includes
// and represents.
//
// This publication cannot be reproduced, neither in whole nor in part, in any
// form whatsoever, without prior written approval by Media5.
//
// Media5 reserves the right to revise this publication and make changes at
// any time and without the obligation to notify any person and/or entity of
// such revisions and/or changes.
//
//* Document Build Date *
//
// This document was built on: %DATEL%
//
//==============================================================================

//@@INTRODUCTION
//<TOPICORDER 0>
//<TITLE Introduction>
//Welcome to the M5T SIP Client Engine API Reference. This document provides
//quick and simple references to the usage of the SIP Client Engine. It should
//be used by programmers who already have some knowledge of the SIP Client
//Engine. Reading "M5T SIP Client Engine v2.1 - Programmer's Guide" is usually
//a good starting point before reading this API Reference.
//
//All modifications to this API Reference are tracked through CRs (Change
//Requests) and are documented in the release notes associated with this product.
//

//--There is no deprecated API in SCE 2.1. If it changes this paragraph should
//--be uncommented and completed.

//--M5T SIP Client Engine v2.1 still supports a number of APIs that are deprecated.
//--A deprecated API means that it is accessible in this version of the software,
//--but will be eventually removed, so its use is discouraged.
//
//--The following are the deprecated APIs of M5T SIP Client Engine v2.1:

//M5T_INTERNAL_USE_BEGIN
// This file is simply used to define the documentation structure of the M5T
// SIP Client Engine.

//
//  Topic Hierarchy for SIP Client Engine project
//
//   - Table of Content                  []
//   - Introduction                      [Introduction]
//   - Overview                          [TID_OVERVIEW]
//      - Core SCE Component             [TID_OVERVIEW_CORE]
//      - FXS Call Manager Option Components    [TID_OVERVIEW_FXSCM]
//      - UA-IMPP Option Components      [TID_OVERVIEW_UAIMPP]
//   - Compile-Time Configuration        [TID_COMPILE_CONFIG]
//      - Configuration Mechanism        [TID_COMPILE_CONFIG_HOWTO]
//          - SceCore                    [TID_COMPILE_CONFIG_HOWTO_SCECORE]
//          - SceCoreComponents          [TID_COMPILE_CONFIG_HOWTO_SCECORECOMPONENTS]
//          - SceUaSsp                   [TID_COMPILE_CONFIG_HOWTO_SCEUASSP]
//          - SceMsp                     [TID_COMPILE_CONFIG_HOWTO_SCEMSP]
//          - SceCmdDigitsInterface      [TID_COMPILE_CONFIG_HOWTO_SCECMDDIGITSINTERFACE]
//          - SceFxsCallManager          [TID_COMPILE_CONFIG_HOWTO_SCEFXSCALLMANAGER]
//          - SceUaImpp                  [TID_COMPILE_CONFIG_HOWTO_SCEUAIMPP]
//          - SceUaImppXml               [TID_COMPILE_CONFIG_HOWTO_SCEUAIMPPXML]
//      - Configuration Macros           [TID_COMPILE_CONFIG_MACROS]
//          - Core SCE Configuration     [TID_COMPILE_CONFIG_MACROS_CORE]
//          - FXS Call Manager Option Configuration  [TID_COMPILE_CONFIG_MACROS_FXSCM]
//          - UA-IMPP Option Configuration [TID_COMPILE_CONFIG_MACROS_UAIMPP]
//   - Tracing Information               [TID_TRACING_INFORMATION]
//      - Mechanism                      [TID_TRACING_INFORMATION_MECHANISM]
//      - Tracing Nodes                  [TID_TRACING_INFORMATION_TRACINGNODES]
//   - Package Startup                   [TID_PACKAGE_STARTUP]
//      - Classes                        [TID_PACKAGE_STARTUP_CLASSES]
//   - Package SCE Core                  [TID_PACKAGE_SCECORE]
//      - Classes                        [TID_PACKAGE_SCECORE_CLASSES]
//      - Functions                      [TID_PACKAGE_SCECORE_FUNCTIONS]
//      - Struct & Enums                 [TID_PACKAGE_SCECORE_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_SCECORE_VARS]
//      - Macros                         [TID_PACKAGE_SCECORE_MACROS]
//   - Package SCE Core Components       [TID_PACKAGE_SCECORECOMPONENTS]
//      - Classes                        [TID_PACKAGE_SCECORECOMPONENTS_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_SCECORECOMPONENTS_VARS]
//      - Macros                         [TID_PACKAGE_SCECORECOMPONENTS_MACROS]
//   - Package UA SSP                    [TID_PACKAGE_UASSP]
//      - Classes                        [TID_PACKAGE_UASSP_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_UASSP_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_UASSP_VARS]
//      - Macros                         [TID_PACKAGE_UASSP_MACROS]
//   - Package MSP                       [TID_PACKAGE_MSP]
//      - Classes                        [TID_PACKAGE_MSP_CLASSES]
//   - Package Command Digits Interface  [TID_PACKAGE_CMDDIGITSINTERFACE]
//      - Classes                        [TID_PACKAGE_CMDDIGITSINTERFACE_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_CMDDIGITSINTERFACE_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_CMDDIGITSINTERFACE_VARS]
//      - Macros                         [TID_PACKAGE_CMDDIGITSINTERFACE_MACROS]
//   - Package FXS Call Manager          [TID_PACKAGE_FXSCALLMANAGER]
//      - Classes                        [TID_PACKAGE_FXSCALLMANAGER_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_FXSCALLMANAGER_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_FXSCALLMANAGER_VARS]
//      - Macros                         [TID_PACKAGE_FXSCALLMANAGER_MACROS]
//   - Package FXS Line                  [TID_PACKAGE_FXSLINE]
//      - Classes                        [TID_PACKAGE_FXSLINE_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_FXSLINE_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_FXSLINE_VARS]
//      - Macros                         [TID_PACKAGE_FXSLINE_MACROS]
//   - Package UAIMPP                    [TID_PACKAGE_UAIMPP]
//      - Classes                        [TID_PACKAGE_UAIMPP_CLASSES]
//      - Struct & Enums                 [TID_PACKAGE_UAIMPP_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_UAIMPP_VARS]
//      - Macros                         [TID_PACKAGE_UAIMPP_MACROS]
//   - Package SCE XML                   [TID_PACKAGE_SCEXML]
//      - Classes                        [TID_PACKAGE_SCEXML_CLASSES]
//      - Functions                      [TID_PACKAGE_SCEXML_FUNCTIONS]
//      - Struct & Enums                 [TID_PACKAGE_SCEXML_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_SCEXML_VARS]
//      - Macros                         [TID_PACKAGE_SCEXML_MACROS]
//
//M5T_INTERNAL_USE_END


//@@TID_OVERVIEW
//<TOPICORDER 1>
//<TITLE Components Overview>
//
// The M5T SIP Client Engine offers a number of integrated components offering a
// wide breadth of interfaces. This section introduces each available components
// and the interfaces they support.
//
// The M5T SIP Client Engine offers three different options: the SCE Core, the
// FXS Call Manager and the UA-IMPP. The SCE Core is the central option (the
// framework for other options) which offers advanced services to implement a
// SIP user-agent.
//
// The FXS Call Manager and UA-IMPP packages are offered in option and are based
// on the SCE Core option. The FXS Call Manager option offers components realize
// a complete FXS state machine. The UA-IMPP option offers components to reliaze
// a complete framework to manage XML documents related to the instant messaging
// and the presence.

//@@TID_OVERVIEW_CORE
//<TITLE Core SCE>
//<GROUP TID_OVERVIEW>
//<TOPICORDER 1>
//
// #M5T SIP Client Engine Configuration Object#
//
// The M5T SIP Client Engine offers a unified configuration object for settings
// that apply to the whole engine. Moreover, most of the M5T SIP stack settings
// are still available through the ISipCoreConfig object.
//
//  Created with the following class-id:
//      - CLSID_CSceCoreConfig
//
//  Supports the following interfaces:
//      - ISceCoreConfig
//
// #The User Configuration Object#
// The user configuration object holds all configurations related to a user.
// This object implements many interfaces that may or may not be available at
// execution time, depending on the compile-time configuration of the Client
// Engine. When instantiating any component (call, registration, notifier,
// subscriber, MWI, all described below), a single user configuration object
// must be associated with the component. Moreover, the configuration object can
// be shared across multiple components if all components represent the same
// user.
//
//  Created with the following class-id:
//      - CLSID_CSceUserConfig
//
//  Supports the following interfaces:
//      - ISceUserConfig
//      - ISceGruuConfig
//      - ISceUserAuthentication
//      - ISceUserSecurityConfig
//      - ISceUserIdentityConfig
//      - ISceUaCapabilitiesConfig
//      - IUaSspRegistrationConfig, when compiled with the M5T UA-SSP
//      - IUaSspMwiConfig, when compiled with the M5T UA-SSP
//      - IUaSspCallConfig, when compiled with the M5T UA-SSP
//      - ISceFxsLineConfig, when compiled with the M5T FXS Call Manager
//      - IUaImppPresencePublisherConfig, when compiled with the M5T UA-IMPP
//      - IUaImppXcapConfig, when compiled with the M5T UA-IMPP
//
// #The Registration Component#
// The registration component is central to the M5T SIP Client Engine, as it
// performs registrations, refreshes and registration retries for a single user.
// Moreover, it recognizes incoming requests for the user it manages and
// immediately associates the proper user configuration object with the incoming
// request.
//
//  Created with the following class-id:
//      - CLSID_CUaSspRegistration
//
//  Supports the following interfaces:
//      - IUaSspRegistration
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
// #The Call Component#
// The call component is used to establish and manage SIP dialogs established
// with the INVITE request. The call component is special in that it can be
// created by the user of the M5T SIP Client Engine or automatically by the
// engine, when an incoming call arrives.
//
//  Created with the following class-id:
//      - CLSID_CUaSspCall
//
//  Supports the following interfaces:
//      - IUaSspCall
//      - ISceUserIdentityConfig
//      - ISceUaCapabilitiesConfig
//      - ISceBasicExtensionControl
//
//  New incoming calls are reported through:
//      - IUaSspNewCallMgr
//
//  How to configure an IUaSspNewCallMgr:
//      - Query IUaSspCallHandler from ISceCoreConfig::GetHandlerInterface()
//      - Set the IUaSspNewCallMgr in IUaSspCallHandler::SetManager()
//
// #The Subscriber Component#
// The subscriber component implements the necessary mechanisms to subscribe to
// a SIP resource and receive NOTIFY requests with the resource state. It
// supports standard subscriptions, where the state of the subscribed-to
// resource is simply provided to the user, and it also performs full handling
// of subscriptions to resource lists and to watcher-info. In the later two
// cases, the subscriber component implements the management of the XML payload
// found in NOTIFY requests and provides detailled information to the
// application about either the resource list and the resource state, or
// detailled information about the watchers.
//
// The subscriber component is special in that it is a generic component, as it
// can manage subscriptions to any event type. Being generic, the subscriber
// component does not have an associated configuration object. This component
// implements the subscriber side of RFC 3265, and the management of the
// payloads defined in RFC 4662, 3857 and 3858.
//
//  Created with the following class-id:
//      - CLSID_CSceSubscriber
//
//  Supports the following interfaces:
//      - ISceSubscriber
//      - ISceUserIdentityConfig
//      - ISceUaCapabilitiesConfig
//      - ISceBasicExtensionControl
//
// #The Notifier Component#
// The notifier component implements the necessary mechanisms to handle incoming
// subscription requests and issue the corresponding NOTIFY requests. The
// notifier component is special in that it is a generic component, as it can
// manage subscriptions to any event type. Being generic, the notifier component
// does not have an associated configuration object. Moreover, the user of the
// component is responsible to notify the component when the resource state has
// changed, and is also responsible to provide the payload to send along with
// the NOTIFY. This component implements the notifier side of RFC 3265 and, as
// such, is always a server component as it is automatically created upon the
// reception of a SUBSCRIBE request.
//
//  Created with the following class-id:
//      - CLSID_CSceNotifier
//
//  Supports the following interfaces:
//      - ISceNotifier
//      - ISceUserIdentityConfig
//      - ISceUaCapabilitiesConfig
//      - ISceBasicExtensionControl
//
//  New incoming subscription requests are reported through:
//      - ISceNewNotifierMgr
//
//  How to configure an ISceNewNotifierMgr:
//      - Query ISceNotifierHandler from ISceCoreConfig::GetHandlerInterface()
//      - Set the ISceNewNotifierMgr in ISceNotifierHandler::SetManager()
//
// #The Unsolicited MWI Component#
//  The unsolicited MWI component implements the required mechanisms to
//  interpret and report unsolicited MWI NOTIFY requests for specific users.
//
//  Here is the lifecycle of this component:
//      - It is created. The reference on the component is kept until
//        EvSubscriptionStatus(eTERMINATED) is called on the manager;
//      - SetManager and SetConfiguration is called on it;
//      - It notifies its manager about the received unsolicited MWI
//        notifications;
//      - Terminate is called on it;
//      - EvSubscriptionStatus(eTERMINATED) is called on the manager;
//      - The reference on the component is released.
//
//  Created with the following class-id:
//      - CLSID_CUaSspBlindMwi
//
//  Supports the following interfaces:
//      - IUaSspMwi
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
// #The Subscribing MWI Component#
//  The subscribing MWI component implements the necessary glue over the
//  subscriber component for managing the Message Waiting Indication service, as
//  specified in RFC 3842.
//
//  Created with the following class-id:
//      - CLSID_CUaSspMwi
//
//  Supports the following interfaces:
//      - IUaSspMwi
//      - ISceUserIdentityConfig
//      - ISceUaCapabilitiesConfig
//      - ISceBasicExtensionControl
//
// #The Out-of-Dialog Request Sender Component#
//  The out-of-dialog (OOD) request sender component allows users of the M5T
//  Client Engine to send any type of request without having to use an existing
//  component. The only useful task that can be done with this component is to
//  send requests and handle their corresponding responses.
//
//  This component must be used only to send requests that do not establish a
//  dialog. Thus, INVITE, REFER and SUBSCRIBE must not be sent with this
//  component, as they are all dialog establishing requests.
//
//  Created with the following class-id:
//      - CLSID_CSceOodRequestSender
//
//  Supports the following interfaces:
//      - ISceOodRequestSender
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
// #The Referrer Component#
//  The referrer component allows users to send and receive REFER requests
//  that do not match an existing dialog. The REFER request creates its own
//  dialog and is thus separated from other dialogs.
//
//  Created with the following class-id:
//      - CLSID_CSceReferrer
//
//  Supports the following interfaces:
//      - ISceReferrer
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
//  New incoming REFER requests are reported through:
//      - ISceNewReferrerMgr
//
//  How to configure an ISceNewReferrerMgr:
//      - Query ISceReferrerHandler from ISceCoreConfig::GetHandlerInterface()
//      - Set the ISceNewReferrerMgr in ISceReferrerHandler::SetManager()
//
// #MSP Package Components#
// Media negotiation handling for the M5T SIP Client Engine is provided by the
// Multimedia Service Provider (MSP) package. This section quickly introduces
// the interfaces supported by each component. The Call Component of the SCE
// Core uses the MSP interfaces a generate/negotiate SDP offers and answers.
//
// #The MSP Session Component#
//  Created with the following class-id:
//      - CLSID_CMspSession
//
//  Supports the following interfaces:
//      - IMspSession
//
// #The MSP Media (Audio) Component#
//  Created with the following class-id:
//      - CLSID_CMspMediaAudio
//
//  Supports the following interfaces:
//      - IMspMedia
//      - IMspMediaAudio
//

//@@TID_OVERVIEW_FXSCM
//<TITLE FXS Call Manager Option Components>
//<GROUP TID_OVERVIEW>
//<TOPICORDER 2>
//
// #FXS Call Manager Package Components#
// The M5T SIP Client Engine offers an additional optional package managing a
// Foreign eXchange Station (FXS) endpoint. This section quickly introduces the
// interfaces supported by the component that is part of the FXS Call Manager
// option.
//
// #The FXS Line Component#
// The FXS Line component allows an application to register, make calls, receive
// calls and receive message waiting (MWI) notifcations. It offers at a high
// level of abstraction the functionnalities required to realize a SIP
// user-agent. This component is the glue code that uses other components to
// offer a complete solution. It uses the components such as the Call, the
// Registration, the MSP Audio, the MSP Session and connects them together with
// the Command Digits Interface and the FXS Call Manager packages.
//
// The FXS Call Manager package offers a complete FXS telephony state machine.
// It manages the states of a telephone and can be configured to enable several
// services such as:
//
//      - Second call
//      - Call waiting
//      - Call forward
//      - Call transfer
//      - 3-way call
//
// It uses the Command Digits Interface package to detect dialed digits and then
// treat the number according to its internal state.
//
// The ISceFxsLine interface is the central point that the application should
// interact with.
//
//  Created with the following class-id:
//      - CLSID_CSceFxsLine
//
//  Supports the following interfaces:
//      - ISceFxsLine
//
//  The application can configure the FXS Line:
//
//  Supports the following interface:
//      - ISceFxsLineConfig
//

//@@TID_OVERVIEW_UAIMPP
//<TITLE UA-IMPP Option Components>
//<GROUP TID_OVERVIEW>
//<TOPICORDER 3>
//
// #IMPP Package Components#
// The M5T SIP Client Engine offers a number of additional optional integrated
// components related to Instant Messaging and Presence Provider (IMPP). This
// section quickly introduces the interfaces supported by each component.
//
// #The Page Receiver Component#
//  The UA IMPP Page Receiver component allows an application to receive and
//  handle page mode IM requests. More specifically, for the SIP protocol, this
//  component handles all incoming SIP MESSAGE requests for a specific user.
//
//  This component allows the application to handle the reception of MESSAGE
//  requests for a single SIP user. Note that a single instance of this
//  component can handle all MESSAGE requests from different peers for a single
//  local user.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPageReceiver
//
//  Supports the following interfaces:
//      - IUaImppPageReceiver
//
// #The Page Sender Component#
//  The UA IMPP Page Sender component allows an application to send page mode
//  IM requests. More specifically, for the SIP protocol, this component sends
//  SIP MESSAGE requests.
//
//  The SendMessage API allows the application to send any type of payload. It
//  is expected that applications that want to offer a rich IM experience will
//  use this API to send multipart messages that contains CPIM and quite
//  possibly pictures or images to display along with the message.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPageSender
//
//  Supports the following interfaces:
//      - IUaImppPageSender
//      - ISceOodRequestSenderMgr
//
//  New incoming responses to sent pages are reported through:
//      - IUaImppPageSenderMgr
//
// #The Presence Publisher Component#
//  The UA IMPP Presence Publisher component manages the publication of the
//  presence state of a user to a presence server. This corresponds to the
//  softstate of the user, which can change frequently and will expire if not
//  refreshed.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPresencePublisher
//
//  Supports the following interfaces:
//      - IUaImppPresencePublisher
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
// #The Presence Subscriber Component#
// The UA IMPP Presence Subscriber component enables the user to subscribe to
// the presence state of one or more users. It supports normal subscriptions to
// a single user and also subscriptions to a list of users using an RLS server.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPresenceSubscriber
//
//  Supports the following interfaces:
//      - IUaImppPresenceSubscriber
//      - ISceUserIdentityConfig
//      - ISceBasicExtensionControl
//      - ISceUaCapabilitiesConfig
//
// #The Generic XML Document Control (XDC) Component#
//  The generic XML Document Control (Generic XDC for short) is a component that
//  manages an XML document stored on a server. It allows to access and modify
//  the document using XCAP and it also can detect changes done to the document
//  by subscribing to its state.
//
//  The generic XDC can be used to manage any type of document, while
//  specialized versions of the XDC exist to manage specific document types.
//
//  Created with the following class-id:
//      - CLSID_CUaImppXmlDocumentControl
//
//  Supports the following interfaces:
//      - IUaImppXmlDocumentControl
//
// #The PIDF XDC Component#
//  The PIDF XML Document Control is used to manage a PIDF document stored on a
//  XCAP server. This PIDF information usually corresponds to the presence
//  "hard-state" of a user.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPidf
//
//  Supports the following interfaces:
//      - IUaImppXmlDocumentControl
//
// #The Presence Authorization XDC Component#
//  The Presence Authorization XML Document Control is used to manage a presence
//  authorization document stored on a XCAP server.
//
//  Created with the following class-id:
//      - CLSID_CUaImppPresenceAuthorization
//
//  Supports the following interfaces:
//      - IUaImppXmlDocumentControl
//
// #The Resource Lists XDC Component#
//  The Resource Lists XML Document Control is used to manage a resource list
//  document stored on a XCAP server.
//
//  Created with the following class-id:
//      - CLSID_CUaImppResourceLists
//
//  Supports the following interfaces:
//      - IUaImppXmlDocumentControl
//
// #The RLS XDC Component#
//  The RLS XML Document Control is used to manage a RLS document stored on a
//  XCAP server.
//
//  Created with the following class-id:
//      - CLSID_CUaImppRlsServices
//
//  Supports the following interfaces:
//      - IUaImppXmlDocumentControl
//
// #Other XML Documents#
// Other document components may be created by the application without going
// through an XML Document Control (XDC).
//
// #The Filter Document Component#
//  The Filter Document is used to create and manipulate a filter XML document.
//
//  Created with the following class-id:
//      - CSceFilterDocument
//
//  Supports the following interfaces:
//      - ISceFilterDocument
//
// #The PIDF Document Component#
//  The PIDF Document represents presence information of a single user. It is
//  used to access the highest level elements of a presence document, like the
//  device, service (tuple) and person elements.
//
//  Created with the following class-id:
//      - CScePidfDocument
//
//  Supports the following interfaces:
//      - IScePidfDocument
//
// #The Presence Authorization Document Component#
//  The Presence Authorization Document is needed to create and manipulate a
//  presence authorization document
//
//  Created with the following class-id:
//      - CScePresenceAuthorizationDocument
//
//  Supports the following interfaces:
//      - IScePresenceAuthorizationDocument
//
// #The Resource Lists Document Component#
//  The Resource Lists Document implements a document that manages zero or more
//  resource list. The resource list document can be used to represent a
//  buddy list or a contact list.
//
//  Created with the following class-id:
//      - CSceResourceListsDocument
//
//  Supports the following interfaces:
//      - ISceResourceListsDocument
//
// #The RLS Services Document Component#
//  The RLS Service document is used to define services (identified by an URI)
//  provided by a Resource List Server (RLS).
//
//  Created with the following class-id:
//      - CSceRlsServicesDocument
//
//  Supports the following interfaces:
//      - ISceRlsServicesDocument

//@@TID_COMPILE_CONFIG
//<TOPICORDER 2>
//<TITLE Compile-Time Configuration>

//@@TID_COMPILE_CONFIG_HOWTO
//<TITLE Configuration Mechanism>
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 1>

//@@TID_COMPILE_CONFIG_HOWTO_SCECORE
//<TITLE Configuring the SCE Core with "PreSceCoreCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 1>
//
// The core mechanisms of the SIP Client Engine come with the file
// "Config/SceCoreCfg.h", which defines many compilation configuration options
// and values used throughout the source code. Generally, these values need
// updating for the specific application being developed with the SIP Client
// Engine.
//
// To update these default values, you must create the "PreSceCoreCfg.h"
// file with the updated configuration options for your application.
// "PreSceCoreCfg.h" is always included first by "SceCoreCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/SceCoreCfg.h" are applied for all
// items that have not been configured by the application.
//
// "PreSceCoreCfg.h" is not packaged with the SIP Client Engine and must be
// created for the specific application being developed. This file must be
// placed somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by the SIP Client Engine.

//@@TID_COMPILE_CONFIG_HOWTO_SCECORECOMPONENTS
//<TITLE Configuring the SCE Core Components with "PreSceCoreComponentsCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 2>
//
// The SIP Client Engine Core Components come with the file
// "Config/SceCoreComponentsCfg.h", which defines many compilation configuration
// options and values used throughout the source code. Generally, these values
// need updating for the specific application being developed with the SIP
// Client Engine.
//
// To update these default values, you must create the
// "PreSceCoreComponentsCfg.h" file with the updated configuration options for
// your application. "PreSceCoreComponentsCfg.h" is always included first by
// "SceCoreComponentsCfg.h" to retrieve application-specific configurations, and
// then the default configuration options found in
// "Config/SceCoreComponentsCfg.h" are applied for all items that have not been
// configured by the application.
//
// "PreSceCoreComponentsCfg.h" is not packaged with the SIP Client Engine and
// must be created for the specific application being developed. This file must
// be placed somewhere in the compiler search path to permit the retrieval of
// the application-specific configuration options by the SIP Client Engine.

//@@TID_COMPILE_CONFIG_HOWTO_SCEUASSP
//<TITLE Configuring the UA-SSP with "PreSceUaSspCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 3>
//
// The UA-SSP comes with the file "Config/SceUaSspCfg.h", which
// defines many compilation configuration options and values used throughout the
// source code. Generally, these values need updating for the specific
// application being developed with the UA-SSP.
//
// To update these default values, you must create the "PreSceUaSspCfg.h"
// file with the updated configuration options for your application.
// "PreSceUaSspCfg.h" is always included first by "SceUaSspCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/SceUaSspCfg.h" are applied for all
// items that have not been configured by the application.
//
// "PreSceUaSspCfg.h" is not packaged with the Client Engine and must be created
// for the specific application being developed. This file must be placed
// somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by the UA-SSP.

//@@TID_COMPILE_CONFIG_HOWTO_SCEMSP
//<TITLE Configuring the MSP with "PreMspCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 4>
//
// The MSP comes with the file "Config/MspCfg.h", which
// defines many compilation configuration options and values used throughout the
// source code. Generally, these values need updating for the specific
// application being developed with the MSP.
//
// To update these default values, you must create the "PreMspCfg.h"
// file with the updated configuration options for your application.
// "PreMspCfg.h" is always included first by "MspCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/MspCfg.h" are applied for all
// items that have not been configured by the application.
//
// "PreMspCfg.h" is not packaged with the Client Engine and must be created
// for the specific application being developed. This file must be placed
// somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by the MSP.

//@@TID_COMPILE_CONFIG_HOWTO_SCECMDDIGITSINTERFACE
//<TITLE Configuring the Command Digits Interface with "PreCommandDigitsInterfaceCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 5>
//
// The CommandDigitsInterface comes with the file
// "Config/CommandDigitsInterfaceCfg.h", which defines compilation configuration
// options and values used throughout the source code. Generally, these values
// need updating for the specific application being developed with the
// CommandDigitsInterface.
//
// To update these default values, you must create the
// "PreCommandDigitsInterfaceCfg.h" file with the updated configuration options
// for your application. "PreCommandDigitsInterfaceCfg.h" is always included
// first by "CommandDigitsInterfaceCfg.h" to retrieve application-specific
// configurations, and then the default configuration options found in
// "Config/CommandDigitsInterfaceCfg.h" are applied for all items that were not
// configured by the application.
//
// "PreCommandDigitsInterfaceCfg.h" is not packaged with the
// CommandDigitsInterface and must be created for the specific application being
// developed. This file must be placed somewhere in the compiler search path to
// permit the retrieval of the application-specific configuration options by the
// CommandDigitsInterface.

//@@TID_COMPILE_CONFIG_HOWTO_SCEFXSCALLMANAGER
//<TITLE Configuring the FXS Call Manager with "PreFxsCallManagerCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 6>
//
// The FxsCallManager comes with the file "Config/FxsCallManagerCfg.h", which
// defines compilation configuration options and values used throughout the
// source code. Generally, these values need updating for the specific
// application being developed with the FxsCallManager.
//
// To update these default values, you must create the "PreFxsCallManagerCfg.h"
// file with the updated configuration options for your application.
// "PreFxsCallManagerCfg.h" is always included first by "FxsCallManagerCfg.h" to
// retrieve application specific configurations, and then the default
// configuration options found in "Config/FxsCallManagerCfg.h" are applied for
// all items that were not configured by the application.
//
// "PreFxsCallManagerCfg.h" is not packaged with the FxsCallManager and must be
// created for the specific application being developed. This file must be
// placed somewhere in the compiler search path to permit the retrieval of the
// application specific configuration options by the FxsCallManager.

//@@TID_COMPILE_CONFIG_HOWTO_SCEUAIMPP
//<TITLE Configuring the UA-IMPP with "PreSceUaImppCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 7>
//
// UA-IMPP comes with the file "Config/SceUaImppCfg.h", which defines many
// compilation configuration options and values used throughout the source code.
// Generally, these values need updating for the specific application being
// developed with UA-IMPP.
//
// To update these default values, you must create the "PreSceUaImppCfg.h"
// file with the updated configuration options for your application.
// "PreSceUaImppCfg.h" is always included first by "SceUaImppCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/SceUaImppCfg.h" are applied for all
// items that have not been configured by the application.
//
// "PreSceUaImppCfg.h" is not packaged with the UA-IMPP and must be created
// for the specific application being developed. This file must be placed
// somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by UA-IMPP.

//@@TID_COMPILE_CONFIG_HOWTO_SCEUAIMPPXML
//<TITLE Configuring the UA-IMPP XML with "PreSceUaImppXmlCfg.h">
//<GROUP TID_COMPILE_CONFIG_HOWTO>
//<TOPICORDER 8>
//
// UA-IMPP comes with the file "Config/SceUaImppXmlCfg.h", which defines many
// compilation configuration options and values used throughout the source code.
// Generally, these values need updating for the specific application being
// developed with UA-IMPP.
//
// To update these default values, you must create the "PreSceUaImppXmlCfg.h"
// file with the updated configuration options for your application.
// "PreSceUaImppXmlCfg.h" is always included first by "SceUaImppXmlCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/SceUaImppXmlCfg.h" are applied for all
// items that have not been configured by the application.
//
// "PreSceUaImppXmlCfg.h" is not packaged with the UA-IMPP and must be created
// for the specific application being developed. This file must be placed
// somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by UA-IMPP XML.

//@@TID_COMPILE_CONFIG_MACROS
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 2>
//<TITLE Configuration Macros>

//@@TID_COMPILE_CONFIG_MACROS_CORE
//<TITLE Core SCE Configuration>
//<GROUP TID_COMPILE_CONFIG_MACROS>
//<TOPICORDER 1>
//

//@@TID_COMPILE_CONFIG_MACROS_FXSCM
//<TITLE FXS Call Manager Option Configuration>
//<GROUP TID_COMPILE_CONFIG_MACROS>
//<TOPICORDER 2>
//

//@@TID_COMPILE_CONFIG_MACROS_UAIMPP
//<TITLE UA-IMPP Option Configuration>
//<GROUP TID_COMPILE_CONFIG_MACROS>
//<TOPICORDER 3>
//

//@@TID_TRACING_INFORMATION
//<TOPICORDER 3>
//<TITLE Tracing in the SIP Client Engine>

//@@TID_TRACING_INFORMATION_MECHANISM
//<GROUP TID_TRACING_INFORMATION>
//<TOPICORDER 1>
//<TITLE The M5T Tracing Mechanism>
//
// The M5T SIP Client Engine integrates the M5T tracing mechanism, which uses
// tracing nodes to allow the application to control which part of the M5T
// component can output traces. See MxTraceEnableNode and MxTraceDisableNode in
// the M5T Framework API reference document to find out more on how to enable
// and disable tracing through tracing nodes.

//@@TID_TRACING_INFORMATION_TRACINGNODES
//<GROUP TID_TRACING_INFORMATION>
//<TOPICORDER 2>
//<TITLE M5T SIP Client Engine Tracing Nodes>
//
// The root node of the M5T SIP Client Engine is "Sce". The following tracing
// nodes are available.
//
// <CODE>
//      /Sce
//      /Sce/Core
//      /Sce/Core/Ecom
//      /Sce/Core/Config
//      /Sce/Core/Init
//      /Sce/CoreComponents
//      /Sce/CoreComponents/Authentication
//      /Sce/CoreComponents/Authentication/Ecom
//      /Sce/CoreComponents/CallerPreferences
//      /Sce/CoreComponents/DefaultHandler
//      /Sce/CoreComponents/DefaultHandler/Ecom
//      /Sce/CoreComponents/GenericEventInfo
//      /Sce/CoreComponents/GenericEventInfo/Ecom
//      /Sce/CoreComponents/Notifier
//      /Sce/CoreComponents/Notifier/Ecom
//      /Sce/CoreComponents/Notifier/Initialization
//      /Sce/CoreComponents/OodRequestSender
//      /Sce/CoreComponents/OodRequestSender/Ecom
//      /Sce/CoreComponents/PersistentConnectionList
//      /Sce/CoreComponents/PersistentConnectionList/Ecom
//      /Sce/CoreComponents/Referrer
//      /Sce/CoreComponents/Referrer/Ecom
//      /Sce/CoreComponents/SipCallerPrefsSorter
//      /Sce/CoreComponents/SipFeatureSet
//      /Sce/CoreComponents/Subscriber
//      /Sce/CoreComponents/Subscriber/Ecom
//      /Sce/CoreComponents/Subscriber/Initialization
//      /Sce/CoreComponents/UserConfig
//      /Sce/CoreComponents/UserConfig/Ecom
//      /Sce/Msp
//      /Sce/Msp/Helpers
//      /Sce/Msp/Session
//      /Sce/Msp/Config
//      /Sce/Msp/Media
//      /Sce/Msp/Base
//      /Sce/Msp/Audio
//      /Sce/Msp/Generic
//      /Sce/StackServices
//      /Sce/StackServices/SipPacketObserverSvc
//      /Sce/StackServices/SipPacketObserverSvc/Ecom
//      /Sce/StackServices/SipGenericClientAuthenticationSvc
//      /Sce/StackServices/SipGenericClientAuthenticationSvc/Ecom
//      /Sce/UaSsp
//      /Sce/UaSsp/Call
//      /Sce/UaSsp/Call/Ecom
//      /Sce/UaSsp/Mwi
//      /Sce/UaSsp/Mwi/Ecom
//      /Sce/UaSsp/Registration
//      /Sce/UaSsp/Registration/Ecom
//      /Sce/UaSsp/Registration/RegEvent
// </CODE>
//
// The following tracing nodes are also available when compiling the SCE with
// the FXS Call Manager option.
//
// <CODE>
//      /Sce/Cmd
//      /Sce/Cmd/CTransition
//      /Sce/Cmd/CDigitMap
//      /Sce/Cmd/CDtmfTransaformation
//      /Sce/Cmd/CCommandDigitsParser
//      /Sce/Cmd/CState
//      /Sce/Cmd/CDialingConfig
//      /Sce/Cmd/CCommandDigitsInterfaceProvision
//      /Sce/Cmd/CCommandDigitsInterfaceState
//      /Sce/Cmd/CCommandDigitsInterface
//      /Sce/Cmd/CCommandDigitsInterfaceDestination
//      /Sce/Fxs
//      /Sce/Fxs/StateContext
//      /Sce/Fxs/RootState
//      /Sce/Fxs/RinginState
//      /Sce/Fxs/ResponseFailedState
//      /Sce/Fxs/OnHookIdleState
//      /Sce/Fxs/InProgressTransferredState
//      /Sce/Fxs/InProgressTerminatedState
//      /Sce/Fxs/InProgressSecondCallState
//      /Sce/Fxs/InProgressHoldState
//      /Sce/Fxs/InProgressConferenceWaitingState
//      /Sce/Fxs/InProgressConferenceState
//      /Sce/Fxs/InProgressComplexState
//      /Sce/Fxs/InProgressCallWaitingState
//      /Sce/Fxs/InProgressCallWaitingAnsweredState
//      /Sce/Fxs/InProgressCallState
//      /Sce/Fxs/ControlConfigTerminationState
//      /Sce/Fxs/ControlComplexState
//      /Sce/Fxs/ControlCollectionState
//      /Sce/Fxs/EuroCollectionState
//      /Sce/Fxs/CallIdProvider
//      /Sce/Fxs/CallMessageInfoHold
//      /Sce/Fxs/CallProgressInfoRinging
//      /Sce/Fxs/CallReleaseInfo
//      /Sce/Fxs/CallSetupInfo
//      /Sce/Fxs/DisabledState
//      /Sce/Fxs/Transition
//      /Sce/Fxs/CallManager
//      /Sce/Fxs/CallManagerConfig
//      /Sce/FxsLine
//      /Sce/FxsLine/FxsLine
//      /Sce/FxsLine/FxsLine/Ecom
//      /Sce/FxsLine/FxsLineRepository
// </CODE>
//
// The following tracing nodes are also available when compiling the SCE with
// the UA-IMPP option.
//
// <CODE>
//      /Sce/UaImpp
//      /Sce/UaImpp/PageReceiver
//      /Sce/UaImpp/PageReceiver/Ecom
//      /Sce/UaImpp/PageSender
//      /Sce/UaImpp/PageSender/Ecom
//      /Sce/UaImpp/Pidf
//      /Sce/UaImpp/Pidf/Ecom
//      /Sce/UaImpp/PresenceAuthorization
//      /Sce/UaImpp/PresenceAuthorization/Ecom
//      /Sce/UaImpp/PresencePublisher
//      /Sce/UaImpp/PresencePublisher/Ecom
//      /Sce/UaImpp/PresenceSubscriber
//      /Sce/UaImpp/PresenceSubscriber/Ecom
//      /Sce/UaImpp/ResourceLists
//      /Sce/UaImpp/ResourceLists/Ecom
//      /Sce/UaImpp/RlsServices
//      /Sce/UaImpp/RlsServices/Ecom
//      /Sce/UaImpp/XcapDocumentSubscriber
//      /Sce/UaImpp/XcapDocumentSubscriber/Ecom
//      /Sce/UaImpp/XcapPatchOperations
//      /Sce/UaImpp/XmlDocumentControl
//      /Sce/UaImpp/XmlDocumentControl/Ecom
//      /Sce/UaImpp/XmlPatchOperations
//      /Sce/Xml
//      /Sce/Xml/ResourceList
//      /Sce/Xml/ResourceListDocument
//      /Sce/Xml/RlsServicesDocument
// </CODE>


//@@TID_PACKAGE_STARTUP
//<TOPICORDER 4>
//<TITLE Startup APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine Startup package, which is located under the Startup
// directory.
//
// The Startup offers a set of classes that are responsible to initialize and
// terminate different parts of the SIP Client Engine before it can be used.
// Since the higher-level parts always initialize their dependencies, it is not
// required to explicitely initialize all packages of the SIP Client Engine.
// Depending on which option is needed, an application only has to initialize
// the following:
//
// - SCE Core Option: The UA-SSP and MSP packages using CSceUaSspInitializer and
//                    CMspInitializer.
// - UA-IMPP Option: The UA-SSP, the MSP and the UA-IMPP packages using
//                   CSceUaSspInitializer, CMspInitializer and
//                   CSceUaImppInitializer.
// - FXS Call Manager Option: The FxsLine package using CSceFxsLineInitializer.
//

//@@TID_PACKAGE_STARTUP_CLASSES
//<GROUP TID_PACKAGE_STARTUP>
//<TOPICORDER 1>
//<TITLE Startup Classes>


//@@TID_PACKAGE_SCECORE
//<TOPICORDER 5>
//<TITLE SceCore APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine Core (SceCore) package, which is located under the SceCore
// directory.
//
// The SceCore package offers the basic glue interface to the M5T SIP stack and
// also offers basic mechanisms that allow all packages of the SIP Client Engine
// to share their capabilities. The most important application interface of this
// package is the core configuration object, which serves as the main
// configuration interface for all non-user configuration options of the SIP
// Client Engine.

//@@TID_PACKAGE_SCECORE_CLASSES
//<GROUP TID_PACKAGE_SCECORE>
//<TOPICORDER 1>
//<TITLE SIP Client Engine Core Classes>

//@@TID_PACKAGE_SCECORE_FUNCTIONS
//<GROUP TID_PACKAGE_SCECORE>
//<TOPICORDER 2>
//<TITLE SIP Client Engine Core Functions>

//@@TID_PACKAGE_SCECORE_STRUCTSENUMS
//<GROUP TID_PACKAGE_SCECORE>
//<TOPICORDER 3>
//<TITLE SIP Client Engine Core Structs & Enums>

//M5T_INTERNAL_USE_BEGIN

//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_SCECORE_VARS
//<GROUP TID_PACKAGE_SCECORE>
//<TOPICORDER 4>
//<TITLE SIP Client Engine Core Variables>


//@@TID_PACKAGE_SCECORE_MACROS
//<GROUP TID_PACKAGE_SCECORE>
//<TOPICORDER 5>
//<TITLE SIP Client Engine Core Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_SCECORECOMPONENTS
//<TOPICORDER 6>
//<TITLE SceCoreComponents APIs>
//
// This section documents the application level APIs that are available from the
// Core Components of the SIP Client Engine (SceCoreComponents) package, which
// is located under the SceCoreComponents directory.
//
// The SceCoreComponents package offers many important objects and components,
// like the user configuration object, which is used throughout the client
// engine, the base component, from which all other components inherit, and all
// necessary extension mechanisms for extensions built by the application.

//@@TID_PACKAGE_SCECORECOMPONENTS_CLASSES
//<GROUP TID_PACKAGE_SCECORECOMPONENTS>
//<TOPICORDER 1>
//<TITLE Core Components Classes>

//@@TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS
//<GROUP TID_PACKAGE_SCECORECOMPONENTS>
//<TOPICORDER 2>
//<TITLE Core Components Structs & Enums>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_SCECORECOMPONENTS_VARS
//<GROUP TID_PACKAGE_SCECORECOMPONENTS>
//<TOPICORDER 3>
//<TITLE Core Components Variables>


//@@TID_PACKAGE_SCECORECOMPONENTS_MACROS
//<GROUP TID_PACKAGE_SCECORECOMPONENTS>
//<TOPICORDER 4>
//<TITLE Core Components Macros>

//M5T_INTERNAL_USE_END


//@@TID_PACKAGE_UASSP
//<TOPICORDER 7>
//<TITLE SceUaSsp APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine User-Agent Signaling Service Provider (SceUaSsp) package,
// which is located under the SceUaSsp directory.
//
// The UA-SSP package offers components implementing the services required by
// SIP User-Agents for supporting calls, registration and message waiting
// indication.

//@@TID_PACKAGE_UASSP_CLASSES
//<GROUP TID_PACKAGE_UASSP>
//<TOPICORDER 1>
//<TITLE SceUaSsp Classes>

//@@TID_PACKAGE_UASSP_STRUCTSENUMS
//<GROUP TID_PACKAGE_UASSP>
//<TOPICORDER 2>
//<TITLE SceUaSsp Structs & Enums>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.


//@@TID_PACKAGE_UASSP_VARS
//<GROUP TID_PACKAGE_UASSP>
//<TOPICORDER 3>
//<TITLE SceUaSsp Variables>


//@@TID_PACKAGE_UASSP_MACROS
//<GROUP TID_PACKAGE_UASSP>
//<TOPICORDER 4>
//<TITLE SceUaSsp Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_MSP
//<TOPICORDER 8>
//<TITLE MSP APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine Multimedia Service Provider (MSP) package,
// which is located under the SceMsp directory.
//
// The MSP package offers components implementing the services required to
// handle media negotiation using SDP.

//@@TID_PACKAGE_MSP_CLASSES
//<GROUP TID_PACKAGE_MSP>
//<TOPICORDER 1>
//<TITLE MSP Classes>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.
//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_CMDDIGITSINTERFACE
//<TOPICORDER 9>
//<TITLE Command Digits Interface APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine Command Digits Interface (CDI) package, which is located
// under the SceCmDigitsInterface directory. The CDI package is part of the FXS
// Call Manager option.
//
// The CDI package offers components implementing the collecting of digits
// according to configured dialing rules.

//@@TID_PACKAGE_CMDDIGITSINTERFACE_CLASSES
//<GROUP TID_PACKAGE_CMDDIGITSINTERFACE>
//<TOPICORDER 1>
//<TITLE Command Digits Interface Classes>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_CMDDIGITSINTERFACE_STRUCTSENUMS
//<GROUP TID_PACKAGE_CMDDIGITSINTERFACE>
//<TOPICORDER 2>
//<TITLE SceCmdDigitsInterface Structs & Enums>


//@@TID_PACKAGE_CMDDIGITSINTERFACE_VARS
//<GROUP TID_PACKAGE_CMDDIGITSINTERFACE>
//<TOPICORDER 3>
//<TITLE SceCmdDigitsInterface Variables>


//@@TID_PACKAGE_CMDDIGITSINTERFACE_MACROS
//<GROUP TID_PACKAGE_CMDDIGITSINTERFACE>
//<TOPICORDER 4>
//<TITLE SceCmdDigitsInterface Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_FXSCALLMANAGER
//<TOPICORDER 10>
//<TITLE FXS Call Manager APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine FXS Call Manager (FXS CM) package, which is located under
// the SceFxsCallManager and the SceTei directories. The FXS CM package is part
// of the FXS Call Manager option.
//
// The FXS Call Manager package offers a complete state machine of an endpoint.
// This endpoint could be an FXS port or an IP Phone.
//
// The SceFxsCallManager directory contains the implementation of the FXS state
// machine. The SceTei directory contains interfaces that are used by the FXS
// Call Manager to report events to the media engine (IFxs et ITone). It also
// contains an interface IToneMgr so the media engine session can inform the FXS
// Call Manager about dialed digits and another interface IFxsMgr so the media
// engine can report hook events such as (off-hook, on-hook and flash-hook).

//@@TID_PACKAGE_FXSCALLMANAGER_CLASSES
//<GROUP TID_PACKAGE_FXSCALLMANAGER>
//<TOPICORDER 1>
//<TITLE FXS Call Manager Classes>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_FXSCALLMANAGER_STRUCTSENUMS
//<GROUP TID_PACKAGE_FXSCALLMANAGER>
//<TOPICORDER 2>
//<TITLE SceFxsCallManager Structs & Enums>


//@@TID_PACKAGE_FXSCALLMANAGER_VARS
//<GROUP TID_PACKAGE_FXSCALLMANAGER>
//<TOPICORDER 3>
//<TITLE SceFxsCallManager Variables>


//@@TID_PACKAGE_FXSCALLMANAGER_MACROS
//<GROUP TID_PACKAGE_FXSCALLMANAGER>
//<TOPICORDER 4>
//<TITLE SceFxsCallManager Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_FXSLINE
//<TOPICORDER 11>
//<TITLE FXS Line APIs>
//
// This section documents the application level APIs that are available from the
// SIP Client Engine FXS Line package, which is located under the SceFxsLine
// directory. The FXS Line package is part of the FXS Call Manager option.
//
// The FXS Line package offers is the glue code between the MSP, the UA-SSP, the
// FXS Call Manager and the Command Digits Interface as it manages the
// registration, the calls, MWI and the SDP media.

//@@TID_PACKAGE_FXSLINE_CLASSES
//<GROUP TID_PACKAGE_FXSLINE>
//<TOPICORDER 1>
//<TITLE FXS Line Classes>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_FXSLINE_STRUCTSENUMS
//<GROUP TID_PACKAGE_FXSLINE>
//<TOPICORDER 2>
//<TITLE SceFxsLine Structs & Enums>


//@@TID_PACKAGE_FXSLINE_VARS
//<GROUP TID_PACKAGE_FXSLINE>
//<TOPICORDER 3>
//<TITLE SceFxsLine Variables>


//@@TID_PACKAGE_FXSLINE_MACROS
//<GROUP TID_PACKAGE_FXSLINE>
//<TOPICORDER 4>
//<TITLE SceFxsLine Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_UAIMPP
//<TOPICORDER 12>
//<TITLE SceUaImpp APIs>
//
// This section documents the application level APIs that are available from the
// optional SIP Client Engine User-Agent Instant Messaging and Presence Provider
// (SceUaImpp) package, which is located under the SceUaImpp directory.
//
// The UA-IMPP package offers components implementing the services required by
// SIP User-Agents for supporting instant messaging, presence subscription,
// presence publication and the management of different documents relating with
// presence information.

//@@TID_PACKAGE_UAIMPP_CLASSES
//<GROUP TID_PACKAGE_UAIMPP>
//<TOPICORDER 1>
//<TITLE SceUaImpp Classes>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_UAIMPP_STRUCTSENUMS
//<GROUP TID_PACKAGE_UAIMPP>
//<TOPICORDER 2>
//<TITLE SceUaImpp Structs & Enums>

//@@TID_PACKAGE_UAIMPP_VARS
//<GROUP TID_PACKAGE_UAIMPP>
//<TOPICORDER 3>
//<TITLE SceUaImpp Variables>

//@@TID_PACKAGE_UAIMPP_MACROS
//<GROUP TID_PACKAGE_UAIMPP>
//<TOPICORDER 4>
//<TITLE SceUaImpp Macros>

//M5T_INTERNAL_USE_END

//@@TID_PACKAGE_SCEXML
//<TOPICORDER 13>
//<TITLE SceXml APIs>
//

//@@TID_PACKAGE_SCEXML_CLASSES
//<GROUP TID_PACKAGE_SCEXML>
//<TOPICORDER 1>
//<TITLE SceXml Classes>

//@@TID_PACKAGE_SCEXML_FUNCTIONS
//<GROUP TID_PACKAGE_SCEXML>
//<TOPICORDER 2>
//<TITLE SceXml Functions>

//@@TID_PACKAGE_SCEXML_STRUCTSENUMS
//<GROUP TID_PACKAGE_SCEXML>
//<TOPICORDER 3>
//<TITLE SceXml Structs & Enums>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_SCEXML_VARS
//<GROUP TID_PACKAGE_SCEXML>
//<TOPICORDER 4>
//<TITLE SceXml Variables>

//@@TID_PACKAGE_SCEXML_MACROS
//<GROUP TID_PACKAGE_SCEXML>
//<TOPICORDER 5>
//<TITLE SceXml Macros>

//M5T_INTERNAL_USE_END

