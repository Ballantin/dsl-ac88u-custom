//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_ITIMERSERVICEMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ITIMERSERVICEMGR_H
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
//== Class: ITimerServiceMgr
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   This is the interface that must be implemented to use ITimerServices.
//
// Description:
//   This is the interface that must be implemented to use ITimerServices.
//
// Location:
//   ServicingThread/ITimerServiceMgr.h
//
// See Also:
//   IActivationService, IMessageService, ISocketService, ITimerServiceMgr
//
//==EDOC========================================================================
class ITimerServiceMgr
{
// Published Interface
//---------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvTimerServiceMgrAwaken
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a new timer elapsed or has been stopped.
    //
    //  Parameters:
    //    bStopped:
    //      True if the timer has been explicitly stopped, false if it normally
    //      elapsed. For an exponential timer, true is also sent if the ceiling
    //      has been reached and bStopAtCeiling is true.
    //
    //    uTimerId:
    //      The identifier of the timer.
    //
    //    opq:
    //      An optional opaque supplied by the caller.
    //
    //  Description:
    //      Notifies the manager that a new timer elapsed.
    //
    //      For non-periodic timers, this method is called once, with bStopped
    //      equal to false.
    //      For example,
    //          EvTimerServiceMgrAwaken(false, uTimerId, opq)
    //
    //      For periodic and exponential timers, this method is called as many
    //      times as the timer elapses + 1. The + 1 means that the timer has
    //      been destroyed.
    //      For example,
    //          EvTimerServiceMgrAwaken(false, uTimerId, opq)
    //          ...
    //          EvTimerServiceMgrAwaken(false, uTimerId, opq) always followed by
    //          EvTimerServiceMgrAwaken(true, uTimerId, opq)
    //
    //      If a timer is stopped with StopTimer or StopAllTimers, this method
    //      is also called with bStopped set to true.
    //
    //==EDOC====================================================================
    virtual void EvTimerServiceMgrAwaken(IN bool bStopped,
                                         IN unsigned int uTimerId,
                                         IN mxt_opaque opq) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  ITimerServiceMgr
    //==
    //==========================================================================
    //
    // Summary:
    //  Constructor.
    //
    // Description:
    //  Constructor.
    //
    //==EDOC====================================================================
    ITimerServiceMgr() {}

    //==SDOC====================================================================
    //==
    //==  ~ITimerServiceMgr
    //==
    //==========================================================================
    //
    // Summary:
    //  Destructior.
    //
    // Description:
    //  Destructior.
    //
    //==EDOC====================================================================
    virtual ~ITimerServiceMgr() {}

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_ITIMERSERVICEMGR_H

