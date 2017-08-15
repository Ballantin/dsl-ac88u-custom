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
#ifndef MXG_SCECORECOMPONENTSCFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCECORECOMPONENTSCFG_H
//M5T_INTERNAL_USE_END

// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreSceCoreComponentsCfg.h".
//     2 : You have created "PreSceCoreComponentsCfg.h" but the compiler cannot
//         find it. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header files
//         are included by specifying the package name and filename (e.g.:
//         "SipCore/ISipContext.h"), which permits setting the include path to
//         the "Source" directory of the package only, while "PreSceCoreComponentsCfg.h"
//         is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreSceCoreComponentsCfg.h"

MX_NAMESPACE_START(MXD_GNS)

// Below this is the documentation of the various configuration macros
// available.
//---------------------------------------------------------------------
#if 0


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Enables the inclusion of the ISceReferrer component.
//
// Description:
//  Enables the inclusion of the ISceReferrer component and related files.
//
//  The ISceReferrer component can be used to do out-of-dialog transfers. It
//  can manage one client or server REFER at a time.
//
// Location:
//  Define this in PreSceCoreComponentsCfg.h.
//
// See Also:
//  ISceReferrer
//
//==============================================================================
#define MXD_SCE_ENABLE_REFERRER_COMPONENT_SUPPORT


//<GROUP TID_COMPILE_CONFIG_MACROS_CORE>
//
// Summary:
//  Enables the inclusion of "PostSceCoreComponentsCfg.h" right at the end of
//  SceCoreComponentsCfg.h.
//
// Description:
//  Enables the inclusion of "PostSceCoreComponentsCfg.h" right at the end of
//  SceCoreComponentsCfg.h. "PostSceCoreComponentsCfg.h" is an application provided file that can
//  contain additional configuration options to possibly override the
//  configuration found in PreSceCoreComponentsCfg.h and SceCoreComponentsCfg.h.
//
// Location:
//  Define this in PreSceCoreComponentsCfg.h or in your makefile.
//
// See Also:
//  PreSceCoreComponentsCfg.h
//
//==============================================================================
#define MXD_POST_SCECORECOMPONENTSCFG

#endif // #if 0 for documentation.



MX_NAMESPACE_END(MXD_GNS)


#if defined(MXD_POST_SCECORECOMPONENTSCFG)
#include "PostSceCoreComponentsCfg.h"
#endif


#endif // #ifndef MXG_SCECORECOMPONENTSCFG_H


