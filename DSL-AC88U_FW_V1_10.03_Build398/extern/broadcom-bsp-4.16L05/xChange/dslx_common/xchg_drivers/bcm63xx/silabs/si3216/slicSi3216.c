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
*    Filename: slicSi3216.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      SI3216 Subscriber Line Interface Circuits (SLIC).
*
****************************************************************************/

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include "slicSi3216.h"
#include "slicSi3216Locale.h"
#include <proslic.h>
#include <interface_driver_si321x_spi.h>
#include <interface_driver_si_timer.h>
#include <si321x_constants.h>

/* Maximum slic channels supported */
#define  MAX_CHANNELS 4

/* SI3216 SLIC states */
#define SI3216_STATE_DISCONNECT        LF_OPEN        /* Disconnect */
#define SI3216_STATE_FWD_ACTIVE        LF_FWD_ACTIVE  /* Forward active */
#define SI3216_STATE_FWD_ONHOOK_TX     LF_FWD_OHT     /* On hook transmission, forward */
#define SI3216_STATE_TIP_OPEN          LF_TIP_OPEN    /* Tip Open mode */
#define SI3216_STATE_RINGING           LF_RINGING     /* Ringing */
#define SI3216_STATE_REV_ACTIVE        LF_REV_ACTIVE  /* Reverse active */
#define SI3216_STATE_REV_ONHOOK_TX     LF_REV_OHT     /* On hook transmission, reverse */
#define SI3216_STATE_RING_OPEN         LF_RING_OPEN   /* Ring Open mode */
                                                            

/* SLIC initial/shutdown states */
#define SI3216_STATE_INIT              SI3216_STATE_DISCONNECT
#define SI3216_STATE_DEINIT            SI3216_STATE_DISCONNECT

/* private functions */
static int mapVrgLocaleToSi321x ( VRG_COUNTRY vrgLocale );

/* SLIC "meta-driver" interface functions */
static void slicSi3216ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );
static void slicSi3216LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static void slicSi3216PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicSi3216IsOffhook( XDRV_SLIC *pDrv );
static int slicSi3216SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
static int slicSi3216GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
static int slicSi3216SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static int slicSi3216SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
static XDRV_BOOL slicSi3216GetOverCurrentStatus( XDRV_SLIC *pDrv );
static XDRV_SINT16 slicSi3216GetDlp( XDRV_SLIC *pDrv );
static void slicProcessEvents( XDRV_SLIC *pDrv );
static XDRV_UINT32 slicProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                              XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                              XDRV_UINT8 set );
                              
/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS  slicSi3216DrvFuncs =
{
   slicSi3216ModeControl,
   slicSi3216LedControl,
   slicSi3216PhaseReversalControl,
   slicSi3216IsOffhook,
   slicSi3216SetRingParms,
   slicSi3216GetRingParms,
   slicSi3216SetBoostedLoopCurrent,
   slicSi3216SetPowerSource,
   slicSi3216GetOverCurrentStatus,
   slicSi3216GetDlp,
   slicProcessEvents,
   slicProbe
};

/*****************************************************************
 *  Private PROSlic variables                                    *
 *                                                               *
 * Depending on how many different devices are present, whether  *
 * they are single slic/dual slic, whether they have different   *
 * mspi ids, whether they are daisy chained or not, the sizes    *
 * of the following arrays will change. Currently this driver is *
 * configured as follows:                                        *
 *    - Single channel per slic                                  *
 *    - Different mspi id per slic                               *
 *    - Non-daisy chain mode connection                          *
 *                                                               *
 * The array elements are interconnected by the PROSlic api as   *
 * follows:                                                      *
 *                                                               *
 *  +-----------------+           +-------------------+          *
 *  | pProSlicChanPtr | --------> | ProslicDeviceType | -----+   *
 *  +-----------------+           +-------------------+      |   *                                                      
 *                                                           |   *
 *                                                           |   *
 *     +--------+                 +----------------------+   |   *
 *     | ctrl_S | <-------------- | controlInterfaceType | <-+   *
 *     +--------+                 +----------------------+       *
 *                                                               *
 *                                                               *
 *                                                               *
 *                                                               *
 *****************************************************************/
static ctrl_S               spiRstObj[MAX_CHANNELS];         // H/W Control interface objects, indexed per channel
static proslicChanType_ptr  pProSlicChanPtr[MAX_CHANNELS];    // Array of ptr to Proslic Channel types, indexed per channel
static controlInterfaceType * pProHWIntf[MAX_CHANNELS];       // Array of ptr to controller interfaces, indexed per channel
static ProslicDeviceType    * pProSLICDevices[MAX_CHANNELS];  // Array of ptr to Proslic Device types, indexed per channel


/*
*****************************************************************************
** FUNCTION:   slicSi3216Init
**
** PURPOSE:    Initialize and open the Si3216 SLIC
**
** PARAMETERS: chan      - endpt channel associated with slic
**             pDev      - pointer to the Si3216 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSi3216Init( int chan, SLIC_SI3216_DRV *pDev )
{
   int si321xLocale;
   
   /* Null timer object */
   int timerObj = (int)NULL;
   
   /* Initialize the SLIC information for each channel */
   pDev->chan = chan;
   pDev->pDrvFuncs = &slicSi3216DrvFuncs;

   /* Driver enabled */
   pDev->bDrvEnabled = XDRV_TRUE;
      
   /* Assign mspidevice id to spireset object per channel */
   spiRstObj[pDev->chan].portID = pDev->mspiDevId;
   
	/* Create a control interface */
	ProSLIC_createControlInterface( &pProHWIntf[pDev->chan] );
	
	/* Create Single Device */
	ProSLIC_createDevice( &pProSLICDevices[pDev->chan] );
	
	/* Create Single Channel */
	ProSLIC_createChannel( &pProSlicChanPtr[pDev->chan] ); 
      
   /* Configure the interface driver apis */
   ProSLIC_setSWDebugMode ( pProSlicChanPtr[pDev->chan], 1);
	ProSLIC_setControlInterfaceCtrlObj        (pProHWIntf[pDev->chan], &spiRstObj[pDev->chan]    );
	ProSLIC_setControlInterfaceReset          (pProHWIntf[pDev->chan], ctrl_si321xResetWrapper         );
	ProSLIC_setControlInterfaceWriteRegister  (pProHWIntf[pDev->chan], ctrl_si321xWriteRegisterWrapper );
	ProSLIC_setControlInterfaceReadRegister   (pProHWIntf[pDev->chan], ctrl_si321xReadRegisterWrapper  );		
	ProSLIC_setControlInterfaceWriteRAM       (pProHWIntf[pDev->chan], ctrl_si321xWriteRAMWrapper      );
	ProSLIC_setControlInterfaceReadRAM        (pProHWIntf[pDev->chan], ctrl_si321xReadRAMWrapper       );
	ProSLIC_setControlInterfaceTimerObj       (pProHWIntf[pDev->chan], &timerObj                       );
	ProSLIC_setControlInterfaceDelay          (pProHWIntf[pDev->chan], time_DelayWrapper               );
	ProSLIC_setControlInterfaceTimeElapsed    (pProHWIntf[pDev->chan], time_TimeElapsedWrapper         );
	ProSLIC_setControlInterfaceGetTime        (pProHWIntf[pDev->chan], time_GetTimeWrapper             );

	/* Initialize SLIC channel, 1 slic device / channel, 1 hardware interface / channel */
   ProSLIC_SWInitChan (    pProSlicChanPtr[pDev->chan], 
                           pDev->chan, 
                           SI321X_TYPE, 
                           pProSLICDevices[pDev->chan], 
                           pProHWIntf[pDev->chan]        );	

                              
   /* Prosslic reset calls the reset wrapper function which does nothing in our case since slic hw resets
      are handled at the boardhal level. However, one important thing which ProSLIC_Reset does do is that
      it sets the slics to operate in DAISY CHAIN spi mode. Since we wont use ProsSLIC_Reset, we dont need 
      to worry about it */
      /* ProSLIC_Reset(pProSlicChanPtr[pDev->chan]); */
   
   /* Init single device at a time, the size parameter refers to the number of slic channels per device,
      and the &pProSlicChanPtr[pDev->chan] points to the channel structure for the first channel on the device  */
  	ProSLIC_Init(&pProSlicChanPtr[pDev->chan],1);
  		  	
  	/* Setup some parameters */
	ProSLIC_DCFeedSetup (pProSlicChanPtr[pDev->chan],DEFAULT_DCFEED);
		
	/* PCM setup */
		
	if( pDev->pcmMode == SI3216_LINEAR || pDev->pcmMode == SI3216_WIDEBAND )
   {
      ProSLIC_PCMTimeSlotSetup( pProSlicChanPtr[pDev->chan],((pDev->rxTimeSlot*16)+1),((pDev->txTimeSlot*16)+1) );
      if( pDev->pcmMode == SI3216_LINEAR )
	   {
         ProSLIC_PCMSetup (pProSlicChanPtr[pDev->chan],PCM_LINR_NGTVEDGE_TRISTATE);   	
	   }
	   else
	   {
   	   /* wideband */
         ProSLIC_PCMSetup (pProSlicChanPtr[pDev->chan],PCM_WBND_NGTVEDGE_TRISTATE);
      }
   }
   else
   {
      ProSLIC_PCMTimeSlotSetup( pProSlicChanPtr[pDev->chan],((pDev->rxTimeSlot*8)+1),((pDev->txTimeSlot*8)+1) );
      if( pDev->pcmMode == SI3216_ALAW )
      {
         ProSLIC_PCMSetup (pProSlicChanPtr[pDev->chan], PCM_ALAW_NGTVEDGE_TRISTATE);      
      }
      else
      {
         ProSLIC_PCMSetup (pProSlicChanPtr[pDev->chan], PCM_ULAW_NGTVEDGE_TRISTATE);      
      }
   }
   
   /* Start pcm interface */
	ProSLIC_PCMStart(pProSlicChanPtr[pDev->chan]);	
	
	/* Set initial linefeed state */
   ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_INIT );
   
   /* Map vrg locale to si321x locale */
   si321xLocale = mapVrgLocaleToSi321x ( pDev->country );
   
   /* Set locale specific parameters */
   if ( si321xLocale != SI321X_LOCALE_MAX )
   {
      /* Set Audio gains */
      ProSLIC_TXAudioGainSetup (pProSlicChanPtr[pDev->chan], si321xLocale);
      ProSLIC_RXAudioGainSetup (pProSlicChanPtr[pDev->chan], si321xLocale);
      
      /* Set Impedance */
      ProSLIC_ZsynthSetup(pProSlicChanPtr[pDev->chan], si321xLocale);          
      
      /* Set ringing params */
	   ProSLIC_RingSetup(pProSlicChanPtr[pDev->chan], si321xLocale);
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "XDRV_SLIC: ERROR - Invalid country code mapping. Vrg code:%d Si321x code:%d",
                      pDev->country, si321xLocale));          
   }

   pDev->bDrvEnabled = XDRV_TRUE;
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "XDRV_SLIC: Channel %d Initialized", pDev->chan ));
      
}


/*
*****************************************************************************
** FUNCTION:   slicSi3216Deinit
**
** PURPOSE:    Shutdown the Si3216 SLIC
**
** PARAMETERS: pDev  - pointer to the Si3216 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSi3216Deinit( SLIC_SI3216_DRV *pDev )
{
   if ( pDev->bDrvEnabled == XDRV_FALSE )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "XDRV_SLIC: ERROR - driver already disabled !!!"));
      return;
   }

   /* Set the SLIC to the appropriate disabled state */
   ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_DEINIT );
   
   /* Shut down PCM interface */
	ProSLIC_PCMStop(pProSlicChanPtr[pDev->chan]);
   
   /* Shut down DC-DC converter */
   /* TODO: Waiting for API update from SiLabs. Need to pwr down dc-dc converter/bias/pwr/ADC */
   
	/* Destroy Single Channel */
	ProSLIC_destroyChannel( &pProSlicChanPtr[pDev->chan] ); 
	
	/* Destroy Single Device */
	ProSLIC_destroyDevice( &pProSLICDevices[pDev->chan] );
   
	/* Destroy a control interface */
	ProSLIC_destroyControlInterface( &pProHWIntf[pDev->chan] );
	
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"SLIC %d Powered down", pDev->chan ));
}

/*
*****************************************************************************
** FUNCTION:   slicSi3216IsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the SLIC driver device structure
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
static XDRV_BOOL slicSi3216IsOffhook( XDRV_SLIC *pDrv )
{
   SLIC_SI3216_DRV *pDev = (SLIC_SI3216_DRV *)pDrv;
   XDRV_UINT8            hookState;

	/* Hook state check */
	ProSLIC_ReadHookStatus(pProSlicChanPtr[pDev->chan], &hookState);
	
   if ( hookState == PROSLIC_ONHOOK )
   {       
      return ( XDRV_FALSE );
   }
   else
   {      
      return ( XDRV_TRUE );
   }		
}

/*
*****************************************************************************
** FUNCTION:   slicProcessEvents
**
** PURPOSE:    Increment time and process events
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicProcessEvents( XDRV_SLIC *pDrv )
{
   (void) pDrv;
   return;
}

/*
*****************************************************************************
** FUNCTION:   slicProbe
**
** PURPOSE:    
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
static XDRV_UINT32 slicProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                              XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                              XDRV_UINT8 set )
{
   (void) pDrv;
   return 0;
}

/*
*****************************************************************************
** FUNCTION:   slicSi3216ModeControl
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
static void slicSi3216ModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SLIC_SI3216_DRV *pDev = (SLIC_SI3216_DRV *)pDrv;

   switch( mode )
   {
      /* Loop current feed off */
      case XDRV_SLIC_MODE_LCFO:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_DISCONNECT );
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_FWD_ACTIVE );
      }
      break;

      /* On-hook transmission */
      case XDRV_SLIC_MODE_OHT:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_FWD_ONHOOK_TX );
      }
      break;

      /* On-hook transmission reverse */
      case XDRV_SLIC_MODE_OHTR:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_REV_ONHOOK_TX );
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
	      ProSLIC_RingStart(pProSlicChanPtr[pDev->chan]); 
      }
      break;

      /* Loop current feed */
      case XDRV_SLIC_MODE_LCF:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_FWD_ACTIVE );
      }
      break;

      /* Reverse loop current feed */
      case XDRV_SLIC_MODE_RLCF:
      {
         ProSLIC_SetLinefeedStatus (pProSlicChanPtr[pDev->chan], SI3216_STATE_REV_ACTIVE );
      }
      break;

      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, " XDRV_SLIC: ERROR - unrecognized SLIC mode 0x%02x", mode));
      }
      break;
   }

}


/*
*****************************************************************************
** FUNCTION:   slicSi3216LedControl
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
static void slicSi3216LedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* SLICs do not control LEDs on the  */
   (void) pDrv;
   (void) value;
}


static void slicSi3216PhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;
};

static int slicSi3216SetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

static int slicSi3216GetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

static int slicSi3216SetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static int slicSi3216SetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

static XDRV_BOOL slicSi3216GetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

static XDRV_SINT16 slicSi3216GetDlp( XDRV_SLIC *pDrv )
{
   SLIC_SI3216_DRV *pDev    = NULL;
   VRG_COUNTRY      country = VRG_COUNTRY_MAX;

   pDev = (SLIC_SI3216_DRV *)pDrv;
   country = pDev->country;

   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   if( country >= VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "ERROR! Unsupported locale %d", country ));
      return ( 0 );   
   }
   else
   {
      return ( si321xLocaleMap[country].Dlp );
   }
}

static int mapVrgLocaleToSi321x ( VRG_COUNTRY vrgLocale )
{

   if( vrgLocale > VRG_COUNTRY_MAX-1 )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "ERROR! Unsupported locale %d", vrgLocale ));
      /* return last entry */
      return ( si321xLocaleMap[VRG_COUNTRY_MAX-1].si321xLocale );   
   }
   else
   {
      return ( si321xLocaleMap[vrgLocale].si321xLocale );
   }
   
}
