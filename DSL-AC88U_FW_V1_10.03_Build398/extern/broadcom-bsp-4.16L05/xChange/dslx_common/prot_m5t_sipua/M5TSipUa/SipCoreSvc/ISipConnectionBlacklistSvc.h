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
#ifndef MXG_ISIPCONNECTIONBLACKLISTSVC_H
#define MXG_ISIPCONNECTIONBLACKLISTSVC_H

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


#if !defined(MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT)
#error You must define \
MXD_SIPSTACK_ENABLE_SIP_CONNECTION_BLACKLIST_SVC_SUPPORT to be able to use \
this class.
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


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
MX_DECLARE_ECOM_IID(ISipConnectionBlacklistSvc);
//M5T_INTERNAL_USE_END

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

//==SDOC========================================================================
//== Class: ISipConnectionBlacklistSvc
//========================================
// <GROUP SIPCORESVC_CLASSES>
//
// Description:
//   This service automatically manages a blacklist of destinations to use. A
//   destination is an IP address, a port number, and a transport. A blacklist
//   is a list of destinations that are not used to contact a peer. The only way
//   a destination blacklisted is used is when all possible destinations to
//   reach a peer are blacklisted. In this case, the preferred one is used.
//
//   When sending a packet to a destination fails, this service adds the
//   destination to the blacklist for the configured amount of time. After the
//   blacklisted destination expires, it is removed from the blacklist.
//
//   When sending a packet to a destination succeeds, this service removes the
//   destination from the blacklist.
//
//   The blacklist can directly be accessed by the application. To access it,
//   call CSipConnectionBlacklist::Instance().
//
//   IMPORTANT: To work properly, this service must be added to the context
//   AFTER the ISipServerLocationSvc.
//
//#The ISipConnectionBlacklistSvc is an ECOM object#
//
//  The ISipConnectionBlacklistSvc is an ECOM object that is created and
//  accessed through the following ECOM identifiers:
//
//  <B>Class Id: CLSID_CSipConnectionBlacklistSvc</B>
//
//  <B>Interface Id: IID_ISipConnectionBlacklistSvc</B>
//
//  A user can query the ISipContext to which this service is attached by
//  calling QueryIf on it. It can also directly access all other services
//  attached to the ISipContext through the same mean.
//
// Location:
//   SipCoreSvc/ISipConnectionBlacklistSvc.h
//
// See Also:
//   CSipConnectionBlacklist
//
//==EDOC========================================================================
class ISipConnectionBlacklistSvc : public IEComUnknown
{
//-- Friend Declarations
//------------------------


//-- Published Interface
//------------------------
public:
    //M5T_INTERNAL_USE_BEGIN
    MX_DECLARE_ECOM_GETIID(ISipConnectionBlacklistSvc);
    //M5T_INTERNAL_USE_END


//-- Hidden Methods
//-------------------
protected:
    //-- << Constructors / Destructors / Operators >>
    //-------------------------------------------------

    // Summary:
    //  Default Constructor.
    //-----------------------
    ISipConnectionBlacklistSvc(){}

    // Summary:
    //  Destructor.
    //--------------
    virtual ~ISipConnectionBlacklistSvc(){}


private:
    //-- << Deactivated Constructors / Destructors / Operators >>
    //-------------------------------------------------------------

    // Summary:
    //  Copy Constructor.
    //--------------------
    ISipConnectionBlacklistSvc(IN const ISipConnectionBlacklistSvc& rSrc);

    // Summary:
    //  Assignment Operator.
    //-----------------------
    ISipConnectionBlacklistSvc& operator=(
                                    IN const ISipConnectionBlacklistSvc& rSrc);


//-- Hidden Data Members
//------------------------
protected:
private:


};


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_ISIPCONNECTIONBLACKLISTSVC_H

