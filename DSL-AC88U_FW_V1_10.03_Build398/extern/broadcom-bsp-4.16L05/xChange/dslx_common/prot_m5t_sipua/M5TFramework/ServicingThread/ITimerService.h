//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ITIMERSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ITIMERSERVICE_H
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
#endif

#if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------
class ITimerServiceMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ITimerService);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ITimerService
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Provides support for starting and stopping timers.
//
// Description:
//   The timer service provides support for starting and stopping timers. Two
//   different types of timers currently exist: linear and exponential.
//
//   A linear timer always fires after a fixed timeout. A linear timer may be
//   single shot or multiple shot, depending on its associated periodicity type.
//
//   An exponential timer fires with an ever increasing timeout controlled by a
//   configured (uMultBy, uDivBy) ratio. It first starts from a configured floor
//   timeout and exponentially increases up to a configured ceiling timeout.
//   A configured bStopAtCeilingOnce determines what happens when the ceiling
//   timeout is reached. If true, the timer is stopped. If false, it continues
//   to be fired but with a constant timeout. This constant timeout is equal to
//   the last timeout calculated (exponentially) prior to reaching the ceiling.
//
//   An optional opaque may be configured when the StartTimer is called. This
//   opaque is sent back to the user through the manager notification. The user
//   is free to use whatever value. The service does not try to interpret it.
//
//   Stopping a timer triggers a notification to the manager, indicating that
//   the timer has been stopped. A notification is also triggered when a timer
//   elapsed.
//
//   Notifications are sent to the manager through the method
//   ITimerServiceMgr::EvTimerServiceMgrAwaken. The manager must overload this
//   method to receive notifications.
//
// Location:
//   ServicingThread/ITimerService.h
//
// See Also:
//   IActivationService, IMessageService, ISocketService
//
//==EDOC========================================================================
class ITimerService : public IEComUnknown
{
// Published Enum
//----------------
public:
    //<GROUP SERVICING_THREAD_ENUMS>
    //
    // Summary:
    // Defines the possible periodicity of a timer.
    //
    // Description:
    // Defines the possible periodicity types that a timer may have.
    enum EPeriodicity
    {
        // Description:
        // The timer is not periodic. Not available to exponential timers.
        ePERIODICITY_NOT_PERIODIC,

        // Description:
        // The next notification will occur at (system timer plus timeout).
        ePERIODICITY_REAJUST_WITH_CURRENT_TIME,

        // Description:
        // The next notification will occur at (previous notification plus
        // timeout). If the next computed notification is already in the past,
        // timeout is added once or more until the computed notification is in
        // the future.
        //
        // For example: suppose that a timer was started at current time equal
        // to 0ms with a timeout equal to 10ms. Suppose also that the previous
        // notification occured at 30ms and the current time is now 53ms, then
        // the next computed notification will be equal to 60ms. Thus, the
        // timer will sleep for 7ms.
        ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_CYCLE_LOST,

        // Description:
        // The next notification will occur at (previous notification plus
        // timeout).
        //
        // For example: suppose that a timer was started at current time equal
        // to 0ms with a timeout equal to 10ms. Suppose also that the previous
        // notification occured at 30ms and the current time is now 53ms, then
        // the notification will be done twice sequentially, once for 40ms and
        // another for 50ms. The next cumputed notification will be equal to
        // 60ms. Thus, the timer will sleep for 7ms.
        ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_NO_CYCLE_LOST
    };

// Published Interface
//---------------------
public:

    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ITimerService);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  StartTimer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts a new exponential timer.
    //
    //  Parameters:
    //    pManager:
    //      The manager that owns the timer.
    //
    //    uTimerId:
    //      The identifier of the timer that must be started.
    //
    //    uFloorTimeoutMs:
    //      The initial timeout value when the timer is started.
    //
    //    uCeilingTimeoutMs:
    //      The last timeout value, which will never be passed.
    //
    //    uMultBy:
    //      The last timeout value is multiplied by uMultBy and divided by
    //      uDivBy to provide control over the variation of the timeout.
    //
    //    uDivBy:
    //      The last timeout value is multiplied by uMultBy and divided by
    //      uDivBy to provide control over the variation of the timeout.
    //
    //    bStopAtCeiling:
    //      Specifies what happens when the timeout ceiling is reached.
    //          If true, the timer is stopped automatically.
    //          If false, the timer continuee to fire at uCeilingTimeoutMs.
    //
    //    opq:
    //      An optional opaque that may be supplied by the caller.
    //
    //    ePeriodicity:
    //      The type of periodicity attached to the timer. In this case,
    //      ePERIODICITY_NOT_PERIODIC is invalid and the periodicity is related
    //      only to the inter-increment steps.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Starts a new timer that is owned by the provided manager. Allows the
    //      creation of exponential timers. An existing timer is first stopped
    //      as if StopTimer had been called before it is started again.
    //
    //      Control over the timeout increment is given. It is also possible to
    //      specify the behaviour when the ceiling timeout is reached.
    //
    //      For example, suppose that a timer that starts at the floor timeout
    //      of 500 ms, increases exponentially by a factor of 2, and reaches its
    //      ceiling at 32000 ms is required. 500, 1000, 2000, 4000, 8000, 16000,
    //      32000, 32000, 32000, 32000...
    //
    //      <CODE>
    //      mxt_result res =
    //          StartTimer(pMagager, uTimer, 500, 32000, 2, 1, false);
    //      </CODE>
    //
    //==EDOC====================================================================
    virtual mxt_result StartTimer(IN ITimerServiceMgr* pManager,
                                  IN unsigned int uTimerId,
                                  IN uint64_t uFloorTimeoutMs,
                                  IN uint64_t uCeilingTimeoutMs,
                                  IN unsigned int uMultBy,
                                  IN unsigned int uDivBy,
                                  IN bool bStopAtCeiling,
                                  IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                                  IN EPeriodicity ePeriodicity =
                                    ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_NO_CYCLE_LOST) = 0;

    //==SDOC====================================================================
    //==
    //==  StartTimer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Starts a new linear timer.
    //
    //  Parameters:
    //    pManager:
    //      The manager that owns the timer.
    //
    //    uTimerId:
    //      The identifier of the timer that must be started.
    //
    //    uTimeoutMs:
    //      The timeout before the timer elapses.
    //
    //    opq:
    //      An optional opaque that may be supplied by the caller.
    //
    //    ePeriodicity:
    //      The type of periodicity attached to the timer.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Starts a new timer that is owned by the provided manager. This may
    //      be a periodic or a one shot timer. An existing timer is first
    //      stopped as if StopTimer had been called before it is started again.
    //
    //      Once uTimeoutMs is elapsed, the manager gets notified. If the timer
    //      is periodic, it then gets automatically restarted.
    //
    //==EDOC====================================================================
    virtual mxt_result StartTimer(IN ITimerServiceMgr* pManager,
                                  IN unsigned int uTimerId,
                                  IN uint64_t uTimeoutMs,
                                  IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                                  IN EPeriodicity ePeriodicity = ePERIODICITY_NOT_PERIODIC) = 0;

    //==SDOC====================================================================
    //==
    //==  StopTimer
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stops a timer owned by a manager.
    //
    //  Parameters:
    //    pManager:
    //      The manager that owns the timers.
    //
    //    uTimerId:
    //      The timer that must be stopped.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Stops a timer owned by a manager. This call is simply ignored if the
    //      timer is inexistent.
    //
    //      The manager EvTimerServiceMgrAwaken method is called with bStopped
    //      equal to true if the timer exists.
    //
    //==EDOC====================================================================
    virtual mxt_result StopTimer(IN ITimerServiceMgr* pManager,
                                 IN unsigned int uTimerId) = 0;

    //==SDOC====================================================================
    //==
    //==  StopAllTimers
    //==
    //==========================================================================
    //
    //  Summary:
    //      Stops all timers owned by a manager.
    //
    //  Parameters:
    //    pManager:
    //      The manager that owns the timers.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      This method stops all timers that are owned by a manager.
    //      The manager EvTimerServiceMgrAwaken method is called with bStopped
    //      equal to true for each stopped timer.
    //
    //==EDOC====================================================================
    virtual mxt_result StopAllTimers(IN ITimerServiceMgr* pManager) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    ITimerService() {}
    virtual ~ITimerService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_ITIMERSERVICE_H

