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
#ifndef MXG_TXTMESSAGECONSTANTS_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_TXTMESSAGECONSTANTS_H
//M5T_INTERNAL_USE_END

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================

//-- M5T Global definitions
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

MX_NAMESPACE_START(MXD_GNS)

//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================
//M5T_INTERNAL_USE_BEGIN
const char g_cNUL = '\0';
const char g_cBACKSPACE = 0x08;
const char g_cTAB = '\t';
const char g_cLF = '\n';
const char g_cCR = '\r';
const char g_cSP = ' ';
const char g_cDQUOTE = '\"';
const char g_cPERCENT = '%';
const char g_cSQUOTE = '\'';
const char g_cSTAR = '*';
const char g_cPLUS = '+';
const char g_cCOMMA = ',';
const char g_cMINUS = '-';
const char g_cDOT = '.';
const char g_cSLASH = '/';
const char g_cZERO = '0';
const char g_cEIGHT = '8';
const char g_cCOLON = ':';
const char g_cSEMI = ';';
const char g_cLESS_THAN = '<';
const char g_cEQUAL = '=';
const char g_cGREATER_THAN = '>';
const char g_cQUESTION_MARK = '?';
const char g_cOPENING_SQUARE_BRACKET = '[';
const char g_cBACKSLASH = '\\';
const char g_cCLOSING_SQUARE_BRACKET = ']';
const char g_cLOW_B = 'b';
const char g_cLOW_N = 'n';
const char g_cLOW_R = 'r';
const char g_cLOW_T = 't';

const char* const g_pszEMPTY = "";
const char* const g_pszCRLF = "\r\n";
const char* const g_pszSLASH = "/";
const char* const g_pszSPACE = " ";
const char* const g_pszAMPERSAND = "&";

const char* const g_pszDOUBLECRLF = "\r\n\r\n";

const char* const g_pszDASHBOUNDARY = "--";

const char* const g_pszCOMMA_SEPARATOR = ",";
const char* const g_pszCOLON_SEPARATOR = ":";
const char* const g_pszEQUAL_SEPARATOR = "=";

const char* const g_pszESCAPED_COMMA_SEPARATOR = "%2C%20";
const char* const g_pszESCAPED_SLASH = "%2F";
const char* const g_pszESCAPED_SPACE = "%20";

// URI schemes:
//  - sip     : RFC3261 "sip" URI scheme name. See ITxtUri::EUriType::eSIP.
//  - sips    : RFC3261 "sips" URI scheme name. See ITxtUri::EUriType::eSIPS.
//  - tel     : RFC3966 "tel" URI scheme name. See ITxtUri::EUriType::eTEL.
//  - mailbox : RFC2822 "mailbox" URI scheme name. See ITxtUri::EUriType::eMAILBOX.
//  - pres    : RFC3859 "pres" URI scheme name. See ITxtUri::EUriType::ePRES.
//  - im      : RFC3860 "im" URI scheme name. See IUri::EUriType::eIM.
//  - http    : RFC2616 "http" URI scheme name. See IUri::EUriType::eHTTP.
//  - https   : RFC2818 "https" URI scheme name. See IUri::EUriType::eHTTPS.
const char* const g_pszURISCHEME_SIP  = "sip";
const char* const g_pszURISCHEME_SIPS = "sips";
const char* const g_pszURISCHEME_TEL = "tel";
const char* const g_pszURISCHEME_MAILBOX = "mailbox";
const char* const g_pszURISCHEME_PRES = "pres";
const char* const g_pszURISCHEME_IM = "im";
const char* const g_pszURISCHEME_HTTP = "http";
const char* const g_pszURISCHEME_HTTPS = "https";

//M5T_INTERNAL_USE_END

MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_TXTMESSAGECONSTANTS_H

