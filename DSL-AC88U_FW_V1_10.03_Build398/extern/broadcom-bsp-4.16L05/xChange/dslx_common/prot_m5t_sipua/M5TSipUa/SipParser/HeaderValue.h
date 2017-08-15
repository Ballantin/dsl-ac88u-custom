//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_HEADERVALUE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_HEADERVALUE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations
//-------------------------

//@@HEADERVALUE
//==SDOC========================================================================
//== File: HeaderValue.h
//==========================
// <TITLE Header Value>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains some Header values. Note that Option tags are also
//   Header values and they can be found in SipParser/OptionTag.h.
//
// Location:
//   SipParser/HeaderValue.h
//
// See Also:
//   SipParser/OptionTag.h
//
//==EDOC========================================================================


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// <TITLE RFC 3323 Privacy header values>
// <GROUP HEADERVALUE>
//
// Description:
//  Privacy header value defined in RFC 3323 are:
//
//  <PRE>
//      priv-value = "header" / "session" / "user" / "none" / "critical"
//  </PRE>
//
//----------------------------------------------------------------------
const char* const pszHDR_PRIVACY_VALUE_HEADER     = "header";
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_PRIVACY_VALUE_HEADER>
const char* const pszHDR_PRIVACY_VALUE_SESSION    = "session";
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_PRIVACY_VALUE_HEADER>
const char* const pszHDR_PRIVACY_VALUE_USER       = "user";
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_PRIVACY_VALUE_HEADER>
const char* const pszHDR_PRIVACY_VALUE_NONE       = "none";
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_PRIVACY_VALUE_HEADER>
const char* const pszHDR_PRIVACY_VALUE_CRITICAL   = "critical";


// <TITLE RFC 3325 Privacy header values>
// <GROUP HEADERVALUE>
//
// Description:
//  RFC 3325 section 9.3 defines
//
//  <PRE>
//      priv-value = "id"
//  </PRE>
//----------------------------------------------------------------------
const char* const pszHDR_PRIVACY_VALUE_ID         = "id";


// <TITLE RFC 5373 Answer-Mode header values>
// <GROUP HEADERVALUE>
//
// Description:
//  Answer-Mode header value defined in RFC 5373 are:
//
//  <PRE>
//      answer-mode-value = "Manual" / "Auto" / token
//  </PRE>
//
const char* const pszHDR_ANSWER_MODE_VALUE_MANUAL = "Manual";
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_ANSWER_MODE_VALUE_MANUAL>
const char* const pszHDR_ANSWER_MODE_VALUE_AUTO = "Auto";


// <TITLE RFC 5373 Priv-Answer-Mode header values>
// <GROUP HEADERVALUE>
//
// Description:
//  Priv-Answer-Mode header value defined in RFC 5373 are:
//
//  <PRE>
//      answer-mode-value = "Manual" / "Auto" / token
//  </PRE>
//
const char* const pszHDR_PRIV_ANSWER_MODE_VALUE_MANUAL = pszHDR_ANSWER_MODE_VALUE_MANUAL;
// <GROUP HEADERVALUE>
// <COMBINE pszHDR_PRIV_ANSWER_MODE_VALUE_MANUAL>
const char* const pszHDR_PRIV_ANSWER_MODE_VALUE_AUTO = pszHDR_ANSWER_MODE_VALUE_AUTO;

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_HEADERVALUE_H

