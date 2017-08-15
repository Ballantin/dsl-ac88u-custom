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

#include "codec.h"
#include <bosTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

#define VRG_ENDPT_NO_RING_INTERLEAVE   0xABBA

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
   EPEVT_CHATTER_LINE_BAD_DIGIT,           /* Chattering line - bad digit event */
   EPEVT_CHATTER_LINE_EXCESSIVE_DIALING,   /* Chattering line - excessive dialing event */
   EPEVT_CHATTER_LINE_PULSE_EVENT,         /* Chattering line - indicates a pulse event is comming */  
   EPEVT_ENCODER_SWITCH,          /* Encoder switch by DSP */
   EPEVT_DECODER_SWITCH,          /* Decoder switch by DSP */
   EPEVT_SES_STATUS,
   EPEVT_LAST

}EPEVT;

/*
** Accessible information
*/
typedef enum
{
   EPINFO_RTPSTATS,
   EPINFO_T38STATS,
   EPINFO_OVERCURRENTSTATUS,
   EPINFO_LAST
} EPINFO;

/*
** Provisioned values
*/
typedef enum
{
   EPPROV_R0Cadence = 0,
   EPPROV_R1Cadence,
   EPPROV_R2Cadence,
   EPPROV_R3Cadence,
   EPPROV_R4Cadence,
   EPPROV_R5Cadence,
   EPPROV_R6Cadence,
   EPPROV_R7Cadence,
   EPPROV_RGCadence,
   EPPROV_RSCadence,
   EPPROV_RTCadence,
   EPPROV_CallWtgDelay,
   EPPROV_CallWtgMaxRep,

   /* Tones - 22 total (5 unused, 16 used, 1 CAS tone) */
   EPPROV_ToneBusy,                       /* EPSIG_BUSY            */
   EPPROV_ToneConfirmation,               /* EPSIG_CONF            */
   EPPROV_ToneDial,                       /* EPSIG_DIAL            */
   EPPROV_ToneMessageWaiting,             /* EPSIG_MSGW            */
   EPPROV_ToneOffHookWarning,             /* EPSIG_OHWARN          */
   EPPROV_ToneRingBack,                   /* EPSIG_RINGBACK        */
   EPPROV_ToneReOrder,                    /* EPSIG_REORDER         */
   EPPROV_ToneStutterdial,                /* EPSIG_STUTTER         */
   EPPROV_ToneCallWaiting1,               /* EPSIG_CALLW1          */
   EPPROV_ToneCallWaiting2,               /* EPSIG_CALLW2          */
   EPPROV_ToneCallWaiting3,               /* EPSIG_CALLW3          */
   EPPROV_ToneCallWaiting4,               /* EPSIG_CALLW4          */
   EPPROV_ToneAlertingSignal,             /* CAS TONE              */
   EPPROV_ToneSpecialDial,                /* EPSIG_SPECIAL_DIAL    */
   EPPROV_ToneSpecialInfo,                /* ???-04                */
   EPPROV_ToneRelease,                    /* ???-05                */
   EPPROV_ToneCongestion,                 /* EPSIG_NETBUSY         */
   EPPROV_ToneUserDefined1,               /* EPSIG_RINGBACK_CUST1  */
   EPPROV_ToneUserDefined2,               /* ???-07                */
   EPPROV_ToneUserDefined3,               /* ???-08                */
   EPPROV_ToneUserDefined4,               /* ???-09                */


   EPPROV_MinHookFlash,
   EPPROV_MaxHookFlash,
   EPPROV_PlsDlInterdigitTime,
   EPPROV_PlsDlMinMakeTime,
   EPPROV_PlsDlMaxMakeTime,
   EPPROV_PlsDlMinBreakTime,
   EPPROV_PlsDlMaxBreakTime,
   EPPROV_PowerRingFrequency,
   EPPROV_CIDMode,
   EPPROV_CIDFskAfterRing,
   EPPROV_CIDFskAfterDTAS,
   EPPROV_CIDFskAfterRPAS,
   EPPROV_CIDRingAfterFSK,
   EPPROV_CIDDTASAfterLR,
   EPPROV_CIDSigProtocol,

   EPPROV_VMWIMode,
   EPPROV_VMWISigProtocol,
   EPPROV_VMWIFskAfterDTAS,
   EPPROV_VMWIFskAfterRPAS,
   EPPROV_VMWIDTASAfterLR,

   EPPROV_PulseSignalEntry,
   EPPROV_RingCadenceEntry,
   EPPROV_StandardRingCadence,
   EPPROV_RingSplashCadence,

/*
** EMTA management package-specific defines
*/

   EPPROV_AnalogLoopback,
   EPPROV_DiagTestsStart,
   EPPROV_DiagTestsStop,
   EPPROV_DiagTestsValid,
   EPPROV_DiagTestsResult,
   EPPROV_DiagTestsValue,
   EPPROV_DiagTestsFault,
   EPPROV_BoostedLoopCurrent,
   EPPROV_VoiceJitterBuffFixed,
   EPPROV_VoiceJitterBuffMin,
   EPPROV_VoiceJitterBuffMax,
   EPPROV_VoiceJitterBuffTarget,
   EPPROV_DataJitterBuffTarget,
   EPPROV_DataJitterBuffMax,
   EPPROV_DtmfToneRelay,
   EPPROV_TxGain,
   EPPROV_RxGain,
   EPPROV_VBDTxGain,
   EPPROV_VBDRxGain,
   EPPROV_ToneDetectionCtrlExt,
   EPPROV_RingWaveform,
   EPPROV_GR303Support,
   EPPROV_ClidDateMissingAction,
   EPPROV_ToneRfLoss,            /* RF loss */
   EPPROV_ToneEndptDisabled,     /* Endpt disabled */
   EPPROV_ToneEndptDisconnected, /* Endpt disconnected */
   EPPROV_ToneAcLoss,            /* Loss of AC power */
   EPPROV_ToneLowBattery,        /* Low battery */
   EPPROV_ToneBadBattery,        /* Bad battery */
   EPPROV_ToneOverTempBattery,   /* Battery over temperature */

   EPPROV_FixedDtmfPower,

/*
** E-line package-specific defines
*/

   /*
   ** Pulsed signals
   */
   EPPROV_InitRing,
   EPPROV_LoopClose,
   EPPROV_LoopOpen,
   EPPROV_EnableMetering,
   EPPROV_MeteringBurst,
   EPPROV_NoBattery,
   EPPROV_NormalPolarity,
   EPPROV_ReducedBattery,
   EPPROV_ReversePolarity,


   /* Cadence Ringing base enum (i.e. cr(0)) */
   EPPROV_CadenceRingBase,

   /*** DO NOT insert anything here!! ***/

   /* Cadence Ringing max enum (i.e. cr(127)) */
   EPPROV_CadenceRingMax = ( EPPROV_CadenceRingBase + MAX_CADENCE_RING - 1 ),

/*
** End of E-line package-specific defines
*/

   EPPROV_Pulse2DtmfConvert,              /* controls pulse dial to DTMF digit conversion */
   EPPROV_CustomCWCIDCfgEnable,           /* Controls whether EPPROV_CASLevelCfg and EPPROV_FSKLevelCfg are used */
   EPPROV_CASLevelCfg,                    /* Custom CAS tone level in dBm */
   EPPROV_FSKLevelCfg,                    /* Custom FSK tone level in dBm */

/*
** Enhanced NCS signalling specific defines
*/

   EPPROV_FaxDetection,
   EPPROV_DTMFPlayout,

   EPPROV_PcDiagTestsStart,
   EPPROV_PcDiagTestsClear,
   EPPROV_PcDiagTestsValid,
   EPPROV_PcDiagTestsResult,

   EPPROV_HighVring,                      /* High voltage ringing */
   EPPROV_RingDCOffset,                   /* Ring offset */
   EPPROV_RingOffsetCal,                  /* Ring offset calibration */
   EPPROV_LoadBalancing,

   /* Items for notifying when line tests is in progress or done */
   EPPROV_LineTestInProgress,
   EPPROV_LineTestDone,

/*
** SIP specific defines
*/

   EPPROV_HookFlashRelay,
   EPPROV_PreferredVBDCodec,

/*
** PC 2.0 defines
*/
   EPPROV_HookState,
   EPPROV_NetDisc,
   EPPROV_AnsSup,
   EPPROV_G711Pkt,
   EPPROV_T38,
   EPPROV_RTCPXR,
   EPPROV_RTCPRATE,   

   EPPROV_LastItem, /* Must be the last item */

} EPPROV;

/*
** Returned status
*/
typedef enum
{
   EPSTATUS_SUCCESS = 0,         /* success */
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
   EPSTATUS_RESET_PENDING,       /* reset is pending */
   EPSTATUS_ERROR                /* general error */
} EPSTATUS;

/* Data Mode Types */
typedef enum
{
   EPDATAMODE_NONE,          /* Disable CDIS */
   EPDATAMODE_VBD,           /* VBD (no FAXR or auto encoder switch) - default mode */
   EPDATAMODE_VBD_AUTOENC,   /* VBD with automatic encoder switch to pcmu */
   EPDATAMODE_T38,           /* Use T38 fax relay (no auto encoder switch) */
   EPDATAMODE_T38_AUTOENC,   /* Use T38 fax relay with automatic encoder switch */
   EPDATAMODE_T38_MUTE       /* Use T38 fax relay (but mute until instructed to switch to T.38) */
} EPDATAMODE;


/* Auto Encoder switch */
typedef enum
{
   EPAUTOENC_OFF = 0,   /* Auto encoder switch is turned off */
   EPAUTOENC_ON         /* Auto encoder switch is turned on */
} EPAUTOENC;

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


/* VBD parameters */
typedef struct
{
   EPVBDMODE    vbdMode;
   EPVBDREDMODE vbdRedMode;
} EPZVBDPARAM;

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
#define EPTONEDET_FAX_DISABLED         0x80
#define EPTONEDET_MODEM_DISABLED       0x40
#define EPTONEDET_V18BAUDOT_DISABLED   0x20
#define EPTONEDET_V18PREAMBLE_DISABLED 0x10
#define EPTONEDET_V18ASCII_DISABLED    0x08
#define EPTONEDET_ALL_V18_DISABLED     (EPTONEDET_V18BAUDOT_DISABLED|EPTONEDET_V18PREAMBLE_DISABLED|EPTONEDET_V18ASCII_DISABLED)
#define EPTONEDET_ALL_DISABLED         (EPTONEDET_FAX_DISABLED|EPTONEDET_MODEM_DISABLED|EPTONEDET_ALL_V18_DISABLED)

/*
** Connection parameters
*/
typedef struct
{
   EPCNXMODE            mode;             /* connection mode */
   CODECCNXPARMLIST     cnxParmList;      /* list (in ordered preference) of negotiated
                                           * codecs/packetization period between the local
                                           * and remote endpoint */
   BOS_UINT32        namedPhoneEvts;   /* bit-field of negotiated named telephone events */
   BOS_UINT16        echocancel;       /* echo cancellation */
   BOS_UINT16        silence;          /* silence suppression */
   BOS_UINT16        comfortNoise;     /* comfort noise */
   BOS_UINT16        preserveFaxMode;  /* preserve a fax mode (if we are in already) */
   BOS_UINT16        secHdrSize;       /* Authentication mode -- security header size */
   EPDATAMODE           dataMode;         /* fax relay, vbd, or none */
   EPAUTOENC            autoEncoder;      /* Auto Encoder Switch */
   EPT_T38_PARM      t38param;         /* T.38 fax parameters */
   EPRTCPXRMODE         rtcpXRMode;       /* rtcp XR mode -- on, off or negotiate */
   EPZVBDPARAM          vbdparam;         /* VBD parameters */
   EPDTMFRFC2833CONFIG  digitRelayType;   /* DTMF digit relay type. */
   BOS_BOOL          bEnableRTCP;      /* set if RTCP is supported */

} EPZCNXPARAM;

/*
** Connection statistics
*/
typedef struct
{
   BOS_UINT32     txpkts;           /* packets sent */
   BOS_UINT32     txbytes;          /* octets sent */
   BOS_UINT32     rxpkts;           /* packets received */
   BOS_UINT32     rxbytes;          /* octets received */
   int     lost;             /* packets lost */
   BOS_UINT16     jitter;           /* interarrival jitter estimate */
   BOS_UINT16     latency;          /* avg tx delay */
} EPZCNXSTATS;

/*
** Reason code associated with events reported from the endpoint module
** to the upper layer application.  It is up to the application receiving
** the event to use the reason code for event processing.
*/
typedef enum
{
   EPEVTREASON_PTSW,         /* Payload type switch. */
   EPEVTREASON_CNG,          /* CNG signal. */
   EPEVTREASON_V21flag,      /* V.21 flag. */
   EPEVTREASON_V21tone,      /* V.21 tone. */
   EPEVTREASON_BAUDOT,       /* V.18 Baudot tone and character. */
   EPEVTREASON_BELLTONE,     /* Bell 103 carrier. */
   EPEVTREASON_ANS,          /* V.25 ANS. */
   EPEVTREASON_prANS,        /* V.25 ANS with phase reversal. */

} EPEVTREASON;


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
   BOS_UINT16  maxCodec;   /* maximum number of simultaneous operation
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
** PARAMETERS: lineId      - line ID
**             cnxId       - the connection identifier (-1 if not applicable)
**             event       - the event
**             data        - extra information (example: reason code).
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*endptEventCallback)( int lineId, int cnxId,
                                    EPEVT event, int data );

/*
*****************************************************************************
** FUNCTION:   endptRtpPacketCallback
**
** PURPOSE:    Endpoint connection packet callback
**
** PARAMETERS: cnxId       - the connection id.
**             packet      - the endpt packet
**             length      - the endpt packet length including header
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void (*endptPacketCallback)
(
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
** PARAMETERS: lineId         - line ID
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
** PARAMETERS: lineId         - line ID
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
   void          *length
);


/*
*****************************************************************************
** FUNCTION:   endptPacketReleaseCallback
**
** PURPOSE:    Callback handler invoked when the packet passed to endptRtpPacket
**             is released by the endpoint back to the caller.
**
** PARAMETERS: cxid        - connection identifier
**             bufDesc     - buffer descriptor value passed to endptRtpPacket
**
** RETURNS:    nothing
**
*****************************************************************************
*/
typedef void ( *endptPacketReleaseCallback )
(
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
** PARAMETERS: lineId         - line ID
**             capabilities   - a pointer to the capabilities struct to populate
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS endptCapabilities
(
   int            lineId,
   EPZCAP        *capabilities
);

/*
*****************************************************************************
** FUNCTION:   endptCreateConnection
**
** PURPOSE:    Function to create a connection
**
** PARAMETERS: lineId      - line ID
**             cnxId       - connection identifier
**             cnxParam    - ptr to connection parameter block
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS endptCreateConnection
(
   int            lineId,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
);

/*
*****************************************************************************
** FUNCTION:   endptModifyConnection
**
** PURPOSE:    Function to modify a connection
**
** PARAMETERS: lineId      - line ID
**             cnxId       - connection identifier
**             cnxParam    - ptr to connection parameter block
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS endptModifyConnection
(
   int            lineId,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
);

/*
*****************************************************************************
** FUNCTION:   endptDeleteConnection
**
** PURPOSE:    Function to delete a connection
**
** PARAMETERS: lineId      - line ID
**             cnxId       - connection identifier
**
** RETURNS:    EPSTATUS - if success, fills in statistics block
**
** NOTE:
*****************************************************************************
*/
EPSTATUS endptDeleteConnection( int lineId, int cnxId );

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
** PARAMETERS: lineId     - line ID
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
EPSTATUS endptPacket
(
   int            lineId,
   int            cnxId,
   EPPACKET      *epPacket,
   int            length,
   unsigned int   bufDesc
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
void endptPowerSourceChanged
(
   int   newPowerSource
);

/*
*****************************************************************************
** FUNCTION:   endptGetInfo
**
** PURPOSE:    Generic information access function
**
** PARAMETERS: lineId      - line ID
**             cnxId       - connection identifier
**             info        - information type code (see EPINFO)
**             data        - Pointer to the structure to be filled in with the
**                           requested information
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS endptGetInfo
(
   int            lineId,
   int            cnxId,
   EPINFO         info,
   void           *data
);


/*
*****************************************************************************
** FUNCTION:   endptMuteConnection
**
** PURPOSE:    Toggle the mute status of a connection on the endpoint.
**
** PARAMETERS: lineId      - line ID
**             cxid        - connection identifier
**             mute        - mute status for the connection
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS endptMuteConnection
(
   int               lineId,
   int               cxid,
   BOS_BOOL          mute
);


#ifdef __cplusplus
}
#endif


#endif  /* ENDPT_H  */
