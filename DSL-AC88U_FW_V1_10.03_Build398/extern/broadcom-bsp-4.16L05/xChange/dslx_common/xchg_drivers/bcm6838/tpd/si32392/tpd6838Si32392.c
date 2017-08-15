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
*    Filename: tpd6838Si32392.c
*    Creation Date: 04 Oct 2000 (v0.00)
*    VSS Info:
*        $Revision: 44 $
*        $Date: 2/21/03 6:56p $
*
****************************************************************************
*    Description:
*
*      This file contains functions related to linetests for the Si32392
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
#include "tpd6838Si32392.h"
#include "slic6838Si32392.h"
#include "slic6838Si32392Defs.h"
#include <boardHalSlic.h>
#include <tpd6838si32392Cfg.h>

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

typedef enum TPD_SIGNAL_TYPE
{
   TPD_SIGNAL_NORMAL,
   TPD_SIGNAL_OFFSET,
   TPD_SIGNAL_TRES,
   TPD_SIGNAL_TRES1

} TPD_SIGNAL_TYPE;

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

/* Protection resistore conversion factores */
#define BMU6838_SCALE_TRES                   1000

/* Resistive faults test threshholds */
#define RES_FLT_CURRENT                      1800
#define RES_FLT_VOLTAGE                      2000

/* REN test threshholds */
#define DROP_RING_REN_THRESH                10000

/* HP/FEF test threshholds */
#define TPD_MIN_HIRES_VOLTAGE              -60000
#define TPD_MAX_HIRES_VOLTAGE                1500

/* Number of valid channels for testing */
#define TPD_MAX_ENDPT                           4

/* TPD Test Defines */

#define BMU6838_SCALE_ILO                     200      /* In mv / mA */
#define BMU6838_SCALE_IRL                     100      /* In mv / mA */
#define BMU6838_SCALE_IHI                      10      /* In mv / mA */
#define BMU6838_SCALE_IRH                       5      /* In mv / mA */
#define V_SETTLE                              100
#define I_SETTLE                                3
#define SETTLE_WAIT_MAX                       500

#define BMU6838_MAX_CURRENT                 20000
#define BMU6838_MIN_CURRENT                -20000

#define CURRENT_SAMPLING_COEFFICIENT        16780
/* Alternative to defining scales is to define correction factors and calculate scales */
#define COND_SCALE_I_LOOP_HI                14634      /* 0x392a */
#define COND_SCALE_I_LOOP_LO                14545      /* 0x38d1 */

/* TPD Driver Pointers */
static XDRV_SLIC *pSlicDriver[TPD_MAX_ENDPT];
static XDRV_APM *pApmDriver[TPD_MAX_ENDPT];
static XDRV_DEBUG *pDebugDriver[TPD_MAX_ENDPT];

static XDRV_SINT32 tipVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static XDRV_SINT32 ringVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static XDRV_SINT32 hvgVoltDCcor[APM6838_NUM_CHANNELS] = { 0, 0 };
static XDRV_UINT64 condCurOffset[TPD_MAX_ENDPT] = { 0, 0 };

static TPD_CALIBRATION_RESULTS tpdVoltCalibrationDCOffset[APM6838_NUM_CHANNELS] = 
{
   /* tip,   ring */
  { -1260,   -1100 },
  {   350,    -650 }
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
static const XDRV_TPD_FUNCS tpd6838Si32392DrvFuncs =
{
   tpdRunDropTests,
   tpdRunSelfTests,
   tpdTestingSetup,
   tpdTestingShutdown,
   tpdChanReset
};

/* Private Functions */
static XDRV_UINT16 linetestDropHazardForeign(int chan, HSZDROPRSLT_HPFEF *hpfefRsltp);
static XDRV_UINT16 linetestDropROH(int chan, HSZDROPRSLT_ROH *rohRsltp);
static XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp);
static XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp);
static XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp);
static XDRV_UINT16 linetestDropOffhook(int chan);

static XDRV_UINT16 linetestSelfAssert(int chan, XDRV_SINT32 *selfAssertPassed);
static XDRV_UINT16 linetestSelfDspCount(int chan, XDRV_SINT32 *selfDspCountPassed);
static XDRV_UINT16 linetestSelfHvg(int chan, XDRV_SINT32 *selfHvgPassed);
static XDRV_UINT16 linetestSelfApm(int chan, XDRV_SINT32 *selfApmPassed);

static XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 signalType, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL ringing, XDRV_BOOL lowRes );
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 signalType, XDRV_UINT32 measureType, TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL ringing, XDRV_BOOL voltage, XDRV_BOOL lowRes );

static XDRV_SINT32 tpdExtend21to32( XDRV_UINT32 value );

static void tpdBatteryControl( int chan, TPD_BATTERY_MODE mode );
static void tpdDischargeVoltage( int chan );

static void tpdEnableYFilter( int chan );
static void tpdDisableYFilter( int chan );

static void tpdCasEnable( int chan );
static void tpdCasDisable( int chan );

static void tpdMeasureOffset( int chan );

extern XDRV_UINT64 vrgISqrt(XDRV_UINT64 x);

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
static XDRV_SINT32 tpdExtend21to32( XDRV_UINT32 value )
{
   return (XDRV_SINT32)( value & 0x00100000 ? (XDRV_UINT32)value | 0xFFE00000 : (XDRV_UINT32)value );
}

/*
*****************************************************************************
** FUNCTION:   ignEnable
**
** PURPOSE:    Enable/Disable IGN sense bits.
**
** PARAMETERS: chan   - channel number, zero based
**             enable - true or false
**
** RETURNS:    SUCCESS or FAILURE
** 
** NOTE:
*****************************************************************************
*/
static XDRV_SINT16 ignEnable( int chan, XDRV_BOOL enable )
{
   XDRV_UINT16 testMode = 0;
   
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );

   /* Retrieve the current loop current mode */
   slic6838Si32392RegisterRead( pDrv, SI32392_REG_LINEFEED_CONFIG_3, &testMode );

   /* Only modify IGN settings bits */
   if( chan % 2 )
   {
      if( enable )
      {
         testMode |= SI32392_TEST_IGN2;
      }
      else
      {
         testMode &= ~SI32392_TEST_IGN2;
      }
   }
   else
   {
      if( enable )
      {
         testMode |= SI32392_TEST_IGN1;
      }
      else
      {
         testMode &= ~SI32392_TEST_IGN1;
      }
   }

   /* Store the current loop current mode */
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, testMode );

   return ( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   ignEnabled
**
** PURPOSE:    read IGN on/off
**
** PARAMETERS: chan - channel number, zero based
**
** RETURNS:    TRUE or FALSE
** 
** NOTE:
*****************************************************************************
*/
static XDRV_BOOL ignEnabled( int chan )
{
   XDRV_UINT16 testMode = 0;

   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   /* Retrieve the current loop current mode */
   slic6838Si32392RegisterRead( pDrv, SI32392_REG_LINEFEED_CONFIG_3, &testMode );

   /* Check the IGN bits */
   if( chan % 2 )
   {
      if( testMode & SI32392_TEST_IGN2 )
      {
         return ( XDRV_TRUE );
      }
   }
   else
   {
      if( testMode & SI32392_TEST_IGN1 )
      {
         return ( XDRV_TRUE );
      }
   }

   return ( XDRV_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   linetestReadSignal
**
** PURPOSE:    Samples the selected signal and computes the AC and DC value
**             of the requested signal.
**
** PARAMETERS: chan        - chan to measure
**             signalType  - type of signal to measure
**             measureType - DC only or AC & DC A/D measurements
**             tpdResults  - pointer to measured values
**             ringing     - current ring state
**             lowRes       - use low resolution or high resolution
**
** RETURNS:    SUCCESS or FAILURE
** 
** NOTE:
*****************************************************************************
*/
static XDRV_SINT16 linetestReadSignal( int chan, XDRV_UINT32 signalType, XDRV_UINT32 measureType,
                                       TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL ringing, XDRV_BOOL lowRes )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];
   volatile BCM6838_APM *pApm = pDev->pApm;
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   volatile XDRV_UINT32* pRegApmAudioAdcA;
   volatile XDRV_UINT32* pRegApmAudioAdcB;
   volatile XDRV_UINT32* pRegApmDiagTypeA;
   volatile XDRV_UINT32* pRegApmDiagTypeB;
   XDRV_BOOL   ignActive;
   XDRV_UINT16 testMode;
   XDRV_UINT64 tipMeanSquare;
   XDRV_UINT64 ringMeanSquare;
   XDRV_UINT64 tipRingMeanSquare;
   XDRV_UINT64 hvgMeanSquare;
   XDRV_UINT64 iLoopMeanSquare;
   XDRV_SINT64 iLoopACSquared;
   XDRV_SINT64 iLoopAC;
   XDRV_SINT64 iLoopDC;
   XDRV_SINT32 measurementSign;
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

   pRegApmAudioAdcA = &pApm->reg_codec_config_2;
   pRegApmAudioAdcB = &pApm->reg_codec_config_3;
   pRegApmDiagTypeA = &pApm->reg_codec_config_6;
   pRegApmDiagTypeB = &pApm->reg_codec_config_6;
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

   /* Retrieve the current state of the IGN */
   ignActive = ignEnabled( chan );

   /* Retrieve current state of the test mode register */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterRead( pDrv, SI32392_REG_LINEFEED_CONFIG_3, &testMode );
   testMode &= ~SI32392_TEST_MASK;

   /* Setup the appropriate test mode */
   if(( signalType == TPD_SIGNAL_TRES ) ||
      ( signalType == TPD_SIGNAL_OFFSET ))
   {
      testMode |= SI32392_TEST_TRES;
   }
   else if( signalType == TPD_SIGNAL_TRES1 )
   {
      testMode |= SI32392_TEST_REF;
   }
   else
   {
      testMode |= SI32392_TEST_LOOP_CURRENT_SENSE;
   }

   /* Set test mode to correct channel. Save original settings for after testing. */
   testMode |= ( XDRV_UINT16 )( ( chan % 2 ) << SI32392_TEST_CHAN_SHIFT );
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, testMode );
   testMode &= ~SI32392_TEST_MASK;

   /* Setup global registers for measurements */
   pApm->reg_apm_coeff_config &= ~COEFF_PROG_ADDR;
   pApm->reg_apm_coeff_config |= 0x04;

   /* Need to allow settling for TESTO changes */
   bosSleep( 10 );

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   /* Setup channel and measurement specific registers */
   if( chan == 0 )
   {
      /* Setup the appropriate test mode */
      if(( signalType == TPD_SIGNAL_TRES ) ||
         ( signalType == TPD_SIGNAL_TRES1 ))
      {
         *pRegApmDiagTypeA &= ~DIAG_TYPE_VADC_A;
         *pRegApmDiagTypeA |= (( 0x05 << DIAG_TYPE_VADC_A_SHIFT ) & DIAG_TYPE_VADC_A );
         *pRegApmAudioAdcA &= ~PGA_CTL_VADC;
         *pRegApmAudioAdcA |= (( 0x02 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      }
      else
      {
         *pRegApmDiagTypeA &= ~DIAG_TYPE_VADC_A;
         *pRegApmAudioAdcA &= ~PGA_CTL_VADC;
         *pRegApmAudioAdcA |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      }

      /* Utilize audio ADC for current measurements on 6838 parts */
      pApm->reg_cha_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_cha_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );
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
      /* Setup the appropriate test mode */
      if(( signalType == TPD_SIGNAL_TRES ) ||
         ( signalType == TPD_SIGNAL_TRES1 ))
      {
         *pRegApmDiagTypeB &= ~DIAG_TYPE_VADC_B;
         *pRegApmDiagTypeB |= (( 0x05 << DIAG_TYPE_VADC_B_SHIFT ) & DIAG_TYPE_VADC_B );
         *pRegApmAudioAdcB &= ~( PGA_CTL_VADC );
         *pRegApmAudioAdcB |= (( 0x02 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      }
      else
      {
         *pRegApmDiagTypeB &= ~DIAG_TYPE_VADC_B;
         *pRegApmAudioAdcB &= ~( PGA_CTL_VADC );
         *pRegApmAudioAdcB |= (( 0x00 << PGA_CTL_VADC_SHIFT ) & PGA_CTL_VADC );
      }

      /* Utilize audio ADC for current measurements on 6838 parts */
      pApm->reg_chb_cic_config &= ~CIC_DEC_SHFT;
      pApm->reg_chb_cic_config |= (( 0x05 << CIC_DEC_SHFT_SHIFT ) & CIC_DEC_SHFT );

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
         pApm->reg_stats_config = 0x000088C0;
         pApm->reg_stats_config = 0x000008C0;
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

   /* Sleep to allow setup/measurements to complete */
   if(( signalType == TPD_SIGNAL_TRES ) || ( signalType == TPD_SIGNAL_TRES1 ))
   {
      measurementSign = 1;
   }
   else
   {
      measurementSign = -1;
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
      iLoopDC = ((XDRV_SINT64)( pApm->reg_apm_stats_1 )) * ( measurementSign );
      tpdResults->iLoopDCRaw = pApm->reg_apm_stats_1 * ( measurementSign );

      /* Calculate mean square results */
      tipMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status25 )) * 5625 / 4 / 256;
      ringMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status27 )) * 5625 / 4 / 256;
      tipRingMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status29 )) * 5625 / 4 / 256;
      hvgMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status23 )) * 5625 / 4 / 256;

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
      iLoopDC = ((XDRV_SINT64)( pApm->reg_apm_stats_2 )) * ( measurementSign );
      tpdResults->iLoopDCRaw = pApm->reg_apm_stats_2 * ( measurementSign );

      /* Calculate mean square results */      
      tipMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status26 )) * 5625 / 4 / 256;
      ringMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status28 )) * 5625 / 4 / 256;
      tipRingMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status30 )) * 5625 / 4 / 256;
      hvgMeanSquare = ((XDRV_UINT64)( hvgp->reg_hvg_status24 )) * 5625 / 4 / 256;

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
   /* Values may change upon receiving final gain values for ADC and SLIC */
   /* Current values from 6838 AMS and Si32392 datasheet */
   /* ADC FS increased by 2 dB from 1.68V to  1.68 x 1.2589 */
   apm6838CalculateKadjValues( &kFactorAdjNum, &kFactorAdjDiv, &kDacAdjNum, &kDacAdjDiv );
   if( ignActive )
   {
      if( ringing )
      {
         iLoopDC = iLoopDC - condCurOffset[chan];
         /* SLIC gain = 107 (vs 82.2) ; */
         /* tpdResults->iLoopDC = iLoopDC * 13125 * 2.148 / 1.68  / ( statsDuration << 9 ) / 107 ; simplifies to */
         tpdResults->iLoopDC = apmDiv(apmDiv(apmDiv(iLoopDC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv),  statsDuration << 9 ),107);
         tpdResults->iLoopAC = apmDiv(apmDiv(apmDiv(iLoopAC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv),  statsDuration << 9 ),107);
		 
      }
      else
      {
         iLoopDC = iLoopDC - condCurOffset[chan];
         /* SLIC gain = 202.25 (vs 165) ; */
       /* tpdResults->iLoopDC = iLoopDC * 13125 * 2.148 / 1.68 / ( statsDuration << 9 ) / 202.25 ; simplifies to */
         tpdResults->iLoopDC = apmDiv(apmDiv(apmDiv(iLoopDC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 20225);
         tpdResults->iLoopAC = apmDiv(apmDiv(apmDiv(iLoopAC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 20225);
      }
   }
   else
   {
      if( ringing )
      {
         iLoopDC = iLoopDC - condCurOffset[chan];
         /* SLIC gain = 5.37 (vs 5.025) ; */
         /* tpdResults->iLoopDC = iLoopDC * 13125 * 2.148 / 1.68 / ( statsDuration << 9 ) / 5.37 ; simplifies to */
         tpdResults->iLoopDC = apmDiv(apmDiv(apmDiv(iLoopDC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 537);
         tpdResults->iLoopAC = apmDiv(apmDiv(apmDiv(iLoopAC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 537);

      }
      else
      {
         iLoopDC = iLoopDC - condCurOffset[chan];
         /* SLIC gain = 10.05 (vs 10.25) ; */
         /* tpdResults->iLoopDC = iLoopDC * 13125 * 2.148 / 1.68 / ( statsDuration << 9 ) / 10.05 ; simplifies to */
         tpdResults->iLoopDC = apmDiv(apmDiv(apmDiv(iLoopDC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 1005);
         tpdResults->iLoopAC = apmDiv(apmDiv(apmDiv(iLoopAC * CURRENT_SAMPLING_COEFFICIENT * kFactorAdjNum, kFactorAdjDiv), statsDuration << 9 )*100, 1005);
      }
   }

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
**             signalType  - type of signal to measure
**             measureType - DC only or AC & DC A/D measurements
**             tpdResults  - pointer to measured values
**             ringing     - current ring state
**             voltage     - voltage or current to settle
**             lowRes       - use low resolution or high resolution
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:
*****************************************************************************
*/
static XDRV_SINT16 linetestReadSignalSettle( int chan, XDRV_UINT32 signalType, XDRV_UINT32 measureType,
                                             TPD_MEASURE_RESULTS *tpdResults, XDRV_BOOL ringing, XDRV_BOOL voltage, XDRV_BOOL lowRes )
{
   XDRV_UINT32 attempts;
   XDRV_SINT32 threshold, delta, currentValue, previousValue;

   attempts = SETTLE_WAIT_MAX;
   previousValue = BMU6838_MAX_CURRENT;
   threshold = ( voltage ? V_SETTLE : I_SETTLE );
   delta = threshold;

   while( delta >= threshold )
   {
      bosSleep( 60 );

      /* Determine if the maximum number of attempts has occurred */
      if( attempts == 0 )
      {
         return( XDRV_TPD_FAILURE );
      }

      /* Read the voltages and currents */
      if ( linetestReadSignal( chan, signalType, measureType, tpdResults, ringing, lowRes ) )
      {
         return( XDRV_TPD_FAILURE );
      }

      currentValue = ( voltage ? tpdResults->tipRingVoltDC : tpdResults->iLoopDC );
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
static XDRV_UINT16 linetestDropHazardForeign( int chan, HSZDROPRSLT_HPFEF *hpfefRsltp )
{
   TPD_MEASURE_RESULTS tpdResults;

   /* Initialize the results to zero */
   memset( hpfefRsltp, 0, sizeof( HSZDROPRSLT_HPFEF ));

   /* Disable ign for this procedure */
   ignEnable( chan, XDRV_FALSE );

   /* Change SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Pause for stray voltages to discharge */
   bosSleep( 400 );

   /* Change SLIC to disconnect mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO );

   /* Settling time */
   bosSleep( 200 );

   /* Measure signal to verify foreign/hazard levels */
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Measure signal using low res if voltage significantly large */
   if(( tpdResults.tipVoltDC < TPD_MIN_HIRES_VOLTAGE ) ||
      ( tpdResults.tipVoltDC > TPD_MAX_HIRES_VOLTAGE ) ||
      ( tpdResults.tipVoltAC > TPD_MAX_HIRES_VOLTAGE ))
   {
      if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE, XDRV_TRUE ))
      {
         return ( XDRV_TPD_FAILURE );
      }
   }

   /* Save desired results */
   hpfefRsltp->tipAC = tpdResults.tipVoltAC;
   hpfefRsltp->tipDC = tpdResults.tipVoltDC-tpdVoltCalibrationDCOffset[chan].tipVoltCalibrationDC;

   /* Reverse active mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_OHTR );
   
   /* Short ring to ground */
   slic6838Si32392SetSpecialMode( (SLIC_6838_SI32392_DRV*)pSlicDriver[chan], SLIC_6838_SI32392_SPECIAL_MODE_RING_SHORT, XDRV_TRUE );
   
   /* Pause for stray voltages to discharge */
   bosSleep( 400 );
   
   /* Release ring from ground */
   slic6838Si32392SetSpecialMode( (SLIC_6838_SI32392_DRV*)pSlicDriver[chan], SLIC_6838_SI32392_SPECIAL_MODE_RING_SHORT, XDRV_FALSE );
   
   /* Disconnect mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO );
   
   /* Pause for stray voltages to discharge */
   bosSleep( 200 );

   /* Measure signal to verify foreign/hazard levels */
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Measure signal using low res if voltage significantly large */
   if(( tpdResults.ringVoltDC < TPD_MIN_HIRES_VOLTAGE ) ||
      ( tpdResults.ringVoltDC > TPD_MAX_HIRES_VOLTAGE ) ||
      ( tpdResults.ringVoltAC > TPD_MAX_HIRES_VOLTAGE ))
   {
      if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_FALSE, XDRV_TRUE ))
      {
         return ( XDRV_TPD_FAILURE );
      }
   }

   /* Save desired results */
   hpfefRsltp->ringAC = tpdResults.ringVoltAC;
   hpfefRsltp->ringDC = tpdResults.ringVoltDC-tpdVoltCalibrationDCOffset[chan].ringVoltCalibrationDC;

   /* Restore SLIC sense pin to operating state */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of HAZ & FOR V readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ch=%d,tipAC  = %.8d, tipDC  = %.8d, tipCalibrationDC = %.8d\n", chan,(int)hpfefRsltp->tipAC, (int)hpfefRsltp->tipDC,(int)tpdVoltCalibrationDCOffset[chan].tipVoltCalibrationDC));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "hazfv: ch=%d,ringAC = %.8d, ringDC = %.8d, ringCalibrationDC = %.8d\n", chan,(int)hpfefRsltp->ringAC, (int)hpfefRsltp->ringDC,(int)tpdVoltCalibrationDCOffset[chan].ringVoltCalibrationDC));   

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
static XDRV_UINT16 linetestDropOffhook( int chan )
{
   /* Enable extended operation */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_EXT_REG, SI32392_EXT_REG, 0 );

   /* Engage the built-in test resistor in 32392 */
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_4,
                    ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_ON:SI32392_CHAN1_TEST_LOAD_ON,
                    ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_MASK:SI32392_CHAN1_TEST_LOAD_MASK, 0 );

   /* Disable extended operation */
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, 0, SI32392_EXT_REG, 0 );

   /* Pause to allow CAS to detect and signal offhook condition */
   bosSleep( 2000 );

   /* Enable extended operation */
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_EXT_REG, SI32392_EXT_REG, 0 );

   /* Remove the built-in test resistor in 32392 */
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_4,
                    ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_OFF:SI32392_CHAN1_TEST_LOAD_OFF,
                    ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_MASK:SI32392_CHAN1_TEST_LOAD_MASK, 0 );

   /* Disable extended operation */
   slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, 0, SI32392_EXT_REG, 0 );

   /* Not required */
   return( XDRV_TPD_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   linetestDropROH
**
** PURPOSE:    Runs the Receiver Off Hook test on a specified channel
**
** PARAMETERS: chan     - channel to run the test on
**             rohRsltp - pointer to the structure to post the results to
**
** RETURNS:    SUCCESS or FAILURE
**
*****************************************************************************
*/
static XDRV_UINT16 linetestDropROH( int chan, HSZDROPRSLT_ROH *rohRsltp )
{
   XDRV_SINT32 lowTipRingDC;
   XDRV_SINT32 highTipRingDC;
   XDRV_SINT32 lowCurrentDC;
   XDRV_SINT32 highCurrentDC;
   int lowRes, difRes;
   int offhook;
   TPD_MEASURE_RESULTS tpdResults;
   int temp1, temp2, temp3, tempLoopCurrent;
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );

   /* Initialize the results to zero */
   memset( rohRsltp, 0, sizeof( HSZDROPRSLT_ROH ));

   /* Change to forward active mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCF );

   /* Store current SLIC parms */
   xdrvSlicGetSlicParms( pSlicDriver[chan], &temp1, &tempLoopCurrent, &temp2, &temp3 );

   /* Change loop current limit to 25mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_FALSE );

   /* Disable ign for this procedure */
   ignEnable( chan, XDRV_FALSE );

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

      /* No readings necessary, phone is onhook */
      rohRsltp->phoneOnhook = XDRV_TRUE;

      /* Restore SLIC sense pins to default operating state */
      slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

      return ( XDRV_TPD_SUCCESS );
   }

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
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
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   highTipRingDC = tpdResults.tipRingVoltDC;
   highCurrentDC = tpdResults.iLoopDC;

   /* Change loop current limit to previous setting */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], tempLoopCurrent );

   /* Restore SLIC sense pin to operating state */
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

   /* Return to standby state */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Calculate resistance values */
   lowRes = apmDiv( lowTipRingDC  * 1000, lowCurrentDC);
   difRes = apmDiv(( highTipRingDC - lowTipRingDC ) * 1000, (  highCurrentDC - lowCurrentDC ));

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
static XDRV_UINT16 linetestDropResFlt(int chan, HSZDROPRSLT_RFLT *rfltRsltp )
{
   XDRV_SINT32 Rs;
   XDRV_SINT32 VSBrh,VSBrl,ISBh,ISBl;
   XDRV_SINT32 VTOrh,VTOrl,ITOh,ITOl;
   XDRV_SINT32 VROth,VROtl,IROh,IROl;
   XDRV_SINT64 dVsb,dIsb,Rsb,dVto,dIto,Rto,dVro,dIro,Rro,Radj;
   XDRV_SINT64 Rtr,Rtg,Rrg;
   XDRV_SINT64 TOrint = 0;
   XDRV_SINT64 TOoff = 0;
   int tempRingFrequency;
   int tempRingWaveshape;
   int tempRingVoltage;
   int tempRingOffset;
   int tempRingOffsetCal;
   int temp1, temp2, temp3, tempLoopCurrent;
   TPD_MEASURE_RESULTS tpdResults;
   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;

   /* Initialize the results to zero */
   memset( rfltRsltp, 0, sizeof( HSZDROPRSLT_RFLT ));

   /* Save the ringing amplitude and set it to a known DC value */
   xdrvApmGetRingParms( pApmDriver[chan], &tempRingFrequency, &tempRingWaveshape, &tempRingVoltage, &tempRingOffset, &tempRingOffsetCal );
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, 0, 48, tempRingOffsetCal );
   
   /* Enable the ign for this procedure */
   ignEnable( chan, XDRV_TRUE );

   /* Store current SLIC parms */
   xdrvSlicGetSlicParms( pSlicDriver[chan], &temp1, &tempLoopCurrent, &temp2, &temp3 );

   /* Change loop current limit to 40mA */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], XDRV_TRUE );

   /* Set SLIC to standby */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Delay from SLIC state & IGN change */
   bosSleep( 200 );

   /* Read the offset voltage with TESTO disabled */
   /* Procedure assumes a 49.9K pulldown and 232K pullup */
   if ( linetestReadSignal( chan, TPD_SIGNAL_TRES1, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   TOoff = (XDRV_SINT64)tpdResults.iLoopDCRaw; 

   /* Circumvent divide by zero exceptions */
   if( TOoff == 0 )
   {
      TOoff = 1;
   }

   /* Read the test resistance */
   if ( linetestReadSignal( chan, TPD_SIGNAL_TRES, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   TOrint = (XDRV_SINT64)tpdResults.iLoopDCRaw; 

   /* Circumvent divide by zero exceptions */
   if( TOrint == 0 )
   {
      TOrint = 1;
   }

   /* Calculate Rs */
   Rs = apmDiv(apmDiv(apmDiv(((XDRV_SINT64)1400 * (1 << (9 + 17)) - 2148 * TOrint) * TOoff, (TOrint - TOoff)) * 2900, 1 << (9 + 17)),723);

   bosSleep( 100 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VSBrh = tpdResults.tipRingVoltDC;
   ISBh = tpdResults.iLoopDC;

   /* Change SLIC to Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );

   bosSleep( 400 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VTOrh = tpdResults.ringVoltDC;
   ITOh = tpdResults.iLoopDC;

   /* Reverse the ringing DC offset to discharge the line */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, 0, -44, tempRingOffsetCal );

   /* Disable the ign to change states */
   ignEnable( chan, XDRV_FALSE );

   /* Set SLIC to ringing */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );
   xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_ONHOOK );

   /* Eliminate the common mode on the IGN0 */
   pHvg->reg_hvg_cha_max_hvg_slic = 0x0000;
   pHvg->reg_hvg_chb_max_hvg_slic = 0x0000;

   bosSleep( 400 );

   /* Enable the ign */
   ignEnable( chan, XDRV_TRUE );

   /* Change SLIC to Ring Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RINGOPEN );

   bosSleep( 200 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VROth = tpdResults.tipVoltDC;
   IROh = tpdResults.iLoopDC;

   /* Disable the ign to change battery */
   ignEnable( chan, XDRV_FALSE );

   /* Switch to Low Battery */
   tpdBatteryControl( chan, TPD_BATTERY_MODE_LOW );

   /* Decrease the ringing DC offset to discharge the line */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, 0, -24, tempRingOffsetCal );
   
   /* Set SLIC to ringing */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );
   xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_ONHOOK );

   /* Eliminate the common mode on the IGN0 */
   pHvg->reg_hvg_cha_max_hvg_slic = 0x0000;
   pHvg->reg_hvg_chb_max_hvg_slic = 0x0000;

   bosSleep( 400 );

   /* Enable the ign */
   ignEnable( chan, XDRV_TRUE );

   /* Change SLIC to Ring Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RINGOPEN );

   bosSleep( 200 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VROtl = tpdResults.tipVoltDC;
   IROl = tpdResults.iLoopDC;

   /* Disable the ign to change states */
   ignEnable( chan, XDRV_FALSE );

   /* Set SLIC to standby */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   bosSleep( 400 );

   /* Enable the ign */
   ignEnable( chan, XDRV_TRUE );

   /* Pause for stray voltages to discharge */
   bosSleep( 200 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VSBrl = tpdResults.tipRingVoltDC;
   ISBl = tpdResults.iLoopDC;

   /* Change SLIC to Tip Open */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_TIPOPEN );

   bosSleep( 400 );

   /* Wait for current to settle due to capacitive loading */
   if ( linetestReadSignalSettle( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Store results */
   VTOrl = tpdResults.ringVoltDC;
   ITOl = tpdResults.iLoopDC;

   /* Revert back to ign disabled */
   ignEnable( chan, XDRV_FALSE );

   /* Change loop current limit to previous setting */
   xdrvSlicSetBoostedLoopCurrent( pSlicDriver[chan], tempLoopCurrent );

   /* Restore SLIC sense pin to operating state */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

   /* Revert back to default onhook voltage (48V) */
   tpdBatteryControl( chan, TPD_BATTERY_MODE_MED );
   
   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Reset ring voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   /* Start calculating impedances */
   dVsb = (XDRV_SINT64)(abs(VSBrh - VSBrl));
   dIsb = (XDRV_SINT64)(abs(ISBh - ISBl)) * 1000;
   if ( dIsb == 0 )
   {
      dIsb = 1;
   }

     Rsb = apmDiv( dVsb * 1000000 , dIsb);

   dVto = (XDRV_SINT64)( abs( VTOrh - VTOrl ) );
   dIto = (XDRV_SINT64)( abs( ITOh - ITOl ) ) * 1000;
   if ( dIto == 0 )
   {
      dIto = 1;
   }
     Rto = apmDiv( dVto * 1000000 , dIto);

   dVro = (XDRV_SINT64)( abs( VROth - VROtl ) );
   dIro = (XDRV_SINT64)( abs( IROh - IROl ) ) * 1000;
   if ( dIro == 0 )
   {
      dIro = 1;
   }
     Rro = apmDiv( dVro * 1000000 , dIro);
   /* 10M || 520K internal */

     Radj = apmDiv( (XDRV_SINT64)Rs * 520 * 10000000 , (XDRV_SINT64)Rs * 520  + 10000000 );

   /* Resolve individual resistors */

   Rtr = max( Rsb, (XDRV_SINT64)abs(apmDiv( Rsb * Radj,  Radj - Rsb )) );
   Rrg = max( Rto, (XDRV_SINT64)abs(apmDiv( Rto * Radj,  Radj - Rto )) );
   Rtg = max( Rro, (XDRV_SINT64)abs(apmDiv( Rro * Radj,  Radj - Rro )) );

   if (( dVsb < RES_FLT_VOLTAGE ) && ( ISBh > RES_FLT_CURRENT ))
   {
      if ( ITOh > RES_FLT_CURRENT )
      {
         Rrg = 0;
         Rtr = 10000000;
      }
      else
      {
         Rtr = 0;
      }
   }
   else if (( dVto < RES_FLT_VOLTAGE ) && ( ITOh > RES_FLT_CURRENT ))
   {
      Rrg = 0;
      Rtr = 10000000;
   }
   else if (( dVro < RES_FLT_VOLTAGE ) && ( IROh < -RES_FLT_CURRENT ))
   {
      Rtg = 0;
   }
   /* If the 2 measurements match within 20%, then R is Rrg */
   else if (( Rsb < Radj ) && ( Rto < Radj ) && (( 6 * Rto ) > ( 5 * Rsb )) && (( 6 * Rsb ) > ( 5 * Rto )))
   {
     Rtr = max( 10000000, (int)abs(apmDiv( Rsb * Rto , Rto - Rsb )) );
   }
   /* If Rtr is the lowest, then adjust Rrg & Rtg */
   else if ((( 6 * Rsb ) < ( 5 * Rro )) && (( 6 * Rsb ) < ( 5 * Rto )))
   {
      Rrg = max( 10000000, (int)abs(apmDiv( Rrg * ( Rtr + Radj),  Rtr + Radj - Rrg )) );
      Rtg = max( 10000000, (int)abs(apmDiv( Rtg * ( Rtr + Radj),  Rtr + Radj - Rtg )) );
   }

   /* Save calculated impedances */
   rfltRsltp->tipGroundImpedance = (XDRV_SINT32)abs(Rtg);
   rfltRsltp->ringGroundImpedance = (XDRV_SINT32)abs(Rrg);
   rfltRsltp->tipRingImpedance = (XDRV_SINT32)abs(Rtr);

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "******************************************************************" )); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of ResFLT readings ... \n" )); 
   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Rs = %.8d", (int)Rs));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VSBrh=%.8dmV, ISBh=%.8duA", (int)VSBrh, (int)ISBh ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VSBrl=%.8dmV, ISBl=%.8duA", (int)VSBrl, (int)ISBl ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTOrh=%.8dmV, ITOh=%.8duA", (int)VTOrh, (int)ITOh ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VTOrl=%.8dmV, ITOl=%.8duA", (int)VTOrl, (int)ITOl ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VROth=%.8dmV, IROh=%.8duA", (int)VROth, (int)IROh ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "VROtl=%.8dmV, IROl=%.8duA", (int)VROtl, (int)IROl ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Radj=%.8d, Rsb=%.8d", (int)Radj, (int)Rsb ));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Rto=%.8d, Rro=%.8d", (int)Rto, (int)Rro ));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "dVsb=%.8dmV, dIsb=%.8duA", (int)dVsb, (int)apmDiv(dIsb , 1000 )));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "dVto=%.8dmV, dIto=%.8duA", (int)dVto, (int)apmDiv(dIto , 1000 )));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "dVro=%.8dmV, dIro=%.8duA", (int)dVro, (int)apmDiv(dIro , 1000 )));  
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "TOrint = %.8d", (int)TOrint ));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "TOoff = %.8d", (int)TOoff ));

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
static XDRV_UINT16 linetestDropRing(int chan, HSZDROPRSLT_RNG *rngRsltp)
{
   XDRV_SINT32 VoltACLow;
   XDRV_SINT32 AcCurrentHiRen,AcCurrentLowRen17,AcCurrentLowRen9;
   XDRV_SINT64 ImetHiRen,ImetLowRen;
   XDRV_UINT32 resistanceHiRen,valueLowRen;
   XDRV_SINT32 Rs = 1000;
   XDRV_SINT64 TOrint = 0;
   XDRV_SINT64 TOoff = 0;
   XDRV_UINT32 tempReg;
   XDRV_SINT32 tipRingVoltACHiRen, tipRingVoltACLowRen17, tipRingVoltACLowRen9;
   int tempRingFrequency;
   int tempRingWaveshape;
   int tempRingVoltage;
   int tempRingOffset;
   int tempRingOffsetCal;
   TPD_MEASURE_RESULTS tpdResults;
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];
#if !XDRV_CFG_BALANCED_RINGING
   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;
#endif

   /* Initialize the results to zero */
   memset( rngRsltp, 0, sizeof( HSZDROPRSLT_RNG ));

   /* Save the ringing amplitude and set it to a known value */
   xdrvApmGetRingParms( pApmDriver[chan], &tempRingFrequency, &tempRingWaveshape, &tempRingVoltage, &tempRingOffset, &tempRingOffsetCal );
   xdrvApmSetRingParms( pApmDriver[chan], 18, tempRingWaveshape, 15, 0, tempRingOffsetCal );

#if !XDRV_CFG_BALANCED_RINGING
   /* Override default max HVG voltage */
   pHvg->reg_hvg_cha_max_hvg_slic = 0x0320;
   pHvg->reg_hvg_chb_max_hvg_slic = 0x0320;
#endif

   /* Override ring frequence and set to 18.57Hz */
   tempReg = pDev->pApm->reg_ring_config_1;
   tempReg &= ~RING_OMEGA_NORM;
   tempReg |= (( 0x300 << RING_OMEGA_NORM_SHIFT ) & RING_OMEGA_NORM );
   pDev->pApm->reg_ring_config_1 = tempReg;

   /* Disable the ign */
   ignEnable( chan, XDRV_FALSE );

   /* Set SLIC to ringing */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );
   xdrvApmHvgSetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_ONHOOK );

   bosSleep( 200 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_TRUE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   tipRingVoltACHiRen = tpdResults.tipRingVoltAC;
   AcCurrentHiRen = tpdResults.iLoopAC;

   /* Enable the ign */
   ignEnable( chan, XDRV_TRUE );

   /* Increase the ringing amplitude for IGN1 measurements */
   xdrvApmSetRingParms( pApmDriver[chan], 18, tempRingWaveshape, 17, 0, tempRingOffsetCal );

#if !XDRV_CFG_BALANCED_RINGING
   /* Override default max HVG voltage */
   pHvg->reg_hvg_cha_max_hvg_slic = 0x0320;
   pHvg->reg_hvg_chb_max_hvg_slic = 0x0320;
#endif

   /* Read the offset voltage with TESTO disabled */
   /* Procedure assumes a 49.9K pulldown and 232K pullup */
   if ( linetestReadSignal( chan, TPD_SIGNAL_TRES1, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   TOoff = (XDRV_SINT64)tpdResults.iLoopDCRaw;
   /* Circumvent divide by zero exceptions */
   if( TOoff == 0 )
   {
      TOoff = 1;
   }

   /* Read the test resistance */
   if ( linetestReadSignal( chan, TPD_SIGNAL_TRES, TPD_MEASURE_DC, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   TOrint = (XDRV_SINT64)tpdResults.iLoopDCRaw; 

   /* Circumvent divide by zero exceptions */
   if( TOrint == 0 )
   {
      TOrint = 1;
   }

   /* Calculate Rs */
   Rs =(XDRV_SINT32)apmDiv(apmDiv(apmDiv( ((XDRV_SINT64)1400 * (1 << (9 + 17)) - 2148 * TOrint) * TOoff, TOrint - TOoff) * 2900,1 << (9 + 17)), 723);
   /* Circumvent divide by zero exceptions */
   if( Rs == 0 )
   {
      Rs = 1;
   }

   /* Account for current through shunt impedances */
   /* Multiply all values by 10 and then divide in the end to eliminate truncation error of 3 terms */
   ImetHiRen = apmDiv((XDRV_SINT64)AcCurrentHiRen * 10 - apmDiv((XDRV_SINT64)( tipRingVoltACHiRen ) * 10, (XDRV_SINT64)400 ) - apmDiv((XDRV_SINT64)( tipRingVoltACHiRen ) * 10000, (XDRV_SINT64)520 * (XDRV_SINT64)Rs), 10);
   /* Calculate REN current */
   if ( ImetHiRen == 0 )
   {
      ImetHiRen = 1;
   }

   /* Calculate High REN value */
     resistanceHiRen = (XDRV_SINT32)(apmDiv(((XDRV_SINT64)( tipRingVoltACHiRen ) * (XDRV_SINT64)1000 ), ImetHiRen )); /* Clean up equation */

   /* Circumvent divide by zero exceptions */
   if( resistanceHiRen == 0 )
   {
      resistanceHiRen = 1;
   }

   if( resistanceHiRen < DROP_RING_REN_THRESH )
   {
      rngRsltp->RENValue1 = (XDRV_SINT32)apmDiv((XDRV_SINT64)7012 , (XDRV_SINT64)resistanceHiRen);
      rngRsltp->RENValue2 = (XDRV_SINT32)apmDiv( (XDRV_SINT64)(7012 % resistanceHiRen * 1000) , (XDRV_SINT64)resistanceHiRen);

	   /* Assign values for LowRen variables not calculated */
	   AcCurrentLowRen17 = 0;
	   AcCurrentLowRen9 = 0;
	   ImetLowRen = 0;
      valueLowRen = 0;
      tipRingVoltACLowRen17 = 0;
	   tipRingVoltACLowRen9 = 0;
	   VoltACLow = 0;
   }
   else
   {
      /* Read the voltages and currents */
      if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_TRUE, XDRV_FALSE ))
      {
         return ( XDRV_TPD_FAILURE );
      }

      /* Save desired results */
      tipRingVoltACLowRen17 = tpdResults.tipRingVoltAC;
      AcCurrentLowRen17 = tpdResults.iLoopAC;

      /* Change to 9V ring voltage */
      xdrvApmSetRingParms( pApmDriver[chan], 18, tempRingWaveshape, 9, 0, tempRingOffsetCal );

#if !XDRV_CFG_BALANCED_RINGING
      /* Override default max HVG voltage */
      pHvg->reg_hvg_cha_max_hvg_slic = 0x0320;
      pHvg->reg_hvg_chb_max_hvg_slic = 0x0320;
#endif

      bosSleep( 40 );

      /* Read the voltages and currents */
      if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_ACDC, &tpdResults, XDRV_TRUE, XDRV_FALSE ))
      {
         return ( XDRV_TPD_FAILURE );
      }

      /* Save desired results */
      tipRingVoltACLowRen9 = tpdResults.tipRingVoltAC;
      AcCurrentLowRen9 = tpdResults.iLoopAC;

      /* Delta voltages */
      VoltACLow = tipRingVoltACLowRen17 - tipRingVoltACLowRen9;
      ImetLowRen = AcCurrentLowRen17 - AcCurrentLowRen9;
      valueLowRen = (XDRV_UINT32)( apmDiv( (XDRV_SINT64)7012 * ImetLowRen, (XDRV_SINT64)VoltACLow ) -
                                   apmDiv( (XDRV_SINT64)7012 * (XDRV_SINT64)1, (XDRV_SINT64)400 ) -
                                   apmDiv( (XDRV_SINT64)7012 * (XDRV_SINT64)1000, ( (XDRV_SINT64)520 * Rs ) ) );
      rngRsltp->RENValue1 = (XDRV_UINT32)apmDiv((XDRV_SINT64)valueLowRen, (XDRV_SINT64)1000);
      rngRsltp->RENValue2 = valueLowRen % 1000;
   }

   /* Revert back to ign disabled */
   ignEnable( chan, XDRV_FALSE );

   /* Restore SLIC sense pin to operating state */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Reset ring voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "******************************************************************" )); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of RING readings ... \n" )); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: AcCurrentHiRen = %.8d, AcCurrentLowRen17 = %.8d, AcCurrentLowRen9 = %.8d \n", (int)AcCurrentHiRen, (int)AcCurrentLowRen17, (int)AcCurrentLowRen9 ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: ImetHiRen  = %.8d, ImetLowRen  = %.8d ", (int)ImetHiRen, (int)ImetLowRen ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: resistanceHiRen  = %.8d, valueLowRen  = %.8d ", (int)resistanceHiRen, (int)valueLowRen ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: Rs = %.8d \n", (int)Rs ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: TOrint = %.8d \n", (int)TOrint ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: TOoff = %.8d \n", (int)TOoff ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: tipRingVoltACHiRen = %.8d, tipRingVoltACLowRen17 = %.8d, tipRingVoltACLowRen9 = %.8d \n", (int)tipRingVoltACHiRen, (int)tipRingVoltACLowRen17, (int)tipRingVoltACLowRen9 ));
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: VoltACLow = %.8d \n", (int)VoltACLow ));

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
static XDRV_UINT16 linetestDropCalibration(int chan, HSZDROPRSLT_CAL *calRsltp)
{
   XDRV_SINT32 tipVoltDCoffset;
   XDRV_SINT32 ringVoltDCoffset;
   XDRV_SINT32 ringOffsetcal;
   int tempRingFrequency;
   int tempRingWaveshape;
   int tempRingVoltage;
   int tempRingOffset;
   int tempRingOffsetCal;
   TPD_MEASURE_RESULTS tpdResults;
   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;

   /* Initialize the results to zero */
   memset( calRsltp, 0, sizeof( HSZDROPRSLT_CAL ));

   /* Save the ringing amplitude and set it to zero */
   xdrvApmGetRingParms( pApmDriver[chan], &tempRingFrequency, &tempRingWaveshape, &tempRingVoltage, &tempRingOffset, &tempRingOffsetCal );
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, 0, 0, 0 );
   
   /* Change SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   /* Disable the ign */
   ignEnable( chan, XDRV_FALSE );

   /* Set SLIC to ringing */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_RING );

   /* Override default voltage mode */
   hvg6838SetVoltage( pApmDriver[chan], chan, XDRV_APM_HVG_VOLTAGE_ONHOOK );

   /* Override MAX HVG slic */
   pHvg->reg_hvg_cha_max_hvg_slic = 0x04D5;
   pHvg->reg_hvg_chb_max_hvg_slic = 0x04D5;

   /* Override HVG voltage */
   if( chan == 0 )
   {
      pHvg->reg_hvg_cha_const_volt_goal = 0x0640; 
   }
   else
   {
      pHvg->reg_hvg_chb_const_volt_goal = 0x0640;
   }

   bosSleep( 200 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC, &tpdResults, XDRV_TRUE, XDRV_FALSE ))
   {
      return ( XDRV_TPD_FAILURE );
   }

   /* Save desired results */
   tipVoltDCoffset = tpdResults.tipVoltDC;
   ringVoltDCoffset = tpdResults.ringVoltDC;

   /* Revert back to ign disabled */
   ignEnable( chan, XDRV_FALSE );

   /* Restore SLIC sense pin to operating state */
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   slic6838Si32392RegisterWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_3, SI32392_TEST_NORMAL_OPERATION );

   /* Revert SLIC to standby mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

   bosSleep( 100 );

   /* Reset ring voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   /* Calculate and store result */
   ringOffsetcal = ringVoltDCoffset - tipVoltDCoffset;
   calRsltp->RngVolOffset = ringOffsetcal;

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "****************************************************************** ")); 
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Summary of CALIBRATION readings ... \n")); 

   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: tipVoltDCoffset = %.8d, ringVoltDCoffset = %.8d ", (int)tipVoltDCoffset, (int)ringVoltDCoffset));   
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "ring: ringOffsetcal  = %.8d ", (int)ringOffsetcal ));   

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
static XDRV_UINT16 linetestSelfAssert( int chan, XDRV_SINT32 *selfAssertPassed )
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
static XDRV_UINT16 linetestSelfDspCount( int chan, XDRV_SINT32 *selfDspCountPassed )
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
static XDRV_UINT16 linetestSelfHvg( int chan, XDRV_SINT32 *selfHvgPassed )
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
static XDRV_UINT16 linetestSelfApm( int chan, XDRV_SINT32 *selfApmPassed )
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
** FUNCTION:   tpd6838Si32392InitChannel
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
int tpd6838Si32392InitChannel( int chan, XDRV_TPD *tpdDrv, XDRV_SLIC *slicDrv,
                               XDRV_APM *apmDrv, XDRV_DEBUG *debugDrv )
{
   /* Set the "meta-driver" interface */
   tpdDrv->pDrvFuncs = &tpd6838Si32392DrvFuncs;

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

   /* Measure the offset coefficients */
   tpdMeasureOffset( chan );

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

      /* Reset SLIC State to default on hook state */
      xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_STANDBY );

      /* Reenable CAS */
      tpdCasEnable( chan );
   }
   else
   {
      /* Cut the battery voltage on this channel */
      hvg6838Stop(chan);
      
      /* Disconnect the SLIC device */
      xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO);
   
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
** PURPOSE:    This is a task to control the SLIC battery voltage
**
** PARAMETERS: chan - the channel to control
**             mode - the voltage to switch to
**
** RETURNS:    None
**
*****************************************************************************
*/
static void tpdBatteryControl( int chan, TPD_BATTERY_MODE mode )
{
   SLIC_6838_SI32392_DRV *pDrv = (SLIC_6838_SI32392_DRV *)boardHalSlicGetDriver( chan );
   switch ( mode )
   {
      case TPD_BATTERY_MODE_MED:
      {
         slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_2,
                          ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_ENHANCED_CONTROL_OFF:SI32392_CHAN1_ENHANCED_CONTROL_OFF,
                          ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_ENHANCED_CONTROL_MASK:SI32392_CHAN1_ENHANCED_CONTROL_MASK, 0 );
      }
      break;

      case TPD_BATTERY_MODE_LOW:
      {
         slic6838Si32392RegisterMaskWrite( pDrv, SI32392_REG_LINEFEED_CONFIG_2,
                          ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_ENHANCED_CONTROL_ON:SI32392_CHAN1_ENHANCED_CONTROL_ON,
                          ( ( chan % 2 ) == 0 ) ? SI32392_CHAN0_ENHANCED_CONTROL_MASK:SI32392_CHAN1_ENHANCED_CONTROL_MASK, 0 );
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
static void tpdEnableYFilter( int chan )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];
   volatile BCM6838_APM *pApm = pDev->pApm;

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   switch ( chan )
   {
      case 0:
      {
         pApm->reg_cha_hybal_config |= YFLTR_EN;
      }
      break;
      case 1:
      {
         pApm->reg_chb_hybal_config |= YFLTR_EN;
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
static void tpdDisableYFilter( int chan )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDriver[chan];
   volatile BCM6838_APM *pApm = pDev->pApm;

   chan = pDev->channelSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   switch ( chan )
   {
      case 0:
      {
         pApm->reg_cha_hybal_config &= ~YFLTR_EN;
      }
      break;
      case 1:
      {
         pApm->reg_chb_hybal_config &= ~YFLTR_EN;
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
static void tpdCasDisable( int chan )
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
static void tpdCasEnable( int chan )
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
   tpdDischargeVoltage( chan );

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
**
** RETURNS:    Nothing
**
*****************************************************************************
*/
static void tpdDischargeVoltage( int chan )
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

   /* Pause for stray voltages to discharge */
   bosSleep( 1000 );

   /* Change to disconnected mode */
   xdrvSlicModeControl( pSlicDriver[chan], XDRV_SLIC_MODE_LCFO );

   /* Reset Ring Voltage */
   xdrvApmSetRingParms( pApmDriver[chan], tempRingFrequency, tempRingWaveshape, tempRingVoltage, tempRingOffset, tempRingOffsetCal );

   bosSleep( 1000 );
}

/*
*****************************************************************************
** FUNCTION:   tpdMeasureOffset
**
** PURPOSE:    Measure the current offset coefficients for this line
**
** PARAMETERS: chan - channel to measure offsets on
**
** RETURNS:    Nothing
**
*****************************************************************************
*/
static void tpdMeasureOffset( int chan )
{
   TPD_MEASURE_RESULTS tpdResults;

   /* Disable ign for this procedure */
   ignEnable( chan, XDRV_FALSE );

   bosSleep( 20 );

   /* Read the voltages and currents */
   if ( linetestReadSignal( chan, TPD_SIGNAL_OFFSET, TPD_MEASURE_DC_CHOP, &tpdResults, XDRV_FALSE, XDRV_FALSE ))
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Error updating condCurOffset for chan %d", chan )); 
      return;
   }

   /* Save desired results */
   /* Record conditional offset for this channel */
   condCurOffset[chan] = (XDRV_UINT64)tpdResults.iLoopDCRaw;
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Measured condCurOffset for chan %d is 0x%08X", (int)chan, (unsigned int)condCurOffset[chan] )); 
}

/*
*****************************************************************************
** FUNCTION:   tpd6838Si32392BootupCalibration
**
** PURPOSE:    Run TPD Bootup Calibration
**
** PARAMETERS: results - Pointer to results
**             updateValues - Update offsets only or perform calibration
**
** RETURNS:    SUCCESS or FAILURE of all calibration tests
**
** NOTES:      The HVG must be initialized but not started before
**             running this calibration routine
**
*****************************************************************************
*/
int tpd6838Si32392BootupCalibration( HSZDROPRSLT_CAL *results, XDRV_BOOL updateValues, unsigned int numEndpts )
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
   unsigned int activeEndpts;

   /* Determine number of endpoints to calibrate */
   if( numEndpts < APM6838_NUM_CHANNELS )
   {
      activeEndpts = numEndpts;
   }
   else
   {
      activeEndpts = APM6838_NUM_CHANNELS;
   }

   /* Perform new calibration if desired */
   if( updateValues )
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Running TPD Bootup Calibration ... \n")); 

      /* Disable the PWM output */
      hvgp->reg_hvg_soft_init |= HVG_PWM_DISABLE;

      /* Clear previous results */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         tipVoltDCoffset[i] = 0;
         ringVoltDCoffset[i] = 0;
         hvgVoltDCoffset[i] = 0;
      }   

      /* Start HVG channels */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         hvg6838Start( i );
      }   

      /* Pause for HVG state to stabilize */
      bosSleep(1000);

      /* Measure the voltages and currents */
      for( i = 0; i < activeEndpts; i++ )
      {
         if ( linetestReadSignal( i, TPD_SIGNAL_NORMAL, TPD_MEASURE_DC, 
                                  &tpdResults, XDRV_FALSE, XDRV_FALSE ) )
         {
            return ( XDRV_TPD_FAILURE );
         }
         else
         {
            tipVoltDCoffset[i] = tpdResults.tipVoltDC - 0;
            ringVoltDCoffset[i] = tpdResults.ringVoltDC - 0;
            hvgVoltDCoffset[i] = tpdResults.hvgVoltDC - 325;
         }
      }

      /* Stop HVG channels */
      for( i = 0; i < activeEndpts; i++ )
      {
         hvg6838Stop( i );
      }   

      /* Enable the PWM output */
      hvgp->reg_hvg_soft_init &= ~HVG_PWM_DISABLE;
   }
   else
   {
      XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Loading stored calibration values ... \n")); 

      /* Clear previous results */
      for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
      {
         tipVoltDCoffset[i] = 0;
         ringVoltDCoffset[i] = 0;
         hvgVoltDCoffset[i] = 0;
      }   

      /* Load current values */
      for( i = 0; i < activeEndpts; i++ )
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
      updatedOffset = currentOffset - apmDiv(( tipVoltDCcorCommon * 2 ),  75 ) - apmDiv(((( tipVoltDCcorCommon * 2 ) % 75 ) * 2 ), 75 );
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

      updatedOffset = currentOffset - apmDiv(( ringVoltDCcorCommon * 2 ), 75 ) - apmDiv(((( ringVoltDCcorCommon * 2 ) % 75 ) * 2 ), 75 );
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
      updatedOffset = currentOffset - apmDiv(( hvgVoltDCcorCommon * 2 ), 75 ) - apmDiv(((( hvgVoltDCcorCommon * 2 ) % 75 ) * 2 ), 75 );
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


