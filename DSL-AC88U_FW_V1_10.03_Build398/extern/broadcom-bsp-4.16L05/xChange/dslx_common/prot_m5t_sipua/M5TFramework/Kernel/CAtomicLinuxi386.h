//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================

//M5T_INTERNAL_USE_BEGIN

#ifndef MXG_CATOMICLINUXI386_H
#define MXG_CATOMICLINUXI386_H


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXD_ATOMIC_NATIVE_OPERATIONS_IMPL
    #error This file MUST only be included by CAtomic.h.
#endif

// This file should only be used in Linux x86 and x86-64 (amd64) using the
// gcc compiler. Note that x86 and amd64 share the same assembler instructions
// and results with the same implementation. However, this implementation does
// not support atomic operations for the 64 bits types since it is not
// required.
#if !defined(MXD_OS_LINUX) || \
    !defined(MXD_COMPILER_GNU_GCC) || \
    (!defined(MXD_ARCH_IX86) && !defined(MXD_ARCH_AMD64))

#error This file should only be included for the following OS + compiler + architecture combination: \
       Linux + GNU GCC + x86 or amd64.

#else


MX_NAMESPACE_START(MXD_GNS)

MX_NAMESPACE_START(LinuxNativeAtomicOperations)

inline int16_t AtomicIncrement(INOUT int16_t* pnValue)
{
    int16_t nResult = 0;

    asm volatile ("lock\n\t"
                  "xaddw %0, (%1)\n\t"
                  "incw %0\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(1));  // Inputs.

    return nResult;
}

inline uint16_t AtomicIncrement(INOUT uint16_t* puValue)
{
    uint16_t uResult = 0;

    asm volatile ("lock\n\t"
                  "xaddw %0, (%1)\n\t"
                  "incw %0\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(1));  // Inputs.

    return uResult;
}

inline int32_t AtomicIncrement(INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile ("lock\n\t"
                  "xaddl %0, (%1)\n\t"
                  "incl %0\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(1));  // Inputs.

    return nResult;
}

inline uint32_t AtomicIncrement(INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile ("lock\n\t"
                  "xaddl %0, (%1)\n\t"
                  "incl %0\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(1));  // Inputs.

    return uResult;
}

inline int16_t AtomicDecrement(INOUT int16_t* pnValue)
{
    int16_t nResult = 0;

    asm volatile ("lock\n\t"
                  "xaddw %0, (%1)\n\t"
                  "decw %0\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(-1)); // Inputs.


    return nResult;
}

inline uint16_t AtomicDecrement(INOUT uint16_t* puValue)
{
    uint16_t uResult = 0;

    asm volatile ("lock\n\t"
                  "xaddw %0, (%1)\n\t"
                  "decw %0\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(-1)); // Inputs.

    return uResult;
}


inline int32_t AtomicDecrement(INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile ("lock\n\t"
                  "xaddl %0, (%1)\n\t"
                  "decl %0\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(-1)); // Inputs.

    return nResult;
}

inline uint32_t AtomicDecrement(INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile ("lock\n\t"
                  "xaddl %0, (%1)\n\t"
                  "decl %0\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(-1)); // Inputs.

    return uResult;
}

inline int16_t AtomicExchange(IN int16_t nValue, INOUT int16_t* pnValue)
{
    int16_t nResult = 0;

    asm volatile ("lock xchgw %0, (%1)\n\t"
                  : "=r"(nResult)               // Output.
                  : "r"(pnValue), "0"(nValue)); // Inputs.


    return nResult;
}

inline uint16_t AtomicExchange(IN uint16_t uValue, INOUT uint16_t* puValue)
{
    uint16_t uResult = 0;

    asm volatile ("lock xchgw %0, (%1)\n\t"
                  : "=r"(uResult)               // Output.
                  : "r"(puValue), "0"(uValue)); // Inputs.

    return uResult;
}

inline int32_t AtomicExchange(IN int32_t nValue, INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile ("lock xchgl %0, (%1)\n\t"
                  : "=r"(nResult)               // Output.
                  : "r"(pnValue), "0"(nValue)); // Inputs.

    return nResult;
}

inline uint32_t AtomicExchange(IN uint32_t uValue, INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile ("lock xchgl %0, (%1)\n\t"
                  : "=r"(uResult)               // Output.
                  : "r"(puValue), "0"(uValue)); // Inputs.

    return uResult;
}

inline int16_t AtomicRetrieve(INOUT int16_t const* pnValue)
{
    int16_t nResult = 0;

    asm volatile ("lock xaddw %0, (%1)\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(0));  // Inputs.

    return nResult;
}

inline uint16_t AtomicRetrieve(INOUT uint16_t const* puValue)
{
    uint16_t uResult = 0;

    asm volatile ("lock xaddw %0, (%1)\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(0));  // Inputs.

    return uResult;
}

inline int32_t AtomicRetrieve(INOUT int32_t const* pnValue)
{
    int32_t nResult = 0;

    asm volatile ("lock xaddl %0, (%1)\n\t"
                  : "=r"(nResult)           // Output.
                  : "r"(pnValue), "0"(0));  // Inputs.

    return nResult;
}

inline uint32_t AtomicRetrieve(INOUT uint32_t const* puValue)
{
    uint32_t uResult = 0;

    asm volatile ("lock xaddl %0, (%1)\n\t"
                  : "=r"(uResult)           // Output.
                  : "r"(puValue), "0"(0));  // Inputs.

    return uResult;
}

MX_NAMESPACE_END(LinuxNativeAtomicOperations)


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
            return MX_NAMESPACE(LinuxNativeAtomicOperations)AtomicIncrement(pValue);        \
        }                                                                                   \
        _Type Decrement(INOUT _Type* pValue) const                                          \
        {                                                                                   \
            return MX_NAMESPACE(LinuxNativeAtomicOperations)AtomicDecrement(pValue);        \
        }                                                                                   \
        _Type Exchange(IN _Type source, INOUT _Type* pDest) const                           \
        {                                                                                   \
            return MX_NAMESPACE(LinuxNativeAtomicOperations)AtomicExchange(source, pDest);  \
        }                                                                                   \
        _Type Retrieve(INOUT _Type const* pValue) const                                     \
        {                                                                                   \
            return MX_NAMESPACE(LinuxNativeAtomicOperations)AtomicRetrieve(pValue);         \
        }                                                                                   \
    }

MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(int16_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(uint16_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(int32_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(uint32_t);

MX_NAMESPACE_END(MXD_GNS)

#endif //#if !defined(MXD_OS_LINUX) || ...

#endif //#ifndef MXG_CATOMICLINUXI386_H

//M5T_INTERNAL_USE_END

