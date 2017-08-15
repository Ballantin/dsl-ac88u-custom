//==============================================================================
//==============================================================================
//
//              Copyright(c) 2008 Media5 Corporation ("Media5")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary to
//   Media5.
//
//   Media5 reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in any
//   form whatsoever without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==============================================================================
#ifndef MXG_ISCEUSEREVENTCONTROL_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSEREVENTCONTROL_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


#ifndef MXG_IECOMUNKNOWN_H
#include "ECom/IEComUnknown.h"
#endif

#ifndef MXG_ISCEUSEREVENTMGR_H
#include "SceCoreComponents/ISceUserEventMgr.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISceUserEventControl);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserEventControl
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the user event control APIs.
//
// Description:
//   This interface defines the user event control APIs.
//
//   The user event control can be used for two things:
//
//    - It is used to maintain a list of entities interested in different
//      events associated with a user configuration object. These entities are
//      able to see the events associated with the user configuration object by
//      using AddUserEventMgr. They can stop being notified of events by using
//      RemoveUserEventMgr.
//
//    - It is also used to propagate events associated with the user
//      configuration object to all entities interested in those events. All
//      entities that were registered with AddUserEventMgr will be notified of
//      an event when this event is "published" using PublishUserEvent.
//
// Location:
//   SceCoreComponents/ISceUserEventControl.h
//
// See Also:
//   ISceUserEventMgr
//
//==============================================================================
class ISceUserEventControl : public IEComUnknown
{
//-- Friend Declarations.


//-- Published Interface.
public:
//M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISceUserEventControl);
//M5T_INTERNAL_USE_END

    //==========================================================================
    //==
    //==  AddUserEventMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Adds a user event manager to notify when a user event is published.
    //
    //  Parameters:
    //    pEventMgr:
    //      The manager to notify. MUST not be NULL.
    //
    //  Returns:
    //      - resS_OK upon success;
    //      - resFE_DUPLICATE if the manager is already added;
    //      - resFE_INVALID_ARGUMENT if the manager is NULL.
    //
    //  Description:
    //      Adds a user event manager to notify when a user event is published.
    //
    //      Whenever PublishUserEvent is called, all managers registered with
    //      AddUserEventMgr will be notified of the event.
    //
    //      This should be done as soon as possible by every component that
    //      creates a dialog with the current user.
    //
    //  See Also:
    //      RemoveUserEventMgr
    //
    //==========================================================================
    virtual mxt_result AddUserEventMgr(IN ISceUserEventMgr* pEventMgr) = 0;


    //==========================================================================
    //==
    //==  RemoveUserEventMgr
    //==
    //==========================================================================
    //
    //  Summary:
    //      Removes the user event manager from the list.
    //
    //  Parameters:
    //    pEventMgr:
    //      The manager to release.
    //
    //  Returns:
    //      - resS_OK upon success;
    //      - resFE_FAIL if the manager does not exist;
    //      - resFE_INVALID_ARGUMENT if the manager is NULL.
    //
    //  Description:
    //      Removes the user event manager from the list. The manager, once
    //      released, will no longer be notified of any event.
    //
    //      This should only be done once it is certain a component no longer
    //      needs to be notified of a user event.
    //
    //  See Also:
    //      AddUserEventMgr
    //
    //==========================================================================
    virtual mxt_result RemoveUserEventMgr(IN ISceUserEventMgr* pEventMgr) = 0;


    //==========================================================================
    //==
    //==  PublishUserEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Publishes a new event associated with this user configuration
    //      object.
    //
    //  Parameters:
    //    eEvent:
    //      The event to report.
    //
    //    pUserConfig:
    //      The user configuration object associated with this event.
    //
    //    opqEvent:
    //      Additional event information, as specified in the event type
    //      description.
    //
    //  Description:
    //      This method reports the event passed in parameter to all managers
    //      registered with this user configuration object using
    //      AddUserEventMgr.
    //
    //==========================================================================
    virtual void PublishUserEvent(IN ISceUserEventMgr::EUserEvent eEvent, 
                                  IN IEComUnknown* pUserConfig,
                                  IN mxt_opaque opqEvent) = 0;


//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceUserEventControl() {}

    // Summary:
    //  Destructor.
    virtual ~ISceUserEventControl() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceUserEventControl(IN const ISceUserEventControl& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceUserEventControl& operator=(IN const ISceUserEventControl& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSEREVENTCONTROL_H

