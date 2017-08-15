/****************************************************************************
*
*  Copyright (c) 2001-2008 Broadcom Corporation
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
*
*  Filename: bosTimerLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosTimerLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Timer Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_TIMER

#include <string.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosTimer.h>
#include <bosTimePrivate.h>
#include <bosTimerPrivate.h>
#include <bosLinuxKernel.h>
#include <linux/jiffies.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosTimerInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   bosTimerInit
**
** PURPOSE:    Initializes the timer module.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_STATUS_OK
**
** NOTE:       
**
*****************************************************************************
*/
BOS_STATUS bosTimerInit( void )
{
   BOS_ASSERT( !gBosTimerInitialized );

   bosTimeInitFunctionNames();

   gBosTimerInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosTimeInit */

/*
*****************************************************************************
** FUNCTION:   bosTimerTerm
**
** PURPOSE:    Deinitializes the timer module.
**
** PARAMETERS: None.
**
** RETURNS:    BOS_STATUS_OK
**
** NOTE:       
**
*****************************************************************************
*/
BOS_STATUS bosTimerTerm( void )
{
   BOS_ASSERT( gBosTimerInitialized );

   gBosTimerInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosTimeTerm */

/*
*****************************************************************************
** FUNCTION:   bosTimerCreate
**
** PURPOSE:    Initializes a new linux kernel timer.
**
** PARAMETERS: timer - Pointer to the timer to be initialized
**
** RETURNS:    BOS_STATUS_OK
**
** NOTE:       
**
*****************************************************************************
*/
BOS_STATUS bosTimerCreate( BOS_TIMER *timer )
{
   init_timer(timer);

   return ( BOS_STATUS_OK );

} /* bosTimerCreate */

/*
*****************************************************************************
** FUNCTION:   bosTimerStart
**
** PURPOSE:    Starts a linux kernel timer by registering it with the kernel.
**
** PARAMETERS: timer         - Pointer to the timer structure
**             timeoutMsec   - Expiry time in milliseconds
**             mode          - Only support BOS_TIMER_MODE_REPEAT
**
** RETURNS:    BOS_STATUS_OK
**        
** NOTE:       Please note that this function has three arguments viz: The 
**             bos timer structure, expiry time, and timer mode. The Linux 
**             kernel timer does not make use of mode BUT needs the timer 
**             structure to include the function to be invoked when the timer 
**             expires and the argument to be passed to that function. We will 
**             stick with this standard function signature to be consistent 
**             with the BOS API. It is important to manually set the function 
**             entry point and argument in the timer structure before calling 
**             this function. Always use BOS_TIMER_MODE_REPEAT as the value of 
**             the mode argument.
**
*****************************************************************************
*/
BOS_STATUS bosTimerStart
(
   BOS_TIMER       *timer,
   unsigned int    timeoutMsec,
   BOS_TIMER_MODE  mode
)
{
   (void)mode;

   if ( timer_pending(timer) )
   {
      /* Delete timer if it was already active before creating it */
      del_timer(timer);
   }
   timer->expires = jiffies + BOS_MS_TO_TICKS(timeoutMsec);   
   add_timer(timer);    

   return ( BOS_STATUS_OK );

} /* bosTimerStart */

/*
*****************************************************************************
** FUNCTION:   bosTimerDestroy
**
** PURPOSE:    Stops and destroys the linux kernel timer.
**
** PARAMETERS: timer - Pointer to the timer to be destroyed
**
** RETURNS:    BOS_STATUS_OK
**
** NOTE:       
**
*****************************************************************************
*/
BOS_STATUS bosTimerDestroy( BOS_TIMER *timer )
{
#if 0
   BOS_ASSERT( gBosTimerInitialized );
#endif

   BOS_ASSERT( timer != NULL );
   del_timer(timer);

   return ( BOS_STATUS_OK );

} /* bosTimerDestroy */

#endif   /* BOS_CFG_TIMER */
