/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
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
*    Filename: hdsptone.c
*
****************************************************************************
*    Description:
*
*      Host Tone Generation API
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgCountry.h>
#include <xchgAssert.h>
#include <hapi_net.h>
#include <hdspCfg.h>
#include <hapi_pte.h>
#include <vrgCountry.h>
#include <string.h>
#include <hdspCfg.h>
#include <hdspToneProfile.h>
#include "hdspTone.h"
#include "hdspCmd.h"
#include <hapi_pkt.h>
#include <hapi_cmtd.h>  /* Call Monitor Tone Detection  */
//#include <hdspCmtd400Mod20.h>
#include <xdrvApm.h>
#include <xdrvSlic.h>
#include <boardHalApm.h>
#include <boardHalSlic.h>
#include <boardHalProv.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
#define HDSP_TONEPROV_DEBUG             0  /* Set to 1 to debug provisioned tone patterns */

#define MSB_MASK (0x80000000)

/* ---- Private Variables ------------------------------------------------ */

/* Define the country profile to be the static profile if not already
   define through the compiler */


/* Amplitudes of DTMF Skew from 1dBm to 5dBm step 1dBm.
 * These amplitutes assume dual frequency
 * where the higher frequency is the "zero" as defined G.711 specification
 * (see PTE TRM and SRV for details). The lower frequency is adjusted
 * relative to the higher frequency according to the following:
 * '22656 * 10 ^ (dbmSkew/20)' where dbmSkew is the separation or skew level.
 */
const static VRG_UINT16 dtmfSkewMap[] = {
   20192, /* dbmSkew = 1 */
   17996, /* dbmSkew = 2 */
   16039, /* dbmSkew = 3 */
   14295, /* dbmSkew = 4 */
   12740  /* dbmSkew = 5 */
};
#define MIN_DTMF_SKEW 1 /* Corresponding to Skew of 1dBm */
#define MAX_DTMF_SKEW 5 /* Corresponding to Skew of 5dBm */

/***********************************************************************************
** Generic DTMF tone profile.
**
** The hdspDigitOn routine will change the DTMF tone power level, tone frequencies,
** and the pulse duration based upon the digit played and the country in use.
***********************************************************************************/

static HDSP_CUSTOM_TONE dtmfToneGeneric =
{  "DTMF",
   {
      /* Whole Repeat Count */
      0,
      /* number of frequency groups */
      1,
      /* Freq Groups */
      {
         /* Group 1 */
         {
            /* High group frequency = -4 dBm */
            4,
            /* 4 FREQUENCIES */
            {941, 1633, 0, 0},      /* 941, 1633, 0, 0 Hz */
            /* 4 AMPLITUDES */
            {17996, 22656, 0, 0},   /* There is a 2dB skew between high and low freq group.
                                     ** Low Freq Amplitude = -2 dBm, High Freq Amplitude = 0 dBm */
            /* frequency group repeat */
            0,
            /* number of frequencies and states */
            HAPI_PTE_MKFREQGRP_INFO( 2, 2 ),
            {
               /* State 0 */
               /* Pulse is on continous when entering this state */
               { HAPI_PTE_CADTYPE_PULSEON,
                 HAPI_PTE_CADBITS_NEXTSTATE1,
                 ( HAPI_PTE_CADBIT_FREQ1 | HAPI_PTE_CADBIT_FREQ2 ),
                 100
               },

               /* State 1 */
               /* Pulse is off continous when entering this state */
               { ( HAPI_PTE_CADTYPE_PULSEOFF | HAPI_PTE_CADBIT_COMPLETE ),
                 0,
                 0,
                 100
               }
            }
         }
      }
      /* Group 2-16 + 2 - not used */
   }
};

/* Negative amplitude factor table */
static VRG_UINT16 negFactor[] = {
   20192, 17996, 16039, 14294, 12740, 11354, 10120, 9019,
   8038, 7164, 6385, 5690, 5072, 4520, 4028, 3590,
   3200, 2852, 2542, 2265, 2019, 1799, 1603, 1429,
   1274, 1135, 1012, 901, 803, 716, 638, 569,
   507, 452, 402, 359, 320, 285, 254, 226,
   201, 179, 160, 142, 127, 113, 101, 90,
   80, 71, 63, 56, 50, 45, 40, 35,
   32, 28, 25, 22, 20, 17, 16, 14,
   12, 11, 10, 9, 8, 7, 6, 5,
   5, 4, 4, 3, 3, 2, 2, 2,
   2, 1, 1, 1, 1, 1, 1, 0,
   0, 0
};


/* DTMF low/high freq pair */
typedef struct DTMF_FREQ_PAIR
{
   VRG_UINT16 freqL; /* low freq tone  */
   VRG_UINT16 freqH; /* high freq tone */
} DTMF_FREQ_PAIR;

/* DTMF digit to freq pair map */
typedef struct DTMF_DIGIT2FREQ_MAP
{
   HAPI_PTE_DTMFDDGT digit;      /* dtmf digit */
   DTMF_FREQ_PAIR    freqs;      /* low/high freq pair for the digit */
} DTMF_DIGIT2FREQ_MAP;

/* Mapping from Endpoint DTMF digit events to freq pair */
static const DTMF_DIGIT2FREQ_MAP gDtmfDigit2FreqMap[] =
{
   { HAPI_PTE_DTMFDDGT0,    {941, 1336}},
   { HAPI_PTE_DTMFDDGT1,    {697, 1209}},
   { HAPI_PTE_DTMFDDGT2,    {697, 1336}},
   { HAPI_PTE_DTMFDDGT3,    {697, 1477}},
   { HAPI_PTE_DTMFDDGT4,    {770, 1209}},
   { HAPI_PTE_DTMFDDGT5,    {770, 1336}},
   { HAPI_PTE_DTMFDDGT6,    {770, 1477}},
   { HAPI_PTE_DTMFDDGT7,    {852, 1209}},
   { HAPI_PTE_DTMFDDGT8,    {852, 1336}},
   { HAPI_PTE_DTMFDDGT9,    {852, 1477}},
   { HAPI_PTE_DTMFDDGTSTR,  {941, 1209}},
   { HAPI_PTE_DTMFDDGTPND,  {941, 1477}},
   { HAPI_PTE_DTMFDDGTA,    {697, 1633}},
   { HAPI_PTE_DTMFDDGTB,    {770, 1633}},
   { HAPI_PTE_DTMFDDGTC,    {852, 1633}},
   { HAPI_PTE_DTMFDDGTD,    {941, 1633}},
   { HAPI_PTE_DTMFDDGTERROR,{0,   0   }}
};

/* ---- Private Function Prototypes -------------------------------------- */
static VRG_SINT32 FindMaxFrequencyPowerLevel(const HDSP_TONE_FREQUENCY_COMPONENT freq[]);
static VRG_UINT32 AmpAdjustment(VRG_SINT16 DeltadB);
static void hdspSetCustomToneDelay( HAPI_PTE_CP_PROFILE *profile, VRG_UINT16 delay, VRG_UINT16 type );

#if !HDSP_CFG_TONEPROV_IETF_SIG_MIB
static void leftShift( HDSP_TONE_PROV_PATTERN *tonePatternBits );
#endif   /* !HDSP_CFG_TONEPROV_IETF_SIG_MIB */

static VRG_SINT16 hdspSetTonePattern
(
   HAPI_PTE_CUSTOM_PACKET       *pTone,
   const HDSP_TONE_PROV_PATTERN *pProvTone,         /* Input tone pattern */
   int                           tone,             /* Index into the table of tones (stored on host) */
   int                           lineId,
   VRG_COUNTRY                   country,
   VRG_UINT32                    hdspToneDelay,
   VRG_UINT32                    hdspToneType,
   HDSP_TONE_DIRECTION           hdspToneDirection,
   VRG_BOOL                      hdspToneMediaOption
);

static int mapToneDirection( HDSP_TONE_DIRECTION direction, int *ingress, int *egress );


static HAPI_UINT16 hdspSetPowerLevel( HAPI_UINT16 dBmLevel, int ingress, int egress, int lineId );

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   hdspSetCustomToneDelay
**
** PURPOSE:    Replaces any placeholder values for callwaiting delays in the
**             tone profile with the provisioned value for the delay.
**
** PARAMETERS: profile     - custome tone profile
**             delay       - callwaiting delays from provisioning
**             type        - custom pulse type options to 'or' in with existing options
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
static void hdspSetCustomToneDelay( HAPI_PTE_CP_PROFILE *profile, VRG_UINT16 delay, VRG_UINT16 type )
{
   int   i, j;

   /* Replace any CallWtg Delay placeholders with the provisioned delay value */
   for ( i = 0; i < profile->numFreqGroups; i++ )
   {
      for ( j =0; j < HAPI_PTE_NUMCADENCES; j++ )
      {
         if ( profile->freqGrp[i].pulse[j].PulseTime == HDSP_DELAY_PLACEHOLDER )
         {
#if HDSP_TONEPROV_DEBUG
            VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "Or-ing pulse %d type (0x%x) with (0x%x)", i, profile->freqGrp[i].pulse[j].PulseType, type ));
            VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "Replacing pulse %d with delay %d", i, delay ));
#endif
            profile->freqGrp[i].pulse[j].PulseType |= type;
            profile->freqGrp[i].pulse[j].PulseTime = delay;
         }
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   hdspToneOn
**
** PURPOSE:    Generates a tone on the specified channel (dialtone, ringback ... )
**
** PARAMETERS: vhdHdl         (in)  VHD (channel) handle
**             tone           (in)  Index into the table of tones.
**             hdspToneDelay  (in)  Delay between call-waiting tones. Set to
**                                  0 to use default value.
**             hdspToneType   (in)  Custom pulse type options to 'or' in with existing options.
**                                  Needs to be of HAPI_PTE_CADBIT_xxx type.
**                                  (set to 0 unless you're absolutely sure you know what you're doing)
**             tonePatternBits(in)  Provisioned tone pattern bits, for non-default
**                                  ring and ringback profiles. Set to 0 to use
**                                  default profiles.
**             direction      (in) Signal generation direction: ingress/egress/both
**             mediaOption    (in) TRUE  to overwrite the media queue
**                                 FALSE to overlap the media queue
**             country        (in) Country code.
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspToneOn
(
   VRG_UINT16              vhdHdl,
   int                     lineId,
   int                     tone,
   VRG_UINT16              hdspToneDelay,
   VRG_UINT16              hdspToneType,
   HDSP_TONE_PROV_PATTERN *pProvTonePattern,
   HDSP_TONE_DIRECTION     direction,
   VRG_BOOL                mediaOption,
   VRG_COUNTRY             country
)
{
   HAPI_PTE_CUSTOM_PACKET  ptePacket;
   VRG_SINT16              rc;

   rc = hdspSetTonePattern( &ptePacket, pProvTonePattern, tone, lineId, country,
                            hdspToneDelay, hdspToneType, direction, mediaOption );

   if ( rc != HAPISUCCESS )
   {
      return ( rc );
   }

   rc = hdspSendCmdDataAsync(  vhdHdl,
                               HAPI_PTE_TONE_GEN_CMD,
                               HSCMDEXTDATA_ASYNC,
                               sizeof ( HAPI_PTE_CUSTOM_PACKET ),
                               &ptePacket );
   if ( rc != HAPISUCCESS )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "HDSP: Failed to send tone profile to DSP" ));
   }
   else
   {
      return ( rc );
   }

   return ( HAPIFAILURE );
}

/*
*****************************************************************************
** FUNCTION:   hdspToneOff
**
** PURPOSE:    Stops the tone generation on the specified channel
**
** PARAMETERS: vhdHdl     - the vhd (channel) handle
**             direction  - signal generation direction: ingress/egress/both
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
*****************************************************************************
*/
VRG_SINT16 hdspToneOff( VRG_UINT16 vhdHdl, HDSP_TONE_DIRECTION direction )
{
   VRG_SINT16   rc = HAPISUCCESS;
   int          ingress, egress;

   /* Currently, we do not support for multiple slots.  Thus, we will always
    * use slot # 0 for now. */
   VRG_UINT8    slotNum = 0;

   /* Set up the direction of the tone */
   rc = mapToneDirection( direction, &ingress, &egress );
   if ( rc )
   {
      return ( HAPIFAILURE );
   }

   if ( ingress )
   {
      rc = hdspSendCmdAsync(  vhdHdl,
                              HAPI_PTE_TONE_END_CMD,
                              HSCMDDATA,
                              1,
                              slotNum );

      if ( rc != HAPISUCCESS )
      {
         return ( HAPIFAILURE );
      }
   }

   if ( egress )
   {
      rc = hdspSendCmdAsync(  vhdHdl,
                              HAPI_PTE_TONE_END_CMD,
                              HSCMDDATA,
                              0,
                              slotNum );
   }

   return ( rc );
}



/****************************************************************************
*
*  hdspSetTonePattern - Set tone profile according to provisioned bit pattern
*
*****************************************************************************
*
*  PURPOSE:
*     Set tone profile vaccording to provisioned bit pattern
*
*  PARAMETERS:
*     tonePattern       - Tone pattern state machine to update
*     tonePatternBits   - Ptr to provisioned ring pattern bits (can be NULL)
*     tone              - Index into the table of tones (stored on the host)
*     lineId            - line id
*     country           - Country Code
*     hdspToneDelay     - custom tone delay to replace the delay placeholder with
*     hdspToneType      - custom pulse type options to 'or' in with existing options
*     hdspToneDirection - signal generation direction: ingress/egress/both
*     hdspToneMediaOption - TRUE  to overwrite the media queue
*                           FALSE to overlap the media queue
*
*  RETURNS:
*     Nothing
*
*  NOTE: Tones other than RINGBACK or INGRESS_RINGBACK are set
*        directly from the table of tones (customTone[][] array).
*
****************************************************************************/
static VRG_SINT16 hdspSetTonePattern
(
   HAPI_PTE_CUSTOM_PACKET       *pTone,
   const HDSP_TONE_PROV_PATTERN *pProvTone,         /* Input tone pattern */
   int                           tone,             /* Index into the table of tones (stored on host) */
   int                           lineId,
   VRG_COUNTRY                   country,
   VRG_UINT32                    hdspToneDelay,
   VRG_UINT32                    hdspToneType,
   HDSP_TONE_DIRECTION           hdspToneDirection,
   VRG_BOOL                      hdspToneMediaOption
)
{
   VRG_UINT16        i, j;
   int               pulseIdx;

   VRG_UINT16        freqMask[HAPI_PTE_NUMFREQUENCIES];
   VRG_UINT16        pulseTime[HAPI_PTE_NUMFREQUENCY_GRP];

   HAPI_PTE_PULSE   *pPulse;
   VRG_BOOL          bLastState = VRG_FALSE;
   VRG_BOOL          bLastStateOn = VRG_FALSE;
   VRG_BOOL          bGenerateEvent = VRG_FALSE;
   HAPI_PTE_FREQ_GRP *pFreqGrp = &pTone->profile.freqGrp[0];
   VRG_BOOL          bLastFreqGroup = VRG_FALSE;
   VRG_BOOL          bNeedSteadyState = VRG_FALSE;
   VRG_UINT8         numFrequency;
   VRG_UINT8         numStates;
   VRG_UINT16        maxFreqGroup;
   VRG_UINT16        numFreqGroup;
   VRG_SINT32        DeltadB = 0;
   VRG_SINT32        DbLevel;
   VRG_SINT16        rc;
   int               ingress, egress;
   /* Currently, we do not support for multiple slots.  Thus, we will always
    * use slot # 0 for now. */
   VRG_UINT8 slotNum = 0;

   (void)bLastStateOn;

   memset( &freqMask[0], 0, sizeof(VRG_UINT16)* HAPI_PTE_NUMFREQUENCIES );
   memset( &pulseTime[0], 0, sizeof(VRG_UINT16)* HAPI_PTE_NUMFREQUENCY_GRP );

   /* Set up tone direction */
   rc = mapToneDirection( hdspToneDirection, &ingress, &egress );
   if ( rc )
   {
      return ( HAPIFAILURE );
   }

   /* configure signal control */
   pTone->signalContrl = HAPI_PTE_MKSIGNALCTRL( ingress, egress, hdspToneMediaOption, slotNum );

   if ( pProvTone == NULL && (tone == CT_DTAS_ONHOOK || tone == CT_DTAS_OFFHOOK))
   {
#if VRG_COUNTRY_CFG_STATIC_PROFILE
      /* Early return. Obtain the CAS tone from the Cas Tone Profile */
      pTone->profile = gCASTones[tone].profile;

      /* Configure the power level for all frequency groups */
      for ( i = 0; i < pTone->profile.numFreqGroups; i++ )
      {
         pTone->profile.freqGrp[i].signalLevel =
               hdspSetPowerLevel( pTone->profile.freqGrp[i].signalLevel * 10, ingress, egress, lineId );
      }

      /* Replace any tone delay placeholder values with the desired delay */
      hdspSetCustomToneDelay( &pTone->profile, hdspToneDelay, hdspToneType );

#if HDSP_TONEPROV_DEBUG
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** ============= static tone profile definition: =================== "));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** Name = '%s'", gCASTones[tone].name ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** wholeRepeatCount = %i", pTone->profile.wholeToneRepeatCount ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numFreqGroups = %i", pTone->profile.numFreqGroups ));

      for ( i = 0; i < pTone->profile.numFreqGroups; i++ )
      {
         numFrequency = HAPI_PTE_GETFREQGRP_NUMFREQ(pTone->profile.freqGrp[i].freqGroupInfo);
         numStates = HAPI_PTE_GETFREQGRP_NUMSTATES(pTone->profile.freqGrp[i].freqGroupInfo);

         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** --- Freq Group #%i", i));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numFreq = %i", numFrequency ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freq1 = %i, freq2 = %i", pTone->profile.freqGrp[i].freq[0], pTone->profile.freqGrp[i].freq[1] ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freq3 = %i, freq4 = %i", pTone->profile.freqGrp[i].freq[2], pTone->profile.freqGrp[i].freq[3] ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** amp1 = %i, amp2 = %i", pTone->profile.freqGrp[i].amp[0], pTone->profile.freqGrp[i].amp[1] ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** amp3 = %i, amp4 = %i", pTone->profile.freqGrp[i].amp[2], pTone->profile.freqGrp[i].amp[3] ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freqGroup repeat count = %i", pTone->profile.freqGrp[i].freqGroupRepeatCount ));
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** power = %i", pTone->profile.freqGrp[i].signalLevel ));

         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numStates = %i", numStates ));
         for ( j = 0; j < numStates; j++ )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** pulse (%i) pulseType=%i, pulseNextState = %i, pulseFreqMask = %i, pulseTime = %i", j,
                       pTone->profile.freqGrp[i].pulse[j].PulseType,
                       pTone->profile.freqGrp[i].pulse[j].PulseNextState,
                       pTone->profile.freqGrp[i].pulse[j].PulseFreqMask,
                       pTone->profile.freqGrp[i].pulse[j].PulseTime ));
         }
      }
#endif

      return ( HAPISUCCESS );
#else
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Incorrectly provisioned tone '%d,%d'", country, tone ));

      return ( HAPIFAILURE );
#endif
   }


   /*
   ** Setup convenience arrays to be used during for-loop iteration
   */

   /* Set the 2 possible frequency masks */
   freqMask[0] = HAPI_PTE_CADBIT_FREQ1;
   freqMask[1] = HAPI_PTE_CADBIT_FREQ2;
   freqMask[2] = HAPI_PTE_CADBIT_FREQ3;
   freqMask[3] = HAPI_PTE_CADBIT_FREQ4;


   /* Check for a special condition for call-waiting */
   switch ( tone )
   {
      case CT_CALLW_SAS_CAS:
      case CT_DTAS_ONHOOK:
      case CT_DTAS_OFFHOOK:
      {
         /* For CAS tone or callwaiting tone (first tone before CAS)
          * we need to generate an event when complete for the CLID state machine */
         bGenerateEvent = VRG_TRUE;
      }

      default:
      {
      }
      break;
   }

   /*
   ** Set the custom tone info: name, power, profile
   */

   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "Provisioned tone '%d, %d'", country, tone ));

   /* Specify the whole tone repeat count */
   pTone->profile.wholeToneRepeatCount = pProvTone->repetitions;

   /* check if we need steady state */
   if ( !(pProvTone->noSteadyTone))
   {
      bNeedSteadyState = VRG_TRUE;
   }

   /* Determine the maximum number of frequency groups.  This number is constraint by
   ** 'HAPI_PTE_NUMFREQUENCY_GRP'.  Currently, we have it configured for 5 frequency groups.
   ** However, this value may vary depending on whether steady tone or call waiting delay
   ** is supported since we need to add extra frequency groups to account for these signal
   ** configurations.  Thus, we can support up to:
   **
   **   Signal Configuration             Max number of frequency groups
   **====================================================================
   **    no steady or call waiting        5
   **    steady or call waiting delay     4
   **    steady and call waiting delay    3
   */
   if ( ( hdspToneDelay > 0 ) && ( bNeedSteadyState == VRG_TRUE ) )
   {
      maxFreqGroup = HAPI_PTE_NUMFREQUENCY_GRP - 2;
   }
   else if ( ( hdspToneDelay > 0 ) || ( bNeedSteadyState == VRG_TRUE ) )
   {
      maxFreqGroup = HAPI_PTE_NUMFREQUENCY_GRP - 1;
   }
   else
   {
      maxFreqGroup = HAPI_PTE_NUMFREQUENCY_GRP;
   }

   /* Extract the number of frequency groups */
   numFreqGroup = pProvTone->tones;

  /* Validate the parameters */
   if (numFreqGroup > HAPI_PTE_NUMFREQUENCY_GRP)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "%s: Too many frequency components", __FUNCTION__));
      return 0;
   }


   /*
   ** Set the profile info for each frequency group: freqs, amps, freqGroupRepeatCount, nstates
   */
   for ( i = 0; ( i < numFreqGroup ) && ( i < maxFreqGroup ); i++ )
   {
      /* initialize all state variables */
      bLastState = VRG_FALSE;
      bLastStateOn = VRG_FALSE;
      pulseIdx = 0;

      /*
      ** Set flag if we are processing last frequnecy group
      */
      if  ( (i == ( numFreqGroup - 1 )) ||
            (i == ( maxFreqGroup - 1 )) )
      {
         bLastFreqGroup = VRG_TRUE;
      }

      /* Point to the current frequency group */
      pFreqGrp = &pTone->profile.freqGrp[i];

      /* setup the two pulse duration for easier access */
      pulseTime[0] = pProvTone->tone[i].soundDuration;
      pulseTime[1] = pProvTone->tone[i].silenceDuration;

      /* Clear remaining pulse time */
      for ( j = 2; j < HAPI_PTE_NUMFREQUENCY_GRP; j++ )
      {
         pulseTime[j] = 0;
      }


      /* Set up the DB Level */

      /* DBLevel - a number from -600 to +40 in units of TenthdBm (-13.5 dBm = -135)
       * power - specified as a positive number indicating a -dBm value.
       * Reference amplitude 22656 has been set so that 22656 amplitude gives 0 dBm power for the output.
       * The max dBlevel in a tone group will be applied as signal level in PTE.
       * If power level of each frequency in a group are not equal,
       * the amplitude of different frequency will be tuned down to match Max dBlevel
       * ie.
       * f1 power level = - 5 dBm         f2 power level = -6 dBm
       * dBlevel = max level = -5 dBm, amplitude for f1 = 22656
       * amplitude for f2 = 10^(-1dB/20)*22656 (tune down -1 dB so that -5 dB can be used for this frequency)*/

      if (pProvTone->tone[i].modulateTone == VRG_TRUE)
      {
         if(pProvTone->tone[i].modulatePercent <= 100)
         {
            DbLevel = pProvTone->tone[i].frequency[0].level;
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "Modulate percent value is invalid !!! "));
            return (HAPIFAILURE);
         }
      }
      else
      {
         DbLevel = FindMaxFrequencyPowerLevel(pProvTone->tone[i].frequency);
      }

      pFreqGrp->signalLevel = hdspSetPowerLevel( -DbLevel*10, ingress, egress, lineId );

      /*
      ** Setup frequency components
      */
      pFreqGrp->freq[0] = pProvTone->tone[i].frequency[0].hertz;
      pFreqGrp->freq[1] = pProvTone->tone[i].frequency[1].hertz;
      pFreqGrp->freq[2] = pProvTone->tone[i].frequency[2].hertz;
      pFreqGrp->freq[3] = pProvTone->tone[i].frequency[3].hertz;

      /* 4 valid frequencies for now (although they may be 0) */
      numFrequency = HAPI_PTE_NUMFREQUENCIES;


      /* Clear the amplitude for all frequencies to 0 */
      for ( j = 0; j < HAPI_PTE_NUMFREQUENCIES; j++ )
      {
         pFreqGrp->amp[j] = 0;
      }

      /*
      ** Setup the amplitudes for each valid frequency
      */
      if (pProvTone->tone[i].modulateTone == VRG_TRUE && pProvTone->tone[i].modulatePercent <= 100)
      {
         /* Specify the modulation index based on the modulation percentage
         ** The modulation index is specified by:
         **    32768 x modulation percentage
         */
         pFreqGrp->amp[0] = 22656;
         pFreqGrp->amp[1] = ( 32767 * pProvTone->tone[i].modulatePercent / 100 ) ;
      }
      else
      {
         int numActiveFreqs = 0;

         /* count the number of active frequencies.  An active frequency is defined to
          ** have a value bigger than 0 */
         for ( j = 0; j < 4; j++ )
         {
            if ( pProvTone->tone[i].frequency[j].hertz > 0 )
            {
               numActiveFreqs++;
            }
         }

         for ( j = 0; j < numActiveFreqs; j++ )
         {
            DeltadB = DbLevel - pProvTone->tone[i].frequency[j].level;
            pFreqGrp->amp[j] = AmpAdjustment(DeltadB);
         }
      }

      /*
      ** Set up frequency group repetition
      */
      pFreqGrp->freqGroupRepeatCount = pProvTone->tone[i].repetitions;

      /*
      ** Setup the number of states.  Currently, this value is hard-coded to
      ** 2 states to align with the MIB: 1 ON + 1 OFF
      */
      numStates = 2;

      /*
      ** Set the pulse info: Type, NextState, FreqMask, Time for on/off state pairs
      */
      for ( j = 0; ( j < numStates ) && !bLastState; j++, pulseIdx++ )
      {
         pPulse = &pFreqGrp->pulse[pulseIdx];

         /* The second to last state could be the last state if the final OFF
          * state has a 0 duration */
         if ( ( j  == ( numStates - 2 ) ) &&
              ( pulseTime[ j + 1 ] == 0 ) )
         {
            /* The next state (last OFF state) has 0 duration, so this is the
             * actual last state */
            bLastState = VRG_TRUE;
            bLastStateOn = VRG_TRUE;

            /* Remove the last OFF state */
            numStates--;
         }
         /* Final state */
         else if ( j  == ( numStates - 1 ) )
         {
            bLastState = VRG_TRUE;
         }

         /*
         ** Set the PulseType
         */
         if ( j % 2 )
         {
            /* Odd states are OFF (1,3,..) */
            pPulse->PulseType = HAPI_PTE_CADTYPE_PULSEOFF;
         }
         else
         {
            /* Even states are ON (0,2,..) */
            pPulse->PulseType = HAPI_PTE_CADTYPE_PULSEON;

            /* Set bit to modulate frequencies */
            if ( pProvTone->tone[i].modulateTone == VRG_TRUE && pProvTone->tone[i].modulatePercent <= 100 )
            {
               pPulse->PulseType |= HAPI_PTE_CADBIT_MODULATE;
            }
         }

         /* Additional PulseType settings will be set if this is the last state... */

         /*
         ** Set the NextState
         */
         if ( !bLastState )
         {
            /* Proceed to next state */
            pPulse->PulseNextState = HAPI_PTE_CADBITS_NEXTSTATE1 + pulseIdx;
         }
         else
         {
            /* Last state - pulse completed */
            pPulse->PulseNextState = 0;

            /* Check if we need to repeat the frequency group */
            if ( pFreqGrp->freqGroupRepeatCount > 0 )
            {
               /* We are repeating, so the next state is state 0 */
               pPulse->PulseNextState = HAPI_PTE_CADBITS_NEXTSTATE0;

               /* Last state, using loop count - indicate loopcount in use */
               pPulse->PulseType |= HAPI_PTE_CADBIT_LOOPCOUNT;
            }

            /* check if we are generating steady tones */
            if ( bLastFreqGroup )
            {
               /* If final state is on and we are generating steady tone, then
               ** we will make the final state a steady on.  Note that the DSP
               ** will only apply the steady state on after all the whole
               ** tone repetitions have been completed.
               ** */
               if ( ( numStates % 2 ) && ( bNeedSteadyState ) )
               {
                  /* change the final tone to be on forever */
                  pPulse->PulseType |= HAPI_PTE_CADBIT_NOSTATECHANGE;
                  pPulse->PulseTime = 0;
                  bNeedSteadyState = VRG_FALSE;
               }
            }
         }  /* Last state */


         /*
         ** Set the FreqMask
         */
         if ( pProvTone->tone[i].modulateTone == VRG_TRUE && pProvTone->tone[i].modulatePercent <= 100 )
         {
            /* Modulate freq1 by freq2 */
            pPulse->PulseFreqMask = freqMask[0] | freqMask[1];
         }
         else
         {
            /* freq1 summed with freq2 */
            pPulse->PulseFreqMask = freqMask[0] | freqMask[1] | freqMask[2] | freqMask[3];
         }

         /*
         ** Set the pulse time
         */
         pPulse->PulseTime = pulseTime[j];

      } /* End for numStates */

      /* Configure frequency group information for the specified number of frequencies and states */
      pFreqGrp->freqGroupInfo = HAPI_PTE_MKFREQGRP_INFO( numFrequency, numStates );
   } /* End for all freq groups */

   /* Save the number of frequency groups in the tone */
   pTone->profile.numFreqGroups = i;

   if ( bNeedSteadyState )
   {
      HAPI_PTE_FREQ_GRP    *pNewFreqGrp;
      HAPI_PTE_PULSE       *pNewPulse;

      /* If we still need to set steady state, then the profile ended with an OFF
       * state - repeat the last ON frequency in the final frequency group */

      /* Set up pointers to the final frequency group */
      pFreqGrp = &pTone->profile.freqGrp[pTone->profile.numFreqGroups - 1];
      numStates = HAPI_PTE_GETFREQGRP_NUMSTATES( pFreqGrp->freqGroupInfo );
      numFrequency = HAPI_PTE_GETFREQGRP_NUMFREQ( pFreqGrp->freqGroupInfo );

      if ( numStates % 2 )
      {
         pulseIdx = numStates - 1;
      }
      else
      {
         pulseIdx = numStates - 2;
      }
      pPulse = &pFreqGrp->pulse[ pulseIdx ];

      /* Now, add a NEW state to account for the steady state tone.  This new
      ** state contains only 1 tone */
      pNewFreqGrp = &pTone->profile.freqGrp[pTone->profile.numFreqGroups];
      pNewPulse = &pNewFreqGrp->pulse[0];

      memcpy( &pNewFreqGrp->freq[0], &pFreqGrp->freq[0], sizeof(HAPI_UINT16)*HAPI_PTE_NUMFREQUENCIES );
      memcpy( &pNewFreqGrp->amp[0], &pFreqGrp->amp[0], sizeof(HAPI_UINT16)*HAPI_PTE_NUMFREQUENCIES );

      /* Configure to have the same number of frequencies as the last frequency group and 1 state */
      pNewFreqGrp->freqGroupInfo = HAPI_PTE_MKFREQGRP_INFO( numFrequency, 1 );

      pNewFreqGrp->freqGroupRepeatCount = 0;
      pNewPulse->PulseFreqMask = pPulse->PulseFreqMask;

      /* The DSP will only apply the steady state on after all the whole
      ** tone repetitions have been completed. */
      pNewPulse->PulseType = HAPI_PTE_CADTYPE_PULSEON | HAPI_PTE_CADBIT_NOSTATECHANGE;

      /* Set bit to modulate all frequencies */
      if ( pProvTone->tone[ pTone->profile.numFreqGroups - 1 ].modulateTone == VRG_TRUE && pProvTone->tone[i].modulatePercent <= 100)
      {
         pNewPulse->PulseType |= HAPI_PTE_CADBIT_MODULATE;
      }

      pNewPulse->PulseTime = 0;
      pNewPulse->PulseNextState = 0;

      pNewFreqGrp->signalLevel = pFreqGrp->signalLevel;

      /* Account for this new frequency group */
      pTone->profile.numFreqGroups++;
   }
   else if ( hdspToneDelay > 0 )
   {
      HAPI_PTE_FREQ_GRP    *pNewFreqGrp;
      HAPI_PTE_PULSE       *pNewPulse;
      HAPI_PTE_PULSE       *pLastPulse;

      if ( tone == CT_DTAS_OFFHOOK )
      {
         /* CAS tone used during call-waiting - need a timeout state for the
         ** DTMF ack timeout period */

         /* Set up pointers to the final frequency group */
         pFreqGrp = &pTone->profile.freqGrp[pTone->profile.numFreqGroups - 1];
         numStates = HAPI_PTE_GETFREQGRP_NUMSTATES( pFreqGrp->freqGroupInfo );
         numFrequency = HAPI_PTE_GETFREQGRP_NUMFREQ( pFreqGrp->freqGroupInfo );

         pLastPulse = &pFreqGrp->pulse[ numStates - 1 ];

         if ( numStates % 2 )
         {
            /* Final pulse is on - this will only happen if the final off
            ** state was stripped out due to a null duration.  Create a new
            ** off state */
            pPulse = &pFreqGrp->pulse[ numStates ];

            /* Change last state to iterate to the new state */
            pLastPulse->PulseNextState++;

            /* Increment the number of states */
            pFreqGrp->freqGroupInfo = HAPI_PTE_MKFREQGRP_INFO( numFrequency, numStates + 1 );
         }
         else
         {
            /* Final pulse is off - modify the DTMF delay of the last state */
            pPulse = pLastPulse;
         }

         /* Modify tone to account for DTMF delay */
         pPulse->PulseTime = HDSP_DTMF_ACK_DELAY;
         pPulse->PulseType = HAPI_PTE_CADTYPE_PULSEOFF | HAPI_PTE_CADBIT_EVENT;
         pPulse->PulseNextState = HAPI_PTE_CADBITS_NEXTSTATE0;
         pPulse->PulseFreqMask = 0;
      }

      /* Add a new frequency group to account for the callwaiting
      ** inter-tone delay */
      pNewFreqGrp = &pTone->profile.freqGrp[pTone->profile.numFreqGroups];
      pNewPulse = &pNewFreqGrp->pulse[0];

      memset( pNewFreqGrp, 0, sizeof( HAPI_PTE_FREQ_GRP ) );

      /* Configure for 0 frequencies and 1 state */
      pNewFreqGrp->freqGroupInfo = HAPI_PTE_MKFREQGRP_INFO( 0,  1 );
      pNewPulse->PulseType = HAPI_PTE_CADTYPE_PULSEOFF;
      pNewPulse->PulseTime = hdspToneDelay;
      pNewPulse->PulseNextState = HAPI_PTE_CADBITS_NEXTSTATE0;
      pNewPulse->PulseFreqMask = 0;

      if ( bGenerateEvent )
      {
         pNewPulse->PulseType |= ( HAPI_PTE_CADBIT_EVENT | hdspToneType );
      }

      /* Account for this new frequency group */
      pTone->profile.numFreqGroups++;
   }

#if HDSP_TONEPROV_DEBUG
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** ============= custom tone profile definition: =================== "));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** wholeRepeatCount = %i", pTone->profile.wholeToneRepeatCount ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numFreqGroups = %i", pTone->profile.numFreqGroups ));

   for ( i = 0; i < pTone->profile.numFreqGroups; i++ )
   {
      numFrequency = HAPI_PTE_GETFREQGRP_NUMFREQ(pTone->profile.freqGrp[i].freqGroupInfo);
      numStates = HAPI_PTE_GETFREQGRP_NUMSTATES(pTone->profile.freqGrp[i].freqGroupInfo);

      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** --- Freq Group #%i", i));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numFreq = %i", numFrequency ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freq1 = %i, freq2 = %i", pTone->profile.freqGrp[i].freq[0], pTone->profile.freqGrp[i].freq[1] ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freq3 = %i, freq4 = %i", pTone->profile.freqGrp[i].freq[2], pTone->profile.freqGrp[i].freq[3] ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** amp1 = %i, amp2 = %i", pTone->profile.freqGrp[i].amp[0], pTone->profile.freqGrp[i].amp[1] ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** amp3 = %i, amp4 = %i", pTone->profile.freqGrp[i].amp[2], pTone->profile.freqGrp[i].amp[3] ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** freqGroup repeat count = %i", pTone->profile.freqGrp[i].freqGroupRepeatCount ));
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** power = %i", pTone->profile.freqGrp[i].signalLevel ));

      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** numStates = %i", numStates ));
      for ( j = 0; j < numStates; j++ )
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "** pulse (%i) pulseType=%i, pulseNextState = %i, pulseFreqMask = %i, pulseTime = %i", j,
                    pTone->profile.freqGrp[i].pulse[j].PulseType,
                    pTone->profile.freqGrp[i].pulse[j].PulseNextState,
                    pTone->profile.freqGrp[i].pulse[j].PulseFreqMask,
                    pTone->profile.freqGrp[i].pulse[j].PulseTime ));
      }
   }
#endif

   return ( HAPISUCCESS );

}  /* End hdspSetTonePattern() */



/*
*****************************************************************************
** FUNCTION:   hdspDigitOn
**
** PURPOSE:    Generate a DTMF digit
**
** PARAMETERS: vhdHdl    - the vhd handle
**             digit     - the digit to generate
**             dbLevel   - dbLevel to generate the digit at
**             skew     - separation (in dB) of frequencies
**             pulseTime - digit ON/OFF pulse width
**             direction - signal generation direction: ingress or egress
**                         (can not do simultaneous)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspDigitOn
(
   VRG_UINT16           vhdHdl,
   int                  lineId,
   HAPI_PTE_DTMFDDGT    digit,
   VRG_UINT16           dbLevel,
   VRG_UINT16           skew,
   VRG_UINT16           pulseTime,
   HDSP_TONE_DIRECTION  direction
)
{
   HAPI_PTE_CUSTOM_PACKET     ptePacket;
   HDSP_CUSTOM_TONE           customTone;
   const DTMF_DIGIT2FREQ_MAP  *digit2freqp = gDtmfDigit2FreqMap;
   int                        ingress, egress;
   VRG_BOOL                   mediaOption = VRG_TRUE; /* TRUE to overwrite the media queue.
                                                      ** This is to ensure that the tone is clean
                                                      ** if there is a tone detector on the other end */

   /* Get the generic DTMF tone template */
   customTone = dtmfToneGeneric;

   /* Set up tone direction */
   if ( mapToneDirection( direction, &ingress, &egress ) )
   {
      return ( HAPIFAILURE );
   }

   /* Do not support simultaneous egress & ingress DTMF generation */
   if (ingress && egress)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "HDSP digit direction ERROR: Two way DTMF geration not supported !!! "));
      return ( HAPIFAILURE );
   }

   /* Now configure the actual signal */

   /* Modify the default on/off time to the specified on/off time */
   customTone.profile.freqGrp[0].pulse[0].PulseTime = pulseTime; /* on time */
   customTone.profile.freqGrp[0].pulse[1].PulseTime = pulseTime; /* off time */

   /* Find the dtmf tone freq */
   while ( (digit != digit2freqp->digit) &&
           (HAPI_PTE_DTMFDDGTERROR != digit2freqp->digit ))
   {
      digit2freqp++;
   }

   if (digit2freqp->digit == HAPI_PTE_DTMFDDGTERROR)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "HDSP digit ERROR: Digit %d not found in the freq table !!! ", digit));
      return ( HAPIFAILURE );
   }

   /* Set the dtmf tone frequencies */
   /* Modify the default number of frequencies to the specified number of frequencies */
   customTone.profile.freqGrp[0].freq[0] = digit2freqp->freqs.freqL;
   customTone.profile.freqGrp[0].freq[1] = digit2freqp->freqs.freqH;

   /* we only have 1 frequency group for DTMF digits */
   customTone.profile.freqGrp[0].signalLevel = hdspSetPowerLevel( dbLevel * 10, ingress, egress, lineId );
   if ((skew >= MIN_DTMF_SKEW) && (skew <= MAX_DTMF_SKEW))
   {
      customTone.profile.freqGrp[0].amp[0] = dtmfSkewMap[skew - MIN_DTMF_SKEW];
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "DTMF Skew %d out-of-range, using default", skew ));
   }

   ptePacket.signalContrl = HAPI_PTE_MKSIGNALCTRL( ingress, egress, mediaOption, 0 );
   ptePacket.profile = customTone.profile;

   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: Digit tone '%d', direction '%d' ", digit, direction ));

   return( hdspSendCmdDataAsync(  vhdHdl,
                                  HAPI_PTE_TONE_GEN_CMD,
                                  HSCMDEXTDATA_ASYNC,
                                  sizeof ( HAPI_PTE_CUSTOM_PACKET ),
                                  &ptePacket ) );
}


/*
*****************************************************************************
** FUNCTION:   hdspDigitOff
**
** PURPOSE:    Stop DTMF generation on an endpoint
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspDigitOff( VRG_UINT16 vhdHdl )
{
   return ( hdspToneOff( vhdHdl, EGRESS_TONE ));
}



/*
*****************************************************************************
** FUNCTION:   mapToneDirection
**
** PURPOSE:    Update tone direction variables based on the specified tone
**             generation direction
**
** PARAMETERS: direction (I) - desired tone generation direction
**             ingress   (O) - set to 1 to generate ingress tones
**             egress    (O) - set to 1 to generate egress tones
**
** RETURNS:    0 (valid direction), 1 (invalid direction)
**
** NOTE:
**
*****************************************************************************
*/
static int mapToneDirection( HDSP_TONE_DIRECTION direction, int *ingress, int *egress )
{
   switch ( direction )
   {
      case EGRESS_TONE:
      {
         *egress = 1;
         *ingress = 0;
      }
      break;

      case INGRESS_TONE:
      {
         *egress = 0;
         *ingress = 1;
      }
      break;

      case INGRESS_AND_EGRESS_TONE:
      {
         *egress = 1;
         *ingress = 1;
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, "ERROR: hdspToneOn - invalid signal generation direction: %d", direction ));
         return ( 1 );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   hdspToneAdjustEgressPowerLevel
**
** PURPOSE:    Adjust the tone profile power level to account for
**             APM DLP setting. Both provisionable and static tone profiles
**             have the tone dB level set to the actual level the tone
**             should come out the analong interface (at the tip&ring of
**             the phone). However, because the analog interace is configured
**             to meet certain nominal input/output power levels, the actual
**             level that the tone would come out would be offset for the
**             outpul level analog setting (DLP). Therefore, before
**             generating the tone profile, we must adjust the tone profile
**             to account for DLP so it comes out at the correct level.
**
** PARAMETERS: powerLevel - power level to adjust (pos. value in tens of dBm)
**             lineId - line id
**
** RETURNS:    Adjusted power level (pos. value in tens of dBm).
**
** NOTE:
**
*****************************************************************************
*/
VRG_UINT16 hdspToneAdjustEgressPowerLevel( VRG_UINT16 powerLevel, int lineId )
{
   XDRV_APM  *apmDriver;
   XDRV_SLIC  *slicDriver;
   VRG_SINT16 dlp = 0;
   VRG_SINT16 newPowerLevel;
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap;

   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "AdjustEgressPowerLevel()" ));

   if ( !boardHalIsSlic(lineId) ) /* non SLIC/SLAC channel */
   {
      return powerLevel;
   }
   else   /* SLIC/SLAC channel */
   {
      pDeviceChannelMap = boardHalProvGetChannelMap( lineId );
      if( !pDeviceChannelMap )
      {
        VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR: Invalid channel map pointer! Egress dlp adjust ignored."));
        return powerLevel;
      }
      
      switch ( pDeviceChannelMap->deviceType )
      {
         case BP_VD_ZARLINK_9530:  /* APM SLIC */
         case BP_VD_ZARLINK_9540:
         case BP_VD_ZARLINK_9541:
         case BP_VD_SILABS_3239:
         case BP_VD_SILABS_32392:
         {
           /*
            ** Use DSP 0 to get the power level information.  This is okay as long as we
            ** assume all DSP have equal capabilities, functionalities, etc.  Which is true
            ** for now.
            */
           apmDriver = boardHalApmGetDriver( 0 );
           if (apmDriver != NULL) 
           {
             dlp = xdrvApmGetDlp( apmDriver );
           }
           else
           {
             VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR: APM driver not initialized: Egress tone adjust ignored!"));
           }
         }
         break;
         default: /* PCM SLAC */
         {
            slicDriver = boardHalSlicGetDriver( lineId );
            if (slicDriver != NULL)
            {
               dlp = xdrvSlicGetDlp( slicDriver );
            }
            else
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_HDSP, "ERROR: PCM SLIC driver not initialized: Egress tone adjust ignored!"));
            }
         }
         break;
      }
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "line:%d dlp:%d", lineId, dlp ));
      /*
      ** The power level specified in is a positive number that represents ten times
      ** the negative dB level at which the tone should come out on
      ** the phone line (ie. 135 means generate a tone at -13.5dB level).
      ** The APM DLP setting is a signed number that represents a gain that should
      ** be applied to all signals coming out on the phone line (ie. dlp=-3dB
      ** means attenuate all signals by 3dB).
      **
      ** We can only go as high as 0dB power level via the tone profiles.
      ** Therefore, we need to add the DLP to the tone profile power level
      ** and cap it to 0dB.
      **
      ** After doing this, all the tones whose power level is provisioned or
      ** statically set below the DLP level would come out at the right level.
      ** The tones that are set higher than the DLP (ie. you wanna generate a
      ** 0dB tone with -3dB DLP) would be capped at the DLP level (because
      ** although the tone profile will be set to 0dB, the analog interface
      ** would attenuate this tone for DLP=-3dB).
      */
      newPowerLevel = powerLevel + (dlp*10);
      if (newPowerLevel < 0) 
      {
         newPowerLevel = 0;
      }

#if HDSP_TONEPROV_DEBUG
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "hdspTone - adjusting tone profile power level: OLDPOW=%d; DLP=%d; NEWPOW = %d", (powerLevel/10), dlp, (newPowerLevel/10)));
#endif

      return ( newPowerLevel );
   }

}


#if HDSP_CFG_CMTD_ENABLED

/*
*****************************************************************************
** FUNCTION:   hdspCmtdDBRInit
**
** PURPOSE:    Initialize Call Monitor Tone Detection Service
**
** PARAMETERS: vhdHdl - the vhd handle
**             country - the Country code
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspCmtdDBRInit( VRG_UINT16 vhdHdl, VRG_COUNTRY country )
{
   VRG_SINT16 result = -1;

   /*
   ** Disable CMTD service before starting.
   */
   hdspSendCmd( vhdHdl,
                HAPI_CMTD_ENABLE_CMD,
                HSCMDDATA,
                (HAPI_CMTD_ENABLE_OP1_DIS_FILTERS_BIT | HAPI_CMTD_ENABLE_OP1_DIS_STMS_BIT | HAPI_CMTD_ENABLE_OP1_DIS_SRV_BIT),
                (1 | (1 << HAPI_CMTD_ENABLE_OP2_STM_BITSHIFT)) );

   switch (country)
   {
#if VRG_COUNTRY_CFG_JAPAN
      case  VRG_COUNTRY_JAPAN:
      {
         HAPI_CMTD_FILTER cmtdFilter;
         HAPI_CMTD_STM cmtdStm;
         VRG_UINT32 filtCoefNum;

         /*
         ** Initalize toneIndex 0 for 400 Hz tone detection
         */
         cmtdFilter.index = 0;
         cmtdFilter.coefs.type = ARRAY_LEN(cmtdFilter.coefs.filterTaps);
         cmtdFilter.coefs.minAttenuation = 65536/4;   /* power after filter must be at least 6dB lower */
         for (filtCoefNum = 0; filtCoefNum < ARRAY_LEN(cmtdFilter.coefs.filterTaps); filtCoefNum++)
         {
            /* Narrow filter just for 400 +/- 20Hz signal */
            cmtdFilter.coefs.filterTaps[filtCoefNum] = cmdt400mod20coeffs[filtCoefNum];
         }
         result = hdspSendCmdDataAsync( vhdHdl,
                                        HAPI_CMTD_SETFILTER_CMD,
                                        HSCMDEXTDATA_ASYNC,
                                        sizeof(cmtdFilter),
                                        &cmtdFilter );
         if (result != HAPISUCCESS)
            return HAPIFAILURE;


         /*
         ** Initalize stmIndex 0 for Dial Tone
         */
         memset(&cmtdStm, 0, sizeof(cmtdStm));
         /* Japanese Dial Tone (DT) */
         cmtdStm.index = 0;
         cmtdStm.stmParams.substate[0].activeFilter = 1;
         cmtdStm.stmParams.substate[0].minTimeMs = 100;
         cmtdStm.stmParams.substate[0].maxTimeMs = 0xffff;

         result = hdspSendCmdDataAsync( vhdHdl,
                                        HAPI_CMTD_SETSTM_CMD,
                                        HSCMDEXTDATA_ASYNC,
                                        sizeof(cmtdStm),
                                        &cmtdStm ) ;
         if (result != HAPISUCCESS)
            return HAPIFAILURE;

         /*
         ** Enable CMTD filters up to and including filter #0,
         ** STMS up to and including STM #0, and service
         */

         result = hdspSendCmd( vhdHdl,
                               HAPI_CMTD_ENABLE_CMD,
                               HSCMDDATA,
                               (HAPI_CMTD_ENABLE_OP1_ENA_FILTERS_BIT | HAPI_CMTD_ENABLE_OP1_ENA_STMS_BIT | HAPI_CMTD_ENABLE_OP1_ENA_SRV_BIT),
                               ( 0 | (0 << HAPI_CMTD_ENABLE_OP2_STM_BITSHIFT) )   );

      VRG_LOG_NOTICE(( VRG_LOG_MOD_HDSP, "CMDT: Enabled detection " ));

      }
      break;
#endif /* VRG_COUNTRY_CFG_JAPAN */

#if VRG_COUNTRY_CFG_NORTH_AMERICA
      case VRG_COUNTRY_NORTH_AMERICA:
#endif /* VRG_COUNTRY_CFG_NORTH_AMERICA */
      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_HDSP, " hdspCmtdDBRInit unsupported country"));
         result = HAPIFAILURE;
      }
      break;
   }

   return( result );
}
#endif

/*
*****************************************************************************
** FUNCTION:   hdspSetPowerLevel
**
** PURPOSE:    Set the actual power level for PTE generation
**
** PARAMETERS: dBmLevel   (in) - pointer to the desired power level
**             ingress    (in) - identifies if this is an ingress tone
**             egress     (in) - identifies if this is an egress tone
**             lineId     (in) - line id
**
** RETURNS:    new power level setting
**
** NOTE:
*****************************************************************************
*/
static HAPI_UINT16 hdspSetPowerLevel( HAPI_UINT16 dBmLevel, int ingress, int egress, int lineId )
{
   /* Default tone power is -10 dB */
   VRG_UINT16        ingressPower = 10;
   VRG_UINT16        egressPower = 10;

   /* Set power level in dB */
   if ( ingress )
   {
      ingressPower = dBmLevel / 10;
   }
   if ( egress )
   {
      VRG_LOG_NOTICE(( VRG_LOG_MOD_HDSP, "Adjust Egress Power Level" ));
      egressPower = hdspToneAdjustEgressPowerLevel( dBmLevel, lineId ) / 10;
   }

   /* compose the signal level / control */
   return ( HAPI_PTE_MKSIGNALLEVEL( egressPower, ingressPower ) );
}

/*****************************************************************************
*  FUNCTION:   FindMaxFrequencyPowerLevel
*
*  PURPOSE:    Find out the maximum power level in a frequency group
*
*  PARAMETERS: freq - group of frequencies
*
*  RETURNS:    Maximum Power Level in decibels
*
*  NOTE:
*
*****************************************************************************/
static VRG_SINT32 FindMaxFrequencyPowerLevel(const HDSP_TONE_FREQUENCY_COMPONENT freq[])
{
   VRG_SINT32 MAX = freq[0].level;
   VRG_UINT16 idx;
   for (idx = 0; idx < MAX_FREQUENCY_COMPONENTS; ++idx)
   {
      if (freq[idx].level > MAX && freq[idx].hertz != 0)
      {
         MAX = freq[idx].level;
      }
   }
   return(MAX);
}

/*****************************************************************************
*  FUNCTION:   AmpAdjustment
*
*  PURPOSE:    Tune down the amplitude 22656 based on dB value
*
*  PARAMETERS: freq - group of frequencies
*
*  RETURNS:    Average Power Level in decibels
*
*  NOTE:
*
*****************************************************************************/

static VRG_UINT32 AmpAdjustment(VRG_SINT16 DeltadB)
{
   if (DeltadB > (VRG_SINT16)(sizeof(negFactor)/sizeof(VRG_UINT16)))
   {
      /* Guaranteed underflow */
      return(0);
   }
   else if (DeltadB == 0)
   {
      return(22656);
   }
   else
   {
      /* DeltadB > 1.0 */
      DeltadB = DeltadB - 1;

      return((VRG_SINT32)(negFactor[DeltadB]));
   }
}
