//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_MXDEF_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXDEF_H
//M5T_INTERNAL_USE_END


// Verify that this header file is included by MxConfig.h only. This
// verification is performed because many macros used throughout this file are
// defined in PreMxConfig.h, which is only included by MxConfig.h. If the direct
// direct inclusion of MxDef.h was to be allowed, the configuration done in
// PreMxConfig.h would be lost.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif


// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined( MXD_OS_BOS_ECOS )
   #include <bosCfg.h>
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

//==============================================================================
//==============================================================================
//
// This file follows Mediatrix Coding Guidelines and defines all permitted basic
// data types and additional keywords. Please refer to the Coding Guidelines for
// more information.
//
//==============================================================================
//==============================================================================


//!=============================================================================
//====  BASIC DATA TYPE DEFINITIONS  ===========================================
//==============================================================================
//
// This section defines data types that can be used in M5T software. Permitted
// data types are divided into three main classes:
//      o ANSI C/C++ standard types.
//      o ISO C99 explicitly sized types.
//      o Specific libc types.
//
// Usage of exact integral types is required because standard C/C++ data types
// are not the same size on all architectures.
//
// When possible, the basic type "typedef" is taken from the operating system or
// standard library definition.
//
// Rules:
// -------------------------------------
// o Only the basic types defined below MUST be used.
// o Usage of the old style boolean types BOOL, TRUE, FALSE is forbidden and
//   programmer MUST rely on the C++ standard boolean types "bool", "true",
//   "false".
// o Usage of API specific types is permitted, but programmers SHOULD limit
//   their propagation when possible.
// o All __uN and __intN types are system specific and MUST NOT be used.
// o For constant not requiring any fixed size, the standard C/C++ int type
//   SHOULD be preferred.
// o Types with a system-dependent size MUST NOT be used when the size of the
//   data type is important.
// o When speed is required and data size has no impact, the programmer SHOULD
//   prefer int to any other type.
//
// Standard ANSI C/C++ Types:
// -------------------------------------
// o bool           [true or false]
// o char           [System dependent]
// o int            [System dependent]
// o unsigned int   [System dependent]
// o float          [±1.175494351 E - 38 .. ±3.402823466 E + 38]
// o double         [±2.2250738585072014 E - 308 .. ±1.7976931348623158 E + 308]
// o void*          [Pointer to any variable type]
//
// ISO C99 Exact Integral Types:
// -------------------------------------
// o int8_t         [-128 .. 127]
// o int16_t        [-32 768 .. 32 767]
// o int32_t        [-2 147 483 648 .. 2 147 483 647]
// o int64_t        [-9 223 372 036 854 775 806 ..  9 223 372 036 854 775 807]
// o uint8_t        [0 .. 255]
// o uint16_t       [0 .. 65 535]
// o uint32_t       [0 .. 4 294 967 295]
// o uint64_t       [0 .. 18 446 744 073 709 551 615]
//
// ISO C99 Pointer Types:
// -------------------------------------
// o intptr_t       [Holds a signed pointer value]
// o uintptr_t      [Holds an unsigned pointer value]
//
// Specific Libc Types:
// -------------------------------------
// o size_t         [sizeof operator result]
// o time_t         [Time values]
// o wchar_t        [Wide character on 2 bytes]
// o ptrdiff_t      [pointers subtraction result]
//
//
// -------------------------------------
// OLD AND DEPRECATED TYPES:
// -------------------------------------
// WARNING: Do not use them!
//
// o CHAR
// o UCHAR
// o SCHAR
// o SHORT
// o USHORT
// o SSHORT
// o LONG
// o ULONG
// o SLONG
// o INT
// o UINT
// o SINT
// o INT8
// o UINT8
// o SINT8
// o INT16
// o UINT16
// o SINT16
// o INT32
// o UINT32
// o SINT32
// o INT64
// o UINT64
// o SINT64
// o FLOAT
// o DOUBLE
//
//------------------------------------------------------------------------------

//M5T_INTERNAL_USE_BEGIN

// Defines parameter directions and data objects ownership keywords:
// -----------------------------------------------------------------------------
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif
#ifdef INOUT
#undef INOUT
#endif
#ifdef TO
#undef TO
#endif
#ifdef TOA
#undef TOA
#endif
#ifdef TOS
#undef TOS
#endif
#ifdef GO
#undef GO
#endif

#define IN
#define OUT
#define INOUT
#define TO
#define TOA
#define TOS
#define GO

#include <stddef.h>
#include <time.h>
#if defined ( MXD_OS_LINUX ) || defined (MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <stdint.h>

#elif defined ( MXD_OS_VXWORKS )
    #include <types/vxTypesBase.h>
    #include <types/vxTypes.h>
    #include <types/vxTypesOld.h>
    #include <limits.h>

// BRCM_CUSTOM - [begin] - Do not define 64 bit types in Vxworks 6.3
    #if (MXD_OS_VXWORKS < 0x060300)
        typedef long long int           int64_t;
        typedef unsigned long long int  uint64_t;
    #endif
// BRCM_CUSTOM - [end] - Do not define 64 bit types in Vxworks 6.3

#elif defined(MXD_OS_WINDOWS) || \
      defined(MXD_OS_WINDOWS_CE)
    #include <windef.h>
    #include <basetsd.h>

        typedef signed char         int8_t;
        typedef unsigned char       uint8_t;
        typedef signed short        int16_t;
        typedef unsigned short      uint16_t;
        typedef signed int          int32_t;
        typedef unsigned int        uint32_t;
        typedef __int64             int64_t;
        typedef unsigned __int64    uint64_t;

#elif defined(MXD_OS_SYMBIAN)
        typedef signed char         int8_t;
        typedef unsigned char       uint8_t;
        typedef signed short        int16_t;
        typedef unsigned short      uint16_t;
        typedef signed int          int32_t;
        typedef unsigned int        uint32_t;
        typedef long long int       int64_t;

        // <COMBINE int64_t>
        typedef unsigned long long int  uint64_t;

#elif defined(MXD_OS_NUCLEUS)

        typedef signed char         int8_t;
        typedef unsigned char       uint8_t;
        typedef signed short        int16_t;
        typedef unsigned short      uint16_t;
        typedef signed int          int32_t;
        typedef unsigned int        uint32_t;
        typedef long long int       int64_t;

        // <COMBINE int64_t>
        typedef unsigned long long int  uint64_t;

// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #include <sys/types.h>
    #include <sys/bsdtypes.h>
// BRCM_CUSTOM - [end] - Add ECOS support.

#elif defined(MXD_OS_NONE)

        //No operating system defined, try to map on something standard.
        typedef signed char         int8_t;
        typedef unsigned char       uint8_t;
        typedef signed short        int16_t;
        typedef unsigned short      uint16_t;
        typedef signed int          int32_t;
        typedef unsigned int        uint32_t;

        #if !defined(MXD_64BITS_SUPPORT_DISABLE)
            #if defined(MXD_64BITS_CUSTOM_TYPE)
                #ifndef  MXG_MX64BITSCUSTOM_H
                #include "Basic/Mx64BitsCustom.h"
                typedef SUInt64 uint64_t;
                #endif
            #else
                typedef long long int   int64_t;
                typedef unsigned long long int  uint64_t;
            #endif
        #endif

#endif

#if !defined(__cplusplus)
    #if defined(MXD_OS_VXWORKS)
        typedef int     bool;
    #else
        typedef char    bool;
    #endif
    #define false   0
    #define true    (!false)
#endif

typedef void* mxt_opaque;

//==============================================================================
//====  ADDITIONAL KEYWORD DEFINITIONS  ========================================
//==============================================================================

// Defines NULL keyword:
//------------------------------------------------------------------------------

// When defined, assume NULL has a correct value. Otherwise assign our own !
#if !defined(NULL)
    #if defined(__cplusplus)
        #define NULL 0
    #else
        #define NULL ((void *)0)
    #endif
#endif
//M5T_INTERNAL_USE_END

//==============================================================================
//====  GENERAL CONSTANT DEFINITIONS  ==========================================
//==============================================================================

//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//  Defines the possible bases that can be used to represent an integer.
//
// Description:
//  This enumeration defines the bases that can be used to represent an integer.
//
//  Location:
//    Defined in Basic/MxTrace.h but must include Config/MxConfig.h to
//    access this.
//
//==EDOC========================================================================
typedef enum
{
    // Description:
    // Base determined automatically. Only used in CSocketAddr.
    eBASE_AUTO        = 0,
    // Description:
    // Binary base.
    eBASE_BINARY      = 2,
    // Description:
    // Octal base.
    eBASE_OCTAL       = 8,
    // Description:
    // Decimal base.
    eBASE_DECIMAL     = 10,
    // Description:
    // Hexadecimal base.
    eBASE_HEXADECIMAL = 16,
    // Description:
    // Invalid base.
    eBASE_INVALID
} EMxBase;

//==SDOC========================================================================
//<GROUP BASIC_VARIABLES>
//
// Summary:
//      Empty c-style string used to replace NULL strings.
//
// Description:
//      This symbol provides access to an empty c-style string that can be used
//      in place of NULL strings.
//
//==EDOC========================================================================
extern const char g_szEMPTY_STRING[];

//==SDOC========================================================================
//<GROUP BASIC_VARIABLES>
//
// Summary:
//      c-style string indicating NULL strings.
//
// Description:
//      This symbol provides access to a c-style string that denotes a NULL
//      string in the same way on all platforms. It is primarily used in the
//      MX_MAKE_STRING_NULL_SAFE macro. It should not be used directly.
//
// See Also:
//     MX_MAKE_STRING_NULL_SAFE
//
//==EDOC========================================================================
extern const char g_szNULL[];

#if 0
//==============================================================================
//====  Documentation  =========================================================
//==============================================================================

//<GROUP BASIC_MACROS>
//<TITLE Parameters qualifier>
//==SDOC========================================================================
//
// Summary:
//  Defines parameter direction and ownership.
//
// Description:
//  Modifiers help to eliminate confusion around parameter direction and
//  ownership acquisition.  The modifiers for direction and ownership can be
//  combined.  Modifiers are used in the member function signature but may also
//  be used in function call to eliminate ambiguity.
//
//#IN:#
//  An input parameter; not modified inside the caller scope.
//
//#OUT:#
//  An output parameter; modified to communicate information to the caller.
//
//#INOUT:#
//  An input and ouput parameter.
//
//#TOA:#
//  Indicates that the ownership of the pointer is always taken from the caller.
//  The caller is no longer responsible for the object deletion.
//
//#TOS:#
//  Indicates that the ownership of the pointer is only taken from the caller on
//  success. The caller is no longer responsible for the object deletion.
//
//#GO:#
//  Indicates that the ownership of the pointer is given to the caller. The
//  caller becomes responsible for the object deletion.
//
// See also:
//  <LINK TO, Deprecated parameters qualifier>
//
//==EDOC========================================================================
#define IN

//<GROUP BASIC_MACROS>
//<COMBINE IN>
#define OUT

//<GROUP BASIC_MACROS>
//<COMBINE IN>
#define INOUT

//<GROUP BASIC_MACROS>
//<COMBINE IN>
#define TOA

//<GROUP BASIC_MACROS>
//<COMBINE IN>
#define TOS

//<GROUP BASIC_MACROS>
//<COMBINE IN>
#define GO

//<GROUP BASIC_DEPRECATED_MACROS>
//<TITLE Deprecated parameters qualifier>
//==SDOC========================================================================
//
// Summary:
//  Defines parameter direction and ownership.
//
// Description:
//  Modifiers help to eliminate confusion around parameter direction and
//  ownership acquisition.  The modifiers for direction and ownership can be
//  combined.  Modifiers are used in the member function signature but may also
//  be used in function call to eliminate ambiguity.
//
//#TO:#
//  Indicates that the ownership of the pointer is taken from the caller. The
//  caller is no longer responsible for the object deletion. The TO Macro is
//  deprecated since 2.1.4. TOA (Take Ownership Always) and TOS (Take Ownership
//  Success) are now used instead.
//
// See also :
//  <LINK IN, Parameters qualifier>
//
//==EDOC========================================================================
#define TO

//<GROUP BASIC_TYPES>
//<TITLE Standard data types>
//==SDOC========================================================================
//
// Summary:
//  Standard data type used throughout M5T's software.
//
// Description:
//  M5T's software is moving towards the adoption of both the standard ANSI
//  C/C++ types and the more recent ISO C99 Exact Integral Types.
//
//  If the ISO C99 types are not supported or defined by your compiler, then the
//  file Basic/MxDef.h defines these types to properly compile M5T's software.
//
//  For more information about how these types are defined for your specific
//  compiler and platform, please refer to the up-to-date definition found in
//  Basic/MxDef.h.
//
//#Standard ANSI C/C++ Types#
//
//  The following standard types are used:
//
//  <TABLE>
//  bool           [true or false]
//  char           [System dependent]
//  int            [System dependent]
//  unsigned int   [System dependent]
//  float          [±1.175494351 E - 38 .. ±3.402823466 E + 38]
//  double         [±2.2250738585072014 E - 308 .. ±1.7976931348623158 E + 308]
//  void*          [Pointer to any variable type]
//  </TABLE>
//
//#ISO C99 Exact Integral Types#
//
//  ISO C99 Exact Integral Types defines the following types:
//
//  <TABLE>
//  int8_t         [-128 .. 127]
//  int16_t        [-32 768 .. 32 767]
//  int32_t        [-2 147 483 648 .. 2 147 483 647]
//  int64_t        [-9 223 372 036 854 775 806 ..  9 223 372 036 854 775 807]
//  uint8_t        [0 .. 255]
//  uint16_t       [0 .. 65 535]
//  uint32_t       [0 .. 4 294 967 295]
//  uint64_t       [0 .. 18 446 744 073 709 551 615]
//  </TABLE
//
//#Deprecated Data Types#
//
//  The following data types are still used within some of the M5T's software
//  but it is being slowly replaced by the previously defined data types.
//
//  <TABLE>
//  CHAR      (Use "\char" instead)
//  UCHAR     (Use "\uint8_t" instead)
//  SCHAR     (Use "\int8_t" instead)
//  SHORT     (Use "\int16_t" instead)
//  USHORT    (Use "\uint16_t" instead)
//  SSHORT    (Use "\int16_t" instead)
//  LONG      (Use "\long" instead)
//  ULONG     (Use "\unsigned long" instead)
//  SLONG     (Use "\long" instead)
//  INT       (Use "\int" instead)
//  UINT      (Use "\unsigned int" instead)
//  SINT      (Use "\int" instead)
//  INT8      (Use "\int8_t" instead)
//  UINT8     (Use "\uint8_t" instead)
//  SINT8     (Use "\int8_t" instead)
//  INT16     (Use "\int16_t" instead)
//  UINT16    (Use "\uint16_t" instead)
//  SINT16    (Use "\int16_t" instead)
//  INT32     (Use "\int32_t" instead)
//  UINT32    (Use "\uint32_t" instead)
//  SINT32    (Use "\int32_t" instead)
//  INT64     (Use "\int64_t" instead)
//  UINT64    (Use "\uint64_t" instead)
//  SINT64    (Use "\int64_t" instead)
//  FLOAT     (Use "\float" instead)
//  DOUBLE    (Use "\double" instead)
//  </TABLE>
//
// Location:
//  Basic/MxDef.h
//
//==EDOC========================================================================
typedef signed char             int8_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef unsigned char           uint8_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef signed short            int16_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef unsigned short          uint16_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef signed int              int32_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef unsigned int            uint32_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef long long int           int64_t;

//<GROUP BASIC_TYPES>
//<COMBINE int8_t>
typedef unsigned long long int  uint64_t;

//==SDOC========================================================================
//<GROUP BASIC_TYPES>
//
// Summary:
//  Custom data type that allows opaque type usage.
//
// Description:
//  The mxt_opaque custom data type allows opaque type usage. It is defined as
//  a void* to be able to contain a pointer regardless of the architecture
//  (32 or 64 bits). Since a mxt_opaque is a pointer, it is necessary to use a
//  reinterpret_cast to assign a non pointer value to a mxt_opaque. To ease
//  the mxt_opaque conversions, the following macros are available:
//  - MX_VOIDPTR_TO_OPQ(pParam)
//  - MX_OPQ_TO_VOIDPTR(opq)
//  - MX_INT32_TO_OPQ(nParam)
//  - MX_OPQ_TO_INT32(opq)
//  - MX_UINT32_TO_OPQ(uParam)
//  - MX_OPQ_TO_UINT32(opq)
// 
//  For example, use the following code to assign an integer to a mxt_opaque:
//
//  <code>
//  mxt_opaque opq = MX_INT32_TO_OPQ(1);
//  </code>
//
//  Since it is not permitted to do arithmetics on a void*, it is not permitted
//  on a mxt_opaque either. For example, the following code is not valid:
//
//  <code>
//  mxt_opaque opq = MX_INT32_TO_OPQ(0);
//  opq++;
//  </code>
//
//  One should use a variation of the following code instead:
//
//  <code>
//  mxt_opaque opq = MX_UINT32_TO_OPQ(0);
//  unsigned int uValue = 0;
//  uValue++;
//  opq = MX_UINT32_TO_OPQ(uValue);
//  </code>
//
//  When assigning a class member that is a pointer to a mxt_opaque from
//  within a const method, it is necessary to do a const cast followed by a
//  reinterpret_cast, as in the following code:
//
//  <code>
//  class CExample
//  {
//  public:
//      void GetOpaqueCount(OUT mxt_opaque opq) const
//      {
//          opq = MX_VOIDPTR_TO_OPQ(const_cast<unsigned int*>(&m_uCount));
//      };
//  private:
//      unsigned int m_uCount;
//  };
//  </code>
//
// Location:
//  Basic/MxDef.h
//
//==EDOC========================================================================
typedef void* mxt_opaque;

#endif // #if 0 --> end of documentation section.

#endif // #ifndef MXG_MXDEF_H


