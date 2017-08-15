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
*    Filename: boardHalApm6828.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent APM interface for
*     the 6828.
*
****************************************************************************/

#define  PXD_LOOP_DEBUG    0

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgCountry.h>
#include <vrgProv.h>
#include <boardHalApm.h>
#include <boardHalProv.h>
#include <bcm6828.h>
#include "boardHal6828.h"
#include <halshim.h>
#include <boardHalDsp.h>
#include <xdrvCache.h>
#include <bosCfg.h>
#include <bosSleep.h>
#include <flexiCalc6828.h>

#ifndef BRCM_NONDIST_BUILD
#include <itpcSharedCmds.h>
#endif /* BRCM_NONDIST_BUILD */

/* include driver-specific defines */
#include <apm6828.h>
#include <hvg6828.h>

#if BOS_OS_ECOS
   #include <cyg/infra/diag.h>   /* For diag_printf */
#endif


/* ---- Public Variables ------------------------------------------------- */

/* ITPC instance memory. */
#ifdef BRCM_NONDIST_BUILD  
XDRV_ITPC                     gHalItpcDrv;
#else 
extern XDRV_ITPC              gHalItpcDrv;
#endif 

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */
static APM_6828_DRV gApmDriver;

/* Static memory for dynamically provisioned flexicalc settings */
VRG_UINT8 dynFlexicalc[MAX_PROV_PARM_SIZE];

/* ---- Private Function Prototypes -------------------------------------- */
static void ntrInit( void );

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalApmInit
**
** PURPOSE:    Initialize the APM
**
** PARAMETERS: boardHalInitCfg   - current init cfg
**             widebandMode      - 0=8kHz sampling; 1=16kHz sampling
**             packetRateSamples - packet rate in 8kHz samples
**             useRingGen        - use APM ring generator
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL widebandMode,
                     int packetRateSamples, int useRingGen )
{
   int result;
   APM6828_CFG apmCfg;
   int deviceId = 0;
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
   BP_VOICE_DEVICE_TYPE devType = BP_VD_NONE;

   /* 
   ** Additional NTR-related setup 
   ** All the NTR-related settings should be configured in the data BSP and/or GPON driver.
   ** This call is used for any additional NTR-related setup.
   */
   ntrInit();

   memset( &gApmDriver, 0, sizeof( gApmDriver ) );

   /* Set up the configuration parameters */
   /* Set the sampling rate in kHz */
   if ( widebandMode )
   {
      apmCfg.sampleRate = 16;  /* in kHz */
   }
   else
   {
      apmCfg.sampleRate = 8;
   }

   /* Convert the packet rate from samples to ms */
   apmCfg.packetRate = packetRateSamples >> 3;

   /* We only support 1, 5 and 10ms rates for now */
   if (( apmCfg.packetRate != 1 ) &&
       ( apmCfg.packetRate != 5 ) &&
       ( apmCfg.packetRate != 10 ))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "BoardHalApm: Unsupported packet rate (%d)!!!", apmCfg.packetRate ));
      return ( 1 );
   }

   /* Fill in the APM ring generator control */
   apmCfg.useRingGen = useRingGen;

   /* Fill in Docsis clock frequency control */
   apmCfg.useJapDocsisClock = boardHalInitCfg->useJapDocsisClock;

   /* Fill in the HVG design topology depending on slic type */
   while ( voiceParams->voiceDevice[deviceId].voiceDeviceType != BP_VD_NONE )
   {
      switch ( voiceParams->voiceDevice[deviceId].voiceDeviceType )
      {
         case BP_VD_ZARLINK_9530:
         {
            boardHalInitCfg->hvgDesignType = XDRV_APM_HVG_DESIGN_BUCKBOOST;
            boardHalInitCfg->slicType = XDRV_SLIC_L9530;
            apmCfg.hvgType = XDRV_APM_HVG_DESIGN_BUCKBOOST;
            devType = BP_VD_ZARLINK_9530;
         }
         break;

         case BP_VD_SILABS_3239:
         {
            boardHalInitCfg->hvgDesignType = XDRV_APM_HVG_DESIGN_FLYBACK;
            boardHalInitCfg->slicType = XDRV_SLIC_SI3239;            
            apmCfg.hvgType = XDRV_APM_HVG_DESIGN_FLYBACK;
            devType = BP_VD_SILABS_3239;
         }
         break;

         case BP_VD_SILABS_32392:
         {
            boardHalInitCfg->hvgDesignType = XDRV_APM_HVG_DESIGN_FLYBACK;
            boardHalInitCfg->slicType = XDRV_SLIC_SI32392;        
            apmCfg.hvgType = XDRV_APM_HVG_DESIGN_FLYBACK;
            devType = BP_VD_SILABS_32392;
         }
         break;

         default:
         break;
      }

      if ( devType != BP_VD_NONE )
      {
         /* APM only supports one SLIC */
         break;
      }
      deviceId++;
   }

   if ( devType == BP_VD_NONE )
   {
      /* Voice device is not found, use values corresponding to 9530 as default 
       * Note: No device programming here. 9530 is just a placeholder for initialization
       * of what is most likely a run-time that doesn't use an APM.
       */
      boardHalInitCfg->hvgDesignType = XDRV_APM_HVG_DESIGN_BUCKBOOST;
      apmCfg.hvgType = XDRV_APM_HVG_DESIGN_BUCKBOOST;
      devType = BP_VD_ZARLINK_9530;
   }

   apmCfg.pFlexiCalc = (APM6828_FLEXICALC_CFG*)dynFlexicalc;

   /* Flexicalc settings are the same for both endpoints 
   ** so use endpoint 0 when calling endptProvGetValue */
   if ( endptProvGetValue( 0, EPPROV_Flexicalc, (void*)apmCfg.pFlexiCalc ) == EPSTATUS_SUCCESS )
   {
      /* Success during dynamic provisioning. 
      ** Externally provisioned flexicalc settings are now copied to apmCfg.pFlexicalc */
      VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "Using external dynamically provisioned flexicalc settings")); 
   }
   else
   {
      /* Error during dynamic provisioning. 
      ** Externally provisioned flexicalc settings are NOT used. */

      /* Fill in the FlexiCalc config parameters from static tables */
      apmCfg.pFlexiCalc = flexiCalcGetCfg6828( boardHalInitCfg->country, 
                                               apmCfg.sampleRate * 1000, 
                                               devType );

      VRG_LOG_INFO(( VRG_LOG_MOD_BOARDHAL, "Using statically provisioned flexicalc settings")); 
   }

   /* Fill in the HVG reference voltage flag */
   apmCfg.sharedReferencePin = boardHalInitCfg->sharedReferencePin;

   /* Initialize the APM module */
   if (( result = apm6828_init( &gApmDriver, PERF, &apmCfg )))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "apm6828_init failed, err code (%d)", result ));
      return ( result );
   }

   /* Initialize the High Voltage Generator module */
   if (( result = hvg6828Init( (XDRV_APM*)&gApmDriver )))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "hvg6828Init failed, err code (%d)", result ));
      return ( result );
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalApmDeinit
**
** PURPOSE:    Shutdown the APM
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalApmDeinit( void )
{
   /* Initialize the APM module */
   apm6828_deinit(&gApmDriver);

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmGetDriver
**
** PURPOSE:    Get the APM driver
**
** PARAMETERS: none
**
** RETURNS:    pointer to the APM driver interface
**
*****************************************************************************
*/
XDRV_APM* boardHalApmGetDriver( int dspNum )
{
   (void)dspNum;

   return ( (XDRV_APM*)&gApmDriver );
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmSyncClkSource
**
** PURPOSE:    Get the APM driver
**
** PARAMETERS: clkSource - 0=use internal NTP; 1=use default
**
** RETURNS:    pointer to the APM driver interface
**
*****************************************************************************
*/
void boardHalApmSyncClkSource( int clkSource )
{
   XDRV_APM* pApmDriver = boardHalApmGetDriver( 0 );
   apm6828SetClkSource( pApmDriver, clkSource );
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmGetSlicType
**
** PURPOSE:    Get slic type from APM Flexicalc Cfg
**
** PARAMETERS: none
**
** RETURNS:    slic type
**
*****************************************************************************
*/
XDRV_UINT32 boardHalApmGetSlicType( void )
{
   XDRV_UINT32 slicType = 0;
   if (gApmDriver.Cfg.pFlexiCalc)
   {
      slicType = (XDRV_UINT32)(gApmDriver.Cfg.pFlexiCalc->slic_type);
   }
   return slicType;
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmFlexicalcReinit
**
** PURPOSE:    Change the current flexicalc parameters
**
** PARAMETERS: boardHalInitCfg   - current init cfg
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalApmFlexicalcReinit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   switch( boardHalInitCfg->slicType )
   {
      case XDRV_SLIC_L9530:
      {
         /* Fill in the FlexiCalc config parameters */
         gApmDriver.Cfg.pFlexiCalc = flexiCalcGetCfg6828( boardHalInitCfg->country,
                                                          gApmDriver.Cfg.sampleRate * 1000,
														  BP_VD_ZARLINK_9530 );
      }
      break;

      case XDRV_SLIC_SI3239:
      {
         /* Fill in the FlexiCalc config parameters */
         gApmDriver.Cfg.pFlexiCalc = flexiCalcGetCfg6828( boardHalInitCfg->country,
                                                          gApmDriver.Cfg.sampleRate * 1000,
														  BP_VD_SILABS_3239 );
      }
      break;
	  
      case XDRV_SLIC_SI32392:
      {
         /* Fill in the FlexiCalc config parameters */
         gApmDriver.Cfg.pFlexiCalc = flexiCalcGetCfg6828( boardHalInitCfg->country,
                                                          gApmDriver.Cfg.sampleRate * 1000,
														  BP_VD_SILABS_32392 );
      }
      break;

      default:
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "Unsupported SLIC type %d!!!", boardHalInitCfg->slicType ));
      }
      break;
   }

   /* Reinit flexicalc with new values */
   apm6828_flexicalcConfig( gApmDriver.pApm, gApmDriver.Cfg.pFlexiCalc );
}
/*
*****************************************************************************
** FUNCTION:   ntrInit
**
** PURPOSE:    Initialize NTR frequency that is used as reference clock
**             by the APM block
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
static void ntrInit(void)
{
   #if 0 // Enable if NTR clock should go OUT on GPIO 15
   GPIO->GPIOMode |= (1<<15);
   GPIO->GPIODir |= (1<<15);
   #endif

   /* 
   ** The register GPON_SERDES_MISC_CFG needs to be set
   ** to a value that generates 256kHz input clock to the APM DLL.
   ** This setting is already performed in the main BSP and the GPON driver 
   ** (regardless of whether the GPON link is up or not) so we don't
   ** have to explicitly set it here. 
   ** 
   ** This function is used as a
   ** placeholder in case some additional NTR-related settings are 
   ** needed in the voice board HAL.
   */
}

/*
*****************************************************************************
** FUNCTION:   boardHalApmHPFControl
**
** PURPOSE:    Determines if HPF control is available in the APM
**
** PARAMETERS: none
**
** RETURNS:    True if available, false otherwise
**
*****************************************************************************
*/
VRG_BOOL boardHalApmHPFControl( void )
{
   /* Disable HW HPF, rely on DSP HPF instead */
   return VRG_FALSE;
}

#if 0
/*
*****************************************************************************
** FUNCTION:   boardHalApmDebug
**
** PURPOSE:    HAL debug Apm signal
**
** PARAMETERS: none
**
** RETURNS:    True if available, false otherwise
**
*****************************************************************************
*/
void boardHalApmDebug(unsigned chanNum, int apmDebugType)
{
   ITPC_SHARED_APM_DMA_DEBUG_CONFIG itpcSignalConfig;
   itpcSignalConfig.chanNum = chanNum;
   itpcSignalConfig.debugType = (unsigned int)apmDebugType;
    
   xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_APM_DMA_DEBUG_CONFIG_CMD_ID, &itpcSignalConfig, NULL );
}
#endif
