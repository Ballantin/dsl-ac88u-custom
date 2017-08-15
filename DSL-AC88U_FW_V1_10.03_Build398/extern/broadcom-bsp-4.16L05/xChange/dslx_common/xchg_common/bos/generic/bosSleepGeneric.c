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
*  @file    bosSleepGeneric.c
*
*  @brief   Contains OS independant definitions for the BOS Sleep module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_SLEEP

#include <bosError.h>
#include <bosSleep.h>
#include <bosSleepPrivate.h>

#if BOS_CFG_RESET_SUPPORT
#include <bosTask.h>
#endif

/**
 * @addtogroup bosSleep
 * @{
 */

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

const char *gBosSleepFunctionName[ BOS_SLEEP_NUM_FUNC_ID ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Initializes the contents of @a gBosSleepFunctionName, which is used to
*  translate function IDs into character strings.
*/

void bosSleepInitFunctionNames( void )
{
   gBosSleepFunctionName[ BOS_SLEEP_INIT_FUNC_ID ]    = "bosSleepInit";
   gBosSleepFunctionName[ BOS_SLEEP_TERM_FUNC_ID ]    = "bosSleepTerm";
   gBosSleepFunctionName[ BOS_SLEEP_SLEEP_FUNC_ID ]   = "bosSleep";

   bosErrorAddFunctionStrings( BOS_SLEEP_MODULE, gBosSleepFunctionName, BOS_SLEEP_NUM_FUNC_ID );

} /* bosSleepInitFunctioNames */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/***************************************************************************/

#if BOS_CFG_RESET_SUPPORT

BOS_STATUS bosSleepR( BOS_TIME_MS sleeptime )
{
   BOS_STATUS retval;

   while ( !bosTaskIsResetPending() )
   {
      if ( sleeptime <= BOS_CFG_RESET_GRANULARITY )
      {
         /* Sleeptime is less than the reset granularity, simply sleep normally */
         return ( bosSleep( sleeptime ) );
      }
      else
      {
         /* Sleeptime is more than reset granularity, decrement sleeptime remaining
         ** and sleep for the maximum allowed before checking for a reset */
         if ( sleeptime != BOS_WAIT_FOREVER )
         {
            sleeptime -= BOS_CFG_RESET_GRANULARITY;
         }
         retval = bosSleep( BOS_CFG_RESET_GRANULARITY );
         if ( retval != BOS_STATUS_OK )
         {
            /* Anything but the OK to this chunk of the sleep is returned to the caller */
            return (retval);
         }
      }
   }

   return ( BOS_STATUS_RESET );
}

#endif   /* BOS_CFG_RESET_SUPPORT */

/** @} */

#endif   /* BOS_CFG_SLEEP */

