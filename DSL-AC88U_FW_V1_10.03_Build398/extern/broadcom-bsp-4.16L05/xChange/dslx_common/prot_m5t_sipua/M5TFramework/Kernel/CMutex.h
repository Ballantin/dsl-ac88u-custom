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
#ifndef MXG_CMUTEX_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CMUTEX_H
//M5T_INTERNAL_USE_END

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

// Data Member
#if defined(MXD_OS_VXWORKS)
    #include <semLib.h>
#elif defined(MXD_OS_LINUX) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)
    #include <pthread.h>
    //M5T_INTERNAL_USE_BEGIN
    extern "C" int pthread_mutexattr_setkind_np(pthread_mutexattr_t *attr, int kind);
    //M5T_INTERNAL_USE_END
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    #include <winbase.h>
#elif defined(MXD_OS_SYMBIAN)
    #include <e32std.h>
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #include <bosMutex.h>
    #include <CBosSingleton.h>
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif

// Interface Realized  Parent

// Forwards

MX_NAMESPACE_START(MXD_GNS)

//===========================================================================
//====  CONSTANTS + DEFINES  ================================================
//===========================================================================

//===========================================================================
//====  NEW TYPE DEFINITIONS  ===============================================
//===========================================================================


//==SDOC========================================================================
//== Class: CMutex
//========================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//   Basic synchronization object.
//
// Description:
//   The mutex is a basic synchronization object. A thread can lock
//   the mutex multiple times once it has already entered the protected
//   section. The thread must unlock the mutex the same number of times for it to
//   be released.  Mutexes are mostly used to protect part of code that
//   require mutual exclusion (also referred to as critical section).
//
// Location:
//   Kernel/CMutex.h
//
// See Also:
//   CBinarySemaphore, CCountingSemaphore
//
//==EDOC========================================================================
// BRCM_CUSTOM - [mod] - Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
class CMutex : public CBosSingleton
#else
class CMutex
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.
{
// Published Interface
//--------------------
public:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  CMutex
    //==
    //==========================================================================
    //
    //  Summary:
    //    Constructor.
    //
    //  Description:
    //    Constructor.
    //
    //==EDOC====================================================================
    CMutex();

    //==SDOC====================================================================
    //==
    //==  ~CMutex
    //==
    //==========================================================================
    //
    //  Summary:
    //    Destructor.
    //
    //  Description:
    //    Destructor.
    //
    //==EDOC====================================================================
    virtual ~CMutex();

    //==SDOC====================================================================
    //==
    //==  Lock
    //==
    //==========================================================================
    //
    //  Summary:
    //    Locks the mutex.
    //
    //  Description:
    //    Locks the mutex.  If the mutex is already locked by another thread,
    //    the call will block until the mutex is released.  The mutex can be
    //    locked multiple times by the same thread but must be unlocked the
    //    same number of times.
    //
    //==EDOC====================================================================
    void Lock() const;
    //==SDOC====================================================================
    //==
    //==  TryLock
    //==
    //==========================================================================
    //
    //  Summary:
    //    Tries to lock the mutex.
    //
    //  Returns:
    //    True if the mutex was locked.
    //
    //  Description:
    //    Tries to lock the mutex.  TryLock has the same behavior as Lock,
    //    except that it returns immediatrely with false instead of blocking if
    //    the mutex is locked by another thread.
    //
    //==EDOC====================================================================
    bool TryLock() const;
    //==SDOC====================================================================
    //==
    //==  Unlock
    //==
    //==========================================================================
    //
    //  Summary:
    //    Unlocks the mutex.
    //
    //  Description:
    //    Unlocks the mutex.  A thread can only unlock a mutex it owns.
    //
    //==EDOC====================================================================
    void Unlock() const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//---------------
protected:
private:
    CMutex(const CMutex& from);
    CMutex& operator=(const CMutex& from);

// Hidden Data Members
//--------------------
protected:
private:
#if defined(MXD_OS_VXWORKS)
    mutable SEM_ID m_Mutex;
#elif defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)
    mutable pthread_mutex_t m_Mutex;
    #if defined(MXD_OS_NUCLEUS)
        mutable unsigned int m_uLockCount;
    #endif
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    mutable CRITICAL_SECTION m_mutex;
#elif defined(MXD_OS_SYMBIAN)
    mutable unsigned int m_uLockCount;
    mutable TThreadId m_threadId;
    mutable RCriticalSection m_mutex;
    mutable RCriticalSection m_csInternalProtection;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    mutable BOS_MUTEX m_Mutex;
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif
//M5T_INTERNAL_USE_END
};


//===========================================================================
//===========================================================================
//====  VXWORKS IMPLEMENTATION OF CMUTEX  ===================================
//===========================================================================
//===========================================================================


#if defined(MXD_OS_VXWORKS)

inline
CMutex::CMutex()
{
    m_Mutex = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE | SEM_DELETE_SAFE);
}

inline
CMutex::~CMutex()
{
    semDelete(m_Mutex);

    // Extra protection: Favorise early bug discovering!
    memset(&m_Mutex, 0, sizeof(m_Mutex));
}

inline
void CMutex::Lock() const
{
    semTake(m_Mutex, WAIT_FOREVER);
}

inline
bool CMutex::TryLock() const
{
    return (semTake(m_Mutex, NO_WAIT) == OK);
}

inline
void CMutex::Unlock() const
{
    semGive(m_Mutex);
}


//===========================================================================
//===========================================================================
//====  LINUX IMPLEMENTATION OF CMUTEX  =====================================
//===========================================================================
//===========================================================================


#elif defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)

inline
CMutex::CMutex()
{
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
    #if defined (MXD_OS_LINUX)
        pthread_mutexattr_setkind_np(&mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP);
    #elif defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
        pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    #elif defined(MXD_OS_NUCLEUS)
        m_uLockCount = 0;
    #endif

    pthread_mutex_init(&m_Mutex, &mutexAttr);
}

inline
CMutex::~CMutex()
{
    pthread_mutex_destroy(&m_Mutex);

    // Extra protection: Favorise early bug discovering!
    memset(&m_Mutex, 0, sizeof(m_Mutex));
}

inline
void CMutex::Lock() const
{
    #if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
        pthread_mutex_lock(&m_Mutex);
    #elif defined(MXD_OS_NUCLEUS)
        if (m_Mutex.owner_task_p == NU_Current_Task_Pointer())
        {
            m_uLockCount++;
        }
        else
        {
            pthread_mutex_lock(&m_Mutex);
            m_uLockCount = 1;
        }
    #endif
}

inline
bool CMutex::TryLock() const
{
    #if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
        return (pthread_mutex_trylock(&m_Mutex) == 0);
    #elif defined(MXD_OS_NUCLEUS)
        bool bReturn = false;
        if (m_Mutex.owner_task_p == NU_Current_Task_Pointer())
        {
            m_uLockCount++;
            bReturn = true;
        }
        else
        {
            if (pthread_mutex_trylock(&m_Mutex) == 0)
            {
                m_uLockCount = 1;
                bReturn = true;
            }
        }
        return bReturn;
    #endif
}

inline
void CMutex::Unlock() const
{
    #if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
        pthread_mutex_unlock(&m_Mutex);
    #elif defined(MXD_OS_NUCLEUS)
        if (m_Mutex.owner_task_p == NU_Current_Task_Pointer())
        {
            m_uLockCount--;
            if (m_uLockCount == 0)
            {
                pthread_mutex_unlock(&m_Mutex);
            }
        }
    #endif
}


//===========================================================================
//===========================================================================
//====  WINDOWS IMPLEMENTATION OF CMUTEX  ===================================
//===========================================================================
//===========================================================================


#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)

inline
CMutex::CMutex()
{
    InitializeCriticalSection(&m_mutex);
}

inline
CMutex::~CMutex()
{
    DeleteCriticalSection(&m_mutex);

    // Extra protection: Favorise early bug discovering!
    memset(&m_mutex, 0, sizeof(m_mutex));
}

inline
void CMutex::Lock() const
{
    EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&m_mutex));
}

inline
bool CMutex::TryLock() const
{
    return (TryEnterCriticalSection(const_cast<CRITICAL_SECTION*>(&m_mutex)) != 0);
}

inline
void CMutex::Unlock()  const
{
    LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&m_mutex));
}

//===========================================================================
//===========================================================================
//====  SYMBIAN IMPLEMENTATION OF CMUTEX  ===================================
//===========================================================================
//===========================================================================

#elif defined(MXD_OS_SYMBIAN)

inline
CMutex::CMutex()
: m_threadId(0)
{
    m_mutex.CreateLocal();
    m_csInternalProtection.CreateLocal();
}

inline
CMutex::~CMutex()
{
    m_mutex.Close();
    m_csInternalProtection.Close();
}

inline
void CMutex::Lock() const
{
    RThread thisThread;

    m_csInternalProtection.Wait();
    /**/
    /**/    if (m_threadId == thisThread.Id())
    /**/    {
    /**/        m_uLockCount++;
    /**/    }
    /**/    else
    /**/    {
    /**/        m_csInternalProtection.Signal();
    /**/        m_mutex.Wait();
    /**/        m_csInternalProtection.Wait();
    /**/        m_uLockCount = 1;
    /**/        m_threadId = thisThread.Id();
    /**/    }
    /**/
    m_csInternalProtection.Signal();
}

inline
bool CMutex::TryLock() const
{
    RThread thisThread;
    bool bReturn = false;

    m_csInternalProtection.Wait();
    /**/
    /**/    if (m_threadId == thisThread.Id())
    /**/    {
    /**/        m_uLockCount++;
    /**/        bReturn = true;
    /**/    }
    /**/    else
    /**/    {
    /**/        bReturn = !m_mutex.IsBlocked();
    /**/        if (bReturn)
    /**/        {
    /**/            m_mutex.Wait();
    /**/            m_uLockCount = 1;
    /**/            m_threadId = thisThread.Id();
    /**/        }
    /**/    }
    /**/
    m_csInternalProtection.Signal();

    return bReturn;
}

inline
void CMutex::Unlock()  const
{
    m_csInternalProtection.Wait();
    /**/
    /**/    if (m_threadId == RThread().Id())
    /**/    {
    /**/        m_uLockCount--;
    /**/        if (m_uLockCount == 0)
    /**/        {
    /**/            m_threadId = 0;
    /**/            m_mutex.Signal();
    /**/        }
    /**/    }
    /**/
    m_csInternalProtection.Signal();
}

// BRCM_CUSTOM - [begin] - Add ECOS support.
//===========================================================================
//===========================================================================
//====  BOS IMPLEMENTATION OF CMUTEX  =======================================
//===========================================================================
//===========================================================================
#elif defined(MXD_OS_BOS_ECOS)

inline
CMutex::CMutex()
{
    bosMutexCreate( NULL, &m_Mutex );
};

inline
CMutex::~CMutex()
{
    bosMutexDestroy( &m_Mutex );

    // Extra protection: Favorise early bug discovering!
    memset( &m_Mutex, 0, sizeof(m_Mutex) );
};

inline
void CMutex::Lock() const
{
    bosMutexAcquire( &m_Mutex );
};

inline
bool CMutex::TryLock() const
{
    // Try acquiring mutex for 100 msecs.
    return (bosMutexTimedAcquire( &m_Mutex, 100 ) == BOS_STATUS_OK);
}

inline
void CMutex::Unlock() const
{
    bosMutexRelease( &m_Mutex );
};
// BRCM_CUSTOM - [end] - Add ECOS support.

#elif defined(MXD_OS_NONE)

inline
CMutex::CMutex()
{
}

inline
CMutex::~CMutex()
{
}

inline
void CMutex::Lock() const
{
}

inline
bool CMutex::TryLock() const
{
    return false;
}

inline
void CMutex::Unlock()  const
{
}

#endif
//===========================================================================
//====  INLINE FUNCTIONS  ===================================================
//===========================================================================


MX_NAMESPACE_END(MXD_GNS)


#endif // MXG_CMUTEX_H


