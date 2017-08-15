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

#ifndef MXG_CATOMICLINUXPPC_H
#define MXG_CATOMICLINUXPPC_H


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXD_ATOMIC_NATIVE_OPERATIONS_IMPL
    #error This file MUST only be included by CAtomic.h.
#endif

// This file should only be used in Linux PPC using the gcc compiler.
#if !defined(MXD_OS_LINUX) || !defined(MXD_COMPILER_GNU_GCC) || !defined(MXD_ARCH_PPC)

#error This file should only be included for the Linux PPC operating system \
       with the GNU GCC compiler.

#else


MX_NAMESPACE_START(MXD_GNS)

MX_NAMESPACE_START(LinuxPPCNativeAtomicOperations)

// Note: Each of the functions below use the label "1" for branching because
// textual labels cause multiple definition errors. This seems to happen even if
// each function uses its own unique name for the label (probably because the
// functions are inline). Using "1" as the label seems to solve this problem.

inline int32_t AtomicIncrement(INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      addic   %0, %0, 1\n\t" //Increment register(%0) by 1.

                 "      stwcx.  %0, 0, %1\n\t" //Store register(%0) into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (nResult)  // Outputs
                 : "r" (pnValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return nResult;
}

inline uint32_t AtomicIncrement(INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      addic   %0, %0, 1\n\t" //Increment register(%0) by 1.

                 "      stwcx.  %0, 0, %1\n\t" //Store register(%0) into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (uResult)  // Outputs
                 : "r" (puValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return uResult;
}

inline int32_t AtomicDecrement(INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      addic   %0, %0, -1\n\t"//Decrement register(%0) by 1.

                 "      stwcx.  %0, 0, %1\n\t" //Store register(%0) into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (nResult)  // Outputs
                 : "r" (pnValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return nResult;
}

inline uint32_t AtomicDecrement(INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      addic   %0, %0, -1\n\t"//Decrement register(%0) by 1.

                 "      stwcx.  %0, 0, %1\n\t" //Store register(%0) into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.
                 : "=&r" (uResult)  // Outputs
                 : "r" (puValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return uResult;
}

inline int32_t AtomicExchange(IN int32_t nValue, INOUT int32_t* pnValue)
{
    int32_t nResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      stwcx.  %2, 0, %1\n\t" //Store nValue into *pnValue and
                                               //clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (nResult)             // Outputs
                 : "r" (pnValue), "r" (nValue) // Inputs
                 : "cc", "memory");            // Clobbered list

    return nResult;
}

inline uint32_t AtomicExchange(IN uint32_t uValue, INOUT uint32_t* puValue)
{
    uint32_t uResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      stwcx.  %2, 0, %1\n\t" //Store nValue into *pnValue and
                                               //clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (uResult)             // Outputs
                 : "r" (puValue), "r" (uValue) // Inputs
                 : "cc", "memory");            // Clobbered list

    return uResult;
}

inline int32_t AtomicRetrieve(INOUT int32_t const* pnValue)
{
    int32_t nResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      stwcx.  %0, 0, %1\n\t" //Re-store the same value into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (nResult)  // Outputs
                 : "r" (pnValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return nResult;
}

inline uint32_t AtomicRetrieve(INOUT uint32_t const* puValue)
{
    uint32_t uResult = 0;

    asm volatile("1:    lwarx   %0, 0, %1\n\t" //Load *pnValue into register(%0)
                                               //with reservation.
                 "      stwcx.  %0, 0, %1\n\t" //Re-store the same value into
                                               //*pnValue and clear reservation.
                 "      bne-    1b\n\t"        //If store failed, branch to 1.

                 : "=&r" (uResult)  // Outputs
                 : "r" (puValue)    // Inputs
                 : "cc", "memory"); // Clobbered list

    return uResult;
}

MX_NAMESPACE_END(LinuxPPCNativeAtomicOperations)


// Summary:
//  Specialized version of CAtomicOperations to allow it to work with the native
//  functions on any type specified in the Macro parameter.
//------------------------------------------------------------------------------
#define MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(_Type)                                            \
    template<>                                                                                  \
    class CAtomicOperations<_Type>                                                              \
    {                                                                                           \
    public:                                                                                     \
        CAtomicOperations()                                                                     \
        {                                                                                       \
        }                                                                                       \
        CAtomicOperations(IN CMutex* pmutexShared)                                              \
        {                                                                                       \
            MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING(pmutexShared);                              \
        }                                                                                       \
        _Type Increment(INOUT _Type* pValue) const                                              \
        {                                                                                       \
            return MX_NAMESPACE(LinuxPPCNativeAtomicOperations)AtomicIncrement(pValue);         \
        }                                                                                       \
        _Type Decrement(INOUT _Type* pValue) const                                              \
        {                                                                                       \
            return MX_NAMESPACE(LinuxPPCNativeAtomicOperations)AtomicDecrement(pValue);         \
        }                                                                                       \
        _Type Exchange(IN _Type source, INOUT _Type* pDest) const                               \
        {                                                                                       \
            return MX_NAMESPACE(LinuxPPCNativeAtomicOperations)AtomicExchange(source, pDest);   \
        }                                                                                       \
        _Type Retrieve(INOUT _Type const* pValue) const                                         \
        {                                                                                       \
            return MX_NAMESPACE(LinuxPPCNativeAtomicOperations)AtomicRetrieve(pValue);          \
        }                                                                                       \
    }

MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(int32_t);
MX_ATOMIC_OPERATIONS_SPECIALIZED_TYPE(uint32_t);

MX_NAMESPACE_END(MXD_GNS)

#endif //#if !defined(MXD_OS_LINUX) || !defined(MXD_COMPILER_GNU_GCC) || !defined(MXD_ARCH_PPC)

#endif //#ifndef MXG_CATOMICLINUXPPC_H

//M5T_INTERNAL_USE_END

