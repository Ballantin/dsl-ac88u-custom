//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003 Mediatrix Telecom, Inc. ("Mediatrix")
//     Copyright(c) 2003-200% M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Mediatrix and M5T.
//
//   Mediatrix and M5T reserves all rights to this document as well as to the
//   Intellectual Property of the document and the technology and know-how that
//   it includes and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever, without prior written approval by Mediatrix and M5T.
//
//   Mediatrix and M5T reserves the right to revise this publication and make
//   changes at any time and without the obligation to notify any person and/or
//   entity of such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_MXEVENTNOTIFIER_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_MXEVENTNOTIFIER_H
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
#include "Config/FrameworkCfg.h" // MXD_EVENT_NOTIFIER_ENABLE_SUPPORT
#endif

#if defined(MXD_EVENT_NOTIFIER_ENABLE_SUPPORT)

//-- Data Members
//-----------------

//-- Parameters used by Value
//-----------------------------

//-- Interface Realized and/or Parent
//-------------------------------------

//M5T_INTERNAL_USE_BEGIN
//==SDOC========================================================================
//== MxEventNotifier
//========================================
//
// Description:
//   Manages a set of event observers. These observers are notified when
//   someone calls the function NotifyEventObservers to indicate that an event
//   happened.
//
//   A callback must be registered by using the function RegisterEventObserver
//   to start receiving events.
//
// Location:
//   Basic/MxEventNotifier.h
//
//==EDOC========================================================================
//M5T_INTERNAL_USE_END

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


//==SDOC========================================================================
//<GROUP BASIC_ENUMS>
//
// Summary:
//   Events that can be observed.
//
// Description:
//   Enumeration of events that can be observed by an external observer.
//
//==EDOC========================================================================
enum EEventNotifier
{
    // Description:
    //  This event is generated when the network abstraction layer is unable
    //  to resolve the target FQDN name.
    //
    //  The parameter to this event is the FQDN that was not resolved:
    //  <C>"... = const char* pszFqdn"</C>
    //--------------------------------------------------------------------------
    eEN_FQDN_NOT_RESOLVED,

    // Description:
    //  This event is generated when CMemoryAllocator is unable to allocate a
    //  block of memory.
    //
    //  The parameter is set to NULL.
    //--------------------------------------------------------------------------
    eEN_CMEMORYALLOCATOR_OUT_OF_MEMORY,

    // Description:
    //  This event is generated when CMemoryAllocator reaches a certain amount
    //  of allocated memory.
    //
    //  The parameter is set to the value of the allocated memory, in bytes.
    //--------------------------------------------------------------------------
    eEN_CMEMORYALLOCATOR_TRACKING,

    // Description:
    //  This is the last event. This is never generated and is kept only as an
    //  index.
    //--------------------------------------------------------------------------
    eEN_LAST_EVENT
};

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//==
//==  mxt_pfnEventObserver
//==
//==============================================================================
//<GROUP BASIC_TYPES>
//
//  Summary:
//   Definition of the callback function format.
//
//  Parameters:
//   pContext:
//    The context associated with the observer.
//   eEvent:
//    The event to observe.
//   args:
//    Arguments passed to the observer, depending on the event being observed.
//
//  Description:
//   Definition of the callback function format.
//
//==EDOC========================================================================
typedef void (* mxt_pfnEventObserver)(IN void* pContext,
                                      IN EEventNotifier eEvent,
                                      IN va_list args);


// Summary:
//  Registers an observer.
//--------------------------
void RegisterEventObserver(IN mxt_pfnEventObserver pfnEventObserver, IN void* pContext = NULL);

// Summary:
//  Unregisters an observer.
//--------------------------
void UnregisterEventObserver(IN mxt_pfnEventObserver pfnEventObserver, IN void* pContext = NULL);

// Summary:
//  Notifies observers.
//--------------------------
void NotifyEventObservers(IN EEventNotifier eEvent, ...);

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_EVENT_NOTIFIER_ENABLE_SUPPORT)

#endif // MXG_MXEVENTNOTIFIER_H

