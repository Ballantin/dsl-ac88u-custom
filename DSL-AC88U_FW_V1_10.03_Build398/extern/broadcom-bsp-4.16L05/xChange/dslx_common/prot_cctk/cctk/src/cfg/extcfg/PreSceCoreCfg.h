/**********************************************************************************
** Copyright © 2007-2010 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file contains the pre SCE core configuration definitions.
**
***********************************************************************************/

#ifndef MXG_PRESCECORECFG_H
#define MXG_PRESCECORECFG_H

#include <cctkCfg.h>

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


/* SIP media tags override */
#define MXD_OVERRIDE_TYPE_MEDIA_TYPE                                               \
enum ESipMediaType                                                                 \
{                                                                                  \
    eMEDIATYPE_FIRST                              = 0x00000001,                    \
    eMEDIATYPE_APPLICATION_CONFERENCE_INFO_XML    = 0x00000001,                    \
    eMEDIATYPE_APPLICATION_CPL_XML                = 0x00000002,                    \
    eMEDIATYPE_APPLICATION_DIALOG_INFO_XML        = 0x00000004,                    \
    eMEDIATYPE_APPLICATION_IM_ISCOMPOSING_XML     = 0x00000008,                    \
    eMEDIATYPE_APPLICATION_KPML_REQUEST_XML       = 0x00000010,                    \
    eMEDIATYPE_APPLICATION_KPML_RESPONSE_XML      = 0x00000020,                    \
    eMEDIATYPE_APPLICATION_PIDF_XML               = 0x00000040,                    \
    eMEDIATYPE_APPLICATION_PIDF_DIFF_XML          = 0x00000080,                    \
    eMEDIATYPE_APPLICATION_PKCS10                 = 0x00000100,                    \
    eMEDIATYPE_APPLICATION_PKCS7_MIME             = 0x00000200,                    \
    eMEDIATYPE_APPLICATION_PKCS7_SIGNATURE        = 0x00000400,                    \
    eMEDIATYPE_APPLICATION_POC_SETTINGS_XML       = 0x00000800,                    \
    eMEDIATYPE_APPLICATION_REGINFO_XML            = 0x00001000,                    \
    eMEDIATYPE_APPLICATION_RESOURCE_LISTS_XML     = 0x00002000,                    \
    eMEDIATYPE_APPLICATION_RLMI_XML               = 0x00004000,                    \
    eMEDIATYPE_APPLICATION_RLS_SERVICES_XML       = 0x00008000,                    \
    eMEDIATYPE_APPLICATION_SDP                    = 0x00010000,                    \
    eMEDIATYPE_APPLICATION_SIMPLE_FILTER_XML      = 0x00020000,                    \
    eMEDIATYPE_APPLICATION_SIMPLE_MESSAGE_SUMMARY = 0x00040000,                    \
    eMEDIATYPE_APPLICATION_WATCHERINFO_XML        = 0x00080000,                    \
    eMEDIATYPE_APPLICATION_XCAP_DIFF_XML          = 0x00100000,                    \
    eMEDIATYPE_MESSAGE_CPIM                       = 0x00200000,                    \
    eMEDIATYPE_MESSAGE_SIP                        = 0x00400000,                    \
    eMEDIATYPE_MESSAGE_SIPFRAG                    = 0x00800000,                    \
    eMEDIATYPE_MULTIPART_ALTERNATIVE              = 0x01000000,                    \
    eMEDIATYPE_MULTIPART_MIXED                    = 0x02800000,                    \
    eMEDIATYPE_MULTIPART_RELATED                  = 0x04000000,                    \
    eMEDIATYPE_MULTIPART_SIGNED                   = 0x08000000,                    \
    eMEDIATYPE_APPLICATION_XML                    = 0x10000000,                    \
    eMEDIATYPE_TEXT_XML_SMML                      = 0x20000000,                    \
    eMEDIATYPE_AUDIO_TELEPHONE_EVENT              = 0x40000000,                    \
    eMEDIATYPE_LAST                               = 0x80000000,                    \
};

#define MXD_OVERRIDE_TYPE_MEDIA_TYPE_STRINGS                                       \
const SSipMediaType g_astSIPMEDIATYPE[] =                                          \
{                                                                                  \
    {   "application",  "conference-info+xml"   },                                 \
    {   "application",  "cpl+xml"               },                                 \
    {   "application",  "dialog-info+xml"       },                                 \
    {   "application",  "im-iscomposing+xml"    },                                 \
    {   "application",  "kpml-request+xml"      },                                 \
    {   "application",  "kpml-response+xml"     },                                 \
    {   "application",  "pidf+xml"              },                                 \
    {   "application",  "pidf-diff+xml"         },                                 \
    {   "application",  "pkcs10"                },                                 \
    {   "application",  "pkcs7-mime"            },                                 \
    {   "application",  "pkcs7-signature"       },                                 \
    {   "application",  "poc-settings+xml"      },                                 \
    {   "application",  "reginfo+xml"           },                                 \
    {   "application",  "resource-lists+xml"    },                                 \
    {   "application",  "rlmi+xml"              },                                 \
    {   "application",  "rls-services+xml"      },                                 \
    {   "application",  "sdp"                   },                                 \
    {   "application",  "simple-filter+xml"     },                                 \
    {   "application",  "simple-message-summary"},                                 \
    {   "application",  "watcherinfo+xml"       },                                 \
    {   "application",  "xcap-diff+xml"         },                                 \
    {   "message",      "CPIM"                  },                                 \
    {   "message",      "sip"                   },                                 \
    {   "message",      "sipfrag"               },                                 \
    {   "multipart",    "alternative"           },                                 \
    {   "multipart",    "mixed"                 },                                 \
    {   "multipart",    "related"               },                                 \
    {   "multipart",    "signed"                },                                 \
    {   "application",  "xml"                   },                                 \
    {   "text",         "xml-smml"              },                                 \
    {   "audio",        "telephone-event"       },                                 \
    {   "",             ""                      }                                  \
};

#endif /* MXG_PRESCECORECFG_H */

