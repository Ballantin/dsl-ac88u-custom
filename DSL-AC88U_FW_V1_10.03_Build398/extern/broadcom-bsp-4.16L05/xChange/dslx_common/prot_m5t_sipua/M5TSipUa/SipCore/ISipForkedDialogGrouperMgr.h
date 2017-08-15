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
//   form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPFORKEDDIALOGGROUPERMGR_H
#define MXG_ISIPFORKEDDIALOGGROUPERMGR_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------
class CSipPacket;
class ISipForkedDialogGrouper;
class ISipContext;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipForkedDialogGrouperMgr
//========================================
// <GROUP SIPCORE_CLASSES>
//
// Description:
//   The interface to which the CSipForkedDialogGrouper reports its events.
//
// Location:
//   SipCore/ISipForkedDialogGrouperMgr.h
//
// See Also:
//   CSipForkedDialogGrouper
//
//==EDOC========================================================================
class ISipForkedDialogGrouperMgr
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //-- << Stereotype >>
    //--------------------------------------------

    //==SDOC====================================================================
    //==
    //==  EvNewDialogNeeded
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the manager that a new dialog is needed to handle a new
    //      dialog as a result of a sent request that forked.
    //
    //  Parameters:
    //    rGrouper:
    //      The grouper that needs a new dialog (i.e. the one on which
    //      ContextCreated must be called).
    //
    //    rPacket:
    //      The received packet that needs to be handled on a new dialog.
    //
    //    rOriginator:
    //      The context that sent the request that initiated the dialog.
    //
    //  Description:
    //      This method notifies the manager that a new dialog is needed
    //      because a dialog creating request was forked and the originator
    //      request is already associated with another dialog.
    //
    //      The manager that implements this method should create a new context
    //      with a configuration similar to the originator ISipContext. As a
    //      minimum, the service that generated the request that was forked
    //      should be added to the new context. Once the context is created, the
    //      manager should call ContextCreated on rGrouper.
    //
    //      If the application does not want to allocate ressources for this new
    //      dialog, it should still create a context and add the appropriate
    //      services to reject that dialog. The rejection depends on the type of
    //      request that created the dialog. For dialogs created with an INVITE
    //      request, the application should send a BYE. For dialogs created with
    //      a SUBSCRIBE request, the application could either reject the NOTIFY
    //      received with a 481 response or preferably accept the NOTIFY and
    //      send a SUBSCRIBE request with an expiration of 0 seconds to
    //      terminate the subscription. Note however that terminating
    //      supplemental dialogs can lead to interoperation problems since the
    //      actual target that you are trying to reach is not necessarily the
    //      first one to create a dialog.
    //
    //      This event can be processed synchronously or asynchronously. If the
    //      manager processes this event asynchronously, it must make sure to
    //      count references for the three objects in parameter.
    //
    //  See Also:
    //      ISipForkedDialogGrouper, ISipSessionSvc::Bye,
    //      ISipServerEventCtrl::SendResponse.
    //
    //==EDOC====================================================================
    virtual void EvNewDialogNeeded(IN ISipForkedDialogGrouper& rGrouper,
                                   IN const CSipPacket& rPacket,
                                   IN ISipContext& rOriginator) = 0;

//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipForkedDialogGrouperMgr() {};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipForkedDialogGrouperMgr() {};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipForkedDialogGrouperMgr(IN const ISipForkedDialogGrouperMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipForkedDialogGrouperMgr& operator=(IN const ISipForkedDialogGrouperMgr& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPFORKEDDIALOGGROUPERMGR_H

