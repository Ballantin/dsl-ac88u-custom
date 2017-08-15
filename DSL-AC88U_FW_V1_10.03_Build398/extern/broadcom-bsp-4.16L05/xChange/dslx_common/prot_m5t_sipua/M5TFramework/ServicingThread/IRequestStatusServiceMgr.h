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
#ifndef MXG_IREQUESTSTATUSSERVICEMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IREQUESTSTATUSSERVICEMGR_H
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
//== Class: IRequestStatusServiceMgr
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//<FLAG Symbian OS ONLY>
//
// Summary:
//   This is the interface that must be implemented to use
//   IRequestStatusService.
//
// Description:
//   This is the interface that must be implemented to use
//   IRequestStatusService.
//
// Location:
//   ServicingThread/IRequestStatusServiceMgr.h
//
// See Also:
//   IRequestStatusService
//
//==EDOC========================================================================
class IRequestStatusServiceMgr
{
// Published Interface
//---------------------
public:
    //==SDOC====================================================================
    //==
    //==  EvRequestStatusServiceMgrAwaken
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager about newly completed request.
    //
    //  Parameters:
    //    pRequestStatus:
    //      The identifier of the request status.
    //
    //    opq:
    //      The opaque value provided at registration.
    //
    //  Description:
    //      The request status is registered and the detection is enabled. The
    //      request completed and its status has been updated. From now on,
    //      completion on this specific request will no longer be detected. The
    //      detection needs to be re-enabled before the completion may be
    //      detected again.
    //
    //      For example:
    //          A request status is registered and detection is enabled. At one
    //          point, the manager gets notified about the completion. Following
    //          this notification, the manager is never notified again about the
    //          completion of any new request issued reusing this request status.
    //
    //==EDOC====================================================================
    virtual void EvRequestStatusServiceMgrAwaken(IN TRequestStatus* pRequestStatus,
                                                 IN mxt_opaque opq) = 0;
//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  IRequestStatusServiceMgr
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
    IRequestStatusServiceMgr() {}

    //==SDOC====================================================================
    //==
    //==  ~IRequestStatusServiceMgr
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
    virtual ~IRequestStatusServiceMgr() {}

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_OS_SYMBIAN) && defined(MXD_NETWORK_ENABLE_SUPPORT)

#endif // MXG_IREQUESTSTATUSSERVICEMGR_H

