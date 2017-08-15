/**********************************************************************************
** Copyright (c) 2008-2013 Broadcom Corporation
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
**      This module implements mapping function of any kind necessary for usage
**      of features within the Call Manager.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define    CMMAP__INVALID           "Invalid Map!"
#define    CMMAP__EVENT_UNMAPPED    '?'

#define    _STR( arg ) { arg, (BOS_UINT8 *)#arg }


/* ---- Private Variables ------------------------------------------------------- */

/* Mapping the Net events to CM events.
*/
CMMAPID cmMapNetEvt2CmEvt[] =
{
   { eCMNETEVT_NEWCALL,                      eCMEVT_INCOMING_CALL },
   { eCMNETEVT_ENDCALL,                      eCMEVT_CLEANED_CALL },
   { eCMNETEVT_ALERT,                        eCMEVT_ALERT_CALL },
   { eCMNETEVT_ALERTED,                      eCMEVT_RINGING_CALL },
   { eCMNETEVT_ANSWER,                       eCMEVT_CONNECT_CALL },
   { eCMNETEVT_DISCONNECT,                   eCMEVT_RELEASE_CALL },
   { eCMNETEVT_BUSY,                         eCMEVT_BUSY_CALL },
   { CMGR_INVALID_HANDLE,                   CMGR_INVALID_HANDLE }
};


/* Mapping the physical (endpoint) events to CM events.
*/
CMMAPID cmMapPhysEvt2CmEvt[] =
{
   { eEPTEVT_ONHOOK,              eCMEVT_ONHOOK },
   { eEPTEVT_OFFHOOK,             eCMEVT_OFFHOOK },
   { eEPTEVT_FLASH,               eCMEVT_FLASH },
   { eEPTEVT_MODEM,               eCMEVT_MODEM_TONE },
   { eEPTEVT_FAX,                 eCMEVT_FAX_TONE },
   { eEPTEVT_T38_STOP,            eCMEVT_FAX_END },
   { eEPTEVT_T38_FAILURE,         eCMEVT_FAX_FAIL },
   { eEPTEVT_DTMF0,               eCMEVT_DTMF_0 },
   { eEPTEVT_DTMF1,               eCMEVT_DTMF_1 },
   { eEPTEVT_DTMF2,               eCMEVT_DTMF_2 },
   { eEPTEVT_DTMF3,               eCMEVT_DTMF_3 },
   { eEPTEVT_DTMF4,               eCMEVT_DTMF_4 },
   { eEPTEVT_DTMF5,               eCMEVT_DTMF_5 },
   { eEPTEVT_DTMF6,               eCMEVT_DTMF_6 },
   { eEPTEVT_DTMF7,               eCMEVT_DTMF_7 },
   { eEPTEVT_DTMF8,               eCMEVT_DTMF_8 },
   { eEPTEVT_DTMF9,               eCMEVT_DTMF_9 },
   { eEPTEVT_DTMFS,               eCMEVT_DTMF_STAR },
   { eEPTEVT_DTMFH,               eCMEVT_DTMF_HASH },
   { eEPTEVT_DTMFA,               eCMEVT_DTMF_A },
   { eEPTEVT_DTMFB,               eCMEVT_DTMF_B },
   { eEPTEVT_DTMFC,               eCMEVT_DTMF_C },
   { eEPTEVT_DTMFD,               eCMEVT_DTMF_D },
   { eEPTEVT_SES_STATUS,          eCMEVT_SES_STATUS },
   { eEPTEVT_MEDIA_START,         eCMEVT_MEDIA },
   { eEPTEVT_MEDIA_END,           eCMEVT_MEDIA_END },
   { eEPTEVT_NO_ING_CNX_ACTIVITY, eCMEVT_NO_ACTIVITY },
   { eEPTEVT_NO_EG_CNX_ACTIVITY,  eCMEVT_NO_ACTIVITY },
   { eEPTEVT_INGRESS_DTMF,        eCMEVT_INGRESS_DTMF },

   { CMGR_INVALID_HANDLE,         CMGR_INVALID_HANDLE }
};

/* Mapping the tone to its equivalent with CLID support.
*/
CMMAPID cmMapTone2Clid[] =
{
   { eEPTSIG_RINGING,        eEPTSIG_CALLID_RINGING },
   { eEPTSIG_RING0,          eEPTSIG_CALLID_RING0 },
   { eEPTSIG_RING1,          eEPTSIG_CALLID_RING1 },
   { eEPTSIG_RING2,          eEPTSIG_CALLID_RING2 },
   { eEPTSIG_RING3,          eEPTSIG_CALLID_RING3 },
   { eEPTSIG_RING4,          eEPTSIG_CALLID_RING4 },
   { eEPTSIG_RING5,          eEPTSIG_CALLID_RING5 },
   { eEPTSIG_RING6,          eEPTSIG_CALLID_RING6 },
   { eEPTSIG_RING7,          eEPTSIG_CALLID_RING7 },
   { eEPTSIG_CALLW1,         eEPTSIG_SAS_CAS1 },
   { eEPTSIG_CALLW2,         eEPTSIG_SAS_CAS2 },
   { eEPTSIG_CALLW3,         eEPTSIG_SAS_CAS3 },
   { eEPTSIG_CALLW4,         eEPTSIG_SAS_CAS4 },

   { CMGR_INVALID_HANDLE,    CMGR_INVALID_HANDLE }
};


/* Mapping a ring tone to an equivalent call waiting tone.
*/
CMMAPID cmMapToneRing2CallWait[] =
{
   { eEPTSIG_RING0,          eEPTSIG_CALLW1 },
   { eEPTSIG_RING1,          eEPTSIG_CALLW1 },
   { eEPTSIG_RING2,          eEPTSIG_CALLW2 },
   { eEPTSIG_RING3,          eEPTSIG_CALLW3 },
   { eEPTSIG_RING4,          eEPTSIG_CALLW4 },
   { eEPTSIG_RING5,          eEPTSIG_CALLW1 },
   { eEPTSIG_RING6,          eEPTSIG_CALLW1 },
   { eEPTSIG_RING7,          eEPTSIG_CALLW1 },

   { CMGR_INVALID_HANDLE,    CMGR_INVALID_HANDLE }
};

/* Mapping the CCTK events to CM events.
*/
CMMAPID cmMapCfgCodec2CctkCodec[] =
{
   { eCFG_CODECTYPE_OTHER,        eCCTKCODEC_MAX },
   { eCFG_CODECTYPE_PCMU,         eCCTKCODEC_PCMU },
   { eCFG_CODECTYPE_PCMA,         eCCTKCODEC_PCMA },
   { eCFG_CODECTYPE_G711_LINEAR,  eCCTKCODEC_MAX },
   { eCFG_CODECTYPE_LINPCM,       eCCTKCODEC_MAX },
   { eCFG_CODECTYPE_G729,         eCCTKCODEC_G729 },
   { eCFG_CODECTYPE_G729E,        eCCTKCODEC_G729E },
   { eCFG_CODECTYPE_G728,         eCCTKCODEC_G728 },
   { eCFG_CODECTYPE_G726AT16,     eCCTKCODEC_G726_16 },
   { eCFG_CODECTYPE_G726AT24,     eCCTKCODEC_G726_24 },
   { eCFG_CODECTYPE_G726AT32,     eCCTKCODEC_G726_32 },
   { eCFG_CODECTYPE_G726AT40,     eCCTKCODEC_G726_40 },
   { eCFG_CODECTYPE_G722,         eCCTKCODEC_G722 },
   { eCFG_CODECTYPE_G722AT48,     eCCTKCODEC_G722_48 },
   { eCFG_CODECTYPE_G722AT56,     eCCTKCODEC_G722_56 },
   { eCFG_CODECTYPE_G722AT64,     eCCTKCODEC_G722_64 },
   { eCFG_CODECTYPE_G723,         eCCTKCODEC_G7231 },
   { eCFG_CODECTYPE_ILBC,         eCCTKCODEC_ILBC },
   { eCFG_CODECTYPE_BV16,         eCCTKCODEC_BV16 },
   { eCFG_CODECTYPE_BV32,         eCCTKCODEC_BV32 },
   { eCFG_CODECTYPE_GSMAMR,       eCCTKCODEC_GSMAMR },
   { eCFG_CODECTYPE_GSMEFR,       eCCTKCODEC_GSMEFR },
   { eCFG_CODECTYPE_AMRWB,        eCCTKCODEC_AMRWB },
   { eCFG_CODECTYPE_G7221,        eCCTKCODEC_G7221 },
   { eCFG_CODECTYPE_LINPCMWB,     eCCTKCODEC_LPCMWB },
#if CMGRCFG_RED
   { eCFG_CODECTYPE_RED,	  eCCTKCODEC_RED},
#endif
   { eCFG_CODECTYPE_UNKNOWN,      eCCTKCODEC_MAX },
#if CMGRCFG_VIDEO_MEDIA
   { eCFG_CODECVIDTYPE_H263,      eCCTKCODEC_H263 },
   { eCFG_CODECVIDTYPE_H264,      eCCTKCODEC_H264 },
   { eCFG_CODECVIDTYPE_MPEG4,     eCCTKCODEC_MPEG4 },
   { eCFG_CODECVIDTYPE_UNKNOWN,   eCCTKCODEC_MAX },
#endif
   { CMGR_INVALID_HANDLE,         eCCTKCODEC_MAX }
};


#if CMGRCFG_VIDEO_MEDIA
/* Mapping og the H.264 level.
*/
CMMAPID cmMapCfgH264Lvl2CctkH264Lvl[] =
{
   { 10,                  eCCTKH264LEVEL_1_0 },
   { 11,                  eCCTKH264LEVEL_1_1 },
   { 12,                  eCCTKH264LEVEL_1_2 },
   { 13,                  eCCTKH264LEVEL_1_3 },
   { 20,                  eCCTKH264LEVEL_2_0 },
   { 21,                  eCCTKH264LEVEL_2_1 },
   { 22,                  eCCTKH264LEVEL_2_2 },
   { 30,                  eCCTKH264LEVEL_3_0 },
   { 31,                  eCCTKH264LEVEL_3_1 },
   { 32,                  eCCTKH264LEVEL_3_2 },
   { 40,                  eCCTKH264LEVEL_4_0 },
   { 41,                  eCCTKH264LEVEL_4_1 },
   { 42,                  eCCTKH264LEVEL_4_2 },
   { 50,                  eCCTKH264LEVEL_5_0 },
   { 51,                  eCCTKH264LEVEL_5_1 },

   { CMGR_INVALID_HANDLE, eCCTKH264LEVEL_1_2 }
};


/* Mapping of the H.264 packetization mode.
*/
CMMAPID cmMapCfgH264PktMode2CctkH264PktMode[] =
{
   { 0,                   eCCTKH264PACKETIZATIONMODE_SINGLE_NAL },
   { 1,                   eCCTKH264PACKETIZATIONMODE_NON_INTERLEAVED },
   { 2,                   eCCTKH264PACKETIZATIONMODE_INTERLEAVED },

   { CMGR_INVALID_HANDLE, eCCTKH264PACKETIZATIONMODE_SINGLE_NAL }
};


/* Mapping of the H.264 packetization mode.
*/
CMMAPID cmMapCfgH264Prof2CctkH264Prof[] =
{
   { 66,                  eCCTKH264PROFILE_BASELINE },
   { 77,                  eCCTKH264PROFILE_MAIN },
   { 88,                  eCCTKH264PROFILE_EXTENDED },

   { CMGR_INVALID_HANDLE, eCCTKH264PROFILE_BASELINE }
};
#endif


/* Mapping the CM state names.
*/
CMMAPSTR cmMapCmState[] =
{
   _STR( eCMST_IDLE ),
   _STR( eCMST_ACCEPTED ),
   _STR( eCMST_DIALING ),
   _STR( eCMST_RINGBACK ),
   _STR( eCMST_RINGING ),
   _STR( eCMST_TALK ),
   _STR( eCMST_FLASH ),
   _STR( eCMST_RELEASING ),
   _STR( eCMST_ERROR ),

   { CMGR_INVALID_HANDLE, (BOS_UINT8 *)"eCMST_INVALID" }
};

/* Mapping the CM event names.
*/
CMMAPSTR cmMapCmEvent[] =
{
   _STR( eCMEVT_INVALID ),
   _STR( eCMEVT_INCOMING_CALL ),
   _STR( eCMEVT_CLEANED_CALL ),
   _STR( eCMEVT_ALERT_CALL ),
   _STR( eCMEVT_RINGING_CALL ),
   _STR( eCMEVT_CONNECT_CALL ),
   _STR( eCMEVT_RELEASE_CALL ),
   _STR( eCMEVT_BUSY_CALL ),
   _STR( eCMEVT_ONHOOK ),
   _STR( eCMEVT_OFFHOOK ),
   _STR( eCMEVT_FLASH ),
   _STR( eCMEVT_MODEM_TONE ),
   _STR( eCMEVT_FAX_TONE ),
   _STR( eCMEVT_FAX_END ),
   _STR( eCMEVT_FAX_FAIL ),
   _STR( eCMEVT_DTMF_0 ),
   _STR( eCMEVT_DTMF_1 ),
   _STR( eCMEVT_DTMF_2 ),
   _STR( eCMEVT_DTMF_3 ),
   _STR( eCMEVT_DTMF_4 ),
   _STR( eCMEVT_DTMF_5 ),
   _STR( eCMEVT_DTMF_6 ),
   _STR( eCMEVT_DTMF_7 ),
   _STR( eCMEVT_DTMF_8 ),
   _STR( eCMEVT_DTMF_9 ),
   _STR( eCMEVT_DTMF_STAR ),
   _STR( eCMEVT_DTMF_HASH ),
   _STR( eCMEVT_DTMF_A ),
   _STR( eCMEVT_DTMF_B ),
   _STR( eCMEVT_DTMF_C ),
   _STR( eCMEVT_DTMF_D ),
   _STR( eCMEVT_SES_STATUS ),
   _STR( eCMEVT_MEDIA ),
   _STR( eCMEVT_MEDIA_DELAY ),
   _STR( eCMEVT_MEDIA_END ),
   _STR( eCMEVT_NO_ACTIVITY ),
   _STR( eCMEVT_HOLD_SVC ),
   _STR( eCMEVT_UNHOLD_SVC ),
   _STR( eCMEVT_CONF_SVC ),
   _STR( eCMEVT_BXFER_SVC ),
   _STR( eCMEVT_CXFER_SVC ),
   _STR( eCMEVT_INTCALL_SVC ),
   _STR( eCMEVT_ANSWER ),
   _STR( eCMEVT_ENDCALL ),
   _STR( eCMEVT_INGRESS_DTMF ),

   _STR( eCMEVT_TMR_ONHOOK ),
   _STR( eCMEVT_TMR_DIALTONE ),
   _STR( eCMEVT_TMR_RINGING ),
   _STR( eCMEVT_TMR_RINGBACK ),
   _STR( eCMEVT_TMR_INTERDIGIT_SHORT ),
   _STR( eCMEVT_TMR_INTERDIGIT_LONG ),
   _STR( eCMEVT_TMR_RENEW_SVC ),
   _STR( eCMEVT_TMR_WAITCONF_SVC ),
   _STR( eCMEVT_TMR_OHWARN ),
   _STR( eCMEVT_TMR_CALLWAIT ),
   _STR( eCMEVT_TMR_AUTOCALLBACK ),
   _STR( eCMEVT_TMR_KEYHELD ),
   _STR( eCMEVT_TMR_FAX_EVENT ),
   _STR( eCMEVT_TMR_OSI_DELAY ),
   _STR( eCMEVT_TMR_AUTORECOVER ),
   _STR( eCMEVT_TMR_DTMF ),

   { CMGR_INVALID_HANDLE, (BOS_UINT8 *)"eCMEVT_UNKNOWN" }
};

/* Mapping the digit map action names.
*/
CMMAPSTR cmMapDmAction[] =
{
   { eCMDM_ACRACTIVATE,            (BOS_UINT8 *)"ACR-ACTIVATE" },
   { eCMDM_ACRDEACTIVATE,          (BOS_UINT8 *)"ACR-DEACTIVATE" },
   { eCMDM_ACACTIVATE,             (BOS_UINT8 *)"AC-ACTIVATE" },
   { eCMDM_ACDEACTIVATE,           (BOS_UINT8 *)"AC-DEACTIVATE" },
   { eCMDM_ACTIVATEDCHECK,         (BOS_UINT8 *)"ACTIVATED-CHECK" },
   { eCMDM_ARACTIVATE,             (BOS_UINT8 *)"AR-ACTIVATE" },
   { eCMDM_ARDEACTIVATE,           (BOS_UINT8 *)"AR-DEACTIVATE" },
   { eCMDM_CIDSUPPRESS,            (BOS_UINT8 *)"CID-SUPPRESS" },
   { eCMDM_CIDDELIVER,             (BOS_UINT8 *)"CID-DELIVER" },
   { eCMDM_CFPROGRAM,              (BOS_UINT8 *)"CF-PROGRAM" }, 
   { eCMDM_CFACTIVATE,             (BOS_UINT8 *)"CF-ACTIVATE" }, 
   { eCMDM_CFDEACTIVATE,           (BOS_UINT8 *)"CF-DEACTIVATE" }, 
   { eCMDM_CBACTIVATEALL,          (BOS_UINT8 *)"CB-ACTIVATEALL" },
   { eCMDM_CBACTIVATEDM,           (BOS_UINT8 *)"CB-ACTIVATEDM" },
   { eCMDM_CBDEACTIVATE,           (BOS_UINT8 *)"CB-DEACTIVATE" },
   { eCMDM_CNDBTOGGLE,             (BOS_UINT8 *)"CNDB-TOGGLE" },
   { eCMDM_COTACTIVATE,            (BOS_UINT8 *)"COT-ACTIVATE" },
   { eCMDM_CWTOGGLE,               (BOS_UINT8 *)"CW-TOGGLE" },
   { eCMDM_DEACTIVATEDCHECK,       (BOS_UINT8 *)"DEACTIVATED-CHECK" },
   { eCMDM_DNDACTIVATE,            (BOS_UINT8 *)"DND-ACTIVATE" },
   { eCMDM_DNDDEACTIVATE,          (BOS_UINT8 *)"DND-DEACTIVATE" },
   { eCMDM_EMERGENCYCALL,          (BOS_UINT8 *)"EMERGENCY-CALL" },
   { eCMDM_FEATURECHECK,           (BOS_UINT8 *)"FEATURE-CHECK" },
   { eCMDM_HOLDACTIVATE,           (BOS_UINT8 *)"HOLD-ACTIVATE" },
   { eCMDM_HOTLINEINT,             (BOS_UINT8 *)"HOTLINE-INTERNAL" },
   { eCMDM_HOOKFLASHACTIVATE,      (BOS_UINT8 *)"HOOKFLASH-ACTIVATE" },
   { eCMDM_HOOKFLASHDEACTIVATE,    (BOS_UINT8 *)"HOOKFLASH-DEACTIVATE" },
   { eCMDM_MAKECALL,               (BOS_UINT8 *)"MAKE-CALL" },
   { eCMDM_MAKECALLINT,            (BOS_UINT8 *)"MAKE-INTERNAL-CALL" },
   { eCMDM_RECALL,                 (BOS_UINT8 *)"RECALL" },
   { eCMDM_REORDER,                (BOS_UINT8 *)"REORDER" },
   { eCMDM_RETURN,                 (BOS_UINT8 *)"RETURN" },
   { eCMDM_USEMAP,                 (BOS_UINT8 *)"USEMAP" },
   { eCMDM_WARMLINEINT,            (BOS_UINT8 *)"WARMLINE-INTERNAL" },
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
   { eCMDM_WARMLINE_DM_ACTIVATE,   (BOS_UINT8 *)"WARMLINE-ACTIVATE-VIA-DIIGITMAP" },
   { eCMDM_WARMLINE_DM_DEACTIVATE, (BOS_UINT8 *)"WARMLINE-DEACTIVATE-VIA-DIGITMAP" },
#endif 
   { CMGR_INVALID_HANDLE,          (BOS_UINT8 *)"INVALID-ACTION" }
};

/* Mapping the some FSM events to a corresponding digit value.
*/
CMMAPEVT cmMapCmEvt2DigCol[] =
{
   { eCMEVT_DTMF_0,                (BOS_UINT8)'0' },
   { eCMEVT_DTMF_1,                (BOS_UINT8)'1' },
   { eCMEVT_DTMF_2,                (BOS_UINT8)'2' },
   { eCMEVT_DTMF_3,                (BOS_UINT8)'3' },
   { eCMEVT_DTMF_4,                (BOS_UINT8)'4' },
   { eCMEVT_DTMF_5,                (BOS_UINT8)'5' },
   { eCMEVT_DTMF_6,                (BOS_UINT8)'6' },
   { eCMEVT_DTMF_7,                (BOS_UINT8)'7' },
   { eCMEVT_DTMF_8,                (BOS_UINT8)'8' },
   { eCMEVT_DTMF_9,                (BOS_UINT8)'9' },
   { eCMEVT_DTMF_STAR,             (BOS_UINT8)'*' },
   { eCMEVT_DTMF_HASH,             (BOS_UINT8)'#' },
   { eCMEVT_DTMF_A,                (BOS_UINT8)'A' },
   { eCMEVT_DTMF_B,                (BOS_UINT8)'B' },
   { eCMEVT_DTMF_C,                (BOS_UINT8)'C' },
   { eCMEVT_DTMF_D,                (BOS_UINT8)'B' },
   { eCMEVT_TMR_INTERDIGIT_SHORT,  (BOS_UINT8)CMDM_INTERDIGIT_TMR_SHORT },

   { eCMEVT_INVALID,               (BOS_UINT8)CMMAP__EVENT_UNMAPPED }
};

/* Mapping endpoint events to a corresponding digit value.
*/
CMMAPEVT cmMapEptSig2DigCol[] =
{
   { eEPTSIG_DTMF0,                (BOS_UINT8)'0' },
   { eEPTSIG_DTMF1,                (BOS_UINT8)'1' },
   { eEPTSIG_DTMF2,                (BOS_UINT8)'2' },
   { eEPTSIG_DTMF3,                (BOS_UINT8)'3' },
   { eEPTSIG_DTMF4,                (BOS_UINT8)'4' },
   { eEPTSIG_DTMF5,                (BOS_UINT8)'5' },
   { eEPTSIG_DTMF6,                (BOS_UINT8)'6' },
   { eEPTSIG_DTMF7,                (BOS_UINT8)'7' },
   { eEPTSIG_DTMF8,                (BOS_UINT8)'8' },
   { eEPTSIG_DTMF9,                (BOS_UINT8)'9' },
   { eEPTSIG_DTMFS,                (BOS_UINT8)'*' },
   { eEPTSIG_DTMFH,                (BOS_UINT8)'#' },

   { eCMEVT_INVALID,               (BOS_UINT8)CMMAP__EVENT_UNMAPPED }
};

/* Mapping of some pre-configured tone information to some tone value.
**
** Note: This mapping is based on the some IETF definition of how tones are
**       being assigned to a character string.
*/
CMMAPSTR cmMapCmTone2Name[] =
{
   { eEPTSIG_BUSY,                 (BOS_UINT8 *)"bz" },
   { eEPTSIG_CONF,                 (BOS_UINT8 *)"cf" },
   { eEPTSIG_DIAL,                 (BOS_UINT8 *)"dl" },
   { eEPTSIG_OHWARN,               (BOS_UINT8 *)"ot" },
   { eEPTSIG_RINGING,              (BOS_UINT8 *)"rg" },
   { eEPTSIG_RING0,                (BOS_UINT8 *)"r0" },
   { eEPTSIG_RING1,                (BOS_UINT8 *)"r1" },
   { eEPTSIG_RING2,                (BOS_UINT8 *)"r2" },
   { eEPTSIG_RING3,                (BOS_UINT8 *)"r3" },
   { eEPTSIG_RING4,                (BOS_UINT8 *)"r4" },
   { eEPTSIG_RING5,                (BOS_UINT8 *)"r5" },
   { eEPTSIG_RING6,                (BOS_UINT8 *)"r6" },
   { eEPTSIG_RING7,                (BOS_UINT8 *)"r7" },
   { eEPTSIG_REORDER,              (BOS_UINT8 *)"ro" },
   { eEPTSIG_SPLASH,               (BOS_UINT8 *)"rs" },
   { eEPTSIG_RINGBACK,             (BOS_UINT8 *)"rt" },
   { eEPTSIG_STUTTER,              (BOS_UINT8 *)"sl" },
   { eEPTSIG_CALLW1,               (BOS_UINT8 *)"wt1" },
   { eEPTSIG_CALLW2,               (BOS_UINT8 *)"wt2" },
   { eEPTSIG_CALLW3,               (BOS_UINT8 *)"wt3" },
   { eEPTSIG_CALLW4,               (BOS_UINT8 *)"wt4" },
   { eEPTSIG_OSI,                  (BOS_UINT8 *)"osi" },

   /* Special 'no-tone' tone used to prevent any actual
   ** tone from being played to the user(s).
   */
   { eEPTSIG_NULL,                 (BOS_UINT8 *)"nt" },

   { CMGR_INVALID_HANDLE,          (BOS_UINT8 *)"" }
};

/* Mapping the Call Waiting Tone Names.
*/
CMMAPSTR cmCwToneMap[] =
{
   { eEPTSIG_CALLW1,       (BOS_UINT8 *)"callwaitingtone1"  },
   { eEPTSIG_CALLW2,       (BOS_UINT8 *)"callwaitingtone2"  },
   { eEPTSIG_CALLW3,       (BOS_UINT8 *)"callwaitingtone3"  },
   { eEPTSIG_CALLW4,       (BOS_UINT8 *)"callwaitingtone4"  },
   { CMGR_INVALID_HANDLE,  (BOS_UINT8 *)"INVALID-CWT" }
};

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmMapGetId
**
** PURPOSE:    Maps an identified resource to another one in an ID based map.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT32 cmMapGetId ( CMMAPID *pMap, BOS_UINT32 uId )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;

   if ( pMap )
   {
      while ( (pMap->uId != CMGR_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      uRet = pMap->uMap;
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmMapGetStr
**
** PURPOSE:    Maps an identified resource to its human readable expression.
**
** PARAMETERS: pMap - the map to lookup.
**             uId - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, NULL pointer if no valid
**             mapping can be obtained.
***********************************************************************************/
BOS_UINT8 *cmMapGetStr ( CMMAPSTR *pMap, BOS_UINT32 uId )
{
   BOS_UINT8 *pRet = (BOS_UINT8 *)CMMAP__INVALID;

   if ( pMap )
   {
      while ( (pMap->uId != CMGR_INVALID_HANDLE) &&
              (pMap->uId != uId) )
      {
         pMap++;
      }
      pRet = pMap->pMap;
   }

   return pRet;
}


/***********************************************************************************
** FUNCTION:   cmMapGetEvt
**
** PURPOSE:    Maps an identified event to its human usable character value.
**
** PARAMETERS: pMap - the map to lookup.
**             eEvt - the identified event to lookup.
**
** RETURNS:    The mapped identifier if valid, '?' is no valid mapped event is
**             found.
***********************************************************************************/
BOS_UINT8 cmMapGetEvt ( CMMAPEVT *pMap, CMFSMEVT eEvt )
{
   BOS_UINT8 uRet = (BOS_UINT8)CMMAP__EVENT_UNMAPPED;

   if ( pMap )
   {
      while ( (pMap->eEvt != eCMEVT_INVALID) &&
              (pMap->eEvt != eEvt) )
      {
         pMap++;
      }
      uRet = pMap->uId;
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmMapGetIdFromStr
**
** PURPOSE:    Maps a human readable resource to its internal machine equivalent.
**
** PARAMETERS: pMap - the map to lookup.
**             puStr - the identified resource to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if the
**             mapping can not be obtained.
***********************************************************************************/
BOS_UINT32 cmMapGetIdFromStr ( CMMAPSTR *pMap, BOS_UINT8 *puStr )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;

   if ( pMap && puStr )
   {
      while ( (pMap->uId != CMGR_INVALID_HANDLE) &&
              strCmpNoCase( (const char *)pMap->pMap, (const char *)puStr ) )
      {
         pMap++;
      }
      uRet = pMap->uId;
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   cmMapGetEvtFromChar
**
** PURPOSE:    Maps a character to its event value.
**
** PARAMETERS: pMap  - the map to lookup.
**             uChar - the character to lookup.
**
** RETURNS:    The mapped identifier if valid, CMGR_INVALID_HANDLE if no valid
**             mapped event is found.
***********************************************************************************/
BOS_UINT32 cmMapGetEvtFromChar ( CMMAPEVT *pMap, BOS_UINT8 uChar )
{
   BOS_UINT32 uRet = CMGR_INVALID_HANDLE;

   if ( pMap )
   {
      while ( (pMap->eEvt != eCMEVT_INVALID) &&
              (pMap->uId != uChar) )
      {
         pMap++;
      }
      uRet = pMap->eEvt;
   }

   if( uRet == eCMEVT_INVALID )
   {
      uRet = CMGR_INVALID_HANDLE;
   }

   return uRet;
}
