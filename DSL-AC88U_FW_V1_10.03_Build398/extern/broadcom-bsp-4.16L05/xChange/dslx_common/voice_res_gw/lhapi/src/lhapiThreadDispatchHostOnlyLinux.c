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
*
*    Filename: lhapiThreadDispatchHostOnlyLinux.c
*
****************************************************************************
*    Description:
*
*     Implements functions related to dispatching LDX hausware threads
*     when they are ready to run.
*
*     This file is Linux specific since it uses tasklets to run the hausware
*     threads. The concept of scheduling tasklets is not generically supported
*     by BOS (since other operating systems don't have tasklets); therefore,
*     we have provided this OS-specific source file. Yuck.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>
#include <knl_framesync.h>

/* This file is Linux specific. */
#if ( BOS_OS_LINUXKERNEL )

/* Linux specific headers. */
#include <linux/interrupt.h>

#include <xchgAssert.h>
#include "lhapiPrivate.h"
#include <lhapiExtension.h>

#include <xdrvGlobalInterrupt.h>

#ifdef CONFIG_SMP
#include <boardHalDsp.h>
#endif /* CONFIG_SMP */


/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* ---- Private Variables ------------------------------------------------ */
/* 
** If BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET is defined, the Hausware ready threads will be run from tasklet.
** If BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET not defined, the Hausware ready threads will be run from 
** the same context as lhapiProcessEntry (either ISR or tasklet depending
** how lhapiProcessEntry was scheduled).
*/
#ifndef CONFIG_SMP
#ifdef BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET
DECLARE_TASKLET( hausFrameTasklet, lhapiKNL_RunReadyThreads, 0 );
#endif /* BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET */
#endif /* CONFIG_SMP */ 

/* ---- Private Function Prototypes -------------------------------------- */

/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchInit.
**
** PURPOSE:    Init lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchInit( void )
{
   /* Nothing to do for now. */
}


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchDeinit.
**
** PURPOSE:    De-initialize lhapi thread dispatch sub-module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchDeinit( void )
{
   /* Nothing to do for now. */
}


/*
*****************************************************************************
** FUNCTION:   lhapiKNL_RunReadyThreads.
**
** PURPOSE:    Dispatches hausware threads by calling KNL_RunReadyThreads().
**             Note that we must save and restore the state of some 
**             important registers prior to and after calling KNL_RunReadyThreads(),
**             respectively.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/

void lhapiKNL_RunReadyThreads( unsigned long a )
{
   XDRV_REGISTER_SET registerSet;
   (void) a;

   /* Save the state of some important registers */
   xdrvSaveRegisterState( &registerSet );
   
   KNL_RunReadyThreads();
   
   /* Restore the values of the registers saved above */
   xdrvRestoreRegisterState( &registerSet );
   
#ifdef FEAT_HAL_CAPTURE_TO_FILE 
   /* Trigger the halcapture task - This is for SMP and NON-DIST cases */
   lhapiHalCaptureThreadDispatch();
#endif     
}


/*
*****************************************************************************
** FUNCTION:   lhapiThreadDispatchHostOnly
**
** PURPOSE:    This function is called by hausware when it has threads that
**             are ready to run. It is the application's responsibility to
**             dispatch the threads by calling lhapiKNL_RunReadyThreads(). Note that
**             lhapiKnlScheduleReadyThreads may be called in ISR context. The
**             application must decide whether to invoke lhapiKNL_RunReadyThreads()
**             in ISR context or task context. If it invokes it in ISR context,
**             interrupts MUST be enabled prior to calling lhapiKNL_RunReadyThreads().
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiThreadDispatchHostOnly( void )
{
#ifdef CONFIG_SMP
   DspTaskSchedule();
#else
#ifdef BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET
   /*
   ** Schedule a linux tasklet that will run the hausware threads. This is
   ** necessary to transition from the current ISR context.
   */
   tasklet_hi_schedule( &hausFrameTasklet );
#else 
   lhapiKNL_RunReadyThreads(0); 
#endif /* BRCM_RUN_HAUSWARE_THREADS_FROM_TASKLET */   
#endif /* CONFIG_SMP */
}



/*
*****************************************************************************
** FUNCTION:   lhapi_ProcessEntrySync
**
** PURPOSE:    A wrapper for void lhapProcessEntrySync(void)
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapi_ProcessEntrySync( unsigned long arg )
{
    (void) arg;

    lhapiProcessEntrySync();
}

#endif   /* ( BOS_OS_LINUXKERNEL ) */
