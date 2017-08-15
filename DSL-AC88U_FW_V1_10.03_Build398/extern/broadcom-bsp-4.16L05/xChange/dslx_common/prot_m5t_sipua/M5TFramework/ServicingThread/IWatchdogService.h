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
#ifndef MXG_IWATCHDOGSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IWATCHDOGSERVICE_H
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
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

//-- Interface Realized and/or Parent
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IWatchdogService);
//M5T_INTERNAL_USE_END

//==============================================================================
//== Class: IWatchdogService
//==============================================================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   ECom interface used to start and stop the thread monitoring mechanism.
//
// Description:
//   ECom interface used to start and stop the thread monitoring mechanism.
//
// Location:
//   ServicingThread/IWatchdogService.h
//
// See Also:
//   IWatchdogMonitor
//
//==============================================================================
class IWatchdogService : public IEComUnknown
{
//-- Published Interface
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IWatchdogService);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  StartWatching
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts monitoring of the current thread by a watchdog.
    //
    //  Parameters:
    //      uScratchFrequencyMs:
    //       Frequency to scratch the watchdog in milliseconds.
    //       When the frequency is not specified, the default scratch
    //       frequency value is 10 seconds.
    //
    //      uGracePeriodMs:
    //       This is the number of milliseconds that the watchdog monitor can
    //       tolerate for this thread to be "late". This means that the watchdog
    //       monitor expects to be scratched by this thread before the scratch
    //       interval (ScratchFrequencyMs + GracePeriodMs) expires.  Such a
    //       grace period allows to avoid race conditions when the real scratching
    //       frequency is slightly higher than the expected scratching delay.  The
    //       default value for this grace period is 5 seconds.
    //
    //  Returns:
    //      - resS_OK: Success.
    //      - resFE_INVALID_STATE: The thread is already watched.
    //      - Other return codes: Failure.
    //
    //  Description:
    //      Starts the monitoring of this thread by the watchdog monitor and
    //      expects it to scratch the watchdog each uScratchFrequencyMs
    //      milliseconds.
    //
    //      To stop monitoring of the thread, the recommended usage is to
    //      call StopWatching, so each call to StartWatching is paired
    //      with a call to StopWatching.
    //
    //  See Also:
    //      StopWatching
    //
    //==========================================================================
    virtual mxt_result StartWatching(IN uint64_t uScratchFrequencyMs = 10 * 1000,
                                     IN uint64_t uGracePeriodMs = 5 * 1000) = 0;

    //==========================================================================
    //==
    //==  StopWatching
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stops monitoring the thread.
    //
    //  Description:
    //      Indicates to the watchdog monitor to stop monitoring this thread.
    //
    //      This method must be called to stop the monitoring started using
    //      StartWatching. If StopWatching is not called, the monitoring will
    //      be automatically stopped when the thread is released.
    //
    //  See Also:
    //      StartWatching
    //
    //==========================================================================
    virtual void StopWatching() = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    //-- << Constructors / Destructors / Operators >>
    IWatchdogService() {}
    virtual ~IWatchdogService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif  //#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif //-- #ifndef MXG_IWATCHDOGSERVICE_H
