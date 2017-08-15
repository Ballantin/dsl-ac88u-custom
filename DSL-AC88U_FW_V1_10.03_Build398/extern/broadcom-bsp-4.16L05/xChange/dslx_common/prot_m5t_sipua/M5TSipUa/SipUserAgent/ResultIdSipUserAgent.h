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
#ifndef MXG_RESULTID_SIPUSERAGENT_H
#define MXG_RESULTID_SIPUSERAGENT_H

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
enum EPkgSipUserAgentWarningCodeId
{
    resSW_DIALOG_NOT_REGISTERED =
        MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID( eMX_PKG_SIPUSERAGENT ),
    resSW_DIALOG_ALREADY_UNREGISTERED,
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================
enum EPkgSipUserAgentFailErrorCodeId
{
    resFE_INVALID_CONTACT_HEADER =
        MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( eMX_PKG_SIPUSERAGENT ),
    resFE_NO_ISIPUSERAGENTSVC_ATTACHED,
    resFE_CALL_LEG_TRANSACTION_DOES_NOT_EXIST,
    resFE_REQUEST_PENDING,
    resFE_UNKNOWN_SUBSCRIPTION,
    resFE_UNKNOWN_EVENT,
    resFE_EXPIRED_SUBSCRIPTION,
    resFE_MISSING_HEADER,
    resFE_INTERVAL_TOO_BRIEF,
    resFE_INVALID_CONTENT,
    resFE_UNSUPPORTED_URI_SCHEME,
    resFE_UNKNOWN_QOP
};


MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_RESULTID_SIPUSERAGENT_H
