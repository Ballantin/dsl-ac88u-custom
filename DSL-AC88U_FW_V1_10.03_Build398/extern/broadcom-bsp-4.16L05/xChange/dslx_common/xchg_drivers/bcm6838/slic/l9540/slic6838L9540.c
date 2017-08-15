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
*    Filename: slic6838L9540.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      L9540 Subscriber Line Interface Circuits (SLIC).
*
****************************************************************************/

#include <xdrvLog.h>
#include <xdrvApm.h>
#include <xdrvCfg.h>
#include <string.h>
#include <board.h>
#include "slic6838L9540.h"
#include <hvg6838.h>

/* L9540 SLIC states in the following format: B2|B1|B0 */
#define L9540_STATE_FWD_ACTIVE_LC   0x00     /* Forward Active - Normal */
#define L9540_STATE_FWD_ACTIVE_ICV  0x04     /* Forward Active - ICV */
#define L9540_STATE_REV_ACTIVE_LC   0x02     /* Reverse Active - Normal */
#define L9540_STATE_WINK            0x06     /* Wink */
#define L9540_STATE_SCAN            0x01     /* Scan State */
#define L9540_STATE_RINGING         0x05     /* Ringing */
#define L9540_STATE_GROUND_START    0x03     /* Ground Start */
#define L9540_STATE_DISCONNECT      0x07     /* Disconnect */
#define L9540_STATE_NULL            0x08     /* Invalid mode */

/* Zarlink control bits */
#define L9540_STATE_CH0_MASK         0x0007
#define L9540_STATE_CH0_SHIFT             0
#define L9540_STATE_CH1_MASK         0x0038
#define L9540_STATE_CH1_SHIFT             3
#define L9540_LOOP_CURRENT_BIT_CH0   0x0040
#define L9540_LOOP_CURRENT_BIT_CH1   0x0080
#define L9540_TEST_LOAD_BIT_CH0      0x0100
#define L9540_TEST_LOAD_BIT_CH1      0x0200

/* Zarlink status bits */
#define L9540_LOOP_CLOSURE_LOW_CH0   0x0001
#define L9540_LOOP_CLOSURE_HIGH_CH0  0x0002
#define L9540_RING_TRIP_LOW_CH0      0x0004
#define L9540_RING_TRIP_HIGH_CH0     0x0008
#define L9540_THERMAL_SHUTDOWN_CH0   0x0010
#define L9540_LOOP_CLOSURE_LOW_CH1   0x0020
#define L9540_LOOP_CLOSURE_HIGH_CH1  0x0040
#define L9540_RING_TRIP_LOW_CH1      0x0080
#define L9540_RING_TRIP_HIGH_CH1     0x0100
#define L9540_THERMAL_SHUTDOWN_CH1   0x0200

#define L9540_STATUS_SHIFT               14
#define L9540_STATUS_MASK        0x00FFC000

/* SLIC "meta-driver" interface functions */
static void slic6838L9540ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slic6838L9540LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static void slic6838L9540PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slic6838L9540IsOffhook( XDRV_SLIC *pDrv );
static int slic6838L9540SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
static int slic6838L9540GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static int slic6838L9540SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static int slic6838L9540SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slic6838L9540GetOverCurrentStatus( XDRV_SLIC *pDrv );
static int slic6838L9540GetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );
static XDRV_SINT16 slic6838L9540GetDlp( XDRV_SLIC *pDrv );
static void slic6838L9540ProcessEvents( XDRV_SLIC *pDrv );
static XDRV_UINT32 slic6838L9540Probe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                                      XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                                      XDRV_UINT8 set );
/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS  slic6838L9540DrvFuncs =
{
   slic6838L9540ModeControl,
   slic6838L9540LedControl,
   slic6838L9540PhaseReversalControl,
   slic6838L9540IsOffhook,
   slic6838L9540SetRingParms,
   slic6838L9540GetRingParms,
   slic6838L9540SetBoostedLoopCurrent,
   slic6838L9540SetPowerSource,
   slic6838L9540GetOverCurrentStatus,
   slic6838L9540GetDlp,
   slic6838L9540ProcessEvents,
   slic6838L9540Probe,
   slic6838L9540GetSlicParms
};

/* Private Function Prototypes */
static void OpenSlic( SLIC_6838_L9540_DRV *pDev );
static void CloseSlic( SLIC_6838_L9540_DRV *pDev );
static void SetIOCtrl( SLIC_6838_L9540_DRV *pDev, int mode );
static void SetState( SLIC_6838_L9540_DRV *pDev, int mode, int voltage );

/*
*****************************************************************************
** FUNCTION:   slic6838L9540Init
**
** PURPOSE:    Initialize and open the L9540 SLIC
**
** PARAMETERS: pDev          - pointer to the L9540 SLIC info structure
**             chan          - channel number 
**             pSlicCfgCInfo - SLIC conectivity configuration structure 
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540Init
(
   SLIC_6838_L9540_DRV                   *pDev,
   int                                   chan,
   const SLICCFG_6838_L9540_CONNECT_INFO *pSlicCfgCInfo,
   XDRV_APM                              *pApmDrv
)
{
   memset( pDev, 0, sizeof( SLIC_6838_L9540_DRV ) );

   /* Initialize the SLIC configuration pointer */
   pDev->slicCfgInfo = *pSlicCfgCInfo;

   /* Initialize the chan number */
   pDev->chan = chan;

   /* Set the "meta-driver" interface */
   pDev->pDrvFuncs = &slic6838L9540DrvFuncs;

   /* Initialize the APM driver structure */
   pDev->pApmDrv = pApmDrv;

   /* Driver enabled */
   pDev->bDrvEnabled = XDRV_TRUE;
   pDev->bRingDCoffsetEnabled = XDRV_FALSE;

   /* Enhanced control features */
   pDev->bEnhancedControl = pSlicCfgCInfo->enhancedControl;

   /* Open the SLIC */
   OpenSlic( pDev );
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540Deinit
**
** PURPOSE:    Shutdown the L9540 SLIC
**
** PARAMETERS: pDev  - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540Deinit( SLIC_6838_L9540_DRV *pDev )
{
   if (!pDev->bDrvEnabled)
   {
      /* driver already disabled */      
      return;
   }

   /* Close the SLIC */
   CloseSlic( pDev );

   /* Reset driver structure. Driver disabled */
   memset( pDev, 0, sizeof( SLIC_6838_L9540_DRV ) );
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540IsOffHook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the SLIC driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL slic6838L9540IsOffhook( XDRV_SLIC *pDrv )
{

   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;
   XDRV_UINT32 slicData;

   if( pDev->slicMode == XDRV_SLIC_MODE_LCFO )
   {
      return( XDRV_FALSE );
   }

   /* Get content of the SLIC data register */
   slicData = pHvg->reg_hvg_status22;
   slicData = slicData & L9540_STATUS_MASK;
   slicData = slicData >> L9540_STATUS_SHIFT;

   /* Update the state for this channel */
   if( pDev->chan == 0 )
   {
      if( pDev->slicMode == XDRV_SLIC_MODE_RING )
      {
         if( slicData & L9540_RING_TRIP_HIGH_CH0 )
         {
            return( XDRV_FALSE );
         }
         else
         {
            return( XDRV_TRUE );
         }
      }
      else
      {
         if( slicData & L9540_LOOP_CLOSURE_LOW_CH0 )
         {
            return( XDRV_FALSE );
         }
         else
         {
            return( XDRV_TRUE );
         }
      }
   }
   else
   {
      if( pDev->slicMode == XDRV_SLIC_MODE_RING )
      {
         if( slicData & L9540_RING_TRIP_HIGH_CH1 )
         {
            return( XDRV_FALSE );
         }
         else
         {
            return( XDRV_TRUE );
         }
      }
      else
      {
         if( slicData & L9540_LOOP_CLOSURE_LOW_CH1 )
         {
            return( XDRV_FALSE );
         }
         else
         {
            return( XDRV_TRUE );
         }
      }
   }
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540ModeControl
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
static void slic6838L9540ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;
   int chan = pDev->chan;
   int slicState;

   /* Determine to the appropriate SLIC mode */
   switch( mode )
   {
      /* Power denial mode */
      case XDRV_SLIC_MODE_LCFO:
      {
         slicState = L9540_STATE_DISCONNECT;
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = L9540_STATE_REV_ACTIVE_LC;
         }
         else
         {
            slicState = L9540_STATE_SCAN;
         }
      }
      break;

      /* Forward transmission modes */
      case XDRV_SLIC_MODE_OHT:
      case XDRV_SLIC_MODE_LCF:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = L9540_STATE_REV_ACTIVE_LC;
         }
         else
         {
            slicState = L9540_STATE_FWD_ACTIVE_LC;
         }
      }
      break;

      /* Reverse transmission modes */
      case XDRV_SLIC_MODE_OHTR:
      case XDRV_SLIC_MODE_RLCF:
      {
         if( pDev->phaseReversalActive )
         {
            slicState = L9540_STATE_FWD_ACTIVE_LC;
         }
         else
         {
            slicState = L9540_STATE_REV_ACTIVE_LC;
         }
      }
      break;

      /* Tip Open */
      case XDRV_SLIC_MODE_TIPOPEN:
      {
         slicState = L9540_STATE_GROUND_START;
      }
      break;

      /* Wink */
      case XDRV_SLIC_MODE_WINK:
      {
         slicState = L9540_STATE_WINK;
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
         slicState = L9540_STATE_RINGING;
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
** FUNCTION:   slic6838L9540LedControl
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
static void slic6838L9540LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* SLICs do not control LEDs on the 6838 */
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540PhaseReversalControl
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
static void slic6838L9540PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

   /* Set the phase reversal control flag */
   pDev->phaseReversalActive = value;

   if(( pDev->slicMode == XDRV_SLIC_MODE_STANDBY ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHT ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_OHTR ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_LCF ) ||
      ( pDev->slicMode == XDRV_SLIC_MODE_RLCF ))
   {
      slic6838L9540ModeControl( pDrv, pDev->slicMode );
   }
}

/*
*****************************************************************************
** FUNCTION:   slic6838L9540SetBoostedLoopCurrent
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
static int slic6838L9540SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

   /* Record boosted loop current state */
   pDev->bBoostedLoopCurrent = value;

   /* Update the SLIC and HVG states */
   slic6838L9540ModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Boosted loop current mode change (%d).", pDev->chan, value));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540SetTestLoad
**
** PURPOSE:    This function controls the SLIC test load
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void slic6838L9540SetTestLoad( XDRV_SLIC *pDrv, int value )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

   /* Record boosted loop current state */
   pDev->bTestLoad = value;

   /* Update the SLIC and HVG states */
   slic6838L9540ModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Test load mode change (%d).", pDev->chan, value));

   return;
}


/*
*****************************************************************************
** FUNCTION:   slic6838L9540SetRingParms
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
static int slic6838L9540SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

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
** FUNCTION:   slic6838L9540GetRingParms
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
static int slic6838L9540GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

   /* Retrieve ring settings */
   *ringVoltage = pDev->ringVoltage;
   *ringOffset = pDev->ringOffset;
   *ringOffsetCal = pDev->ringOffsetCal;
   *ringFrequency = pDev->ringFrequency;
   *ringWaveshape = pDev->ringWaveshape;

   return ( 0 );
}

static XDRV_SINT16 slic6838L9540GetDlp( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return( 0 );
}

static void slic6838L9540ProcessEvents( XDRV_SLIC *pDrv )
{
   (void) pDrv;

   return;
}

static XDRV_UINT32 slic6838L9540Probe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                              XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                              XDRV_UINT8 set )
{
   (void) pDrv;

   return (0);
}

/*
*****************************************************************************
** FUNCTION:   slic6838L9540GetSlicParms
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
static int slic6838L9540GetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

   /* Retrieve ring settings */
   *phaseReversal = pDev->phaseReversalActive;
   *loopCurrent = pDev->bBoostedLoopCurrent;
   *powerSource = pDev->bOnBattery;
   *slicMode = pDev->slicMode;

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   OpenSlic
**
** PURPOSE:    Open the SLIC
**
** PARAMETERS: pDev  - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void OpenSlic( SLIC_6838_L9540_DRV *pDev )
{
   /* Set initial SLIC state */
   SetState( pDev, L9540_STATE_FWD_ACTIVE_LC, XDRV_APM_HVG_VOLTAGE_OFFHOOK );

   /* Save the current requested SLIC mode */
   pDev->slicMode = XDRV_SLIC_MODE_LCF;
}


/*
*****************************************************************************
** FUNCTION:   CloseSlic
**
** PURPOSE:    Close the SLIC
**
** PARAMETERS: pDev  - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CloseSlic( SLIC_6838_L9540_DRV *pDev )
{
   /* Set closing SLIC state */
   SetState( pDev, L9540_STATE_FWD_ACTIVE_LC, XDRV_APM_HVG_VOLTAGE_OFFHOOK );
}


/*
*****************************************************************************
** FUNCTION:   SetState
**
** PURPOSE:    This function sets the SLIC into one of the defined states on a
**             specified channel.  In addition, the battery voltage and the
**             HVG are set.
**
** PARAMETERS: pDev      - pointer to the L9540 SLIC driver device structure
**             mode      - the mode to set the SLIC into
**             voltage   - the voltage to set the HVG into
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void SetState( SLIC_6838_L9540_DRV *pDev, int mode, int voltage )
{
   /* Make the mode change */
   SetIOCtrl( pDev, mode );
}

/*
*****************************************************************************
** FUNCTION:   SetIOCtrl
**
** PURPOSE:    This function write a value to the SLIC IO register
**             associated with a specific channel
**
** PARAMETERS: pDev   - pointer to the L9540 SLIC driver device structure
**             mode   - the mode to set the SLIC into
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void SetIOCtrl( SLIC_6838_L9540_DRV *pDev, int value )
{
   volatile BCM6838_HVG *pHvg = BCM6838_HVG_PTR;
   XDRV_UINT32 slicData;

   /* Get content of the SLIC data register */
   slicData = pHvg->reg_hvg_zar_if_xmt_data;

   /* Update the state for this channel */
   if( pDev->chan == 0 )
   {
      /* Clear previous state */
      slicData &= ~( L9540_STATE_CH0_MASK | L9540_LOOP_CURRENT_BIT_CH0 | L9540_TEST_LOAD_BIT_CH0 );

      /* Set new state */
      slicData |= ( value << L9540_STATE_CH0_SHIFT );

      if( pDev->bBoostedLoopCurrent )
      {
         slicData |= L9540_LOOP_CURRENT_BIT_CH0;
      }

      if( pDev->bTestLoad )
      {
         slicData |= L9540_TEST_LOAD_BIT_CH0;
      }
   }
   else
   {
      /* Clear previous state */
      slicData &= ~( L9540_STATE_CH1_MASK | L9540_LOOP_CURRENT_BIT_CH1 | L9540_TEST_LOAD_BIT_CH1 );

      /* Set new state */
      slicData |= ( value << L9540_STATE_CH1_SHIFT );

      if( pDev->bBoostedLoopCurrent )
      {
         slicData |= L9540_LOOP_CURRENT_BIT_CH1;
      }

      if( pDev->bTestLoad )
      {
         slicData |= L9540_TEST_LOAD_BIT_CH1;
      }
   }

   /* Update the register */
   pHvg->reg_hvg_zar_if_xmt_data = slicData;
}

/*
*****************************************************************************
** FUNCTION:   slic6838L9540SetPowerSource
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
static int slic6838L9540SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_6838_L9540_DRV *pDev = (SLIC_6838_L9540_DRV *)pDrv;

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

   /* Reset SLIC mode to current state to force configuration updates */
   slic6838L9540ModeControl( pDrv, pDev->slicMode );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "XDRV_SLIC %d: Power Source mode change (%d).", pDev->chan, pDev->bOnBattery ));

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slic6838L9540GetOverCurrentStatus
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
static XDRV_BOOL slic6838L9540GetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   SLIC_6838_L9540_DRV *pDev  = (SLIC_6838_L9540_DRV *)pDrv;
   XDRV_APM *pApmDrv = pDev->pApmDrv;
   int chan = pDev->chan;
   
   return xdrvApmGetOverCurrentStatus( pApmDrv, chan );
}

/*
*****************************************************************************
** FUNCTION:   slic6838L9540UpdateState
**
** PURPOSE:    Update L9540 SLIC state
**
** PARAMETERS: pDev - pointer to the L9540 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slic6838L9540UpdateState( SLIC_6838_L9540_DRV *pDev )
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


