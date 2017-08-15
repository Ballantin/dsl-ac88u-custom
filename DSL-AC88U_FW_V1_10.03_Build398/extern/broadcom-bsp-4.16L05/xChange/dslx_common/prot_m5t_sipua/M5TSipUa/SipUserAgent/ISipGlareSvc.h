//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation ("Media5")
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
//==EDOC========================================================================
#ifndef MXG_ISIPGLARESVC_H
#define MXG_ISIPGLARESVC_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SIPSTACKCFG_H
#include "Config/SipStackCfg.h"
#endif


#if !defined(MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_GLARE_SVC_SUPPORT to be able to \
use this class.
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
class ISipGlareMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipGlareSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipGlareSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//   The Glare service handles the case where both parties send each other
//   a request simultaneously. As per RFC 3261 section 21.4.27 and
//   section 14.1, when the SIP stack receives a 491 (Request Pending), the
//   Glare service starts a timer. When the timer fires, it is reported to the
//   application through the generation of an event using the Glare manager. The
//   application can then decide whether it wants to reissue the request or not.
//
//   In addition to the INVITE, this service acts on any non-ACK method.
//
//   Refer to the ISipGlareMgr documentation for more details about the events
//   that this service can report.
//
//   Note that an event manager MUST be associated with this service before it
//   is used. See the SetManager method for more information.
//
// #The ISipGlareSvc is an ECOM object#
//
//  The ISipGlareSvc is an ECOM object that is created and accessed through the
//  following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipGlareSvc</B>
//
//  <B>Interface Id: IID_ISipGlareSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipUserAgentSvc/ISipGlareSvc.h
//
//==EDOC========================================================================
class ISipGlareSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------

//   Public Types
//-----------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipGlareSvc);
    //M5T_INTERNAL_USE_END

//-- Published Interface
//------------------------
public:
    //==SDOC====================================================================
    //==
    //==  SetManager
    //==
    //==========================================================================
    //
    //  Summary:
    //      Configures the event manager associated with this service.
    //
    //  Parameters:
    //    pMgr:
    //      The event manager. Must not be NULL.
    //
    //  Description:
    //      Configures the event manager that will receive the events generated
    //      by this service.
    //
    //      Note that an event manager MUST be associated with this service
    //      before it is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipGlareMgr* pMgr) = 0;

//-- Hidden Methods
//-------------------
protected:

    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipGlareSvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipGlareSvc(){}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipGlareSvc(IN const ISipGlareSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipGlareSvc& operator=(IN const ISipGlareSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPGLARESVC_H

