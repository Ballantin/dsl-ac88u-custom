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
#ifndef MXG_HEADERPARAMETER_H
#define MXG_HEADERPARAMETER_H

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

//@@HEADERPARAMETERS
//==============================================================================
//== File: HeaderParameter.h
//==========================
// <TITLE Header Parameters>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains the SIP header parameter names and some pre-defined
//   values.
//
// Location:
//   SipParser/HeaderParameter.h
//
//==============================================================================


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


// <TITLE RFC 3261 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Header parameter names defined in RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_ALGORITHM             = "algorithm";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_BRANCH                = "branch";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_CNONCE                = "cnonce";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_DOMAIN                = "domain";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_DURATION              = "duration";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_EXPIRES               = "expires";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_HANDLING              = "handling";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_MADDR                 = "maddr";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_NC                    = "nc";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_NEXTNONCE             = "nextnonce";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_NONCE                 = "nonce";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_OPAQUE                = "opaque";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_PURPOSE               = "purpose";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_Q                     = "q";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_QOP                   = "qop";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_REALM                 = "realm";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_RECEIVED              = "received";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_RESPONSE              = "response";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_RSPAUTH               = "rspauth";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_STALE                 = "stale";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_TAG                   = "tag";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_TTL                   = "ttl";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_URI                   = "uri";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM>
const char* const szHDRPARAM_USERNAME              = "username";

// <TITLE RFC3261 Magic cookie in "branch" parameter>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  "branch" parameter value for a Via header. Defined in RFC 3261. If the
//  "branch" parameter value of a Via header starts with this string, it means
//  that the branch ID was built following RFC 3261.
//
//-----------------------------------------------------------------------------
const char* const szHDRPARAM_BRANCH_VALUE_MAGIC_COOKIE = "z9hG4bK";

// <TITLE RFC3261 "qop" header parameter values>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Values defined in RFC 3261 for the "qop" header parameter.
//
//-----------------------------------------------------------------------------
const char* const szHDRPARAM_QOP_VALUE_AUTH            = "auth";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_QOP_VALUE_AUTH>
const char* const szHDRPARAM_QOP_VALUE_AUTH_INT        = "auth-int";

// <TITLE RFC3261 "algorithm" header parameter values>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Values defined in RFC 3261 for the "algorithm" header parameter.
//
//------------------------------------------------------------------
const char* const szHDRPARAM_ALGORITHM_VALUE_MD5       = "MD5";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALGORITHM_VALUE_MD5>
const char* const szHDRPARAM_ALGORITHM_VALUE_MD5_SESS  = "MD5-sess";

// <TITLE RFC 3265 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3265 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_ID                    = "id";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ID>
const char* const szHDRPARAM_REASON                = "reason";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ID>
const char* const szHDRPARAM_RETRY_AFTER           = "retry-after";


// <TITLE RFC 3310 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3310 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_AUTS                  = "auts";


// <TITLE RFC 3326 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3326 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_CAUSE                 = "cause";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CAUSE>
const char* const szHDRPARAM_TEXT                  = "text";


// <TITLE RFC 3329 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3329 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_ALG                   = "alg";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_D_ALG                 = "d-alg";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_D_QOP                 = "d-qop";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_D_VER                 = "d-ver";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_EALG                  = "ealg";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_MOD                   = "mod";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_PORT1                 = "port1";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_PORT2                 = "port2";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_PROT                  = "prot";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_ALG>
const char* const szHDRPARAM_SPI                   = "api";


// <TITLE RFC 3455 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3455 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_CCF                   = "ccf";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_CGI_3GPP              = "cgi-3gpp";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_ECF                   = "ecf";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_ICID_VALUE            = "icid-value";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_ICID_GENERATED_AT     = "icid-generated-at";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_ORIG_IOI              = "orig-ioi";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_TERM_IOI              = "term-ioi";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CCF>
const char* const szHDRPARAM_UTRAN_CELL_ID_3GPP    = "utran-cell-id-3gpp";


// <TITLE RFC 3486 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3486 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_COMP                 = "comp";


// <TITLE RFC 3581 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3581 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_RPORT                = "rport";


// <TITLE RFC 3603 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3603 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_CALLED                = "called";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_CALLING               = "calling";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_CHARGE                = "charge";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_CONTENT               = "content";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_COUNT                 = "count";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_KEY                   = "key";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_LOCROUTE              = "locroute";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_REDIRECTOR_URI        = "redirector-uri";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_RKSGROUP              = "rksgroup";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_CALLED>
const char* const szHDRPARAM_ROUTING               = "routing";


// <TITLE RFC 3891 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 3891 but not defined in
//  RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_EARLY_ONLY            = "early-only";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_EARLY_ONLY>
const char* const szHDRPARAM_FROM_TAG              = "from-tag";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_EARLY_ONLY>
const char* const szHDRPARAM_TO_TAG                = "to-tag";


// <TITLE draft-ietf-sip-session-timer-15 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in draft-ietf-sip-session-timer-15
//  but not defined in RFC 3261.
//
//------------------------------------------------------------------------------
const char* const szHDRPARAM_REFRESHER             = "refresher";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_REFRESHER>
const char* const szHDRPARAM_REFRESHER_VALUE_UAC   = "uac";
// <GROUP HEADERPARAMETERS>
// <COMBINE szHDRPARAM_REFRESHER>
const char* const szHDRPARAM_REFRESHER_VALUE_UAS   = "uas";

// <TITLE RFC 1847 parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Parameter names defined in RFC 1847 but not defined in RFC 3261.
//
//------------------------------------------------------------------------------
const char* const pszHDRPARAM_BOUNDARY    = "boundary";


// <TITLE Draft-levy-sip-diversion-08 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in Draft-levy-sip-diversion-08
//  but not defined in RFC3261.
//
//---------------------------------------------------------------------------
const char* const pszHDRPARAM_COUNTER  = "counter";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_COUNTER>
const char* const pszHDRPARAM_LIMIT    = "limit";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_COUNTER>
const char* const pszHDRPARAM_PRIVACY  = "privacy";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_COUNTER>
const char* const pszHDRPARAM_SCREEN   = "screen";


// <TITLE RFC 4538 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC 4538 but not defined
//  in RFC 3261.
//
//---------------------------------------------------------------------------
const char* const pszHDRPARAM_REMOTE_TAG = "remote-tag";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_REMOTE_TAG>
const char* const pszHDRPARAM_LOCAL_TAG  = "local-tag";


// <TITLE Draft-ietf-sip-outbound-15 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in Draft-ietf-sip-outbound-15
//  but not defined in RFC3261.
//
//---------------------------------------------------------------------------
const char* const pszHDRPARAM_REG_ID = "reg-id";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_REG_ID>
const char* const pszHDRPARAM_SIP_INSTANCE = "+sip.instance";
// <GROUP HEADERPARAMETERS>
// <COMBINE pszHDRPARAM_REG_ID>
const char* const pszHDRPARAM_OB = "ob";


// <TITLE RFC5373 Header parameter names>
// <GROUP HEADERPARAMETERS>
//
// Description:
//  Additional header parameter names defined in RFC5373 but not defined in
// RFC3261.
//
const char* const pszHDRPARAM_REQUIRE = "require";

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//M5T_INTERNAL_USE_BEGIN
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_HEADERPARAMETER_H
//M5T_INTERNAL_USE_END

