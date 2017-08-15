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
#ifndef MXG_CTHREAD_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CTHREAD_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_ANDROID)
    #define MXD_THREAD_NAME_SIGNATURE "*TN*"
#endif
//M5T_INTERNAL_USE_END

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
    #include "Config/MxConfig.h"
#endif

#if !defined(MXD_OS_NONE)

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
    #include "Basic/CAlignedVariableStorage.h"
#endif

#ifndef MXG_CMUTEX_H
    #include "Kernel/CMutex.h"
#endif

#ifndef MXG_CSEMAPHORE_H
    #include "Kernel/CSemaphore.h"
#endif

//-- OS-specific inclusions
//---------------------------
#if defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)
    #include <pthread.h>
#elif defined(MXD_OS_WINDOWS)
    #include <process.h>
#elif defined(MXD_OS_WINDOWS_CE)
    #include <winuser.h>
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    #include <bosTask.h>
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif

//<GROUP KERNEL_VARIABLES>
//
// Description:
// Default size of thread name (32 bytes, including the NULL character).
//-----------------------------------------------------------------------
const unsigned int g_uTHREAD_NAME_MAX_SIZE = 32;

MX_NAMESPACE_START(MXD_GNS)

//==SDOC========================================================================
//== Class: CThread
//========================================
//<GROUP KERNEL_CLASSES>
//
// Summary:
//   Allows the creation of a new thread of execution.
//
// Description:
//   This class allows the creation of a new thread of execution.
//
//   It can control the start-up and synchronize on the termination
//   while fetching an optional return code.
//   The execution priority is adjustable on 5 distinctive levels
//   (which are all static and predictable).
//
//   This class also supports registration of thread specific data (TSD),
//   as long as a unique thread Id is created even for non-CThread threads.
//   It also offers global methods to retrieve the CThread object of the
//   currently executing thread (if, of course, it is a derivative of CThread).
//
//   The interface of CThread is identical and the behavior of the threads
//   are predictable for every supported operating system.
//
// Example:
//      Here is an example of how to create a high priority thread of execution.
//
//  <CODE>
//
//      // Starts a new high priority thread with the default stack size.
//      CThread t;
//      mxt_opaque opqResult;
//      t.StartThread(ResolverThreadEntry,
//                    MX_VOIDPTR_TO_OPQ(this),
//                    "ResolverThread",
//                    0,
//                    CThread::eHIGH);
//
//      // Wait for thread termination.
//      t.Join(&opqResult);
//
//  </CODE>
//
// Warning:
//      CThread implementation on the Nucleus platform is based on the NUCLEUS
//      POSIX layer. This layer is not compatible with the standard NUCLEUS
//      task management functions. M5T code behavior is not defined when it is
//      called from a non POSIX thread context.
//
// Location:
//   Kernel/CThread.h
//
//==EDOC========================================================================
class CThread
{
//-- Published fonction pointer types
//-------------------------------------
public:

    //  Summary:
    //    Function pointer that is used for the StartThread method.
    //--------------------------------------------------------------
    typedef mxt_opaque (*PFNEntryPoint) (IN mxt_opaque opq);

    //  Summary:
    //    Function pointer that is used for when thread terminates.
    //--------------------------------------------------------------
    typedef void       (*PFNKeyDeletion)(IN mxt_opaque opq);

    //  Summary:
    //    A TSD key.
    //---------------
    typedef unsigned mxt_tsdkey;

//-- Inner structures used to manage thread-specific data (TSD)
//---------------------------------------------------------------

//M5T_INTERNAL_USE_BEGIN
private:
    // Structure used to hold a unique TSD key. Each object is part of a single
    // linked list and is also referenced by zero to many TSD elements.
    struct STsdKey
    {
        STsdKey*       m_pNext;
        mxt_tsdkey     m_TsdKey;
        PFNKeyDeletion m_pfnKeyDeletion;
        int            m_nReferenceCount;
    };

    // Structure used to hold data which is specific to a thread.
    // Each object has non-null data, is part of a single linked list
    // and the head is referenced by the TSD structure.
    struct STsdElement
    {
        STsdElement*   m_pNext;
        STsdKey*       m_pKey;
        mxt_opaque     m_opqDatum;
    };

    // Structure used to hold the TSD. Each object is unique to a thread
    // and holds the head of the single linked list of TSD element.
    struct STsd
    {
        STsd*        m_pNext;
        CThread*     m_pThread;
        unsigned     m_uId;
        bool         m_bInternalThread;
#if defined(MXD_OS_VXWORKS)
        int     m_uVxThreadId;
#endif
// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
        BOS_TASK_ID  m_uEcosThreadId;
        unsigned     m_uMagic;
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.
        STsdElement* m_pTsdHead;
#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
        // On MS-Windows implementation, the deletion of TSD if handled by
        // a special function since the OS has no facility for this.
        HANDLE       m_hThread;
        HANDLE       m_hTerminationHandlerEvent;
        DWORD        m_uThreadId;
#endif
    };
//M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
#if defined(MXD_THREAD_STACK_INFO_ENABLE_SUPPORT)

    //==SDOC====================================================================
    //==
    //==  SThreadStackInfo
    //==
    //==========================================================================
    //<GROUP KERNEL_STRUCTS>
    //
    //  Summary:
    //      Structure to store stack usage information.
    //
    //  Description:
    //      This structure is used to store different information related to
    //      stack usage.
    //
    //==EDOC====================================================================
    struct SThreadStackInfo
    {
        // Description:
        // The peak usage in the stack of the thread. Note that this value is
        // only an approximate.
        unsigned int    m_uPeakUsage;
        // Description:
        // The name of the thread.
        char            m_szThreadName[g_uTHREAD_NAME_MAX_SIZE];
    };
#endif

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //  Define 5 standard levels of priority.
    enum EPriority
    {
        // Description:
        // Lowest priority.
        eLOWEST,
        // Description:
        // Low priority.
        eLOW,
        // Description:
        // Normal priority.
        eNORMAL,
        // Description:
        // High priority.
        eHIGH,
        // Description:
        // Highest priority.
        eHIGHEST,
        // Description:
        // Thread priority extension mechanism.
        MXD_THREAD_PRIORITY_EXTENSION
        // Must be the last and must not be used.
        eTHREAD_PRIORITY_ENUM_SIZE
    };

    //<GROUP KERNEL_ENUMS>
    //
    // Summary:
    //  Current state of the thread
    //--------------------------------
    enum EState
    {
        // Description:
        // Thread is not started.
        eNOT_STARTED,
        // Description:
        // Thread is strarting.
        eSTARTING,
        // Description:
        // Thread is active.
        eACTIVE,
        // Description:
        // Thread is terminated.
        eTERMINATED,
        // Description:
        // Thread is deleted.
        eDELETED
    };

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CThread();

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CThread();

    // Summary:
    //  Comparison operator.
    //-----------------------
    bool operator==(IN const CThread& rThread) const;

    // Summary:
    //  Different than operator.
    //---------------------------
    bool operator!=(IN const CThread& rThread) const;

    // Summary:
    //  Starts a new thread.
    //-----------------------
    mxt_result StartThread(IN PFNEntryPoint pfnThreadEntry,
                           IN mxt_opaque opqParam,
                           IN const char* pszName = NULL,
                           IN uint32_t uStackSize = 0,
                           IN EPriority ePriority = eNORMAL);

    // Summary:
    //  Waits for thread termination.
    //--------------------------------
    mxt_result Join(OUT mxt_opaque* popqExitCode = NULL);

    // Summary:
    //  Gets the address of the current thread.
    //------------------------------------------
    static CThread* GetThreadSelf();

    // Summary:
    //  Checks if thread is current thread of execution.
    //---------------------------------------------------
    bool IsCurrentThread() const;

    // Summary:
    //  Gets the current state of the thread.
    //----------------------------------------
    EState GetState() const;

    // Summary:
    //  Creates a new TSD key.
    //-----------------------------
    static mxt_result CreateKey(IN PFNKeyDeletion pfnKeyDeletion, OUT mxt_tsdkey& rTsdKey);

    // Summary:
    //  Deletes the specified TSD key.
    //---------------------------------
    static mxt_result DeleteKey(IN mxt_tsdkey tsdKey);

    // Summary:
    //  Sets new data for specified key.
    //-----------------------------------
    static mxt_result SetSpecific(IN mxt_tsdkey tsdKey, IN mxt_opaque opqDatum);

    // Summary:
    //  Gets the data for the specified key.
    //---------------------------------------
    static mxt_result GetSpecific(IN mxt_tsdkey tsdKey, OUT mxt_opaque& ropqDatum);

    // Summary:
    //  Initializes the TSD.
    //-----------------------
    static mxt_result InitializeTsd();

    // Summary:
    //  Finalizes the TSD.
    //---------------------
    static void FinalizeTsd();

    // Summary:
    //  Gets the application ID of this thread.
    //------------------------------------------
    unsigned GetId() const;

    // Summary:
    //  Gets the application ID of the current thread of execution.
    //--------------------------------------------------------------
    static unsigned GetCurrentId();

    // Summary:
    //  Gets the native thread ID.
    //-----------------------------
    mxt_result GetNativeThreadId(OUT int& nNativeThreadId);

    // Summary:
    //  Gets the name of the thread.
    //-------------------------------
    const char* GetName() const;

#if defined(MXD_THREAD_STACK_INFO_ENABLE_SUPPORT)
    // Summary:
    //  Gets the stack information of the current CThread.
    //-----------------------------------------------------
    static mxt_result GetThreadStackInfo(OUT SThreadStackInfo* pstStackInfo);

    // Summary:
    //  Gets the stack information of a specific CThread.
    //----------------------------------------------------
    static mxt_result GetThreadStackInfo(IN const char* pszThreadName,
                                         OUT SThreadStackInfo* pstStackInfo);

    // Summary:
    //  Gets the stack information for all CThread.
    //----------------------------------------------
    static mxt_result GetThreadStackInfo(IN unsigned int uStackInfoCapacity,
                                         OUT SThreadStackInfo* pstStackInfo,
                                         OUT unsigned int* puStackInfoSize);

    // Summary:
    //  Resets the stack information for the current CThread.
    //--------------------------------------------------------
    static mxt_result ResetThreadStackInfo();
#endif

//M5T_INTERNAL_USE_BEGIN
    // This is the internal entry functions of the thread.
    // Must be public because MxTraceCallStack needs them...
// BRCM_CUSTOM - [mod] - Add ECOS support.
#if defined(MXD_OS_VXWORKS) || \
    defined(MXD_OS_LINUX)   || \
    defined(MXD_OS_ANDROID) || \
    defined(MXD_OS_SOLARIS) || \
    defined(MXD_OS_DARWIN)  || \
    defined(MXD_OS_BOS_ECOS)|| \
    defined(MXD_OS_NUCLEUS)
// BRCM_CUSTOM - [end] - Add ECOS support.
    static void* ThreadEntry(IN void* pParam);
    static void ThreadEntryEnd();
#endif
//M5T_INTERNAL_USE_END

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
//-------------------
private:
    CThread(IN const CThread& from);
    CThread& operator=(IN const CThread& from);

    // This is the internal entry function of the thread.
#if defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    static unsigned long __stdcall ThreadEntry(IN void* pParam);
#elif defined(MXD_OS_SYMBIAN)
    static int ThreadEntry(IN void* pParam);
#endif

    // TSD manipulation method.
    static STsd* GetCurrentTsd();
    static STsdKey* FindTsdKey(IN mxt_tsdkey tsdKey, OUT STsdKey** pPreviousKey = NULL);
    static void DeleteTsdKey(IN STsdKey* pKey, IN STsdKey* pPreviousKey);
    static void FreeTsdCallback(IN STsd* pTsd);

    // Peak stack size detection methods.
#if defined(MXD_THREAD_STACK_INFO_ENABLE_SUPPORT)
    static mxt_result InitializeThreadStackInfo(IN CThread* pThread);
    static mxt_result InternalGetThreadStackInfo(IN CThread* pThread,
                                                 OUT SThreadStackInfo* pstStackInfo);
    static bool IsStackIncreasing(IN void* pvoid);
#endif

    // OS-specific global functions.
// BRCM_CUSTOM - [mod] - Add ECOS support.
#if defined(MXD_OS_VXWORKS) || defined(MXD_OS_SYMBIAN) || defined(MXD_OS_BOS_ECOS)
// BRCM_CUSTOM - [end] - Add ECOS support.

    static CAlignedVariableStorage<CBinarySemaphore> ms_waitThreadEntryExit;

    #if defined(MXD_OS_VXWORKS)
        struct SThreadEntryExit
        {
            SThreadEntryExit*   m_pstNext;
            int                 m_nThreadId;
        };

        static void* VxTaskTerminationThread(IN void* pParam);
        static int VxTaskDeleteCallback(int nTaskId);
    #elif defined(MXD_OS_SYMBIAN)
        // Termination thread.
        static RThread ms_hSymbianTerminationThread;
        // Termination thread entry function.
        static int SymbianDeleteCallBack(void* pParam);
// BRCM_CUSTOM - [begin] - Add ECOS support.
    #elif defined(MXD_OS_BOS_ECOS)
        struct SThreadEntryExit
        {
            SThreadEntryExit*   m_pstNext;
            BOS_TASK_ID         m_hThreadId;
            cyg_handle_t              m_pTsd;
        };
        static void* CThread::ECosTaskTerminationThread(IN void *pParam);
        static void ECosTaskExitCallback(cyg_addrword_t data);    
// BRCM_CUSTOM - [end] - Add ECOS support.
    #endif
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    static DWORD WINAPI WinThreadTerminationHandler(LPVOID pParameter);
#endif

    static mxt_result InitializeCThread();
    static void FinalizeCThread();

// Friends declarations
//----------------------
    friend mxt_result InitializeCThread();
    friend void FinalizeCThread();

//-- Hidden Data Members
//------------------------
private:
    // Pointer of the thread entry function and the parameters passed to it.
    PFNEntryPoint    m_pfnThreadEntry;
    mxt_opaque       m_opqParam;

    char             m_szName[g_uTHREAD_NAME_MAX_SIZE];          // Thread name.
    EState           m_eState;          // Thread state.
    unsigned         m_uId;             // Thread unique ID (also kept in STsd).
    mxt_opaque       m_opqLastExitCode; // Exit code after thread termination.
    unsigned int     m_uStackSize;      // Initial size of the thread's stack.

    // This semaphore is in the signalled state only when the thread is not alive.
    // It is used by Join to synchronize on termination.
    CBinarySemaphore m_semTerminated;

    EPriority        m_ePriority;       // Thread priority

    // Peak stack size detection variable.
#if defined(MXD_THREAD_STACK_INFO_ENABLE_SUPPORT)
    uint8_t* m_puStartOfStack;
    bool m_bIsStackIncreasing;
#endif

#if defined(MXD_OS_VXWORKS)
    int              m_hThread;
    static SThreadEntryExit* ms_pstThreadEntryExitHead;
    static int       ms_hThreadVxTaskTermination;
    static STsd*     ms_pTsd;
#elif defined(MXD_OS_LINUX) || defined(MXD_OS_SOLARIS) || defined(MXD_OS_DARWIN) || defined(MXD_OS_NUCLEUS) || defined(MXD_OS_ANDROID)
    pthread_t        m_hThread;
    static pthread_key_t ms_hTsdKey;
    #if defined(MXD_OS_NUCLEUS)
        CMutex       m_mutexJoinProtection;
    #elif defined(MXD_OS_LINUX) || defined(MXD_OS_ANDROID)
        int m_nNativeThreadId;
        CBinarySemaphore* m_psemThreadEntryWait;
    #endif
#elif defined(MXD_OS_WINDOWS) || defined(MXD_OS_WINDOWS_CE)
    HANDLE           m_hThread;

    //Key for the TSD
    static DWORD ms_uTsdKey;

    // On Win32, we need a thread to handle thread termination and free TSD.
    // The operating system does not offer any facilities for this.
    static HANDLE    ms_hTerminationHandlerThread;
    static HANDLE    ms_hTerminationHandlerEvent;

#elif defined(MXD_OS_SYMBIAN)
    // Thread Handle.
    RThread m_hThread;

    // Static handle used for TSD retrieval.
    static int ms_uTsdHandle;

    static CAlignedVariableStorage<RArray<STsd*> > ms_tsdsToDelete;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    BOS_TASK_ID         m_hThread;
    static SThreadEntryExit* ms_pstThreadEntryExitHead;
    static BOS_TASK_ID ms_hECosTaskTerminationThreadId;
    static cyg_ucount32 ms_uTsdIndex;
// BRCM_CUSTOM - [end] - Add ECOS support.
#endif // MXD_OS_...

    // Global TSD and TSD KEY lists.
    static STsd*    ms_pTsdHead;
    static STsdKey* ms_pTsdKeyHead;

    // Unique thread ID generator.
    static unsigned  ms_uThreadId;

    // General mutex for global access protection.
    static CAlignedVariableStorage<CMutex> ms_mutex;

    // Flag used for first-time TSD initialization.
    static volatile bool ms_bTsdInitialized;

    // Used to assign key number to thread.
    static mxt_tsdkey ms_keyCounter;

// BRCM_CUSTOM - [mod] - Add ECOS support.
    static volatile unsigned int ms_uThreadCounter;
// BRCM_CUSTOM - [end] - Add ECOS support.
    static CAlignedVariableStorage<CBinarySemaphore> ms_semFinalizeWaitThread;

// BRCM_CUSTOM - [mod] - Add ECOS support.
    static volatile bool ms_bFinalizing;
// BRCM_CUSTOM - [end] - Add ECOS support.
    static bool ms_bCThreadExists;
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//==SDOC========================================================================
//==
//== CThread::operator!=
//==
//==============================================================================
//
// Parameters:
//   rThread:
//     A reference to the other CThread object to compare with.
//
// Returns:
//   True if both CThread objects have a different unique ID, or false otherwise.
//
// Description:
//   This method reverts the answer returned by the operator ==.
//
// See Also:
//   CThread::operator==
//
//==EDOC========================================================================
inline bool CThread::operator!=(IN const CThread& rThread) const
{
    return !operator==(rThread);
}

//==SDOC========================================================================
//==
//== CThread::GetState
//==
//==============================================================================
//
// Returns:
//   The current state of the CThread object.
//
// Description:
//   Gets the current state of the CThread object.
//
//   The possible states are:
//   - eNOT_STARTED
//       After the creation of the CThread object (before calling StartThread),
//       after a call to StartThread that failed,
//       and after a successful call to Join.
//   - eSTARTING
//       After a successful call to StartThread, but before the thread begins
//       its execution.
//   - eACTIVE
//       The thread is currently executing its thread entry function.
//   - eTERMINATED
//       The thread entry function has exited (the thread execution is completed),
//       but a successful call to Join as not been done yet.
//   - eDELETED
//       The CThread object destructor was called. NOTE: When working from the
//       standard heap memory (malloc/free), this state should never be seen,
//       and it would most certainly indicate a bug.
//
//==EDOC========================================================================
inline CThread::EState CThread::GetState() const
{
    return m_eState;
}

//==SDOC========================================================================
//==
//== CThread::GetId
//==
//==============================================================================
//
// Returns:
//   The unique application ID of the thread of the CThread object. A value of
//   0 means the thread was never started (StartThread never called), and if
//   the thread has terminated, it returns the application ID of the last
//   thread.
//
// Description:
//   Gets the unique application ID of the thread of this CThread object.
//
// See Also:
//   CThread::GetCurrentId
//
//==EDOC========================================================================
inline unsigned CThread::GetId() const
{
    return m_uId;
}

//==SDOC========================================================================
//==
//== CThread::GetName
//==
//==============================================================================
//
// Returns:
//   A pointer to a null-terminated character string which contains the name
//   of the current (or last created) thread. An empty string is returned if
//   no thread was started yet for this CThread object.
//
// Description:
//   Gets the name of the thread, which was given to StartThread.
//
//==EDOC========================================================================
inline const char* CThread::GetName() const
{
    return m_szName;
}

//M5T_INTERNAL_USE_BEGIN
MX_NAMESPACE_END(MXD_GNS)
//M5T_INTERNAL_USE_END

#endif // #if !defined(MXD_OS_NONE)

#endif // MXG_CTHREAD_H
