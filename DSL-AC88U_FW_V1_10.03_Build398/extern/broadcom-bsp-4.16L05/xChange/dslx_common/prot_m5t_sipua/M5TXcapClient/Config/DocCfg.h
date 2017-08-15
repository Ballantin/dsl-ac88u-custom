//==============================================================================
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

//@@!!WELCOME
//<title M5T XCAP Client v1.0 - API Reference>
//
//Summary:
//Proprietary & Confidential
//==============================================================================

//@@!!COPYRIGHT
//<TITLE Legal Information>
//
//* Legal Information *
//
// Copyright(c) 2008 Media5 Corporation. ("Media5")
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
//Welcome to the M5T XCAP Client API Reference. This document provides quick
//and simple references to the usage of the XCAP Client. It should be used by
//programmers who already have some knowledge of the XCAP Client.
//
//All modifications to this API Reference are tracked through CRs (Change
//Requests) and are documented in the release notes associated with this product.
//




//M5T_INTERNAL_USE_BEGIN
// This file is simply used to define the documentation structure of the M5T
// XCAP Client.
//---------------------------------------------------------------------------


//
//  Topic Hierarchy for XCAP Client project
//
//   - Table of Content                  []
//   - Introduction                      [Introduction]
//   - Overview                          [TID_OVERVIEW]
//   - Compile-Time Configuration        [TID_COMPILE_CONFIG]
//      - Configuration Mechanism        [TID_COMPILE_CONFIG_HOWTO]
//      - Configuration Macros           [TID_COMPILE_CONFIG_MACROS]
//          - Services Configuration     [TID_COMPILE_CONFIG_MACROS_SVC_CONFIG]
//          - XYZ Configuration          [TID_COMPILE_CONFIG_MACROS_XYZCONFIG]
//          - ABC Configuration          [TID_COMPILE_CONFIG_MACROS_ABCCONFIG]
//   - Tracing Information               [TID_TRACING_INFORMATION]
//      - Mechanism                      [TID_TRACING_INFORMATION_MECHANISM]
//      - Tracing Nodes                  [TID_TRACING_INFORMATION_TRACINGNODES]
//   - Package Startup                   [TID_PACKAGE_STARTUP]
//      - Classes                        [TID_PACKAGE_STARTUP_CLASSES]
//   - Package XCAP Client               [TID_PACKAGE_XCAP]
//      - Classes                        [TID_PACKAGE_XCAP_CLASSES]
//      - Functions                      [TID_PACKAGE_XCAP_FUNCTIONS]
//      - Struct & Enums                 [TID_PACKAGE_XCAP_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_XCAP_VARS]
//      - Macros                         [TID_PACKAGE_XCAP_MACROS]
//   - Package HTTP XCAP Client          [TID_PACKAGE_XCAP_HTTP]
//      - Classes                        [TID_PACKAGE_XCAP_HTTP_CLASSES]
//      - Functions                      [TID_PACKAGE_XCAP_HTTP_FUNCTIONS]
//      - Struct & Enums                 [TID_PACKAGE_XCAP_HTTP_STRUCTSENUMS]
//      - Variables                      [TID_PACKAGE_XCAP_HTTP_VARS]
//      - Macros                         [TID_PACKAGE_XCAP_HTTP_MACROS]
//
//M5T_INTERNAL_USE_END


//@@TID_OVERVIEW
//<TOPICORDER 2>
//<TITLE Product Overview>
//
// The M5T XCAP Client is designed to read, write and modify configuration data
// stored in a XML document on a XCAP server. These XML documents contain data
// associated with user presence state. Note that it is not yet possible to modify
// or retrieve elements or nodes within a XML document. These modifications are made
// by putting, getting and deleting XML documents from a XCAP server through
// HTTP or HTTPS requests. The allowed HTTP requests are GET, PUT and
// DELETE. The M5T XCAP client is based on RFC 4825.
//


//@@TID_COMPILE_CONFIG
//<TOPICORDER 3>
//<TITLE Compile-Time Configuration>

//M5T_INTERNAL_USE_BEGIN
//NOTE: This topic is defined in XcapClientCfg.h
//@@TID_COMPILE_CONFIG_HOWTO
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 1>
//M5T_INTERNAL_USE_END

//@@TID_COMPILE_CONFIG_MACROS
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 2>
//<TITLE Configuration Macros>

//@@TID_TRACING_INFORMATION
//<TOPICORDER 4>
//<TITLE Tracing in the XCAP Client>

//@@TID_TRACING_INFORMATION_MECHANISM
//<GROUP TID_TRACING_INFORMATION>
//<TOPICORDER 1>
//<TITLE The M5T Tracing Mechanism>
//
// The M5T XCAP Client integrates the M5T tracing mechanism, which uses
// tracing nodes to allow the application to control which part of the M5T
// component can output traces. See MxTraceEnableNode and MxTraceDisableNode in
// the M5T Framework API reference document to find out more on how to enable
// and disable tracing through tracing nodes.

//@@TID_TRACING_INFORMATION_TRACINGNODES
//<GROUP TID_TRACING_INFORMATION>
//<TOPICORDER 2>
//<TITLE M5T XCAP Client Tracing Nodes>
//
// The root node of the M5T XCAP Client is "XcapClient". The following
// tracing nodes are available.
//
// <CODE>
//      /XcapClient
//      /XcapClient/Xcap
//      /XcapClient/Xcap/Ecom
//      /XcapClient/Http
//      /XcapClient/Http/Ecom
// </CODE>


//@@TID_PACKAGE_STARTUP
//<TOPICORDER 5>
//<TITLE Startup APIs>
//
// This section documents the application level APIs that are available from the
// XCAP Client Startup package, which is located under the Startup directory.
//
// The Startup offers a set of classes that are responsible to initialize and
// terminate different parts of the XCAP Client before it can be used.
// Since the higher-level parts always initialize their dependencies, it is
// not required to explicitely initialize all packages of the XCAP Client.
// An application only has to initialize the XCAP-CLIENT package using
// CXcapClientInitializer.
//

//@@TID_PACKAGE_STARTUP_CLASSES
//<GROUP TID_PACKAGE_STARTUP>
//<TOPICORDER 1>
//<TITLE Startup Classes>


//@@TID_PACKAGE_XCAP
//<TOPICORDER 6>
//<TITLE XCAP APIs>
//
// This section documents the application level APIs that are available from the
// XCAP Client XCAP package, which is located under the Xcap directory.
//

//@@TID_PACKAGE_XCAP_CLASSES
//<GROUP TID_PACKAGE_XCAP>
//<TOPICORDER 1>
//<TITLE XCAP Client Classes>


//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_XCAP_FUNCTIONS
//<GROUP TID_PACKAGE_XCAP>
//<TOPICORDER 2>
//<TITLE XCAP Client Functions>

//@@TID_PACKAGE_XCAP_STRUCTSENUMS
//<GROUP TID_PACKAGE_XCAP>
//<TOPICORDER 3>
//<TITLE XCAP Client Structs & Enums>

//@@TID_PACKAGE_XCAP_VARS
//<GROUP TID_PACKAGE_XCAP>
//<TOPICORDER 4>
//<TITLE XCAP Client Variables>


//@@TID_PACKAGE_XCAPCLIENT_MACROS
//<GROUP TID_PACKAGE_XCAP>
//<TOPICORDER 5>
//<TITLE XCAP Client Macros>

//M5T_INTERNAL_USE_END


//@@TID_PACKAGE_XCAP_HTTP
//<TOPICORDER 7>
//<TITLE XCAP HTTP APIs>
//
// This section documents the application level APIs that are available from the
// XCAP Client HTTP package, which is located under the Http directory.
//

//@@TID_PACKAGE_XCAP_HTTP_CLASSES
//<GROUP TID_PACKAGE_XCAP_HTTP>
//<TOPICORDER 1>
//<TITLE XCAP HTTP Client Classes>

//@@TID_PACKAGE_XCAP_HTTP_VARS
//<GROUP TID_PACKAGE_XCAP_HTTP>
//<TOPICORDER 2>
//<TITLE XCAP HTTP Client Variables>

//M5T_INTERNAL_USE_BEGIN
//-- These sections are unused for now they are left for convenience if they
//-- ever become useful.

//@@TID_PACKAGE_XCAP_HTTP_FUNCTIONS
//<GROUP TID_PACKAGE_XCAP_HTTP>
//<TOPICORDER 3>
//<TITLE XCAP HTTP Client Functions>

//@@TID_PACKAGE_XCAP_HTTP_STRUCTSENUMS
//<GROUP TID_PACKAGE_XCAP_HTTP>
//<TOPICORDER 4>
//<TITLE XCAP HTTP Client Structs & Enums>


//@@TID_PACKAGE_XCAP_HTTP_MACROS
//<GROUP TID_PACKAGE_XCAP_HTTP>
//<TOPICORDER 5>
//<TITLE XCAP HTTP Client Macros>

//M5T_INTERNAL_USE_END













