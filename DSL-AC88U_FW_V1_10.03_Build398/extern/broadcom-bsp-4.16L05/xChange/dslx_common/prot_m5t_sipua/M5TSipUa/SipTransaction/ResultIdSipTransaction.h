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
//   form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_RESULTID_SIPTRANSACTION_H
#define MXG_RESULTID_SIPTRANSACTION_H

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
//====  SUCCESS-INFO CODES  ====================================================
//==============================================================================
enum EPkgSipTransactionSuccessCodeId
{
    resSI_SIPTRANSACTION_FINAL_NEGATIVE_RESPONSE =
                MX_RGET_PKG_BASE_SUCCESS_INFO_CODE_ID( eMX_PKG_SIPTRANSACTION ),
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================
enum EPkgSipTransactionFailErrorCodeId
{
    resFE_SIPTRANSACTION_SHUTDOWN =
                MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( eMX_PKG_SIPTRANSACTION ),
};


MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_RESULTID_SIPTRANSACTION_H

