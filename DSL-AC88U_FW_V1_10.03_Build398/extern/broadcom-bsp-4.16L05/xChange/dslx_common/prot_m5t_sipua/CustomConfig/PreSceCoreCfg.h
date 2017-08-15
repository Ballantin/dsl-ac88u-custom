//==SDOC========================================================================
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
//==EDOC========================================================================
#ifndef MXG_PRESCECORECFG_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_PRESCECORECFG_H
//M5T_INTERNAL_USE_END

#if 0
#include <cctkCfg.h>
#endif

/* SIP option tags override */
#define MXD_OVERRIDE_TYPE_OPTION_TAG                                               \
enum ESipOptionTag                                                                 \
{                                                                                  \
   eOPTIONTAG_FIRST              = 0x00000001,                                     \
   eOPTIONTAG_100REL             = 0x00000001,                                     \
   eOPTIONTAG_ANSWERMODE         = 0x00000002, /* draft-ietf-sip-answermode-06 */  \
   eOPTIONTAG_EARLY_SESSION      = 0x00000004,                                     \
   eOPTIONTAG_EVENTLIST          = 0x00000008,                                     \
   eOPTIONTAG_FROM_CHANGE        = 0x00000010,                                     \
   eOPTIONTAG_GRUU               = 0x00000020,                                     \
   eOPTIONTAG_HISTINFO           = 0x00000040,                                     \
   eOPTIONTAG_JOIN               = 0x00000080,                                     \
   eOPTIONTAG_NOREFERSUB         = 0x00000100,                                     \
   eOPTIONTAG_OUTBOUND           = 0x00000200, /* draft-ietf-sip-outbound */       \
   eOPTIONTAG_PATH               = 0x00000400,                                     \
   eOPTIONTAG_PRECONDITION       = 0x00000800,                                     \
   eOPTIONTAG_PREF               = 0x00001000,                                     \
   eOPTIONTAG_PRIVACY            = 0x00002000,                                     \
   eOPTIONTAG_REPLACES           = 0x00004000,                                     \
   eOPTIONTAG_RESOURCE_PRIORITY  = 0x00008000,                                     \
   eOPTIONTAG_SDP_ANAT           = 0x00010000,                                     \
   eOPTIONTAG_SEC_AGREE          = 0x00020000,                                     \
   eOPTIONTAG_TDIALOG            = 0x00040000,                                     \
   eOPTIONTAG_TIMER              = 0x00080000,                                     \
   eOPTIONTAG_LAST               = 0x00100000                                      \
};

#define MXD_OVERRIDE_TYPE_OPTION_TAG_STRINGS                                       \
const char* const g_aszSIPOPTIONTAG[] =                                            \
{                                                                                  \
   "100rel",                                                                       \
   "answermode",                                                                   \
   "early-session",                                                                \
   "eventlist",                                                                    \
   "from-change",                                                                  \
   "gruu",                                                                         \
   "histinfo",                                                                     \
   "join",                                                                         \
   "norefersub",                                                                   \
   "outbound",                                                                     \
   "path",                                                                         \
   "precondition",                                                                 \
   "pref",                                                                         \
   "privacy",                                                                      \
   "replaces",                                                                     \
   "resource-priority",                                                            \
   "sdp-anat",                                                                     \
   "sec-agree",                                                                    \
   "tdialog",                                                                      \
   "timer",                                                                        \
   ""                                                                              \
}; 


/* SIP event tags override */
#define MXD_OVERRIDE_TYPE_EVENT_TYPE                                               \
enum ESipEventType                                                                 \
{                                                                                  \
    eEVENTTYPE_FIRST              = 0x00000001,                                    \
    eEVENTTYPE_CONFERENCE         = 0x00000001,                                    \
    eEVENTTYPE_DIALOG             = 0x00000002,                                    \
    eEVENTTYPE_KPML               = 0x00000004,                                    \
    eEVENTTYPE_MESSAGE_SUMMARY    = 0x00000008,                                    \
    eEVENTTYPE_POC_SETTINGS       = 0x00000010,                                    \
    eEVENTTYPE_PRESENCE           = 0x00000020,                                    \
    eEVENTTYPE_PRESENCE_WINFO     = 0x00000040,                                    \
    eEVENTTYPE_REFER              = 0x00000080,                                    \
    eEVENTTYPE_REG                = 0x00000100,                                    \
    eEVENTTYPE_SIEMENS_RTP_STATS  = 0x00000200,                                    \
    eEVENTTYPE_SPIRITS_INDPS      = 0x00000400,                                    \
    eEVENTTYPE_SPIRITS_USER_PROF  = 0x00000800,                                    \
    eEVENTTYPE_WINFO              = 0x00001000,                                    \
    eEVENTTYPE_XCAP_DIFF          = 0x00002000,                                    \
    eEVENTTYPE_USR_AGT_PROF       = 0x00004000,                                    \
    eEVENTTYPE_LAST               = 0x00008000                                     \
};

#define MXD_OVERRIDE_TYPE_EVENT_TYPE_STRINGS                                       \
const char* const g_aszSIPEVENTTYPE[] =                                            \
{                                                                                  \
    "conference",                                                                  \
    "dialog",                                                                      \
    "kpml",                                                                        \
    "message-summary",                                                             \
    "poc-settings",                                                                \
    "presence",                                                                    \
    "presence.winfo",                                                              \
    "refer",                                                                       \
    "reg",                                                                         \
    "Siemens-RTP-Stats",                                                           \
    "spirits-INDPs",                                                               \
    "spirits-user-prof",                                                           \
    "winfo",                                                                       \
    "xcap-diff",                                                                   \
    "ua-profile",                                                                  \
    ""                                                                             \
};

#endif // MXG_PRESCECORECFG_H

