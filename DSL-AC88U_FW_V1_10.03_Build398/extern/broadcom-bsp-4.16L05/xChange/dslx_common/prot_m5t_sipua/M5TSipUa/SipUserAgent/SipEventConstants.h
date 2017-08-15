//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_SIPEVENTCONSTANTS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPEVENTCONSTANTS_H

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
//M5T_INTERNAL_USE_END

//-- Forward Declarations
//-------------------------


//@@SIPEVENTCONSTANT
// <TITLE Sip Event Constants>
// <GROUP SIPUSERAGENT_VARIABLES>
// Description:
//  This file contains the SIP Event names and their respective default
//  expiration time in seconds. These are used in the subscriber and notifier
//  services.
//
// Location:
//   SipUserAgent/SipEventConstants.h
//

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


// <TITLE RFC 3842 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3842 and related constant.
const char* const szSIP_EVENT_MESSAGE_SUMMARY               = "message-summary";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_MESSAGE_SUMMARY>
const unsigned int uSIP_EVENT_MESSAGE_SUMMARY_DEFAULT_S     = 3600;


// <TITLE RFC 3856 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3856 and related constant.
//
//------------------------------------------------------------------------------
const char* const szSIP_EVENT_PRESENCE                      = "presence";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_PRESENCE>
const unsigned int uSIP_EVENT_PRESENCE_DEFAULT_S            = 3600;


// <TITLE RFC 3515 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3515 and related constant.
//
//  Note that this event is special since it is always the notifier that decides
//  the subscription duration. 60 seconds should be long enough for the notifier
//  to send its initial NOTIFY.
//
//------------------------------------------------------------------------------
const char* const szSIP_EVENT_REFER                         = "refer";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_REFER>
const unsigned int uSIP_EVENT_REFER_DEFAULT_S               = 60;


// <TITLE RFC 3680 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3680 and related constant.
//
//------------------------------------------------------------------------------
const char* const szSIP_EVENT_REG                           = "reg";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_REG>
const unsigned int uSIP_EVENT_REG_DEFAULT_S                 = 3761;


// <TITLE RFC 3910 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3910 and related constant.
//
//  Note that nothing is indicated as a default expiration time.
//
//------------------------------------------------------------------------------
const char* const szSIP_EVENT_SPIRITS_INDPS                 = "spirits-INDPs";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_SPIRITS_INDPS>
const unsigned int uSIP_EVENT_SPIRITS_INDPS_DEFAULT_S       = 3600;
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_SPIRITS_INDPS>
const char* const szSIP_EVENT_SPIRITS_USER_PROF             = "spirits-user-prof";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_SPIRITS_INDPS>
const unsigned int uSIP_EVENT_SPIRITS_USER_PROF_DEFAULT_S   = 3600;


// <TITLE RFC 3857 Subscription Events>
// <GROUP SIPEVENTCONSTANT>
//
// Description:
//  Subscription events defined in RFC 3857 and related constant.
//
//------------------------------------------------------------------------------
const char* const szSIP_EVENT_WINFO                         = "winfo";
// <GROUP SIPEVENTCONSTANT>
// <COMBINE szSIP_EVENT_WINFO>
const unsigned int uSIP_EVENT_WINFO_DEFAULT_S               = 3600;


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPEVENTCONSTANTS_H

