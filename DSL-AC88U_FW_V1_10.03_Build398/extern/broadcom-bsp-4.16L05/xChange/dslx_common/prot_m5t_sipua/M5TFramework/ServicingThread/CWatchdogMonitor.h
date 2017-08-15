//==============================================================================
//==============================================================================
//
//   Copyright(c) 2011 Media5 Corporation ("Media5")
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
//==============================================================================
#ifndef MXG_CWATCHDOGMONITOR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CWATCHDOGMONITOR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//-- M5T Framework Configuration
#ifndef MXG_FRAMEWORKCFG_H
#include "Config/FrameworkCfg.h" // MXD_SERVICING_THREAD_ENABLE_SUPPORT
                                 // MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) && \
    defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)

#ifndef MXG_CEVENTDRIVEN_H
#include "ServicingThread/CEventDriven.h"
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

#ifndef MXG_CMUTEX_H
#include "Kernel/CMutex.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
class CBinarySemaphore;
class IWatchdogMonitorMgr;

//==============================================================================
//== Class: CWatchdogMonitor
//==============================================================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   This class implements the watchdog monitor.
//
// Description:
//   The watchdog monitor allows to watch threads in order to know if they are
//   still up and running.
//
//   To be monitored, a thread must register to the watchdog monitor by
//   calling the RegisterThread method. To indicate that it is up and running,
//   each thread must call the Scratch method at a regular interval.
//
//   When the watchdog monitor finds that a thread hasn't scratched the watchdog
//   in the expected delay then the monitor sends an event to the registered
//   observers (the application) to notify that a thread has failed. Upon
//   reception of this event, the application can take the appropriate action.
//
//   Here is an example of a basic scenario :
//
//   <CODE>
//
//      mxt_result res = resS_OK;
//
//      // CWatchdogObserver realizes IWatchdogMonitorMgr.
//      CWatchdogObserver WatchdogObs;
//
//      // Registers the observer to the watchdog monitor. It will receive
//      // events about all threads registered to the watchdog monitor.
//      res = CWatchdogMonitor::GetInstance()->RegisterObserver(&WatchdogObs);
//
//      // Create a servicing thread.
//      IActivationService* pNewSvcThread = NULL;
//      res = CreateEComInstance(CLSID_CServicingThread,
//                               NULL,
//                               OUT &pNewSvcThread);
//      MX_ASSERT(MX_RIS_S(res));
//
//      res = pNewSvcThread->Activate("Thread name");
//      MX_ASSERT(MX_RIS_S(res));
//
//      // Query the IWatchdogService interface and use it to Start and Stop
//      // monitoring of the thread. The pointer pWatchingSvc will be NULL if
//      // watchdog monitoring is not enabled
//      // (see macro MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT).
//      IWatchdogService* pWatchdogSvc = NULL;
//      res = pNewSvcThread->QueryIf(&pWatchdogSvc);
//
//      if (MX_RIS_S(res))
//      {
//          // Start monitoring of the thread, which will scratch the watchdog
//          // every 20 seconds.  Events about the thread will be sent to the
//          // observer (WatchdogObs).
//          res = pWatchdogSvc->StartWatching(20 * 10000);
//      }
//
//      Do something....
//
//      if (pWatchdogSvc != NULL)
//      {
//          // Stop monitoring of the thread by the watchdog monitor.
//          pWatchdogSvc->StopWatching();
//
//          // Release reference.
//          pWatchdogSvc->ReleaseIfRef();
//          pWatchdogSvc = NULL;
//      }
//
//      // Release thread instance.
//      pNewSvcThread->ReleaseIfRef();
//      pNewSvcThread = NULL;
//
//      // Unregister the observer.
//      CWatchdogMonitor::GetInstance()->UnregisterObserver(&WatchdogObs);
//
//   </CODE>
//
// Location:
//   CServicingThread/CWatchdogMonitor.h
//
// See Also:
//   IWatchdogService
//
//==============================================================================
class CWatchdogMonitor : public CEventDriven
{
//-- << Friends declarations >>
    friend mxt_result InitializeCWatchdogMonitor();
    friend void FinalizeCWatchdogMonitor();

//-- New types within class namespace
private:

    // Structure representing a monitored thread.
    struct SWatchedThread
    {
        // The unique id of the thread.
        unsigned int m_uThreadId;

        // The ID of the thread given by the operating system.
        int m_nOsThreadId;

        // The name of the thread. Can be empty if no name was given when the
        // thread was created;
        CString m_strThreadName;

        // The maximum scratching interval tolerated for the thread, after
        // which it is considered late.
        uint64_t m_uMaxScratchIntervalMs;

        // The ID of the timer for the monitored thread.
        unsigned int m_uTimerId;

        // Indicates that the thread has been declared dead.
        bool m_bThreadDead;
    };

    // Message Ids.
    enum EMessageId
    {
        eMSG_REGISTER_OBSERVER,
        eMSG_UNREGISTER_OBSERVER,
        eMSG_SEND_EV_START_WATCHING,
        eMSG_SEND_EV_STOP_WATCHING,
    };

    // Ids of possible event to send to observers.
    enum EEventId
    {
        eEVENT_START_WATCHING,
        eEVENT_STOP_WATCHING,
        eEVENT_THREAD_DEAD
    };

    enum ETimer
    {
        // Timer used to update the current counter of the monitored threads.
        eCOUNTER_UPDATE_TIMER,
    };

//-- Published Interface
public:

    //-- << Public methods >>

    // Summary:
    //  Returns the unique instance of the CWatchdogMonitor class.
    static CWatchdogMonitor* GetInstance();

    // Summary:
    //   Registers a watchdog monitor observer to be notified.
    mxt_result RegisterObserver(IN IWatchdogMonitorMgr* pWatchdogMonitorMgr);

    // Summary:
    //  Unregisters a watchdog monitor observer.
    void UnregisterObserver(IN IWatchdogMonitorMgr* pWatchdogMonitorMgr);

    // Summary:
    //  Registers a thread to be monitored.
    mxt_result RegisterThread(IN unsigned int uThreadId,
                              IN int nOsThreadId,
                              IN const char* pszThreadName,
                              IN uint64_t uScratchFrequencyMs,
                              IN uint64_t uGracePeriodMs);

    //  Summary:
    //   Unregisters a monitored thread.
    void UnregisterThread(IN unsigned int uThreadId);

    //  Summary:
    //   Scratches the watchdog for the thread.
    mxt_result Scratch(IN unsigned int uThreadId);

//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>
    CWatchdogMonitor();
    virtual ~CWatchdogMonitor();

    //-- << CEventDriven >>

    void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                   IN unsigned int uMessageId,
                                   IN CMarshaler* pParameter);

    //  Summary:
    //   Notifies that a new timer elapsed or has been stopped.
    void EvTimerServiceMgrAwaken(IN bool bStopped,
                                 IN unsigned int uTimer,
                                 IN mxt_opaque opq);

    // Summary:
    //  Initializes the watchdog monitor.
    mxt_result Initialize();

    // Summary:
    //  Finalizes the watchdog monitor.
    void Finalize();

    //-- << Startup mechanism >>
    static mxt_result InitializeCWatchdogMonitor();
    static void FinalizeCWatchdogMonitor();

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    CWatchdogMonitor(IN const CWatchdogMonitor& rSrc);
    CWatchdogMonitor& operator=(IN const CWatchdogMonitor& rSrc);

    //-- << Helpers >>

    // Summary:
    //  Gets the index, from the m_vecstThreads vector, of the SWatchedThread
    //  structure associated with the thread id parameter.
    unsigned int GetThreadIndexByThreadId(IN unsigned int uThreadId) const;

    // Summary:
    //  Gets the index, from the m_vecstThreads vector, of the SWatchedThread
    //  structure associated with the timer id parameter.
    unsigned int GetThreadIndexByTimerId(IN unsigned int uTimerId) const;

    // Summary:
    //  Register an observer.
    void InternalRegisterObserver(IN CMarshaler* pParameter);

    // Summary:
    //  Unregister an observer.
    void InternalUnregisterObserver(IN CMarshaler* pParameter);

    // Summary:
    //  Sends an event to the watchdog monitor observers.
    void SendEventToObservers(IN EEventId eEventId,
                              IN unsigned int uThreadId,
                              IN int nOsThreadId,
                              IN CString& rstrThreadName);

    // Summary:
    //  Post a message for an event to send to the observers.
    void PostMessageForEvent(IN EMessageId eMessageId,
                             IN unsigned int uThreadId,
                             IN int nOsThreadId,
                             IN CString& rstrThreadName);

//-- Hidden Data Members
private:

    static CWatchdogMonitor* ms_pInstance;

    // Description of the watched (monitored) threads.
    CVector<SWatchedThread> m_vecstThreads;

    // Timer ID to use to monitor the threads.
    unsigned int m_uTimerId;

    // List of observers registered to the watchdog monitor. These observers
    // will receive events about the monitored threads.
    CVector<IWatchdogMonitorMgr*> m_vecpObservers;

    // Thread protection for data members related to thread monitoring.
    CMutex m_mutexMonitoringAccess;

    // Number of milliseconds that the watchdog monitor can tolerate to wait
    // for a scratch from a thread with a "scratch interval" expired.
    uint64_t m_uScratchGracePeriodMs;

    // This semaphore is necessary to wait for the registered threads to
    // unregister before finalizing the watchdog monitor.
    CBinarySemaphore* m_psemFinalizationWait;

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif  //#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
        //    defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_CWATCHDOGMONITOR_H

