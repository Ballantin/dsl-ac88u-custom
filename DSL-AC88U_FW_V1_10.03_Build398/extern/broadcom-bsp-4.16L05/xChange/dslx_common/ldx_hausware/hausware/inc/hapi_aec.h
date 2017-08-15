/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*           Broadcom
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  hapi_aec.h
*
*  PURPOSE:
*
*     This file implements the AEC API definitions.
* 
*  NOTES:
*
****************************************************************************/

#ifndef HAPI_AEC_H                          /* support nested includes */
#define HAPI_AEC_H

#ifndef HAPI_TYPES
#include <hapi_types.h>                /* Hausware environment definitions */
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

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
*  DEPENDENCIES
****************************************************************************/

/*
 * Generic commands:
 *    - have the STD/EXT bit set in the HAPI_OPC_xxx define
 *    - don't require descriptions of op1 and op2 - see hpopcode.h for these
 */
#define  HAPI_AEC_ENABLE_CMD        (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_AEC_SETREGS_CMD       (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_AEC_RESETREGS_CMD     (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_RESETREGS_CMD))
#define  HAPI_AEC_SETREG1_CMD       (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_AEC_GETREGS_CMD       (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_AEC_GETSTATE_CMD      (HSF_MKHDR(HAPI_AEC_SRV, HAPI_OPC_GETSTATE_CMD))

/*
 * AEC service specific commands
 */

#define  HAPI_AEC_DISABLE_CMD        (HSF_MKCMD_STD(HAPI_AEC_SRV, 0))  // obsolete (use enable cmd)
#define  HAPI_AEC_BYPASS_CMD         (HSF_MKCMD_STD(HAPI_AEC_SRV, 1))  // obsolete (use enable cmd)
#define  HAPI_AEC_ADAPT_CMD          (HSF_MKCMD_STD(HAPI_AEC_SRV, 2))  // obsolete (use options bits)
#define  HAPI_AEC_NLP_CMD            (HSF_MKCMD_STD(HAPI_AEC_SRV, 3))  // obsolete (use options bits)
/* 
 * Reset canceller: 
 * op1 = HAPI_AEC_RESET_COLD cold reset (complete reset, power up) 
 * op1 = HAPI_AEC_RESET_WARM warm reset (state cleared, cancellers intact) 
 * op1 = HAPI_AEC_RESET_HOT  hot reset (history only)
 * use warm/hot reset for typical conditions, cold reset for G.168 type testing 
 */
#define  HAPI_AEC_RESET_CMD          (HSF_MKCMD_STD(HAPI_AEC_SRV, 4))

/* 
 * Reset canceller: 
 * op1 = HAPI_AEC_RESETCOEFF_ALL clear FG and BG cancellers
 * op1 = HAPI_AEC_RESETCOEFF_FG  clear FG canceller
 * op1 = HAPI_AEC_RESETCOEFF_BG  clear BG canceller 
 */
#define  HAPI_AEC_RESETCOEFF_CMD     (HSF_MKCMD_STD(HAPI_AEC_SRV, 5))

/* 
 * Stream out coefficients in 8 msec chunks.  Disables coefficient copies while doing so.
 * op1 = op2 = 0
 */
#define  HAPI_AEC_GETCOEFF_CMD       (HSF_MKCMD_STD(HAPI_AEC_SRV, 6))

/* set the  second aec register value 
* op1 is the offset index
* op2 is the new value 
*/
#define  HAPI_AEC_SETREG2_CMD       (HSF_MKCMD_STD(HAPI_AEC_SRV, 7))

/*
 * AEC service specific events
 */
#define  HAPI_AEC_STATE_NUM      0
#define  HAPI_AEC_STATE_MASK     (1<<HAPI_AEC_STATE_NUM)
#define  HAPI_AEC_STATE_EVT      (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_STATE_NUM))
   /* op1: mode 0=quiescent, 1=receive, 2=send, 3=doubletalk
      op2: reserved = 0
      op1=252: autotune abort (room too noisy)
      op2=253: autotune peak level during silence monitoring
      op1=254: autotune phase complete (op2=next phase (and if nextphase = 0, autotune complete)).
      op1=255: autotune complete
    */
#define  HAPI_AEC_CONVERGED_NUM  1
#define  HAPI_AEC_CONVERGED_MASK (1<<HAPI_AEC_CONVERGED_NUM)
#define  HAPI_AEC_CONVERGED_EVT  (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_CONVERGED_NUM))
   /* op1: converged=1, diverged=0
      op2: reserved = 0 */
#define  HAPI_AEC_RESET_NUM      2
#define  HAPI_AEC_RESET_MASK     (1<<HAPI_AEC_RESET_NUM)
#define  HAPI_AEC_RESET_EVT      (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_RESET_NUM))
   /* op1: cold reset=1, reset due to bad ERLE=1, ... (see HAPI_AEC_RESET_xxx)
      op2: 0x0eca for ECAN, 0x0aec for AEC) */
#define  HAPI_AEC_PUSH_NUM       3
#define  HAPI_AEC_PUSH_MASK      (1<<HAPI_AEC_PUSH_NUM)
#define  HAPI_AEC_PUSH_EVT       (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_PUSH_NUM))
   /* op1: reason for push
      op2: AcomPush (in dB Q.8) */
#define  HAPI_AEC_NEAREND_NUM    4
#define  HAPI_AEC_NEAREND_MASK   (1<<HAPI_AEC_NEAREND_NUM)
#define  HAPI_AEC_NEAREND_EVT    (HSF_MKEVT_EXT(HAPI_AEC_SRV, HAPI_AEC_NEAREND_NUM))
   /* op1 = near end hangover counter, op2 = whitened hangover counter */
#define  HAPI_AEC_LEVEL_NUM      5
#define  HAPI_AEC_LEVEL_MASK     (1<<HAPI_AEC_LEVEL_NUM)
#define  HAPI_AEC_LEVEL_EVT      (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_LEVEL_NUM))
   /* op1 = rOut level (low byte), suppressor value (high byte) 
    * op2 = sOut level (low byte), suppressor value (high byte) */
#define  HAPI_AEC_AGC_NUM      6
#define  HAPI_AEC_AGC_MASK     (1<<HAPI_AEC_AGC_NUM)
#define  HAPI_AEC_AGC_EVT        (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_AGC_NUM))
   /* op1 = rOut level (low byte), suppressor value (high byte) 
    * op2 = sOut level (low byte), suppressor value (high byte) */
#define  HAPI_AEC_NLP_NUM      7
#define  HAPI_AEC_NLP_MASK     (1<<HAPI_AEC_NLP_NUM)
#define  HAPI_AEC_NLP_EVT        (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_NLP_NUM))
   /* op1: NLP on = 1, off = 0
      op2: NLP mask (reason for NLP turning on/off) */
#define  HAPI_AEC_HIGHLEVEL_NUM      8
#define  HAPI_AEC_HIGHLEVEL_MASK     (1<<HAPI_AEC_HIGHLEVEL_NUM)
#define  HAPI_AEC_HIGHLEVEL_EVT  (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_HIGHLEVEL_NUM))
   /* op1 = 0 => not clipped (otherwise clipped)
    * op2 = 0 => not high level (otherwise high level) */
#define  HAPI_AEC_GETREGS_NUM      9
#define  HAPI_AEC_GETREGS_MASK     (1<<HAPI_AEC_GETREGS_NUM)
#define  HAPI_AEC_GETREGS_EVT    (HSF_MKEVT_EXT(HAPI_AEC_SRV, HAPI_AEC_GETREGS_NUM))
   /* op1: length of register in byte
      op2: pointer to register */

#define  HAPI_AEC_ERL_NUM      10
#define  HAPI_AEC_ERL_MASK     (1<<HAPI_AEC_ERL_NUM)
#define  HAPI_AEC_ERL_EVT          (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_ERL_NUM))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERL dB, Q.8 */
#define  HAPI_AEC_ERLE_NUM      11
#define  HAPI_AEC_ERLE_MASK     (1<<HAPI_AEC_ERLE_NUM)
#define  HAPI_AEC_ERLE_EVT         (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_ERLE_NUM))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERLE dB Q.8 */
#define  HAPI_AEC_ACOM_NUM      12
#define  HAPI_AEC_ACOM_MASK     (1<<HAPI_AEC_ACOM_NUM)
#define  HAPI_AEC_ACOM_EVT         (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_ACOM_NUM))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERL in high byte (dB), ERLE in low byte (dB) */
#define  HAPI_AEC_DEBUG_NUM      13
#define  HAPI_AEC_DEBUG_MASK     (1<<HAPI_AEC_DEBUG_NUM)
#define  HAPI_AEC_DEBUG_EVT        (HSF_MKEVT_EXT(HAPI_AEC_SRV, HAPI_AEC_DEBUG_NUM))
   /* op1: length
      op2: pointer to event data */
#define  HAPI_AEC_MOVEMENT_NUM      14
#define  HAPI_AEC_MOVEMENT_MASK     (1<<HAPI_AEC_MOVEMENT_NUM)
#define  HAPI_AEC_MOVEMENT_EVT     (HSF_MKEVT_STD(HAPI_AEC_SRV, HAPI_AEC_MOVEMENT_NUM))
   /* op1: zone (0-5)
      op2: movement detector output */

/* get state pulled is non-maskable.  Get state pushed is maskable. */
#define  HAPI_AEC_GETSTATE_NUM      15
#define  HAPI_AEC_GETSTATE_MASK     (1<<HAPI_AEC_GETSTATE_NUM)
#define  HAPI_AEC_GETSTATE_EVT         (HSF_MKEVT_EXT(HAPI_AEC_SRV, HAPI_AEC_GETSTATE_NUM))
   /* op1: length of state in byte
      op2: pointer to state */
/* the following are non-maskable events */
#define  HAPI_AEC_GETCOEFF_EVT         (HSF_MKEVT_EXT(HAPI_AEC_SRV, 16))
   /* op1: length of data (64 presumably)
      op2: pointer to coeffs (coeffs are streamed out in order, from index 0 to the end) */
#define  HAPI_AEC_TUNEACOM_EVT         (HSF_MKEVT_STD(HAPI_AEC_SRV, 18))
   /* op1: Acom (dBm, Q.8)
      op2: noise level offset from -12 dBm (99 for tones) */
#define  HAPI_AEC_TUNEERL_EVT          (HSF_MKEVT_STD(HAPI_AEC_SRV, 19))
   /* op1: minmaxERL[0] (dBm, Q.8)
      op2: minmaxERL[1] (dBm, Q.8) */
#define  HAPI_AEC_TUNEERLE_EVT         (HSF_MKEVT_STD(HAPI_AEC_SRV, 20))
   /* op1: minmaxERLE[0] (dBm, Q.8)
      op2: minmaxERLE[1] (dBm, Q.8) */

/* options reg bit definitions */
#define HAPI_AEC_NLP_OFF        0x0001 /* set bit 0 of options (NLP OFF) */
#define HAPI_AEC_ADAPT_OFF      0x0002 /* set bit 1 of options (adapt OFF) */
#define HAPI_AEC_BYPASS_ON      0x0004 /* set bit 2 of options (bypass ON) */
#define HAPI_AEC_CNG_OFF         0x0008 /* set bit 3 of options (CNG off) */
#define HAPI_AEC_HOTHCNG_ON      0x0010 /* set bit 4 of options (hoth CNG) */
#define HAPI_AEC_LOOPBACK_ON     0x0020 /* loopback (egress to ingress) */
#define HAPI_AEC_SUPPRESSORS_OFF 0x0040 /* disable receive/transmit suppressors (AEC only) */
#define HAPI_AEC_PUSH_OFF        0x0080 /* disable push (push while converged) */
#define HAPI_AEC_MUTE_ON         0x0100 /* mute output */
#define HAPI_AEC_EGRESSNOISE_ON  0x0200 /* Inject egress noise @ -12 dBm (white).  Before volume applied. */
#define HAPI_AEC_EGRESSBURST_ON  0x0400 /* Inject egress noise bursts @ -12 dBm (white).  Before volume applied.
                                            On for 5 msec, off for 200 msec */
#define HAPI_AEC_TONESWEEP_ON    0x0800 /* tone sweep */
#define HAPI_AEC_DIGITALMW_ON    0x1000 /* digital milliwatt egress on (0 dBm0 1000 Hz tone) */
#define HAPI_AEC_AUTOTUNE_ON     0x2000 /* autotune monitoring of Sin level over 1 second */
#define HAPI_AEC_UNWGHTPWR_ON    0x4000 /* unweighted power estimates for Rout and Sin */
#define HAPI_AEC_LEVELCTRL_OFF   0x8000 /* level control switch (switch from level control to making NLP more aggressive) */

/* dB conversion (Q8) */
#ifndef Q_DB
#define Q_DB                    256
#endif

#define HAPI_AEC_SUBBANDS        8      /* number of subbands */
/*
 * Default registers and register limits
 */
/* options reg default by default, AEC is in bypass (on hook state) */
#define HAPI_AEC_DEFAULT_OPTIONS  0
#define HAPI_AEC_OPTIONS_ON_HOOK HAPI_AEC_BYPASS_ON
#define HAPI_AEC_OPTIONS_OFF_HOOK 0
#define HAPI_AEC_OPTIONS_CALLSETUP (HAPI_AEC_ADAPT_OFF|HAPI_AEC_LEVELCTRL_OFF)
/* event mask default - no events */
#define HAPI_AEC_DEFAULT_EVENTMASK 0
/* default volume, min and max volume */
#define HAPI_AEC_DEFAULT_VOLUME    0
#define HAPI_AEC_VOLUME_MIN        (-120*Q_DB)
#define HAPI_AEC_VOLUME_MAX        (30*Q_DB)
/* default gain, min and max gain */
#define HAPI_AEC_DEFAULT_GAIN      0
#define HAPI_AEC_GAIN_MIN          (-120*Q_DB)
#define HAPI_AEC_GAIN_MAX          (30*Q_DB)
/* volume and gain compressor defaults */
#define HAPI_AEC_DEFAULT_COMPRESSOR_PEAK   (6*Q_DB)
#define HAPI_AEC_COMPRESSOR_PEAK_MIN       (-25*Q_DB)
#define HAPI_AEC_COMPRESSOR_PEAK_MAX       (6*Q_DB)
#define HAPI_AEC_DEFAULT_NOISE_SUPPRESSION 0
#define HAPI_AEC_NOISE_SUPPRESSION_MIN     0
#define HAPI_AEC_NOISE_SUPPRESSION_MAX     (20*Q_DB)
#define HAPI_AEC_DEFAULT_AGC_TARGET_LEVEL  (-15*Q_DB)
#define HAPI_AEC_DEFAULT_AGC_ATTACK        (0*Q_DB)
#define HAPI_AEC_DEFAULT_AGC_DECAY         (32*Q_DB)
#define HAPI_AEC_DEFAULT_VOLCOMPRESSOR {HAPI_AEC_DEFAULT_COMPRESSOR_PEAK,HAPI_AEC_DEFAULT_NOISE_SUPPRESSION,HAPI_AEC_DEFAULT_AGC_TARGET_LEVEL,HAPI_AEC_DEFAULT_AGC_ATTACK,HAPI_AEC_DEFAULT_AGC_DECAY}
#define HAPI_AEC_DEFAULT_GAINCOMPRESSOR {0,6*Q_DB,HAPI_AEC_DEFAULT_AGC_TARGET_LEVEL,HAPI_AEC_DEFAULT_AGC_ATTACK,HAPI_AEC_DEFAULT_AGC_DECAY}

#define HAPI_AEC_MINIMUM_ERL_DB (20*Q_DB)

#define HAPI_AEC_STATE_CONVERGED  0x0001 /* set converged bit (note: bits 1 and 2 are send/receive mode bits) */
#define HAPI_AEC_STATE_RXSTATIONARY 0x0008 /* stationarity bit (receive) */
#define HAPI_AEC_STATE_HIGH_COMPLEXITY 0x0080
#define HAPI_AEC_STATE_ZONEMASK     0x0070
#define HAPI_AEC_STATE_ZONESHIFT    4

/* indices to AEC registers (mainly for use with setreg1) */
#define HAPI_AEC_INDEX_OPTIONS   0
#define HAPI_AEC_INDEX_EVENTMASK 1
#define HAPI_AEC_INDEX_VOLUME    2
#define HAPI_AEC_INDEX_GAIN      3
#define HAPI_AEC_INDEX_VOLCOMP   4
#define HAPI_AEC_INDEX_GAINCOMP  9
#define HAPI_AEC_INDEX_ERL       14

#define HAPI_AEC_MV_ZONES        5  /* movement zones */

/*
 * Reset events (op1).  Odd reset both cancellers, even resets only one canceller.
 */
#define HAPI_AEC_RESETCOEFF_ALL 0
#define HAPI_AEC_RESETCOEFF_FG  1
#define HAPI_AEC_RESETCOEFF_BG  2
/* Power up reset */
#define HAPI_AEC_RESET_COLD     HAPI_AEC_RESETCOEFF_ALL
/* AEC resets itself (due to anomoly/divergence/severe echo path change) */
#define HAPI_AEC_RESET_ERLE_BAD 1
/* Full reset (wipes out state) */
#define HAPI_AEC_RESET_WARM     3
/* Clear history only.  Use when coming off hook on endpoint device. (e.g. IPphone). */
#define HAPI_AEC_RESET_HOT      4

/* define modes (also used for state) */
#define HAPI_AEC_MODE_QUIESCENT         0
#define HAPI_AEC_MODE_RECEIVE           2
#define HAPI_AEC_MODE_SEND              4
#define HAPI_AEC_MODE_DOUBLETALK        (HAPI_AEC_MODE_RECEIVE|HAPI_AEC_MODE_SEND)

#define HAPI_AEC_LPC_ORDER              6

/* AEC state */
typedef  struct
{
   HAPI_UINT16   state;                  /* state bits */
   HAPI_SINT16   Lrin;                   /* Lrin level dB Q.8*/
   HAPI_SINT16   Lrout;                  /* Lrout level dB Q.8*/
   HAPI_SINT16   Lsin;                   /* Lsin level dB Q.8 */
   HAPI_SINT16   Lsout;                  /* Lsout level dB Q.8 */
   HAPI_SINT16   LroutMin;               /* min Lrout level dB Q.8 (background noise level) */
   HAPI_SINT16   LsoutMin;               /* min Lsout level dB Q.8 (background noise level) */
   HAPI_SINT16   acng[HAPI_AEC_LPC_ORDER];  /* background noise spectral estimate */
   HAPI_SINT16   erldB;                  /* erl estimate dB, Q8 */
   HAPI_SINT16   erlcdB;                 /* erl estimate based on coeff sum of squares (dB), Q8 */
   HAPI_SINT16   erledB;                 /* erle estimate dB, Q8 */
   HAPI_SINT16   volumeAutodB;           /* AGC gain, receive */
   HAPI_SINT16   gainAutodB;             /* AGC gain, send */
   HAPI_UINT16   nlp;                    /* NLP state, last 16 (5msec) blocks [so last 80 msec] */
   HAPI_UINT16   voiceDecisions;         /* voice decisions, each bit represents 5 msec [so last 80 msec] */
   HAPI_UINT16   adaptCount;             /* adapt count */
   HAPI_UINT16   peakReflector;          /* position of the largest reflector [peak in h reg] */
   HAPI_SINT16   peakReflectorValue;     /* abs value of the largest reflector [peak in h reg] */
}
AEC_STATE;

/* conversion from dB to dB (Q.8) is a left shift of 8 */
#define AEC_DB2DBQ8(x)  ((x)<<8)
/* conversion from dB (Q.8) to dB is a right shift of 8 (plus rounding) */
#define AEC_DBQ82DB(x)  (((x)+128)>>8)

/* max dispersion supported for NLP */
#define HAPI_AEC_MAX_DISPERSION_MS 200
#define HAPI_AEC_MAX_DISPERSION_BLOCKS (HAPI_AEC_MAX_DISPERSION_MS/5)

/* send terminal into autotune mode */
/* 
 * autotune command to automatically tune AEC 
 * op1: 00001b check send levels (value is in BINARY)
 * op1: 00010b autotune ERLE and compressor (receive)
 * op1: 00100b autotune ERL
 * op1: 01000b autotune profile
 * op1: 10000b autotune NLP
 * Logical ORing the above values will tune each parameter in turn.  So, 11111b == 31 decimal tunes all params.
 * 011110 binary (30 decimal) tunes all, but does no check levels.
 */
#define  HAPI_AEC_AUTOTUNE_CMD       (HSF_MKCMD_STD(HAPI_AEC_SRV, 11))

#define  HAPI_AEC_DECAYPROFILE_EVT   (HSF_MKEVT_EXT(HAPI_AEC_SRV, 22))
   /* op1: length of decay profile in bytes
      op2: pointer to decay profile */

typedef struct
{
   HAPI_UINT16   options;                           /* bit register - options for AEC */
   HAPI_UINT16   eventMask;                         /* mask to disable events (0xff -> all enabled) */
   HAPI_SINT16   volumedB;                          /* volume (dB Q8) [+12...-12] */
   HAPI_SINT16   gaindB;                            /* gain   (dB Q8) [+12...-12] */
   HAPI_SINT16   volCompressordB[5];                /* 0. peak level at which vol compression attacks (dB, Q8) */
                                               /* 1. suppression of signals near background noise level (dB, Q.8) */
                                               /* 2. AGC target level */
                                               /* 3. AGC attack */
                                               /* 4. AGC decay */
   HAPI_SINT16   gainCompressordB[5];               /* 0. peak level at which gain compression attacks (dB, Q8) */
                                               /* 1. suppression of signals near background noise level (dB, Q.8) */
                                               /* 2. AGC target level */
                                               /* 3. AGC attack */
                                               /* 4. AGC decay */
   HAPI_SINT16   minmaxERLdB[2];                    /* min/max ERL register setting (dB, Q8) */
   HAPI_SINT16   minmaxERLEdB[2];                   /* min/max ERLE register setting (dB, Q8) */
   HAPI_SINT16   rOutMaxSuppdB[2];                  /* min/max suppression for receive (dB, Q8) */
   HAPI_SINT16   sOutMaxSuppdB[2];                  /* min/max suppression for send (dB, Q8) */
   HAPI_SINT16   receiveBiasdB;                     /* bias in favour of receive (over send) (dB, Q8) */
   HAPI_SINT16   hlLimit;                           /* peak level limiter on receive */
   HAPI_SINT16   nlWhite;                           /* non-linearity threshold on whitened detector */
   HAPI_SINT16   nlWhiteHl;                         /* non-linearity threshold on whitened detector (high level) */
   HAPI_SINT16   nlpHl;                             /* nlp high level limit (NLP aggressive above this level) */
   HAPI_SINT16   minSindB;                          /* minimum sIn level before doubletalk/near end can be detected */
   HAPI_SINT16   minRoutdB;                         /* minimum rOut level before doubletalk/near end can be detected */
   HAPI_SINT16   activeSindB;                       /* threshold on sIn level before longer hangover count is used */
   HAPI_SINT16   activeRoutdB;                      /* threshold on rOut level before longer hangover count is used */
   HAPI_SINT16   hpfr[4];                           /* highpass parameters on receive path */
   HAPI_SINT16   hangCountNearNLP[2];               /* NLP hangover counter (for near end detection) */
   HAPI_SINT16   hangCountNearAdapt[2];             /* Adaptation hangover counter */
   HAPI_SINT16   hangCountFar[2];                   /* Far end detected hangover counter (for far end detection) */
   HAPI_SINT16   nlpLowLevel[3];                    /* NLP thresh on low level detection */
   HAPI_SINT16   nlpERLEbrk[2];                     /* NLP thresh on ERLE (breakin vs singletalk) */
   HAPI_SINT16   nlpAcomBrk[2];                     /* NLP thresh on Acom (breakin vs singletalk) */
   HAPI_SINT16   nlpERLEdbltk[2];                   /* NLP thresh on ERLE (doubletalk) */
   HAPI_SINT16   nlpAcomDbltk[2];                   /* NLP thresh on Acom (doubletalk) */
   HAPI_SINT16   decayProfiles[12];                 /* terminal decay profiles */
   HAPI_SINT16   sbErl[HAPI_AEC_SUBBANDS];          /* sb erl */
   HAPI_SINT16   sbHl[HAPI_AEC_SUBBANDS];           /* high level threshold for each subband */
   HAPI_SINT16   sbNl[HAPI_AEC_SUBBANDS];           /* non-linearity threshold for each subband */
   HAPI_SINT16   sbNlUpper[HAPI_AEC_SUBBANDS];      /* upper non-linearity threshold for each subband */
   HAPI_SINT16   sbNlHl[HAPI_AEC_SUBBANDS];         /* non-linearity threshold for each subband (high level) */
   HAPI_SINT16   sbNlHlUpper[HAPI_AEC_SUBBANDS];    /* upper non-linearity threshold for each subband (high level) */
   HAPI_SINT16   erlOffset[3];                      /* erl offsets */
   HAPI_SINT16   erlMv[HAPI_AEC_MV_ZONES];          /* movement offsets on ERL  */
   HAPI_SINT16   mvThresh[HAPI_AEC_MV_ZONES];       /* movement threshold */
   HAPI_SINT16   convergedThresh[2];                /* converged,diverged thresh (Acom) */
   HAPI_SINT16   stateInterval;                     /* interval for issueing getstate event */
   HAPI_SINT16   toneIndex;                         /* index into tone table for tone generation */
   HAPI_SINT16   dbg;                               /* debug info/index */
}
AEC_REGS;



/* options reg2 bit definitions */
#define HAPI_AEC_INGRESS_SUBBAND_OFF        0x0001 /* set bit 0 of options (ingress subband OFF) */
#define HAPI_AEC_EGRESS_SUBBAND_OFF         0x0002 /* set bit 1 of options (egress subband OFF) */

typedef struct
{
   HAPI_SINT16  options;
   HAPI_SINT16  tailLength;
}
AEC_REGS2; 

/* Get AEC regs (default) */
#include <hapi_aecregs.h>

#ifdef __cplusplus
}
#endif

#endif
