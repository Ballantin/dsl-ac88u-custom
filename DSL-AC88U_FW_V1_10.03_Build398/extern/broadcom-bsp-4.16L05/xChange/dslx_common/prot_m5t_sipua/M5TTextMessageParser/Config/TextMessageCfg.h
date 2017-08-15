//==============================================================================
//==============================================================================
//
//      Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_TEXTMESSAGECFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_TEXTMESSAGECFG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//@@PreTextMessageCfg.h
//<GROUP CONFIG_MACROS>
//<TOPICORDER 0>
//
// Description:
// If the compiler complains that it can not include the file below, it may be
// because:
//
//     1. You have not created this file to configure your application for
//        using this package. The documentation included with this package
//        explains how the configuration of the various M5T products works.
//        Please refer to this documentation and create "PreTextMessageCfg.h".
//
//     2. You have created "PreTextMessageCfg.h" but it is not found by the
//        compiler. This file must be located in a directory found within the
//        include path defined for this build. Note that other M5T header
//        files are included by specifying the package name and filename
//        (e.g.: "SipUserAgent/IContext.h"), which permits setting the include
//        path to the "Source" directory of the package only, while
//        "PreTextMessageCfg.h" is included without any root directory.
//
//M5T_INTERNAL_USE_BEGIN
#include "PreTextMessageCfg.h"

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

#ifndef MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_HEADERS_PER_PACKET
    #define MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_HEADERS_PER_PACKET 20
#endif

#ifndef MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_PARAMETERS_PER_HEADER
    #define MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_PARAMETERS_PER_HEADER 10
#endif

#if defined(MXD_POST_TEXTMESSAGECFG)
#include "PostTextMessageCfg.h"
#endif

MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

// Doc-O-Matic documentation section
#if 0

//<GROUP CONFIG_MACROS>
//<TOPICORDER 1>
//
// Summary:
//  Enables the inclusion of "PostTextMessageCfg.h".
//
// Description:
//  Enables the inclusion of "PostTextMessageCfg.h" right at the end of
//  TextMessageCfg.h. "PostTextMessageCfg.h" is an application-provided file
//  that can contain additional configuration options to possibly override the
//  configurations found in PreTextMessageCfg.h and TextMessageCfg.h.
//
// Location:
//  Define this in PreTextMessageCfg.h or in your makefile.
//
// See Also:
//  PreTextMessageCfg.h
//
//==============================================================================
#define MXD_POST_TEXTMESSAGECFG

//<GROUP CONFIG_MACROS>
//<TOPICORDER 4>
//
// Summary:
//  Defines the initial capacity of the containers used to hold CTxtHeader
//  objects.
//
// Description:
//  Defines the initial capacity of the containers used to hold CTxtHeader
//  objects. This value is used to avoid unnecessary memory reallocations while
//  adding headers in packets or during packet parsing.
//
//  However, note that packets support holding more headers than this
//  defined value. Because the header list is only initialized with this value
//  and can grow if necessary.
//
// Location:
//  Define this in PreTextMessageCfg.h or in your makefile to override the
//  default value.
//
//==============================================================================
#define MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_HEADERS_PER_PACKET 20

//<GROUP CONFIG_MACROS>
//<TOPICORDER 3>
//
// Summary:
//  Defines the initial capacity of the containers used to hold CTxtParameter
//  objects.
//
// Description:
//  Defines the initial capacity of the containers used to hold CTxtParameter
//  objects. This value is used to avoid unnecessary memory reallocations while
//  adding parameters in headers or during header parsing.
//
//  However, note that headers support holding more parameters than this
//  defined value. Because the parameter list is only initialized with this
//  value and can grow if necessary.
//
// Location:
//  Define this in PreTextMessageCfg.h or in your makefile to override the
//  default value.
//
//==============================================================================
#define MXD_TEXT_MESSAGE_DEFAULT_NUMBER_OF_PARAMETERS_PER_HEADER 10

#endif //-- #if 0

#endif // #ifndef MXG_TEXTMESSAGECFG_H

