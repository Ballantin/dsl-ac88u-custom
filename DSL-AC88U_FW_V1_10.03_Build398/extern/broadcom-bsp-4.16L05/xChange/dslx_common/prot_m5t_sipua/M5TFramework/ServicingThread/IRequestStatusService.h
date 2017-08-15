//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_IREQUESTSTATUSSERVICE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IREQUESTSTATUSSERVICE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#ifndef MXG_CPOLLREQUESTSTATUS_H
#include "Network/CPollRequestStatus.h"
#endif

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
class IRequestStatusServiceMgr;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(IRequestStatusService);
//M5T_INTERNAL_USE_END
//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IRequestStatusService
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//<FLAG Symbian OS ONLY>
//
// Summary:
//   Provides support for the detection of completion of requests through
//   request status objects.
//
// Description:
//   The request status service provides support for the detection of request
//   status event completion.
//
//   Before receiving notifications about the completion of a request status,
//   the request status must first be registered. The registration associates
//   a manager and an optional opaque value with the request status being
//   registered.
//
//   From now on, completion detection may be enabled. It is also possible to
//   disable the completion detection.
//
//   The associated request status manager is notified when an enabled request
//   status is in the completed state. Following notification, the detected
//   request status is automatically disabled.
//
//   The interface iidRequestStatusService (IRequestStatusService) provides
//   methods to register and unregister request status and methods to enable
//   and disable completion detection on registered request status.
//
//   Notifications are sent to the manager through the method
//   IRequestStatusServiceMgr::EvRequestStatusServiceMgrAwaken. The manager
//   must overload this method to receive notifications.
//
//   Note: This interface is only available on Symbian. Please see
//         ISocketService, which must be used on other OSes.
//
// Location:
//   ServicingThread/IRequestStatusService.h
//
// See Also:
//   IActivationService, IMessageService, IRequestStatusServiceMgr,
//   ITimerService
//
//==EDOC========================================================================
class IRequestStatusService : public IEComUnknown
{
// Published Interface
//---------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(IRequestStatusService);
    //M5T_INTERNAL_USE_END

    //==SDOC====================================================================
    //==
    //==  RegisterRequestStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Registers a request status.
    //
    //  Parameters:
    //    pRequestStatus:
    //      The request status that must be registered.
    //
    //    pManager:
    //      The manager to be notified.
    //
    //    opq:
    //      An opaque value associated with the request status.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Registers a request status. The registration associates a manager
    //      and an optional opaque value with the request status being
    //      registered.
    //
    //==EDOC====================================================================
    virtual mxt_result RegisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                             IN IRequestStatusServiceMgr* pManager,
                                             IN mxt_opaque opq = MX_INT32_TO_OPQ(0)) = 0;

    //==SDOC====================================================================
    //==
    //==  EnableCompletionDetection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Enables the detection of request completion.
    //
    //  Parameters:
    //    pRequestStatus:
    //      The request status for which to enable completion detection.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Enables the detection of request completion.
    //
    //==EDOC====================================================================
    virtual mxt_result EnableCompletionDetection(IN TRequestStatus* pRequestStatus) = 0;

    //==SDOC====================================================================
    //==
    //==  DisableCompletionDetection
    //==
    //==========================================================================
    //
    //  Summary:
    //      Disables the detection of request completion.
    //
    //  Parameters:
    //    pRequestStatus:
    //      The request status for which to enable completion detection.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Disables the detection of request completion.
    //
    //==EDOC====================================================================
    virtual mxt_result DisableCompletionDetection(IN TRequestStatus* pRequestStatus) = 0;

    //==SDOC====================================================================
    //==
    //==  UnregisterRequestStatus
    //==
    //==========================================================================
    //
    //  Summary:
    //      Unregisters a request status.
    //
    //  Parameters:
    //    pRequestStatus:
    //      The request status that must be unregistered.
    //
    //    popq:
    //      On exit, contains the opaque value associated with the request
    //      status. It may be NULL if the value should just be discarded.
    //
    //  Returns:
    //      resFE_INVALID_ARGUMENT
    //
    //  Description:
    //      Unregisters a request status for completion detection notification.
    //
    //==EDOC====================================================================
    virtual mxt_result UnregisterRequestStatus(IN TRequestStatus* pRequestStatus,
                                               OUT mxt_opaque* popq = NULL) = 0;

//M5T_INTERNAL_USE_BEGIN
// Hidden Methods
//----------------
protected:

    // << Constructors / Destructors >>
    //----------------------------------
    IRequestStatusService() {}
    virtual ~IRequestStatusService() {}
//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IREQUESTSTATUSSERVICE_H

