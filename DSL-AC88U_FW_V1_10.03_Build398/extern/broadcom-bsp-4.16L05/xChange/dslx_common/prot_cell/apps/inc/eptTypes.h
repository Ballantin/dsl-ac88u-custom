/****************************************************************************
*  
*  Copyright (c) 2000-2013 Broadcom
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
*    Filename: eptTypes.h
*
****************************************************************************
*    Description:
*
*      Endpoint type definitions
*
****************************************************************************/

#ifndef EPT_TYPES_H
#define EPT_TYPES_H

#if defined(__cplusplus)
extern "C"
{
#endif

/* ---- Include Files ---------------------------------------------------- */
#include <bosTypes.h>
#include <codec.h>

/* ---- Constants and Types ---------------------------------------------- */

/*
** Generic macros for making line manager access IDs
*/
#define CNX_ID_UNKNOWN        0xFFFF
#define LINE_ID_UNKNOWN       0xFFFF

#define EPT_MKT_ID( ulineId, uCnxId )     ( ((ulineId) << 16) | ( uCnxId ) )
#define EPT_GET_LINEID( uIndex )          ( (uIndex) >> 16 )
#define EPT_GET_CNXID( uIndex )           ( (uIndex) & 0xFFFF )

/*
** Endpoint status return types
*/
typedef enum EPTSTATUS
{
   eEPTSTATUS_SUCCESS = 0,        /* success */
   eEPTSTATUS_EPT_UNKNOWN,        /* unknown endpoint */
   eEPTSTATUS_CNX_UNKNOWN,        /* unknown cnx */
   eEPTSTATUS_SIGNAL_UNKNOWN,     /* unknown signal */
   eEPTSTATUS_BAD_MODE,           /* endpoint is in wrong mode, cannot perform 
                                  ** operation */
   eEPTSTATUS_BAD_COMMAND,        /* unknown command */
   eEPTSTATUS_NO_RESOURCES,       /* insufficient resources */
   eEPTSTATUS_BUSY,               /* endpoint is busy */
   eEPTSTATUS_MEDIA_TYPE_UNKNOWN, /* unrecognized media type */
   eEPTSTATUS_SHUTDOWN_ERR,       /* unable to shutdown the endpoint */
   eEPTSTATUS_RESET_PENDING,      /* pending reset */

   eEPTSTATUS_ERROR               /* generic error return */

} EPTSTATUS;


/*
** Endpt initialization structure
*/
typedef struct EPTINITCFG
{
   /* Current power source */
   BOS_BOOL       bOnBatteryPower;

} EPTINITCFG;



/*
** Events detected
*/
typedef enum
{
   eEPTEVT_NULL,                    /* internal use: Null event */
   eEPTEVT_DTMF0,                   /* DTMF Tone 0 */
   eEPTEVT_DTMF1,                   /* DTMF Tone 1 */
   eEPTEVT_DTMF2,                   /* DTMF Tone 2 */
   eEPTEVT_DTMF3,                   /* DTMF Tone 3 */
   eEPTEVT_DTMF4,                   /* DTMF Tone 4 */
   eEPTEVT_DTMF5,                   /* DTMF Tone 5 */
   eEPTEVT_DTMF6,                   /* DTMF Tone 6 */
   eEPTEVT_DTMF7,                   /* DTMF Tone 7 */
   eEPTEVT_DTMF8,                   /* DTMF Tone 8 */
   eEPTEVT_DTMF9,                   /* DTMF Tone 9 */
   eEPTEVT_DTMFS,                   /* DTMF Tone * */
   eEPTEVT_DTMFH,                   /* DTMF Tone # */
   eEPTEVT_DTMFA,                   /* DTMF Tone A */
   eEPTEVT_DTMFB,                   /* DTMF Tone B */
   eEPTEVT_DTMFC,                   /* DTMF Tone C */
   eEPTEVT_DTMFD,                   /* DTMF Tone D */
   eEPTEVT_OFFHOOK,                 /* Off-hook event */
   eEPTEVT_ONHOOK,                  /* On-hook event */
   eEPTEVT_FLASH,                   /* Hook-flash event */
   eEPTEVT_FAX,                     /* Fax tone detected */
   eEPTEVT_MODEM,                   /* Modem detect event */
   eEPTEVT_LOWENERGY,               /* Low energy detect event */
   eEPTEVT_DTMFL,                   /* DTMF long duration */
   eEPTEVT_MEDIA_START,             /* Media start */
   eEPTEVT_MEDIA_END,               /* Media end */
   eEPTEVT_TDD,                     /* Telecom Devices for the Deaf */
   eEPTEVT_T38_STOP,                /* T.38 completion */
   eEPTEVT_T38_FAILURE,             /* T.38 failure */
   eEPTEVT_RINGING_ON,              /* Phones are currently ringing */
   eEPTEVT_RINGING_OFF,             /* Phones are currently not ringing  */
   eEPTEVT_VBD_STOP,                /* VBD stop event */
   eEPTEVT_VBD_START,               /* VBD start event */
   eEPTEVT_CHATTER_LINE_BAD_DIGIT,           /* Chattering line - bad digit event */
   eEPTEVT_CHATTER_LINE_EXCESSIVE_DIALING,   /* Chattering line - excessive dialing event */
   eEPTEVT_CHATTER_LINE_PULSE_EVENT,         /* Chattering line - indicates a pulse event is comming */  
   eEPTEVT_ENCODER_SWITCH,          /* Encoder switch by DSP */
   eEPTEVT_DECODER_SWITCH,          /* Decoder switch by DSP */
   eEPTEVT_SES_STATUS,
   eEPTEVT_V18,                     /* V18 tone detected */
   eEPTEVT_INGRESS_DTMF,            /* Ingress DTMF generated */
   eEPTEVT_NO_ING_CNX_ACTIVITY,     /* No ingress activity threshold reached */
   eEPTEVT_NO_EG_CNX_ACTIVITY,      /* No egress activity threshold reached */
   eEPTEVT_LAST

} EPTEVT;


/*
** Accessible information
*/
typedef enum
{
   eEPTINFOTYPE_RTPSTATS,
   eEPTINFOTYPE_RTPVOIPMETRICS,
   eEPTINFOTYPE_T38STATS,
   eEPTINFOTYPE_OVERCURRENTSTATUS,
   eEPTINFOTYPE_LAST

} EPTINFOTYPE;


/* T.38 Error Correction Scheme */
typedef enum
{
   eEPTT38ECMODE_NONE,             /* No error correction */
   eEPTT38ECMODE_UDPREDUNDANCY,    /* UDP Redundancy */
   /* eEPT_T38EC_UDPFEC, ******* UDP Forward Error Correction - NOT SUPPORTED *****/

} EPTT38ECMODE;

/* T.38 Connection Parameters */
typedef struct
{
   EPTT38ECMODE    eEcMode;              /* Error correction mode associated with the connection */
   BOS_UINT32      uMaxRemDgrm;          /* Maximum datagram size the remote end can handle      */
   BOS_UINT32      uMaxBitRate;          /* Maximum bit rate for the fax call                    */
   BOS_UINT16      uKeepAliveInterval;   /* Interval at which keepAlive T.30 no signal indicator is sent during T.38 mode*/

} EPTT38PARM;

/*
** T.38 connection specific statistics structure 
*/
typedef struct
{
   BOS_UINT32     uTxpg;             /* pages sent */
   BOS_UINT32     uRxpg;             /* pages received */

   BOS_UINT32     uPktSent;          /* T.38 packets sent */
   BOS_UINT32     uOctetSent;        /* T.38 octets sent */
   BOS_UINT32     uPktRecvd;         /* T.38 packets received */
   BOS_UINT32     uOctetRecvd;       /* T.38 octets received */

} EPTT38STATS;

#define INGRESS_CODEC_LIST_SIZE   400
#define EGRESS_CODEC_LIST_SIZE    400

/*
** RTP connection specific statistics structure
*/
typedef struct       
{
   /* connection statistics */      
   BOS_UINT32   uPktSent;        /* packets sent */
   BOS_UINT32   uOctetSent;      /* octets sent */
   BOS_UINT32   uPktRecvd;       /* packets received */
   BOS_UINT32   uOctetRecvd;     /* octets received */
   BOS_SINT32   sPktLost;        /* packet lost */
   BOS_UINT16   uJitter;         /* interarrival jitter estimate (in ms) */
   BOS_UINT16   uLatency;        /* avg tx delay (in ms) */

   BOS_UINT32   uRemPktSent;
   BOS_UINT32   uRemOctetSent;
   BOS_SINT32   sRemPktLost;
   BOS_UINT32   uRemJitter;

   BOS_UINT32   uSilencePktSent;    /* silence packets sent */
   BOS_UINT32   uSilenceOctetSent;  /* silence octets sent */

   BOS_UINT32   uSilencePktRecvd;    /* silence packets received */
   BOS_UINT32   uSilenceOctetRecvd;  /* silence octets received */

   BOS_UINT16   uBurstLost;          /* maximum lost packets in a burst */
   BOS_UINT16   uLostSequence;       /* number of lost sequence */
   
   BOS_UINT8    uIngressCodecList[INGRESS_CODEC_LIST_SIZE]; /* Ingress Codec List */
   BOS_UINT8    uEgressCodecList[EGRESS_CODEC_LIST_SIZE];  /* Egress Codec List */

} EPTRTPSTATS;


/* RTCP XR Mode */
typedef enum
{
   eEPTRTCP_XRMODE_UNSET,
   eEPTRTCP_XRMODE_ON,          /* Enable XR Collecting & Reporting */
   eEPTRTCP_XRMODE_OFF,         /* Disable XR Collecting & Reporting */
   eEPTRTCP_XRMODE_NEGOTIATE,   /* Enable XR Collecting & Disable XR Reporting */

} EPTRTCP_XRMODE;

/* RTCP Connection Parameters */
typedef struct EPTRTCPPARM
{
   BOS_BOOL             bEnableRTCP;        /* set if RTCP is supported */
   EPTRTCP_XRMODE       eXRMode;            /* rtcp XR mode -- on, off or negotiate */
   BOS_BOOL             bRemXrVoipMetrics;  /* rtcp XR mode from remote -- voip metrics: on, off */

} EPTRTCPPARM;

/*
** Connection specific statistics structure (RTCP-XR VoipMetrics)
*/
typedef struct
{
   BOS_UINT8  uNetPktLossRate;     /* Network Packet Loss Rate */
   BOS_UINT8  uJbDiscardRate;      /* Jitter Buffer Discard Rate */
   BOS_UINT8  uBurstLossDensity;   /* Burst Loss Density */
   BOS_UINT8  uGapLossDensity;     /* Gap Loss Density */
   BOS_UINT16 uBurstDuration;      /* Burst Duration */
   BOS_UINT16 uGapDuration;        /* Gap Duration */
   BOS_UINT16 uRdTripNetDelay;     /* Round Trip Network Delay */
   BOS_UINT16 uEndSystemDelay;     /* End System Delay */
   BOS_UINT8  uMinGapThreshold;    /* Minimum Gap Threshold */
   BOS_SINT8  sSignalLevel;        /* Signal Level */
   BOS_SINT8  sNoiseLevel;         /* Noise Level */
   BOS_UINT8  uResidualEchoRetLoss;/* Residual Echo Return Loss */
   BOS_UINT8  uRFactor;            /* R Factor */
   BOS_UINT8  uExtRFactor;         /* External R Factor */
   BOS_UINT8  uEstMOSLQ;           /* Estimated MOS-LQ */
   BOS_UINT8  uEstMOSCQ;           /* Estimated MOS-CQ */
   BOS_UINT8  uPktLossConcealmentType;  /* Packet Loss Concealment Type */
   BOS_UINT8  uJbAdaptive;              /* Jitter Buffer Adaptive */
   BOS_UINT8  uJbRate;                  /* Jitter Buffer Rate */
   BOS_UINT8  uReserved;                /* reserved */ 
   BOS_UINT16 uJbNominal;               /* Nominal Jitter Buffer Delay */
   BOS_UINT16 uJbMax;                   /* Maximum Jitter Buffer Delay */
   BOS_UINT16 uJbAbsMax;                /* Absolute Jitter Buffer Delay */
   BOS_UINT16 uInterArrivalJitter;      /* Inter-Arrival Jitter */

} EPTVOIPMETRICS;

/*
** RTP VoipMetrics stats 
*/
typedef struct
{
   BOS_BOOL        bLocalCollected;   /* Local RTCP XR VoipMetrics collected */
   BOS_BOOL        bRemoteCollected;  /* Remote RTCP XR VoipMetrics collected */
   EPTVOIPMETRICS localVoipMetrics;  /* Local RTCP XR VoipMetrics */
   EPTVOIPMETRICS remoteVoipMetrics; /* Remote RTCP XR VoipMetrics */

} EPTVOIPMETRICSSTATS;

/*
** Endpoint commands
** NOTE: this is different from console commands.  Both sets of commands should
** eventually be merged together
*/
typedef enum EPTCMD
{
   eEPTCMD_ANALOG_LOOPBACK,           /* Enable analog loopback */
   eEPTCMD_LINE_RESET,                /* Line reset */
   eEPTCMD_DIAGTESTS_PROV_START,      /* Start diag tests with provisioning */
   eEPTCMD_DIAGTESTS_NOPROV_START,    /* Start diag tests without provisioning */
   eEPTCMD_DIAGTESTS_PC15_START,      /* Start diag tests with PC15 provisioning */
   eEPTCMD_DIAGTESTS_CALIBRATE_START, /* Start calibartion process */
   eEPTCMD_UPDATE_DTMFPLAYOUT,        /* Update DTMF Playout duration */
   eEPTCMD_TXGAIN,                    /* Change Tx Gain setting */
   eEPTCMD_RXGAIN,                    /* Change Rx Gain setting */
   eEPTCMD_BOOSTED_LOOP_CURRENT,      /* change Boosted Loop Current setting */
   eEPTCMD_ADMIN_STATUS,              /* line admin status change */
   eEPTCMD_RING_WAVEFORM,             /* Change Ring Waveform settings */

   eEPTCMD_LAST

} EPTCMD;


/*
** Endpoint console commands
*/
typedef enum EPTCONSOLE
{
   eEPTCONSOLE_PRINT_VHDS=0xff,        /* Display VHD list. */
   eEPTCONSOLE_PVE_STATE_FROM_CNX,     /* Display DSP VHD PVE state based on cnx ID */
   eEPTCONSOLE_VHD_STATS,              /* Display DSP VHD stats */
   eEPTCONSOLE_VHD_STATS_FROM_CNX,     /* Display DSP VHD stats based on cnx ID */
   eEPTCONSOLE_VHD_STATS_TIMER,        /* Set DSP VHD stats interval based on cnx ID */
   eEPTCONSOLE_VHD_JBREGS,             /* Display current jitter buffer registers */
   eEPTCONSOLE_FAXR_STATS,             /* Display DSP FAXR stats */
   eEPTCONSOLE_FAXR_STATS_FROM_ENDPT,  /* Display DSP FAXR stats based on endpt number */
   eEPTCONSOLE_IPC_STATS,              /* Display DSP IPC stats */
   eEPTCONSOLE_IPC_STATS_FROM_ENDPT,   /* Display DSP IPC stats based on endpt number */
   eEPTCONSOLE_DSP_BUFFERS,            /* Display DSP buffer pool stats */
   eEPTCONSOLE_DSP_GETCNT,             /* Display DSP debug counters */
   eEPTCONSOLE_DSP_FORCEASSERT,        /* Force DSP assert */
   eEPTCONSOLE_DSP_CHECKASSERT,        /* Check DSP assert stack pointer */
   eEPTCONSOLE_DSP_GETREGS,            /* Get a snapshot of DSP regs */
   eEPTCONSOLE_DSP_READDMEM,           /* Read internal DSP data memory */
   eEPTCONSOLE_APM_STATUS,             /* Display APM status info */
   eEPTCONSOLE_PWRMGMT_VPM,            /* Set the VPM power mode */
   eEPTCONSOLE_PWRMGMT_AFE,            /* Power up/down the Analog Front End */
   eEPTCONSOLE_TR57_ENABLE,            /* Enable/Disable TR57 test mode */
   eEPTCONSOLE_VHD_LOOPBACK_ENABLE,    /* Enable VHD loopback */
   eEPTCONSOLE_READ_HOOK_STATE,        /* Read hook state on the channel */
   eEPTCONSOLE_SET_SLIC_STATE,         /* Set slic state on the channel */
   eEPTCONSOLE_PRINT_DSP_CAPABILITIES, /* Display DSP capabilities. */
   eEPTCONSOLE_DSP_STACK_SHOW,         /* Display DSP stack usage statistics. */
   eEPTCONSOLE_HAPINET_CONFIG_SETREG,  /* Set configuration registers of the NET Service */
   eEPTCONSOLE_DSP_DPR_START,          /* Start DSP idle profiler. */
   eEPTCONSOLE_CONFIGURE_NETWTEST,     /* Configure switchboard connections for network test. */
   eEPTCONSOLE_GET_ECAN_STATE,         /* Display Ecan stats */
   eEPTCONSOLE_SET_ECAN_STATE,         /* Force ECAN to a certain state (on, off, NLP off) */
   eEPTCONSOLE_NULL                    /* internal use: NULL cmd */

} EPTCONSOLE;

/*
** Console command parameters
*/
typedef struct
{
   int arg1;
   int arg2;
   int arg3;

} EPCONSOLE_PARMS;

/*
** Codec definitions
*/
typedef struct
{
   BOS_BOOL       bSupportedFamily;    /* bit set if the codec family is supported */
   BOS_UINT16     uSupportedOptions;   /* bit definitions of codec variants/rates supported */
   
} EPTCODECCAP;

/*
** Connection modes
*/
typedef enum
{
   eEPTCNXMODE_SNDO,                /* Send only */
   eEPTCNXMODE_RCVO,                /* Receive only */
   eEPTCNXMODE_SNDRX,               /* Send-Receive mode */
   eEPTCNXMODE_CONF,                /* Send-Receive Conference mode */
   eEPTCNXMODE_INACT,               /* Inactive */
   eEPTCNXMODE_LOOP,                /* Network Loopback test */
   eEPTCNXMODE_TEST,                /* Network Continuity test */
   eEPTCNXMODE_REPLCATE,            /* Replicate */
   eEPTCNXMODE_MAX

} EPTCNXMODE;


/*
** Endpoint capabilities
*/
typedef struct EPTCAP   /* Endpoint Capabilities */
{
   /* Packetazation period (msec). MS-Bit contains "1" if a range is defined */
   BOS_UINT16  pCap[2];

   /* Array value representing supported codec types */
   EPTCODECCAP codecCap[CODEC_MAX_TYPES];

   /* Bit value representing supported named telephone events */
   BOS_UINT32  uNteCap;

   /* Echo cancelation (0 if NOT supported) */
   BOS_UINT16  uEcanCap;

   /* Silence Suppression parameter (0 if NOT supported) */
   BOS_UINT16  uSilenceCap;

   /* Connection Modes.  Set if the connection mode is supported */
   int         sModeCap[eEPTCNXMODE_MAX];

   /* Bit value representing supported voice band data capabilities. */
   BOS_UINT8   uVbdCap;

   /* Maximum number of redundancy level(s) supported per RFC 2198. */
   BOS_UINT8   uRedLvl;

} EPTCAP;

/*
** DTMF tone on/off
*/
typedef enum
{
   eEPTDTMF_TONEON = 0,
   eEPTDTMF_TONEOFF,
   eEPTDTMF_NOTONE,   /* Indicates pulse dialing */
} EPTDTMF;

/*
** Hook Flash mode
*/
typedef enum
{
   eEPTHOOKFLASH_IGNORED, 
   eEPTHOOKFLASH_PROCESSED,
   eEPTHOOKFLASH_REPORTED, 
   eEPTHOOKFLASH_RFC2833RELAY

} EPTHOOKFLASH_MODE;

/* 
** MWI Signaling Protocols
*/
typedef enum
{
   eEPTMWISIGPROT_FSK = 3, 
   eEPTMWISIGPROT_DTMF

} EPTMWISIGPROT;

/* Data Mode Types */
typedef enum
{
   eEPTDATAMODE_NONE,          /* Disable CDIS */
   eEPTDATAMODE_VBD,           /* VBD (no FAXR or auto encoder switch) - default mode */
   eEPTDATAMODE_VBD_AUTOENC,   /* VBD with automatic encoder switch to pcmu */
   eEPTDATAMODE_T38,           /* Use T38 fax relay (no auto encoder switch) */
   eEPTDATAMODE_T38_AUTOENC,   /* Use T38 fax relay with automatic encoder switch */
   eEPTDATAMODE_T38_MUTE       /* Use T38 fax relay (but mute until instructed to switch to T.38) */

} EPTDATAMODE;

/* VBD (V.152) Redundancy support */
typedef enum
{
   eEPTVBDRED_OFF,             /* Enable redundancy for VBD */
   eEPTVBDRED_ON,              /* Disable redundancy for VBD */

} EPTVBDREDMODE;

/* VBD mode */
typedef enum
{
   eEPTVBDMODE_NONE   = (0),
   eEPTVBDMODE_LEGACY = (1 << 0),         /* Legacy VBD mode */
   eEPTVBDMODE_V152   = (1 << 1),         /* V.152 VBD mode */
   eEPTVBDMODE_MAX    = 2

} EPTVBDMODE;

/* VBD parameters */
typedef struct
{
   EPTVBDMODE    vbdMode;
   EPTVBDREDMODE vbdRedMode;

} EPTVBDPARM;

/* DTMF RFC 2833 mode */
typedef enum
{
   eEPTDTMFRFC2833_ENABLED,
   eEPTDTMFRFC2833_DISABLED,
   eEPTDTMFRFC2833_SUBTRACT,

} EPTDTMFRFC2833CONFIG;

/* Preferred VBD codec */
typedef enum
{
   eEPTVBDCODEC_NONE, 
   eEPTVBDCODEC_PCMU,
   eEPTVBDCODEC_PCMA

} EPTPREFERREDVBDCODEC;

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
   EPTCNXMODE        eMode;             /* connection mode */
   CODECCNXPARMLIST  cnxParmList;      /* list (in ordered preference) of negotiated
                                        * codecs/packetization period between the local
                                        * and remote endpoint */
   BOS_UINT32        uNamedPhoneEvts;   /* bit-field of negotiated named telephone events */
   BOS_UINT16        uEchocancel;       /* echo cancellation */
   BOS_UINT16        uSilence;          /* silence suppression */
   BOS_UINT16        uComfortNoise;     /* comfort noise */
   BOS_BOOL          bPreserveFaxMode;  /* preserve a fax mode (if we are in already) */
   BOS_UINT16        uSecHdrSize;       /* Authentication mode -- security header size */
   EPTDATAMODE       eDataMode;         /* fax relay, vbd, or none */
   EPTT38PARM        t38param;         /* T.38 fax parameters */
   EPTRTCPPARM       rtcpparam;        /* RTCP parameters */
   EPTVBDPARM        vbdparm;          /* VBD parameters */

} EPTCNXPARM;

/*
** Reason code associated with events reported from the endpoint module
** to the upper layer application.  It is up to the application receiving
** the event to use the reason code for event processing.
*/
typedef enum
{
   eEPTEVTREASON_PTSW,         /* Payload type switch. */
   eEPTEVTREASON_CNG,          /* CNG signal. */
   eEPTEVTREASON_V21flag,      /* V.21 flag. */
   eEPTEVTREASON_V21tone,      /* V.21 tone. */
   eEPTEVTREASON_BAUDOT,       /* V.18 Baudot tone and character. */
   eEPTEVTREASON_BELLTONE,     /* Bell 103 carrier. */
   eEPTEVTREASON_ANS,          /* V.25 ANS. */
   eEPTEVTREASON_prANS,        /* V.25 ANS with phase reversal. */
   eEPTEVTREASON_DCN,          /* DCN signal. */
   eEPTEVTREASON_HDLC,         /* HDLC signal. */

} EPTEVTREASON;



/* Media Types */
typedef enum
{
   eEPTMEDIATYPE_MEDIA,        /* media packet: t38 or rtp */
   eEPTMEDIATYPE_RTP,          /* rtp packet */
   eEPTMEDIATYPE_RTCP,         /* rtcp packet */
   eEPTMEDIATYPE_T38           /* t38 packet */

} EPTMEDIATYPE;

/* Endpt Packet Type */
typedef struct
{
   EPTMEDIATYPE   eMediaType;    /* Type of packet (fax/non-fax) */
   void          *pPacket;       /* Pointer to the packet */

} EPTPACKET;

/*
** Endpoint test modes
*/
typedef enum EPTTESTMODE
{
   eEPTTESTMODE_NONE = 0,
   eEPTTESTMODE_TR57,
   eEPTTESTMODE_ANALOGLOOPBACK,
   eEPTTESTMODE_PACKETLOOPBACK,
   eEPTTESTMODE_LINEDIAG

} EPTTESTMODE;


/*
** Codec table configurations
*/

/*
** Maximum number of entries in the codec table array.  This
** number should really be tied to the number of supported cnxs. */
#define EPT_CODEC_TABLE_MAX_ROW_ENTRY      4

/*
** Definition of a single codec entry in the codec table array
*/
typedef struct EPTCODECTABLE_ROW_ENTRY
{
   CODEC_TYPE  codec;      /* codec type */
   BOS_UINT16  uMaxCodec;  /* maximum number of simultaneous operation
                           ** of the assiocated codec type */

} EPTCODECTABLE_ROW_ENTRY;

/*
** Definition of a single row of the codec table
**
** Each row is composed of 2 components:
**    1. an array of codec entries
**    2. number of elements in the array that is being used
*/
typedef struct EPTCODECTABLE_ROW
{
   /* Array of codec entries */
   EPTCODECTABLE_ROW_ENTRY    codecTableEntry[EPT_CODEC_TABLE_MAX_ROW_ENTRY];

   /* Number of entries used in the array */
   BOS_UINT16                 uCodecTableNumEntry;


} EPTCODECTABLE_ROW;


/* Maximum size of the output buffer for the EPCMD_VHD_STATS_FROM_CNX endpt
 * command. */
#define MAX_EPCMD_VHD_STATS_FROM_CNX_BUFFER_SIZE 750


#if defined(__cplusplus)
}
#endif

#endif  /* EPT_TYPES_H  */


