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
*  hapi_hec.h
*
*  PURPOSE:
*
*     This file implements the HEC API definitions.
* 
*  NOTES:
*
****************************************************************************/
#ifndef HAPI_HEC_H                          /* support nested includes */
#define HAPI_HEC_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>                  /* Hausware API Global definitions */
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
#define  HAPI_HEC_ENABLE_CMD        (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_HEC_SETREGS_CMD       (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_HEC_RESETREGS_CMD     (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_RESETREGS_CMD))
#define  HAPI_HEC_SETREG1_CMD       (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_HEC_GETREGS_CMD       (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_HEC_GETSTATE_CMD      (HSF_MKHDR(HAPI_HEC_SRV, HAPI_OPC_GETSTATE_CMD))

/*
 * HEC service specific commands
 */

#define  HAPI_HEC_DISABLE_CMD        (HSF_MKCMD_STD(HAPI_HEC_SRV, 0))  // obsolete (use enable cmd)
#define  HAPI_HEC_BYPASS_CMD         (HSF_MKCMD_STD(HAPI_HEC_SRV, 1))  // obsolete (use enable cmd)
#define  HAPI_HEC_ADAPT_CMD          (HSF_MKCMD_STD(HAPI_HEC_SRV, 2))  // obsolete (use options bits)
#define  HAPI_HEC_NLP_CMD            (HSF_MKCMD_STD(HAPI_HEC_SRV, 3))  // obsolete (use options bits)
/* 
 * Reset canceller: 
 * op1 = HAPI_HEC_RESET_COLD cold reset (complete reset, power up) 
 * op1 = HAPI_HEC_RESET_WARM warm reset (state cleared, cancellers intact) 
 * op1 = HAPI_HEC_RESET_HOT  hot reset (history only)
 * use warm/hot reset for typical conditions, cold reset for G.168 type testing 
 */
#define  HAPI_HEC_RESET_CMD          (HSF_MKCMD_STD(HAPI_HEC_SRV, 4))

/* 
 * Reset canceller: 
 * op1 = HAPI_HEC_RESETCOEFF_ALL clear FG and BG cancellers
 * op1 = HAPI_HEC_RESETCOEFF_FG  clear FG canceller
 * op1 = HAPI_HEC_RESETCOEFF_BG  clear BG canceller 
 */
#define  HAPI_HEC_RESETCOEFF_CMD     (HSF_MKCMD_STD(HAPI_HEC_SRV, 5))

/* 
 * Stream out coefficients in 8 msec chunks.  Disables coefficient copies while doing so.
 * op1 = op2 = 0
 */
#define  HAPI_HEC_GETCOEFF_CMD       (HSF_MKCMD_STD(HAPI_HEC_SRV, 6))

#define HAPI_HEC_LAST_CMD            6    // set this to the last HEC value (previous line).  AEC CMDs start from here
/* if you change it here, you need change it in hapi_aec.h*/

/*
 * HEC service specific events
 */
#define  HAPI_HEC_STATE_EVT      (HSF_MKEVT_STD(HAPI_HEC_SRV, 0))
   /* op1: mode 0=quiescent, 1=receive, 2=send, 3=doubletalk
      op2: reserved = 0
      op1=252: autotune abort (room to noisy)
      op2=253: autotune peak level during silence monitoring
      op1=254: autotune phase complete (op2=next phase (and if nextphase = 0, autotune complete)).
      op1=255: autotune complete
    */
#define  HAPI_HEC_CONVERGED_EVT  (HSF_MKEVT_STD(HAPI_HEC_SRV, 1))
   /* op1: converged=1, diverged=0
      op2: reserved = 0 */
#define  HAPI_HEC_RESET_EVT      (HSF_MKEVT_STD(HAPI_HEC_SRV, 2))
   /* op1: cold reset=1, reset due to bad ERLE=1, ... (see HAPI_HEC_RESET_xxx)
      op2: 0x0eca for HEC, 0x0aec for AEC) */
#define  HAPI_HEC_PUSH_EVT       (HSF_MKEVT_STD(HAPI_HEC_SRV, 3))
   /* op1: reason for push
      op2: AcomPush (in dB Q.8) */
#define  HAPI_HEC_NEAREND_EVT    (HSF_MKEVT_STD(HAPI_HEC_SRV, 4))
   /* op1 = near end hangover counter, op2 = whitened hangover counter */
#define  HAPI_HEC_LEVEL_EVT      (HSF_MKEVT_STD(HAPI_HEC_SRV, 5))
   /* op1 = rOut level (low byte), suppressor value (high byte) 
    * op2 = sOut level (low byte), suppressor value (high byte) */
#define  HAPI_HEC_AGC_EVT        (HSF_MKEVT_STD(HAPI_HEC_SRV, 6))
   /* op1 = rOut level (low byte), suppressor value (high byte) 
    * op2 = sOut level (low byte), suppressor value (high byte) */
#define  HAPI_HEC_NLP_EVT        (HSF_MKEVT_STD(HAPI_HEC_SRV, 7))
   /* op1: NLP on = 1, off = 0
      op2: NLP mask (reason for NLP turning on/off) */
#define  HAPI_HEC_HIGHLEVEL_EVT  (HSF_MKEVT_STD(HAPI_HEC_SRV, 8))
   /* op1 = 0 => not clipped (otherwise clipped)
    * op2 = 0 => not high level (otherwise high level) */
#define  HAPI_HEC_GETREGS_EVT    (HSF_MKEVT_EXT(HAPI_HEC_SRV, 9))
   /* op1: length of register in byte
      op2: pointer to register */

/* if you change it here, you need change it in hapi_aec.h*/
#define  HAPI_HEC_ERL_EVT          (HSF_MKEVT_STD(HAPI_HEC_SRV, 10))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERL dB, Q.8 */
#define  HAPI_HEC_ERLE_EVT         (HSF_MKEVT_STD(HAPI_HEC_SRV, 11))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERLE dB Q.8 */
#define  HAPI_HEC_ACOM_EVT         (HSF_MKEVT_STD(HAPI_HEC_SRV, 12))
   /* op1: tone frequency (if > 8000, signal is not tonal)
      op2: ERL in high byte (dB), ERLE in low byte (dB) */
#define HAPI_HEC_DEBUG_EVT_NUM     13
#define  HAPI_HEC_DEBUG_EVT        (HSF_MKEVT_EXT(HAPI_HEC_SRV, HAPI_HEC_DEBUG_EVT_NUM))
   /* op1: length
      op2: pointer to event data */
#define  HAPI_HEC_MOVEMENT_EVT     (HSF_MKEVT_STD(HAPI_HEC_SRV, 14))
   /* op1: zone (0-5)
      op2: movement detector output */

/* get state pulled is non-maskable.  Get state pushed is maskable. */
#define HAPI_HEC_GETSTATE_EVT_NUM     15
#define  HAPI_HEC_GETSTATE_EVT         (HSF_MKEVT_EXT(HAPI_HEC_SRV, HAPI_HEC_GETSTATE_EVT_NUM))
   /* op1: length of state in byte
      op2: pointer to state */
/* the following are non-maskable events */
#define  HAPI_HEC_GETCOEFF_EVT         (HSF_MKEVT_EXT(HAPI_HEC_SRV, 16))
   /* op1: length of data (64 presumably)
      op2: pointer to coeffs (coeffs are streamed out in order, from index 0 to the end) */
#define  HAPI_HEC_TUNEACOM_EVT         (HSF_MKEVT_STD(HAPI_HEC_SRV, 18))
   /* op1: Acom (dBm, Q.8)
      op2: noise level offset from -12 dBm (99 for tones) */
#define  HAPI_HEC_TUNEERL_EVT          (HSF_MKEVT_STD(HAPI_HEC_SRV, 19))
   /* op1: minmaxERL[0] (dBm, Q.8)
      op2: minmaxERL[1] (dBm, Q.8) */
#define  HAPI_HEC_TUNEERLE_EVT         (HSF_MKEVT_STD(HAPI_HEC_SRV, 20))
   /* op1: minmaxERLE[0] (dBm, Q.8)
      op2: minmaxERLE[1] (dBm, Q.8) */

#define HAPI_HEC_LAST_EVT            20    // set this to the last HEC value (previous line).  AEC EVTs start from here

/* options reg bit definitions */
#define HAPI_HEC_NLP_OFF        0x0001 /* set bit 0 of options (NLP OFF) */
#define HAPI_HEC_ADAPT_OFF      0x0002 /* set bit 1 of options (adapt OFF) */
#define HAPI_HEC_BYPASS_ON      0x0004 /* set bit 2 of options (bypass ON) */
#define HAPI_HEC_CNG_OFF        0x0008 /* CNG disable */
#define HAPI_HEC_CNG_SUPPRESSOR 0x0010 /* Silence generation enable */
#define HAPI_HEC_CNG_CAP_ON     0x0020 /* CNG cap enable */
#define HAPI_HEC_LOOPBACK_ON    0x0040 /* loopback (egress to ingress) */
#define HAPI_HEC_PUSH_OFF        0x0080 /* disable push (push while converged) */
#define HAPI_HEC_MUTE_ON         0x0100 /* mute output (applicable to both HEC and AEC) */
#define HAPI_HEC_EGRESSNOISE_ON  0x0200 /* Inject egress noise @ -12 dBm (white).  Before volume applied. */
#define HAPI_HEC_EGRESSBURST_ON  0x0400 /* Inject egress noise bursts @ -12 dBm (white).  Before volume applied.
                                            On for 5 msec, off for 200 msec */
#define HAPI_HEC_COMPRESSOR_ON   0x4000 /* always enable compressor on receive path */
#define HAPI_HEC_LEVELCTRL_OFF   0x8000 /* level control switch (switch from level control to making NLP more aggressive) */

/* dB conversion (Q8) */
#ifndef Q_DB
#define Q_DB                    256
#endif
/* conversion from dB to dB (Q.8) is a left shift of 8 */
#define HEC_DB2DBQ8(x)  ((x)<<8)
/* conversion from dB (Q.8) to dB is a right shift of 8 (plus rounding) */
#define HEC_DBQ82DB(x)  (((x)+128)>>8)

/*
 * Default registers and register limits
 */
/* options reg default by default, HEC is in bypass (on hook state) */
#define HAPI_HEC_DEFAULT_OPTIONS  0
#define HAPI_HEC_OPTIONS_ON_HOOK HAPI_HEC_BYPASS_ON
#define HAPI_HEC_OPTIONS_OFF_HOOK 0
#define HAPI_HEC_OPTIONS_VBD      (HAPI_HEC_NLP_OFF|HAPI_HEC_LEVELCTRL_OFF)
#define HAPI_HEC_OPTIONS_CALLSETUP (HAPI_HEC_ADAPT_OFF|HAPI_HEC_LEVELCTRL_OFF)
/* event mask default - no events */
#define HAPI_HEC_DEFAULT_EVENTMASK 0
/* default volume, min and max volume */
#define HAPI_HEC_DEFAULT_VOLUME    0
#define HAPI_HEC_VOLUME_MIN        (-120*Q_DB)
#define HAPI_HEC_VOLUME_MAX        (30*Q_DB)
/* default gain, min and max gain */
#define HAPI_HEC_DEFAULT_GAIN      0
#define HAPI_HEC_GAIN_MIN          (-120*Q_DB)
#define HAPI_HEC_GAIN_MAX          (30*Q_DB)
/* volume and gain compressor defaults */
#define HAPI_HEC_DEFAULT_COMPRESSOR_PEAK   (6*Q_DB)
#define HAPI_HEC_COMPRESSOR_PEAK_MIN       (-25*Q_DB)
#define HAPI_HEC_COMPRESSOR_PEAK_MAX       (6*Q_DB)
#define HAPI_HEC_DEFAULT_NOISE_SUPPRESSION 0
#define HAPI_HEC_NOISE_SUPPRESSION_MIN     0
#define HAPI_HEC_NOISE_SUPPRESSION_MAX     (20*Q_DB)
#define HAPI_HEC_DEFAULT_AGC_TARGET_LEVEL  (-15*Q_DB)
#define HAPI_HEC_DEFAULT_AGC_ATTACK        (0*Q_DB)
#define HAPI_HEC_DEFAULT_AGC_DECAY         (32*Q_DB)
#define HAPI_HEC_DEFAULT_VOLCOMPRESSOR {HAPI_HEC_DEFAULT_COMPRESSOR_PEAK,HAPI_HEC_DEFAULT_NOISE_SUPPRESSION,HAPI_HEC_DEFAULT_AGC_TARGET_LEVEL,HAPI_HEC_DEFAULT_AGC_ATTACK,HAPI_HEC_DEFAULT_AGC_DECAY}
#define HAPI_HEC_DEFAULT_GAINCOMPRESSOR {0,6*Q_DB,HAPI_HEC_DEFAULT_AGC_TARGET_LEVEL,HAPI_HEC_DEFAULT_AGC_ATTACK,HAPI_HEC_DEFAULT_AGC_DECAY}

#define HAPI_HEC_MINIMUM_ERL_DB (6*Q_DB)

#define HAPI_HEC_STATE_CONVERGED  0x0001 /* set converged bit (note: bits 1 and 2 are send/receive mode bits) */
#define HAPI_HEC_STATE_RXSTATIONARY 0x0008 /* stationarity bit (receive) */
#define HAPI_HEC_STATE_HIGH_COMPLEXITY 0x0080
#define HAPI_HEC_STATE_ZONEMASK     0x0070
#define HAPI_HEC_STATE_ZONESHIFT    4

/* indices to HEC registers (mainly for use with setreg1) */
#define HAPI_HEC_INDEX_OPTIONS   0
#define HAPI_HEC_INDEX_EVENTMASK 1
#define HAPI_HEC_INDEX_VOLUME    2
#define HAPI_HEC_INDEX_GAIN      3
#define HAPI_HEC_INDEX_VOLCOMP   4
#define HAPI_HEC_INDEX_GAINCOMP  9
#define HAPI_HEC_INDEX_ERL       14

#define HAPI_HEC_MV_ZONES        5  /* movement zones */

/*
 * Reset events (op1).  Odd reset both cancellers, even resets only one canceller.
 */
#define HAPI_HEC_RESETCOEFF_ALL 0
#define HAPI_HEC_RESETCOEFF_FG  1
#define HAPI_HEC_RESETCOEFF_BG  2
/* Power up reset */
#define HAPI_HEC_RESET_COLD     HAPI_HEC_RESETCOEFF_ALL
/* HEC resets itself (due to anomoly/divergence/severe echo path change) */
#define HAPI_HEC_RESET_ERLE_BAD 1
/* Full reset (wipes out state) */
#define HAPI_HEC_RESET_WARM     3
/* Clear history only.  Use when coming off hook on endpoint device. (e.g. IPphone). */
#define HAPI_HEC_RESET_HOT      4

/* autotuning support (bitfields) */
#define HAPI_HEC_AUTOTUNE_OFF           -1
#define HAPI_HEC_AUTOTUNE_SILENCE       0
#define HAPI_HEC_AUTOTUNE_ERLE          1
#define HAPI_HEC_AUTOTUNE_ERL           2
#define HAPI_HEC_AUTOTUNE_PROFILE       3
#define HAPI_HEC_AUTOTUNE_NLP           4
#define HAPI_HEC_AUTOTUNE_LAST          4

/* define modes (also used for state) */
#define HAPI_HEC_MODE_QUIESCENT         0
#define HAPI_HEC_MODE_RECEIVE           2
#define HAPI_HEC_MODE_SEND              4
#define HAPI_HEC_MODE_DOUBLETALK        (HAPI_HEC_MODE_RECEIVE|HAPI_HEC_MODE_SEND)

#define HAPI_HEC_LPC_ORDER              6

/* HEC registers */
typedef  struct
{
   HAPI_UINT16   options;                /* bit register - options for HEC */
   HAPI_UINT16   eventMask;              /* mask to disable events (0xff -> all enabled) */
   HAPI_SINT16   volumedB;               /* volume (dB Q8) [+30...-120] */
   HAPI_SINT16   gaindB;                 /* gain   (dB Q8) [+30...-120] */
   HAPI_SINT16   volCompressordB[5];     /* 0. peak level at which vol compression attacks (dB, Q8) */
                                    /* 1. suppression of signals near background noise level (dB, Q.8) */
                                    /* 2. AGC target level */
                                    /* 3. AGC attack */
                                    /* 4. AGC decay */
   HAPI_SINT16   gainCompressordB[5];    /* 0. peak level at which gain compression attacks (dB, Q8) */
                                    /* 1. suppression of signals near background noise level (dB, Q.8) */
                                    /* 2. AGC target level */
                                    /* 3. AGC attack */
                                    /* 4. AGC decay */
   HAPI_SINT16   minmaxERLdB[2];         /* minimum/maximum ERL register setting (dB, Q8) */
}
HEC_REGS;

/* HEC state */
typedef  struct
{
   HAPI_UINT16   state;                  /* state bits */
   HAPI_SINT16   Lrin;                   /* Lrin level dB Q.8*/
   HAPI_SINT16   Lrout;                  /* Lrout level dB Q.8*/
   HAPI_SINT16   Lsin;                   /* Lsin level dB Q.8 */
   HAPI_SINT16   Lsout;                  /* Lsout level dB Q.8 */
   HAPI_SINT16   LroutMin;               /* min Lrout level dB Q.8 (background noise level) */
   HAPI_SINT16   LsoutMin;               /* min Lsout level dB Q.8 (background noise level) */
   HAPI_SINT16   acng[HAPI_HEC_LPC_ORDER];  /* background noise spectral estimate */
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
HEC_STATE;

#ifdef __cplusplus
}
#endif

#endif
