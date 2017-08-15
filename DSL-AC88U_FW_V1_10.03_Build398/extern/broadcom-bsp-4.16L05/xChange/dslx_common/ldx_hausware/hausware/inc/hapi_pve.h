/****************************************************************************
*
*  Copyright (c) 2002-2008 Broadcom
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
*
*  Filename: hapi_pve.h
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
#ifndef HAPI__PVE_H
#define HAPI__PVE_H

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif
#if !defined( HAPIGDEF_H )
#  include <hapigdef.h>
#endif
#if !defined( HAPI_OPCODE_H )
#  include <hapi_opcode.h>
#endif
#if !defined( PAYLOAD_H )
#  include <payload.h>
#endif
#if !defined( HAPI__CODEC_H )
#  include <hapi_codec.h>
#endif
#if !defined( HAPIIDS_H )
#  include <hapiids.h>
#endif
#if !defined( HAPI_PKT_H )
#  include <hapi_pkt.h>
#endif
#if !defined( _HAPI_WRAP_DEFS_H )
#  include <hapi_wrap_defs.h>
#endif
#if !defined( HAPI_GVAD_DEFS_H )
#  include <hapi_gvad_defs.h>
#endif
#define HAPIPVEPKTSRV         HAPI_PVE_SRV

/*************************************************************************
**          PVE SERVICE COMMANDS
****************************************************************************/

#define HAPI_PVE_PKT_ENABLE_CMD  (HSF_MKHDR(HAPIPVEPKTSRV, HAPI_OPC_ENABLE_CMD))

#define HAPI_PVE_CLRPAYLOAD_CMD  (HSF_MKCMD_STD(HAPIPVEPKTSRV, 1)) /* clear PVE host payload buffer */
      /* op1 : index to payload buffer */
      /* op2 : timestamp of packet */

#define HAPI_PVE_PAYLOADFLUSH_CMD (HSF_MKCMD_STD(HAPIPVEPKTSRV, 2)) /* flush host payload buffer */


/* set/change the vocoder rate */
#define HAPI_PVE_PKT_SETVOICERATE_CMD   (HSF_MKCMD_STD(HAPIPVEPKTSRV, 3))
      /* OP1 : bits 0:7 payloadCode, bits 8:15 SID */
      /* OP2 : superpacketization peroid */

#define HAPI_PVE_PKT_ENCSTART_CMD   (HSF_MKCMD_STD(HAPIPVEPKTSRV, 4))
      /* op1 : reserved */
      /* op2 : reserved */

#define HAPI_PVE_PKT_OPTION_CMD  (HSF_MKCMD_STD(HAPIPVEPKTSRV, 5))
      /* op1 : PVE voice register option */
      /* op2 : reserved */

#define HAPI_PVE_PKT_VOCODER_SUPPORT_CMD  (HSF_MKCMD_EXT(HAPIPVEPKTSRV, 6))
      /* op1 : length of array (HAPI_PKT_VC_ALG_MAX) */
      /* op2 : pointer to array, unsupported vocoder will have entry equal to 0 */

#define HAPI_PVE_CLRPAYLOAD_GRP_CMD  (HSF_MKCMD_EXT(HAPIPVEPKTSRV, 7)) /* clear a group of PVE host payload buffer */
      /* op1 : length of buffer (size of buffer = number of payload buffer / 16) */
      /* op2 : pointer to buffer indicating which payload buffer needs to be freed */

/* support of old commands */

#define HAPI_PVE_GETSTATS_CMD       HAPI_CODEC_GETSTATS_CMD
      /* OP1: sizeof(STATS)      */
      /* OP2: * to regs          */

#define HAPI_PVE_ENCSTART_CMD       HAPI_CODEC_ENCSTART_CMD
      /* OP1: 1=START, 0=STOP */
      /* OP2: reserved = 0        */

#define HAPI_PVE_DECFLUSH_CMD       HAPI_CODEC_DECFLUSH_CMD
      /* OP1: reserved = 0        */
      /* OP2: reserved = 0        */

#define HAPI_PVE_FREEZE_CMD         HAPI_CODEC_FREEZE_CMD
      /* OP1: 1=FREEZE, 0=UNFREEZE  */
      /* OP2: 1=VOICEBANDDATA, 0=VOICE/SID    */

#define HAPI_PVE_ENABLE_CMD         HAPI_CODEC_ENABLE_CMD
      /* OP1: 1=ENABLE, 0=DISABLE */
      /* OP2: reserved = 0        */

#define HAPI_PVE_SETVOICERATE_CMD   HAPI_CODEC_SETVOICERATE_CMD
      /* OP1: vocoder rate (refer to hapi_pkt.h) */
      /* OP2: superpacketization interval in number of 8kHz samples(40, 80,160,240)  */

#define HAPI_PVE_SETREGS_CMD        HAPI_CODEC_SETREGS_CMD
      /* OP1: sizeof (HSZVOICEREGS) in bytes          */
      /* OP2: pointer to HSZVOICEREGS structure       */

#define HAPI_PVE_SETREGS1_CMD       HAPI_CODEC_SETREG1_CMD


/* Mapping new to the old defines to reduce code changes */


#define HAPI_PVE_ENABLE       HAPI_PVE_ENABLE_CMD
#define HAPI_PVE_GETSTATS     HAPI_PVE_GETSTATS_CMD
#define HAPI_PVE_ENCSTART     HAPI_PVE_ENCSTART_CMD
#define HAPI_PVE_DECFLUSH     HAPI_PVE_DECFLUSH_CMD
#define HAPI_PVE_FREEZE       HAPI_PVE_FREEZE_CMD
#define HAPI_PVE_SETVOICERATE HAPI_PVE_SETVOICERATE_CMD
#define HAPI_PVE_SETREGS      HAPI_PVE_SETREGS_CMD
#define HAPI_PVE_SETREGS1     HAPI_PVE_SETREGS1_CMD

/*************************************************************************
**          PVE SERVICE ASYNCHRONOUS EVENTS
*************************************************************************/
/* None - all defined in hapi_net.h */

#define HAPI_PVE_EXCEPTION   (HSF_MKEVT_STD(HAPIPVEPKTSRV, 0))   /* EXCEPTION event from the PVE service */
      /* OP1: type of problem, defined in PVE_EXCEPTION */
      /* OP2: reserved = 0    */

typedef enum
{
   PVEEXCEPTION_UNKNOWN = 0x0000,
   PVEEXCEPTION_WRONGSIZE,
   PVEEXCEPTION_MEMORY,
   PVEEXCEPTION_TOOMUCHJITTER,
   PVEEXCEPTION_ENC_ALG_UNSUPPORTED = 0x0010,
   PVEEXCEPTION_DEC_ALG_UNSUPPORTED,
   PVEEXCEPTION_SUPERINTERVAL
} PVE_EXCEPTION;

#define HAPI_PVE_EGRESS_FRAME_CHANGE_EVENT (HSF_MKEVT_STD(HAPIPVEPKTSRV, 1))   /* Egress frame size change event */
      /* OP1: New frame size (in ms) */
      /* OP2: reserved = 0    */

#define HAPI_PVE_SID_EGRESS_EVENT (HSF_MKEVT_STD(HAPIPVEPKTSRV, 2))    /* Egress SID received event */
      /* OP1: 0 for generic, 1 for built-in */
      /* OP2: reserved = 0    */

/* CNG generation start/stop event
   Defined in hpcodec.h */
#define HAPI_PVE_CNG_EVT      HAPI_CODEC_CNG_EVT

/* Status word defines */
#define HSPVEENABLE           0x1      /* status word bit 0 */

/* AJB Registers */

typedef struct
{
   HAPI_UINT16   jitterMin;        /* Minimum Holding time (in 125 usec samples)
                               * Setting this register will limit the jitter
                               * buffer from adapting below this value. */
   HAPI_UINT16   jitterMax;        /* Maximum Holding Time (in 125 usec samples)
                               * Setting this register to 0 uses the maximum
                               * compile time jitter buffer size. */
   HAPI_UINT16   jitterTarget;     /* Target Holding Time (in 125 usec samples) [initial setting] */
   HAPI_UINT16   options;          /* PVE options. See AJC_OPTIONS_... for usage. */
   HAPI_UINT16   jitterVBDTarget;  /* Target Holding Time in VBD mode (in 125 usec samples) */
   HAPI_UINT16   customFeatures;   /* Special register for tracking the state of custom features */
}
AJC_REGS;

/* AMR-specific control registers */

typedef struct
{
   HAPI_UINT16  modeSet;            /* Set of negotiated useable modes. */
   HAPI_UINT16  modeChangeNeighbor; /* Set to 1 if we can only change to a
                                       neighboring encoder mode on mode switch,
                                       0 if we can switch to any rate */
   HAPI_UINT16  modeChangePeriod;   /* If set to 0 we can switch on any frame,
                                       1 can switch on every second frame */
   HAPI_UINT16  ptime;              /* ptime for each RTP packet. Must be a
                                       multiple of 20, and less than 240 */
   HAPI_UINT16  red;                /* Redundancy to include per RTP packet.
                                       Must be a multiple of 20 and must be
                                       less than 220. Additionally, ptime+red
                                       must be <= 240 */
} AMR_REGS;

/* Vocoder Registers */

typedef struct
{
   HAPI_UINT16     hsxVadCngPlcOptions;      /* VAD/CNG/PLC options */
   HAPI_SINT16     pveEgressVolume;          /* Egress volume control */
   HAPI_SINT16     pveIngressVolume;         /* Ingress volume control */

   /* AMR-specific control registers */
   AMR_REGS        amrOptions;

} HSZVOCREGS;

/* Voice regs */

typedef struct
{
   HSZVOCREGS voxregs;
   AJC_REGS   ajbregs;
}  HSZVOICEREGS;


typedef enum
{
   PLC_OFF = 0,         /* Packet loss concealment off */
   PLC_FRAMEREPEAT,     /* Frame Repeat with 3dB attenuation envelope */
   PLC_G711_APXI,       /* G.711 Appendix I PLC */
   PLC_BVC              /* BroadVoice Concealment (BVC) */

} PLCMODE;

/* Bit fields defined for hsxVoiceCoderOptions */
/* bit0-3 for vad option, bit4-7 reserved bit8-11 cng option bit12-15 plc option */


/* Bit fields defined for hsxVoiceCoderOptions */
/* bit0-3 for vad option, bit4-7 reserved bit8-11 cng option bit12-15 plc option */


/* Bit masks for handling VAD and CNG options */
#define  VAD_MASK    0x000f           /* VAD options are in low 4 bits */
#define  CNG_SHIFT   8                /* CNG options are in bit 11 to bit 8 bits */
#define  CNG_MASK    0x000f           /* CNG options only takes up 4 bits */
#define  PLC_SHIFT   12               /* PLC options are in bit 15 to bit 12 bits */
#define  PLC_MASK    0x000f           /* PLC options only takes up 4 bits */

/***********************************************************************
*
*  New statistics definition from XME
*
************************************************************************/
/* statistics registers for PVE service */

/* Adaptive Jitter Control Module (decoder) Statistics */
typedef struct
{
   /***************** README README README *****************************
    *
    * Add new entries to the end, and update the TRM
    *
    **************** README README README *******************************/

   /* Not in state structure because we want to clear it often with stats reset flag */
   HAPI_UINT16    peakHoldingTime;    /* Peak Holding Time since last statistic */

   /* Total number of superpackets arriving on the egress path */
   HAPI_UINT16    packetCount;        /* # of packets received */

   /* addTail and reorder are successful additions to the JB */
   /* addTail is typically 1..N times the packetCount if superpackets are enabled */
   HAPI_UINT16    addTailCount;       /* # of frames added to tail of JB - normal case */
   HAPI_UINT16    reorderCount;       /* # of frames reordered */

   /* overrun, duplicate, outOfRange, tooLate, jitterMax excess are packets that have been discarded */
   HAPI_UINT16    overrunCount;       /* Decoder overrun count */
   HAPI_UINT16    duplicateCount;     /* # of duplicate frames deleted */
   HAPI_UINT16    outOfRangeCount;    /* # of frames with timestamps too far from current TS to be handled */
   HAPI_UINT16    tooLateCount;       /* Packet arrived too late (it's redundant mate already played) */

   /* cantDecode are packets that can't be played out due to algorithm not available */
   HAPI_UINT16    cantDecodeCount;    /* Can't decode packet - decoder not in load or pkt hdr bad */

   /* The following are internal to the AJC module - they do not represent physical packets */
   HAPI_UINT16    ajcUnderrunCount;   /* Adaptive Jitter Control: jitter buffer underruns */
   HAPI_UINT16    ajcDeleteCount;     /* Adaptive Jitter Control: # of packet deletes done to reduce holding time */
   HAPI_UINT16    ajcRepeatCount;     /* Adaptive Jitter Control: # of packet repeats done to either increase holding time
                                    or due to late frame or lost frames. */
   HAPI_UINT16    ajcResyncCount;     /* Number of times ajb resynced (went through buildout state) */
   HAPI_UINT16    ajcPhaseJitterCount;     /* Number of times ajb inserted a phase discontinuity
                                    (possibly in silence or during CNG or due to a repeat/delete). */
   /* 14 entries.  Make sure this is even (!) */
}
HAPI_PVE_DECODERSTATS;

/* Encoder Statistics */
typedef struct
{
   /***************** README README README *****************************
    *
    * Add new entries to the end, and update the TRM
    *
    **************** README README README *******************************/
   HAPI_UINT16   inactiveFrameCount;     /* Inactive Frame Count of Suppressed Frames*/
   HAPI_UINT16   activeFrameCount;       /* Active Frame Count - actually sent ingress */
   HAPI_UINT16   sidFrameCount;          /* SID Frame Count */
   HAPI_UINT16   toneRelayCount;         /* # of tone packets from PTE relayed ingress */
}
HAPI_PVE_ENCODERSTATS;

/* PVE stats structure */
typedef struct
{
   /***************** README README README *****************************
    *
    * Add new entries to the end, and update the TRM
    *
    **************** README README README *******************************/
   HAPI_PVE_DECODERSTATS   decoder; /* Decoder Statistics */
   HAPI_PVE_ENCODERSTATS   encoder; /* Encoder Statistics */
}
HAPI_PVE_STATS;

/* PVE State */
typedef struct
{
   /***************** README README README *****************************
    *
    * Add new entries to the end, and update the TRM
    *
    **************** README README README *******************************/
   /* These are public Adaptive Jitter Control (AJC) state variables. */
   HAPI_UINT16 timer;                 /* Real time clock */
   HAPI_UINT16 expectedTimestamp;     /* Next expected packet timestamp */
   HAPI_SINT16 lastArrivalTime;       /* Time since last packet arrived (-ve implies packet has never arrived) */
   HAPI_UINT16 lastArrTimeStamp;      /* Timestamp of last arrived packet */
   HAPI_UINT16 lastReleaseTimestamp;  /* Timestamp of last released packet */
   HAPI_UINT16 actualHoldTime;        /* Time from packet arrival to release */
   HAPI_UINT16 currentHoldTime;       /* AJC algorithms adjusted hold time metric */
   HAPI_UINT16 minHoldTimeTracker;    /* Estimate of minimum hold time (in samples) */
   HAPI_UINT16 maxHoldTimeTracker;    /* Estimate of maximum hold time (in samples) */
   HAPI_UINT16 releaseBump;           /* Amount that packets can be released early by */
   HAPI_UINT16 underrunDuration;      /* Used for hold time calculations and packet replays */
   HAPI_SINT16 actionPendingTimer;    /* Replay/delete on low energy or when this timer is <= 0 */
                                 /* Also used to detect when no repeat/delete has been issued in a while */

   /* Public PVE decoder state variables */
   HAPI_UINT16 currentDec;            /* Current decoder index of type HAPI_PKT_VOICECODE */
   HAPI_UINT16 timerLastArrival;      /* last packet arrival time */

   /* Public PVE encoder state variables */
   HAPI_UINT16 currentEnc;            /* Current encoder voice code (vad type + voice code) */
   HAPI_UINT16 currentEncPktTime;     /* Current encoder packet time in samples */
   HAPI_UINT16 currentEncPktSize;     /* Superpacket size including header and preamble */
}
HAPI_PVE_STATE;

typedef struct
{
   HAPI_UINT16 status;
   HAPI_UINT16 clkdriftactive;
   HAPI_UINT16 encodeAllowed;
   HAPI_UINT16 encoderstate;
   HAPI_UINT16 allowratechange;
   HAPI_SINT16 payloadCountDown;
   HAPI_PKT_VOICECODE currentEnc;
   HAPI_SINT16 superpacketinterval;
   HAPI_UINT16 decodeAllowed;
   HAPI_UINT16 decoderactive;
   HAPI_PKT_VOICECODE currentDec;
}
HAPI_PVE_STATUS;

/* Default register definition */


/* default register set */

#define HAPI_VOC_DEF_REGS \
{ \
   (PLC_BVC<<PLC_SHIFT)|(HAPI_GCNG_MATCHSPECTRUM<<CNG_SHIFT)|(HAPI_GVAD_MODE_TRANSPARENT),   /* default VAD/CNG Options */ \
   4096 \
}

#define HAPI_PVE_MIN_JITTER_REG_DEFAULT         (0 << 3)
#define HAPI_PVE_MAX_JITTER_REG_DEFAULT         (0 << 3)
#define HAPI_PVE_TARGET_JITTER_REG_DEFAULT      (0 << 3)
#define HAPI_PVE_OPTIONS_REG_DEFAULT            0
#define HAPI_PVE_VBD_TARGET_JITTER_REG_DEFAULT  (0 << 3)
#define HAPI_PVE_AJC_CUSTOM_DEFAULT             0

/* Custom feature list for the AJC */
#define HAPI_PVE_AJC_IGNORE_SILENCE_DELETION    (1 << 0)

#define HAPI_JIT_DEF_REGS \
{ \
   HAPI_PVE_MIN_JITTER_REG_DEFAULT,          /* Default min jitter buffer length (in 125 us) */  \
   HAPI_PVE_MAX_JITTER_REG_DEFAULT,          /* Default max jitter buffer length (in 125 us) 0 = use max available */  \
   HAPI_PVE_TARGET_JITTER_REG_DEFAULT,       /* Default target time (in 125 us) for jitter buffer */  \
   HAPI_PVE_OPTIONS_REG_DEFAULT,  \
   HAPI_PVE_VBD_TARGET_JITTER_REG_DEFAULT,   /* Default target time (in 125 us) for jitter buffer in VBD */  \
   HAPI_PVE_AJC_CUSTOM_DEFAULT               /* Default value for AJC customer features register */  \
} \


/*
** 'options' register bit format. For normal operation, the ingress/egress bits
** are is set to 0. This means all mutes are off, no forced cng playout over voice,
** and normal audio is sent each way.
** NOTE: These MUST agree with PVE REGISTERS. They are duplicated to break the
** link to the PVE for code sharing. Keep them in sync!
*/
#define AJC_OPTIONS_NOADAPT       0x0080      /* Non adaptive jitter buffer */
#define AJC_OPTIONS_PRINT_EVENTS  0x0100      /* print events/log info (if it is compiled in)  */


/* constants definition of voice coders native frame size */
/* These definitions are inherit from the voice coder and should not be changed by the client */
/* These definitions are put here for test script building purpose */
#define G711WRAP_PKTTIME         40     /* G.711                  packet time in 8 kHz samples */
#define G726WRAP_PKTTIME         40     /* G.726                  packet time in 8 kHz samples */
#define G7231WRAP_PKTTIME        240    /* G.723.1/A 5.3/6.3 kbps packet time in 8 kHz samples */
#define G729WRAP_PKTTIME         80     /* G.729A/B/E             packet time in 8 kHz samples */
#define G728WRAP_PKTTIME         40     /* G.728                  packet time in 8 kHz samples */
#define BVWRAP_PKTTIME           40     /* BroadVoice             packet time in 8 kHz samples */
#define LINPCMWRAP_PKTTIME       40     /* Linear PCM             packet time in 8 KHz samples */
#define G722WRAP_PKTTIME         80     /* G.722                  packet time in 8 kHz samples */
#define G7221WRAP_PKTTIME        160    /* G.722.1                packet time in 8 kHz samples */
#define ILBC20WRAP_PKTTIME       160    /* iLBC (20ms frame)      packet time in 8 kHz samples */
#define ILBC30WRAP_PKTTIME       240    /* iLBC (30ms frame)      packet time in 8 kHz samples */
#define GSMAMRWRAP_PKTTIME       160    /* GSM AMR                packet time in 8 kHz samples */
#define GSMEFRWRAP_PKTTIME       160    /* GSM EFR                packet time in 8 kHz samples */
#define GSMFRWRAP_PKTTIME        160    /* GSM FR                 packet time in 8 kHz samples */
#define GSMHRWRAP_PKTTIME        160    /* GSM HR                 packet time in 8 kHz samples */
#define AMRWBWRAP_PKTTIME        160    /* AMR-WB                 packet time in 8 kHz samples */

#endif      /* HAPI PVE H */



