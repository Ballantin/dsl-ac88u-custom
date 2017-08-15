//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//      Copyright(c) 2007 Media5 Corporation. ("Media5")
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
//   form whatsoever, without prior written approval by Media5.
//
//   Media5 reserves the right to revise this publication and make changes at
//   any time and without the obligation to notify any person and/or entity of
//   such revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_RESULTIDSIPTRANSPORT_H
#define MXG_RESULTIDSIPTRANSPORT_H

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
enum EPkgSipTransportSuccessCodeId
{
    resSI_SIPTRANSPORT_INCOMPLETE_SEND =
                MX_RGET_PKG_BASE_SUCCESS_INFO_CODE_ID( eMX_PKG_SIPTRANSPORT ),
    resSI_SIPTRANSPORT_PACKET_COMPLETE,
    resSI_SIPTRANSPORT_TRYING
};


//==============================================================================
//====  SUCCESS-WARNING CODES  =================================================
//==============================================================================
enum EPkgSipTransportWarningCodeId
{
    resSW_SIPTRANSPORT_MAX_BUFFER_SIZE_EXCEEDED =
            MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID( eMX_PKG_SIPTRANSPORT ),
    resSW_SIPTRANSPORT_PACKET_UNHANDLED,
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================
enum EPkgSipTransportFailErrorCodeId
{
    // Sockets.
    //----------
    resFE_SIPTRANSPORT_INVALID_CONTENT_LENGTH =
                    MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( eMX_PKG_SIPTRANSPORT ),
    resFE_SIPTRANSPORT_INCONSISTANT_CONTENT,
    resFE_SIPTRANSPORT_PAYLOAD_TOO_LARGE,
    resFE_SIPTRANSPORT_SHUTDOWN,
    resFE_SIPTRANSPORT_MTU_SIZE_ERROR,
};


//==============================================================================
//====  FAILURE-CRITICAL CODES  ================================================
//==============================================================================
enum EPkgSipTransportCriticalCodeId
{
    resFC_SIPTRANSPORT_INIT_FAILED =
                MX_RGET_PKG_BASE_FAIL_CRITICAL_CODE_ID( eMX_PKG_SIPTRANSPORT ),
};


MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_RESULTIDSIPTRANSPORT_H

