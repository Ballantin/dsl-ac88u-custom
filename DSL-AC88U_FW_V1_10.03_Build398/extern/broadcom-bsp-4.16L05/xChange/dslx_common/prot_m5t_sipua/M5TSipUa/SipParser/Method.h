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
#ifndef MXG_METHOD_H
#define MXG_METHOD_H

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
#ifndef MXG_CSTRING_H
#include "Cap/CString.h"
#endif

//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------


//==SDOC========================================================================
//== File: Method.h
//======================
//
// Description:
//   This file contains the SIP method-related enums and conversion methods.
//
// Location:
//   SipParser/Method.h
//
//==EDOC========================================================================

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// <GROUP SIPPARSER_ENUMS>
//
// Description:
//  ESipMethod enumerates some of the various SIP methods that can be used with
//  the stack.
//------------------------------------------------------------------------------
enum ESipMethod
{
    eSIP_METHOD_ACK = 0,        // RFC 3261 (SIP).
    eSIP_METHOD_BYE,            // RFC 3261 (SIP).
    eSIP_METHOD_CANCEL,         // RFC 3261 (SIP).
    eSIP_METHOD_INFO,           // RFC 2976 (The SIP INFO Method).
    eSIP_METHOD_INVITE,         // RFC 3261 (SIP).
    eSIP_METHOD_MESSAGE,        // RFC 3428 (SIP Extension for Instant Messaging).
    eSIP_METHOD_NOTIFY,         // RFC 3265 (SIP-Specific Event Notification).
    eSIP_METHOD_OPTIONS,        // RFC 3261 (SIP).
    eSIP_METHOD_PING,           // draft-sen-midcom-fw-nat-02.txt. This draft is currently outdated and has been removed from the IETF site.
    eSIP_METHOD_PRACK,          // RFC 3262 (PRACK).
    eSIP_METHOD_PUBLISH,        // RFC 3903 (Session Initiation Protocol (SIP) Extension for Event State Publication)
    eSIP_METHOD_REFER,          // RFC 3515 (The SIP Refer Method).
    eSIP_METHOD_REGISTER,       // RFC 3261 (SIP).
    eSIP_METHOD_SERVICE,        // draft-deason-sip-soap-00.txt. This draft is currently outdated and has been removed from the IETF site.
    eSIP_METHOD_SUBSCRIBE,      // RFC 3265 (SIP-Specific Event Notification).
    eSIP_METHOD_UPDATE,         // RFC 3311 (SIP UPDATE Method).
    
    // Description
    //  A type of method that the SipStack is unaware of.
    eSIP_METHOD_UNKNOWN,
};


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//  This array holds the actual strings representing some of the various SIP
//  methods supported by the stack. Note that this array is indexed according
//  to the ESipMethod enumeration. Thus, g_aszMETHOD_NAME[eREGISTER] yields the
//  "REGISTER" string.
//------------------------------------------------------------------------------
extern const char* const g_aszMETHOD_NAME[eSIP_METHOD_UNKNOWN+1];

//==============================================================================
//====  METHOD PROTOTYPES  =====================================================
//==============================================================================

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  Converts a method string to the equivalent enum.
//---------------------------------------------------
ESipMethod MxConvertSipMethod(IN const CString& rstrMethod);

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  Converts a method enum to its equivalent string.
//---------------------------------------------------
const char* MxConvertSipMethod(IN ESipMethod eMethod);


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_METHOD_H

