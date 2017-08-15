//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//     Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_MXCONFIG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXCONFIG_H
//M5T_INTERNAL_USE_END

//==============================================================================
//==============================================================================
//
// NOTE: This file MUST be the first header file included by *ALL* of M5T's
// header files, including interfaces. The reason for this is to prevent users
// of the header file from having to include this specific MxConfig.h file, as
// it is somehow always needed for the definition of IN, OUT and other macros
// that are spread out across all of M5T's header files. All header files must
// be stand alone, so they must include the proper headers and not rely
// on someone else doing it for them.
//
//==============================================================================
//==============================================================================


// If the compiler complains that it cannot include the file below, it may be
// because:
//
//     1 : You have not created this file to configure your application for
//         using this package. The documentation included with this package
//         explains how the configuration of the various M5T products works.
//         Please refer to this documentation and create "PreMxConfig.h".
//
//     2 : You have created "PreMxConfig.h" but it is not found by the
//         compiler. This file must be located in a directory found within the
//         include path defined for this build. Note that other M5T header
//         files are included by specifying the package name and filename
//         (e.g.: "SipUserAgent/IContext.h") which permits setting the include
//         path to the "Source" directory of the package only, while
//         "PreMxConfig.h" is included without any root directory.
//
//------------------------------------------------------------------------------
#include "PreMxConfig.h"

//==============================================================================
//====  GENERAL DEFINES, CONSTANTS AND COMPILE SWITCHS  ========================
//==============================================================================

// Compiler Type Definitions.

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC) || defined(__GNUC__)
    #if !defined(__GNUC__)
        #error "Compiler type definition mismatch!"
    #else
        #undef MXD_COMPILER_GNU_GCC
        #if !defined( __GNUC_PATCHLEVEL__)
            #define __GNUC_PATCHLEVEL__ 0
        #endif
        #define MXD_COMPILER_GNU_GCC ((__GNUC__ << 16) | \
                                      (__GNUC_MINOR__ << 8) | \
                                      (__GNUC_PATCHLEVEL__))
    #endif

#elif defined(MXD_COMPILER_MS_VC) || defined(_MSC_VER)
    #if !defined(_MSC_VER)
        #error "Compiler type definition mismatch!"
    #else
        #undef MXD_COMPILER_MS_VC
        #define MXD_COMPILER_MS_VC _MSC_VER
    #endif

    // Disable deprecated warnings in MSVC8.
    #if (_MSC_VER >= 1400)
        #ifndef _CRT_SECURE_NO_DEPRECATE
            #define _CRT_SECURE_NO_DEPRECATE
        #endif

        #ifndef _CRT_NONSTDC_NO_DEPRECATE
            #define _CRT_NONSTDC_NO_DEPRECATE
        #endif
    #endif

#elif defined(MXD_COMPILER_WINSCW) || defined(__WINSCW__)
    // Fetch WINSCW compiler version here.
    #ifndef MXD_COMPILER_WINSCW
        #define MXD_COMPILER_WINSCW
    #endif

#elif defined(MXD_COMPILER_DIAB)
    // Fetch DIAB compiler version here.

#elif defined(MXD_COMPILER_TI_CL6X)
    // Fetch Code Composer Studio version here.

#else
    // Nothing.

#endif

// Compiler Version Definitions.
#undef MXD_COMPILER_GNU_GCC_4_1
#define MXD_COMPILER_GNU_GCC_4_1 ((4 << 16) | (1 << 8))
#undef MXD_COMPILER_GNU_GCC_3_4
#define MXD_COMPILER_GNU_GCC_3_4 ((3 << 16) | (4 << 8))
#undef MXD_COMPILER_GNU_GCC_3_3
#define MXD_COMPILER_GNU_GCC_3_3 ((3 << 16) | (3 << 8))
#undef MXD_COMPILER_GNU_GCC_3_2
#define MXD_COMPILER_GNU_GCC_3_2 ((3 << 16) | (2 << 8))
#undef MXD_COMPILER_GNU_GCC_3_1
#define MXD_COMPILER_GNU_GCC_3_1 ((3 << 16) | (1 << 8))
#undef MXD_COMPILER_GNU_GCC_3_0
#define MXD_COMPILER_GNU_GCC_3_0 (3 << 16)
#undef MXD_COMPILER_GNU_GCC_2_95
#define MXD_COMPILER_GNU_GCC_2_95 ((2 << 16) | (95 << 8))

// WRS gcc 2.96+ can identify itself as 2.9 or 2.96.
// - It identifies itself as 2.9 in our test framework.
// - It identifies itself as 2.96 in some client implementations.
// The reason for this is still unknown.
#undef MXD_COMPILER_GNU_GCC_2_96
#define MXD_COMPILER_GNU_GCC_2_96 ((2 << 16) | (96 << 8))
#if defined(MXD_COMPILER_GNU_GCC)
    #if (__GNUC__ == 2 && __GNUC_MINOR__ == 9)
        #undef MXD_COMPILER_GNU_GCC_2_96
        #define MXD_COMPILER_GNU_GCC_2_96 ((2 << 16) | (9 << 8))
    #endif
#endif

#undef MXD_COMPILER_GNU_GCC_2_7
#define MXD_COMPILER_GNU_GCC_2_7 ((2 << 16) | (7 << 8))

#undef MXD_COMPILER_MS_VC_6
#define MXD_COMPILER_MS_VC_6 1200
#undef MXD_COMPILER_MS_VC_7
#define MXD_COMPILER_MS_VC_7 1300
#undef MXD_COMPILER_MS_VC_8
#define MXD_COMPILER_MS_VC_8 1400

// Operating System Version Definitions.
#undef  MXD_OS_LINUX_2_4
#define MXD_OS_LINUX_2_4 0x020400
#undef  MXD_OS_LINUX_2_6
#define MXD_OS_LINUX_2_6 0x020600
#undef  MXD_OS_VXWORKS_5_4
#define MXD_OS_VXWORKS_5_4 0x050400
#undef  MXD_OS_VXWORKS_5_5
#define MXD_OS_VXWORKS_5_5 0x050500
#undef  MXD_OS_VXWORKS_6_0
#define MXD_OS_VXWORKS_6_0 0x060000
#undef  MXD_OS_VXWORKS_6_5
#define MXD_OS_VXWORKS_6_5 0x060500
#undef  MXD_OS_VXWORKS_6_6
#define MXD_OS_VXWORKS_6_6 0x060600
#undef  MXD_OS_WINDOWS_2K
#define MXD_OS_WINDOWS_2K 0x0500
#undef  MXD_OS_WINDOWS_XP
#define MXD_OS_WINDOWS_XP 0x0501
#undef  MXD_OS_WINDOWS_2K3
#define MXD_OS_WINDOWS_2K3 0x0502
#undef  MXD_OS_WINDOWS_VISTA
#define MXD_OS_WINDOWS_VISTA 0x0600

// Operating System Type Definitions.
#if defined(MXD_OS_NONE)
    // No operating system used.

#elif defined(MXD_OS_ANDROID)
    // The Android section MUST be before the Linux section, otherwise Android is wrongly detected as Linux.

    // This is used to determine the Linux version at runtime in the Kernel/COsVersion class.
    #include <sys/utsname.h>

#elif defined(MXD_OS_LINUX) || defined(__linux__)
    #if !defined(__linux__)
        #error "OS type definition mismatch!"
    #endif
    #include <linux/version.h>
    #if !defined(LINUX_VERSION_CODE)
        #error "Could not determine Linux kernel version. "\
               "Please define LINUX_VERSION_CODE as 0xMMmmpp. "
    #endif
    #undef MXD_OS_LINUX
    #define MXD_OS_LINUX LINUX_VERSION_CODE

    // This is used to determine the Linux version at runtime in the
    // Kernel/COsVersion class.
    #include <sys/utsname.h>

#elif defined (MXD_OS_SOLARIS) || defined(__sun)
    #if !defined(__sun)
        #error "OS type definition mismatch!"
    #endif

    // NOTE: Solaris operating system version should be determined at runtime
    // using the uname method that returns a utsname structure. The OS version
    // will be in the release member of the returned utsname structure.
    #include <sys/utsname.h>

#elif defined(MXD_OS_VXWORKS) || defined(__vxworks)
    // NOTE: could not determine VxWorks OS target through GNU GCC compiler.
    // __vxworks is only defined in the WRS 2.96 compiler.
    // MUST be manually defined in Makefile.
    #include <vxWorks.h>
    #include <version.h>
    // Take for granted that this nomenclature is always used by VxWorks
    #if !defined(VXWORKS_VERSION)
        #error "OS type definition mismatch!"
    #endif

    // NOTE: The VXWORKS_VERSION macro cannot be used at compile time to
    // determine the VxWorks version, since it is defined to a string. It is
    // therefore necessary to define it using MXD_VXWORKS_VERSION.
    #undef MXD_OS_VXWORKS
    #define MXD_OS_VXWORKS MXD_VXWORKS_VERSION

#elif defined(MXD_OS_WINDOWS_CE) || defined(_WIN32_WCE)
    #if !defined(_WIN32_WCE)
        #error "OS type definition mismatch!"
    #endif
    // NOTE: Windows operating system version should be determined at run-time.
    #undef MXD_OS_WINDOWS_CE
    #define MXD_OS_WINDOWS_CE

#elif defined(MXD_OS_SYMBIAN)
    // MXD_SYMBIAN_VERSION MUST be manually defined in compiler flags.
    #if !defined(MXD_SYMBIAN_VERSION)
        #error "Symbian version not defined!"
    #elif MXD_SYMBIAN_VERSION < 0x090100
        #error "Unsupported version of Symbian"
    #else
        #undef MXD_OS_SYMBIAN
        #define MXD_OS_SYMBIAN MXD_SYMBIAN_VERSION
    #endif

    // MXD_OS_SYMBIAN must be before MXD_OS_WINDOWS otherwise the emulator
    // compiler will think it is in Windows since _WIN32 is defined.

#elif defined(MXD_OS_WINDOWS) || defined(_WIN32)
    #if !defined(_WIN32)
        #error "OS type definition mismatch!"
    #endif

    // This macro is not defined inside the Windows SDK and must be defined
    // prior to the inclusion of any SDK header file.
    //
    // Note: the WINVER macro cannot be used to define _WIN32_WINNT, since it is
    // only defined in the SDK's windows.h header file. If the latter file is
    // included here, winbase.h, included by windows.h, will not define
    // TryEnterCriticalSection, needed by Cmutex. The _WIN32_WINNT macro is
    // usually defined in the pre-compiled header.
    //--------------------------------------------------------------------------

    #if !defined(_WIN32_WINNT)
        #if defined(MXD_LIB_SDK_WINDOWSSDK)
            // Use MXD_LIB_SDK_WINDOWSSDK only if you are using WIndows Vista's
            // SDK. If this file cannot be included it is probably because the
            // sdk used is not the right one.
            #include "sdkddkver.h"
        #else
            #define _WIN32_WINNT  MXD_OS_WINDOWS_2K
        #endif
    #elif (_WIN32_WINNT < 0x0500)
        #error "Windows version must be Windows 2000 or more recent!"
    #endif

    #undef MXD_OS_WINDOWS
    #define MXD_OS_WINDOWS _WIN32_WINNT

#elif defined(MXD_OS_NUCLEUS)
    // MXD_NUCLEUS_VERSION MUST be manually defined in compiler flags.
    #if !defined(MXD_NUCLEUS_VERSION)
        #error "Nucleus version not defined!"
    #else
        #undef MXD_OS_NUCLEUS
        #define MXD_OS_NUCLEUS MXD_NUCLEUS_VERSION
    #endif

#elif defined(MXD_OS_DARWIN)

// BRCM_CUSTOM - [add] Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] Add ECOS support.
#else
// BRCM_CUSTOM - [mod] Add ECOS support.
    #error "Could not determine operating system type. Please define "\
           "MXD_OS_LINUX or "      \
           "MXD_OS_SOLARIS or"     \
           "MXD_OS_DARWIN or "     \
           "MXD_OS_VXWORKS or "    \
           "MXD_OS_WINDOWS or "    \
           "MXD_OS_WINDOWS_CE or " \
           "MXD_OS_SYMBIAN or "    \
           "MXD_OS_NUCLEUS or "    \
           "MXD_OS_BOS_ECOS or "   \
           "MXD_OS_NONE."
// BRCM_CUSTOM - [end] Add ECOS support.
#endif

// Librairies Type Definitions
#if defined(MXD_OS_LINUX)
    #include "features.h"

    #if defined(__UCLIBC__)
        #define MXD_LIB_GNU_UCLIBC
    #elif defined(__GLIBC__)
        #define MXD_LIB_GNU_LIBC
    #endif
#endif  //-- #if defined(MXD_OS_LINUX)

// Processor Architecture Type Definitions.
#if   defined(MXD_ARCH_PPC)
    #if !defined(__powerpc__)
        #error "Processor Architecture type definition mismatch!"
    #endif

#elif defined(MXD_ARCH_MIPS)
    #if !defined(__mips__)
        #error "Processor Architecture type definition mismatch!"
    #endif

#elif defined(MXD_ARCH_IX86)
    #if !defined(__i386__) && \
        !defined(_M_IX86)
       #error "Processor Architecture type definition mismatch!"
    #endif

#elif defined(MXD_ARCH_AMD64)
    #if !defined(__x86_64__) || \
        !defined(__amd64__)
        #error "Processor Architecture type definition mismatch!"
    #endif

#elif defined(MXD_ARCH_SPARC)
    #if !defined(__sparc__)
        #error "Processor Architecture type definition mismatch!"
    #endif


#elif defined(MXD_ARCH_ARM)

#elif defined(MXD_ARCH_NIOS2)

#elif defined(__powerpc__)
    #define MXD_ARCH_PPC

#elif defined(__mips__)
    #define MXD_ARCH_MIPS

#elif defined(__arm__)
    #define MXD_ARCH_ARM

#elif defined(__i386__) || /*! From GNU CC */ \
      defined(_M_IX86)     /*! From MSVC++ */
    #define MXD_ARCH_IX86

#elif defined(__x86_64__) || \
      defined(__amd64__)
    #define MXD_ARCH_AMD64

#elif defined(__sparc__)  /*! From GNU CC */
    #define MXD_ARCH_SPARC
#else
    // Nothing.
#endif

// 32 and 64-bit data models.
//
// So far, only ILP32 and LP64 data models are supported.
// Furthermore, LP64 data model is only supported and detected for the
// following combination of compiler, OS and architecture:
// GCC, LINUX and AMD64.
// If the LP64 combination is not detected, ILP32 is chosen.
//
// Eventually, LLP64 data model used by windows could be detected as below:
// #elif defined(MXD_COMPILER_MS_VC) && defined(MXD_OS_WINDOWS)
//     #if defined(_WIN64)
//         #define MXD_DATA_MODEL_LLP64
//     #endif

#undef MXD_DATA_MODEL_ILP32
#undef MXD_DATA_MODEL_LP64

#if defined(MXD_COMPILER_GNU_GCC) && defined(MXD_OS_LINUX) && defined(MXD_ARCH_AMD64)
    #if defined(__LP64__)
        #define MXD_DATA_MODEL_LP64
    #endif
#endif

#if !defined(MXD_DATA_MODEL_LP64)
#define MXD_DATA_MODEL_ILP32
#endif

// Byte Ordering Type Definition:
//
// Determines byte ordering from the architecture and the compiler definitions.
// When MXD_LITTLE_ENDIAN is defined, the processor architecture uses little
// endian byte ordering convention. Otherwise, MXD_BIG_ENDIAN is defined and the
// convention in use is big endian byte ordering.
//
// WARNING: The byte ordering define must match the processor byte ordering.
//          For example, all x86 architectures use the LITTLE_ENDIAN byte
//          ordering and the PowerPC use the BIG_ENDIAN byte ordering.
#undef MXD_BE_TMP
#undef MXD_LE_TMP

#if defined(MXD_ARCH_PPC)
    #if defined(__BIG_ENDIAN__)
        #define MXD_BE_TMP
    #elif defined(__LITTLE_ENDIAN__)
        #define MXD_LE_TMP
    #endif

#elif defined(MXD_ARCH_MIPS)
    #if defined(__MIPSEB__)
        #define MXD_BE_TMP
    #elif defined(__MIPSEL__)
        #define MXD_LE_TMP
    #endif

#elif defined(MXD_ARCH_IX86) || defined(MXD_ARCH_AMD64)
    #define MXD_LE_TMP

#elif defined(MXD_ARCH_ARM)
    // Assume ARM is in little endian
    #define MXD_LE_TMP

#elif defined(MXD_ARCH_SPARC)
    #define MXD_BE_TMP
#endif

#if defined(MXD_LITTLE_ENDIAN)
    #if defined(MXD_BE_TMP)
        #error "Byte ordering type definition mismatch!"
    #endif

#elif defined(MXD_BIG_ENDIAN)
    #if defined(MXD_LE_TMP)
        #error "Byte ordering type definition mismatch!"
    #endif

#elif defined(MXD_BE_TMP)
    #define MXD_BIG_ENDIAN

#elif defined(MXD_LE_TMP)
    #define MXD_LITTLE_ENDIAN

#else
    #error "Could not determine byte ordering. "\
           "Please define MXD_BIG_ENDIAN or MXD_LITTLE_ENDIAN."
#endif

#undef MXD_BE_TMP
#undef MXD_LE_TMP


//==============================================================================
//======= OS SPECIAL DEFINES  ==================================================
//==============================================================================

// Linux and Solaris Defines.
#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_ANDROID)

// VxWork Defines.
#elif defined(MXD_OS_VXWORKS)

// Windows Defines.
#elif defined(MXD_OS_WINDOWS)

// Windows CE Defines.
#elif defined(MXD_OS_WINDOWS_CE)

// Nucleus Defines.
#elif defined(MXD_OS_NUCLEUS)
    /* For Metaware tools TI, Paradigm and Hitachi Tools */
    #ifndef   _SIZE_T
    #define   _SIZE_T

    /* For KMC GNU Tools */
    #ifndef _SIZE_T_DEF
    #define _SIZE_T_DEF

    /* Microsoft Visual C and Microtec Tools*/
    #ifndef _SIZE_T_DEFINED
    #define _SIZE_T_DEFINED

    /* For Metrowerks Tools */
    #ifndef __size_t__
    #define __size_t__

    #ifndef __SIZE_T_
    #define __SIZE_T_

    typedef long unsigned int size_t;

    #endif  /*  #ifndef __SIZE_T_  */
    #endif  /*  #ifndef __size_t__ */
    #endif  /*  #ifndef _SIZE_T_DEFINED  */
    #endif  /*  #ifndef _SIZE_T_DEF  */
    #endif  /*  #ifndef _SIZE_T  */

// Symbian Defines.
#elif defined(MXD_OS_SYMBIAN)

// BRCM_CUSTOM - [begin] - Add ECOS support.
// eCos Defines (using BOS).
#elif defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] - Add ECOS support.

#elif defined(MXD_OS_NONE)

#endif


//==============================================================================
//==== ANSI AND OS STANDARD INCLUDE FILES ======================================
//==============================================================================

// Linux and Solaris standard include files.
#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <netinet/in.h>
    #include <pthread.h>

// VxWork Standard Include Files.
#elif defined(MXD_OS_VXWORKS)
    #include <netinet/in.h>

// Windows Standard Include Files.
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    #include <winsock2.h>       // Needed for NetLayer.
                                // This also includes <windows.h> if not already
                                // included.
    #include <ws2tcpip.h>       // Include needed to activate IPPROTO_IP
                                // flags like IP_TOS
    #if defined(MXD_OS_WINDOWS)
        #include <process.h>    // Required for threading.
    #endif

    #include <mmsystem.h>       // Required for multimedia timers.

// Symbian Standard include files.
#elif defined(MXD_OS_SYMBIAN) && defined (__cplusplus)
    #include <e32base.h>
    #include <estlib.h>
    #include <f32file.h> // Needed to redirect stdout to a file.
    // sys/types.h MUST be included before netinet/in.h.
    #include <sys/types.h>
    #include <netinet/in.h>

// Nucleus Standard include files.
#elif defined (MXD_OS_NUCLEUS) && defined (__cplusplus)
    // This is for the inclusion of Nucleus C++ base primitives
    #include "npp/npp.h"
    #include <pthread.h>
// BRCM_CUSTOM - [begin] - Add ECOS support.
// Ecos Standard Include Files:
//------------------------------------------------------------------------------
#elif defined(MXD_OS_BOS_ECOS)
    #include <sys/bsdtypes.h>
    #include <netinet/in.h>
    #include <limits.h>
    #include <stddef.h>
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif

// ANSI Standard Include files.
#ifndef __cplusplus
    #if defined(MXD_COMPILER_MS_VC) && ! defined (inline)
        #define inline __inline
    #endif
#endif

#ifdef __cplusplus
    #ifndef MXD_INCLUDE_NEW
        #define MXD_INCLUDE_NEW 1
    #endif

    #if MXD_INCLUDE_NEW == 1
        #include <new>
    #elif MXD_INCLUDE_NEW == 2
        #include <new.h>
    #endif
#endif

#if !defined(MXD_OS_WINDOWS_CE)
    #include <errno.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>

// <COMBINE MXD_UINT_MAX>
#if defined(MXD_UINT_MAX) || defined(UINT_MAX)
    #if !defined(UINT_MAX)
        #define UINT_MAX MXD_UINT_MAX
    #elif !defined(MXD_UINT_MAX)
        #define MXD_UINT_MAX UINT_MAX
    #elif MXD_UINT_MAX != UINT_MAX
        #error "Word size mismatch!"
    #endif
#else
    #error "Please define MXD_UINT_MAX to the maximum value of an unsigned word for this architecture"
#endif

// va_copy is normally defined by including stdarg.h however it is not
// available on all systems.
#if !defined(va_copy)
    #if defined(__va_copy)
        #define va_copy __va_copy
    #else
        #if defined(MXD_COMPILER_GNU_GCC)
            #if (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7) && \
                 defined(MXD_ARCH_PPC) && \
                 defined(MXD_OS_VXWORKS)
                 #define va_copy(x,y) *(x)=*(y)
            #else
                #define va_copy(x,y) (x)=(y)
            #endif
        #else
            #define va_copy(x,y) (x)=(y)
        #endif
    #endif
#endif


//==============================================================================
//==== COMPILER SPECIAL DIRECTIVES  ============================================
//==============================================================================

// GCC Directives.
#if defined(MXD_COMPILER_GNU_GCC)

// MS VC++ Directives.
#elif defined(MXD_COMPILER_MS_VC)
    #pragma warning( disable : 4530 ) // C++ exception handler used
    #pragma warning( disable : 4786 ) // Identifiers was truncated to 255 chars
#endif

//==============================================================================
//==== MITOS STANDARD INCLUDE FILES  ===========================================
//==============================================================================
#ifndef  MXG_MXDEF_H
#include "Basic/MxDef.h"
#endif

#ifndef  MXG_MXMACRO_H
#include "Basic/MxMacro.h"
#endif

#ifndef  MXG_MXDEFUNALIGNEDTYPE_H
#include "Basic/MxDefUnalignedType.h"
#endif

//==============================================================================
//
//  WARNING!!!!!!
//
//  The following order : Trace, Assert and Result MUST NOT BE CHANGED
//  Otherwise, we fall in a deadlock dependency since Result references
//  Assert which in its turn references Trace.
//
//==============================================================================

#ifndef  MXG_MXTRACE_H
#include "Basic/MxTrace.h"
#endif

#ifndef  MXG_MXASSERT_H
#include "Basic/MxAssert.h"
#endif

#ifndef MXG_RESULT_H
#include "Basic/Result.h"
#endif


//==============================================================================
//
//  WARNING!!!!!!
//
//  This creates a dependency to pkg_Kernel!!!!!!!!!!!!!!!!!!
//
//  CMemoryAllocator must be included for the MX_NEW* and MX_DELETE* macros to
//  be available from anywhere.
//
//==============================================================================
#if defined(__cplusplus)
    #ifndef MXG_CMEMORYALLOCATOR_H
        #include "Kernel/CMemoryAllocator.h"
    #endif
#endif

#if defined(MXD_POST_CONFIG)
#include "PostMxConfig.h"
#endif

//M5T_INTERNAL_USE_END

//==============================================================================
//====  Documentation  =========================================================
//==============================================================================
// The remaining of this file is for documentation purposes only. Leave it
// commented out with the following #if 0.
#if 0

//<GROUP CONFIG_MACROS>
//<TITLE Configuring Assertion Categories>
//==============================================================================
//
// Summary:
//  Assertion categories configuration macros.
//
// Description:
//
//  It is possible to define which categories of assertions are actually
//  compiled through the following macros.  Currently, there are two categories:
//  Standard and Real-Time.
//
//  MXD_ASSERT_ENABLE_STD enables:
//      - MX_ASSERT
//      - MX_ASSERT_EX
//      - MX_ASSERT_PERROR
//      - MX_ASSERT_PERROR_EX
//      - MX_ASSERT_ONLY
//
//  MXD_ASSERT_ENABLE_RT enables:
//      - MX_ASSERT_RT
//      - MX_ASSERT_RT_EX
//      - MX_ASSERT_PERROR_RT
//      - MX_ASSERT_PERROR_RT_EX
//      - MX_ASSERT_ONLY_RT
//
//  MXD_ASSERT_ENABLE_SUPPORT enables:
//      - MX_ASSERT_ONLY
//      - MX_ASSERT_ONLY_RT
//
//  MXD_ASSERT_ENABLE_ALL enables:
//    - MXD_ASSERT_ENABLE_STD
//    - MXD_ASSERT_ENABLE_RT
//
//  As noted above, zero or more of these macros can be defined. If no
//  MXD_ASSERT_ENABLE_XXX macros are defined, then no assertion information is
//  compiled.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  <LINK ASSERTION_MECHANISM_OVERVIEW, Assertion Mechanism Overview>
//
//==============================================================================
#define MXD_ASSERT_ENABLE_STD

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_ASSERT_ENABLE_STD>
#define MXD_ASSERT_ENABLE_RT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_ASSERT_ENABLE_STD>
#define MXD_ASSERT_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_ASSERT_ENABLE_STD>
#define MXD_ASSERT_ENABLE_ALL

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Preprocessor macro to disable the filename display in the trace output.
//
// Description:
//  When defined, the displayed message on assertion failure does not include
//  the filename in which the assertion has been validated.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MXD_ASSERT_DISABLE_OUTPUT_LINENUMBER.
//
//==EDOC========================================================================
#define MXD_ASSERT_DISABLE_OUTPUT_FILENAME

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Preprocessor macro to disable the line number display in the trace output.
//
// Description:
//  When defined, the displayed message on assertion failure does not include
//  the line number at which the assertion has been validated.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MXD_ASSERT_DISABLE_OUTPUT_FILENAME.
//
//==EDOC========================================================================
#define MXD_ASSERT_DISABLE_OUTPUT_LINENUMBER

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the debug break behaviour.
//
// Description:
//  When defined, this macro prevents the suspention of the execution and
//  prevents the debugger from being notified.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MX_ASSERT_DEBUG_BREAK
//
//==EDOC========================================================================
#define MXD_ASSERT_DISABLE_DEBUG_BREAK

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that the default "Assertion Failed Handler" is overridden
//  at compile time.
//
// Description:
//  This macro is used to indicate that the default assertion failed handler
//  is overridden at compile time. When defined, the application must define
//  its own handler in the following way, in a single .cpp file:
//
//  <CODE>
//    // Must previously have included MxAssert.h
//    //------------------------------------------
//    #include "Basic/MxAssert.h"
//
//    SAssertFailHandler g_stAssertFailHandler =
//        { YourAssertionFailedHandler, 0 }
//  </CODE>
//
//  This avoids having to call MxAssertSetNewFailHandler to set the function
//  pointer at runtime, and makes sure the new handler is set early at
//  initialization time.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  mxt_PFNAssertFailHandler MxAssertSetNewFailHandler.
//
//==EDOC========================================================================
#define MXD_ASSERT_FAIL_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that the default "Final Behavior Handler" is overridden at compile
//  time.
//
// Description:
//  This macro is used to indicate that the global final behaviour handler
//  is overridden at compile time.  When defined, the application must define
//  its own handler in the following way, in a single .cpp file:
//
//  <CODE>
//    // Must previously have included MxAssert.h
//    //------------------------------------------
//    #include "Basic/MxAssert.h"
//
//    SAssertFinalBehaviorHandler g_stAssertFinalBehaviorHandler =
//        { YourFinalBehaviorHandler, 0 }
//  </CODE>
//
//  This avoids having to call MxAssertSetNewFinalBehaviorHandler to set the
//  function pointer at runtime.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MxAssertSetNewFinalBehaviorHandler mxt_PFNAssertFinalBehaviorHandler.
//
//==EDOC========================================================================
#define MXD_ASSERT_FINAL_BEHAVIOR_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the final behaviour on assertion failure.
//
// Description:
//  When defined, this allows the execution to continue after an assertion
//  failure.
//
//  Note that this is useful only when using the default "Assertion Failed
//  Handler".
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ASSERT_FINAL_BEHAVIOR_DISABLE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the fatal final behaviour on assertion failure.
//
// Description:
//  When defined, causes the application to exit after an assertion failure
//  occurs.
//
//  Note that this is useful only when using the default "Assertion Failed
//  Handler".
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ASSERT_FINAL_BEHAVIOR_IS_FATAL

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables override of assert, assert_perror, and ASSERT.
//
// Description:
//  By default, the <C>assert</C> and <C>assert_perror</C> macros are overridden
//  to use the assertion mechanism. <C>ASSERT</C> is also undefined so calling
//  it has no effect. The MXD_DISABLE_EXTERNAL_ASSERT_OVERRIDE macro lets the
//  user keep the original behaviour of these three macros.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_DISABLE_EXTERNAL_ASSERT_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that the "Trace Handler" is overridden at compile time.
//
// Description:
//  This macro is used to indicate that the trace handler is overridden at
//  compile time. When defined, the application must define its own handler in
//  the following way, in a single .cpp file:
//
//  <CODE>
//    // Must previously have included MxAssert.h
//    //------------------------------------------
//    #include "Basic/MxAssert.h"
//
//    SAssertTraceHandler g_stAssertTraceHandler =
//        { YourAssertTraceHandler, 0 }
//  </CODE>
//
//  This avoids having to call MxAssertSetNewTraceHandler to set the function
//  pointer at runtime.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MxAssertSetNewTraceHandler mxt_PFNAssertTraceHandler.
//
//==EDOC========================================================================
#define MXD_ASSERT_TRACE_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the tracing of the message on assertion failure.
//
// Description:
//  When defined, no message is displayed upon assertion failure. The execution
//  can be stopped anyways if debug break is enabled.
//
//  Note that this is useful only when using the default "Assertion Failed
//  Handler".
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ASSERT_TRACE_DISABLE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that the default "Call-Stack Trace Handler" is overridden at
//  compile time.
//
// Description:
//  This macro is used to indicate that the default call-stack trace handler is
//  overridden at compile time. When defined, the application must define
//  its own handler in the following way, in a single .cpp file:
//
//  <CODE>
//    // Must previously have included MxAssert.h
//    //------------------------------------------
//    #include "Basic/MxAssert.h"
//
//    SAssertCallStackTraceHandler g_stAssertCallStackTraceHandler =
//        { YourCallStackTraceHandler, 0 }
//  </CODE>
//
//  This avoids having to call MxAssertSetNewCallStackTraceHandler to set the
//  function pointer at runtime.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MxAssertSetNewCallStackTraceHandler mxt_PFNAssertCallStackTraceHandler.
//
//==EDOC========================================================================
#define MXD_ASSERT_CALL_STACK_TRACE_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables the display of the call stack trace.
//
// Description:
//  When defined, the call stack trace is not displayed after an assertion
//  failure.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ASSERT_CALL_STACK_TRACE_DISABLE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the use of namespaces.
//
// Description:
//  By defining MXD_ENABLE_NAMESPACE, standard namespacing macros are defined
//  and namespacing is enabled on compilers that support it.  Note that some
//  debuggers might have difficulties with namespacing.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MX_NAMESPACE_START MX_NAMESPACE_END MX_NAMESPACE_USE.
//
//==EDOC========================================================================
#define MXD_ENABLE_NAMESPACE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables package IDs override.
//
// Description:
//  When defined, this macro enables the application to define its own
//  EMxPackageId enumerations in its PreMxConfig.h file.
//  Note that this file must be found somewhere in the compiler
//  include path.
//
//  The application must define this if it wants to use  the result
//  mechanism for its own functions/methods.
//
//  The application must make sure that the original enumeration is copied in
//  PreMxConfig.h and only augment the list, otherwise it is possible that the
//  packages will no longer compile.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  EMxPackageId
//
//==EDOC========================================================================
#define MXD_PKG_ID_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables shared error messages.
//
// Description:
//  When defined, the MX_RGET_MSG_STR method produces additional shared error
//  messages.
//
//==EDOC========================================================================
#define MXD_RESULT_ENABLE_SHARED_ERROR_MESSAGES

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Macro that permits overriding the default trace output handler.
//
// Description:
//  By default, the framework tracing mechanism uses stdout to output traces.
//  In order to allow tracing to be output to the correct destination when
//  tracing before "main", it must be possible to override the default handler
//  at compile time. It is however not recommended to trace before the Framework
//  is initialized. This macro allows controlling the tracing mechanism during
//  the initialization process, when the tracing mechanism configuration
//  functions could not have been called. This can be accomplished by defining
//  MXD_TRACE_OUTPUT_HANDLER_OVERRIDE to an application-defined trace output
//  handler.  Although many output handlers can be registered to output the
//  traces at the same time, it is only possible to override with one output
//  handler.  However, more output handlers can be registered at run-time. To
//  do this, it is necessary to have an extern declaration of the application
//  defined output handler in the file "PostMxConfig.h" as well as the define
//  MXD_TRACE_OUTPUT_HANDLER_OVERRIDE. The inclusion of the "PostMxConfig.h" is
//  enabled by the macro MXD_POST_CONFIG in the "PreMxConfig.h" file.
//
//  In "PreMxConfig.h" add:
//  <CODE>
//  #define MXD_POST_CONFIG
//  </CODE>
//
//  In "PostMxConfig.h" add:
//  <CODE>
//  extern
//  void TraceOutputHandler(IN EMxTraceLevel eLevel,
//                          IN EMxTraceCategory eCategory,
//                          IN EMxPackageId ePackage,
//                          IN const char* pszTrace,
//                          IN int nMsgSize);
//
//  #define MXD_TRACE_OUTPUT_HANDLER_OVERRIDE TraceOutputHandler
//  </CODE>
//  where TraceOutputHandler is the application-defined output handler.
//
// Location:
//  Define this in "PostMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==EDOC========================================================================
#define MXD_TRACE_OUTPUT_HANDLER_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Macro that permits overriding the default trace format handler.
//
// Description:
//  The framework tracing mechanism comes with a default trace format, described
//  in mxt_PFNTraceFormatHandler.
//
//  If this default format does not meet the application's requirements, it can
//  be changed by defining MXD_TRACE_FORMAT_HANDLER_OVERRIDE to an application-
//  defined trace format handler.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==EDOC========================================================================
#define MXD_TRACE_FORMAT_HANDLER_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Macro that permits overriding the default call stack trace handler.
//
// Description:
//  The MX_TRACE_CALLSTACK macro permits an application (and the MX_ASSERT
//  module) to print the current call stack. This macro calls a default call
//  stack trace handler that can be overridden by the application with some
//  other handler.
//
//  Defining the macro MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE permits overriding
//  this default call stack trace handler with one defined by the application.
//
//  When this macro is defined, the application must create a handler function
//  that follows the mxt_PFNTraceCallStackHandler type and assigns it to the
//  external variable defined in MxTrace.h:
//
//   <CODE>
//      #define MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE YourCallStackHandler
//   </CODE>
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  mxt_PFNTraceCallStackHandler, MX_TRACE_CALLSTACK
//
//==EDOC========================================================================
#define MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the maximum trace size.
//
// Description:
//  By default, with the default format handler, the traces have a limited
//  length of 512 characters, including the trace header fields plus terminating
//  newline and NULL characters. The size of this buffer can be modified by
//  defining MXD_TRACE_BUFFER_CAPACITY to an integer value. Use it with care,
//  with regards to the allocation method selected. For instance, for the
//  default allocation method (stack allocation), a buffer of that size is
//  allocated on the stack for each trace. For performance reasons, there is no
//  check for buffer overload.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  mxt_PFNTraceFormatHandler
//
//==EDOC========================================================================
#define MXD_TRACE_BUFFER_CAPACITY 512

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Maximal backtrace capacity for call-stack traces under Linux.
//
// Description:
//  When using the default call-stack trace handler, this defines the maximal
//  backtrace capacity under Linux only. This define has no effect on other
//  platforms.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_TRACE_BACKTRACE_CAPACITY 128

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines the separator to be used between each header field.
//
// Description:
//  An application can define this macro to a character, which serves as
//  separator between each header field of a trace. If this is not defined, it
//  defaults to '|'.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_TRACE_SEPARATOR

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Defines a program name to be added to all traces.
//
// Description:
//  An application can define this macro to a character string, which appears in
//  one header field of the default format handler for each trace. If this is
//  not defined, it defaults to an empty character string.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  mxt_PFNTraceFormatHandler
//
//==EDOC========================================================================
#define MXD_TRACE_PROGNAME ""

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the maximum number of tracing output handlers.
//
// Description:
//  This macro configures the maximum number of elements that can be inserted in
//  the output handlers buffer.  The output handlers buffer is traversed when a
//  trace needs to be output, therefore making the trace go through all
//  registered handlers.
//
//  When no value is specified by the user, the macro is automatically set to 1.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  mxt_PFNTraceOutputHandler
//
//==EDOC========================================================================
#define MXD_TRACE_MAX_NB_OF_OUTPUT_HANDLERS

//<GROUP CONFIG_MACROS>
//<TITLE Configuring Tracing Levels>
//==SDOC========================================================================
//
// Summary:
//  Enables traces.
//
// Description:
//  It is possible to define which levels of tracing are actually compiled
//  through the following macros.
//
//  <CODE>
//  The columns correspond to the 10 levels of tracing, as follows:
//    0 = eLEVEL0 | 1 = eLEVEL1 | 2 = eLEVEL2 | 3 = eLEVEL3 | 4 = eLEVEL4
//    5 = eLEVEL0 | 6 = eLEVEL1 | 7 = eLEVEL2 | 8 = eLEVEL3 | 9 = eLEVEL4
//
//                                                -----------------------------------------
//                                                | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
//  ++ Control each tracing macro individually    -----------------------------------------
//  #define MXD_TRACE0_ENABLE_SUPPORT ............| X |   |   |   |   |   |   |   |   |   |
//  #define MXD_TRACE1_ENABLE_SUPPORT ............|   | X |   |   |   |   |   |   |   |   |
//  #define MXD_TRACE2_ENABLE_SUPPORT ............|   |   | X |   |   |   |   |   |   |   |
//  #define MXD_TRACE3_ENABLE_SUPPORT ............|   |   |   | X |   |   |   |   |   |   |
//  #define MXD_TRACE4_ENABLE_SUPPORT ............|   |   |   |   | X |   |   |   |   |   |
//  #define MXD_TRACE5_ENABLE_SUPPORT ............|   |   |   |   |   | X |   |   |   |   |
//  #define MXD_TRACE6_ENABLE_SUPPORT ............|   |   |   |   |   |   | X |   |   |   |
//  #define MXD_TRACE7_ENABLE_SUPPORT ............|   |   |   |   |   |   |   | X |   |   |
//  #define MXD_TRACE8_ENABLE_SUPPORT ............|   |   |   |   |   |   |   |   | X |   |
//  #define MXD_TRACE9_ENABLE_SUPPORT ............|   |   |   |   |   |   |   |   |   | X |
//                                                -----------------------------------------
//  ++ Control on all tracing macro level
//  #define MXD_TRACE_ENABLE_ALL .................| X | X | X | X | X | X | X | X | X | X |
//                                                -----------------------------------------
//  ++ Control on consecutive tracing macro level
//  #define MXD_TRACE_MAX_LEVEL  0 ...............| X |   |   |   |   |   |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  1 ...............| X | X |   |   |   |   |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  2 ...............| X | X | X |   |   |   |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  3 ...............| X | X | X | X |   |   |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  4 ...............| X | X | X | X | X |   |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  5 ...............| X | X | X | X | X | X |   |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  6 ...............| X | X | X | X | X | X | X |   |   |   |
//  #define MXD_TRACE_MAX_LEVEL  7 ...............| X | X | X | X | X | X | X | X |   |   |
//  #define MXD_TRACE_MAX_LEVEL  8 ...............| X | X | X | X | X | X | X | X | X |   |
//  #define MXD_TRACE_MAX_LEVEL  9 ...............| X | X | X | X | X | X | X | X | X | X |
//                                                -----------------------------------------
//  ++ These do not belong to any specific level
//  #define MXD_TRACE_ENABLE_MACRO_CALLSTACK......| X | X | X | X | X | X | X | X | X | X |
//  #define MXD_TRACE_ENABLE_MACRO_X..............| X | X | X | X | X | X | X | X | X | X |
//                                                -----------------------------------------
//  </CODE>
//
//  As noted above, zero or more of these macros can be defined. If no
//  MXD_TRACE{LEVEL}_ENABLE_SUPPORT macros are defined, then no tracing
//  information is compiled.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
// See Also:
//  <LINK GENERAL_TRACING_CONFIGURATION, General Tracing Configuration>
//
//==EDOC========================================================================
#define MXD_TRACE0_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE1_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE2_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE3_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE4_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE5_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE6_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE7_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE8_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE9_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACEX_ENABLE_SUPPORT

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE_MAX_LEVEL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE_ENABLE_ALL

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE0_ENABLE_SUPPORT>
#define MXD_TRACE_ENABLE_MACRO_CALLSTACK

//<GROUP CONFIG_MACROS>
//<TITLE Tracing Internal Buffer configuration>
//==SDOC========================================================================
//
// Summary:
//  Sets the default configuration for the MxTrace internal buffer.
//
// Description:
//  The tracing mechanism requires a buffer to format the trace to output.
//  Depending on system limitations or even on implementation choices, this
//  buffer may be required to be allocated by different means. The default
//  behaviour was chosen to use a stack allocated buffer.
//
//  <CODE>
//      MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER
//  </CODE>
//  Dynamic buffer allocation mechanism.  A buffer is dynamically allocated
//  (using new and delete operators) and formatted everytime a trace needs to be
//  outputted.
//
//  <CODE>
//      MXD_TRACE_USE_STACK_BUFFER
//  </CODE>
//  Static buffer allocation mechanism.  A buffer is allocated on the stack and
//  formatted everytime a trace needs to be outputted.
//
//  <CODE>
//      MXD_TRACE_USE_MEMORY_QUEUE_BUFFER
//  </CODE>
//  Tracing mechanism that uses a memory queue. New traces are pushed on a
//  memory queue. If this mechanism is chosen, the user has the choice to let an
//  internal thread de-queue the traces and call the output handler periodically.
//  The user can decide not to use the internal thread, in which case the user
//  is responsible of de-queuing and outputting the traces. This can be done by
//  defining MXD_TRACE_USE_EXTERNAL_THREAD and calling MxTraceCallOutputHandler.
//
//  <CODE>
//      MXD_TRACE_BUFFER_OVERRIDE
//  </CODE>
//  User-specified buffer allocation mechanism.  This allows a user to
//  provide his own allocation mechanism.  When this is defined, the user must
//  also define the MX_TRACE_NEW_BUFFER and MX_TRACE_RELEASE_BUFFER
//  macros.  The "x" parameter is a pointer to a char buffer.
//
//  The MX_TRACE_NEW_BUFFER and MX_TRACE_RELEASE_BUFFER will allocate and free
//  the buffer on behalf of the trace system.
//
// Location:
//  Define this in "PreMxConfig.h" or in your makefile for special builds.
//
//==EDOC========================================================================
#define MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER>
#define MXD_TRACE_USE_STACK_BUFFER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER>
#define MXD_TRACE_USE_MEMORY_QUEUE_BUFFER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER>
#define MXD_TRACE_BUFFER_OVERRIDE

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER>
#define MX_TRACE_NEW_BUFFER

//<GROUP CONFIG_MACROS>
//<COMBINE MXD_TRACE_USE_DYNAMIC_ALLOC_BUFFER>
#define MX_TRACE_RELEASE_BUFFER

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the tracing queue size in bytes.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen, this define
//  lets the user specify the queue size in bytes. The default size is 100K.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_QUEUE_SIZE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Use an external thread for the memory queue tracing mechanism.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen, this allows
//  the user to configure the mechanism to not use the provided internal thread
//  to output the traces stored in the memory queue. If this is done, the user
//  must call MxTraceCallOutputHandler to output the messages from the memory
//  queue. This gives more flexibility to the user on how the messages from the
//  memory queue are outputted. However, the MxTraceCallOutputHandler is not
//  protected against concurrent access for performance reasons, and it is the
//  user's responsibility that it is not called from different threads.
//  MxTraceCallOutputHandler can not be called after the call to
//  CFrameworkInitializer::Finalize.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  MXD_TRACE_USE_MEMORY_QUEUE_BUFFER, MxTraceCallOutputHandler, PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_USE_EXTERNAL_THREAD

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the internal tracing thread name.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this define lets the user specify the name of the
//  internal thread.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_THREAD_NAME

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the internal tracing thread's stack size.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this define lets the user specify the stack size
//  of the internal thread. The default is the same as for CThread.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_THREAD_STACK_SIZE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the internal tracing thread's priority.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this define lets the user specify the priority of
//  the internal thread. The default is the same as for CThread.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_THREAD_PRIORITY

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the sleeping period in ms between outputs of the internal tracing
//  thread.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this define lets the user specify the sleeping
//  period in ms between outputs of the internal thread. The default is 100 ms.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  MxTraceSetThreadPeriodMs, PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_PERIOD_MS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Specifies the number of messages outputted per period.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this define lets the user specify the number of
//  messages outputted per period. The default is 10.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  MxTraceSetThreadMessagesPerPeriod, PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_MESSAGES_PER_PERIOD

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Empties the tracing memory queue before finalizing the framework.
//
// Description:
//  If the tracing mechanism that uses the memory queue is chosen and the
//  internal thread is used, this allows the user to configure if the internal
//  thread outputs all messages in the memory queue before quitting or if they
//  are lost when the framework finalizes. This is not defined by default so the
//  messages present in the memory queue when CFrameworkInitializer::Finalize is
//  called are lost.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_EMPTY_MEMORY_QUEUE_ON_FINALIZE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Default state of the tracing nodes.
//
// Description:
//  This allows the user to determine whether or not the traces are enabled by
//  default during registration. A value of true means the trace nodes are
//  enabled by default while false means they are disabled. If
//  MXD_TRACE_NODES_ENABLED_AT_REGISTRATION is not defined, the trace nodes are
//  enabled by default.
//
// Location:
//  Define this in PreMxConfig.h.
//
// See Also:
//  PreMxConfig.h
//
//==EDOC========================================================================
#define MXD_TRACE_NODES_ENABLED_AT_REGISTRATION

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables the use of the PostMxConfig header file.
//
// Description:
//  When defined, the PostMxConfig header file is included at the end of
//  MxConfig.h.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  PostMxConfig.h.
//
//==EDOC========================================================================
#define MXD_POST_CONFIG

//@@PreMxConfig.h
//<GROUP CONFIG_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Configuration header file for all of M5T's software.
//
// Description:
//  The PreMxConfig header file is where an application configures the
//  system-wide compilation options that affect all of the provided software.
//  This can include configuring assertions, tracing, and other system-wide
//  options.
//
// Warning:
//  Since all of M5T's files have a dependency on PreMxConfig.h and on
//  PostMxConfig.h (through MxConfig.h), any modification done to these files
//  requires the re-compilation of all the source code including MxConfig.h.
//
// See Also:
//  PostMxConfig.h.
//
//==EDOC========================================================================

//@@PostMxConfig.h
//<GROUP CONFIG_MACROS>
//==SDOC========================================================================
//
// Summary:
//  Header file that can be included at the end of the MxConfig header file.
//
// Description:
//  The PostMxConfig header file can be used to add information required after
//  the inclusion of MxConfig, or to override informaton given by PreMxConfig or
//  MxConfig itself.  An example of use would be to add an application-specific
//  preprocessor macro based on the information generated by MxConfig
//  (architecture, OS version, etc.).  This file is automatically included
//  by MxConfig.h when MXD_POST_CONFIG is defined.
//
// See Also:
//  MXD_POST_CONFIG.
//
//==EDOC========================================================================

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the gcc compiler.
//
// Description:
//  Preprocessor macro used to enable gnu gcc compiler specific code.
//  MXD_COMPILER_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_COMPILER_GNU_GCC

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the MSVC compiler.
//
// Description:
//  Preprocessor macro used to enable MSVC compiler specific code.
//  MXD_COMPILER_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_COMPILER_MS_VC

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the DIAB compiler.
//
// Description:
//  Preprocessor macro used to enable DIAB compiler specific code.
//  MXD_COMPILER_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_COMPILER_DIAB

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the TI CL6X compiler.
//
// Description:
//  Preprocessor macro used to enable cl6x (Code Composer Studio) compiler
//  specific code. MXD_COMPILER_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_COMPILER_TI_CL6X

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the Linux operating system.
//
// Description:
//  Preprocessor macro used to enable Linux operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_LINUX

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the VxWorks operating system.
//
// Description:
//  Preprocessor macro used to enable VxWorks operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  MXD_SYSTEM_MEMORY_SIZE.
//
//==EDOC========================================================================
#define MXD_OS_VXWORKS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the Windows operating system.
//
// Description:
//  Preprocessor macro used to enable Windows operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Note:
//  The Framework on the Windows operating system requires the Microsoft
//  Platform Software Development Kit. The MS platform SDK contains Windows
//  header files and Windows import libraries. The MS platform SDK can be
//  downloaded from:
//  http://www.microsoft.com/downloads/details.aspx?FamilyId=0BAF2B35-C656-4969-ACE8-E4C0C0716ADB&displaylang=en.
//
//  Additional documentation about the MS platform SDK can be found on:
//  http://msdn.microsoft.com.
//
//  Please refer to the Microsoft documentation on how to install the
//  Platform SDK and for any additional information.
//
//  The following MS platform SDK are required for given version of Windows OS.
//
//  - The Microsoft platform SDK 2003 (February 2003 version) is required for
//    Microsoft Visual C++ 6.0 builds.
//  - The Microsoft platform SDK 2003 or later is required for Microsoft Visual
//    Studio 7 and 8 builds.
//  - The Microsoft SDK v6.0 or later is required for executions under Windows
//    Vista.
//
//  The following Include Paths must be added in Visual C++ 6/7/8 to the
//  Tools/Options/Directories:
//
//      - Include files:
//         - ${PLATFORM_SDK_INSTALL_DIR}\\INCLUDE
//         - ${PLATFORM_SDK_INSTALL_DIR}\\INCLUDE\\PRERELEASE
//
//      - Library files:
//         - ${PLATFORM_SDK_INSTALL_DIR}\\LIB
//
//  Replace the '${PLATFORM_SDK_INSTALL_DIR}' part by the actual installation
//  directory of the MS platform SDK.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_WINDOWS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the Windows CE operating system.
//
// Description:
//  Preprocessor macro used to enable Windows CE operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_WINDOWS_CE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the Nucleus operating system.
//
// Description:
//  Preprocessor macro used to enable Nucleus operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_NUCLEUS

// BRCM_CUSTOM - [begin] - Add ECOS support.
//@@MXD_OS_BOS_ECOS
//
//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the Ecos operating system.
//
// Description:
//  Preprocessor macro to enable BOS wrapper operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_BOS_ECOS
// BRCM_CUSTOM - [end] - Add ECOS support.

//@@MXD_OS_NONE
//
//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for an unspecified operating system.
//
// Description:
//  Preprocessor macro used to disable operating system specific code.
//  MXD_OS_XXX macros are mutually exclusive. This allows to have a minimal
//  port of the framework without most of its operating system abstractions like
//  threads, timers, mutexes, and semaphores. When defined, the support for
//  native 64 bits cannot be deduced.
//
// See also:
//  MXD_64BITS_SUPPORT_DISABLE, MXD_64BITS_CUSTOM_TYPE,
//  MXD_TRACE_FORMAT_HANDLER_OVERRIDE, MXD_TRACE_CALLSTACK_HANDLER_OVERRIDE.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_OS_NONE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the specified VxWorks version.
//
// Description:
//  Preprocessor macro used to specify the version of the VxWorks operating
//  system for which we are building. Must take a value of 0x50400 for
//  VxWorks 5.4 and 0x050500 for VxWorks 5.5.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_VXWORKS_VERSION

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the GNU LIBC library.
//
// Description:
//  Preprocessor macro used to enable GNU LIBC library specific code.
//  MXD_LIB_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_LIB_GNU_LIBC

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done by using the GNU UCLIBC library.
//
// Description:
//  Preprocessor macro used to enable GNU UCLIBC library specific code.
//  MXD_LIB_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_LIB_GNU_UCLIBC

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the PowerPC architecture.
//
// Description:
//  Preprocessor macro used to enable PPC architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_PPC

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the MIPS architecture.
//
// Description:
//  Preprocessor macro used to enable MIPS architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_MIPS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the NIOS2 architecture.
//
// Description:
//  Preprocessor macro used to enable NIOS2 architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_NIOS2

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the ARM architecture.
//
// Description:
//  Preprocessor macro used to enable ARM architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_ARM

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the ix86 architecture.
//
// Description:
//  Preprocessor macro used to enable Ix86 architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_IX86

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for the AMD64 architecture.
//
// Description:
//  Preprocessor macro used to enable AMD64 architecture specific code.
//  MXD_ARCH_XXX macros are mutually exclusive.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ARCH_AMD64

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables unaligned data types.
//
// Description:
//  Preprocessor macro used to enable unaligned types to avoid unaligned access
//  when aligned access to memory is required. Only meaningful in combination
//  with MXD_ARCH_OTHER_XXX, ignored in other cases.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_ALIGNED_ACCESS_REQUIRED

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for a ILP32 data model.
//
// Description:
//  The ILP32 data model is the default data model. It is chosen when other
//  data models cannot be deduced from the compiler's information. Below, the
//  data type information related to the data model:
//
//  <code>
//  Data type           size in bits
//  char                8
//  short               16
//  int                 32
//  long                32
//  pointer             32
//  </code>
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_DATA_MODEL_ILP32

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for a LP64 data model.
//
// Description:
//  The LP64 is deduced from the compiler's information. So far, it is detected
//  only for the following combination of compiler, OS and architecture:
//  GCC + LINUX + AMD64. Below, the data type information related to the data
//  model:
//
//  <code>
//  Data type           size in bits
//  char                8
//  short               16
//  int                 32
//  long                64
//  pointer             64
//  </code>
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_DATA_MODEL_LP64

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for a little endian architecture.
//
// Description:
//  Preprocessor macro used to enable little endian architecture specific code.
//  One of MXD_BIG_ENDIAN or MXD_LITTLE_ENDIAN must be defined or deduced from
//  the compiler's information.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_LITTLE_ENDIAN

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates that build is done for a big endian architecture.
//
// Description:
//  Preprocessor macro used to enable big endian architecture specific code.
//  One of MXD_BIG_ENDIAN or MXD_LITTLE_ENDIAN must be defined or deduced from
//  the compiler's information.
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_BIG_ENDIAN

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Indicates the maximum value of an unsigned integer.
//
// Description:
//  Preprocessor macro used to define the maximum value an unsigned integer can
//  hold on the current architecture. MXD_UINT_MAX must be defined if it can not
//  be deduced from the compiler's information. As an example, a 32 bits
//  architecture has its maximum unsigned integer value set to 0xffffffff
//  and on a 64 bits architecture, the maximum value is (2^64 - 1).
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_UINT_MAX

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Disables 64 bits types and methods.
//
// Description:
//  Preprocessor macro used to disable support and inclusion of code for 64 bits
//  methods in a minimal port of the framework.
//
//  Usually, this preprocessor define is suitable when the following conditions
//  are met:
//    - The MXD_OS_NONE preprocessor define is present.
//    - The framework is included in its minimal way.
//    - Native 64 bits data type is not available.
//    - 64 bits support is NOT required.
//
//  When NOT defined, the 64 bits support is normally offered natively or only
//  minimal support is required. See MXD_64BITS_CUSTOM_TYPE.
//
//  IMPORTANT: When this define is present, the standard tracing mechanism MUST
//  be disabled because it relies on 64 bits support. In such a case, a trace
//  handler must be provided.
//
// See also:
//  MXD_OS_NONE, MXD_64BITS_CUSTOM_TYPE
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_64BITS_SUPPORT_DISABLE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Enables a minimal 64 bits variable support.
//
// Description:
//  Preprocessor macro used to enable a minimal 64 bits variable support.
//
//  Usually, this preprocessor define is suitable when the following conditions
//  are met:
//    - The MXD_OS_NONE preprocessor define is present.
//    - The framework is included in its minimal way.
//    - Native 64 bits data type is not available.
//    - Minimal 64 bits support is required.
//
//  IMPORTANT: When this define is present, the standard tracing mechanism MUST
//  be disabled because it relies on 64 bits support. In such a case, a trace
//  handler must be provided.
//
// See also:
//  MXD_OS_NONE, MXD_64BITS_SUPPORT_DISABLE
//
// Location:
//  Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_64BITS_CUSTOM_TYPE

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//  Activates the code allowing to set the error message strings.
//
// Description:
//  When this preprocessor macro is defined, it is possible to associate
//  messages with the error codes by using the MX_XXX_MSG_TBL_BEGIN/END macros
//  defined in Result.h. These messages can then be returned by MX_RGET_MSG_STR.
//  When this macro is not defined, MX_RGET_MSG_STR always returns an empty
//  string.
//
// Location:
//  Define this in "PreMxConfig.h".
//
// See Also:
//  Result.h.
//
//==EDOC========================================================================
#define MXD_RESULT_ENABLE_ERROR_MESSAGES

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Adds the error message strings of every packages to the compiled code.
//
// Description:
//      When this preprocessor macro is defined, the messages associated
//      with the results used in any packages are put in the compiled code
//      and can then be returned by MX_RGET_MSG_STR.
//      If this macro is not defined, MX_RGET_MSG_STR might return an empty
//      string unless a package-specific error message macro
//      (such as MXD_RESULT_ENABLE_MITOSFW_ERROR_MESSAGES) is defined.
//
//      This macro has no effect when MXD_RESULT_ENABLE_ERROR_MESSAGES
//      is not defined.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_RESULT_ENABLE_MITOSFW_ERROR_MESSAGES,
//      MXD_RESULT_ENABLE_ERROR_MESSAGES.
//
//==EDOC========================================================================
#define MXD_RESULT_ENABLE_ALL_ERROR_MESSAGES

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//    Enables support for the memory allocator.
//
// Description:
//      When this preprocessor macro is defined, calls to the MX_NEW,
//      MX_NEW_ARRAY, MX_DELETE, and MX_DELETE_ARRAY macros use the memory
//      allocator implemented in the CMemoryAllocator class.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT,
//      MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT,
//      MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT,
//      MX_NEW, MX_NEW_ARRAY, MX_DELETE, MX_DELETE_ARRAY.
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//    Enables concurrency protection for the memory allocator.
//
// Description:
//      When this preprocessor macro is defined, calls to the MX_NEW,
//      MX_NEW_ARRAY, MX_DELETE, and MX_DELETE_ARRAY macros are protected
//      against concurent access via a mutex.  This is necessary when using a
//      memory allocation library that offers no such protection.
//
//      <b>NOTE: Defining this macro creates a dependency towards CMutex.  Make
//      sure that it is available when enabling concurency protection.</b>
//
//      NOTE: MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be enabled when using
//      this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//    Enables concurrency protection for the memory allocator.
//
// Description:
//      When this preprocessor macro is defined, calls to the MX_NEW,
//      MX_NEW_ARRAY, MX_DELETE, and MX_DELETE_ARRAY macros update memory
//      statistics, which are available via the
//      CMemoryAllocator::GetMemoryStatistics method.  Defining this macro also
//      enables concurrency protection.
//
//      <b>NOTE: Defining this macro creates a dependency towards CMutex.  Make
//      sure that it is available when enabling memory statistics.</b>
//
//      NOTE: MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be enabled when using
//      this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT,
//      MXD_MEMORY_ALLOCATOR_PROTECTION_ENABLE_SUPPORT.
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_STATISTICS_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//    Enables bound checking on memory allocation.
//
// Description:
//      When this preprocessor macro is defined alongside
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT, a magic word is added at the
//      beginning and at the end of each memory block allocated using MX_NEW
//      or MX_NEW_ARRAY.  When a subsequent call to MX_DELETE or
//      MX_DELETE_ARRAY is made, a verification is performed on the magic words
//      to make sure that it has not been tampered with.
//
//  Note:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be enabled when using
//      this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT,
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_BOUND_CHECK_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Enables memory tracking.
//
// Description:
//      When this preprocessor macro is defined alongside
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT, the CMemoryAllocator keeps track
//      of every memory block allocated that has not been deallocated yet.
//
//      NOTE: MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be enabled when using
//      this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
//      MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Enables the storage of additional information about each allocated
//      memory block.
//
// Description:
//      When this preprocessor macro is defined alongside
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT, extra information is stored about
//      each allocated memory block. This information contains the type of the
//      allocated memory and the file name and line number of the allocation
//      site as well as a bit field used to store flags.
//
//      NOTE: MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT must be enabled when using
//      this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
//      MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_EXTRA_INFORMATION_ENABLE_SUPPORT


//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      When defined, this macro enables the memory pooling support of the
//      CMemoryAllocator class.
//
// Description:
//      When this preprocessor macro is defined, it enables the memory pooling
//      mechanism of the CMemoryAllocator. Once the memory pooling mechanism is
//      enabled, it needs to be configured through the use of the
//      MXD_astMEMORY_ALLOCATOR_POOL_INFO preprocessor macro.
//
//      NOTE: If you enabled the pooling mechanism without defining a set of
//      pools, the CMemoryAllocator does not use any pooling.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
//      MXD_astMEMORY_ALLOCATOR_POOL_INFO
//
//==EDOC========================================================================
#define MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Defines the configuration for the memory pooling feature of the
//      CMemoryAllocator.
//
// Description:
//      When this preprocessor macro is defined along with the
//      MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT preprocessor macro,
//      it defines the configuration for the memory pooling mechanism of the
//      CMemoryAllocator. This macro must take a specific format to be valid.
//
//      The following is the description of the format:
//
//      The MXD_astMEMORY_ALLOCATOR_POOL_INFO is used as an aggregate
//      initializer to an array of CMemoryAllocator::SMemoryPoolInfo.
//
//      Example:
//      <CODE>
//          CMemoryAllocator::SMemoryPoolInfo s_stPoolInformations[] =
//              MXD_astMEMORY_ALLOCATOR_POOL_INFO;
//      </CODE>
//
//      As such, the macro takes the format of a list of SMemoryPoolInfo
//      aggregate initializers, as described below:
//
//      <CODE>
//      #define MXD_astMEMORY_ALLOCATOR_POOL_INFO
//          {
//          { 0 + g_uMEMORY_BLOCK_OVERHEAD_SIZE,  32 + g_uMEMORY_BLOCK_OVERHEAD_SIZE, 500, NULL, NULL},
//          {33 + g_uMEMORY_BLOCK_OVERHEAD_SIZE,  64 + g_uMEMORY_BLOCK_OVERHEAD_SIZE, 200, NULL, NULL},
//          {64 + g_uMEMORY_BLOCK_OVERHEAD_SIZE, 128 + g_uMEMORY_BLOCK_OVERHEAD_SIZE,   0, MyAllocator, MyDeallocator},
//          }
//      </CODE>
//
//      NOTE: The above example omits the '\' multi-line wrapping because it
//      triggers warnings on some compilers.
//
//      When examining the above example, it is clear that the parameters map to
//      the SMemoryPoolInfo structure.
//
//      The following notes are worth pointing out about the above definition:
//
//      - The g_uMEMORY_BLOCK_OVERHEAD_SIZE variable holds the overhead
//        size that the CMemoryAllocator allocates for memory management purposes.
//        Here it is used to pad the lower bound and upper bound of the memory
//        pool as it is sometimes useful for the implementation of the allocation
//        functions (last 2 parameters).
//      - The last pool configured does not act as a pool at all because it has
//        zero capacity.
//      - The last pool configures its own allocation subroutines.
//      - The lower bound is exclusive (i.e., a block size of
//        g_uMEMORY_BLOCK_OVERHEAD_SIZE would not be allocated in the first pool).
//      - The upper bound is inclusive.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT,
//      MXD_MEMORY_ALLOCATOR_MEMORY_POOL_ENABLE_SUPPORT
//
//==EDOC========================================================================
#define MXD_astMEMORY_ALLOCATOR_POOL_INFO

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Controls the maximum number of memory block information copies that the
//      CFrameworkInitializer::SFrameworkFinalizeInfo structure holds.
//
// Description:
//      This preprocessor macro can be defined to any number greater than 0. If
//      left undefined, the SFrameworkFinalizeInfo structure uses a default
//      value to initialize the memory snapshot array.
//
//      NOTE: MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT and
//      MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT must be enabled when
//      using this macro.
//
// Location:
//      Define this in "PreMxConfig.h".
//
// See Also:
//      MXD_MEMORY_ALLOCATOR_ENABLE_SUPPORT
//      MXD_MEMORY_ALLOCATOR_MEMORY_TRACKING_ENABLE_SUPPORT
//
//==EDOC========================================================================
#define MXD_FRAMEWORK_FINALIZE_INFO_NUMBER_OF_STORED_LEAKED_MEMORY_BLOCKS

//==SDOC========================================================================
//<GROUP CONFIG_MACROS>
//
// Summary:
//      Determines which file contains the definition of new and delete
//      operators.
//
// Description:
//      This macro determines which file contains the definition of the new and
//      delete operators. In recent C++ compilers this is \<new\>. However, in
//      older compilers, it may be \<new.h\>. Finally, some C++ compilers may
//      automatically define these operators and therefore, no inclusions are
//      needed.
//
//      Define this macro to:
//          - 0: no inclusions are made.
//          - 1: includes \<new\> (this the default behaviour, i.e., When the
//          macro is not defined).
//          - 2: includes \<new.h\>.
//
//      Finally, if the definitions are in another file, define this macro to 0
//      and include the file in PreMxConfig.h.
//
// Location:
//      Define this in "PreMxConfig.h".
//
//==EDOC========================================================================
#define MXD_INCLUDE_NEW

#endif  // #if 0 for documentation.

#endif // #ifndef MXG_MXCONFIG_H
