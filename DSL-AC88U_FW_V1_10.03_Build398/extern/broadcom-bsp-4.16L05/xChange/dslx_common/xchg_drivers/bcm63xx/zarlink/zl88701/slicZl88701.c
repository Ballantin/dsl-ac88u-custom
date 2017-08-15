/****************************************************************************
*
*  Copyright (c) 2005-2008 Broadcom Corporation
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
*    Description: 
*      This is the Legerity Le88506 - Single Channel Digital SLIC Device
*      driver implementation.
*
****************************************************************************/


/* ---- Include Files ---------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <bosSleep.h>
#include <xchgAssert.h>
#include <xdrvTypes.h>
#include <xdrvLog.h>

#include "slicZl88701.h"
#include "zarlinkCommonSlic.h"


/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */

#define LOG_MODULE_NAME       "SLIC"
#define LOG_DEV_NAME          "88701"


/* ---- Private Variables ------------------------------------ */

/*
 * Application memory space for the API requires Device/Line contexts and
 * objects.  This application uses the simplest memory scheme -- statically allocated, 
 * managed by the single CAS process
 */
/*
** NOTE: These are kept in each specific SLIC/DAA driver file in case
**       you require your own local version of a slicDrvFunc or daaDrvFunc.
**       In which case, you will need access to these device and line contexts.
*/
static VpDevCtxType           pDevCtx[MAX_NUM_DEVICES];
static VpLineCtxType          pLineCtx[MAX_NUM_LINES];
static Vp886DeviceObjectType  pDevObj[MAX_NUM_DEVICES];
static Vp886LineObjectType    pLineObj[MAX_NUM_LINES]; 


/*
** NOTE: If you require a specific version of a slicDrvFuncs, create locally in this file and change
**       file pointer in the structure to your local version below.
*/
/* SLIC "meta-driver" interface */
static const XDRV_SLIC_FUNCS slicDrvFuncs =
{
   slicZarlinkModeControl,
   slicZarlinkLedControl,
   slicZarlinkPhaseReversalControl,
   slicZarlinkIsOffhook,
   slicZarlinkSetRingParms,
   slicZarlinkGetRingParms,
   slicZarlinkSetBoostedLoopCurrent,
   slicZarlinkSetPowerSource,
   slicZarlinkGetOverCurrentStatus,
   slicZarlinkGetDlp,
   slicZarlinkProcessEvents,
   slicZarlinkProbe,
   slicZarlinkGetSlicParms
};

/* Configuration Structure */
static ZARLINK_CONFIG zarlinkConfig;


/*
*****************************************************************************
** FUNCTION:   slicZl88701Init
**
** PURPOSE:    Initialize and open the Zarlink 88701 device (FXSx2)
**
** PARAMETERS: pDev           -  SLIC(FXS) configuration structure
**             resetGpioPin   -  GPIO pin for resetting the device
**
** RETURNS:    XDRV_SLIC_STATUS
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicZl88701Init( SLIC_DEVICE_DRV *pDev, XDRV_UINT16 resetGpioPin )
{
   XDRV_SLIC_STATUS retStatus = XDRV_SLIC_STATUS_SUCESS;
   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Starting %s device and line initializion...", LOG_MODULE_NAME, LOG_DEV_NAME ));

   memset( &zarlinkConfig, 0, sizeof(ZARLINK_CONFIG) );

   /* NOTE: These should be setup according to configuration of device: 1FXS, 2FXS, 2FXS+1FXO, etc. */
   pDev->slicChannel[0].pDrvFuncs  = &slicDrvFuncs;
   pDev->slicChannel[1].pDrvFuncs  = &slicDrvFuncs;

   /* Reset the device if needed */
   if ( pDev->requiresReset )
   {
     deviceReset(resetGpioPin);
     bosSleep(5);
   }
   else
      {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Skipping SLIC chip reset", LOG_MODULE_NAME ));
   }

   if( pDev->deviceProfile == ZARLINK_INVBOOST )
   {
      zarlinkConfig.pDevProfile =   ZLR88742L_IB90V_DEVICE;
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: Configured for ZL887", LOG_DEV_NAME ));
   }
   else
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: ZL88701 Device profile not supported!", LOG_MODULE_NAME ));
      return XDRV_SLIC_STATUS_ERROR;
   } 

   /* Setup Configuration Structure */
   zarlinkConfig.pDev              = pDev;
   zarlinkConfig.numFxsPerDevice   = ZL88701_FXS_LINES;
   zarlinkConfig.numFxoPerDevice   = 0;
   zarlinkConfig.deviceType        = VP_DEV_887_SERIES;
   zarlinkConfig.fxsTermType       = VP_TERM_FXS_LOW_PWR;
   zarlinkConfig.fxoTermType       = 0;
   zarlinkConfig.pDevCtx           = pDevCtx;
   zarlinkConfig.pDevObj           = pDevObj;
   zarlinkConfig.pLineCtx          = pLineCtx;
   zarlinkConfig.pLineObj          = pLineObj;
   zarlinkConfig.pDcProfile        = DC_FXS_ZL880_IB90V_DEF;
   zarlinkConfig.pRingProfile      = RING_ZL880_FB100V_DEF;
   zarlinkConfig.rProt             = ZARLINK_RF14;
   
   /* Initialize the global device settings */
   retStatus = initZarlinkDevice( &zarlinkConfig );
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s device initializion FAILED !.", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   }

   /* Initialize the FXS lines */
   retStatus = initZarlinkLines( &zarlinkConfig );             
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s FXS line initializion FAILED !.", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   }

   XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "%s: %s device %d and FXS initializion COMPLETED OK !", LOG_MODULE_NAME, LOG_DEV_NAME, pDev->deviceId ));

   return ( retStatus );
}

/*
*****************************************************************************
** FUNCTION:   slicLe88506Deinit
**
** PURPOSE:    Shutdown the ZL88701 SLIC
**
** PARAMETERS: 
**
** RETURNS:    XDRV_SLIC_STATUS
**
*****************************************************************************
*/
XDRV_SLIC_STATUS slicZl88701Deinit( SLIC_DEVICE_DRV *pDev )
{
   XDRV_SLIC_STATUS retStatus = XDRV_SLIC_STATUS_SUCESS;

   /* Make sure correct pDev is pointed to in zarlinkConfig structure for each deinit. */ 
   zarlinkConfig.pDev = pDev;     
   retStatus = deinitZarlink( &zarlinkConfig );
   if ( retStatus != XDRV_SLIC_STATUS_SUCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "%s: %s FXS device de-initializion FAILED !.\n", LOG_MODULE_NAME, LOG_DEV_NAME ));
      return ( retStatus );
   }

   return ( retStatus );
}

#ifdef DYING_GASP_API
/*****************************************************************************
*
* FUNCTION:   slicZl88701Shutdown
*
* PURPOSE:    Reset the SLIC and keep SLIC in shutdown mode
*
* PARAMETERS: none
*
* RETURNS:    0 - success 
*
*****************************************************************************/
XDRV_SLIC_STATUS slicZl88701Shutdown( XDRV_UINT16 rstGpioPin )
{

   kerSysSetGpio( rstGpioPin, kGpioActive );

   return ( XDRV_SLIC_STATUS_SUCESS );
}
#endif
