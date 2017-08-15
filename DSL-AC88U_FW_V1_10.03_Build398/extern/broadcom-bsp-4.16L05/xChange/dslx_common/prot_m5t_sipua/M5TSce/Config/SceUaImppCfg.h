//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
#ifndef MXG_SCEUAIMPPCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCEUAIMPPCFG_H
//M5T_INTERNAL_USE_END

// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSceUaImppCfg.h".
//
//     2 : You have created "PreSceUaImppCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while
//         "PreSceUaImppCfg.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreSceUaImppCfg.h"

MX_NAMESPACE_START(MXD_GNS)

// Below this is the documentation of the various configuration macros
// available.
#if 0 // for documentation

//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Enables the inclusion of "PostSceUaImppCfg.h" right at the end of
//  SceUaImppCfg.h.
//
// Description:
//  Enables the inclusion of "PostSceUaImppCfg.h" right at the end of
//  SceUaImppCfg.h. "PostSceUaImppCfg.h" is an application provided file
//  that can contain additional configuration options to possibly override the
//  configuration found in PreSceUaImppCfg.h and SceUaImppCfg.h.
//
// Location:
//  Define this in PreSceUaImppCfg.h or in your makefile.
//
// See Also:
//  PreSceUaImppCfg.h
//
//==============================================================================
#define MXD_POST_SCEUAIMPPCFG


//<GROUP TID_COMPILE_CONFIG_MACROS_UAIMPP>
//
// Summary:
//  Enables Xcap support.
//
// Description:
//  This configuration macro enables the support of Xcap throughout UA-IMPP.
//
// Location:
//  Define this in PreSceUaImppCfg.h.
//
//------------------------------------------------------------------------------
#define MXD_SCEUAIMPP_XCAP_ENABLE_SUPPORT


#endif // #if 0 for documentation.


MX_NAMESPACE_END(MXD_GNS)


#if defined(MXD_POST_SCEUAIMPPCFG)
#include "PostSceUaImppCfg.h"
#endif

#endif //-- #ifndef MXG_SCEUAIMPPCFG_H

