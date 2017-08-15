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
#ifndef MXG_IMESSAGESERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IMESSAGESERVICE_H
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
class CMarshaler;
class IMessageServiceMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IMessageService);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IMessageService
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Interface providing support for posting messages to a shared message queue.
//
// Description:
//   The message queuing service provides support for posting messages to a
//   shared message queue. The message is then processed within the execution
//   context. It eventually generates an event notification to the associated
//   manager. When a message is posted within the message queue, it is possible
//   for the user to specify whether the message should be processed
//   synchronously or asynchronously. The user is also allowed to attach an
//   optional CMarshaler reference to the message it is posting. The reference
//   is given back to the manager upon notification. The CMarshaler is used to
//   store additional parameters that are required to process the message.
//
//   Message queuing works differently for messages that must be processing
//   synchronously or asynchronously depending on the execution context from
//   which the message is posted. For asynchronous messages, the message is
//   always added to the message queue. For synchronous messages, if the posting
//   execution context is different than the processing execution process, the
//   message is added to the shared message queue and the posting execution
//   context is put to sleep until the message is processed. If the message
//   posting execution context is the same as the processing execution context,
//   the associated manager is called directly, bypassing the message queue.
//   The reason is simple: if this was not the case, the code would dead lock
//   because the message would never be processed.
//
//   The interface iidMessageService (IMessageService) provides the method
//   PostMessage to request the processing of a message. The bWaitCompletion
//   parameter controls how the message is processed: asynchronously or
//   synchronously. The uMessage parameter identifies the message.
//
//   Notifications are sent to the manager through the method
//   IMessageServiceMgr::EvMessageServiceMgrAwaken. The manager must overload
//   this method to receive notifications. The bWaitingCompleting parameter of
//   the manager notification EvMessageServiceMgr method is true if the poster
//   is waiting for the message processing completion.
//
// Location:
//   ServicingThread/IMessageService.h
//
// See Also:
//   IActivationService, IMessageServiceMgr, ISocketService, ITimerService
//
//==EDOC========================================================================
class IMessageService : public IEComUnknown
{
// Published Interface
//---------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IMessageService);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  PostMessage
    //==
    //==========================================================================
    //
    //  Summary:
    //      Pushes a new message onto the message queue.
    //
    //  Parameters:
    //    pManager:
    //      The manager to be notified.
    //
    //    bWaitCompletion:
    //      True if the message should be processed synchronously,
    //      false if the message should be processed asynchronously.
    //
    //    uMessageId:
    //      The identifier of the message that must be posted.
    //
    //    pParameter:
    //      An optional CMarshaler parameter. The ownership of the CMarshaler
    //      is taken by PostMessage. Ownership is not taken if an error is
    //      returned.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      PostMessage is used to push a new message onto the message queue.
    //      The message may be processed synchronously or asynchronously,
    //      depending on the value of bWaitCompletion.
    //
    //==EDOC====================================================================
    virtual mxt_result PostMessage(IN IMessageServiceMgr* pManager,
                                   IN bool bWaitCompletion,
                                   IN unsigned int uMessageId,
                                   IN TOS CMarshaler* pParameter = NULL) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    IMessageService() {}
    virtual ~IMessageService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_IMESSAGESERVICE_H

