/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
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
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
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
*    Filename: vrgEndptPrivate.c
*
****************************************************************************
*    Description:
*
*      This file contains all the private data types/function used by the
*      endpoint.
*
****************************************************************************/
#ifndef VRG_ENDPT_PRIVATE_H
#define VRG_ENDPT_PRIVATE_H

#if defined(__cplusplus)
extern "C"
{
#endif

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

#define getMipsCycles(dest) __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest))

#define  CNX_UNUSED   -1
#if FLEXIBLE_CONFERENCING_SUPPORTED
#define  CNX_IDLE     -2 /* allocated, but not active */
#endif
#define  BAD_ENDPTID  -1

/* connection index into the endpoint's cnx queue */
#define  PRIMARY_CNX_INDEX 0
#define  SECONDARY_CNX_START_INDEX 1

/* Maximum number of rows in the codec table.  This number should
** really be determined by the maximum number of codecs as well as
** the maximum number of cnxs supported.  For now, we will hard-coded
** the number to account for the worst-case scenario */
#if 0
#define VRG_CODEC_TABLE_MAX_ROWS    100
#endif

/* Set to 1 to enable packet loopback between endpt 0 and 1. */
#define PACKET_LOOPBACK    0

#define DPR_ENABLED        0

#define VRG_ENDPT_PRINT_RTP_STATS         0


/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/

/*
** Endpoint initialization state - keep track of which sub-modules have been
** initialized. This is important for a module de-initialization. Only modules
** that have been initialized should be de-initialized.
*/
typedef enum EPEINIT_STATE
{
   EPEINIT_CAS                   = (1 << 0),
   EPEINIT_VRGDISP               = (1 << 1),
   EPEINIT_HDSP                  = (1 << 2),
   EPEINIT_TPD                   = (1 << 3),
   EPEINIT_HEARTBEAT             = (1 << 4),
   EPEINIT_LHAPI                 = (1 << 5),
   EPEINIT_CNXVHD                = (1 << 6),
   EPEINIT_CNXSTATE              = (1 << 7),

} EPEINIT_STATE;


/*
** Endpoint Configuration structure
*/
typedef struct ENDPTZCONFIG
{
   VRG_UINT32                 initState;         /* Initialization state */
   VRG_COUNTRY                country;           /* Country code */
   endptEventCallback         notify;            /* event callback function */
   endptPacketCallback        packet;            /* egress packet callback function */
   endptGetProvCallback       getProvision;      /* get provisioning callback function */
   endptSetProvCallback       setProvision;      /* set provisioning callback function */
   endptPacketReleaseCallback packetRelease;     /* endpt packet release callback function */
   int                        acPowerHBId;       /* Heartbeat patient ID for the AC Power */
   int                        ringing;           /* Information on current CAS ring status */
   VRG_BOOL                   bGR303Support;     /* Flag set if GR303 is supported */
   
} ENDPTZCONFIG;

/*
** Fax-related detected events
*/
typedef enum EPFAX_EVT
{
   EPFAX_EVT_NONE,
   EPFAX_EVT_CNG_INGRESS,     /* Originating CNG */
   EPFAX_EVT_HDLC,            /* Originating / Destination HDLC */
   EPFAX_EVT_CDIS_EGRESS,     /* Egress CIDS */

} EPFAX_EVT;

/*
** Mapping from Endpoint signals to PTE tone profiles
*/
typedef struct EPZTONEMAP
{
   VRG_UINT16     event;
   int            provItem;

} EPZTONEMAP;


/*
** Mapping from Endpoint digit signals to PTE digits
*/
typedef struct EPZSIG2PTEDIGITMAP
{
   EPSIG             signal;   /* EP signal */
   HAPI_PTE_DTMFDDGT digit;    /* PTE digit */

} EPZSIG2PTEDIGITMAP;

/*
** Mapping from PTE digits to Endpoint Events
*/
typedef struct EPZPTEDIGIT2EPEVTMAP
{
   HAPI_PTE_DTMFDDGT digit;      /* PTE digit */
   EPEVT             epEvent;    /* EP event */
   CAS_CTL_EVENT     casEvent;   /* CAS event */

} EPZPTEDIGIT2EPEVTMAP;

/*
** Mapping from generic codec type to DSP specific payload code
*/
typedef struct EPZRATEMAP
{
   CODEC_TYPE codec;
   VRG_UINT16 rate;

} EPZRATEMAP;


/*
** Voiceband data state
*/
typedef struct
{
   VRG_BOOL          bEnabled;         /* Voiceband data enable flag */
   EPZVBDPARAM       param;            /* Voiceband data parameters */

} EPZVBDATASTATE;


/*
** Voice mode associated with a connection. The default voice mode is VM_IDLE,
** which means the endpoint associated with the connection is onhook and there
** is no active connection. The voice mode will transition to VM_ACTIVE
** when either the endpoint goes offhook or a connection is created. When the
** connection is deleted, the voice mode will transition back to VM_IDLE.
*/
typedef enum
{
   VM_IDLE,             /* Idle mode, onhook with no active connection */
   VM_ACTIVE,           /* Active voice connection */
   VM_FAX_RELAY         /* T.38 fax call. */
} VOICEMODE;

typedef struct
{
   VRG_UINT8    event;
   VRG_UINT8    volume;
   VRG_UINT16   duration;
} NTE_PAYLOAD;

/*
** Per-source state information
*/
typedef struct RTPSTATS
{
   unsigned char  bPaddingByte;    /* padding octet                   */
   VRG_UINT16     wExtensionByte;  /* extension octet                 */
   VRG_UINT16     wCSRCByte;       /* CSRC octet                      */

   VRG_UINT16     uPktIngress;     /* packets arriving from DSP       */
   VRG_UINT16     uPktEgress;      /* packets arriving from network   */
   VRG_UINT16     uPktIngressProcessed;     /* packets successfully sent from endpoint to network  */
   VRG_UINT16     uPktEgressProcessed;      /* packets successfully sent from endpoint to DSP      */

} RTPSTATS;

typedef struct RTPSTATE
{
   RTPSTATS     stats;

   VRG_UINT8    sendNtePayloadType;             /* NTE RTP payload type for local codec */
   VRG_UINT8    recvNtePayloadType;             /* NTE RTP payload type for remote codec */

   VRG_UINT8    sendRedPayloadType;             /* RED RTP payload type for local codec */
   VRG_UINT8    recvRedPayloadType;             /* RED RTP payload type for remote codec */
   VRG_UINT8    redLevel;                       /* RED level */

   int          rtpCnxId;                       /* save connection id for RTCP packet */
   void         *pPayload;

} RTPSTATE;

/*
** RTP Packet Types
*/
typedef enum RTPPKT_TYPE
{
   RTPPKT_MEDIA,
   RTPPKT_DTMF_EVENT,
   RTPPKT_RING_EVENT,
   RTPPKT_RINGOFF_EVENT,
   RTPPKT_OSI_EVENT,
   RTPPKT_ONHOOK_EVENT,
   RTPPKT_UNSUPPORTED
} RTPPKT_TYPE;


/*
** Mapping of endpt & cnxid
**
** Note: Both inMask and outMask are bit fields, with each bit indicating whether
**       the cnx contains a DSP switchboard cnx from/to a set of netside/lineside VHDs.
**       A "set" bit field denotes a SB cnx, whereas a "clear" bit indicates no cnx.
**
**       The bit field takes on the following form: Vm-1...V0 Pn-1...P0
**       where V = VHDhandle index, P = PHDhandle index, m = VRG_ENDPT_CFG_NUM_CNX, and
**       n = VRG_ENDPT_CFG_NUM_ENDPT.
*/
typedef struct CNXSTATE
{
   int            dspNum;                    /* DSP number */
   int            cnxId;                     /* Connection index */
   int            lineId;                    /* Associated line id */
   VRG_UINT16     vhdhdl;                    /* Connection VHD handle */
   VRG_UINT16     vhdMode;                   /* Connection VHD mode */
   EPCNXMODE      mode;                      /* Connection mode */
   VOICEMODE      voiceMode;                 /* Voice mode */
   VRG_UINT16     period;                    /* packetization period (msec) */
   VRG_UINT16     silence;                   /* Silence suppression enabled/disabled */
   VRG_UINT16     comfortNoise;              /* Comfort noise value */
   CODEC_TYPE     codecType;                 /* Codec type, e.g. CODEC_G728 */
   int            firstValidRTPPkt;          /* Used for media start event */
   VRG_BOOL       activeSignal;              /* Active signal on connection flag */
   EPZVBDATASTATE vbData;                    /* Voiceband data mode */
   RTPSTATE       rtpstate;                  /* RTP state */
#if !FLEXIBLE_CONFERENCING_SUPPORTED
   VRG_BOOL       bPrimary;                  /* Primary connection - each endpt must
                                                be assigned at least one cnx, referred
                                                to as the primary cnx */
#endif
   VRG_BOOL       bLiteWeight;               /* Lite-weight connnetions do not support
                                                advanced codecs */
   int            inMask;                    /* Input mask for SB configurations */
   int            outMask;                   /* Output mask for SB configurations */
   VRG_BOOL       digitRelay;                /* digit relay mode */
   EPDTMFRFC2833CONFIG digitRelayType;       /* Type of digit relay (no relay, RFC 2833, subtracted RFC 2833) */
   VRG_UINT16     toneRelayRefreshInterval;  /* Tone relay refresh interval */
   EPDATAMODE     dataMode;                  /* fax/data mode */
   VRG_UINT32     namedPhoneEvts;            /* bit-field of named telephone events */

   VRG_UINT16     lastFaxEvent;              /* Identify the last fax-related event detected
                                             ** on the endpt.  We need to keep track of
                                             ** the last recorded event so that we can
                                             ** configure the right faxr start mode
                                             ** when we switch to faxr */

   VRG_BOOL       vrgVoiceJBFixed;           /* VRG_TRUE - fixed, VRG_FALSE - adaptive */
   VRG_UINT16     vrgVoiceJBMin;             /* Minimum size for voice JB. */
   VRG_UINT16     vrgVoiceJBMax;             /* Maximum size for voice JB. */
   VRG_UINT16     vrgVoiceJBTarget;          /* Target size for voice JB. */
   VRG_UINT16     vrgDataJBTarget;           /* Target size for data JB. */
   VRG_UINT16     vrgDataJBMax;              /* Maximum size for data JB. */

   int            cnxStateHBId;              /* Heartbeat patient ID for this cnx's activity */

   VRG_BOOL       bCngDetEnabled;            /* CNG detection enabled flag
                                             ** ( CNG detection is on by default) */
   VRG_BOOL       bNotifyFaxCall;            /* Bit set if we've notified the start of a fax call
                                             ** (either via CNG or V.21 fax preamble detection) */

   VRG_BOOL       bMuteAudio;                /* Bit set if we are supposed to mute the audio. */
   VRG_BOOL       bMuteT38;                  /* Bit set if we are supposed to mute in T.38 mode.
                                             ** we are using the fax package and we are waiting
                                             ** for an RCD w/ T.38 support */
   VRG_BOOL       bBell103Enabled;           /* Bell103 detection enabled flag
                                             ** ( Bell103 detection is off by default) */
   VRG_UINT16     toneDetCtrl;               /* Contains MIB values of tone Detection settings (which ones are disabled) */     
   EPZT38PARAM    t38param;                  /* T.38 fax parameters */
   int            rxVolume;                  /* Current volume setting for this connection */
   int            txVolume;                  /* Current volume setting for this connection */   
   HAPI_RTP_REGS  rtpRegs;                   /* Copy of RTP service registers for this cnx */
   HAPI_RTP_REGS2 rtpRegs2;                  /* Copy of 2nd set of RTP service registers for this cnx */
   HSZNETREGS     netRegs;                   /* Copy of Net regisster for this cnx. */
   VRG_UINT16     VadMode;                   /* Current VAD mode */
   VRG_UINT16     CngMode;                   /* Current CNG mode */
   VRG_UINT16     PrevVadMode;               /* Previous VAD mode */   
   VRG_UINT16     PlcMode;                   /* Current PLC mode */
   VRG_BOOL       bSBDisconnect;             /* Force SB disconnect of cnx vhd and line vhd, for collecting DTMF and playing upon request */
   VRG_UINT32     localSsrc;                 /* Local SSRC for ingress RTP packets */
   VRG_BOOL       cnxModeControlsLineVhd;    /* If the value is 1 the connection mode applies to all switchboard
                                             ** connections going out of the line VHD */
   VRG_UINT16     ajcCustomReg;              /* Stores the value of the custom features register for AJC */
   VRG_UINT16     faxrCustomReg;             /* Stores the value of the custom features register for FAXR */
   VRG_BOOL       csCall;                    /* Call connection type */
   VRG_BOOL       bDisableUSPkt;             /* Flag to indicate if the application is
                                             ** accepting upstream packets.  When this
                                             ** flag is set to true, the application
                                             ** is accepting upstream packets only if
                                             ** one of the following cases apply:
                                             **    - corresponding line is offhook
                                             **    - ingress tone is being played
                                             **    - GR303 events
                                             ** For all other scenarios, ingress packets
                                             ** should be dropped by the endpoint.  We
                                             ** need to add the ability to disable
                                             ** ingress packets because we are observing
                                             ** jittering conditions when packets are
                                             ** being sent on the Best Effort Service
                                             ** Flow.
                                             **
                                             ** When this flag is set to false, the
                                             ** application is accepting all ingress
                                             ** packets */
} CNXSTATE;


/*
** Events of interest for Ecan Control
*/
typedef enum ECANEVT
{
   ECANEVT_CRCX_ON,     /* CRCX event requesting Ecan ON */
   ECANEVT_CRCX_OFF,    /* CRCX event requesting Ecan OFF */
   ECANEVT_MDCX_ON,     /* MDCX event requesting Ecan ON */
   ECANEVT_MDCX_OFF,    /* MDCX event requesting Ecan OFF */
   ECANEVT_DLCX,        /* DLCX event (implies Ecan OFF) */
   ECANEVT_CNG,         /* CDIS CNG event */
   ECANEVT_HDLC,        /* CDIS v.21 HDLC event */
   ECANEVT_MACHINE,     /* CDIS MACHINE event */
   ECANEVT_PHREV,       /* CDIS Phase Reversal event */
   ECANEVT_LOWEN,       /* CDIS Low Energy event */
   ECANEVT_V18,         /* CDIS V18 event */
   ECANEVT_BELL103,     /* CDIS BELL103 event */
   ECANEVT_ONHOOK,      /* ONHOOK event */
   ECANEVT_OFFHOOK,     /* OFFHOOK event */
   ECANEVT_DIALTONE_ON, /* Dialtone ON event */
   ECANEVT_DIALTONE_OFF /* Dialtone OFF event */
} ECANEVT;

/*
** Endpoint test modes
*/
typedef enum
{
   TESTMODE_NONE           = 0x00000000,
   TESTMODE_TR57           = 0x00000001,
   TESTMODE_ANALOGLOOPBACK = 0x00000002,
   TESTMODE_PACKETLOOPBACK = 0x00000004,
   TESTMODE_LINELOOPBACK   = 0x00000008,
   TESTMODE_HALLOOPBACK    = 0x00000010,
   TESTMODE_LINEDIAG       = 0x00000020,
   TESTMODE_ECANSET        = 0x00000040,
   TESTMODE_VADSET         = 0x00000080,
   TESTMODE_ECANDEBUGCMD
} ENDPTTESTMODE;

/*
** Echo Canceller states
*/
typedef enum EPEECANSTATE
{
   ECAN_OFF = 0,
   ECAN_ON,
   ECAN_NLP_OFF,
#ifdef VRG_ENDPT_CFG_G168_TEST 
   ECAN_NLP_ON,
   ECAN_ADAPT_OFF,
   ECAN_ADAPT_ON,
   ECAN_BYPASS_OFF,
   ECAN_BYPASS_ON,
   ECAN_CNG_ON,
   ECAN_CNG_OFF,
   ECAN_H_RESET,
   ECAN_RESET
#endif
} EPEECANSTATE;

typedef enum EP_CAS_CTL_BITS
{
   /* Loop current feed. */
   EP_CAS_CTL_LCF,

   /* Loop current feed open. */
   EP_CAS_CTL_LCFO,

   /* Reverse loop current feed. */
   EP_CAS_CTL_RLCF,

   /* Ringing. */
   EP_CAS_CTL_RINGING,
} EP_CAS_CTL_BITS;


/*
** Echo cancelation state info
*/
typedef struct EPZECANSTATE
{
   EPEECANSTATE   currState;           /* Current Echo canceller state */
   EPEECANSTATE   origState;           /* Original state of the echo cannceller
                                          (before endpt took over the control) */
   VRG_BOOL       isNLPoff;            /* boolean flag describing ECAN NLP state */
} EPZECANSTATE;

typedef struct EPRINGPARAM
{
   int frequency; /* Ring frequency */
   int waveshape; /* Ring waveshape */
   int voltage;   /* RMS Ring voltage */
   int offset;    /* Ring offset voltage */
   int offsetCal; /* Ring offset calibration voltage */
} EPRINGPARAM;

/*
** Endpoint State structure
**
** Note: 'cnx' is an array of connections associated with an endpoint. Each endpoint
**       has one "primary" connection associated with it. The primary connection
**       is bound to a specific endpt. i.e. All endpt must be capable of supporting
**       at least a single connection. Any additional connections created on the
**       endpoint are refered to as "secondary". Secondary connections come from
**       the global connection pool: 'vrgCnxState' (as do primary connections). The
**       number of available secondary connections for all VRG endpoints (not
**       per endpoint) is: VRG_ENDPT_CFG_NUM_CNX - VRG_ENDPT_CFG_NUM_ENDPT. However, each
**       endpoint is restricted to VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT connections (primary and
**       secondary). Secondary connections are intended to be used for conferencing
**       or call-waiting applications.
*/
typedef struct VRG_ENDPT
{
   int               dspNum;                 /* DSP number */
   VRG_BOOL          isLineUsed;             /* line usage flag */
   int               lineId;                 /* endpoint handle */
   VRG_UINT16        ldxHalHdl;              /* Hausware HAL handle */
   VRG_UINT16        lineVhdHdl;             /* line VHD handle */
   VRG_UINT16        intLinkVhdHdl;          /* line VHD handle of other line VHD that this endpoint has been internally linked to*/
   VRG_UINT16        endptNum;               /* endpoint number (0 referenced) */
   VRG_UINT16        offHook;                /* hook state */
   VRG_COUNTRY       country;                /* country code */
   CLASSSTM_MEM      classState;             /* caller id info */
   EP_CAS_CTL_BITS   casCtlBits;               /* the current value of the endpoint CAS control bits */
   EPZECANSTATE      ecanState;              /* echo cancellation state */
   CNXSTATE*         cnx[VRG_ENDPT_CFG_MAX_CNX_PER_ENDPT]; /* an array of cnxs on an endpt basis */
   ENDPTTESTMODE     testMode;               /* test mode */
   VRG_BOOL          testModeUpdate;         /* endpoint updates occurred during testing */
   int               diagVadMode;            /* diag vad mode */
   CAS_CTL_HANDLE    casCtlHandle;           /* Handle for CAS control module. */
   ENDPT_STATE      *endptArchive;
   VRG_UINT16        signalRep;              /* Number of times to repeat a signal */
   VRG_UINT16        earlyOffHook;           /* The non-debounced slic state */
   VRG_UINT16        delayOffHook;           /* The non-debounced slic state with delay for endpoint muting */

   VRG_BOOL          bPulse2dtmf;            /* enable/disable pulse dial to dtmf conversion */
   VRG_BOOL          bGR303Support;          /* enable/disable GR303 support (aka RFC 2833 for trunking aka Nuera support) */
#if VRG_ENDPT_CFG_USE_HFE
   BOS_TIMER         hookflashTimer;         /* Timer required for hookflash emulation */
#endif

   int               hookStateHBId;          /* Heartbeat patient ID for this endpoint's offhook state */
   int               signalHBId;             /* Heartbeat patient ID for this endpoint's brief/timeout signal activity state */
   int               signalCallerIdHBId;     /* Heartbeat patient ID for this endpoint's caller ID signal activity state */
   int               signalOnOffHBId;        /* Heartbeat patient ID for this endpoint's on/off signal activity state */
   int               cnxSignalHBId;          /* Heartbeat patient ID for this endpoint's cnx signal activity state */
   int               battSignalHBId;         /* Heartbeat patient ID for this endpoint's battery signal activity state */
   int               endptCfgHBId;           /* Heartbeat patient ID for this endpoint's config / query activity state */

   EPSIG             activeBriefSignal;      /* Current active brief signal */
   VRG_UINT16        cnxCnt;                 /* Number of connections on this Endpoint */
   EPTYPE            endptType;              /* FXO/FXS type of endpoint */
   int               txGain;                 /* Current gain setting for this endpoint */
   int               rxGain;                 /* Current gain setting for this endpoint */
   int               ringWaveshape;          /* Ring waveshape */
   int               ringFrequency;          /* Ring frequency */      
#ifdef VRG_ENDPT_CFG_G168_TEST
   VRG_UINT16        ecanOptions;            /* ECAN Options for G.168 Compliance testing */
#endif
#ifdef HDSP_CFG_CLIDRX
   HDSP_CLASS_CLIDRX_STATE clidRxState;      /* endpoint CLIDRX state */
#endif
   VRG_UINT16        pstnVhdHdl;             /* Needed for FXO Ept only. This is a new requirement for LDX2.1 */
   int               routedFxsLineId;        /* For the FXO channel, this is the line id
                                             ** of the FXS channel that is routed over FXO.
                                             ** For the FXS channels, this value is set to -1.
                                             ** If this value is equal to the channel identifier,
                                             ** then it is an FXO channel routed over VoIP. */
   VRG_BOOL          isFxsFxoCnxEstablished; /* True if the connection between the FXS
                                             ** channel and the FXO channel is established */
   VRG_UINT16        confCnxVhdHdl;          /* Conference connection handle */

   EPRINGPARAM       ringParam;              /* Parameterization of Ring Waveform */
   int               ringOffset;             /* Ring offset voltage */
   int               ringOffsetCal;          /* Ring offset calibration voltage */   
} VRG_ENDPT;


/*
** cnx mode directions
*/
typedef enum
{
  SENDMODE,
  RECVMODE
} CNX_DIRECTION;

/*
** VRG Event Structure
*/

typedef enum VRG_EVT_TYPE
{
   VRG_EVT_TYPE_CAS,
   VRG_EVT_TYPE_HAPI_RM,
   VRG_EVT_TYPE_HAPI_ENDPT,
   VRG_EVT_TYPE_HAPI_CNX,
   VRG_EVT_TYPE_NTE,
   VRG_EVT_TYPE_PSTN,
#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
   VRG_EVT_TYPE_TPD,
#else
   VRG_EVT_TYPE_MLT,
#endif
#endif
   VRG_EVT_TYPE_DECT
} VRG_EVT_TYPE;

typedef struct VRG_CAS_EVT
{
   CAS_CTL_HANDLE       handle;
   CAS_CTL_EVENT_TYPE   eventType;
   CAS_CTL_EVENT        eventId;

} VRG_CAS_EVT;

typedef struct VRG_HAPI_EVT
{
   VRG_UINT16  dspNum;
   VRG_UINT16  reqId;
   VRG_UINT16  handle;
   VRG_UINT16  eventId;
   VRG_UINT16  op1;
   HSUOP2      op2;

} VRG_HAPI_EVT;

typedef struct VRG_NTE_EVT
{
   int lineNum;
   VRG_SINT32 id;

} VRG_NTE_EVT;

typedef struct VRG_PSTN_EVT
{
   VRG_UINT16          vhdHdl;
   PSTN_CTL_EVENT_INFO *pEventInfo;
   
} VRG_PSTN_EVT;

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
typedef struct VRG_DECT_EVT
{
   VRG_UINT16 chanNum;
   DECT_EVENT eventId;
   void*      pData;
   VRG_UINT32 uSize;

} VRG_DECT_EVT;
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
typedef struct VRG_TPD_EVT
{
   TPD_CTL_EVENT eventId;
   VRG_BOOL      midSet;
   void*         provValue;
   int           chan;
   
} VRG_TPD_EVT;
#else
typedef struct VRG_MLT_EVT
{
   MLT_PORT    portId;
   MLT_REPORT* pResult;
   
} VRG_MLT_EVT;
#endif
#endif

typedef struct VRG_EVT
{
   VRG_EVT_TYPE      type;

   union
   {
      VRG_CAS_EVT    cas;
      VRG_HAPI_EVT   hapi;
      VRG_NTE_EVT    nte;
      VRG_PSTN_EVT   pstn;
#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
      VRG_DECT_EVT   dect;
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
      VRG_TPD_EVT    tpd;
#else
      VRG_MLT_EVT    mlt;
#endif
#endif      
   } evt;

} VRG_EVT;


/*
** VRG Endpoint API Commands
*/
typedef enum VRG_EPT_CMD_TYPE
{
   VRG_EPT_CREATE,
   VRG_EPT_DESTROY,
   VRG_EPT_CREATE_CNX,
   VRG_EPT_DELETE_CNX,
   VRG_EPT_MODIFY_CNX, 
   VRG_EPT_MUTE_CNX,
   VRG_EPT_LINK_INT,
   VRG_EPT_CAPABILITIES, 
   VRG_EPT_SIGNAL,
   VRG_EPT_GET_RTP_STATS,
   VRG_EPT_GET_T38_STATS,
   VRG_EPT_NTR_CMD,
   VRG_EPT_CONSOLE_CMD

} VRG_EPT_CMD_TYPE;

/*
** VRG Endpoint API Command structure
*/
typedef struct VRG_EPT_CMD
{
   VRG_EPT_CMD_TYPE command;       /* Command name */
   unsigned int     arguments[10]; /* Command arguments */
   EPSTATUS         status;        /* Command */
   
} VRG_EPT_CMD;


#if defined(__cplusplus)
}
#endif

/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/

/* VRG command and event dispatcher task id variable */
extern BOS_TASK_ID vrgDispProcessTaskId;

/* VRG endpoint command Queue */
extern MEMCIRCBUFDRV vrgEptCmdQ;

/* VRG Event Queue */
extern MEMCIRCBUFDRV vrgEventQ;

/* Binary semaphore for queues services by dispatcher */
extern BOS_SEM       vrgDispSem;

/* Binary semaphore for synchronizing VRG endpoint API commands */
extern BOS_SEM       vrgEptApiSem;

/* Binary semaphore for synchronizing PSTN callback commands */
extern BOS_SEM       vrgPstnSem;

#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
/* Binary semaphore for synchronizing DECT callback commands */
extern BOS_SEM       vrgDectSem;
#endif

#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
/* Binary semaphore for synchronizing TPD callback commands */
extern BOS_SEM       vrgTpdSem;
#else
/* Binary semaphore for synchronizing MLT callback commands */
extern BOS_SEM       vrgMltSem;
#endif
#endif
/* Binary semaphore for synchronizing NTE callback commands */
extern BOS_SEM       vrgNteSem;

extern ENDPTZCONFIG endptConfig;

/*
** Mapping from PTE and CAS events to Endpoint events
*/
extern const EPZPTEDIGIT2EPEVTMAP digit2epevtMap[];

/*
** Default Endpoint State
*/
extern VRG_ENDPT vrgEndpt[VRG_ENDPT_CFG_NUM_ENDPT];

/*
**  Note: If PSTN is enabled then we have an additional Endpoint which is of type
**  PSTN. However, this leads to the unusual situation that the number of endpoints
**  is actually greater than the number of possible connections. Thus, the size of
**  this array must be increased by 1 to compensate.
*/
extern CNXSTATE vrgCnxState[VRG_ENDPT_CFG_NUM_CNX+1];


/*
** Default Endpoint Capabilities
*/
extern EPZCAP vrgEndptCap[VRG_GLOBAL_CFG_MAX_NUM_DSP];

/*
** Codec Table
*/
#if 0
extern VRG_CODEC_TABLE_ROW   vrgCodecTable[VRG_CODEC_TABLE_MAX_ROWS];
#endif
/*
** DSP capabilities - this list needs to be expanded when we have multi-DSP support
*/
extern BOARD_HAL_DSP_CFG   gDspCfg;

extern const ENDPT_FUNCS gVrgEndptFuncs;

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

extern void RmEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum );
extern void CnxVhdEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum );
extern void EndptVhdEventCallback( HSZEVT *evtp, VRG_UINT16 dspNum );
extern void CasEventCallback( CAS_CTL_HANDLE handle, CAS_CTL_EVENT_TYPE eventType, CAS_CTL_EVENT event );
extern void PstnEventCallback( VRG_UINT16 vhdHdl, PSTN_CTL_EVENT_INFO *pEventInfo );
#if VRG_ENDPT_CFG_USE_TPD
#ifndef VRG_ENDPT_CFG_USE_MLT
extern void TpdEventCallback(TPD_CTL_EVENT event, VRG_BOOL mibSet, void *value, int chan);
#else
extern void MltCallback( MLT_PORT portId, MLT_REPORT *pResult );
#endif
#endif

VRG_SINT16 VrgDispatcherProcessInit(void);
VRG_SINT16 VrgDispatcherProcessDeinit(void);
BUF_STATUS VrgQueueGet(MEMCIRCBUFDRV queue, void *item, unsigned int size);
BUF_STATUS VrgQueuePost(MEMCIRCBUFDRV queue, void* item, unsigned int size);

extern void CallProgressToneDetect(VRG_ENDPT *pState, HSZEVT *evtp);
extern void ClassEvent(VRG_ENDPT* state, VRG_HAPI_EVT* evtp);
extern void ClearConnectionOptions(CNXSTATE *cnx);
extern EPSTATUS ConnectPstnGwSB(int gwVhdhdl, int pstnLineVhdhdl);
extern EPSTATUS ConnectSB(int line1, int line2);
extern VRG_BOOL ConnectionOptionsChanged(CNXSTATE *cnx, EPZCNXPARAM *newParm);
extern EPSTATUS DisConnectPstnGwSB(int gwVhdhdl, int pstnLineVhdhdl);
extern EPSTATUS DisconnectSB(int line1, int line2);
extern void vrgEndptPrintRtpStats ( EPZCNXSTATS *epRtpStats );
extern EPSTATUS enableFskClidRx(VRG_BOOL enable, int pstnVhdl);
extern void EcanProcessStm(VRG_ENDPT *endptState, ECANEVT evt);
extern void EcanStateSet(VRG_ENDPT *endptState, EPEECANSTATE ecanState);
extern int EnableRelay(CNXSTATE *cnx, VRG_BOOL enable, VRG_UINT16 rate, EPDATAMODE dataMode, EPDTMFRFC2833CONFIG digitRelayType);
extern EPSTATUS EndptUpdateCasState( int lineId, EPCASSTATE casState );
extern void GenerateNteEvent(CNXSTATE *cnx, int event);
extern CNXSTATE* GetCnxState(int cnxId);
extern CNXSTATE* GetCnxStateFromVHD(int vhdhdl);
extern VRG_ENDPT* GetPstnVrgEndptState( void );
extern EPEVT GetDigitEvent(HAPI_PTE_DTMFDDGT pteEvt);
extern VRG_ENDPT* GetEndptState( int endptId );
extern VRG_ENDPT* GetEndptStateFromPhysId( VRG_UINT16 physId );
extern VRG_UINT16 GetEndptIndexFromCasHandle(CAS_CTL_HANDLE casHandle, VRG_UINT16 *endptIndex);
extern VRG_ENDPT* GetEndptStateFromVHD(VRG_UINT16 vhdHdl);
extern CNXSTATE* GetNewSecondaryCnxState(int dspNum);
extern void GetRingPattern(EPSIG ringSignal, VRG_COUNTRY country, CLASSSTM_RING_STATE *pattern);
extern void InitRtpServiceRegisters(CNXSTATE *cnx);
extern EPSTATUS InitVhds(VRG_ENDPT* state);
extern VRG_BOOL IsCnxModeSupported(EPCNXMODE mode, int dspNum);
extern VRG_BOOL IsCnxVhd(VRG_UINT16 vhdHdl);
extern VRG_BOOL IsT38Negotiated(CNXSTATE  *cnx);
extern int MapRtpPayloadType(VRG_UINT16 codec, VRG_UINT8 payloadType, HAPI_UINT16* mediaPayloadTypeMap);
extern VRG_BOOL ModeCheck(EPCNXMODE mode, CNX_DIRECTION direction);
extern void PrintVhdInfo(void);
extern EPSTATUS Provision(ENDPT_STATE *endptState, EPPROV provItemId, void *provItemValue);
extern EPSTATUS ProvisionRingParams(VRG_ENDPT *state, EPRINGPARAM *ringParam);
extern void RtpInit(RTPSTATE* state);
extern void SaveConnectionOptions(CNXSTATE *cnx, EPZCNXPARAM *cnxParam);
extern EPSTATUS SetConnectionOptions(CNXSTATE *cnx, EPZCNXPARAM *cnxParam, VRG_BOOL bInitialChange);
extern EPSTATUS SetRingParams(VRG_ENDPT *state);
extern void SetRtpMode(CNXSTATE *cnx, EPCNXMODE mode);
extern void SetRtpCSCallDisable ( CNXSTATE *cnx );
extern void SetRtpCSCallEnable  ( CNXSTATE *cnx );
extern void SetRtpPayloadTypes(CNXSTATE *cnx, CODECCNXPARMLIST *codecList);
extern void SetRtpServiceRegisters(CNXSTATE *cnx, EPRTCPXRMODE XRmode, EPZVBDPARAM *vbdParam, VRG_UINT16 secHdrSize);
extern EPSTATUS GetConnectionRtpStats(CNXSTATE *cnx, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset);
extern EPSTATUS SetT38EcMode(CNXSTATE *cnx, EPT38ECMODE ecMode);
extern EPSTATUS SetT38PktSize( CNXSTATE *cnx, CODEC_PKT_PERIOD ptime );
extern void SetVoiceMode(CNXSTATE *cnx, VOICEMODE mode);
extern void SetupSB(VRG_UINT16 vhdhdl, int inMask, int inMaskDiff, int outMask, int outMaskDiff);
extern void UpdateSBMasks(CNXSTATE *cnx, EPCNXMODE mode);
extern void printConnectionParams(EPZCNXPARAM* cnxParam);
extern void resetCodecCap(VRG_UINT8 codecCap[]);
#if DPR_ENABLED
#include <hapi_dpr.h>
extern void startDpr( HAPI_UINT16 handle, HAPI_UINT16 direction );
extern void stopDpr( void );
extern void startDprIngress( HAPI_UINT16 handle );
extern void startDprEgress( HAPI_UINT16 handle );
#endif /* DPR_ENABLED */
extern void translateRingProfile(VRG_RING_PROFILE *pVrgProfile, CLASSSTM_RING_STATE *pCasProfile);
extern void requestTpd(VRG_ENDPT* state, EPSIG signal, unsigned int value);
extern void requestLoopback(VRG_ENDPT* state, unsigned int value);
extern void applyIngressTone(VRG_ENDPT* state, int cnxIdx, VRG_COUNTRY country, const EPZTONEMAP* toneMapEntry);
extern void applyEgressTone(VRG_ENDPT* state, VRG_COUNTRY country, const EPZTONEMAP* toneMapEntry, int duration);
extern void removeIngressTone(VRG_ENDPT* state, int cnxIdx);
extern void removeEgressTone(VRG_ENDPT* state, EPSIG signal);
extern void SetDigitalMode( VRG_ENDPT* state, CNXSTATE* cnx, VRG_BOOL value );
extern const EPZTONEMAP* getEpzToneMapEntry(EPSIG signal);

#if BRCM_TRACK_IDLE_STATUS
void idleStatusWatchDogInit(void);
void idleStatusWatchDogDeinit(void);
#endif /* BRCM_TRACK_IDLE_STATUS */

extern void vrgEndptResetBuildOut( int lineId, int vhdhdl );
extern VRG_UINT16 vrgEndptRetrievePlcSettings( CODEC_TYPE codec );
extern EPSTATUS vrgEndptGetRtpStats( ENDPT_STATE *endptState, int cnxId, EPZCNXSTATS *epRtpStats, VRG_BOOL bReset );
extern XDRV_DEBUG* vrgEndptGetDebugDriver( void );
extern EPSTATUS vrgEndptSignal( ENDPT_STATE   *endptState, int cnxId, EPSIG signal, 
                                unsigned int value, int duration, int period, int repetition );

extern void VrgEventProcess( VRG_EVT *vrgEvt );
extern void VrgEptCmdProcess( VRG_EPT_CMD *vrgEptCmd );

#endif /* VRG_ENDPT_PRIVATE_H */
