//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
#ifndef MXG_RESULTID_SIPCORE_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_RESULTID_SIPCORE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  SUCCESS-WARNING CODES  =================================================
//==============================================================================

//==============================================================================
// <GROUP SIPCORE_ENUMS>
//
// Summary:
//  This enumeration defines the shared success-warning code IDs.
//
// Description:
//  This enumeration defines the shared success-warning code IDs.
//
//  The success-warning codes are meant to carry additional information when a
//  function/method succeeds, but with the addition of raising a yellow flag.
//
//  Location:
//    Defined in SipCore/ResultIdSipCore.h
//
// See Also:
//  mxt_result
//
//==============================================================================
enum EPkgSipCoreWarningCodeId
{
    resSW_SIPCORE_SAME_CALLID_FOUND = MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID(eMX_PKG_SIPCORE),
    resSW_SIPCORE_WAIT_ASYNC_RESPONSE,
    resSW_SIPCORE_MUST_RETRY,
    resSW_SIPCORE_PACKET_UNHANDLED,
    resSW_SIPCORE_ACTIVE_DIALOG,
    resSW_SIPCORE_ONLY_LOCAL_ADDRESS_FOUND,
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================

//==============================================================================
// <GROUP SIPCORE_ENUMS>
//
// Summary:
//  This enumeration defines the shared fail-error code IDs.
//
// Description:
//  This enumeration defines the shared fail-error code IDs.
//
//  The fail-error codes are meant to carry additional information when a
//  function/method fails or encounters an unhandled condition. If no additional
//  information is to be returned, it is recommended to use the shared resFE_FAIL
//  return code ID.
//
//  Location:
//    Defined in SipCore/ResultIdSipCore.h
//
// See Also:
//  mxt_result
//
//==============================================================================
enum EPkgSipCoreFailErrorCodeId
{
    resFE_SIPCORE_PACKET_BLOCKED = MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID(eMX_PKG_SIPCORE),
    resFE_SIPCORE_REQUEST_CANCELLED,
};


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif // MXG_RESULTID_SIPCORE_H

