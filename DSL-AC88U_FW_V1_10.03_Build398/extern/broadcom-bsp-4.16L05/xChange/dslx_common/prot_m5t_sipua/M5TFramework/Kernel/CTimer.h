//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 1998 Mediatrix Telecom, Inc. ("Mediatrix")
//   Copyright(c) 2003 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation (“Media5”)
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserve all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserve the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CTIMER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTIMER_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES  ===========================================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if !defined(MXD_OS_NONE)

//-- Framework Configuration
//---------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_ENABLE_STUBBING
#endif

// Data Member
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif

#ifndef MXG_CSEMAPHORE_H
#include "Kernel/CSemaphore.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

// Interface Realized & Parent

MX_NAMESPACE_START(MXD_GNS)

//===========================================================================
//====  FORWARD DECLARATIONS  ===============================================
//===========================================================================

// NOTE: The Forward Declarations MUST be inside of the Namespace

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================

// Mechanism to permit the stubbing of CTimer.
//--------------------------------------------
#if defined(MXD_ENABLE_STUBBING) && \
    defined(MXD_STUB_CTIMER)
MX_NAMESPACE_END(MXD_GNS)
#ifndef MXG_CTIMERSTUB_H
    #include "KernelStubs/CTimerStub.h"
#endif
MX_NAMESPACE_START(MXD_GNS)
#else

//==SDOC=====================================================================
//==
//==  Class: CTimer
//==
//===========================================================================
//<GROUP KERNEL_CLASSES>
//
//  Summary:
//      Class implementing timers.
//
//  Description:
//      The CTimer class is the class implementing timers. There are two types
//      of timers available: cyclic timers and one shot timers.
//
//      One shot timers wait for the specified time in ms and return. Cyclic
//      timers are timers that wait for a specified time, and when the wait is
//      over the time wait starts over. Every time the application calls
//      a wait on a cyclic timer, it verifies if the time passed between calls
//      to wait. If it is less than the timer, the wait waits for the remaining
//      time. If it is greater, the timer can either set the current time in the
//      wait so that the next call will work properly or it will exit without
//      waiting until the overrun is corrected.
//
//
//==EDOC=====================================================================
class CTimer
{
// Published Interface
//--------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CTimer();

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CTimer();

    // << Waiting >>
    //---------------

    // WARNING: VxWorks and Linux timers precision cannot reach 1 msec.
    //------------------------------------------------------------------

    // Summary:
    //  Waits for the specified number of ms.
    //----------------------------------------
    static void Wait(IN uint64_t uMs);

    // << Cyclic waiting >>
    //----------------------

    // Summary:
    //  Starts the cyclic wait.
    //--------------------------
    bool StartCyclicWait(IN uint64_t uMs);

    // Summary:
    //  Waits for the reamining time of the cycle.
    //---------------------------------------------
    bool CyclicWait(IN bool bAllowCycleLost = true);

    // Summary:
    //  Stops the cyclic timer.
    //--------------------------
    void StopCyclicWait();

    // << Time retrieval >>
    //----------------------
    // GetSystemUpTimeMs() returns the number of ms since system start-up.
    // WARNING: GetSysUpTime MUST be called at least once every 49 days to work
    //          properly.
    //--------------------------------------------------------------------------

    // Summary:
    //  Gets the number of ms since system start-up.
    //----------------------------------------------
    static uint64_t GetSystemUpTimeMs();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//---------------
protected:
private:
    CTimer(const CTimer& from);
    CTimer& operator=(const CTimer& from);

    static mxt_result InitializeCTimer();
    static void FinalizeCTimer();

// Friends declarations
//----------------------
    friend mxt_result InitializeCTimer();
    friend void FinalizeCTimer();

// Hidden Data Members
//--------------------
protected:
private:

    uint64_t  m_uLastTime;
    uint64_t  m_uPeriod;
    CBinarySemaphore m_semaphore;
    static CAlignedVariableStorage<CBinarySemaphore> ms_semWait;

#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE) || !defined(MXD_USE_MEDIATRIX_SDK)
#if defined(MXD_OS_LINUX)   || \
    defined(MXD_OS_ANDROID) || \
    defined(MXD_OS_SOLARIS) || \
    defined(MXD_OS_VXWORKS) || \
    defined(MXD_OS_NUCLEUS) || \
    defined(MXD_OS_SYMBIAN)
    static uint32_t ms_uLastSysUpTimeTick;
    static uint64_t ms_uSysUpTimeTick;
#else
    static uint64_t ms_uLastSysUpTimeMs;
    static uint64_t ms_uSysUpTimeMs;
#endif
#endif

    static CAlignedVariableStorage<CMutex> ms_mutexThreadingProtection;

#if defined(MXD_OS_LINUX)   || \
    defined(MXD_OS_ANDROID) || \
    defined(MXD_OS_SOLARIS) || \
    defined(MXD_OS_VXWORKS) || \
    defined(MXD_OS_NUCLEUS) || \
    defined(MXD_OS_SYMBIAN)
    static unsigned int ms_uTickPerSec;
#endif

//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  Wait
//==
//==============================================================================
//
//  Parameters:
//      uMs:
//          Time to wait in ms.
//
//  Description:
//    Waits for the specified time in milliseconds.
//
// WARNING: The real time elapsed during the call to wait will be as precise
//          as the platform can be. As such a wait time of 1 millisecond may
//          wait longer, depending on the scheduler of the operating system.
//
//==EDOC========================================================================
inline
void CTimer::Wait(IN uint64_t uMs)
{
    ms_semWait.Instance()->Wait(uMs);
}

#endif // #if defined(MXD_ENABLE_STUBBING)
       // defined(MXD_STUB_CTIMER)

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_NONE)

#endif // MXG_CTIMER_H

