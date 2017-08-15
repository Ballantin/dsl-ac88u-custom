/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
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
*    Filename: vrgEndptIdleWd.c
*
****************************************************************************
*    Description:
*
*      This file contains the implementation of the voice IDLE status 
*      watchdog task.  
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

/* These are needed for the randomization functions  */
#include <kernelMainWrapper.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <asm/uaccess.h>
#if __arm__
#include <linux/cpufreq.h>
#endif

#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <vrgEndptCfg.h>
#include <hapi_net.h>                  /* Net VHD API */
#include <hapi_pte.h>                  /* PTE Service */
#include <hapi_t38.h>                  /* FXR Service */
#include <hapi_faxr.h>                 /* FXR Service */
#include <hapi_cdis.h>                 /* CDIS Service */
#include <hapi_clidtx.h>               /* CLIDTX Service */
#include <hapi_rtp.h>                  /* RTP Service */
#include <endpt.h>                     /* Endpoint include file */
#include <codec.h>                     /* codec map */
#include <ctype.h>
#include <bosSem.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <bosSocket.h>
#if VRG_ENDPT_CFG_USE_HFE
#include <bosTimer.h>
#endif

#if BRCM_DUAL_3341
#include <hapi.h>
#endif

#include <hapi_rm.h>
#include <hapi_tone.h>
#include <hapi_gvad_defs.h>
#include <hapi_wrap_defs.h>
#include <hapi_gain.h>
#include <haus_hapi.h>
#include <hapi_swb.h>
#include <hapi_hec.h>

#include <hdspCmd.h>                   /* DSP interface */
#include <hdspTone.h>                  /* DSP tone interface */
#include <vrgTone.h>
#include <vrgClass.h>
#include <hapi_cmtd.h>                 /* CMTD Service */
#include <hapi_dtas.h>                 /* DTAS Service */
#include <hdspInit.h>
#include <hdspVhd.h>
#include <hdspRm.h>
#include <hdspHal.h>
#include <hdspIcp.h>
#include <hdspVhdStateControl.h>

#include <boardHalInit.h>
#include <boardHalApm.h>
#include <boardHalCas.h>
#include <boardHalDsp.h>
#include <boardHalDaa.h>
#include <boardHalSlic.h>
#include <boardHalPower.h>
#include <boardHalDect.h>
#include <boardHalProv.h>
#include <vrgEndpt.h>                  /* VRG local header file */
#include <vrgProv.h>                   /* Endpoint provisioning */
#include "vrgEndptGlobe.h"             /* Endpoint around the "Globe" interface */
#include <classStm.h>                  /* CLASS state machine */
#include <casCtl.h>                    /* CAS signalling support */
#include <vrgRing.h>                   /* Provisioned Ring Cadences */
#include <cmtdCtl.h>
#include <pstnCtl.h>                   /* PSTN call setup support */
#include <xdrvDaa.h>                   /* DAA driver interface */
#if VRG_ENDPT_CFG_USE_TPD
#include <tpdCtl.h>                    /* TPD testing support */
#endif
#if VRG_ENDPT_CFG_USE_MLT
#include <mltApi.h>
#endif
#if VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT
#include <dectCtl.h>                /* DECT control interface */
#include <xdrvDect.h>
#include <dectLineIf.h>
#include <dectCallMgr.h>
#endif /* VRG_ENDPT_CFG_DECTCALLMGR_SUPPORT */

#if VRG_ENDPT_CFG_DECTTEST_SUPPORT
#include "dectTest.h"
#endif

#include <xdrvApm.h>                   /* APM driver interface */
#include <xdrvSlic.h>                  /* SLIC driver interface */
#include <debug.h>                     /* Debug module interface */
#include <heartbeat.h>                 /* Heartbeat monitoring module */

#include <boardHalIpc.h>
#include <lhapiExtension.h>

#include <str.h>
#include <memCircBuf.h>
#include <bosTime.h>

#include <leds.h>                      /* LED control wappers */

#include "vrgEndptPrivate.h"


#if BRCM_TRACK_IDLE_STATUS
/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

/* Enable or disable tracking of idle status (for power management) */
#define IDLESTATUS_WD_SLEEP_TIME          2000
/* Interval to actually sleep and check reset */
#define IDLESTATUS_WD_SLEEP_INTERVAL      100

#define VOICE_CPUFREQ_MIN                 650000

/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/

BOS_TASK_ID wdTaskId;

/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

static void idleStatusWdTaskMain( BOS_TASK_ARG taskArg );
static void idleStatusWdTaskInitCB( BOS_TASK_ARG taskArg );
static void idleStatusWdTaskDeinitCB( BOS_TASK_ARG taskArg );

#if ( PWRMNGT_ENABLED || IDLEVOICE_ENABLED )
static VRG_BOOL idleStateWdAllLinesIdle( void );
static VRG_BOOL idleStateWdAllConnectionsIdle( void );
#endif



/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/

extern BOS_SEM   vrgIdleWdSyncSem;
extern BOS_SEM   vrgIdleWdSleepSem;
extern void halDmaInterruptDisable ( void );



/*
 ***************************************************************************
 * EXPORTED VARIABLES
 ***************************************************************************
*/
volatile int isVoiceIdle = 0; /* Need to assume voice is going when system first came up */

/*****************************************************************************
*  FUNCTION:   idleStatusWatchDogInit
*
*  PURPOSE:    Initializes the IDLE status watch dog task.
*
*  PARAMETERS: NONE
*
*  RETURNS:    NONE
*
*  NOTE:
*****************************************************************************/
void idleStatusWatchDogInit(void)
{
   if ( BOS_STATUS_OK != bosTaskCreateEx(    "ISTW",
                                             (8 * 1024),
                                             BOS_TASK_CLASS_MED,
                                             idleStatusWdTaskInitCB,
                                             idleStatusWdTaskMain,
                                             idleStatusWdTaskDeinitCB,
                                             0,
                                             &wdTaskId ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "ERROR: Cannot create idle status Watchdog task "));
   }
}




/*****************************************************************************
*  FUNCTION:   idleStatusWatchDogDeinit
*
*  PURPOSE:    De-initializes the IDLE status watch dog task.
*
*  PARAMETERS: NONE
*
*  RETURNS:    NONE
*
*  NOTE:
*****************************************************************************/
void idleStatusWatchDogDeinit(void)
{
   if ( wdTaskId )
   {
      bosTaskProceedToShutdown( &wdTaskId );

      /* Wakeup idle watchdog task in case it is sleeping */
      bosSemGive (&vrgIdleWdSleepSem);

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &wdTaskId );
   }

   VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "Idle status watchdog shutdown complete!" ));
}


/*****************************************************************************
*  FUNCTION:   idleStatusWdTaskMain
*
*  PURPOSE:    Main function for the IDLE status watch dog task. Periodically
*              checks the endpoint hook states and connection states to 
*              determine the IDLE status of the endpoint.
*
*  PARAMETERS: NONE
*
*  RETURNS:    NONE
*
*  NOTE:
*****************************************************************************/
static void idleStatusWdTaskMain( BOS_TASK_ARG taskArg )
{
   VRG_UINT32 ticks;
   BOS_STATUS status;
#if ( PWRMNGT_ENABLED || IDLEVOICE_ENABLED)
   VRG_BOOL allLinesIdle;
   VRG_BOOL allConnectionsIdle;
#endif

#if __arm__
   /* Make a call to the CPU governor requesting a minimum operating frequency */
   cpufreq_minimum_reserve(VOICE_CPUFREQ_MIN);
#endif

   while ( 1 )
   {
      for ( ticks = 0; ticks * IDLESTATUS_WD_SLEEP_INTERVAL < IDLESTATUS_WD_SLEEP_TIME; ticks++ )
      {
         status = bosSleep( IDLESTATUS_WD_SLEEP_INTERVAL );
         if ( status == BOS_STATUS_RESET )
         {
            /* The task has been notified that a reset is pending. 
             ** Acknowledge the notification and then block until the task is resumed.
             */
            bosTaskResetAck();

#if __arm__
            cpufreq_minimum_unreserve(VOICE_CPUFREQ_MIN);
#endif
            /* The task should stop running by exiting from this main function */
            return;
         }
         else if ( status == BOS_STATUS_EXIT )
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Exiting task ISTW" ));
#if __arm__
            cpufreq_minimum_unreserve(VOICE_CPUFREQ_MIN);
#endif
            return;
         }
      }

#if (PWRMNGT_ENABLED || IDLEVOICE_ENABLED)
      /* Need to serialize with dispatcher task, so that this idle watchdog task does not report wrong state  */
      status = bosSemTake (&vrgIdleWdSyncSem);  
      allLinesIdle = idleStateWdAllLinesIdle();
      allConnectionsIdle = idleStateWdAllConnectionsIdle();
      isVoiceIdle = ( allLinesIdle && allConnectionsIdle );

      if (isVoiceIdle ) 
      {
#if IDLEVOICE_ENABLED
         /* Now it is OK to turn off the voice (DMA) interrupts */
         halDmaInterruptDisable();
#endif
         bosSemGive (&vrgIdleWdSyncSem);

#if __arm__
         /* Since voice is no longer running, unreserve the min CPU */
         cpufreq_minimum_unreserve(VOICE_CPUFREQ_MIN);
#endif
         /* Now idle watchdog can go to sleep */
         bosSemTake (&vrgIdleWdSleepSem);  
#if __arm__
         cpufreq_minimum_reserve(VOICE_CPUFREQ_MIN);
#endif
      }
      else
      {
         bosSemGive (&vrgIdleWdSyncSem);        
      }
#endif /* PWRMNGT_ENABLED || IDLEVOICE_ENABLED */
   }
}


/*****************************************************************************
*  FUNCTION:   idleStatusWdTaskInitCB
*
*  PURPOSE:    Init callback for the IDLE status watchdog task.
*
*  PARAMETERS: NONE
*
*  RETURNS:    NONE
*
*  NOTE:
*****************************************************************************/
static void idleStatusWdTaskInitCB( BOS_TASK_ARG taskArg )
{
}


/*****************************************************************************
*  FUNCTION:   idleStatusWdTaskDeinitCB
*
*  PURPOSE:    De-init callback for the IDLE status watchdog task.
*
*  PARAMETERS: NONE
*
*  RETURNS:    NONE
*
*  NOTE:
*****************************************************************************/
static void idleStatusWdTaskDeinitCB( BOS_TASK_ARG taskArg )
{
}


#if ( PWRMNGT_ENABLED || IDLEVOICE_ENABLED )
/*****************************************************************************
*  FUNCTION:   idleStateWdAllLinesIdle
*
*  PURPOSE:    Checks if all voice lines are idle. This function also monitors
*              the states of the VHDs associated with the lines and disables 
*              them if their timout increment expires.
*
*  PARAMETERS: NONE
*
*  RETURNS:    VRG_TRUE if all lines are IDLE, VRG_FALSE otherwise
*
*  NOTE:
*****************************************************************************/
static VRG_BOOL idleStateWdAllLinesIdle( void )
{
   int i;
   VRG_SINT16 status;
   VRG_BOOL ret = VRG_TRUE;
   VRG_UINT16 increment = IDLESTATUS_WD_SLEEP_TIME/1000;  /* Value in seconds */

   for ( i = 0; i < VRG_ENDPT_CFG_NUM_ENDPT; i++ )
   {
      /* Check if valid endpoint exists */
      VRG_ENDPT* eptState = GetEndptState( i );
      if ( eptState != NULL )
      {
         /* If a line is OFFHOOK, all lines are not IDLE */
         if ( eptState->offHook == VRG_TRUE )
         {
            ret = VRG_FALSE;
         }
         else
         {
            /* 
            ** The line VHD must be registered with the VHD state control block and must be enabled.
            */
            if ( (hdspVhdStateControlIsVhdRegistered( eptState->lineVhdHdl ) == VRG_TRUE) &&
                 (hdspVhdStateControlIsVhdEnabled( eptState->lineVhdHdl ) == VRG_TRUE) )
            {
               VRG_UINT16 timerValue = 0;
               
               /* Update the VHD's timer increment */
               hdspVhdStateControlAdjustTimeIncrement( eptState->lineVhdHdl, increment, &timerValue );
               if ( timerValue == 0 )
               {
                  /* Line VHD's timer increment is 0. Disable the VHD */
                  status = hdspVhdStateControlDisableVhd( eptState->lineVhdHdl );
                  if ( status != EPSTATUS_SUCCESS )
                  {
                     ret = VRG_FALSE;
                  }
               }
               else
               {
                  ret = VRG_FALSE;
               }
            }
         }
      }
   }      
   
   return ret;
}


/*****************************************************************************
*  FUNCTION:   idleStateWdAllConnectionsIdle
*
*  PURPOSE:    Checks if all voice connections are idle. This function also monitors
*              the states of the VHDs associated with the connections and disables 
*              them if their timout increment expires.
*
*  PARAMETERS: NONE
*
*  RETURNS:    VRG_TRUE if all lines are IDLE, VRG_FALSE otherwise
*
*  NOTE:
*****************************************************************************/
static VRG_BOOL idleStateWdAllConnectionsIdle( void )
{
   int i;
   VRG_SINT16 status;
   VRG_BOOL ret = VRG_TRUE;
   VRG_UINT16 increment = IDLESTATUS_WD_SLEEP_TIME/1000;  /* Value in seconds */

   for ( i = 0; i < VRG_ENDPT_CFG_NUM_CNX; i++ )
   {
      /* If a connection is not IDLE, all connections are not IDLE */
      if ( (vrgCnxState[i].cnxId != CNX_UNUSED) && (vrgCnxState[i].cnxId != CNX_IDLE) )
      {
         ret = VRG_FALSE;
      }
      else
      {
         /*
         ** A valid gateway VHD handle must exist, the gateway VHD must be registered with the 
         ** state control block and the gatewate VHD must be enabled. 
         */
         if ( vrgCnxState[i].vhdhdl                                                     && 
              (hdspVhdStateControlIsVhdRegistered( vrgCnxState[i].vhdhdl ) == VRG_TRUE) && 
              (hdspVhdStateControlIsVhdEnabled( vrgCnxState[i].vhdhdl ) == VRG_TRUE) )
         {
            VRG_UINT16 timerValue = 0;

            /* Update the VHD's timer increment */
            hdspVhdStateControlAdjustTimeIncrement( vrgCnxState[i].vhdhdl, increment, &timerValue );
            if ( timerValue == 0 )
            {
               /* Connection VHD's timer increment is 0. Disable the VHD */
               status = hdspVhdStateControlDisableVhd( vrgCnxState[i].vhdhdl );
               if ( status != EPSTATUS_SUCCESS )
               {
                  ret = VRG_FALSE;
               }
            }
            else
            {
               ret = VRG_FALSE;
            }
         }
      }
   } 
   
   return ret;
}
#endif /* PWRMNGT_ENABLED || IDLEVOICE_ENABLED */

#endif /* BRCM_TRACK_IDLE_STATUS */
