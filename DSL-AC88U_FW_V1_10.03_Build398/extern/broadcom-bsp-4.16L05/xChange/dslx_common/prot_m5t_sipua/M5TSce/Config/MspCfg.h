//==============================================================================
//==============================================================================
//
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
#ifndef MXG_MSPCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MSPCFG_H
//M5T_INTERNAL_USE_END

// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSMspCfg.h".
//
//     2 : You have created "PreMspCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while "PreMspCfg.h"
//         is included without any root directory.
#include "PreMspCfg.h"

MX_NAMESPACE_START(MXD_GNS)

#if !defined(MXD_MSP_DTMF_TEL_EVENT_MAX)
    #define MXD_MSP_DTMF_TEL_EVENT_MAX 15
#endif

#if !defined(MXD_MSP_NTE_ARRAY_CAPACITY)
    #define MXD_MSP_NTE_ARRAY_CAPACITY 256
#endif

#if !defined(MXD_MSP_RED_ARRAY_CAPACITY)
    #define MXD_MSP_RED_ARRAY_CAPACITY 256
#endif

//-- Below this is the documentation of the various configuration macros
//-- available.
#if 0
//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Enables the inclusion of "PostMspCfg.h" right at the end of
//  MSPCfg.h.
//
// Description:
//  Enables the inclusion of "PostMspCfg.h" right at the end of
//  MspCfg.h. "PostMspCfg.h" is an application provided file that can
//  contain additional configuration options to possibly override the
//  configuration found in PostMspCfg.h and MspCfg.h.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile to modify the default value.
//
// See Also:
//  PreMspCfg.h
//
//==============================================================================
#define MXD_POST_MSPCFG

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Specifies the maximum number of DTMF telephony events.
//
// Description:
//  This define specifies the maximum number of DTMF telephony events that
//  are supported by medias. The default value is 15.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile.
//
//==============================================================================
#define MXD_MSP_DTMF_TEL_EVENT_MAX 15

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Specifies the maximum number of named telephony events.
//
// Description:
//  This define specifies the maximum number of named telephony events that
//  are supported by medias. The default value is 256.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile to modify the default value.
//
//==============================================================================
#define MXD_MSP_NTE_ARRAY_CAPACITY 256

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
//  Summary:
//      Enables the mptime attribute in SDP packets.
//
//  Description:
//      When defined, MSP will generate mptime attribute in packet that it will
//      send. Even if it is not defined, mptime attribute will be used in
//      reception only. Be sure to always define MXD_SDP_ENABLE_MPTIME in
//      PreSdpParserCfg.h.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile to modify the default value.
//
//==============================================================================
#define MXD_MSP_ENABLE_MPTIME

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
//  Summary:
//      Enables generic silence suppression support.
//
//  Description:
//      When defined, MSP will try to negotiate generic silence suppression
//      support via the silenceSupp SDP attribute. Be sure to always define
//      MXD_SDP_SILENCE_SUPPRESSION_ENABLE_SUPPORT in PreSdpParserCfg.h.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile to modify the default value.
//
//==============================================================================
#define MXD_MSP_SILENCE_SUPPRESSION_ENABLE_SUPPORT

//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
//  Summary:
//      Enables support for different SDP previews and SDP final answer.
//
//  Description:
//      When defined, MSP will apply the configuration from the received SDP
//      payload every time, for non-reliable provisional responses. This
//      behaviour deviates from the standard behaviour from the Offer-Answer
//      specifications, which is to only apply the first SDP configuration and
//      ignore any subsequent ones.
//
// Location:
//  Define this in PreMspCfg.h or in your makefile to modify the default value.
//
//==============================================================================
#define MXD_MSP_DIFFERENT_SDP_PREVIEW_ANSWERS_ENABLE_SUPPORT

#endif //-- #if 0 for documentation.

MX_NAMESPACE_END(MXD_GNS)

#if defined(MXD_POST_MSPCFG)
#include "PostMspCfg.h"
#endif

#endif //-- #ifndef MXG_MSPCFG_H
