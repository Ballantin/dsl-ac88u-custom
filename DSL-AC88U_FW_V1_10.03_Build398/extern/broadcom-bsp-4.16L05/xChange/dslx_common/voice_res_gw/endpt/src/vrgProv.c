/****************************************************************************
*
* Copyright  2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: vrgProv.c
*
****************************************************************************
*    Description:
*
*     This file implements the endpoint provisioning layer
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <string.h>

#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgEndpt.h>
#include <vrgProv.h>
#include <vrgTone.h>
#include <vrgEndptCfg.h>
#include "vrgEndptGlobe.h"

#include <xchgAssert.h>
#include <vrgClass.h>

#include <hapi_pve.h>                  /* PVE Service */

#include <vrgRing.h>

/* ---- Public Variables ------------------------------------------------- */

/* imported defines from vrgEndpt.c */
#define OFFSET_CAL_DEFAULT   0
#define DTMF_SKEW_DEFAULT    2

/* MIB value for CNG detection */
#define MIB_CNG_DET_ENABLED      1
#define MIB_CNG_DET_DISABLED     2


/* ---- Private Variables, Constants and Types --------------------------- */


#define PROV_UNINITIALIZED_VALUE       0xBADDECAF
#define MAKESTRING(arg)     #arg

typedef struct PROV_ENTRY
{
   EPPROV            provItemId;
   char              provItemName[32];
   VRG_UINT32        provItemSize;
   VRG_BOOL          isCountrySpecific;
   /* NOTE: The default value type may need to be changed if a default value needs to be set that is not a VRG_UINT32 or subtype */
   VRG_UINT32        defaultValue;
   void*             provItemValue[VRG_ENDPT_CFG_NUM_ENDPT];
} PROV_ENTRY;


/*
** Main endpoint provisioning table.
*/
static PROV_ENTRY provTable[] =
{
   { EPPROV_R0Cadence,              MAKESTRING(EPPROV_R0Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R1Cadence,              MAKESTRING(EPPROV_R1Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R2Cadence,              MAKESTRING(EPPROV_R2Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R3Cadence,              MAKESTRING(EPPROV_R3Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R4Cadence,              MAKESTRING(EPPROV_R4Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R5Cadence,              MAKESTRING(EPPROV_R5Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R6Cadence,              MAKESTRING(EPPROV_R6Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_R7Cadence,              MAKESTRING(EPPROV_R7Cadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RGCadence,              MAKESTRING(EPPROV_RGCadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RSCadence,              MAKESTRING(EPPROV_RSCadence),          sizeof(VRG_RING_PROFILE),     VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_CallWtgDelay,           MAKESTRING(EPPROV_CallWtgDelay),       sizeof(VRG_UINT32),            VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_ToneBusy,               MAKESTRING(EPPROV_ToneBusy),           sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneConfirmation,       MAKESTRING(EPPROV_ToneConfirmation),   sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneDial,               MAKESTRING(EPPROV_ToneDial),           sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneSpecialDial,        MAKESTRING(EPPROV_ToneSpecialDial),    sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneMessageWaiting,     MAKESTRING(EPPROV_ToneMessageWaiting), sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneOffHookWarning,     MAKESTRING(EPPROV_ToneOffHookWarning), sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneRingBack,           MAKESTRING(EPPROV_ToneRingBack),       sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneReOrder,            MAKESTRING(EPPROV_ToneReOrder),        sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneStutterdial,        MAKESTRING(EPPROV_ToneStutterdial),    sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCallWaiting,        MAKESTRING(EPPROV_ToneCallWaiting),    sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCallWaiting1,       MAKESTRING(EPPROV_ToneCallWaiting1),   sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCallWaiting2,       MAKESTRING(EPPROV_ToneCallWaiting2),   sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCallWaiting3,       MAKESTRING(EPPROV_ToneCallWaiting3),   sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCallWaiting4,       MAKESTRING(EPPROV_ToneCallWaiting4),   sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCongestion,         MAKESTRING(EPPROV_ToneCongestion),     sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneCongestion1,        MAKESTRING(EPPROV_ToneCongestion),     sizeof(VRG_TONE_SPEC),         VRG_TRUE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined1,       MAKESTRING(EPPROV_ToneUserDefined1),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined2,       MAKESTRING(EPPROV_ToneUserDefined2),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined3,       MAKESTRING(EPPROV_ToneUserDefined3),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined4,       MAKESTRING(EPPROV_ToneUserDefined4),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined5,       MAKESTRING(EPPROV_ToneUserDefined4),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_ToneUserDefined6,       MAKESTRING(EPPROV_ToneUserDefined4),   sizeof(VRG_TONE_SPEC),        VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CallingCard,            MAKESTRING(EPPROV_CallingCard),         sizeof(VRG_TONE_SPEC),       VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_MinHookFlash,           MAKESTRING(EPPROV_MinHookFlash),       sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_MaxHookFlash,           MAKESTRING(EPPROV_MaxHookFlash),       sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PlsDlInterdigitTime,    MAKESTRING(EPPROV_PlsDlInterdigitTime),sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PlsDlMinMakeTime,       MAKESTRING(EPPROV_PlsDlMinMakeTime),   sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PlsDlMaxMakeTime,       MAKESTRING(EPPROV_PlsDlMaxMakeTime),   sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PlsDlMinBreakTime,      MAKESTRING(EPPROV_PlsDlMinBreakTime),  sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PlsDlMaxBreakTime,      MAKESTRING(EPPROV_PlsDlMaxBreakTime),  sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDMode,                MAKESTRING(EPPROV_CIDMode),            sizeof(int),                  VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDFskAfterRing,        MAKESTRING(EPPROV_CIDFskAfterRing),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDFskAfterDTAS,        MAKESTRING(EPPROV_CIDFskAfterDTAS),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDFskAfterRPAS,        MAKESTRING(EPPROV_CIDFskAfterRPAS),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDRingAfterFSK,        MAKESTRING(EPPROV_CIDRingAfterFSK),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDDTASAfterLR,         MAKESTRING(EPPROV_CIDDTASAfterLR),     sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDSigProtocol,         MAKESTRING(EPPROV_CIDSigProtocol),     sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_MinConnect,             MAKESTRING(EPPROV_MinConnect),         sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_MinDisconnect,          MAKESTRING(EPPROV_MinDisconnect),      sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_VMWIMode,               MAKESTRING(EPPROV_VMWIMode),           sizeof(int),                  VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_VMWIFskAfterDTAS,       MAKESTRING(EPPROV_VMWIFskAfterDTAS),   sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_VMWIFskAfterRPAS,       MAKESTRING(EPPROV_VMWIFskAfterRPAS),   sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_VMWIDTASAfterLR,        MAKESTRING(EPPROV_VMWIDTASAfterLR),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_AnalogLoopback,         MAKESTRING(EPPROV_AnalogLoopback),     sizeof(int),                  VRG_FALSE,    0,                                       {NULL} },
#if VRG_ENDPT_CFG_USE_TPD
   { EPPROV_DiagTestsStart,         MAKESTRING(EPPROV_DiagTestsStart),     sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DiagTestsStop,          MAKESTRING(EPPROV_DiagTestsStop),      sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DiagTestsValid,         MAKESTRING(EPPROV_DiagTestsValid),     sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DiagTestsResult,        MAKESTRING(EPPROV_DiagTestsResult),    sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DiagTestsValue,         MAKESTRING(EPPROV_DiagTestsValue),     MAX_PROV_PARM_SIZE,           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DiagTestsFault,         MAKESTRING(EPPROV_DiagTestsFault),     sizeof(int),                  VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
#endif /* VRG_ENDPT_CFG_USE_TPD */

   { EPPROV_BoostedLoopCurrent,     MAKESTRING(EPPROV_BoostedLoopCurrent), sizeof(VRG_UINT32),           VRG_FALSE,    0,                                       {NULL} },
   { EPPROV_VoiceJitterBuffFixed,   MAKESTRING(EPPROV_VoiceJitterBuffFixed),sizeof(int),                 VRG_FALSE,    (VRG_UINT32)VRG_FALSE,                   {NULL} },
   { EPPROV_VoiceJitterBuffMin,     MAKESTRING(EPPROV_VoiceJitterBuffMin), sizeof(int),                  VRG_FALSE,    HAPI_PVE_MIN_JITTER_REG_DEFAULT,         {NULL} },
   { EPPROV_VoiceJitterBuffMax,     MAKESTRING(EPPROV_VoiceJitterBuffMax), sizeof(int),                  VRG_FALSE,    HAPI_PVE_MAX_JITTER_REG_DEFAULT,         {NULL} },
   { EPPROV_VoiceJitterBuffTarget,  MAKESTRING(EPPROV_VoiceJitterBuffTarget),sizeof(int),                VRG_FALSE,    HAPI_PVE_TARGET_JITTER_REG_DEFAULT,      {NULL} },
   { EPPROV_DataJitterBuffTarget,   MAKESTRING(EPPROV_DataJitterBuffTarget),sizeof(int),                 VRG_FALSE,    HAPI_PVE_VBD_TARGET_JITTER_REG_DEFAULT,  {NULL} },
   { EPPROV_TxGain,                 MAKESTRING(EPPROV_TxGain),             sizeof(VRG_SINT32),           VRG_FALSE,    (VRG_UINT32)((VRG_SINT32)0),             {NULL} },
   { EPPROV_RxGain,                 MAKESTRING(EPPROV_RxGain),             sizeof(VRG_SINT32),           VRG_FALSE,    (VRG_UINT32)((VRG_SINT32)0),             {NULL} },
   { EPPROV_GR303Support,           MAKESTRING(EPPROV_GR303Support),       sizeof(VRG_BOOL),             VRG_FALSE,    (VRG_UINT32)VRG_FALSE,                   {NULL} },

   { EPPROV_Pulse2DtmfConvert,      MAKESTRING(EPPROV_Pulse2DtmfConvert),  sizeof(VRG_BOOL),             VRG_FALSE,    (VRG_UINT32)VRG_FALSE,                   {NULL} },

   { EPPROV_MeteringFrequency,      MAKESTRING(EPPROV_MeteringFrequency), sizeof(VRG_UINT32),            VRG_FALSE,    PULSE_SIGNAL_FREQ_16KHZ,                 {NULL} },

   { EPPROV_FaxDetection,           MAKESTRING(EPPROV_FaxDetection),       sizeof(int),                  VRG_FALSE,    MIB_CNG_DET_ENABLED,                     {NULL} },
   { EPPROV_DTMFPlayout,            MAKESTRING(EPPROV_DTMFPlayout),        sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DtmfDbLevel,            MAKESTRING(EPPROV_DtmfDbLevel),        sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DtmfPulseTime,          MAKESTRING(EPPROV_DtmfPulseTime),      sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_DTMFSkew,               MAKESTRING(EPPROV_DTMFSkew),           sizeof(VRG_UINT32),           VRG_FALSE,    (VRG_UINT32)DTMF_SKEW_DEFAULT,           {NULL} },

   { EPPROV_Rfc2833ToneLevel,       MAKESTRING(EPPROV_Rfc2833ToneLevel),   sizeof(VRG_UINT32),           VRG_FALSE,    64,                                      {NULL} },

#if VRG_ENDPT_CFG_USE_TPD
   { EPPROV_PcDiagTestsStart,       MAKESTRING(EPPROV_PcDiagTestsStart),   sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PcDiagTestsClear,       MAKESTRING(EPPROV_PcDiagTestsClear),   sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PcDiagTestsValid,       MAKESTRING(EPPROV_PcDiagTestsValid),   sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PcDiagTestsResult,      MAKESTRING(EPPROV_PcDiagTestsResult),  MAX_PROV_PARM_SIZE,           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
#endif /* VRG_ENDPT_CFG_USE_TPD */

   { EPPROV_HighVring,              MAKESTRING(EPPROV_HighVring),          sizeof(VRG_UINT32),           VRG_FALSE,     (VRG_UINT32)VRG_FALSE,                  {NULL} },
   { EPPROV_PowerRingFrequency,     MAKESTRING(EPPROV_PowerRingFrequency), sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RingWaveform,           MAKESTRING(EPPROV_RingWaveform),       sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RingVoltage ,           MAKESTRING(EPPROV_RingVoltage),        sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RingDCOffset,           MAKESTRING(EPPROV_RingDCOffset),       sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_RingOffsetCal,          MAKESTRING(EPPROV_RingOffsetCal),      sizeof(VRG_UINT32),           VRG_FALSE,    OFFSET_CAL_DEFAULT,                      {NULL} },
   { EPPROV_LoadBalancing,          MAKESTRING(EPPROV_LoadBalancing),      sizeof(VRG_UINT32),           VRG_FALSE,    (VRG_UINT32)VRG_FALSE,                   {NULL} },
   { EPPROV_LineTestInProgress,     MAKESTRING(EPPROV_LineTestInProgress), sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_LineTestDone,           MAKESTRING(EPPROV_LineTestDone),       sizeof(VRG_UINT32),           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_Flexicalc,              MAKESTRING(EPPROV_Flexicalc),          MAX_PROV_PARM_SIZE,           VRG_FALSE,    PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PreferredVBDCodec,      MAKESTRING(EPPROV_PreferredVBDCodec),  sizeof(int),                  VRG_FALSE,    HAPI_PKT_VC_G711_ALAW,                   {NULL} },

   { EPPROV_CIDFskParms,            MAKESTRING(EPPROV_CIDFskParms),        sizeof(VRG_CLASS_FSK),        VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CIDDtmfParms,           MAKESTRING(EPPROV_CIDDtmfParms),       sizeof(VRG_CLASS_DTMF),       VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PteDetectAdmin,         MAKESTRING(EPPROV_PteDetectAdmin),     sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PteMinDetectPower,      MAKESTRING(EPPROV_PteMinDetectPower),  sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_PveEgressVolume,        MAKESTRING(EPPROV_PveEgressVolume),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_CngLevel,               MAKESTRING(EPPROV_CngLevel),           sizeof(VRG_UINT32),           VRG_FALSE,    0,                                       {NULL} },
   { EPPROV_VMWISigProtocol,        MAKESTRING(EPPROV_VMWISigProtocol),    sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },
   { EPPROV_VMWIMsgFormat,          MAKESTRING(EPPROV_VMWIMsgFormat),      sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },

   { EPPROV_LastLocale,             MAKESTRING(EPPROV_LastLocale),         sizeof(VRG_UINT32),           VRG_TRUE,     PROV_UNINITIALIZED_VALUE,                {NULL} },

   /* egress dtmf detection feature */
   { EPPROV_EgressDtmfDetection,    MAKESTRING(EPPROV_EgressDtmfDetection),sizeof(VRG_BOOL),             VRG_FALSE,    (VRG_UINT32)VRG_FALSE,                   {NULL} },
   /*
   ** The following items are currently not used in the endpoint code so
   ** setting the value does not have any effect on the operation.
   ** For this reason they are currently commented out.
   */

   // { EPPROV_RTCadence,              sizeof(VRG_RING_PROFILE)           {NULL} },
   // { EPPROV_ToneAlertingSignal,     sizeof(VRG_TONE_SPEC),        {NULL} },
   // { EPPROV_ToneSpecialInfo,        sizeof(VRG_TONE_SPEC),        {NULL} },
   // { EPPROV_ToneRelease,            sizeof(VRG_TONE_SPEC),        {NULL} },
   // { EPPROV_ToneRelease,            sizeof(VRG_TONE_SPEC),        {NULL} },
   // { EPPROV_PulseSignalEntry,       sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_RingCadenceEntry,       sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_StandardRingCadence,    sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_RingSplashCadence,      sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_RingWithDCOffset,       sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_DataJitterBuffMax,      sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_DtmfToneRelay,          sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_ClidDateMissingAction,  sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_InitRing,               sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_LoopClose,              sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_LoopOpen,               sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_EnableMetering,         sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_MeteringBurst,          sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_NoBattery,              sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_NormalPolarity,         sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_ReducedBattery,         sizeof(VRG_UINT32),                    {NULL} },
   // { EPPROV_ReversePolarity,        sizeof(VRG_UINT32),                    {NULL} },

   /* Last item to indicate the end of the table */
   { EPPROV_LastItem,               MAKESTRING(EPPROV_LastItem),           0,                            VRG_FALSE,     PROV_UNINITIALIZED_VALUE,              {NULL} }
};

static int totalAlocatedMemory = 0;

/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   endptProvInit
**
** PURPOSE:    Initialize the endpoint provisioning layer
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvInit(void)
{
   int line;

   for ( line = 0; line < VRG_ENDPT_CFG_NUM_ENDPT; line++ )
   {
      PROV_ENTRY* pTable = provTable;

      /* Allocate memory for the provisionable items */
      while ( pTable->provItemId != EPPROV_LastItem )
      {
         pTable->provItemValue[line] = malloc( pTable->provItemSize );
         XCHG_ASSERT( pTable->provItemValue[line] != NULL );
         totalAlocatedMemory += pTable->provItemSize;

         /* Set the content of the value field to 0 except for the
         ** first 4 bytes that are set to PROV_UNINITIALIZED_VALUE */
         memset( pTable->provItemValue[line], 0, pTable->provItemSize );
         *((VRG_UINT32*)pTable->provItemValue[line]) = PROV_UNINITIALIZED_VALUE;

#if 0
         VRG_LOG_DBG(( VRG_LOG_MOD_PROV,
                       "ALLOC item = %04u location = 0x%08X size = %lu line = %d",
                       pTable->provItemId,
                       (unsigned int)pTable->provItemValue[line],
                       pTable->provItemSize,
                       line ));
#endif

         pTable++;
      }
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "Allocated memory = %d ", totalAlocatedMemory ));

   return ( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   endptProvDeinit
**
** PURPOSE:    Deinitialize the endpoint provisioning layer
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvDeinit(void)
{
   int line;

   for ( line = 0; line < VRG_ENDPT_CFG_NUM_ENDPT; line++ )
   {
      PROV_ENTRY* pTable = provTable;

      /* Deallocate memory for the provisionable items */
      while ( pTable->provItemId != EPPROV_LastItem )
      {
         if ( pTable->provItemValue[line] != NULL )
         {
            free( pTable->provItemValue[line] );
            pTable->provItemValue[line] = NULL;
         }
         pTable++;
      }
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "Deallocated prov memory" ));

   totalAlocatedMemory = 0;

   return ( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   endptProvSetValue
**
** PURPOSE:    Set value into the endpoint provisioning subsystem
**
** PARAMETERS: line        -  [IN]  Line id (0-based)
**             itemId      -  [IN]  Id of the provisioning item (based on the EPPROV enum)
**             itemValue   -  [IN]  Pointer to the value that is passed in
**             itemLength  -  [IN]  Pointer to the length of the value that is passed in
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvSetValue( int line, EPPROV itemId, void *itemValue, int itemLength )
{
   PROV_ENTRY* pTable = provTable;

   if ( (itemValue == NULL) || (itemLength == 0) )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "NULL pointer passed to %s", __FUNCTION__ ));
      return ( EPSTATUS_PROV_FAILURE );
   }

   if ( line < 0 || line >= VRG_ENDPT_CFG_NUM_ENDPT )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "Trying to set a value for invalid line %d", line ));
      return ( EPSTATUS_PROV_FAILURE );
   }

   while ( (pTable->provItemId != itemId) && (pTable->provItemId != EPPROV_LastItem) )
   {
      pTable++;
   }

   if ( pTable->provItemId != EPPROV_LastItem )
   {
      if ( itemLength > pTable->provItemSize )
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_PROV,
                        "Requested size for SET = %d bytes. Allowed for EPPROV parameter %d = %u bytes",
                        itemLength,
                        pTable->provItemId,
                        (unsigned int)pTable->provItemSize
                     ));
         VRG_LOG_WARN(( VRG_LOG_MOD_PROV, "Setting only %u bytes.", (unsigned int)pTable->provItemSize ));
      }

      if ( pTable->provItemValue[line] != NULL )
      {
#if 0
         VRG_LOG_DBG(( VRG_LOG_MOD_PROV,
                       "SET item = %03u location = 0x%08X value (first 4 bytes) = 0x%08X size = %lu line = %d",
                       itemId,
                       (unsigned int)pTable->provItemValue[line],
                       (unsigned int)*((VRG_UINT32*)itemValue),
                       pTable->provItemSize,
                       line ));
#endif

         memcpy( pTable->provItemValue[line], itemValue, pTable->provItemSize );
      }
      else
      {
         /* NULL pointer for the variable storage.
         ** Check if the provisioning module is initialized */
         VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "NULL pointer provided for variable storage" ));
         return ( EPSTATUS_PROV_FAILURE );
      }

      return ( EPSTATUS_SUCCESS );
   }
   else
   {
      /* Couldn't find item in the endpoint provisioning table */
      VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "No item %d in the endpoint provisioning table", itemId ));
      return ( EPSTATUS_PROV_FAILURE );
   }
}


/*
*****************************************************************************
** FUNCTION:   endptProvGetValue
**
** PURPOSE:    Get value from the endpoint provisioning subsystem
**
** PARAMETERS: line        -  [IN]  Line id (0-based)
**             itemId      -  [IN]  Id of the provisioning item (based on the EPPROV enum)
**             itemValue   -  [OUT  Pointer to the value that needs to be set
**                                  (note it is assumed that adequate memory was
**                                  allocated by the caller)
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvGetValue( int line, EPPROV itemId, void *itemValue )
{
   PROV_ENTRY* pTable = provTable;

   if ( itemValue == NULL )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "NULL pointer passed to %s", __FUNCTION__ ));
      return ( EPSTATUS_PROV_FAILURE );
   }

   if ( line < 0 || line >= VRG_ENDPT_CFG_NUM_ENDPT )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "Trying to get a value for invalid line %d", line ));
      return ( EPSTATUS_PROV_FAILURE );
   }

   while ( (pTable->provItemId != itemId) && (pTable->provItemId != EPPROV_LastItem) )
   {
      pTable++;
   }

   if ( pTable->provItemId != EPPROV_LastItem )
   {
      if ( pTable->provItemValue[line] != NULL )
      {
         VRG_UINT32 value = *((VRG_UINT32*)pTable->provItemValue[line]);

#if 0
         VRG_LOG_DBG(( VRG_LOG_MOD_PROV,
                       "GET item = %03u location = 0x%08X value (first 4 bytes) = 0x%08X size = %lu line = %d",
                       itemId,
                       (unsigned int)pTable->provItemValue[line],
                       (unsigned int)value,
                       pTable->provItemSize,
                       line ));
#endif

         if ( value != PROV_UNINITIALIZED_VALUE )
         {
            memcpy( itemValue, pTable->provItemValue[line], pTable->provItemSize );
            return ( EPSTATUS_SUCCESS );
         }
         else
         {
            //VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "Item %03u uninitialized", itemId ));
            return ( EPSTATUS_PROV_FAILURE );
         }
      }
      else
      {
         /* NULL pointer for the variable storage.
         ** Check if the provisioning module is initialized */
         VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "NULL pointer provided for variable storage" ));
         return ( EPSTATUS_PROV_FAILURE );
      }

      return ( EPSTATUS_SUCCESS );
   }
   else
   {
      /* Couldn't find item in the endpoint provisioning table */
      VRG_LOG_DBG(( VRG_LOG_MOD_PROV, "No item %d in the endpoint provisioning table", itemId ));
      return ( EPSTATUS_PROV_FAILURE );
   }

}


/*
*****************************************************************************
** FUNCTION:   endptProvShow
**
** PURPOSE:    Print out the endpoint provisioning table
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvShow( void )
{
   int line;

   VRG_LOG_INFO(( VRG_LOG_MOD_PROV, " Line | Id    | Name                         | Size | Address   | Value (first 4 bytes)" ));
   for ( line = 0; line < vrgEndptGetNumEndpoints(); line++ )
   {
      PROV_ENTRY* pTable = provTable;
      while ( pTable->provItemId != EPPROV_LastItem )
      {
         unsigned char* p = pTable->provItemValue[line];
         VRG_LOG_INFO(( VRG_LOG_MOD_PROV, " %-4u | %-4u  | %-28s | %-4u | 0x%08X| %02X %02X %02X %02X",
                        line,
                        pTable->provItemId,
                        pTable->provItemName,
                        (unsigned int)pTable->provItemSize,
                        (unsigned int)p,
                        *p, *(p+1), *(p+2), *(p+3) ));
         pTable++;
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   endptProvIntegrityCheck
**
** PURPOSE:    Go through provision table item list to make sure all paramters
**             are configured
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvIntegrityCheck( void )
{
   int line;

   for ( line = 0; line < VRG_ENDPT_CFG_NUM_ENDPT; line++ )
   {
      PROV_ENTRY* pTable = provTable;
      while ( pTable->provItemId != EPPROV_LastItem )
      {
         if( *((VRG_UINT32*)pTable->provItemValue[line]) == PROV_UNINITIALIZED_VALUE )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "ERROR provision item is not configured\n%-4u | %-4u  | %-28s | %-4u",
                            line,
                            pTable->provItemId,
                            pTable->provItemName,
                            (unsigned int)pTable->provItemSize ));
            return EPSTATUS_PROV_FAILURE;
         }
         pTable++;
      }
   }
   return EPSTATUS_SUCCESS;
}

/*
*****************************************************************************
** FUNCTION:   endptProvInitDefaults
**
** PURPOSE:    Go through provision table item list to make sure all parameters
**             are configured. If they aren't, set default values
**
** PARAMETERS: [IN] country code (VRG_COUNTRY)
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvInitDefaults(VRG_COUNTRY country)
{
   int line;


   for ( line = 0; line < vrgEndptGetNumEndpoints(); line++ )
   {
      PROV_ENTRY* pTable = provTable;
      while ( pTable->provItemId != EPPROV_LastItem )
      {
        /*  check to see if there is a default value to use and nothing already initialized */
      if(pTable->defaultValue != PROV_UNINITIALIZED_VALUE && *((VRG_UINT32*)pTable->provItemValue[line]) == PROV_UNINITIALIZED_VALUE )
         {
            /* directly copy the default value */
            memcpy(pTable->provItemValue[line], &pTable->defaultValue, pTable->provItemSize);
         }
 
         
         /* An error log will pop up if the locale provItemValue is uninitialized */
        else if( *((VRG_UINT32*)pTable->provItemValue[line]) == PROV_UNINITIALIZED_VALUE && (pTable->isCountrySpecific == VRG_TRUE) )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_PROV, "ERROR provision item is not configured\n%-4u | %-4u  | %-28s | %-4u",
                            line,
                            pTable->provItemId,
                            pTable->provItemName,
                            (unsigned int)pTable->provItemSize ));
         }

         pTable++;
      }
   }

   return ( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   endptProvResetDefaults
**
** PURPOSE:    Go through provision table item list to make sure all country
**             specific parameters are defaulted.
**
** PARAMETERS: [IN] country code (VRG_COUNTRY)
**
** RETURNS:    EPSTATUS_SUCESS on success
**
*****************************************************************************
*/
EPSTATUS endptProvResetDefaults(VRG_COUNTRY country)
{
   int line;


   for ( line = 0; line < vrgEndptGetNumEndpoints(); line++ )
   {
      PROV_ENTRY* pTable = provTable;
      while ( pTable->provItemId != EPPROV_LastItem )
      {
         /* if we have a non-country-specific default value, reset that too */
       if(pTable->defaultValue != PROV_UNINITIALIZED_VALUE)
         {
            /* directly copy the default value */
            memcpy(pTable->provItemValue[line], &pTable->defaultValue, pTable->provItemSize);
         }

         pTable++;
      }
   }

   return ( EPSTATUS_SUCCESS );
}
