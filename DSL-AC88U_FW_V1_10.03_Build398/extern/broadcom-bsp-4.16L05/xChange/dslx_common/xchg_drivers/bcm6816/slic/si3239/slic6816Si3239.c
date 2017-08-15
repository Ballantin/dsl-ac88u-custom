/***************************************************************************
*    Copyright (c) 2008-2012 Broadcom Corporation                                        
*                                                                           
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*    Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: slic6816Si3239.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      SI3239 Subscriber Line Interface Circuits (SLIC).
*
****************************************************************************/

#include <xdrvTypes.h>
#include <kernelMainWrapper.h>
#include "mspiDef.h"
#include <xdrvLog.h>
#include <xdrvApm.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include "slic6816Si3239.h"
#include "slic6816Si3239Defs.h"

#include "bcmSpiRes.h"


/* SI3239 SLIC states */
#define SI3239_STATE_DISCONNECT              0x00     /* Disconnect */
#define SI3239_STATE_STANDBY                 0x01     /* Standby */
#define SI3239_STATE_FWD_ACTIVE              0x02     /* Forward Active */
#define SI3239_STATE_REV_ACTIVE              0x03     /* Reverse Active */
#define SI3239_STATE_RING_OPEN               0x04     /* Ring open */
#define SI3239_STATE_TIP_OPEN                0x05     /* Tip open */
#define SI3239_STATE_AC_RING_TRIP            0x06     /* Ringing with AC ring trip */
#define SI3239_STATE_DC_RING_TRIP            0x07     /* Ringing with DC ring trip */
#define SI3239_STATE_NULL                    0x08     /* Invalid mode */

/* SLIC initial/shutdown states */
#define SI3239_STATE_INIT                    SI3239_STATE_FWD_ACTIVE
#define SI3239_STATE_DEINIT                  SI3239_STATE_FWD_ACTIVE

/* SLIC "meta-driver" interface functions */
static void slic6816Si3239ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slic6816Si3239LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static void slic6816Si3239PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slic6816Si3239IsOffhook( XDRV_SLIC *pDrv );
static int slic6816Si3239SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                       int ringVoltage, int ringOffset, int ringOffsetCal );
static int slic6816Si3239GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                       int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static int slic6816Si3239SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static int slic6816Si3239SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slic6816Si3239GetOverCurrentStatus( XDRV_SLIC *pDrv );
static XDRV_SINT16 slic6816Si3239GetDlp( XDRV_SLIC *pDrv );
static void slic6816Si3239ProcessEvents( XDRV_SLIC *pDrv );
static XDRV_UINT32 slic6816Si3239Probe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                                      XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                                      XDRV_UINT8 set );
static int slic6816Si3239GetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );
/*
** Mapping from SLIC modes to voltage levels
*/
typedef struct MODEVOLTAGEMAP
{
   XDRV_UINT16         mode;
   XDRV_APM_HVG_VOLTAGE  voltage;

} MODEVOLTAGEMAP;

/* Slic Mode - battery voltage level mappings */
static const MODEVOLTAGEMAP modeVoltageMap[] =
{
   { SI3239_STATE_DISCONNECT,    XDRV_APM_HVG_VOLTAGE_ONHOOK  },
   { SI3239_STATE_STANDBY,       XDRV_APM_HVG_VOLTAGE_ONHOOK  },
   { SI3239_STATE_FWD_ACTIVE,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { SI3239_STATE_REV_ACTIVE,    XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { SI3239_STATE_RING_OPEN,     XDRV_APM_HVG_VOLTAGE_ONHOOK  },
   { SI3239_STATE_TIP_OPEN,      XDRV_APM_HVG_VOLTAGE_ONHOOK },
   { SI3239_STATE_AC_RING_TRIP,  XDRV_APM_HVG_VOLTAGE_RING    },
   { SI3239_STATE_DC_RING_TRIP,  XDRV_APM_HVG_VOLTAGE_RING    },
   { SI3239_STATE_NULL,          XDRV_APM_HVG_VOLTAGE_NONE    }
};

/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS slic6816Si3239DrvFuncs =
{
   slic6816Si3239ModeControl,
   slic6816Si3239LedControl,
   slic6816Si3239PhaseReversalControl,
   slic6816Si3239IsOffhook,
   slic6816Si3239SetRingParms,
   slic6816Si3239GetRingParms,
   slic6816Si3239SetBoostedLoopCurrent,
   slic6816Si3239SetPowerSource,
   slic6816Si3239GetOverCurrentStatus,
   slic6816Si3239GetDlp,
   slic6816Si3239ProcessEvents,
   slic6816Si3239Probe,
   slic6816Si3239GetSlicParms
};

/* Private Functions */
static void OpenSlic( SLIC_6816_SI3239_DRV *pDev );
static void CloseSlic( SLIC_6816_SI3239_DRV *pDev );
static void InitSlicRegs( SLIC_6816_SI3239_DRV *pDev );
static void setSlicState( int chan, XDRV_SLIC_MODE mode );
static XDRV_UINT16 MSPI_Read( int chan, XDRV_UINT8 addr );
static void MSPI_Write( int chan, XDRV_UINT8 addr, XDRV_UINT16 data );
static int MSPI_Write_Verify( int chan, XDRV_UINT8 addr, XDRV_UINT16 data );
static int MSPI_Mask_Write( int chan, XDRV_UINT8 addr, XDRV_UINT16 data, XDRV_UINT16 mask, int verify );
#if defined(CONFIG_BCM96816)  
static int spi_bus_num_special = 0;
#endif
/*
*****************************************************************************
** FUNCTION:   slic6816Si3239Init
**
** PURPOSE:    Initialize and open the Si3239 SLIC
**
** PARAMETERS: chan          - the line number ( 0 referenced )
**             pDev          - pointer to the Si3239 SLIC info structure
**             pSlicCfgInfo  - SLIC configuration structure
**             pApmDrv       - pointer to APM driver associate with this slic
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6816Si3239Init
(
   int                   chan,
   SLIC_6816_SI3239_DRV *pDev,
   const SLICCFG_6816_SI3239_INFO *pSlicCfgInfo,
   XDRV_APM             *pApmDrv
)
{
   int status;

   if ( pDev->bDrvEnabled )
   {
      XDRV_LOG_WARN(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC: WARNING - chan %d driver already enabled !!!", chan));
   }

   /* Reset the slic driver */
   memset( pDev, 0, sizeof( SLIC_6816_SI3239_DRV ) );

   /* Initialize the SLIC information for each channel */
   pDev->chan = chan;
   pDev->pDrvFuncs = &slic6816Si3239DrvFuncs;

   /* Initialize the APM driver structure */
   pDev->pApmDrv = pApmDrv;

   /* Driver enabled */
   pDev->bDrvEnabled = XDRV_TRUE;
   pDev->bRingDCoffsetEnabled = XDRV_FALSE;

   /* Enhanced control features */
   pDev->bEnhancedControl = pSlicCfgInfo->enhancedControl;

#if defined(CONFIG_BCM96816) 
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif
    status = BcmSpiReserveSlave(MSPI_BUS_ID, 1, MSPI_CLK_FREQUENCY);
 
    if ( status != SPI_STATUS_OK )
    {
       XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Error during BcmSpiReserveSlave (status %d) ", status ));
       // return ( XDRV_SLIC_STATUS_ERROR );
       return;
    }

   /* Open the SLIC */
   OpenSlic( pDev );
}


/*
*****************************************************************************
** FUNCTION:   slic6816Si3239Deinit
**
** PURPOSE:    Shutdown the Si3239 SLIC
**
** PARAMETERS: pDev  - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6816Si3239Deinit( SLIC_6816_SI3239_DRV *pDev )
{
   if ( pDev->bDrvEnabled == XDRV_FALSE )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC: ERROR - driver already disabled !!!"));
   }

   /* Close the SLIC */
   CloseSlic( pDev );

   memset( pDev, 0, sizeof( SLIC_6816_SI3239_DRV ) );
}


/*
*****************************************************************************
** FUNCTION:   slic6816Si3239IsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the SLIC driver device structure
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL slic6816Si3239IsOffhook( XDRV_SLIC *pDrv )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   int                   chan = pDev->chan;
   XDRV_UINT16           rc;

   if( pDev->slicMode == XDRV_SLIC_MODE_LCFO )
   {
      return( XDRV_FALSE );
   }

   rc = MSPI_Read( chan, SI3239_REG_DEVICE_STATUS );

   if ( rc & (( chan == 0 ) ? SI3239_CHAN0_HOOKSWITCH_STAT_OFF:SI3239_CHAN1_HOOKSWITCH_STAT_OFF ) )
   {
      return ( XDRV_TRUE );
   }
   else
   {
      return ( XDRV_FALSE );
   }
}


/*
*****************************************************************************
** FUNCTION:   slic6816Si3239ModeControl
**
** PURPOSE:    Sets the SLIC into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             mode  - the mode to set the SLIC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slic6816Si3239ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   int chan = pDev->chan;
   int slicState;

   switch( mode )
   {
      case XDRV_SLIC_MODE_LCFO:
      {
         slicState = SI3239_STATE_DISCONNECT;
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = SI3239_STATE_REV_ACTIVE;
         }
         else
         {
            slicState = SI3239_STATE_STANDBY;
         }
      }
      break;

      /* On-hook transmission */
      /* Loop current feed */
      case XDRV_SLIC_MODE_OHT:
      case XDRV_SLIC_MODE_LCF:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = SI3239_STATE_REV_ACTIVE;
         }
         else
         {
            slicState = SI3239_STATE_FWD_ACTIVE;
         }
      }
      break;

      /* On-hook transmission reverse */
      /* Reverse loop current feed */
      case XDRV_SLIC_MODE_OHTR:
      case XDRV_SLIC_MODE_RLCF:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = SI3239_STATE_FWD_ACTIVE;
         }
         else
         {
            slicState = SI3239_STATE_REV_ACTIVE;
         }
      }
      break;

      /* Tip Open */
      case XDRV_SLIC_MODE_TIPOPEN:
      {
         slicState = SI3239_STATE_TIP_OPEN;
      }
      break;

      /* Ring Open */
      case XDRV_SLIC_MODE_RINGOPEN:
      {
         slicState = SI3239_STATE_RING_OPEN;
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
         slicState = SI3239_STATE_AC_RING_TRIP;
      }
      break;

      default:
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, " XDRV_SLIC: ERROR - unrecognized SLIC mode 0x%02x", mode));
         return;
      }
      break;
   }

   if(( pDev->slicMode == XDRV_SLIC_MODE_RING ) &&
      ( mode == XDRV_SLIC_MODE_RING ) &&
      ( pDev->pendingSlicTime > 0 ))
   {
      /* Ringing stopping */
      pDev->pendingSlicState = slicState;
      pDev->pendingSlicMode = mode;
   }
   else if(( pDev->slicMode == XDRV_SLIC_MODE_RING ) &&
           ( pDev->disableRingMode == XDRV_FALSE ) &&
           (( mode == XDRV_SLIC_MODE_STANDBY ) ||
            ( mode == XDRV_SLIC_MODE_OHT ) ||
            ( mode == XDRV_SLIC_MODE_OHTR )))
   {
      if( pDev->pendingSlicTime > 0 )
      {
         /* Ringing already stopping */
         pDev->pendingSlicState = slicState;
         pDev->pendingSlicMode = mode;
      }
      else
      {
         /* Initialize delayed state change */
         pDev->pendingSlicState = slicState;
         pDev->pendingSlicMode = mode;
         pDev->pendingSlicTime = 100;

         /* Update the HVG parameters based on the current SLIC state */
         xdrvApmHvgUpdateSlicStatus( pDev->pApmDrv, chan, XDRV_SLIC_MODE_RING,
                                     pDev->bRingDCoffsetEnabled, pDev->bOnBattery,
                                     pDev->bBoostedLoopCurrent, XDRV_TRUE );
      }
   }
   else
   {
      /* Clear pending state changes */
      pDev->pendingSlicState = 0;
      pDev->pendingSlicMode = 0;
      pDev->pendingSlicTime = 0;

      /* Start the appropriate mode change */
      setSlicState( chan, slicState );

      /* Update the HVG parameters based on the current SLIC state */
      xdrvApmHvgUpdateSlicStatus( pDev->pApmDrv, chan, mode,
                                  pDev->bRingDCoffsetEnabled, pDev->bOnBattery,
                                  pDev->bBoostedLoopCurrent, XDRV_FALSE );

      /* Save the current requested SLIC mode */
      pDev->slicMode = mode;
   }
}


/*
*****************************************************************************
** FUNCTION:   slic6816Si3239LedControl
**
** PURPOSE:    This function controls nothing - legacy compiling support
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    None
**
*****************************************************************************
*/
static void slic6816Si3239LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* SLICs do not control LEDs on the 6816 */
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239PhaseReversalControl
**
** PURPOSE:    This function controls the phase of the SLICs
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slic6816Si3239PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;

   /* Set the phase reversal control flag */
   pDev->phaseReversalActive = value;

   if(( pDev->slicMode == XDRV_SLIC_MODE_STANDBY ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHT ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHTR ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_LCF ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_RLCF ))
   {
      slic6816Si3239ModeControl( pDrv, pDev->slicMode );
   }
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239SetRingParms
**
** PURPOSE:    This function controls the ring waveform
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int slic6816Si3239SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   XDRV_UINT16 slicControlReg;

   if ( ringOffset )
   {
      pDev->bRingDCoffsetEnabled = XDRV_TRUE;
   }
   else
   {
      pDev->bRingDCoffsetEnabled = XDRV_FALSE;
   }

   /* Update RSPOL setting */
   slicControlReg = MSPI_Read( pDev->chan, SI3239_REG_LINEFEED_CONFIG_2 );
   slicControlReg &= ~SI3239_RSPOL_MASK;
   if ( ringOffset < 0 )
   {
      slicControlReg |= SI3239_RSPOL_REVERSED;
   }
   MSPI_Write( pDev->chan, SI3239_REG_LINEFEED_CONFIG_2, slicControlReg );

   /* Save ring settings */
   pDev->ringVoltage = ringVoltage;
   pDev->ringOffset = ringOffset;
   pDev->ringOffsetCal = ringOffsetCal;
   pDev->ringFrequency = ringFrequency;
   pDev->ringWaveshape = ringWaveshape;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239GetRingParms
**
** PURPOSE:    This function gets the current ring waveform parameters
**
** PARAMETERS: pDrv           - pointer to the SLIC driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int slic6816Si3239GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                       int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;

   /* Retrieve ring settings */
   *ringVoltage = pDev->ringVoltage;
   *ringOffset = pDev->ringOffset;
   *ringOffsetCal = pDev->ringOffsetCal;
   *ringFrequency = pDev->ringFrequency;
   *ringWaveshape = pDev->ringWaveshape;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239GetSlicParms
**
** PURPOSE:    This function gets the current slic configuration parameters
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             phaseReversal  - current phase reversal setting
**             loopCurrent    - current loop current setting
**             powerSource    - current power source config setting
**             slicMode       - current slic mode
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int slic6816Si3239GetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;

   /* Retrieve ring settings */
   *phaseReversal = pDev->phaseReversalActive;
   *loopCurrent = pDev->bBoostedLoopCurrent;
   *powerSource = pDev->bOnBattery;
   *slicMode = pDev->slicMode;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239SetBoostedLoopCurrent
**
** PURPOSE:    This function controls boosted loop current
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int slic6816Si3239SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   int chan = pDev->chan;

   /* Record boosted loop current state */
   pDev->bBoostedLoopCurrent = value;

   /* 
   ** Program the SLIC for appropriate current limit: 
   ** 0 - 25mA, 1 - 40mA 
   */
   if ( value )
   {
      MSPI_Mask_Write( chan, SI3239_REG_LINEFEED_CONFIG_2,
                       ( chan == 0 ) ? SI3239_CHAN0_LOOP_CUR_THRESH_40:SI3239_CHAN1_LOOP_CUR_THRESH_40,
                       ( chan == 0 ) ? SI3239_CHAN0_LOOP_CUR_THRESH_MASK:SI3239_CHAN1_LOOP_CUR_THRESH_MASK, 0 );
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Loop current set to 40 mA", pDev->chan ));
   }
   else
   {
      MSPI_Mask_Write( chan, SI3239_REG_LINEFEED_CONFIG_2,
                       ( chan == 0 ) ? SI3239_CHAN0_LOOP_CUR_THRESH_25:SI3239_CHAN1_LOOP_CUR_THRESH_25,
                       ( chan == 0 ) ? SI3239_CHAN0_LOOP_CUR_THRESH_MASK:SI3239_CHAN1_LOOP_CUR_THRESH_MASK, 0 );
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Loop current set to 25 mA", pDev->chan ));
   }

   /* Update the SLIC and HVG states */
   slic6816Si3239ModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Boosted loop current mode change (%d).", pDev->chan, value));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   OpenSlic
**
** PURPOSE:    Open the SLIC
**
** PARAMETERS: pDev - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void OpenSlic( SLIC_6816_SI3239_DRV *pDev )
{
   int chan = pDev->chan;

   /* Initialize the SLIC registers for this channel */
   InitSlicRegs( pDev );

   /* Set initial SLIC state */
   MSPI_Mask_Write( chan, SI3239_REG_LINEFEED_CONFIG_1,
                    SI3239_STATE_INIT << ( ( chan == 0 ) ? SI3239_CHAN0_MODE_SHIFT : SI3239_CHAN1_MODE_SHIFT ),
                    ( ( chan == 0 ) ? SI3239_CHAN0_MODE_MASK : SI3239_CHAN1_MODE_MASK ), 0 );

   /* Save the current requested SLIC mode */
   pDev->slicMode = XDRV_SLIC_MODE_LCF;
}


/*
*****************************************************************************
** FUNCTION:   CloseSlic
**
** PURPOSE:    Close the SLIC
**
** PARAMETERS: pDev  - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CloseSlic( SLIC_6816_SI3239_DRV *pDev )
{
   int chan = pDev->chan;

   /* Set the SLIC to the appropriate disabled state */
   MSPI_Mask_Write( chan, SI3239_REG_LINEFEED_CONFIG_1,
                    SI3239_STATE_DEINIT << ( ( chan == 0 ) ? SI3239_CHAN0_MODE_SHIFT : SI3239_CHAN1_MODE_SHIFT ),
                    ( ( chan == 0 ) ? SI3239_CHAN0_MODE_MASK : SI3239_CHAN1_MODE_MASK ), 0 );
}


/*
*****************************************************************************
** FUNCTION:   InitSlicRegs
**
** PURPOSE:    Initialize the SLIC registers and RAM
**
** PARAMETERS: pDev - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void InitSlicRegs( SLIC_6816_SI3239_DRV *pDev )
{
   int chan = pDev->chan;

   /* Ring Trip Threshold - 70mA */
   MSPI_Write( chan, SI3239_REG_DEVICE_STATUS, 0x0000 );

   /*
   ** At this point we do not need to initialize register 1, as when line 2
   ** gets initialized, we will crash line 1.  Both lines share
   ** the same register.
   */
   if(( pDev->chan % 2 ) == 0 )
   {
      MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_1, 0x0000 );
   }
   MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_2, SI3239_RINGTRIP_DETECT_THRESH_70 );
   MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_3, SI3239_TEST_NORMAL_OPERATION );
}

/*
*****************************************************************************
** FUNCTION:   setSlicState
**
** PURPOSE:    Set SLIC state and related registers
**
** PARAMETERS: pDev - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void setSlicState( int chan, XDRV_SLIC_MODE mode )
{
   XDRV_UINT16 slicStateReg;
   XDRV_UINT16 slicControlReg;
   XDRV_UINT16 currentTemp;
   XDRV_UINT16 requiredTemp;
   XDRV_UINT16 mask;
   XDRV_UINT16 shift;

   /* Retrieve old data */
   slicStateReg = MSPI_Read( chan, SI3239_REG_LINEFEED_CONFIG_1 );
   slicControlReg = MSPI_Read( chan, SI3239_REG_LINEFEED_CONFIG_3 );

   /* Update the register value */
   mask = ( ( chan % 2 ) == 0 ) ? SI3239_CHAN0_MODE_MASK : SI3239_CHAN1_MODE_MASK;
   shift = ( ( chan % 2 ) == 0 ) ? SI3239_CHAN0_MODE_SHIFT : SI3239_CHAN1_MODE_SHIFT;
   slicStateReg &= ( ~mask );
   slicStateReg |= ( ( mode << shift ) & mask );

   if( slicControlReg & SI3239_TEST_TSDT )
   {
      currentTemp = 1;
   }
   else
   {
      currentTemp = 0;
   }

   if((( slicStateReg & SI3239_CHAN0_MODE_MASK ) == SI3239_CHAN0_MODE_AC_RING_TRIP ) ||
      (( slicStateReg & SI3239_CHAN0_MODE_MASK ) == SI3239_CHAN0_MODE_DC_RING_TRIP ) ||
      (( slicStateReg & SI3239_CHAN1_MODE_MASK ) == SI3239_CHAN1_MODE_AC_RING_TRIP ) ||
      (( slicStateReg & SI3239_CHAN1_MODE_MASK ) == SI3239_CHAN1_MODE_DC_RING_TRIP ))
   {
      requiredTemp = 1;
      slicControlReg |= SI3239_TEST_TSDT;
   }
   else
   {
      requiredTemp = 0;
      slicControlReg &= ~SI3239_TEST_TSDT;
   }

   /* Write the modified state data to the SLIC */
   MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_1, slicStateReg );

   /* If necessary update the temperature control data to the SLIC */
   if( requiredTemp != currentTemp )
   {
      MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_3, slicControlReg );
   }
}

/*
******************************************************************************
** FUNCTION:  MSPI_Read
**
** PURPOSE:   Read from a memory address via MSPI
**
** PARAMETERS:
**            chan  - the line number ( 0 referenced )
**            addr  - address of the control register
**
** RETURNS:
**            data  - 8-bit data for the control register
**
*****************************************************************************
*/
static XDRV_UINT16 MSPI_Read( int chan, XDRV_UINT8 addr )
{
   XDRV_UINT16    rxData;
   XDRV_UINT8     txBuffer[4] = { 0, 0, 0, 0 };
   XDRV_UINT8     rxBuffer[4] = { 0, 0, 0, 0 };

   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = ( addr << 5 ) | SI3239_READ_CMD;
   txBuffer[1] = 0x00;
   txBuffer[2] = 0x00;
   txBuffer[3] = 0x00;

   /* Write to the MSPI */
#if defined(CONFIG_BCM96816) 
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif
   if ( BcmSpiSyncTrans(txBuffer, rxBuffer, 0, 4, MSPI_BUS_ID, 1) )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "MSPI_Read Error"));
   }

   /* Process the returned values */
   rxData = (((( XDRV_UINT16 )rxBuffer[2] << 8 ) | (( XDRV_UINT16 )rxBuffer[3] )) & 0x0FFF );
   return( rxData );
}


/*
*****************************************************************************
** FUNCTION:  MSPI_Write
**
** PURPOSE:   Write to a memory address via MSPI
**
** PARAMETERS:
**            chan  - the line number ( 0 referenced )
**            addr  - memory address to be written to
**            data  - 16-bit data to be written
**
** RETURNS:   nothing
**
*****************************************************************************
*/
static void MSPI_Write( int chan, XDRV_UINT8 addr, XDRV_UINT16 data )
{
   XDRV_UINT8     txBuffer[2];
   XDRV_UINT8     dummy[2];

   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = ( (( XDRV_UINT8 )( data >> 8 )) & 0x0F ) | ( addr << 5 ) | SI3239_WRITE_CMD;
   txBuffer[1] = ( XDRV_UINT8 )data;

   /* Write to the MSPI */
#if defined(CONFIG_BCM96816) 
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif   
   if ( BcmSpiSyncTrans(txBuffer, dummy, 0, 2, MSPI_BUS_ID, 1) )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "MSPI_Write Error"));
   }
}


/*
*****************************************************************************
** FUNCTION:  MSPI_Write_Verify
**
** PURPOSE:   Write to a memory address via MSPI and verify the write
**
** PARAMETERS:
**            chan - the line number ( 0 referenced )
**            addr - memory address to be written to
**            data - 8-bit data to be written
**
** RETURNS:   0 on success, 1 otherwise
**
*****************************************************************************
*/
static int MSPI_Write_Verify( int chan, XDRV_UINT8 addr, XDRV_UINT16 data )
{
   XDRV_UINT8     txBuffer[2];
   XDRV_UINT8     dummy[2];
   XDRV_UINT16    readData;


   /* Place the command/address and data into the lower byte of each txBuffer */
   txBuffer[0] = (((XDRV_UINT8)(data >> 8)) & 0x1f) | ((addr << 5) | SI3239_WRITE_CMD);
   txBuffer[1] = (XDRV_UINT8)data;

   /* Write to the MSPI */
#if defined(CONFIG_BCM96816) 
   /* Set bus number if on BCM6816 */
   if( (MISC->miscStrapBus & MISC_STRAP_BUS_LS_SPIM_ENABLED) == MISC_STRAP_BUS_LS_SPIM_ENABLED )
   {
      /* LS mode */
      spi_bus_num_special = LEG_SPI_BUS_NUM;
   }
   else
   {
      /* HS mode */
      spi_bus_num_special = HS_SPI_BUS_NUM;
   }
#endif   
   if ( BcmSpiSyncTrans(txBuffer, dummy, 0, 2, MSPI_BUS_ID, 1) )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "ERROR: MSPI_Write_Verify - Addr %d - Data 0x%02X ", addr, data));
   }

   /* Read the register that was just written to */
   readData = MSPI_Read( chan, addr );

   /* Verify that the write operation was successful */
   if( readData != data )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "ERROR: MSPI_Write_Verify - Addr %d - Data 0x%04X ", addr, data));
      return( 1 );
   }
   else
   {
      return( 0 );
   }
}

/*
*****************************************************************************
** FUNCTION:  MSPI_Mask_Write
**
** PURPOSE:   Write to the masked location of a memory address via MSPI
**            and verify the write if requested to
**
** PARAMETERS:
**            chan - the line number ( 0 referenced )
**            addr - memory address to be written to
**            data - 8-bit data to be written
**            mask - mask to the data to be written
**            verify - bool flag to verify the write
**
** RETURNS:   0 on success, 1 otherwise
**
*****************************************************************************
*/
static int MSPI_Mask_Write( int chan, XDRV_UINT8 addr, XDRV_UINT16 data, XDRV_UINT16 mask, int verify )
{
   XDRV_UINT16 localData;

   /* Retrieve Old Data */
   localData = MSPI_Read( chan, addr );
   /* Clear the masked location */
   localData &= ( ~mask );

   /* Add the new data */
   localData |= ( data & mask );

   /* Write the modified data to the SLIC */
   if ( verify )
   {
      return MSPI_Write_Verify( chan, addr, localData );
   }
   else
   {
      MSPI_Write( chan, addr, localData );
   }

   return 0;
}

/*
******************************************************************************
** FUNCTION:  slic6816Si3239RegisterRead
**
** PURPOSE:   Read from a memory address via MSPI
**
** PARAMETERS:
**            chan     - the line number ( 0 referenced )
**            addr     - address of the control register
**            data     - data read from the control register
**
** RETURNS:   none
**
*****************************************************************************
*/
void slic6816Si3239RegisterRead( int chan, XDRV_UINT8 addr, XDRV_UINT16 *data )
{
   *data = MSPI_Read( chan, addr );
}

/*
******************************************************************************
** FUNCTION:  slic6816Si3239RegisterWrite
**
** PURPOSE:   Write to a memory address via MSPI
**
** PARAMETERS:
**            chan     - the line number ( 0 referenced )
**            addr     - address of the control register
**            data     - 8 or 16 bit data for the control register
**
** RETURNS:   none
**
*****************************************************************************
*/
void slic6816Si3239RegisterWrite( int chan, XDRV_UINT8 addr, XDRV_UINT16 data )
{
   MSPI_Write( chan, addr, data );
}

/*
******************************************************************************
** FUNCTION:  slic6816Si3239RegisterMaskWrite
**
** PURPOSE:   Write to the masked location of a memory address via MSPI
**            and verify the write if requested to
**
** PARAMETERS:
**            chan - the line number ( 0 referenced )
**            addr - memory address to be written to
**            data - 8-bit data to be written
**            mask - mask to the data to be written
**            verify - bool flag to verify the write
**
** RETURNS:   0 on success, 1 otherwise
**
*****************************************************************************
*/
void slic6816Si3239RegisterMaskWrite( int chan, XDRV_UINT8 addr, XDRV_UINT16 data, XDRV_UINT16 mask, int verify )
{
   MSPI_Mask_Write( chan, addr, data, mask, verify );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239SetPowerSource
**
** PURPOSE:    This function notifies the SLIC driver of any changes in power
**             conditions (on battery or AC power)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (battery power on)  = device operating on battery power
**                     0 (battery power off) = device operating on AC power
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
static int slic6816Si3239SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   int                   chan = pDev->chan;
   XDRV_UINT16           configRegister;

   /* Record AC power status */
   if( pDev->bEnhancedControl )
   {
      pDev->bOnBattery = value;
   }
   else if( value )
   {
      pDev->bOnBattery = XDRV_TRUE;
   }
   else
   {
      pDev->bOnBattery = XDRV_FALSE;
   }

   /* Enable enhance control */
   if ( pDev->bEnhancedControl )
   {
      if ( value & 0xF0 )
      {
         configRegister = MSPI_Read( chan, SI3239_REG_LINEFEED_CONFIG_2 );
         configRegister |= ( SI3239_CHAN0_ENHANCED_CONTROL_ON | SI3239_CHAN1_ENHANCED_CONTROL_ON );
         MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_2, configRegister );
      }
      else if ( value == 0 )
      {
         configRegister = MSPI_Read( chan, SI3239_REG_LINEFEED_CONFIG_2 );
         configRegister &= ~( SI3239_CHAN0_ENHANCED_CONTROL_ON | SI3239_CHAN1_ENHANCED_CONTROL_ON );
         MSPI_Write( chan, SI3239_REG_LINEFEED_CONFIG_2, configRegister );
      }
   }

   /* Reset SLIC mode to current state to force configuration updates */
   slic6816Si3239ModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Power Source mode change (%d).", pDev->chan, pDev->bOnBattery ));

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239Probe
**
** PURPOSE:    To probe registers. (DEBUG ONLY)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static XDRV_UINT32 slic6816Si3239Probe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                              XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                              XDRV_UINT8 set )
{
   (void) pDrv;

   return (0);
}


/*
*****************************************************************************
** FUNCTION:   slic6816Si3239SetSpecialMode
**
** PURPOSE:    This function notifies the SLIC to enable or disable
**             various special modes
**
** PARAMETERS: pDrv   - pointer to the SLIC driver device structure
**             mode   - special mode to enable or disable
**             enable - enable or disable
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
int slic6816Si3239SetSpecialMode( SLIC_6816_SI3239_DRV *pDev, SLIC_6816_SI3239_SPECIAL_MODE mode, XDRV_BOOL enable )
{
   switch( mode )
   {
      case SLIC_6816_SI3239_SPECIAL_DISABLE_RING_TRANISTION_MODE:
      {
         /* Set flag appropriately */
         pDev->disableRingMode = enable;
      }
      break;

      default:
      {
      }
      break;
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239GetOverCurrentStatus
**
** PURPOSE:    Check to see if the over current protection has kicked in
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    XDRV_TRUE  - If the over current protection has kicked in
**             XDRV_FALSE - Otherwise
**
*****************************************************************************
*/
static XDRV_BOOL slic6816Si3239GetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   SLIC_6816_SI3239_DRV *pDev = (SLIC_6816_SI3239_DRV *)pDrv;
   XDRV_APM *pApmDrv = pDev->pApmDrv;
   int chan = pDev->chan;

   return xdrvApmGetOverCurrentStatus( pApmDrv, chan );
}

static XDRV_SINT16 slic6816Si3239GetDlp( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return( 0 );
}

static void slic6816Si3239ProcessEvents( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return;
}

/*
*****************************************************************************
** FUNCTION:   slic6816Si3239UpdateState
**
** PURPOSE:    Update Si3239 SLIC state
**
** PARAMETERS: pDev - pointer to the Si3239 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6816Si3239UpdateState( SLIC_6816_SI3239_DRV *pDev )
{
   if( pDev->pendingSlicTime > 0 )
   {
      if( pDev->pendingSlicTime > 10 )
      {
         /* Decrement counter by task rate */
         pDev->pendingSlicTime -= 10;
         return;
      }
      else
      {
         /* Timer expiring */
         pDev->pendingSlicTime = 0;

         /* Update SLIC state */
         setSlicState( pDev->chan, pDev->pendingSlicState );

         /* Update the HVG parameters based on the current SLIC state */
         xdrvApmHvgUpdateSlicStatus( pDev->pApmDrv, pDev->chan, pDev->pendingSlicMode,
                                     pDev->bRingDCoffsetEnabled, pDev->bOnBattery,
                                     pDev->bBoostedLoopCurrent, XDRV_FALSE );

         /* Save the current requested SLIC mode */
         pDev->slicMode = pDev->pendingSlicMode;

         /* Clear pending state information */
         pDev->pendingSlicState = 0;
         pDev->pendingSlicMode = 0;
      }
   }
}


