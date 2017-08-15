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
*    Filename: hvg6818.c
*
****************************************************************************
*
*    Description: HVG (high voltage generateor) device driver for BCM6818
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <xdrvTypes.h>
#include <xchgAssert.h>
#include <xdrvSlic.h>
#include <xdrvLog.h>
#include <bosSleep.h>
#include "hvg6818.h"
#include "apm6818.h"
#include <hvg6818Cfg.h>

typedef struct BCM6818_HVG_CUSTOM
{
   /* HVG Shared Settings */
   XDRV_UINT32 reg_hvg_track_max;          /* Reg 7 */ 
   XDRV_UINT32 reg_hvg_ring;               /* Reg 9 */ 
   XDRV_UINT16 reg_hvg_off_hook;           /* Reg 10 */ 
   XDRV_UINT16 reg_hvg_off_hook_bl;        /* Reg 10 */ 

   /* HVG Channel Settings */
   XDRV_UINT16 reg_hvg_max_hvg_slic;       /* Reg 14,18 - Upper 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal;    /* Reg 14,18 - Lower 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal_lp; /* Reg 14,18 - Lower 16 Bits */

} BCM6818_HVG_CUSTOM;

/*
** This is the 6818 custom register table with values customized for
** specific HW layouts with specific SLICs.
*/

static BCM6818_HVG_CUSTOM regCustomTable[HVG_DESIGN_NUM_MAX][XDRV_APM_HVG_DESIGN_NUM_TYPES] =
{
  {
    /* for all HVG design on 6818Chip */
   { 0x02DD078D,   0x006B0F7E,   0x00B2,   0x00D3,   0x0000,   0x02DD,   0x019D   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x03200708,   0x02790922,   0x00A0,   0x00BA,   0x0700,   0x0320,   0x0215   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   }
  },
  {
    /* for 6818GRRG2LBOSA board HVG design */
   { 0x02DD051A,   0x006B0F7E,   0x00B2,   0x00D3,   0x0000,   0x02DD,   0x019D   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x0320051A,   0x02790922,   0x00A0,   0x00BA,   0x0700,   0x0320,   0x0215   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   },
   { 0x00000000,   0x00000000,   0x0000,   0x0000,   0x0000,   0x0000,   0x0000   }
  }
};
static XDRV_SINT32 hvgSubType = HVG_DESIGN_GENERAL;
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
   { XDRV_SLIC_MODE_TIPOPEN,   XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { XDRV_SLIC_MODE_RING,      XDRV_APM_HVG_VOLTAGE_RING,      XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_LCF,       XDRV_APM_HVG_VOLTAGE_OFFHOOK,   XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_RLCF,      XDRV_APM_HVG_VOLTAGE_OFFHOOK,   XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_WINK,      XDRV_APM_HVG_VOLTAGE_ONHOOK,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { XDRV_SLIC_MODE_NULL,      XDRV_APM_HVG_VOLTAGE_NONE,      XDRV_APM_HVG_VOLTAGE_NONE     }
};

static int hvg6818RegSetup( XDRV_APM *pApmDrv );
static int hvg6818UpdateHvgParms( XDRV_APM *pApmDrv, int chan );
static XDRV_APM_HVG_VOLTAGE hvg6818GetVoltage( XDRV_UINT16 mode, XDRV_UINT16 hookState, int battery );
static XDRV_SINT32 hvgExtend21to32( XDRV_UINT32 value );

/*****************************************************************************
**
**  FUNCTION:   hvg6818RegSetup
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
static int hvg6818RegSetup( XDRV_APM *pApmDrv )
{
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   /* HVG Shared Settings */
   hvgp->reg_hvg_scale_vtip      =  0x3FC8;          /* Reg 0 - Upper 16 Bits */
   hvgp->reg_hvg_scale_vring     =  0x3FC8;          /* Reg 1 - Lower 16 Bits */
   hvgp->reg_hvg_scale_vcal      =  0x0000;          /* Reg 2 - Upper 16 Bits */
   hvgp->reg_hvg_offset_vtip     =  0x180F;          /* Reg 3 - Upper 16 Bits */
   hvgp->reg_hvg_offset_vring    =  0x180F;          /* Reg 4 - Lower 16 Bits */
   hvgp->reg_hvg_offset_vcal     =  0x0000;          /* Reg 5 - Upper 16 Bits */
   hvgp->reg_hvg_offset_vinput   =  0x0000;          /* Reg 5 - Lower 16 Bits */
   hvgp->reg_hvg_shift          &=  ~COND_SHIFT_V_HVG & ~COND_SHIFT_V_TIP & ~COND_SHIFT_V_RING;/*Reg 6 Lower 12 Bits*/

   if( hvgSubType == HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
   {
      hvgp->reg_hvg_scale_vhvg      =  0x1811;          /* Reg 0 - Lower 16 Bits */
      hvgp->reg_hvg_scale_vloop     =  0x07FB;          /* Reg 1 - Upper 16 Bits */  
      hvgp->reg_hvg_offset_vhvg     =  0x1A0C;          /* Reg 3 - Lower 16 Bits */
      hvgp->reg_hvg_offset_vloop    =  0x1E11;          /* Reg 4 - Upper 16 Bits */
      hvgp->reg_hvg_duty_cycle      =  0x991A01F0;      /* Reg 8 */  
      hvgp->reg_hvg_bg              =  0x08000000;      /* Reg 11 */
   }
   else
   {
      hvgp->reg_hvg_scale_vhvg      =  0x225E;          /* Reg 0 - Lower 16 Bits */
      hvgp->reg_hvg_scale_vloop     =  0x392A;          /* Reg 1 - Upper 16 Bits */
      hvgp->reg_hvg_offset_vhvg     =  0x1779;          /* Reg 3 - Lower 16 Bits */
      hvgp->reg_hvg_offset_vloop    =  0x1A0C;          /* Reg 4 - Upper 16 Bits */
      hvgp->reg_hvg_duty_cycle      =  0x991A01A0;      /* Reg 8 */  
      hvgp->reg_hvg_bg              =  0x20000000;      /* Reg 11 */
   }

   /* Scale setting depends on input voltage */
   if( pDev->Cfg.inputSupplyVoltage == 12 )
   {
      hvgp->reg_hvg_scale_vinput  = HVG_6818_INPUT_SCALE_12V;  /* Reg 2 - Lower 16 Bits */
      hvgp->reg_hvg_vbat          = HVG_6818_VBAT_SCALE_12V;   /* Reg 12 */
   }
   else
   {
      hvgp->reg_hvg_scale_vinput  = HVG_6818_INPUT_SCALE_15V;  /* Reg 2 - Lower 16 Bits */
      hvgp->reg_hvg_vbat          = HVG_6818_VBAT_SCALE_15V;   /* Reg 12 */
   }

   /* Utilize BG OTP trim value */
   hvgp->reg_hvg_shift           |= BG_TRIM_SOURCE;

   /* HVG Channel A Settings */
   
   hvgp->reg_hvg_cha_window_ctrl       =  hvgSubType == HVG_DESIGN_GRRG2LBOSA? 0x07878448 : 0x07777337; /* Reg 13 */
   hvgp->reg_hvg_cha_misc              =  0x0000E884;      /* Reg 15 */
   hvgp->reg_hvg_cha_spare             =  0x00000000;      /* Reg 16 */

   /* HVG Channel B Settings */
   hvgp->reg_hvg_chb_window_ctrl       =  hvgSubType == HVG_DESIGN_GRRG2LBOSA? 0x07878448 : 0x07777337; /* Reg 17 */
   hvgp->reg_hvg_chb_misc              =  0x0000E884;      /* Reg 19 */
   hvgp->reg_hvg_chb_spare             =  0x00000000;      /* Reg 20 */

   /* Platform Specific Settings */
   /* HVG Shared Settings */
   hvgp->reg_hvg_track_max             =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_track_max;         /* Reg 7 */ 
   hvgp->reg_hvg_ring                  =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_ring;              /* Reg 9 */ 
   hvgp->reg_hvg_off_hook              =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_off_hook |         /* Reg 10 */
                                          0x0001E000;

   /* HVG Channel A Settings */
   hvgp->reg_hvg_cha_max_hvg_slic      =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_max_hvg_slic;      /* Reg 14 - Upper 16 Bits */
   hvgp->reg_hvg_cha_const_volt_goal   =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_const_volt_goal;   /* Reg 14 - Lower 16 Bits */

   /* HVG Channel B Settings */
   hvgp->reg_hvg_chb_max_hvg_slic      =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_max_hvg_slic;      /* Reg 18 - Upper 16 Bits */
   hvgp->reg_hvg_chb_const_volt_goal   =  regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_const_volt_goal;   /* Reg 18 - Lower 16 Bits */

   /* Program new settings for modified PWM */
   hvgp->reg_hvg_aux0               =  BMU_HVG_ADC_CLK_MODE;
   hvgp->reg_hvg_aux3               =  hvgSubType == HVG_DESIGN_GRRG2LBOSA? 0x0000C2B7 : 0x0000C308;
   
   if( hvgSubType != HVG_DESIGN_GRRG2LBOSA )
   {
      /* Override default current measurements factors */
      hvgp->reg_hvg_scale_vloop        =  0x08C3;          /* Reg 1 - Upper 16 Bits */
      hvgp->reg_hvg_offset_vloop       =  0x1DDF;          /* Reg 4 - Upper 16 Bits */

      /* Override default window settings */
      hvgp->reg_hvg_cha_window_ctrl    =  0x07878448;      /* Reg 13 */
      hvgp->reg_hvg_chb_window_ctrl    =  0x07878448;      /* Reg 17 */
   }
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
int hvg6818Init( XDRV_APM *pApmDrv )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   int i;

   /* Ensure valid HVG type */
   if((( pDev->Cfg.hvgType    < 0 ) || ( pDev->Cfg.hvgType    >= XDRV_APM_HVG_DESIGN_NUM_TYPES )) ||
      (( pDev->Cfg.hvgSubType < 0 ) || ( pDev->Cfg.hvgSubType >= HVG_DESIGN_NUM_MAX )) )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Unsupported HVG type or sub type!" ));
      XCHG_ASSERT( 0 );
   }
   
   hvgSubType = ( XDRV_SINT32 )pDev->Cfg.hvgSubType;

   /* Disable HVG block */
   hvgp->reg_hvg_shift |= HVG_SOFT_INIT;
   hvgp->reg_hvg_cha_misc |= HVG_SOFT_INIT_0;
   hvgp->reg_hvg_chb_misc |= HVG_SOFT_INIT_0;

   /* Setup the correct HVG register values */
   hvg6818RegSetup( pApmDrv );

   /* Enable HVG block */
   hvgp->reg_hvg_shift &= ~HVG_SOFT_INIT;

   /* Enable HVG updates based on hookstate */
   for( i=0; i<APM6818_NUM_CHANNELS; i++ )
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
int hvg6818Start( int chan )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   XDRV_UINT32 hvgRegData;

   /* Reduce duty cycle to lower in rush current */
   hvgRegData = hvgp->reg_hvg_duty_cycle;
   hvgRegData &= ~MAX_DUTY_CYCLE;
   hvgRegData |= 0x00000020;
   hvgp->reg_hvg_duty_cycle = hvgRegData;

   /* Enable HVG block */
   if( chan == 0 )
   {
      hvgp->reg_hvg_cha_misc &= ~HVG_SOFT_INIT_0;
   }
   else
   {
      hvgp->reg_hvg_chb_misc &= ~HVG_SOFT_INIT_0;
   }

   /* Sleep after enabliing each line to reduce in-rush current */
   bosSleep( 20 );

   /* Reset maximum duty cycle to normal level */
   hvgRegData = hvgp->reg_hvg_duty_cycle;
   hvgRegData &= ~MAX_DUTY_CYCLE;
   hvgRegData |= hvgSubType == HVG_DESIGN_GRRG2LBOSA? 0x000000F0 : 0x000000A0;
   hvgp->reg_hvg_duty_cycle = hvgRegData;

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: hvg6818Stop
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
int hvg6818Stop( int chan )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;

   /* Disable HVG block */
   if( chan == 0 )
   {
      hvgp->reg_hvg_cha_misc |= HVG_SOFT_INIT_0;
   }
   else
   {
      hvgp->reg_hvg_chb_misc |= HVG_SOFT_INIT_0;
   }

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION: hvg6818Reset
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
int hvg6818Reset( int chan )
{
   /* Reset the digital logic */
   hvg6818Stop( chan );
   hvg6818Start( chan );

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   hvg6818SetVoltage
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
int hvg6818SetVoltage( XDRV_APM *pApmDrv, int chan, int voltage )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   XDRV_UINT16 volt_goal;
   XDRV_UINT32 hvgRegData;

   /* Select the correct reference voltage */
   switch (voltage)
   {
      case XDRV_APM_HVG_VOLTAGE_OFFHOOK:
      {
         if ( chan == 0 )
         {
            /* Clear HVG mode and set to FASD or Tracking mode */
            hvgRegData = hvgp->reg_hvg_cha_misc;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= hvgSubType == HVG_DESIGN_GRRG2LBOSA? HVG_MODE_OFFHOOK_FASD : HVG_MODE_OFFHOOK_TRACKING;
            hvgp->reg_hvg_cha_misc = hvgRegData;
         }
         else
         {
            /* Clear HVG mode and set to FASD or Tracking mode */
            hvgRegData = hvgp->reg_hvg_chb_misc;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= hvgSubType == HVG_DESIGN_GRRG2LBOSA? HVG_MODE_OFFHOOK_FASD : HVG_MODE_OFFHOOK_TRACKING;
            hvgp->reg_hvg_chb_misc = hvgRegData;
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
            volt_goal = 0x14D;
         }
         else if( voltage == XDRV_APM_HVG_VOLTAGE_ONHOOK_LP )
         {
            volt_goal = regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_const_volt_goal_lp;
         }
         else
         {
            volt_goal = regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_const_volt_goal;
         }

         /* Adjust voltage level */
         if ( chan == 0 )
         {
            /* Adjust settings for constant 55V */
            hvgp->reg_hvg_cha_const_volt_goal = volt_goal;

            /* Clear HVG mode and set to constant voltage mode */
            hvgRegData = hvgp->reg_hvg_cha_misc;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_ONHOOK_FIXED;
            hvgp->reg_hvg_cha_misc = hvgRegData;
         }
         else
         {
            /* Adjust settings for constant 55V */
            hvgp->reg_hvg_chb_const_volt_goal = volt_goal;

            /* Clear HVG mode and set to constant voltage mode */
            hvgRegData = hvgp->reg_hvg_chb_misc;
            hvgRegData &= ~HVG_MODE;
            hvgRegData |= HVG_MODE_ONHOOK_FIXED;
            hvgp->reg_hvg_chb_misc = hvgRegData;
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
            hvgp->reg_hvg_cha_misc &= ~HVG_MODE;
         }
         else
         {
            /* Clear HVG mode and set to ring tracking mode */
            hvgp->reg_hvg_chb_misc &= ~HVG_MODE;
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
** FUNCTION:   hvg6818UpdateSlicStatus
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
int hvg6818UpdateSlicStatus( XDRV_APM *pApmDrv, int chan, int slicState,
                             int dcOffset, int battery, int loopCurrent,
                             XDRV_BOOL ringStop )
{
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   volatile BCM6818_APM *pApm = pDev->pApm;
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   
   const APM6818_FLEXICALC_CFG *flexicalcp = pDev->Cfg.pFlexiCalc;
   XDRV_APM_HVG_VOLTAGE voltage;
   int index, i;
   int ringingOn = XDRV_FALSE;
   int batteryOn = XDRV_FALSE;
   int loopCurrentOn = XDRV_FALSE;
   int prevSlicState;
   XDRV_UINT32 hvgRegData;
   /* Index of line in APM driver */
   index = chan % APM6818_NUM_CHANNELS;

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
      prevSlicState = pDev->slicState[index];
      pDev->slicState[index] = slicState;
      pDev->dcOffset[index] = dcOffset;
      pDev->battery[index] = battery;
      pDev->loopCurrent[index] = loopCurrent;
      pDev->ringStop[index] = ringStop;
   }

   /* Determine parameters for adjusting global HVG settings */
   for( i = 0; i < APM6818_NUM_CHANNELS; i++ )
   {
      if( pDev->slicState[i] == XDRV_SLIC_MODE_RING )
      {
         ringingOn = XDRV_TRUE;
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
      hvgRegData = hvgp->reg_hvg_off_hook;
      hvgRegData &= ~( OFF_HOOK_OVHD );
      hvgRegData |= regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_off_hook_bl;
      hvgp->reg_hvg_off_hook = hvgRegData;
   }
   else
   {
      hvgRegData = hvgp->reg_hvg_off_hook;
      hvgRegData &= ~( OFF_HOOK_OVHD );
      hvgRegData |= regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_off_hook;
      hvgp->reg_hvg_off_hook = hvgRegData;
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
      hvgRegData |= hvgSubType == HVG_DESIGN_GRRG2LBOSA? 0x000000F0 : 0x000000A0;
      hvgp->reg_hvg_duty_cycle = hvgRegData;
   }
   /* Determine the battery voltage associated with the current state */
   voltage = hvg6818GetVoltage( pDev->slicState[index], pDev->hookState[index], pDev->battery[index] );
   /* Set the desired battery voltage */
   hvg6818SetVoltage( pApmDrv, chan, voltage );
   /* Update HVG switching parameters */
   hvg6818UpdateHvgParms( pApmDrv, chan );
   /* Platform specific settings */
   switch( pDev->Cfg.hvgType )
   {
      case( XDRV_APM_HVG_DESIGN_BUCKBOOST ):
      {
         if( chan == 0 )
         {
            if( slicState == XDRV_SLIC_MODE_RING )
            {
               if( ringStop )
               {
                  pApm->reg_ring_config_4 |= RING_STOP_NEXT_BREAK_A;
                  pApm->reg_ring_config_4 &= ~RING_STOP_NEXT_BREAK_A;
               }
               else
               {
                  pApm->reg_ring_config_3 |= RING_START_CADENCE_A;
                  pApm->reg_ring_config_3 &= ~RING_START_CADENCE_A;
                  pApm->reg_codec_config_4 &= ~PGA_GAIN_CTL_VDAC_A;
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
               pApm->reg_ring_config_4 |= RING_STOP_IMMED_A;
               pApm->reg_ring_config_4 &= ~RING_STOP_IMMED_A;

               hvgRegData = pApm->reg_codec_config_4;
               hvgRegData &= ~PGA_GAIN_CTL_VDAC_A;
               hvgRegData |= (( pDev->vDac[chan] << PGA_GAIN_CTL_VDAC_A_SHIFT ) & PGA_GAIN_CTL_VDAC_A );
               pApm->reg_codec_config_4 = hvgRegData;

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
                  pApm->reg_ring_config_4 |= RING_STOP_NEXT_BREAK_B;
                  pApm->reg_ring_config_4 &= ~RING_STOP_NEXT_BREAK_B;
               }
               else
               {
                  pApm->reg_ring_config_3 |= RING_START_CADENCE_B;
                  pApm->reg_ring_config_3 &= ~RING_START_CADENCE_B;
                  pApm->reg_codec_config_4 &= ~PGA_GAIN_CTL_VDAC_B;
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
               pApm->reg_ring_config_4 |= RING_STOP_IMMED_B;
               pApm->reg_ring_config_4 &= ~RING_STOP_IMMED_B;

               hvgRegData = pApm->reg_codec_config_4;
               hvgRegData &= ~PGA_GAIN_CTL_VDAC_B;
               hvgRegData |= (( pDev->vDac[chan] << PGA_GAIN_CTL_VDAC_B_SHIFT ) & PGA_GAIN_CTL_VDAC_B );
               pApm->reg_codec_config_4 = hvgRegData;

               hvgRegData = pApm->reg_chb_cic_config;
               hvgRegData &= ~CIC_INC_SHFT;
               hvgRegData |= (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
               pApm->reg_chb_cic_config = hvgRegData;

               pApm->reg_chb_test_config &= ~RX_98K_MUX_CTRL;
            }
         }
      }
      break;
      
      case( XDRV_APM_HVG_DESIGN_FLYBACK ):
      {
         if( ringingOn == XDRV_TRUE )
         {
            if( ringStop )
            {
               pApm->reg_ring_config_4 |= ( RING_STOP_NEXT_BREAK_A | RING_STOP_NEXT_BREAK_B );
               pApm->reg_ring_config_4 &= ~( RING_STOP_NEXT_BREAK_A | RING_STOP_NEXT_BREAK_B );
            }
            else
            {
               pApm->reg_ring_config_3 |= ( RING_START_CADENCE_A | RING_START_CADENCE_B );
               pApm->reg_ring_config_3 &= ~( RING_START_CADENCE_A | RING_START_CADENCE_B );
            }
         }
         else
         {
            pApm->reg_ring_config_4 |= ( RING_STOP_IMMED_A | RING_STOP_IMMED_B );
            pApm->reg_ring_config_4 &= ~( RING_STOP_IMMED_A | RING_STOP_IMMED_B );
         }

         /* Update APM register when entering or exiting STANDBY mode */
         if ( (prevSlicState == XDRV_SLIC_MODE_STANDBY) || (slicState == XDRV_SLIC_MODE_STANDBY) )
         {
            hvgRegData = ( chan == 0 )? pApm->reg_cha_cic_config : pApm->reg_chb_cic_config;
            hvgRegData &= ~CIC_INC_SHFT;
            if ( slicState == XDRV_SLIC_MODE_STANDBY )
            {
               hvgRegData |= ( ( 7 << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
            }
            else
            {
               hvgRegData |= (( flexicalcp->cic_inc_shft << CIC_INC_SHFT_SHIFT ) & CIC_INC_SHFT );
            }
            if ( chan == 0 )
            {
               pApm->reg_cha_cic_config = hvgRegData;
            } 
            else
            {
               pApm->reg_chb_cic_config = hvgRegData;
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
** FUNCTION:   hvg6818UpdateHookStatus
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
int hvg6818UpdateHookStatus( XDRV_APM *pApmDrv, int chan, int hookState )
{
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   XDRV_APM_HVG_VOLTAGE voltage;
   int index;

   /* Index of line in APM driver */
   index = chan % APM6818_NUM_CHANNELS;

   /* Record new hookstate */
   pDev->hookState[index] = hookState;

   /* Update HVG settings if enabled */
   if( pDev->hookStateUpdatesEnabled[index] )
   {
      /* Determine the battery voltage associated with the current state */
      voltage = hvg6818GetVoltage( pDev->slicState[index], pDev->hookState[index], pDev->battery[index] );

      /* Set the desired battery voltage */
      hvg6818SetVoltage( pApmDrv, chan, voltage );

      /* Set the desired battery voltage */
      hvg6818UpdateHvgParms( pApmDrv, chan );
   }

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   hvg6818UpdateHvgParms
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
static int hvg6818UpdateHvgParms( XDRV_APM *pApmDrv, int chan )
{
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   volatile HvgControlRegisters *hvgp = HVG;
   int index;
   int i;
   XDRV_UINT32 hvgRegData;
   XDRV_UINT32 hvgKpropKint=0;
   XDRV_UINT32 hvgWindowSize=0;
   XDRV_UINT32 hvgTrackMax;

   /* Index of line in APM driver */
   index = chan % APM6818_NUM_CHANNELS;

   /* Initialize new HVG track MAX value to default */
   hvgTrackMax = regCustomTable[hvgSubType][pDev->Cfg.hvgType].reg_hvg_track_max;

   /* Use non-default value if either line is in RT state */
   for( i = 0; i < 2; i++ )
   {
      if(( pDev->slicState[index] == XDRV_SLIC_MODE_RING ) &&
         ( pDev->hookState[index] ))
      {
         hvgTrackMax = 0x050D0000;
      }
   }

   /* Determine appropriate HVG analysis parameters */
   switch( pDev->slicState[index] )
   {
      case XDRV_SLIC_MODE_LCFO:
      case XDRV_SLIC_MODE_TIPOPEN:
      {
         if( hvgSubType == HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
         {
            hvgKpropKint = 0x00000084;
         }
         else
         {
            hvgKpropKint = 0x000000B5;
            hvgWindowSize = 0x00000550;
         }
      }

      case XDRV_SLIC_MODE_RING:
      {
         if( hvgSubType == HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
         {
             hvgKpropKint = 0x00000083;
         }
         else
         {
             if( pDev->hookState[index] )
             {
                hvgKpropKint = 0x00000062;
                hvgWindowSize = 0x00000110;
             }
             else
             {
                hvgKpropKint = 0x00000062;
                hvgWindowSize = 0x00000330;
             }
         }
      }
      break;

      case XDRV_SLIC_MODE_STANDBY:
      case XDRV_SLIC_MODE_OHT:
      case XDRV_SLIC_MODE_OHTR:
      case XDRV_SLIC_MODE_WINK:
      {
         if( pDev->hookState[index] )
         {
            hvgKpropKint = 0x00000084;
            if( hvgSubType != HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
            {
               hvgWindowSize = 0x00000330;
            }
         }
         else
         {
            hvgKpropKint = 0x00000084;
            if( hvgSubType != HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
            {
               hvgWindowSize = 0x00000330;
            }
         }
      }
      break;

      case XDRV_SLIC_MODE_LCF:
      case XDRV_SLIC_MODE_RLCF:
      {
         if( pDev->hookState[index] )
         {
            hvgKpropKint = 0x00000084;
            if( hvgSubType != HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
            {
               hvgKpropKint = 0x000000B5;
               hvgWindowSize = 0x00000770;

               /* Reduce window size by one for Si3239 SLICs */
               if( pDev->Cfg.hvgType == XDRV_APM_HVG_DESIGN_FLYBACK )
               {
                  hvgWindowSize = 0x00000660;
               }
            }
         }
         else
         {
            if( hvgSubType != HVG_DESIGN_GRRG2LBOSA) /* board 6818GRRG2LBOSA installed 9540 and new HVG circuit */
            {
              hvgKpropKint = 0x00000084;
              hvgWindowSize = 0x00000330;
            }
         }
      }
      break;

      default:
      {
         return( 1 );
      }
      break;
   }
    if( hvgSubType != HVG_DESIGN_GRRG2LBOSA)
    {
      hvgWindowSize += 0x00000110;
    }

   /* Update HVG analysis parameters */
   if( index == 0 )
   {
      if( hvgSubType != HVG_DESIGN_GRRG2LBOSA)
      {
         hvgRegData = hvgp->reg_hvg_cha_window_ctrl;
         hvgRegData &= ~( TIP_WINDOW_SIZE | RING_WINDOW_SIZE );
         hvgRegData |= hvgWindowSize;
         hvgRegData |= NEW_BLK_RQST;       /* assert bit */
         hvgp->reg_hvg_cha_window_ctrl = hvgRegData;
         hvgRegData &= ~( NEW_BLK_RQST );  /* clear bit */
         hvgp->reg_hvg_cha_window_ctrl = hvgRegData;
      }
      
      hvgRegData = hvgp->reg_hvg_cha_misc;
      hvgRegData &= ~( K_PROP | K_INTEG );
      hvgRegData |= hvgKpropKint;
      hvgp->reg_hvg_cha_misc = hvgRegData;
   }
   else
   {
      if( hvgSubType != HVG_DESIGN_GRRG2LBOSA)
      {
         hvgRegData = hvgp->reg_hvg_chb_window_ctrl;
         hvgRegData &= ~( TIP_WINDOW_SIZE | RING_WINDOW_SIZE );
         hvgRegData |= hvgWindowSize;
         hvgRegData |= NEW_BLK_RQST;       /* assert bit */
         hvgp->reg_hvg_chb_window_ctrl = hvgRegData;
         hvgRegData &= ~( NEW_BLK_RQST );  /* clear bit */
         hvgp->reg_hvg_chb_window_ctrl = hvgRegData;
      }

      hvgRegData = hvgp->reg_hvg_chb_misc;
      hvgRegData &= ~( K_PROP | K_INTEG );
      hvgRegData |= hvgKpropKint;
      hvgp->reg_hvg_chb_misc = hvgRegData;
   }

   hvgRegData = hvgp->reg_hvg_track_max;
   hvgRegData &= ~( OFF_HOOK_MAX );
   hvgRegData |= hvgTrackMax;
   hvgp->reg_hvg_track_max = hvgRegData;

   return( 0 );
}

/*****************************************************************************
*
*  FUNCTION:   hvg6818IsEnabled
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
int hvg6818IsEnabled( int chan )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;
   volatile XDRV_UINT32 *pHvgCtrl;

   if ( chan == 0 )
   {
      pHvgCtrl = &hvgp->reg_hvg_cha_misc;
   }
   else
   {
      pHvgCtrl = &hvgp->reg_hvg_chb_misc;
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
** FUNCTION:   hvg6818GetOverCurrentStatus
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
XDRV_BOOL hvg6818GetOverCurrentStatus( XDRV_APM *pDrv, int chan )
{
   volatile BCM6818_HVG *hvgp = BCM6818_HVG_PTR;

   if(( chan == 0 ) && ( hvgp->reg_hvg_status7 & HVG_SHORT_FLAG_A ))
   {
      /* Record status info */
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG overcurrent condtion on line %d !!!", chan ));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG hvg voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status8 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG tip voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status10 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG ring voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status12 )));

      /* Shutdown HVG */
      hvgp->reg_hvg_cha_misc |= HVG_SOFT_INIT_0;

      return XDRV_TRUE;
   }
   else if(( chan == 1 ) && ( hvgp->reg_hvg_status7 & HVG_SHORT_FLAG_B ))
   {
      /* Record status info */
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG overcurrent condtion on line %d !!!", chan ));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG hvg voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status9 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG tip voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status11 )));
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "HVG ring voltage %d !!!", (int)hvgExtend21to32( hvgp->reg_hvg_status13 )));

      /* Shutdown HVG */
      hvgp->reg_hvg_chb_misc |= HVG_SOFT_INIT_0;

      return XDRV_TRUE;
   }
   else
   {
   return XDRV_FALSE;
}
}

/*
*****************************************************************************
** FUNCTION:   hvg6818GetOverVoltageStatus
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
void hvg6818GetOverVoltageStatus( XDRV_APM *pDrv, int chan )
{
   /* Nothing to do here */
   return;
}

/*
*****************************************************************************
** FUNCTION:   hvg6818GetVoltage
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
static XDRV_APM_HVG_VOLTAGE hvg6818GetVoltage( XDRV_UINT16 mode, XDRV_UINT16 hookState, int battery )
{
   const MODEVOLTAGEMAP *pVoltageMap = modeVoltageMap;
   XDRV_APM_HVG_VOLTAGE voltage;

   while ( mode != XDRV_SLIC_MODE_NULL )
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
** FUNCTION:   hvg6818UpdateHookStatusEnable
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
int hvg6818UpdateHookStatusEnable( XDRV_APM *pApmDrv, int chan, XDRV_BOOL update )
{
   APM_6818_DRV *pDev = (APM_6818_DRV *)pApmDrv;
   int index;

   /* Index of line in APM driver */
   index = chan % APM6818_NUM_CHANNELS;

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
