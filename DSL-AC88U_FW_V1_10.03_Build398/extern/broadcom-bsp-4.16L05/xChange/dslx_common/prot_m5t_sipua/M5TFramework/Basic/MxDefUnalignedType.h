//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2002 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_MXDEFUNALIGNEDTYPE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXDEFUNALIGNEDTYPE_H
//M5T_INTERNAL_USE_END

// Verify that this header file is included by MxConfig.h only. This
// verification is performed because many macros used throughout this file are
// defined in PreMxConfig.h, which is only included by MxConfig.h. If we were
// to allow the direct inclusion of MxDefUnalignedType.h, we would loose the
// configuration done in PreMxConfig.h.
//-----------------------------------------------------------------------------
#if !defined(MXG_MXCONFIG_H)
    #error This file must be included through MxConfig.h only!
#endif


#ifndef  MXG_MXMACRO_H
#include "Basic/MxMacro.h"
#endif

//M5T_INTERNAL_USE_BEGIN

#if defined(MXD_ARCH_MIPS) || defined(MXD_ARCH_NIOS2) || defined(MXD_ARCH_ARM) || defined(MXD_ARCH_SPARC)

    #if defined(__cplusplus)
    #include "Basic/MxPack.h"

    typedef struct SUnalignedInt16
    {
        SUnalignedInt16() {}
        SUnalignedInt16(IN int16_t other) { n = other; }
        operator int16_t() const { return n; }
        int16_t operator=(IN int16_t other) { return n = other; }
        int16_t n MX_PACK_MEMBER;
    } MX_PACK_STRUCT mxt_UNALIGNED_INT16;

    typedef struct SUnalignedUInt16
    {
        SUnalignedUInt16() {}
        SUnalignedUInt16(IN uint16_t other) { un = other; }
        operator uint16_t() const { return un; }
        uint16_t operator=(IN uint16_t other) { return un = other; }
        uint16_t un MX_PACK_MEMBER;
    } MX_PACK_STRUCT mxt_UNALIGNED_UINT16;

    typedef struct SUnalignedInt32
    {
        SUnalignedInt32() {}
        SUnalignedInt32(IN int32_t other) { n = other; }
        operator int32_t() const { return n; }
        int32_t operator=(IN int32_t other) { return n = other; }
        int32_t n MX_PACK_MEMBER;
    } MX_PACK_STRUCT mxt_UNALIGNED_INT32;

    typedef struct SUnalignedUInt32
    {
        SUnalignedUInt32() {}
        SUnalignedUInt32(IN uint32_t other) { un = other; }
        operator uint32_t() const { return un; }
        uint32_t operator=(IN uint32_t other) { return un = other; }
        uint32_t un MX_PACK_MEMBER;
    } MX_PACK_STRUCT mxt_UNALIGNED_UINT32;

    typedef struct SUnalignedInt64
    {
        SUnalignedInt64() {}
        SUnalignedInt64(IN int64_t other) { n = other; }
        operator int64_t() const { return n; }
        int64_t operator=(IN int64_t other) { return n = other; }
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_95)
        int64_t volatile n MX_PACK_MEMBER;
    #else
        int64_t n MX_PACK_MEMBER;
    #endif
    } MX_PACK_STRUCT mxt_UNALIGNED_INT64;

    typedef struct SUnalignedUInt64
    {
        SUnalignedUInt64() {}
        SUnalignedUInt64(IN uint64_t other) { un = other; }
        operator uint64_t() const { return un; }
        uint64_t operator=(IN uint64_t other) { return un = other; }
    #if (MXD_COMPILER_GNU_GCC == MXD_COMPILER_GNU_GCC_2_95)
        uint64_t volatile un MX_PACK_MEMBER;
    #else
        uint64_t un MX_PACK_MEMBER;
    #endif
    } MX_PACK_STRUCT mxt_UNALIGNED_UINT64;

    #include "Basic/MxPack.h"
    #endif // #if defined(__cplusplus)

#elif defined(_TMS320C6400)

    // To implement unaligned load and store for this platform, we use
    // the compiler intrinsics operator to access assembly language
    //  statements. For more information refer to the TMS320C6000
    // optimizing compiler user's guide section 8.5.2. The Texas Instrument
    // (TI) document number is: SPRU187K (october 2002).
    // The intrinsics operator used are:
    // - ushort & _mem2(void * ptr);
    // - const ushort & _mem2_const(const void * ptr);
    // - uint & _mem4(void * ptr);
    // - const uint & _mem4_const(const void * ptr);

    #if defined(__cplusplus)

    typedef struct SUnalignedInt16
    {
        SUnalignedInt16() {}
        SUnalignedInt16(IN int16_t other) { _mem2(&n) = other; }
        operator int16_t() const { return _mem2_const(&n); }
        int16_t operator=(IN int16_t other) { return _mem2(&n) = other; }
        int16_t operator=(IN const SUnalignedInt16& other) { return _mem2(&n) = _mem2_const(&other); }
        int16_t n;
    } mxt_UNALIGNED_INT16;

    typedef struct SUnalignedUInt16
    {
        SUnalignedUInt16() {}
        SUnalignedUInt16(IN uint16_t other) { _mem2(&un) = other; }
        operator uint16_t() const { return _mem2_const(&un); }
        uint16_t operator=(IN uint16_t other) { return _mem2(&un) = other; }
        uint16_t operator=(IN const SUnalignedUInt16& other) { return _mem2(&un) = _mem2_const(&other); }
        uint16_t un;
    } mxt_UNALIGNED_UINT16;

    typedef struct SUnalignedInt32
    {
        SUnalignedInt32() {}
        SUnalignedInt32(IN int32_t other) { _mem4(&n) = other; }
        operator int32_t() const { return _mem4_const(&n); }
        int32_t operator=(IN int32_t other) { return _mem4(&n) = other; }
        int32_t operator=(IN const SUnalignedInt32& other) { return _mem4(&n) = _mem4_const(&other); }
        int32_t n;
    } mxt_UNALIGNED_INT32;

    typedef struct SUnalignedUInt32
    {
        SUnalignedUInt32() {}
        SUnalignedUInt32(IN uint32_t other) { _mem4(&un) = other; }
        operator uint32_t() const { return _mem4_const(&un); }
        uint32_t operator=(IN uint32_t other) { return _mem4(&un) = other; }
        uint32_t operator=(IN const SUnalignedUInt32& other) { return _mem4(&un) = _mem4_const(&other); }
        uint32_t un;
    } mxt_UNALIGNED_UINT32;

    #endif // #if defined(__cplusplus)
#else
    typedef int16_t  mxt_UNALIGNED_INT16;
    typedef uint16_t mxt_UNALIGNED_UINT16;
    typedef int32_t  mxt_UNALIGNED_INT32;
    typedef uint32_t mxt_UNALIGNED_UINT32;

#if !defined(MXD_64BITS_SUPPORT_DISABLE) && !defined(MXD_64BITS_CUSTOM_TYPE)
    typedef int64_t  mxt_UNALIGNED_INT64;
    typedef uint64_t mxt_UNALIGNED_UINT64;
#endif // #if !defined(MXD_64BITS_SUPPORT_DISABLE) && !defined(MXD_64BITS_CUSTOM_TYPE)
#endif // #if defined(MXD_ARCH_MIPS)

//M5T_INTERNAL_USE_END

#if 0
//==============================================================================
//====  Documentation  =========================================================
//==============================================================================

//<GROUP BASIC_TYPES>
//<TITLE Unaligned data types>
//==SDOC========================================================================
//
// Summary:
//  M5T Framework defines data types for platforms not supporting unaligned access.
//
// Description:
//
//  Special type definitions for self-alignment on misaligned data access.
//
//  To avoid misalignment problems, these particular types are created for
//  processor architectures that do not handle misalignment exceptions. For
//  example, the MIPS32 core used on some processors.
//
//  The following types are defined:
//
//  - mxt_UNALIGNED_INT16 for signed 16 bits access;
//  - mxt_UNALIGNED_UINT16 for unsigned 16 bits access;
//  - mxt_UNALIGNED_INT32 for signed 32 bits access;
//  - mxt_UNALIGNED_UINT32 for unsigned 32 bits access;
//  - mxt_UNALIGNED_INT64 for signed 64 bits access;
//  - mxt_UNALIGNED_UINT64 for unsigned 64 bits access.
//
//  The above types map to the corresponding standard types for platforms that
//  do not require aligned access.
//
// Location:
//  Basic/MxDefUnalignedType.h
//
//==EDOC========================================================================
typedef SUnalignedInt16 mxt_UNALIGNED_INT16;

//<GROUP BASIC_TYPES>
//<COMBINE mxt_UNALIGNED_INT16>
typedef SUnalignedUInt16 mxt_UNALIGNED_UINT16;

//<GROUP BASIC_TYPES>
//<COMBINE mxt_UNALIGNED_INT16>
typedef SUnalignedInt32  mxt_UNALIGNED_INT32;

//<GROUP BASIC_TYPES>
//<COMBINE mxt_UNALIGNED_INT16>
typedef SUnalignedUInt32 mxt_UNALIGNED_UINT32;

//<GROUP BASIC_TYPES>
//<COMBINE mxt_UNALIGNED_INT16>
typedef SUnalignedInt64 mxt_UNALIGNED_INT64;

//<GROUP BASIC_TYPES>
//<COMBINE mxt_UNALIGNED_INT16>
typedef SUnalignedUInt64 mxt_UNALIGNED_UINT64;

#endif  //#if 0 --> end of documentation section.

#endif // #ifndef MXG_MXDEFUNALIGNEDTYPE_H
