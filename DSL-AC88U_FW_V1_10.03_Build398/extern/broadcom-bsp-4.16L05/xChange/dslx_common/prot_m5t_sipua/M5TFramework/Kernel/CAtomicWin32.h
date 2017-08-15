//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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

//M5T_INTERNAL_USE_BEGIN

#ifndef MXG_CATOMICWIN32_H
#define MXG_CATOMICWIN32_H


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXD_ATOMIC_NATIVE_OPERATIONS_IMPL
    #error This file MUST only be included by CAtomic.h.
#endif

// This file should only be used in windows using the visual C++ compiler.
#if !defined(MXD_OS_WINDOWS) || !defined(MXD_COMPILER_MS_VC) || !defined(MXD_ARCH_IX86)

#error This file should only be included for the windows operating system \
           with the Visual C++ compiler.

#else

MX_NAMESPACE_START(MXD_GNS)


MX_NAMESPACE_START(Win32NativeAtomicOperations)

// Note: For performance, all of these functions are inline, naked and use the
// __fastcall calling convention. With the __fastcall calling convention
// the first 2 dword size parameter are passed in ecx and edx respectively.
// Because of that, we have to put the MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING
// macro after the __asm block, so as not to interfere with the registers used
// by __fastcall.
//-----------------------------------------------------------------------------

inline __declspec(naked) int16_t __fastcall AtomicIncrement(INOUT int16_t* pnValue)
{
    __asm
    {
        mov ax, 1
        lock xadd word ptr[ecx], ax
        inc ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint16_t __fastcall AtomicIncrement(INOUT uint16_t* puValue)
{
    __asm
    {
        mov ax, 1
        lock xadd word ptr[ecx], ax
        inc ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}

inline __declspec(naked) int32_t __fastcall AtomicIncrement(INOUT int32_t* pnValue)
{
    __asm
    {
        mov eax, 1
        lock xadd dword ptr[ecx], eax
        inc eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint32_t __fastcall AtomicIncrement(INOUT uint32_t* puValue)
{
    __asm
    {
        mov eax, 1
        lock xadd dword ptr[ecx], eax
        inc eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}

inline __declspec(naked) int16_t __fastcall AtomicDecrement(INOUT int16_t* pnValue)
{
    __asm
    {
        mov ax, 0FFFFh
        lock xadd word ptr[ecx], ax
        dec ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint16_t __fastcall AtomicDecrement(INOUT uint16_t* puValue)
{
    __asm
    {
        mov ax, 0FFFFh
        lock xadd word ptr[ecx], ax
        dec ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}

inline __declspec(naked) int32_t __fastcall AtomicDecrement(INOUT int32_t* pnValue)
{
    __asm
    {
        mov eax, 0FFFFFFFFh
        lock xadd dword ptr[ecx], eax
        dec eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint32_t __fastcall AtomicDecrement(INOUT uint32_t* puValue)
{
    __asm
    {
        mov eax, 0FFFFFFFFh
        lock xadd dword ptr[ecx], eax
        dec eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}


inline __declspec(naked) int16_t __fastcall AtomicExchange(IN int16_t nSource, INOUT int16_t* pnDest)
{
    __asm
    {
        lock xchg word ptr[edx], cx
        mov ax, cx
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(nSource);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnDest);
}

inline __declspec(naked) uint16_t __fastcall AtomicExchange(IN uint16_t uSource, INOUT uint16_t* puDest)
{
    __asm
    {
        lock xchg word ptr[edx], cx
        mov ax, cx
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uSource);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puDest);
}

inline __declspec(naked) int32_t __fastcall AtomicExchange(IN int32_t nSource, INOUT int32_t* pnDest)
{
    __asm
    {
        lock xchg dword ptr[edx], ecx
        mov eax, ecx
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(nSource);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnDest);
}

inline __declspec(naked) uint32_t __fastcall AtomicExchange(IN uint32_t uSource, INOUT uint32_t* puDest)
{
    __asm
    {
        lock xchg dword ptr[edx], ecx
        mov eax, ecx
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(uSource);
    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puDest);
}

inline __declspec(naked) int16_t __fastcall AtomicRetrieve(INOUT int16_t const* pnValue)
{
    __asm
    {
        mov ax, 0
        lock xadd word ptr[ecx], ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint16_t __fastcall AtomicRetrieve(INOUT uint16_t const* puValue)
{
    __asm
    {
        mov ax, 0
        lock xadd word ptr[ecx], ax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}

inline __declspec(naked) int32_t __fastcall AtomicRetrieve(INOUT int32_t const* pnValue)
{
    __asm
    {
        mov eax, 0
        lock xadd dword ptr[ecx], eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pnValue);
}

inline __declspec(naked) uint32_t __fastcall AtomicRetrieve(INOUT uint32_t const* puValue)
{
    __asm
    {
        mov eax, 0
        lock xadd dword ptr[ecx], eax
        ret
    }

    MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(puValue);
}

MX_NAMESPACE_END(Win32NativeAtomicOperations)

// Summary:
//  Specialized version of CAtomicOperations to allow it to work with the native
//  functions on any type specified in the Macro parameter.
//------------------------------------------------------------------------------
#define MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(_Type)                                        \
    template<>                                                                              \
    class CAtomicOperations<_Type>                                                          \
    {                                                                                       \
    public:                                                                                 \
        CAtomicOperations()                                                                 \
        {                                                                                   \
        }                                                                                   \
        CAtomicOperations(IN CMutex* pmutexShared)                                          \
        {                                                                                   \
            MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pmutexShared);                          \
        }                                                                                   \
        _Type Increment(INOUT _Type* pValue) const                                          \
        {                                                                                   \
            return MX_NAMESPACE(Win32NativeAtomicOperations)AtomicIncrement(pValue);        \
        }                                                                                   \
        _Type Decrement(INOUT _Type* pValue) const                                          \
        {                                                                                   \
            return MX_NAMESPACE(Win32NativeAtomicOperations)AtomicDecrement(pValue);        \
        }                                                                                   \
        _Type Exchange(IN _Type source, INOUT _Type* pDest) const                           \
        {                                                                                   \
            return MX_NAMESPACE(Win32NativeAtomicOperations)AtomicExchange(source, pDest);  \
        }                                                                                   \
        _Type Retrieve(INOUT _Type const* pValue) const                                     \
        {                                                                                   \
            return MX_NAMESPACE(Win32NativeAtomicOperations)AtomicRetrieve(pValue);         \
        }                                                                                   \
    }

MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(int16_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(uint16_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(int32_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(uint32_t);

MX_NAMESPACE_END(MXD_GNS)

#endif //#if !defined(MXD_OS_WINDOWS) || !defined(MXD_COMPILER_MS_VC) || !defined(MXD_ARCH_IX86)

#endif //#ifndef MXG_CATOMICWIN32_H

//M5T_INTERNAL_USE_END

