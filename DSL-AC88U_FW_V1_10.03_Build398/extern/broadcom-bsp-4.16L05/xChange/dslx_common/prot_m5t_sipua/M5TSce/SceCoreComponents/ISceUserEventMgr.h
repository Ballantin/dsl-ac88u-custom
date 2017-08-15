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
#ifndef MXG_ISCEUSEREVENTMGR_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_ISCEUSEREVENTMGR_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==============================================================================
//== Class: ISceUserEventMgr
//========================================
//<GROUP TID_PACKAGE_SCECORECOMPONENTS_CLASSES>
//
// Summary:
//   This interface defines the manager used by the ISceUserEventControl to
//   report events associated with a user configuration object.
//
// Description:
//  This interface defines the manager used by the ISceUserEventControl to
//  report events associated with a user configuration object.
//
// Location:
//   SceCoreComponents/ISceUserEventMgr.h
//
// See Also:
//   ISceUserEventControl
//
//==============================================================================
class ISceUserEventMgr
{
//-- Friend Declarations.


//-- Published Interface.
public:

    //==========================================================================
    //<GROUP TID_PACKAGE_SCECORECOMPONENTS_STRUCTSENUMS>
    //
    // Summary:
    //  This enumeration defines the possible events that can be reported.
    //
    // Description:
    //  This enumeration defines the possible events that can be reported.
    //  These events are always associated with a specific user configuration
    //  object.
    //
    //==========================================================================
    enum EUserEvent
    {
        
        // Description:
        //  This event reports that all dialogs associated with the specified
        //  user configuration object must be terminated. The opqEvent
        //  parameter is unused and must be ignored by the event handler when
        //  this event is reported.
        eEVENT_TERMINATEUSER,

// BRCM_CUSTOM - [add] Add Timer F expiration handling support per IMS
        // Description:
        //  This event reports that all dialogs associated with the specified
        //  user configuration object must be terminated silently (i.e. without
        //  sending any SIP packets out onto the network. The opqEvent
        //  parameter is unused and must be ignored by the event handler when
        //  this event is reported.
        eEVENT_TERMINATEUSER_SILENT,
// BRCM_CUSTOM - [end] Add Timer F expiration handling support per IMS

        // This is defined in order to perform range control.
        eEVENT_LAST
    };

    //==========================================================================
    //==
    //==  EvUserConfigEvent
    //==
    //==========================================================================
    //
    //  Summary:
    //      Reports an event associated with a user configuration object.
    //
    //  Parameters:
    //    eEvent:
    //      The event being reported.
    //
    //    pUserConfig:
    //      Pointer to the user configuration object for which this event is
    //      reported.
    //
    //    opqEvent:
    //      Data associated with the event, as described in the event type.
    //
    //  Description:
    //      Reports an event associated with a user configuration object.
    //
    //      The implementation of this event handler must refer to the
    //      EUserEvent documentation to know what the eEvent parameter means and
    //      also to further know how the opqEvent parameter can be used.
    //
    //==========================================================================
    virtual void EvUserConfigEvent(IN EUserEvent eEvent,
                                   IN IEComUnknown* pUserConfig,
                                   IN mxt_opaque opqEvent) = 0;

//-- Hidden Methods.
protected:
    //-- << Constructors / Destructors / Operators >>

    // Summary:
    //  Default Constructor.
    ISceUserEventMgr() {}

    // Summary:
    //  Destructor.
    virtual ~ISceUserEventMgr() {}

private:

    //-- << Deactivated Constructors / Destructors / Operators >>

    // Summary:
    //  Copy Constructor.
    ISceUserEventMgr(IN const ISceUserEventMgr& rSrc);

    // Summary:
    //  Assignment Operator.
    ISceUserEventMgr& operator=(IN const ISceUserEventMgr& rSrc);


//-- Hidden Data Members.
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISCEUSEREVENTMGR_H

