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
#ifndef MXG_RESULTIDSIPPARSER_H
#define MXG_RESULTIDSIPPARSER_H

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
enum EPkgSipParserSuccessCodeId
{
    resSI_SIPPARSER_DATACONT =
                    MX_RGET_PKG_BASE_SUCCESS_INFO_CODE_ID( eMX_PKG_SIP_PARSER ),
    resSI_SIPPARSER_SIPPACKET_HEADER_INCOMPLETE_START_LINE,
    resSI_SIPPARSER_SIPPACKET_FOUND_DOUBLE_CRLF,
    resSI_SIPPARSER_RAWHEADER_END_FOUND,
};


//==============================================================================
//====  SUCCESS-WARNING CODES  =================================================
//==============================================================================
enum EPkgSipParserWarningCodeId
{
    resSW_SIPPARSER_SIPURI_USERINFO_NOT_FOUND =
                MX_RGET_PKG_BASE_SUCCESS_WARNING_CODE_ID( eMX_PKG_SIP_PARSER ),
    resSW_SIPPARSER_NAMEADDR_DISPLAY_NOT_FOUND,
    resSW_SIPPARSER_HEADER_NOT_ENOUGH_HEADERS,
    resSW_SIPPARSER_NULL_PTR,
    resSW_SIPPARSER_RAWHEADER_INCOMPLETE,
    resSW_SIPPARSER_MWI_NO_DATA,
    resSW_SIPPARSER_SIPPACKET_COMMIT_COMPLETE_WITH_APPEND_ERRORS,
    resSW_SIPPARSER_TELURI_PHONE_CONTEXT_NOT_FOUND,
};


//==============================================================================
//====  FAILURE-ERROR CODES  ===================================================
//==============================================================================
enum EPkgSipParserFailErrorCodeId
{
    resFE_SIPPARSER_STRHELP_CHAR_NOT_FOUND =
                    MX_RGET_PKG_BASE_FAIL_ERROR_CODE_ID( eMX_PKG_SIP_PARSER ),
    resFE_SIPPARSER_TOKEN_NOT_FOUND,
    resFE_SIPPARSER_HEADERLIST_NOT_FOUND,
    resFE_SIPPARSER_STARTLINE_UNKNOWN_PROTOCOL,
    resFE_SIPPARSER_SIPPACKET_INVALID_REQUEST_START_LINE,
    resFE_SIPPARSER_SIPPACKET_COMMIT_INVALID_STATE,
};


MX_NAMESPACE_END(MXD_GNS)

#endif // MXG_RESULTIDSIPPARSER_H

