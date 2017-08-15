/***************************************************************************
*
*    Copyright 2008  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description: 
*      This is common code for all SiLabs SLIC devices, that uses ProSLIC API.
*
***************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <bosSleep.h>
#include <xdrvLog.h>
#include <board.h>
#include <boardparms.h>
#include <xchgAssert.h>
#include <xdrvSlicGains.h>
#include <bosMutex.h>
#include <linux/jiffies.h>

#include "silabsCommonSlic.h"
#include "slicSiLabsLocale.h"
#include "interface_driver_si_timer.h"
#include "vdaa.h"

#define LOG_MODULE_NAME       "SLIC"
#define LOG_DEV_NAME          "SiLabs"

#define SILABS_RINGBLOCK_DELAY     msecs_to_jiffies(1000)

/* SILABS SLIC states */
#define SILABS_STATE_DISCONNECT        LF_OPEN        /* Disconnect */
#define SILABS_STATE_FWD_ACTIVE        LF_FWD_ACTIVE  /* Forward active */
#define SILABS_STATE_FWD_ONHOOK_TX     LF_FWD_OHT     /* On hook transmission, forward */
#define SILABS_STATE_TIP_OPEN          LF_TIP_OPEN    /* Tip Open mode */
#define SILABS_STATE_RINGING           LF_RINGING     /* Ringing */
#define SILABS_STATE_REV_ACTIVE        LF_REV_ACTIVE  /* Reverse active */
#define SILABS_STATE_REV_ONHOOK_TX     LF_REV_OHT     /* On hook transmission, reverse */
#define SILABS_STATE_RING_OPEN         LF_RING_OPEN   /* Ring Open mode */
                                                            
/* SLIC initial/shutdown states */
#define SILABS_STATE_INIT              SILABS_STATE_DISCONNECT
#define SILABS_STATE_DEINIT            SILABS_STATE_DISCONNECT

#define LINEAR_NEGATIVEDGE_DAA					PCM_16LIN
#define ULAW_NEGATIVEEDGE_DAA						PCM_8ALAW
#define ALAW_NEGATIVEEDGE_DAA						PCM_8ULAW

/* Loop current requirements in 3.3 mA/bit */
#define SIDAAOFFHLCMIN 0x04		/* minimum current needed for off-hook  */
#define SIDAAOFFHLCMAX 0x1F		/* current indicates overload condition */
#define SIDAA_LCS_SCALE_NUM 33		/* Line Current Status Scale - from vdaa.c */
#define SIDAA_LCS_SCALE_DEN 10		/* Line Current Status Scale - from vdaa.c */


/* Helpers */
static int mapVrgLocaleToSiLabs ( VRG_COUNTRY vrgLocale );
static void slicSilabsReadRtpStatus(XDRV_SLIC *pDrv);
static void slicSilabsRingUnblock( unsigned long data );
static void slicSilabsMutexDeinit( SI_SLIC_CHANNEL_INFO *chanInfoP );
static void slicSilabsMutexInit( SI_SLIC_CHANNEL_INFO *chanInfoP );
static void slicSilabsWaitLineSettle(SI_SLIC_CHANNEL_INFO* pDev);

/*****************************************************************************
*
* FUNCTION:   initSiLabsLines
*
* PURPOSE:    Initialize lines of SiLabs devices.
*
* PARAMETERS: Lots!
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/
XDRV_SLIC_STATUS initSiLabsLines( SILABS_CONFIG* silabsConfig )
{
   XDRV_UINT8           deviceId = silabsConfig->pDev->deviceId;
   XDRV_UINT8           i;

   XDRV_UINT8           channelId[silabsConfig->numFxsPerDevice];
   XDRV_UINT8           daaChannelId[silabsConfig->numFxoPerDevice];

   XDRV_UINT8           numChansPerDevice;
   XDRV_UINT8           siLabsLocale;
   
   /* Total number of channels per device */
   numChansPerDevice = silabsConfig->numFxsPerDevice + silabsConfig->numFxoPerDevice;

   /* Map vrg locale to SiLabs locale */
   siLabsLocale = mapVrgLocaleToSiLabs( silabsConfig->pDev->locale );
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s Locale set to %d", LOG_MODULE_NAME, LOG_DEV_NAME, siLabsLocale ));


   for( i = 0; i < silabsConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = silabsConfig->pDev->slicChannel[i].chan;
      
      /*Initialize FXS line */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s FXS line %d", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));

      /* Setup some parameters */
      ProSLIC_DCFeedSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], siLabsLocale );
      ProSLIC_ZsynthSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], siLabsLocale );   

      /* Configures FXS gain. The gain applied is RELATIVE to the built-in gain */ 
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "FXS channel %d Rx Gain: %d", channelId[i], siLabsLocaleMap[silabsConfig->pDev->locale].Dlp ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "FXS channel %d Tx Gain: %d", channelId[i], siLabsLocaleMap[silabsConfig->pDev->locale].TxGain )); 
      /* TWEAK: TR57 and JP presets cannot be generated therefore they still have built-in gains. Since Silabs gains are relative,
      ** I need to apply 0/0 gain. JP Profiles does not actually work, and TR57 built-in gains measured at -6dB.
      */
      if ((siLabsLocale == SILABS_TR57) || (siLabsLocale == SILABS_JAPAN)) //locale for TR57 // JP according to siLabsLocale
      {
          siLabsLocaleMap[silabsConfig->pDev->locale].Dlp = 0;
          siLabsLocaleMap[silabsConfig->pDev->locale].TxGain = 0;
      } /* TWEAK */

      ProSLIC_AudioGainSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], 
                              siLabsLocaleMap[silabsConfig->pDev->locale].Dlp, 
                              siLabsLocaleMap[silabsConfig->pDev->locale].TxGain, 
                              siLabsLocale );
  
      ProSLIC_RingSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], siLabsLocale );

      /* PCM setup */
     if( silabsConfig->pDev->slicChannel[i].pcmMode == SILABS_WIDEBAND )
     {
        ProSLIC_PCMTimeSlotSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])],
                                  ((silabsConfig->pDev->slicChannel[i].rxTimeSlot*16)+1),
                                  ((silabsConfig->pDev->slicChannel[i].txTimeSlot*16)+1) );
        ProSLIC_PCMSetup ( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], LINEAR_NEGATIVEEDGE_WB_PCM );
     }
     else
     {	                  
      if( silabsConfig->pDev->slicChannel[i].pcmMode == SILABS_LINEAR )
      {
         ProSLIC_PCMTimeSlotSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])],
                                   ((silabsConfig->pDev->slicChannel[i].rxTimeSlot*16)+1),
                                   ((silabsConfig->pDev->slicChannel[i].txTimeSlot*16)+1) );
         ProSLIC_PCMSetup ( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], LINEAR_NEGATIVEEDGE_PCM );
      }
      else
      {
         ProSLIC_PCMTimeSlotSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])],
                                   ((silabsConfig->pDev->slicChannel[i].rxTimeSlot*16)+1),
                                   ((silabsConfig->pDev->slicChannel[i].txTimeSlot*16)+1) );
         if( silabsConfig->pDev->slicChannel[i].pcmMode == SILABS_ALAW )
         {
            ProSLIC_PCMSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], ALAW_NEGATIVEEDGE_PCM );      
         }
         else
         {
            ProSLIC_PCMSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], ULAW_NEGATIVEEDGE_PCM );      
         }
      }
    }

      /* Start pcm interface */
      ProSLIC_PCMStart( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])] );	

      /* Set initial linefeed state */
      ProSLIC_SetLinefeedStatus( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], SILABS_STATE_INIT );


      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: FXS line %d RX timeslot = %d ", LOG_MODULE_NAME, channelId[i], silabsConfig->pDev->slicChannel[i].rxTimeSlot ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: FXS line %d TX timeslot = %d ", LOG_MODULE_NAME, channelId[i], silabsConfig->pDev->slicChannel[i].txTimeSlot ));

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s FXS line %d initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));
      
      /* Driver enabled */
      silabsConfig->pDev->slicChannel[i].bDrvEnabled = XDRV_TRUE;
      
      /* init SLIC previous mode */ 
      silabsConfig->pDev->slicChannel[i].previousSlicMode = XDRV_SLIC_MODE_NULL;
   } /* for */
   
   
   for( i = 0; i < silabsConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = silabsConfig->pDev->daaChannel[i].chan;

      /* Initialize FXO line */     
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s FXO line", LOG_MODULE_NAME, LOG_DEV_NAME ));

      /* NOTE: Until SiLabs fixes their mess, use size of 'numFxoPerDevice' here and point channel pointer to DAA. */
      /* The extra ProSLIC_Init call here is so that channelType = DAA for the channel pointer in the ProSLIC API source. */
      /* The previous call to ProSLIC_Init in FXS section, only enables the DAA_CTL at the end of it. */
//      ProSLIC_Init( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]), silabsConfig->numFxoPerDevice );
      ProSLIC_Init_MultiBOM( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]), 
                             silabsConfig->numFxoPerDevice,
                             silabsConfig->bomType );	
      /* NOTE: Again, use size of 'numFxoPerDevice' here, and point channel pointer to DAA */
      Vdaa_Init( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]), silabsConfig->numFxoPerDevice );

   	/* Setup some parameters */
      Vdaa_CountrySetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], siLabsLocale );       
  	   Vdaa_RingDetectSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], siLabsLocale );
      
      /*PCM setup */
      if( silabsConfig->pDev->daaChannel[i].pcmMode == SILABS_LINEAR )
      {
         Vdaa_PCMTimeSlotSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])],
                                ((silabsConfig->pDev->daaChannel[i].rxTimeSlot*16)+1),
                                ((silabsConfig->pDev->daaChannel[i].txTimeSlot*16)+1) );
         Vdaa_PCMSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], LINEAR_NEGATIVEDGE_DAA);
      }
      else
      {
         Vdaa_PCMTimeSlotSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])],
                                ((silabsConfig->pDev->daaChannel[i].rxTimeSlot*8)+1),
                                ((silabsConfig->pDev->daaChannel[i].txTimeSlot*8)+1) );
         if( silabsConfig->pDev->daaChannel[i].pcmMode == SILABS_ALAW )
         {
            Vdaa_PCMSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], ALAW_NEGATIVEEDGE_DAA );      
         }
         else
         {
            Vdaa_PCMSetup( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], ULAW_NEGATIVEEDGE_DAA );      
         }
      }
   
      /* Start pcm interface */
	   Vdaa_PCMStart( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])] );	
 
      Vdaa_SetHookStatus( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], VDAA_ONHOOK);
      
      
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: FXO line %d RX timeslot = %d ", LOG_MODULE_NAME, daaChannelId[i], silabsConfig->pDev->daaChannel[i].rxTimeSlot ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: FXO line %d TX timeslot = %d ", LOG_MODULE_NAME, daaChannelId[i], silabsConfig->pDev->daaChannel[i].txTimeSlot ));  

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s FXO line initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME ));
    
      /* Driver enabled */
      silabsConfig->pDev->daaChannel[i].bDrvEnabled = XDRV_TRUE;  
   }

   return XDRV_SLIC_STATUS_SUCESS;
}

/*****************************************************************************
*
* FUNCTION:   initSiLabsDevice
*
* PURPOSE:    Initialize SiLabs SLIC/SLAC device.
*
* PARAMETERS: Lots!
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/                                 
XDRV_SLIC_STATUS initSiLabsDevice( SILABS_CONFIG* silabsConfig )
{
   XDRV_UINT8     deviceId = silabsConfig->pDev->deviceId;
   XDRV_UINT8     i;
   XDRV_UINT8     channelId[silabsConfig->numFxsPerDevice];
   XDRV_UINT8     daaChannelId[silabsConfig->numFxoPerDevice];
   XDRV_UINT8     numChansPerDevice;

  
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s device %d", LOG_MODULE_NAME, LOG_DEV_NAME, silabsConfig->pDev->deviceId ));

   /* Total number of channels per device */
   numChansPerDevice = silabsConfig->numFxsPerDevice + silabsConfig->numFxoPerDevice;

   /* Create a control interface */
   SiVoice_createControlInterface( &(silabsConfig->voiceHWIntf[deviceId]) );
   
   /* Create Device */
   SiVoice_createDevice( &(silabsConfig->voiceDevices[deviceId]) );   
	   
   for( i = 0; i < silabsConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = silabsConfig->pDev->slicChannel[i].chan;
      
      SiVoice_createChannel( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]) );      
      SiVoice_SWInitChan( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])],
                          channelId[i],
                          silabsConfig->chipType,
                          silabsConfig->voiceDevices[deviceId],
                          silabsConfig->voiceHWIntf[deviceId] );
      SiVoice_setSWDebugMode( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], TRUE );
      
      /* Save address. */
      silabsConfig->pDev->slicChannel[i].pProSlicChan = silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])];
      
      /* setup timer for SoC control RTP */
      setup_timer(&silabsConfig->pDev->slicChannel[i].ringtripTimer, slicSilabsRingUnblock, (unsigned long)&silabsConfig->pDev->slicChannel[i]);
      /* Create SILAB Mutex */
      slicSilabsMutexInit( &silabsConfig->pDev->slicChannel[i] );
   }
#if 0   
   if( numFxoPerDevice == 0 )
   {  
      if( chipType == SI3217X_TYPE )
      { 
         Si3217x_General_Configuration.daa_enable = 0;  /* disable DAA directly in table */
      }
   }
#endif
   for( i = 0; i < silabsConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = silabsConfig->pDev->daaChannel[i].chan;
      
      SiVoice_createChannel( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]) );      
      SiVoice_SWInitChan( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])],
                          daaChannelId[i],
                          silabsConfig->chipType,
                          silabsConfig->voiceDevices[deviceId],
                          silabsConfig->voiceHWIntf[deviceId] );
      SiVoice_setSWDebugMode( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])], TRUE );
      
      /* Save address. */
      silabsConfig->pDev->daaChannel[i].pDaaProSlicChan = silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])];
   }

   /* Configure the interface driver apis */
   SiVoice_setControlInterfaceCtrlObj        ( silabsConfig->voiceHWIntf[deviceId], (void *)&(silabsConfig->spiObj[deviceId]) );
   SiVoice_setControlInterfaceReset          ( silabsConfig->voiceHWIntf[deviceId], NULL );
   SiVoice_setControlInterfaceWriteRegister  ( silabsConfig->voiceHWIntf[deviceId], silabsConfig->writeRegister_fptr );
   SiVoice_setControlInterfaceReadRegister   ( silabsConfig->voiceHWIntf[deviceId], silabsConfig->readRegister_fptr  );		
   SiVoice_setControlInterfaceWriteRAM       ( silabsConfig->voiceHWIntf[deviceId], silabsConfig->writeRAM_fptr      );
   SiVoice_setControlInterfaceReadRAM        ( silabsConfig->voiceHWIntf[deviceId], silabsConfig->readRAM_fptr       );
   SiVoice_setControlInterfaceTimerObj       ( silabsConfig->voiceHWIntf[deviceId], silabsConfig->timerObj                  );
   SiVoice_setControlInterfaceDelay          ( silabsConfig->voiceHWIntf[deviceId], time_DelayWrapper         );
   SiVoice_setControlInterfaceTimeElapsed    ( silabsConfig->voiceHWIntf[deviceId], time_TimeElapsedWrapper   );
   SiVoice_setControlInterfaceGetTime        ( silabsConfig->voiceHWIntf[deviceId], time_GetTimeWrapper       );
   SiVoice_setControlInterfaceSemaphore      ( silabsConfig->voiceHWIntf[deviceId], NULL );

   /* In 5.2.0 of API, the second parameter (size) is suppose to include both FXS and FXO channels */
   /* NOTE: Until SiLabs fixes their mess, point channel pointer to FXS and use 'numFxsPerDevice' instead of 'numChansPerDevice'. */
   /*       For non-combo chips, it does not really matter. */
   /*       slicChannel[0].chan is used on purpose. */
//   ProSLIC_Init( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,silabsConfig->pDev->slicChannel[0].chan)]), 
//                 silabsConfig->numFxsPerDevice );
   ProSLIC_Init_MultiBOM( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,silabsConfig->pDev->slicChannel[0].chan)]), 
                          silabsConfig->numFxsPerDevice,
                          silabsConfig->bomType );	
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s device BOM Type: %d", LOG_MODULE_NAME, LOG_DEV_NAME, silabsConfig->bomType ));

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s device %d initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME, silabsConfig->pDev->deviceId ));

   return ( XDRV_SLIC_STATUS_SUCESS );
}

/*****************************************************************************
*
* FUNCTION:   deinitSiLabs
*
* PURPOSE:    De-initialize SiLabs SLIC/SLAC device.
*
* PARAMETERS: Lots!
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/ 
XDRV_SLIC_STATUS deinitSiLabs( SILABS_CONFIG* silabsConfig )
{
   XDRV_UINT8           deviceId = silabsConfig->pDev->deviceId;
   XDRV_UINT8           i;

   XDRV_UINT8           channelId[silabsConfig->numFxsPerDevice];
   XDRV_UINT8           daaChannelId[silabsConfig->numFxoPerDevice];

   XDRV_UINT8           numChansPerDevice;
 
   /* Total number of channels per device */
   numChansPerDevice = silabsConfig->numFxsPerDevice + silabsConfig->numFxoPerDevice;

   for( i = 0; i < silabsConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = silabsConfig->pDev->slicChannel[i].chan;
      
      /*De-initialize FXS line */
      XDRV_LOG_INFO((XDRV_LOG_MOD_SLIC, "%s: De-initializing %s FXS line %d\n", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));
      
      if( silabsConfig->pDev->slicChannel[i].bDrvEnabled == XDRV_FALSE )  
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "XDRV_SLIC: ERROR - SLIC driver already disabled !!!\n"));
      }
      else
      {
         silabsConfig->pDev->slicChannel[i].bDrvEnabled = XDRV_FALSE;
      }
     
      /* Set linefeed state */
      ProSLIC_SetLinefeedStatus( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])], SILABS_STATE_DEINIT );

      /* Shut down PCM interface */
      ProSLIC_PCMStop( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])] );	
      
      del_timer_sync(&silabsConfig->pDev->slicChannel[i].ringtripTimer);
         /* Destroy SILAB Mutex */
      slicSilabsMutexDeinit( &silabsConfig->pDev->slicChannel[i] );

      SiVoice_destroyChannel( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]) ); 
   }

   for( i = 0; i < silabsConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = silabsConfig->pDev->daaChannel[i].chan;

      /* De-initialize FXO line */     
      XDRV_LOG_INFO((XDRV_LOG_MOD_SLIC, "%s: De-initializing %s FXO line\n", LOG_MODULE_NAME, LOG_DEV_NAME ));

      /* Driver enabled */ 
      if( silabsConfig->pDev->daaChannel[i].bDrvEnabled == XDRV_FALSE )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "XDRV_SLIC: ERROR - DAA driver already disabled !!!\n"));
      }
      else
      {
         silabsConfig->pDev->daaChannel[i].bDrvEnabled = XDRV_FALSE;
      }
      
      Vdaa_PCMStop( silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])] );
      
      SiVoice_destroyChannel( &(silabsConfig->voiceChans[SI_SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]) );	
   }

   /* Destroy Device */
   SiVoice_destroyDevice( &(silabsConfig->voiceDevices[deviceId]) ); 
   
   /* Destroy control interface */
   SiVoice_destroyControlInterface( &(silabsConfig->voiceHWIntf[deviceId]) );
   
   XDRV_LOG_INFO((XDRV_LOG_MOD_SLIC, "%s: %s device %d de-initialized OK\n", LOG_MODULE_NAME, LOG_DEV_NAME, silabsConfig->pDev->deviceId ));

   return ( XDRV_SLIC_STATUS_SUCESS );
}
/*****************************************************************************
*
* FUNCTION:   deviceSiLabsReset
*
* PURPOSE:    Reset the SLIC
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void deviceSiLabsReset( XDRV_UINT16 rstGpioPin )
{
   /* 
    * Set up the GPIO output pin that resets the SLIC, reset the SLIC 
    */
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep(5);
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep(250);
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep(250);
   
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Reset applied on GPIO pin %d", LOG_MODULE_NAME, rstGpioPin&BP_GPIO_NUM_MASK ));
}

/*
*****************************************************************************
** FUNCTION:   mapVrgLocaleToSiLabs
**
** PURPOSE:    This function maps VRG_COUNTRY locale to SiLabs locale.
**
** PARAMETERS: vrgLocale - country
**
** RETURNS:    Country value in Si32176 locale table.
**
*****************************************************************************
*/
static int mapVrgLocaleToSiLabs ( VRG_COUNTRY vrgLocale )
{

   if( vrgLocale >= VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: ERROR! Unsupported locale %d", LOG_MODULE_NAME, vrgLocale ));
      /* return last entry */
      return ( siLabsLocaleMap[VRG_COUNTRY_MAX-1].siLabsLocale );   
   }
   else
   {
      return ( siLabsLocaleMap[vrgLocale].siLabsLocale );
   }
   
}


/*******************/
/* XDRV_SLIC_FUNCS */
/*******************/
/*
*****************************************************************************
** FUNCTION:   slicSiLabsGetDlp
**
** PURPOSE:    This function retrieves the Egress RX gains for the locale.
**
** PARAMETERS: 
**
** RETURNS:    Rx gain.
**
*****************************************************************************
*/
XDRV_SINT16 slicSiLabsGetDlp( XDRV_SLIC *pDrv )
{
   SI_SLIC_CHANNEL_INFO *pDev = (SI_SLIC_CHANNEL_INFO *)pDrv;
   VRG_COUNTRY country = VRG_COUNTRY_MAX;
   
   country = ((SI_SLIC_DEVICE_DRV *)pDev->pParent)->locale; 
   if( country >= VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"slicSiLabsGetDlp: ERROR! Unsupported locale %d", 
                country));
   }
   else
   {
      return ( siLabsLocaleMap[country].Dlp );
   }

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsProcessEvents
**
** PURPOSE:    Increment time and process events
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSiLabsProcessEvents( XDRV_SLIC *pDrv )
{
   (void) pDrv;
   return;
}

/*
*****************************************************************************
** FUNCTION:   slicSiLabsIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: 
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL slicSiLabsIsOffhook( XDRV_SLIC *pDrv )
{
   SI_SLIC_CHANNEL_INFO* pDev = (SI_SLIC_CHANNEL_INFO*)pDrv;
   XDRV_UINT8            hookState;

   /* use this polling mechanism to poll rtp bit on SLAC*/
   slicSilabsReadRtpStatus( pDrv );

   /* Hook state check */
   ProSLIC_ReadHookStatus(pDev->pProSlicChan, &hookState);
   if ( hookState == PROSLIC_ONHOOK )
   {
      pDev->lastOffhookStatus = false;
      return ( XDRV_FALSE );
   }
   else
   {
      if( !pDev->lastOffhookStatus )
      {
         pDev->ringBurstBlockFlag = true;
         bosMutexAcquire( &pDev->silabsPolRevMutex );
         /* stop ringing, set to LCF/RLCF mode */
         slicSiLabsModeControl( pDrv, pDev->phaseReversalActive == false ? XDRV_SLIC_MODE_LCF : XDRV_SLIC_MODE_RLCF );
         bosMutexRelease( &pDev->silabsPolRevMutex );
         mod_timer( &pDev->ringtripTimer,jiffies + SILABS_RINGBLOCK_DELAY );
         pDev->lastOffhookStatus = true;
      }
      return ( XDRV_TRUE );
   }
}

/*
*****************************************************************************
** FUNCTION:   slicSiLabsModeControl
**
** PURPOSE:    Sets the SLIC into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLIC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicSiLabsModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SI_SLIC_CHANNEL_INFO* pDev = (SI_SLIC_CHANNEL_INFO* )pDrv;

   bosMutexAcquire( &pDev->silabsEquCriMutex );

   if( pDev->previousSlicMode == mode )
   {
     bosMutexRelease( &pDev->silabsEquCriMutex );
     return;
   }

   /* poll and wait until change mode command is allowed or timeout*/
   slicSilabsWaitLineSettle( pDev ); 

   switch( mode )
   {
      /* Loop current feed off */
      case XDRV_SLIC_MODE_LCFO:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_DISCONNECT );
         pDev->previousSlicMode = XDRV_SLIC_MODE_LCFO;
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_FWD_ACTIVE );
         pDev->previousSlicMode = XDRV_SLIC_MODE_STANDBY;
      }
      break;

      /* On-hook transmission */
      case XDRV_SLIC_MODE_OHT:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_FWD_ONHOOK_TX );
         pDev->previousSlicMode = XDRV_SLIC_MODE_OHT;
      }
      break;

      /* On-hook transmission reverse */
      case XDRV_SLIC_MODE_OHTR:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_REV_ONHOOK_TX );
         pDev->previousSlicMode = XDRV_SLIC_MODE_OHTR;
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
        if( !pDev->ringBurstBlockFlag )
        {
           ProSLIC_RingStart(pDev->pProSlicChan); 
           pDev->previousSlicMode = XDRV_SLIC_MODE_RING;
        }
      }
      break;

      /* Loop current feed */
      case XDRV_SLIC_MODE_LCF:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_FWD_ACTIVE );
         pDev->previousSlicMode = XDRV_SLIC_MODE_LCF;
      }
      break;

      /* Reverse loop current feed */
      case XDRV_SLIC_MODE_RLCF:
      {
         ProSLIC_SetLinefeedStatus (pDev->pProSlicChan, SILABS_STATE_REV_ACTIVE );
         pDev->previousSlicMode = XDRV_SLIC_MODE_RLCF;
      }
      break;

      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC," XDRV_SLIC: ERROR - unrecognized SLIC mode 0x%02x", mode));
      }
      break;
   }
   bosMutexRelease( &pDev->silabsEquCriMutex );
}

/* Unused functions */
void slicSiLabsLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;
}

void slicSiLabsPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{

  SI_SLIC_CHANNEL_INFO* pDev = (SI_SLIC_CHANNEL_INFO* )pDrv;

  bosMutexAcquire( &pDev->silabsPolRevMutex );
  ProSLIC_PolRev (pDev->pProSlicChan,0,(XDRV_UINT8)value);
  if( value == POLREV_STOP || value == POLREV_START )
  {
     pDev->phaseReversalActive = value == POLREV_START ? true : false ;
  }
  bosMutexRelease( &pDev->silabsPolRevMutex );

};

int slicSiLabsSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

int slicSiLabsGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

int slicSiLabsSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

int slicSiLabsSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

XDRV_BOOL slicSiLabsGetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}


/************************
 * SLIC DEBUG FUNCTIONS *
 ************************/
#define WriteDirect       pDev->pProSlicChan->deviceId->ctrlInterface->WriteRegister_fptr
#define ReadDirect        pDev->pProSlicChan->deviceId->ctrlInterface->ReadRegister_fptr
#define ReadIndirect      pDev->pProSlicChan->deviceId->ctrlInterface->ReadRAM_fptr
#define WriteIndirect     pDev->pProSlicChan->deviceId->ctrlInterface->WriteRAM_fptr
#define pHW               pDev->pProSlicChan->deviceId->ctrlInterface->hCtrl

/* Note: regSize is not applicable for SiLabs parts.  Either 1 byte for direct or 4 bytes for indirect ram registers. */
XDRV_UINT32 slicSiLabsProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                             void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set )
{
   SI_SLIC_CHANNEL_INFO *pDev = (SI_SLIC_CHANNEL_INFO *)pDrv;
   XDRV_UINT8           *bufferPtr;
   XDRV_UINT32          status = 0;
   XDRV_UINT8           i;
   
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "WARNING: Silicon Labs DEBUG USAGE ONLY!"));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "ProSLIC channel: %d", pDev->pProSlicChan->channel));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "ProSLIC channeltype: %d", pDev->pProSlicChan->channelType));
   
   bufferPtr = (XDRV_UINT8 *)dataPtr;
   
   if( set == 0 ) /* get */
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "deviceId %d", ((SI_SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "chan: %d", pDev->pProSlicChan->channel));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "reg: 0x%02x  (decimal: %lu)", (unsigned int)reg, reg));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "regLen: %u", (unsigned int)regSize));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "dataPtr: 0x%08lx", (XDRV_UINT32)bufferPtr));      
      
      if( indirect == 0 ) /* direct */
      {
         *bufferPtr = ReadDirect( pHW, pDev->pProSlicChan->channel, reg );
         /* SiLabs direct registers are only 8-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER GET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         *bufferPtr = ReadIndirect( pHW, pDev->pProSlicChan->channel, reg );
         /* SiLabs indirect RAM registers are 32-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "(RAM) REGISTER GET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }   
      }   
   }
   else /* set */
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "deviceId %d", ((SI_SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "chan: %d", pDev->pProSlicChan->channel));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "reg: 0x%02x  (decimal: %lu)", (unsigned int)reg, reg));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "regLen: %u", (unsigned int)regSize));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "dataPtr: 0x%08lx", (XDRV_UINT32)bufferPtr));      
      
      if( indirect == 0 ) /* direct */
      {
         WriteDirect( pHW, pDev->pProSlicChan->channel, reg, *bufferPtr );
         /* SiLabs direct registers are only 8-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER SET BYTE %i: 0x%02x", i, bufferPtr[i]));
         } 
      }
      else if( indirect == 1 )
      {
         WriteIndirect( pHW, pDev->pProSlicChan->channel, reg, *bufferPtr );
         /* SiLabs indirect RAM registers are 32-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "(RAM) REGISTER SET BYTE %i: 0x%02x", i, bufferPtr[i]));
         } 
      }
   }
   
   return( status );
}

/*******************/
/* XDRV_DAA_FUNCS */
/*******************/
/*
*****************************************************************************
** FUNCTION:   daaSiLabsIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL daaSiLabsIsOffhook( XDRV_DAA *pDrv )
{
   SI_DAA_CHANNEL_INFO *pDev = (SI_DAA_CHANNEL_INFO *)pDrv;
   int                  vdaaRet;
   
   (void) pDrv;

   /* New way in 5.2.0+ */
   vdaaRet = Vdaa_GetHookStatus( pDev->pDaaProSlicChan );
   if( vdaaRet == VDAA_ONHOOK || vdaaRet == VDAA_ONHOOK_MONITOR )
   {
      return XDRV_FALSE;
   }
   
   if( vdaaRet == VDAA_OFFHOOK )
   {
      return XDRV_TRUE;
   }
  
   /* should not get here. if we do print the vadd value and return XDRV_FALSE */
   XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA_ERR: vdaaRet value = %d", vdaaRet));
   return ( XDRV_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   daaSiLabsProcessEvents
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
void daaSiLabsProcessEvents( XDRV_DAA *pDrv )
{
   (void) pDrv;
   return;
}

/*
*****************************************************************************
** FUNCTION:   daaSiLabsIsRingActive
**
** PURPOSE:    Determine if a channel's ring signal is active
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if ring active, XDRV_FALSE otherwise
**
**
*****************************************************************************
*/
XDRV_BOOL daaSiLabsIsRingActive( XDRV_DAA *pDrv )
{
   SI_DAA_CHANNEL_INFO *pDev = (SI_DAA_CHANNEL_INFO *)pDrv;
   vdaaRingDetectStatusType  ringStatus;
   
   (void) pDrv;

   /* Using 2nd ring detection method, we need to monitor rdt, rdtp, rdtn  */
   /* Note: When using ring validation enabled mode, rdt is no longer a one-shot timer */
  
   /* 5.2.0 Method */ 
   Vdaa_ReadRingDetectStatus( pDev->pDaaProSlicChan, &ringStatus );
   
   if( ringStatus.ringDetected && (ringStatus.ringDetectedPos || ringStatus.ringDetectedNeg) )
   {
      return( XDRV_TRUE );
   }
   return( XDRV_FALSE );
   
}
/*
*****************************************************************************
** FUNCTION:   daaSiLabsGetLoopCurrent
**
** PURPOSE:    Determine the state of a channel's loop current
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_DAA_LOOP_OPEN if there is no loop current detected
**             XDRV_DAA_LOOP_CLOSED if loop current is detected
**
** NOTE:       No polarity reversal detection is implemented
**
*****************************************************************************
*/
XDRV_DAA_LOOP daaSiLabsGetLoopCurrent( XDRV_DAA *pDrv )
{
   SI_DAA_CHANNEL_INFO *pDev = (SI_DAA_CHANNEL_INFO *)pDrv;
   char            vloop;
   short int       iloop;
   int                   vdaaRet;
   
   (void) pDrv;

   vdaaRet = Vdaa_ReadLinefeedStatus( pDev->pDaaProSlicChan, &vloop, &iloop ); 
   
   if ( iloop < (SIDAAOFFHLCMIN*SIDAA_LCS_SCALE_NUM/SIDAA_LCS_SCALE_DEN) )
   {   
      return XDRV_DAA_LOOP_OPEN;
   }
   else
   {  
      if( vdaaRet == RC_VDAA_ILOOP_OVLD )
      {
         /* loop current > 127 mA--overload may exist */
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"DAA_ERR: loop current > 127 mA--overload may exist"));
      }
      return XDRV_DAA_LOOP_CLOSED;
   }  
}

/*
*****************************************************************************
** FUNCTION:   daaSiLabsModeControl
**
** PURPOSE:    Sets the DAA into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the DAA driver device structure
**             mode  - the mode to set the DAA into.
**
** RETURNS:    none
**
*****************************************************************************
*/
void daaSiLabsModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode )
{
   SI_DAA_CHANNEL_INFO *pDev = (SI_DAA_CHANNEL_INFO *)pDrv;
   XDRV_UINT8 hookstatus = VDAA_ONHOOK;

   switch( mode )
   {
      /* Put device into on-hook - release line */
      case XDRV_DAA_MODE_ONHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA: Going OnHook"));
         hookstatus = VDAA_ONHOOK;
      }
      /*Pass through so that it will be ready to detect CLID RX for next incoming call */
   
      /*  Caller ID receive mode, enable onhook data reception  */
      case XDRV_DAA_MODE_ONHOOK_DATA:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA: Enable on-hook Caller ID receive. "));
         hookstatus = VDAA_ONHOOK_MONITOR;     
      }
      break;

      /* Put device into off-hook - seize line */
      case XDRV_DAA_MODE_OFFHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"DAA: Going OffHook"));
         hookstatus = VDAA_OFFHOOK;
      }
      break;
  
      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"DAA: unrecognized DAA mode"));
      }
      break;
   }

   Vdaa_SetHookStatus( pDev->pDaaProSlicChan, hookstatus );
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"%s: Mode for FXO channel changed to %d ", LOG_MODULE_NAME, mode ));
}

/* Unused functions */
void daaSiLabsLedControl( XDRV_DAA *pDrv, int value )
{
    (void) pDrv;
    (void) value;
    /* Currently not applicable */
}


/***********************
 * DAA DEBUG FUNCTIONS *
 ***********************/
#define DaaWriteDirect   pDev->pDaaProSlicChan->deviceId->ctrlInterface->WriteRegister_fptr
#define DaaReadDirect    pDev->pDaaProSlicChan->deviceId->ctrlInterface->ReadRegister_fptr
#define pDaaHW           pDev->pDaaProSlicChan->deviceId->ctrlInterface->hCtrl

/* Note: regSize is not applicable for SiLabs parts.  Either 1 byte for direct or 4 bytes for indirect ram registers. */
XDRV_UINT32 daaSiLabsProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                            void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set )
{
   SI_DAA_CHANNEL_INFO *pDev = (SI_DAA_CHANNEL_INFO *)pDrv;
   XDRV_UINT8          *bufferPtr;
   XDRV_UINT32         status = 0;
   XDRV_UINT8          i;
   
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "WARNING: Silicon Labs DEBUG USAGE ONLY!"));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "ProSLIC channel: %d", pDev->pDaaProSlicChan->channel));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "ProSLIC channeltype: %d", pDev->pDaaProSlicChan->channelType));
   
   bufferPtr = (XDRV_UINT8 *)dataPtr;
   
   if( set == 0 ) /* get */
   {
      if( indirect == 0 ) /* direct */
      {
         *bufferPtr = DaaReadDirect( pDaaHW, pDev->pDaaProSlicChan->channel, reg );
         /* SiLabs direct registers are only 8-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER GET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on DAA device."));
         status = 1;   
      }   
   }
   else /* set */
   {
      if( indirect == 0 ) /* direct */
      {
         DaaWriteDirect( pDaaHW, pDev->pDaaProSlicChan->channel, reg, *bufferPtr );
         /* SiLabs direct registers are only 8-bits wide */
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER SET BYTE %i: 0x%02x", i, bufferPtr[i]));
         } 
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on DAA device."));
         status = 1; 
      }
   }

   return( status );
}
/*
*****************************************************************************
** FUNCTION:   slicSiLabsGetSlicParms
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
int slicSiLabsGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode )
{
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsReadRtpStatus
**
** PURPOSE:    This function gets RTP bit, and take action once RTP bit set
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicSilabsReadRtpStatus( XDRV_SLIC *pDrv )
{
   XDRV_UINT8 rtpDetected;
   SI_SLIC_CHANNEL_INFO* pDev = (SI_SLIC_CHANNEL_INFO*)pDrv;

   if( pDev->ringBurstBlockFlag )
   {
     return;
   }

   rtpDetected = PROSLIC_LCRRTP_CLEAR;
   if( ProSLIC_ReadRTPStatus(pDev->pProSlicChan,&rtpDetected) == RC_COMPLETE_NO_ERR ) /* Get RTP bit status */
   {
     if( rtpDetected )
     {
       pDev->ringBurstBlockFlag = true;
       bosMutexAcquire( &pDev->silabsPolRevMutex );
       /* set to LCF/RLCF mode */
       slicSiLabsModeControl( pDrv, pDev->phaseReversalActive == false ? XDRV_SLIC_MODE_LCF : XDRV_SLIC_MODE_RLCF );
       bosMutexRelease( &pDev->silabsPolRevMutex );
       mod_timer(&pDev->ringtripTimer,jiffies + SILABS_RINGBLOCK_DELAY);
       if( ProSLIC_ClearRTPStatus ( pDev->pProSlicChan ) == RC_IGNORE ) /* clear RTP bit */
       {
          XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "Clear_RTP_ERR: Clear RTP bit fails!"));
       }
     }
   }
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsMutexInit
**
** PURPOSE:    This function inits mutex required
**
** PARAMETERS: chanInfoP  - pointer to channel info.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicSilabsMutexInit( SI_SLIC_CHANNEL_INFO *chanInfoP )
{
   bosMutexCreate( "SI-POL-MUTEX", &chanInfoP->silabsPolRevMutex );
   bosMutexCreate( "SI-EQUCRI-MUTEX", &chanInfoP->silabsEquCriMutex );
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsMutexDeinit
**
** PURPOSE:    This function destroy mutex initialized
**
** PARAMETERS: chanInfoP  - pointer to channel info.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicSilabsMutexDeinit( SI_SLIC_CHANNEL_INFO *chanInfoP )
{
   if( chanInfoP->silabsPolRevMutex.mutex != NULL )
   {
      bosMutexDestroy( &chanInfoP->silabsPolRevMutex );
   }
   
   if( chanInfoP->silabsEquCriMutex.mutex != NULL )
   {
      bosMutexDestroy( &chanInfoP->silabsEquCriMutex );
   }
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsRingUnblock
**
** PURPOSE:    timer function
**
** PARAMETERS: datap  - pointer to channel info.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicSilabsRingUnblock( unsigned long datap )
{
   ((SI_SLIC_CHANNEL_INFO *)datap)->ringBurstBlockFlag = false;
}

/*
*****************************************************************************
** FUNCTION:   slicSilabsWaitLineSettle
**
** PURPOSE:    wait until send mode change allowed
**
** PARAMETERS: pDev  - pointer to channel info.
**
** RETURNS:    none
**
*****************************************************************************
*/
static void slicSilabsWaitLineSettle(SI_SLIC_CHANNEL_INFO* pDev)
{
   int LfRegId,iCnt;
   unsigned int Linefeed;

   LfRegId = ProSLIC_GetLfRegId(pDev->pProSlicChan);
   if( LfRegId >= 0 )
   {
     Linefeed = ProSLIC_ReadReg( pDev->pProSlicChan,(uInt8)LfRegId );  /* Get LINEFEED */
     iCnt=0;
     /* wait to send command until LINEFEED reg bits [2:0]==[6:4] or timeout*/
     while(( Linefeed & LF_MASK ) != ( (Linefeed >> LFS_BIT_OFFSET) & LF_MASK ))
     {
        bosSleep(5);
        Linefeed = ProSLIC_ReadReg( pDev->pProSlicChan,(uInt8)LfRegId );
        if( ++iCnt > 21 ) /* 105ms time out, 20HZ ring needs max 100ms to settle */
        {
          break;    
        }
     };
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s LineFeed Reg ID(%d) is invalid.", LOG_MODULE_NAME, LOG_DEV_NAME, LfRegId ));
   }
}