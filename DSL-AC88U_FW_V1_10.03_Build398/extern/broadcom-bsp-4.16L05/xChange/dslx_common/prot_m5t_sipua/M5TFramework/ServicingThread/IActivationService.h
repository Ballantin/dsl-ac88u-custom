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
#ifndef MXG_IACTIVATIONSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IACTIVATIONSERVICE_H
//M5T_INTERNAL_USE_BEGIN

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
#ifndef MXG_CTHREAD_H
#include "Kernel/CThread.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------
#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//-- Forward Declarations
//-------------------------

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IActivationService);
//M5T_INTERNAL_USE_END
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IActivationService
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   Interface providing functionalities to activate a servicing thread.
//
// Description:
//   The interface iidActivationService (IActivationService) provides
//   functionalities to activate a Servicing Thread. The activation gives life
//   to the Servicing Thread, giving it an execution context to process its
//   services. There are two different activation mechanisms: the first is the
//   activation throughout the creation of an internal thread that provides the
//   execution context for the Servicing Thread, the second is a periodic
//   polling of the activation method from an execution context, thus providing
//   an external execution context to the Servicing Thread. This second mode is
//   most useful when an application already has its own execution context.
//   Instead of forcing the creation of another thread, the framework allows
//   the activation of the Servicing Thread from an application thread.
//
// Location:
//   ServicingThread/IActivationService.h
//
// See Also:
//   IMessageService, ISocketService, ITimerService
//
//==EDOC========================================================================
class IActivationService : public IEComUnknown
{
// Published Interface
//---------------------
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IActivationService);
//M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  Activate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Requests the activation by using an internal thread.
    //
    //  Parameters:
    //    pszName:
    //      The name associated with the spawned thread.
    //
    //    uStackSize:
    //       The stack size of the associated thread. 0 is allowed to use the
    //       default stack size.
    //
    //    ePriority:
    //      The priority that must be associated with the internal thread.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      This method should be called when an internal thread should be
    //      created and assigned to supply the execution context.
    //
    //==EDOC====================================================================
    virtual mxt_result Activate(IN const char* pszName = NULL,
                                IN uint32_t uStackSize = 0,
                                IN CThread::EPriority ePriority = CThread::eNORMAL) = 0;

    //==SDOC====================================================================
    //==
    //==  Activate
    //==
    //==========================================================================
    //
    //  Summary:
    //      Requests the activation by using an external thread.
    //
    //  Parameters:
    //    uTimeoutMs:
    //      The maximum period of time that may be passed to process the
    //      execution context.
    //
    //    pbReadyToRelease:
    //      On exit, contains true if the execution context is no longer
    //      required and may be released.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //      resFE_INVALID_STATE
    //
    //  Description:
    //      This method should be called periodically when an external thread
    //      is used to supply the execution context.
    //
    //      The fact that a reference is required to call this method renders
    //      impossible the automatic release of the object. To resolve this
    //      issue, the caller may pass a pointer to a bool that is set to true
    //      if the only reference left is this one. Normally, the caller should
    //      then release the reference it owns and the execution context would
    //      then be released.
    //
    //  Warning:
    //      This method MUST ALWAYS be called at least once before any other
    //      method is called on a Servicing Thread. A dead lock will occur if
    //      this is not enforced.
    //
    //  Warning:
    //      This method MUST ALWAYS be called from the same thread.
    //      Trying to call this method from multiple threads, even if
    //      synchronized, leads to undefined behaviour. This is because each
    //      thread has its own Thread Specific Data ("TSD"). Code that uses the
    //      TSD does not behave correctly under these circumstances.
    //
    //==EDOC====================================================================
    virtual mxt_result Activate(IN uint64_t uTimeoutMs,
                                OUT bool* pbReadyToRelease) = 0;

    //==SDOC====================================================================
    //==
    //==  IsCurrentExecutionContext
    //==
    //==========================================================================
    //
    //  Summary:
    //      Determines if this Servicing Thread is the current execution
    //      context.
    //
    //  Returns:
    //      true if this Servicing Thread is the current execution context,
    //      false otherwise.
    //
    //  Description:
    //      Determines if this Servicing Thread is the current execution
    //      context.
    //
    //==EDOC====================================================================
    virtual bool IsCurrentExecutionContext() = 0;


//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    IActivationService() {}
    virtual ~IActivationService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_IACTIVATIONSERVICE_H
