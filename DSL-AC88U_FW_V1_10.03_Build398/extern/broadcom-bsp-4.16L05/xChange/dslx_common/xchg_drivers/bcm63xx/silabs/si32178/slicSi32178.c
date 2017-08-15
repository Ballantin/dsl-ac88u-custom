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
*    Filename: slicSi32178.c
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the control of the
*      SI32178 which consists of 1FXS(Si32178) + 1FXO(Si32918)
*
****************************************************************************/

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include "slicSi32178.h"
#include <proslic.h>
#include <interface_driver_si3217x_spi.h>
#include "board.h"

#include "silabsCommonSlic.h"
#include "si3217x_constants.h"

/***Used for disabling DAA on Si32178 for Green Contest***/
#include "si3217x.h"
/*********************************************************/

#define LOG_MODULE_NAME       "SLICDAA"
#define LOG_DEV_NAME          "32178"

/*
** NOTE: If you require a specific version of a slicDrvFuncs, create locally in this file and change
**       file pointer in the structure to your local version below.
*/
/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS slicDrvFuncs =
{
   slicSiLabsModeControl,
   slicSiLabsLedControl,
   slicSiLabsPhaseReversalControl,
   slicSiLabsIsOffhook,
   slicSiLabsSetRingParms,
   slicSiLabsGetRingParms,
   slicSiLabsSetBoostedLoopCurrent,
   slicSiLabsSetPowerSource,
   slicSiLabsGetOverCurrentStatus,
   slicSiLabsGetDlp,
   slicSiLabsProcessEvents,
   slicSiLabsProbe,
   slicSiLabsGetSlicParms   
};

/* DAA "meta-driver" interface */
static const XDRV_DAA_FUNCS daaDrvFuncs =
{
   daaSiLabsModeControl,
   daaSiLabsLedControl,
   daaSiLabsIsOffhook,
   daaSiLabsIsRingActive,
   daaSiLabsGetLoopCurrent,
   daaSiLabsProcessEvents,
   daaSiLabsProbe
};

/* Static variables */
extern Si3217x_General_Cfg  Si3217x_General_Configuration;  /* Until SiLabs creates API for modifying DAA_ENABLE */
static int numFxoLines;

/*****************************************************************
 *  Private PROSlic variables                                    *
 *                                                               *
 * Depending on how many different devices are present, whether  *
 * they are single slic/dual slic, whether they have different   *
 * mspi ids, whether they are daisy chained or not, the sizes    *
 * of the following arrays will change. Currently this driver is *
 * configured as follows:                                        *
 *    - Single SLIC channel per device                           *
 *    - Different mspi id per device                             *
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
/*
** NOTE: These are kept in each specific SLIC/DAA driver file in case
**       you require your own local version of a slicDrvFunc or daaDrvFunc.
**       In which case, you will need access to these device and line contexts.
*/
static ctrl_SiLabs                  spiRstObj[SI_MAX_NUM_DEVICES];        // H/W Control interface objects, indexed per device
static SiVoiceChanType_ptr          pProSlicChanPtr[SI_MAX_NUM_LINES];    // Array of ptr to Proslic Channel types, indexed per channel
static SiVoiceControlInterfaceType* pProHWIntf[SI_MAX_NUM_DEVICES];       // Array of ptr to controller interfaces, indexed per device
static SiVoiceDeviceType*           pProSLICDevices[SI_MAX_NUM_DEVICES];  // Array of ptr to Proslic Device types, indexed per device

/* Configuration Structure */
static SILABS_CONFIG silabsConfig;

/*
*****************************************************************************
** FUNCTION:   slicSi32178Init
**
** PURPOSE:    Initialize and open the Si32178 device (FXS + FXO)
**
** PARAMETERS: pDev         - pointer to the Si32178 SLIC info structure
**             resetGpioPin - reset GPIO for device
**
** RETURNS:    XDRV_SLIC_STATUS
**
** NOTE:  This is called PER DEVICE!!!
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicSi32178Init( SI_SLIC_DEVICE_DRV *pDev, XDRV_UINT16 resetGpioPin, int numFxo )
{
   XDRV_SLIC_STATUS retStatus = XDRV_SLIC_STATUS_SUCESS;

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Starting %s device and line initializion...", LOG_MODULE_NAME, LOG_DEV_NAME ));
   
   memset( &silabsConfig, 0, sizeof(SILABS_CONFIG) );

   /* NOTE: These should be setup according to configuration of device: 1FXS, 2FXS, 2FXS+1FXO, etc. */
   pDev->slicChannel[0].pDrvFuncs  = &slicDrvFuncs;
   pDev->daaChannel[0].pDrvFuncs   = &daaDrvFuncs;
   
   /* Assign mspidevice id to spireset object per SPI chip select */
   spiRstObj[pDev->deviceId].portID = pDev->mspiDevId;
   /* Null timer object */
   int timerObj = (int)NULL;  

   /* Reset the device if needed */
   if ( pDev->requiresReset )
   {
     deviceSiLabsReset(resetGpioPin);
     bosSleep(5);
   }
   else
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Skipping SLIC chip reset", LOG_MODULE_NAME ));
   } 

   if( pDev->deviceProfile == SILABS_FLYBACK )
   {
      silabsConfig.bomType = SI3217X_GEN_PARAM_FLYBACK;  
   }

   /* Setup Configuration Structure */
   silabsConfig.pDev               = pDev;
   silabsConfig.numFxsPerDevice    = 1;
   silabsConfig.numFxoPerDevice    = 1;
   silabsConfig.spiObj             = spiRstObj;
   silabsConfig.timerObj           = &timerObj;
   silabsConfig.voiceChans         = pProSlicChanPtr;
   silabsConfig.voiceDevices       = pProSLICDevices;
   silabsConfig.voiceHWIntf        = pProHWIntf;
   silabsConfig.chipType           = SI3217X_TYPE;
   silabsConfig.writeRegister_fptr = ctrl_3217x_WriteRegisterWrapper;
   silabsConfig.readRegister_fptr  = ctrl_3217x_ReadRegisterWrapper;
   silabsConfig.writeRAM_fptr      = ctrl_3217x_WriteRAMWrapper;
   silabsConfig.readRAM_fptr       = ctrl_3217x_ReadRAMWrapper;

   retStatus = initSiLabsDevice( &silabsConfig );
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s FXS device initializion FAILED !.", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   }
   
   /* Check if DAA is to be disabled for Green Contest */
   numFxoLines = numFxo;
   if( numFxoLines == 0 )
   {   
      /* Note: This is a hack since SiLabs does not provide an API to disable DAA */
      Si3217x_General_Configuration.daa_enable = 0;  /* disable DAA directly in table */
   }
   
   retStatus = initSiLabsLines( &silabsConfig );
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s FXS line initializion FAILED !.", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   } 
    
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s device %d initializion COMPLETED OK !", LOG_MODULE_NAME, LOG_DEV_NAME, pDev->deviceId ));

   return ( retStatus );
}

  
/*
*****************************************************************************
** FUNCTION:   slicSi32178Deinit
**
** PURPOSE:    Shutdown the Si32178 SLIC
**
** PARAMETERS: pDev  - pointer to the Si32178 SLIC info structure
**
** RETURNS:    none
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicSi32178Deinit( SI_SLIC_DEVICE_DRV *pDev )
{
   XDRV_SLIC_STATUS retStatus = XDRV_SLIC_STATUS_SUCESS;

   /* Make sure correct pDev is pointed to in silabsConfig structure for each deinit. */ 
   silabsConfig.pDev = pDev;    
   retStatus = deinitSiLabs( &silabsConfig );
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s FXS/FXO device de-initializion FAILED !.\n", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   }
   
   return ( retStatus );
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   slicSi32178Shutdown
*
* PURPOSE:    Reset the SLIC and keep SLIC in shutdown mode
*
* PARAMETERS: none
*
* RETURNS:    0 - success 
*
*****************************************************************************/
XDRV_SLIC_STATUS slicSi32178Shutdown( XDRV_UINT16 rstGpioPin )
{
   kerSysSetGpio( rstGpioPin, kGpioActive );
   
   return ( XDRV_SLIC_STATUS_SUCESS );
}
#endif



   
