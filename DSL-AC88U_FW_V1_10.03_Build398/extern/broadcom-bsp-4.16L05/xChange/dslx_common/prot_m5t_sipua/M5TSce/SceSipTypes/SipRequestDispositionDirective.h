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
#ifndef MXG_SIPREQUESTDISPOSITIONDIRECTIVE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_SIPREQUESTDISPOSITIONDIRECTIVE_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions.
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SCECORECFG_H
#include "Config/SceCoreCfg.h" // MXD_OVERRIDE_TYPE_OPTION_TAG
#endif

#ifndef MXG_CVECTOR_H
#include "Cap/CVector.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//-- Forward Declarations.
class CString;

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

// Make sure that either both options are not defined or that both options are
// defined.

#if !defined(MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE) && \
    defined(MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE_STRINGS)
#error Must synchronize type and strings.
#endif

#if defined(MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE) && \
    !defined(MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE_STRINGS)
#error Must synchronize type and strings.
#endif

//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

#if !defined(MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE)

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Defines SIP request disposition directives.
//
// Description:
//  This type defines the currently registered SIP request disposition 
//  directives at the IANA (as defined in RFC 3841).
// 
//  It is allowed to add non-IANA supported types, but such items must be
//  clearly indicated.
// 
//  The list of registered request disposition directives can be found at:
//  http://www.iana.org/assignments/sip-parameters.
//
//  This type must be ordered alphabetically without regards to case.
//
//  This type can be overriden by defining MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE.
//
//  See Also:
//      MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE
enum ERequestDispositionDirective
{
//M5T_INTERNAL_USE_BEGIN
    eDIRECTIVE_FIRST        = 0x00000001,
//M5T_INTERNAL_USE_END

    // Description:
    // This directive indicates that the caller would like each server to
    // proxy the request. eDIRECTIVE_PROXY and eDIRECTIVE_REDIRECT are
    // mutually exclusive; both cannot be set at the same time. If both are
    // set, eDIRECTIVE_PROXY takes precedence.
    eDIRECTIVE_PROXY        = 0x00000001,

    // Description:
    // This directive indicates that the caller would like each server to
    // redirect the request. eDIRECTIVE_PROXY and eDIRECTIVE_REDIRECT are
    // mutually exclusive; both cannot be set at the same time. If both are
    // set, eDIRECTIVE_PROXY takes precedence.
    eDIRECTIVE_REDIRECT     = 0x00000002,

    // Description:
    // This directive indicates that the caller would like each proxy
    // server to send a CANCEL on early branches downstream in response to
    // a 200 OK from the downstream server. This is the normal mode of
    // operation. eDIRECTIVE_CANCEL and eDIRECTIVE_NOCANCEL are mutually
    // exclusive; both cannot be set at the same time. If both are set,
    // eDIRECTIVE_CANCEL takes precedence.
    eDIRECTIVE_CANCEL       = 0x00000004,

    // Description:
    // This directive indicates that the caller would like each proxy
    // server to NOT send a CANCEL on early branches downstream in response
    // to a 200 OK from the downstream server. eDIRECTIVE_CANCEL and
    // eDIRECTIVE_NOCANCEL are mutually exclusive; both cannot be set at
    // the same time. If both are set, eDIRECTIVE_CANCEL takes precedence.
    eDIRECTIVE_NOCANCEL     = 0x00000008,
    
    
    // Description:
    // This directive indicates that the caller would like each proxy
    // server to fork the request whenever there are multiple potential
    // targets for the request. This directive is ignored by servers when
    // eDIRECTIVE_REDIRECT is set. eDIRECTIVE_FORK and eDIRECTIVE_NOFORK
    // are mutually exclusive; both cannot be set at the same time. If both
    // are set, eDIRECTIVE_FORK takes precedence.
    eDIRECTIVE_FORK         = 0x00000010,

    // Description:
    // This directive indicates that the caller would like each proxy
    // server to forward the request only to the best matching target,
    // ignoring other potential targets. This directive is ignored by
    // servers when eDIRECTIVE_REDIRECT is set. eDIRECTIVE_FORK and
    // eDIRECTIVE_NOFORK are mutually exclusive; both cannot be set at the
    // same time. If both are set, eDIRECTIVE_FORK takes precedence.
    eDIRECTIVE_NOFORK       = 0x00000020,

    // Description:
    // This directive indicates that a proxy server receiving a 3xx
    // response should send requests to the addresses listed in the
    // response. This directive is ignored by servers if
    // eDIRECTIVE_REDIRECT is set. eDIRECTIVE_RECURSE and
    // eDIRECTIVE_NORECURSE are mutually exclusive; both cannot be set at
    // the same time. If both are set, eDIRECTIVE_RECURSE takes precedence.
    eDIRECTIVE_RECURSE      = 0x00000040,

    // Description:
    // This directive indicates that a proxy server receiving a 3xx
    // response should forward the addresses listed in the response to the
    // caller. This directive is ignored by servers if eDIRECTIVE_REDIRECT
    // is set. eDIRECTIVE_RECURSE and eDIRECTIVE_NORECURSE are mutually
    // exclusive; both cannot be set at the same time. If both are set,
    // eDIRECTIVE_RECURSE takes precedence.
    eDIRECTIVE_NORECURSE    = 0x00000080,

    // Description:
    // This directive indicates that a forking proxy server should proxy
    // the request to all known addresses at once. This directive is
    // ignored by servers if eDIRECTIVE_REDIRECT is set. eDIRECTIVE_PARALLEL
    // and eDIRECTIVE_SEQUENTIAL are mutually exclusive; both cannot be set
    // at the same time. If both are set, eDIRECTIVE_PARALLEL takes
    // precedence.
    eDIRECTIVE_PARALLEL     = 0x00000100,

    // Description:
    // This directive indicates that a forking proxy server should proxy
    // the request to all known addresses at once. This directive is
    // ignored by servers if eDIRECTIVE_REDIRECT is set. eDIRECTIVE_PARALLEL
    // and eDIRECTIVE_SEQUENTIAL are mutually exclusive; both cannot be set
    // at the same time. If both are set, eDIRECTIVE_PARALLEL takes
    // precedence.
    eDIRECTIVE_SEQUENTIAL   = 0x00000200,

    // Description:
    // This directive indicates that if the called party is temporarily
    // unreachable, e.g., because it is in another call, the caller wants
    // to have its call queued. eDIRECTIVE_QUEUE and eDIRECTIVE_NOQUEUE are
    // mutually exclusive; both cannot be set at the same time. If both are
    // set, eDIRECTIVE_QUEUE takes precedence.
    eDIRECTIVE_QUEUE        = 0x00000400,

    // Description:
    // This directive indicates that if the called party is temporarily
    // unreachable, e.g., because it is in another call, the caller wants
    // to have its call rejected immediately. eDIRECTIVE_QUEUE and
    // eDIRECTIVE_NOQUEUE are mutually exclusive; both cannot be set at the
    // same time. If both are set, eDIRECTIVE_QUEUE takes precedence.
    eDIRECTIVE_NOQUEUE      = 0x00000800,

//M5T_INTERNAL_USE_BEGIN
    eDIRECTIVE_LAST         = 0x00001000,
//M5T_INTERNAL_USE_END
};
#else
// Use the updated type instead, which is defined within
// MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE.
MXD_OVERRIDE_TYPE_REQUEST_DISPOSITION_DIRECTIVE
#endif

//M5T_INTERNAL_USE_BEGIN

// Summary:
//  Initializes the Sip option tags.
mxt_result InitializeSipRequestDispositionDirectives();

// Summary:
//  Finalizes the Sip option tags.
void FinalizeSipRequestDispositionDirectives();

extern const char* const g_aszSIP_REQUEST_DISPOSITION_DIRECTIVE[];
extern CVector<CString>* g_pvecstrSIP_REQUEST_DISPOSITION_DIRECTIVE;

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_SIPREQUESTDISPOSITIONDIRECTIVE_H

