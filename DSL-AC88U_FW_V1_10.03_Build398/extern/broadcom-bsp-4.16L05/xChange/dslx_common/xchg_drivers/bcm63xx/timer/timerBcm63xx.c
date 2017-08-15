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
*    Filename: timerBcm63xx.c
*
****************************************************************************
*    Description:
*
*     Implements an interface to BOS kernel timer module used
*     for registering and starting kernel timers.
*
****************************************************************************/


/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <bosTimer.h>
#include <bosSleep.h>
#include <xdrvTimer.h>
#include <xdrvLog.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicInstall
**
** PURPOSE:    Register a periodic timer.
**
** PARAMETERS: isrHandler  (in)  ISR callback to be invoked periodically.
**             periodMs    (in)  Rate at which ISR will be invoked.
**             userArg     (in)  Passed to ISR callback.
**             timer       (out) Periodic timer struct to intialize.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:       xdrvTimerPeriodicStart() must be called to start the timer,
**             this function simply registers the callback.
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicInstall
(
   XDRV_TIMER_PERIODIC_ISR    isrHandler,
   unsigned int               periodMs,
   void                      *userArg,
   XDRV_TIMER_PERIODIC       *timer
)
{
   BOS_TIMER* tickTimer = malloc(sizeof(BOS_TIMER));

   if ( tickTimer == NULL )
   {
      XDRV_LOG_CRIT(( XDRV_LOG_MOD_XDRV, "Can't allocate memory for timer!"));
      return ( XDRV_STATUS_ERR );
   }

   /* Fill in XDRV_TIMER_PERIODIC timer parameters. This structure
   ** is not used for the BOS timer calls, but is used in the other
   ** xdrvTimer APIs in this file. */
   timer->isrHandler = isrHandler;
   timer->periodMs   = periodMs;
   /* Fill the address of the BOS timer as user argument of XDRV_TIMER_PERIODIC.
   ** This argument will then be used in other xdrvTimerXYZ functions to determine 
   ** the exact BOS timer that is used. */
   timer->userArg    = tickTimer;

   /* Initialize timer parameters */
   tickTimer->data = 0;
   tickTimer->function = (void*) isrHandler;
   if ( bosTimerCreate( tickTimer ) != BOS_STATUS_OK )
   {
      return ( XDRV_STATUS_ERR );
   }

   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicUninstall
**
** PURPOSE:    To de-register a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicUninstall( XDRV_TIMER_PERIODIC *timer )
{
   XDRV_STATUS status = XDRV_STATUS_OK;

   BOS_TIMER* tickTimer = timer->userArg;

   if ( tickTimer != NULL )
   {
      if ( bosTimerDestroy( tickTimer ) != BOS_STATUS_OK )
      {
         status = XDRV_STATUS_ERR;
      }
      free( tickTimer );
   }

   return ( status );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicStart
**
** PURPOSE:    Start a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicStart( XDRV_TIMER_PERIODIC *timer )
{
   BOS_TIMER* tickTimer = timer->userArg;

   /* Start the timer */
   if ( bosTimerStart( tickTimer, timer->periodMs, BOS_TIMER_MODE_REPEAT ) != BOS_STATUS_OK )
   {
      return ( XDRV_STATUS_ERR );
   }

   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerPeriodicStop
**
** PURPOSE:    Stop a periodic timer.
**
** PARAMETERS: timer (mod) Periodic timer struct.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvTimerPeriodicStop( XDRV_TIMER_PERIODIC *timer )
{
   /* Currently the BOS timers for Linux kernel are implemented 
   ** in a way that they have to be rescheduled after every iteration.
   ** The requirement to stop a timer at certain point is not needed
   ** since the timer will stop automatically unlesss it is rescheduled.

   ** If the BOS timer code changes to have automatic rescheduling then
   ** this function should contain code for stopping the timer */

   return ( XDRV_STATUS_OK );
}


/*
*****************************************************************************
** FUNCTION:   xdrvTimerDelayMSec
**
** PURPOSE:    Timer delay in msec.
**
** PARAMETERS: mSec - number of millisecond to delay
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvTimerDelayMSec( unsigned int mSec )
{
   bosSleep( mSec );
}
