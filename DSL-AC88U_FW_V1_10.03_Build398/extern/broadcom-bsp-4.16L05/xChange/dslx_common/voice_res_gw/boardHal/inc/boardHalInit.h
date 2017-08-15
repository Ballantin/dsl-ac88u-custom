/****************************************************************************
*
* Copyright (c) 2000-2012 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
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
*    Filename: boardHalInit.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      initialization of a board.
*
****************************************************************************/

#ifndef BOARD_HAL_INIT_H
#define BOARD_HAL_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgCountry.h>
#include <vrgTypes.h>
#include <xdrvApm.h>

/* ---- Constants and Types ---------------------------------------------- */
#define BOARDHAL_MAX_CALDATA_SIZE 32

/*
** Board HAL configurable init/deinit modules.
** Enables Board HAL to selectively bypass some of the modules that
** are not needed for all platforms.
*/
#define BOARDCFG_MODULE_GPIO        (1<<0)      /* GPIO */
#define BOARDCFG_MODULE_TPD         (1<<1)      /* TPD (linetests) */
#define BOARDCFG_MODULE_DIAG        (1<<2)      /* 6816 diagnostics */
#define BOARDCFG_MODULE_HVGRING     (1<<3)      /* HVG ring support */
#define BOARDCFG_MODULE_MSPI        (1<<4)      /* MSPI support */
#define BOARDCFG_MODULE_FASTSLIC    (1<<5)      /* Fast SLIC mode change support */
#define BOARDCFG_MODULE_HVGALTREGS  (1<<6)      /* Use Alternate HVG Settings */
#define BOARDCFG_MODULE_HVGTYPEFB   (1<<7)      /* HVG design topology */

#define boardHalRunningOnTP0() ((read_c0_diag3() >> 31) == 0) ? VRG_TRUE : VRG_FALSE

/*
** BoardHal initialization structure
*/
typedef struct VRG_BOARDHAL_INIT_CFG
{
   /* Platform specific SLIC control functions */
   const struct   BOARD_HAL_SLIC_FUNCS *pDrvSlicFuncs;

   /* Current country code based on nvol settings */
   VRG_COUNTRY  country;

   /* Setup clock registers based on input clock frequency */
   int          useJapDocsisClock;

   /* HVG layout Settings */
   int            slicType;
   int            hvgDesignType;
   int            sharedReferencePin;
   int            inputSupplyVoltage;

   /* Advanced feature control */
   int          enhancedControl;

   /* Reset control */
   int            driverRefresh;

   /* Calibraion data */
   int            calData[BOARDHAL_MAX_CALDATA_SIZE];
   
   /* APM channel swap flag */
   unsigned int   apmChannelSwap;

} VRG_BOARDHAL_INIT_CFG;

typedef enum VRG_BOARDHAL_BCMCHIP
{
   VRG_BOARDHAL_BCM96358  = 0x6358,
   VRG_BOARDHAL_BCM96368  = 0x6368,
   VRG_BOARDHAL_BCM96362  = 0x6362,
   VRG_BOARDHAL_BCM96328  = 0x6328,
   VRG_BOARDHAL_BCM96816  = 0x6816,
   VRG_BOARDHAL_BCM96818  = 0x6818,
   VRG_BOARDHAL_BCM96838  = 0x6838,
   VRG_BOARDHAL_BCM96828  = 0x6828,
   VRG_BOARDHAL_BCM963268 = 0x63268,
   VRG_BOARDHAL_BCM963138 = 0x63138,
   VRG_BOARDHAL_BCM963148 = 0x63148,
   VRG_BOARDHAL_BCM968500 = 0x68500,
   VRG_BOARDHAL_BCM963381 = 0x63381,
   VRG_BOARDHAL_BCM96848  = 0x6848,
   VRG_BOARDHAL_BCM_UNKNOWN

} VRG_BOARDHAL_BCMCHIP;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalInit
**
** PURPOSE:    Initialize the Board Hardware Abstraction Layer (HAL) for the
**             Voice Residential Gateway.
**
** PARAMETERS: boardHalInitCfg - board dependant configuration
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg );

/*
*****************************************************************************
** FUNCTION:   boardHalInitUni
**
** PURPOSE:    Initialize the Board Hardware Abstraction Layer (HAL) for the
**             Voice Residential Gateway.
**
** PARAMETERS: boardHalInitCfg - board dependant configuration
**             initModules       - modules to be initialized
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInitUni( VRG_BOARDHAL_INIT_CFG  *boardHalInitCfg,
                     int                    initModules
#ifdef BRCM_NONDIST_BUILD
                   , XDRV_UINT32            dmaBufferSize
                   , VRG_BOARDHAL_BCMCHIP   bcmChipId
                   , XDRV_UINT32            apmModeEnabled
#endif
                   );


/*
*****************************************************************************
** FUNCTION:   boardHalDeinit
**
** PURPOSE:    Uninitialize the Board HAL for the Voice Residential Gateway.
**             Reset any local storage and free all resources.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, otherwise error code.
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
*****************************************************************************
*/
int boardHalDeinit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg  );

#ifdef DYING_GASP_API
void boardHalShutdown(void *context);
#endif

/*
*****************************************************************************
** FUNCTION:   boardHalDeinitUni
**
** PURPOSE:    Uninitialize the HAL. Reset any local storage and
**             free all resources.
**
** PARAMETERS: VRG_BOARDHAL_INIT_CFG pointer, boad hal cfg modules
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalDeinitUni( VRG_BOARDHAL_INIT_CFG *, int );

/*
*****************************************************************************
** FUNCTION:   boardHalCountryReinit
**
** PURPOSE:    Re-configure boardHal to the new country code.
**
** PARAMETERS: country - new country code
**
** RETURNS:    0 on success.
**
** NOTE:       This function assumes that the DSP has been booted, but
**             none of the underlying dsp interfaces have been initialized yet
**             (i.e. hdsp layer, lhapi layer..etc).  Calling this function
**             after the DSP has been fully initialized will most likely result
**             in problems since the host-dsp communication is now active,
**             and you will need to ensure none of the underlying resources
**             (semaphore, tasks..) are in a locked state for a graceful
**             re-init.
**
*****************************************************************************
*/
int boardHalCountryReinit( VRG_COUNTRY country );

#ifdef PSTN_LIFE_LINE_SUPPORT
/*
*****************************************************************************
** FUNCTION:   boardHalIsPstnLifeLineSupported
**
** PURPOSE:    Determines whether pstn life line is supported for this platform
**
** PARAMETERS: nothing
**
** RETURNS:    VRG_TRUE if pstn life line is supported; VRG_FALSE otherwise
**
** NOTE:
**
*****************************************************************************
*/
VRG_BOOL boardHalIsPstnLifeLineSupported( void );
#endif /* PSTN_LIFE_LINE_SUPPORT */

#ifdef BRCM_NONDIST_BUILD
/*
*****************************************************************************
** FUNCTION:   boardHalPcmEnable
**
** PURPOSE:    Enable PCM ISR
**
** PARAMETERS: none
**
** RETURNS:    nothing
*****************************************************************************
*/
void boardHalPcmEnable( void );

/*
*****************************************************************************
** FUNCTION:   boardHalPcmDisable
**
** PURPOSE:    Disalbe PCM ISR
**
** PARAMETERS: none
**
** RETURNS:    nothing
*****************************************************************************
*/
void boardHalPcmDisable( void );

/*
*****************************************************************************
** FUNCTION:   boardHalIsrEnable
**
** PURPOSE:    Calls either boardHalPcmEnable or boardHalApmEnable depending
**             on the mode
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalIsrEnable( void );

/*
*****************************************************************************
** FUNCTION:   boardHalIsrDisable
**
** PURPOSE:    Calls either boardHalPcmDisable or boardHalApmDisable depending
**             on the mode
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalIsrDisable( void );

/*
*****************************************************************************
** FUNCTION:   boardHalIsrDisable
**
** PURPOSE:    Retrieves the ISR counter
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
int boardHalIntCountGet( void );

#endif

#ifdef __cplusplus
    }
#endif

#endif  /* BOARD_HAL_INIT_H  */
