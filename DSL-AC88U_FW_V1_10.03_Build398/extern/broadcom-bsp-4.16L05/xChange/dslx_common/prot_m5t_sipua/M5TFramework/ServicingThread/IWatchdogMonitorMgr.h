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
#ifndef MXG_IWATCHDOGMONITORMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IWATCHDOGMONITORMGR_H
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


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: IWatchdogMonitorMgr
//==============================================================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   This is the interface through which thread monitoring events are
//   reported to the application.
//
// Description:
//   This is the interface through which thread monitoring events are
//   reported to the application.
//
// Location:
//   ServicingThread/IWatchdogMonitorMgr.h
//
// See also:
//   IWatchdogService.
//
//==============================================================================
class IWatchdogMonitorMgr
{
//-- Published Interface
public:

    //==========================================================================
    //==
    //==  EvStartWatchingThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the watchdog monitor started monitoring a thread.
    //
    //  Parameters:
    //    uThreadId:
    //      The thread ID of the monitored thread.
    //
    //    nOsThreadId:
    //      The thread ID given by the operating system to the monitored thread.
    //      Can be zero when it was not possible to obtain this ID.
    //
    //    pszThreadName:
    //      The thread name of the monitored thread. Can be NULL if no name
    //      was given when the thread was created.
    //
    //  Description:
    //      This event is reported when the watchdog monitor starts monitoring
    //      a thread.
    //
    //==========================================================================
    virtual void EvStartWatchingThread(IN unsigned int uThreadId,
                                       IN int nOsThreadId,
                                       IN const char* pszThreadName) = 0;

    //==========================================================================
    //==
    //==  EvStopWatchingThread
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that the watchdog monitor stopped monitoring a thread.
    //
    //  Parameters:
    //    uThreadId:
    //      The thread ID of the monitored thread.
    //
    //    nOsThreadId:
    //      The thread ID given by the operating system to the monitored thread.
    //      Can be zero when it was not possible to obtain this ID.
    //
    //    pszThreadName:
    //      The thread name of the monitored thread. Can be NULL if no name
    //      was given when the thread was created.
    //
    //  Description:
    //      This event is reported when the watchdog monitor stops monitoring
    //      a thread.
    //
    //==========================================================================
    virtual void EvStopWatchingThread(IN unsigned int uThreadId,
                                      IN int nOsThreadId,
                                      IN const char* pszThreadName) = 0;

    //==========================================================================
    //==
    //==  EvThreadDead
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports that a thread stopped signaling its presence
    //      to the watchdog monitor.
    //
    //  Parameters:
    //    uThreadId:
    //      The thread ID of the monitored thread.
    //
    //    nOsThreadId:
    //      The thread ID given by the operating system to the monitored thread.
    //      Can be zero when it was not possible to obtain this ID.
    //
    //    pszThreadName:
    //      The thread name of the monitored thread. Can be NULL if no name
    //      was given when the thread was created.
    //
    //  Description:
    //      This event is reported when a thread stopped signaling
    //      its presence to the watchdog monitor.
    //
    //      This can happen when a thread is deadlocked or when a thread is
    //      just late scratching the watchdog monitor (because something is
    //      slowing it). When a thread is deadlocked, it is probably not
    //      possible for the application to terminate normally just by releasing
    //      the thread because the release process will block (the thread is
    //      deadlocked).
    //
    //==========================================================================
    virtual void EvThreadDead(IN unsigned int uThreadId,
                              IN int nOsThreadId,
                              IN const char* pszThreadName) = 0;


//M5T_INTERNAL_USE_BEGIN
//-- Hidden Methods
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    IWatchdogMonitorMgr(){};

    // Summary:
    //  Destructor.
    virtual ~IWatchdogMonitorMgr(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    IWatchdogMonitorMgr(IN const IWatchdogMonitorMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    IWatchdogMonitorMgr& operator=(IN const IWatchdogMonitorMgr& rSrc);


//-- Hidden Data Members
protected:
private:
//M5T_INTERNAL_USE_END
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif  //#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT) &&
        //    defined(MXD_SERVICING_THREAD_WATCHDOG_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_IWATCHDOGMONITORMGR_H

