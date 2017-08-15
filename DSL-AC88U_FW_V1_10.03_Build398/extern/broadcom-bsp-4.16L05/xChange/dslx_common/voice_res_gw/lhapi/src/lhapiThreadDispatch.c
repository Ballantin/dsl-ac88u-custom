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
*    Filename: lhapiThreadDispatch.c
*
****************************************************************************
*    Description:
*
*     Implements functions related to dispatching LDX hausware threads
*     when they are ready to run.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <lhapi.h>
#include <knl_framesync.h>
#include <vrgTypes.h>
#include <boardHalDsp.h>
#include "lhapiPrivate.h"
#include <lhapiExtension.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   lhapiKnlScheduleReadyThreads
**
** PURPOSE:    This function is called by hausware when it has threads that
**             are ready to run. It is the application's responsibility to
**             dispatch the threads by calling KNL_RunReadyThreads(). Note that
**             lhapiKnlScheduleReadyThreads may be called in ISR context. The
**             application must decide whether to invoke KNL_RunReadyThreads()
**             in ISR context or task context. If it invokes it in ISR context,
**             interrupts MUST be enabled prior to calling KNL_RunReadyThreads().
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiKnlScheduleReadyThreads( void )
{

   /*
   ** Dispatch all ready to run hausware threads.
   */
   if ( lhapiIsHauswareDistributed() )
   {
      /*
      ** Hausware is distributed, and the synchronous lower-half services are
      ** running on the DSP. We are already running in task context, so we
      ** can directly call the hausware kernel to schedule tasks that are
      ** ready to run.
      */
      KNL_RunReadyThreads();
      
#ifdef FEAT_HAL_CAPTURE_TO_FILE 
      /* Trigger the halcapture task - This is for distributed case */
      lhapiHalCaptureThreadDispatch();
#endif     
      
   }
   else
   {
      /*
      ** Hausware is not distributed, it is running entirely on the host
      ** processor. This function will likely be called in ISR context. We
      ** can continue to run in ISR context, or switch to task or some other
      ** intermediate type context to run the hausware kernel threads. This
      ** decision is really application and OS specific. If we continue to
      ** run in ISR context, we need to remember to re-enable interrupts
      ** prior to invoking the hausware framework.
      */
      lhapiThreadDispatchHostOnly();
   }
}
