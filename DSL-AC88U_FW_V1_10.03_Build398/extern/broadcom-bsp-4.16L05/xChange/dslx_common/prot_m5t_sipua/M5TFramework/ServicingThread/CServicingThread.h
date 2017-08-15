//==SDOC========================================================================
//==============================================================================
//
//   Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_CSERVICINGTHREAD_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CSERVICINGTHREAD_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
//-------------------------------
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
                                 // MXD_NETWORK_ENABLE_SUPPORT
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)
//-- Data Members
//-----------------
#ifndef MXG_CALIGNEDVARIABLESTORAGE_H
#include "Basic/CAlignedVariableStorage.h"
#endif

#ifndef MXG_CLIST_H
#include "Cap/CList.h"
#endif
#ifndef MXG_CUNCMP_H
#include "Cap/CUncmp.h"
#endif
#ifndef MXG_CAATREE_H
#include "Cap/CAATree.h"
#endif
#ifndef MXG_IMESSAGESERVICEMGR_H
#include "ServicingThread/IMessageServiceMgr.h"
#endif
#ifndef MXG_ITIMERSERVICEMGR_H
#include "ServicingThread/ITimerServiceMgr.h"
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    #ifndef MXG_IPOLLEDREQUESTSTATUSMGR_H
    #include "Network/IPolledRequestStatusMgr.h"
    #endif
    #ifndef MXG_IREQUESTSTATUSSERVICEMGR_H
    #include "ServicingThread/IRequestStatusServiceMgr.h"
    #endif
#else
    #ifndef MXG_IPOLLEDSOCKETMGR_H
    #include "Network/IPolledSocketMgr.h"
    #endif
    #ifndef MXG_ISOCKETSERVICEMGR_H
    #include "ServicingThread/ISocketServiceMgr.h"
    #endif
#endif
#endif


//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_CECOMDELEGATINGUNKNOWN_H
#include "ECom/CEComDelegatingUnknown.h"
#endif

#ifndef MXG_CALIVEOBJ_H
#include "Kernel/CAliveObj.h"
#endif

#ifndef MXG_IACTIVATIONSERVICE_H
#include "ServicingThread/IActivationService.h"
#endif

#ifndef MXG_IMESSAGESERVICE_H
#include "ServicingThread/IMessageService.h"
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
    #if defined(MXD_OS_SYMBIAN)
        #ifndef MXG_IREQUESTSTATUSSERVICE_H
        #include "ServicingThread/IRequestStatusService.h"
        #endif
    #else
        #ifndef MXG_ISOCKETSERVICE_H
        #include "ServicingThread/ISocketService.h"
        #endif
    #endif
#endif

#ifndef MXG_ITIMERSERVICE_H
#include "ServicingThread/ITimerService.h"
#endif

#ifndef MXG_IMESSAGESERVICEMGR_H
#include "ServicingThread/IMessageServiceMgr.h"
#endif

#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
    #ifndef MXG_ITIMERSERVICEMGR_H
    #include "ServicingThread/ITimerServiceMgr.h"
    #endif
    #ifndef MXG_IWATCHDOGSERVICE_H
    #include "ServicingThread/IWatchdogService.h"
    #endif
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class CMarshaler;
#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    class CPollRequestStatus;
#else
    class CPollSocket;
    class CUdpSocket;
#endif
#endif

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const unsigned int uMAX_VISITED_EXECUTION_CONTEXTS = 5;
//M5T_INTERNAL_USE_END
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CServicingThread
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Class offering different services that are processed in the execution
//   context of a single shared thread.
//
// Description:
//   CServicingThread offers different services that are processed in the
//   execution context of a single shared thread.
//
//   Three types of service are offered: asynchronous/synchronous messages
//   queuing, periodic/single shot timers, and socket status change detection
//   (request status change detection on Symbian). By using a single execution
//   context, all events generated by these services are synchronized, helping
//   to simplify the burden of concurrent access protection.
//
//   CServicingThread implements four interfaces: IActivationService,
//   IMessageService, ITimerService, and ISocketService (or
//   IRequestStatusService on Symbian).
//
//   CServicingThread is an ECOM object. It must be created with a call to
//   CreateEComInstance. It is released from memory when its reference count
//   becomes 0.
//
//   CServicingThread does the detection of inter servicing thread dead lock on
//   synchronous calls. When a dead lock is detected, the servicing thread
//   processes the synchronous call as if it was made from the same servicing
//   thread; in other words, as if it was made from internally, bypassing the
//   message queue.
//
//  <CODE>
//
//  IActivationService* pActivationService = NULL;
//
//  mxt_result res =
//      CreateEComInstance(CLSID_CServicingThread,
//                         NULL,
//                         IID_IActivationService,
//                         reinterpret_cast<void**>(&pActivationService));
//
//  if (MX_RIS_S(res))
//  {
//      // Activate the CServicingThread. A new thread will be recreated.
//      res = pActivationService->Activate();
//  }
//
//  if (MX_RIS_S(res))
//  {
//      ... do something here ...
//  }
//
//  // Release the CServicingThread.
//  if (pActivationService)
//  {
//      pActivationService->ReleaseIfRef();
//      pActivationService = NULL;
//  }
//
//  </CODE>
//
// Location:
//   ServicingThread/CServicingThread.h
//
// See Also:
//
//==EDOC========================================================================
class CServicingThread : public CEComDelegatingUnknown,
                         protected CAliveObj,
                         protected IActivationService,
                         protected IMessageService,
                         protected IMessageServiceMgr,
                         protected ITimerService
#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
                         ,protected ITimerServiceMgr,
                         protected IWatchdogService
#endif
#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
                         ,protected IRequestStatusService,
                         protected IPolledRequestStatusMgr
#else
                         ,protected ISocketService,
                         protected IPolledSocketMgr
#endif
#endif
{
// Published Interface
//---------------------
public:

    // << Constructors / Destructors >>
    //----------------------------------

    // Summary:
    //  Constructor.
    //---------------
    CServicingThread(IN IEComUnknown* pOuterIEComUnknown = NULL);

    // Summary:
    //  Destructor.
    //---------------
    virtual ~CServicingThread();

    // << ECom Pattern >>
    //--------------------

    // Summary:
    //  Creates a CServicingThread instance.
    //---------------------------------------
    static mxt_result CreateInstance(IN IEComUnknown* pOuterIEComUnknown,
                                     OUT CEComUnknown** ppCEComUnknown);

    MX_DECLARE_DELEGATING_IECOMUNKNOWN


    // Summary:
    //  Initializes the instance.
    //----------------------------
    virtual mxt_result InitializeInstance();

    // Summary:
    //  Uninitializes the instance.
    //------------------------------
    virtual void UninitializeInstance(OUT bool* pbDeleteThis);

    // Summary:
    //  Queries an object for a supported interface.
    //------------------------------------------------
    virtual mxt_result NonDelegatingQueryIf(IN mxt_iid iidRequested,
                                            OUT void** ppInterface);

    // Summary:
    //  Decrements the reference count on the ECOM implementing this interface.
    //--------------------------------------------------------------------------
    virtual unsigned int NonDelegatingReleaseIfRef();

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // Inherited from CAliveObj.
    //---------------------------

    // Summary:
    // It is called when the thread is started.
    //------------------------------------------
    virtual void Behavior();

    // Summary:
    // Signals the termination of the thread using whatever method it supports.
    //--------------------------------------------------------------------------
    virtual void BehaviorTerminating();

    // Inherited from IActivationService.
    //------------------------------------

    // Summary:
    //  Requests the activation using an internal thread.
    //----------------------------------------------------
    virtual mxt_result Activate(IN const char* pszName = NULL,
                                IN uint32_t uStackSize = 0,
                                IN EPriority ePriority = CThread::eNORMAL);

    // Summary:
    //  Requests the activation using an external thread.
    //----------------------------------------------------
    virtual mxt_result Activate(IN uint64_t uTimeoutMs,
                                OUT bool* pbReadyToRelease);

    // Summary:
    //  Determines whether or not this Servicing Thread is the current
    //  execution context.
    //-----------------------------------------------------------------
    virtual bool IsCurrentExecutionContext();

    // Inherited from IMessageService.
    //---------------------------------

    // Summary:
    //  Pushes a new message onto the message queue.
    //-----------------------------------------------
    virtual mxt_result PostMessage(IN IMessageServiceMgr* pManager,
                                   IN bool bWaitCompletion,
                                   IN unsigned int uMessageId,
                                   IN TOS CMarshaler* pParameter = NULL);


    // Inherited from IMessageServiceMgr.
    //------------------------------------

    // Summary:
    //  Notifies the manager that a new message must be processed.
    //-------------------------------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessageId,
                                           IN CMarshaler* pParameter);

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    // Inherited from IRequestStatusService.
    //---------------------------------------

    // Summary:
    //  Registers a request status.
    //------------------------------
    virtual mxt_result RegisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                             IN IRequestStatusServiceMgr* pManager,
                                             IN mxt_opaque opq = MX_INT32_TO_OPQ(0));
    // Summary:
    //  Enables the detection of request status completion.
    //------------------------------------------------------
    virtual mxt_result EnableCompletionDetection(IN TRequestStatus* pRequestStatus);
    // Summary:
    //  Disables the detection of request status completion.
    //-------------------------------------------------------
    virtual mxt_result DisableCompletionDetection(IN TRequestStatus* pRequestStatus);
    // Summary:
    //  Unregisters a request status.
    //--------------------------------
    virtual mxt_result UnregisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                               OUT mxt_opaque* popq = NULL);
    // Summary:
    //  Internally registers a request status.
    //-----------------------------------------
    void InternalRegisterRequestStatus(IN CMarshaler* pParameter);
    // Summary:
    //  Internally enables the detection of request status completion.
    //-----------------------------------------------------------------
    void InternalEnableCompletionDetection(IN CMarshaler* pParameter);
    // Summary:
    //  Internally disables the detection of request status completion.
    //------------------------------------------------------------------
    void InternalDisableCompletionDetection(IN CMarshaler* pParameter);
    // Summary:
    //  Internally unregisters a request status.
    //-------------------------------------------
    void InternalUnregisterRequestStatus(IN CMarshaler* pParameter);

#else
    // Summary:
    //  Initializes the sockets.
    mxt_result InitializeSocket();

    // Summary:
    //  Uninitializes the sockets.
    mxt_result UninitializeSocket();

    // Summary:
    //  Refreshes broken sockets.
    mxt_result RefreshBrokenSocket();

    // Inherited from ISocketService.
    //--------------------------------
    // Summary:
    //  Registers a socket.
    //----------------------
    virtual mxt_result RegisterSocket(IN mxt_hSocket hSocket,
                                      IN ISocketServiceMgr* pManager,
                                      IN mxt_opaque opq = MX_INT32_TO_OPQ(0));

    // Summary:
    //  Enables the detection of events.
    //-----------------------------------
    virtual mxt_result EnableEventsDetection(IN mxt_hSocket hSocket,
                                             IN unsigned int uEvents);
    // Summary:
    //  Disables the detection of events.
    //------------------------------------
    virtual mxt_result DisableEventsDetection(IN mxt_hSocket hSocket,
                                              IN unsigned int uEvents);
    // Summary:
    //  Unregisters a socket.
    //------------------------
    virtual mxt_result UnregisterSocket(IN mxt_hSocket hSocket,
                                        OUT mxt_opaque* popq = NULL);

    // Summary:
    //  Internal registers a socket.
    //-------------------------------
    void InternalRegisterSocket(IN CMarshaler* pParameter);

    // Summary:
    //  Internal enables the detection of events.
    //--------------------------------------------
    void InternalEnableEventsDetection(IN CMarshaler* pParameter);

    // Summary:
    //  Internal disables the detection of events.
    //---------------------------------------------
    void InternalDisableEventsDetection(IN CMarshaler* pParameter);

    // Summary:
    //  Internal unregisters a socket.
    //---------------------------------
    void InternalUnregisterSocket(IN CMarshaler* pParameter);

#endif // #if defined(MXD_OS_SYMBIAN)
#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

    // Inherited from ITimerService.
    //-------------------------------
    // Summary:
    //  Starts a new exponential timer.
    //----------------------------------
    virtual mxt_result StartTimer(IN ITimerServiceMgr* pManager,
                                  IN unsigned int uTimerId,
                                  IN uint64_t uFloorTimeoutMs,
                                  IN uint64_t uCeilingTimeoutMs,
                                  IN unsigned int uMultBy,
                                  IN unsigned int uDivBy,
                                  IN bool bStopAtCeiling,
                                  IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                                  IN EPeriodicity ePeriodicity =
                                    ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_NO_CYCLE_LOST);
    // Summary:
    //  Starts a new linear timer.
    //-----------------------------
    virtual mxt_result StartTimer(IN ITimerServiceMgr* pManager,
                                  IN unsigned int uTimerId,
                                  IN uint64_t uTimeoutMs,
                                  IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                                  IN EPeriodicity ePeriodicity =
                                      ePERIODICITY_NOT_PERIODIC);
    // Summary:
    //  Stops a timer owned by a manager.
    //------------------------------------
    virtual mxt_result StopTimer(IN ITimerServiceMgr* pManager,
                                 IN unsigned int uTimerId);
    // Summary:
    //  Stops all timers owned by a manager.
    //---------------------------------------
    virtual mxt_result StopAllTimers(IN ITimerServiceMgr* pManager);

    // Summary:
    //  Starts a new internal exponential timer.
    //-------------------------------------------
    void InternalStartTimerExponential(IN CMarshaler* pParameter);

    // Summary:
    //  Starts a new internal linear timer.
    //--------------------------------------
    void InternalStartTimerLinear(IN CMarshaler* pParameter);

    // Summary:
    //  Stops an internal timer owned by a manager.
    //----------------------------------------------
    void InternalStopTimer(IN CMarshaler* pParameter);

    // Summary:
    //  Stops all internal timers owned by a manager.
    //------------------------------------------------
    void InternalStopAllTimers(IN CMarshaler* pParameter);

#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
    // Inherited from ITimerServiceMgr.
    //-------------------------------
    //  Summary:
    //   Notifies the manager that a new timer elapsed or has been stopped.
    //---------------------------------------------------------------------
    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimer,
                                 IN mxt_opaque opq);

    // Inherited from IWatchdogService.
    //-------------------------------
    // Summary:
    //  Starts monitoring of the current thread by a watchdog.
    //--------------------------------------------------------
    mxt_result StartWatching(IN uint64_t uScratchFrequencyMs,
                             IN uint64_t uGracePeriodMs);

    // Summary:
    //  Stops monitoring of the current thread.
    //---------------------------------------------
    void StopWatching();
#endif

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    // Inherited from IPolledRequestStatusMgr.
    //----------------------------------------

    // Summary:
    //  Notifies the manager about a completed request status.
    //---------------------------------------------------------
    virtual void EvPolledRequestStatusMgrEventDetected(IN TRequestStatus* pRequestStatus,
                                                       IN mxt_opaque opq);
#else
    // Inherited from IPolledSocketMgr.
    //----------------------------------

    // Summary:
    //  Notifies the manager about newly detected events on a socket.
    //----------------------------------------------------------------
    virtual void EvPolledSocketMgrEventDetected(IN mxt_hSocket hSocket,
                                                IN unsigned int uEvents,
                                                IN mxt_opaque opq);
#endif // #if defined(MXD_OS_SYMBIAN)
#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

    // Queue notification and timeout mechanism.
    //-------------------------------------------

    // Summary:
    //  Releases the semaphore.
    //--------------------------
    mxt_result Signal();

    // Summary:
    //  Tries to acquire the semaphore with a timeout.
    //-------------------------------------------------
    mxt_result Wait(IN uint64_t uTimeoutMs);

private:

    // Execution Context operations.
    //-------------------------------
    bool IsCurrentExecutionContext(OUT bool* pbAlreadyVisited);
    bool IsCurrentExecutionContextAlreadyVisited();

#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
    // Watched thread registration.
    //-----------------------------
    void InternalRegisterWatchedThread(IN CMarshaler* pParameter);
    void InternalUnregisterWatchedThread();
#endif

    // Timer operations.
    //-------------------

    // Protected is used here because Visual C++ 6.0 generates warnings if the
    // structures are predeclared in a different access mode than they are
    // defined later.
protected:
    struct STimer;
    struct STimerManager;
private:

    void AddTimer(IN const STimer* pstTimer);
    void AddTimerExponential(IN ITimerServiceMgr* pManager,
                             IN unsigned int uTimerId,
                             IN uint64_t uFloorTimeoutMs,
                             IN uint64_t uCeilingTimeoutMs,
                             IN uint8_t uMultBy,
                             IN uint8_t uDivBy,
                             IN bool bStopAtCeiling,
                             IN mxt_opaque opq,
                             IN EPeriodicity ePeriodicty);
    void AddTimerLinear(IN ITimerServiceMgr* pManager,
                              IN unsigned int uTimerId,
                              IN uint64_t uTimeoutMs,
                              IN mxt_opaque opq,
                              IN EPeriodicity ePeriodicity);
    static int CompareTimer(IN const CUncmp<STimer>& rstOneElement,
                            IN const CUncmp<STimer>& rstOtherElement,
                            IN mxt_opaque opq);
    static int CompareTimerManager(IN const CUncmp<STimerManager>& rstOneElement,
                                   IN const CUncmp<STimerManager>& rstOtherElement,
                                   IN mxt_opaque opq);
    STimer* GetNextTimer();
    void PrepareTimerExponential(IN uint64_t uActualTimeMs,
                                 INOUT STimer* pstTimer,
                                 INOUT bool* pbAddTimer);
    void PrepareTimerExponentialUpdateNextTimeout(INOUT STimer* pstTimer,
                                                  OUT bool* pbCeilingWasAlreadyReached);
    void PrepareTimerLinear(IN uint64_t uActualTimeMs,
                            INOUT STimer* pstTimer,
                            INOUT bool* pbAddTimer,
                            INOUT bool* pbNotify);
    void RemoveAllTimers(IN ITimerServiceMgr* pManager);
    void RemoveTimer(IN bool bNotify,
                     IN ITimerServiceMgr* pManager,
                     IN unsigned int uTimerId,
                     IN bool bRemoveTimerManagerOnEmptyList);

    static mxt_result InitializeCServicingThread();
    static void FinalizeCServicingThread();

// Friends declarations
//----------------------
    friend mxt_result InitializeCServicingThread();
    friend void FinalizeCServicingThread();

// Hidden Data Members
//---------------------
protected:

    // Protects the queue and signalisation variables.
    CMutex m_mutex;

    // Semaphore to signal incomming message.
    CBinarySemaphore m_sem;

    // The current thread id associated with the execution context thread.
    // Normally, this will be equal to CThread::GetId(). However, if the
    // execution context is provided via an external thread, this won't be true.
    unsigned int m_uCurrentId;

    // This is the TSD key which may contain the stack of all visited execution
    // context. This is use to detect inter thread synchronous message deadlock.
    static mxt_tsdkey ms_tsdKeyVisitedExecutionContexts;

    unsigned int m_auVisitedExecutionContexts[uMAX_VISITED_EXECUTION_CONTEXTS + 1];

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    // Used to signal incoming message (complete a request status).
    RThread* m_pPollingThread;
    // Manages request status registration/unregistration and completion
    // detection.
    CPollRequestStatus* m_pPollRequestStatus;
#else
    // Used to signal incoming message (unlock the select).
    CUdpSocket* m_pUdpSocket;

    // Manages sockets registration/unregistration and states changes detection.
    CPollSocket* m_pPollSocket;
#endif
#endif

    // Used to optimize the Signal() code.
    // If m_bUnblockPoll is false, no need to send a udp packet to unblock
    // Poll()
    bool m_bUnblockPoll;

    enum EWhoHasPriority
    {
        eMESSAGE_HAS_PRIORITY,
        eTIMER_HAS_PRIORITY
    };

    EWhoHasPriority m_eWhoHasPriority;

    enum EActivationState
    {
        eACTIVATION_NONE,
        eACTIVATION_INTERNAL,
        eACTIVATION_EXTERNAL
    };

    // Used by the Activate method on an external thread activation mode.
    // Exits before the timeout is elapsed when ReleaseIfRef is called
    // and the reference count is down to 1. This allow the caller to know
    // that the CServicingThread could be released.
    EActivationState m_eActivationState;
    bool m_bReferenceCountDownToOne;

    bool m_bTerminating;

    // Message Management.
    //---------------------

    struct SMessage
    {

        unsigned int m_uMessageId;

        IMessageServiceMgr* m_pManager;

        // User is waiting the completion.
        CBinarySemaphore* m_psemWaitCompletion;
        // Must be deleted.
        CMarshaler* m_pParameter;
        // NULL if asynchronous. Thus, the user is not waiting for the
        // completion of the message processing. Not NULL if synchronous.
        // Contain a pointer to a stack of visited execution contexts.
        unsigned int* m_puVisitedExecutionContexts;
    };

    // A CList is chosen because we will always insert to one end and remove at
    // the other.
    CList<SMessage> m_lstMessages;

#if defined(MXD_NETWORK_ENABLE_SUPPORT)
#if defined(MXD_OS_SYMBIAN)
    // Request Status Management.
    //----------------------------

    struct SRequestStatus
    {
        // Optimize the memory used for free and used request status.
        union
        {
            IRequestStatusServiceMgr* m_pManager;
            // Used to chain free request status.
            SRequestStatus* m_pstNext;
        };

        mxt_opaque m_opq;
    };

    // Pointer to the first request status of a list of available
    // SRequestStatus elements.
    SRequestStatus* m_pstFreeRequestStatus;

#else

    // Socket Management.
    //--------------------

    struct SSocket
    {
        // Optimize the memory used for free and used sockets.
        union
        {
            ISocketServiceMgr* m_pManager;
            // Used to chain free sockets.
            SSocket* m_pstNext;
        };

        mxt_opaque m_opq;
    };

    // Pointer to the first socket of a list of available SSocket elements.
    SSocket* m_pstFreeSockets;
#endif // #if defined(MXD_OS_SYMBIAN)
#endif // #if defined(MXD_NETWORK_ENABLE_SUPPORT)

#if defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)
    // Thread watching Management.
    //----------------------------

    // Indicates if the servicing thread is currently watched by
    // the watchdog service.
    bool m_bThreadIsWatched;

    // The Thread ID used by the watchdog monitor to represent this thread.
    unsigned int m_uWatchedThreadId;
#endif

    // Timer Management.
    //-------------------

    //  7    6    5      4             3             2         1       0
    // +----+----+------+-------------+-------------+---------+-------+-------+
    // |    |    | STOP | TIMER       | TIMER       | TIMER   | TIMER | TIMER |
    // |    |    | AT   | PERIODICITY | PERIODICITY | TO BE   | TYPE  | TYPE  |
    // |    |    | CEIL |             |             | REMOVED |       |       |
    // +----+----+------+-------------+-------------+---------+-------+-------+
    //

    // Bits 1 and 0.
    #define uTIMER_TYPE_MASK                                                0x03
    #define uTIMER_TYPE_EXPONENTIAL                                         0x00
    #define uTIMER_TYPE_LINEAR                                              0x01

    // Bit 2.
    // This flag is only used by RemoveAllTimers. It is temporary set to true
    // on all timer to be removed, before sending the notification. If a timer
    // is marked to be removed is stopped and restarted during a notification,
    // it would be removed even if it should now be considered a new timer.
    #define uTIMER_TO_BE_REMOVED_MASK                                       0x04
    #define uTIMER_TO_BE_REMOVED                                            0x04

    // Bit 4 and 3.
    // Inter-increment periodicity
    #define uTIMER_PERIODICITY_MASK                                         0x18
    #define uTIMER_PERIODICITY_NOT_PERIODIC                                 0x00
    #define uTIMER_PERIODICITY_REAJUST_WITH_CURRENT_TIME                    0x08
    #define uTIMER_PERIODICITY_REAJUST_WITH_PREVIOUS_TIME_CYCLE_LOST        0x10
    #define uTIMER_PERIODICITY_REAJUST_WITH_PREVIOUS_TIME_NO_CYCLE_LOST     0x18

    // Only for uTIMER_TYPE_EXPONENTIAL

    // Bit 5.
    #define uTIMER_EXPONENTIAL_STOP_AT_CEILING_MASK                         0x20
    #define uTIMER_EXPONENTIAL_STOP_AT_CEILING                              0x20

    #include "Basic/MxPack.h"

    struct STimerExponential
    {
        uint64_t m_uLastTimeoutMs                                MX_PACK_MEMBER;
        uint64_t m_uCeilingTimeoutMs                             MX_PACK_MEMBER;
        uint8_t m_uMultBy                                        MX_PACK_MEMBER;
        uint8_t m_uDivBy                                         MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    struct STimerLinear
    {
        // Period to wait until the timer elapses.
        uint64_t m_uTimeoutMs                                    MX_PACK_MEMBER;
    } MX_PACK_STRUCT;


    struct STimer
    {
        // A user supplied manager to be notified when timer elapsed or
        // stopped.
        ITimerServiceMgr* m_pManager                             MX_PACK_MEMBER;

        // The next timer in the current manager timer list.
        STimer* m_pstNextManagerTimer                            MX_PACK_MEMBER;

        // The timer identifier.
        unsigned int m_uTimerId                                  MX_PACK_MEMBER;

        // A user supplied value.
        mxt_opaque m_opq                                         MX_PACK_MEMBER;

        // Absolute time of the next timer event.
        uint64_t m_uNextTimeMs                                   MX_PACK_MEMBER;

        union
        {
            STimerExponential m_stTimerExponential               MX_PACK_MEMBER;
            STimerLinear m_stTimerLinear                         MX_PACK_MEMBER;
        };

        // Specific flag described above.
        uint8_t m_uFlags                                         MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    struct STimerManager
    {
        // A user supplied manager to be notified when timer elapsed or stopped.
        ITimerServiceMgr* m_pManager                             MX_PACK_MEMBER;
        STimer* m_pstFirstTimer                                  MX_PACK_MEMBER;
    } MX_PACK_STRUCT;

    #include "Basic/MxPack.h"

    CAATree<CUncmp<STimer> > m_treeTimers;
    CAATree<CUncmp<STimerManager> > m_treeTimerManagers;

    // Internal Management.
    //----------------------

    static unsigned int ms_uServicingThreadCounter;
    static CAlignedVariableStorage<CBinarySemaphore> ms_semFinalizeWait;
    static CAlignedVariableStorage<CMutex> ms_mutexCounterProtect;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

inline
CServicingThread::STimer* CServicingThread::GetNextTimer()
{
      return m_treeTimers.IsEmpty() ? NULL : &m_treeTimers[m_treeTimers.GetFirstIndex()];
}

MX_NAMESPACE_END(MXD_GNS)

#endif // MXD_SERVICING_THREAD_ENABLE_SUPPORT
#endif // MXG_CSERVICINGTHREAD_H

