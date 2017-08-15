/***************************************************************************
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*    Description:
*
*      Packet Tone Exchange Service Interface
*
****************************************************************************/
#ifndef HAPI_PTE_H
#define HAPI_PTE_H

/* ---- Include Files ---------------------------------------- */
#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif
#ifndef HAPIGDEF_H
#include <hapigdef.h>                  /* Hausware API Global definitions */
#endif
#ifndef HAPIIDS_H
#include <hapiids.h>                   /* Hausware service ids */
#endif
#ifndef HAPI_OPCODE_H
#include <hapi_opcode.h>                  /* Hausware generic cmds/events */
#endif
#ifndef HAPI_PKT_H
#include <hapi_pkt.h>
#endif

/* Service name */
#define HAPI_PTE_SRV_NAME "PTE"

/***************************************************************************
**        PTE SERVICE COMMANDS
****************************************************************************/

/* Generic commands and events have the STD/EXT bit set in the HAPI_OPC_xxx define */
/* Also they don't require descriptions of OP1 and OP2 - see hapi_opcode.h for these */
#define HAPI_PTE_ENABLE_CMD         (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_ENABLE_CMD))
#define HAPI_PTE_SETREGS_CMD        (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_SETREGS_CMD))
#define HAPI_PTE_SETREG1_CMD        (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_SETREG1_CMD))
#define HAPI_PTE_GETREGS_CMD        (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_GETREGS_CMD))
#define HAPI_PTE_GETSTATS_CMD       (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_GETSTATS_CMD))
#define HAPI_PTE_GETSTATE_CMD       (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_GETSTATE_CMD))
#define HAPI_PTE_RESET_STATS_CMD    (HSF_MKHDR(HAPI_PTE_SRV, HAPI_OPC_RESET_STATS_CMD))

#define HAPI_PTE_TONE_END_CMD       (HSF_MKCMD_STD(HAPI_PTE_SRV, 0))
#define HAPI_PTE_TONE_GEN_CMD       (HSF_MKCMD_EXT(HAPI_PTE_SRV, 1))
#define HAPI_PTE_DIALSTRING_CMD     (HSF_MKCMD_EXT(HAPI_PTE_SRV, 2))
#define HAPI_PTE_FILTER_CMD         (HSF_MKCMD_EXT(HAPI_PTE_SRV, 3))
#define HAPI_PTE_TONE_VOLUME_CMD        (HSF_MKCMD_STD(HAPI_PTE_SRV, 7))
#define HAPI_PTE_RESTORE_DECAY_AMP_CMD  (HSF_MKCMD_STD(HAPI_PTE_SRV, 8))
#define HAPI_PTE_CTP_GEN_CMD        (HSF_MKCMD_EXT(HAPI_PTE_SRV, 9))

/***************************************************************************
**        PTE SERVICE RESPONSES
****************************************************************************/

/***************************************************************************
**        PTE SERVICE ASYNCHRONOUS EVENTS
****************************************************************************/

#define HAPI_PTE_TONE_ON_EVT           (HSF_MKEVT_STD(HAPI_PTE_SRV, 0))
#define HAPI_PTE_TONE_OFF_EVT          (HSF_MKEVT_STD(HAPI_PTE_SRV, 1))
#define HAPI_PTE_TONE_GEN_EVT          (HSF_MKEVT_EXT(HAPI_PTE_SRV, 2))
#define HAPI_PTE_ABORT_TONE_EVT        (HSF_MKEVT_STD(HAPI_PTE_SRV, 3))
#define HAPI_PTE_DR_EGRESS_EVT         (HSF_MKEVT_STD(HAPI_PTE_SRV, 4))
#define HAPI_PTE_GETREGS_EVT           (HSF_MKEVT_EXT(HAPI_PTE_SRV, 8))
#define HAPI_PTE_GETSTATS_EVT          (HSF_MKEVT_EXT(HAPI_PTE_SRV, 9))
#define HAPI_PTE_GETSTATE_EVT          (HSF_MKEVT_EXT(HAPI_PTE_SRV, 10))

#define HAPI_PTE_TONE_ON_EGRESS_EVT           (HSF_MKEVT_STD(HAPI_PTE_SRV, 11))
#define HAPI_PTE_TONE_OFF_EGRESS_EVT          (HSF_MKEVT_STD(HAPI_PTE_SRV, 12))
#define HAPI_PTE_ABORT_TONE_EGRESS_EVT        (HSF_MKEVT_STD(HAPI_PTE_SRV, 13))
/* op1 = digit, op2 = pulse time in ticks */

#define HAPI_PTE_ERROR_EVT             (HSF_MKEVT_STD(HAPI_PTE_SRV, 8))

/* Control-bits for PTE mode-settings */
/* See TRM for more details */
#define HAPI_PTE_MODE_DETECTOR_MASK       0x000F /* Digit detector mask (HAPI_PTE_TONE_TYPE) */
#define HAPI_PTE_MODE_DIG_RELAY           0x0010 /* Enable digit (DTMF/MF) relay */
#define HAPI_PTE_MODE_DIG_EVT             0x0020 /* Enable digit (DTMF/MF) detection & events */
#define HAPI_PTE_MODE_DR_EG_EVT           0x0040 /* Enable digit (DTMF/MF) relay egress  events */
#define HAPI_PTE_MODE_SQUELCH             0x0080 /* Squelch ingress digits after detection */
#define HAPI_PTE_MODE_G711_UPSPEED        0x0100 /* OBSOLETE - was Use G711 upspeed for relay */
#define HAPI_PTE_MODE_G711_UPSPEED_LAW    0x0200 /* OBSOLETE - was Use G711 A-law for upspeed relay */
#define HAPI_PTE_MODE_INGRESS_GEN_TO_MEDQ 0x0400 /* ingress generation to media queue instead of IBD media packet */

/********************************************************************************************
** Digit Filtering Definitions and Types.
*********************************************************************************************/
/*
 * Digit filter control structure
 */
typedef struct
{
   HAPI_UINT16 filterControl;                     /* Bit0=filterType, bit1=filterOff */
   HAPI_UINT16 filterDigit;
   HAPI_UINT16 filterLongDigit;
   HAPI_UINT16 longDetectTime;
}
HAPI_PTE_FILTER;

/*
 * filterControl settings
 */
#define HAPI_PTE_FILTER_TYPE_PERMANENT  0
#define HAPI_PTE_FILTER_TYPE_TRIGGER    1
#define HAPI_PTE_FILTER_OFF_TO_HOST     0
#define HAPI_PTE_FILTER_OFF_NOT_TO_HOST 2

#define HAPI_PTE_LONG_DETECT_TIME_DEFAULT    0

/********************************************************************************************
** Programmable tone generation defines and structures.
*********************************************************************************************/

#define  HAPI_PTE_NUMFREQUENCIES         4     /* up to 2 frequencies per tone */
#define  HAPI_PTE_NUMCADENCES            2     /* up to 2 pulses per tone */
#define  HAPI_PTE_NUMFREQUENCY_GRP       3+2   /* Support up to 5 frequency groups.
                                               **     -1 if steady tone is used
                                               **     -1 if additional delay is required between 
                                               **        whole tone repeat counts */

#define  HAPI_PTE_PROFILERECORDLENGTH    4     /* OBSOLETE - length of a profile record */
#define  HAPI_PTE_DIALDIGITARRAYLENGTH   20    /* OBSOLETE - length of dial string digit array */

/* Max digit length string supported */
#define HAPI_PTE_GEN_MAX_DIGITS  32     /* Support maximum of 32 characters in buffer */

#define  HAPI_PTE_NUM_TONEGEN_SLOTS       3     /* up to 3 programmable tone profiles may be
                                                   generated with PTE enhancements enabled */
/* this was misspelled */
#define  HAPI_PTE_NUMCUSTOMETONEPROFILE  HAPI_PTE_NUM_TONEGEN_SLOTS 

/* Tone Type */
typedef enum
{
   HAPI_PTE_DTMF    = 0,                     /* DTMF tone */
   HAPI_PTE_TONEMAX = 1
}
HAPI_PTE_TONE_TYPE;

#define HAPI_PTE_DTMF_INGRESS_MASK  HAPI_PTE_INGRESSMASK
#define HAPI_PTE_DTMF_EGRESS_MASK   HAPI_PTE_EGRESSMASK


/* Pulse States */
typedef enum
{
   HAPI_PTE_CADTYPE_PULSEOFF = 0,            /* Pulse is off with some Cadence */
   HAPI_PTE_CADTYPE_PULSEON = 1              /* Pulse is on with some Cadence */
}  HAPI_PTE_CADENCETYPE;

#define HAPI_PTE_CADTYPEMASK           0x0001
#define HAPI_PTE_CADBIT_MODULATE       0x0002 /* Modulate Freq 1 x Freq 2 */
#define HAPI_PTE_CADBIT_DECAY          0x0004 /* Decay bit */
#define HAPI_PTE_CADBIT_OFFSIGNAL      0x0008 /* 0 = generate silence between generated tones
                                                 1 = leave egress media stream alone between generated tones */
#define HAPI_PTE_CADBIT_NOSTATECHANGE  0x0020 /* No State Changes */
#define HAPI_PTE_CADBIT_COMPLETE       0x0040 /* Complete(Halt and Enable detectors) */
#define HAPI_PTE_CADBIT_LOOPCOUNT      0x0080 /* Loop count in effect, break loop after a while */
#define HAPI_PTE_CADBIT_EVENT          0x0100 /* Bit indicates to generate an event */

#define HAPI_PTE_CADBIT_FREQ1           0x0001 /* Generate Frequency 1 */
#define HAPI_PTE_CADBIT_FREQ2           0x0002 /* Generate Frequency 2 */
#define HAPI_PTE_CADBIT_FREQ3           0x0004 /* Generate Frequency 3 */
#define HAPI_PTE_CADBIT_FREQ4           0x0008 /* Generate Frequency 4 */
#define HAPI_PTE_CADBIT_FREQ5           0x0010 /* Generate Frequency 5 */
#define HAPI_PTE_CADBIT_FREQ6           0x0020 /* Generate Frequency 6 */
#define HAPI_PTE_CADBIT_FREQ7           0x0040 /* Generate Frequency 7 */
#define HAPI_PTE_CADBIT_FREQ8           0x0080 /* Generate Frequency 8 */
#define HAPI_PTE_CADBIT_FREQ9           0x0100 /* Generate Frequency 9 */
#define HAPI_PTE_CADBIT_FREQ10          0x0200 /* Generate Frequency 10 */
#define HAPI_PTE_CADBIT_FREQ11          0x0400 /* Generate Frequency 11 */
#define HAPI_PTE_CADBIT_FREQ12          0x0800 /* Generate Frequency 12 */
#define HAPI_PTE_CADBIT_FREQ13          0x1000 /* Generate Frequency 13 */
#define HAPI_PTE_CADBIT_FREQ14          0x2000 /* Generate Frequency 14 */
#define HAPI_PTE_CADBIT_FREQ15          0x4000 /* Generate Frequency 15 */
#define HAPI_PTE_CADBIT_FREQ16          0x8008 /* Generate Frequency 16 */

#define HAPI_PTE_CADBITS_NEXTSTATE0 0     /* Transition to next state = 0 */
#define HAPI_PTE_CADBITS_NEXTSTATE1 1     /* Transition to next state = 1 */
#define HAPI_PTE_CADBITS_NEXTSTATE2 2     /* Transition to next state = 2 */
#define HAPI_PTE_CADBITS_NEXTSTATE3 3     /* Transition to next state = 3 */
#define HAPI_PTE_CADBITS_NEXTSTATE4 4     /* Transition to next state = 4 */
#define HAPI_PTE_CADBITS_NEXTSTATE5 5     /* Transition to next state = 5 */
#define HAPI_PTE_CADBITS_NEXTSTATE6 6     /* Transition to next state = 6 */
#define HAPI_PTE_CADBITS_NEXTSTATE7 7     /* Transition to next state = 7 */
#define HAPI_PTE_CADBITS_NEXTSTATE8 8     /* Transition to next state = 8 */
#define HAPI_PTE_CADBITS_NEXTSTATE9 9     /* Transition to next state = 9 */
#define HAPI_PTE_CADBITS_NEXTSTATE10 10   /* Transition to next state = 10 */
#define HAPI_PTE_CADBITS_NEXTSTATE11 11   /* Transition to next state = 11 */
#define HAPI_PTE_CADBITS_NEXTSTATE12 12   /* Transition to next state = 12 */
#define HAPI_PTE_CADBITS_NEXTSTATE13 13   /* Transition to next state = 13 */
#define HAPI_PTE_CADBITS_NEXTSTATE14 14   /* Transition to next state = 14 */
#define HAPI_PTE_CADBITS_NEXTSTATE15 15   /* Transition to next state = 15 */



/* custom tone type */
typedef enum
{
   HAPI_PTE_SINEWAVE = 0,          /* sine wave tone */
   HAPI_PTE_SQUAREWAVE,            /* square wave tone */
   HAPI_PTE_TRIANGLEWAVE,          /* triangle wave tone */
   HAPI_PTE_RAMPWAVE,              /* ramp wave tone */
   HAPI_PTE_SQUAREWAVESOS,         /* sum of sines square wave tone,
                                      if supported */
   HAPI_PTE_CUSTOMTONEMAX
} HAPI_PTE_CUSTOMTONE_TYPE;

/* Pulse characteristics */
typedef struct
{
   HAPI_UINT16            PulseType;           /* Pulse Type (HAPI_PTE_CADENCETYPE) */
   HAPI_UINT16            PulseNextState;      /* Next state in state machine */
   HAPI_UINT16            PulseFreqMask;       /* Frequency selection MASK */
   HAPI_UINT16            PulseTime;           /* Pulse time (ms) */
}
HAPI_PTE_PULSE;

/* tone profile freqCtrl field mask bits */
#define HAPI_PTE_NUMFREQMASK           0x00FF
#define HAPI_PTE_TONETYPEMASK          0xFF00
#define HAPI_PTE_TONETYPESHIFT         8
#define HAPI_PTE_MKFREQCTRL(nfreq, tonetype) ((tonetype << HAPI_PTE_TONETYPESHIFT) + nfreq)

/*
** Freq Group Profile
*/
typedef struct
{
   /* Power level:
   **    bit 0:7 egress power level in negative dB
   **    bit 8:15 ingress power level in negative dB 
   */
   HAPI_UINT16 signalLevel;

   /* frequency types */
   HAPI_UINT16 freq[HAPI_PTE_NUMFREQUENCIES];

   /* amplitude & modulation index - do we need 4 amplitudes?? only amp[0] and amp[1] are used */
   HAPI_UINT16 amp[HAPI_PTE_NUMFREQUENCIES];

   /* frequnecy group repeat count */
   HAPI_UINT16 freqGroupRepeatCount;

   /* 
   ** frequency group info:
   **    MSB - number of valid frequencies in the frequency group
   **    LSB - number of valid states in the frequency group 
   */
   HAPI_UINT16 freqGroupInfo;

   /* 2 Cadence states */
   HAPI_PTE_PULSE pulse[HAPI_PTE_NUMCADENCES];

} HAPI_PTE_FREQ_GRP;

#define HAPI_PTE_MKFREQGRP_INFO( numFreq, numStates )    (((numFreq) << 8) | (numStates))
#define HAPI_PTE_GETFREQGRP_NUMFREQ( freqGroupInfo )     (((freqGroupInfo) >> 8))
#define HAPI_PTE_GETFREQGRP_NUMSTATES( freqGroupInfo )   ((freqGroupInfo) & 0xff)


/* 
** Call Progress Tone Profile
*/
typedef struct
{
   /* Number of times to repeat the entire tone */
   HAPI_UINT16 wholeToneRepeatCount;

   /* Number of frequency groups */
   HAPI_UINT16 numFreqGroups;

   /*
   ** Frequency group specific settings
   */
   HAPI_PTE_FREQ_GRP freqGrp[ HAPI_PTE_NUMFREQUENCY_GRP ];

} HAPI_PTE_CP_PROFILE;

/* tone information for use in extended custom tone profile */
typedef struct
{
   HAPI_UINT16 freq;
   HAPI_UINT16 amp;
}
HAPI_PTE_TONE_INFO;

/* Extended custom tone profile */
typedef struct
{
   HAPI_UINT16 freqCtrl;                       /* bit 0-7 number of active frequency. bit 8-15 tone type */
   HAPI_UINT16 loopCount;                      /* Loop counter used for state machine */
   HAPI_UINT16 timeout;                        /* Timeout for completion of generation (ms) */
   HAPI_UINT16 nstate;                         /* number of state it used */
   HAPI_PTE_PULSE pulse[HAPI_PTE_NUMCADENCES]; /* Cadence states */
   HAPI_PTE_TONE_INFO tone[1];                 /* This structure member is a placeholder
                                                  for an array of frequencies and amplitudes,
                                                  the length being bounded by XCFG_PTE_MAX_FREQ_PER_PROFILE
                                                */
}
HAPI_PTE_CUSTOM_TONE_PROFILE;

/* custom tone packet signalContrl field mask bits */
#define HAPI_PTE_SLOTNUMMASK           0xFF00
#define HAPI_PTE_MEDIAQWMASK           0x0004
#define HAPI_PTE_EGRESSMASK            0x0001
#define HAPI_PTE_INGRESSMASK           0x0002
#define HAPI_PTE_SLOTNUMSHIFT          8
#define HAPI_PTE_MEDIAQWSHIFT          2
#define HAPI_PTE_INGRESSSHIFT          1

/* these were misspelled */
#define HAPI_PTE_EGRRESSMASK           HAPI_PTE_EGRESSMASK
#define HAPI_PTE_INGRRESSMASK          HAPI_PTE_INGRESSMASK
#define HAPI_PTE_INGRRESSSHIFT         HAPI_PTE_INGRESSSHIFT

#define HAPI_PTE_MKSIGNALCTRL(ingress, egress, media_ctrl, slotnumber) \
   (egress  + (ingress << 1) + (media_ctrl << 2) + (slotnumber << 8))

#define HAPI_PTE_MKSIGNALLEVEL(eglevel, inglevel) \
        ((eglevel & 0x00FF) + ((inglevel & 0x00FF) << 8))

/* PTE Custom Tone Generation Packet Structure */
typedef struct
{
   HAPI_UINT16 signalContrl;                /* bit 0: 1 generate in Egress,
                                          bit 1: 1 generate in Ingress direction
                                          bit 2: 1 media queue overwrite
                                                 0 media queue overlap
                                          bit 8-15 tone profile slot number */
   HAPI_PTE_CP_PROFILE  profile;
}
HAPI_PTE_CUSTOM_PACKET;

/* PTE Custom Tone Profile Packet Structure */
typedef struct
{
   HAPI_UINT16 signalLevel;                 /* bit 0:7 egress power level in negative dB,
                                          bit 8:15 ingress power level in negative dB */
   HAPI_UINT16 signalContrl;                /* bit 0: 1 generate in Egress,
                                          bit 1: 1 generate in Ingress direction
                                          bit 2: 1 media queue overwrite
                                                 0 media queue overlap
                                          bit 8-15 tone profile slot number */
   HAPI_PTE_CUSTOM_TONE_PROFILE  profile;
}
HAPI_PTE_CTP_PACKET;

typedef struct
{
   HAPI_PKT_HDR packetHdr;
   HAPI_PKT_TONEPAYLOAD tonepayload;
}
HAPI_PKT_TONEPACKET;

/* PTE Dialstring structure */
typedef struct
{
   HAPI_UINT16 encoderType;                 /* 0 = DTMF, 1 = MFR1, 2 = MFR2F, 3 = MFR2B */
   HAPI_UINT16 dBmLevel;                    /* Total power level of DTMF/MF signal */
   HAPI_UINT16 dBmSkew;                     /* Relative power of each frequency (DTMF-only) */
   HAPI_UINT16 pulseOnTime;                 /* Pulse on Time in samples (125us) */
   HAPI_UINT16 pulseOffTime;                /* Pulse off Time in samples (125us) */
   HAPI_UINT16 pauseTime;                   /* Pause time in samples (125us) */
   HAPI_UINT16 numCharacters;               /* Number of characters in dialstring */
   HAPI_UINT16 digitArray[HAPI_PTE_GEN_MAX_DIGITS/2];  /* Packet Digit array to generate */
}
HAPI_PTE_DIALSTRING;

#define HAPI_PTE_PAUSE_DIGIT        0xFF

/* enumeration of international administrations */
typedef enum
{
   HAPI_PTE_ADM_N_AMERICA = 0,
   HAPI_PTE_ADM_GENERAL   = 1,
   HAPI_PTE_ADM_JAPAN     = 2,
   HAPI_PTE_ADM_EUROPE    = 3,
   HAPI_PTE_ADM_AUSTRALIA = 4,
   HAPI_PTE_ADM_BRAZIL    = 5,
   HAPI_PTE_ADM_MAXADM    = 6
}
HAPI_PTE_ADM;

/* PTE tone generation event structure */
typedef struct
{
   HAPI_UINT16 slotNum;                  /* Use slotNum 0 unless generating multiple
                                       custom tones */
   HAPI_UINT16 toneIndex;
   HAPI_UINT16 toneState;
} HAPI_PTE_TONEGENEVT;

/* Complete Tone registers */
typedef struct
{
   HAPI_UINT16   refreshInterval;        /* Refresh interval for Tone Relay (in samples) */
                                    /* Minimum = 30ms = 240 samples */

   HAPI_UINT16   configFlags;            /* Configuration flags */
   HAPI_UINT16   minDetectPower;         /* minimum DTMF detect power in -dBm/freq */
   HAPI_UINT16   detectAdmin;            /* Minimum amount of off time between pulses */
   HAPI_UINT16   pauseTime;               /* Pause duration */
   HAPI_UINT16   minPulseOff;            /* Minimum amount of off time between pulses */
   HAPI_UINT16   minPulseOn;             /* Minimum amount of tone to generate in PulseOn state */

}
HAPI_PTE_REGS;

#define HAPI_PTE_DEFAULT_MINPULSEON  (50*8)     /* Guarantee 50ms of ON time for DTMF digit */
#define HAPI_PTE_DEFAULT_MINPULSEOFF (50*8)     /* Guarantee 50ms of OFF time for DTMF digit */
#define HAPI_PTE_DEFAULT_PAUSETIME   (50*8)     /* Pause characters are 1000ms */

#define HAPI_PTE_DEF_REGS \
{ \
   0,                               /* off by default */                                 \
   0,                                                                                    \
   28,                              /* default -28 dBm/freq (-25dBm nominal) */          \
   HAPI_PTE_ADM_N_AMERICA,           /* default operation is for North America */         \
   HAPI_PTE_DEFAULT_PAUSETIME, \
   HAPI_PTE_DEFAULT_MINPULSEOFF, \
   HAPI_PTE_DEFAULT_MINPULSEON \
} \

/* PTE stats structure */
typedef struct
{
   HAPI_UINT16 numEarlyFalses;           /* Number of times a partial digit was detected */
   HAPI_UINT16 numDetectedDigits;        /* Number of digits successfully detected */
   HAPI_UINT16 numLostDigits;            /* Number of digits lost due to overflow */
   HAPI_UINT16 numGeneratedDigits;       /* Number of digits successfully generated */
}
HAPI_PTE_STATS;

/* PTE state structure */
typedef struct
{
   HAPI_UINT16 modeOptions;              /* PTE mode options as set by enable */
   HAPI_UINT16 filterControl;            /* Copied from HAPI_PTE_FILTER */
   HAPI_UINT16 filterDigit;              /* Copied from HAPI_PTE_FILTER */
   HAPI_UINT16 filterLongDigit;          /* Copied from HAPI_PTE_FILTER */
   HAPI_UINT16 longDetectTime;           /* Copied from HAPI_PTE_FILTER */
   HAPI_UINT16 reserved;                 /* reserved */
}
HAPI_PTE_STATE;

#define HAPI_PTE_DEF_STATE \
{ \
   0, \
   0, \
   0xFFFF, \
   0xFFFF, \
   HAPI_PTE_LONG_DETECT_TIME_DEFAULT, \
   0  \
}

/* DTMF Digits */
typedef enum
{
   HAPI_PTE_DTMFDDGTERROR = 127,
   HAPI_PTE_DTMFDDGT0 = 0,
   HAPI_PTE_DTMFDDGT1 = 1,
   HAPI_PTE_DTMFDDGT2 = 2,
   HAPI_PTE_DTMFDDGT3 = 3,
   HAPI_PTE_DTMFDDGT4 = 4,
   HAPI_PTE_DTMFDDGT5 = 5,
   HAPI_PTE_DTMFDDGT6 = 6,
   HAPI_PTE_DTMFDDGT7 = 7,
   HAPI_PTE_DTMFDDGT8 = 8,
   HAPI_PTE_DTMFDDGT9 = 9,
   HAPI_PTE_DTMFDDGTSTR = 10,
   HAPI_PTE_DTMFDDGTPND = 11,
   HAPI_PTE_DTMFDDGTA   = 12,
   HAPI_PTE_DTMFDDGTB   = 13,
   HAPI_PTE_DTMFDDGTC   = 14,
   HAPI_PTE_DTMFDDGTD   = 15,
   HAPI_PTE_DTMFDMAXDGT = 16
}
HAPI_PTE_DTMFDDGT;

#endif      /* HAPI PTE H */



