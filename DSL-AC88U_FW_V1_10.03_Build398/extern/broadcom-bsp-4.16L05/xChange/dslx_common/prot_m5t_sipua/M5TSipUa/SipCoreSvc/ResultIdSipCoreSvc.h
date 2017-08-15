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
//   form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_RESULTID_SIPCORESVC_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_RESULTID_SIPCORESVC_H

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
// <GROUP SIPCORESVC_ENUMS>
//
// Summary:
//  Defines the shared success-info IDs.
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
//    Defined in SipCoreSvc/ResultIdSipCoreSvc.h
//
// See Also:
//  mxt_result
//
//==============================================================================
enum EPkgSipCoreSvcWarningCodeId
{
    resSW_SIPCORESVC_ALREADY_BLACKLISTED =
                MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID( eMX_PKG_SIPCORESVC ),
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================

//==============================================================================
// <GROUP SIPCORESVC_ENUMS>
//
// Summary:
//  This enumeration defines the shared fail-error code IDs.
//
// Description:
//  This enumeration defines the shared fail-error code IDs.
//
//  The fail-error codes are meant to carry additional information when a
//  function/method fails or encounters an unhandled condition. If no additional
//  information is to be retuned, it is recommended to use the shared resFE_FAIL
//  return code ID.
//
//  Location:
//    Defined in SipCoreSvc/ResultIdSipCoreSvc.h
//
// See Also:
//  mxt_result
//
//==============================================================================
enum EPkgSipCoreSvcFailErrorCodeId
{
    resFE_SIPCORESVC_STALE_NONCE =
                        MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID(eMX_PKG_SIPCORESVC),
    resFE_SIPCORESVC_NO_NAPTR_RECORD,
    resFE_SIPCORESVC_NAPTR_FAILOVER_PROHIBITED,
    resFE_SIPCORESVC_PERSISTENT_CONNECTION_USER_DISCONNECTED
};


MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END
#endif // MXG_RESULTID_SIPCORESVC_H

