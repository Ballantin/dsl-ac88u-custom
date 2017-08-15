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
*    Filename: vrgEndptDispatcher.c
*
****************************************************************************
*    Description:
*
*      This file contains the implementation endpoint command/event 
*      dispatcher task.
*
****************************************************************************/


/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

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
#include <bosMisc.h>
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

/* These are needed for the randomization functions  */
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/uaccess.h>

#include "vrgEndptPrivate.h"

#include <linux/types.h>
#include <linux/delay.h>

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/
#define DISP_SLEEP_DELAY    100



/*
*****************************************************************************
** TYPE DEFINITIONS
*****************************************************************************
*/


/*
 ***************************************************************************
 * EXPORTED VARIABLES
 ***************************************************************************
*/
BOS_SEM   vrgIdleWdSyncSem;  /* Needed to serialize status update between idle watchdog and dispather task */
BOS_SEM   vrgIdleWdSleepSem; /* Needed to put idle watchdog task into sleep and wakeup  */


/*
*****************************************************************************
** LOCAL FUNCTION PROTOTYPES
*****************************************************************************
*/

static VRG_BOOL      VrgDispatcherTaskCheckPendingReset(void);
static void          VrgDispatcherTaskMain(BOS_TASK_ARG taskArg);
static MEMCIRCBUFDRV VrgQueueCreate(VRG_UINT32 count, VRG_UINT32 size);

/*
*****************************************************************************
** EXTERNS
*****************************************************************************
*/
extern volatile int isVoiceIdle;
extern void halDmaInterruptEnable ( void );
extern void halDmaInterruptDisable ( void );
#ifdef VRG_ENDPT_2G3G_SUPPORT
extern BOS_TASK_ID vrgCSMediaProcessTaskId;
extern void VrgCSMediaTxTaskMain( BOS_TASK_ARG taskArg );
#endif

/****************************************************************************
*
*  VrgDispatcherProcessInit
*
*****************************************************************************
*
*  PURPOSE:
*     Launches a VRGDISP task to process the VRG Command and Events
*
*  PARAMETERS:
*
*  RETURNS:
*     HAPISUCCESS or HAPIFAILURE
*
*  NOTE:
*
****************************************************************************/
VRG_SINT16 VrgDispatcherProcessInit( void )
{
   BOS_TASK_CLASS tskPrio = VRG_ENDPT_CFG_DISP_TASK_PRIORITY;

   /* Create message queue to hold VRG endpoint commands */
   vrgEptCmdQ = VrgQueueCreate( 20, sizeof(VRG_EPT_CMD*) );
   if ( vrgEptCmdQ == -1 )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgEptCmdQ"));
      return ( HAPIFAILURE );
   }

   /* Create message queue to hold VRG events */
   vrgEventQ = VrgQueueCreate( 150, sizeof(VRG_EVT) );
   if ( vrgEventQ == -1 )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgEventQ"));
      return ( HAPIFAILURE );
   }

   /* Create semaphore for command queue */
   if ( bosSemCreate("vrgDispSem", 0, 1, &vrgDispSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgDispSem"));
      return ( HAPIFAILURE );
   }

   /* Create semaphore to synchronize with idle watchdog task */
   if ( bosSemCreate("vrgIdleWdSyncSem", 1, 1, &vrgIdleWdSyncSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgIdleWdSyncSem"));
      return ( HAPIFAILURE );
   }

   /* Create semaphore to allow idle watchdog task to sleep */
   if ( bosSemCreate("vrgIdleWdSleepSem", 0, 1, &vrgIdleWdSleepSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgIdleWdSleepSem"));
      return ( HAPIFAILURE );
   }   

   /*
   ** Create a thread "VRGDISP" from which to process the VRG Command and Events
   */
   bosTaskCreateEx( "VRGDISP",
                    (8 * 1024),
                    tskPrio,
                    NULL,
                    VrgDispatcherTaskMain,
                    NULL,
                    0,
                    &vrgDispProcessTaskId );

   if( !vrgDispProcessTaskId )
   {
      VRG_LOG_CRIT(( VRG_LOG_MOD_EPT, "Error creating VRGDISP task" ));
      return ( HAPIFAILURE );
   }

#ifdef VRG_ENDPT_2G3G_SUPPORT
   bosTaskCreateEx( "CSINGRESS",
                    (8 * 1024),
                    BOS_TASK_CLASS_HIGH,
                    NULL,
                    VrgCSMediaTxTaskMain,
                    NULL,
                    0,
                    &vrgCSMediaProcessTaskId );


   if( !vrgCSMediaProcessTaskId )
   {
      VRG_LOG_CRIT(( VRG_LOG_MOD_EPT, "Error creating CSINGRESS task" ));
      return ( HAPIFAILURE );
   }
#endif

   return ( HAPISUCCESS );
}

/****************************************************************************
*
*  VrgDispatcherTaskMain - VRG Dispatcher Task
*
*  PURPOSE:
*     This is a task which processes all VRG endpoint API commands and
*     events posted by callback routines.
*
*  PARAMETERS:
*
*  RETURNS:
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void VrgDispatcherTaskMain( BOS_TASK_ARG taskArg )
{
   VRG_EVT evt;
   VRG_EPT_CMD *cmd;
   int status;
   BOS_STATUS semStatus;

   /*
   ** Process all events from the VRG event queue
   */
   while( 1 )
   {
      /* wait untill there is something to process or shutdown to happen */
      semStatus = bosSemTake( &vrgDispSem );
      if ( VrgDispatcherTaskCheckPendingReset() == VRG_TRUE )
      {
         /* A reset has been initiated. The VRGEVPR task should stop running by exiting
          * from this main task function. */
         return;
      }
      else if ( semStatus == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Exiting task VRGDISP" ));
         return;
      }

      /* Get an event from the VRG event queue - non-blocking call */
      status = VrgQueueGet(vrgEventQ, &evt, sizeof(evt));
      if ( status == EPSTATUS_SUCCESS )
      {
         /* Need to serialize with idle watchdog task, so that idle watchdog task does not report wrong state */
         bosSemTake (&vrgIdleWdSyncSem);
      
         /* First enable voice (DMA) interrupt if it was disabled */
         if ( isVoiceIdle )
         {
#if IDLEVOICE_ENABLED  
            /* Now it is the time to turn on the voice (DMA) interrupts */
            halDmaInterruptEnable();
#endif
            /* voice is no longer idle */
            isVoiceIdle = BOS_FALSE;
            /* Wakeup idle watchdog task since it was sleeping */
            bosSemGive (&vrgIdleWdSleepSem);
         }         
         VrgEventProcess( &evt );
    
         bosSemGive (&vrgIdleWdSyncSem);         
         continue;
      }

      /* Get an ept command from the VRG ept command queue - non-blocking call */
      status = VrgQueueGet(vrgEptCmdQ, (void*)&cmd, sizeof(VRG_EPT_CMD*));
      if ( status == EPSTATUS_SUCCESS )
      {
         /* Need to serialize with idle watchdog task, so that idle watchdog task does not report wrong state */
         bosSemTake (&vrgIdleWdSyncSem);   
         
         /* First enable voice interrupt if it was disabled */         
         if ( isVoiceIdle )
         {
#if IDLEVOICE_ENABLED     
            /* Now it is the time to turn on the voice (DMA) interrupts */
            halDmaInterruptEnable();     
#endif
            /* voice is no longer idle */
            isVoiceIdle = BOS_FALSE;            
            /* Wakeup idle watchdog task since it was sleeping */
            bosSemGive (&vrgIdleWdSleepSem);            
         }          
         VrgEptCmdProcess( cmd );
         
         bosSemGive (&vrgIdleWdSyncSem);         
      }
   }
}


/****************************************************************************
*
*  VrgDispatcherTaskCheckPendingReset
*
*****************************************************************************
*
*  PURPOSE:    To determine if the VRGEVPR task was notified of a pending
*              application reset.
*
*  PARAMETERS: none
*
*  RETURNS:    TRUE if VRGEVPR was notified of an application reset, else FALSE
*
*  NOTE:       If the VRGEVPR task was notified of a pending application
*              reset, it will acknowledge the notification. The acknowledge
*              will block the VRGEVPR task until it is resumed. Once the VRGEVPR
*              task is resumed, it should exit the main task function to
*              complete the task reset procedure.
*
****************************************************************************/
static VRG_BOOL VrgDispatcherTaskCheckPendingReset( void )
{
   if ( bosTaskIsResetPending() )
   {
      /* The VRGEVPR task has been notified that a reset is pending. Acknowledge
         * the notification and then block until the VRGEVPR task is resumed. */
      bosTaskResetAck();

      return ( VRG_TRUE );
   }

   return ( VRG_FALSE );
}

/****************************************************************************
*
*  VrgDispatcherProcessDeinit - VRGDISP module de-initialization
*
*****************************************************************************
*
*  PURPOSE:
*     VRG Dispatcher module deinit  - call once during system shutdown. This will
*                          shutdown the VRGDISP task and free all resources
*                          used by the VRGDISP module.
*
*  PARAMETERS: none
*
*  RETURNS:    HAPISUCCESS or HAPIFAILURE
*              This function should only return an error under catastrophic
*              circumstances. i.e. Something that cannot be fixed by re-invoking
*              the module initialization function.
*
*  NOTE:       It is assumed that this function is only called after the VRGEVPR
*              task has been notified of a pending application reset, and it
*              has acknowledged the notification. This implies that the VRGEVPR
*              task is currently blocked, waiting to be resumed so that they may
*              complete the shutdown procedure.
*
*              It is also assumed that no task is currently blocked on any OS
*              resource that was created in the module initialization function.
*
****************************************************************************/
VRG_SINT16 VrgDispatcherProcessDeinit( void )
{
   if ( vrgDispProcessTaskId )
   {
      /* Currently, the VRGDISP task should be blocked in the reset-acknowledgement state.
       * Advance the VRGDISP task to the reset-active state. This will cause the task to
       * resume operation. In this state, the VRGDISP task should perform any operations
       * required for proper shutdown before exiting from it's main task function. */
      bosTaskProceedToShutdown( &vrgDispProcessTaskId );
      /* Wake dispatcher in case it is sleeping */
      bosSemGive( &vrgDispSem );
      /* Wait for the VRGDISP task to complete de-initialization */
      bosTaskWaitForDone( &vrgDispProcessTaskId );
   }

   /* Release the message queue */
   if ( memCircBufFree(&vrgEptCmdQ) != BUF_STATUS_OK || memCircBufFree(&vrgEventQ) != BUF_STATUS_OK )
   {
      return ( HAPIFAILURE );
   }
   
   bosSemDestroy( &vrgDispSem );
   bosSemDestroy( &vrgIdleWdSyncSem);
   bosSemDestroy( &vrgIdleWdSleepSem);

   VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "VRGDISP shutdown complete!" ));
   return ( HAPISUCCESS );
}

/*
*****************************************************************************
** FUNCTION:    VrgQueuePost
**
** PURPOSE:     Post an item to a VRG queue
**
** PARAMETERS:  queue - Queue ID
**              item  - Pointer to the memory in which item is stored
**              size  - Size of item to be retrieved
**
** RETURNS:     status - 0 == EPSTATUS_SUCCESS
**
*****************************************************************************
*/
BUF_STATUS VrgQueuePost(MEMCIRCBUFDRV queue, void* item, unsigned int size )
{
   BUF_STATUS status = BUF_STATUS_OK;

   if ( (status = memCircBufWrite(queue, item, size)) == BUF_STATUS_OK )
   {
      bosSemGive( &vrgDispSem );
   }
   else
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "%s: Could not write item to queue %d", __FUNCTION__, queue ));
   }

   return status;
}

/*
*****************************************************************************
** FUNCTION:    VrgQueueGet
**
** PURPOSE:     Get an item from the VRG queue
**
** PARAMETERS:  queue - Queue ID
**              item  - Pointer to the memory in which item is stored
**              size  - Size of item to be retrieved
**              
**
** RETURNS:     pointer to the item retrieved, NULL if queue empty
**
*****************************************************************************
*/
BUF_STATUS VrgQueueGet(MEMCIRCBUFDRV queue, void *item, unsigned int size )
{
   return memCircBufRead(queue, item, size);
}

/*
*****************************************************************************
** FUNCTION:    VrgQueueCreate
**
** PURPOSE:     Create a VRG queue
**
** PARAMETERS:  Number of items in queue
**              Size of each item
**
** RETURNS:     queue Id
**
** NOTE:
*****************************************************************************
*/
static MEMCIRCBUFDRV VrgQueueCreate( VRG_UINT32 count, VRG_UINT32 size )
{
   MEMCIRCBUFDRV bufDrv = -1;

   if ( memCircBufAlloc(&bufDrv, (count*size)) != BUF_STATUS_OK )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Exit VrgQueueCreate bad" ));
      return (-1);
   }

   return bufDrv;
}
