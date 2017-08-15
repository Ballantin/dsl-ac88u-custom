//==SDOC========================================================================
//==============================================================================
//
//      Copyright(c) 2004 M5T Centre d'Excellence en Telecom Inc. ("M5T")
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
//==EDOC========================================================================
//M5T_INTERNAL_USE_BEGIN
#ifndef MXG_STATUSCODE_H
#define MXG_STATUSCODE_H

//==============================================================================
//====  INCLUDES + FORWARD DECLARATIONS  =======================================
//==============================================================================


//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif


//-- Data Members
//-----------------


//-- Parameters used by Value
//-----------------------------


//-- Interface Realized and/or Parent
//-------------------------------------


MX_NAMESPACE_START(MXD_GNS)
//M5T_INTERNAL_USE_END


//-- Forward Declarations
//-------------------------

//@@STATUSCODE
//==SDOC========================================================================
//== File: StatusCode.h
//======================
// <TITLE Status Codes>
// <GROUP SIPPARSER_VARIABLES>
//
// Description:
//   This file contains the SIP status codes and reason phrases associated with
//   them. It also contains enums and helper methods.
//
// Location:
//   SipParser/StatusCode.h
//
//==EDOC========================================================================


//==============================================================================
//====  CONSTANTS + DEFINES  ===================================================
//==============================================================================


// <TITLE RFC 3261 Status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in RFC 3261.
//
//------------------------------------------------------------------------------
const uint8_t uTRYING                       = 100;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szTRYING                  = "Trying";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint8_t uRINGING                      = 180;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szRINGING                 = "Ringing";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint8_t uCALL_IS_BEING_FORWARDED      = 181;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szCALL_IS_BEING_FORWARDED = "Call Is Being Forwarded";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint8_t uQUEUED                       = 182;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szQUEUED                  = "Queued";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint8_t uSESSION_PROGRESS             = 183;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szSESSION_PROGRESS        = "Session Progress";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint8_t uOK       = 200;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szOK  = "OK";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uMULTIPLE_CHOICES        = 300;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMULTIPLE_CHOICES    = "Multiple Choices";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uMOVED_PERMANENTLY       = 301;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMOVED_PERMANENTLY   = "Moved Permanently";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uMOVED_TEMPORARILY       = 302;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMOVED_TEMPORARILY   = "Moved Temporarily";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uUSE_PROXY               = 305;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUSE_PROXY           = "Use Proxy";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uALTERNATIVE_SERVICE     = 380;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szALTERNATIVE_SERVICE = "Alternative Service";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uBAD_REQUEST                             = 400;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBAD_REQUEST                         = "Bad Request";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBAD_REQUEST_INVALID_CONTACT         = "Missing, Erroneous or Multiple Contact header field(s)";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBAD_REQUEST_INVALID_MESSAGE_SUMMARY = "Missing or invalid message-summary";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szINVALID_SUBSCRIPTION_STATE          = "Invalid Subscription-State Header";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMISSING_SUBSCRIPTION_STATE          = "Missing Subscription-State Header";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szINVALID_P_ASSERTED_IDENTITY         = "Too many or erroneous P-Asserted-Identity header(s)";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uUNAUTHORIZED                            = 401;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUNAUTHORIZED                        = "Unauthorized";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uPAYMENT_REQUIRED                        = 402;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szPAYMENT_REQUIRED                    = "Payment Required";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uFORBIDDEN                               = 403;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szFORBIDDEN                           = "Forbidden";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szAUTOMATIC_ANSWER_FORBIDDEN          = "automatic answer forbidden"; // Defined in RFC5373.
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMANUAL_ANSWER_FORBIDDEN             = "manual answer forbidden"; // Defined in RFC5373.
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szCERTIFICATE_VALIDATION_NOT_COMPATIBLE_WITH_AUTOMATIC_ANSWER = "certificate validation requires user input not compatible with automatic answer"; // Defined in RFC5373.

// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const uint16_t uNOT_FOUND                               = 404;
// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const char* const szNOT_FOUND                           = "Not Found";

// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const uint16_t uMETHOD_NOT_ALLOWED                      = 405;
// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const char* const szMETHOD_NOT_ALLOWED                  = "Method Not Allowed";

// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const uint16_t uNOT_ACCEPTABLE                          = 406;
// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const char* const szNOT_ACCEPTABLE                      = "Not Acceptable";

// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const uint16_t uPROXY_AUTHENTICATION_REQUIRED           = 407;
// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const char* const szPROXY_AUTHENTICATION_REQUIRED       = "Proxy Authentication Required";

// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const uint16_t uREQUEST_TIMEOUT                         = 408;
// <COMBINE uTRYING>
// <GROUP STATUSCODE>
const char* const szREQUEST_TIMEOUT                     = "Request Timeout";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uGONE                                    = 410;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szGONE                                = "Gone";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uREQUEST_ENTITY_TOO_LARGE                = 413;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szREQUEST_ENTITY_TOO_LARGE            = "Request Entity Too Large";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uREQUEST_URI_TOO_LARGE                   = 414;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szREQUEST_URI_TOO_LARGE               = "Request-URI Too Large";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uUNSUPPORTED_MEDIA_TYPE                  = 415;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUNSUPPORTED_MEDIA_TYPE              = "Unsupported Media Type";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uUNSUPPORTED_URI_SCHEME                  = 416;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUNSUPPORTED_URI_SCHEME              = "Unsupported URI Scheme";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uBAD_EXTENSION                           = 420;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBAD_EXTENSION                       = "Bad Extension";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uEXTENSION_REQUIRED                      = 421;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szEXTENSION_REQUIRED                  = "Extension Required";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uINTERVAL_TOO_BRIEF                      = 423;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szINTERVAL_TOO_BRIEF                  = "Interval Too Brief";

// BRCM_CUSTOM [add] - Add status codes.
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uANONYMOUS_CALL_REJECT                   = 433;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szANONYMOUS_CALL_REJECT               = "Anonymous Call Reject";
// BRCM_CUSTOM [end] - Add status codes.

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uTEMPORARILY_NOT_AVAILABLE               = 480;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szTEMPORARILY_NOT_AVAILABLE           = "Temporarily not available";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uCALL_LEG_TRANSACTION_DOES_NOT_EXIST     = 481;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szCALL_LEG_TRANSACTION_DOES_NOT_EXIST = "Call Leg/Transaction Does Not Exist";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uLOOP_DETECTED                           = 482;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szLOOP_DETECTED                       = "Loop Detected";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uTOO_MANY_HOPS                           = 483;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szTOO_MANY_HOPS                       = "Too Many Hops";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uADDRESS_INCOMPLETE                      = 484;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szADDRESS_INCOMPLETE                  = "Address Incomplete";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uAMBIGUOUS                               = 485;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szAMBIGUOUS                           = "Ambiguous";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uBUSY_HERE                               = 486;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBUSY_HERE                           = "Busy Here";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uREQUEST_TERMINATED                      = 487;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szREQUEST_TERMINATED                  = "Request Terminated";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uNOT_ACCEPTABLE_HERE                     = 488;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szNOT_ACCEPTABLE_HERE                 = "Not Acceptable Here";
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUNSUPPORTED_REFER_TO_URI_SCHEME     = "Unsupported Refer-To URI Scheme";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uREQUEST_PENDING                         = 491;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szREQUEST_PENDING                     = "Request Pending";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uUNDECIPHERABLE                          = 493;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szUNDECIPHERABLE                      = "Undecipherable";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uINTERNAL_SERVER_ERROR           = 500;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szINTERNAL_SERVER_ERROR       = "Internal Server Error";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uNOT_IMPLEMENTED                 = 501;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szNOT_IMPLEMENTED             = "Not Implemented";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uBAD_GATEWAY                     = 502;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBAD_GATEWAY                 = "Bad Gateway";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uSERVICE_UNAVAILABLE             = 503;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szSERVICE_UNAVAILABLE         = "Service Unavailable";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uSERVER_TIME_OUT                 = 504;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szSERVER_TIME_OUT             = "Server Time-out";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uSIP_VERSION_NOT_SUPPORTED       = 505;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szSIP_VERSION_NOT_SUPPORTED   = "SIP Version not supported";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uMESSAGE_TOO_LARGE               = 513;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szMESSAGE_TOO_LARGE           = "Message Too Large";

// BRCM_CUSTOM [add] - Add status codes.
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uPRECONDITION_FAILURE            = 580;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szPRECONDITION_FAILURE        = "Precondition Failure";
// BRCM_CUSTOM [end] - Add status codes.

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uBUSY_EVERYWHERE                 = 600;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szBUSY_EVERYWHERE             = "Busy Everywhere";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uDECLINE                         = 603;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szDECLINE                     = "Decline";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uDOES_NOT_EXIST_ANYWHERE         = 604;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szDOES_NOT_EXIST_ANYWHERE     = "Does not exist anywhere";

// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const uint16_t uGLOBAL_NOT_ACCEPTABLE           = 606;
// <GROUP STATUSCODE>
// <COMBINE uTRYING>
const char* const szGLOBAL_NOT_ACCEPTABLE       = szNOT_ACCEPTABLE;


// <TITLE RFC 3265 (Event notification) status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in RFC 3265.
//
//------------------------------------------------------------------------------
const uint8_t uACCEPTABLE       = 202;
// <GROUP STATUSCODE>
// <COMBINE uACCEPTABLE>
const char* const szACCEPTABLE  = "Accepted";

// <GROUP STATUSCODE>
// <COMBINE uACCEPTABLE>
const uint16_t uBAD_EVENT       = 489;
// <GROUP STATUSCODE>
// <COMBINE uACCEPTABLE>
const char* const szBAD_EVENT   = "Bad Event";


// <TITLE draft-ietf-sip-referredby-05.txt status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in draft-ietf-sip-referredby-05.txt.
//
//------------------------------------------------------------------------------
const uint16_t uPROVIDE_REFERRER_IDENTITY       = 429;
// <GROUP STATUSCODE>
// <COMBINE uPROVIDE_REFERRER_IDENTITY>
const char* const szPROVIDE_REFERRER_IDENTITY   = "Provide Referrer Identity";


// <TITLE draft-ietf-sip-session-timer-14.txt status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in draft-ietf-sip-session-timer-14.txt.
//
//------------------------------------------------------------------------------
const uint16_t uSESSION_INTERVAL_TOO_SMALL      = 422;
// <GROUP STATUSCODE>
// <COMBINE uSESSION_INTERVAL_TOO_SMALL>
const char* const szSESSION_INTERVAL_TOO_SMALL  = "Session Interval Too Small";

// <TITLE draft-ietf-sip-publish-04.txt status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in draft-ietf-sip-publish-04.txt.
//
//------------------------------------------------------------------------------
const uint16_t uCONDITIONAL_REQUEST_FAILED      = 412;
// <GROUP STATUSCODE>
// <COMBINE uCONDITIONAL_REQUEST_FAILED>
const char* const szCONDITIONAL_REQUEST_FAILED  = "Conditional Request Failed";

// <TITLE draft-ietf-sip-resource-priority-03.txt status codes>
// <GROUP STATUSCODE>
//
// Description:
//  Status codes defined in draft-ietf-sip-resource-priority-03.txt.
//
//------------------------------------------------------------------------------
const uint16_t uUNKNOWN_RESOURCE_PRIORITY       = 417;
// <GROUP STATUSCODE>
// <COMBINE uUNKNOWN_RESOURCE_PRIORITY>
const char* const szUNKNOWN_RESOURCE_PRIORITY   = "Unknown Resource-Priority";


//==============================================================================
//====  NEW TYPE DEFINITIONS  ==================================================
//==============================================================================

// <GROUP SIPPARSER_ENUMS>
//
// Description:
//  This enumerate contains all RFC 3261 status classes.
//------------------------------------------------------
enum ESipStatusClass
{
    // RFC 3261 Informational status class.
    //-------------------------------------
    eSIP_STATUS_CLASS_INFORMATIONAL = 0,

    // RFC 3261 Success status class.
    //-------------------------------
    eSIP_STATUS_CLASS_SUCCESS,

    // RFC 3261 Redirection status class.
    //-----------------------------------
    eSIP_STATUS_CLASS_REDIRECTION,

    // RFC 3261 Client-Error status class.
    //------------------------------------
    eSIP_STATUS_CLASS_CLIENT_ERROR,

    // RFC 3261 Server-Error status class.
    //------------------------------------
    eSIP_STATUS_CLASS_SERVER_ERROR,

    // RFC 3261 Global-Failure status class.
    //--------------------------------------
    eSIP_STATUS_CLASS_GLOBAL_FAILURE,

    // Non-RFC 3261 status class.
    //---------------------------
    eSIP_STATUS_CLASS_UNKNOWN
};


//==============================================================================
//====  METHOD PROTOTYPES  =====================================================
//==============================================================================

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  Finds in which status class interval a specific status code can be found.
//----------------------------------------------------------------------------
ESipStatusClass MxGetSipStatusClass(IN uint16_t uCode);

// <GROUP SIPPARSER_FUNCTIONS>
//
// Summary:
//  Finds the default reason phrase associated with the status code.
//-------------------------------------------------------------------
const char* MxGetDefaultReasonPhrase(IN uint16_t uCode);


//==============================================================================
//====  INLINE METHODS  ========================================================
//==============================================================================


//M5T_INTERNAL_USE_BEGIN
MX_NAMESPACE_END(MXD_GNS)

#endif //-- #ifndef MXG_STATUSCODE_H
//M5T_INTERNAL_USE_END

