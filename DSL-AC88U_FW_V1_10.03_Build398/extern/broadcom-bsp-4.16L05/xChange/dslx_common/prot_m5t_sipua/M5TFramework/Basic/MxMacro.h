//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_MXMACRO_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXMACRO_H
//M5T_INTERNAL_USE_END


// Verify that this header file is included by MxConfig.h only. This
// verification is performed because many macros used throughout this file are
// defined in PreMxConfig.h, which is only included by MxConfig.h. If we were
// to allow the direct inclusion of MxMacro.h, we would loose the configuration
// done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif

#if !defined(MXD_OS_WINDOWS_CE)
    #include <errno.h>
#endif

#if defined(MXD_OS_VXWORKS)
    #include <errnoLib.h>
#endif

//==============================================================================
//====  #define Documentation  =================================================
//==============================================================================

//@@MXD_ENABLE_NAMESPACE
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MXD_ENABLE_NAMESPACE
//==
//==============================================================================
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
//  MX_NAMESPACE_START, MX_NAMESPACE_END, MX_NAMESPACE_USE
//
//==EDOC========================================================================


// Namespace Macros:
//------------------------------------------------------------------------------
#undef MX_NAMESPACE_START
#undef MX_NAMESPACE_END
#undef MX_NAMESPACE_USE

// Automatically disable namespaces on gcc 2.7.2 or earlier because namespaces
// are not supported.
#if defined(MXD_COMPILER_GNU_GCC)
     #if (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
        #undef MXD_ENABLE_NAMESPACE
     #endif
#endif

#if !defined (__cplusplus)
    #undef MXD_ENABLE_NAMESPACE
#endif

#if defined(MXD_ENABLE_NAMESPACE)

    //@@MXD_GNS
    //<GROUP BASIC_MACROS>
    //==SDOC====================================================================
    //==
    //==  MXD_GNS
    //==
    //==========================================================================
    //
    // Summary:
    //  Defines the global namespace.
    //
    // Description:
    //  When namespacing is enabled, defines the name assigned to the namespace
    //  of this product. Defaults to m5t.
    //
    // Location:
    //  Define this in "PreMxConfig.h" to change the default value.
    //
    // See Also:
    //  MX_NAMESPACE_START, MX_NAMESPACE_END, MX_NAMESPACE_USE
    //
    //==EDOC====================================================================
    #if !defined MXD_GNS
    //M5T_INTERNAL_USE_BEGIN
    #define MXD_GNS  m5t
    //M5T_INTERNAL_USE_END
    #endif

    //==SDOC====================================================================
    //==
    //==  MX_NAMESPACE_START
    //==
    //==========================================================================
    //<GROUP BASIC_MACROS>
    //
    // Summary:
    //   Starts a namespace.
    //
    // Parameters:
    //  x: Name of the namespace to start.
    //
    // Description:
    //  When namespacing is enabled, marks the beginning of a code section to be
    //  interpreted within a certain namespace.
    //
    // See Also:
    //  MXD_ENABLE_NAMESPACE, MX_NAMESPACE_END, MX_NAMESPACE_USE
    //
    //==EDOC====================================================================
    #define MX_NAMESPACE_START(x) namespace x {

    //==SDOC====================================================================
    //==
    //==  MX_NAMESPACE_END
    //==
    //==========================================================================
    //<GROUP BASIC_MACROS>
    //
    // Summary:
    //  Ends a namespace.
    //
    // Parameters:
    //  x: Name of the namespace to end.
    //
    // Description:
    //  When namespacing is enabled, marks the end of a code section to be
    //  interpreted within a certain namespace.
    //
    // See Also:
    //  MXD_ENABLE_NAMESPACE, MX_NAMESPACE_START, MX_NAMESPACE_USE
    //
    //==EDOC====================================================================
    #define MX_NAMESPACE_END(x)   }

    //==SDOC====================================================================
    //==
    //==  MX_NAMESPACE_USE
    //==
    //==========================================================================
    //<GROUP BASIC_MACROS>
    //
    // Summary:
    //  Uses a namespace.
    //
    // Parameters:
    //  x: Name of the namespace to use.
    //
    // Description:
    //  When namespacing is enabled, notifies the compiler that the following
    //  code section can use variables or methods that are defined in another
    //  namespace.
    //
    // See Also:
    //  MXD_ENABLE_NAMESPACE, MX_NAMESPACE_START, MX_NAMESPACE_END
    //
    //==EDOC====================================================================
    #define MX_NAMESPACE_USE(x)   using namespace x;

    //==SDOC====================================================================
    //==
    //==  MX_NAMESPACE
    //==
    //==========================================================================
    //<GROUP BASIC_MACROS>
    //
    // Summary:
    //  Specifies a namespace for a variable or method.
    //
    // Parameters:
    //  x: Name of the namespace to use.
    //
    // Description:
    //  When namespacing is enabled, specifies a namespace for a variable or a
    //  method.  For instance, if nTest is a variable defined within the MxTest
    //  namespace, it can be referenced by MX_NAMESPACE(MxTest)nTest from
    //  outside the MxTest namespace.
    //
    // See Also:
    //  MXD_ENABLE_NAMESPACE, MX_NAMESPACE_START, MX_NAMESPACE_END
    //
    //==EDOC====================================================================
    #define MX_NAMESPACE(x) x::
#else
//M5T_INTERNAL_USE_BEGIN
    #define MXD_GNS
    #define MX_NAMESPACE_START(x)
    #define MX_NAMESPACE_END(x)
    #define MX_NAMESPACE_USE(x)
    #define MX_NAMESPACE(x)
//M5T_INTERNAL_USE_END
#endif

// Size of Array Macro:
//------------------------------------------------------------------------------
#undef MX_SIZEOFARRAY

//==SDOC========================================================================
//==
//==  MX_SIZEOFARRAY
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Returns the size of an array.
//
// Parameters:
//  array: Array for which to get the size.
//
// Description:
//  Returns the size of an array of elements in element count.
//
//==EDOC========================================================================
#define MX_SIZEOFARRAY(array) (sizeof (array) / sizeof (array[0]))


// Integer Manipulation Macros:
//------------------------------------------------------------------------------
#undef MX_DEFINEUINT64
#undef MX_DEFINEINT64
#undef MX_MAKEUINT16
#undef MX_MAKEUINT32
#undef MX_MAKEUINT64
#undef MX_LOW8
#undef MX_LOW16
#undef MX_LOW32
#undef MX_HI8
#undef MX_HI16
#undef MX_HI32

#if 0
//==SDOC========================================================================
//==
//==  MX_DEFINEUINT64
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Defines a 64 bits unsigned integer by adding the required suffix.
//
// Parameters:
//  u64bitVal: 64 bits value to define.
//
// Description:
//  Defines a 64 bits unsigned integer by adding the required suffix. For
//  example, instead of using:
//
//  <CODE>
//      const uint64_t u64var = 0x123456789abcdef0ui64; // with MSVC
//      const uint64_t u64var = 0x123456789abcdef0ULL;  // with GCC
//  </CODE>
//
//  use:
//  <CODE>
//      const uint64_t u64var = MX_DEFINEUINT64(0x123456789abcdef0);
//  </CODE>
//
// See Also:
//  MX_DEFINEINT64
//
//==EDOC========================================================================
#define MX_DEFINEUINT64(u64bitVal)

//==SDOC========================================================================
//==
//==  MX_DEFINEINT64
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Defines a 64 bits integer by adding the required suffix.
//
// Parameters:
//  u64bitVal: 64 bits value to define.
//
// Description:
//  Defines a 64 bits integer by adding the required suffix. For example,
//  instead of using:
//  <CODE>
//      const int64_t u64var = 0x123456789abcdef0i64; // with MSVC
//      const int64_t u64var = 0x123456789abcdef0LL;  // with GCC
//  </CODE>
//
//  use:
//  <CODE>
//      const int64_t u64var = MX_DEFINEINT64(0x123456789abcdef0);
//  </CODE>
//
// See Also:
//  MX_DEFINEUINT64
//
//==EDOC========================================================================
#define MX_DEFINEINT64(u64bitVal)

#endif // #if 0

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_MS_VC)
    #define MX_DEFINEUINT64(u64bitVal) u64bitVal ## ui64

#elif defined ( MXD_COMPILER_GNU_GCC ) && defined ( MXD_DATA_MODEL_LP64 )
    #define MX_DEFINEUINT64(u64bitVal) u64bitVal ## UL

#else
    #define MX_DEFINEUINT64(u64bitVal) u64bitVal ## ULL
#endif

#if defined(MXD_COMPILER_MS_VC)
    #define MX_DEFINEINT64(u64bitVal)  u64bitVal ## i64

#elif defined ( MXD_COMPILER_GNU_GCC ) && defined ( MXD_DATA_MODEL_LP64 )
    #define MX_DEFINEINT64(u64bitVal) u64bitVal ## L

#else
    #define MX_DEFINEINT64(u64bitVal)  u64bitVal ## LL
#endif
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  MX_MAKEUINT16
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Makes a 16 bits unsigned integer from two 8 bits, hi and low portions.
//
// Parameters:
//  high: Higher 8 bits of the unsigned integer to define.
//  low: Lower 8 bits of the unsigned integer to define.
//
// Description:
//  Makes a 16 bits unsigned integer from two 8 bits, hi and low portions.
//
// See Also:
//  MX_MAKEUINT32, MX_MAKEUINT64
//
//==EDOC========================================================================
#define MX_MAKEUINT16(high, low)    \
            static_cast<uint16_t>(  \
                static_cast<uint16_t>(high) << 8 | static_cast<uint8_t>(low) )

//==SDOC========================================================================
//==
//==  MX_MAKEUINT32
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Makes a 32 bits unsigned integer from two 16 bits, hi and low portions.
//
// Parameters:
//  high: Higher 16 bits of the unsigned integer to define.
//  low: Lower 16 bits of the unsigned integer to define.
//
// Description:
//  Makes a 32 bits unsigned integer from two 16 bits, hi and low portions.
//
// See Also:
//  MX_MAKEUINT16, MX_MAKEUINT64
//
//==EDOC========================================================================
#define MX_MAKEUINT32(high, low)    \
            static_cast<uint32_t>(  \
                static_cast<uint32_t>(high) << 16 | static_cast<uint16_t>(low) )

//@@MX_MAKEUINT64
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_MAKEUINT64
//==
//==============================================================================
//
// Summary:
//  Makes a 64 bits unsigned integer from two 32 bits, hi and low portions.
//
// Parameters:
//  high: Higher 32 bits of the unsigned integer to define.
//  low: Lower 32 bits of the unsigned integer to define.
//
// Description:
//  Makes a 64 bits unsigned integer from two 32 bits, hi and low portions.
//
// See Also:
//  MX_MAKEUINT16, MX_MAKEUINT32
//
//==EDOC========================================================================
#if defined(MXD_64BITS_SUPPORT_DISABLE)
    #define MX_MAKEUINT64(high, low)
#elif defined(MXD_64BITS_CUSTOM_TYPE)
    #define MX_MAKEUINT64(high, low) uint64_t(high, low)
#else
//M5T_INTERNAL_USE_BEGIN
    #if defined(MXD_COMPILER_GNU_GCC)
        #if (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
            // The gcc 2.7.2 compiler warns that uint64_t variables might be
            // used uninitialized unless they are declated using a function
            // returned value.
            uint64_t MxMakeUint64(uint32_t uHigh, uint32_t uLow);
            #define MX_MAKEUINT64(high, low) MxMakeUint64(high, low)
        #endif
    #endif
//M5T_INTERNAL_USE_END

    #if !defined(MX_MAKEUINT64)
        #define MX_MAKEUINT64(high, low) \
                static_cast<uint64_t>(   \
                    static_cast<uint64_t>(high) << 32 | static_cast<uint32_t>(low) )
    #endif
#endif

//==SDOC========================================================================
//==
//==  MX_LOW8
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Gets the low 8 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the lowest 8 bits.
//
// Description:
//  Gets the low 8 bits portion of an integer.
//
// See Also:
//  MX_LOW16, MX_LOW32, MX_HI8, MX_HI16, MX_HI32
//
//==EDOC========================================================================
#define MX_LOW8(n) \
            (static_cast<uint8_t>(n))

//==SDOC========================================================================
//==
//==  MX_LOW16
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Gets the low 16 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the lowest 16 bits.
//
// Description:
//  Gets the low 16 bits portion of an integer.
//
// See Also:
//  MX_LOW8, MX_LOW32, MX_HI8, MX_HI16, MX_HI32
//
//==EDOC========================================================================
#define MX_LOW16(n) \
            (static_cast<uint16_t>(n))

//==SDOC========================================================================
//==
//==  MX_LOW32
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Gets the low 32 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the lowest 32 bits.
//
// Description:
//  Gets the low 32 bits portion of an integer.
//
// See Also:
//  MX_LOW8, MX_LOW16, MX_HI8, MX_HI16, MX_HI32
//
//==EDOC========================================================================
#define MX_LOW32(n) \
            (static_cast<uint32_t>(n))

//==SDOC========================================================================
//==
//==  MX_HI8
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Gets the hi 8 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the highest 8 bits.
//
// Description:
//  Gets the hi 8 bits portion of an integer.
//
// See Also:
//  MX_LOW8, MX_LOW16, MX_LOW32, MX_HI16, MX_HI32
//
//==EDOC========================================================================
#define MX_HI8(n) \
            (static_cast<uint8_t>((static_cast<uint16_t>(n) >> 8) & 0x00FF))

//==SDOC========================================================================
//==
//==  MX_HI16
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Gets the hi 16 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the highest 16 bits.
//
// Description:
//  Gets the hi 16 bits portion of an integer.
//
// See Also:
//  MX_LOW8, MX_LOW16, MX_LOW32, MX_HI8, MX_HI32
//
//==EDOC========================================================================
#define MX_HI16(n) \
            (static_cast<uint16_t>((static_cast<uint32_t>(n) >> 16) & 0x0000FFFF))

//@@MX_HI32
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_HI32
//==
//==============================================================================
//
// Summary:
//  Gets the hi 32 bits portion of an integer.
//
// Parameters:
//  n: Integer from which to get the highest 32 bits.
//
// Description:
//  Gets the hi 32 bits portion of an integer.
//
// See Also:
//  MX_LOW8, MX_LOW16, MX_LOW32, MX_HI8, MX_HI16
//
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_COMPILER_GNU_GCC)
    #if (MXD_COMPILER_GNU_GCC <= MXD_COMPILER_GNU_GCC_2_7)
        // When compiling with GCC 2.7.2 on PowerPC, the shift operator
        // (in assembler) can only shift up to 31 bit. By shifting 32 bit GCC
        // generates warnings.
        uint32_t MxHigh32(uint64_t uVal);
        #define MX_HI32(n) MxHigh32(n)
    #endif
#endif // #if defined(MXD_COMPILER_GNU_GCC)
//M5T_INTERNAL_USE_END

#if !defined(MX_HI32)
    #define MX_HI32(n) (static_cast<uint32_t>((static_cast<uint64_t>(n) >> 32)))
#endif

// Byte Swapping Macros:
//------------------------------------------------------------------------------

//==SDOC========================================================================
//==
//==  MX_SWAPBYTES16
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Swaps the hi and low bytes within a 16 bits unsigned integer.
//
//  Parameters:
//    un:
//      16 bits unsigned integer that needs its bytes swapped.
//
//  Returns:
//    The bytes swapped resulting 16 bits unsigned integer.
//
//  Description:
//    Swaps the hi and low bytes within a 16 bits unsigned integer. 0xaabb
//    becomes 0xbbaa.
//
//==EDOC========================================================================
#define MX_SWAPBYTES16(un) ((uint16_t)(((uint16_t)un << 8) | ((uint16_t)un >> 8)))

//==SDOC========================================================================
//==
//==  MX_SWAPBYTES
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//    Swaps the hi and low end bytes.
//
//  Parameters:
//    un:
//      16/32/64 bits unsigned integer that needs its bytes swapped.
//
//  Returns:
//    The bytes swapped resulting 16/32/64 bits unsigned integer.
//
//  Description:
//    Swaps the hi and low bytes within a 16/32/64 bits unsigned integer. 0xaabb
//    becomes 0xbbaa for 16 bits.
//
//==EDOC========================================================================
#if defined(__cplusplus)
inline uint16_t MX_SWAPBYTES(IN uint16_t un)
{
    return MX_SWAPBYTES16(un);
}
#endif // #if defined(__cplusplus)

//==SDOC========================================================================
//==
//==  MX_SWAPBYTES32
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Swaps the hi and low bytes within a 32 bits unsigned integer.
//
//  Parameters:
//    un:
//      32 bits unsigned integer that needs its bytes swapped.
//
//  Returns:
//    The bytes swapped resulting 32 bits unsigned integer.
//
//  Description:
//    Swaps all 4 bytes within a 32 bits unsigned integer.  0xaabbccdd becomes
//    0xddccbbaa.
//
//==EDOC========================================================================
#define MX_SWAPBYTES32(un) ((uint32_t)                                      \
                           (((uint32_t)un >> 24) | ((uint32_t)un << 24) |   \
                           (((uint32_t)un & 0x00ff0000ul) >> 8) |           \
                           (((uint32_t)un & 0x0000ff00ul) << 8))            )

//<GROUP BASIC_MACROS>
//<COMBINE MX_SWAPBYTES@IN uint16_t>
#if defined(__cplusplus)
inline uint32_t MX_SWAPBYTES(IN uint32_t un)
{
    return MX_SWAPBYTES32(un);
}
#endif // #if defined(__cplusplus)

//@@MX_SWAPBYTES64
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_SWAPBYTES64
//==
//==============================================================================
//
//  Summary:
//   Swaps the hi and low bytes within a 64 bits unsigned integer.
//
//  Parameters:
//    un:
//      64 bits unsigned integer that needs its bytes swapped.
//
//  Returns:
//    The bytes swapped resulting 64 bits unsigned integer.
//
//  Description:
//    Swaps all 8 bytes within a 64 bits unsigned integer. 0x11223344aabbccdd
//    becomes 0xddccbbaa44223311.
//
//==EDOC========================================================================
#if !defined(MXD_64BITS_SUPPORT_DISABLE)

    #define MX_SWAPBYTES64(un)((uint64_t)                                                     \
                               (((uint64_t)un >> 56) | ((uint64_t)un << 56)                 | \
                               (((uint64_t)un & MX_MAKEUINT64(0x00ff0000,0x00000000)) >> 40) | \
                               (((uint64_t)un & MX_MAKEUINT64(0x00000000,0x0000ff00)) << 40) | \
                               (((uint64_t)un & MX_MAKEUINT64(0x0000ff00,0x00000000)) >> 24) | \
                               (((uint64_t)un & MX_MAKEUINT64(0x00000000,0x00ff0000)) << 24) | \
                               (((uint64_t)un & MX_MAKEUINT64(0x000000ff,0x00000000)) >> 8)  | \
                               (((uint64_t)un & MX_MAKEUINT64(0x00000000,0xff000000)) << 8) )  )

//<GROUP BASIC_MACROS>
//<COMBINE MX_SWAPBYTES@IN uint16_t>
#if defined(__cplusplus)
inline uint64_t MX_SWAPBYTES(IN uint64_t un)
{
    return MX_SWAPBYTES64(un);
}
#endif // #if defined(__cplusplus)

#endif // #if !defined(MXD_64BITS_SUPPORT_DISABLE)

#if defined(__cplusplus)

// Value Alignment Macros:
//------------------------------------------------------------------------------
#if defined(_Type) || defined(_PtrType)
    #error ""_Type" or ""_PtrType is already defined as a MACRO. "\
           "Cannot be used again in template definition."
#endif

//==SDOC========================================================================
//==
//==  MX_ALIGNUP_PTR
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//      This function returns the next aligned pointer.
//
//  Parameters:
//    ptr:
//      Pointer to be realigned.
//
//    alignment:
//      Alignment in bytes.
//
//  Returns:
//      A pointer realigned on a frontier of "alignment" bytes.
//
//  Description:
//      This function returns the next aligned pointer, starting at ptr, with
//      regards to alignment.
//
//  See Also:
//      MX_ALIGNDOWN_PTR
//
//==EDOC========================================================================
template<class _PtrType> inline
_PtrType MX_ALIGNUP_PTR(IN _PtrType ptr, IN ptrdiff_t alignment)
{
    return reinterpret_cast<_PtrType>(
                (reinterpret_cast<ptrdiff_t>(ptr) + alignment - 1) &
                ~(alignment - 1) );
}

//==SDOC========================================================================
//==
//==  MX_ALIGNDOWN_PTR
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//      This function returns the previous align pointer.
//
//  Parameters:
//    ptr:
//      Pointer to be realigned.
//
//    alignment:
//      Alignment in bytes.
//
//  Returns:
//      A pointer realigned on a frontier of "alignment" bytes.
//
//  Description:
//      This function returns the previous align pointer, starting at ptr, with
//      regards to alignment.
//
//  See Also:
//      MX_ALIGNUP_PTR
//
//==EDOC========================================================================
template<class _PtrType> inline
_PtrType MX_ALIGNDOWN_PTR(IN _PtrType ptr, IN ptrdiff_t alignment)
{
    return reinterpret_cast<_PtrType>(reinterpret_cast<ptrdiff_t>(ptr) & ~(alignment - 1));
}

//==SDOC========================================================================
//==
//==  MX_ALIGNUP_VAL
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//      This function returns the next aligned value.
//
//  Parameters:
//    val:
//      A value stored in type _Type to be realigned. Here, _Type is used to
//      represent any basic data type that can contain a numeric value.
//
//    alignment:
//      Alignment in bytes.
//
//  Returns:
//      The realigned numeric value in _Type on a frontier of "alignment" bytes.
//
//  Description:
//      This function returns the next aligned value, starting at the value
//      contained in val, with regards to alignment.
//
//  See Also:
//      MX_ALIGNDOWN_VAL
//
//==EDOC========================================================================
template<class _Type> inline
_Type MX_ALIGNUP_VAL(IN _Type val, IN size_t alignment)
{
    return ((val + alignment - 1) & ~(alignment - 1));
}

//==SDOC========================================================================
//==
//==  MX_ALIGNDOWN_VAL
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//      This function returns the previous aligned value.
//
//  Parameters:
//    val:
//      A value stored in type _Type to be realigned. Here, _Type is used to
//      represent any basic data type that can contain a numeric value.
//
//    alignment:
//      Alignment in bytes.
//
//  Returns:
//      The realigned numeric value in _Type on a frontier of "alignment" bytes.
//
//  Description:
//      This function returns the previous aligned value, starting at the value
//      contained in val, with regards to alignment.
//
//  See Also:
//      MX_ALIGNUP_VAL
//
//==EDOC========================================================================
template<class _Type> inline
_Type MX_ALIGNDOWN_VAL(IN _Type val, IN size_t alignment)
{
    return (val & ~(alignment - 1));
}

#endif // #if defined(__cplusplus)


// Min and Max Macros:
//------------------------------------------------------------------------------
#undef Max // deprecated, do not use
#undef Min // deprecated, do not use
#undef MX_MAX
#undef MX_MIN

//==SDOC========================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Returns the largest of two values.
//
// Parameters:
//  x: First value to compare.
//  y: Second value to compare.
//
// Description:
//  Returns the largest of two values.
//
// See Also:
//  MX_MIN.
//
//==EDOC========================================================================
#define MX_MAX(x,y) ((x) > (y) ? (x) : (y))

//==SDOC========================================================================
//<GROUP BASIC_MACROS>
//
// Summary:
//  Returns the smallest of two values.
//
// Parameters:
//  x: First value to compare.
//  y: Second value to compare.
//
// Description:
//  Returns the smallest of two values.
//
// See Also:
//  MX_MAX.
//
//==EDOC========================================================================
#define MX_MIN(x,y) ((x) > (y) ? (y) : (x))


// errno manipulation:
//------------------------------------------------------------------------------
#undef MX_ERRNO_SET

//@@MX_ERRNO_SET
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_ERRNO_SET
//==
//==============================================================================
//
// Summary:
//  Sets the errno value.
//
// Parameters:
//  ErrorCode: Value to set.
//
// Description:
//  Sets the errno value.
//
//==EDOC========================================================================
#if defined ( MXD_OS_VXWORKS )
    #define MX_ERRNO_SET(ErrorCode)      (errnoSet((ErrorCode)))
#elif defined ( MXD_OS_WINDOWS_CE)
    #define MX_ERRNO_SET(ErrorCode)      (SetLastError((DWORD)ErrorCode))
#else
    #define MX_ERRNO_SET(ErrorCode)      (errno = (ErrorCode))
#endif

//@@MX_ERRNO_GET
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_ERRNO_GET
//==
//==============================================================================
//
// Summary:
//  Gets the errno value.
//
// Description:
//  Gets the errno value.
//
//==EDOC========================================================================
#if defined ( MXD_OS_VXWORKS )
    #define MX_ERRNO_GET() (errnoGet())
#elif defined ( MXD_OS_WINDOWS_CE)
    #define MX_ERRNO_GET() (GetLastError())
#else
    #define MX_ERRNO_GET() (errno)
#endif

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//==
//==  Host/Network byte order conversion.
//==
//==============================================================================
//
// Description:
//   Defines standard network to host and host to network integer conversion
//   when not available.
//
//==EDOC========================================================================
#if (defined(MXD_OS_NONE) || defined(MXD_OS_NUCLEUS)) && !defined(ntohl)
    #if defined(htonl)
        #error "Something wrong...htonl() defined but not ntohl()"
    #endif
    #if defined(MXD_LITTLE_ENDIAN)
        #define ntohl(x)    MX_SWAPBYTES32(x)
        #define ntohs(x)    MX_SWAPBYTES16(x)
        #define htonl(x)    MX_SWAPBYTES32(x)
        #define htons(x)    MX_SWAPBYTES16(x)
    #elif defined(MXD_BIG_ENDIAN)
        #define ntohl(x)    (x)
        #define ntohs(x)    (x)
        #define htonl(x)    (x)
        #define htons(x)    (x)
    #endif
#endif

#if (!defined(MXD_64BITS_SUPPORT_DISABLE) && !defined(ntoh64))
    #if defined(hton64)
        #error "Something wrong...ntoh64() defined but not hton64()"
    #endif
    #if defined(MXD_LITTLE_ENDIAN)
        #define ntoh64(x)   MX_SWAPBYTES64(x)
        #define hton64(x)   MX_SWAPBYTES64(x)
    #elif defined(MXD_BIG_ENDIAN)
        #define ntoh64(x)   (x)
        #define hton64(x)   (x)
    #endif
#endif
//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//==
//==  MX_NTOH64
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 64 bit word from network to host byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to host byte order.
//
//  Returns:
//   The value in host byte order.
//
//  Description:
//   Macro used to convert a 64 bit word from network to host byte
//   order. It is guaranteed to always return a value of uint64_t type.
//
//==EDOC========================================================================
#define MX_NTOH64(uParam)    (uint64_t)ntoh64(uParam)

//==SDOC========================================================================
//==
//==  MX_NTOHL
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 32 bit unsigned integer from network to host byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to host byte order.
//
//  Returns:
//   The value in host byte order.
//
//  Description:
//   Macro used to convert a 32 bit unsigned integer from network to host byte
//   order. It is guaranteed to always return a value of uint32_t type.
//
//==EDOC========================================================================
#define MX_NTOHL(uParam)    (uint32_t)ntohl(uParam)

//==SDOC========================================================================
//==
//==  MX_NTOHS
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 16 bit unsigned integer from network to host byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to host byte order.
//
//  Returns:
//   The value in host byte order.
//
//  Description:
//   Macro used to convert a 16 bit unsigned integer from network to host byte
//   order. It is guaranteed to always return a value of uint16_t type.
//
//==EDOC========================================================================
#define MX_NTOHS(uParam)    (uint16_t)ntohs(uParam)

//==SDOC========================================================================
//==
//==  MX_HTON64
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 64 bit word from host to network byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to network byte order.
//
//  Returns:
//   The value in network byte order.
//
//  Description:
//   Macro used to convert a 64 bit word from host to network byte
//   order. It is guaranteed to always return a value of uint64_t type.
//
//==EDOC========================================================================
#define MX_HTON64(uParam)    (uint64_t)hton64(uParam)

//==SDOC========================================================================
//==
//==  MX_HTONL
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 32 bit unsigned integer from host to network byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to network byte order.
//
//  Returns:
//   The value in network byte order.
//
//  Description:
//   Macro used to convert a 32 bit unsigned integer from host to network byte
//   order. It is guaranteed to always return a value of uint32_t type.
//
//==EDOC========================================================================
#define MX_HTONL(uParam)    (uint32_t)htonl(uParam)

//==SDOC========================================================================
//==
//==  MX_HTONS
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//   Macro to convert a 16 bit unsigned integer from host to network byte
//   order.
//
//  Parameters:
//   uParam:
//    Value to convert to network byte order.
//
//  Returns:
//   The value in network byte order.
//
//  Description:
//   Macro used to convert a 16 bit unsigned integer from host to network byte
//   order. It is guaranteed to always return a value of uint16_t type.
//
//==EDOC========================================================================
#define MX_HTONS(uParam)    (uint16_t)htons(uParam)

//@@MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING
//==SDOC========================================================================
//==
//==  MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING
//==
//==============================================================================
//<GROUP BASIC_MACROS>
//
//  Summary:
//      Removes warning about unused function parameters.
//
//  Parameters:
//    rParam:
//      Unused parameter for which to remove warning.
//
//  Returns:
//      Returns a constant reference on the parameter itself.
//
//  Description:
//      Macro used to avoid the "unused function parameter" compiler warning. In
//      optimized mode, this macro does not generate any code.
//
//==EDOC========================================================================
#if defined(__cplusplus)
template<class _Type> inline
const _Type& MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(IN const _Type& rParam)
{
    return rParam;
}
#else
    #define MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(x) MxRemoveUnusedFunctionParamWarning((void*)&(x));
#endif // #if defined(__cplusplus)

//M5T_INTERNAL_USE_BEGIN
#ifdef __cplusplus
extern "C" {
#endif
    void* MxRemoveUnusedFunctionParamWarning(void* pPtr);
#ifdef __cplusplus
}
#endif
//M5T_INTERNAL_USE_END

//@@MX_MAKE_STRING_NULL_SAFE
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_MAKE_STRING_NULL_SAFE
//==
//==============================================================================
//
//  Summary:
//    Converts a NULL string to its system-dependent equivalent.
//
//  Parameters:
//    pszValue:
//      The string to make null safe.
//
//  Returns:
//    If pszValue is non-NULL, the current value of pszValue. If pszValue is
//    NULL, the special string g_szNULL.
//
//  Description:
//    This macro converts a NULL string to its system-dependent equivalent. It
//    is useful mainly for tracing and for calls to system run-time libraries.
//    It helps the developer to prevent crashes caused by NULL strings.
//
//  <CODE>
//  void PrintStringToStdout(const char* pszStringToPrint)
//  {
//      // pszStringToPrint comes from an unknown source.
//      // Make sure it will not crash the system on platforms that do not
//      // check for NULL string in the run-time libraries (i.e. Symbian).
//      printf("%s", MX_MAKE_STRING_NULL_SAFE(pszStringToPrint));
//  }
//  </CODE>
//
//==EDOC========================================================================
#if defined(MXD_OS_SYMBIAN)
#define MX_MAKE_STRING_NULL_SAFE(pszValue) (pszValue != NULL ? pszValue : g_szNULL)
#else
#define MX_MAKE_STRING_NULL_SAFE(pszValue) pszValue
#endif

//@@MX_MAKE_NULL_EMPTY_STRING
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_MAKE_NULL_EMPTY_STRING
//==
//==============================================================================
//
//  Summary:
//    Converts a NULL string to an empty string.
//
//  Parameters:
//    pszValue:
//      The string to make NULL safe.
//
//  Returns:
//    If pszValue is non-NULL, the current value of pszValue. If pszValue is
//    NULL, an empty string (g_szEMPTY_STRING).
//
//  Description:
//    This macro converts a NULL string to a NULL string (i.e. ""). It
//    is useful mainly FOR C-style string comparison.
//
//    It helps the developer to prevent crashes caused by NULL strings.
//
//  <CODE>
//  int CompareStrings(const char* pszString1,
//                     const char* pszString2)
//  {
//      // pszString1 and pszString2 come from an unknown source.
//
//      // Make sure that it will not crash the system. Consider "" equivalent
//      // to NULL.
//      return strcmp(MX_MAKE_NULL_EMPTY_STRING(pszString1),
//					  MX_MAKE_NULL_EMPTY_STRING(pszString2));
//  }
//  </CODE>
//
//==EDOC========================================================================
#define MX_MAKE_NULL_EMPTY_STRING(pszValue) (pszValue != NULL ? pszValue : g_szEMPTY_STRING)

//@@MX_ALIGNMENT_OF
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_ALIGNMENT_OF
//==
//==============================================================================
//
// Summary:
//  Returns a value, of type size_t, that is the alignment requirement of
//  the given type.
//
// Description:
//  Returns a value, of type size_t, that is the alignment requirement of
//  the given type.
//
// Note:
//  Some older compilers, namely MSVC 6.0, do not support getting
//  the alignment from a variable instance, they only support
//  getting the alignment from a type.
//
//==EDOC========================================================================
#if defined(MXD_COMPILER_MS_VC)
    #define MX_ALIGNMENT_OF(x) ((unsigned int)__alignof(x))
#elif defined (MXD_COMPILER_GNU_GCC)
    #define MX_ALIGNMENT_OF(x) ((unsigned int)__alignof__(x))
#endif

//@@MX_VOIDPTR_TO_OPQ
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_VOIDPTR_TO_OPQ
//==
//==============================================================================
//
// Summary:
//  Converts a void pointer to an opaque (mxt_opaque).
//
// Parameters:
//  pParam: Pointer to convert.
//
// Returns:
//  The converted value to opaque.
//
// Description:
//  Converts a void pointer to an opaque (mxt_opaque). The usage of this macro
//  is mandatory as shown in the following example:
//
//  <CODE>
//      CObject* pObject = MX_NEW(CObject);
//      mxt_opaque opq = MX_VOIDPTR_TO_OPQ(pObject);
//      CObject* pObject2 = reinterpret_cast<CObject*>(MX_OPQ_TO_VOIDPTR(opq));
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_OPQ_TO_VOIDPTR
//
//==EDOC========================================================================
#define MX_VOIDPTR_TO_OPQ(pParam)      reinterpret_cast<mxt_opaque>(pParam)

//@@MX_OPQ_TO_VOIDPTR
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_OPQ_TO_VOIDPTR
//==
//==============================================================================
//
// Summary:
//  Converts an opaque (mxt_opaque) to a void pointer.
//
// Parameters:
//  opq: opaque to convert.
//
// Returns:
//  The converted pointer value.
//
// Description:
//  Converts an opaque (mxt_opaque) to a void pointer. The usage of this macro
//  is mandatory as shown in the following example:
//
//  <CODE>
//      CObject* pObject = MX_NEW(CObject);
//      mxt_opaque opq = MX_VOIDPTR_TO_OPQ(pObject);
//      CObject* pObject2 = reinterpret_cast<CObject*>(MX_OPQ_TO_VOIDPTR(opq));
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_VOIDPTR_TO_OPQ
//
//==EDOC========================================================================
#define MX_OPQ_TO_VOIDPTR(opq)         reinterpret_cast<void*>(opq)

//@@MX_INT32_TO_OPQ
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_INT32_TO_OPQ
//==
//==============================================================================
//
// Summary:
//  Converts a 32 bits signed integer to an opaque (mxt_opaque).
//
// Parameters:
//  nParam: 32 bits signed integer to convert.
//
// Returns:
//  The converted value to opaque.
//
// Description:
//  Converts a 32 bits signed integer to an opaque (mxt_opaque). 
//
//  The mxt_opaque is defined as a void pointer. This results in a 64 bits type
//  on a 64 bits platform and in a 32 bits type on a 32 bits platform. To ease
//  the Framework portability on any 32 and 64 bits platforms and for better
//  performance, only 32 bits integer may be converted to opaque. This is also
//  true in a 64 bits platform.
//
//  The usage of this macro is mandatory as shown in the following example:
//
//  <CODE>
//      int nNumber = 0;
//      mxt_opaque opq = MX_INT32_TO_OPQ(nNumber);
//      int nNumber2 = MX_OPQ_TO_INT32(opq);
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_OPQ_TO_INT32
//
//==EDOC========================================================================
#define MX_INT32_TO_OPQ(nParam)     reinterpret_cast<mxt_opaque>(nParam)

//@@MX_OPQ_TO_INT32
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_OPQ_TO_INT32
//==
//==============================================================================
//
// Summary:
//  Converts an opaque (mxt_opaque) to a 32 bits signed integer.
//
// Parameters:
//  opq: opaque to convert.
//
// Returns:
//  The converted pointer value.
//
// Description:
//  Converts an opaque (mxt_opaque) to a 32 bits signed integer.
//
//  The usage of this macro is mandatory as shown in the following example:
//
//  <CODE>
//      int nNumber = 0;
//      mxt_opaque opq = MX_INT32_TO_OPQ(nNumber);
//      int nNumber2 = MX_OPQ_TO_INT32(opq);
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_INT32_TO_OPQ
//
//==EDOC========================================================================
#define MX_OPQ_TO_INT32(opq)    static_cast<int32_t>(reinterpret_cast<size_t>(opq))

//@@MX_UINT32_TO_OPQ
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_UINT32_TO_OPQ
//==
//==============================================================================
//
// Summary:
//  Converts a 32 bits unsigned integer to an opaque (mxt_opaque).
//
// Parameters:
//  uParam: 32 bits unsigned integer to convert.
//
// Returns:
//  The converted value to opaque.
//
// Description:
//  Converts a 32 bits unsigned integer to an opaque (mxt_opaque). 
//
//  The mxt_opaque is defined as a void pointer. This results in a 64 bits type
//  on a 64 bits platform and in a 32 bits type on a 32 bits platform. To ease
//  the Framework portability on any 32 and 64 bits platforms and for better
//  performance, only 32 bits integer may be converted to opaque. This is also
//  true in a 64 bits platform.
//
//  The usage of this macro is mandatory as shown in the following example:
//
//  <CODE>
//      unsigned int uNumber = 0;
//      mxt_opaque opq = MX_UINT32_TO_OPQ(uNumber);
//      unsigned int uNumber2 = MX_OPQ_TO_UINT32(opq);
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_OPQ_TO_UINT32
//
//==EDOC========================================================================
#define MX_UINT32_TO_OPQ(uParam)    reinterpret_cast<mxt_opaque>(uParam)

//@@MX_OPQ_TO_UINT32
//<GROUP BASIC_MACROS>
//==SDOC========================================================================
//==
//==  MX_OPQ_TO_UINT32
//==
//==============================================================================
//
// Summary:
//  Converts an opaque (mxt_opaque) to a 32 bits unsigned integer.
//
// Parameters:
//  opq: opaque to convert.
//
// Returns:
//  The converted pointer value.
//
// Description:
//  Converts an opaque (mxt_opaque) to a 32 bits unsigned integer.
//
//  The usage of this macro is mandatory as shown in the following example:
//
//  <CODE>
//      unsigned int uNumber = 0;
//      mxt_opaque opq = MX_UINT32_TO_OPQ(uNumber);
//      unsigned int uNumber2 = MX_OPQ_TO_UINT32(opq);
//  </CODE>
//
// See Also:
//  mxt_opaque, MX_UINT32_TO_OPQ
//
//==EDOC========================================================================
#define MX_OPQ_TO_UINT32(opq)   static_cast<uint32_t>(reinterpret_cast<size_t>(opq))

//@@MX_ISDIGIT
//<GROUP BASIC_MACROS>
//==============================================================================
//==
//==  MX_ISDIGIT
//==
//==============================================================================
//
// Summary:
//  Verifies if a character is a decimal digit.
//
// Parameters:
//  cCharacter: character to verify.
//
// Returns:
//  - 0: cCharacter is not a decimal digit.
//  - different than 0: cCharacter is either '0', '1', '2', '3', '4', '5', '6',
//                      '7', '8' or '9'.
//
// Description:
//  Verifies if a character is a decimal digit.
//
//  Decimal digits are '0', '1', '2', '3', '4', '5', '6', '7', '8' and '9'.
//
// See Also:
//  MX_ISALPHA, MX_ISALNUM
//
//==============================================================================
#define MX_ISDIGIT(cCharacter) isdigit(static_cast<unsigned char>(cCharacter))

//@@MX_ISALPHA
//<GROUP BASIC_MACROS>
//==============================================================================
//==
//==  MX_ISALPHA
//==
//==============================================================================
//
// Summary:
//  Verifies if a character is alphabetic.
//
// Parameters:
//  cCharacter: character to verify.
//
// Returns:
//  - 0: cCharacter is not alphabetic.
//  - different than 0: cCharacter is a lowercase or uppercase alphabetic
//                      character.
//
// Description:
//  Verifies if a character is a lowercase or uppercase alphabetic character.
//
// See Also:
//  MX_ISDIGIT, MX_ISALNUM
//
//==============================================================================
#define MX_ISALPHA(cCharacter) isalpha(static_cast<unsigned char>(cCharacter))

//@@MX_ISALNUM
//<GROUP BASIC_MACROS>
//==============================================================================
//==
//==  MX_ISALNUM
//==
//==============================================================================
//
// Summary:
//  Verifies if a character is alphabetic or numeric.
//
// Parameters:
//  cCharacter: character to verify.
//
// Returns:
//  - 0: cCharacter is neither alphabetic nor numeric.
//  - different than 0: cCharacter is a lowercase or uppercase alphabetic
//                      character or a number.
//
// Description:
//  Verifies if a character is a lowercase or uppercase alphabetic character or
//  a number.
//
// See Also:
//  MX_ISDIGIT, MX_ISALPHA
//
//==============================================================================
#define MX_ISALNUM(cCharacter) isalnum(static_cast<unsigned char>(cCharacter))


#endif // #ifndef MXG_MXMACRO_H
