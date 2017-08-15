//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 1998-2002 Mediatrix Telecom, Inc. ("Mediatrix")
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
#ifndef MXG_CSEMAPHORE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSEMAPHORE_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
//--------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- Framework Configuration
//---------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h"
#endif

#if !defined(MXD_OS_NONE)

// Data Member

// Interface Realized & Parent

#if defined(MXD_OS_VXWORKS)
    #include <sysLib.h>
    #include <semLib.h>
    #include <taskLib.h>
#elif defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)
    #include <pthread.h>
    #include <semaphore.h>
    #include <unistd.h>
    #include <errno.h>

    #if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #include <sys/time.h>

        #if defined(MXD_OS_DARWIN)
            #include <mach/semaphore.h>
        #endif
#elif  defined(MXD_OS_NUCLEUS)
    #include "time.h"
#endif
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    #include <winbase.h>
#elif defined(MXD_OS_SYMBIAN)
    #include <e32cmn.h>
    #include <e32std.h>
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #include <bosSem.h>
    #include <bosTask.h>
    #include <bosSleep.h>
    #include <CBosSingleton.h>
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif

// Forwards

MX_NAMESPACE_START(MXD_GNS)

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//M5T_INTERNAL_USE_BEGIN
// The numeric representation to wait on the semaphore for an infinite time
//--------------------------------------------------------------------------
const uint64_t uSEM_WAIT_INFINITE = MX_MAKEUINT64(0xFFFFFFFF, 0xFFFFFFFF);

// The maximum count the semaphore can have
//------------------------------------------
const uint32_t uSEM_MAX_COUNT       = 0x7FFFFFFF;

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: CSemaphore
//==============================================================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//  Class contains the common implementation for the CBinarySemaphore and
//  CCountingSemaphore.
//
// Description:
//  The Semaphore object is a synchronization object.
//
//  This semaphore class contains the common implementation for the
//  CBinarySemaphore and CCountingSemaphore. This class can not be used directly,
//  use the binary or counting version instead.
//
// Location:
//   Kernel/CSemaphore.h
//
// See Also:
//   CMutex, CBinarySemaphore, CCountingSemaphore
//
//==EDOC========================================================================
// BRCM_CUSTOM - [mod] - Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
class CSemaphore : public CBosSingleton
#else
class CSemaphore
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.
{
    friend class CBinarySemaphore;
    friend class CCountingSemaphore;

// Published Interface
//---------------------
public:

// Hidden Methods
//----------------
protected:

private:
    // Special case to ensure that only use of specialised objects are allowed.
    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CSemaphore(IN uint32_t uStartCount,
               IN uint32_t uMaxCount,
               IN bool bAllowTimedWait = true);

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CSemaphore();

    // << Synchronization >>
    //-----------------------

    // Summary:
    //  Acquires the semaphore.
    //--------------------------
    bool Wait() const;

    // Summary:
    //  Tries to acquire the semaphore with a timeout.
    //-------------------------------------------------
    bool Wait(IN uint64_t uWaitTimeMs) const;

    // Summary:
    //  Releases the semaphore.
    //--------------------------
    void Signal(IN bool bReschedule = false) const ;


#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE)
    //  Summary:
    //      Gets the handle of the semaphore.
    //------------------------------------
    HANDLE GetHandle();
#endif

    // Summary:
    //  Copy constructor.
    //--------------------------
    CSemaphore(const CSemaphore& from);

    // Summary:
    //  Assignment operator.
    //--------------------------
    CSemaphore& operator=(const CSemaphore& from);

// Hidden Data Members
//---------------------
protected:
private:
#if defined(MXD_OS_VXWORKS)
    mutable SEM_ID       m_hSemaphore;

#elif defined(MXD_OS_LINUX)   || \
      defined(MXD_OS_ANDROID) || \
      defined(MXD_OS_SOLARIS) || \
      defined(MXD_OS_DARWIN)  || \
      defined(MXD_OS_NUCLEUS) || \
      defined(MXD_OS_SYMBIAN)
    mutable uint32_t     m_uCount;
    mutable uint32_t     m_uMaxCount;

    #if defined(MXD_OS_SYMBIAN)
        mutable RCriticalSection m_Mutex;
        mutable RSemaphore       m_hSemaphore;

    #else
        mutable pthread_mutex_t m_Mutex;
        mutable pthread_cond_t  m_Condition;
        #if defined(MXD_OS_DARWIN)
            mutable semaphore_t m_Sem;
        #else
            mutable sem_t m_Sem;
        #endif
    #endif

#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    mutable HANDLE  m_hSemaphore;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    mutable BOS_SEM m_hSemaphore;
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif

    bool m_bAllowTimedWait;
};

//M5T_INTERNAL_USE_END

//==SDOC========================================================================
//== Class: CBinarySemaphore
//==============================================================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//  Synchronization object with two states.
//
// Description:
//  The CBinarySemaphore object is a synchronization object with two states:
//  Acquired and Free.  Once a binary semaphore is acquired, any following call
//  to the Wait method will block until the semaphore is released using the
//  Signal method, the given timeout has expired (for waitable semaphores only)
//  or the semaphore is destroyed.  CBinarySemaphores are not re-entrant.
//
//  A CBinarySemaphore can be created with or without the timed wait option.
//  Semaphores that do not have the timed wait option are at least as fast as
//  the ones with the option set but may be faster depending on the
//  implementation.  Therefore, it is recommended to set the timed wait option to
//  false when timed wait is not required.
//
//  By default, the CBinarySemaphore is created acquired, with the timed wait
//  option set.
//
// Location:
//   Kernel/CSemaphore.h
//
// See Also:
//   CMutex, CCountingSemaphore
//
//==EDOC========================================================================
class CBinarySemaphore
{
// Published enum
//----------------
public:
    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //  Defines the possible initial states of the binary semaphore.
    //--------------------------------------------------------------
    enum EInitValue
    {
        // Description:
        // Semaphore is aquired.
        eACQUIRED   = 0,
        // Description:
        // Semaphore is free.
        eFREE       = 1
    };

// Published Interface
//---------------------
public:
    // Summary:
    //  Constructor.
    //---------------
    CBinarySemaphore(IN EInitValue eVal = eACQUIRED,
                     IN bool bAllowTimedWait = true);

    // Summary:
    //  Destructor.
    //--------------
    ~CBinarySemaphore();

    // Summary:
    //  Acquires the semaphore.
    //--------------------------
    bool Wait() const;

    // Summary:
    //  Waits on a semaphore.
    //------------------------
    bool Wait(IN uint64_t uWaitTimeMs) const;

    // Summary:
    //  Signals a semaphore.
    //-----------------------
    void Signal(IN bool bReschedule = false) const;

#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE)
    // Summary:
    //  Gets the handle of the semaphore.
    //------------------------------------
    HANDLE GetHandle();
#endif

//M5T_INTERNAL_USE_BEGIN
protected:
private:
    CBinarySemaphore(const CBinarySemaphore& from);
    CBinarySemaphore& operator=(const CBinarySemaphore& from);

// Hidden Data Members
//--------------------
protected:
    CSemaphore m_sem;
//M5T_INTERNAL_USE_END
};

//==SDOC========================================================================
//== Class: CCountingSemaphore
//========================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//  Synchronization object with a lock count.
//
// Description:
//  The Semaphore object is a synchronization object. The Semaphore object holds
//  a lock count, which represents the number of times that any threads can Wait
//  to acquire the Semaphore. Once the count of the semaphore has reached zero,
//  any thread that waits to acquire it will have to wait until another thread
//  releases the semaphore with the Signal method (or until the time-out value
//  expires or until the semaphore is deleted).
//
//  A CCountingSemaphore can be created with or without the timed wait option.
//  Semaphores that do not have the timed wait option are at least as fast as
//  the ones with the option set but may be faster depending on the
//  implementation.  Therefore, it is recommended to set the timed wait option to
//  false when timed wait is not required.
//
//  By default, the CCountingSemaphore is created all acquired, with the timed
//  wait option set.
//
// Location:
//   Kernel/CSemaphore.h
//
// See Also:
//   CMutex, CBinarySemaphore
//
//==EDOC========================================================================
class CCountingSemaphore
{
// Published enum
//----------------
public:
    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //  Defines the possible initial states of the binary semaphore.
    //--------------------------------------------------------------
    enum EInitValue
    {
        // Description:
        // Counting semaphore is set to all aquired.
        eALL_ACQUIRED    = 0,
        // Description:
        // Counting semaphore is set to all free.
        eALL_FREE        = uSEM_MAX_COUNT
    };

// Published Interface
//---------------------
public:
    // Summary:
    //  Constructor.
    //---------------
    CCountingSemaphore(IN uint32_t uStartCount = static_cast<uint32_t>(eALL_ACQUIRED),
                       IN bool bAllowTimedWait = true);

    // Summary:
    //  Destructor.
    //--------------
    ~CCountingSemaphore();

    // Summary:
    //  Acquires the semaphore.
    //--------------------------
    bool Wait() const;

    // Summary:
    //  Waits on a semaphore.
    //------------------------
    bool Wait(IN uint64_t uWaitTimeMs) const;

    // Summary:
    //  Signals a semaphore.
    //-----------------------
    void Signal(IN bool bReschedule = false) const;

#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE )
    // Summary:
    //  Gets the handle of the semaphore.
    //------------------------------------
    HANDLE GetHandle();
#endif

//M5T_INTERNAL_USE_BEGIN
protected:
private:
    CCountingSemaphore(const CCountingSemaphore& from);
    CCountingSemaphore& operator=(const CCountingSemaphore& from);

// Hidden Data Members
//---------------------
protected:
    CSemaphore m_sem;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE)
//==SDOC========================================================================
//==
//==  GetHandle
//==
//==============================================================================
//
//  Returns:
//      The handle to this object.
//
//  Description:
//      Gets the Windows handle of the semaphore.
//
//  Note:
//      This method is for Win32 and WinCE implementations only.
//
//==EDOC========================================================================
inline
HANDLE CSemaphore::GetHandle()
{
    return m_hSemaphore;
}
#endif

//==SDOC========================================================================
//==
//==  CBinarySemaphore
//==
//==============================================================================
//
//  Parameters:
//    eVal:
//      Initial value of the semaphore.  Can be one of eACQUIRED (default)
//      or eFREE.
//
//    bAllowTimedWait:
//      Sets the timed wait option, making it possible or not to wait on a
//      semaphore up to the specified timeout.  Otherwise, only infinite
//      wait or no wait are permitted.
//
//  Description:
//      Constructs a CBinarySemaphore synchronization object.
//
//==EDOC========================================================================
inline
CBinarySemaphore::CBinarySemaphore(IN EInitValue eVal,
                                   IN bool bAllowTimedWait)
  : m_sem(static_cast<uint32_t>(eVal), 1, bAllowTimedWait)
{
}

//==SDOC========================================================================
//==
//==  ~CBinarySemaphore
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CBinarySemaphore::~CBinarySemaphore()
{
}

//==SDOC========================================================================
//==
//==  Wait
//==
//==============================================================================
//
//  Returns:
//      True if the semaphore has been acquired.
//      False if there was an error.
//
//  Description:
//      Tries to acquire the semaphore with regards to the current count.
//
//==EDOC========================================================================
inline
bool CBinarySemaphore::Wait() const
{
    return m_sem.Wait();
}

//==SDOC========================================================================
//==
//==  Wait
//==
//==============================================================================
//
//  Parameters:
//    uWaitTimeMs:
//      When the timed wait option is set, the maximum time in ms to wait before
//      failing to acquire the semaphore.  Another possible value, also valid
//      when the timed wait option is not set, is 0 (no wait or "try wait").
//
//  Returns:
//      True if the semaphore has been acquired.
//      False if there was an error, if the timeout expired (timed wait
//      option set) or if a timeout was given to Wait on a non time waitable
//      semaphore.
//
//  Description:
//      Tries to acquire the semaphore with regards to the uWaitTimeMs
//      parameter, the timed wait option and the current count.
//
//==EDOC========================================================================
inline
bool CBinarySemaphore::Wait(IN uint64_t uWaitTimeMs) const
{
    return m_sem.Wait(uWaitTimeMs);
}

//==SDOC========================================================================
//==
//==  Signal
//==
//==============================================================================
//
//  Parameters:
//    bReschedule:
//      Once the semaphore is signalled, ask for rescheduling.  There is
//      no guaranty that the current thread will release control.
//
//  Returns:
//      Nothing.
//
//  Description:
//      Signals a semaphore, making available to another (possibly
//      pending) thread.
//
//==EDOC========================================================================
inline
void CBinarySemaphore::Signal(IN bool bReschedule) const
{
    m_sem.Signal(bReschedule);
}

#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE)
//==SDOC========================================================================
//==
//==  GetHandle
//==
//==============================================================================
//
//  Returns:
//      The handle to this object.
//
//  Description:
//      Gets the Windows handle of the semaphore.
//
//  Note:
//      This method is for Win32 and WinCE implementations only.
//
//==EDOC========================================================================
inline
HANDLE CBinarySemaphore::GetHandle()
{
    return m_sem.GetHandle();
}
#endif

//==SDOC========================================================================
//==
//==  CCountingSemaphore
//==
//==============================================================================
//
//  Parameters:
//    uStartCount:
//      Initial value of the semaphore count.  Can be any value between
//      default value 0 (eALL_ACQUIRED) and uSEM_MAX_COUNT (eALL_FREE).
//
//    bAllowTimedWait:
//      Sets the timed wait option, making it possible or not to wait on a
//      semaphore up to the specified timeout.  Otherwise, only infinite
//      wait or no wait are permitted.
//
//  Description:
//      Constructs a CCountingSemaphore synchronization object.
//
//==EDOC========================================================================
inline
CCountingSemaphore::CCountingSemaphore(IN uint32_t uStartCount,
                                       IN bool bAllowTimedWait)
 : m_sem(uStartCount, uSEM_MAX_COUNT, bAllowTimedWait)
{
}

//==SDOC========================================================================
//==
//==  ~CCountingSemaphore
//==
//==============================================================================
//
//  Description:
//      Destructor.
//
//==EDOC========================================================================
inline
CCountingSemaphore::~CCountingSemaphore()
{
}

//==SDOC========================================================================
//==
//==  Wait
//==
//==============================================================================
//
//  Returns:
//      True if the semaphore has been acquired.
//      False if there was an error.
//
//  Description:
//      Tries to acquire the semaphore with regards to the current count.
//
//==EDOC========================================================================
inline
bool CCountingSemaphore::Wait() const
{
    return m_sem.Wait();
}

//==SDOC========================================================================
//==
//==  Wait
//==
//==============================================================================
//
//  Parameters:
//    uWaitTimeMs:
//      When the timed wait option is set, the maximum time in ms to wait before
//      failing to acquire the semaphore.  Another possible value, also valid
//      when the timed wait option is not set, is 0 (no wait or "try wait").
//
//  Returns:
//      True if the semaphore has been acquired.
//      False if there was an error, if the timeout expired (timed wait
//      option set) or if a timeout was given to Wait on a non time waitable
//      semaphore.
//
//  Description:
//      Tries to acquire the semaphore with regards to the current count, the
//      uWaitTimeMs parameter, the timed wait option and the current count.
//
//==EDOC========================================================================
inline
bool CCountingSemaphore::Wait(IN uint64_t uWaitTimeMs) const
{
    return m_sem.Wait(uWaitTimeMs);
}

//==SDOC========================================================================
//==
//==  Signal
//==
//==============================================================================
//
//  Parameters:
//    bReschedule:
//      Once the semaphore is signalled, ask for rescheduling.  There is
//      no guaranty that the current thread will release control.
//
//  Returns:
//      Nothing.
//
//  Description:
//      Signals a semaphore, incrementing the current count.
//
//==EDOC========================================================================
inline
void CCountingSemaphore::Signal(IN bool bReschedule) const
{
    m_sem.Signal(bReschedule);
}

#if defined (MXD_OS_WINDOWS) || defined (MXD_OS_WINDOWS_CE )
//==SDOC========================================================================
//==
//==  GetHandle
//==
//==============================================================================
//
//  Returns:
//      The handle to this object.
//
//  Description:
//      Gets the Windows handle of the semaphore.
//
//  Note:
//      This method is for Win32 and WinCE implementations only.
//
//==EDOC========================================================================
inline
HANDLE CCountingSemaphore::GetHandle()
{
    return m_sem.GetHandle();
}
#endif

MX_NAMESPACE_END(MXD_GNS)

#endif // #if !defined(MXD_OS_NONE)

#endif // MXG_CSEMAPHORE_H

