/****************************************************************************
*
* Copyright � 2008 Broadcom Corporation
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
*    Filename: boardHalPower6828.c
*
****************************************************************************
*    Description:
*
*     Implements power savings features that are specific to 6828-based
*     reference design.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <boardHalPower.h>
#include <boardHal6828.h>
#include <boardHalDspTask.h>
#include <bosSleep.h>

#ifndef BRCM_NONDIST_BUILD
#include <itpcSharedCmds.h>
#endif /* BRCM_NONDIST_BUILD */

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalPowerSavingsInit
**
** PURPOSE:    Configure power savings on DSP.
**
** PARAMETERS: reducedVoltage - 1 - VRX needs to be shutdown for power save mode.
**                              0 - VRX is enabled in power save mode.
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void boardHalPowerSavingsInit( int reducedVoltage )
{
#ifndef BRCM_NONDIST_BUILD
   /*
   ** Configure power savings on the DSP thread processor.
   */
   xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_POWERSAVE_VRX_SHUTDOWN_CMD_ID, &reducedVoltage, 0 );
#endif /* BRCM_NONDIST_BUILD */

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "6828 power savings initialized!"));

}

/*
*****************************************************************************
** FUNCTION:   boardHalPowerSavingsEnable
**
** PURPOSE:    Disable appropriate peripherals and/or external devices to
**             reduce power consumption.
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void boardHalPowerSavingsEnable( void )
{
#ifndef BRCM_NONDIST_BUILD
   int enable = 1;
#endif /* BRCM_NONDIST_BUILD */

#if defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD)
   /* Halt hausware task */
   boardHalDspTaskHalt();
#endif /* define(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD) */   

   /* Sleep for a little while to let DSP settle */
   bosSleep( 50 );
   
#ifndef BRCM_NONDIST_BUILD
   /*
   ** Enable power savings on the DSP thread processor.
   */
   xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_POWERSAVE_CMD_ID, &enable, 0 );
#endif /* BRCM_NONDIST_BUILD */

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "6828 power savings Enabled!"));

}


/*
*****************************************************************************
** FUNCTION:   boardHalPowerSavingsDisable
**
** PURPOSE:    Resume full-power mode.
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
*****************************************************************************
*/
void boardHalPowerSavingsDisable( void )
{
#ifndef BRCM_NONDIST_BUILD
   int enable = 0;

   /*
   ** Disable power savings on the DSP thread processor.
   */
   xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_POWERSAVE_CMD_ID, &enable, 0 );
#endif /* BRCM_NONDIST_BUILD */

#if defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD)
   /* Resume hausware task */
   boardHalDspTaskResume();
#endif /* define(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD) */      

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "6828 power savings Disabled!"));


}
