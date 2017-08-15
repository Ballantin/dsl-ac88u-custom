//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//   form whatsoever, without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_CEVENTDRIVEN_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_CEVENTDRIVEN_H
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

//-- Parameters used by Value
//-----------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_CTHREAD_H
#include "Kernel/CThread.h"
#endif

// For Symbian, sockets are not used by CEventDriven
#if !defined(MXD_OS_SYMBIAN)
    #ifndef MXG_SOCKETDEFINES_H
    #include "Network/SocketDefines.h"
    #endif
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IACTIVATIONSERVICE_H
#include "ServicingThread/IActivationService.h"
#endif
#ifndef MXG_IMESSAGESERVICE_H
#include "ServicingThread/IMessageService.h"
#endif
#ifndef MXG_IMESSAGESERVICEMGR_H
#include "ServicingThread/IMessageServiceMgr.h"
#endif
// The Symbian implementation uses request status
// instead of sockets.
#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
    #ifndef MXG_IREQUESTSTATUSERVICE_H
    #include "ServicingThread/IRequestStatusService.h"
    #endif
    #ifndef MXG_IREQUESTSTATUSSERVICEMGR_H
    #include "ServicingThread/IRequestStatusServiceMgr.h"
    #endif
#else
    #ifndef MXG_ISOCKETSERVICE_H
    #include "ServicingThread/ISocketService.h"
    #endif
    #ifndef MXG_ISOCKETSERVICEMGR_H
    #include "ServicingThread/ISocketServiceMgr.h"
    #endif
#endif
#ifndef MXG_ITIMERSERVICE_H
#include "ServicingThread/ITimerService.h"
#endif
#ifndef MXG_ITIMERSERVICEMGR_H
#include "ServicingThread/ITimerServiceMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: CEventDriven
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Base class from which objects that use servicing threads may inherit.
//
// Description:
//   CEventDriven is a base class from which objects that use Servicing Threads
//   may inherit. Its main goal is to simplify the utilization of the
//   IMessageService, ISocketService (IRequestStatusService under Symbian),
//   and ITimerService interfaces.Please note that ISocketService is only
//   available when MXD_NETWORK_ENABLE_SUPPORT is defined.
//
//   One area where Servicing Threads are not trivial to use is related with
//   the releasing of an object that uses the Message Service. Releasing is
//   complicated by the fact that an object may be released from the same
//   execution context as the Servicing Thread, from another one, or may also
//   have messages pending to be processed within the queue. CEventDriven offers
//   a simple template that may be used to get around this complexity.
//
//   Two different releasing methods exist: FinalizeAndReleaseA and Release.
//   FinalizeAndReleaseA should be called when messages that are pending within
//   the queue should be processed normally before the object is deleted.
//   Upon its return, the caller MUST NOT assume that all pending messages have
//   been processed. Calling IsSilent from a IMessageServiceMgr callback always
//   returns false for a call to FinalizeAndRelease.
//
//   Release should be called when messages that are pending within the queue
//   should be silently processed before the object is deleted. Upon its return,
//   from its point of view, the caller MAY assume that all pending messages
//   have been processed and that the object has been released. However, from
//   an internal point of view, it is quite possible that the object is still
//   alive and trying to release itself.
//
//   In all cases, it is expected that an implementation calls IsSilent before
//   any callbacks and notifications are fired.
//
//   It is possible to overload ReleaseInstance if the object must not be
//   deleted. That may be the case if the object must wait for an aynchronous
//   event. In such cases, the default behaviour is not advisable and must be
//   overriden.
//
// Warning:
//   CEventDriven inheriting classes must be allocated on the heap.
//
// Location:
//   ServicingThread/CEventDriven.h
//
// See Also:
//
//==EDOC========================================================================
class CEventDriven : public IMessageServiceMgr,
#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
                     public IRequestStatusServiceMgr,
#elif defined(MXD_NETWORK_ENABLE_SUPPORT)
                     public ISocketServiceMgr,
#endif
                     public ITimerServiceMgr
{
// Published Interface
//---------------------
public:

    // << Constructors  >>
    //--------------------

    // Summary:
    //  Constructor.
    //---------------
    CEventDriven();

    // << Destructors >>
    //------------------

    // Summary:
    //  Destructor.
    //--------------
    virtual ~CEventDriven();

    // Summary:
    //  Associates a Servicing Thread with this Event Driven.
    mxt_result Activate(IN IEComUnknown* pIEComUnknown,
                        IN const char* pszName = NULL,
                        IN uint32_t uStackSize = 0,
                        IN CThread::EPriority ePriority = CThread::eNORMAL);

    // Summary:
    //  Finalizes and releases an Event Driven.
    //------------------------------------------
    void FinalizeAndReleaseA();

    // Summary:
    //  Releases an Event Driven.
    //----------------------------
    void Release();

    // Summary:
    //  Returns a pointer to the Servicing Thread. AddIfRef is already called.
    //-------------------------------------------------------------------------
    IEComUnknown* GetIEComUnknown() const;

    // Summary:
    //  Returns a pointer to the Servicing Thread. AddIfRef is already called.
    //-------------------------------------------------------------------------
    mxt_result GetServicingThread(OUT IEComUnknown** ppEComUnknown) const;

    // Activation Service.
    //---------------------

    // Summary:
    //  Returns whether or not the code is executing within the current
    //  execution context.
    //------------------------------------------------------------------
    bool IsCurrentExecutionContext() const;

    // Message Service.
    //------------------

    // Summary:
    //  Pushes a new message into the message queue.
    //----------------------------------------------
    mxt_result PostMessage(IN bool bWaitCompletion,
                           IN unsigned int uMessage,
                           IN TOS CMarshaler* pParameter = NULL) const;

#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
    // Request Status Service.
    //-------------------------

    // Summary:
    //  Registers a request status.
    //------------------------------
    mxt_result RegisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                     IN mxt_opaque opq = MX_INT32_TO_OPQ(0)) const;

    // Summary:
    //  Enables the detection of request completion.
    //-----------------------------------------------
    mxt_result EnableCompletionDetection(IN TRequestStatus* pRequestStatus) const;

    // Summary:
    //  Disables the detection of request completion.
    //------------------------------------------------
    mxt_result DisableCompletionDetection(IN TRequestStatus* pRequestStatus) const;

    // Summary:
    //  Unregisters a request status.
    //--------------------------------
    mxt_result UnregisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                       OUT mxt_opaque* popq = NULL) const;

#elif defined(MXD_NETWORK_ENABLE_SUPPORT)

    // Socket Service.
    //-----------------

    // Summary:
    //  Registers a socket.
    //----------------------
    mxt_result RegisterSocket(IN mxt_hSocket hSocket,
                              IN mxt_opaque opq = MX_INT32_TO_OPQ(0)) const;

    // Summary:
    //  Enables the detection of events.
    //-----------------------------------
    mxt_result EnableEventsDetection(IN mxt_hSocket hSocket,
                                     IN unsigned int uEvents) const;
    // Summary:
    //  Disables the detection of events.
    //------------------------------------
    mxt_result DisableEventsDetection(IN mxt_hSocket hSocket,
                                      IN unsigned int uEvents) const;
    // Summary:
    //  Unregisters a socket.
    //------------------------
    mxt_result UnregisterSocket(IN mxt_hSocket hSocket,
                                IN mxt_opaque* popq = NULL) const;
#endif // #if defined(MXD_OS_SYMBIAN)

    // Timer Service.
    //----------------

    // Summary:
    //  Starts a new exponential timer.
    //----------------------------------
    mxt_result StartTimer(IN unsigned int uTimer,
                          IN uint64_t uFloorTimeoutMs,
                          IN uint64_t uCeilingTimeoutMs,
                          IN unsigned int uMultBy,
                          IN unsigned int uDivBy,
                          IN bool bStopAtCeiling,
                          IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                          IN ITimerService::EPeriodicity ePeriodicity =
                            ITimerService::ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_NO_CYCLE_LOST) const;

    // Summary:
    //  Starts a new linear timer.
    //-----------------------------
    mxt_result StartTimer(IN unsigned int uTimer,
                          IN uint64_t uTimeoutMs,
                          IN mxt_opaque opq = MX_INT32_TO_OPQ(0),
                          IN ITimerService::EPeriodicity ePeriodicity =
                            ITimerService::ePERIODICITY_NOT_PERIODIC) const;

    // Summary:
    //  Stops a timer owned by a manager.
    //------------------------------------
    mxt_result StopTimer(IN unsigned int uTimer) const;

    // Summary:
    //  Stops all timers owned by a manager.
    //---------------------------------------
    mxt_result StopAllTimers() const;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // Summary:
    //  Determines if the object should be silent.
    bool IsSilent() const;

    // Summary:
    //  By default, this method releases the IActivationService,
    //  IMessageService, ISocketService, and ITimerService interfaces.
    void ReleaseInterfaces();

    // Summary:
    //  By default, this method does a "delete this". It should be overloaded
    //  if the user needs to delay the releasing process.
    virtual void ReleaseInstance();

    // Summary:
    //  By default, this method starts the releasing first steps.
    virtual void ReleasionFirstStep();

    // Summary:
    //  By default, this method calls ReleaseInstance. It should be overloaded
    //  if the user needs to delay the releasing process. From now on, the user
    //  MUST act like it was released. It may still use the Servicing Thread
    //  if it wants.
    virtual void ReleasionSecondStep();

    // Inherited from IMessageServiceMgr.
    //------------------------------------
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessage,
                                           IN CMarshaler* pParameter);

#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
    // Inherited from IRequestStatusServiceMgr.
    //------------------------------------------
    virtual void EvRequestStatusServiceMgrAwaken(IN TRequestStatus* pRequestStatus,
                                                 IN mxt_opaque opq);
#elif defined(MXD_NETWORK_ENABLE_SUPPORT)
    // Inherited from ISocketServiceMgr.
    //-----------------------------------
    virtual void EvSocketServiceMgrAwaken(IN mxt_hSocket hSocket,
                                          IN unsigned int uEvents,
                                          IN mxt_opaque opq);
#endif

    // Inherited from ITimerServiceMgr.
    //----------------------------------
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimer,
                                         IN mxt_opaque opq);

private:

// Hidden Data Members
//---------------------
protected:

    IActivationService* m_pIActivationService;
    IMessageService* m_pIMessageService;
#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
    IRequestStatusService* m_pIRequestStatusService;
#elif defined(MXD_NETWORK_ENABLE_SUPPORT)
    ISocketService* m_pISocketService;
#endif
    ITimerService* m_pITimerService;

    // Are we releasing the Event Driven?
    bool m_bIsSilent;

    // This enum contains the different types of releases that may be called.
    // eALIVE means that the object is alive and quite well. Not in release
    // mode.
    enum EReleasion
    {
        eFINALIZE_AND_RELEASE,
        eRELEASE,
        eALIVE
    };

    EReleasion m_eReleasion;
//M5T_INTERNAL_USE_END
};

//==============================================================================
//====  INLINE FUNCTIONS  ======================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  IsCurrentExecutionContext
//==
//==============================================================================
//
//  Returns:
//      True if it is executing within the current execution context, false
//      if it is not.
//
//  Description:
//      This method returns whether or not the code is executing within the
//      current execution context.
//
//  See also:
//      IActivationService::IsCurrentExecutionContext
//
//==EDOC========================================================================
inline
bool CEventDriven::IsCurrentExecutionContext() const
{
    return m_pIActivationService->IsCurrentExecutionContext();
}

//==SDOC========================================================================
//==
//==  PostMessage
//==
//==============================================================================
//
//  Parameters:
//    bWaitCompletion:
//      True if the message should be processed synchronously, false if the
//      message should be processed asynchronously.
//
//    uMessage:
//      The identifier of the message that must be posted.
//
//    pParameter:
//      An optional CMarshaler parameter. The ownership of the CMarshaler
//      is taken by PostMessage. Ownership is not taken if an error is returned.
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      PostMessage is used to push a new message onto the message queue. The
//      message may be processed synchronously or asynchronously, depending on
//      the value of bWaitCompletion.
//
//      The method EvMessageServiceMgrAwaken MUST be overloaded.
//
//  See also:
//      IMessageService::PostMessage
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::PostMessage(IN bool bWaitCompletion,
                                     IN unsigned int uMessage,
                                     IN TOS CMarshaler* pParameter) const
{
    if (m_pIMessageService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pIMessageService->PostMessage(const_cast<CEventDriven*>(this),
                                           bWaitCompletion,
                                           uMessage,
                                           pParameter);
}

#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)
//==SDOC========================================================================
//==
//==  RegisterRequestStatus
//==
//==============================================================================
//
//  Summary:
//      Registers a request status.
//
//  Parameters:
//    pRequestStatus:
//      The request status that must be registered.
//
//    opq:
//      An opaque value associated with the request status.
//
//  Returns:
//      resFE_INVALID_STATE
//
//  Description:
//      Registers a request status. The registration associates a manager
//      and an optional opaque value with the request status being registered.
//
//==EDOC========================================================================
inline mxt_result CEventDriven::RegisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                                      IN mxt_opaque opq) const
{
    if (m_pIRequestStatusService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pIRequestStatusService->RegisterRequestStatus(pRequestStatus,
                                                           const_cast<CEventDriven*>(this),
                                                           opq);
}


//==SDOC========================================================================
//==
//==  EnableCompletionDetection
//==
//==============================================================================
//
//  Summary:
//      Enables the detection of request completion.
//
//  Parameters:
//    pRequestStatus:
//      The request status for which to enable completion detection.
//
//  Returns:
//      resFE_INVALID_STATE
//
//  Description:
//      Enables the detection of request completion.
//
//==EDOC========================================================================
inline mxt_result CEventDriven::EnableCompletionDetection(IN TRequestStatus* pRequestStatus) const
{
    if (m_pIRequestStatusService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pIRequestStatusService->EnableCompletionDetection(pRequestStatus);
}

//==SDOC========================================================================
//==
//==  DisableCompletionDetection
//==
//==============================================================================
//
//  Summary:
//      Disables the detection of request completion.
//
//  Parameters:
//    pRequestStatus:
//      The request status for which to enable completion detection.
//
//  Returns:
//      resFE_INVALID_STATE
//
//  Description:
//      Disables the detection of request completion.
//
//==EDOC========================================================================
inline mxt_result CEventDriven::DisableCompletionDetection(IN TRequestStatus* pRequestStatus) const
{
    if (m_pIRequestStatusService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pIRequestStatusService->DisableCompletionDetection(pRequestStatus);
}

//==SDOC========================================================================
//==
//==  UnregisterRequestStatus
//==
//==============================================================================
//
//  Summary:
//      Unregisters a request status.
//
//  Parameters:
//    pRequestStatus:
//      The request status that must be unregistered.
//
//    popq:
//      On exit, contains the opaque value associated with the request status.
//      It may be NULL if the value should just be discarded.
//
//  Returns:
//      resFE_INVALID_STATE
//
//  Description:
//      Unregisters a request status for completion detection notification.
//
//==EDOC========================================================================
inline mxt_result CEventDriven::UnregisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                                        OUT mxt_opaque* popq) const
{
    if (m_pIRequestStatusService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pIRequestStatusService->UnregisterRequestStatus(pRequestStatus,
                                                             popq);
}

#elif defined(MXD_NETWORK_ENABLE_SUPPORT)

//==SDOC========================================================================
//==
//==  RegisterSocket
//==
//==============================================================================
//
//  Parameters:
//    hSocket:
//      The identifier of the socket that must be registered.
//
//    opq:
//      An opaque value associated with the socket.
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Registers a socket. The registration associates a manager and an
//      optional opaque value with the socket being registered.
//
//      Method EvSocketServiceMgrAwaken MUST be overloaded.
//
//  See also:
//      ISocketService::RegisterSocket
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::RegisterSocket(IN mxt_hSocket hSocket,
                                        IN mxt_opaque opq) const
{
    if (m_pISocketService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pISocketService->RegisterSocket(hSocket,
                                             const_cast<CEventDriven*>(this),
                                             opq);
}

//==SDOC========================================================================
//==
//==  EnableEventsDetection
//==
//==============================================================================
//
//  Parameters:
//    hSocket:
//      The identifier of the socket.
//
//    uEvents:
//      The events that are to be enabled. Possible events are:
//          uSOCKET_IN_EXCEPTION
//          uSOCKET_READABLE
//          uSOCKET_WRITABLE
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Enables the detection of specific events for a socket. Updates the
//      events to be detected. Only events that are true are enabled. Events
//      that are false are not disabled.
//
//      For example, for a new socket hSocket:
//          EnableEventDetection(hSocket, 0x06);
//          EnableEventDetection(hSocket, 0x01);
//      this results in all events being enabled.
//
//  See also:
//      ISocketService::EnableEventsDetection
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::EnableEventsDetection(IN mxt_hSocket hSocket,
                                               IN unsigned int uEvents) const
{
    if (m_pISocketService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pISocketService->EnableEventsDetection(hSocket, uEvents);
}

//==SDOC========================================================================
//==
//==  DisableEventsDetection
//==
//==============================================================================
//
//  Parameters:
//    hSocket:
//      The identifier of the socket.
//
//    uEvents:
//      The events that are to be disabled. Possible events are:
//          uSOCKET_IN_EXCEPTION
//          uSOCKET_READABLE
//          uSOCKET_WRITABLE
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Disables the detection of specific events for a socket. Updates the
//      events to be detected. Only events that are true are disabled. Events
//      that are false are not enabled.
//
//      For example, for a new socket hSocket:
//          DisableEventDetection(hSocket, 0x06);
//          DisableEventDetection(hSocket, 0x01);
//      this results in all events being disabled.
//
//  See also:
//      ISocketService::DisableEventsDetection
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::DisableEventsDetection(IN mxt_hSocket hSocket,
                                                IN unsigned int uEvents) const
{
    if (m_pISocketService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pISocketService->DisableEventsDetection(hSocket, uEvents);
}

//==SDOC========================================================================
//==
//==  UnregisterSocket
//==
//==============================================================================
//
//  Parameters:
//    hSocket:
//      The identifier of the socket that must be unregistered.
//
//    popq:
//      On exit, contains the opaque value associated with the socket. It MAY be
//      NULL if the value should just be discarded.
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Unregisters a socket for states changes detection notification.
//
//  See also:
//      ISocketService::UnregisterSocket
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::UnregisterSocket(IN mxt_hSocket hSocket,
                                          IN mxt_opaque* popq) const
{
    if (m_pISocketService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pISocketService->UnregisterSocket(hSocket, popq);
}
#endif // #if defined(MXD_OS_SYMBIAN)

//==SDOC========================================================================
//==
//==  StartTimer
//==
//==============================================================================
//
//  Parameters:
//    uTimer:
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
//          If false, the timer continues to fire at uCeilingTimeoutMs.
//
//    opq:
//      An optional opaque that may be supplied by the caller.
//
//    ePeriodicity:
//      The type of periodicity attached to the timer.
//      It this case, ePERIODICITY_NOT_PERIODIC is invalid and the
//      periodicity is related only to the inter-increment steps.
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Starts a new timer owned by the provided manager. Allows the creation of
//      exponential timers. An existing timer is first stopped as if StopTimer
//      had been called before it is started again.
//
//      Control over the timeout increment is given. It is also possible to
//      specify the behaviour when the ceiling timeout is reached.
//
//      For example, let's suppose a timer that starts at the floor timeout of
//      500 ms, increases exponentially by a factor of 2, and reaches its
//      ceiling at 32000 ms is required.
//      500, 1000, 2000, 4000, 8000, 16000, 32000, 32000, 32000, 32000...
//
//      <CODE>
//      mxt_result res =
//          StartTimer(pMagager, uTimer, 500, 32000, 2, 1, false);
//      </CODE>
//
//  See also:
//      ITimerService::StartTimer
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::StartTimer(IN unsigned int uTimer,
                                    IN uint64_t uFloorTimeoutMs,
                                    IN uint64_t uCeilingTimeoutMs,
                                    IN unsigned int uMultBy,
                                    IN unsigned int uDivBy,
                                    IN bool bStopAtCeiling,
                                    IN mxt_opaque opq,
                                    IN ITimerService::EPeriodicity ePeriodicity) const
{
    if (m_pITimerService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pITimerService->StartTimer(const_cast<CEventDriven*>(this),
                                        uTimer,
                                        uFloorTimeoutMs,
                                        uCeilingTimeoutMs,
                                        uMultBy,
                                        uDivBy,
                                        bStopAtCeiling,
                                        opq,
                                        ePeriodicity);
}

//==SDOC========================================================================
//==
//==  StartTimer
//==
//==============================================================================
//
//  Parameters:
//    uTimer:
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
//      Starts a new timer owned by the provided manager. This may be a periodic
//      or a one shot timer. An existing timer is first stopped as if StopTimer
//      had been called before it is started again.
//
//      Once uTimeoutMs is elapsed, the manager gets notified. If the timer is
//      periodic, it then gets automatically restarted.
//
//      The method EvTimerServiceMgrAwaken MUST be overloaded.
//
//  See also:
//      ITimerService::StartTimer
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::StartTimer(IN unsigned int uTimer,
                                    IN uint64_t uTimeoutMs,
                                    IN mxt_opaque opq,
                                    IN ITimerService::EPeriodicity ePeriodicity) const
{
    if (m_pITimerService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pITimerService->StartTimer(const_cast<CEventDriven*>(this),
                                        uTimer,
                                        uTimeoutMs,
                                        opq,
                                        ePeriodicity);
}

//==SDOC========================================================================
//==
//==  StopTimer
//==
//==============================================================================
//
//  Parameters:
//    uTimer:
//      The timer that must be stopped.
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      Stops a timer. This call is simply ignored if the timer is non-existent.
//
//      The manager EvTimerServiceMgrAwaken method is called with bStopped equal
//      to true if the timer exists.
//
//  See also:
//      ITimerService::StopTimer
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::StopTimer(IN unsigned int uTimer) const
{
    if (m_pITimerService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return  m_pITimerService->StopTimer(const_cast<CEventDriven*>(this), uTimer);
}

//==SDOC========================================================================
//==
//==  StopAllTimers
//==
//==============================================================================
//
//  Returns:
//      resFE_INVALID_ARGUMENT
//
//  Description:
//      This method stops all timers that are owned by a manager.
//
//      The manager EvTimerServiceMgrAwaken method is called with bStopped equal
//      to true for each stopped timer.
//
//  See also:
//      ITimerService::StopAllTimers
//
//==EDOC========================================================================
inline
mxt_result CEventDriven::StopAllTimers() const
{
    if (m_pITimerService == NULL)
    {
        return resFE_INVALID_STATE;
    }

    return m_pITimerService->StopAllTimers(const_cast<CEventDriven*>(this));
}

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//==
//==  IsSilent()
//==
//==============================================================================
//
//  Summary:
//      Returns whether or not the Event Driven is silent.
//
//  Returns:
//      True if it must be silent, false if it may do notification.
//
//  Description:
//      This method should be called before doing anything that would show the
//      Event Driven as being alive. For example, it should be called before
//      sending a notification to a manager.
//
//==EDOC========================================================================
inline
bool CEventDriven::IsSilent() const
{
    return m_bIsSilent;
}
//M5T_INTERNAL_USE_END
MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)
#endif // MXG_CEVENTDRIVEN_H

