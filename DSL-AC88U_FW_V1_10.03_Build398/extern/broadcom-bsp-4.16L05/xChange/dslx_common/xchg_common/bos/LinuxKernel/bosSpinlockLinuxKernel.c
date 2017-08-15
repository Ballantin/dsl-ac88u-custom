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
*  Filename: bosSpinlockLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosSpinlockLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Spinlock Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <kernelMainWrapper.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/threads.h>
#include <linux/sched.h>
#include <linux/version.h>

#include <asm/current.h>

#include <string.h>  
#include <bosCfg.h>

#if BOS_CFG_SPINLOCK

#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosLog.h>
#include <bosSpinlockLinuxKernel.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */


/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */

static unsigned int bosSpinlockGetCaller( void );

/*
*****************************************************************************
 * Function Name: bosSpinlockCreate
 * Description  : Creates a BOS spinlock.
 * Returns      : BOS_STATUS_OK     = successful
 *                BOS_STATUS_ERR = failure
*****************************************************************************
*/
BOS_STATUS bosSpinlockCreate( BOS_SPINLOCK *spinLock )
{
   /* Sanity check */
   if ( !spinLock )
   {
      blogStrErr(( gBosBlog, "bosSpinlockCreate() - invalid arguments"));
      return BOS_STATUS_ERR;
   }

   spinLock->owner = 0;
   spinLock->nesting = 0;

   spin_lock_init( &spinLock->lock );

   return BOS_STATUS_OK;
} /* bosSemCreate */


/*
*****************************************************************************
 * Function Name: bosSpinlockDestroy
 * Description  : Destroys a BOS spinlock.
 * Returns      : BOS_STATUS_OK     = successful
 *                BOS_STATUS_ERR = failure
*****************************************************************************
*/
BOS_STATUS bosSpinlockDestroy( BOS_SPINLOCK *spinLock )
{
   /* Sanity check */
   if ( !spinLock )
   {
      blogStrErr(( gBosBlog, "bosSpinlockDestroy() - invalid arguments"));
      return BOS_STATUS_ERR;
   }

   spinLock->owner = 0;
   spinLock->nesting = 0;

   return BOS_STATUS_OK;
} /* bosSemDestroy */


/*
*****************************************************************************
 * Function Name: bosSpinlockAcquire
 * Description  : Acquires a spinlock. The function also disables local
 *                interrupts and saves the CPU flags in the "flags" argument.
 * Returns      : BOS_STATUS_OK     = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosSpinlockAcquire( BOS_SPINLOCK *spinLock, unsigned long *flags )
{
   unsigned long state = 0;
   unsigned int caller = 0;

   /* Sanity check */
   if ( !spinLock || !flags )
   {
      blogStrErr(( gBosBlog, "bosSpinlockAcquire() - invalid arguments"));
      return BOS_STATUS_ERR;
   }
   
   caller = bosSpinlockGetCaller();
   if ( spinLock->owner == caller )
   {
      spinLock->nesting++;
   }
   else
   {
      /* Acquire the spinlock */
      spin_lock_irqsave( &spinLock->lock, state );
      spinLock->owner = caller;
   }

   /* Copy CPU flags to the flags argument */
   *flags = state;

   return BOS_STATUS_OK;
}

/*
*****************************************************************************
 * Function Name: bosSpinlockAcquire
 * Description  : Releases a spinlock. The function also reenables local
 *                interrupts and copies the "flags" argument to the CPU flags.
 * Returns      : BOS_STATUS_OK     = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosSpinlockRelease( BOS_SPINLOCK *spinLock, unsigned long flags )
{
   unsigned int caller = 0;
   
   /* Sanity check */
   if ( !spinLock )
   {
      blogStrErr(( gBosBlog, "bosSpinlockAcquire() - invalid arguments"));
      return BOS_STATUS_ERR;
   }
   
   caller = bosSpinlockGetCaller();
   if ( spinLock->owner == caller )
   {
      if ( spinLock->nesting > 0 )
      {
         spinLock->nesting--;
      }
      else
      { 
         /* Release the spinlock */
         spinLock->owner = 0;    
         spin_unlock_irqrestore( &spinLock->lock, flags );
      }
   }

   return BOS_STATUS_OK;
}

/*
*****************************************************************************
 * Function Name: bosSpinlockGetCaller
 * Description  : Get the ID of the context trying to acquire spinlock.
 * Returns      : PID if the caller is a task or PID_MAX_LIMIT+1 of the 
 *                caller is the PCM/APM ISR.
 * Note         : Here we assume that PCM/APM DMA ISR is the only interrupt 
 *                context accessing the critical section.
*****************************************************************************
*/
static unsigned int bosSpinlockGetCaller( void )
{
   return (!in_interrupt() ? current->pid : PID_MAX_LIMIT+1);
}

#endif   /* BOS_CFG_SPINLOCK */
