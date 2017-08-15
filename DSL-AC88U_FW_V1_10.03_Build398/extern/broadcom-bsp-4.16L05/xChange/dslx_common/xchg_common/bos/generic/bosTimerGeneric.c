/****************************************************************************
*
* Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom Corporation
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom Corporation
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosTimerGeneric.c
*
*  @brief   Contains OS independant definitions for the BOS Timer module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_TIMER

#include <bosError.h>
#include <bosTimer.h>
#include <bosTimerPrivate.h>

#if BOS_CFG_RESET_SUPPORT
#include <bosTask.h>
#endif

/**
 * @addtogroup bosTimer
 * @{
 */

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

const char *gBosTimerFunctionName[ BOS_TIMER_NUM_FUNC_ID ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Initializes the contents of @a gBosTimerFunctionName, which is used to
*  translate function IDs into character strings.
*/

void bosTimerInitFunctionNames( void )
{
   gBosTimerFunctionName[ BOS_TIMER_INIT_FUNC_ID ]          = "bosTimerInit";
   gBosTimerFunctionName[ BOS_TIMER_TERM_FUNC_ID ]          = "bosTimerTerm";
   gBosTimerFunctionName[ BOS_TIMER_CREATE_FUNC_ID ]        = "bosTimerCreate";
   gBosTimerFunctionName[ BOS_TIMER_DESTROY_FUNC_ID ]       = "bosTimerDestroy";
   gBosTimerFunctionName[ BOS_TIMER_START_FUNC_ID ]         = "bosTimerStart";
   gBosTimerFunctionName[ BOS_TIMER_RESTART_FUNC_ID ]       = "bosTimerRestart";
   gBosTimerFunctionName[ BOS_TIMER_STOP_FUNC_ID ]          = "bosTimerStop";
   gBosTimerFunctionName[ BOS_TIMER_WAIT_FUNC_ID ]          = "bosTimerWait";

   bosErrorAddFunctionStrings( BOS_TIMER_MODULE, gBosTimerFunctionName, BOS_TIMER_NUM_FUNC_ID );

} /* bosTimerInitFunctioNames */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/***************************************************************************/


/** @} */

#endif   /* BOS_CFG_TIMER */

