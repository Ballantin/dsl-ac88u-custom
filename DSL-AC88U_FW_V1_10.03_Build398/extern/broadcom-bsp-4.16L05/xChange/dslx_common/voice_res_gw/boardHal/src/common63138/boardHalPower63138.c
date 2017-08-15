/***************************************************************************
*    Copyright 2013  Broadcom Corporation
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
*    Filename: boardHalPower63138.c
*
****************************************************************************
*    Description:
*
*     Implements power savings features that are specific to 63138-based
*     reference design.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <vrgLog.h>
#include <boardHalPower.h>
#include <boardHalInit.h>
#include <boardHal63138.h>
#include <boardHalDspTask.h>
#include <bosSleep.h>

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
   /* Halt hausware task */
   boardHalDspTaskHalt();

   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "63138 Board Hal: power savings Enabled!"));
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
   /* Resume hausware task */
   boardHalDspTaskResume();

   VRG_LOG_DBG((VRG_LOG_MOD_BOARDHAL, "63138 Board Hal: power savings Disabled!"));
}
