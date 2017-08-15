/****************************************************************************
*
* Copyright (c) 2000-2012 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
*****************************************************************************
*
*    Filename: tpd6838L9540.c
*    Creation Date: 04 Oct 2000 (v0.00)
*    VSS Info:
*        $Revision: 44 $
*        $Date: 2/21/03 6:56p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the L9540
*      SLIC on the 6838 reference design
*
****************************************************************************/

#include <bosTask.h>
#include <bosSleep.h>
#include <bcm6838.h>
#include <apm6838.h>
#include <hvg6838.h>
#include <xdrvLog.h>
#include <xdrvTimer.h>
#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <xdrvTpd.h>
#include <xdrvGlobalInterrupt.h>
#include <casCtl.h>
#include "tpd6838L9540.h"
#include "slic6838L9540.h"
#include <boardHalSlic.h>
#include <tpd6838L9540Cfg.h>

typedef long long int XDRV_SINT64;
typedef unsigned long long int XDRV_UINT64;

/* ---- Local Type Definitions ------------------------------- */

typedef enum TPD_BATTERY_MODE
{
   TPD_BATTERY_MODE_LOW,
   TPD_BATTERY_MODE_MED,
   TPD_BATTERY_MODE_HIGH,
   TPD_BATTERY_MODE_TEST1,
   TPD_BATTERY_MODE_TEST2

} TPD_BATTERY_MODE;

typedef enum TPD_MEASURE_TYPE
{
   TPD_MEASURE_DC,                    /* Measure DC signal only */
   TPD_MEASURE_DC_CHOP,               /* Measure DC signal only */
   TPD_MEASURE_ACDC,                  /* Measure AC and DC signal */
   TPD_MEASURE_NONE

} TPD_MEASURE_TYPE;

typedef struct TPD_MEASURE_RESULTS
{
   XDRV_SINT32 tipVoltAC;                  /* Measured AC tip voltage */
   XDRV_SINT32 tipVoltDC;                  /* Measured DC tip voltage */
   XDRV_SINT32 ringVoltAC;                 /* Measured AC ring voltage */
   XDRV_SINT32 ringVoltDC;                 /* Measured DC ring voltage */
   XDRV_SINT32 tipRingVoltAC;              /* Measured AC tip to ring voltage */
   XDRV_SINT32 tipRingVoltDC;              /* Measured DC tip to ring voltage */
   XDRV_SINT32 hvgVoltAC;                  /* Measured AC HVG voltage */
   XDRV_SINT32 hvgVoltDC;                  /* Measured DC HVG voltage */
   XDRV_SINT32 iLoopAC;                    /* Measured AC loop current */
   XDRV_SINT32 iLoopDC;                    /* Measured DC loop current */
   XDRV_UINT32 iLoopACRaw;                 /* Raw AC loop measurement */
   XDRV_UINT32 iLoopDCRaw;                 /* Raw DC loop measurement */

} TPD_MEASURE_RESULTS;

typedef struct TPD_CALIBRATION_RESULTS
{
   XDRV_SINT32 tipVoltCalibrationDC;            /* Measured DC tip Calibration voltage */
   XDRV_SINT32 ringVoltCalibrationDC;           /* Measured DC ring Calibration voltage */
}TPD_CALIBRATION_RESULTS;

/* ---- Private Constants ------------------------------------ */

/* HP/FEF test threshholds */
#define TPD_MIN_HIRES_VOLTAGE           -60000
#define TPD_MAX_HIRES_VOLTAGE             1500

/* Number of valid channels for testing */
#define TPD_MAX_ENDPT                        4

/* TPD Test Defines */
#define TPD_SAMPLES_CURRENT_PRETEST        200
#define TPD_SAMPLES_FEW                    200
#define TPD_SAMPLES_MANY                  2000
#define TPD_SAMPLE_TIME_SLOW              1000
#define TPD_SAMPLE_TIME_FAST               100
#define TPD_RESOLUTION                    1000
#define V_SETTLE                           100
#define I_SETTLE                             3
#define SETTLE_WAIT_MAX                     12

#define BMU6838_MAX_CURRENT              32000
#define BMU6838_MIN_CURRENT             -32000

#define CURRENT_SAMPLING_COEFFICIENT         2237
/* TPD Driver Pointers */
static XDRV_SLIC *pSlicDriver[TPD_MAX_ENDPT];
static XDRV_APM *pApmDriver[TPD_MAX_ENDPT];
static XDRV_DEBUG *pDebugDriver[TPD_MAX_ENDPT];

static XDRV_SINT32 tipVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static XDRV_SINT32 ringVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static XDRV_SINT32 hvgVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static TPD_CALIBRATION_RESULTS tpdVoltCalibrationDCOffset[APM6838_NUM_CHANNELS] = 
{
   /* tip,   ring */
  {  0,      0 },
  {  0,      0 }
};

/*
*****************************************************************************
** MACROS
*****************************************************************************
*/
#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

/* ---- Private Function Prototypes -------------------------- */


/* SLIC "meta-driver" interface functions */
static XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp );
static XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp );
static XDRV_TPD_STATUS tpdTestingSetup( int chan );
static XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled );
static XDRV_TPD_STATUS tpdChanReset( int chan );

/* SLIC "meta-driver" interface */
static const XDRV_TPD_FUNCS tpd6838L9540DrvFuncs =
{
  tpdRunDropTests,
  tpdRunSelfTests,
  tpdTestingSetup,
  tpdTestingShutdown,
  tpdChanReset
};

/* Private Functions */
XDRV_UINT16 linetestDropHazardForeign(int chan, HSZDROPRSLT_HPFEF *hpfefRsltp);
XDRV_UINT16 linetestDropROH(int chan, HSZDROPRSLT_ROH *rohRsltp);
XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp);
XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp);
XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp);
XDRV_UINT16 linetestDropOffhook(int chan);

XDRV_UINT16 linetestSelfAssert(int chan, XDRV_SINT32 *selfAssertPassed);
XDRV_UINT16 linetestSelfDspCount(int chan, XDRV_SINT32 *selfDspCountPassed);
XDRV_UINT16 linetestSelfHvg(int chan, XDRV_SINT32 *selfHvgPassed);
XDRV_UINT16 linetestSelfApm(int chan, XDRV_SINT32 *selfApmPassed);
XDRV_UINT16 linetestSelfCap(int chan, XDRV_SINT32 *selfCap);
XDRV_UINT16 linetestSelfRing(int chan, XDRV_SINT32 *selfRing);
XDRV_UINT16 linetestSelfFeed(int chan, XDRV_SINT32 *voltDc, XDRV_SINT32 *currentDc);

XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL lowRes );
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL voltage, XDRV_BOOL lowRes );

XDRV_SINT32 tpdExtend21to32( XDRV_UINT32 value );

static void tpdBatteryControl( int chan, TPD_BATTERY_MODE mode );
static void tpdDischargeVoltage( int chan, TPD_MEASURE_RESULTS *tpdResults );

void tpdEnableYFilter( int chan );
void tpdDisableYFilter( int chan );

void tpdCasEnable( int chan );
void tpdCasDisable( int chan );

XDRV_UINT64 vrgISqrt(XDRV_UINT64 x);

/*
*****************************************************************************
** FUNCTION:   tpdExtend21to32
**
** PURPOSE:    Converts a 21 bit signed integer stored as an unsigned,
**             32 bit integer to a 32 bit signed integer
**
** PARAMETERS: value - value which to convert to 32 bit signed integer
**
** RETURNS:    32 bit signed integer
** 
** NOTE:
*****************************************************************************
*/
XDRV_SINT32 tpdExtend21to32( XDRV_UINT32 value )
{
   return (XDRV_SINT32)( value & 0x00100000 ? (XDRV_UINT32)value | 0xFFE00000 : (XDRV_UINT32)value );
}

/*
*****************************************************************************
** FUNCTION:   vrgISqrt
**
** PURPOSE:    Estimate Integer Square Root of double-word types
**
** PARAMETERS: x - Square to be estimated
**
** RETURNS:    64-bit unsigned estimate of square root
**
** NOTE:
*****************************************************************************
*/
XDRV_UINT64 vrgISqrt(XDRV_UINT64 x)
{
   unsigned long long op, res, one;

   op = x;
   res = 0;

   one = 1ull << 62;
   while(one > op) one >>= 2;

   while(one != 0)
   {
      if(op >= res + one)
      {
         op = op - (res + one);
         res = res + 2 * one;
      }
      res >>= 1;
      one >>= 2;
   }

   return( res );
}

/*
*****************************************************************************
** FUNCTION:   linetestReadSignal
**
** PURPOSE:    Samples the selected signal and computes the AC and DC value
**             of the requested signal.
**
** PARAMETERS: chan        - chan to measure
**             measureType - DC only or AC & DC A/D measurements
**             tpdResults  - pointer to measured values
**             lowRes       - use low resolution or high resolution
**
** RETURNS:    SUCCESS or FAILURE
** 
** NOTE:
*****************************************************************************
*/
XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL lowRes )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];
   volatile BCM6838_APM *pApm = pDev->pApm;
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   volatile XDRV_UINT32* pRegApmAudioAdcA;
   volatile XDRV_UINT32* pRegApmAudioAdcB;

   XDRV_UINT64 tipMeanSquare;
   XDRV_UINT64 ringMeanSquare;
   XDRV_UINT64 tipRingMeanSquare;
   XDRV_UINT64 hvgMeanSquare;
   XDRV_UINT64 iLoopMeanSquare;
   XDRV_SINT64 iLoopACSquared;
   XDRV_SINT64 iLoopAC;
   XDRV_SINT64 iLoopDC;
   XDRV_UINT32 statsDuration;
   XDRV_UINT32 hvgWindowSize;
   XDRV_UINT32 temp_reg_apm_coeff_config;
   XDRV_UINT32 temp_reg_hvg_vtip_lores_cond;
   XDRV_UINT32 temp_reg_hvg_vhvg_lores_cond;
   XDRV_UINT32 temp_reg_hvg_vring_lores_cond;
   XDRV_UINT32 temp_reg_stats_config;
   XDRV_UINT32 temp_reg_cha_cic_config;
   XDRV_UINT32 temp_reg_audio_adc_a;
   XDRV_UINT32 temp_reg_hvg_cha_reg_2;
   XDRV_UINT32 temp_reg_hvg_cha_reg_0;
   XDRV_UINT32 temp_reg_chb_cic_config;
   XDRV_UINT32 temp_reg_audio_adc_b;
   XDRV_UINT32 temp_reg_hvg_chb_reg_2;
   XDRV_UINT32 temp_reg_hvg_chb_reg_0;
   XDRV_SINT64 kFactorAdjNum;
   XDRV_SINT64 kFactorAdjDiv;
   XDRV_SINT64 kDacAdjNum;
   XDRV_SINT64 kDacAdjDiv;

   /* swap channel if required */
   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   pRegApmAudioAdcA = &pApm->reg_codec_config_2;
   pRegApmAudioAdcB = &pApm->reg_codec_config_3;

   /* Store previous settings */
   temp_reg_apm_coeff_config = pApm->reg_apm_coeff_config;
   temp_reg_hvg_vtip_lores_cond = hvgp->reg_hvg_vtip_lores_cond;
   temp_reg_hvg_vhvg_lores_cond = hvgp->reg_hvg_vhvg_lores_cond;
   temp_reg_hvg_vring_lores_cond = hvgp->reg_hvg_vring_lores_cond;
   temp_reg_stats_config = pApm->reg_stats_config;
   temp_reg_cha_cic_config = pApm->reg_cha_cic_config;
   temp_reg_audio_adc_a = *pRegApmAudioAdcA;
   temp_reg_hvg_cha_reg_2 = hvgp->reg_hvg_cha_reg_2;
   temp_reg_hvg_cha_reg_0 = hvgp->reg_hvg_cha_reg_0;
   temp_reg_chb_cic_config = pApm->reg_chb_cic_config;
   temp_reg_audio_adc_b = *pRegApmAudioAdcB;
   temp_reg_hvg_chb_reg_2 = hvgp->reg_hvg_chb_reg_2;
   temp_reg_hvg_chb_reg_0 = hvgp->reg_hvg_chb_reg_0;

   /* Setup global registers for measurements */
   pApm->reg_apm_coeff_config &= ~COEFF_PROG_ADDR;
   pApm->reg_apm_coeff_config |= 0x04;

   /* Need to allow settling for TESTO changes */
   bosSleep( 10 );

   /* Setup channel and measurement specific registers */
   if( chan == 0 )
   {
      pApm->reg_cha_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_cha_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );
      *pRegApmAudioAdcA &= ~PGA_CTL_VADC;
      *pRegApmAudioAdcA |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      hvgp->reg_hvg_cha_reg_0 &= ~CONT_OR_BLOCK;

      /* Utilize audio ADC for current measurements on 6838 parts */
      if( measureType == TPD_MEASURE_DC_CHOP )
      {
         pApm->reg_stats_config = 0x00008699;
         pApm->reg_stats_config = 0x00000699;
      }
      else if( measureType == TPD_MEASURE_DC )
      {
         pApm->reg_stats_config = 0x00008490;
         pApm->reg_stats_config = 0x00000490;
      }
      else
      {
         pApm->reg_stats_config = 0x000084c0;
         pApm->reg_stats_config = 0x000004c0;
      }

      /* Setup HVG window sizes */
      if(( measureType == TPD_MEASURE_DC_CHOP ) ||
         ( measureType == TPD_MEASURE_DC ))
      {
         if( lowRes )
         {
            hvgWindowSize = 0x0388788A;
         }
         else
         {
            hvgWindowSize = 0xC788788A;
         }                  
      }                  
      else
      {
         if( lowRes )
         {
            hvgWindowSize = 0x03BB7BBD;
         }
         else
         {
            hvgWindowSize = 0xC7BB7BBD;
         }
      }

      /* Adjust window size */
      hvgWindowSize += 0x00010111;

      hvgp->reg_hvg_cha_reg_0 = ( hvgWindowSize );
      hvgp->reg_hvg_cha_reg_0 = ( hvgWindowSize | NEW_BLK_RQST );
      hvgp->reg_hvg_cha_reg_0 = ( hvgWindowSize );
   }
   else
   {
      /* Utilize audio ADC for current measurements on 6838 parts */
      pApm->reg_chb_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_chb_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );
      *pRegApmAudioAdcB &= ~( PGA_CTL_VADC );
      *pRegApmAudioAdcB |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );

      hvgp->reg_hvg_chb_reg_0 &= ~CONT_OR_BLOCK;

      /* Utilize audio ADC for current measurements on 6838 parts */
      if( measureType == TPD_MEASURE_DC_CHOP )
      {
         pApm->reg_stats_config = 0x00008999;
         pApm->reg_stats_config = 0x00000999;
      }
      else if( measureType == TPD_MEASURE_DC )
      {
         pApm->reg_stats_config = 0x00008890;
         pApm->reg_stats_config = 0x00000890;
      }
      else
      {
         pApm->reg_stats_config = 0x000088c0;
         pApm->reg_stats_config = 0x000008c0;
      }

      /* Setup HVG window sizes */
      if(( measureType == TPD_MEASURE_DC_CHOP ) ||
         ( measureType == TPD_MEASURE_DC ))
      {
         if( lowRes )
         {
            hvgWindowSize = 0x0388788A;
         }
         else
         {
            hvgWindowSize = 0xC788788A;
         }
      }                  
      else
      {
         if( lowRes )
         {
            hvgWindowSize = 0x03BB7BBD;
         }
         else
         {
            hvgWindowSize = 0xC7BB7BBD;
         }
      }

      /* Adjust window size */
      hvgWindowSize += 0x00101111;

      hvgp->reg_hvg_chb_reg_0 = ( hvgWindowSize );
      hvgp->reg_hvg_chb_reg_0 = ( hvgWindowSize | NEW_BLK_RQST );
      hvgp->reg_hvg_chb_reg_0 = ( hvgWindowSize );
   }

   /* Sleep to allow setup/measurements to complete */
   if(( measureType == TPD_MEASURE_DC ) || ( measureType == TPD_MEASURE_DC_CHOP ))
   {
      bosSleep( 30 );
   }
   else
   {
      bosSleep( 170 );
   }

   /* Calculate stats duration */
   statsDuration = 2 << ((( pApm->reg_stats_config & STATS_DURATION ) >> 4 ) + 1 );

   /* Calculate results */
   if( chan == 0 )
   {
      /* Calculate DC results */
      tpdResults->tipVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status9 & TIP_MEAN ))) * 75 / 2 / 256;
      tpdResults->ringVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status11 & RING_MEAN ))) * 75 / 2 / 256;
      tpdResults->tipRingVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status13 & TP_RG_MEAN ))) * 75 / 2 / 256;
      tpdResults->hvgVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status7 & HVG_MEAN ))) * 75 / 2 / 256;

      /* Utilize audio ADC for current measurements on 6838 parts */
      iLoopDC = ((XDRV_SINT64)( pApm->reg_apm_stats_1 ));
      tpdResults->iLoopDCRaw = pApm->reg_apm_stats_1;

      /* Calculate mean square results */
      tipMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status25 )) * 5625 , 4 ), 256 );
      ringMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status27 )) * 5625 , 4 ), 256 );
      tipRingMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status29 )) * 5625 , 4 ), 256 );
      hvgMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status23 )) * 5625 , 4 ), 256 );

      /* Utilize audio ADC for current measurements on 6838 parts */
      iLoopMeanSquare = (((((XDRV_UINT64)( pApm->reg_apm_stats_4 )) & MEAN_SQUARE_HI ) << 32 ) | ((XDRV_UINT64)( pApm->reg_apm_stats_3 )));
      iLoopMeanSquare = iLoopMeanSquare * statsDuration;
   }
   else
   {
      /* Calculate DC results */
      tpdResults->tipVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status10 & TIP_MEAN ))) * 75 / 2 / 256;
      tpdResults->ringVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status12 & RING_MEAN ))) * 75 / 2 / 256;
      tpdResults->tipRingVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status14 & TP_RG_MEAN ))) * 75 / 2 / 256;
      tpdResults->hvgVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status8 & HVG_MEAN ))) * 75 / 2 / 256;

      /* Utilize audio ADC for current measurements on 6838 parts */
      iLoopDC = ((XDRV_SINT64)( pApm->reg_apm_stats_2 ));
      tpdResults->iLoopDCRaw = pApm->reg_apm_stats_2;

      /* Calculate mean square results */      
      tipMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status26 )) * 5625 , 4 ), 256 );
      ringMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status28 )) * 5625 , 4 ), 256 );
      tipRingMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status30 )) * 5625 , 4 ), 256 );
      hvgMeanSquare = apmDiv( apmDiv( ((XDRV_UINT64)( hvgp->reg_hvg_status24 )) * 5625 , 4 ), 256 );

      /* Utilize audio ADC for current measurements on 6838 parts */
      iLoopMeanSquare = (((((XDRV_UINT64)( pApm->reg_apm_stats_6 )) & MEAN_SQUARE_HI ) << 32 ) | ((XDRV_UINT64)( pApm->reg_apm_stats_5 )));
      iLoopMeanSquare = iLoopMeanSquare * statsDuration;
   }

   /* Calculate AC results from DC and MNSQ results */
   tpdResults->tipVoltAC = ( vrgISqrt( tipMeanSquare - (( (XDRV_SINT64)tpdResults->tipVoltDC ) * ( (XDRV_SINT64)tpdResults->tipVoltDC ))));
   tpdResults->ringVoltAC = ( vrgISqrt( ringMeanSquare - (( (XDRV_SINT64)tpdResults->ringVoltDC ) * ( (XDRV_SINT64)tpdResults->ringVoltDC ))));
   tpdResults->tipRingVoltAC = ( vrgISqrt( tipRingMeanSquare - (( (XDRV_SINT64)tpdResults->tipRingVoltDC ) * ( (XDRV_SINT64)tpdResults->tipRingVoltDC ))));
   tpdResults->hvgVoltAC = ( vrgISqrt( hvgMeanSquare - (( (XDRV_SINT64)tpdResults->hvgVoltDC ) * ( (XDRV_SINT64)tpdResults->hvgVoltDC ))));

   iLoopACSquared = ( iLoopMeanSquare - ( iLoopDC * iLoopDC ) );
   if( iLoopACSquared > 0 )
   {
      iLoopAC = vrgISqrt( iLoopACSquared );
   }
   else
   {
      iLoopAC = 0;
   }

   /* Adjust the current measurements */
   /* SLIC gain = 10.05 (vs 10.25) ; */
   /* tpdResults->iLoopDC = iLoopDC * 1750 * 2.1478 / 1.68 / ( statsDuration << 8 ) ; simplifies to */
   apm6838CalculateKadjValues( &kFactorAdjNum, &kFactorAdjDiv, &kDacAdjNum, &kDacAdjDiv );
   tpdResults->iLoopDC = apmDiv(apmDiv( iLoopDC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, ( statsDuration << 10 ) ), kFactorAdjDiv);
   tpdResults->iLoopAC = apmDiv(apmDiv( iLoopAC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, ( statsDuration << 10 ) ), kFactorAdjDiv);

   /* Restore previous settings */
   pApm->reg_apm_coeff_config = temp_reg_apm_coeff_config;
   hvgp->reg_hvg_vtip_lores_cond = temp_reg_hvg_vtip_lores_cond;
   hvgp->reg_hvg_vhvg_lores_cond = temp_reg_hvg_vhvg_lores_cond;
   hvgp->reg_hvg_vring_lores_cond = temp_reg_hvg_vring_lores_cond;
   pApm->reg_stats_config = temp_reg_stats_config;

   if( chan == 0 )
   {
      pApm->reg_cha_cic_config = temp_reg_cha_cic_config;
      *pRegApmAudioAdcA = temp_reg_audio_adc_a;
      hvgp->reg_hvg_cha_reg_2 = temp_reg_hvg_cha_reg_2;
      hvgp->reg_hvg_cha_reg_0 = temp_reg_hvg_cha_reg_0;
   }
   else
   {
      pApm->reg_chb_cic_config = temp_reg_chb_cic_config;
      *pRegApmAudioAdcB = temp_reg_audio_adc_b;
      hvgp->reg_hvg_chb_reg_2 = temp_reg_hvg_chb_reg_2;
      hvgp->reg_hvg_chb_reg_0 = temp_reg_hvg_chb_reg_0;
   }

   /* Adjust for calibration factors */
   tpdResults->tipVoltDC -= tipVoltDCcor[chan];
   tpdResults->ringVoltDC -= ringVoltDCcor[chan];

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestReadSignalSettle
**
** PURPOSE:    Samples the selected signal and computes the AC and DC value
**             of the requested signal but only after the signal has settled
**
** PARAMETERS: chan        - chan to measure
**             measureType - DC only or AC & DC A/D measurements
**             tpdResults  - pointer to measured values
**             voltage     - voltage or current to settle
**             lowRes       - use low resolution or high resolution
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL voltage, XDRV_BOOL lowRes )
{
   XDRV_UINT32 attempts;
   XDRV_SINT32 threshold, delta, currentValue, previousValue;

   attempts = SETTLE_WAIT_MAX;
   previousValue = BMU6838_MAX_CURRENT;
   threshold = ( voltage ? V_SETTLE : I_SETTLE );
   delta = threshold;

   while( delta >= threshold )
   {
      if( attempts < SETTLE_WAIT_MAX )
      {
         bosSleep( 160 );
      }

      /* Determine if the maximum number of attempts has occurred */
      if( attempts == 0 )
      {
         return( XDRV_TPD_FAILURE );
      }

      /* Read the voltages and currents */
      if ( linetestReadSignal( chan, measureType, tpdResults, lowRes ) )
      {
         return( XDRV_TPD_FAILURE );
      }

      currentValue = ( voltage ? tpdResults->tipVoltDC : tpdResults->iLoopDC );
      delta = abs( currentValue - previousValue );
      previousValue = currentValue;

      /* Decrement number of attempts remaining */
      attempts--;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropHazardForeign
**
** PURPOSE:    Runs the Hazardous Potential and the Foreign Electomotive
**             Force tests on a specified channel
**
** PARAMETERS: chan       - channel to run the tests on
**             hpfefRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             HAZARD_FAIL if Hazardous Potential test fails,
**             FOREIGN_FAIL if Foreign Electomotive Force test fails,
**             PASS if both pass
*****************************************************************************
*/
XDRV_UINT16 linetestDropHazardForeign( int chan, HSZDROPRSLT_HPFEF *hpfefRsltp )
{
   TPD_MEASURE_RESULTS tpdResults;

   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));

   /* Discharge any stored voltage on this line */
   tpdDischargeVoltage( chan, &tpdResults );

   /* Measure signal to verify foreign/hazard levels */
   if ( linetestReadSignal( chan, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Measure signal using low res if voltage significantly large */
   /* Check both AC and DC voltages since we can't detect saturated measurements */
   if(( tpdResults.tipVoltDC < TPD_MIN_HIRES_VOLTAGE ) ||
      ( tpdResults.ringVoltDC < TPD_MIN_HIRES_VOLTAGE ) ||
      ( tpdResults.tipVoltDC > TPD_MAX_HIRES_VOLTAGE ) ||
      ( tpdResults.ringVoltDC > TPD_MAX_HIRES_VOLTAGE ) ||
      ( tpdResults.tipVoltAC > TPD_MAX_HIRES_VOLTAGE ) ||
      ( tpdResults.ringVoltAC > TPD_MAX_HIRES_VOLTAGE ))
   {
      if ( linetestReadSignal( chan, TPD_MEASURE_ACDC, &tpdResults, XDRV_TRUE ))
      {
         return ( XDRV_TPD_FAILURE );
      }
   }

   hpfefRsltp->tipAC = tpdResults.tipVoltAC;
   hpfefRsltp->tipDC = tpdResults.tipVoltDC-tpdVoltCalibrationDCOffset[chan].tipVoltCalibrationDC;
   hpfefRsltp->ringAC = tpdResults.ringVoltAC;
   hpfefRsltp->ringDC = tpdResults.ringVoltDC-tpdVoltCalibrationDCOffset[chan].ringVoltCalibrationDC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of HAZ & FOR V readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: tipAC  = %.8d, tipDC  = %.8d ", (int)hpfefRsltp->tipAC, (int)hpfefRsltp->tipDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ringAC = %.8d, ringDC = %.8d \n", (int)hpfefRsltp->ringAC, (int)hpfefRsltp->ringDC));   

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropOffhook
**
** PURPOSE:    Runs the Offhook simulation test on a specifeid channel
**
** PARAMETERS: chan - channel to run the test on
**
** RETURNS:    SUCCESS or FAILURE
**
*****************************************************************************
*/
XDRV_UINT16 linetestDropOffhook( int chan )
{
   /* Engage off hook simulation hardware */
   slic6838L9540SetTestLoad( pSlicDriver[chan], XDRV_TRUE );

   /* Pause to allow CAS to detect and signal offhook condition */
   bosSleep( 2000 );

   /* Disengage off hook simulation hardware */
   slic6838L9540SetTestLoad( pSlicDriver[chan], XDRV_FALSE );

   /* Not required */
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropROH
**
** PURPOSE:    Runs the Receiver Off Hook test on a specifeid channel
**
** PARAMETERS: chan     - channel to run the test on
**             rohRsltp - pointer to the structure to post the results to
**
** RETURNS:    SUCCESS or FAILURE
**
*****************************************************************************
*/
XDRV_UINT16 linetestDropROH( int chan, HSZDROPRSLT_ROH *rohRsltp )
{
   XDRV_SINT32 lowTipRingDC;
   XDRV_SINT32 highTipRingDC;
   XDRV_SINT32 lowCurrentDC;
   XDRV_SINT32 highCurrentDC;
   int lowRes, difRes;
   int offhook;
   TPD_MEASURE_RESULTS tpdResults;
   int temp1, temp2, temp3, tempLoopCurrent;

   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));

   /* Store current SLIC parms */
   xdrvSlicGetSlicParms( pSlicDriver[chan], &temp1, &tempLoopCurrent, &temp2, &temp3 );

   /* Change loop current limit to 25mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_FALSE );

   /* Change to forward active mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

   /* Allow voltages to settle */
   bosSleep( 200 );

   /* Determine if the phone is offhook */
   offhook = xdrvSlicIsOffhook( pSlicDriver[chan] );

   /* If the phone is onhook according to the SLIC this test is not necessary */
   if( offhook == XDRV_FALSE )
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "No ROH readings necessary \n")); 

      /* Change loop current limit to previous setting */
      xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], tempLoopCurrent );

      rohRsltp->phoneOnhook = XDRV_TRUE;
      return ( XDRV_TPD_SUCCESS );
   }

   /* Allow voltages to settle further */
   bosSleep( 300 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   lowTipRingDC = tpdResults.tipRingVoltDC;
   lowCurrentDC = tpdResults.iLoopDC;

   /* Change loop current limit to 40mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_TRUE );

   /* Allow voltages to settle */
   bosSleep( 500 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   highTipRingDC = tpdResults.tipRingVoltDC;
   highCurrentDC = tpdResults.iLoopDC;

   /* Change loop current limit to previous setting */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], tempLoopCurrent );

   /* Return to standby state */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Calculate resistance values */
   lowRes = ( lowTipRingDC ) * 1000 / ( lowCurrentDC );
   difRes = ( highTipRingDC - lowTipRingDC ) * 1000 /
            ( highCurrentDC - lowCurrentDC );

   if((( highTipRingDC - lowTipRingDC ) > 300 ) ||
      (( lowTipRingDC ) > 1000 ))
   {
      /* If ( lowRes / difRes ) > 1.33 report an offhook condition */
      if(( lowRes * 3 ) > ( difRes * 4 ))
      {
         rohRsltp->phoneOnhook = XDRV_FALSE;
      }
      else
      {
         rohRsltp->phoneOnhook = XDRV_TRUE;
      }
   }
   else
   {
      rohRsltp->phoneOnhook = XDRV_TRUE;
   }

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ROH readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowTipRingDC = %.8d ", (int)lowTipRingDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: highTipRingDC = %.8d ", (int)highTipRingDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowCurrentDC = %.8d ", (int)lowCurrentDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: highCurrentDC = %.8d ", (int)highCurrentDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowRes = %.8d ", (int)lowRes));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: difRes = %.8d ", (int)difRes));

   return (XDRV_TPD_SUCCESS);
}

/*
*****************************************************************************
** FUNCTION:   linetestDropResFlt
**
** PURPOSE:    Runs the Resistive Faults test on a specified channel
**
** PARAMETERS: chan      - channel to run the test on
**             rohRsltp  - pointer to the result structure of the previous test
**             rfltRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp )
{
   XDRV_SINT32 VTfh,VRfh,IMTfh;
   XDRV_SINT32 VTrh,VRrh,IMTrh;
   XDRV_SINT32 VTfl,VRfl,IMTfl;
   XDRV_SINT32 VTrl,VRrl,IMTrl;
   XDRV_SINT32 VToh,VRoh,IMToh;
   XDRV_SINT32 VTol,VRol,IMTol;

   XDRV_SINT64 Rtr,Rtg,Rrg;
   XDRV_SINT64 Rdiv;

   TPD_MEASURE_RESULTS tpdResults;
   int temp1, temp2, temp3, tempLoopCurrent;

   /* Define initial values to force 1st pass through measurement loops */
   IMTfh = BMU6838_MAX_CURRENT;
   IMTrh = BMU6838_MAX_CURRENT;
   IMTfl = BMU6838_MAX_CURRENT;
   IMTrl = BMU6838_MAX_CURRENT;
   VToh = BMU6838_MAX_CURRENT;
   VTol = BMU6838_MAX_CURRENT;

   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));

   /* Store current SLIC parms */
   xdrvSlicGetSlicParms( pSlicDriver[chan], &temp1, &tempLoopCurrent, &temp2, &temp3 );

   /* Change loop current limit to 40mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_TRUE );

   /* Change SLIC to High Battery On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHT );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE );

   /* Store results */
   VTfh = tpdResults.tipVoltDC;
   VRfh = tpdResults.ringVoltDC;
   IMTfh = tpdResults.iLoopDC;

   /* Change SLIC to High Battery Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_TRUE, XDRV_FALSE );

   /* Store results */
   VToh = tpdResults.tipVoltDC;
   VRoh = tpdResults.ringVoltDC;
   IMToh = tpdResults.iLoopDC;

   /* Change SLIC to High Battery Reverse On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHTR );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE );

   /* Store results */
   VTrh = tpdResults.tipVoltDC;
   VRrh = tpdResults.ringVoltDC;
   IMTrh = tpdResults.iLoopDC;

   /* Change loop current limit to 25mA */
   /* By setting different limits for high & low, lower impedances can be accurately measured */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_FALSE );

   /* Change SLIC to Low Battery Reverse On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHTR );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE );

   /* Store results */
   VTrl = tpdResults.tipVoltDC;
   VRrl = tpdResults.ringVoltDC;
   IMTrl = tpdResults.iLoopDC;

   /* Change SLIC to Low Battery On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHT );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE );

   /* Store results */
   VTfl = tpdResults.tipVoltDC;
   VRfl = tpdResults.ringVoltDC;
   IMTfl = tpdResults.iLoopDC;

   /* Change SLIC to Low Battery Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );
   
   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   linetestReadSignalSettle( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_TRUE, XDRV_FALSE );

   /* Store results */
   VTol = tpdResults.tipVoltDC;
   VRol = tpdResults.ringVoltDC;
   IMTol = tpdResults.iLoopDC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ResFLT readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTrl = %.8d,VRrl = %.8d ,IMTrl = %.8d ", (int)VTrl,(int)VRrl,(int)IMTrl));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTfl = %.8d,VRfl = %.8d ,IMTfl = %.8d ", (int)VTfl,(int)VRfl,(int)IMTfl));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTol = %.8d,VRol = %.8d ,IMTol = %.8d ", (int)VTol,(int)VRol,(int)IMTol));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTrh = %.8d,VRrh = %.8d ,IMTrh = %.8d ", (int)VTrh,(int)VRrh,(int)IMTrh));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTfh = %.8d,VRfh = %.8d ,IMTfh = %.8d ", (int)VTfh,(int)VRfh,(int)IMTfh));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VToh = %.8d,VRoh = %.8d ,IMToh = %.8d \n", (int)VToh,(int)VRoh,(int)IMToh));   

   /* Calculate tip-ground impedance, including offset adjustment */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTrh - IMTrl + 4 ) * ( XDRV_SINT64 )(( VRol - VRoh ) - ( VTol - VToh )));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rtg = 1000 * apmDiv((( XDRV_SINT64 )( VTrh - VTrl ) * ( XDRV_SINT64 )(( VRol - VRoh ) + ( VTol - VToh ))), Rdiv);

   /* Calculate tip-ring impedance, including offset adjustment */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTrh - IMTrl + 4 ) * apmDiv(Rtg, 1000) - ( XDRV_SINT64 )( VTrh - VTrl ));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rtr = apmDiv( ( 2 * ( XDRV_SINT64 )( VTrh - VTrl ) * Rtg ) , Rdiv);

   /* Calculate ring-ground impedance, including offset adjustment */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTfh - IMTfl + 8 ) *  apmDiv(Rtr, 1000) - 2 * ( XDRV_SINT64 )( VRfl - VRfh ));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rrg = apmDiv((( XDRV_SINT64 )( VRfl - VRfh ) * Rtr ), Rdiv);
   
   /* Remove contribution of 10M sense resistors */
   Rtg = apmDiv((10079000 * Rtg), (10079000 - Rtg));
   Rrg = apmDiv((10079000 * Rrg), (10079000 - Rrg));

   /* Check for invalid measurments due to current limiting */
   if(( IMTrh < BMU6838_MIN_CURRENT ) &&
      ( IMTfh > BMU6838_MAX_CURRENT ))
   {
      /* Calculate tip-ring impedance, including offset adjustment */
      Rdiv = ( 2 * ( XDRV_SINT64 )( IMTrh - IMTrl + 4 ) * apmDiv(Rtg, 1000) - ( XDRV_SINT64 )(( VTrh - VTrl ) - ( VRrh - VRrl )));
      Rtr = apmDiv(( 2 * ( XDRV_SINT64 )(( VTrh - VTrl ) - ( VRrh - VRrl )) * Rtg ), Rdiv);
      Rtg = 1000000;
      Rrg = 1000000;
   }
   else if (( abs(IMTrl) > ( BMU6838_MAX_CURRENT / 2 ) ) &&
            ( abs(IMTrh) > ( BMU6838_MAX_CURRENT ) ) &&
            ( abs(IMTol) < ( BMU6838_MAX_CURRENT / 200 ) ) &&
            ( abs(IMToh) < ( BMU6838_MAX_CURRENT / 200 ) ) &&
            ( IMTfl < 0 ))
   {
      Rrg = 1000000;
      Rtr = 1000000;
      Rtg = 0;
   }
   else if (( abs(IMTfl) > ( BMU6838_MAX_CURRENT * 5 / 8 ) ) &&
            ( abs(IMTfh) > ( BMU6838_MAX_CURRENT ) ) &&
            ( abs(IMTol) > ( BMU6838_MAX_CURRENT * 3 / 8 ) ) &&
            ( abs(IMToh) > ( BMU6838_MAX_CURRENT / 2 ) ))
   {
      Rtg = 1000000;
      Rtr = 1000000;
      Rrg = 0;
   }

   /* Save calculated impedances */
   rfltRsltp->tipGroundImpedance = abs(Rtg);
   rfltRsltp->ringGroundImpedance = abs(Rrg);
   rfltRsltp->tipRingImpedance = abs(Rtr);

   /* Change loop current limit to previous value */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], tempLoopCurrent );

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropRing
**
** PURPOSE:    Runs the Ringer test on a specified channel
**
** PARAMETERS: chan     - channel to run the test on
**             rngRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp)
{
   XDRV_SINT32 tipRingVoltAC;
   XDRV_SINT32 AcCurrent;
   XDRV_UINT32 resistance;
   int tempRingFrequency;
   int tempRingWaveshape;
   int tempRingVoltage;
   int tempRingOffset;
   int tempRingOffsetCal;
   TPD_MEASURE_RESULTS tpdResults;

   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));

   /* Save the ringing amplitude and set it to a known value */
   xdrvApmGetRingParms( pApmDriver[chan], &tempRingFrequency, &tempRingWaveshape, &tempRingVoltage, &tempRingOffset, &tempRingOffsetCal );
   xdrvApmSetRingParms( pApmDriver[chan], 18, tempRingWaveshape, 6, 0, tempRingOffsetCal );

   /* Set SLIC to ringing */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_MED );

   bosSleep( 5 * 100 );

   if ( linetestReadSignal( chan, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE ))
   {
      return( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   tipRingVoltAC = tpdResults.tipRingVoltAC;
   AcCurrent = tpdResults.iLoopAC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of RING readings ... \n")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: trVac = %.8d ", (int)tipRingVoltAC ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: Iac  = %.8d \n", (int)AcCurrent ));   

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Reset ring voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   /* Calculate Tip/Ring voltage and REN value */
   if ( AcCurrent == 0 )
   {
      AcCurrent = 1;
   }

   resistance = tipRingVoltAC * 1000 / AcCurrent;

   if( resistance == 0 )
   {
      resistance = 1;
   }

   rngRsltp->RENValue1 = 7000 / resistance;
   rngRsltp->RENValue2 = 7000 % resistance * 1000 / resistance;
   rngRsltp->tipRingVoltRingAC = tipRingVoltAC;
   rngRsltp->iLoopRingAC = AcCurrent;      

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropCalibration
**
** PURPOSE:    Runs the calibration tests for this platform
**
** PARAMETERS: chan     - channel to run the test on
**             calRsltp - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp)
{
   /* Nothing to do here */
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfAssert
**
** PURPOSE:    Runs the self assert test on a specifeid channel
**
** PARAMETERS: chan           - channel to run the tests on
**             selfAssertTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestSelfAssert( int chan, XDRV_SINT32 *selfAssertPassed )
{
   if ( xdrvDebugDspGetAssertStackPtr( pDebugDriver[chan] ) )
   {
      *selfAssertPassed = XDRV_FALSE;
   }
   else
   {
      *selfAssertPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfDspCount
**
** PURPOSE:    Runs the self dsp count test on a specifeid channel
**
** PARAMETERS: chan         - channel to run the tests on
**             dspCountTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestSelfDspCount( int chan, XDRV_SINT32 *selfDspCountPassed )
{
   if ( xdrvDebugDspReadCnt( pDebugDriver[chan] ) )
   {
      *selfDspCountPassed = XDRV_FALSE;
   }
   else
   {
      *selfDspCountPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfHvg
**
** PURPOSE:    Runs the self hvg test on a specifeid channel
**
** PARAMETERS: chan        - channel to run the tests on
**             selfHvgTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestSelfHvg( int chan, XDRV_SINT32 *selfHvgPassed )
{
   if( xdrvApmGetOverCurrentStatus( pApmDriver[chan], chan ) )
   {
      *selfHvgPassed = XDRV_FALSE;
   }
   else
   {
      *selfHvgPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfApm
**
** PURPOSE:    Runs the self apm test on a specifeid channel
**
** PARAMETERS: chan        - channel to run the tests on
**             selfApmTest - pointer to the structure to post the results to
**
** RETURNS:
**             FAIL on failure, otherwise PASS
*****************************************************************************
*/
XDRV_UINT16 linetestSelfApm( int chan, XDRV_SINT32 *selfApmPassed )
{
   if( xdrvApmDebugStatusDump( pApmDriver[chan] ))
   {
      *selfApmPassed = XDRV_FALSE;
   }
   else
   {
      *selfApmPassed = XDRV_TRUE;
   }

   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfCap
**
** PURPOSE:    Estimates line capacitance based on REN calculation
**
** PARAMETERS: chan    - channel under test
**             selfCap - pointer to results structure
**
** RETURNS: Nothing
*****************************************************************************
*/
XDRV_UINT16 linetestSelfCap(int chan, XDRV_SINT32 *selfCap)
{
   HSZDROPRSLT_RNG rngRslt;

   linetestDropRing(chan, &rngRslt);

   /* Convert to nanofarads */
   *selfCap = 8 * ((rngRslt.RENValue1 * 1000) + rngRslt.RENValue2);
   
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfRing
**
** PURPOSE:    Estimate ring voltage
**
** PARAMETERS: chan     - channel under test
**             selfRing - pointer to results structure
**
** RETURNS: Nothing
*****************************************************************************
*/
XDRV_UINT16 linetestSelfRing(int chan, XDRV_SINT32 *selfRing)
{
   HSZDROPRSLT_RNG rngRslt;

   linetestDropRing(chan, &rngRslt);

   /* Should already be in mVAC */
   *selfRing = rngRslt.tipRingVoltRingAC;
   
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestSelfFeed
**
** PURPOSE:    Estimate DC feed parameters
**
** PARAMETERS: chan      - channel under test
**             voltDc    - pointer to feed voltage estimate
**             currentDc - pointer to feed current estimate
**
** RETURNS: Nothing
*****************************************************************************
*/
XDRV_UINT16 linetestSelfFeed(int chan, XDRV_SINT32 *voltDc, XDRV_SINT32 *currentDc)
{
   TPD_MEASURE_RESULTS tpdResults;
   int offhook;
   
   /* Force loop current limit to 25mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_FALSE );

   /* Change to forward active mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

   /* Allow voltages to settle */
   bosSleep( 200 );

   /* Determine if the phone is offhook */
   offhook = xdrvSlicIsOffhook( pSlicDriver[chan] );

   /* If phone is offhook then current measurements are not possible */
   if( offhook == XDRV_TRUE )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Feed test not supported for ROH")); 

      *voltDc = 0;
      *currentDc = 0;
      return ( XDRV_TPD_FAILURE );
   }

   /* Change loop current limit to 40mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_TRUE );

   /* Change SLIC to High Battery On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Allow voltages to settle */
   bosSleep( 1000 );
     
   /* Measure signal to verify foreign/hazard levels */
   if ( linetestReadSignal( chan, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }
   
   /* Save desired results */
   *voltDc = tpdResults.ringVoltDC;
   *currentDc = tpdResults.iLoopDC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** "));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Feed test readings ..."));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "FeedVoltage = %.8ld,LoopCurrent = %.8ld", (long int)voltDc, (long int)currentDc));
  
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   tpd6838L9540InitChannel
**
** PURPOSE:    TPD initialization - called once for each channel during initialization
**
** PARAMETERS: chan - channel to initialize
**             tpdDrv - pointer to TPD driver
**             slicDrv - pointer to SLIC driver
**             apmDrv - pointer to APM driver
**             debugDrv - pointer to DEBUG driver
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
int tpd6838L9540InitChannel( int chan, XDRV_TPD *tpdDrv, XDRV_SLIC *slicDrv,
                             XDRV_APM *apmDrv, XDRV_DEBUG *debugDrv )
{
  /* Set the "meta-driver" interface */
  tpdDrv->pDrvFuncs = &tpd6838L9540DrvFuncs;

   if ( chan < TPD_MAX_ENDPT )
   {
      pSlicDriver[chan] = slicDrv;
      pApmDriver[chan] = apmDrv;
      pDebugDriver[chan] = debugDrv;
   }
   else
   {
      return ( XDRV_TPD_FAILURE );
   }

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   tpdRunDropTests
**
** PURPOSE:    Run TPD Drop Tests
**
** PARAMETERS: chan - channel to perform tests on
**             cmd  - tests to perform
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp )
{
   /*
   ** Test 1: Run the Hazardous Voltage and Foreign Electromotive tests
   */
   if( cmd & XDRV_TPD_RUN_DROP_HPFEF )
   {
      linetestDropHazardForeign( chan, &dropRsltp->dropHpFefRslt );
   }

   /*
   ** Test 2: Run the Resistive Faults test
   */
   if( cmd & XDRV_TPD_RUN_DROP_RESFLT )
   {
      linetestDropResFlt( chan, &dropRsltp->dropRfltRslt );
   }

   /*
   ** Test 3: Run the Receiver Off Hook test
   */
   if( cmd & XDRV_TPD_RUN_DROP_ROH )
   {
      linetestDropROH( chan, &dropRsltp->dropRohRslt );
   }

   /*
   ** Test 4: Run the Ringer test
   */
   if( cmd & XDRV_TPD_RUN_DROP_RING )
   {
      linetestDropRing( chan, &dropRsltp->dropRngRslt );
   }

   /*
   ** Test 5: Run the Offhook Simulation Test
   */
   if( cmd & XDRV_TPD_RUN_DROP_OFFHOOK )
   {
      linetestDropOffhook( chan );
   }

   /*
   ** Test 6: Run the Calibration Tests
   */
   if( cmd & XDRV_TPD_RUN_DROP_CALIBRATION )
   {
      linetestDropCalibration( chan, &dropRsltp->dropCalRslt );
   }

   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdRunSelfTests
**
** PURPOSE:    Run TPD Self Tests
**
** PARAMETERS: chan - channel to perform tests on
**             cmd - tests to perform
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp )
{
   /*
   ** Test 1: Run the Assert tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_ASSERT )
   {
      linetestSelfAssert( chan, &selfRsltp->selfAssertPassed );
   }

   /*
   ** Test 2: Run the DSP Count tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_DSP )
   {
      linetestSelfDspCount( chan, &selfRsltp->selfDspCountPassed );
   }

   /*
   ** Test 3: Run the HVG Diagnostic tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_HVG )
   {
      linetestSelfHvg( chan, &selfRsltp->selfHvgPassed );
   }

   /*
   ** Test 4: Run the APM tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_APM )
   {
      linetestSelfApm( chan, &selfRsltp->selfApmPassed );
   }

   /*
   ** Test 5: Run the Voltage tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_LOOP ||
       cmd & XDRV_TPD_RUN_SELF_VOLTAGE )
   {
      linetestSelfFeed( chan, &selfRsltp->selfBattery, &selfRsltp->selfLoop );
   }

   /*
   ** Test 6: Run the ring tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_RING )
   {
      linetestSelfRing( chan, &selfRsltp->selfRing );
   }

   /*
   ** Test 7: Run the cap tests
   */
   if( cmd & XDRV_TPD_RUN_SELF_CAP )
   {
      linetestSelfCap( chan, &selfRsltp->selfCap );
   }
   
   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdTestingSetup
**
** PURPOSE:    Set up TPD testing suite
**
** PARAMETERS: chan - channel to perform tests on
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdTestingSetup( int chan )
{
   /* Disable CAS */
   tpdCasDisable( chan );

   /* Return SLIC to default hook state */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

   /* Start HVG if necessary */
   if ( !hvg6838IsEnabled( chan ) )
   {
      /* HVG is disabled - need to restart it */
      hvg6838Start( chan );
   }   

   /* Wait for state change to complete */
   bosSleep( 40 );

   /* Disable audio filters */
   tpdDisableYFilter( chan );

   return ( XDRV_TPD_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   tpdTestingShutdown
**
** PURPOSE:    Shutdown TPD testing suite
**
** PARAMETERS: chan - channel to perform tests on
**             isEnabled - XDRV_BOOLean indicating admin status of this line 
**                         (enabled = 1; disabled = 0)
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled )
{
   /* 
   ** Need to put the line into a correct state based on current admin status.
   ** If the line was administratively disabled, we want to disconnect the physical 
   ** interface and if it was enabled, we want to restore to regular operating state 
   */
   
   if ( isEnabled )
   {
      if ( !hvg6838IsEnabled( chan ) )
      {
         /* HVG is disabled - need to restart it */
         hvg6838Start( chan );
      }   

      /* Reenable CAS */
      tpdCasEnable( chan );
   }
   else
   {
      /* Cut the battery voltage on this channel */
      hvg6838Stop( chan );
      
      /* Disable CAS */
      tpdCasDisable( chan );
   }

   /* Enable audio filters */
   tpdEnableYFilter( chan );

   return ( XDRV_TPD_SUCCESS );
}


/*
******************************************************************************
** FUNCTION:   tpdBatteryControl
**
** PURPOSE:    This is a task to control the HVG battery voltage
**
** PARAMETERS: chan - the channel to control
**             mode - the voltage to switch to
**
** RETURNS:    None
**
*****************************************************************************
*/
void tpdBatteryControl( int chan, TPD_BATTERY_MODE mode )
{
   switch ( mode )
   {
      case TPD_BATTERY_MODE_HIGH:
      {
         xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_RING );
      }
      break;

      case TPD_BATTERY_MODE_MED:
      {
         xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_ONHOOK );
      }
      break;

      case TPD_BATTERY_MODE_LOW:
      {
         xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_OFFHOOK );
      }
      break;

      case TPD_BATTERY_MODE_TEST1:
      case TPD_BATTERY_MODE_TEST2:
      {
         xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_TEST1 );
      }
      break;

      default:
      {
      }
      break;
   }
}

/*
******************************************************************************
** FUNCTION:   tpdEnableYFilter
**
** PURPOSE:    This is a function to enable the APM Y-Filter
**
** PARAMETERS: chan - the channel to enable
**
** RETURNS:    None
**
*****************************************************************************
*/
void tpdEnableYFilter( int chan )
{
   volatile BCM6838_APM *pApm = BCM6838_APM_PTR;
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   switch ( chan )
   {
      case 0:
      {
         pApm->reg_cha_hybal_config |= 0x00008000;
      }
      break;
      case 1:
      {
         pApm->reg_chb_hybal_config |= 0x00008000;
      }
      break;

      default:
      {
         /* Invalid channel */
         return;
      }
      break;
   }
}


/*
******************************************************************************
** FUNCTION:   tpdDisableYFilter
**
** PURPOSE:    This is a function to disable the APM Y-Filter
**
** PARAMETERS: chan - the channel to disable
**
** RETURNS:    None
**
*****************************************************************************
*/
void tpdDisableYFilter( int chan )
{
   volatile BCM6838_APM *pApm = BCM6838_APM_PTR;
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   switch ( chan )
   {
      case 0:
      {
         pApm->reg_cha_hybal_config &= ~0x00008000;
      }
      break;
      case 1:
      {
         pApm->reg_chb_hybal_config &= ~0x00008000;
      }
      break;

      default:
      {
         /* Invalid channel */
         return;
      }
      break;
   }
}


/*
*****************************************************************************
*
*  tpdCasDisable - Disables CAS service on a specified channel
*
*****************************************************************************
*
*  PURPOSE:
*     Disables CAS service on a specified channel
*
*  PARAMETERS:
*     chan - index of the channel to disable
*
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************
*/
void tpdCasDisable( int chan )
{
   /* Disable hookstate updates */
   xdrvApmHvgUpdateHookStatusEnable( pApmDriver[chan], chan, XDRV_FALSE );
                          
   /* Send CAS control command to go service denied state */
   casCtlSetState( chan, CAS_CTL_FXO_SERVICEDENIED );

   /* Wait for CAS to properly shutdown */
   bosSleep( 40 );
}


/*
*****************************************************************************
*
*  tpdCasEnable - Enables CAS service on a specified channel
*
*****************************************************************************
*
*  PURPOSE:
*     Enables CAS service on a specified channel
*
*  PARAMETERS:
*     chan - index of the channel to enable
*
*  RETURNS:
*
*  NOTE:
*
*****************************************************************************
*/
void tpdCasEnable( int chan )
{
   /* Enable hookstate updates */
   xdrvApmHvgUpdateHookStatusEnable( pApmDriver[chan], chan, XDRV_TRUE );
                          
   /* Send CAS control command to go idle state with normal polarity */
   casCtlSetState( chan, CAS_CTL_FXO_IDLE );

   /* Wait for CAS to properly start up */
   bosSleep( 40 );
}

/*
*****************************************************************************
** FUNCTION:   tpdChanReset
**
** PURPOSE:    Reset the given channel
**
** PARAMETERS: chan - channel to reset
**
** RETURNS:    XDRV_TPD_ERR_ABORT if we were interupted by a BOS_STATUS_RESET
**             XDRV_TPD_SUCCESS otherwise
**
*****************************************************************************
*/
static XDRV_TPD_STATUS tpdChanReset( int chan )
{
   XCHG_ASSERT( chan < TPD_MAX_ENDPT );

   /* Discharge any stored voltage on this line */
   tpdDischargeVoltage( chan, NULL );

   /* Now go to sleep for effect. */
   if( bosSleep( 1000 ) == BOS_STATUS_RESET )
   {
      /* We're being reset. Abort. */
      return XDRV_TPD_ERR_ABORT;
   }

   /* Reset the HVG digital logic while disconnected */
   hvg6838Reset(chan);

   /* Now go to sleep for effect. */
   if( bosSleep( 1000 ) == BOS_STATUS_RESET )
   {
      /* We're being reset. Abort. */
      return XDRV_TPD_ERR_ABORT;
   }

   /* Reset the SLIC state based on current hook state */
   if ( xdrvSlicIsOffhook( pSlicDriver[chan] ) )
   {
      xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );
   }
   else
   {
      xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );
   }

   return XDRV_TPD_SUCCESS;
}

/*
*****************************************************************************
** FUNCTION:   tpdDischargeVoltage
**
** PURPOSE:    Discharge the voltage on this line
**
** PARAMETERS: chan - channel to discharge voltage on
**             tpdResults - pointer to store wink results
**
** RETURNS:    Nothing
**
*****************************************************************************
*/
static void tpdDischargeVoltage( int chan, TPD_MEASURE_RESULTS *tpdResults )
{
   /* Change to wink to discharge voltage on load */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_WINK );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_MED );

   /* Pause for stray voltages to discharge */
   bosSleep( 400 );

   /* Measure signal to verify absence of foreign/hazard levels if desired */
   if( tpdResults != NULL )
   {
      linetestReadSignal( chan, TPD_MEASURE_DC, tpdResults, XDRV_FALSE );
   }

   /* Change to disconnected mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO );

   bosSleep( 200 );
}

/*
*****************************************************************************
** FUNCTION:   tpd6838L9540BootupCalibration
**
** PURPOSE:    Run TPD Bootup Calibration
**
** PARAMETERS: results - Pointer to results
**             updateValues - Update offsets only or perform calibration
**
** RETURNS:    SUCCESS or FAILURE of all calibration tests
**
*****************************************************************************
*/
int tpd6838L9540BootupCalibration( HSZDROPRSLT_CAL *results, XDRV_BOOL updateValues )
{
   XDRV_SINT32 tipVoltDCoffset[APM6838_NUM_CHANNELS];
   XDRV_SINT32 ringVoltDCoffset[APM6838_NUM_CHANNELS];
   XDRV_SINT32 hvgVoltDCoffset[APM6838_NUM_CHANNELS];
   XDRV_SINT32 tipVoltDCcorCommon;
   XDRV_SINT32 ringVoltDCcorCommon;
   XDRV_SINT32 hvgVoltDCcorCommon;
   XDRV_SINT32 currentOffset;
   XDRV_SINT32 updatedOffset;
   XDRV_SINT32 updatedReg;
   TPD_MEASURE_RESULTS tpdResults;
   unsigned int i;
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   int result = XDRV_TPD_SUCCESS;

   /* Perform new calibration if desired */
   if( updateValues )
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Running TPD Bootup Calibration ... \n")); 

      /* Disable the PWM output */
      hvgp->reg_hvg_soft_init |= HVG_PWM_DISABLE;

      /* Start HVG channels */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         hvg6838Start( i );
      }   

      /* Pause for HVG state to stabilize */
      bosSleep(1000);

      /* Measure the voltages and currents */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         linetestReadSignal( i, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE );

         tipVoltDCoffset[i] = tpdResults.tipVoltDC - 475;
         ringVoltDCoffset[i] = tpdResults.ringVoltDC - 475;
         hvgVoltDCoffset[i] = tpdResults.hvgVoltDC - 300;
      }

      /* Stop HVG channels */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         hvg6838Stop( i );
      }   

      /* Enable the PWM output */
      hvgp->reg_hvg_soft_init &= ~HVG_PWM_DISABLE;
   }
   else
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Loading stored calibration values ... \n")); 

      /* Load current values */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         tipVoltDCoffset[i] = results[i].TipOffset;
         ringVoltDCoffset[i] = results[i].RingOffset;
         hvgVoltDCoffset[i] = results[i].HvgOffset;
      }   
   }

   /* Determine tip calibration factors */
   if( max(abs(tipVoltDCoffset[0]),abs(tipVoltDCoffset[1])) < 2000 )
   {
      tipVoltDCcorCommon = max(tipVoltDCoffset[0],tipVoltDCoffset[1]);
      tipVoltDCcor[0] = tipVoltDCoffset[0] - tipVoltDCcorCommon;
      tipVoltDCcor[1] = tipVoltDCoffset[1] - tipVoltDCcorCommon;

      /* Update TIP offsets */
      currentOffset = ( ( hvgp->reg_hvg_vtip_hires_cond & HVG_COND_OFFSET ) >> HVG_COND_OFFSET_SHIFT );
      updatedOffset = currentOffset - (( tipVoltDCcorCommon * 2 ) / 75 ) - (((( tipVoltDCcorCommon * 2 ) % 75 ) * 2 ) / 75 );
      updatedReg    = hvgp->reg_hvg_vtip_hires_cond & ~HVG_COND_OFFSET;
      updatedReg    = updatedReg | ( ( updatedOffset << HVG_COND_OFFSET_SHIFT ) & HVG_COND_OFFSET );
      hvgp->reg_hvg_vtip_hires_cond = updatedReg;

      /* Results */
      results[0].TipOffset = tipVoltDCoffset[0];
      results[1].TipOffset = tipVoltDCoffset[1];
   }
   else
   {
      result = XDRV_TPD_FAILURE;
   }

   /* Determine ring calibration factors */
   if( max(abs(ringVoltDCoffset[0]),abs(ringVoltDCoffset[1])) < 2000 )
   {
      ringVoltDCcorCommon = max(ringVoltDCoffset[0],ringVoltDCoffset[1]);
      ringVoltDCcor[0] = ringVoltDCoffset[0] - ringVoltDCcorCommon;
      ringVoltDCcor[1] = ringVoltDCoffset[1] - ringVoltDCcorCommon;

      /* Update RING offsets */
      currentOffset = ( ( hvgp->reg_hvg_vring_hires_cond & HVG_COND_OFFSET ) >> HVG_COND_OFFSET_SHIFT );
      updatedOffset = currentOffset - (( ringVoltDCcorCommon * 2 ) / 75 ) - (((( ringVoltDCcorCommon * 2 ) % 75 ) * 2 ) / 75 );
      updatedReg    = hvgp->reg_hvg_vring_hires_cond & ~HVG_COND_OFFSET;
      updatedReg    = updatedReg | ( ( updatedOffset << HVG_COND_OFFSET_SHIFT ) & HVG_COND_OFFSET );
      hvgp->reg_hvg_vring_hires_cond = updatedReg;

      /* Results */
      results[0].RingOffset = ringVoltDCoffset[0];
      results[1].RingOffset = ringVoltDCoffset[1];
   }
   else
   {
      result = XDRV_TPD_FAILURE;
   }

   /* Determine hvg calibration factors */
   if( max(abs(hvgVoltDCoffset[0]),abs(hvgVoltDCoffset[1])) < 750 )
   {
      hvgVoltDCcorCommon = ( ( hvgVoltDCoffset[0] + hvgVoltDCoffset[1] ) / 2 );
      hvgVoltDCcor[0] = hvgVoltDCoffset[0] - hvgVoltDCcorCommon;
      hvgVoltDCcor[1] = hvgVoltDCoffset[1] - hvgVoltDCcorCommon;

      /* Update HVG offsets */
      currentOffset = ( ( hvgp->reg_hvg_vhvg_hires_cond & HVG_COND_OFFSET ) >> HVG_COND_OFFSET_SHIFT );
      updatedOffset = currentOffset - (( hvgVoltDCcorCommon * 2 ) / 75 ) - (((( hvgVoltDCcorCommon * 2 ) % 75 ) * 2 ) / 75 );
      updatedReg    = hvgp->reg_hvg_vhvg_hires_cond & ~HVG_COND_OFFSET;
      updatedReg    = updatedReg | ( ( updatedOffset << HVG_COND_OFFSET_SHIFT ) & HVG_COND_OFFSET );
      hvgp->reg_hvg_vhvg_hires_cond = updatedReg;

      /* Results */
      results[0].HvgOffset = hvgVoltDCoffset[0];
      results[1].HvgOffset = hvgVoltDCoffset[1];
   }
   else
   {
      result = XDRV_TPD_FAILURE;
   }

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of Calibration settings/readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hvgVoltDCoffset[0] = %.8d", (int)hvgVoltDCoffset[0]));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hvgVoltDCoffset[1] = %.8d", (int)hvgVoltDCoffset[1]));   

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "tipVoltDCoffset[0] = %.8d", (int)tipVoltDCoffset[0]));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "tipVoltDCoffset[1] = %.8d", (int)tipVoltDCoffset[1]));   

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ringVoltDCoffset[0] = %.8d", (int)ringVoltDCoffset[0]));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ringVoltDCoffset[1] = %.8d", (int)ringVoltDCoffset[1]));

   /* Return result */
   return( result );
}
