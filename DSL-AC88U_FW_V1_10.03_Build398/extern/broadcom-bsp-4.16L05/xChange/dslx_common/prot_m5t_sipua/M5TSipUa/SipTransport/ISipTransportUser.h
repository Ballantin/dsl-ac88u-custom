//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without prior written approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_ISIPTRANSPORTUSER_H
#define MXG_ISIPTRANSPORTUSER_H

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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipTransportUser
//========================================
// <GROUP SIPTRANSPORT_CLASSES>
//
// Description:
//   The SIP transport user interface defines the event that can be reported
//   back to modules using the transport manager.
//
// Location:
//   SipTransport/ISipTransportUser.h
//
// See Also:
//   ISipTransportMgr
//
//==EDOC========================================================================
class ISipTransportUser
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:

    //==SDOC====================================================================
    //==
    //==  EvCommandResult
    //==
    //==========================================================================
    //
    //  Summary:
    //      Notifies the transport user of the result of an operation.
    //
    //  Parameters:
    //    res:
    //      The result code.  For further details, see the async method
    //      reporting this event upon completion or error.
    //
    //    opq:
    //      Opaque parameter given to the transport manager.
    //
    //  Description:
    //      This event is reported by the transport manager to a specific user
    //      when the transport has completed an operation, successfully or not.
    //
    //  See Also:
    //      ISipTransportMgr async methods
    //
    //==EDOC====================================================================
    virtual void EvCommandResult(IN mxt_result res,
                                 IN mxt_opaque opq) = 0;




//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipTransportUser(){};

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipTransportUser(){};

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipTransportUser(IN const ISipTransportUser& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipTransportUser& operator=(IN const ISipTransportUser& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:

};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPTRANSPORTUSER_H

