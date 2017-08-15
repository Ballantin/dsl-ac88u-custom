/****************************************************************************
*
* Copyright (c) 2009-2012 Broadcom Corporation
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
*    Filename: hvg6838.c
*
****************************************************************************
*
*    Description: HVG (high voltage generateor) device driver for BCM6838
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <xdrvTypes.h>
#include <xchgAssert.h>
#include <xdrvLog.h>
#include <xdrvApm.h>
#include <xdrvSlic.h>
#include <bosSleep.h>
#include <hvg6838Cfg.h>
#include "hvg6838.h"
#include "apm6838.h"

typedef struct BCM6838_HVG_CUSTOM
{
   /* HVG Shared Settings */
   XDRV_UINT32 reg_hvg_offhook_track_max;  /* Reg 12 */ 
   XDRV_UINT32 reg_hvg_ring_track_max;     /* Reg 9 */ 
   XDRV_UINT32 reg_hvg_ring;               /* Reg 11 */ 
   XDRV_UINT16 reg_hvg_off_hook;           /* Reg 12 */ 
   XDRV_UINT16 reg_hvg_off_hook_bl;        /* Reg 12 */ 

   /* HVG Channel Settings */
   XDRV_UINT16 reg_hvg_max_hvg_slic;       /* Reg 1 - Upper 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal;    /* Reg 1 - Lower 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal_sb; /* Reg 1 - Lower 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal_lp; /* Reg 1 - Lower 16 Bits */

} BCM6838_HVG_CUSTOM;

/*
** This is the 6838 custom register table with values customized for
** specific HW layouts with specific SLICs.
*/
static BCM6838_HVG_CUSTOM regCustomTable[XDRV_APM_HVG_DESIGN_NUM_TYPES] =
{
   { 0x5BB00000,   0x00000A35,   0x00D61EFC,   0x01FF,   0x01FF,   0x0000,   0x05BB,   0x0578,   0x033B   }, /* Le9540 : -114V Vhvg limit */
   { 0x00000000,   0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x63A00000,   0x00000A35,   0x047E1FEA,   0x0103,   0x0176,   0x03DF,   0x063A,   0x05C0,   0x03C2   }, /* Si32392 : -114V Vhvg limit */
   { 0x00000000,   0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x00000000,   0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   }
};

/*
** Mapping from SLIC modes to voltage levels
*/
typedef struct MODEVOLTAGEMAP
{
   XDRV_UINT16           mode;
   XDRV_APM_HVG_VOLTAGE  onHookVoltage;
   XDRV_APM_HVG_VOLTAGE  offHookVoltage;

} MODEVOLTAGEMAP;

/* SLIC mode - battery voltage level mappings */
static const MODEVOLTAGEMAP modeVoltageMap[] =
{
   { XDRV_SLIC_MODE_LCFO,      XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { XDRV_SLIC_MODE_STANDBY,   XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { XDRV_SLIC_MODE_OHT,       XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_OHTR,      XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_RINGOPEN,  XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { XDRV_SLIC_MODE_TIPOPEN,   XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { XDRV_SLIC_MODE_RING,      XDRV_APM_HVG_VOLTAGE_RING,      XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_LCF,       XDRV_APM_HVG_VOLTAGE_OFFHOOK,   XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_RLCF,      XDRV_APM_HVG_VOLTAGE_OFFHOOK,   XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_WINK,      XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_NULL,      XDRV_APM_HVG_VOLTAGE_NONE,      XDRV_APM_HVG_VOLTAGE_NONE     }
};

static int hvgChanSwap=XDRV_FALSE; /* to store channel swap status from apm driver*/

static int hvg6838RegSetup( XDRV_APM *pApmDrv );
static int hvg6838UpdateHvgParms( XDRV_APM *pApmDrv, int chan );
static XDRV_APM_HVG_VOLTAGE hvg6838GetVoltage( XDRV_UINT16 mode, XDRV_UINT16 hookState, int battery );
static int hvg6838SetVoltageInt( XDRV_APM *pApmDrv, int chan, int voltage );
static XDRV_SINT32 hvgExtend21to32( XDRV_UINT32 value );

/*****************************************************************************
**
**  FUNCTION:   hvg6838RegSetup
**
**  PURPOSE:    Check the HVG registers on startup against their default value.
**              Fix the registers where needed.
**
**  PARAMETERS: pApmDrv - pointer to the APM driver interface
**
**  RETURNS:    0 on SUCCESS
**
**  NOTES:      None
**
*****************************************************************************/
static int hvg6838RegSetup( XDRV_APM *pApmDrv )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;

   /* HVG Shared Settings */
   hvgp->reg_hvg_vhvg_lores_cond     = 0x1A141830;		/* -114V Vhvg limit */
   hvgp->reg_hvg_vhvg_hires_cond     = 0x0E9D0C18;		/* -114V Vhvg limit */
   hvgp->reg_hvg_vtip_lores_cond     = 0x28169FE5;
   hvgp->reg_hvg_vtip_hires_cond     = 0x0C3E1FE5;
   hvgp->reg_hvg_vring_lores_cond    = 0x28169FE5;
   hvgp->reg_hvg_vring_hires_cond    = 0x0C3E1FE5;
   hvgp->reg_hvg_vinput_cond         = 0x00002300;
   hvgp->reg_hvg_cal_cond            = 0x00001000;
   hvgp->reg_hvg_vbat_cond           = 0x00001660;
   hvgp->reg_hvg_duty_cycle          = 0x991A01F0;		/* Inverting Boost */
   hvgp->reg_hvg_circuit_parms       = 0x000002A0;		/* Inverting Boost */
   hvgp->reg_hvg_fasd_multi_speed    = 0x80400080;
   hvgp->reg_hvg_hvg_sar_timing      = 0x01000000;
   hvgp->reg_hvg_bmu_sar_timing      = 0x01800080;
   hvgp->reg_hvg_hi_sens_setup       = 0x00000060;
   hvgp->reg_hvg_clk_ser_intrf       = 0x68000000;
   hvgp->reg_hvg_zar_if_config       = 0x00000000;
   hvgp->reg_hvg_zar_if_xmt_data     = 0x00000000;
   
   /* Scale setting depends on input voltage */
   if( pDev->Cfg.inputSupplyVoltage == 12 )
   {
      hvgp->reg_hvg_vinput_cond  = HVG_6838_INPUT_SCALE_12V;
      hvgp->reg_hvg_vbat_cond    = HVG_6838_VBAT_SCALE_12V;
   }
   else
   {
      hvgp->reg_hvg_vinput_cond  = HVG_6838_INPUT_SCALE_15V;
      hvgp->reg_hvg_vbat_cond    = HVG_6838_VBAT_SCALE_15V;
   }

   /* HVG Channel A Settings */
   hvgp->reg_hvg_cha_reg_0              =  0xCF387448;      /* Reg 0 */
   hvgp->reg_hvg_cha_reg_2              =  0x0000E795;      /* Reg 2 */

   /* HVG Channel B Settings */
   hvgp->reg_hvg_chb_reg_0              =  0xCF387448;      /* Reg 0 */
   hvgp->reg_hvg_chb_reg_2              =  0x0000E795;      /* Reg 2 */

   /* Platform Specific Settings */
   /* HVG Shared Settings */
   hvgp->reg_hvg_soft_init              =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_ring_track_max |
                                           0xA0FA0000;
   hvgp->reg_hvg_ring_setup             =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_ring;
   hvgp->reg_hvg_off_hook_setup         =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_offhook_track_max |
                                           regCustomTable[pDev->Cfg.hvgType].reg_hvg_off_hook |
                                           0x0003C000;

   /* HVG Channel A Settings */
   hvgp->reg_hvg_cha_max_hvg_slic       =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_max_hvg_slic;      /* Reg 1 - Upper 16 Bits */
   hvgp->reg_hvg_cha_const_volt_goal    =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_const_volt_goal;   /* Reg 1 - Lower 16 Bits */

   /* HVG Channel B Settings */
   hvgp->reg_hvg_chb_max_hvg_slic       =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_max_hvg_slic;      /* Reg 1 - Upper 16 Bits */
   hvgp->reg_hvg_chb_const_volt_goal    =  regCustomTable[pDev->Cfg.hvgType].reg_hvg_const_volt_goal;   /* Reg 1 - Lower 16 Bits */
   
   return ( 0 );
}


/*****************************************************************************
**
**  FUNCTION:   hvgInit
**
**  PURPOSE:    Initize the HVG before start
**
**  PARAMETERS: pApmDrv - pointer to the APM driver interface
**
**  RETURNS:    0 on SUCCESS
**
**  NOTES:      None
**
*****************************************************************************/
int hvg6838Init( XDRV_APM *pApmDrv )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   int i;

   /* Ensure valid HVG type */
   if(( pDev->Cfg.hvgType < 0 ) || ( pDev->Cfg.hvgType >= XDRV_APM_HVG_DESIGN_NUM_TYPES ))
   {
      XCHG_ASSERT( 0 );
   }

   hvgChanSwap=pDev->channelSwap;
   
   /* Disable HVG block */
   hvgp->reg_hvg_soft_init |= HVG_SOFT_INIT;
   hvgp->reg_hvg_cha_reg_2 |= HVG_SOFT_INIT_0;
   hvgp->reg_hvg_chb_reg_2 |= HVG_SOFT_INIT_0;

   /* Setup the correct HVG register values */
   hvg6838RegSetup( pApmDrv );

   /* Enable HVG block */
   hvgp->reg_hvg_soft_init &= ~HVG_SOFT_INIT;

   /* Enable HVG updates based on hookstate */
   for( i=0; i<APM6838_NUM_CHANNELS; i++ )
   {
      pDev->hookStateUpdatesEnabled[i] = XDRV_TRUE;
   }   

   /* Now channels can be independantly controlled */
   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: hvgStart
*
*  PURPOSE:  Start the HVG on a channel
*
*  PARAMETERS:
*      chan  - channel to start the HVG on
*
*  RETURNS:  0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int hvg6838Start( int chan )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   XDRV_UINT32 hvgRegData;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   /* Reduce duty cycle and min on time to lower in rush current */
   hvgRegData = hvgp->reg_hvg_duty_cycle;
   hvgRegData &= ~MAX_DUTY_CYCLE;
   hvgRegData |= 0x00000010;
   hvgRegData &= ~ MIN_ON_TIME;
   hvgRegData |= 0x000C0000;
   hvgp->reg_hvg_duty_cycle = hvgRegData;

   /* Enable HVG block */
   if( chan == 0 )
   {
      hvgp->reg_hvg_cha_reg_2 &= ~HVG_SOFT_INIT_0;
   }
   else
   {
      hvgp->reg_hvg_chb_reg_2 &= ~HVG_SOFT_INIT_0;
   }

   /* Sleep after enabliing each line to reduce in-rush current */
   bosSleep( 20 );

   /* Reduce duty cycle and reset min on time to lower in rush current */
   hvgRegData = hvgp->reg_hvg_duty_cycle;
   hvgRegData &= ~MAX_DUTY_CYCLE;
   hvgRegData |= 0x00000020;
   hvgRegData &= ~ MIN_ON_TIME;
   hvgRegData |= 0x001A0000;
   hvgp->reg_hvg_duty_cycle = hvgRegData;

   /* Sleep after enabliing each line to reduce in-rush current */
   bosSleep( 80 );

   /* Reset maximum duty cycle to normal level */
   hvgRegData = hvgp->reg_hvg_duty_cycle;
   hvgRegData &= ~MAX_DUTY_CYCLE;
   hvgRegData |= 0x000000F0;
   hvgp->reg_hvg_duty_cycle = hvgRegData;

   return ( 0 );
}




/*****************************************************************************
*
*  FUNCTION: hvg6838Stop
*
*  PURPOSE:  Stop the HVG on all channels
*
*  PARAMETERS:
*      pDrv  - pointer to 3341 driver structure
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*     Currently there is no need to stop the HVG on individual channels.
*     Should such a need arise, we would need to revrite this a little.
*
*****************************************************************************/
int hvg6838Stop( int chan )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   /* Disable HVG block */
   if( chan == 0 )
   {
      hvgp->reg_hvg_cha_reg_2 |= HVG_SOFT_INIT_0;
   }
   else
   {
      hvgp->reg_hvg_chb_reg_2 |= HVG_SOFT_INIT_0;
   }

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: hvg6838Reset
*
*  PURPOSE:  Reset the HVG state machine
*
*  PARAMETERS:
*      chan  - channel to reset
*
*  RETURNS: 0 on SUCCESS
*
*  NOTES:
*
*****************************************************************************/
int hvg6838Reset( int chan )
{  
   
#if HVG_6838_CFG_APM_CHAN_SWAP
   /* Do nothing here as channel swap will be done in hvg6838Stop and hvg6838Start */
#endif /* HVG_6838_CFG_APM_CHAN_SWAP */
   
   /* Reset the digital logic */
   hvg6838Stop( chan );
   hvg6838Start( chan );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   hvg6838SetVoltage
**
** PURPOSE:    Set the battery voltage
**
** PARAMETERS: pApmDrv  - pointer to the APM driver interface
**             chan     - the line number ( 0 referenced )
**             voltage  - voltage to set
**
*  RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
int hvg6838SetVoltage( XDRV_APM *pApmDrv, int chan, int voltage )
{   
   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   return ( hvg6838SetVoltageInt( pApmDrv, chan, voltage ) );
}
/*
*****************************************************************************
** FUNCTION:   hvg6838SetVoltageInt
**
** PURPOSE:    Set the battery voltage Internal Function. Should only be called from
**             within HVG code
**
** PARAMETERS: pApmDrv  - pointer to the APM driver interface
**             chan     - the line number ( 0 referenced )
**             voltage  - voltage to set
**
*  RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
static int hvg6838SetVoltageInt( XDRV_APM *pApmDrv, int chan, int voltage )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   XDRV_UINT16 volt_goal;
   XDRV_UINT32 hvgRegData;
   
   /* Select the correct reference voltage */
   switch (voltage)
   {
      case XDRV_APM_HVG_VOLTAGE_OFFHOOK:
      {
         /* We now always use default value in any offhook state */
         if ( chan == 0 )
         {
            /* Clear HVG mode and set to FASD mode */
            hvgRegData = hvgp->reg_hvg_cha_reg_2;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_OFFHOOK_FASD;
            hvgp->reg_hvg_cha_reg_2 = hvgRegData;
         }
         else
         {
            /* Clear HVG mode and set to FASD mode */
            hvgRegData = hvgp->reg_hvg_chb_reg_2;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_OFFHOOK_FASD;
            hvgp->reg_hvg_chb_reg_2 = hvgRegData;
         }
      }
      break;

      case XDRV_APM_HVG_VOLTAGE_TEST1:
      case XDRV_APM_HVG_VOLTAGE_TEST2:
      case XDRV_APM_HVG_VOLTAGE_ONHOOK:
      case XDRV_APM_HVG_VOLTAGE_ONHOOK_LP:
      {
         /* Determine voltage level for this state */
         if(( voltage == XDRV_APM_HVG_VOLTAGE_TEST1 ) ||
            ( voltage == XDRV_APM_HVG_VOLTAGE_TEST2 ))
         {
            volt_goal = 0x29B;
         }
         else if( voltage == XDRV_APM_HVG_VOLTAGE_ONHOOK_LP )
         {
            volt_goal = regCustomTable[pDev->Cfg.hvgType].reg_hvg_const_volt_goal_lp;
         }
         else
         {
            /* Adjust voltage in standby mode */
            if( pDev->slicState[chan] == XDRV_SLIC_MODE_STANDBY )
            {
               volt_goal = regCustomTable[pDev->Cfg.hvgType].reg_hvg_const_volt_goal_sb;
            }
            else
            {
               volt_goal = regCustomTable[pDev->Cfg.hvgType].reg_hvg_const_volt_goal;
            }
         }

         /* Adjust voltage level */
         if ( chan == 0 )
         {
            /* Adjust settings for fixed voltage */
            hvgp->reg_hvg_cha_const_volt_goal = volt_goal;

            /* Clear HVG mode and set to constant voltage mode */
            hvgRegData = hvgp->reg_hvg_cha_reg_2;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_ONHOOK_FIXED;
            hvgp->reg_hvg_cha_reg_2 = hvgRegData;
         }
         else
         {
            /* Adjust settings for fixed voltage */
            hvgp->reg_hvg_chb_const_volt_goal = volt_goal;

            /* Clear HVG mode and set to constant voltage mode */
            hvgRegData = hvgp->reg_hvg_chb_reg_2;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_ONHOOK_FIXED;
            hvgp->reg_hvg_chb_reg_2 = hvgRegData;
         }
      }
      break;

      case XDRV_APM_HVG_VOLTAGE_RING:
      case XDRV_APM_HVG_VOLTAGE_UHV_RING_13:
      case XDRV_APM_HVG_VOLTAGE_UHV_RING_15:
      {
         if ( chan == 0 )
         {
            /* Clear HVG mode and set to ring tracking mode */
            hvgp->reg_hvg_cha_reg_2 &= ~HVG_MODE;
         }
         else
         {
            /* Clear HVG mode and set to ring tracking mode */
            hvgp->reg_hvg_chb_reg_2 &= ~HVG_MODE;
         }
      }
      break;

      case XDRV_APM_HVG_VOLTAGE_NONE:
      default:
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_APM: ERROR - voltage level '%d' not supported !!!", voltage ));
         return ( -1 );
      }
      break;
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   hvg6838UpdateSlicStatus
**
** PURPOSE:    Update HVG parameters based on latest operating state
**
** PARAMETERS: pApmDrv     - pointer to the APM driver interface
**             chan        - the line number
**             slicState   - current slic mode
**             dcOffset    - ring offset enabled
**             battery     - on battery power
**             loopCurrent - boosted loop current status
**             ringStop    - set if ringing is ending
**
** RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
int hvg6838UpdateSlicStatus( XDRV_APM *pApmDrv, int chan, int slicState,
                             int dcOffset, int battery, int loopCurrent,
                             XDRV_BOOL ringStop )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   volatile BCM6838_APM *pApm = pDev->pApm;
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   volatile XDRV_UINT32* pRegApmAudioDac;
   const APM6838_FLEXICALC_CFG *flexicalcp = pDev->Cfg.pFlexiCalc;
   XDRV_APM_HVG_VOLTAGE voltage;
   int index, i;
   /* int ringing = XDRV_FALSE; */
   /* int ringingOn = XDRV_FALSE; */
   /* int dcOffsetOn = XDRV_FALSE; */
   int batteryOn = XDRV_FALSE;
   int loopCurrentOn = XDRV_FALSE;
   /* int prevSlicState; */
   XDRV_UINT32 hvgRegData;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   pRegApmAudioDac  = &pApm->reg_codec_config_4;

   /* Index of line in APM driver */
   index = chan % APM6838_NUM_CHANNELS;

   /* Determine if any parameters changed */
   if (( pDev->slicState[index] == slicState ) &&
       ( pDev->dcOffset[index] == dcOffset ) &&
       ( pDev->battery[index] == battery ) &&
       ( pDev->loopCurrent[index] == loopCurrent ) &&
       ( pDev->ringStop[index] == ringStop ))
   {
      /* Current mode on this channel is already correct 
      ** and this is not the ringing termination state */
      return ( 0 );
   }
   else
   {
      /* Remember mode parameters for future reference */
      /* prevSlicState = pDev->slicState[index]; */
      pDev->slicState[index] = slicState;
      pDev->dcOffset[index] = dcOffset;
      pDev->battery[index] = battery;
      pDev->loopCurrent[index] = loopCurrent;
      pDev->ringStop[index] = ringStop;
   }

   /* Determine if SLIC is ringing */
   if( slicState == XDRV_SLIC_MODE_RING )
   {
      /* ringing = XDRV_TRUE; */
   }

   /* Determine parameters for adjusting global HVG settings */
   for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
   {
      if( pDev->slicState[i] == XDRV_SLIC_MODE_RING )
      {
         /* ringingOn = XDRV_TRUE; */
      }

      if( pDev->dcOffset[i] )
      {
         /* dcOffsetOn = XDRV_TRUE; */
      }

      if( pDev->loopCurrent[i] )
      {
         loopCurrentOn = XDRV_TRUE;
      }
   }

   /* Determine if battery power is in use */
   if( pDev->battery[index] )
   {
      batteryOn = XDRV_TRUE;
   }

   /* Update offhook overhead voltage */
   if( loopCurrentOn == XDRV_TRUE )
   {
      hvgRegData = hvgp->reg_hvg_off_hook_setup;
      hvgRegData &= ~( OFF_HOOK_OVHD );
      hvgRegData |= regCustomTable[pDev->Cfg.hvgType].reg_hvg_off_hook_bl;
      hvgp->reg_hvg_off_hook_setup = hvgRegData;
   }
   else
   {
      hvgRegData = hvgp->reg_hvg_off_hook_setup;
      hvgRegData &= ~( OFF_HOOK_OVHD );
      hvgRegData |= regCustomTable[pDev->Cfg.hvgType].reg_hvg_off_hook;
      hvgp->reg_hvg_off_hook_setup = hvgRegData;
   }

   /* Update max duty cycle */
   if( batteryOn == XDRV_TRUE )
   {
      hvgRegData = hvgp->reg_hvg_duty_cycle;
      hvgRegData &= ~MAX_DUTY_CYCLE;
      hvgRegData |= 0x000000E0;
      hvgp->reg_hvg_duty_cycle = hvgRegData;
   }
   else
   {
      hvgRegData = hvgp->reg_hvg_duty_cycle;
      hvgRegData &= ~MAX_DUTY_CYCLE;
      hvgRegData |= 0x000000F0;
      hvgp->reg_hvg_duty_cycle = hvgRegData;
   }

   /* Determine the battery voltage associated with the current state */
   voltage = hvg6838GetVoltage( pDev->slicState[index], pDev->hookState[index], pDev->battery[index] );

   /* Set the desired battery voltage */
   hvg6838SetVoltageInt( pApmDrv, chan, voltage );

   /* Update HVG switching parameters */
   hvg6838UpdateHvgParms( pApmDrv, chan );

   /* Platform specific settings */
   switch( pDev->Cfg.hvgType )
   {
      case( XDRV_APM_HVG_DESIGN_BUCKBOOST ):
      case( XDRV_APM_HVG_DESIGN_FLYBACK ):
      {
         if( chan == 0 )
         {
            if( slicState == XDRV_SLIC_MODE_RING )
            {
               if( ringStop )
               {
                  pApm->reg_ring_config_1 |= RING_STOP_NEXT_PHASE_A;
                  pApm->reg_ring_config_1 &= ~RING_STOP_NEXT_PHASE_A;
               }
               else
               {
                  pApm->reg_ring_config_1 |= RING_START_NEXT_PHASE_A;
                  pApm->reg_ring_config_1 &= ~RING_START_NEXT_PHASE_A;
                  *pRegApmAudioDac &= ~PGA_GAIN_CTL_VDAC_A;
               }

               hvgRegData = pApm->reg_cha_cic_config;
               hvgRegData &= ~CIC_INC_SHFT;
               hvgRegData |= ( ( 3 << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
               pApm->reg_cha_cic_config = hvgRegData;

               hvgRegData = pApm->reg_cha_test_config;
               hvgRegData &= ~RX_98K_MUX_CTRL;
               hvgRegData |= ( ( 2 << RX_98K_MUX_CTRL_SHIFT ) & RX_98K_MUX_CTRL );
               pApm->reg_cha_test_config = hvgRegData;
            }
            else
            {
               pApm->reg_ring_config_1 |= RING_STOP_IMMED_A;
               pApm->reg_ring_config_1 &= ~RING_STOP_IMMED_A;

               hvgRegData = *pRegApmAudioDac;
               hvgRegData &= ~PGA_GAIN_CTL_VDAC_A;
               hvgRegData |= (( pDev->vDac[chan] << PGA_GAIN_CTL_VDAC_A_SHIFT ) & PGA_GAIN_CTL_VDAC_A );
               *pRegApmAudioDac = hvgRegData;

               hvgRegData = pApm->reg_cha_cic_config;
               hvgRegData &= ~CIC_INC_SHFT;
               hvgRegData |= (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
               pApm->reg_cha_cic_config = hvgRegData;

               pApm->reg_cha_test_config &= ~RX_98K_MUX_CTRL;
            }
         }
         else
         {
            if( slicState == XDRV_SLIC_MODE_RING )
            {
               if( ringStop )
               {
                  pApm->reg_ring_config_1 |= RING_STOP_NEXT_PHASE_B;
                  pApm->reg_ring_config_1 &= ~RING_STOP_NEXT_PHASE_B;
               }
               else
               {
                  pApm->reg_ring_config_1 |= RING_START_NEXT_PHASE_B;
                  pApm->reg_ring_config_1 &= ~RING_START_NEXT_PHASE_B;
                  *pRegApmAudioDac &= ~PGA_GAIN_CTL_VDAC_B;
               }

               hvgRegData = pApm->reg_chb_cic_config;
               hvgRegData &= ~CIC_INC_SHFT;
               hvgRegData |= ( ( 3 << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
               pApm->reg_chb_cic_config = hvgRegData;

               hvgRegData = pApm->reg_chb_test_config;
               hvgRegData &= ~RX_98K_MUX_CTRL;
               hvgRegData |= ( ( 2 << RX_98K_MUX_CTRL_SHIFT ) & RX_98K_MUX_CTRL );
               pApm->reg_chb_test_config = hvgRegData;
            }
            else
            {
               pApm->reg_ring_config_1 |= RING_STOP_IMMED_B;
               pApm->reg_ring_config_1 &= ~RING_STOP_IMMED_B;

               hvgRegData = *pRegApmAudioDac;
               hvgRegData &= ~PGA_GAIN_CTL_VDAC_B;
               hvgRegData |= (( pDev->vDac[chan] << PGA_GAIN_CTL_VDAC_B_SHIFT ) & PGA_GAIN_CTL_VDAC_B );
               *pRegApmAudioDac = hvgRegData;

               hvgRegData = pApm->reg_chb_cic_config;
               hvgRegData &= ~CIC_INC_SHFT;
               hvgRegData |= (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
               pApm->reg_chb_cic_config = hvgRegData;

               pApm->reg_chb_test_config &= ~RX_98K_MUX_CTRL;
            }
         }
      }
      break;

      default:
      {
         /* Nothing to do for other HVG types */
      }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   hvg6838UpdateHookStatus
**
** PURPOSE:    Update HVG parameters based on latest hook state
**
** PARAMETERS: pApmDrv     - pointer to the APM driver interface
**             chan        - the line number
**             hookState   - current hook state (zero for onhook)
**
** RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
int hvg6838UpdateHookStatus( XDRV_APM *pApmDrv, int chan, int hookState )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   XDRV_APM_HVG_VOLTAGE voltage;
   int index;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   /* Index of line in APM driver */
   index = chan % APM6838_NUM_CHANNELS;

   /* Record new hookstate */
   pDev->hookState[index] = hookState;

   /* Update HVG settings if enabled */
   if( pDev->hookStateUpdatesEnabled[index] )
   {
      /* Determine the battery voltage associated with the current state */
      voltage = hvg6838GetVoltage( pDev->slicState[index], pDev->hookState[index], pDev->battery[index] );

      /* Set the desired battery voltage */
      hvg6838SetVoltageInt( pApmDrv, chan, voltage );

      /* Set the desired battery voltage */
      hvg6838UpdateHvgParms( pApmDrv, chan );
   }

   return( 0 );
}


/*
*****************************************************************************
** FUNCTION:   hvg6838UpdateHvgParms
**
** PURPOSE:    Update HVG switching parameters
**
** PARAMETERS: pApmDrv     - pointer to the APM driver interface
**             chan        - the line number
**
** RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
static int hvg6838UpdateHvgParms( XDRV_APM *pApmDrv, int chan )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   volatile HvgControlRegisters *hvgp = HVG;
   int index;
   int i;
   int loopCurrentOn = XDRV_FALSE;
   int hvgHighSenseMode = XDRV_TRUE;
   XDRV_UINT32 hvgRegData;
   XDRV_UINT32 hvgKpropKint;
   XDRV_UINT32 hvgTrackMax;
   XDRV_UINT32 hvgOffhookOvhd;
  
   /* Index of line in APM driver */
   index = chan % APM6838_NUM_CHANNELS;

   /* Determine parameters for adjusting global HVG settings */
   for( i = 0; i < APM6838_NUM_CHANNELS; i++ )
   {
      if( pDev->loopCurrent[i] )
      {
         loopCurrentOn = XDRV_TRUE;
      }
   }

   /* Initialize new HVG track MAX value to default */
   hvgTrackMax = regCustomTable[pDev->Cfg.hvgType].reg_hvg_offhook_track_max;

   /* Initialize the offhook overhead */
   if( loopCurrentOn == XDRV_TRUE )
   {
      hvgOffhookOvhd = regCustomTable[pDev->Cfg.hvgType].reg_hvg_off_hook_bl;
   }
   else
   {
      hvgOffhookOvhd = regCustomTable[pDev->Cfg.hvgType].reg_hvg_off_hook;
   }

   /* Use non-default value if either line is in RT state */
   for( i = 0; i < 2; i++ )
   {
      if(( pDev->slicState[index] == XDRV_SLIC_MODE_RING ) &&
         ( pDev->hookState[index] ))
      {
         hvgTrackMax = ( regCustomTable[pDev->Cfg.hvgType].reg_hvg_ring_track_max << 20 );
         hvgOffhookOvhd = 0x1FF;
         hvgHighSenseMode = XDRV_FALSE;
      }
   }

   /* Determine appropriate HVG analysis parameters */
   switch( pDev->slicState[index] )
   {
      case XDRV_SLIC_MODE_LCFO:
      case XDRV_SLIC_MODE_TIPOPEN:
      {
         hvgKpropKint = 0x00000095;
      }
      break;

      case XDRV_SLIC_MODE_RING:
      {
         hvgKpropKint = 0x00000094;
      }
      break;

      case XDRV_SLIC_MODE_STANDBY:
      case XDRV_SLIC_MODE_OHT:
      case XDRV_SLIC_MODE_OHTR:
      case XDRV_SLIC_MODE_WINK:
      {
         if( pDev->hookState[index] )
         {
            hvgKpropKint = 0x00000095;
         }
         else
         {
            hvgKpropKint = 0x00000095;
         }
      }
      break;

      case XDRV_SLIC_MODE_LCF:
      case XDRV_SLIC_MODE_RLCF:
      {
         if( pDev->hookState[index] )
         {
            hvgKpropKint = 0x00000095;
         }
         else
         {
            hvgKpropKint = 0x00000095;
         }
      }
      break;

      default:
      {
         return( 1 );
      }
      break;
   }

   /* Update HVG analysis parameters */
   if( index == 0 )
   {
      hvgRegData = hvgp->reg_hvg_cha_reg_0;
      if( hvgHighSenseMode )
      {
         hvgRegData |= VTIP_VRING_HI_SENS_MODE;
      }
         else
      {
         hvgRegData &= ~VTIP_VRING_HI_SENS_MODE;
      }
      hvgp->reg_hvg_cha_reg_0 = hvgRegData;

      hvgRegData = hvgp->reg_hvg_cha_reg_2;
      hvgRegData &= ~( K_PROP | K_INTEG );
      hvgRegData |= hvgKpropKint;
      hvgp->reg_hvg_cha_reg_2 = hvgRegData;
   }
   else
   {
      hvgRegData = hvgp->reg_hvg_chb_reg_0;
      if( hvgHighSenseMode )
      {
         hvgRegData |= VTIP_VRING_HI_SENS_MODE;
      }
      else
      {
         hvgRegData &= ~VTIP_VRING_HI_SENS_MODE;
      }
      hvgp->reg_hvg_chb_reg_0 = hvgRegData;

      hvgRegData = hvgp->reg_hvg_chb_reg_2;
      hvgRegData &= ~( K_PROP | K_INTEG );
      hvgRegData |= hvgKpropKint;
      hvgp->reg_hvg_chb_reg_2 = hvgRegData;
   }

   hvgRegData = hvgp->reg_hvg_off_hook_setup;
   hvgRegData &= ~( OFF_HOOK_MAX | OFF_HOOK_OVHD );
   hvgRegData |= hvgTrackMax | hvgOffhookOvhd;
   hvgp->reg_hvg_off_hook_setup = hvgRegData;

   return( 0 );
}


/*****************************************************************************
*
*  FUNCTION:   hvg6838IsEnabled
*
*  PURPOSE:    Check if HVG is enabled or disabled on channel.
*
*  PARAMETERS: chan - the line number ( 0 referenced )
*
*  RETURNS:    1 - enabled, 0 - disabled
*
*  NOTES:
*
*****************************************************************************/
int hvg6838IsEnabled( int chan )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;
   volatile XDRV_UINT32 *pHvgCtrl;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;

   if ( chan == 0 )
   {
      pHvgCtrl = &hvgp->reg_hvg_cha_reg_2;
   }
   else
   {
      pHvgCtrl = &hvgp->reg_hvg_chb_reg_2;
   }

   if (!( *pHvgCtrl & HVG_SOFT_INIT_0 ))
   {
      return ( 1 );
   }
   else
   {
      return ( 0 );
   }
}

/*
*****************************************************************************
** FUNCTION:   hvg6838GetOverCurrentStatus
**
** PURPOSE:    Check to see if the HVG over current protection has kicked in
**
** PARAMETERS: pDrv  - pointer to the APM driver device structure
**             chan  - channel for which the over current status should be
**                     checked
**
** RETURNS:    XDRV_TRUE  - If the over current protection has kicked in on
**                          the HVG registers
**             XDRV_FALSE - Otherwise
**
** NOTES:      none
**
*****************************************************************************
*/
XDRV_BOOL hvg6838GetOverCurrentStatus( XDRV_APM *pDrv, int chan )
{
   volatile BCM6838_HVG *hvgp = BCM6838_HVG_PTR;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   if(( chan == 0 ) && ( hvgp->reg_hvg_status6 & HVG_SHORT_FLAG_A ))
   {
      /* Record status info */
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG overcurrent condtion on line %d !!!", chan ));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG hvg voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status7 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG tip voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status9 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG ring voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status11 )));

      /* Shutdown HVG */
      hvgp->reg_hvg_cha_reg_2 |= HVG_SOFT_INIT_0;

      return XDRV_TRUE;
   }
   else if(( chan == 1 ) && ( hvgp->reg_hvg_status6 & HVG_SHORT_FLAG_B ))
   {
      /* Record status info */
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG overcurrent condtion on line %d !!!", chan ));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG hvg voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status8 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG tip voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status10 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG ring voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status12 )));

      /* Shutdown HVG */
      hvgp->reg_hvg_chb_reg_2 |= HVG_SOFT_INIT_0;

      return XDRV_TRUE;
   }
   else
   {
      return XDRV_FALSE;
   }
}

/*
*****************************************************************************
** FUNCTION:   hvg6838GetOverVoltageStatus
**
** PURPOSE:    Check to see if an HVG over voltage condition exists
**
** PARAMETERS: pDrv  - pointer to the APM driver device structure
**             chan  - channel for which the over voltage status should be
**                     checked
**
** RETURNS:    none
**
** NOTES:      none
**
*****************************************************************************
*/
void hvg6838GetOverVoltageStatus( XDRV_APM *pDrv, int chan )
{
   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   /* Nothing to do here */
   return;
}

/*
*****************************************************************************
** FUNCTION:   hvg6838GetVoltage
**
** PURPOSE:    Look up the voltage associated with the SLIC mode
**
** PARAMETERS: mode       - SLIC mode
**             hookState  - current hook state
**             batttery   - battery status
**
** RETURNS:    associated voltage
**
*****************************************************************************
*/
static XDRV_APM_HVG_VOLTAGE hvg6838GetVoltage( XDRV_UINT16 mode, XDRV_UINT16 hookState, int battery )
{
   const MODEVOLTAGEMAP *pVoltageMap = modeVoltageMap;
   XDRV_APM_HVG_VOLTAGE voltage;

   while ( pVoltageMap->mode != XDRV_SLIC_MODE_NULL )
   {
      if ( pVoltageMap->mode == mode )
      {
         if( hookState )
         {
            voltage = pVoltageMap->offHookVoltage;
         }
         else
         {
            voltage = pVoltageMap->onHookVoltage;
         }

         /* Adjust voltage if necessary */
         if(( voltage == XDRV_APM_HVG_VOLTAGE_ONHOOK ) &&
            ( battery & XDRV_APM_ENHANCED_CONTROL_ENABLED ))
         {
            return ( XDRV_APM_HVG_VOLTAGE_ONHOOK_LP );
         }
         else
         {
            return ( voltage );
         }
      }
      else
      {
         pVoltageMap++;
      }
   }

   return ( XDRV_APM_HVG_VOLTAGE_NONE );
}

/*
*****************************************************************************
** FUNCTION:   hvg6838UpdateHookStatusEnable
**
** PURPOSE:    Disable HVG changes on hook state updates
**
** PARAMETERS: pApmDrv     - pointer to the APM driver interface
**             chan        - the line number
**             update      - enable or disable updates
**
** RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
int hvg6838UpdateHookStatusEnable( XDRV_APM *pApmDrv, int chan, XDRV_BOOL update )
{
   APM_6838_DRV *pDev = (APM_6838_DRV *)pApmDrv;
   int index;

   chan = hvgChanSwap == XDRV_TRUE ? (APM6838_NUM_CHANNELS-1) - chan : chan;
   
   /* Index of line in APM driver */
   index = chan % APM6838_NUM_CHANNELS;

   /* Record new hookstate */
   pDev->hookStateUpdatesEnabled[index] = update;

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   hvgExtend21to32
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
static XDRV_SINT32 hvgExtend21to32( XDRV_UINT32 value )
{
   return (XDRV_SINT32)( value & 0x00100000 ? (XDRV_UINT32)value | 0xFFE00000 :
                                              (XDRV_UINT32)value & 0x001FFFFF );
}


