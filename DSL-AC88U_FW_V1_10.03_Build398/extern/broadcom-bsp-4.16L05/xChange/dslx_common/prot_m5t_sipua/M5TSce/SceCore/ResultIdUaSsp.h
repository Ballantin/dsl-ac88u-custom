//==============================================================================
//==============================================================================
//
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
//==============================================================================

#ifndef MXG_RESULTIDUASSP_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_RESULTIDUASSP_H
//M5T_INTERNAL_USE_END

#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

//<GROUP TID_PACKAGE_SCECORE_STRUCTSENUMS>
// Summary:
//  Success warning codes specific to UaSsp.
//
// Description:
//  Success warning codes specific to UaSsp.
enum EMxResultUaSspSuccessWarningCodeId
{
    // Description:
    // An asynchronous TEL URI resolution is in progress, wait for the event before
    // continuing.
    resSW_UASSP_ASYNC_TELURI_RESOLUTION_IN_PROGRESS = MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID(eMX_PKG_UASSP),

    // Description:
    // Persistent connection is not yet establised, wait for the event before
    // continuing.
    resSW_UASSP_ASYNC_PERSISTENT_CONNECTION_ESTABLISMENT_IN_PROGRESS
};

#endif // MXG_RESULTIDUASSP_H
