/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
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
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: boardHalInit96828pvwm.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent initialization interface
*     for the 96828pvwm.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgCountry.h>
#include <vrgTypes.h>
#include <bcm6828.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <boardparms_voice.h>
#include <boardHalInit.h>
#include <boardHalProv.h>
#include <boardHalInit6828uni.h>
#include <boardHal6828.h>

#ifdef BRCM_NONDIST_BUILD
#include <pcm6828.h>
#endif

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalInit
**
** PURPOSE:    Initialize the HAL.
**
** PARAMETERS: boardHalInitCfg   - configuration for boardHalInit
**
** RETURNS:    0 on success, otherwise error code.
**
*****************************************************************************
*/
int boardHalInit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   int boardCfgModules;
   int status;

   /* Register logging module */
   VRG_LOG_REGISTER( VRG_LOG_MOD_BOARDHAL );

   /*
   ** Setup modules for common initialization sequence.
   */
   boardCfgModules = BOARDCFG6828_MODULE_TPD        |
                     BOARDCFG6828_MODULE_HVGRING    |
                     BOARDCFG6828_MODULE_FASTSLIC   |
                     BOARDCFG6828_MODULE_MSPI;  /* Added for le88276, si3226 */

   /*
   ** Setup other board dependant configuration parameters
   */
   boardHalInitCfg->sharedReferencePin = VRG_FALSE;
   boardHalInitCfg->enhancedControl = VRG_FALSE;

   status = boardHalInitUni( boardHalInitCfg, 
                             boardCfgModules 
#ifdef BRCM_NONDIST_BUILD
                           , PCM6828_DMA_MAX_BUFFER_SIZE
                           , VRG_BOARDHAL_BCM96828
                           , (( boardHalProvGetFlags() & BP_FLAG_DSP_APMHAL_ENABLE ) ? XDRV_TRUE : XDRV_FALSE)
#endif 
                           );
   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   boardHalDeinit
**
** PURPOSE:    Uninitialize the HAL. Reset any local storage and
**             free all resources.
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
int boardHalDeinit( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   int boardCfgModules;
   int rc;

   /*
   ** Setup modules for common deinitialization sequence.
   */
   boardCfgModules = BOARDCFG6828_MODULE_TPD        |
                     BOARDCFG6828_MODULE_HVGRING; 

   /*
   ** Use common 6828 initialization sequence
   */
   rc = boardHalDeinitUni(boardHalInitCfg, boardCfgModules );

   /* Deregister logging module */
   VRG_LOG_DEREGISTER( VRG_LOG_MOD_BOARDHAL );

   return ( rc );
}

