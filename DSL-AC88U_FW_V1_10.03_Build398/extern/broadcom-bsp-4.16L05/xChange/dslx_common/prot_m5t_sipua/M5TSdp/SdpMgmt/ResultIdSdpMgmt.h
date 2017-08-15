//==============================================================================
//==============================================================================
//
//   Copyright(c) 2006 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//   Copyright(c) 2007 Media5 Corporation. ("Media5")
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
#ifndef MXG_RESULTIDSDPMGMT_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_RESULTIDSDPMGMT_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#ifndef MXG_SDPPARSERCFG_H
#include "Config/SdpParserCfg.h"
#endif

MX_NAMESPACE_START(MXD_GNS)


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================

//==============================================================================
//====  SUCCESS-WARNING CODES  =================================================
//==============================================================================


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================

//<GROUP TID_PACKAGE_SDPMGMT_STRUCTSENUMS>
// Summary:
//  Failure codes specific to the SdpMgmt package.
//
// Description:
//  Failure codes specific to the SdpMgmt package.
enum EPkgSdpMgmtFailErrorCodeId
{
#if defined(MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)
    // Description:
    // Key management is not supported by the peer.
    resFE_KEY_MANAGEMENT_NOT_SUPPORTED_BY_PEER =
        MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( eMX_PKG_SDP ),
    // Description:
    // There is more than one key management in the media answer.
    resFE_MULTIPLE_KEY_MANAGEMENT_IN_MEDIA_ANSWER,
    // Description:
    // There is a key management error.
    resFE_KEY_MANAGEMENT_ERROR,
#endif // #if defined(MXD_SDP_KEY_MANAGEMENT_NEGOTIATION_ENABLE_SUPPORT)
#if defined(MXD_SDP_ENABLE_ICE_LITE_WITHOUT_CC_NEGOTIATION)
    // Description:
    //  Cannot generate an answer since there is no ICE candidate that match the offer.
    resFE_ICE_NO_CANDIDATE_MATCH,
#endif
};


MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_RESULTIDSDPMGMT_H
