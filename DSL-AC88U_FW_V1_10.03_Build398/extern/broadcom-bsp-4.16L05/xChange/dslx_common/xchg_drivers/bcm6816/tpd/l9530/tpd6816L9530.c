/****************************************************************************
*
* Copyright © 2000-2009 Broadcom Corporation
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
*    Filename: tpd6816L9530.c
*    Creation Date: 04 Oct 2000 (v0.00)
*    VSS Info:
*        $Revision: 44 $
*        $Date: 2/21/03 6:56p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the L9530
*      SLIC on the 6816 reference design
*
****************************************************************************/

#include <bosTask.h>
#include <bosSleep.h>
#include <bcm6816.h>
#include <apm6816.h>
#include <hvg6816.h>
#include <xdrvLog.h>
#include <xdrvTimer.h>
#include <xdrvSlic.h>
#include <xdrvApm.h>
#include <xdrvTpd.h>
#include <xdrvGlobalInterrupt.h>
#include <casCtl.h>
#include <boardHalSlic.h>
#include "tpd6816L9530.h"

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
   TPD_MEASURE_DC_CHOP,               /* Measure DC signal only - use chopping */
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

/* ---- Private Constants ------------------------------------ */

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
#define I_SETTLE                             5
#define SETTLE_WAIT_MAX                    100

#define BMU6816_MAX_CURRENT             20000
#define BMU6816_MIN_CURRENT            -20000

/* TPD Driver Pointers */
static XDRV_SLIC *pSlicDriver[TPD_MAX_ENDPT];
static XDRV_APM *pApmDriver[TPD_MAX_ENDPT];
static XDRV_DEBUG *pDebugDriver[TPD_MAX_ENDPT];

/* ---- Private Function Prototypes -------------------------- */

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

XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults );
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL voltage );

XDRV_SINT32 tpdExtend21to32( XDRV_UINT32 value );

void tpdBatteryControl( int chan, TPD_BATTERY_MODE mode );
void tpdDischargeVoltage( int chan );

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
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults )
{
   APM_6816_DRV *pDev = (APM_6816_DRV *)pApmDriver[chan];
   volatile BCM6816_APM *pApm = pDev->pApm;
   volatile BCM6816_HVG *hvgp = BCM6816_HVG_PTR;

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
   XDRV_UINT32 temp_reg_hvg_offset_vtip;
   XDRV_UINT32 temp_reg_hvg_offset_vhvg;
   XDRV_UINT32 temp_reg_hvg_offset_vring;
   XDRV_UINT32 temp_reg_stats_config;
   XDRV_UINT32 temp_reg_cha_cic_config;
   XDRV_UINT32 temp_reg_codec_config_2;
   XDRV_UINT32 temp_reg_hvg_cha_misc;
   XDRV_UINT32 temp_reg_hvg_cha_window_ctrl;
   XDRV_UINT32 temp_reg_chb_cic_config;
   XDRV_UINT32 temp_reg_codec_config_3;
   XDRV_UINT32 temp_reg_hvg_chb_misc;
   XDRV_UINT32 temp_reg_hvg_chb_window_ctrl;

   /* Store previous settings */
   temp_reg_apm_coeff_config = pApm->reg_apm_coeff_config;
   temp_reg_hvg_offset_vtip = hvgp->reg_hvg_offset_vtip;
   temp_reg_hvg_offset_vhvg = hvgp->reg_hvg_offset_vhvg;
   temp_reg_hvg_offset_vring = hvgp->reg_hvg_offset_vring;
   temp_reg_stats_config = pApm->reg_stats_config;
   temp_reg_cha_cic_config = pApm->reg_cha_cic_config;
   temp_reg_codec_config_2 = pApm->reg_codec_config_2;
   temp_reg_hvg_cha_misc = hvgp->reg_hvg_cha_misc;
   temp_reg_hvg_cha_window_ctrl = hvgp->reg_hvg_cha_window_ctrl;
   temp_reg_chb_cic_config = pApm->reg_chb_cic_config;
   temp_reg_codec_config_3 = pApm->reg_codec_config_3;
   temp_reg_hvg_chb_misc = hvgp->reg_hvg_chb_misc;
   temp_reg_hvg_chb_window_ctrl = hvgp->reg_hvg_chb_window_ctrl;

   /* Setup global registers for measurements */
   pApm->reg_apm_coeff_config &= ~COEFF_PROG_ADDR;
   pApm->reg_apm_coeff_config |= 0x04;
   hvgp->reg_hvg_offset_vtip = 0x180F;
   hvgp->reg_hvg_offset_vhvg = 0x1779;
   hvgp->reg_hvg_offset_vring = 0x180F;

   /* Need to allow settling for TESTO changes */
   bosSleep( 10 );

   /* Setup channel and measurement specific registers */
   if( chan == 0 )
   {
      pApm->reg_cha_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_cha_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );
      pApm->reg_codec_config_2 &= ~PGA_CTL_VADC;
      pApm->reg_codec_config_2 |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      hvgp->reg_hvg_cha_misc &= ~CONT_OR_BLOCK;

      if( measureType == TPD_MEASURE_DC_CHOP )
      {
         pApm->reg_stats_config = 0x00008698;
         pApm->reg_stats_config = 0x00000698;
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
         hvgWindowSize = 0x0887888A;
      }
      else
      {
         hvgWindowSize = 0x0BB7BBBD;
      }

      hvgp->reg_hvg_cha_window_ctrl = ( hvgWindowSize );
      hvgp->reg_hvg_cha_window_ctrl = ( hvgWindowSize | NEW_BLK_RQST );
      hvgp->reg_hvg_cha_window_ctrl = ( hvgWindowSize );
   }
   else
   {
      /* Utilize audio ADC for current measurements on 6816 parts */
      pApm->reg_chb_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_chb_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );
      pApm->reg_codec_config_3 &= ~( PGA_CTL_VADC );
      pApm->reg_codec_config_3 |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );

      hvgp->reg_hvg_chb_misc &= ~CONT_OR_BLOCK;

      /* Utilize audio ADC for current measurements on 6816 parts */
      if( measureType == TPD_MEASURE_DC_CHOP )
      {
         pApm->reg_stats_config = 0x00008998;
         pApm->reg_stats_config = 0x00000998;
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
         hvgWindowSize = 0x0887888A;
      }
      else
      {
         hvgWindowSize = 0x0BB7BBBD;
      }

      hvgp->reg_hvg_chb_window_ctrl = ( hvgWindowSize );
      hvgp->reg_hvg_chb_window_ctrl = ( hvgWindowSize | NEW_BLK_RQST );
      hvgp->reg_hvg_chb_window_ctrl = ( hvgWindowSize );
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
      tpdResults->tipVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status10 & TIP_MEAN ))) * 75 / 256;
      tpdResults->ringVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status12 & RING_MEAN ))) * 75 / 256;
      tpdResults->tipRingVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status14 & TP_RG_MEAN ))) * 75 / 256;
      tpdResults->hvgVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status8 & HVG_MEAN ))) * 75 / 256;

      /* Utilize audio ADC for current measurements on 6816 parts */
      iLoopDC = ((XDRV_SINT32)( pApm->reg_apm_stats_1 ));
      tpdResults->iLoopDCRaw = iLoopDC;

      /* Calculate mean square results */
      tipMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status24 )) * 5625, 256);
      ringMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status26 )) * 5625, 256);
      tipRingMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status28 )) * 5625, 256);
      hvgMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status22 )) * 5625, 256);

      /* Utilize audio ADC for current measurements on 6816 parts */
      iLoopMeanSquare = (((((XDRV_UINT64)( pApm->reg_apm_stats_4 )) & MEAN_SQUARE_HI ) << 32 ) | ((XDRV_UINT64)( pApm->reg_apm_stats_3 )));
      iLoopMeanSquare = iLoopMeanSquare * statsDuration;
   }
   else
   {
      /* Calculate DC results */
      tpdResults->tipVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status11 & TIP_MEAN ))) * 75 / 256;
      tpdResults->ringVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status13 & RING_MEAN ))) * 75 / 256;
      tpdResults->tipRingVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status15 & TP_RG_MEAN ))) * 75 / 256;
      tpdResults->hvgVoltDC = ( tpdExtend21to32((XDRV_UINT32)( hvgp->reg_hvg_status9 & HVG_MEAN ))) * 75 / 256;

      /* Utilize audio ADC for current measurements on 6816 parts */
      iLoopDC = ((XDRV_SINT32)( pApm->reg_apm_stats_2 ));
      tpdResults->iLoopDCRaw = iLoopDC;

      /* Calculate mean square results */
      tipMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status25 )) * 5625, 256);
      ringMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status27 )) * 5625, 256);
      tipRingMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status29 )) * 5625, 256);
      hvgMeanSquare = apmDiv(((XDRV_UINT64)( hvgp->reg_hvg_status23 )) * 5625, 256);

      /* Utilize audio ADC for current measurements on 6816 parts */
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

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "%.8ld, %.8ld, %.8ld, %.8ld, %.8llu, %.8llu, %.8llu, %.8llu, %.8lld, %.8llu, %.8lld",
      tpdResults->tipVoltDC, tpdResults->ringVoltDC, tpdResults->tipRingVoltDC, tpdResults->iLoopDC,
      tipMeanSquare, ringMeanSquare, tipRingMeanSquare, hvgMeanSquare,
      iLoopDC, iLoopMeanSquare, iLoopACSquared ));

   /* Adjust the current measurements */
   tpdResults->iLoopDC = apmDiv(iLoopDC * 875, ( statsDuration << 8 ));
   tpdResults->iLoopAC = apmDiv(iLoopAC * 875, ( statsDuration << 8 ));

   /* Restore previous settings */
   pApm->reg_apm_coeff_config = temp_reg_apm_coeff_config;
   hvgp->reg_hvg_offset_vtip = temp_reg_hvg_offset_vtip;
   hvgp->reg_hvg_offset_vhvg = temp_reg_hvg_offset_vhvg;
   hvgp->reg_hvg_offset_vring = temp_reg_hvg_offset_vring;
   pApm->reg_stats_config = temp_reg_stats_config;

   if( chan == 0 )
   {
      pApm->reg_cha_cic_config = temp_reg_cha_cic_config;
      pApm->reg_codec_config_2 = temp_reg_codec_config_2;
      hvgp->reg_hvg_cha_misc = temp_reg_hvg_cha_misc;
      hvgp->reg_hvg_cha_window_ctrl = temp_reg_hvg_cha_window_ctrl;
   }
   else
   {
      pApm->reg_chb_cic_config = temp_reg_chb_cic_config;
      pApm->reg_codec_config_3 = temp_reg_codec_config_3;
      hvgp->reg_hvg_chb_misc = temp_reg_hvg_chb_misc;
      hvgp->reg_hvg_chb_window_ctrl = temp_reg_hvg_chb_window_ctrl;
   }

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
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL voltage )
{
   XDRV_UINT32 attempts;
   XDRV_SINT32 threshold, delta, currentValue, previousValue;

   attempts = SETTLE_WAIT_MAX;
   previousValue = BMU6816_MAX_CURRENT;
   threshold = ( voltage ? V_SETTLE : I_SETTLE );
   delta = threshold;

   while( delta >= threshold )
   {
      bosSleep( 70 );

      /* Determine if the maximum number of attempts has occurred */
      if( attempts == 0 )
      {
         return( XDRV_TPD_FAILURE );
      }

      /* Read the voltages and currents */
      if ( linetestReadSignal( chan, measureType, tpdResults ) )
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
**             Force tests on a specifeid channel
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
   tpdDischargeVoltage( chan );

   /* Measure signal to verify foreign/hazard levels */
   if ( linetestReadSignal( chan, TPD_MEASURE_ACDC, &tpdResults ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   hpfefRsltp->tipAC = tpdResults.tipVoltAC;
   hpfefRsltp->tipDC = tpdResults.tipVoltDC;
   hpfefRsltp->ringAC = tpdResults.ringVoltAC;
   hpfefRsltp->ringDC = tpdResults.ringVoltDC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** "));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of HAZ & FOR V readings ... "));

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: tipAC  = %.8ld, tipDC  = %.8ld ", hpfefRsltp->tipAC, hpfefRsltp->tipDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ringAC = %.8ld, ringDC = %.8ld ", hpfefRsltp->ringAC, hpfefRsltp->ringDC));

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
   if ( chan == 0 )
   {
      GPIO->GPIODir |= LINE_OFFHOOK0;
      GPIO->GPIOio |= LINE_OFFHOOK0;
   }
   else
   {
      GPIO->GPIODir |= LINE_OFFHOOK1;
      GPIO->GPIOio |= LINE_OFFHOOK1;
   }

   /* Pause to allow CAS to detect and signal offhook condition */
   bosSleep( 2000 );

   /* Disengage off hook simulation hardware */
   if ( chan == 0 )
   {
      GPIO->GPIODir |= LINE_OFFHOOK0;
      GPIO->GPIOio &= ~LINE_OFFHOOK0;
   }
   else
   {
      GPIO->GPIODir |= LINE_OFFHOOK1;
      GPIO->GPIOio &= ~LINE_OFFHOOK1;
   }

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

   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));

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
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "No ROH readings necessary "));

      rohRsltp->phoneOnhook = XDRV_TRUE;
      return ( XDRV_TPD_SUCCESS );
   }

   /* Allow voltages to settle further */
   bosSleep( 300 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_MEASURE_DC, &tpdResults ))
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Save desired results */
   lowTipRingDC = tpdResults.tipRingVoltDC;
   lowCurrentDC = tpdResults.iLoopDC;

   /* Change loop current limit to 40mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_TRUE );

   /* Allow voltages to settle */
   bosSleep( 500 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_MEASURE_DC, &tpdResults ))
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Save desired results */
   highTipRingDC = tpdResults.tipRingVoltDC;
   highCurrentDC = tpdResults.iLoopDC;

   /* Change loop current limit to previous setting */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_FALSE );

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
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ROH readings ... "));

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowTipRingDC = %.8ld ", lowTipRingDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: highTipRingDC = %.8ld ", highTipRingDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowCurrentDC = %.8ld ", lowCurrentDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: highCurrentDC = %.8ld ", highCurrentDC));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: lowRes = %.8d ", lowRes));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "roh: difRes = %.8d ", difRes));

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
   XDRV_SINT32 testMode;

   XDRV_SINT64 Rtr,Rtg,Rrg;
   XDRV_SINT64 Rdiv;

   TPD_MEASURE_RESULTS tpdResults;

   /* Define initial values to force 1st pass through measurement loops */
   IMTfh = BMU6816_MAX_CURRENT;
   IMTrh = BMU6816_MAX_CURRENT;
   IMTfl = BMU6816_MAX_CURRENT;
   IMTrl = BMU6816_MAX_CURRENT;
   VToh = BMU6816_MAX_CURRENT;
   VTol = BMU6816_MAX_CURRENT;

   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));

   /* Allow Test Mode to vary according to chip/block-level support */
   testMode = TPD_MEASURE_DC_CHOP;

   /* Change SLIC to High Battery On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHT );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_FALSE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VTfh = tpdResults.tipVoltDC;
   VRfh = tpdResults.ringVoltDC;
   IMTfh = tpdResults.iLoopDC;

   /* Change SLIC to High Battery Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );

   bosSleep( 100 );

   /* Wait for tip voltage to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_TRUE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VToh = tpdResults.tipVoltDC;
   VRoh = tpdResults.ringVoltDC;
   IMToh = tpdResults.iLoopDC;

   /* Change SLIC to High Battery Reverse On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHTR );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_FALSE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VTrh = tpdResults.tipVoltDC;
   VRrh = tpdResults.ringVoltDC;
   IMTrh = tpdResults.iLoopDC;

   /* Change SLIC to Low Battery Reverse On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHTR );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_FALSE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VTrl = tpdResults.tipVoltDC;
   VRrl = tpdResults.ringVoltDC;
   IMTrl = tpdResults.iLoopDC;

   /* Change SLIC to Low Battery On Hook Transmission */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHT );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_FALSE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VTfl = tpdResults.tipVoltDC;
   VRfl = tpdResults.ringVoltDC;
   IMTfl = tpdResults.iLoopDC;

   /* Change SLIC to Low Battery Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   bosSleep( 100 );

   /* Wait for tip voltage to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, testMode, &tpdResults, XDRV_TRUE ) )
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Store results */
   VTol = tpdResults.tipVoltDC;
   VRol = tpdResults.ringVoltDC;
   IMTol = tpdResults.iLoopDC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** "));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ResFLT readings ... "));

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTrl = %.8ld,VRrl = %.8ld ,IMTrl = %.8ld ", VTrl,VRrl,IMTrl));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTfl = %.8ld,VRfl = %.8ld ,IMTfl = %.8ld ", VTfl,VRfl,IMTfl));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTol = %.8ld,VRol = %.8ld ,IMTol = %.8ld ", VTol,VRol,IMTol));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTrh = %.8ld,VRrh = %.8ld ,IMTrh = %.8ld ", VTrh,VRrh,IMTrh));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTfh = %.8ld,VRfh = %.8ld ,IMTfh = %.8ld ", VTfh,VRfh,IMTfh));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VToh = %.8ld,VRoh = %.8ld ,IMToh = %.8ld", VToh,VRoh,IMToh));

   /* Calculate tip-ground impedance */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTrh - IMTrl ) * ( XDRV_SINT64 )(( VRol - VRoh ) - ( VTol - VToh )));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rtg = 1000 * apmDiv((( XDRV_SINT64 )( VTrh - VTrl ) * ( XDRV_SINT64 )(( VRol - VRoh ) + ( VTol - VToh ))), Rdiv);

   /* Calculate tip-ring impedance */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTrh - IMTrl ) * apmDiv(Rtg, 1000) - ( XDRV_SINT64 )( VTrh - VTrl ));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rtr = apmDiv(( 2 * ( XDRV_SINT64 )( VTrh - VTrl ) * Rtg ), Rdiv);

   /* Calculate ring-ground impedance */
   Rdiv = ( 2 * ( XDRV_SINT64 )( IMTfh - IMTfl ) * apmDiv(Rtr, 1000) - 2 * ( XDRV_SINT64 )( VRfl - VRfh ));
   if ( Rdiv == 0 )
   {
      Rdiv = 1;
   }
   Rrg = apmDiv((( XDRV_SINT64 )( VRfl - VRfh ) * Rtr ), Rdiv);

   /* Check for invalid measurments due to current limiting */
   if(( IMTrh < BMU6816_MIN_CURRENT ) &&
      ( IMTfh > BMU6816_MAX_CURRENT ))
   {
      Rtg = 1000000;
      Rrg = 1000000;
      Rtr = 0;
   }
   else if (( abs(IMTrl) > ( BMU6816_MAX_CURRENT ) ) &&
            ( abs(IMTrh) > ( BMU6816_MAX_CURRENT ) ) &&
            ( abs(IMTol) < ( BMU6816_MAX_CURRENT / 2 ) ) &&
            ( abs(IMToh) < ( BMU6816_MAX_CURRENT / 2 ) ))
   {
      Rrg = 1000000;
      Rtr = 1000000;
      Rtg = 0;
   }
   else if (( abs(IMTfl) > ( BMU6816_MAX_CURRENT ) ) &&
            ( abs(IMTfh) > ( BMU6816_MAX_CURRENT ) ) &&
            ( abs(IMTol) > ( BMU6816_MAX_CURRENT / 2 ) ) &&
            ( abs(IMToh) > ( BMU6816_MAX_CURRENT / 2 ) ))
   {
      Rtg = 1000000;
      Rtr = 1000000;
      Rrg = 0;
   }

   /* Save calculated impedances */
   rfltRsltp->tipGroundImpedance = Rtg;
   rfltRsltp->ringGroundImpedance = Rrg;
   rfltRsltp->tipRingImpedance = Rtr;

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

   bosSleep( 200 );

   if ( linetestReadSignal( chan, TPD_MEASURE_ACDC, &tpdResults ))
   {
      return (XDRV_TPD_FAILURE);
   }

   /* Save desired results */
   tipRingVoltAC = tpdResults.tipRingVoltAC;
   AcCurrent = tpdResults.iLoopAC;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** "));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of RING readings ... "));

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: trVac = %.8ld ", tipRingVoltAC ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: Iac  = %.8ld ", AcCurrent ));

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
** FUNCTION:   tpd6816InitChannel
**
** PURPOSE:    TPD initialization - called once for each channel during initialization
**
** PARAMETERS: chan - channel to initialize
**             slicDrv - pointer to SLIC driver
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
int tpd6816InitChannel( int chan, XDRV_SLIC *slicDrv, XDRV_APM *apmDrv, XDRV_DEBUG *debugDrv )
{
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
XDRV_TPD_STATUS tpdRunDropTests( int chan, int cmd, HSZDROPRSLT *dropRsltp )
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
XDRV_TPD_STATUS tpdRunSelfTests( int chan, int cmd, HSZSELFRSLT *selfRsltp )
{
   if( cmd & XDRV_TPD_RUN_SELF_HVG )
   {
      linetestSelfHvg( chan, &selfRsltp->selfHvgPassed );
   }

   if( cmd & XDRV_TPD_RUN_SELF_APM )
   {
      linetestSelfApm( chan, &selfRsltp->selfApmPassed );
   }

   if( cmd & XDRV_TPD_RUN_SELF_LOOP ||
       cmd & XDRV_TPD_RUN_SELF_VOLTAGE )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, 
                       "Self-tests %d and %d not supported", 
                       XDRV_TPD_RUN_SELF_LOOP, XDRV_TPD_RUN_SELF_LOOP));
   }

   if( cmd & XDRV_TPD_RUN_SELF_RING )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Self-test %d not supported", XDRV_TPD_RUN_SELF_RING));
   }

   if( cmd & XDRV_TPD_RUN_SELF_CAP )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Self-test %d not supported", XDRV_TPD_RUN_SELF_CAP));
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
XDRV_TPD_STATUS tpdTestingSetup( int chan )
{
   /* Disable CAS */
   tpdCasDisable( chan );

   /* Return SLIC to default hook state */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

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
**             isEnabled - boolean indicating admin status of this line
**                         (enabled = 1; disabled = 0)
**
** RETURNS:    XDRV_TPD_STATUS
**
*****************************************************************************
*/
XDRV_TPD_STATUS tpdTestingShutdown( int chan, XDRV_BOOL isEnabled )
{
   /*
   ** Need to put the line into a correct state based on current admin status.
   ** If the line was administratively disabled, we want to disconnect the physical
   ** interface and if it was enabled, we want to restore to regular operating state
   */

   if (isEnabled)
   {
      if ( !hvg6816IsEnabled( chan ) )
      {
         /* HVG is disabled - need to restart it */
         hvg6816Start( chan );
      }

      /* Reenable CAS */
      tpdCasEnable( chan );
   }
   else
   {
      /* Cut the battery voltage on this channel */
      hvg6816Stop(chan);

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
   volatile BCM6816_APM *pApm = BCM6816_APM_PTR;

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
   volatile BCM6816_APM *pApm = BCM6816_APM_PTR;

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
XDRV_TPD_STATUS tpdChanReset( int chan )
{
   XCHG_ASSERT( chan < TPD_MAX_ENDPT );

   /* Discharge any stored voltage on this line */
   tpdDischargeVoltage( chan );

   /* Now go to sleep for effect. */
   if( bosSleep( 1000 ) == BOS_STATUS_RESET )
   {
      /* We're being reset. Abort. */
      return XDRV_TPD_ERR_ABORT;
   }

   /* Reset the HVG digital logic while disconnected */
   hvg6816Reset(chan);

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
**
** RETURNS:    Nothing
**
*****************************************************************************
*/
void tpdDischargeVoltage( int chan )
{
   int tempRingFrequency;
   int tempRingWaveshape;
   int tempRingVoltage;
   int tempRingOffset;
   int tempRingOffsetCal;

   /* Make tip and ring the same voltage to discharge any voltage across an REN load */
   xdrvApmGetRingParms( pApmDriver[chan], &tempRingFrequency, &tempRingWaveshape, &tempRingVoltage, &tempRingOffset, &tempRingOffsetCal );
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, 0, 0, tempRingOffsetCal );

   /* Change to ringing to discharge voltage on load */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_TEST1 );

   /* Pause for stray voltages to discharge */
   bosSleep( 400 );

   /* Change to disconnected mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO );
   tpdBatteryControl( chan, TPD_BATTERY_MODE_MED );

   /* Reset Ring Voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   bosSleep( 900 );
}
