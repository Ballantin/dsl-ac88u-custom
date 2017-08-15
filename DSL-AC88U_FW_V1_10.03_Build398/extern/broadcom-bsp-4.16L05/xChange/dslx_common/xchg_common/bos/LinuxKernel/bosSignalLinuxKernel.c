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
*  Filename: bosSignalLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosSignalLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Signal Module
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>
#include <linux/sched.h>      /* for force_sig() */
#include <linux/signal.h>     /* for signal handling */
#include <asm/current.h>      /* for accessing current process */

#include <kernelMainWrapper.h>

#include <string.h>

#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosLog.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define BOS_SIG_GROUP_SIZE  4 /* Maximum number of signals in a group */

/* Structure for grouping signals */
typedef struct
{
   int signalGroup[BOS_SIG_GROUP_SIZE]; /* Table for holding signals in a group */
   BOS_STATUS status;                   /* Corresponding BOD_STATUS for signal group */
} BOS_SIGNAL_GROUP;

/* Defines all the signal groups sorted by priority (HIGH -> LOW) */
static BOS_SIGNAL_GROUP bosSignalGroups[] =
{
   /* NOTE: Signal group can be zero-terminated since Linux signals start at 1 */
   { { SIGKILL, SIGTERM, 0, }, BOS_STATUS_EXIT  }, /* Exit signal group */
   { { SIGSTOP, SIGCONT, 0, }, BOS_STATUS_DEBUG }, /* Debug signal group */
   { { 0, },                   BOS_STATUS_ERR   }  /* NULL group */
};

/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** Function Name: bosSignalMapSigToStatus
** Description  : Compares the current task's pending signals to the
**                various signal groups to determine which BOS_STATUS to
**                send to the caller.
** Returns      : BOS_STATUS_EXIT  = Exit group signal pending
**                BOS_STATUS_DEBUG = Debug group signal pending
**                BOS_STATUS_ERR   = Other signals pending
*****************************************************************************
*/
BOS_STATUS bosSignalMapSigToStatus( void )
{
   int i, j;
   unsigned long state = 0;
   BOS_BOOL found = BOS_FALSE;
   BOS_STATUS status = BOS_STATUS_ERR;

   spin_lock_irqsave(&current->sighand->siglock, state);

   /* Iterate through all signal groups */
   for(i = 0; !found && i < BOS_SIG_GROUP_SIZE && bosSignalGroups[i].signalGroup[0]; i++)
   {
      /* Iterate through all the signals in a signal group */
      for(j = 0; bosSignalGroups[i].signalGroup[j]; j++)
      {
         /* Check if a signal belonging to this signal group is pending */
         if ( sigismember(&current->pending.signal, bosSignalGroups[i].signalGroup[j]) ||
              sigismember(&current->signal->shared_pending.signal, bosSignalGroups[i].signalGroup[j]) )
         {
            status = bosSignalGroups[i].status;
            found = BOS_TRUE;
            break;
         }
      }
   }

   spin_unlock_irqrestore(&current->sighand->siglock, state);

   return status;
}
