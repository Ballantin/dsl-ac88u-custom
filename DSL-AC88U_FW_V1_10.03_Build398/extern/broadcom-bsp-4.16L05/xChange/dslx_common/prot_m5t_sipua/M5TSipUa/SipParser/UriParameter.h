//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_URIPARAMETER_H
#define MXG_URIPARAMETER_H

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

//@@URIPARAMETER
//==SDOC========================================================================
//== File: UriParameter.h
//=======================
// <TITLE Uri Parameter>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains the SIP URI parameter names and some pre-defined values.
//
// Location:
//   SipParser/UriParameter.h
//
//==EDOC========================================================================


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


// <TITLE RFC 3261 URI parameter names>
// <GROUP URIPARAMETER>
//
// Description:
//  URI parameter names defined in RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szURIPARAM_LR                             = "lr";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_LR>
const char* const szURIPARAM_MADDR                          = "maddr";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_LR>
const char* const szURIPARAM_METHOD                         = "method";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_LR>
const char* const szURIPARAM_TRANSPORT                      = "transport";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_LR>
const char* const szURIPARAM_TTL                            = "ttl";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_LR>
const char* const szURIPARAM_USER                           = "user";

// <TITLE RFC3261 "user" URI parameter values>
// <GROUP URIPARAMETER>
//
// Description:
//  Values defined in RFC 3261 for the "user" URI parameter.
//-----------------------------------------------------------------------------
const char* const szURIPARAM_USER_VALUE_IP       = "ip";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_USER_VALUE_IP>
const char* const szURIPARAM_USER_VALUE_PHONE    = "phone";

// <TITLE RFC3261 "transport" URI parameter values>
// <GROUP URIPARAMETER>
//
// Description:
//  Values defined in RFC 3261 for the "transport" URI parameter.
//-----------------------------------------------------------------------------
const char* const szURIPARAM_TRANSPORT_UDP = "udp";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_TRANSPORT_UDP>
const char* const szURIPARAM_TRANSPORT_TCP = "tcp";
// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_TRANSPORT_UDP>
const char* const szURIPARAM_TRANSPORT_TLS = "tls";

// <TITLE RFC 3486 URI parameter names>
// <GROUP URIPARAMETER>
//
// Description:
//  Additional URI parameter names defined in RFC 3486 but not defined in
//  RFC 3261.
//------------------------------------------------------------------------------
const char* const szURIPARAM_COMP                = "comp";

// <TITLE RFC3486 "comp" URI parameter values>
// <GROUP URIPARAMETER>
//
// Description:
//  Values defined in RFC 3486 for the "comp" URI parameter.
//-----------------------------------------------------------------------------

// <GROUP URIPARAMETER>
// <COMBINE szURIPARAM_COMP>
const char* const szURIPARAM_COMP_VALUE_SIGCOMP  = "sigcomp";

// <TITLE RFC 3966 URI parameter names>
// <GROUP URIPARAMETER>
//
// Description:
//  URI parameter names defined in RFC 3966.
//------------------------------------------------------------------------------
const char* const szURIPARAM_PHONE_CONTEXT  = "phone-context";


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================

//M5T_INTERNAL_USE_BEGIN
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_URIPARAMETER_H
//M5T_INTERNAL_USE_END

