//==============================================================================
//==============================================================================
//
//     Copyright(c) 2008 Media5 Corporation. ("Media5")
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
#ifndef MXG_XCAPCLIENTCURLCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_XCAPCLIENTCURLCFG_H
//M5T_INTERNAL_USE_END


//@@TID_COMPILE_CONFIG_HOWTO_UASSP
//<TITLE Configuring the XCAP Client with "PreXCapClientCurlCfg.h">
//<GROUP TID_COMPILE_CONFIG>
//<TOPICORDER 2>
//
// The XCAP Client comes with the file "Config/XcapClientCurlCfg.h", which
// defines many compilation configuration options and values used throughout the
// source code. Generally, these values need updating for the specific
// application being developed with the XCAP Client if using the CUrl HTTP
// library.
//
// To update these default values, you must create the "PreXcapClientCurlCfg.h"
// file with the updated configuration options for your application.
// "PreXcapClientCurlCfg.h" is always included first by "XcapClientCurlCfg.h" to
// retrieve application-specific configurations, and then the default
// configuration options found in "Config/XcapClientCurlCfg.h" are applied for
// all items that have not been configured by the application.
//
// "PreXcapClientCurlCfg.h" is not packaged with the XCAP Client and must be
// created for the specific application being developed. This file must be
// placed somewhere in the compiler search path to permit the retrieval of the
// application-specific configuration options by the XCAP Client.
//
//==============================================================================
//==============================================================================


// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreXcapClientCurlCfg.h".
//
//     2 : You have created "PreXcapClientCurlCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while
//         "PreXcapClientCurlCfg.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreXcapClientCurlCfg.h"

#ifndef MXG_CURL_MAX_HTTP_TRANSFER_SIZE
#define MXG_CURL_MAX_HTTP_TRANSFER_SIZE 2000
#endif


MX_NAMESPACE_START(MXD_GNS)

// Below this is the documentation of the various configuration macros
// available.
//---------------------------------------------------------------------
#if 0

//<GROUP TID_COMPILE_CONFIG_MACROS>
//
// Summary:
//  Enables the inclusion of "PostXcapClientCurlCfg.h" right at the end of
//  XcapClientCurlCfg.h.
//
// Description:
//  Enables the inclusion of "PostXcapClientCurlCfg.h" right at the end of
//  XcapClientCurlCfg.h. "PostXcapClientCurlCfg.h" is an application-provided file that
//  can contain additional configuration options to possibly override the
//  configuration found in PreXcapClientCurlCfg.h and XcapClientCurlCfg.h.
//
// Location:
//  Define this in PreXcapClientCurlCfg.h or in your makefile.
//
// See Also:
//  PreXcapClientCurlCfg.h
//
//==============================================================================
#define MXD_POST_XCAPCLIENTCURLCFG

//<GROUP MXG_CURL_MAX_HTTP_TRANSFER_SIZE>
//
// Summary:
//  Specifies the maximum size of a HTTP transfer.
//
// Description:
//  Specifies the maximum allowed size (in bytes) of a HTTP transfer. If a
//  transfer size exceeds this value, the transfer will be aborted. Note that
//  the size of a transfer is not always known and in such a case this define
//  has no effect on the transfer if the transfer ends up being bigger.
//
//  The default value for this is 2000.
//
// Location:
//  Define this in PreXcapClientCurlCfg.h or in your makefile.
//
// See Also:
//  PreXcapClientCurlCfg.h
//
//==============================================================================
#define MXG_CURL_MAX_HTTP_TRANSFER_SIZE

#endif // #if 0 for documentation.


MX_NAMESPACE_END(MXD_GNS)


#if defined(MXD_POST_XCAPCLIENTCFG)
#include "PostXcapClientCurlCfg.h"
#endif


#endif // #ifndef MXG_XCAPCLIENTCURLCFG_H

