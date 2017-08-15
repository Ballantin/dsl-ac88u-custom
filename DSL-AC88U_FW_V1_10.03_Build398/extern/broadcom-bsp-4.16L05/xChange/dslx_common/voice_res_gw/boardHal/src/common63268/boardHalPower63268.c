/***************************************************************************
*    Copyright 2000  Broadcom Corporation
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
*    Filename: boardHalPower63268.c
*
****************************************************************************
*    Description:
*
*     Implements power savings features that are specific to 63268-based
*     reference design.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <boardHalPower.h>
#include <boardHalInit.h>
#include <boardHal63268.h>
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
#endif /* defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD) */   

   /* Sleep for a little while to let DSP settle */
   bosSleep( 50 );
   
#ifndef BRCM_NONDIST_BUILD
   /*
   ** Enable power savings on the DSP thread processor.
   */
   xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_POWERSAVE_CMD_ID, &enable, 0 );
#endif /* BRCM_NONDIST_BUILD */

   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "63268 Board Hal: power savings Enabled!"));

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
#endif /* defined(CONFIG_SMP) || !defined(BRCM_NONDIST_BUILD) */

   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "63268 Board Hal: power savings Disabled!"));


}
