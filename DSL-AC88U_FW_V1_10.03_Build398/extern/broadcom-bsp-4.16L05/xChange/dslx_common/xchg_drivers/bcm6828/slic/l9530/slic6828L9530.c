/****************************************************************************
*
* Copyright (c) 2008-2012 Broadcom Corporation
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
*    Filename: slic6828L9530.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      L9530 Subscriber Line Interface Circuits (SLIC).
*
****************************************************************************/

#include <xdrvLog.h>
#include <xdrvApm.h>
#include <string.h>
#include "slic6828L9530.h"

/* L9530 SLIC states in the following format: B2|B1|B0 */
#define L9530_STATE_FWD_ACTIVE_LC   0x00     /* Forward Active - Low Loop Current */
#define L9530_STATE_FWD_ACTIVE_HC   0x04     /* Forward Active - High Loop Current */
#define L9530_STATE_REV_ACTIVE_LC   0x02     /* Reverse Active - Low Loop Current */
#define L9530_STATE_REV_ACTIVE_HC   0x06     /* Reverse Active - High Loop Current */
#define L9530_STATE_SCAN            0x01     /* Scan State */
#define L9530_STATE_RINGING         0x05     /* Ringing */
#define L9530_STATE_GROUND_START    0x03     /* Ground Start */
#define L9530_STATE_DISCONNECT      0x07     /* Disconnect */
#define L9530_STATE_NULL            0x08     /* Invalid mode */

/* SLIC initialization and shutdown states */
#define L9530_STATE_INIT            L9530_STATE_FWD_ACTIVE_LC
#define L9530_STATE_DEINIT          L9530_STATE_FWD_ACTIVE_LC

#define BIT1POSITION                1
#define BIT2POSITION                2


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
   { L9530_STATE_FWD_ACTIVE_LC, XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { L9530_STATE_FWD_ACTIVE_HC, XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { L9530_STATE_REV_ACTIVE_LC, XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { L9530_STATE_REV_ACTIVE_HC, XDRV_APM_HVG_VOLTAGE_OFFHOOK  },
   { L9530_STATE_SCAN,          XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { L9530_STATE_RINGING,       XDRV_APM_HVG_VOLTAGE_RING     },
   { L9530_STATE_GROUND_START,  XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { L9530_STATE_DISCONNECT,    XDRV_APM_HVG_VOLTAGE_ONHOOK   },
   { L9530_STATE_NULL,          XDRV_APM_HVG_VOLTAGE_NONE     }
};


/* SLIC driver API functions (accessed through XDRV_SLIC interface) */
static void slicModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slicLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static void slicPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicIsOffhook( XDRV_SLIC *pDrv );
static int slicSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
static int slicGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static int slicSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static int slicSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicGetOverCurrentStatus( XDRV_SLIC *pDrv );
static XDRV_SINT16 slicGetDlp( XDRV_SLIC *pDrv );
static void slicProcessEvents( XDRV_SLIC *pDrv );
static XDRV_UINT32 slicProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                                      XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                                      XDRV_UINT8 set );
static int slicGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );

/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS  slicDrvFuncs =
{
   slicModeControl,
   slicLedControl,
   slicPhaseReversalControl,
   slicIsOffhook,
   slicSetRingParms,
   slicGetRingParms,
   slicSetBoostedLoopCurrent,
   slicSetPowerSource,
   slicGetOverCurrentStatus,
   slicGetDlp,
   slicProcessEvents,
   slicProbe,
   slicGetSlicParms
};


/* Private Functions */
static void OpenSlic( SLIC_6828_L9530_DRV *pDev );
static void CloseSlic( SLIC_6828_L9530_DRV *pDev );
static void SetIOCtrl( SLIC_6828_L9530_DRV *pDev, int mode );
static void SetState( SLIC_6828_L9530_DRV *pDev, int mode );
static XDRV_APM_HVG_VOLTAGE GetVoltage( XDRV_UINT16 mode );
static void SetupGpioControl( SLIC_6828_L9530_DRV *pDev, const SLICCFG_6828_L9530_PIN_INFO* pSlicPinInfo );
static void SetupGpioRegs( XDRV_UINT32 pin, XDRV_UINT32* ctrlp, XDRV_UINT32* datap, XDRV_UINT32* mask );
static XDRV_UINT32 ConvertBpinsToMask( SLICCFG_6828_L9530_PIN_INFO* pDev, XDRV_UINT32 value );


/*
*****************************************************************************
** FUNCTION:   slic6828L9530Init
**
** PURPOSE:    Initialize and open the L9530 SLIC
**
** PARAMETERS: pDev           - pointer to the L9530 SLIC info structure
**             chan           - channel number
**             pSlicPinInfo   - SLIC conectivity configuration structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6828L9530Init
(
   SLIC_6828_L9530_DRV              *pDev,
   int                              chan,
   const SLICCFG_6828_L9530_PIN_INFO *pSlicPinInfo,
   XDRV_APM                         *pApmDrv
)
{
   memset( pDev, 0, sizeof( SLIC_6828_L9530_DRV ) );

   /* Copy the pin information to the internal driver structure */
   memcpy( &pDev->pinInfo, pSlicPinInfo, sizeof(pDev->pinInfo) );

   /* Set up GPIOs and masks for B0, B1, B2, NSTAT and TLDEN */
   SetupGpioControl( pDev, pSlicPinInfo );

   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d BO-B3 ctrlp     = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoCtrlp));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d BO-B3 datap     = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoDatap));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d B0-B3 mask      = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoStateMask));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d NSTAT ctrlp     = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoNstatCtrlp));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d NSTAT datap     = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoNstatDatap));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d NSTAT mask      = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoNstatMask));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d TLD ctrlp       = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoTldCtrlp));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d TLD datap       = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoTldDatap));
   XDRV_LOG_DBG((  XDRV_LOG_MOD_XDRV, "chan %d TLD mask        = 0x%08x", chan, (unsigned int)pDev->slicCfgInfo.slicIoTldMask));

   /* Initialize the chan number */
   pDev->chan = chan;

   /* Set the "meta-driver" interface */
   pDev->pDrvFuncs = &slicDrvFuncs;

   /* Initialize the APM driver structure */
   pDev->pApmDrv = pApmDrv;

   /* Driver enabled */
   pDev->bDrvEnabled = XDRV_TRUE;
   pDev->bRingDCoffsetEnabled = XDRV_FALSE;

   /* Enhanced control features (DSL will disable for now) */
   pDev->bEnhancedControl = XDRV_FALSE;

   /* Open the SLIC */
   OpenSlic( pDev );
}


/*
*****************************************************************************
** FUNCTION:   slic6828L9530Deinit
**
** PURPOSE:    Shutdown the L9530 SLIC
**
** PARAMETERS: pDev  - pointer to the L9530 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6828L9530Deinit( SLIC_6828_L9530_DRV *pDev )
{
   if (!pDev->bDrvEnabled)
   {
      /* driver already disabled */
      return;
   }

   /* Close the SLIC */
   CloseSlic( pDev );

   /* Reset driver structure. Driver disabled */
   memset( pDev, 0, sizeof( SLIC_6828_L9530_DRV ) );
}


/*
*****************************************************************************
** FUNCTION:   slicIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the SLIC driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL slicIsOffhook( XDRV_SLIC *pDrv )
{
   SLIC_6828_L9530_DRV *pDev  = (SLIC_6828_L9530_DRV *)pDrv;

   if ( *pDev->slicCfgInfo.slicIoNstatDatap & pDev->slicCfgInfo.slicIoNstatMask )
   {
      return( XDRV_FALSE );
   }
   else
   {
      return( XDRV_TRUE );
   }
}


/*
*****************************************************************************
** FUNCTION:   slicModeControl
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
static void slicModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;
   int chan = pDev->chan;
   int slicState;

   /* Determine to the appropriate SLIC mode */
   switch( mode )
   {
      /* Power denial mode */
      case XDRV_SLIC_MODE_LCFO:
      {
         slicState = L9530_STATE_DISCONNECT;
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = L9530_STATE_REV_ACTIVE_LC;
         }
         else
         {
            slicState = L9530_STATE_SCAN;
         }
      }
      break;

      /* Forward transmission modes */
      case XDRV_SLIC_MODE_OHT:
      case XDRV_SLIC_MODE_LCF:
      {
         if( pDev->phaseReversalActive )
         {
            if( pDev->bBoostedLoopCurrent )
            {
               slicState = L9530_STATE_REV_ACTIVE_HC;
            }
            else
            {
               slicState = L9530_STATE_REV_ACTIVE_LC;
            }
         }
         else
         {
            if( pDev->bBoostedLoopCurrent )
            {
               slicState = L9530_STATE_FWD_ACTIVE_HC;
            }
            else
            {
               slicState = L9530_STATE_FWD_ACTIVE_LC;
            }
         }
      }
      break;

      /* Reverse transmission modes */
      case XDRV_SLIC_MODE_OHTR:
      case XDRV_SLIC_MODE_RLCF:
      {
         if( pDev->phaseReversalActive )
         {
            if( pDev->bBoostedLoopCurrent )
            {
               slicState = L9530_STATE_FWD_ACTIVE_HC;
            }
            else
            {
               slicState = L9530_STATE_FWD_ACTIVE_LC;
            }
         }
         else
         {
            if( pDev->bBoostedLoopCurrent )
            {
               slicState = L9530_STATE_REV_ACTIVE_HC;
            }
            else
            {
               slicState = L9530_STATE_REV_ACTIVE_LC;
            }
         }
      }
      break;

      /* Tip Open */
      case XDRV_SLIC_MODE_TIPOPEN:
      {
         slicState = L9530_STATE_GROUND_START;
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
         slicState = L9530_STATE_RINGING;
      }
      break;

      default:
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC: ERROR - unrecognized SLIC mode 0x%x !!!", mode));
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

      /* Update SLIC state */
      SetIOCtrl( pDev, slicState );

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
** FUNCTION:   slicLedControl
**
** PURPOSE:    This function controls the on board Hook State LED
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* SLICs do not control LEDs on the 6828 */
}


/*
*****************************************************************************
** FUNCTION:   slicPhaseReversalControl
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
static void slicPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;

   /* Set the phase reversal control flag */
   pDev->phaseReversalActive = value;

   if(( pDev->slicMode == XDRV_SLIC_MODE_STANDBY ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHT ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHTR ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_LCF ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_RLCF ))
   {
      slicModeControl( pDrv, pDev->slicMode );
   }
}

/*
*****************************************************************************
** FUNCTION:   slicSetBoostedLoopCurrent
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
static int slicSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;

   /* Record boosted loop current state */
   pDev->bBoostedLoopCurrent = value;

   /* Update the SLIC and HVG states */
   slicModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Boosted loop current mode change (%d).", pDev->chan, value));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   slicSetRingParms
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
static int slicSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;

   if ( ringOffset )
   {
      pDev->bRingDCoffsetEnabled = XDRV_TRUE;
   }
   else
   {
      pDev->bRingDCoffsetEnabled = XDRV_FALSE;
   }

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
** FUNCTION:   slicGetRingParms
**
** PURPOSE:    This function gets the current ring waveform parameters
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
static int slicGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;

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
** FUNCTION:   OpenSlic
**
** PURPOSE:    Open the SLIC
**
** PARAMETERS: pDev  - pointer to the L9530 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void OpenSlic( SLIC_6828_L9530_DRV *pDev )
{

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "OpenSlic pre : *slicCfgInfo.slicIoCtrlp = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoCtrlp));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "OpenSlic pre : *slicCfgInfo.slicIoDatap = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoDatap));

   /* Enable B0-B2 as output pins */
   *pDev->slicCfgInfo.slicIoCtrlp |= pDev->slicCfgInfo.slicIoStateMask;

   /* Enable TLDEN as output pin */
   *pDev->slicCfgInfo.slicIoTldCtrlp |= pDev->slicCfgInfo.slicIoTldMask;

   /* Disable TLDEN (set to 0) by default */
   *pDev->slicCfgInfo.slicIoTldDatap &= ~pDev->slicCfgInfo.slicIoTldMask;

   /* Enable NSTAT (hook detect) as input pin */
   *pDev->slicCfgInfo.slicIoNstatCtrlp &= ~(pDev->slicCfgInfo.slicIoNstatMask);

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "OpenSlic post: *slicCfgInfo.slicIoCtrlp = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoCtrlp));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "OpenSlic post: *slicCfgInfo.slicIoDatap = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoDatap));

   /* Set initial SLIC state */
   SetState( pDev, L9530_STATE_INIT );
}


/*
*****************************************************************************
** FUNCTION:   CloseSlic
**
** PURPOSE:    Close the SLIC
**
** PARAMETERS: pDev  - pointer to the L9530 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CloseSlic( SLIC_6828_L9530_DRV *pDev )
{
   /* Disable the SLIC */
   SetState( pDev, L9530_STATE_DEINIT );
}


/*
*****************************************************************************
** FUNCTION:   SetState
**
** PURPOSE:    This function sets the SLIC into one of the defined states on a
**             specified channel.  In addition, the battery voltage and the
**             HVG are set.
**
** PARAMETERS: pDev      - pointer to the L9530 SLIC driver device structure
**             mode      - the mode to set the SLIC into
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void SetState( SLIC_6828_L9530_DRV *pDev, int mode )
{
   XDRV_APM_HVG_VOLTAGE  voltage;
   int                   chan      = pDev->chan;

   /* Lookup the battery voltage associated with the SLIC mode */
   voltage = GetVoltage( mode );

   /* If ring with DC offset option enabled, need to crank up
   ** the ring voltage to ultra high level during ringing */
   if ((pDev->bRingDCoffsetEnabled) && (mode == L9530_STATE_RINGING))
   {
      voltage = XDRV_APM_HVG_VOLTAGE_UHV_RING_15;
   }
   else if (( pDev->bEnhancedControlEna ) && ( voltage == XDRV_APM_HVG_VOLTAGE_ONHOOK ))
   {
      voltage = XDRV_APM_HVG_VOLTAGE_ONHOOK_LP;
   }

   /* Set the appropriate battery voltage level */
   if ( xdrvApmHvgSetVoltage( pDev->pApmDrv, chan, voltage ) )
   {
      /* We better disconnect the SLIC if we failed to apply proper voltage */
      SetIOCtrl( pDev, L9530_STATE_DISCONNECT );
   }
   else
   {
      /* Now make the mode change */
      SetIOCtrl( pDev, mode );
   }
}


/*
*****************************************************************************
** FUNCTION:   GetVoltage
**
** PURPOSE:    Lookup the voltage associated with the mode
**
** PARAMETERS: mode - SLIC mode
**
** RETURNS:    associated voltage
**
*****************************************************************************
*/
static XDRV_APM_HVG_VOLTAGE GetVoltage( XDRV_UINT16 mode )
{
   const MODEVOLTAGEMAP *pVoltageMap = modeVoltageMap;

   while ( mode != L9530_STATE_NULL )
   {
      if ( pVoltageMap->mode == mode )
      {
         return ( pVoltageMap->voltage );
      }
      else
      {
         pVoltageMap++;
      }
   }

   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, " XDRV_SLIC: ERROR - unrecognized SLIC mode 0x%x !!!", mode));

   return ( XDRV_APM_HVG_VOLTAGE_NONE );
}


/*
*****************************************************************************
** FUNCTION:   SetIOCtrl
**
** PURPOSE:    This function write a value to the SLIC IO register
**             associated with a specific channel
**
** PARAMETERS: pDev   - pointer to the L9530 SLIC driver device structure
**             mode   - the mode to set the SLIC into
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void SetIOCtrl( SLIC_6828_L9530_DRV *pDev, int value )
{
   XDRV_UINT32 slicData;
   XDRV_UINT32 valueMask = 0;

   valueMask = ConvertBpinsToMask( &pDev->pinInfo, value );

//   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "SetIOCtrl pre : *slicCfgInfo.slicIoDatap = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoDatap));

   /* get content of the SLIC data register */
   slicData = *pDev->slicCfgInfo.slicIoDatap;

   /* clear the state for this channel */
   slicData &= ~(pDev->slicCfgInfo.slicIoStateMask);

   /* update the state for this channel */
   slicData |= valueMask;

   /* write the state to the slic control register */
   *pDev->slicCfgInfo.slicIoDatap = slicData;

//   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "SetIOCtrl post: *slicCfgInfo.slicIoDatap = 0x%.8x", (unsigned int)*pDev->slicCfgInfo.slicIoDatap));
}

/*
*****************************************************************************
** FUNCTION:   slicSetPowerSource
**
** PURPOSE:    This function notifies the SLIC driver of any changes in
**             power  source (on battery or AC power)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 = device operating on battery power
**                     0 = device operating on AC power
**
** RETURNS:    0 on success
*****************************************************************************
*/
static int slicSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6828_L9530_DRV    *pDev      = (SLIC_6828_L9530_DRV *)pDrv;

   /* Record AC power status */
   if ( value )
   {
      pDev->bOnBattery = XDRV_TRUE;
   }
   else
   {
      pDev->bOnBattery = XDRV_FALSE;
   }

   /* Enable enhance control */
   if( pDev->bEnhancedControl )
   {
      if( value & 0xF0 )
      {
         pDev->bEnhancedControlEna = XDRV_TRUE;
      }
      else if ( value == 0 )
      {
         pDev->bEnhancedControlEna = XDRV_FALSE;
      }
   }

   /* Reset SLIC mode to current state to force configuration updates */
   slicModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Power Source mode change (%d).", pDev->chan, pDev->bOnBattery));

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slicGetOverCurrentStatus
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
static XDRV_BOOL slicGetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   SLIC_6828_L9530_DRV *pDev  = (SLIC_6828_L9530_DRV *)pDrv;
   XDRV_APM *pApmDrv = pDev->pApmDrv;
   int chan = pDev->chan;

   return xdrvApmGetOverCurrentStatus( pApmDrv, chan );
}

static XDRV_SINT16 slicGetDlp( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return( 0 );
}

static void slicProcessEvents( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return;
}

static XDRV_UINT32 slicProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                              XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                              XDRV_UINT8 set )
{
   (void) pDrv;

   return (0);
}

/*
*****************************************************************************
** FUNCTION:   slic3380L9530GetSlicParms
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
static int slicGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode )
{
   SLIC_6828_L9530_DRV *pDev = (SLIC_6828_L9530_DRV *)pDrv;

   /* Retrieve ring settings */
   *phaseReversal = pDev->phaseReversalActive;
   *loopCurrent = pDev->bBoostedLoopCurrent;
   *powerSource = pDev->bOnBattery;
   *slicMode = pDev->slicMode;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   SetupGpioControl
**
** PURPOSE:    Set up GPIO direction (control) pointers,
**             GPIO data pointers and GPIO mask for controling the SLIC
**
** PARAMETERS: pDev           -  [IN/OUT] Pointer to the driver structure
**             pSlicPinInfo   -  [IN]     Pointer to the SLIC pin information
**                                        (passed by board hal)
**
** RETURNS:
**
*****************************************************************************
*/
static void SetupGpioControl( SLIC_6828_L9530_DRV *pDev, const SLICCFG_6828_L9530_PIN_INFO* pSlicPinInfo )
{
   /* Set up GPIOs and masks for B0, B1, B2 */
   SetupGpioRegs( pSlicPinInfo->B0_pin,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoCtrlp,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoDatap,
                  &pDev->slicCfgInfo.slicIoStateMask );

   SetupGpioRegs( pSlicPinInfo->B1_pin,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoCtrlp,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoDatap,
                  &pDev->slicCfgInfo.slicIoStateMask );

   SetupGpioRegs( pSlicPinInfo->B2_pin,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoCtrlp,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoDatap,
                  &pDev->slicCfgInfo.slicIoStateMask );

   /* Set up GPIOs and masks for NSTAT */
   SetupGpioRegs( pSlicPinInfo->NSTAT_pin,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoNstatCtrlp,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoNstatDatap,
                  &pDev->slicCfgInfo.slicIoNstatMask );

   /* Set up GPIOs and masks for TLDEN */
   SetupGpioRegs( pSlicPinInfo->TLDEN_pin,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoTldCtrlp,
                  (XDRV_UINT32*)&pDev->slicCfgInfo.slicIoTldDatap,
                  &pDev->slicCfgInfo.slicIoTldMask );
}


/*
*****************************************************************************
** FUNCTION:   SetupGpioRegs
**
** PURPOSE:    Set up GPIO direction (control) pointers,
**             GPIO data pointers and GPIO mask for controling the SLIC
**
** PARAMETERS: pin   -  [IN]  Pin for which the GPIO pointers and masks are set up
**             ctrlp -  [OUT] Pointer to GPIO direction (control) based on pin number
**             datap -  [OUT] Pointer to GPIO data based on pin number
**             mask  -  [OUT] GPIO mask based on pin number
**
** RETURNS:
**
*****************************************************************************
*/
static void SetupGpioRegs( XDRV_UINT32 pin,
                           XDRV_UINT32* ctrlp,
                           XDRV_UINT32* datap,
                           XDRV_UINT32* mask )
{
   if ( pin >= 32 )
   {
      /* User high GPIO registers if
      ** the pin number is higher than 32 */
      *ctrlp = (XDRV_UINT32)&GPIO->GPIODir;
      *datap = (XDRV_UINT32)&GPIO->GPIOio;
      *mask |= ( 1 << (pin-32) );
   }
   else
   {
      /* User low GPIO registers if
      ** the pin number is higher than 32 */
      *ctrlp = (XDRV_UINT32)&GPIO->GPIODir + sizeof(XDRV_UINT32);
      *datap = (XDRV_UINT32)&GPIO->GPIOio + sizeof(XDRV_UINT32);
      *mask |= ( 1 << pin );
   }
}


/*
*****************************************************************************
** FUNCTION:   ConvertBpinsToMask
**
** PURPOSE:    Convert the 3-bit value of B2 B1 B0 to
**             a 32-bit mask based on the pin numbers for B2, B1, B0.
**             For example, if the pins for B2, B1, B0 are 29, 28, and 26
**             consecutively, the mask will be 0x34000000
**
** PARAMETERS: pinInfo  -  [IN]  Pointer to the SLIC pin layout
**             value    -  [IN]  3-bit value for B2, B1, B0
**
** RETURNS:    32-bit mask based on the B2, B1, B0 pin numbers
**
*****************************************************************************
*/
static XDRV_UINT32 ConvertBpinsToMask( SLICCFG_6828_L9530_PIN_INFO* pinInfo, XDRV_UINT32 value )
{
   XDRV_UINT32 mask = 0;
   XDRV_UINT8  bit;

   /* Bit 0 of value */
   bit = value & 1;
   mask |= bit << pinInfo->B0_pin;

   /* Bit 1 of value */
   bit = (value & (1 << BIT1POSITION)) >> BIT1POSITION;
   mask |= bit << pinInfo->B1_pin;

   bit = (value & (1 << BIT2POSITION)) >> BIT2POSITION;
   mask |= bit << pinInfo->B2_pin;

   return ( mask );
}

/*
*****************************************************************************
** FUNCTION:   slic6828L9530UpdateState
**
** PURPOSE:    Update L9530 SLIC state
**
** PARAMETERS: pDev - pointer to the L9530 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6828L9530UpdateState( SLIC_6828_L9530_DRV *pDev )
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
         SetIOCtrl( pDev, pDev->pendingSlicState );

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