//==============================================================================
//==============================================================================
//
//   Copyright(c) 2008 Media5 Corporation ("Media5")
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
//==============================================================================
#ifndef MXG_SCESIPCONSTANTS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SCESIPCONSTANTS_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


// This file contains SIP constants that are used within the SIP Client Engine
// but are NOT defined in SIP-UA v4.1.

// Reason-Phrase values.

const char szPAYLOAD_PARSING_ERROR[] = "Payload Parsing Error";


// Header values

// Content-Disposition header values.
const char* const pszHDR_CONTENT_DISPOSITION_VALUE_EARLY_SESSION = "early-session";
const char* const pszHDR_CONTENT_DISPOSITION_VALUE_RENDER = "render";
const char* const pszHDR_CONTENT_DISPOSITION_VALUE_SESSION = "session";

// Content-Encoding header values.
const char* const pszHDR_CONTENT_ENCODING_VALUE_IDENTITY = "identity";

// BRCM_CUSTOM - [add] Add content language validation
// Content-Language header values.
const char* const pszHDR_CONTENT_LANGUAGE_VALUE_ENGLISH = "en";
// BRCM_CUSTOM - [end] Add content language validation

// Header parameter names.

// Event header parameters.
const char* const pszHDRPARAM_DIFF_PROCESSING = "diff-processing"; // [draft-ietf-sip-xcapevent-04]


// Header parameter values

// Content-Disposition "handling" parameter values.
const char* const pszHDRPARAM_HANDLING_VALUE_OPTIONAL = "optional";

// Event "diff-processing" parameter values.
const char* const pszHDRPARAM_DIFF_PROCESSING_VALUE_NO_PATCHING = "no-patching"; // [draft-ietf-sip-xcapevent-04]
const char* const pszHDRPARAM_DIFF_PROCESSING_VALUE_XCAP_PATCHING = "xcap-patching"; // [draft-ietf-sip-xcapevent-04]

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif //-- #ifndef MXG_SCESIPCONSTANTS_H

