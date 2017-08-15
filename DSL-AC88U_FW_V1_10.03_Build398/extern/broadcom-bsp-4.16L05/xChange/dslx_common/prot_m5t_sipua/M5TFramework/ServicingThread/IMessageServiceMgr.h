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
#ifndef MXG_IMESSAGESERVICEMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_IMESSAGESERVICEMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

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
class CMarshaler;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: IMessageServiceMgr
//========================================
//<GROUP SERVICING_THREAD_CLASSES>
//
// Summary:
//   This is the interface that must be implemented to use IMessageService.
//
// Description:
//   This is the interface that must be implemented to use IMessageService.
//
// Location:
//   ServicingThread/IMessageServiceMgr.h
//
// See Also:
//   IActivationService, IMessageServiceMgr, ISocketService, ITimerService
//
//==EDOC========================================================================
class IMessageServiceMgr
{
// Published Interface
//---------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvMessageServiceMgrAwaken
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a new message must be processed.
    //
    //  Parameters:
    //    bWaitingCompletion:
    //      True if the message is being processed synchronously, false if
    //      processed asynchronously.
    //
    //    uMessageId:
    //      The identifier of the message that must be posted.
    //
    //    pParameter:
    //      An optional CMarshaler parameter.
    //
    //  Description:
    //      Notifies the manager that a new message must be processed.
    //
    //==EDOC====================================================================
    virtual void EvMessageServiceMgrAwaken(IN bool bWaitingCompletion,
                                           IN unsigned int uMessageId,
                                           IN CMarshaler* pParameter) = 0;

//M5T_INTERNAL_USE_BEGIN
protected:
    // << Constructors / Destructors >>
    //----------------------------------

    //==SDOC====================================================================
    //==
    //==  IMessageServiceMgr
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
    IMessageServiceMgr() {}

    //==SDOC====================================================================
    //==
    //==  ~IMessageServiceMgr
    //==
    //==========================================================================
    //
    // Summary:
    //  Destructor.
    //
    // Description:
    //  Destructor.
    //
    //==EDOC====================================================================
    virtual ~IMessageServiceMgr() {}

//M5T_INTERNAL_USE_END
};

MX_NAMESPACE_END(MXD_GNS)

#endif // #if defined(MXD_SERVICING_THREAD_ENABLE_SUPPORT)

#endif // MXG_IMESSAGESERVICEMGR_H

