/****************************************************************************
*  
*  Copyright © 2000-2008 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.  
*
*    Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: eptProv.h
*
****************************************************************************
*    Description:
*
*      Public endpoint interface for provisioned value access
*
****************************************************************************/

#ifndef EPT_PROV_H
#define EPT_PROV_H

#if defined(__cplusplus)
extern "C"
{
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <eptSig.h>

/* ---- Constants and Types ---------------------------------------------- */

/*
** Provisioned values
*/
typedef enum
{
   /* Ring cadence */
   eEPTPROV_R0Cadence = 0,
   eEPTPROV_R1Cadence,
   eEPTPROV_R2Cadence,
   eEPTPROV_R3Cadence,
   eEPTPROV_R4Cadence,
   eEPTPROV_R5Cadence,
   eEPTPROV_R6Cadence,
   eEPTPROV_R7Cadence,
   eEPTPROV_RGCadence,
   eEPTPROV_RSCadence,
   eEPTPROV_RTCadence,

   /* Tones */
   eEPTPROV_ToneBusy,
   eEPTPROV_ToneConfirmation,
   eEPTPROV_ToneDial,
   eEPTPROV_ToneMessageWaiting,
   eEPTPROV_ToneOffHookWarning,
   eEPTPROV_ToneRingBack,
   eEPTPROV_ToneReOrder,
   eEPTPROV_ToneStutterdial,
   eEPTPROV_ToneCallWaiting1,
   eEPTPROV_ToneCallWaiting2,
   eEPTPROV_ToneCallWaiting3,
   eEPTPROV_ToneCallWaiting4,
   eEPTPROV_ToneAlertingSignal,
   eEPTPROV_ToneSpecialDial,
   eEPTPROV_ToneSpecialInfo,
   eEPTPROV_ToneRelease,
   eEPTPROV_ToneCongestion,
   eEPTPROV_ToneUserDefined1,
   eEPTPROV_ToneUserDefined2,
   eEPTPROV_ToneUserDefined3,
   eEPTPROV_ToneUserDefined4,

   /* Call waiting */
   eEPTPROV_CallWtgDelay,
   eEPTPROV_CallWtgMaxRep,

   /* Caller ID */
   eEPTPROV_CIDMode,
   eEPTPROV_CIDFskAfterRing,
   eEPTPROV_CIDFskAfterDTAS,
   eEPTPROV_CIDFskAfterRPAS,
   eEPTPROV_CIDRingAfterFSK,
   eEPTPROV_CIDDTASAfterLR,
   eEPTPROV_CIDSigProtocol,

   /* VMWI */
   eEPTPROV_VMWIMode,
   eEPTPROV_VMWISigProtocol,
   eEPTPROV_VMWIFskAfterDTAS,
   eEPTPROV_VMWIFskAfterRPAS,
   eEPTPROV_VMWIDTASAfterLR,

   /* Hook state parameters */
   eEPTPROV_MinHookFlash,
   eEPTPROV_MaxHookFlash,
   eEPTPROV_PlsDlInterdigitTime,
   eEPTPROV_PlsDlMinMakeTime,
   eEPTPROV_PlsDlMaxMakeTime,
   eEPTPROV_PlsDlMinBreakTime,
   eEPTPROV_PlsDlMaxBreakTime,
   eEPTPROV_PowerRingFrequency,


   eEPTPROV_PulseSignalEntry,
   eEPTPROV_RingCadenceEntry,
   eEPTPROV_StandardRingCadence,
   eEPTPROV_RingSplashCadence,

   /* EMTA management package defines */
   eEPTPROV_AnalogLoopback,
   eEPTPROV_DiagTestsStart,
   eEPTPROV_DiagTestsStop,
   eEPTPROV_DiagTestsValid,
   eEPTPROV_DiagTestsResult,
   eEPTPROV_DiagTestsValue,
   eEPTPROV_DiagTestsFault,
   eEPTPROV_BoostedLoopCurrent,
   eEPTPROV_VoiceJitterBuffFixed,
   eEPTPROV_VoiceJitterBuffMin,
   eEPTPROV_VoiceJitterBuffMax,
   eEPTPROV_VoiceJitterBuffTarget,
   eEPTPROV_DataJitterBuffTarget,
   eEPTPROV_DataJitterBuffMax,
   eEPTPROV_DtmfToneRelay,
   eEPTPROV_TxGain,
   eEPTPROV_RxGain,
   eEPTPROV_VBDTxGain,
   eEPTPROV_VBDRxGain,
   eEPTPROV_ToneDetectionCtrlExt,
   eEPTPROV_RingWaveform,
   eEPTPROV_GR303Support,
   eEPTPROV_ToneRfLoss,
   eEPTPROV_ToneEndptDisabled,
   eEPTPROV_ToneEndptDisconnected,
   eEPTPROV_ToneAcLoss,
   eEPTPROV_ToneLowBattery,
   eEPTPROV_ToneBadBattery,
   eEPTPROV_ToneOverTempBattery,
   eEPTPROV_FixedDtmfPower,
   eEPTPROV_Pulse2DtmfConvert,
   eEPTPROV_CustomCWCIDCfgEnable,
   eEPTPROV_CASLevelCfg,
   eEPTPROV_OffhookFSKLevelCfg,
   eEPTPROV_OnhookFSKLevelCfg,
   eEPTPROV_VMWIOSIDuration,
   eEPTPROV_AdjustedFSKLevelCfg,
   eEPTPROV_ClidDateMissingAction,

   /* E-line package signals */
   eEPTPROV_InitRing,                  /* Pulse signals */
   eEPTPROV_LoopClose,
   eEPTPROV_LoopOpen,
   eEPTPROV_EnableMetering,
   eEPTPROV_MeteringBurst,
   eEPTPROV_NoBattery,
   eEPTPROV_NormalPolarity,
   eEPTPROV_ReducedBattery,
   eEPTPROV_ReversePolarity,
   eEPTPROV_CadenceRingBase,           /* Cadence ringing base enum - cr(0) */
   /*** DO NOT insert anything here!! ***/
   eEPTPROV_CadenceRingMax = ( eEPTPROV_CadenceRingBase + MAX_CADENCE_RING - 1 ),

   /* NCS signaling specific defines */
   eEPTPROV_FaxDetection,
   eEPTPROV_DTMFPlayout,

   eEPTPROV_PcDiagTestsStart,
   eEPTPROV_PcDiagTestsClear,
   eEPTPROV_PcDiagTestsValid,
   eEPTPROV_PcDiagTestsResult,

   eEPTPROV_HighVring,
   eEPTPROV_RingDCOffset,
   eEPTPROV_RingOffsetCal,
   eEPTPROV_LoadBalancing,

   /*Items for notifying when line tests is in progress or done*/
   eEPTPROV_LineTestInProgress,
   eEPTPROV_LineTestDone,

   /* SIP defines */
   eEPTPROV_HookFlashRelay,
   eEPTPROV_PreferredVBDCodec,

   /* PC 2.0 defines */
   eEPTPROV_HookState,
   eEPTPROV_NetDisc,
   eEPTPROV_AnsSup,
   eEPTPROV_G711Pkt,
   eEPTPROV_T38,
   eEPTPROV_RTCPXR,
   eEPTPROV_RTCPRATE,   

    /* Must be the last item */
   eEPTPROV_NoProvID

} EPTPROV;

#if defined(__cplusplus)
}
#endif

#endif  /* EPT_PROV_H  */


