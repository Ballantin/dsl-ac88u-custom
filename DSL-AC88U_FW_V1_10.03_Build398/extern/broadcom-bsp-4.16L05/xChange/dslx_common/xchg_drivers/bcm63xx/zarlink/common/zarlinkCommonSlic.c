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
*      This is common code for all Zarlink (Legerity) SLIC devices, that uses API-2
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

#include "zarlinkCommonSlic.h"
#include "zarlinkCommon.h"
#include "zarlinkCommonDaa.h"


#define LOG_MODULE_NAME       "SLIC"
#define LOG_DEV_NAME          "Zarlink"


#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##_Profile_Dlp,
static const short int gFXSProfilesDlp[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##_Profile_Tx_Gain,
static const short int gFXSProfilesTxGain[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##Profile,
static const VpProfileDataType *gFXSProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##Profile_RF14,
static const VpProfileDataType *gFXSProfilesRF14[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_WB##country##Profile,
static const VpProfileDataType *gFXSWBProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_WB##country##Profile_RF14,
static const VpProfileDataType *gFXSWBProfilesRF14[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile,
static const VpProfileDataType *gFXSRingProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile_886,
static const VpProfileDataType *gFXSRingProfiles_886[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile_887,
static const VpProfileDataType *gFXSRingProfiles_887[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

/* Create a static array of each country's WinSLAC parameters */
#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##Profile_890,
static const VpProfileDataType *gFXSProfiles_890[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_WB##country##Profile_890,
static const VpProfileDataType *gFXSWBProfiles_890[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile_890,
static const VpProfileDataType *gFXSRingProfiles_890[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef  COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXS_##country##RingProfile_890HV,
static const VpProfileDataType *gFXSRingProfiles_890HV[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

/****************/
/* FXO Profiles */
/****************/
#undef COUNTRY_ARCHIVE_MAKE_NAME
/* Create a static array of each country's WinSLAC parameters */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_##country##Profile,
static const VpProfileDataType *gFXOProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_DIALING_##country##_Profile,
static const VpProfileDataType *gFXODialProfiles[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef COUNTRY_ARCHIVE_MAKE_NAME
/* Create a static array of each country's WinSLAC parameters */
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_##country##Profile_890,
static const VpProfileDataType *gFXOProfiles_890[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};

#undef COUNTRY_ARCHIVE_MAKE_NAME
#define COUNTRY_ARCHIVE_MAKE_NAME( country ) FXO_DIALING_##country##_Profile_890,
static const VpProfileDataType *gFXODialProfiles_890[VRG_COUNTRY_MAX] =
{
   #include <countryArchive.h>
};


/* Helpers */
static VpOptionCodecType getCodecType( XDRV_UINT8 pcmMode );
static VpProfileDataType *slicGetLocale( VpDeviceType deviceType, VRG_COUNTRY country, VpOptionCodecType codec, XDRV_UINT8 rProt );
static VpProfileDataType *slicGetRingLocale( VpDeviceType deviceType, VpDevCtxType* pDevCtx, VRG_COUNTRY country );
static VpProfileDataType *daaGetLocale( VpDeviceType deviceType, VRG_COUNTRY country );
static VpProfileDataType *daaGetDialLocale( VpDeviceType deviceType, VRG_COUNTRY country );
VpLineStateType VpGetReverseState(VpLineStateType currentState);
/*
*****************************************************************************
** FUNCTION:   zarlinkWaitForEvent
**
** PURPOSE:    Waiting for specific event category and event Id on device.
**             USED ONLY DURING DEVICE INITIALIZATION!!!
**
** PARAMETERS: deviceNum - Device number of SLIC
**             category  - Event Category to wait for
**             event     - Event Id to wait for
**
** RETURNS:    TRUE  - Event occurred.
**             FALSE - Event did not occur.
**
*****************************************************************************
*/
XDRV_BOOL zarlinkWaitForEvent( VpDevCtxType* pZDevCtx, VpEventCategoryType category, XDRV_UINT16 event)
{
   XDRV_UINT16 i;
   VpEventCategoryType category_save=0;
   XDRV_UINT16 eventid_save=0;

   for (i = 0; i < SLIC_INIT_TIMEOUT; i++)
   {
      zarlinkProcessEvents( pZDevCtx, &category_save, &eventid_save, 0 /* force to always 0 for init */ );

      if ( (category_save == category) && (eventid_save == event) )
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Found event: pEvent.eventCategory = %d pEvent.eventId = %d i=%d", category_save, eventid_save, i));
         return ( XDRV_TRUE );
      }
      else
      {
         /* Debugging purposes */
         //XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Unexpected response event: pEvent.eventCategory = %d pEvent.eventId = %d i=%d", category_save, eventid_save, i));
      }

      bosSleep(10);
   } /* for */

   /* Could not find any events if we reach here */
   return ( XDRV_FALSE );
}

/*
*****************************************************************************
** FUNCTION:   zarlinkLineModeControl
**
** PURPOSE:    Sets the SLAC into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLAC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
void zarlinkFxsLineModeControl( VpLineCtxType *pZLineCtx, XDRV_SLIC_MODE mode )
{
   VpLineStateType lineState;

   switch ( mode )
   {
      case XDRV_SLIC_MODE_LCFO:
      {
         lineState = VP_LINE_DISCONNECT;
      }
      break;

      /* Standby mode */
      case XDRV_SLIC_MODE_STANDBY:
      {
         lineState = VP_LINE_STANDBY;
      }
      break;

      /* On-hook transmission */
      case XDRV_SLIC_MODE_OHT:
      {
         /* Active mode supports OHT */
         lineState = VP_LINE_OHT;
      }
      break;

      /* On-hook transmission reverse */
      case XDRV_SLIC_MODE_OHTR:
      {
         /* Active mode supports OHT AND reverse polarity */
         lineState = VP_LINE_OHT_POLREV;
      }
      break;

      /* Ringing */
      case XDRV_SLIC_MODE_RING:
      {
         lineState = VP_LINE_RINGING;
      }
      break;

      /* Loop current feed */
      case XDRV_SLIC_MODE_LCF:
      {
         lineState = VP_LINE_TALK;
      }
      break;

      /* Reverse loop current feed */
      case XDRV_SLIC_MODE_RLCF:
      {
         lineState = VP_LINE_TALK_POLREV;
      }
      break;

      default:
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, " ERROR: unrecognized SLIC mode (%d)", mode));
         return;
      }
      break;
   }

   VpSetLineState( pZLineCtx, lineState );
}

/*
*****************************************************************************
** FUNCTION:   zarlinkFxsLineIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS:
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL zarlinkFxsLineIsOffhook( VpLineCtxType *pZLineCtx )
{
   bool lineStatus = FALSE;

   if ( VpGetLineStatus( pZLineCtx, VP_INPUT_HOOK, &lineStatus ) == VP_STATUS_SUCCESS )
   {
      if ( lineStatus == TRUE )
      {
         return (XDRV_TRUE);
      }
      else
      {
         return (XDRV_FALSE);
      }
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR %s %d", __FUNCTION__, __LINE__));
   }

   return (XDRV_FALSE);
}


/*
*****************************************************************************
** FUNCTION:   zarlinkProcessFxsEvents
**
** PURPOSE:    Call VpApiTick and process events.
**
** PARAMETERS: pDevCtx    - Device Context
**             deviceChan - Channel number within device itself
**
** RETURNS:    none
**
*****************************************************************************
*/
void zarlinkProcessFxsEvents( VpDevCtxType* pZDevCtx, XDRV_UINT32 deviceChan )
{
   VpEventCategoryType category_save=0;
   XDRV_UINT16 eventid_save=0;

   zarlinkProcessEvents( pZDevCtx, &category_save, &eventid_save, deviceChan );
}


/*****************************************************************************
*
* FUNCTION:   initFxsLine
*
* PURPOSE:    Initialize lines of Zarlink device with dual lines.
*
* PARAMETERS: Zarlink Config parameter from SLAC Device Driver.
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/
XDRV_SLIC_STATUS initZarlinkLines( ZARLINK_CONFIG* zarlinkConfig )
{
   VpStatusType            status;
   XDRV_UINT8              deviceId = zarlinkConfig->pDev->deviceId;
   XDRV_UINT8              i;
   VpOptionTimeslotType    timeslot[zarlinkConfig->numFxsPerDevice];
   VpOptionTimeslotType    daaTimeslot[zarlinkConfig->numFxoPerDevice];
   XDRV_UINT8              channelId[zarlinkConfig->numFxsPerDevice];
   XDRV_UINT8              daaChannelId[zarlinkConfig->numFxoPerDevice];
   VpOptionCodecType       codecType[zarlinkConfig->numFxsPerDevice];
   VpOptionCodecType       daaCodecType[zarlinkConfig->numFxoPerDevice];
   VpOptionAbsGainType     fxsGainType[zarlinkConfig->numFxsPerDevice];
   VpOptionCriticalFltType criticalFlt; /* Device option */
   XDRV_UINT8              numChansPerDevice;

   VpOptionEventMaskType eventMask;
   VpMemSet(&eventMask, 0xFF, sizeof(VpOptionEventMaskType));
   /* enable desired events of the API here */
   eventMask.faults = VP_EVENT_MASK_ALL;
   eventMask.response = ~(VP_EVID_CAL_CMP);
   eventMask.signaling = ~(VP_LINE_EVID_HOOK_OFF | VP_LINE_EVID_HOOK_ON |
   VP_LINE_EVID_PULSE_DIG | VP_LINE_EVID_STARTPULSE | VP_LINE_EVID_FLASH);
   eventMask.test = ~(VP_LINE_EVID_TEST_CMP);
   eventMask.process = VP_EVENT_MASK_ALL;
   eventMask.fxo = VP_EVCAT_FXO_UNMASK_ALL;
   eventMask.packet = VP_EVENT_MASK_ALL;

   /* Total number of channels per device */
   numChansPerDevice = zarlinkConfig->numFxsPerDevice + zarlinkConfig->numFxoPerDevice;

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: %s Locale set to %d", LOG_MODULE_NAME, LOG_DEV_NAME, zarlinkConfig->pDev->locale ));

   for( i = 0; i < zarlinkConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = zarlinkConfig->pDev->slicChannel[i].chan;
      codecType[i] = getCodecType( zarlinkConfig->pDev->slicChannel[i].pcmMode );
      timeslot[i].tx = zarlinkConfig->pDev->slicChannel[i].txTimeSlot;
      timeslot[i].rx = zarlinkConfig->pDev->slicChannel[i].rxTimeSlot;

      /* Initialize FXS line */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s FXS line %d", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));
	  
      /* Initialize the line with proper profile settings */
      status = VpInitLine( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                           slicGetLocale(zarlinkConfig->deviceType, zarlinkConfig->pDev->locale, codecType[i], zarlinkConfig->rProt ),
                           zarlinkConfig->pDcProfile,
                           zarlinkConfig->pRingProfile == NULL ? slicGetRingLocale(zarlinkConfig->deviceType, &(zarlinkConfig->pDevCtx[deviceId]), zarlinkConfig->pDev->locale ) : zarlinkConfig->pRingProfile);
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpInitLine failed (%d) ", LOG_MODULE_NAME, status ));
         return ( XDRV_SLIC_STATUS_ERROR );
      }

      /* Check if VP_LINE_EVID_LINE_INIT_CMP occurred */
      if( TRUE != zarlinkWaitForEvent( &(zarlinkConfig->pDevCtx[deviceId]), VP_EVCAT_RESPONSE, VP_LINE_EVID_LINE_INIT_CMP ) )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"SLIC: ERROR: Line initialization was not completed"));
         return( XDRV_SLIC_STATUS_ERROR );
      }

      /* Set the initial line state */
      status = VpSetLineState( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                               VP_LINE_STANDBY );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetLineState failed (%d) ", LOG_MODULE_NAME, status ));
         return ( XDRV_SLIC_STATUS_ERROR );
      }

      status = VpSetOption(VP_NULL, &(zarlinkConfig->pDevCtx[deviceId]), VP_OPTION_ID_EVENT_MASK, &eventMask);
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption failed (%d) ", LOG_MODULE_NAME, status ));
         return ( XDRV_SLIC_STATUS_ERROR );
      }

      /* Calibrate the line */
      status = VpCalLine(&(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]));
      if ( (status != VP_STATUS_SUCCESS) && (status != VP_STATUS_FUNC_NOT_SUPPORTED) )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC ERROR: %s %d status = %d ", __FUNCTION__, __LINE__, status ));
         return( XDRV_SLIC_STATUS_ERROR );
      }
      else if ( status == VP_STATUS_SUCCESS )
      {
         /* Check if VP_EVID_CAL_CMP occurred */
         if( TRUE != zarlinkWaitForEvent( &(zarlinkConfig->pDevCtx[deviceId]), VP_EVCAT_RESPONSE, VP_EVID_CAL_CMP ) )
         {
            XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC: ERROR: Line calibration was not completed. deviceId=%d",(int)deviceId));
            return( XDRV_SLIC_STATUS_ERROR );
         }
      }

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: FXS line %d RX timeslot = %d ", LOG_MODULE_NAME, channelId[i], timeslot[i].rx ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: FXS line %d TX timeslot = %d ", LOG_MODULE_NAME, channelId[i], timeslot[i].tx ));
      /* Don't give device context (just give NULL) if you are configuring any parameters for a specific line */
      status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                            VP_NULL,
                            VP_OPTION_ID_TIMESLOT,
                            (void*)&(timeslot[i]) );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption failed (%d) ", LOG_MODULE_NAME, status  ));
      }

      /* First time always try whatever codec type is set.
      ** If we get an invalid argument error we try linear */
      status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                            VP_NULL,
                            VP_OPTION_ID_CODEC,
                            (void*)&(codecType[i]) );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for codec type failed. status = %d ", LOG_MODULE_NAME, status ));
         if ( status == VP_STATUS_INVALID_ARG )
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: Defaulting to Linear codec. ", LOG_MODULE_NAME ));
            codecType[i] = VP_OPTION_LINEAR;
            status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                                  VP_NULL,
                                  VP_OPTION_ID_CODEC,
                                  (void*)&(codecType[i]) );
            if ( status != VP_STATUS_SUCCESS )
            {
               XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,"%s: VpSetOption for codec type failed. status = %d ", LOG_MODULE_NAME, status ));
            }
         }
      }

      /* Configures FXS gain */
      /* Multiply by 10 because Zarlink defines 10 as 1dB */
      fxsGainType[i].gain_DToA = gFXSProfilesDlp[zarlinkConfig->pDev->locale]*10; /* D-to-A Rx (w.r.t SLIC) */
      fxsGainType[i].gain_AToD = gFXSProfilesTxGain[zarlinkConfig->pDev->locale]*10; /* A-to-D Tx (w.r.t SLIC) */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "FXS channel %d Rx Gain: %d", channelId[i], fxsGainType[i].gain_DToA/10 ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "FXS channel %d Tx Gain: %d", channelId[i], fxsGainType[i].gain_AToD/10 ));

      status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                            VP_NULL,
                            VP_OPTION_ID_ABS_GAIN,
                            (void*)&(fxsGainType[i]) );

      if (status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for FXS gain failed. status = %d ", LOG_MODULE_NAME, status ));
      }

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s FXS line %d initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));
   } /* for */


   for( i = 0; i < zarlinkConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = zarlinkConfig->pDev->daaChannel[i].chan;
      daaCodecType[i] = getCodecType( zarlinkConfig->pDev->daaChannel[i].pcmMode );
      daaTimeslot[i].tx = zarlinkConfig->pDev->daaChannel[i].txTimeSlot;
      daaTimeslot[i].rx = zarlinkConfig->pDev->daaChannel[i].rxTimeSlot;

      /* Initialize FXO line */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s FXO line", LOG_MODULE_NAME, LOG_DEV_NAME ));

      status = VpInitLine( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                           daaGetLocale(zarlinkConfig->deviceType, zarlinkConfig->pDev->locale),
                           daaGetDialLocale(zarlinkConfig->deviceType, zarlinkConfig->pDev->locale),
                           VP_PTABLE_NULL );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpInitLine failed (%d) ", LOG_MODULE_NAME, status ));
      }

      /* Check if VP_LINE_EVID_LINE_INIT_CMP occurred */
      if( TRUE != zarlinkWaitForEvent( &(zarlinkConfig->pDevCtx[deviceId]), VP_EVCAT_RESPONSE, VP_LINE_EVID_LINE_INIT_CMP ) )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA: ERROR: Line initialization was not completed"));
         return( XDRV_SLIC_STATUS_ERROR );
      }

      status = VpSetLineState( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                               VP_LINE_FXO_LOOP_OPEN );
      if ( status != VP_STATUS_SUCCESS )
      {
       XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLICDAA: VpSetLineState failed (%d) ", status ));
      }

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: FXO line RX timeslot = %d ", LOG_MODULE_NAME, daaTimeslot[i].tx ));
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: FXO line TX timeslot = %d ", LOG_MODULE_NAME, daaTimeslot[i].rx));

      status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                            VP_NULL,
                            VP_OPTION_ID_TIMESLOT,
                            (void*)&(daaTimeslot[i]) );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for timeslot failed (%d) ", LOG_MODULE_NAME, status ));
      }

      status = VpSetOption( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                            VP_NULL,
                            VP_OPTION_ID_CODEC,
                            (void*)&(daaCodecType[i]) );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for codec type failed (%d) ", LOG_MODULE_NAME, status ));
      }

      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s FXO line initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME ));

   }

   /* Disable Thermal Fault Auto-Disconnect at Device Level */
   criticalFlt.acFltDiscEn    = FALSE;
   criticalFlt.dcFltDiscEn    = FALSE;
   criticalFlt.thermFltDiscEn = FALSE;
   status = VpSetOption( VP_NULL,
                         &(zarlinkConfig->pDevCtx[deviceId]),
                         VP_DEVICE_OPTION_ID_CRITICAL_FLT,
                         &criticalFlt );
   if ( status != VP_STATUS_SUCCESS )
   {
      if( status == VP_STATUS_OPTION_NOT_SUPPORTED )
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for criticalFlt not supported (%d) ", LOG_MODULE_NAME, status ));
      }
      else
      {
        XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetOption for criticalFlt failed (%d) ", LOG_MODULE_NAME, status ));
      }
   }

   return XDRV_SLIC_STATUS_SUCESS;
}

/*****************************************************************************
*
* FUNCTION:   initZarlinkDevice
*
* PURPOSE:    Initialize Zarlink SLIC/SLAC device.
*
* PARAMETERS: Lots!
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/
/* Device initialization */
XDRV_SLIC_STATUS initZarlinkDevice( ZARLINK_CONFIG* zarlinkConfig )
{
   VpStatusType   status;
   XDRV_UINT8     deviceId = zarlinkConfig->pDev->deviceId;
   XDRV_UINT8     i;
   XDRV_UINT8     channelId[zarlinkConfig->numFxsPerDevice];
   XDRV_UINT8     daaChannelId[zarlinkConfig->numFxoPerDevice];
   XDRV_UINT8     numChansPerDevice;
   void*          pDevObject = NULL;
   void*          pLineObject = NULL;


   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Initializing %s device %d", LOG_MODULE_NAME, LOG_DEV_NAME, zarlinkConfig->pDev->deviceId ));

   if( zarlinkConfig->deviceType == VP_DEV_880_SERIES )
   {
      pDevObject = &(((Vp880DeviceObjectType *)(zarlinkConfig->pDevObj))[deviceId]);
   }
   else if( zarlinkConfig->deviceType == VP_DEV_886_SERIES )
   {
      pDevObject = &(((Vp886DeviceObjectType *)(zarlinkConfig->pDevObj))[deviceId]);
   }
   else if( zarlinkConfig->deviceType == VP_DEV_887_SERIES )
   {
      pDevObject = &(((Vp886DeviceObjectType *)(zarlinkConfig->pDevObj))[deviceId]);
   }
   else if( zarlinkConfig->deviceType == VP_DEV_890_SERIES )
   {
      pDevObject = &(((Vp890DeviceObjectType *)(zarlinkConfig->pDevObj))[deviceId]);
   }

   /* Total number of channels per device */
   numChansPerDevice = zarlinkConfig->numFxsPerDevice + zarlinkConfig->numFxoPerDevice;

   /* Create the API-2 device */
   status = VpMakeDeviceObject(  zarlinkConfig->deviceType,
                                 zarlinkConfig->pDev->mspiDevId,
                                 &(zarlinkConfig->pDevCtx[deviceId]),
                                 pDevObject
                              );
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC,  "%s: Error during VpMakeDeviceObject (status %d) ", LOG_MODULE_NAME, status ));
      return ( XDRV_SLIC_STATUS_ERROR );
   }

   /* Save pDevCtx address so that we can access pDevCtx from original driver file later on. */
   zarlinkConfig->pDev->pDeviceCtx = &(zarlinkConfig->pDevCtx[deviceId]);

   for( i = 0; i < zarlinkConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = zarlinkConfig->pDev->slicChannel[i].chan;

      if( zarlinkConfig->deviceType == VP_DEV_880_SERIES )
      {
         pLineObject = &(((Vp880LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_886_SERIES )
      {
         pLineObject = &(((Vp886LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_887_SERIES )
      {
         pLineObject = &(((Vp886LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_890_SERIES )
      {
         pLineObject = &(((Vp890LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]);
      }

      /* Rare, but when termination type is not the same on every line */
      if( ( zarlinkConfig->fxsTermTypeSecondValid == 1 ) &&
          ( zarlinkConfig->fxsTermTypeSecond != zarlinkConfig->fxsTermType ) &&
          ( channelId[i] == 1 ) /* phys chan */ )
      {
         status = VpMakeLineObject( zarlinkConfig->fxsTermTypeSecond,
                                    channelId[i],   /* Physical Channel */
                                    &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                                    pLineObject,
                                    &(zarlinkConfig->pDevCtx[deviceId])
                                  );
      }
      else /* Normal Case here */
      {
         status = VpMakeLineObject( zarlinkConfig->fxsTermType,
                                    channelId[i],   /* Physical Channel */
                                    &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                                    pLineObject,
                                    &(zarlinkConfig->pDevCtx[deviceId])
                                  );
      }
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: Error during VpMakeLineObject %d (status %d)", LOG_MODULE_NAME, i, status));
         return ( XDRV_SLIC_STATUS_ERROR );
      }

      /* Save pLineCtx address so that we can access pLineCtx from original driver file later on. */
      zarlinkConfig->pDev->slicChannel[i].pChanLineCtx = &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]);

      /* Store line ID to make future event handling easier */
      VpMapLineId( zarlinkConfig->pDev->slicChannel[i].pChanLineCtx, SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i]) );                    
   }

   for( i = 0; i < zarlinkConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = zarlinkConfig->pDev->daaChannel[i].chan;

      if( zarlinkConfig->deviceType == VP_DEV_880_SERIES )
      {
         pLineObject = &(((Vp880LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_886_SERIES )
      {
         pLineObject = &(((Vp886LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_887_SERIES )
      {
         pLineObject = &(((Vp886LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]);
      }
      else if( zarlinkConfig->deviceType == VP_DEV_890_SERIES )
      {
         pLineObject = &(((Vp890LineObjectType *)(zarlinkConfig->pLineObj))[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]);
      }

      status = VpMakeLineObject( zarlinkConfig->fxoTermType,
                                 daaChannelId[i],   /* Physical Channel */
                                 &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                                 pLineObject,
                                 &(zarlinkConfig->pDevCtx[deviceId])
                               );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: Error during VpMakeLineObject %d (status %d)", LOG_MODULE_NAME, i, status));
         return ( XDRV_SLIC_STATUS_ERROR );
      }

      /* Save pLineCtx address so that we can access pLineCtx from original driver file later on. */
      zarlinkConfig->pDev->daaChannel[i].pDaaChanLineCtx = &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]);
   }

   /* Initialize the device */
   status = VpInitDevice(  &(zarlinkConfig->pDevCtx[deviceId]),
                           zarlinkConfig->pDevProfile,
                           VP_PTABLE_NULL,
                           zarlinkConfig->pDcProfile,
                           zarlinkConfig->pRingProfile,
                           VP_PTABLE_NULL,
                           VP_PTABLE_NULL
                        );
   if ( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpInitDevice failed (%d)", LOG_MODULE_NAME, status ));
      XCHG_ASSERT( status == VP_STATUS_SUCCESS );
   }

   /* Check if VP_DEV_EVID_DEV_INIT_CMP occurred */
   if( TRUE != zarlinkWaitForEvent( &(zarlinkConfig->pDevCtx[deviceId]), VP_EVCAT_RESPONSE, VP_DEV_EVID_DEV_INIT_CMP ) )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: ERROR: Device initialization was not completed", LOG_MODULE_NAME ));
      return ( XDRV_SLIC_STATUS_ERROR );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,  "%s: %s device %d initialized OK", LOG_MODULE_NAME, LOG_DEV_NAME, zarlinkConfig->pDev->deviceId ));

   return ( XDRV_SLIC_STATUS_SUCESS );
}

/*****************************************************************************
*
* FUNCTION:   deinitZarlink
*
* PURPOSE:    De-initialize Zarlink SLIC/SLAC device.
*
* PARAMETERS: Lots!
*
* RETURNS:    XDRV_SLIC_STATUS_SUCESS on success.
*             XDRV_SLIC_STATUS_ERROR on failure.
*
*****************************************************************************/
XDRV_SLIC_STATUS deinitZarlink( ZARLINK_CONFIG* zarlinkConfig )
{
   VpStatusType         status;
   XDRV_UINT8           deviceId = zarlinkConfig->pDev->deviceId;
   XDRV_UINT8           i;
   XDRV_UINT8           channelId[zarlinkConfig->numFxsPerDevice];
   XDRV_UINT8           daaChannelId[zarlinkConfig->numFxoPerDevice];
   XDRV_UINT8           numChansPerDevice;

   /* Total number of channels per device */
   numChansPerDevice = zarlinkConfig->numFxsPerDevice + zarlinkConfig->numFxoPerDevice;

   /* Zarlink does not really allocate extra memory in its API.  Not much to do. */
   for( i = 0; i < zarlinkConfig->numFxsPerDevice; i++ )
   {
      channelId[i] = zarlinkConfig->pDev->slicChannel[i].chan;

      /* Initialize FXS line */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Deinitializing %s FXS line %d", LOG_MODULE_NAME, LOG_DEV_NAME, channelId[i] ));

      /* Set the initial line state */
      status = VpSetLineState( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,channelId[i])]),
                               VP_LINE_DISCONNECT );
      if ( status != VP_STATUS_SUCCESS )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpSetLineState failed (%d) ", LOG_MODULE_NAME, status ));
         return ( XDRV_SLIC_STATUS_ERROR );
      }
   }

   for( i = 0; i < zarlinkConfig->numFxoPerDevice; i++ )
   {
      daaChannelId[i] = zarlinkConfig->pDev->daaChannel[i].chan;

      /* Initialize FXO line */
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Deinitializing %s FXO line", LOG_MODULE_NAME, LOG_DEV_NAME ));

      status = VpSetLineState( &(zarlinkConfig->pLineCtx[SLICSLAC_LINE(deviceId,numChansPerDevice,daaChannelId[i])]),
                               VP_LINE_FXO_LOOP_OPEN );
      if ( status != VP_STATUS_SUCCESS )
      {
       XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLICDAA: VpSetLineState failed (%d) ", status ));
      }
   }

   return ( XDRV_SLIC_STATUS_SUCESS );
}

/*****************************************************************************
*
* FUNCTION:   deviceReset
*
* PURPOSE:    Reset the SLIC
*
* PARAMETERS: none
*
* RETURNS:    none
*
*****************************************************************************/
void deviceReset( XDRV_UINT16 rstGpioPin )
{
#if defined(BRCM_68500)
   /*
    * Set up the GPIO output pin that resets the SLIC, reset the SLIC
    * Lilac doesnt know about active/inactive states, assume active = 1, inactive = 0
    */
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep(5);
   kerSysSetGpio( rstGpioPin, kGpioInactive);
   bosSleep(10);
   kerSysSetGpio( rstGpioPin, kGpioActive );

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Reset applied on GPIO pin %d", LOG_MODULE_NAME, rstGpioPin ));
#else
   /*
    * Set up the GPIO output pin that resets the SLIC, reset the SLIC
    */
   kerSysSetGpio( rstGpioPin, kGpioInactive );
   bosSleep(5);
   kerSysSetGpio( rstGpioPin, kGpioActive );
   bosSleep(10);
   kerSysSetGpio( rstGpioPin, kGpioInactive );

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Reset applied on GPIO pin %d", LOG_MODULE_NAME, rstGpioPin&BP_GPIO_NUM_MASK ));
#endif
}

/*****************************************************************************
*
* FUNCTION:   getCodecType
*
* PURPOSE:    Retrieve codec type specified.
*
* PARAMETERS: none
*
* RETURNS:    Codec type.
*
*****************************************************************************/
static VpOptionCodecType getCodecType( XDRV_UINT8 pcmMode )
{
   VpOptionCodecType codecType = VP_OPTION_LINEAR;

   switch ( pcmMode )
   {
      case ZARLINK_LINEAR:
      {
         codecType = VP_OPTION_LINEAR;
         break;
      }
      case ZARLINK_ULAW:
      {
         codecType = VP_OPTION_MLAW;
         break;
      }
      case ZARLINK_ALAW:
      {
         codecType = VP_OPTION_ALAW;
         break;
      }
      case ZARLINK_WIDEBAND:
      {
         codecType = VP_OPTION_WIDEBAND;
         break;
      }
      default:
      {
         codecType = VP_OPTION_LINEAR;
         break;
      }
   }

   return ( codecType );
}


/*
*****************************************************************************
** FUNCTION:   slicGetLocale
**
** PURPOSE:    This function gets the WinSLAC profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *slicGetLocale( VpDeviceType deviceType, VRG_COUNTRY country, VpOptionCodecType codec, XDRV_UINT8 rProt )
{
   VpProfileDataType *pCountryProfile = NULL;

   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "slicGetLocale: ERROR! Unsupported locale %d",
                country));
   }
   else
   {
      if ( codec == VP_OPTION_WIDEBAND )
      {
         if( deviceType == VP_DEV_880_SERIES )
         {
            if( rProt == ZARLINK_RF14 )
            {
               pCountryProfile = (VpProfileDataType *)gFXSWBProfilesRF14[country];
            }
            else
            {
               pCountryProfile = (VpProfileDataType *)gFXSWBProfiles[country];
            }
         }
         else if (( deviceType == VP_DEV_886_SERIES )||( deviceType == VP_DEV_887_SERIES ))
         {
            if( rProt == ZARLINK_RF14 )
            {
               pCountryProfile = (VpProfileDataType *)gFXSWBProfilesRF14[country];/* temp added need to change to wb*/
            }
            else
            {
               pCountryProfile = (VpProfileDataType *)gFXSWBProfiles[country];/* temp added need to change to wb*/
            }
         }
         else if( deviceType == VP_DEV_890_SERIES )
         {
            pCountryProfile = (VpProfileDataType *)gFXSWBProfiles_890[country];
         }
      }
      else
      {
         if( deviceType == VP_DEV_880_SERIES )
         {
            if( rProt == ZARLINK_RF14 )
            {
               pCountryProfile = (VpProfileDataType *)gFXSProfilesRF14[country];
            }
            else
            {
               pCountryProfile = (VpProfileDataType *)gFXSProfiles[country];
            }
         }
         else if (( deviceType == VP_DEV_886_SERIES )||( deviceType == VP_DEV_887_SERIES ))
         {
            if( rProt == ZARLINK_RF14 )
            {
               pCountryProfile = (VpProfileDataType *)gFXSProfilesRF14[country];
            }
            else
            {
               pCountryProfile = (VpProfileDataType *)gFXSProfiles[country];
            }
         }
         else if( deviceType == VP_DEV_890_SERIES )
         {
            pCountryProfile = (VpProfileDataType *)gFXSProfiles_890[country];
         }
      }
   }

   return pCountryProfile;
}

/*
*****************************************************************************
** FUNCTION:   slicGetRingLocale
**
** PURPOSE:    This function gets the Ring tone profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *slicGetRingLocale( VpDeviceType deviceType, VpDevCtxType* pDevCtx, VRG_COUNTRY country )
{
   VpProfileDataType *pCountryProfile = NULL;
   VpDeviceInfoType  deviceInfo;

   deviceInfo.pLineCtx = VP_NULL;
   deviceInfo.pDevCtx = pDevCtx;

   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "slicGetRingLocale: ERROR! Unsupported locale %d",
                country));
   }
   else
   {
      if( deviceType == VP_DEV_880_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXSRingProfiles[country];
      }
      else if( deviceType == VP_DEV_886_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXSRingProfiles_886[country];
      }
      else if( deviceType == VP_DEV_887_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXSRingProfiles_887[country];
      }
      else if( deviceType == VP_DEV_890_SERIES )
      {
         VpGetDeviceInfo( &deviceInfo );
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "******DeviceInfo PRODUCT CODE: %d ", deviceInfo.productCode ));
         if( deviceInfo.productCode == 0x4 || deviceInfo.productCode == 0x5 ) /* VP890_DEV_PCN_89136 || VP890_DEV_PCN_89336 */
         {
            pCountryProfile = (VpProfileDataType *)gFXSRingProfiles_890HV[country];
         }
         else
         {
            pCountryProfile = (VpProfileDataType *)gFXSRingProfiles_890[country];
         }
      }
   }

   return pCountryProfile;
}

/*
*****************************************************************************
** FUNCTION:   daaGetLocale
**
** PURPOSE:    This function gets the WinSLAC profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**             pFXOProfiles - pointer to FXO Profile structure
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *daaGetLocale( VpDeviceType deviceType, VRG_COUNTRY country )
{
   VpProfileDataType *pCountryProfile = NULL;


   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: daaGetLocale %d", LOG_MODULE_NAME, country));
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: ERROR! Unsupported locale %d", LOG_MODULE_NAME, country));
   }
   else
   {
      if( deviceType == VP_DEV_880_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXOProfiles[country];
      }
      else if( deviceType == VP_DEV_886_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXOProfiles[country];/*temp added, need change for 886 fxo */
      }
      else if( deviceType == VP_DEV_890_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXOProfiles_890[country];
      }
   }

   return pCountryProfile;
}

/*
*****************************************************************************
** FUNCTION:   daaGetDialLocale
**
** PURPOSE:    This function gets the WinSLAC profile for the
**             selected country.
**
** PARAMETERS: country - the country to select.
**             pFXOProfiles - pointer to FXO Profile structure
**
** RETURNS:    Pointer to the correct profile data.
**
*****************************************************************************
*/
static VpProfileDataType *daaGetDialLocale( VpDeviceType deviceType, VRG_COUNTRY country )
{
   VpProfileDataType *pCountryProfile = NULL;


   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: daaGetDialLocale %d", LOG_MODULE_NAME, country));
   if( country > VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: ERROR! Unsupported locale %d", LOG_MODULE_NAME, country));
   }
   else
   {
      if( deviceType == VP_DEV_880_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXODialProfiles[country];
      }
      else if( deviceType == VP_DEV_886_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXODialProfiles[country]; /* temp added need to change for 886*/
      }
      else if( deviceType == VP_DEV_890_SERIES )
      {
         pCountryProfile = (VpProfileDataType *)gFXODialProfiles_890[country];
      }
   }

   return pCountryProfile;
}


/*******************/
/* XDRV_SLIC_FUNCS */
/*******************/
/*
*****************************************************************************
** FUNCTION:   slicZarlinkGetDlp
**
** PURPOSE:    This function retrieves the Egress RX gains for the locale.
**
** PARAMETERS:
**
** RETURNS:    Rx gain.
**
*****************************************************************************
*/
XDRV_SINT16 slicZarlinkGetDlp( XDRV_SLIC *pDrv )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   VRG_COUNTRY country = VRG_COUNTRY_MAX;

   country = ((SLIC_DEVICE_DRV *)pDev->pParent)->locale;
   if( country >= VRG_COUNTRY_MAX )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "slicZarlinkGetDlp: ERROR! Unsupported locale %d",
                country));
   }
   else
   {
      return gFXSProfilesDlp[country];
   }

   return( 0 );
}

/*
*****************************************************************************
** FUNCTION:   slicZarlinkProcessEvents
**
** PURPOSE:    Increment time and process events
**
** PARAMETERS:
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicZarlinkProcessEvents( XDRV_SLIC *pDrv )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   VpDevCtxType      *pDevCtx;
   XDRV_UINT8        channelId;

   pDevCtx = ((SLIC_DEVICE_DRV *)pDev->pParent)->pDeviceCtx;
   channelId = pDev->chan;

   zarlinkProcessFxsEvents( pDevCtx, channelId ); /* Note: 'channelId' is physical device channel */
}

/*
*****************************************************************************
** FUNCTION:   slicZarlinkIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS:
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL slicZarlinkIsOffhook( XDRV_SLIC *pDrv )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx;

   pLineCtx = pDev->pChanLineCtx;

   return ( zarlinkFxsLineIsOffhook( pLineCtx ) );
}

/*
*****************************************************************************
** FUNCTION:   slicZarlinkModeControl
**
** PURPOSE:    Sets the SLIC into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLIC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
void slicZarlinkModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx;

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Mode for FXS channel changed to %d ", LOG_MODULE_NAME, mode ));

   pLineCtx = pDev->pChanLineCtx;
   zarlinkFxsLineModeControl( pLineCtx, mode );
}

/* Unused functions */
void slicZarlinkLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* Currently not applicable */
}

void slicZarlinkPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
	 VpLineStateType currentState;
   SLIC_CHANNEL_INFO *pDev 			=	(SLIC_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx	=	pDev->pChanLineCtx;

   if( VpGetLineState( pLineCtx, &currentState) != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "Line State ERROR: %s %d", __FUNCTION__, __LINE__));
      return;
   }

   VpSetLineState(pLineCtx, VpGetReverseState(currentState));

};

int slicZarlinkSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

int slicZarlinkGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

int slicZarlinkSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx;
   VpOptionDcFeedParamsType feedParms;

   /* Record boosted loop current state */
   pDev->bBoostedLoopCurrent = value;

   /* Set new loop current value */
   feedParms.validMask = VP_OPTION_CFG_ILA;
   feedParms.ila = ( value ? 40000 : 25000 );
   pLineCtx = pDev->pChanLineCtx;
   VpSetOption( pLineCtx, 
                VP_NULL,
                VP_OPTION_ID_DCFEED_PARAMS,
                (void*)&(feedParms) );

   XDRV_LOG_INFO(( XDRV_LOG_MOD_SLIC, "XDRV_SLIC %d: Boosted loop current mode change (%d).", pDev->globalId, value));

   return( 0 );
}

int slicZarlinkSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */
   (void) pDrv;
   (void) value;

   return( 0 );
}

XDRV_BOOL slicZarlinkGetOverCurrentStatus( XDRV_SLIC *pDrv )
{
   /* TODO: Only STUB for now. Maybe needs to be filled in. */

   return( 0 );
}

/************************
 * SLIC DEBUG FUNCTIONS *
 ************************/
XDRV_UINT32 slicZarlinkProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                              void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;
   XDRV_UINT8        *bufferPtr;
   XDRV_UINT32       status = 0;

   XDRV_UINT8        i;

   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "WARNING: Zarlink DEBUG USAGE ONLY!"));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "Zarlink device (physical) channel: %d", pDev->chan));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "Zarlink device type: %d", pDev->pChanLineCtx->pDevCtx->deviceType));

   bufferPtr = (XDRV_UINT8 *)dataPtr;

   if( set == 0 ) /* get */
   {
      if( indirect == 0 )
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "deviceId %d", ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Ecval: %d", pDev->chan+1));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Cmd: 0x%02x", (unsigned int)reg));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "CmdLen: %u", (unsigned int)regSize));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "dataPtr: 0x%08lx", (XDRV_UINT32)bufferPtr));

         VpMpiCmd( ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId, pDev->chan+1, reg, regSize, bufferPtr );
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER GET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on Zarlink device."));
         status = 1;
      }
   }
   else /* set */
   {
      if( indirect == 0 )
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "deviceId %d", ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Ecval: %d", pDev->chan+1));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "Cmd: 0x%02x", (unsigned int)reg));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "CmdLen: %u", (unsigned int)regSize));
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "dataPtr: 0x%08lx", (XDRV_UINT32)bufferPtr));

         VpMpiCmd( ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId, pDev->chan+1, reg, regSize, bufferPtr );
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER SET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on Zarlink device."));
         status = 1;
      }
   }

   return (status);
}

/*******************/
/* XDRV_DAA_FUNCS */
/*******************/
/*
*****************************************************************************
** FUNCTION:   daaZarlinkIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL daaZarlinkIsOffhook( XDRV_DAA *pDrv )
{
   DAA_CHANNEL_INFO *pDev = (DAA_CHANNEL_INFO *)pDrv;
   VpLineCtxType    *pLineCtx;

   pLineCtx = pDev->pDaaChanLineCtx;
   return ( zarlinkFxoLineIsOffhook( pLineCtx ) );
}

/*
*****************************************************************************
** FUNCTION:   daaZarlinkProcessEvents
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the DAA driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
void daaZarlinkProcessEvents( XDRV_DAA *pDrv )
{
   DAA_CHANNEL_INFO *pDev = (DAA_CHANNEL_INFO *)pDrv;
   VpDevCtxType     *pDevCtx;
   XDRV_UINT8       channelId;

   pDevCtx = ((SLIC_DEVICE_DRV *)pDev->pParent)->pDeviceCtx;
   channelId = pDev->chan;
   zarlinkProcessFxoEvents( pDevCtx, channelId ); /* Note: 'channelId' is physical device channel */
}

/*
*****************************************************************************
** FUNCTION:   daaZarlinkIsRingActive
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
XDRV_BOOL daaZarlinkIsRingActive( XDRV_DAA *pDrv )
{
   bool             lineStatus = FALSE;
   DAA_CHANNEL_INFO *pDev = (DAA_CHANNEL_INFO *)pDrv;
   VpLineCtxType    *pLineCtx;

   pLineCtx = pDev->pDaaChanLineCtx;

   if ( VpGetLineStatus( pLineCtx, VP_INPUT_RINGING, &lineStatus ) == VP_STATUS_SUCCESS )
   {
      if ( lineStatus == TRUE )
      {
         return (XDRV_TRUE);
      }
      else
      {
         return (XDRV_FALSE);
      }
   }
   else
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }

   return (XDRV_FALSE);
}

/*
*****************************************************************************
** FUNCTION:   daaZarlinkGetLoopCurrent
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
XDRV_DAA_LOOP daaZarlinkGetLoopCurrent( XDRV_DAA *pDrv )
{
   VpStatusType      status;
   VpLineStateType   lineState;
   DAA_CHANNEL_INFO  *pDev = (DAA_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx;

   pLineCtx = pDev->pDaaChanLineCtx;

   status = VpGetLineState( pLineCtx, &lineState );

   if( status != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: VpGetLineState failed (%d)", LOG_MODULE_NAME, status ));
      return ( XDRV_DAA_LOOP_OPEN );
   }

   if(lineState == VP_LINE_FXO_LOOP_CLOSE || lineState == VP_LINE_FXO_TALK)
   {
      return ( XDRV_DAA_LOOP_CLOSED );
   }
   else
   {
      return ( XDRV_DAA_LOOP_OPEN );
   }
}

/*
*****************************************************************************
** FUNCTION:   daaZarlinkModeControl
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
void daaZarlinkModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode )
{
   DAA_CHANNEL_INFO  *pDev = (DAA_CHANNEL_INFO *)pDrv;
   VpLineCtxType     *pLineCtx;

   pLineCtx = pDev->pDaaChanLineCtx;
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Mode for FXO channel changed to %d ", LOG_MODULE_NAME, mode ));
   zarlinkFxoLineModeControl( pLineCtx, mode );
}

/* Unused functions */
void daaZarlinkLedControl( XDRV_DAA *pDrv, int value )
{
    (void) pDrv;
    (void) value;
    /* Currently not applicable */
}


/***********************
 * DAA DEBUG FUNCTIONS *
 ***********************/
XDRV_UINT32 daaZarlinkProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                             void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set )
{
   DAA_CHANNEL_INFO *pDev = (DAA_CHANNEL_INFO *)pDrv;
   XDRV_UINT8       *bufferPtr;
   XDRV_UINT8       i;
   XDRV_UINT32      status = 0;

   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "WARNING: Zarlink DEBUG USAGE ONLY!"));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "Zarlink device (physical) channel: %d", pDev->chan));
   XDRV_LOG_NOTICE((XDRV_LOG_MOD_SLIC, "Zarlink device type: %d", pDev->pDaaChanLineCtx->pDevCtx->deviceType));

   bufferPtr = (XDRV_UINT8 *)dataPtr;

   if( set == 0 ) /* get */
   {
      if( indirect == 0 )
      {
         VpMpiCmd( ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId, pDev->chan+1, reg, regSize, bufferPtr );
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER GET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on Zarlink device."));
         status = 1;
      }
   }
   else /* set */
   {
      if( indirect == 0 )
      {
         VpMpiCmd( ((SLIC_DEVICE_DRV *)pDev->pParent)->mspiDevId, pDev->chan+1, reg, regSize, bufferPtr );
         for( i=0; i < regSize; i++)
         {
            XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "REGISTER SET BYTE %i: 0x%02x", i, bufferPtr[i]));
         }
      }
      else if( indirect == 1 )
      {
         XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "No (indirect) RAM registers on Zarlink device."));
         status = 1;
      }
   }

   return (status);
}

/*
*****************************************************************************
** FUNCTION:   slicZarlinkGetSlicParms
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
int slicZarlinkGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode )
{
   SLIC_CHANNEL_INFO *pDev = (SLIC_CHANNEL_INFO *)pDrv;

   /* Retrieve slic settings */
   *phaseReversal = pDev->phaseReversalActive;
   *loopCurrent = pDev->bBoostedLoopCurrent;
   *powerSource = pDev->bOnBattery;
   *slicMode = pDev->slicMode;

   return ( 0 );
}
