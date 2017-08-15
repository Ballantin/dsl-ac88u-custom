/****************************************************************************
*
* Copyright © 2000-2008 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
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
****************************************************************************
*
*    Filename: hdsptone.h
*
****************************************************************************
*    Description:
*
*      Host Tone Generation profiles - header file
*
****************************************************************************/

#ifndef _HDSPTONE_H
#define _HDSPTONE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vrgTypes.h>
#include <vrgCountry.h>
#include <hapi_pte.h>

#include <hdspCfg.h>

/* Default delay between call-waiting signals. */
#define HDSP_DEFAULT_CALL_WAITING_DELAY  10000


/* Tone indexes */
typedef enum
{
   CT_NOTONE = -1,
   CT_DTAS_ONHOOK = 0,
   CT_DTAS_OFFHOOK,
   CT_CALLW_SAS_CAS,
   
} CUSTOM_TONE_ID;


#if !HDSP_CFG_TONEPROV_IETF_SIG_MIB
/*
** Provisioned ring pattern.
**    This is a 64-bit pattern representing a 6 second ring pattern. The first
**    60 MSb represent the ring pattern. A set bit indicates that a ring signal
**    should be applied, a clear bit indicates that no ring signal should be
**    applied. Each bit corresponds to 100 ms. The 61st bit is set to 0 for a
**    repeating pattern, and 1 for a one-shot pattern. The last three bits are
**    reserved for future use. Leading zeros in the pattern are ignored and
**    can be used to shorten the ring pattern length.
*/
typedef struct HDSP_TONE_PROV_PATTERN
{
   VRG_UINT32 msw;
   VRG_UINT32 lsw;
} HDSP_TONE_PROV_PATTERN;

#define HDSP_RING_BITPATTERN_DURATION_MS   100

#else /* HDSP_CFG_TONEPROV_IETF_SIG_MIB */

/* Tone mixing mode */
typedef enum
{
/******************************************************************************/
/**** KK - Feb.17th 2006
      Remove this once we fully integrated draft-9.  Put in place to allow for
      compilation until everything is integrated *****/
   HDSP_TONE_FREQMODE_ignoreSecondary,
   HDSP_TONE_FREQMODE_primarySummedSecondary,
/******************************************************************************/
   HDSP_TONE_FREQMODE_primaryModulateBySecondary,
   HDSP_TONE_FREQMODE_primarySummedAll,

} HDSP_TONE_FREQMODE;


/** Frequency description. */
typedef struct
{
   VRG_UINT32 hertz; /**< Frequency in Hertz. */
   VRG_SINT32 level; /**< Power level in decibels where -63 means off. */
} HDSP_TONE_FREQUENCY_COMPONENT;

/** Support for multiple frequencies with a tone. */
#define MAX_FREQUENCY_COMPONENTS 4

/** Tone description. */
typedef struct
{
   VRG_UINT32 repetitions;     /**< Number of times to repeat frequencies in this tone. */
   VRG_UINT32 soundDuration;   /**< Sound duration in milliseconds. */
   VRG_UINT32 silenceDuration; /**< Silence duration in milliseconds. */
   VRG_UINT32 frequencies;     /**< Number of constituent frequencies. */
   VRG_BOOL   modulateTone;    /**< Tone modulation on/off. */
   VRG_UINT16 modulatePercent; /**< Percent of modulation. The modulation index is calculated by 32767 * (percent of modulation). */
   HDSP_TONE_FREQUENCY_COMPONENT frequency[MAX_FREQUENCY_COMPONENTS]; /**< Frequency composition. If tone modulation is on, frequency 1 is the base carrier frequency and 
                                                                          frequency 2 is the modulation frequency */
} HDSP_TONE_COMPONENT;

/** Support for multiple tones within a tone group. */
#define MAX_TONE_COMPONENTS 5

 
/*
** Provisioned tone pattern.
**    This is a provisioned tone pattern that specifies a custom tone profile.
*/
typedef struct HDSP_TONE_PROV_PATTERN
{
   VRG_UINT32 repetitions;       /**< Number of iterations over the group. */
   VRG_UINT32 tones;             /**< Number of constituents. */
   VRG_BOOL noSteadyTone;        /**< Controls whether steady tone is played after tone has been completely played */
   HDSP_TONE_COMPONENT tone[MAX_TONE_COMPONENTS]; /**< Constituent tones. */
   
} HDSP_TONE_PROV_PATTERN;

#endif   /* HDSP_CFG_TONEPROV_IETF_SIG_MIB */

/* Tone generation direction */
typedef enum
{
   INGRESS_TONE,
   EGRESS_TONE,
   INGRESS_AND_EGRESS_TONE

} HDSP_TONE_DIRECTION;


/*
*****************************************************************************
** FUNCTION PROTOTYPES
*****************************************************************************
*/

VRG_SINT16 hdspToneOn
(
   VRG_UINT16              vhdHdl,
   int                     lineId,
   int                     tone,
   VRG_UINT16              hdspToneDelay,
   VRG_UINT16              hdspToneType,
   HDSP_TONE_PROV_PATTERN *tonePatternBits,
   HDSP_TONE_DIRECTION     direction,
   VRG_BOOL                mediaOption,
   VRG_COUNTRY             country
);

VRG_SINT16 hdspDigitOn
(
   VRG_UINT16           vhdHdl,
   int                  lineId,
   HAPI_PTE_DTMFDDGT    digit,
   VRG_UINT16           dbLevel,
   VRG_UINT16           twist,
   VRG_UINT16           pulseTime,
   HDSP_TONE_DIRECTION  direction
);

VRG_SINT16 hdspToneOff( VRG_UINT16 vhdHdl, HDSP_TONE_DIRECTION direction );
VRG_UINT16 hdspToneAdjustEgressPowerLevel( VRG_UINT16 powerLevel, int lineId );
VRG_SINT16 hdspDigitOff( VRG_UINT16 vhdHdl );
VRG_SINT16 hdspCmtdDBRInit( VRG_UINT16 vhdHdl, VRG_COUNTRY country );

#ifdef __cplusplus
}
#endif

#endif /* _HDSPTONE_H */
