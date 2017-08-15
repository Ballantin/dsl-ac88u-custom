//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_MXFAULT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXFAULT_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_FAULT_HANDLER_ENABLE_SUPPORT
#endif

#if defined(MXD_FAULT_HANDLER_ENABLE_SUPPORT)

#if defined (MXD_OS_WINDOWS)   || \
    defined(MXD_OS_WINDOWS_CE) || \
    defined(MXD_OS_SYMBIAN)    || \
    defined(MXD_OS_NONE)       || \
    (defined(MXD_OS_VXWORKS) && defined(MXD_ARCH_PPC))
    #error "The fault handler is not implemented for the current OS. Please undefine MXD_FAULT_HANDLER_ENABLE_SUPPORT."
#else

// Summary:
// Installs fault handler.
//---------------------------
bool InstallGenericFaultHandler();

// Summary:
// Uninstalls fault handler.
//---------------------------
bool UnInstallGenericFaultHandler();

#endif  // #if defined (MXD_OS_WINDOWS)   ||
        //     defined(MXD_OS_WINDOWS_CE) ||
        //     defined(MXD_OS_SYMBIAN)    ||
        //     defined(MXD_OS_NONE)       ||
        //     (defined(MXD_OS_VXWORKS) && defined(MXD_ARCH_PPC))

//===========================================================================
//====  MACROS  =============================================================
//===========================================================================

#endif // #if defined(MXD_FAULT_HANDLER_ENABLE_SUPPORT)

#endif // #ifndef MXG_MXFAULT_H

