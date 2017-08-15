/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
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
*    Filename: boardHalSlicUni.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface
*
*     The terms SLIC and SLAC are used interchangeably in the comments
*     or variables names in this file.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <kernelMainWrapper.h>
#include <boardparms_voice.h>
#include <vrgTypes.h>
#include <bcmChip.h>
#include <shared_utils.h>
#include <mspiDef.h>
#include <vrgLog.h>
#include <xchgAssert.h>

#include <boardHalInit.h>
#include "boardHal6xxx.h"
#include <boardHalSlic.h>
#include <boardHalProv.h>
#include <boardHalGpio.h>

#if SLACVENDOR_CFG_ZARLINK
#include <slicLe88506.h>
#include <slicZl88601.h>
#include <slicZl88701.h>
#include <slicZl88702.h>
#include <slicZl88801.h>
#include <slicLe88004.h>
#include <slicLe88536.h>
#include <slicLe88264.h>
#include <slicLe88266.h>
#include <slicLe88276.h>
#include <slicLe89116.h>
#include <slicLe89316.h>
#include <slicLe89136.h>
#include <slicLe89336.h>
#include <slicLe9661.h>
#include <slicLe9662.h>
#include <slicLe9642.h>
#include <slicLe9641.h>
#include <slicLe9652.h>
#include <slicLe9672.h>
#include <daaLe88010.h>
#include <daaLe89010.h>
/* SPI Mutexes */
#include "vp_zarlink_common.h"
#endif

#if SLACVENDOR_CFG_SILABS
#include <slicSi32178.h>
#include <slicSi32176.h>
#include <slicSi3226.h>

#ifdef SI32261ENABLE
#include <slicSi32261.h>
#endif
#ifdef SI32267ENABLE
#include <slicSi32267.h>
#endif

#include <daaSi3050.h>
/* SPI Mutexes */
#include "interface_driver_si_common.h"
#endif

#include "bosSleep.h"

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

typedef struct
{
   BP_VOICE_CHANNEL_SAMPLE_SIZE  sampleSize;
   BP_VOICE_CHANNEL_PCMCOMP_MODE pcmCompMode;
   BP_VOICE_CHANNEL_FREQRANGE    freqRange;
   int   zarlinkPcmMode;
   int   silabsPcmMode;
} SLIC_PCMMODE_MAP;

#if SLACVENDOR_CFG_SILABS && SLACVENDOR_CFG_ZARLINK
#define SILABS_CODEC_LINEAR    SILABS_LINEAR
#define SILABS_CODEC_WIDEBAND  SILABS_WIDEBAND
#define SILABS_CODEC_ULAW      SILABS_ULAW
#define SILABS_CODEC_ALAW      SILABS_ALAW
#define ZARLINK_CODEC_LINEAR   ZARLINK_LINEAR
#define ZARLINK_CODEC_WIDEBAND ZARLINK_WIDEBAND
#define ZARLINK_CODEC_ULAW     ZARLINK_ULAW
#define ZARLINK_CODEC_ALAW     ZARLINK_ALAW
#elif SLACVENDOR_CFG_ZARLINK
#define ZARLINK_CODEC_LINEAR   ZARLINK_LINEAR
#define ZARLINK_CODEC_WIDEBAND ZARLINK_WIDEBAND
#define ZARLINK_CODEC_ULAW     ZARLINK_ULAW
#define ZARLINK_CODEC_ALAW     ZARLINK_ALAW
#define SILABS_CODEC_LINEAR    0
#define SILABS_CODEC_WIDEBAND  0
#define SILABS_CODEC_ULAW      0
#define SILABS_CODEC_ALAW      0
#elif SLACVENDOR_CFG_SILABS
#define SILABS_CODEC_LINEAR    SILABS_LINEAR
#define SILABS_CODEC_WIDEBAND  SILABS_WIDEBAND
#define SILABS_CODEC_ULAW      SILABS_ULAW
#define SILABS_CODEC_ALAW      SILABS_ALAW
#define ZARLINK_CODEC_LINEAR   0
#define ZARLINK_CODEC_WIDEBAND 0
#define ZARLINK_CODEC_ULAW     0
#define ZARLINK_CODEC_ALAW     0
#endif

/* ---- Private Variables ------------------------------------------------ */
#if defined(CONFIG_BCM96816)
static int spi_bus_num_special = 0;
#endif

/* TODO:
** For each SLAC we allocate array of BP_MAX_VOICE_DEVICES
** This is more than needed since in realistic scenario we
** are going to have one or two devices of one type (e.g. Si32176),
** one or two of other type (e.g. Si32178) and zero of many other
** types. The static allocation should be replaced with dynamic one */

/* For SLIC_*_DRV structures that contain devices instead of channels!!!*/

#if SLACVENDOR_CFG_ZARLINK
/* ------------- Zarlink settings ----------- */
/* 89116 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe89116[BP_MAX_VOICE_DEVICES];
static int slicDriverLe89116index = 0;

/* 89316 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe89316[BP_MAX_VOICE_DEVICES];
static int slicDriverLe89316index = 0;

/* 89136 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe89136[BP_MAX_VOICE_DEVICES];
static int slicDriverLe89136index = 0;

/* 89336 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe89336[BP_MAX_VOICE_DEVICES];
static int slicDriverLe89336index = 0;

/* Le88266 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe88266[BP_MAX_VOICE_DEVICES];
static int slicDriverLe88266index = 0;

/* Le88276 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe88276[BP_MAX_VOICE_DEVICES];
static int slicDriverLe88276index = 0;

/* Le88506 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe88506[BP_MAX_VOICE_DEVICES];
static int slicDriverLe88506index = 0;

/* Zl88601 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverZl88601[BP_MAX_VOICE_DEVICES];
static int slicDriverZl88601index = 0;

/* Le9661 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9661[BP_MAX_VOICE_DEVICES];
static int slicDriverLe96611index = 0;

/* Le9662 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9662[BP_MAX_VOICE_DEVICES];
static int slicDriverLe96621index = 0;

/* Le9642 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9642[BP_MAX_VOICE_DEVICES];
static int slicDriverLe9642index = 0;

/* Le9641 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9641[BP_MAX_VOICE_DEVICES];
static int slicDriverLe9641index = 0;
/* Le9652 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9652[BP_MAX_VOICE_DEVICES];
static int slicDriverLe9652index = 0;


/* Le9672 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe9672[BP_MAX_VOICE_DEVICES];
static int slicDriverLe96721index = 0;

/* Zl88701 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverZl88701[BP_MAX_VOICE_DEVICES];
static int slicDriverZl88701index = 0;

/* Zl88702 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverZl88702[BP_MAX_VOICE_DEVICES];
static int slicDriverZl88702index = 0;

/* Zl88801 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverZl88801[BP_MAX_VOICE_DEVICES];
static int slicDriverZl88801index = 0;


/* Le88536 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe88536[BP_MAX_VOICE_DEVICES];
static int slicDriverLe88536index = 0;

/* Le88264 configuration structure */
static SLIC_DEVICE_DRV gSlicDriverLe88264[BP_MAX_VOICE_DEVICES];
static int slicDriverLe88264index = 0;

/* Le89010 configuration structure */
static SLIC_DEVICE_DRV gDaaDriverLe89010[BP_MAX_VOICE_DEVICES];
static int daaDriverLe89010index = 0;

/* Le88010 configuration structure */
static DAA_Le88010_DRV gDaaDriverLe88010[BP_MAX_VOICE_DEVICES];
static int daaDriverLe88010index = 0;
#endif

/* ------------- SILABS settings ----------- */
#if SLACVENDOR_CFG_SILABS
/* Si32178 configuration structure */
static SI_SLIC_DEVICE_DRV gSlicDriverSi32178[BP_MAX_VOICE_DEVICES];
static int slicDriverSi32178index = 0;

/* Si32176 configuration structure */
static SI_SLIC_DEVICE_DRV gSlicDriverSi32176[BP_MAX_VOICE_DEVICES];
static int slicDriverSi32176index = 0;

/* Si3226 configuration structure */
static SI_SLIC_DEVICE_DRV gSlicDriverSi3226[BP_MAX_VOICE_DEVICES];
static int slicDriverSi3226index = 0;

#ifdef SI32261ENABLE
/* Si32261 configuration structure */
static SI_SLIC_DEVICE_DRV gSlicDriverSi32261[BP_MAX_VOICE_DEVICES];
static int slicDriverSi32261index = 0;
#endif
#ifdef SI32267ENABLE
/* Si32267 configuration structure */
static SI_SLIC_DEVICE_DRV gSlicDriverSi32267[BP_MAX_VOICE_DEVICES];
static int slicDriverSi32267index = 0;
#endif

/* For older structures that contain channels instead of devices!!!*/
/* Si3050 configuration structure */
static DAA_Si3050_DRV gDaaDriverSi3050[BP_MAX_VOICE_DEVICES];
static int daaDriverSi3050index = 0;

#endif

/* PCM Mode map */
SLIC_PCMMODE_MAP slicPcmModeMap[] =
   {   /*         Sample Size                            COMP-MODE                        Freq Range              Zarlink Mode     Silabs Mode */
      {  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,   BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, ZARLINK_CODEC_LINEAR  , SILABS_CODEC_LINEAR   },
      {  BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,   BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_WIDEBAND  , ZARLINK_CODEC_WIDEBAND, SILABS_CODEC_WIDEBAND },
      {  BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS,    BP_VOICE_CHANNEL_PCMCOMP_MODE_ULAW, BP_VOICE_CHANNEL_NARROWBAND, ZARLINK_CODEC_ULAW    , SILABS_CODEC_ULAW     },
      {  BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS,    BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW, BP_VOICE_CHANNEL_NARROWBAND, ZARLINK_CODEC_ALAW    , SILABS_CODEC_ALAW     },
   };


/* ---- Private Function Prototypes -------------------------------------- */

static int boardHalSetGpios( void );
static int getPcmModeMapIndex(  BP_VOICE_CHANNEL * pVoiceChan );
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalIsSlic
**
** PUPOSE:     Indicate if the channel is SLAC/SLIC
**
** PARAMETERS: chan  [IN]  -  channel/line number
**
** Returns:    True if SLIC, false if not
**
*****************************************************************************
**/
XDRV_BOOL boardHalIsSlic( int chan )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   if ( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_SLIC )
   {
      return XDRV_TRUE;
   }
   else
   {
      return XDRV_FALSE;
   }
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicInit
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: boardHalInitCfg   [IN]  -  Init configuration flags
**                                        (currently used only to fetch country code)
**             bHighVring        [IN]  -  NOT USED
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL bHighVring )
{
   int deviceId;
   int status;
#if SLACVENDOR_CFG_SILABS
   int numFxo;
#endif
   int ctrlState=0;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   (void)bHighVring;

   if ( (voiceParams->flags & BP_FLAG_DSP_APMHAL_ENABLE) != 0 )
   {
      boardHalSlicInitApm(boardHalInitCfg, bHighVring);
   }

   /* Set GPIO direction and overlay modes needed to initialize voice devices */
   if ( boardHalSetGpios() != 0 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Can't set proper GPIOs for initializing voice devices"  ));
      return( 1 );
   }

#if SLACVENDOR_CFG_SILABS
   spiSiInit();
#endif
#if SLACVENDOR_CFG_ZARLINK
   spiZarInit();
   VpSysServiceInit();
#endif

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
      ctrlState = SPI_CONTROLLER_STATE_CPHA_EXT;
   }
#endif

   /*
   ** Iterate through the device table in boardParams and fetch the parameters
   ** needed for voice device (e.g. SLIC) initialization.
   */
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
         switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
         {
#if SLACVENDOR_CFG_ZARLINK
            case BP_VD_ZARLINK_89336:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe89336[slicDriverLe89336index];

               pDeviceDriver->deviceId                = slicDriverLe89336index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan        = 0;

               pDeviceDriver->daaChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->daaChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->daaChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->daaChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->daaChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->daaChannel[0].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing 89336 device %d ...", slicDriverLe89336index ));
               if ( slicLe89336Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89336 SLIC initialization!!!" ));
               }

               slicDriverLe89336index++;
            }
            break;

            case BP_VD_ZARLINK_89136:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe89136[slicDriverLe89136index];

               pDeviceDriver->deviceId                = slicDriverLe89136index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan        = 0;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing 89136 device %d ...", slicDriverLe89136index ));
               if ( slicLe89136Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89136 SLIC initialization!!! "));
               }

               slicDriverLe89136index++;
            }
            break;

            case BP_VD_ZARLINK_89316:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe89316[slicDriverLe89316index];

               pDeviceDriver->deviceId                = slicDriverLe89316index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan        = 0;

               pDeviceDriver->daaChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->daaChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->daaChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->daaChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->daaChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->daaChannel[0].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing 89316 device %d ...", slicDriverLe89316index ));
               if ( slicLe89316Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89316 SLIC initialization!!!" ));
               }

               slicDriverLe89316index++;
            }
            break;

            case BP_VD_ZARLINK_89116:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe89116[slicDriverLe89116index];

               pDeviceDriver->deviceId                = slicDriverLe89116index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan        = 0;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing 89116 device %d ...", slicDriverLe89116index ));
               if ( slicLe89116Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89116 SLIC initialization!!! "));
               }

               slicDriverLe89116index++;
            }
            break;

            case BP_VD_ZARLINK_88266:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe88266[slicDriverLe88266index];

               pDeviceDriver->deviceId                  = slicDriverLe88266index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88266 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88266 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88266 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88266 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88266 device ..." ));
               if ( slicLe88266Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88266 SLIC initialization!!!" ));
               }
               slicDriverLe88266index++;
            }
            break;

            case BP_VD_ZARLINK_88276:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe88276[slicDriverLe88276index];

               pDeviceDriver->deviceId                  = slicDriverLe88276index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88276 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88276 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88276 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88276 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);

               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88276 device ..." ));
               if ( slicLe88276Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88276 SLIC initialization!!!" ));
               }
               slicDriverLe88276index++;
            }
            break;
            case BP_VD_ZARLINK_88601:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverZl88601[slicDriverZl88601index];

               pDeviceDriver->deviceId                  = slicDriverZl88601index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88601 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88601 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88601 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88601 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Zl88604 device ..." ));
               if ( slicZl88601Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88604 SLIC initialization!!!" ));
               }
               slicDriverZl88601index++;
            }
            break;
            case BP_VD_ZARLINK_88701:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverZl88701[slicDriverZl88701index];

               pDeviceDriver->deviceId                  = slicDriverZl88701index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88701 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88701 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88701 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88701 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Zl88701 device ..." ));
               if ( slicZl88701Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88701 SLIC initialization!!!" ));
               }
               slicDriverZl88701index++;
            }
            break;

            case BP_VD_ZARLINK_88801:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverZl88801[slicDriverZl88801index];

               pDeviceDriver->deviceId                  = slicDriverZl88801index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Zl88801 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Zl88801 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Zl88801 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Zl88801 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Zl88801 device ..." ));
               if ( slicZl88801Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88801 SLIC initialization!!!" ));
               }
               slicDriverZl88801index++;
            }
            break;
            
            case BP_VD_ZARLINK_88702_ZSI:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverZl88702[slicDriverZl88702index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverZl88702index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88702 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88702 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On ZL88702 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on ZL88702 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing ZL88702 device ..." ));
                  isolatorPresent = 0;  /* Present Le9662 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Zl88702 device ..." ));
               if ( slicZl88702Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88702 SLIC initialization!!!" ));
               }
               slicDriverZl88702index++;
            }
            break;

            case BP_VD_ZARLINK_9672_ZSI:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9672[slicDriverLe96721index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe96721index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9672 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9672 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9672 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9672 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9672 device ..." ));
                  isolatorPresent = 0;  /* Present Le9672 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9672 device ..." ));
               if ( slicLe9672Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9672 SLIC initialization!!!" ));
               }
               slicDriverLe96721index++;
            }
            break;

            case BP_VD_ZARLINK_9661:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9661[slicDriverLe96611index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe96611index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9661 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9661 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9661 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9661 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9661 device ..." ));
                  isolatorPresent = 0;  /* Present Le9661 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9661 device ..." ));
               if ( slicLe9661Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9661 SLIC initialization!!!" ));
               }
               slicDriverLe96611index++;
            }
            break;

            case BP_VD_ZARLINK_9662:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9662[slicDriverLe96621index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe96621index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9662 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9662 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9662 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9662 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9662 device ..." ));
                  isolatorPresent = 0;  /* Present Le9662 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9662 device ..." ));
               if ( slicLe9662Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9662 SLIC initialization!!!" ));
               }
               slicDriverLe96621index++;
            }
            break;
            case BP_VD_ZARLINK_9642_ZSI:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9642[slicDriverLe9642index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe9642index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9642 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9642 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9642 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9642 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9642 device ..." ));
                  isolatorPresent = 0;  /* Present Le9642 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9642 device ..." ));
               if ( slicLe9642Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9642 SLIC initialization!!!" ));
               }
               slicDriverLe9642index++;
            }
            break;
            case BP_VD_ZARLINK_9641_ZSI:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9641[slicDriverLe9641index];

               pDeviceDriver->deviceId                  = slicDriverLe9641index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9641 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9641 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9641 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9641 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9641 device ..." ));
               if ( slicLe9641Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9641 SLIC initialization!!!" ));
               }
               slicDriverLe9641index++;
            }
            break;
            case BP_VD_ZARLINK_9652_ZSI_IB:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe9652[slicDriverLe9652index];

               pDeviceDriver->deviceId                  = slicDriverLe9652index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9652 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9652 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le9652 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le9652 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_MASTERSLAVE_FB:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_MASTERSLAVE_FB;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le9652 device ..." ));
               if ( slicLe9652Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9652 SLIC initialization!!!" ));
               }
               slicDriverLe9652index++;
            }
            break;
            case BP_VD_ZARLINK_88506:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe88506[slicDriverLe88506index];

               pDeviceDriver->deviceId                  = slicDriverLe88506index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88506 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88506 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88506 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88506 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88506 device ..." ));
               if ( slicLe88506Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88506 SLIC initialization!!!" ));
               }
               slicDriverLe88506index++;
            }
            break;

            case BP_VD_ZARLINK_88536:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe88536[slicDriverLe88536index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe88536index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88536 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88536 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88536 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88536 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;


               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST_TH:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST_THALASSA;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF|SPI_CONTROLLER_STATE_CPHA_EXT;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, 1024000, SPI_MODE_2, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

#if 0  /* This will be done through BcmSpixxx API call */
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl &= ~HS_SPI_CLK_GATE_SSOFF;
                  bosSleepNs(2000);
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_POLARITY;
                  HS_SPI->hs_spiGlobalCtrl |= HS_SPI_CLK_GATE_SSOFF;
#endif

                  /* MSPI DEV ID is modified to indicate it is a ZSI device for the vp_hal. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_ZSI);

                  /* For BCM63268 A0 Mode ONLY!!!*/
                  /* ZSI requires TX timeslots to be incremented by 1 to offset Zarlink's timing change in their source code.
                     This can be changed back to normal if Zarlink changes their source code OR if we update our PCM block h/w
                     to remove our Zarlink IP 2-bit delay compensation so that Zarlink SLAC will handle compensation instead. */
                  //pDeviceDriver->slicChannel[0].txTimeSlot += 1;   /* For A0 mode only */
                  //pDeviceDriver->slicChannel[1].txTimeSlot += 1;   /* For A0 mode only */
               }
               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  /* SPI setup for SLAC */
                  ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
                  status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
                  XCHG_ASSERT( status == SPI_STATUS_OK );

                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88004 device ..." ));
                  isolatorPresent = 0;  /* Present Le88536 daughtercard does *not* have digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88536 device ..." ));
               if ( slicLe88536Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88536 SLIC initialization!!!" ));
               }
               slicDriverLe88536index++;
            }
            break;

            case BP_VD_ZARLINK_88264:
            {
               SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverLe88264[slicDriverLe88264index];
               XDRV_UINT8 isolatorPresent = 0;

               pDeviceDriver->deviceId                  = slicDriverLe88264index;
               pDeviceDriver->locale                    = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId                 = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset             = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88264 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88536 device */
               pDeviceDriver->slicChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
               pDeviceDriver->slicChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;
               pDeviceDriver->slicChannel[0].chan         = 0;

               pDeviceDriver->slicChannel[1].pParent      = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs    = NULL;
               pDeviceDriver->slicChannel[1].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].zarlinkPcmMode;
               /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
               ** would mean "next 16-bit slot". On Le88264 devices counting of timeslots is based on 8-bit slots Therefore we
               ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on Le88536 device */
               pDeviceDriver->slicChannel[1].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot * 2;
               pDeviceDriver->slicChannel[1].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot * 2;
               pDeviceDriver->slicChannel[1].chan         = 1;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK;
                  }
                  break;
                  case BP_VD_FLYBACK_TH:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_FLYBACK_THALASSA;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_BUCKBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST;
                  }
                  break;
                  case BP_VD_INVBOOST_TH:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_INVBOOST_THALASSA;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = ZARLINK_UNDEFINED;
                  }
                  break;
               }

               if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
               {
                  VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88004 device ..." ));
                  isolatorPresent = 1;  /* Le88264 daughtercard has real digital isolator present on-board */
                  slicLe88004Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, isolatorPresent );

                  /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                     DEV ID when VpMakeDeviceObject is called. */
                  pDeviceDriver->mspiDevId |= (ZARLINK_THALASSA|ZARLINK_THALASSA_SUBSLAC1);

                  /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                     would of undergone a reset and "represents the reset" for the device. */
                  pDeviceDriver->requiresReset = XDRV_FALSE;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88264 device ..." ));
               if ( slicLe88264Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88264 SLIC initialization!!!" ));
               }
               slicDriverLe88264index++;
            }
            break;
#endif
#if SLACVENDOR_CFG_SILABS
            case BP_VD_SILABS_32176:
            {
               SI_SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverSi32176[slicDriverSi32176index];

               pDeviceDriver->deviceId                = slicDriverSi32176index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot;
               pDeviceDriver->slicChannel[0].chan        = 0;
               pDeviceDriver->slicChannel[0].bDrvEnabled = XDRV_FALSE;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_BUCKBOOST;
                  }
                  break;
                  case BP_VD_QCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_QCUK;
                  }
                  break;
                  case BP_VD_FIXEDRAIL:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FIXEDRAIL;
                  }
                  break;
                  case BP_VD_LCQCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_LCQCUK;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si32176 device %d ...", slicDriverSi32176index ));
               if ( slicSi32176Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32176 SLIC initialization!!!" ));
               }

               slicDriverSi32176index++;
            }
            break;

            case BP_VD_SILABS_32178:
            {
               SI_SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverSi32178[slicDriverSi32178index];

               pDeviceDriver->deviceId                = slicDriverSi32178index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot;
               pDeviceDriver->slicChannel[0].chan        = 0;
               pDeviceDriver->slicChannel[0].bDrvEnabled = XDRV_FALSE;

               pDeviceDriver->daaChannel[0].pParent      = pDeviceDriver;
               pDeviceDriver->daaChannel[0].pDrvFuncs    = NULL;
               pDeviceDriver->daaChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->daaChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot;
               pDeviceDriver->daaChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot;
               pDeviceDriver->daaChannel[0].chan         = 1;
               pDeviceDriver->daaChannel[0].bDrvEnabled  = XDRV_FALSE;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_BUCKBOOST;
                  }
                  break;
                  case BP_VD_QCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_QCUK;
                  }
                  break;
                  case BP_VD_FIXEDRAIL:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FIXEDRAIL;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_UNDEFINED;
                  }
                  break;
               }

               numFxo = boardHalGetNumFxoEndpoints();
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si32178 device %d with %d FXO lines...", slicDriverSi32178index, numFxo ));
               if ( slicSi32178Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio, numFxo ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32178 SLIC initialization!!!" ));
               }

               slicDriverSi32178index++;
            }
            break;

            case BP_VD_SILABS_3226:
            {
               SI_SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverSi3226[slicDriverSi3226index];

               pDeviceDriver->deviceId                = slicDriverSi3226index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot;
               pDeviceDriver->slicChannel[0].chan        = 0;
               pDeviceDriver->slicChannel[0].bDrvEnabled = XDRV_FALSE;

               pDeviceDriver->slicChannel[1].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[1].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[1].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot;
               pDeviceDriver->slicChannel[1].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot;
               pDeviceDriver->slicChannel[1].chan        = 1;
               pDeviceDriver->slicChannel[1].bDrvEnabled = XDRV_FALSE;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_BUCKBOOST;
                  }
                  break;
                  case BP_VD_QCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_QCUK;
                  }
                  break;
                  case BP_VD_FIXEDRAIL:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FIXEDRAIL;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si3226 device ..." ));
               if ( slicSi3226Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si3226 SLIC initialization!!!" ));
               }
               slicDriverSi3226index++;
            }
            break;

#ifdef SI32261ENABLE
            case BP_VD_SILABS_32261:
            {
               SI_SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverSi32261[slicDriverSi32261index];

               pDeviceDriver->deviceId                = slicDriverSi32261index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot;
               pDeviceDriver->slicChannel[0].chan        = 0;
               pDeviceDriver->slicChannel[0].bDrvEnabled = XDRV_FALSE;

               pDeviceDriver->slicChannel[1].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[1].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].silabsPcmMode;
               pDeviceDriver->slicChannel[1].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot;
               pDeviceDriver->slicChannel[1].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot;
               pDeviceDriver->slicChannel[1].chan        = 1;
               pDeviceDriver->slicChannel[1].bDrvEnabled = XDRV_FALSE;

               /* SPI setup for SLAC */
               ctrlState |= (SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF);
               status = BcmSpiReserveSlave2(MSPI_BUS_ID, pDeviceDriver->mspiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_BUCKBOOST;
                  }
                  break;
                  case BP_VD_QCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_QCUK;
                  }
                  break;
                  case BP_VD_LCQCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_LCQCUK;
                  }
                  break;
                  case BP_VD_FIXEDRAIL:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FIXEDRAIL;
                  }
                  break;
                  case BP_VD_FB_TSS:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FB_TSS;
                  }
                  break;
                  case BP_VD_PMOS_BUCK_BOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_PMOS_BUCK_BOOST;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si32261 device %d ...", slicDriverSi32261index ));
               if ( slicSi32261Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32261 SLIC initialization!!!" ));
               }

               slicDriverSi32261index++;
            }
            break;
#endif
#ifdef SI32267ENABLE
            case BP_VD_SILABS_32267:
            {
               SI_SLIC_DEVICE_DRV* pDeviceDriver = &gSlicDriverSi32267[slicDriverSi32267index];

               pDeviceDriver->deviceId                = slicDriverSi32267index;
               pDeviceDriver->locale                  = boardHalInitCfg->country;
               pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
               pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

               pDeviceDriver->slicChannel[0].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[0].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[0].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].silabsPcmMode;
               pDeviceDriver->slicChannel[0].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot;
               pDeviceDriver->slicChannel[0].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot;
               pDeviceDriver->slicChannel[0].chan        = 0;
               pDeviceDriver->slicChannel[0].bDrvEnabled = XDRV_FALSE;

               pDeviceDriver->slicChannel[1].pParent     = pDeviceDriver;
               pDeviceDriver->slicChannel[1].pDrvFuncs   = NULL;
               pDeviceDriver->slicChannel[1].pcmMode     = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[1])].silabsPcmMode;
               pDeviceDriver->slicChannel[1].txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].txTimeslot;
               pDeviceDriver->slicChannel[1].rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[1].rxTimeslot;
               pDeviceDriver->slicChannel[1].chan        = 1;
               pDeviceDriver->slicChannel[1].bDrvEnabled = XDRV_FALSE;

               /* SPI setup for ISI.  Note: Bus is always HS_SPI for ISI devices. */
               ctrlState = SPI_CONTROLLER_STATE_SET | SPI_CONTROLLER_STATE_CPHA_EXT;
               status = BcmSpiReserveSlave2(HS_SPI_BUS_NUM, pDeviceDriver->mspiDevId, 4000000, SPI_MODE_3, ctrlState);
               XCHG_ASSERT( status == SPI_STATUS_OK );

               switch( voiceParams->deviceProfile )
               {
                  case BP_VD_FLYBACK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FLYBACK;
                  }
                  break;
                  case BP_VD_BUCKBOOST:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_BUCKBOOST;
                  }
                  break;
                  case BP_VD_QCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_QCUK;
                  }
                  break;
                  case BP_VD_LCQCUK:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_LCQCUK;
                  }
                  break;
                  case BP_VD_FIXEDRAIL:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FIXEDRAIL;
                  }
                  break;
                  case BP_VD_FB_TSS_ISO:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_FB_TSS_ISO;
                  }
                  break;
                  default:
                  {
                     pDeviceDriver->deviceProfile  = SILABS_UNDEFINED;
                  }
                  break;
               }

               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si32267 device %d ...", slicDriverSi32267index ));
               if ( slicSi32267Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
               {
                  VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32267 SLIC initialization!!!" ));
               }

               slicDriverSi32267index++;
            }
            break;
#endif
#endif
            default:
            break;
         }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinit
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   int status;
   int   deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   if ( (voiceParams->flags & BP_FLAG_DSP_APMHAL_ENABLE) != 0 )
   {
      boardHalSlicDeinitApm(boardHalInitCfg);
   }

   /*
   ** Iterate through the device table in boardParams and fetch the parameters
   ** needed for voice device (e.g. SLIC) deinitialization.
   */
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if SLACVENDOR_CFG_ZARLINK
         case BP_VD_ZARLINK_89336:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe89336index--;

            pDeviceDriver = &gSlicDriverLe89336[slicDriverLe89336index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 89336 device %d ...", slicDriverLe89336index ));
            if ( slicLe89336Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89336 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "89336 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_89136:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe89136index--;

            pDeviceDriver = &gSlicDriverLe89136[slicDriverLe89136index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 89136 device %d ...", slicDriverLe89136index ));
            if ( slicLe89136Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89136 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "89136 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_89316:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe89316index--;

            pDeviceDriver = &gSlicDriverLe89316[slicDriverLe89316index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 89316 device %d ...", slicDriverLe89316index ));
            if ( slicLe89316Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89316 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "89316 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_89116:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe89116index--;

            pDeviceDriver = &gSlicDriverLe89116[slicDriverLe89116index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 89116 device %d ...", slicDriverLe89116index ));
            if ( slicLe89116Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89116 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "89116 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88266:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe88266index--;

            pDeviceDriver = &gSlicDriverLe88266[slicDriverLe88266index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88266 device " ));

            if ( slicLe88266Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88266 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le88266 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88276:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe88276index--;

            pDeviceDriver = &gSlicDriverLe88276[slicDriverLe88276index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88276 device " ));

            if ( slicLe88276Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88276 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le88276 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88601:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverZl88601index--;

            pDeviceDriver = &gSlicDriverZl88601[slicDriverZl88601index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88604 device %d ...", slicDriverZl88601index ));
            if ( slicZl88601Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88604 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Zl88604 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88701:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverZl88701index--;

            pDeviceDriver = &gSlicDriverZl88701[slicDriverZl88701index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88701 device %d ...", slicDriverZl88701index ));
            if ( slicZl88701Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88701 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Zl88701 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88801:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverZl88801index--;

            pDeviceDriver = &gSlicDriverZl88801[slicDriverZl88801index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88801 device %d ...", slicDriverZl88801index ));
            if ( slicZl88801Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88801 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Zl88801 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
         
         case BP_VD_ZARLINK_88702_ZSI:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverZl88702index--;

            pDeviceDriver = &gSlicDriverZl88702[slicDriverZl88702index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88702 device %d ...", slicDriverZl88702index ));
            if ( slicZl88702Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Zl88702 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Zl88702 device deinitialized OK" ));
            }

            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9672_ZSI:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe96721index--;

            pDeviceDriver = &gSlicDriverLe9672[slicDriverLe96721index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le9672 device %d ...", slicDriverLe96721index ));
            if ( slicLe9672Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9672 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9672 device deinitialized OK" ));
            }

            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }
            
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88506:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe88506index--;

            pDeviceDriver = &gSlicDriverLe88506[slicDriverLe88506index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88506 device %d ...", slicDriverLe88506index ));
            if ( slicLe88506Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88506 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le88506 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88536:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe88536index--;

            pDeviceDriver = &gSlicDriverLe88536[slicDriverLe88536index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88536 device %d ...", slicDriverLe88536index ));
            if ( slicLe88536Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88536 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le88536 device deinitialized OK" ));
            }

            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe88004Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9661:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe96611index--;

            pDeviceDriver = &gSlicDriverLe9661[slicDriverLe96611index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le9661 device %d ...", slicDriverLe96611index ));
            if ( slicLe9661Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9661 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9661 device deinitialized OK" ));
            }
            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le9661 device ..." ));
               slicLe9661Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9662:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe96621index--;

            pDeviceDriver = &gSlicDriverLe9662[slicDriverLe96621index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88604 device %d ...", slicDriverLe96621index ));
            if ( slicLe9662Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9662 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9662 device deinitialized OK" ));
            }
            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe9662Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9642_ZSI:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe9642index--;

            pDeviceDriver = &gSlicDriverLe9642[slicDriverLe9642index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88604 device %d ...", slicDriverLe9642index ));
            if ( slicLe9642Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9642 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9642 device deinitialized OK" ));
            }
            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe9642Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9641_ZSI:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe9641index--;

            pDeviceDriver = &gSlicDriverLe9641[slicDriverLe9641index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le9641 device %d ...", slicDriverLe9641index ));
            if ( slicLe9641Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9641 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9641 device deinitialized OK" ));
            }
            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe9641Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_9652_ZSI_IB:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe9652index--;

            pDeviceDriver = &gSlicDriverLe9652[slicDriverLe9652index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Zl88604 device %d ...", slicDriverLe9652index ));
            if ( slicLe9652Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le9652 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le9652 device deinitialized OK" ));
            }
            if ( (voiceParams->flags & BP_FLAG_ZSI_SUPPORT) != 0 )
            {
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe9652Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }
            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_ZARLINK_88264:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverLe88264index--;

            pDeviceDriver = &gSlicDriverLe88264[slicDriverLe88264index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88264 device %d ...", slicDriverLe88264index ));
            if ( slicLe88264Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Le88264 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Le88264 device deinitialized OK" ));
            }

            if ( (voiceParams->flags & BP_FLAG_THALASSA_SUPPORT) != 0 )
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88004 device ..." ));
               slicLe88004Deinit( pDeviceDriver );

               /* MSPI DEV ID must be modified for SLACS behind the Thalassa device since the MSPI DEV ID translates to the
                  DEV ID when VpMakeDeviceObject is called. */
               pDeviceDriver->mspiDevId &= 0xFF;    /* Revert MSPI DEV ID to the original */

               /* 'requiresReset' field must be forced to "NO" for SLACS behind the Thalassa device since the Thalassa device
                  would of undergone a reset and "represents the reset" for the device. */
               pDeviceDriver->requiresReset = XDRV_TRUE;   /* Revert 'requiresReset' filed back to original */
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
#endif
#if SLACVENDOR_CFG_SILABS
         case BP_VD_SILABS_32176:
         {
            SI_SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverSi32176index--;

            pDeviceDriver = &gSlicDriverSi32176[slicDriverSi32176index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 32176 device %d ...", slicDriverSi32176index ));
            if ( slicSi32176Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32176 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Si32176 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_SILABS_32178:
         {
            SI_SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverSi32178index--;

            pDeviceDriver = &gSlicDriverSi32178[slicDriverSi32178index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 32178 device %d ...", slicDriverSi32178index ));
            if ( slicSi32178Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32178 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Si32178 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

         case BP_VD_SILABS_3226:
         {
            SI_SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverSi3226index--;

            pDeviceDriver = &gSlicDriverSi3226[slicDriverSi3226index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 3226 device %d ...", slicDriverSi3226index ));
            if ( slicSi3226Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si3226 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Si3226 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;

#ifdef SI32261ENABLE
         case BP_VD_SILABS_32261:
         {
            SI_SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverSi32261index--;

            pDeviceDriver = &gSlicDriverSi32261[slicDriverSi32261index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 32261 device %d ...", slicDriverSi32261index ));
            if ( slicSi32261Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32261 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Si32261 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
#endif
#ifdef SI32267ENABLE
         case BP_VD_SILABS_32267:
         {
            SI_SLIC_DEVICE_DRV* pDeviceDriver;
            /* First decrement the index since it was previously incremented in boardHalSlicInit */
            slicDriverSi32267index--;

            pDeviceDriver = &gSlicDriverSi32267[slicDriverSi32267index];
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing 32267 device %d ...", slicDriverSi32267index ));
            if ( slicSi32267Deinit( pDeviceDriver ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during Si32267 SLIC deinitialization!!!" ));
            }
            else
            {
               VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Si32267 device deinitialized OK" ));
            }

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( HS_SPI_BUS_NUM, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
#endif
#endif
         default:
         break;
      }
   }

   /* Set GPIO direction and overlay modes needed to deinitialize the SLIC/DAA devices */
   if ( boardHalResetGpios() != 0 )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Can't set proper GPIOs for deinitializing the voice devices!!!" ));
   }

   /* Destroy SPI Mutex */
#if SLACVENDOR_CFG_SILABS
   spiSiDeinit();
#endif
#if SLACVENDOR_CFG_ZARLINK
   spiZarInit();
#endif

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriver
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface
**
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriver( int chan )
{
   void* pSlicDriver = NULL;

   /* We will need to fetch the device index based on the channel number
   ** in order to determine the appropriate slicDriver structure */
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   XCHG_ASSERT( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_SLIC );

   switch ( pDeviceChannelMap->deviceType )
   {
#if SLACVENDOR_CFG_ZARLINK
      case BP_VD_ZARLINK_89136:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe89136[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_ZARLINK_89336:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe89336[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_ZARLINK_89116:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe89116[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_ZARLINK_89316:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe89316[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_ZARLINK_88601:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88601[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88601[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_88701:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88701[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88701[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_88801:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88801[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88801[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_88702_ZSI:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88702[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverZl88702[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9672_ZSI:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9672[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9672[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9661:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9661[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9661[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9662:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9662[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9662[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9642_ZSI:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9642[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9642[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9641_ZSI:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9641[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9641[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_9652_ZSI_IB:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9652[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe9652[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
      case BP_VD_ZARLINK_88506:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88506[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88506[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;

      case BP_VD_ZARLINK_88536:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88536[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88536[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;

      case BP_VD_ZARLINK_88264:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88264[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88264[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;

      case BP_VD_ZARLINK_88266:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88266[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88266[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;

      case BP_VD_ZARLINK_88276:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88276[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)&(gSlicDriverLe88276[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
#endif
      case BP_VD_ZARLINK_9530:
      case BP_VD_ZARLINK_9540:
      case BP_VD_ZARLINK_9541:
      /* fall through */
      case BP_VD_SILABS_3239:
      case BP_VD_SILABS_32392:
      {
         pSlicDriver = boardHalSlicGetDriverApm( pDeviceChannelMap->deviceChannel );
      }
      break;
#if SLACVENDOR_CFG_SILABS
      case BP_VD_SILABS_32176:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverSi32176[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_SILABS_32178:
      {
         pSlicDriver = (XDRV_SLIC*)&(gSlicDriverSi32178[pDeviceChannelMap->deviceIndex].slicChannel[0]);
      }
      break;

      case BP_VD_SILABS_3226:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi3226[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi3226[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;

#ifdef SI32261ENABLE
      case BP_VD_SILABS_32261:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi32261[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi32261[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
#endif
#ifdef SI32267ENABLE
      case BP_VD_SILABS_32267:
      {
         if( pDeviceChannelMap->deviceChannel == 0 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi32267[pDeviceChannelMap->deviceIndex].slicChannel[0]);
         }
         else if( pDeviceChannelMap->deviceChannel == 1 )
         {
            pSlicDriver = (XDRV_SLIC*)(&gSlicDriverSi32267[pDeviceChannelMap->deviceIndex].slicChannel[1]);
         }
      }
      break;
#endif
#endif
      default:
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Unrecognized SLIC device %d for channel %d !!!", pDeviceChannelMap->deviceType, chan ));
         pSlicDriver = NULL;
      }
      break;
   }

   return ( pSlicDriver );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicPowerSave
**
** PURPOSE:    Disable SLICs for power savings
**
** PARAMETERS: lineId      - physical interface to control
**             disableSlic - controls whether to enable power savings
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicPowerSave( int lineId, VRG_BOOL disableSlic )
{
   /* TODO:
   ** Review and implement boardHalSlicPowerSave
   ** used to shut down the SLIC.
   */

   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s: NOT IMPLEMENTED", __FUNCTION__ ));

   return ( 0 );
}


/*****************************************************************************
** FUNCTION:   boardHalSlicReset
**
** PURPOSE:    Reset the SLIC
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSlicReset( void )
{
   /* TODO:
   ** Review and implement boardHalSlicReset
   */

   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s: NOT IMPLEMENTED", __FUNCTION__ ));
   return;
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicEnable
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicEnable( int lineId, VRG_BOOL enableSlic )
{
   /* TODO:
   ** Review and implement boardHalSlicEnable
   */

   if( (boardHalProvGetFlags() & BP_FLAG_DSP_APMHAL_ENABLE) != 0)
   {
      return boardHalSlicEnableApm( lineId, enableSlic );
   }

   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s: NOT IMPLEMENTED", __FUNCTION__ ));
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefresh
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a
**             higher application layer (such as Maintanance Task). Satisfying these
**             requirements may necessitate to shutdown the HVG even after the
**             boardHalInit has configured the HVG for the last time.
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.
**
*****************************************************************************
*/
int boardHalSlicHvgRefresh( void )
{
   if( (boardHalProvGetFlags() & BP_FLAG_DSP_APMHAL_ENABLE) != 0)
   {
      return boardHalSlicHvgRefreshApm();
   }

   /* Nothing to do here */
   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSetAnalogLoopback
**
** PURPOSE:    Set GPIO offhook pin to enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSetAnalogLoopback( int lineId, VRG_BOOL enabled )
{
   if( (boardHalProvGetFlags() & BP_FLAG_DSP_APMHAL_ENABLE) != 0)
   {
      return boardHalSetAnalogLoopbackApm( lineId, enabled );
   }
   return;
}

#ifdef DYING_GASP_API
/*
*****************************************************************************
** FUNCTION:   boardHalSlicShutdown
**
** PURPOSE:    Shutdown the Le89316 SLIC
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
void boardHalSlicShutdown( void *context)
{
   /* TODO:
   ** Review and implement boardHalSlicShutdown
   */

   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s: NOT IMPLEMENTED", __FUNCTION__ ));

#if 0
   /*
    *  get the reset pin for SLICs
    */
   unsigned short gpioNum;
   if ( BpGetSlicGpio( 0, SLIC_GPIO_RESET, &gpioNum) == BP_BOARD_ID_NOT_FOUND )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s Slic 0 usGpioSlic0Reset not set!!!", __FUNCTION__ ));
   }

   slicLe89316Shutdown( gpioNum );
#endif
}
#endif

/*
*****************************************************************************
** FUNCTION:   boardHalSlicIsDisabled
**
** PURPOSE:    Query status of SLIC for a given endpt
**
** PARAMETERS: chan - endpt or line number (starting from 0)
**
** RETURNS:    VRG_TRUE if disabled, VRG_FALSE if enabled
**
*****************************************************************************
*/
VRG_BOOL boardHalSlicIsDisabled( int chan )
{
   /* TODO:
   ** Review and implement boardHalSlicIsDisabled
   */
   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "%s: NOT IMPLEMENTED", __FUNCTION__ ));

   return ( VRG_FALSE );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDaaGetDriver
**
** PURPOSE:    Get the DAA driver associated with an endpoint
**
** PARAMETERS: chan - endpt or line number (starting from 0)
**
** RETURNS:    pointer to the DAA driver interface
**
*****************************************************************************
*/
XDRV_DAA* boardHalDaaGetDriver( int chan )
{
   /* We will need to fetch the device index based on the channel number
   ** in order to determine the appropriate DAA structure within the slicDriver structure */
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   XCHG_ASSERT( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_DAA );

   switch ( pDeviceChannelMap->deviceType )
   {
#if SLACVENDOR_CFG_ZARLINK
      case BP_VD_ZARLINK_89336:
      {
         return ( (XDRV_DAA*)(&(gSlicDriverLe89336[pDeviceChannelMap->deviceIndex].daaChannel[0])) );
      }
      break;

      case BP_VD_ZARLINK_89316:
      {
         return ( (XDRV_DAA*)(&(gSlicDriverLe89316[pDeviceChannelMap->deviceIndex].daaChannel[0])) );
      }
      break;

      case BP_VD_ZARLINK_88010:
      {
         return ( (XDRV_DAA*)(&(gDaaDriverLe88010[pDeviceChannelMap->deviceIndex].daaInfo)) );
      }
      break;

      case BP_VD_ZARLINK_89010:
      {
         return ( (XDRV_DAA*)(&(gDaaDriverLe89010[pDeviceChannelMap->deviceIndex].daaChannel[0])) );
      }
      break;
#endif
#if SLACVENDOR_CFG_SILABS
      case BP_VD_SILABS_32178:
      {
         return ( (XDRV_DAA*)(&(gSlicDriverSi32178[pDeviceChannelMap->deviceIndex].daaChannel[0])) );
      }
      break;

      case BP_VD_SILABS_3050:
      {
         return ( (XDRV_DAA*)(&(gDaaDriverSi3050[pDeviceChannelMap->deviceIndex].daaInfo)) );
      }
      break;
#endif

      default:
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "Unrecognized FXO/DAA device %d for channel %d !!!", pDeviceChannelMap->deviceType, chan ));
      }
      break;
   }

   return ( NULL );
}


/****************************************************************************
*
* FUNCTION:   boardHalIsDaa
*
* PURPOSE:    Determine whether a particular line uses a DAA
*
* PARAMETERS: chan - the line number ( 0 referenced )
*
* RETURNS:    XDRV_TRUE if DAA, XDRV_FALSE otherwise
*
****************************************************************************/
XDRV_BOOL boardHalIsDaa( int chan )
{
   BOARDHALPROV_DEVICE_CHANNEL_MAP* pDeviceChannelMap = boardHalProvGetChannelMap( chan );

   if ( pDeviceChannelMap->deviceChannelType == BP_VCTYPE_DAA )
   {
      return XDRV_TRUE;
   }
   else
   {
      return XDRV_FALSE;
   }
}

#ifdef BRCM_6848
extern void bcm_set_pinmux(unsigned int pin_num, unsigned int mux_num);
#endif
/*****************************************************************************
*
* FUNCTION:   boardHalSetGpios
*
* PURPOSE:    Set directions and mode for various GPIOs
*             involved with SLIC initialization (relay GPIOs, reset GPIO)
*
* PARAMETERS: none
*
* RETURNS:    0 on success, else 1
*
*****************************************************************************/
static int boardHalSetGpios( void )
{
   unsigned long gpioNumL = 0;
   int i;
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   /* Set chip select GPIO pins (if defined for the device) */
   /* 6328 does not have GPIOMode register.  Use PinMuxing. */

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      if ( voiceParams->voiceDevice[deviceId].spiCtrl.spiGpio != BP_NOT_DEFINED )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "GPIO 0x%08x used for SPI control", voiceParams->voiceDevice[deviceId].spiCtrl.spiGpio ));
         gpioNumL = (unsigned long)(voiceParams->voiceDevice[deviceId].spiCtrl.spiGpio);
        
#if defined( BRCM_63268 ) || defined( BRCM_6362 ) || defined( BRCM_6368 )          
         GPIO->GPIOMode |= GPIO_NUM_TO_MASK(gpioNumL);
#endif /* defined( BRCM_63268 ) || defined( BRCM_6362 ) || defined( BRCM_6368 ) */

#ifdef BRCM_6838
         /* Set SPI Pinmux */
         set_pinmux( (gpioNumL & BP_GPIO_NUM_MASK), (( (gpioNumL & BP_GPIO_NUM_MASK) == 61 )? PINMUX_0:PINMUX_MSPI) );
#endif /* BRCM_6838 */
         
#ifdef BRCM_6848
         bcm_set_pinmux( (gpioNumL & BP_GPIO_NUM_MASK), PINMUX_MSPI);
#endif /* BRCM_6848 */
      }

      if ( voiceParams->voiceDevice[deviceId].resetGpio != BP_NOT_DEFINED )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Device %d using Reset GPIO 0x%08X ", deviceId, voiceParams->voiceDevice[deviceId].resetGpio ));
         gpioNumL = (unsigned long)(voiceParams->voiceDevice[deviceId].resetGpio);

#if defined( BRCM_63268 ) || defined( BRCM_6362 ) || defined( BRCM_6368 )          
         /* Enable SPI_SS signals to be driven over selecte GPIOs */
         GPIO->GPIOMode &= ~GPIO_NUM_TO_MASK(gpioNumL);
#endif /* defined( BRCM_63268 ) || defined( BRCM_6362 ) || defined( BRCM_6368 ) */

#ifdef BRCM_6838
         /* Set GPIO Pinmux 5 */
         set_pinmux( (gpioNumL & BP_GPIO_NUM_MASK), PINMUX_GPIO);
#endif /* BRCM_6838 */

#ifdef BRCM_6848
         bcm_set_pinmux( (gpioNumL & BP_GPIO_NUM_MASK), PINMUX_MSPI);
#endif /* BRCM_6848 */

         /* Set GPIO Direction */
         kerSysSetGpioDir(gpioNumL & BP_GPIO_NUM_MASK);
      }
   }

   /* Set GPIOs for faiolver relays */
   for ( i = 0; i < voiceParams->numFailoverRelayPins; i++ )
   {
      if ( voiceParams->pstnRelayCtrl.relayGpio[i] == BP_NOT_DEFINED )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "GPIO for relay %d not set !!!", i ));
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Relay %d GPIO is 0x%08X !!!", i, voiceParams->pstnRelayCtrl.relayGpio[i] ));

#ifndef PSTN_LIFE_LINE_SUPPORT
         kerSysSetGpio( voiceParams->pstnRelayCtrl.relayGpio[i], kGpioActive );
#endif
      }
   }

   return ( 0 );
}


/*****************************************************************************
*
* FUNCTION:   boardHalResetGpios
*
* PURPOSE:    Set directions and mode for various GPIOs
*             involved with SLIC deinitialization (relay GPIOs, reset GPIO)
*
* PARAMETERS: none
*
* RETURNS:    0 on success, else 1
*
*****************************************************************************/
int boardHalResetGpios( void )
{
   int deviceId;
   int i;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   /* Reset devices */
   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      if ( voiceParams->voiceDevice[deviceId].resetGpio != BP_NOT_DEFINED &&
           voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_IDECT1)
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Resetting device %d using GPIO 0x%08X ", deviceId, voiceParams->voiceDevice[deviceId].resetGpio ));
         kerSysSetGpio( voiceParams->voiceDevice[deviceId].resetGpio, kGpioActive );
      }
   }

   /* Reset GPIOs for failover relays */
   for ( i = 0; i < voiceParams->numFailoverRelayPins; i++ )
   {
      if ( voiceParams->pstnRelayCtrl.relayGpio[i] == BP_NOT_DEFINED )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "GPIO for relay %d not set !!!", i ));
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Resetting relay %d using GPIO 0x%08X !!!", i, voiceParams->pstnRelayCtrl.relayGpio[i] ));
         kerSysSetGpio( voiceParams->pstnRelayCtrl.relayGpio[i], kGpioInactive );
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalDaaInit
**
** PURPOSE:    Initialize the discrete DAA chip
**
** PARAMETERS: VRG_COUNTRY
**
** RETURNS:    0 on success, 1 otherwise
**
** NOTE:       This function doesn't perform any action for
**             SLAC/SLIC devices where the DAA is integrated
**             (e.g. VE89316 or SI32178)
**
*****************************************************************************
*/
int boardHalDaaInit( VRG_COUNTRY locale )
{
   int deviceId;
   int status;
   int ctrlState;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {

/* NOTE: 88010 driver has NOT been converted yet to utilize the re-architectured centralized driver like 89010.
         88010 may not even work here. */
#if SLACVENDOR_CFG_ZARLINK
         case BP_VD_ZARLINK_88010:
         {
            DAA_Le88010_DRV* pDeviceDriver = &gDaaDriverLe88010[daaDriverLe88010index];

            pDeviceDriver->mspiId = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
            pDeviceDriver->pcmMode = LE88010_PCM_MODE_LINEAR;   /* default value */
            if ( voiceParams->voiceDevice[deviceId].channel[0].sampleSize == BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS )
            {
               if ( voiceParams->voiceDevice[deviceId].channel[0].pcmCompMode == BP_VOICE_CHANNEL_PCMCOMP_MODE_ULAW )
               {
                  pDeviceDriver->pcmMode = LE88010_PCM_MODE_ULAW;
               }
               else if ( voiceParams->voiceDevice[deviceId].channel[0].pcmCompMode == BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW )
               {
                  pDeviceDriver->pcmMode = LE88010_PCM_MODE_ALAW;
               }
            }

            /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
            ** would mean "next 16-bit slot". On VE880/VE890 devices counting of timeslots is based on 8-bit slots Therefore we
            ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
            pDeviceDriver->txTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
            pDeviceDriver->rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;

            /* Set Physical Channel of device */
            pDeviceDriver->daaInfo.chan = 0;

            /* This gets set in actual device driver */
            pDeviceDriver->daaInfo.pDrvFuncs = NULL;

            /* SPI setup for DAA */
            ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
            status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
            XCHG_ASSERT( status == SPI_STATUS_OK );

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le88010 device %d ...", daaDriverLe88010index ));
            daaLe88010Init( locale, daaDriverLe88010index, pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio );
            daaDriverLe88010index++;
         }
         break;

         case BP_VD_ZARLINK_89010:
         {
            SLIC_DEVICE_DRV* pDeviceDriver = &gDaaDriverLe89010[daaDriverLe89010index];

            pDeviceDriver->deviceId                = slicDriverLe89316index;
            pDeviceDriver->locale                  = locale;
            pDeviceDriver->mspiDevId               = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
            pDeviceDriver->requiresReset           = (voiceParams->voiceDevice[deviceId].requiresReset) ? XDRV_TRUE : XDRV_FALSE;

            pDeviceDriver->daaChannel[0].pParent      = pDeviceDriver;
            pDeviceDriver->daaChannel[0].pDrvFuncs    = NULL;
            pDeviceDriver->daaChannel[0].pcmMode      = slicPcmModeMap[getPcmModeMapIndex(&voiceParams->voiceDevice[deviceId].channel[0])].zarlinkPcmMode;
            /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
            ** would mean "next 16-bit slot". On VE890 devices counting of timeslots is based on 8-bit slots Therefore we
            ** have to multiple the timeslot from VOICE_BOARD_PARMS by two to get the right timeslot on VE890 device */
            pDeviceDriver->daaChannel[0].txTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 2;
            pDeviceDriver->daaChannel[0].rxTimeSlot   = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 2;

            /* NOTE: Normally, this is 0.  As per Zarlink's instructions -physical channel ID always 1 on Le89010*/
            pDeviceDriver->daaChannel[0].chan         = 1;

            /* SPI setup for DAA */
            ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
            status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
            XCHG_ASSERT( status == SPI_STATUS_OK );

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Le89010 device %d ...", daaDriverLe89010index ));
            if( daaLe89010Init( pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio ) != XDRV_SLIC_STATUS_SUCESS )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL, "ERROR during 89010 SLIC initialization!!!" ));
            }
            daaDriverLe89010index++;
         }
         break;
#endif
#if SLACVENDOR_CFG_SILABS
         case BP_VD_SILABS_3050:
         {
            DAA_Si3050_DRV* pDeviceDriver = &gDaaDriverSi3050[daaDriverSi3050index];

            pDeviceDriver->mspiId = voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId;
            pDeviceDriver->pcmMode = SI3050_LINEAR;   /* default value */
            if ( voiceParams->voiceDevice[deviceId].channel[0].sampleSize == BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS )
            {
               if ( voiceParams->voiceDevice[deviceId].channel[0].pcmCompMode == BP_VOICE_CHANNEL_PCMCOMP_MODE_ULAW )
               {
                  pDeviceDriver->pcmMode = SI3050_ULAW;
               }
               else if ( voiceParams->voiceDevice[deviceId].channel[0].pcmCompMode == BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW )
               {
                  pDeviceDriver->pcmMode = SI3050_ALAW;
               }
            }

            /* Counting of timeslots in VOICE_BOARD_PARMS is based on 16-bit slots. In other words increment of one timeslot
            ** would mean "next 16-bit slot". On Si3250 counting of timeslots is based on PCM clock cycles slots
            ** after framesync */
            pDeviceDriver->txTimeSlot = voiceParams->voiceDevice[deviceId].channel[0].txTimeslot * 16 + 1;
            pDeviceDriver->rxTimeSlot  = voiceParams->voiceDevice[deviceId].channel[0].rxTimeslot * 16 + 1;

            /* Set Physical Channel of device */
            pDeviceDriver->daaInfo.chan = 0;

            /* This gets set in actual device driver */
            pDeviceDriver->daaInfo.pDrvFuncs = NULL;

            /* SPI setup for DAA */
            ctrlState = SPI_CONTROLLER_STATE_SET|SPI_CONTROLLER_STATE_GATE_CLK_SSOFF;
            status = BcmSpiReserveSlave2(MSPI_BUS_ID, voiceParams->voiceDevice[deviceId].spiCtrl.spiDevId, MSPI_CLK_FREQUENCY, SPI_MODE_3, ctrlState);
            XCHG_ASSERT( status == SPI_STATUS_OK );

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Initializing Si3050 device %d ...", daaDriverSi3050index ));
            daaSi3050Init( locale, daaDriverSi3050index, pDeviceDriver, voiceParams->voiceDevice[deviceId].resetGpio );
            daaDriverSi3050index++;
         }
         break;
#endif

         default:
            break;
      }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDaaDeinit
**
** PURPOSE:    Deinitialize the discrete DAA chip
**
** PARAMETERS: none
**
** RETURNS:    0 on success, 1 otherwise
**
** NOTE:       This function doesn't perform any action for
**             SLAC/SLIC devices where the DAA is integrated
**             (e.g. VE89316 or SI32178)
**
*****************************************************************************
*/
int boardHalDaaDeinit( void )
{
   int deviceId;
   int status;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {

/* NOTE: 88010 driver has NOT been converted yet to utilize the re-architectured centralized driver like 89010.
         88010 may not even work here. */
#if SLACVENDOR_CFG_ZARLINK
         case BP_VD_ZARLINK_88010:
         {
            DAA_Le88010_DRV* pDeviceDriver;
            daaDriverLe88010index--;
            pDeviceDriver = &gDaaDriverLe88010[daaDriverLe88010index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le88010 device %d ...", daaDriverLe88010index ));
            daaLe88010Deinit( pDeviceDriver );
         }
         break;

         case BP_VD_ZARLINK_89010:
         {
            SLIC_DEVICE_DRV* pDeviceDriver;
            daaDriverLe89010index--;
            pDeviceDriver = &gDaaDriverLe89010[daaDriverLe89010index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Le89010 device %d ...", daaDriverLe89010index ));
            daaLe89010Deinit( pDeviceDriver );

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiDevId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
#endif
#if SLACVENDOR_CFG_SILABS
         case BP_VD_SILABS_3050:
         {
            DAA_Si3050_DRV* pDeviceDriver;
            daaDriverSi3050index--;
            pDeviceDriver = &gDaaDriverSi3050[daaDriverSi3050index];

            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Deinitializing Si3050 device %d ...", daaDriverSi3050index ));
            daaSi3050Deinit( pDeviceDriver );

            /* Unreserve slave ID */
            status = BcmSpiReleaseSlave( MSPI_BUS_ID, pDeviceDriver->mspiId );
            XCHG_ASSERT( status == SPI_STATUS_OK );
         }
         break;
#endif
         default:
         break;
      }
   }

   return ( 0 );
}

/*****************************************************************************
*
* FUNCTION:   getPcmModeMapIndex
*
* PURPOSE:    Gets the index of pcmmodemap table which applies to this voice channel
*
* PARAMETERS: BP_VOICE_CHANNEL * pVoiceChan
*
* RETURNS:    0 on success, else 1
*
*****************************************************************************/
int getPcmModeMapIndex(  BP_VOICE_CHANNEL * pVoiceChan )
{
   int i;
   int modeMapTableMaxEntry = sizeof(slicPcmModeMap) / sizeof( SLIC_PCMMODE_MAP );

   for( i = 0; i < modeMapTableMaxEntry; i++ )
   {
      if( pVoiceChan->sampleSize == slicPcmModeMap[i].sampleSize )
      {
         if( pVoiceChan->sampleSize == BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS )
         {
            if( pVoiceChan->pcmCompMode == slicPcmModeMap[i].pcmCompMode )
            {
               return i;
            }
         }
         else
         {
            if( pVoiceChan->freqRange == slicPcmModeMap[i].freqRange )
            {
               return i;
            }
         }
      }
   }

   VRG_LOG_ERROR(( VRG_LOG_MOD_BOARDHAL,"No Matching PCM mode configuration found. Using default 16-bit Linear configuration!"));
   return 0;
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   boardHalDaaShutdown
*
* PURPOSE:    Shutdown the DAAs
*
* PARAMETERS: none
*
* RETURNS:    0 on success, else 1
*
*****************************************************************************/
void boardHalDaaShutdown( void *context)
{
   int deviceId;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();

   for ( deviceId = 0; voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE; deviceId++ )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
#if SLACVENDOR_CFG_ZARLINK
         case BP_VD_ZARLINK_88010:
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Shutting down Le88010 device ..." ));
            daaLe88010Shutdown( voiceParams->voiceDevice[deviceId].resetGpio );
         }
         break;

         case BP_VD_ZARLINK_89010:
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Shutting down Le89010 device ..." ));
            daaLe89010Shutdown( voiceParams->voiceDevice[deviceId].resetGpio );
         }
         break;
#endif
#if SLACVENDOR_CFG_SILABS
         case BP_VD_SILABS_3050:
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Shutting down Si3050 device ..." ));
            daaSi3050Shutdown( voiceParams->voiceDevice[deviceId].resetGpio );
         }
         break;
#endif

         default:
            break;
      }
   }
}
#endif

