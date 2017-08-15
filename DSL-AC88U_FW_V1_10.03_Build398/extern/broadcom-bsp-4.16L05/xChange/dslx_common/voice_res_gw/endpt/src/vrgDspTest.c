/****************************************************************************
*
* Copyright © 2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: vrgDspTest.c
*
****************************************************************************
*    Description:
*
*     This file implements various DSP tests initiated by the host
*
****************************************************************************/

#ifdef G729_DSPTEST_APP

/* ---- Include Files ---------------------------------------------------- */

#include <bosTask.h>
#include <bosSleep.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgTest.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Variables, Constants and Types --------------------------- */

static BOS_TASK_ID dspTest1ProcessTaskId;

/* ---- Private Function Prototypes -------------------------------------- */

static void dspTest1TaskMain( BOS_TASK_ARG taskArg );

/* ---- Functions -------------------------------------------------------- */




/*
*****************************************************************************
** FUNCTION:   dspTestInit();
**
** PURPOSE:    Initialize test on the host that will affect the DSP
**             by using common resources (e.g. D-cache)
**
** PARAMETERS: 
**
** RETURNS:
**
*****************************************************************************
*/

void dspTestInit( void )
{
   bosTaskCreateEx( "DSPTEST1",
                    (8 * 1024),
                    BOS_TASK_CLASS_MED,
                    NULL,
                    dspTest1TaskMain,
                    NULL,
                    0,
                    &dspTest1ProcessTaskId );


   if( ! dspTest1ProcessTaskId )
   {
      VRG_LOG_CRIT(( VRG_LOG_MOD_EPT, "Error creating DSPTEST1 task" ));
   }

   return;
}


static unsigned int dtSleepTime=5000;
static void dspTest1TaskMain( BOS_TASK_ARG taskArg )
{
   int i;

   VRG_LOG_INFO(( VRG_LOG_MOD_EPT, "**** &dtSleepTime = 0x%08X", (unsigned int)&dtSleepTime ));

   while( 1 )
   {
      if ( bosTaskIsResetPending() )
      {
         bosTaskResetAck();
         return;
      }

      bosSleep(dtSleepTime);
      for (i = 0; i < 250; i++)
      {
         vrgDCacheInvalidate();
      }
   }
}

#endif /* G729_DSPTEST_APP */
