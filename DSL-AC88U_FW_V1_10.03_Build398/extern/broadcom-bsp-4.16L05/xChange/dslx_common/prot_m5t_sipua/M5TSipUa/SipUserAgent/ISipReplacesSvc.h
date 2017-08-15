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
#ifndef MXG_ISIPREPLACESSVC_H
#define MXG_ISIPREPLACESSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT)
#error You must define MXD_SIPSTACK_ENABLE_SIP_REPLACES_SVC_SUPPORT to be able \
to use this class.
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
class ISipReplacesMgr;


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipReplacesSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipReplacesSvc
//========================================
// <GROUP SIPUSERAGENT_CLASSES>
//
// Description:
//  The replaces service is used to determine cases where a dialog can be
//  replaced by another.  It generates events to the application through the
//  ISipReplacesMgr interface.
//
//  Its purpose is to analyse incoming INVITE requests and to check whether or 
//  not they contain a Replaces header. In the affirmative, if a single existing
//  dialog corresponds to the header's parameters, it calls an EvReplaces event.
//  If no dialog, or more than one dialog, corresponds to the header's
//  parameters, a 481 Call Does Not Exist response is sent back.  If the header
//  is wrong, a 400 Bad Request response is sent back.  In both error cases, an
//  EvReplacesWithError event is returned to the application.  Such an event is
//  also returned when the dialog matcher list has not been set beforehand.
//
//  See RFC 3891 for more details on Replaces headers mechanism.
//
// #The ISipReplacesSvc is an ECOM object#
//
//  The ISipReplacesSvc is an ECOM object that is created and accessed through
//  the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipReplacesSvc</B>
//
//  <B>Interface Id: IID_ISipReplacesSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
//  Location:
//       SipUserAgent/ISipReplacesSvc.h
//
//==EDOC========================================================================
class ISipReplacesSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipReplacesSvc);
    //M5T_INTERNAL_USE_END

    //-- << Stereotype >>
    //--------------------------------------------

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
    //      The event manager. It must not be NULL.
    //
    //  Returns:
    //    resFE_INVALID_ARGUMENT:
    //      pMgr is NULL.
    //
    //    resS_OK:
    //      Otherwise.
    //
    //  Description:
    //      Configures the event manager that will receive the events generated
    //      by this service.
    //
    //      Note that an event manager MUST be associated with this service
    //      before it is used.
    //
    //==EDOC====================================================================
    virtual mxt_result SetManager(IN ISipReplacesMgr* pMgr) = 0;


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipReplacesSvc() {}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipReplacesSvc() {}

private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipReplacesSvc(IN const ISipReplacesSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipReplacesSvc& operator=(IN const ISipReplacesSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:
};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPREPLACESSVC_H

