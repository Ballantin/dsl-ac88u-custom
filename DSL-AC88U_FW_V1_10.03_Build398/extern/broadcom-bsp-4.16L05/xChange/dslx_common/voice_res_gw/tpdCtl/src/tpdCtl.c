/***************************************************************************
*    Copyright © 2000-2008 Broadcom Corporation
*
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*    Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: tpdctl.c
*
****************************************************************************
*    Description:
*
*      This module contains the functions of the tpdery monitoring service
*
****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <xdrvTpd.h>
#include <xdrvCas.h>
#include <xdrvSlic.h>
#include <tpdCtl.h>
#include <tpdCtlCfg.h>
#include <bosMsgQ.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <boardHalCas.h>
#include <heartbeat.h>                 /* Heartbeat monitoring module */
#include <vrgEndptCfg.h>
#include <boardHalTpd.h>
#include <boardHalSlic.h>
#include <boardHalProv.h>

/* ---------------------- Local Type Definitions -------------------------- */

#define TPD_MIB_GET VRG_FALSE
#define TPD_MIB_SET VRG_TRUE
#define TPD_MSG_QUEUE_SIZE 2

#define TPD_NUM_LINES   VRG_ENDPT_CFG_NUM_ENDPT

typedef struct
{
   /* Callback function to endpoint */
   TPD_CTL_CALLBACK callback;

   /* TPD Task ID */
   BOS_TASK_ID taskId;

   /* TPD Task message queue */
   BOS_MSGQ msgQId;

   /* TPD HeartBeat patient ID */
   int hbId[TPD_NUM_LINES];

   /* Dialtone Requested from Call Agent */
   VRG_BOOL dialTone[TPD_NUM_LINES];

   /* Current endpoint admin status (enabled/disabled) */
   VRG_BOOL isEndptEnabled[TPD_NUM_LINES];

   /* Test related variables */
   VRG_UINT32 testStart;
   VRG_UINT32 testStop;
   VRG_UINT32 testClear;
   VRG_UINT32 testValid;
   VRG_UINT32 testResult;

   /* ASCII string of results */
   VRG_UINT8 testValue[256];

} TPDSTATE;

/* ---------------------- Local Variables --------------------------------- */

static TPDSTATE tpdState;

/* ---------------------- Local Function Definitions ---------------------- */

static void tpdTaskMain( BOS_TASK_ARG taskArg );
static void tpdTaskInitCB( BOS_TASK_ARG taskArg );
static void tpdTaskDeinitCB( BOS_TASK_ARG taskArg );
static int processSignal( TPD_CTL_SIGNAL signal, int lineId, VRG_BOOL bActive, int value );

/****************************************************************************
*
*  tpdCtlInit - TPD controller init - call once during system initialization
*
*****************************************************************************
*
*  PURPOSE:    TPD controller module init - called once during system
*              initialization. Conceptually, the TPD controller manages the
*              TPD state machines for the board via the tpdery management API
*
*  PARAMETERS: callback - callback to access endpoint and callclient layer
*
*  RETURNS:    TPD_CTL_SUCCESS or TPD_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
TPD_CTL_STATUS tpdCtlInit( TPD_CTL_CALLBACK callback )
{
   BOS_TASK_CLASS tskPrio = TPD_CTL_CFG_TASK_PRIORITY;
   int i;

   /* register logging module */
   VRG_LOG_REGISTER( VRG_LOG_MOD_TPD );

   /* Clear the tpdery state structure */
   memset( &tpdState, 0, sizeof( TPDSTATE ) );

   /* Save callback function pointer for future use */
   tpdState.callback = callback;


   /* Register TPD patients with Heartbeat module for both lines */
   for (i=0; i<TPD_NUM_LINES; i++)
   {
      tpdState.isEndptEnabled[i] = VRG_TRUE;
      hbRegisterPatient( HBSTATE_INACTIVE, HBPATIENT_TYPE_TPD, i, &tpdState.hbId[i] );
   }

   /*
   ** Create a thread "TPD" from which to run the TPD service
   */
   if ( BOS_STATUS_OK != bosTaskCreateEx( "TPD",
                                          (8 * 1024),
                                          tskPrio,
                                          tpdTaskInitCB,
                                          tpdTaskMain,
                                          tpdTaskDeinitCB,
                                          0,
                                          &tpdState.taskId ))
   {
      return ( TPD_CTL_FAILURE );
   }

   return ( TPD_CTL_SUCCESS );
}


/****************************************************************************
*
*  tpdCtlDeinit - tpd controller module de-initialization
*
*****************************************************************************
*
*  PURPOSE:    tpd module deinit  - call once during system shutdown. This
*              will shutdown the tpd task and free all resources used by
*              the tpd controller. ( i.e. this function should free all
*              resources allocated in tpdCtlInit()).
*
*  PARAMETERS: none
*
*  RETURNS:    TPD_CTL_SUCCESS or TPD_CTL_FAILURE
*              This function should only return an error under catastrophic
*              circumstances. i.e. Something that cannot be fixed by re-invoking
*              the module initialization function.
*
*  NOTE:       It is assumed that this function is only called after the tpd
*              task has been notified of a pending application reset, and it
*              has acknowledged the notification. This implies that the tpd
*              task is currently blocked, waiting to be resumed so that they may
*              complete the shutdown procedure.
*
*              It is also assumed that no task is currently blocked on any OS
*              resource that was created in the module initialization function.
*
****************************************************************************/
TPD_CTL_STATUS tpdCtlDeinit( void )
{
   if ( tpdState.taskId )
   {
      /* Currently, the tpd task should be blocked in the reset-acknowledgement state.
       * Advance the tpd task to the reset-active state. This will cause the task to
       * resume operation. In this state, the tpd task should perform any operations
       * required for proper shutdown before exiting from it's main task function. */
      bosTaskProceedToShutdown( &tpdState.taskId );

      /* Wait for the tpd task to complete de-initialization */
      bosTaskWaitForDone( &tpdState.taskId );
   }

   VRG_LOG_INFO(( VRG_LOG_MOD_TPD, "TPD Control Task Shutdown Complete!" ));

   /* deregister logging module */
   VRG_LOG_DEREGISTER( VRG_LOG_MOD_TPD );

   return ( TPD_CTL_SUCCESS );
}

/****************************************************************************
*
*  tpdCtlProcessSignal
*
*****************************************************************************
*
*  PURPOSE:    Tell the TPD task to process the given signal on the given line.
*
*  PARAMETERS: signal - the signal to process
*              lineId - the line on which the signal applies
*              bActive - boolean indicating if this line was active or not
*              value - used for multiple purposes depending on signal
*
*  RETURNS:    TPD_CTL_SUCCESS or TPD_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
TPD_CTL_STATUS tpdCtlProcessSignal( TPD_CTL_SIGNAL signal, int lineId, VRG_BOOL bActive, int value )
{
   BOS_MSGQ_MSG msgBuf;

   /* Note: if you change this order, must propagate the change through the file !!! */
   msgBuf.buffer[0] = signal;
   msgBuf.buffer[1] = lineId;
   msgBuf.buffer[2] = bActive;
   msgBuf.buffer[3] = value;

   if( bosMsgQSend(&tpdState.msgQId, &msgBuf) == BOS_STATUS_OK )
   {
      return TPD_CTL_SUCCESS;
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "tpdCtlProcessSignal failure: Could not queue the TPD msg !!!!" ));
      return TPD_CTL_FAILURE;
   }
}

/****************************************************************************
*
*  tpdCtlSignalDialtone
*
*****************************************************************************
*
*  PURPOSE:    Tell the TPD task that dialtone was requested on the line
*
*  PARAMETERS: lineId - the line on which the signal applies
*
*  RETURNS:    TPD_CTL_SUCCESS or TPD_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
TPD_CTL_STATUS tpdCtlSignalDialtone( int lineId )
{
   if ( lineId < TPD_NUM_LINES )
   {
      tpdState.dialTone[lineId] = VRG_TRUE;
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "tpdSignalDialtone: suspicious line id (%d) !!!", lineId ));
   }

   return TPD_CTL_SUCCESS;
}

/****************************************************************************
*
*  tpdCtlSignalAdminStatus
*
*****************************************************************************
*
*  PURPOSE:    Tell the TPD task that admin status (enabled/disabled) 
*              on the line has changed
*
*  PARAMETERS: lineId   - the line on which the signal applies
*              bEnabled - new admin status (true=enabled, false=disabled)
*
*  RETURNS:    TPD_CTL_SUCCESS or TPD_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
TPD_CTL_STATUS tpdCtlSignalAdminStatus( int lineId, VRG_BOOL bEnabled )
{
   if ( lineId < TPD_NUM_LINES )
   {
      tpdState.isEndptEnabled[lineId] = bEnabled;
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "tpdCtlSignalAdminStatus: suspicious line id (%d) !!!", lineId ));
   }

   return TPD_CTL_SUCCESS;
}

/****************************************************************************
*
*  tpdTaskInitCB
*
*****************************************************************************
*
*  PURPOSE:    TPD task initialization function. This function will be
*              automatically called by the OS prior to invoking the
*              tpd task's main function. Task OS resource allocation and
*              initialization may be performed here.
*
*  PARAMETERS: none
*
*  RETURNS:    none
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void tpdTaskInitCB( BOS_TASK_ARG taskArg )
{
   char *qName = "TPDQ";

   /* Create message queue used by TPD task. */
   XCHG_ASSERT( BOS_STATUS_OK ==
         bosMsgQCreate(qName, TPD_MSG_QUEUE_SIZE, &tpdState.msgQId) );
}


/****************************************************************************
*
*  tpdTaskDeinitCB
*
*****************************************************************************
*
*  PURPOSE:    TPD task deinitialization function. This function will be
*              automatically called by the OS after the tpd task's main
*              function has exited. Any task OS resources allocated by the
*              task initialization function (tpdTaskInitCB) should be
*              de-allocated here.
*
*  PARAMETERS: none
*
*  RETURNS:    none
*
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
*
****************************************************************************/
static void tpdTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Cleanup */
   XCHG_ASSERT( bosMsgQDestroy( &tpdState.msgQId ) == BOS_STATUS_OK);
}


/****************************************************************************
*
*  tpdTaskMain - Main loop for the tpd service task
*
*****************************************************************************
*
*  PURPOSE:     Main loop for the tpd service
*
*  PARAMETERS:  context - BOS context pointer
*
*  RETURNS:     Nothing
*
*  NOTE:        This function should not be called directly. It should be
*               registered with the OS and invoked indirectly.
*
****************************************************************************/
static void tpdTaskMain( BOS_TASK_ARG context )
{
   BOS_MSGQ_MSG  msgBuf;
   BOS_STATUS    status;
   int lineId;

   while(1)
   {
      status = bosMsgQReceive( &tpdState.msgQId, &msgBuf );

      if ( status == BOS_STATUS_RESET )
      {
         /* The tpd task has been notified that a reset is pending. Acknowledge
         ** the notification and then block until the tpd task is resumed. */
         bosTaskResetAck();

         /* The tpd task should stop running by exiting from this main function */
         return;
      }
      else if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_TPD, "Exiting task TPD" ));
         return;
      }
      else if ( status == BOS_STATUS_OK )
      {

         lineId = msgBuf.buffer[1];

         if (lineId < TPD_NUM_LINES)
         {
            /* We better let the heartbeat module know we need to do some processing */
            hbUpdatePatientState( tpdState.hbId[lineId], HBSTATE_ACTIVE );

            if( processSignal( msgBuf.buffer[0], lineId, msgBuf.buffer[2], msgBuf.buffer[3] )
                  == TPD_CTL_FAILURE )
            {
               VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "tpdTaskMain: processSignal returned a failure." ));
            }

            /* Let the heartbeat module know we have nothing else to do. */
            hbUpdatePatientState( tpdState.hbId[lineId], HBSTATE_INACTIVE );
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "tpdTaskMain: suspissious line id (%d) !!!", lineId ));
         }
      }
   }
}

/****************************************************************************
*
*  processSignal
*
*****************************************************************************
*
*  PURPOSE:     Private function that actually processes the given signal for
*               the given lineId.
*
*  PARAMETERS:  signal  - the signal to process
*               lineId  - the line on which the signal applies
*               bActive - boolean indicating if this line was active or not
*               value - used for multiple purposes depending on signal
*
*  RETURNS:     TPD_CTL_FAILURE if the TPD task should end now.
*
*  NOTE:        This function is called from within the context of the TPD
*               task.
*
****************************************************************************/
static int processSignal( TPD_CTL_SIGNAL signal, int lineId, VRG_BOOL bActive, int value )
{
   XDRV_TPD *tpdDrv;
   XDRV_SLIC *slicDrv;
   int temp1, temp2, temp3;
   int tempPhaseReversal;
   
   VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "TPD signal request: signal (%d) line (%d) Value=%d", signal, lineId ,value));

   if( !boardHalTpdIsSupportedLine( lineId ) )
   {
     VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "TPD: TPD is not supported on line(%d)!", lineId));
     return TPD_CTL_FAILURE;
   }

#if defined ( BRCM_6838 )
   VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms();
   if(voiceParams->flags & BP_FLAG_DSP_APMHAL_ENABLE ) /*TPD init for APM SLICs*/
   {
      tpdDrv = boardHalTpdGetDriver();
   }
   else                                               /*TPD init for PCM SLACs*/
#endif
   {
      tpdDrv = boardHalPcmTpdGetDriver();
   }
   
   slicDrv = boardHalSlicGetDriver( lineId );
   if (( !tpdDrv ) || ( !slicDrv ))
   {
      return TPD_CTL_FAILURE;
   }

   switch( signal )
   {
      /* Line reset */
      case TPD_CTL_SIGNAL_LINE_RESET:
      {
         XDRV_CAS *casDriver;

         casDriver = boardHalCasGetDriver( lineId );

         if( !casDriver )
         {
            return TPD_CTL_FAILURE;
         }

         /* Reset the line. */
         if( xdrvTpdChanReset(tpdDrv,lineId) == XDRV_TPD_ERR_ABORT )
         {
            return TPD_CTL_FAILURE;
         }
      }
      break;

      /* Diag tests start */
      case TPD_CTL_SIGNAL_PROV_TESTS:
      case TPD_CTL_SIGNAL_NOPROV_TESTS:
      {
         HSZDROPRSLT   dropRslt;
         HSZSELFRSLT   selfRslt;
         int           testsFailed = 0;
         int           useProv = 0;
#if TPD_CTL_CFG_REPORT
         TPD_CTL_REPORT report;
#endif

         /* Check if a callback is installed */
         if( tpdState.callback == NULL )
         {
            /* The provisioning callback has not been set, we can not do anything right now */
            return ( TPD_CTL_FAILURE );
         }

         /* Check if provisioning was selected */
         if ( signal == TPD_CTL_SIGNAL_PROV_TESTS )
         {
            useProv = 1;
         }

         if ( useProv )
         {
            /* Retrieve the test start MIB item */
            tpdState.callback( TPD_CTL_EVENT_TEST_START, TPD_MIB_GET, &tpdState.testStart, lineId );
         }
         else
         {
            tpdState.testStart = value;
         }

         /* Check if testing is required */
         if( !tpdState.testStart )
         {
            /* Notify callclient/prov app that testing is done */
            tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );

            /* There is nothing we need to do right now */
            return ( TPD_CTL_FAILURE );
         }

         /* Testing required, proceed with setup ... */

         /* Reset test status results */
         tpdState.testStop = 0;
         tpdState.testValid = 0;
         tpdState.testResult = 0;
#if TPD_CTL_CFG_REPORT
         memset(tpdState.testValue, 0, sizeof(tpdState.testValue));
#else
         tpdState.testValue[0] = '\0';
#endif

         if ( useProv )
         {
            /* Update test status MIBs accordingly */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_SET, &tpdState.testStop, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_VALID, TPD_MIB_SET, &tpdState.testValid, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_RESULT, TPD_MIB_SET, &tpdState.testResult, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_VALUE, TPD_MIB_SET, &tpdState.testValue, lineId );
         }

         /* Check if the line is active */
         if (( bActive ) && !( tpdState.testStart & TPD_CTL_TEST_FORCE ))
         {
            /* Line is active, can not run the tests. Leave the 'valid' MIB cleared and return. */
            VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "Line is active or battery power in use. TPD testing aborted!" ));

            /* Reset test control bits */
            tpdState.testStart = 0;

            if ( useProv )
            {
               /* Clear start field to indicate completion */
               tpdState.callback( TPD_CTL_EVENT_TEST_START, TPD_MIB_SET, &tpdState.testStart, lineId );
            }

            /* Notify callclient/prov app that testing is done */
            tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );

            return ( TPD_CTL_FAILURE );
         }

         /* Notify callclient/prov app that testing is in progress */
         tpdState.callback( TPD_CTL_EVENT_TEST_IN_PROGRESS, TPD_MIB_SET, NULL, lineId );

         /* Save current status */
         xdrvSlicGetSlicParms( slicDrv, &tempPhaseReversal, &temp1, &temp2, &temp3 );

         /* Initialize testing apparatus */
         xdrvTpdTestingSetup( tpdDrv, lineId );

         /* Initialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, XDRV_FALSE );

         /* Reset results structure */
         memset( &dropRslt, 0, sizeof(HSZDROPRSLT));
         memset( &selfRslt, 0, sizeof(HSZSELFRSLT));

         /* Check if short testing is required */
         if((( tpdState.testStart & TPD_CTL_TEST_HAZ_POT ) && !( tpdState.testStop & TPD_CTL_TEST_HAZ_POT )) ||
            (( tpdState.testStart & TPD_CTL_TEST_FOR_EMF ) && !( tpdState.testStop & TPD_CTL_TEST_FOR_EMF )) ||
            (( tpdState.testStart & TPD_CTL_TEST_PTC_CHECK ) && !( tpdState.testStop & TPD_CTL_TEST_PTC_CHECK )))
         {
            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_HPFEF, &dropRslt );

            if( tpdState.testStart & TPD_CTL_TEST_HAZ_POT )
            {
               if (( dropRslt.dropHpFefRslt.tipAC <= TPD_MAX_HP_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.tipDC ) <= TPD_MAX_HP_DC_VOLTAGE_MV ) &&
                   ( dropRslt.dropHpFefRslt.ringAC <= TPD_MAX_HP_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.ringDC ) <= TPD_MAX_HP_DC_VOLTAGE_MV ))
               {
                  tpdState.testResult |= TPD_CTL_TEST_HAZ_POT;
               }

               tpdState.testValid |= TPD_CTL_TEST_HAZ_POT;
#if TPD_CTL_CFG_REPORT
               report.tipAC = dropRslt.dropHpFefRslt.tipAC;
               report.tipDC = dropRslt.dropHpFefRslt.tipDC;
               report.ringAC = dropRslt.dropHpFefRslt.ringAC;
               report.ringDC = dropRslt.dropHpFefRslt.ringDC;
#endif
            }

            if( tpdState.testStart & TPD_CTL_TEST_FOR_EMF )
            {
               if (( dropRslt.dropHpFefRslt.tipAC <= TPD_MAX_FEF_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.tipDC ) <= TPD_MAX_FEF_DC_VOLTAGE_MV ) &&
                   ( dropRslt.dropHpFefRslt.ringAC <= TPD_MAX_FEF_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.ringDC ) <= TPD_MAX_FEF_DC_VOLTAGE_MV ))
               {
                  tpdState.testResult |= TPD_CTL_TEST_FOR_EMF;
               }

               tpdState.testValid |= TPD_CTL_TEST_FOR_EMF;
#if TPD_CTL_CFG_REPORT
               report.tipAC = dropRslt.dropHpFefRslt.tipAC;
               report.tipDC = dropRslt.dropHpFefRslt.tipDC;
               report.ringAC = dropRslt.dropHpFefRslt.ringAC;
               report.ringDC = dropRslt.dropHpFefRslt.ringDC;
#endif
            }

            if( tpdState.testStart & TPD_CTL_TEST_PTC_CHECK )
            {
               if (( dropRslt.dropHpFefRslt.tipAC <= TPD_MAX_PTC_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.tipDC ) <= TPD_MAX_PTC_DC_VOLTAGE_MV ) &&
                   ( dropRslt.dropHpFefRslt.ringAC <= TPD_MAX_PTC_AC_VOLTAGE_MV ) &&
                   ( abs( dropRslt.dropHpFefRslt.ringDC ) <= TPD_MAX_PTC_DC_VOLTAGE_MV ))
               {
                  tpdState.testResult |= TPD_CTL_TEST_PTC_CHECK;
               }

               tpdState.testValid |= TPD_CTL_TEST_PTC_CHECK;
            }
         }

         if ( useProv )
         {
            /* Recheck the stop value in case tests were canceled */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_GET, &tpdState.testStop, lineId );
         }

         /* Check if short testing is required */
         if((( tpdState.testStart & TPD_CTL_TEST_TG_SHORT ) && !( tpdState.testStop & TPD_CTL_TEST_TG_SHORT )) ||
            (( tpdState.testStart & TPD_CTL_TEST_RG_SHORT ) && !( tpdState.testStop & TPD_CTL_TEST_RG_SHORT )) ||
            (( tpdState.testStart & TPD_CTL_TEST_TR_SHORT ) && !( tpdState.testStop & TPD_CTL_TEST_TR_SHORT )))
         {
            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_RESFLT, &dropRslt );

            dropRslt.dropRfltRslt.tipGroundImpedance = abs( dropRslt.dropRfltRslt.tipGroundImpedance ); 
            dropRslt.dropRfltRslt.ringGroundImpedance = abs( dropRslt.dropRfltRslt.ringGroundImpedance );
            dropRslt.dropRfltRslt.tipRingImpedance = abs( dropRslt.dropRfltRslt.tipRingImpedance );

            if( tpdState.testStart & TPD_CTL_TEST_TG_SHORT )
            {
               if( abs( dropRslt.dropRfltRslt.tipGroundImpedance ) > TPD_SHORT_RESISTANCE )
               {
                  tpdState.testResult |= TPD_CTL_TEST_TG_SHORT;
               }

               tpdState.testValid |= TPD_CTL_TEST_TG_SHORT;
#if TPD_CTL_CFG_REPORT
               report.tipGroundImpedance = dropRslt.dropRfltRslt.tipGroundImpedance;
#endif
            }

            if( tpdState.testStart & TPD_CTL_TEST_RG_SHORT )
            {
               if( abs( dropRslt.dropRfltRslt.ringGroundImpedance ) > TPD_SHORT_RESISTANCE )
               {
                  tpdState.testResult |= TPD_CTL_TEST_RG_SHORT;
               }

               tpdState.testValid |= TPD_CTL_TEST_RG_SHORT;
#if TPD_CTL_CFG_REPORT
               report.ringGroundImpedance = dropRslt.dropRfltRslt.ringGroundImpedance;
#endif
            }

            if( tpdState.testStart & TPD_CTL_TEST_TR_SHORT )
            {
               if( abs( dropRslt.dropRfltRslt.tipRingImpedance ) > TPD_SHORT_RESISTANCE )
               {
                  tpdState.testResult |= TPD_CTL_TEST_TR_SHORT;
               }

               tpdState.testValid |= TPD_CTL_TEST_TR_SHORT;
#if TPD_CTL_CFG_REPORT
               report.tipRingImpedance = dropRslt.dropRfltRslt.tipRingImpedance;
#endif
            }
         }

         if ( useProv )
         {
            /* Recheck the stop value in case tests were canceled */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_GET, &tpdState.testStop, lineId );
         }

         /* Check if load testing is required */
         if(( tpdState.testStart & TPD_CTL_TEST_REC_OFFHOOK ) && !( tpdState.testStop & TPD_CTL_TEST_REC_OFFHOOK ))
         {
            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_ROH, &dropRslt );

            if ( dropRslt.dropRohRslt.phoneOnhook )
            {
               tpdState.testResult |= TPD_CTL_TEST_REC_OFFHOOK;
            }

            tpdState.testValid |= TPD_CTL_TEST_REC_OFFHOOK;
#if TPD_CTL_CFG_REPORT
               report.phoneOnhook = dropRslt.dropRohRslt.phoneOnhook;
#endif
         }

         if ( useProv )
         {
            /* Recheck the stop value in case tests were canceled */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_GET, &tpdState.testStop, lineId );
         }

         /* Check if load testing is required */
         if(( tpdState.testStart & TPD_CTL_TEST_REN ) && !( tpdState.testStop & TPD_CTL_TEST_REN ))
         {
            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_RING, &dropRslt );

            if (((( dropRslt.dropRngRslt.RENValue1 * 1000 ) + dropRslt.dropRngRslt.RENValue2 ) > ( TPD_MIN_MILLIRENS )) &&
                ((( dropRslt.dropRngRslt.RENValue1 * 1000 ) + dropRslt.dropRngRslt.RENValue2 ) < ( TPD_MAX_MILLIRENS )))
            {
               tpdState.testResult |= TPD_CTL_TEST_REN;
            }

            tpdState.testValid |= TPD_CTL_TEST_REN;
#if TPD_CTL_CFG_REPORT
            report.ren = ( dropRslt.dropRngRslt.RENValue1 * 1000 ) + dropRslt.dropRngRslt.RENValue2;
#else
            sprintf( tpdState.testValue, "[P=GR-909 Ringers Test,V=%ld.%03ld,U=RENs]", dropRslt.dropRngRslt.RENValue1, dropRslt.dropRngRslt.RENValue2 );
#endif
         }

         if ( useProv )
         {
            /* Recheck the stop value in case tests were canceled */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_GET, &tpdState.testStop, lineId );
         }

         /* Check if self testing is required */
         if(( tpdState.testStart & TPD_CTL_TEST_SELF_CHECK ) && !( tpdState.testStop & TPD_CTL_TEST_SELF_CHECK ))
         {
            int suite = (XDRV_TPD_RUN_SELF_APM  | XDRV_TPD_RUN_SELF_HVG |
                         XDRV_TPD_RUN_SELF_LOOP | XDRV_TPD_RUN_SELF_VOLTAGE |
                         XDRV_TPD_RUN_SELF_RING | XDRV_TPD_RUN_SELF_CAP);
            xdrvTpdRunSelfTests( tpdDrv, lineId, suite, &selfRslt );

            if(( selfRslt.selfHvgPassed && selfRslt.selfApmPassed) &&
               ( selfRslt.selfLoop && selfRslt.selfBattery ) &&
               ( selfRslt.selfRing ) &&
               ( selfRslt.selfCap ))
            {
               tpdState.testResult |= TPD_CTL_TEST_SELF_CHECK;
            }

            tpdState.testValid |= TPD_CTL_TEST_SELF_CHECK;
#if TPD_CTL_CFG_REPORT
            report.loop = selfRslt.selfLoop;
            report.battery = selfRslt.selfBattery;
            report.ring = selfRslt.selfRing;
            report.cap  = selfRslt.selfCap;
#endif
         }

         /* Deinitialize testing apparatus - only offhook simulation testing should occur after this point */
         xdrvTpdTestingShutdown( tpdDrv, lineId, tpdState.isEndptEnabled[lineId] );

         /* Deinitialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, tempPhaseReversal );

         if ( useProv )
         {
            /* Recheck the stop value in case tests were canceled */
            tpdState.callback( TPD_CTL_EVENT_TEST_STOP, TPD_MIB_GET, &tpdState.testStop, lineId );
         }

         /* Check if offhook simulation testing is required */
         if(( tpdState.testStart & TPD_CTL_TEST_OFFHOOK ) && !( tpdState.testStop & TPD_CTL_TEST_OFFHOOK ))
         {
            tpdState.dialTone[lineId] = VRG_FALSE;

            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_OFFHOOK, &dropRslt );

            if( tpdState.dialTone[lineId] )
            {
               tpdState.testResult |= TPD_CTL_TEST_OFFHOOK;
            }

            tpdState.testValid |= TPD_CTL_TEST_OFFHOOK;
         }

         if( tpdState.testResult != tpdState.testValid )
         {
            testsFailed = 1;
         }

         if ( useProv )
         {
            /* Notify call client of success or failure of the line tests */
            tpdState.callback( TPD_CTL_EVENT_TEST_FAILED, TPD_MIB_SET, &testsFailed, lineId );
         }

#ifndef TPD_CTL_CFG_REPORT
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "TPD Standard Drop Test Results for line (%d)", lineId ));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "-------------------------------------------------------------"));

         if(( tpdState.testValid & TPD_CTL_TEST_HAZ_POT ) ||
            ( tpdState.testValid & TPD_CTL_TEST_FOR_EMF ) ||
            ( tpdState.testValid & TPD_CTL_TEST_PTC_CHECK ))
         {
            if((( tpdState.testStart & TPD_CTL_TEST_HAZ_POT ) && (!( tpdState.testResult & TPD_CTL_TEST_HAZ_POT ))) ||
               (( tpdState.testStart & TPD_CTL_TEST_FOR_EMF ) && (!( tpdState.testResult & TPD_CTL_TEST_FOR_EMF ))) ||
               (( tpdState.testStart & TPD_CTL_TEST_PTC_CHECK ) && (!( tpdState.testResult & TPD_CTL_TEST_PTC_CHECK ))))
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "HPFEF tests selected FAILED:"));
            }
            else
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "HPFEF tests selected PASSED:"));
            }
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip AC Voltage is %ld mVrms", dropRslt.dropHpFefRslt.tipAC));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip DC Voltage is %ld mVdc", dropRslt.dropHpFefRslt.tipDC));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Ring AC Voltage is %ld mVrms ", dropRslt.dropHpFefRslt.ringAC));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Ring DC Voltage is %ld mVdc", dropRslt.dropHpFefRslt.ringDC));
         }

         if(( tpdState.testValid & TPD_CTL_TEST_TG_SHORT ) ||
            ( tpdState.testValid & TPD_CTL_TEST_RG_SHORT ) ||
            ( tpdState.testValid & TPD_CTL_TEST_TR_SHORT ))
         {
            if((( tpdState.testStart & TPD_CTL_TEST_TG_SHORT ) && (!( tpdState.testResult & TPD_CTL_TEST_TG_SHORT ))) ||
               (( tpdState.testStart & TPD_CTL_TEST_RG_SHORT ) && (!( tpdState.testResult & TPD_CTL_TEST_RG_SHORT ))) ||
               (( tpdState.testStart & TPD_CTL_TEST_TR_SHORT ) && (!( tpdState.testResult & TPD_CTL_TEST_TR_SHORT ))))
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "RESFLT tests selected FAILED:"));
            }
            else
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "RESFLT tests selected PASSED:"));
            }
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip-Ground Resistance is %ld ohms", dropRslt.dropRfltRslt.tipGroundImpedance));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Ring-Ground Resistance is %ld ohms", dropRslt.dropRfltRslt.ringGroundImpedance));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip-Ring Resistance is %ld ohms", dropRslt.dropRfltRslt.tipRingImpedance));
         }

         if( tpdState.testValid & TPD_CTL_TEST_REC_OFFHOOK )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "ROH Phone Onhook %s\n", dropRslt.dropRohRslt.phoneOnhook ? "PASSED" : "FAILED"));
         }

         if( tpdState.testValid & TPD_CTL_TEST_REN )
         {
            if( tpdState.testResult & TPD_CTL_TEST_REN )
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "REN tests PASSED:"));
            }
            else
            {
               VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "REN tests FAILED:"));
            }
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "REN value is %ld.%03ld RENs", dropRslt.dropRngRslt.RENValue1, dropRslt.dropRngRslt.RENValue2 ));
         }

         if( tpdState.testValid & TPD_CTL_TEST_REC_OFFHOOK )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "ROH Phone Onhook %s", dropRslt.dropRohRslt.phoneOnhook ? "PASSED" : "FAILED"));
         }

#if 0 /* DSL does not support XDRV_DEBUG API  */
         if( tpdState.testValid & TPD_CTL_TEST_SELF_CHECK )
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Self Test Assert Check %s", selfRslt.selfAssertPassed ? "PASSED" : "FAILED"));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Self Test Dsp Check %s", selfRslt.selfDspCountPassed ? "PASSED" : "FAILED"));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Self Test Hvg Check %s", selfRslt.selfHvgPassed ? "PASSED" : "FAILED"));
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Self Test Apm Check %s", selfRslt.selfApmPassed ? "PASSED" : "FAILED"));
         }
#endif

         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Start MIB - 0x%x", tpdState.testStart));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Stop MIB - 0x%x", tpdState.testStop));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Valid MIB - 0x%x", tpdState.testValid));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Result MIB - 0x%x", tpdState.testResult));

         if ( testsFailed )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_TPD, "TPD Standard Drop Tests FAILED !!!"));
         }
         else
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "TPD Standard Drop Tests Passed."));
         }
#endif
         if ( useProv )
         {
            /* Reset test control bits */
            tpdState.testStart = 0;

            /* Return results */
            tpdState.callback( TPD_CTL_EVENT_TEST_START, TPD_MIB_SET, &tpdState.testStart, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_VALID, TPD_MIB_SET, &tpdState.testValid, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_RESULT, TPD_MIB_SET, &tpdState.testResult, lineId );
            tpdState.callback( TPD_CTL_EVENT_TEST_VALUE, TPD_MIB_SET, tpdState.testValue, lineId );
         }

#if TPD_CTL_CFG_REPORT
         tpdState.callback( TPD_CTL_EVENT_TEST_VALID, TPD_MIB_SET, &tpdState.testValid, lineId );
         memcpy(tpdState.testValue, &report, sizeof(report));
         tpdState.callback( TPD_CTL_EVENT_TEST_VALUE, TPD_MIB_SET, tpdState.testValue, lineId );
#else
         /* Notify callclient/prov app that testing is done */
         tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );
#endif

      }
      break;

      /* PC Diag tests start */
      case TPD_CTL_SIGNAL_PC15_TESTS:
      {
         HSZDROPRSLT dropRslt;

         /* Check if a callback is installed */
         if( tpdState.callback == NULL )
         {
            /* The provisioning callback has not been set, we can not do anything right now */
            return ( TPD_CTL_FAILURE );
         }

         /* Retreive the test start MIB item  */
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_START, TPD_MIB_GET, &tpdState.testStart, lineId );

         /* Check if testing is required */
         if( !tpdState.testStart )
         {
            /* Notify callclient/prov app that testing is done */
            tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );

            /* There is nothing we need to do right now */
            return ( TPD_CTL_FAILURE );
         }

         /* Testing required, proceed with setup ... */

         /* Reset test status results */
         tpdState.testClear = 0;
         tpdState.testValid = 0;
         tpdState.testResult = 0;

         /* Update test status MIBs accordingly */
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_CLEAR, TPD_MIB_SET, &tpdState.testClear, lineId );
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_VALID, TPD_MIB_SET, &tpdState.testValid, lineId );
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_RESULT, TPD_MIB_SET, &tpdState.testResult, lineId );

         /* Check if the line is active */
         if ( bActive )
         {
            /* Line is active, can not run the tests. Leave the 'valid' MIB cleared and return. */
            VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "Line is active or battery power in use. TPD testing aborted!"));

            /* Reset test control bits */
            tpdState.testStart = 0;

            /* Return results */
            tpdState.callback( TPD_CTL_EVENT_PC_TEST_START, TPD_MIB_SET, &tpdState.testStart, lineId );

            /* Notify callclient/prov app that testing is done */
            tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );

            return ( TPD_CTL_FAILURE );
         }

         /* Notify callclient/prov app that testing is in progress */
         tpdState.callback( TPD_CTL_EVENT_TEST_IN_PROGRESS, TPD_MIB_SET, NULL, lineId );

         /* Save current status */
         xdrvSlicGetSlicParms( slicDrv, &tempPhaseReversal, &temp1, &temp2, &temp3 );

         /* Initialize testing apparatus */
         xdrvTpdTestingSetup( tpdDrv, lineId );

         /* Initialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, XDRV_FALSE );

         /* Reset results structure */
         memset( &dropRslt, 0, sizeof(HSZDROPRSLT));

         /* Check if voltage testing is required */
         if(( tpdState.testStart & TPD_CTL_TEST_PC_FOREIGN_VOLTAGE ) ||
            ( tpdState.testStart & TPD_CTL_TEST_PC_HAZARDOUS_POTENTIAL ))
         {
            /* Run tests */
            xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_HPFEF, &dropRslt );

            if ( tpdState.testStart & TPD_CTL_TEST_PC_FOREIGN_VOLTAGE )
            {
               /* ORing of these conditions indicates the presence of foreign voltage */
               if (( dropRslt.dropHpFefRslt.tipAC > TPD_MAX_FEF_AC_VOLTAGE_MV ) ||
                   ( abs( dropRslt.dropHpFefRslt.tipDC ) > TPD_MAX_FEF_DC_VOLTAGE_MV ) ||
                   ( dropRslt.dropHpFefRslt.ringAC > TPD_MAX_FEF_AC_VOLTAGE_MV ) ||
                   ( abs( dropRslt.dropHpFefRslt.ringDC ) > TPD_MAX_FEF_DC_VOLTAGE_MV ))
               {
                  /* From pktcEnSigMib: Set the corresponding bit to a value of '1'
                  ** if the tests indicated the presence of a foreign potential as
                  ** per the associated test case */
                  tpdState.testResult |= TPD_CTL_TEST_PC_FOREIGN_VOLTAGE;
               }

               /* From pktcEnSigMib: return a value of '1' if the tests were
               ** run successfully and the results are valid */
               tpdState.testValid |= TPD_CTL_TEST_PC_FOREIGN_VOLTAGE;
            }

            if ( tpdState.testStart & TPD_CTL_TEST_PC_HAZARDOUS_POTENTIAL )
            {
               /* Either of these conditions indicates the presence of hazardous potential */
               if (( dropRslt.dropHpFefRslt.tipAC > TPD_MAX_HP_AC_VOLTAGE_MV ) ||
                   ( abs( dropRslt.dropHpFefRslt.tipDC ) > TPD_MAX_HP_DC_VOLTAGE_MV ) ||
                   ( dropRslt.dropHpFefRslt.ringAC > TPD_MAX_HP_AC_VOLTAGE_MV ) ||
                   ( abs( dropRslt.dropHpFefRslt.ringDC ) > TPD_MAX_HP_DC_VOLTAGE_MV ))
               {
                  /* From pktcEnSigMib: Set the corresponding bit to a value of '1'
                  ** if the tests indicated the presence of a hazardous potential
                  ** as per the associated test case */
                  tpdState.testResult |= TPD_CTL_TEST_PC_HAZARDOUS_POTENTIAL;
               }

               /* From pktcEnSigMib: return a value of '1' if the tests were
               ** run successfully and the results are valid */
               tpdState.testValid |= TPD_CTL_TEST_PC_HAZARDOUS_POTENTIAL;
            }
         }

         /* Deitialize testing apparatus */
         xdrvTpdTestingShutdown( tpdDrv, lineId, tpdState.isEndptEnabled[lineId] );

         /* Deinitialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, tempPhaseReversal );

         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "TPD PacketCable 1.5 Drop Test Results for line (%d)", lineId));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "-------------------------------------------------------------"));

         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip AC Voltage is %ld mVrms", dropRslt.dropHpFefRslt.tipAC));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Tip DC Voltage is %ld mVdc", dropRslt.dropHpFefRslt.tipDC));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Ring AC Voltage is %ld mVrms", dropRslt.dropHpFefRslt.ringAC));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Ring DC Voltage is %ld mVdc", dropRslt.dropHpFefRslt.ringDC));

         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Start MIB - 0x%x", (unsigned int)tpdState.testStart));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Valid MIB - 0x%x", (unsigned int)tpdState.testValid));
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Test Result MIB - 0x%x", (unsigned int)tpdState.testResult));

         /* Reset test control bits */
         tpdState.testStart = 0;

         /* Return results */
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_START, TPD_MIB_SET, &tpdState.testStart, lineId );
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_VALID, TPD_MIB_SET, &tpdState.testValid, lineId );
         tpdState.callback( TPD_CTL_EVENT_PC_TEST_RESULT, TPD_MIB_SET, &tpdState.testResult, lineId );

         /* Notify callclient/prov app that testing is done */
         tpdState.callback( TPD_CTL_EVENT_TEST_DONE, TPD_MIB_SET, NULL, lineId );
      }
      break;

      case TPD_CTL_SIGNAL_CALIBRATION_TESTS:
      {
         HSZDROPRSLT dropRslt;

         /* Check if a callback is installed */
         if( tpdState.callback == NULL )
         {
            /* The provisioning callback has not been set, we can not do anything right now */
            return ( TPD_CTL_FAILURE );
         }

         /* Check if the line is active */
         if ( bActive )
         {
            /* Line is active, can not run the tests. */
            VRG_LOG_WARN(( VRG_LOG_MOD_TPD, "Line is active or battery power in use. TPD testing aborted!"));

            return ( TPD_CTL_FAILURE );
         }

         /* Reset results structure */
         memset( &dropRslt, 0, sizeof(HSZDROPRSLT) );

         /* Save current status */
         xdrvSlicGetSlicParms( slicDrv, &tempPhaseReversal, &temp1, &temp2, &temp3 );

         /* Initialize testing apparatus */
         xdrvTpdTestingSetup( tpdDrv, lineId );

         /* Initialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, XDRV_FALSE );

         /* Run tests */
         xdrvTpdRunDropTests( tpdDrv, lineId, XDRV_TPD_RUN_DROP_CALIBRATION, &dropRslt );

         /* Deitialize testing apparatus */
         xdrvTpdTestingShutdown( tpdDrv, lineId, tpdState.isEndptEnabled[lineId] );

         /* Deinitialize other items */
         xdrvSlicPhaseReversalControl( slicDrv, tempPhaseReversal );

         /* Return ring calibration results */
         tpdState.callback( TPD_CTL_EVENT_CALIBRATION_DONE, TPD_MIB_SET,
                            &dropRslt.dropCalRslt.RngVolOffset, lineId );
      }
      break;

      default:
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_TPD, "Unknown signal (%d) received", signal));
      }
   }

   return ( TPD_CTL_SUCCESS );
}
