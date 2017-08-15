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
#ifndef MXG_OPTIONTAG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_OPTIONTAG_H
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

//@@OPTIONTAG
//==SDOC========================================================================
//== File: OptionTag.h
//==========================
// <TITLE Option tag>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains some Option tags registered at IANA. Option tags are
//   header values. They are are used in header fields such as Require, 
//   Supported, Proxy-Require, and Unsupported in support of SIP compatibility
//   mechanisms for extensions. The option tag itself is a string that
//   is associated with a particular SIP option.
//
// Location:
//   SipParser/OptionTag.h
//
// See Also:
//   SipParser/HeaderValue.h
//
//==EDOC========================================================================


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// <TITLE IANA option tags>
// <GROUP OPTIONTAG>
//
// Description:
//  Options tags registered at IANA.
//
//------------------------------------------------------------------------------
const char* const pszOPTIONTAG_100REL                  = "100rel";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_ANSWERMODE              = "answermode";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_EARLYSESSION            = "early-session";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_EVENTLIST               = "eventlist";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
// BRCM_CUSTOM - [add] Add connected ID support per RFC 4916
const char* const pszOPTIONTAG_FROMCHANGE              = "from-change";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_FROMCHANGE>
// BRCM_CUSTOM - [end] Add connected ID support per RFC 4916
const char* const pszOPTIONTAG_HISTINFO                = "histinfo";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_JOIN                    = "join";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_NOREFERSUB              = "norefersub";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_OUTBOUND                = "outbound";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_PATH                    = "path";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_PRECONDITION            = "precondition";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_PREF                    = "pref";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_PRIVACY                 = "privacy";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_REPLACES                = "replaces";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_RESOURCEPRIORITY        = "resource-priority";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_SDPANAT                 = "sdp-anat";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_SECAGREE                = "sec-agree";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_TDIALOG                 = "tdialog";
// <GROUP OPTIONTAG>
// <COMBINE pszOPTIONTAG_100REL>
const char* const pszOPTIONTAG_TIMER                   = "timer";


MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_OPTIONTAG_H

