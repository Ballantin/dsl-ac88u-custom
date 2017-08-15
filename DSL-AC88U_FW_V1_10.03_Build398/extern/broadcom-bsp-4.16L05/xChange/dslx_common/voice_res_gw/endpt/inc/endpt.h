/****************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
****************************************************************************
*
*    Filename: endpt.h
*
****************************************************************************
*    Description:
*
*      Endpoint abstraction layer.
*
****************************************************************************/

#ifndef ENDPT_H
#define ENDPT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgCountry.h>
#include <codec.h>
#include <bosTypes.h>

#include <casCtl.h>

/* ---- Constants and Types ---------------------------------------------- */

/* MIB value for Tone Detection */
#define MIB_TONE_DET_ALL_ENABLED            1
#define MIB_TONE_DET_ALL_DISABLED           2
#define MIB_TONE_DET_V18ALARMPOS_DISABLED   3
#define MIB_TONE_DET_ALARMPOS_DISABLED      4
#define MIB_TONE_DET_V18_DISABLED           5

/* Only used by E-Package */
#define MAX_CADENCE_RING      128

#define VRG_ENDPT_NO_RING_INTERLEAVE   0xABBA

/*
 * The power (a priori) of a 1mW, 1.5kHz sine wave (assuming zero gain)
 * is represented by (<amplitude>^2)/2 where <amplitude> = 22656 (the digital
 * equivalent of peak amplitude).
 * Use this value in a decibel calculation
 * e.g. <measured dbm> = 10*log10(<measured power>/DIALTONE_ENERGYZERO)
 */
#define DIALTONE_ENERGYZERO 256647168L

/*
** Events detected
*/
typedef enum
{
   EPEVT_NULL,                    /* internal use: Null event */
   EPEVT_DTMF0,                   /* DTMF Tone 0 */
   EPEVT_DTMF1,                   /* DTMF Tone 1 */
   EPEVT_DTMF2,                   /* DTMF Tone 2 */
   EPEVT_DTMF3,                   /* DTMF Tone 3 */
   EPEVT_DTMF4,                   /* DTMF Tone 4 */
   EPEVT_DTMF5,                   /* DTMF Tone 5 */
   EPEVT_DTMF6,                   /* DTMF Tone 6 */
   EPEVT_DTMF7,                   /* DTMF Tone 7 */
   EPEVT_DTMF8,                   /* DTMF Tone 8 */
   EPEVT_DTMF9,                   /* DTMF Tone 9 */
   EPEVT_DTMFS,                   /* DTMF Tone * */
   EPEVT_DTMFH,                   /* DTMF Tone # */
   EPEVT_DTMFA,                   /* DTMF Tone A */
   EPEVT_DTMFB,                   /* DTMF Tone B */
   EPEVT_DTMFC,                   /* DTMF Tone C */
   EPEVT_DTMFD,                   /* DTMF Tone D */
   EPEVT_OFFHOOK,                 /* Off-hook event */
   EPEVT_ONHOOK,                  /* On-hook event */
   EPEVT_EARLY_OFFHOOK,           /* Early off-hook event */
   EPEVT_EARLY_ONHOOK,            /* Early on-hook event */
   EPEVT_FLASH,                   /* Hook-flash event */
   EPEVT_FAX,                     /* Fax tone detected */
   EPEVT_MODEM,                   /* Modem detect event */
   EPEVT_LOWENERGY,               /* Low energy detect event */
   EPEVT_DTMFL,                   /* DTMF long duration */
   EPEVT_MEDIA,                   /* Media start */
   EPEVT_TDD,                     /* Telecom Devices for the Deaf */
   EPEVT_T38_STOP,                /* T.38 completion */
   EPEVT_T38_FAILURE,             /* T.38 failure */
   EPEVT_RINGING_ON,              /* Phones are currently ringing */
   EPEVT_RINGING_OFF,             /* Phones are currently not ringing  */
   EPEVT_VBD_STOP,                /* VBD stop event */
   EPEVT_VBD_START,               /* VBD start event */
   EPEVT_FSK_CLIDRX,              /* FSK CLIDRX */
   EPEVT_DTMF_CLIDRX,             /* DTMF CLIDRX */
   EPEVT_OPCOMPLETE,              /* Operation complete */
   EPEVT_CPTONETIMEOUT,           /* Call progress tone detect timeout */
   EPEVT_RINGDET,                 /* Ring signal on detected */
   EPEVT_RINGOFF,                 /* Ring signal off detected */
   EPEVT_RINGEND,                 /* Ring signal end detected */
   EPEVT_ENCODER_SWITCH,          /* Encoder switch event */
   EPEVT_DECODER_SWITCH,          /* Decoder switch event */
   EPEVT_DIALTONE,                /* Dial Tone Detection event */
   EPEVT_DIALTONE_END,            /* Dial Tone End event */
   EPEVT_DTMF_LEVEL,              /* Dial Tone Level event */
   EPEVT_MLT_MEASUREMENT,         /* Event associated with requested measurement */
   EPEVT_INGRESS_DTMF,            /* Ingress DTMF generated */
   EPEVT_CUSTOMTONE,              /* Custom tone detect event */
   EPEVT_CHATTER_LINE_BAD_DIGIT,           /* Chattering line - bad digit event */
   EPEVT_CHATTER_LINE_EXCESSIVE_DIALING,   /* Chattering line - excessive dialing event */
   EPEVT_CHATTER_LINE_PULSE_EVENT,         /* Chattering line - indicates a pulse event is comming */  
   EPEVT_V18,                     /* V18 tone detected */   
   EPEVT_LAST

}EPEVT;

/*
** Signals generated
*/
typedef enum
{
   EPSIG_NULL,                    /* internal use: Null signal */
   EPSIG_BUSY,                    /* busy tone */
   EPSIG_CONF,                    /* confirmation tone */
   EPSIG_DIAL,                    /* dial tone */
   EPSIG_MSGW,                    /* message waiting indicator tone */
   EPSIG_OHWARN,                  /* off-hook warning tone */
   EPSIG_RINGING,                 /* enable ringer */
   EPSIG_RING0,                   /* distinctive ringing 0 */
   EPSIG_RING1,                   /* distinctive ringing 1 */
   EPSIG_RING2,                   /* distinctive ringing 2 */
   EPSIG_RING3,                   /* distinctive ringing 3 */
   EPSIG_RING4,                   /* distinctive ringing 4 */
   EPSIG_RING5,                   /* distinctive ringing 5 */
   EPSIG_RING6,                   /* distinctive ringing 6 */
   EPSIG_RING7,                   /* distinctive ringing 7 */
   EPSIG_CALLID_RINGING,          /* enable ringer, pending callid */
   EPSIG_CALLID_RING0,            /* distinctive ringing 0, pending callid */
   EPSIG_CALLID_RING1,            /* distinctive ringing 1, pending callid */
   EPSIG_CALLID_RING2,            /* distinctive ringing 2, pending callid */
   EPSIG_CALLID_RING3,            /* distinctive ringing 3, pending callid */
   EPSIG_CALLID_RING4,            /* distinctive ringing 4, pending callid */
   EPSIG_CALLID_RING5,            /* distinctive ringing 5, pending callid */
   EPSIG_CALLID_RING6,            /* distinctive ringing 6, pending callid */
   EPSIG_CALLID_RING7,            /* distinctive ringing 7, pending callid */
   EPSIG_REORDER,                 /* reorder tone */
   EPSIG_SPLASH,                  /* ring splash tone */
   EPSIG_RINGBACK,                /* ringback tone */
   EPSIG_STUTTER,                 /* stutter dial tone */
   EPSIG_INTRUSION,               /* intrusion tone (Norway) */
   EPSIG_WARNING,                 /* warning tone (Norway) */
   EPSIG_DIAL2,                   /* dial tone 2 (Norway's centrex dial tone) */
   EPSIG_CALLWT,                   /* call waiting tone */
   EPSIG_CALLW1,                  /* call waiting tone 1 */
   EPSIG_CALLW2,                  /* call waiting tone 2 */
   EPSIG_CALLW3,                  /* call waiting tone 3 */
   EPSIG_CALLW4,                  /* call waiting tone 4  */
   EPSIG_SAS_CAS,                 /* combined SAS and CAS tone for CLASS2 */
   EPSIG_SAS_CAS1,                /* combined SAS and CAS tone for CLASS2 */
   EPSIG_SAS_CAS2,                /* combined SAS and CAS tone for CLASS2 */
   EPSIG_SAS_CAS3,                /* combined SAS and CAS tone for CLASS2 */
   EPSIG_SAS_CAS4,                /* combined SAS and CAS tone for CLASS2 */
   EPSIG_VMWI,                    /* visual message waiting indicator */
   EPSIG_REV,                     /* phase reversal */
   EPSIG_DTMF0,                   /* DTMF Tone 0 */
   EPSIG_DTMF1,                   /* DTMF Tone 1 */
   EPSIG_DTMF2,                   /* DTMF Tone 2 */
   EPSIG_DTMF3,                   /* DTMF Tone 3 */
   EPSIG_DTMF4,                   /* DTMF Tone 4 */
   EPSIG_DTMF5,                   /* DTMF Tone 5 */
   EPSIG_DTMF6,                   /* DTMF Tone 6 */
   EPSIG_DTMF7,                   /* DTMF Tone 7 */
   EPSIG_DTMF8,                   /* DTMF Tone 8 */
   EPSIG_DTMF9,                   /* DTMF Tone 9 */
   EPSIG_DTMFS,                   /* DTMF Tone * */
   EPSIG_DTMFH,                   /* DTMF Tone # */
   EPSIG_DTMFA,                   /* DTMF Tone A */
   EPSIG_DTMFB,                   /* DTMF Tone B */
   EPSIG_DTMFC,                   /* DTMF Tone C */
   EPSIG_DTMFD,                   /* DTMF Tone D */
   EPSIG_CALLID,                  /* Caller ID message */
   EPSIG_CALLCARD,                /* Calling Card Service tone */
   EPSIG_NETBUSY,                 /* Network busy tone */
   EPSIG_NETBUSY1,                /* Network busy tone 1 */
   EPSIG_DISCONNECT,              /* Line Disconnect tone */
   EPSIG_PAYPHONE_REC,            /* Special Information tone */
   EPSIG_OSI,                     /* Open switch interval */
   EPSIG_PSTNCALLSETUP,           /* PSTN call setup */
   EPSIG_PSTNCALLSETUP_CONNECTONLY,/* PSTN call setup, where no dialtone detection and dialstring generation happens */
   EPSIG_OFFHOOK,                 /* Offhook - seize */
   EPSIG_ONHOOK,                  /* Onhook - release */
   EPSIG_ANSWER,                  /* Offhook - answer */
   EPSIG_ANALOG_LOOPBACK,         /* Enable analog loopback */
   EPSIG_LINE_RESET,              /* Line reset */
   EPSIG_DIAGTESTS_PROV_START,    /* Start diag tests with provisioning */
   EPSIG_DIAGTESTS_NOPROV_START,  /* Start diag tests without using provisioning */
   EPSIG_DIAGTESTS_PC15_START,    /* Start diag tests with PC15 provisioning */
   EPSIG_DIAGTESTS_CALIBRATE_START, /* Start calibartion process */
   EPSIG_UPDATE_DTMFPLAYOUT,      /* Update DTMF Playout duration */
   EPSIG_TXGAIN,                  /* Change Tx Gain setting */
   EPSIG_RXGAIN,                  /* Change Rx Gain setting */
   EPSIG_RING_WITH_DC_OFFSET,     /* Change Ring with DC Offset setting */
   EPSIG_BOOSTED_LOOP_CURRENT,    /* change Boosted Loop Current setting */
   EPSIG_ADMIN_STATUS,            /* line admin status change (enabled/disabled) */
   EPSIG_RING_WAVEFORM,           /* Change Ring Waveform settings */
   EPSIG_RINGBACK_CUST1,          /* Custom ringback tone 1 */
   EPSIG_SPECIAL_DIAL,            /* Special dial tone */
   EPSIG_SB_DISCONNECT,           /* Disconnect switchboard */
   EPSIG_INGRESS_DTMF,            /* Generate ingress DTMF tone */
   EPSIG_LINE_MODE_CONTROL,       /* Set to 1 so that subsequent mode controls apply to line VHD, 0 otherwise */
   EPSIG_FILL_INFO,               /* Filler Information tone for UAE */
/*
** E-line package-specific defines
*/

   /*
   ** Pulsed Signals encoding request
   */
   EPSIG_INIT_RING,
   EPSIG_LOOP_CLOSE,
   EPSIG_LOOP_OPEN,
   EPSIG_ENABLE_METERING,
   EPSIG_METERING_BURST,
   EPSIG_NO_BATTERY,
   EPSIG_NORMAL_POLARITY,
   EPSIG_REDUCED_BATTERY,
   EPSIG_REVERSE_POLARITY,

   /*
   ** Steady signals
   */
   EPSIG_STEADY_FULL_BATTERY,
   EPSIG_STEADY_LOOP_CLOSE,
   EPSIG_STEADY_LOOP_OPEN,
   EPSIG_STEADY_NO_BATTERY,
   EPSIG_STEADY_NORMAL_POLARITY,
   EPSIG_STEADY_REDUCED_BATTERY,
   EPSIG_STEADY_REVERSED_POLARITY,


   /*
   ** Cadence Ringing base enum (i.e. cr(0))
   */
   EPSIG_CADENCE_RING_BASE,

   /*** DO NOT insert anything here!! ***/

   /* Cadence Ringing max enum (i.e. cr(127)) */
   EPSIG_CADENCE_RING_MAX = ( EPSIG_CADENCE_RING_BASE + MAX_CADENCE_RING - 1),

/*
** end of E-line package-specific defines
*/

   /* DAA related signalling */
   EPSIG_DAA_DISABLE,
   EPSIG_DAA_ENABLE,

   EPSIG_LAST

}EPSIG;

/* Signal Direction */
typedef enum
{
   EPSIG_DIRECTION_EGRESS_OFF,
   EPSIG_DIRECTION_EGRESS_ON,
   EPSIG_DIRECTION_INGRESS_OFF,
   EPSIG_DIRECTION_INGRESS_ON,
   EPSIG_DIRECTION_BOTH_OFF,
   EPSIG_DIRECTION_BOTH_ON
} EPSIG_DIRECTION;

/*
** Provisioned values
*/
typedef enum
{
   EPPROV_R0Cadence           = 0,        /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R1Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R2Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R3Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R4Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R5Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R6Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_R7Cadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_RGCadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_RSCadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_RTCadence,                      /* Effective on next vrgEndptSignal (ring signal) */
   EPPROV_CallWtgDelay,                   /* Effective on next vrgEndptSignal (SAS_CAS or call waiting signal) */
   EPPROV_CallWtgMaxRep,

   /* Tones - 22 total (6 unused, 15 used, 1 CAS tone) */
   EPPROV_ToneBusy            = 200,      /* EPSIG_BUSY                 Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneConfirmation,               /* EPSIG_CONF                 Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneDial,                       /* EPSIG_DIAL                 Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneMessageWaiting,             /* EPSIG_MSGW                 Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneOffHookWarning,             /* EPSIG_OHWARN               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneRingBack,                   /* EPSIG_RINGBACK             Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneReOrder,                    /* EPSIG_REORDER              Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneStutterdial,                /* EPSIG_STUTTER              Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCallWaiting,                /* EPSIG_CALLWT               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCallWaiting1,               /* EPSIG_CALLW1               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCallWaiting2,               /* EPSIG_CALLW2               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCallWaiting3,               /* EPSIG_CALLW3               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCallWaiting4,               /* EPSIG_CALLW4               Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneAlertingSignal,             /* CAS TONE                   Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneSpecialDial,                /* ???-03                     Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneSpecialInfo,                /* ???-04                     Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneRelease,                    /* ???-05                     Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCongestion,                 /* EPSIG_NETBUSY              Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneCongestion1,                /* EPSIG_NETBUSY1             Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined1,               /* EPSIG_RINGBACK_CUST1       Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined2,               /* Intrusion tone (Norway)    Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined3,               /* Warning tone (Norway)      Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined4,               /* Centrex dial tone (Norway) Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined5,               /* Disconnect tone            Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_ToneUserDefined6,               /* EPSIG_PAYPHONE_REC         Effective on next vrgEndptSignal (tone signal)  */
   EPPROV_CallingCard,                    /* EPSIG_CALLCARD             Effective on next vrgEndptSignal (tone signal)  */


   EPPROV_MinHookFlash        = 400,      /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_MaxHookFlash,                   /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_PlsDlInterdigitTime,            /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_PlsDlMinMakeTime,               /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_PlsDlMaxMakeTime,               /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_PlsDlMinBreakTime,              /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_PlsDlMaxBreakTime,              /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_CIDMode,                        /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDFskAfterRing,                /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDFskAfterDTAS,                /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDFskAfterRPAS,                 /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDRingAfterFSK,                /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDDTASAfterLR,                 /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_CIDSigProtocol,                 /* Effective on next vrgEndptSignal (for ringing with caller id) */
   EPPROV_VMWIMode,                       /* Effective on next vrgEndptSignal (for EPSIG_VMWI) */
   EPPROV_VMWISigProtocol,                /* Currently not sourced from provisioning (only from vrgEndptGlobe static tables) */
   EPPROV_VMWIFskAfterDTAS,               /* Effective on next vrgEndptSignal (for EPSIG_VMWI) */
   EPPROV_VMWIFskAfterRPAS,               /* Effective on next vrgEndptSignal (for EPSIG_VMWI) */
   EPPROV_VMWIDTASAfterLR,                /* Effective on next vrgEndptSignal (for EPSIG_VMWI) */
   EPPROV_PulseSignalEntry,               /* Not used */
   EPPROV_RingCadenceEntry,               /* Not used */
   EPPROV_StandardRingCadence,            /* Not used */
   EPPROV_RingSplashCadence,              /* Not used */
   EPPROV_MinConnect,                     /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_MinDisconnect,                  /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_CIDFskParms,                    /* Fsk CID specific parameters*/
   EPPROV_CIDDtmfParms,                   /* Dtmf CID specific paramters*/
   EPPROV_VMWIMsgFormat,                  /* Visual Message waiting indicator formats TR-NWT-001401 */
   EPPROV_PteDetectAdmin,                 /* Dtmf CID specific paramters*/
   EPPROV_PteMinDetectPower,              /* minimum detection power level */
   EPPROV_PveEgressVolume,                /* Egress Volume */
   EPPROV_CngLevel,                       /* CNG level */

/*
** EMTA management package-specific defines
*/

   EPPROV_AnalogLoopback      = 600,      /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_DiagTestsStart,                 /* Effective on next run of MLT tests */
   EPPROV_DiagTestsStop,                  /* Effective on next run of MLT tests */
   EPPROV_DiagTestsValid,                 /* Effective on next run of MLT tests */
   EPPROV_DiagTestsResult,                /* Effective on next run of MLT tests */
   EPPROV_DiagTestsValue,                 /* Effective on next run of MLT tests */
   EPPROV_DiagTestsFault,                 /* Effective on next run of MLT tests */
   EPPROV_RingWithDCOffset,               /* Not used */
   EPPROV_BoostedLoopCurrent,             /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_VoiceJitterBuffFixed,           /* Effective on next vrgEndptCreateConnection */
   EPPROV_VoiceJitterBuffMin,             /* Effective on next vrgEndptCreateConnection */
   EPPROV_VoiceJitterBuffMax,             /* Effective on next vrgEndptCreateConnection */
   EPPROV_VoiceJitterBuffTarget,          /* Effective on next vrgEndptCreateConnection */
   EPPROV_DataJitterBuffTarget,           /* Effective on next vrgEndptCreateConnection */
   EPPROV_DataJitterBuffMax,              /* Not used */
   EPPROV_DtmfToneRelay,                  /* Not used */
   EPPROV_DTMFSkew,                       /* Effective on next DTMF Generation request for the specific line*/
   EPPROV_TxGain,                         /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_RxGain,                         /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_VBDTxGain,
   EPPROV_VBDRxGain,
   EPPROV_ToneDetectionCtrlExt,
   EPPROV_GR303Support,                   /* Effective on next vrgEndptCreate for the specific line*/
   EPPROV_ToneRfLoss,            /* RF loss */
   EPPROV_ToneEndptDisabled,     /* Endpt disabled */
   EPPROV_ToneEndptDisconnected, /* Endpt disconnected */
   EPPROV_ToneAcLoss,            /* Loss of AC power */
   EPPROV_ToneLowBattery,        /* Low battery */
   EPPROV_ToneBadBattery,        /* Bad battery */
   EPPROV_ToneOverTempBattery,   /* Battery over temperature */
   EPPROV_PlayZeroDurationPkt, 
   EPPROV_FixedDtmfPower,
   EPPROV_CustomCWCIDCfgEnable,  /* Controls whether EPPROV_CASLevelCfg, etc. are used */
   EPPROV_CASLevelCfg,           /* Custom CAS tone level in dBm */
   EPPROV_OffhookFSKLevelCfg,    /* Custom FSK tone level in dBm */
   EPPROV_OnhookFSKLevelCfg,     /* Custom FSK tone level in dBm */
   EPPROV_VMWIOSIDuration,
   EPPROV_AdjustedFSKLevelCfg,
   EPPROV_DlcxOsiDelay,
   EPPROV_ClidDateMissingAction,          /* Not used */

/*
** E-line package-specific defines
*/

   /*
   ** Pulsed signals
   */
   EPPROV_InitRing            = 800,      /* Not used */
   EPPROV_LoopClose,                      /* Not used */
   EPPROV_LoopOpen,                       /* Not used */
   EPPROV_EnableMetering,                 /* Not used */
   EPPROV_MeteringBurst,                  /* Not used */
   EPPROV_MeteringFrequency,              /* Effective on next vrgEndptSignal (EPSIG_METERING_BURST) */
   EPPROV_NoBattery,                      /* Not used */
   EPPROV_NormalPolarity,                 /* Not used */
   EPPROV_ReducedBattery,                 /* Not used */
   EPPROV_ReversePolarity,                /* Not used */


   /* Cadence Ringing base enum (i.e. cr(0)) */
   EPPROV_CadenceRingBase     = 1000,     /* Effective on next vrgEndptSignal (ring signal) */

   /*** DO NOT insert anything here!! ***/

   /* Cadence Ringing max enum (i.e. cr(127)) */
   EPPROV_CadenceRingMax = ( EPPROV_CadenceRingBase + MAX_CADENCE_RING - 1 ),    /* Not used */

/*
** End of E-line package-specific defines
*/

   EPPROV_Pulse2DtmfConvert   = 2000,     /* Effective on next vrgEndptCreate for the specific line*/

/*
** Enhanced NCS signalling specific defines
*/

   EPPROV_FaxDetection        = 2200,     /* Effective on next vrgEndptCreateConnection */
   EPPROV_DTMFPlayout,                    /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal for EPSIG_UPDATE_DTMFPLAYOUT */
   EPPROV_DtmfDbLevel,                    /* Effective on next vrgEndptSignal (ingress DTMF) or pulse to DTMF conversion */
   EPPROV_DtmfPulseTime,                  /* Effective on next vrgEndptSignal (ingress DTMF) or pulse to DTMF conversion */

   EPPROV_Rfc2833ToneLevel,               /* Tone Power Level over-ride */

   EPPROV_PcDiagTestsStart    = 2400,     /* Not used */
   EPPROV_PcDiagTestsClear,               /* Not used */
   EPPROV_PcDiagTestsValid,               /* Not used */
   EPPROV_PcDiagTestsResult,              /* Not used */

   EPPROV_HighVring           = 2600,     /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_PowerRingFrequency,             /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_RingWaveform,                   /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_RingVoltage,                    /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_RingDCOffset,                   /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_RingOffsetCal,                  /* Effective on next vrgEndptCreate for the specific line
                                          ** or on next vrgEndptSignal (EPSIG_RING_WAVEFORM) */
   EPPROV_LoadBalancing,                  /* Not used */

   /*Items for notifying when line tests is in progress or done*/
   EPPROV_LineTestInProgress = 2800,
   EPPROV_LineTestDone,

   EPPROV_Flexicalc,                      /* Effective on next vrgEndptInit */
   EPPROV_PreferredVBDCodec,              /* Effective on next vrgEndptCreateConnection or vrgEndptModifyConnection */

   /*Item to store the locale seen at the last voice start*/
   EPPROV_LastLocale,

   /* egress dtmf detection */
   EPPROV_EgressDtmfDetection,

/*
** SIP specific defines
*/
   EPPROV_HookFlashRelay,   

   EPPROV_LastItem, /* Must be the last item */

} EPPROV;



/*
** Returned status
*/
typedef enum
{
   EPSTATUS_SUCCESS = 0,         /* success */
   EPSTATUS_ERROR,               /* generic endpoint driver error */
   EPSTATUS_ENDPT_UNKNOWN,       /* endpoint is unknown */
   EPSTATUS_NOTREADY,            /* endpoint is not ready */
   EPSTATUS_RESOURCES,           /* endpoint has insufficient resources */
   EPSTATUS_SIGNAL_UNKNOWN,      /* endpoint cant generate that signal */
   EPSTATUS_CNX_UNKNOWN,         /* incorrect connection ID */
   EPSTATUS_UNSUPPORTED_MODE,    /* unsupported connection mode */
   EPSTATUS_UNKNOWN_TYPE,        /* unknown endpoint type */
   EPSTATUS_BUSY,                /* endpoint is busy */
   EPSTATUS_ONHOOK,              /* endpoint is onhook, can't perform operation */
   EPSTATUS_BAD_MODE,            /* endpoint is in wrong mode, can't perform operation */
   EPSTATUS_SHUTDOWN_ERR,        /* unable to shutdown the endpoint */
   EPSTATUS_PROV_FAILURE,        /* endpoint provisioning failure */
   EPSTATUS_RESET_PENDING,       /* reset is pending */

} EPSTATUS;

/*
** Connection modes
*/
typedef enum
{
   EPCNXMODE_SNDO,                /* Send only */
   EPCNXMODE_RCVO,                /* Receive only */
   EPCNXMODE_SNDRX,               /* Send-Receive mode */
   EPCNXMODE_CONF,                /* Send-Receive Conference mode */
   EPCNXMODE_INACT,               /* Inactive */
   EPCNXMODE_LOOP,                /* Network Loopback test */
   EPCNXMODE_TEST,                /* Network Continuity test */
   EPCNXMODE_REPLCATE,            /* Replicate */
   EPCNXMODE_TRANSCODE,           /* Transcoding */
   EPCNXMODE_MAX
} EPCNXMODE;



/*
** Cas states
*/
typedef enum
{
   EPCAS_ONHOOK,                  /* on-hook */
   EPCAS_OFFHOOK,                 /* off-hook */
   EPCAS_FLASH,                   /* flash */
   EPCAS_RINGON,                  /* ring on */
   EPCAS_RINGOFF,                 /* ring off */
   EPCAS_RINGEND,                 /* ring end */
   EPCAS_FIRST_RINGOFF,           /* first ring off for on-hook caller id */
   EPCAS_VMWI_OSI_SENT,           /* Vmwi supervisory disconnect complete */
   EPCAS_EARLY_OFFHOOK,           /* Early off hook event */
   EPCAS_EARLY_ONHOOK,            /* Early on hook event */
   EPCAS_DELAY_OFFHOOK,           /* Delayed off hook event */
   EPCAS_DELAY_ONHOOK             /* Delayed on hook event */

} EPCASSTATE;


/** SLIC/SLAC Modes. */
typedef enum
{
   EPSLIC_MODE_LCF,               /**< Loop Current Forward. */
   EPSLIC_MODE_STANDBY,           /**< Stand-by. */
   EPSLIC_MODE_LCFO               /**< Loop Current Forward Off (Disconnect). */
} EPSLICMODE;


/*
** Endpoint console commands
*/
typedef enum
{
   EPCMD_DSP_MIPS=0xff,          /* Display DSP MIPS usage */
   EPCMD_PRINT_VHDS,             /* Display VHD list. */
   EPCMD_ENDPTVHD_STATS,         /* Display DSP EndptVHD stats */
   EPCMD_PVE_STATE_FROM_CNX,     /* Display DSP VHD PVE state based on cnx ID */
   EPCMD_VHD_STATS,              /* Display DSP VHD stats */
   EPCMD_VHD_STATS_FROM_CNX,     /* Display DSP VHD stats based on cnx ID */
   EPCMD_RTP_STATS,              /* Display DSP RTP stats */
   EPCMD_VHD_JBREGS,             /* Display current jitter buffer registers */
   EPCMD_FAXR_STATS,             /* Display DSP FAXR stats */
   EPCMD_FAXR_STATS_FROM_ENDPT,  /* Display DSP FAXR stats based on endpt number */
   EPCMD_IPC_STATS,              /* Display DSP IPC stats */
   EPCMD_IPC_STATS_FROM_ENDPT,   /* Display DSP IPC stats based on endpt number */
   EPCMD_DSP_BUFFERS,            /* Display DSP buffer pool stats */
   EPCMD_DSP_GETCNT,             /* Display DSP debug counters */
   EPCMD_DSP_FORCEASSERT,        /* Force DSP assert */
   EPCMD_DSP_CHECKASSERT,        /* Check DSP assert stack pointer */
   EPCMD_DSP_GETREGS,            /* Get a snapshot of DSP regs */
   EPCMD_DSP_READDMEM,           /* Read internal DSP data memory */
   EPCMD_APM_STATUS,             /* Display APM status info */
   EPCMD_PWRMGMT_VPM,            /* Set the VPM power mode */
   EPCMD_PWRMGMT_AFE,            /* Power up/down the Analog Front End */
   EPCMD_TR57_ENABLE,            /* Enable/Disable TR57 test mode */
   EPCMD_VHD_LOOPBACK_ENABLE,    /* Enable VHD loopback */
   EPCMD_READ_HOOK_STATE,        /* Read hook state on the channel */
   EPCMD_SET_SLIC_STATE,         /* Set slic state on the channel */
   EPCMD_MEDIA_Q_START,          /* Start DSP media Q relay */
   EPCMD_MEDIA_Q_DUMP,           /* Display media Q relay results to console */
   EPCMD_MEDIA_Q_SET_SKIP_NUM,   /* Num of buffers to skip before capture starts */
   EPCMD_PRINT_DSP_CAPABILITIES, /* Display DSP capabilities. */
   EPCMD_DSP_STACK_SHOW,         /* Display DSP stack usage statistics. */
   EPCMD_HAPINET_CONFIG_SETREG,  /* Set configuration registers of the NET Service */
   EPCMD_DSP_ICP_CALIBRATE,      /* Calibrate DSP idle profiler. */
   EPCMD_DSP_ICP_START,          /* Start DSP idle profiler. */
   EPCMD_DSP_DPR_START,          /* Start development DSP profiler. */
   EPCMD_DSP_ICP_STOP,           /* Stop DSP idle profiler. */
   EPCMD_CONFIGURE_NETWTEST,     /* Configure switchboard connections for network test. */
   EPCMD_SET_ECAN_STATE,         /* Force ECAN to a certain state (on, off, NLP off) */
   EPCMD_HEC_BURST_ENABLE,
   EPCMD_HEC_NOISE_ENABLE,
   EPCMD_HEC_DEFAULT,
   EPCMD_HEC_GET_STATE,
   EPCMD_HEC_DBG_SET,
   EPCMD_HEC_RESET,
   EPCMD_HEC_RESET_COEF,
   EPCMD_CASEVT_GENERATE,
   EPCMD_TONEEVT_GENERATE,
   EPCMD_INDTMF_GENERATE,
   EPCMD_G168SETUP,
   EPCMD_CONNECTSB,
   EPCMD_INGRESSGAIN_SET,
   EPCMD_EGRESSGAIN_SET,
   EPCMD_CNX_INGRESSGAIN_SET,
   EPCMD_CNX_EGRESSGAIN_SET,
   EPCMD_DECT_START_BUFF_PROC,
   EPCMD_DECT_STOP_BUFF_PROC,
   EPCMD_PBDT_START,
   EPCMD_PBDT_STOP,
   EPCMD_PBDT_POWER,
   EPCMD_TPD_START,
   EPCMD_RTCP_NTP_TS_UPDATE,     /* Update RTCP NTP timestamp */
   EPCMD_PROVTABLE_SHOW,         /* Print out dynamic provisioning table */
   EPCMD_RESET_PROVTABLE,        /* Reset the provision table to defaults */
   EPCMD_DECTTEST,
   EPCMD_HOSTASSERT,
   EPCMD_HALCAP_SETUP,
   EPCMD_HALCAP_START,
   EPCMD_HALCAP_STOP,
   EPCMD_HALCAP_DEINIT,
   EPCMD_SET_AMR_CONFIG,
   EPCMD_MAX
} EPCONSOLECMD;

/*
** Console command parameters
*/
typedef struct
{
   unsigned int arg[8];
} EPCMD_PARMS;

#if defined( NTR_SUPPORT )
/*
** Network Timing Reference Synchronization parameters
*/
typedef enum
{
   GET_COUNTS,  /* Get the latest NTR and MIPS register values */
   SET_FEEDBACK,/* Set the PCM feedback register */
   GET_NDIV,    /* Get the latest NDIV_INT and NDIV_FRAC register values */
   PRINT_REGS   /* Debug command that prints out NTR and MIPS register values over a period of time */
} EPNTR_ACT;

typedef struct
{
   EPNTR_ACT                  ntrAction;
   VRG_UINT8                  isEnbl;
   VRG_SINT32                 pcmStepsAdjust;
   VRG_UINT32                 localCount;
   VRG_UINT32                 ntrCount;
   VRG_UINT32                 ndivInt;
   VRG_UINT32                 ndivFrac;
} EPNTR_PARMS;
#endif /* NTR_SUPPORT */

/* Data Mode Types */
typedef enum
{
   EPDATAMODE_NONE,          /* Disable CDIS */
   EPDATAMODE_VBD,           /* VBD (no FAXR or auto encoder switch) - default mode */
   EPDATAMODE_T38,           /* Use T38 fax relay (no auto encoder switch) */
   EPDATAMODE_T38_MUTE       /* Use T38 fax relay (but mute until instructed to switch to T.38) */
} EPDATAMODE;

/* Auto Encoder switch */
typedef enum
{
   EPAUTOENC_OFF = 0,   /* Auto encoder switch is turned off */
   EPAUTOENC_ON         /* Auto encoder switch is turned on */
} EPAUTOENC;

/*
** Endpoint types
*/
typedef enum
{
   EPTYPE_FXS,               /* Foreign Exchange Station, we talk to
                              * phone-like devices */
   EPTYPE_PSTN,              /* (FXO) Public switched telephone network,
                              * we talk to a phone switch */
   EPTYPE_DECT,              /* It is a DECT endpt which has no signaling
                                on its audio path */
   EPTYPE_NOSIG              /* No signalling, i.e. no CAS, tones, CLID.
                              * Used for paging/music-on-hold endpoints */

} EPTYPE;

/*
** CLID type supported by the endpoint.  The CLID type is dependent of the actual
** locale (aka country) settings for the endpoint.
**
** This enum matches the content of the CLASS_SIG_TYPE enum in hdspClass.h for ease
** of usage.
*/
typedef enum
{
   EPCLIDTYPE_FSK = 1,       /* CLID for this endpoint is FSK */
   EPCLIDTYPE_DTMF,          /* CLID for this endpoint is DTMF */

} EPCLIDTYPE;

/* T.38 Error Correction Scheme */
typedef enum
{
   EPT38EC_NONE,             /* No error correction */
   EPT38EC_UDPREDUNDANCY,    /* UDP Redundancy */
   /* EPT38EC_UDPFEC, ******* UDP Forward Error Correction - NOT SUPPORTED *****/

} EPT38ECMODE;

/* T.38 Connection Parameters */
typedef struct
{
   EPT38ECMODE     ecMode;              /* Error correction mode associated with the connection */
   VRG_UINT32      maxRemDgrm;          /* Maximum datagram size the remote end can handle      */
   VRG_UINT32      maxBitRate;          /* Maximum bit rate for the fax call                    */
   VRG_UINT32      numFaxPagesTx;       /* Number of transmitted pages for the life of the cnx */
   VRG_UINT32      numFaxPagesRx;       /* Number of received pages for the life of the cnx */
   VRG_UINT16      keepAliveInterval;   /* Interval at which keepAlive T.30 no signal indicator is sent during T.38 mode*/

} EPZT38PARAM;

typedef struct
{
   VRG_UINT16      modeSet;             /* Bitflag of amr modes from 0-7 for AMR-NB, and 0-8 for AMR-WB */
   VRG_BOOL        modeChangeNeighbor;  /* TRUE means we should only switch to a neighboring mode on rate change,
                                           FALSE means we can switch to any other rate in the mode-set */
   VRG_BOOL        modeChangePeriod;    /* If set to FALSE we can switch on any frame, if TRUE can only switch
                                           on every second frame */
   VRG_UINT16      ptime;               /* Packetization time per RTP packet. Must be a multiple of 20, and
                                           20 <= maxptime <= 240 */
   VRG_UINT16      red;                 /* Redundancy to include per RTP packet. Must be a multiple of 20
                                           and 0 <= maxred <= 220. Additionally, ptime + red must be <= 240 */
} EPAMRCFG;

/* XR Reporting Mode Types - only for VoIP Metrics*/
typedef enum
{
   EPRTCPXRMODE_UNSET,
   EPRTCPXRMODE_ON,          /* Enable XR Collecting & Reporting */
   EPRTCPXRMODE_OFF,         /* Disable XR Collecting & Reporting */
   EPRTCPXRMODE_NEGOTIATE,   /* Enable XR Collecting & Disable XR Reporting */
} EPRTCPXRMODE;


/* VBD (V.152) Redundancy support */
typedef enum
{
   EPVBDRED_OFF,             /* Enable redundancy for VBD */
   EPVBDRED_ON,              /* Disable redundancy for VBD */
} EPVBDREDMODE;


/* VBD mode */
typedef enum
{
   EPVBDMODE_NONE   = (0),
   EPVBDMODE_LEGACY = (1 << 0),         /* Legacy VBD mode */
   EPVBDMODE_V152   = (1 << 1),         /* V.152 VBD mode */
   EPVBDMODE_MAX    = 2

} EPVBDMODE;

/* DTMF tone on/off */
typedef enum
{
   EPDTMFACT_TONEON = 0,
   EPDTMFACT_TONEOFF,
   EPDTMFACT_EGRESS_TONEON,
   EPDTMFACT_EGRESS_TONEOFF,
} EPDTMFACT;

typedef enum
{
   EPDTMFINJECT_INGOFF = 0,
   EPDTMFINJECT_INGON,        /* Inject DTMF to the ingress direction */
} EPDTMFINJECT;

/* VBD parameters */
typedef struct
{
   EPVBDMODE    vbdMode;
   EPVBDREDMODE vbdRedMode;
} EPZVBDPARAM;

/* Tone relay parameters */
typedef enum DTMF_RELAY_TYPE
{
   DTMF_RELAY_RFC2833_DISABLED,
   DTMF_RELAY_RFC2833_ENABLED,
   DTMF_RELAY_RFC2833_SUBTRACT
} DTMF_RELAY_TYPE;

/* DTMF RFC 2833 mode */
typedef enum
{
   EPDTMFRFC2833_ENABLED,
   EPDTMFRFC2833_DISABLED,
   EPDTMFRFC2833_SUBTRACT,
} EPDTMFRFC2833CONFIG;

/* Preferred VBD codec */
typedef enum
{
   EPVBDCODEC_NONE,
   EPVBDCODEC_PCMU,
   EPVBDCODEC_PCMA

} EPPREFERREDVBDCODEC;

/* Tone Detection mode */
typedef enum
{
   EPTONEDET_ALL_ENABLED,
   EPTONEDET_ALL_DISABLED,
   EPTONEDET_V18ALARMPOS_DISABLED,
   EPTONEDET_ALARMPOS_DISABLED,
} EPTONEDETMODE;

/* Tone Detection mode */
#define EPTONEDET_FAX_DISABLED         0x80
#define EPTONEDET_MODEM_DISABLED       0x40
#define EPTONEDET_V18BAUDOT_DISABLED   0x20
#define EPTONEDET_V18PREAMBLE_DISABLED 0x10
#define EPTONEDET_V18ASCII_DISABLED    0x08
#define EPTONEDET_ALL_V18_DISABLED     (EPTONEDET_V18BAUDOT_DISABLED|EPTONEDET_V18PREAMBLE_DISABLED|EPTONEDET_V18ASCII_DISABLED)
#define EPTONEDET_ALL_DISABLED         (EPTONEDET_FAX_DISABLED|EPTONEDET_MODEM_DISABLED|EPTONEDET_ALL_V18_DISABLED)

/* Metering burst frequency */
typedef enum
{
   PULSE_SIGNAL_FREQ_12KHZ = 12000,
   PULSE_SIGNAL_FREQ_16KHZ = 16000,

} EPPULSESIGFREQUENCY;

/*
** Connection parameters
*/
typedef struct
{
   EPCNXMODE         mode;             /* connection mode */
   CODECCNXPARMLIST  cnxParmList;      /* list (in ordered preference) of negotiated
                                        * codecs/packetization period between the local
                                        * and remote endpoint */
   VRG_UINT32        namedPhoneEvts;   /* bit-field of negotiated named telephone events */
   VRG_UINT16        echocancel;       /* echo cancellation */
   VRG_UINT16        silence;          /* silence suppression */
   VRG_UINT16        comfortNoise;     /* comfort noise */
   VRG_UINT16        preserveFaxMode;  /* preserve a fax mode (if we are in already) */
   VRG_UINT16        secHdrSize;       /* Authentication mode -- security header size */
   EPDATAMODE        dataMode;         /* fax relay, vbd, or none */
   EPAUTOENC         autoEncoder;      /* Auto Encoder Switch */
   EPZT38PARAM       t38param;         /* T.38 fax parameters */
   EPRTCPXRMODE      rtcpXRMode;       /* rtcp XR mode -- on, off or negotiate */
   EPZVBDPARAM       vbdparam;         /* VBD parameters */
   EPDTMFRFC2833CONFIG digitRelayType; /* DTMF tone relay type */
   VRG_UINT32        localSsrc;        /* Local SSRC for the ingress RTP packets */
   VRG_UINT16        rtcpIntMs;        /* RTCP transmission interval in ms */
   EPAMRCFG          amrConfig;        /* AMR and AMR-WB config */
   VRG_SINT16        rxVolume;         /* Current volume setting for this connection */
   VRG_SINT16        txVolume;         /* Current volume setting for this connection */  
   VRG_UINT16        V18Support;       /* To configure V.18 detection */
   VRG_BOOL          csCall;           /* 2G3G voice call */
} EPZCNXPARAM;

/*
** Connection statistics
*/
typedef struct
{
   VRG_UINT32     txpkts;           /* packets sent */
   VRG_UINT32     txbytes;          /* octets sent */
   VRG_UINT32     rxpkts;           /* packets received */
   VRG_UINT32     rxbytes;          /* octets received */
   VRG_SINT32     lost;             /* packets lost */
   VRG_UINT32     discarded;        /* discarded packets */
   VRG_UINT32     txRtcpPkts;       /* Number of ingress RTCP packets */
   VRG_UINT32     rxRtcpPkts;       /* Number of egress RTCP packets */
   VRG_UINT16     txRtcpXrPkts;     /* Number of egress RTCP XR packets */
   VRG_UINT16     rxRtcpXrPkts;     /* Number of egress RTCP XR packets */
   VRG_UINT16     overRuns;         /* Number of jitter buffer overruns */
   VRG_UINT16     underRuns;        /* Number of jitter buffer underruns */
   VRG_UINT32     jitter;           /* Interarrival jitter estimate from RTP service (in ms) */
   VRG_UINT16     peakJitter;       /* Peak jitter / holding time from voice PVE service (in ms)*/
   VRG_UINT16     jbMin;            /* Jitter buffer nominal delay */
   VRG_UINT16     jbAvg;            /* Jitter buffer average delay */
   VRG_UINT16     jbMax;            /* Jitter buffer absolute maximum delay */
   VRG_UINT32     latency;          /* Avg tx delay (in ms) */
   VRG_UINT16     peakLatency;      /* Peak tx delay (in ms) */
   VRG_UINT16     MOSLQ;            /* MOS-listening quality (value*10) */
   VRG_UINT16     MOSCQ;            /* MOS-conversational quality (value*10) */
} EPZCNXSTATS;

/*
** Connection T.38 specific statistics
*/
typedef struct
{
   VRG_UINT32     txpg;             /* pages sent */
   VRG_UINT32     rxpg;             /* pages received */

} EPZT38CNXSTATS;

/*
** Endpoint capabilities
*/
typedef struct EPZCAP   /* Endpoint Capabilities */
{
   VRG_UINT16  pCap[2];      /* Packetazation period data (msec). MS-Bit contains "1" if a range is defined */
   VRG_UINT8   codecCap[CODEC_MAX_TYPES];   /* Codec caps for the given endpoint */
   VRG_UINT32  nteCap;       /* Bit encoded value representing supported named telephone events */
   VRG_UINT16  eCap;         /* Echo cancelation (0 if NOT supported) */
   VRG_UINT16  sCap;         /* Silence Suppression parameter (0 if NOT supported) */
   int         mCap[16];     /* Connection Modes. Each char codes the connection mode coded as MGECMODE enum type */
   EPTYPE      endptType;    /* Endpoint type */
   EPCLIDTYPE  clidType;     /* CLID type */
   VRG_UINT8   clidData[2];  /* Extra data.  Used for EPCLIDTYPE_DTMF to carry head/tail information */
   VRG_UINT8   vbdCap;     /* Bit encoded value representing supported voice band data capabilities. */
   VRG_UINT8   redLvl;     /* Maximum number of redundancy level(s) supported per RFC 2198. */
} EPZCAP;

/*
** RTP header format
** NOTE: The first six fields are bit-length specific.
**       The RTP payload directly follows the RTP header.
*/
typedef struct {

#if BOS_CFG_BIG_ENDIAN
    VRG_UINT8  version:2;   /* protocol version */
    VRG_UINT8  p:1;         /* padding flag */
    VRG_UINT8  x:1;         /* header extension flag */
    VRG_UINT8  cc:4;        /* CSRC count */
    VRG_UINT8  m:1;         /* marker bit */
    VRG_UINT8  pt:7;        /* payload type */
#elif BOS_CFG_LITTLE_ENDIAN
    VRG_UINT8  cc:4;        /* CSRC count */
    VRG_UINT8  x:1;         /* header extension flag */
    VRG_UINT8  p:1;         /* padding flag */
    VRG_UINT8  version:2;   /* protocol version */
    VRG_UINT8  pt:7;        /* payload type */
    VRG_UINT8  m:1;         /* marker bit */
#else
   #error "BOS_CFG_xxx_ENDIAN not defined!"
#endif
    VRG_UINT8  seq[2];      /* 16-bit sequence number */
    VRG_UINT8  ts[4];       /* 32-bit timestamp */
    VRG_UINT8  ssrc[4];     /* synchronization source */
} RTPPACKET;

/*
** RTCP common header format
*/
typedef struct
{
#if BOS_CFG_BIG_ENDIAN
   /***************************
    * First octet contains:
    *    UINT8    version  : 2
    *    UINT8    p        : 1
    *    UINT8    rc       : 5
    **************************/
   VRG_UINT8    version:2;   /* protocol version */
   VRG_UINT8    p:1;         /* padding flag */
   VRG_UINT8    rc:5;        /* rc count */
   VRG_UINT8    packetType;
   VRG_UINT16   length;
#elif BOS_CFG_LITTLE_ENDIAN
  VRG_UINT8    rc:5;        /* rc count */
  VRG_UINT8    p:1;         /* padding flag */
  VRG_UINT8    version:2;   /* protocol version */
  VRG_UINT8    packetType;
  VRG_UINT16   length;
#else
   #error "BOS_CFG_xxx_ENDIAN not defined!"
#endif
} RTCP_COMMON_HEADER;

/*
** Reason code associated with events reported from the endpoint module
** to the upper layer application.  It is up to the application receiving
** the event to use the reason code for event processing.
*/
typedef enum
{
   EPEVTREASON_PTSW,         /* Payload type switch. */
   EPEVTREASON_CNG,          /* CNG signal. */
   EPEVTREASON_EGRESS_CNG,   /* Egress CNG signal. */
   EPEVTREASON_V21flag,      /* V.21 flag. */
   EPEVTREASON_V21tone,      /* V.21 tone. */
   EPEVTREASON_BAUDOT,       /* V.18 Baudot tone and character. */
   EPEVTREASON_BAUDOT_SHORT, /* V.18 Baudot tone and character. Shorter duration (100ms instead of 110ms). */
   EPEVTREASON_EGRESS_BAUDOT_SHORT, /* V.18 Baudot tone and character. Shorter duration (100ms instead of 110ms). */
   EPEVTREASON_BELLTONE,     /* Bell 103 carrier. */
   EPEVTREASON_ANS,          /* V.25 ANS. */
   EPEVTREASON_EGRESS_ANS,   /* Egress V.25 ANS. */
   EPEVTREASON_prANS,        /* V.25 ANS with phase reversal. */
   EPEVTREASON_EGRESS_prANS, /* Egress V.25 ANS with phase reversal. */
   EPEVTREASON_CUSTTONE_INGRESS, /* Ingress custom tone detected */
   EPEVTREASON_CUSTTONE_EGRESS, /* Ingress custom tone detected */
   EPEVTREASON_DCN,          /* DCN signal. */
   EPEVTREASON_HDLC,         /* HDLC signal. */
} EPEVTREASON;

/*
** Reason code associated with a VBD_STOP event. For now, enxtended low
** energy is the only event of interest. More values may be introduced
** in the future.
*/
typedef enum
{
   EVEVTVDBSTOP_EXTLOWENERGY /* Extended low energy */
} EPEVTVBDSTOPREASON;

/*
** Maximum number of entries in the codec table array.  This
** number should really be tied to the number of supported cnxs. */
#define VRG_CODEC_TABLE_MAX_ROW_ENTRY      4


/*
** Definition of a single codec entry in the codec table array
*/
typedef struct VRG_CODEC_TABLE_ROW_ENTRY
{
   CODEC_TYPE  codec;      /* codec type */
   VRG_UINT16  maxCodec;   /* maximum number of simultaneous operation
                           ** of the assiocated codec type */

} VRG_CODEC_TABLE_ROW_ENTRY;


/*
** Definition of a single row of the codec table
**
** Each row is composed of 2 components:
**    1. an array of codec entries
**    2. number of elements in the array that is being used
*/
typedef struct VRG_CODEC_TABLE_ROW
{
   /* Array of codec entries */
   VRG_CODEC_TABLE_ROW_ENTRY  codecTableEntry[VRG_CODEC_TABLE_MAX_ROW_ENTRY];

   /* Number of entries used in the array */
   int                        codecTableNumEntry;


} VRG_CODEC_TABLE_ROW;

/* Maximum size of the output buffer for the EPCMD_VHD_STATS_FROM_CNX endpt
 * command. */
#define MAX_EPCMD_VHD_STATS_FROM_CNX_BUFFER_SIZE 750

/* Get/set macros for RTPPACKET structure  - access to (non-bit field) RTPPACKET member
 * variables should be made through these macros. The macros resolve alignment issues
 * that may occur if the endpoint produces or consumes an RTP packet that isn't 32-bit aligned. */
#define RTPPACKET_GET_SEQNUM( rtp ) ( (rtp)->seq[0] << 8 | (rtp)->seq[1] )

#define RTPPACKET_SET_SEQNUM( rtp, val ) ((rtp)->seq[0] = (VRG_UINT8)((( val ) & 0xff00 ) >> 8), \
                                          (rtp)->seq[1] = (VRG_UINT8)(( val ) & 0x00ff ))

#define RTPPACKET_GET_TIMESTAMP( rtp ) ((rtp)->ts[0] << 24 | (rtp)->ts[1] << 16 | \
                                          (rtp)->ts[2] << 8 | (rtp)->ts[3] )

#define RTPPACKET_SET_TIMESTAMP( rtp, val ) ((rtp)->ts[0] = (VRG_UINT8)((( val ) & 0xff000000 ) >> 24),  \
                                            (rtp)->ts[1] = (VRG_UINT8)((( val ) & 0x00ff0000 ) >> 16),  \
                                            (rtp)->ts[2] = (VRG_UINT8)((( val ) & 0x0000ff00 ) >> 8),   \
                                            (rtp)->ts[3] = (VRG_UINT8)(( val ) & 0x000000ff ))

#define RTPPACKET_IS_VALID_VERSION( rtp ) ( (rtp)->version == 0x2 )


#if BOS_CFG_BIG_ENDIAN

#define RTPPACKET_GET_SSRC( rtp ) ((rtp)->ssrc[0] << 24 | (rtp)->ssrc[1] << 16 | \
                                          (rtp)->ssrc[2] << 8 | (rtp)->ssrc[3] )

#define RTPPACKET_SET_SSRC( rtp, val ) ((rtp)->ssrc[0] = (VRG_UINT8)((( val ) & 0xff000000 ) >> 24), \
                                         (rtp)->ssrc[1] = (VRG_UINT8)((( val ) & 0x00ff0000 ) >> 16), \
                                         (rtp)->ssrc[2] = (VRG_UINT8)((( val ) & 0x0000ff00 ) >> 8),  \
                                         (rtp)->ssrc[3] = (VRG_UINT8)(( val ) & 0x000000ff ))
#else
#define RTPPACKET_GET_SSRC( rtp ) ((rtp)->ssrc[3] << 24 | (rtp)->ssrc[2] << 16 | \
                                          (rtp)->ssrc[1] << 8 | (rtp)->ssrc[0] )

#define RTPPACKET_SET_SSRC( rtp, val ) ((rtp)->ssrc[3] = (VRG_UINT8)((( val ) & 0xff000000 ) >> 24), \
                                         (rtp)->ssrc[2] = (VRG_UINT8)((( val ) & 0x00ff0000 ) >> 16), \
                                         (rtp)->ssrc[1] = (VRG_UINT8)((( val ) & 0x0000ff00 ) >> 8),  \
                                         (rtp)->ssrc[0] = (VRG_UINT8)(( val ) & 0x000000ff ))
#endif


/* Media Types */
typedef enum
{
   EPMEDIATYPE_RTP,          /* rtp packet */
   EPMEDIATYPE_RTCP,         /* rtcp packet */
   EPMEDIATYPE_T38           /* t38 packet */
} EPMEDIATYPE;

/* Endpt Packet Type */
typedef struct
{
   EPMEDIATYPE mediaType;    /* Type of packet (fax/non-fax) */
   void* packetp;            /* Pointer to the packet */
} EPPACKET;


/* --------------------------------------------------------------------------
** 'ENDPT_STATE' is the base, generic endpt structure; it specifies a set of
** function pointers that define the generic endpt API that must be provided by
** all derived endpt types.
**
** All members of the 'ENDPT_STATE' structure must be present in all derived
** endpt structures, in the same order.
**
** Derived endpoint types should provide a "derived" version of ENDPT_STATE, e.g.:
**
**    typedef struct MY_ENDPT_STATE
**    {
**       const struct ENDPT_FUNCS  *endptFuncs;
**       int                        lineId;
**
**       int                        myEndptSpecificData1;
**       int                        myEndptSpecificData2;
**       ...
**
**    } MY_ENDPT_STATE;
**
** Clients, which have access to ENDPT_STATE structs, can use the generic
** macros provided below to transparently invoke derived endpt specific
** functions. The derived endpt functions will receive a pointer to the
** ENDPT_STATE struct as the first parameter. This struct can be cast to the
** derived endpt structure to gain access to endpt specific instance memory.
*/
typedef struct
{
   /* All derived structure must contain all members below. */
   const struct ENDPT_FUNCS  *endptFuncs;
   int                        lineId;

} ENDPT_STATE;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/* --------------------------------------------------------------------------
** Callback functions that should be registered with the endpoint.
*/

/*
*****************************************************************************
** FUNCTION:   endptEventCallback
**
** PURPOSE:    Endpoint event callback function prototype
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - the connection identifier (-1 if not applicable)
**             event       - the event
**             data        - additional data for the event
**             datap       - pointer to received data
**             length      - length of the received data
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*endptEventCallback)( ENDPT_STATE *endptState, int cnxId,
                                    EPEVT event, void *datap, int length, int data );

/*
*****************************************************************************
** FUNCTION:   endptRtpPacketCallback
**
** PURPOSE:    Endpoint connection packet callback
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - the connection id.
**             packet      - the endpt packet
**             length      - the endpt packet length including header
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*endptPacketCallback)
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPPACKET      *packet,
   int            length
);

/*
*****************************************************************************
** FUNCTION:   endptGetProvCallback
**
** PURPOSE:    Endpoint provisioned value callback
**
** PARAMETERS: lineId         - endpt indentifier
**             provItemId     - the enumerated id of the provisionable item
**             provItemValue  - pointer to the provisioned data value
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef EPSTATUS (*endptGetProvCallback)
(
   int            lineId,
   EPPROV         provItemId,
   void          *provItemValue
);


/*
*****************************************************************************
** FUNCTION:   endptSetProvCallback
**
** PURPOSE:    Endpoint provisioned value callback
**
** PARAMETERS: lineId         - endpt indentifier
**             provItemId     - the enumerated id of the provisionable item
**             provItemValue  - pointer to the provisioned data value
**             length         - pointer to the length to set
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef EPSTATUS (*endptSetProvCallback)
(
   int            lineId,
   EPPROV         provItemId,
   void          *provItemValue,
   int            length
);


/*
*****************************************************************************
** FUNCTION:   endptPacketReleaseCallback
**
** PURPOSE:    Callback handler invoked when the packet passed to endptRtpPacket
**             is released by the endpoint back to the caller.
**
** PARAMETERS: endptState  - endpt object.
**             cxid        - connection identifier
**             bufDesc     - buffer descriptor value passed to endptRtpPacket
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *endptPacketReleaseCallback )
(
   ENDPT_STATE   *endpt,
   int            cxid,
   unsigned int   bufDesc
);


/*
*****************************************************************************
** FUNCTION:   endptTaskShutdownCallback
**
** PURPOSE:    Callback handler invoked just prior to the shutdown of
**             endpoint tasks.
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
** NOTE:       This callback will be triggered in response to a call to
**             vrgEndptDeinit. Currently, it MUST be invoked within the
**             context of the task that invokes the endptRtpPacketCallback
**             (i.e. RTP procuder task). It may be invoked within
**             the context of other tasks in the future (depending on system
**             requirements).
**
**             This callback has been provided to support lame OSs (e.g. pSOS)
**             that only allow the creator of some resources (e.g. sockets) to
**             use those resources. If another task (e.g. RTP packet producer)
**             needs to use that resource, it needs to be shared. Subsequently,
**             the shared resource needs to be freed from *within the context
**             of the task that it was shared with*. i.e. shared resources
**             should be freed within the context of this callback.
**
*****************************************************************************
*/
typedef void ( *endptTaskShutdownCallback ) ( void );



/* --------------------------------------------------------------------------
** Endpoint API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   endptCapabilities
**
** PURPOSE:    Retrieve the capabilities for an endpoint
**
** PARAMETERS: endptState     - endpt object.
**             capabilities   - a pointer to the capabilities struct to populate
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptCapabilities)
(
   ENDPT_STATE   *endptState,
   EPZCAP        *capabilities
);

/*
*****************************************************************************
** FUNCTION:   endptSignal
**
** PURPOSE:    Generate a signal on the endpoint (or connection)
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier (-1 if not applicable)
**             signal      - signal type code (see EPSIG)
**             value       - signal value
**                            BR signal types - 1
**                            OO signal types - 0 == off, 1 == on
**                            TO signal types - 0 = stop/off, 1= start/on
**                            String types - (char *) cast to NULL-term string value
**              duration   - pulse duration (-1 if not used)
**              period     - pulse period (-1 if not used)
**              repetition - depending on the pulsed signal, this parameter
**                           has different meaning:
**                             - 'em': defines the report pulse count (rpc)
**                             - other pulsed signals: defines the repetition (rep)
**                           (-1 if not used)
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
typedef EPSTATUS (*endptSignal)
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPSIG          signal,
   unsigned int   value,
   int            duration,
   int            period,
   int            repetition
);

/*
*****************************************************************************
** FUNCTION:   endptCreateConnection
**
** PURPOSE:    Function to create a connection
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier
**             cnxParam    - ptr to connection parameter block
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptCreateConnection)
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
);

/*
*****************************************************************************
** FUNCTION:   endptModifyConnection
**
** PURPOSE:    Function to modify a connection
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier
**             cnxParam    - ptr to connection parameter block
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptModifyConnection)
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
);

/*
*****************************************************************************
** FUNCTION:   endptDeleteConnection
**
** PURPOSE:    Function to delete a connection
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier
**
** RETURNS:    EPSTATUS - if success, fills in statistics block
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptDeleteConnection)( ENDPT_STATE *endptState, int cnxId );


/*****************************************************************************
** FUNCTION:   endptLinkInternal
**
** PURPOSE:    Function to internally link two endpoints
**
** PARAMETERS:  endptState       - first endpt object.
**              otherEndptState  - second endpt object.
**              link             - whether or not to link or unlink.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptLinkInternal)
(
   ENDPT_STATE *endptState,
   ENDPT_STATE *otherEndptState,
   VRG_BOOL link
);

/*
*****************************************************************************
** FUNCTION:   endptPacket
**
** PURPOSE:    Transmit a endpt packet to the endpoint. This function is asynchronous,
**             it does not wait for the data to be transmitted  before returning.
**             In addition, the buffer release mechanism is asynchronous.
**             The endpoint owns the packet passed to this function until a callback
**             is invoked releasing the packet back to the caller (endptPacketReleaseCallback).
**             The value of 'bufDesc' will be passed to the callback function so that
**             the caller can identify which packet is being released by the endpoint.
**
** PARAMETERS: endptState - endpt object.
**             epPacket   - pointer to the EPPACKET packet.
**             length     - the length of the endpt packet including header
**             bufDesc    - buffer descriptor value that uniquely identifies
**                          the packet. This value is returned in the
**                          callback invoked once the buffer has been completely
**                          transmitted. This value is only meaningful to the
**                          caller and is opaque to the endpoint.
**
** RETURNS:    EPSTATUS
**
** NOTE:       If this function does not return EPSTATUS_SUCCESS, it is the caller's
**             responsibility to free the RTP packet.
**
*****************************************************************************
*/
typedef EPSTATUS (*endptPacket)
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPPACKET      *epPacket,
   int            length,
   unsigned int   bufDesc
);


/*
*****************************************************************************
** FUNCTION:   endptConsoleCmd
**
** PURPOSE:    Display endpoint info to the console
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - the connection index
**             cmd         - endpoint console command type
**
** RETURNS:    nothing
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptConsoleCmd)
(
   ENDPT_STATE       *endptState,
   EPCONSOLECMD      cmd,
   EPCMD_PARMS       *pConsoleCmdParams
);


/*
*****************************************************************************
** FUNCTION:   endptPowerSourceChanged
**
** PURPOSE:    Notify endpoint of power source changed
**
** PARAMETERS: newPowerSource - 0: operating on AC
**                              1: operating on battery power
**
** RETURNS:    nothing
**
** NOTE:
*****************************************************************************
*/
typedef void (*endptPowerSourceChanged)
(
   int   newPowerSource
);


/*
*****************************************************************************
** FUNCTION:   endptRtpStatistics
**
** PURPOSE:    get RTP statistics
**
** PARAMETERS: endptState  - endpt object
**             cnxId       - connection identifier
**             epRtpStats  - RTP stats
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
typedef EPSTATUS (*endptRtpStatistics)
(
   ENDPT_STATE    *endptState,
   int            cnxId,
   EPZCNXSTATS    *epRtpStats,
   VRG_BOOL       bReset
);


/*
*****************************************************************************
** FUNCTION:   endptIsEnabled
**
** PURPOSE:    Determine if the endpoint is enabled.
**
** PARAMETERS: int lineId;
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *endptIsEnabled )
(
   int               lineId,
   int             *enabled
);


#if defined( NTR_SUPPORT )
/*
*****************************************************************************
** FUNCTION:   endptNtrCmd
**
** PURPOSE:    Either update PCM feedback registers in kernel space, or obtain latest counts
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *endptNtrCmd )
(
   ENDPT_STATE             *endpt,
   VRG_UINT8               ntrAction,
   VRG_UINT8               isEnbl,
   VRG_SINT32              pcmStepsAdjust,
   VRG_UINT32              *localCount,
   VRG_UINT32              *ntrCount,
   VRG_UINT32              *ndivInt,
   VRG_UINT32              *ndivFrac
);
#endif /* NTR_SUPPORT */

/*
*****************************************************************************
** FUNCTION:   endptT38Statistics
**
** PURPOSE:    Get T.38 stats from the endpoint.
**
** PARAMETERS: endptState  - endpt object.
**             cnxId        - connection identifier
**             epT38Stats  - T.38 stats
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *endptT38Statistics )
(
   ENDPT_STATE       *endpt,
   int               cnxId,
   EPZT38CNXSTATS    *epT38Stats
);


/*
*****************************************************************************
** FUNCTION:   endptMuteConnection
**
** PURPOSE:    Toggle the mute status of a connection on the endpoint.
**
** PARAMETERS: endptState  - endpt object.
**             cnxId        - connection identifier
**             mute        - mute status for the connection
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
typedef EPSTATUS ( *endptMuteConnection )
(
   ENDPT_STATE       *endpt,
   int               cnxId,
   VRG_BOOL          mute
);

/*
*****************************************************************************
** FUNCTION:   endptSetCasState
**
** PURPOSE:    To set the CAS state of a given endpoint. Used by external CAS.
**
** PARAMETERS: endptState  - endpt object.
**             lineId      - line number
**             casState    - New CAS state to set to the given endpoint
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
typedef EPSTATUS ( *endptSendCasEvt )
(
   int                 lineId,
   CAS_CTL_EVENT_TYPE  casEvtType,
   CAS_CTL_EVENT       casCtlEvt
);

/*
** Structure that contains all function pointers that define the endpt API.
*/
typedef struct ENDPT_FUNCS
{
   endptCapabilities       getCapabilities;     /* Retrieve endpoint capabilities */
   endptSignal             generateSignal;      /* Generate signal on endpoint */
   endptCreateConnection   createConnection;    /* Create connection to endpoint */
   endptModifyConnection   modifyConnection;    /* Modify existing connection */
   endptDeleteConnection   deleteConnection;    /* Delete connection */
   endptLinkInternal       linkInternal;        /* Link or unlink two internal endpts */
   endptPacket             sendPacket;          /* Send endpt packet to connection */
   endptConsoleCmd         consoleCmd;          /* Display endpoint info to console */
   endptPowerSourceChanged powerSourceChanged;  /* Notify power source changed */
   endptRtpStatistics      rtpStatistics;       /* RTP statistics for the connection. */
   endptT38Statistics      t38Statistics;       /* T.38 statistics for the connection. */
   endptIsEnabled          isEnabled;           /* Is the endpoint enabled*/
   endptMuteConnection     muteConnection;      /* Toggle mute status on a connection */
   endptSendCasEvt         sendCasEvt;          /* Function used by external CAS to send CAS events to endpt */
#if defined( NTR_SUPPORT )
   endptNtrCmd             ntrCmd;
#endif /* NTR_SUPPORT */
} ENDPT_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the endpt.
*/

#define endptCapabilities( state, cap ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->getCapabilities( (state), (cap) ) : EPSTATUS_ERROR )

#define endptSignal( state, cxid, signal, value, duration, period, repetition ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->generateSignal( (state), (cxid), (signal), (value), \
           (duration), (period), (repetition) ) : EPSTATUS_ERROR )

#define endptCreateConnection( state, cxid, parm ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->createConnection( (state), (cxid), (parm) ) : EPSTATUS_ERROR )

#define endptModifyConnection( state, cxid, parm ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->modifyConnection( (state), (cxid), (parm) ) : EPSTATUS_ERROR )

#define endptDeleteConnection( state, cxid ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->deleteConnection( (state), (cxid) ) : EPSTATUS_ERROR )

#define endptPacket( state, cxid, pkt, len, desc ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->sendPacket( (state), (cxid), (pkt), (len), (desc) ) : EPSTATUS_ERROR )

#define endptConsoleCmd( state, cmd, params ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->consoleCmd( (state), (cmd), (params) ) : EPSTATUS_ERROR )

#define endptPowerSourceChanged( state, newPowerSource ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->powerSourceChanged( (newPowerSource) ) : EPSTATUS_ERROR )

#define endptRtpStatistics( state, cxid, epRtpStats, bReset ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->rtpStatistics( (state), (cxid), (epRtpStats), (bReset) ) : EPSTATUS_ERROR )

#define endptT38Statistics( state, cxid, t38Stats ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->t38Statistics( (state), (cxid), (t38Stats) ) : EPSTATUS_ERROR )

#define endptIsEnabled( state, lineId, enabled ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->isEnabled( (lineId), (enabled) ) : EPSTATUS_ERROR )

#define endptMuteConnection( state, cxid, mute ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->muteConnection( (state), (cxid), (mute) ) : EPSTATUS_ERROR )

#define endptLinkInternal( state, otherState, link ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->linkInternal( (state), (otherState), (link) ) : EPSTATUS_ERROR )

#define endptSendCasEvt( state, lineId, casEvtType, casCtlEvt ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->sendCasEvt( (lineId), (casEvtType), (casCtlEvt) ) : EPSTATUS_ERROR )

#if defined( NTR_SUPPORT )
#define endptNtrCmd( state, action, isEnbl, pcmStepsAdjust, localCount, ntrCount, ndivInt, ndivFrac ) \
         ( (state)->endptFuncs ? (state)->endptFuncs->ntrCmd( (state), (action), (isEnbl), (pcmStepsAdjust), (localCount), (ntrCount), (ndivInt), (ndivFrac)) : EPSTATUS_ERROR )
#endif /* NTR_SUPPORT */

#ifdef __cplusplus
}
#endif


#endif  /* ENDPT_H  */
