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
*    Filename: pstnCtl.c
*
****************************************************************************
*    Description:
*
*      This module contains the functions of the PSTN call setup control.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */
#include <bosTask.h>                   /* Task control interface */
#include <bosSleep.h>                  /* Task delay interface */
#include <hdspCmd.h>                   /* DSP interface */
#include <hapi_pte.h>                  /* PTE Service */
#include <hapi_cmtd.h>                 /* CMTD Service */
#include <vrgLog.h>                    /* Logging interface */
#include <casCtl.h>                    /* For accessing CAS command API */
#include <cmtdCtl.h>                   /* CMTD control interface */
#include <pstnCtl.h>                   /* PSTN control interface */
#include <xdrvDaa.h>                   /* FXO line release interface */
#include <boardHalDaa.h>               /* Board HAL interface */
#include <leds.h>                      /* LED control wappers */
#include <boardHalRelay.h>             /* Board HAL interface */
#include <vrgProv.h>                   /* Access to Provision-able settings */
#include <boardHalProv.h>
#include <bosSem.h>                    /* BOS semaphore API */
#include <boardHalInit.h>              /* for boardHalIsPstnLifeLineSupported */

#define PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE     0
#define PSTNCTL_DTMFCLIDRX_DEBUG     0

/* ---- Private Constants and Types -------------------------------------- */
#define PSTN_DTMF_POWER    10          /* Tone power, in negative dBm */
#define PSTN_DTMF_ON       150         /* Tone on time, in ms */
#define PSTN_DTMF_OFF      150         /* Interval between tones, in ms */
#define PSTN_PAUSE         1000        /* Pause time, in ms */

/* event/state define for state machine switch() */
#define FSM(event, state) ((event * DTMFCLIDRX_MAXSTATES) + state)\

/* ---- Private Macros --------------------------------------------------- */

/* Start Timer */
#define START_TIMER(timer)      \
{                               \
   (timer).running = VRG_TRUE;  \
   (timer).counter = 0;         \
}

/* Reset Timer */
#define RESET_TIMER(timer)      \
{                               \
   (timer).counter = 0;         \
}

/* Stop Timer */
#define STOP_TIMER(timer)        \
{                                \
   (timer).running = VRG_FALSE;  \
   (timer).counter = 0;          \
}

/* Find the starting digit of a token */
#define IS_START_TOKEN( digitp, dtmfParmsp )     \
   ( *digitp == dtmfParmsp->startCode ||         \
     *digitp == dtmfParmsp->informationCode ||   \
     *digitp == dtmfParmsp->redirectingCode )

/* Find the stopping digit of a token. This can be the start
   of another token or the end of the sequence */
#define IS_STOP_TOKEN( digitp, dtmfParmsp )        \
   ( (*digitp == dtmfParmsp->startCode) ||         \
     (*digitp == dtmfParmsp->informationCode) ||   \
     (*digitp == dtmfParmsp->redirectingCode) ||   \
     (*digitp == dtmfParmsp->endCode) )

/* Convert HAPI DTMF digits to char format (based on ASCII) */
#define CONVERT_HAPI_TO_CHAR( digitp )           \
{                                                \
   if( *digitp < HAPI_PTE_DTMFDDGTSTR )          \
   {                                             \
      *digitp = *digitp + '0';                   \
   }                                             \
   else if( (*digitp >= HAPI_PTE_DTMFDDGTA) &&   \
            (*digitp <= HAPI_PTE_DTMFDDGTD) )    \
   {                                             \
      *digitp = *digitp + '5';                   \
   }                                             \
   else if( *digitp == HAPI_PTE_DTMFDDGTSTR )    \
   {                                             \
      *digitp = *digitp + ' ';                   \
   }                                             \
   else if( *digitp == HAPI_PTE_DTMFDDGTPND )    \
   {                                             \
      *digitp = *digitp + 'Z' - 'B';             \
   }                                             \
   else                                          \
   {                                             \
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "Conversion Error!" ));        \
   }                                             \
}

/* ---- Private Variables ------------------------------------------------ */

static PSTN_CTL_STATE pstnCtlState;    /* State of the PSTN service */

/* Binary semaphore for PSTN event queue */
static BOS_SEM pstnEvtQSem;

/* ---- Private Function Prototypes -------------------------------------- */
static void pstnCtlTaskMain( BOS_TASK_ARG context );
static HAPIRESULT genDialString( int lineId, PSTN_CTL_HANDLE vhdHdl, char *dialStringp );
static VRG_BOOL eventTaskCheckPendingReset( void );
static void pstnCtlEventProcess( PSTN_CTL_EVT *evtp );
static PSTN_CTL_CHAN_INFO  *getChanState( VRG_UINT16 vhdHdl );
static PSTN_CTL_STATUS resetDTMFClidRxState( PSTN_CTL_DTMFCLIDRX_STATE *state );
static void pstnCtlDTMFClidRxProcess( PSTN_CTL_EVT *evtp );
static void processDTMFClidRxEvent( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState, PSTN_CTL_EVT *evtp );
static void processDTMFClidRxStm( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState, PSTN_CTL_EVT *evtp );
static void parseDTMFClidRx( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState );
static VRG_UINT16 updateTimers( DTMFCLIDRX_STATE_INFO *stateInfo );

/* ---- Functions -------------------------------------------------------- */

/****************************************************************************
*
* FUNCTION:    pstnCtlSendEvent
*
* PURPOSE:     public function to post an event for the PSTN_CTL thread
*
* PARAMETERS:  evtp - a pointer to the event
*
* RETURNS:     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlSendEvent( PSTN_CTL_EVT *evtp )
{
   BUF_STATUS status;    /* Message queue status */
   BOS_MSGQ_MSG qMsg;    /* Message structure */
   void *ptr;            /* Temporary pointer */

   /* Initialize queue entry */
   memset( &qMsg, 0, sizeof( BOS_MSGQ_MSG ) );
   qMsg.buffer[0] = evtp->hsxdevhdl;
   qMsg.buffer[1] = evtp->hsxevt;
   qMsg.buffer[2] = evtp->hsxop1;
   if ( evtp->hsxop2.ref != NULL )
   {
#if defined(BRCM_NONDIST_BUILD)
      ptr = mallocAtomic( ( size_t )( strlen( ( char * )evtp->hsxop2.ref ) + 1) );
#else
      ptr = malloc( ( size_t )( strlen( ( char * )evtp->hsxop2.ref ) + 1) );
#endif

      if ( ptr == NULL )
      {
         return PSTN_CTL_FAILURE;
      }
      qMsg.buffer[3] = ( BOS_UINT32 )ptr;
      strcpy( ( char * )qMsg.buffer[3], ( char * )evtp->hsxop2.ref );
   }

   if ( pstnCtlState.evtQ == -1 )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "pstnCtlSendEvent: NULL QiD!" ));
      return ( PSTN_CTL_FAILURE );
   }

   if ( (status = memCircBufWrite(pstnCtlState.evtQ, (char*)&qMsg, sizeof(BOS_MSGQ_MSG))) == BUF_STATUS_OK )
   {
      bosSemGive( &pstnEvtQSem );
      return ( PSTN_CTL_SUCCESS );
   }
   else
   {
      return ( PSTN_CTL_FAILURE );
   }
}

/****************************************************************************
*
* FUNCTION:    eventGet
*
* PURPOSE:     get an event from the queue
*
* PARAMETERS:  event structure to be initialized
*
* RETURNS:     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE 
*
****************************************************************************/
static int eventGet( PSTN_CTL_EVT *evtp )
{
   BUF_STATUS     status;     /* return status */
   BOS_MSGQ_MSG   qMsg;       /* queue message structure */

   /* Get message from queue */
   bosSemTimedTake( &pstnEvtQSem, 0 );

   status = memCircBufRead(pstnCtlState.evtQ, (char*)&qMsg, sizeof(BOS_MSGQ_MSG));   
   if ( status == BUF_STATUS_EMPTY || status == BUF_STATUS_FAIL )
   {
      return ( PSTN_CTL_FAILURE );
   }
   else
   {
      /* Copy message to event block */
      evtp->hsxdevhdl  = (VRG_UINT16)qMsg.buffer[0];
      evtp->hsxevt     = (VRG_UINT16)qMsg.buffer[1];
      evtp->hsxop1     = (VRG_UINT16)qMsg.buffer[2];
      evtp->hsxreqid   = HSEVTDATA;
      evtp->hsxop2.ref = (void *)qMsg.buffer[3];

      return ( PSTN_CTL_SUCCESS );
   }
}

/****************************************************************************
*
* FUNCTION:    eventQueueCreate
*
* PURPOSE:     create an event queue
*
* PARAMETERS:  size of queue
*
* RETURNS:     queue Id, or 0 if error
*
****************************************************************************/
static MEMCIRCBUFDRV eventQueueCreate( VRG_UINT32 size )
{
   MEMCIRCBUFDRV bufDrv = -1;

   if ( memCircBufAlloc(&bufDrv, (size*sizeof(BOS_MSGQ_MSG))) != BUF_STATUS_OK )
   {
      return (-1);
   }

   return bufDrv;
}

/****************************************************************************
*
*  FUNCTION:   eventTaskCheckPendingReset
*
*  PURPOSE:    To determine if the PSTN task was notified of a pending
*              application reset.
*
*  PARAMETERS: none
*
*  RETURNS:    TRUE if PSTN was notified of an application reset, else FALSE
*
*  NOTE:       If the PSTN task was notified of a pending application
*              reset, it will acknowledge the notification. The acknowledge
*              will block the PSTN task until it is resumed. Once the PSTN
*              task is resumed, it should exit the main task function to
*              complete the task reset procedure.
*
****************************************************************************/
static VRG_BOOL eventTaskCheckPendingReset( void )
{
   if ( bosTaskIsResetPending() )
   {
      /* 
      ** The PSTN task has been notified that a reset is pending. Acknowledge
      ** the notification and then block until the PSTN task is resumed. 
      */
      bosTaskResetAck();

      return ( VRG_TRUE );
   }

   return ( VRG_FALSE );
}

/****************************************************************************
*
*  FUNCTION:   pstnCtlDeinit
*
*  PURPOSE:    Shuts down the PSTN task and frees all resources used by the 
*              PSTN_CTL module.
*
*  PARAMETERS: none
*
*  RETURNS:    PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE
*              This function should only return an error under catastrophic
*              circumstances. i.e. Something that cannot be fixed by 
*              re-invoking the module initialization function.
*
*  NOTES:      It is assumed that this function is only called after the PSTN
*              task has been notified of a pending application reset, and it
*              has acknowledged the notification. This implies that the PSTN
*              task is currently blocked, waiting to be resumed so that they
*              may complete the shutdown procedure.
*
*              It is also assumed that no task is currently blocked on any OS
*              resource that was created in the module initialization 
*              function.
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlDeinit( void )
{
   /* On deinit, destroy all DTMFClidRx parameters */
   pstnCtlDTMFClidRxDestroy( PSTN_CTL_DTMFDESTROY_ALL_HANDLE );

    if ( pstnCtlState.taskId )
   {
      /* 
      ** Currently, the PSTN task should be blocked in the 
      ** reset-acknowledgement state. Advance the PSTN task to the reset-active
      ** state. This will cause the task to resume operation. In this state, 
      ** the PSTN task should perform any operations required for proper 
      ** shutdown before exiting from it's main task function. 
      */
      bosTaskProceedToShutdown( &pstnCtlState.taskId );

      /* Wait for the PSTN task to complete de-initialization */
      bosTaskWaitForDone( &pstnCtlState.taskId );
   }

   /* Clear the task ID */
   pstnCtlState.taskId = 0;

   /* Release the message queue */
   if ( pstnCtlState.evtQ != -1 )
   {
      if ( memCircBufFree(&pstnCtlState.evtQ) != BUF_STATUS_OK )
      {
         return ( PSTN_CTL_FAILURE );
      }
   }

   /* Clear the message queue ID */
   pstnCtlState.evtQ = -1;
 
   /* Destroy PSTN event queue semaphore */  
   bosSemDestroy( &pstnEvtQSem );

   VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "PSTN_CTL shutdown complete!" ));
   return ( PSTN_CTL_SUCCESS );
}

/****************************************************************************
*
*  FUNCTION:   pstnCtlInit 
*
*  PURPOSE:    PSTN controller module init - call once during system init
*
*  PARAMETERS:
*              country  - country of operation
*              callback - event callback function
*
*  RETURNS:
*              PSTN_CTL_SUCCESS, or PSTN_CTL_FAILURE on error
*  NOTES:
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlInit( VRG_COUNTRY country, PSTN_CTL_CALLBACK callback )
{
   unsigned int i = 0;
    
   /* Init local storage */
   memset( &pstnCtlState, 0, sizeof( PSTN_CTL_STATE ) );

   pstnCtlState.callback = callback;
   pstnCtlState.country = country;

   for(i = 0; i < PSTN_CTL_MAX_CHANNELS; i++)
   {
      pstnCtlState.chanState[i].state = PSTN_CTL_CHAN_STATE_IDLE;
      pstnCtlState.chanState[i].vhdHdl = 0xFF;
      pstnCtlState.chanState[i].timeoutCount = 0;
   }
   
   /* Create PSTN event queue semaphore */
   if ( bosSemCreate("pstnEvtQSem", 0, 1, &pstnEvtQSem) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgEptCmdSem"));
      return ( PSTN_CTL_FAILURE );
   }

   /* Create message queue to hold PSTN_CTL events */
   pstnCtlState.evtQ = eventQueueCreate( PSTN_CTL_MAX_CHANNELS * 2 );
   if (!pstnCtlState.evtQ)
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "Error creating pstnCtlState.evtQ" ));
      return ( PSTN_CTL_FAILURE );
   }

   /* Create a thread "PSTN" */
   if ( BOS_STATUS_OK != bosTaskCreateEx( "PSTN",
                                          (8 * 1024),
                                          PSTN_CTL_CFG_TASK_PRIORITY,
                                          NULL,
                                          pstnCtlTaskMain,
                                          NULL,
                                          0,
                                          &pstnCtlState.taskId ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "Error creating PSTN_CTL task" ));
      return ( PSTN_CTL_FAILURE );
   }

   return ( PSTN_CTL_SUCCESS );
}


/****************************************************************************
*
*  FUNCTION:      pstnCtlEndptInit
*
*  PURPOSE:       call once per endpoint to enable PSTN call setup control
*
*  PARAMETERS:
*     handle      (in)  VHD handle for endpoint
*
*  RETURNS:
*     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE on error
*
*  NOTES:
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlEndptInit( PSTN_CTL_HANDLE handle )
{
   pstnCtlState.numChannels++;

   if ( pstnCtlState.numChannels > PSTN_CTL_MAX_CHANNELS )
   {
      return ( PSTN_CTL_FAILURE );
   }

   pstnCtlState.chanState[ pstnCtlState.numChannels - 1 ].state = 
      PSTN_CTL_CHAN_STATE_IDLE;
   pstnCtlState.chanState[ pstnCtlState.numChannels - 1 ].vhdHdl = handle;

   return ( PSTN_CTL_SUCCESS );
}

/****************************************************************************
*
*  FUNCTION:      pstnCtlDTMFClidRxInit
*
*  PURPOSE:       call once per endpoint to enable DTMF CLID Rx service. 
*                 Must call this after the VHD handle is registered using 
*                 pstnCtlEndptInit(). 
*
*  PARAMETERS:
*     handle      (in)  VHD handle for endpoint
*     dtmfParmsp  (in)  Pointer to DTMF CLID parms struct for the country
*
*  RETURNS:
*     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE on error
*
*  NOTES:
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlDTMFClidRxInit( VRG_COUNTRY country, PSTN_CTL_HANDLE handle, 
                                   PSTN_CTL_DTMF_CLID_PARMS *dtmfParmsp )
{
   PSTN_CTL_CHAN_INFO *chanState = getChanState( handle );
   int i;

   if( chanState == NULL )
   {
      /* PSTN Vhd Handle was not registered */
      return PSTN_CTL_FAILURE;
   }

   /* reset the CLID Rx structure */
   if( resetDTMFClidRxState( &chanState->dtmfClidRx ) != PSTN_CTL_SUCCESS )
   {
      return PSTN_CTL_FAILURE;
   }
   
   /* set necessary parameters that cant be reset */
   chanState->dtmfClidRx.vhdHdl = handle;
   chanState->dtmfClidRx.enabled = VRG_TRUE;
   
   /* set up timers based on the timers in DTMFCLIDRX_TIMER_TYPE enum */
   for( i=0; i < DTMFCLIDRX_MAX_TIMERS; i++ )
   {
      switch( i )
      {
         case DTMFCLIDRX_DTMF_PAUSE_TIMER:
         {
            chanState->dtmfClidRx.stateInfo.timer[i].timeout = dtmfParmsp->DTMFpauseTimeout;
            chanState->dtmfClidRx.stateInfo.timer[i].timeoutEvent |= DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT;
            chanState->dtmfClidRx.stateInfo.timer[i].type = DTMFCLIDRX_DTMF_PAUSE_TIMER;
         }
         break;

         case DTMFCLIDRX_SESSION_TIMER:
         {
            chanState->dtmfClidRx.stateInfo.timer[i].timeout = dtmfParmsp->DTMFsessionTimeout;
            chanState->dtmfClidRx.stateInfo.timer[i].timeoutEvent |= DTMFCLIDRX_EVT_SESSION_TIMEOUT;
            chanState->dtmfClidRx.stateInfo.timer[i].type = DTMFCLIDRX_SESSION_TIMER;
         }
         break;

         case DTMFCLIDRX_RINGAFTERCLID_TIMER:
         {
            chanState->dtmfClidRx.stateInfo.timer[i].timeout = dtmfParmsp->ringAfterClidTimeout;
            chanState->dtmfClidRx.stateInfo.timer[i].timeoutEvent |= DTMFCLIDRX_EVT_RINGAFTERCLID_TIMEOUT;
            chanState->dtmfClidRx.stateInfo.timer[i].type = DTMFCLIDRX_RINGAFTERCLID_TIMER;
         }
         break;

         default:
         {
         }
      }
   }
#if PSTNCTL_DTMFCLIDRX_DEBUG

   /* Dump DTMF parms: */
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "DTMF CLID Parms:"));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "dbLevel = %u",  dtmfParmsp->dbLevel ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "pulseTime = %u",  dtmfParmsp->pulseTime ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "startCode = 0x%04x",  dtmfParmsp->startCode ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "endCode = 0x%04x", dtmfParmsp->endCode ));

   int k=0;
   for( i=1; i < 17; i++)
   {
      switch( k )
      {
         case 0:
         {
            if( i == 1 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "privateCode = " ));
            }
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->privateCode[i-1] ));
            if( i == 16 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->privateCode[i-1] ));
               k++;
               i=0;
            }
         }
         break;

         case 1:
         {
            if( i == 1 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "unavailCode = " ));
            }
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->unavailCode[i-1] ));
            if( i == 16 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->unavailCode[i-1] ));
               k++;
               i=0;
            }
         }
         break;

         case 2:
         {
            if( i == 1 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "errorCode = " ));
            }
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->errorCode[i-1] ));
            if( i == 16 )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", dtmfParmsp->errorCode[i-1] ));
            }
         }
         break;

         default:
         break;
      }
   }
   
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "redirectingCode = 0x%04x", dtmfParmsp->redirectingCode ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "informationCode = 0x%04x", dtmfParmsp->informationCode ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "DTMFpauseTimeout = %u", dtmfParmsp->DTMFpauseTimeout ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "DTMFsessionTimeout = %u", dtmfParmsp->DTMFsessionTimeout ));
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "ringAfterClidTimeout = %u", dtmfParmsp->ringAfterClidTimeout ));

#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   /* malloc the memory for dtmfClidParms. This needs to be copied entirely because dtmfParmsp is local to this function only */
   chanState->dtmfClidRx.dtmfClidParms = (PSTN_CTL_DTMF_CLID_PARMS*)malloc(sizeof(PSTN_CTL_DTMF_CLID_PARMS));
    memcpy(chanState->dtmfClidRx.dtmfClidParms, dtmfParmsp, sizeof(CLASS_MSG_DTMF_PARMS));
   chanState->dtmfClidRx.country = country;
   
   return PSTN_CTL_SUCCESS;
}


/****************************************************************************
*
*  FUNCTION:      pstnCtlDTMFClidRxDestroy
*
*  PURPOSE:       call once per endpoint to destroy DTMF CLID Rx resources. 
*
*  PARAMETERS:
*     handle      (in)  VHD handle for endpoint
*
*  RETURNS:
*     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE on error
*
*  NOTES:         to destroy all resources for all handles, pass
*                 PSTN_CTL_DTMFDESTROY_ALL_HANDLE.
*
****************************************************************************/
PSTN_CTL_STATUS pstnCtlDTMFClidRxDestroy( PSTN_CTL_HANDLE handle )
{
   int i;

   /* if handle is special all handle, we want to destroy all endpoint dtmfclid info */
   if(handle == PSTN_CTL_DTMFDESTROY_ALL_HANDLE)
   {    
      /* free the dmtfClidRx memory */
      for(i = 0; i < PSTN_CTL_MAX_CHANNELS; i++)
      {
         if(pstnCtlState.chanState[i].vhdHdl != 0xFF && pstnCtlState.chanState[i].dtmfClidRx.dtmfClidParms != NULL)
         {
            VRG_LOG_DBG((VRG_LOG_MOD_EPT, "pstnCtlDTMFClidRxDestroy: Freeing resources for channel %d", i));
            free(pstnCtlState.chanState[i].dtmfClidRx.dtmfClidParms);
         }
      }
   }
   /* otherwise assume we are trying to destroy a particular handle */
   else
   {
      /* free the dtmfClidRx memory */
      for(i = 0; i < PSTN_CTL_MAX_CHANNELS; i++)
      {
         /* if we found our handle */
         if(pstnCtlState.chanState[i].vhdHdl == handle && pstnCtlState.chanState[i].dtmfClidRx.dtmfClidParms != NULL)
         {
            VRG_LOG_DBG((VRG_LOG_MOD_EPT, "pstnCtlDTMFClidRxDestroy: Freeing resources for specific channel %d", i));
            free(pstnCtlState.chanState[i].dtmfClidRx.dtmfClidParms);
         }
      }
   }
   return PSTN_CTL_SUCCESS;
}


/****************************************************************************
*
*  FUNCTION:      resetDTMFClidRxState
*
*  PURPOSE:       resets the CLID Rx service structure. Can be used for init 
*                 or reset of service. Some parameters must be set at the init 
*                 stage after this function call, and should not be reset after 
*                 that point (dtmfParms for ex)
*
*  PARAMETERS:
*     state       (in)  pointer to the CLID Rx state structure
*
*  RETURNS:
*     PSTN_CTL_SUCCESS or PSTN_CTL_FAILURE on error
*
*  NOTES:
*
****************************************************************************/
PSTN_CTL_STATUS resetDTMFClidRxState( PSTN_CTL_DTMFCLIDRX_STATE *state )
{
   int i;

   state->stateInfo.serviceState = DTMFCLIDRX_IDLE;
   state->stateInfo.offHook = VRG_FALSE;
   state->stateInfo.ringState = DTMFCLIDRX_NORING;
   state->stateInfo.clidType = DTMFCLIDRX_INVALID_CLID;

   for( i=0; i < DTMFCLIDRX_MAX_TIMERS; i++ )
   {
      state->stateInfo.timer[i].counter = 0;
      state->stateInfo.timer[i].running = VRG_FALSE;
   }

   state->internalEvents &= DTMFCLIDRX_EVT_NO_INTERNAL_EVENTS;
   state->eventSent = VRG_FALSE;
   
   memset( &state->clidInfo, 0, sizeof( DTMFCLIDRX_CLID_INFO ) );

   return PSTN_CTL_SUCCESS;
}

/****************************************************************************
*
*  FUNCTION:   pstnCtlTaskMain
*
*  PURPOSE:    Main loop for the PSTN call setup control task
*
*  PARAMETERS:
*     context  Not used 
*
*  RETURNS:
*     Nothing
*
*  NOTES:       
*     This function should not be called directly. It should be registered 
*     with the OS and invoked indirectly.
*
****************************************************************************/
static void pstnCtlTaskMain( BOS_TASK_ARG context )
{
   BOS_STATUS  status;
   PSTN_CTL_EVT evt;
   PSTN_CTL_EVENT_INFO eventInfo;

   unsigned int i = 0;

   (void) context;

   while ( 1 )
   {
      status = bosSleep( PSTN_CTL_CFG_TASK_RATE_MS );
      if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_EPT, "Exiting task PSTN" ));
         return;
      }

      /* check if any of the initialized PSTN channels has a timeout while waiting for dialtone*/
      for (i = 0; i < PSTN_CTL_MAX_CHANNELS; i++)
      {
         if ((pstnCtlState.chanState[i].vhdHdl != 0xFF) && 
             (pstnCtlState.chanState[i].state == PSTN_CTL_CHAN_STATE_WAIT_DIAL_TONE) )
         {
            pstnCtlState.chanState[i].timeoutCount += PSTN_CTL_CFG_TASK_RATE_MS;
            if (pstnCtlState.chanState[i].timeoutCount== PSTN_CPTONE_TIMEOUT_PERIOD)
            {
               /* disable CMTD service */
               if ( HAPISUCCESS != cmtdCtlDisable( pstnCtlState.chanState[i].vhdHdl ) )
               {
                  VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable FAILED!!!" ));
               }
               else
               {
                  VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable success vhd=0x%x", 
                            pstnCtlState.chanState[i].vhdHdl ));
               }

               eventInfo.event = PSTN_CTL_EVT_CPTONE_TIMEOUT;
               eventInfo.pInfo = NULL;

               pstnCtlState.chanState[i].state = PSTN_CTL_CHAN_STATE_IDLE;
               pstnCtlState.chanState[i].timeoutCount = 0;
               pstnCtlState.callback( pstnCtlState.chanState[i].vhdHdl, 
                                      &eventInfo );
            }
         }
      }

      /* Get an event from the PSTN_CTL event queue - blocking call */
      status = eventGet( &evt );

      if ( eventTaskCheckPendingReset() == VRG_TRUE )
      {
         /* 
         ** A reset has been initiated. The PSTN_CTL task should stop running
         ** by exiting from this main task function.
         */
         return;
      }

#if PSTNCTL_DTMFCLIDRX_DEBUG
      if( status == PSTN_CTL_SUCCESS)
      {
         /* Ignore NONE evt */
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "pstnCtlTaskMain: evt.hsxdevhdl = %u, evt.hsxevt = %u, evt.hsxop1 = %u", 
                 evt.hsxdevhdl, evt.hsxevt, evt.hsxop1 ));
      }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      if( (int)status != (int)PSTN_CTL_SUCCESS )
      {
         /* No event retrieved from queue, pass in NONE event */
         evt.hsxevt = PSTN_CTL_EVT_NONE;
      }
      pstnCtlDTMFClidRxProcess( &evt );

      if ( (int)status == (int)PSTN_CTL_SUCCESS )
      {
         pstnCtlEventProcess( &evt );

         /* free event data buffer if present */
         if ( evt.hsxop2.ref != NULL )
         {
            free( evt.hsxop2.ref );
         }
      }
   }
}

/*****************************************************************************
*  FUNCTION:   genDialString
*
*  PURPOSE:    Compose a PTE dialstring generate command
*
*  PARAMETERS: vhdHdl      - VHD handle
*              dialStringp - dial string digits, in a null terminated string
*
*  RETURNS:    HAPIRESULT
*
*  NOTE:       This executes from within the context of the PSTN_CTL task.
*              Valid digits are 0-9, *, #, A, B, C, D, and P for pause.
*
*****************************************************************************/
HAPIRESULT genDialString( int lineId, PSTN_CTL_HANDLE vhdHdl, char *dialStringp )
{
   int i;                                       /* Character counter */
   VRG_UINT8 * pDigits = NULL;                   /* Temporary digit string */
   HAPI_PTE_DIALSTRING dialString;              /* HAPI dialstring command */
   VRG_UINT32 skew;
   
   /* Initialize digit pointer */
   pDigits = (VRG_UINT8*)&dialString.digitArray[0];

   /* Initialize dialstring parameters */
   dialString.dBmLevel = PSTN_DTMF_POWER;

   endptProvGetValue(lineId, EPPROV_DTMFSkew, &skew);
   dialString.dBmSkew = skew;

   dialString.encoderType = 0; /* DTMF */
   dialString.numCharacters = strlen( dialStringp );
   dialString.pauseTime = PSTN_PAUSE * 8;
   dialString.pulseOffTime = PSTN_DTMF_OFF * 8;
   dialString.pulseOnTime = PSTN_DTMF_ON * 8;

   /* Check if dialstring fits */
   if( dialString.numCharacters > HAPI_PTE_GEN_MAX_DIGITS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: Too many digits %d in dial string. Limit is %d!!!",dialString.numCharacters
                                                                                                    ,HAPI_PTE_GEN_MAX_DIGITS  )); 
      return HAPIFAILURE;                                                                                                       
   }
   
   /* Encode dial string */
   for ( i = 0; i < dialString.numCharacters; i++ )
   {
      switch ( dialStringp[i] )
      {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
         {
            pDigits[i] = dialStringp[i] - '0' + HAPI_PTE_DTMFDDGT0;
         }
         break;

         case 'A':
         case 'B':
         case 'C':
         case 'D':
         {
            pDigits[i] = dialStringp[i] - 'A' + HAPI_PTE_DTMFDDGTA;
         }
         break;

         case '*':
         {
            pDigits[i] = HAPI_PTE_DTMFDDGTSTR;
         }
         break;

         case '#':
         {
            pDigits[i] = HAPI_PTE_DTMFDDGTPND;
         }
         break;
         
         case 'P':
         {
            /* Insert a pause during the dialing, defined by pauseTime */
            pDigits[i] = HAPI_PTE_PAUSE_DIGIT;
         }
         break;

         default:
         {
            VRG_LOG_WARN((VRG_LOG_MOD_EPT,  "PSTN_CTL: Invalid dialstring digit (%c)!!!", 
                      dialStringp[i] ));
         }
         break;
      }      
   }
   
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: genDialString %d digits (%s)", 
             dialString.numCharacters, dialStringp ));
   
   /* Send dial string command to PTE service */
   return hdspSendCmdDataAsync( vhdHdl, 
                                HAPI_PTE_DIALSTRING_CMD, 
                                HSCMDEXTDATA_ASYNC, 
                                sizeof( HAPI_PTE_DIALSTRING ), 
                                ( void* )&dialString );
}

/*****************************************************************************
*  FUNCTION:   getChanState
*
*  PURPOSE:    Lookup for channel state based on VHD handle
*
*  PARAMETERS: vhdHdl - VHD Handle
*
*  RETURNS:    nothing
*
*  NOTE:
*
*****************************************************************************/
static PSTN_CTL_CHAN_INFO *getChanState( VRG_UINT16 vhdHdl )
{
   int i;

   /* Find the VHD in the channel info */
   for ( i = 0; i < PSTN_CTL_MAX_CHANNELS; i++ )
   {
      if ( pstnCtlState.chanState[ i ].vhdHdl == vhdHdl )
      {
         return &pstnCtlState.chanState[ i ];
      }
   }
   return NULL;
}

/*****************************************************************************
*
*  FUNCTION:   pstnCtlEventProcess
*
*  PURPOSE:    Event handler for various PSTN_CTL events
*
*  PARAMETERS: evtp   - event message
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the PSTN_CTL task
*
*****************************************************************************/
static void pstnCtlEventProcess( PSTN_CTL_EVT *evtp )
{
   PSTN_CTL_CHAN_INFO  *chanState = getChanState( evtp->hsxdevhdl );
   PSTN_CTL_EVENT_INFO eventInfo;

   if ( chanState == NULL )
   {
      return; /* unregistered PSTN VHD handle */
   }

   /*
   ** Switch on the events of interest.
   */
   switch( evtp->hsxevt )
   {
      case PSTN_CTL_EVT_CALL_SETUP: /* call setup event */
      {
         if ( chanState->state == PSTN_CTL_CHAN_STATE_IDLE )
         {
            if (evtp->hsxop2.ref != NULL)
            {
               strcpy( chanState->dialString, ( char * )evtp->hsxop2.ref );
            }
            else
            {
               chanState->dialString[0] = 0;
            }

            /* Check if we just want to make a pstn connection only */
            if ( strcmp( chanState->dialString, PSTN_CONNECTONLY_DLSTRNG ) !=0 )
            {
                                 
               chanState->state = PSTN_CTL_CHAN_STATE_WAIT_DIAL_TONE;
               if ( HAPISUCCESS != cmtdCtlConfigFilter( chanState->vhdHdl, 
                                                     pstnCtlState.country, 
                                                     CMTD_TONE_TYPE_DIAL,
                                                     CMTD_FILTER_1 ) )
               {
                  VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlConfigFilter FAILED!!!" ));
               }
               else
               {
                  VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlConfigFilter success vhd=0x%x", 
                            chanState->vhdHdl ));
               }
            }
            else
            {
               /* Bypass CMTD and dialstring generation, just connect to pstn */
               chanState->state = PSTN_CTL_CHAN_STATE_IDLE;
               eventInfo.event = PSTN_CTL_EVT_CALL_SETUP_COMPLETE;
               eventInfo.pInfo = NULL;
               pstnCtlState.callback( chanState->vhdHdl, 
                                      &eventInfo );         
            }
         }
         else
         {
            VRG_LOG_INFO((VRG_LOG_MOD_EPT,  "PSTN_CTL: Dialing in progress, "
                      "ignoring new dial command." ));
         }
      }
      break;

      case PSTN_CTL_EVT_CPTONE_DETECT: /* dial tone detect */
      {
         CMTD_FILTER_INDEX filterIndex = CMTD_FILTER_MAX;
         CMTD_TONE_TYPE detectedTone;

         int lineId = (int)(evtp->hsxop1);

         if ( evtp->hsxop2.ref != NULL )
         {
            /* Convert op2 from char to CMTD_FILTER_INDEX */
            filterIndex = ( CMTD_FILTER_INDEX )( *( char* )evtp->hsxop2.ref );
         }

         /* Determine which tone triggered the detection event */
         detectedTone = cmtdCtlGetConfigTone( filterIndex );

         /* Log which tone was detected */
         VRG_LOG_DBG( (VRG_LOG_MOD_EPT, "PSTN_CTL: %s detected!", cmtdCtlMapToneType( detectedTone ) ) );
                   
         if ( chanState->state == PSTN_CTL_CHAN_STATE_WAIT_DIAL_TONE )
         {
            /* reset the timeout */
            chanState->timeoutCount = 0;

            /* Disable dial tone detection and CMTD service */
            if ( HAPISUCCESS != cmtdCtlDisable( chanState->vhdHdl ) )
            {
               VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable FAILED!!!" ));
            }
            else
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable success vhd=0x%x", 
                         chanState->vhdHdl ));
            }

            chanState->state = PSTN_CTL_CHAN_STATE_GENERATE_DTMF;
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: generating digits: %s!!!", 
                      chanState->dialString ));
            genDialString( lineId, chanState->vhdHdl, chanState->dialString );
         }
         else if ( chanState->state == PSTN_CTL_CHAN_STATE_IDLE ) /* Busy tone detected */
         {
            detectedTone = CMTD_TONE_TYPE_BUSY;
            /* disable CMTD service */
            if ( HAPISUCCESS != cmtdCtlDisable( chanState->vhdHdl ) )
            {
               VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable FAILED!!!" ));
            }
            else
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlDisable success vhd=0x%x",
                         chanState->vhdHdl ));
            }

            /* Additional logic can be added here to release FXO line, etc */
         }
         /* Log which tone was detected */
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,"PSTN_CTL: %s detected!", cmtdCtlMapToneType( detectedTone ) ) );
      }
      break;
      
      case PSTN_CTL_EVT_DIAL_COMPLETE: /* dialstring gen complete */
      {
         if ( chanState->state == PSTN_CTL_CHAN_STATE_GENERATE_DTMF )
         {
            chanState->state = PSTN_CTL_CHAN_STATE_IDLE;

            eventInfo.event = PSTN_CTL_EVT_CALL_SETUP_COMPLETE;
            eventInfo.pInfo = NULL;

            pstnCtlState.callback( chanState->vhdHdl, 
                                   &eventInfo );

            /* Enable Release Tone detection */
            if ( HAPISUCCESS != cmtdCtlConfigFilter( chanState->vhdHdl, 
                                                     pstnCtlState.country,
                                                     CMTD_TONE_TYPE_RELEASE,
                                                     CMTD_FILTER_1 ) )
            {
               VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlConfigFilter FAILED!!!" ));
            }
            else
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "PSTN_CTL: cmtdCtlConfigFilter success vhd=0x%x", 
                         chanState->vhdHdl ));
            }
         }
      }
      break;

      case PSTN_CTL_EVT_ANSWER: 
      {
         if ( chanState->state == PSTN_CTL_CHAN_STATE_IDLE )
         {

          
            if ( HAPISUCCESS != cmtdCtlConfigFilter( chanState->vhdHdl, 
                                                     pstnCtlState.country,
                                                     CMTD_TONE_TYPE_RELEASE,
                                                     CMTD_FILTER_1 ) )
            {
               VRG_LOG(( "PSTN_CTL: cmtdCtlConfigFilter FAILED!!!\n" ));
            }
            else
            {
               VRG_LOG(( "PSTN_CTL: cmtdCtlConfigFilter success vhd=0x%x\n", 
                         chanState->vhdHdl ));
            }
         }
		    
      }
      break;  

      default:
      break;
   }
}


#if PSTN_LIFE_LINE_SUPPORT
/* pstn lifeline support
*****************************************************************************
** FUNCTION:   pstnCtlGetHookStatus
**
** PURPOSE:    This function is used to get current FXO line hook status.
**
** PARAMETERS: status  (out) hook status.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
PSTN_CTL_STATUS pstnCtlGetHookStatus( PSTN_CTL_HOOK_STAT *status )
{
   if ( boardHalIsPstnLifeLineSupported() )
   {
      CAS_CTL_EXT_CMD_PARM parms;

      /* By default set status onhook, so when PSTN is not configured
       * the FXO status is alway onhook
       */
   
      parms.drvHdl = (void*)boardHalDaaGetDriver( boardHalGetNumEndpoints() - boardHalGetNumFxoEndpoints() );

      if( casCtlSendExtendedCommand( CAS_REQ_DAA_GET_HOOK_STATUS, CAS_CMD_MODE_SYNC, &parms ) == CAS_CTL_SUCCESS && 
          parms.arguments[0] != 0 )
      {
         *status = PSTN_CTL_STAT_OFFHOOK;
      }
      else
      {
         *status = PSTN_CTL_STAT_ONHOOK;
      }
   }   

   return (PSTN_CTL_SUCCESS);
}

/* pstn lifeline support
*****************************************************************************
** FUNCTION:   pstnCtlSetRelayStat
**
** PURPOSE:    This function is used to set voice channel relay stat.
**
** PARAMETERS: status  (out) hook status.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
PSTN_CTL_STATUS pstnCtlSetRelayStat( PSTN_CTL_RELAY_CMD cmd )
{
   XDRV_RELAY         *relayDriver;
   int                i;

   VRG_LOG_DBG((VRG_LOG_MOD_EPT, "vrgEndptSetRelayStat(), cmd =%d", cmd));

   for ( i = 0; i < boardHalGetNumFailoverRelays(); i++ )
   {
      relayDriver = boardHalRelayGetDriver( i );
   
      xdrvRelaySetActive( relayDriver, cmd ); 
   }

   return (PSTN_CTL_SUCCESS);
}

#endif /* PSTN_LIFE_LINE_SUPPORT */

/*****************************************************************************
*
*  FUNCTION:   pstnCtlDTMFClidRxProcess
*
*  PURPOSE:    Main process function for DTMF Clid Rx Service
*
*  PARAMETERS: none
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the PSTN_CTL task.
*              This DTMF CLID Rx service is designed to run for any # of 
*              channels, but under the current event send/get it is best suited
*              to run for 1 FXO endpt, which is the usual case. If it is 
*              necessary to run multiple FXO endpts, note that the event buffer
*              may be over flowed, and it may be better to have seperate queues
*              for each endpoint (This would allow us to process 1 event for each
*              endpoint if desired) or a larger queue (This is for worst case where 
*              we have multiple incoming calls at the same time). Also note that
*              right now only 1 event is processed per iteration of the PSTN task.
*              This will not matter to the CLID Rx service unless there is extensive
*              delay in the system and we reach the pause or session timeouts. This
*              is highly unlikely.
*
*****************************************************************************/
static void pstnCtlDTMFClidRxProcess( PSTN_CTL_EVT *evtp )
{
   int i;
   PSTN_CTL_EVT nullEvt;

   /* Create a null event for our use */
   nullEvt.hsxdevhdl = 0x0000;
   nullEvt.hsxevt = PSTN_CTL_EVT_NONE;

   /* For each channel run the DTMF Clid Rx service. This assumes that each channel 
      up to numChannels has its own vhdHdl (that we were initialized properly for 
      each endpt through pstnCtlEndptInit()). The Clid Rx service was initialized 
      if its version of the vhdHdl is the same as the channels version and the 
      enabled flag is true */
   
   for ( i = 0; i < pstnCtlState.numChannels; i++ )
   {
      if( (pstnCtlState.chanState[ i ].vhdHdl == pstnCtlState.chanState[ i ].dtmfClidRx.vhdHdl) &&
          (pstnCtlState.chanState[ i ].dtmfClidRx.enabled == VRG_TRUE) )
      {
         /* Pass in the current event to the channel that matches the vhdHdl */
         if ( pstnCtlState.chanState[ i ].dtmfClidRx.vhdHdl == evtp->hsxdevhdl )
         {
#if PSTNCTL_DTMFCLIDRX_DEBUG
            if( evtp->hsxevt != PSTN_CTL_EVT_NONE )
            {
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "chanState[ %d ].vhdHdl = %d, evtp->hsxevt = %d", i, 
                       pstnCtlState.chanState[ i ].vhdHdl, evtp->hsxevt ));
            }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
            processDTMFClidRxEvent( &pstnCtlState.chanState[ i ].dtmfClidRx, evtp );
         }
         else
         {
            processDTMFClidRxEvent( &pstnCtlState.chanState[ i ].dtmfClidRx, &nullEvt );
         }
      }
   }

   return;
}

/*****************************************************************************
*
*  FUNCTION:   processDTMFClidRxEvent
*
*  PURPOSE:    Main event process function for an instnace of the DTMF Clid 
*              Rx Service
*
*  PARAMETERS: clidRxState - main state structure for DTMF Clid Rx on channel
*              evtp        - pointer to external event
*
*  RETURNS:    nothing
*
*  NOTE:       This executes from within the context of the PSTN_CTL task. This 
*              function and the state machine are in charge of collecting the 
*              CLID properly and sending up an event upon completion with either
*              the final CLID or an event marking invalid CLID.
*
*****************************************************************************/
static void processDTMFClidRxEvent( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState, PSTN_CTL_EVT *evtp )
{
   PSTN_CTL_EVENT_INFO eventInfo;
   PSTN_CTL_CLID_EVENT_INFO clidEventInfo;

   /* Update timers based on task rate and collect timeouts for the state machine*/
   clidRxState->internalEvents |= updateTimers( &clidRxState->stateInfo );

#if PSTNCTL_DTMFCLIDRX_DEBUG
   if( evtp->hsxevt != PSTN_CTL_EVT_NONE )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Pre-State Machine Events: internalEvents = 0x%04x",
              clidRxState->internalEvents ));
   }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   /* Update the state machine with the external and internal events */
   processDTMFClidRxStm( clidRxState, evtp );

   /* Check internal events and process events as needed */

#if PSTNCTL_DTMFCLIDRX_DEBUG
   if( evtp->hsxevt != PSTN_CTL_EVT_NONE )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Post-State Machine Events: internalEvents = 0x%04x",
              clidRxState->internalEvents ));
   }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_NEEDRESET) ||
       (clidRxState->internalEvents & DTMFCLIDRX_EVT_ERROR) )
   {
      /* Need to reset the state machine to idle so it is ready to run again. It
         will only send the NEEDRESET event once it has sent its CLID events to 
         the client application and has recieved the proper events notifying it
         that the call was completed/aborted and the channel is idle again, or
         if a timeout requires us to reset the state machine */

#if PSTNCTL_DTMFCLIDRX_DEBUG
      
      /* Dump the state of the DTMF CLID Rx service */
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "**Current State**"));
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Event: %u, op1: %u, State: %u, OffHook: %u, RingState: %u", 
              evtp->hsxevt, evtp->hsxop1, clidRxState->stateInfo.serviceState,
              clidRxState->stateInfo.offHook, clidRxState->stateInfo.ringState ));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      resetDTMFClidRxState( clidRxState );
      return;
   }

   if( clidRxState->internalEvents & DTMFCLIDRX_EVT_DTMF_PARSE_ERROR )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxEvent: Error Parsing DTMF CLID!" ));
   }
   
   /* Set up the CLID events, but only send once we know that we can (below). We
      will assume that at that point the correct info is in the right structures */
   clidEventInfo.clidType = clidRxState->stateInfo.clidType;
   clidEventInfo.clidData = clidRxState->clidInfo.parsedData;
   eventInfo.pInfo = &clidEventInfo;

   /* Check if valid/partial/invalid CLID was found and send the corresponding event.
      Once the event is sent we should not send anymore up */
   if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_VALID_DTMF_CLID) &&
       (clidRxState->eventSent == VRG_FALSE) )
   {
      eventInfo.event = PSTN_CTL_EVT_VALID_DTMFCLID;
      clidRxState->eventSent = VRG_TRUE;

      pstnCtlState.callback( clidRxState->vhdHdl, &eventInfo );

   }
   else if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_PARTIAL_DTMF_CLID) &&
            (clidRxState->eventSent == VRG_FALSE) )
   {
      eventInfo.event = PSTN_CTL_EVT_PARTIAL_DTMFCLID;
      clidRxState->eventSent = VRG_TRUE;

      pstnCtlState.callback( clidRxState->vhdHdl, &eventInfo );
   }
   else if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_INVALID_DTMF_CLID) &&
            (clidRxState->eventSent == VRG_FALSE) )
   {
      eventInfo.event = PSTN_CTL_EVT_INVALID_DTMFCLID;
      eventInfo.pInfo = NULL;
      clidRxState->eventSent = VRG_TRUE;

      pstnCtlState.callback( clidRxState->vhdHdl, &eventInfo );
   }

   return;
}


/*****************************************************************************
*
*  FUNCTION:   processDTMFClidRxStm
*
*  PURPOSE:    Main state machine for the Clid Rx service. This will run based
*              on all events recieved (external and internal), and raise its
*              own internal events if necessary. It will collect the DTMF 
*              string and if it is valid, parse it. The parser will decide 
*              whether the CLID collected is valid.
*
*  PARAMETERS: clidRxState - main state structure for DTMF Clid Rx on channel
*              evtp        - pointer to external event
*
*  RETURNS:    PSTN_CTL_FAILURE if an error occured in stm and reset is needed
*
*  NOTE:       This executes from within the context of the PSTN_CTL task
*
*****************************************************************************/
static void processDTMFClidRxStm( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState, PSTN_CTL_EVT *evtp )
{
   /* Create pointers to different parts of the DTMF CLID Rx instance structure */
   DTMFCLIDRX_STATE_INFO *stateInfo = &clidRxState->stateInfo;
   DTMFCLIDRX_CLID_INFO *clidInfo = &clidRxState->clidInfo;
   PSTN_CTL_DTMF_CLID_PARMS *dtmfParms = 
                           (PSTN_CTL_DTMF_CLID_PARMS *)clidRxState->dtmfClidParms;

#if PSTNCTL_DTMFCLIDRX_DEBUG
   if( !(evtp->hsxevt == PSTN_CTL_EVT_NONE) )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "processClidRxStm: FSM: Event: %u, State: %u", evtp->hsxevt, stateInfo->serviceState ));
   }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   switch( FSM( evtp->hsxevt, stateInfo->serviceState ) )
   {
      case FSM( PSTN_CTL_EVT_NONE, DTMFCLIDRX_IDLE ):
      {
         /* We dont have a relevant event to process. Also shouldnt have any time
            outs to check for in IDLE state, as we shouldnt be timing anything if 
            we are IDLE. Do nothing. */
      }
      break;

      case FSM( PSTN_CTL_EVT_DTMF_DIGIT, DTMFCLIDRX_IDLE ):
      {
         
         /* We are beginning a DTMF CLID session, check ring and hook 
               states first */
         if(  !stateInfo->offHook )
         {
            /* Check to see if it is a start digit to a DTMF CLID sequence. It 
            can either be the normal start digit, redirecting number digit,
            or an information sequence start digit (See macro) */
            if( IS_START_TOKEN( &evtp->hsxop1, dtmfParms ) )
            {
               /* Type I CLID, check the ring state */
               if( (stateInfo->ringState == DTMFCLIDRX_NORING) ||
                   (stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF) )
               {
                  if( stateInfo->ringState == DTMFCLIDRX_NORING )
                  {
                     stateInfo->clidType = DTMFCLIDRX_PRERING_CLID;
                  }
                  else
                  {
                     stateInfo->clidType = DTMFCLIDRX_FIRSTRING_CLID;
                  }
                  /* set CLID type */
                  stateInfo->serviceState = DTMFCLIDRX_COLLECTING;
                  
                  /* store the digit in the collection buffer (collectBufLen 
                     should be 0 at this point) */
                  clidInfo->collectBuffer[clidInfo->collectBufLen] = evtp->hsxop1;
                  clidInfo->collectBufLen++;
                  
                  /* launch the CLID session timer and the DTMF pause timer */
                  START_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
                  START_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);
               }
               else
               {
                  /* We are already past the second ring, no onhook CLID now,
                     we will wait for the ring to end and the line to go idle */
                  stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
               }
            }
            else
            {
               /* else bad token */
               stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
#if PSTNCTL_DTMFCLIDRX_DEBUG
               VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "processClidRxStm: Bad Token!"));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
            }
         } 
         else
         {
            /* Not supporting Type II */
            stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
#if PSTNCTL_DTMFCLIDRX_DEBUG
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "processClidRxStm: DTMF detected while offhook!"));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
         }
      }
      break;
      
      case FSM( PSTN_CTL_EVT_RINGON, DTMFCLIDRX_IDLE ):
      {
         if( stateInfo->ringState == DTMFCLIDRX_NORING )
         {
            /* Wait for off event, then we know we have our first ring */
            stateInfo->ringState = DTMFCLIDRX_FIRSTRINGON;
         }
         else if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF )
         {
            /* We already had our first ring, this is the second. Set state 
               to DTMFCLIDRX_SECONDRING so we know we have had a min of two rings 
               already */
            stateInfo->ringState = DTMFCLIDRX_SECONDRING;
         }
      }
      break;
      
      case FSM( PSTN_CTL_EVT_RINGOFF, DTMFCLIDRX_IDLE ):
      {
         /* We have the first ring, other ring off events dont matter */
         if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGON )
         {
            stateInfo->ringState = DTMFCLIDRX_FIRSTRINGOFF;
         }
      }
      break;
      
      case FSM( PSTN_CTL_EVT_RINGEND, DTMFCLIDRX_IDLE ):
      {
         /* Set ring state to no ring */
         if( stateInfo->ringState != DTMFCLIDRX_NORING )
         {
            stateInfo->ringState = DTMFCLIDRX_NORING;
         }
      }
      break;
      
      case FSM( PSTN_CTL_EVT_OFFHOOK, DTMFCLIDRX_IDLE ):
      {
         /* We went off-hook on the DAA */
         stateInfo->offHook = VRG_TRUE;
         stateInfo->ringState = DTMFCLIDRX_RINGEND;
      }
      break;
      
      case FSM( PSTN_CTL_EVT_ONHOOK, DTMFCLIDRX_IDLE ):
      {
         /* We went on-hook on the DAA, the line is idle now, reset just 
            incase */
         stateInfo->offHook = VRG_FALSE;
         stateInfo->ringState = DTMFCLIDRX_NORING;
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
      }
      break;
      
      case FSM( PSTN_CTL_EVT_NONE, DTMFCLIDRX_COLLECTING ):
      {
         /* Unlike the IDLE case, we are running and need to be aware of time
            out events */
         if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT ) ||
             (clidRxState->internalEvents & DTMFCLIDRX_EVT_SESSION_TIMEOUT) )
         {
            /* We timed out while waiting for the next digit. Check ring state
               to decide on what to do. We will leave the option of parsing a 
               partial CLID open. */
            if( stateInfo->ringState == DTMFCLIDRX_NORING )
            {
               /* We havent got a ring yet and we timed out. Raise reset event
                  and exit state machine so we can be ready for next call */
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
               break;
            }
            else if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF )
            {
               /* We are done with the timers, stop them */
               clidRxState->internalEvents &= ~(DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT |
                                                DTMFCLIDRX_EVT_SESSION_TIMEOUT);
               STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
               STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);
#if PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE

               /* We may have a piece of useful CLID, and we had the first ring. See if
                  we can salvage some information for this call. The parser will 
                  raise the necessary events for the exit of this state */
               
               stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
               parseDTMFClidRx( clidRxState );
               break;
#else
               /* If we will not salvage this partial CLID, we will drop this CLID.
                  We may have a call though, so do not reset and wait for the line to
                  go onhook/idle. Send up an event that we will not have a valid CLID */
               stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
               stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_INVALID_DTMF_CLID;
               break;

#endif /* PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE */
            }
            else
            {
               /* This shouldnt happen. This means we got no event, after the second ring,
                  or after ringing has ended, and we are still in the collection state and 
                  we had a timeout. Something is wrong! */
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_ERROR;
               VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxStm: ERROR: Bad ring state for FSM(EVT_NONE/COLLECTING)" ));
               break;
            }
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_DTMF_DIGIT, DTMFCLIDRX_COLLECTING ):
      {
         /* We have a new digit, process it. If a DTMF pause timeout occured
            at the same time we dequeued this DTMF digit, we will keep it, but
            this should not happen (after almost 1 second of waiting we probably 
            will not recieve another digit). */

         if( clidRxState->internalEvents & DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT )
         {
            /* Keep the digit even though we just timed out. Clear the event, we
               will reset the timer further down anyways after the digit is 
               stored and we begin waiting for the next one */
            clidRxState->internalEvents &= ~DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT;
         }

         /* Note: It is possible we had both the pause and session timeout, thus
                  we dont use "else if". If it was just the pause timeout, we will
                  fall through this next "if" */

         if( (clidRxState->internalEvents & DTMFCLIDRX_EVT_SESSION_TIMEOUT) &&
             (evtp->hsxop1 != dtmfParms->endCode) )
         {
            /* This is different. We timed out of the CLID session and got this digit.
               We will leave the option open to parse the partial CLID depending on the
               ring state. If we have not rung yet we will reset, else if we are now 
               after the first ring (but before the second), we will store this digit, 
               and salvage the partial CLID (If we have been recieving digits for the full
               session time between the first ring and second ring, something is wrong with
               the Central Office - this is a long time between rings..). Other wise we 
               shouldnt be in the collection state and recieving digits anyways. */

            if( stateInfo->ringState == DTMFCLIDRX_NORING )
            {
               /* We havent got a ring yet and we timed out. Raise reset event
                  and exit state machine so we can be ready for next call */
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
               break;
            }
            else if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF )
            {
               clidRxState->internalEvents &= ~DTMFCLIDRX_EVT_SESSION_TIMEOUT;

               /* We are done with the timers, stop them */
               STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
               STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);

#if PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE

               /* We may have a piece of useful CLID, and we had the first ring. See if
                  we can salvage some information for this call. The parser will 
                  raise the necessary events for the exit of this state. We know the 
                  buffer still has room at this point, else we would have stopped
                  collection already */

               clidInfo->collectBuffer[clidInfo->collectBufLen] = evtp->hsxop1;
               clidInfo->collectBufLen++;

               stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
               parseDTMFClidRx( clidRxState );
               break;
#else
               /* If we will not salvage this partial CLID, we will drop this CLID.
                  We may have a call though, so do not reset and wait for the line to
                  go onhook/idle. Send up an event that we will not have a valid CLID */
               stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
               stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_INVALID_DTMF_CLID;
               break;

#endif /* PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE */
            }
            else
            {
               /* This shouldnt happen. This means we got a digit event, after the second ring,
                  or after ringing has ended, and we are still in the collection state, and we
                  had a session timeout. Something is wrong! */
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_ERROR;
               VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxStm: ERROR: Bad ring state for FSM(EVT_DTMF_DIGIT/COLLECTING)" ));
               break;
            }
         } /* if( timeout ) */ 

         /* Store the digit, and check to see if it is the stop digit */
         clidInfo->collectBuffer[clidInfo->collectBufLen] = evtp->hsxop1;
         clidInfo->collectBufLen++;

         if( (evtp->hsxop1 != dtmfParms->endCode) &&
             (clidInfo->collectBufLen < (DTMFCLIDRX_MAX_BUFFER_SIZE + 1)) )
         {
            /* We are not done collecting, and the buffer still has room. Reset 
               timer for the digit pause */
            RESET_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
            break;
         }
         else if( evtp->hsxop1 == dtmfParms->endCode )
         {
            /* We have our stop digit. Proceed to the parsing. We are done with 
               the pause/session timers, stop them, and clear any events */
            clidRxState->internalEvents &= ~(DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT |
                                             DTMFCLIDRX_EVT_SESSION_TIMEOUT);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);

            /* Now that we have a full CLID, see if we had a ring yet. If not, we
               will start a timer to wait for a ring. If we dont get one, we will
               reset ourselves. This is to avoid the case where the call ends 
               after the CLID is collected but we dont have a ring yet, and we are
               left in the waiting state for a ringend or onhook event that will not 
               come */
            if( stateInfo->ringState == DTMFCLIDRX_NORING )
            {
               START_TIMER(stateInfo->timer[DTMFCLIDRX_RINGAFTERCLID_TIMER]);
            }

            clidRxState->internalEvents |= DTMFCLIDRX_EVT_DTMF_COLLECT_COMPLETE;
            stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
            parseDTMFClidRx( clidRxState );
            break;
         }
         else if( clidInfo->collectBufLen == (DTMFCLIDRX_MAX_BUFFER_SIZE + 1) )
         {
            /* We have maxed out the buffer. Stop collecting. We are done with 
               the timers, stop them, and clear any events */
            clidRxState->internalEvents &= ~(DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT |
                                             DTMFCLIDRX_EVT_SESSION_TIMEOUT);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);

#if PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE
            /* If defined, parse the partial CLID */

            /* Now that we have a partial CLID, see if we had a ring yet. If not, we
               will start a timer to wait for a ring. If we dont get one, we will
               reset ourselves. This is to avoid the case where the call ends 
               after the CLID is collected but we dont have a ring yet, and we are
               left in the waiting state for a ringend or onhook event that will not 
               come */
            if( stateInfo->ringState == DTMFCLIDRX_NORING )
            {
               START_TIMER(stateInfo->timer[DTMFCLIDRX_RINGAFTERCLID_TIMER]);
            }

            stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
            parseDTMFClidRx( clidRxState );
            break;
#else
            /* If we will not salvage this partial CLID, we will drop this CLID.
               We may have a call though, so do not reset and wait for the line to
               go onhook/idle if we have had a ring. Send up an event that we will 
               not have a valid CLID. Otherwise, reset the state machine */
            if( stateInfo->ringState != DTMFCLIDRX_NORING )
            {
               stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
               stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_INVALID_DTMF_CLID;
            }
            else
            {
               clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
            }
            break;
#endif
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_RINGON, DTMFCLIDRX_COLLECTING ):
      {
         /* We have a ring in the middle of collecting the DTMF CLID. Stop collection, 
            and if desired, parse the partial CLID, else we will ignore this partial 
            string and continue with the call sequence. If the ring state is not NORING
            or FIRSTRINGOFF, something is wrong. ERROR! */

         if( (stateInfo->ringState == DTMFCLIDRX_NORING) ||
             (stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF) )
         {
            if( stateInfo->ringState == DTMFCLIDRX_NORING )
            {
               stateInfo->ringState = DTMFCLIDRX_FIRSTRINGON;
            }
            else
            {
               stateInfo->ringState = DTMFCLIDRX_SECONDRING;
            }
            /* We are done with the timers, stop them, and clear any events */
            clidRxState->internalEvents &= ~(DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT |
                                             DTMFCLIDRX_EVT_SESSION_TIMEOUT);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);

#if PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE

            /* We may have a piece of useful CLID, and we have a ring. See if
               we can salvage some information for this call. The parser will 
               raise the necessary events for the exit of this state. */

            stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
            parseDTMFClidRx( clidRxState );
            break;
#else
            /* If we will not salvage this partial CLID, we will drop this CLID.
               We may have a call though, so do not reset and wait for the line to
               go onhook/idle. Send up an event that we will not have a valid CLID */
            stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
            stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
            clidRxState->internalEvents |= DTMFCLIDRX_EVT_INVALID_DTMF_CLID;
            break;

#endif /* PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE */
         }
         else
         {
            /* We are still in collection state and are in a wrong ring state, error! */
            clidRxState->internalEvents |= DTMFCLIDRX_EVT_ERROR;
            VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxStm: ERROR: Bad ring state for FSM(EVT_RINGON/COLLECTING)" ));
            break;
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_RINGOFF, DTMFCLIDRX_COLLECTING ):
      case FSM( PSTN_CTL_EVT_RINGEND, DTMFCLIDRX_COLLECTING ):
      {
         /* We shouldnt get these events in the collecting state. After a ring on event,
            we should of already exited collection and moved to the waiting state, where
            we will expect either the off event or and end event */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_ERROR;
         VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxStm: ERROR: Bad event/state combo: RINGOFF|RINGEND/COLLECTING" ));
      }
      break;

      case FSM( PSTN_CTL_EVT_OFFHOOK, DTMFCLIDRX_COLLECTING ):
      {
         /* We went offhook in the collection state. End collection, and wait for the
            call to end. There is nothing to salvage as we will not transfer any onhook
            CLID while offhook. If anything, this is an error case, but we will not reset
            ourselves, just wait. */

         /* We are done with the timers, stop them, and clear any events */
         clidRxState->internalEvents &= ~(DTMFCLIDRX_EVT_DTMF_PAUSE_TIMEOUT |
                                          DTMFCLIDRX_EVT_SESSION_TIMEOUT);
         STOP_TIMER(stateInfo->timer[DTMFCLIDRX_DTMF_PAUSE_TIMER]);
         STOP_TIMER(stateInfo->timer[DTMFCLIDRX_SESSION_TIMER]);

         stateInfo->offHook = VRG_TRUE;
         stateInfo->ringState = DTMFCLIDRX_RINGEND;
         stateInfo->clidType = DTMFCLIDRX_INVALID_CLID;
         stateInfo->serviceState = DTMFCLIDRX_WAITINGFOREND;
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_INVALID_DTMF_CLID;
      }

      case FSM( PSTN_CTL_EVT_ONHOOK, DTMFCLIDRX_COLLECTING ):
      {
         /* This is just wrong. We are in collection state and we went onhook?? Error. */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_ERROR;
         VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "processDTMFClidRxStm: ERROR: Bad event/state combo: ONHOOK/COLLECTING)" ));
      }

      case FSM( PSTN_CTL_EVT_NONE, DTMFCLIDRX_WAITINGFOREND ):
      case FSM( PSTN_CTL_EVT_DTMF_DIGIT, DTMFCLIDRX_WAITINGFOREND ):
      {
         if( clidRxState->internalEvents & DTMFCLIDRX_EVT_RINGAFTERCLID_TIMEOUT )
         {
            /* We got either a full CLID, or a partial CLID due to buffer overflow,
               and havent had our first ring yet. This timeout says we have waited
               long enough for the ring, and still havent got it. We will now reset
               and be prepared for another call. */

            clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
            break;
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_RINGON, DTMFCLIDRX_WAITINGFOREND ):
      {
         /* Update the ring state */
         if( stateInfo->ringState == DTMFCLIDRX_NORING )
         {
            /* This is the beginning of the first ring. Make sure that our ring timer
               is now not running if it was */
            STOP_TIMER(stateInfo->timer[DTMFCLIDRX_RINGAFTERCLID_TIMER]);
            stateInfo->ringState = DTMFCLIDRX_FIRSTRINGON;
         }
         else if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGOFF )
         {
            /* Second ring */
            stateInfo->ringState = DTMFCLIDRX_SECONDRING;
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_RINGOFF, DTMFCLIDRX_WAITINGFOREND ):
      {
         if( stateInfo->ringState == DTMFCLIDRX_FIRSTRINGON )
         {
            /* First ring completed */
            stateInfo->ringState = DTMFCLIDRX_FIRSTRINGOFF;
         }
      }
      break;

      case FSM( PSTN_CTL_EVT_RINGEND, DTMFCLIDRX_WAITINGFOREND ):
      {
         /* We should be in some ring state other then NORING at this point.
            This event says that ringing has stopped on the line, no one went
            offhook for the call. Reset the state machine for the next call.
            Note: if we were still in NORING state, this is an error, but we will
            reset anyways on this event. */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
      }
      break;

      case FSM( PSTN_CTL_EVT_OFFHOOK, DTMFCLIDRX_WAITINGFOREND ):
      {
         /* We have gone offhook while in waiting state. This means that we have a 
            call in place. Change hook and ring states, and stop the ring timer if
            it was running. We will then wait for the onhook event before reseting */
         stateInfo->offHook = VRG_TRUE;
         stateInfo->ringState = DTMFCLIDRX_RINGEND;
         STOP_TIMER(stateInfo->timer[DTMFCLIDRX_RINGAFTERCLID_TIMER]);
      }
      break;

      case FSM( PSTN_CTL_EVT_ONHOOK, DTMFCLIDRX_WAITINGFOREND ):
      {
         /* We have gone onhook while in waiting state. We are done the call. Reset
            the state machine so we are ready for the next call. */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_NEEDRESET;
      }
      break;

      default:
      {
         /* Invalid event/state -> This just means we will not process the event/state
            as it is not necessary */
      }
   }
#if PSTNCTL_DTMFCLIDRX_DEBUG
   if( !(evtp->hsxevt == PSTN_CTL_EVT_NONE) )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "processClidRxStm: FSM: New State: %u", stateInfo->serviceState ));
   }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   return;
}

/*****************************************************************************
*
*  FUNCTION:   parseDTMFClidRx
*
*  PURPOSE:    parse the collected DTMF Clid Rx string, and convert the numbers
*              and CLID strings to char form.
*
*  PARAMETERS: clidRxState - state structure of the DTMF Clid Rx service on
*                            a channel
*
*  RETURNS:    nothing
*
*  NOTE:       This function parses what was collected based on the country
*              DTMF parms and saves the results to be sent up.
*
*****************************************************************************/
static void parseDTMFClidRx( PSTN_CTL_DTMFCLIDRX_STATE *clidRxState )
{
   DTMFCLIDRX_CLID_INFO *clidInfo = &clidRxState->clidInfo;
   DTMFCLIDRX_PARSED_DATA *parsedData = &clidRxState->clidInfo.parsedData;
   PSTN_CTL_DTMF_CLID_PARMS *dtmfParms = 
                      (PSTN_CTL_DTMF_CLID_PARMS *)clidRxState->dtmfClidParms;

   signed char infoBuffer[DTMFCLIDRX_MAX_NUMBER_SIZE + 1] = {0}; /* temp buffer for info
                                                                    string */
   char *bufp = NULL;            /* pointer to a buffer for copying */
   char *parseBufp = NULL;       /* pointer to beginning of collection buffer */
   char *tokenStart = NULL;      /* pointer to start of a token */
   VRG_BOOL parseDone, error, infoCode, redirCode, tokenCopied;
   int i, j, offset;

   /* set start point and init variables */
   parseBufp = (char *)&clidInfo->collectBuffer[0];
   offset = -1;
   tokenCopied = VRG_FALSE;
   infoCode = VRG_FALSE;
   redirCode = VRG_FALSE;
   parseDone = VRG_FALSE;
   error = VRG_FALSE;

   infoBuffer[0] = -1; /* Mark as empty with -1, as 0 (\0) will be a valid 
                          number to have in the first index of this buffer */

#if PSTNCTL_DTMFCLIDRX_DEBUG
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Collect Buffer: parseBufp = %u, clidInfo->collectBuffer = %u",
            parseBufp, clidInfo->collectBuffer ));
   for( i=0; i < clidInfo->collectBufLen + 2; i++ )
   {
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", *(parseBufp + i) ));
   }
   VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

   /* Increment the pointer through the collected string */
   while( (offset < clidInfo->collectBufLen) &&
          (!parseDone) &&
          (!error) )
   {
      /* Only allow parsing if the first digit in the buffer is a valid one. i.e:
         We should start with a startcode, infocode, or redirectingcode. */
      if( IS_START_TOKEN( parseBufp, dtmfParms ) )
      {
         /* Find the offset of the next start token. Start searching at + 1 the previous 
            offset. For the first start token at 0, we should have offset initialized to
            -1 by now. */
         for( i = (offset + 1); i < clidInfo->collectBufLen; i++ )
         {
            if( IS_START_TOKEN( (parseBufp + i), dtmfParms ) )
            {
               tokenStart = parseBufp + i;

               /* Update the offset to new token start */
               offset = i;
               break;
            }
         }
         if( i == clidInfo->collectBufLen )
         {
            /* We didnt find a start token in the buffer. Exit. */
            error = VRG_TRUE;
            break;
         }

#if VRG_COUNTRY_CFG_NETHERLANDS
         if( clidRxState->country == VRG_COUNTRY_NETHERLANDS )
         {
            /* Netherlands use the same start and info code (either the calling
               number is recieved or 10 0's are recieved) so we have to be 
               careful parsing the CLID. We only expect either a calling number
               or an info code (10 0's), so we will set the buffer to the 
               information buffer. Later, if we decide we got no useful info
               code, we will copy over the infocode to the calling number, as
               it should be the calling number. */

            if( *tokenStart == dtmfParms->startCode /* = informationCode */ )
            {
               if( !infoCode )
               {
                  bufp = (char *)&infoBuffer[0];
                  infoCode = VRG_TRUE;
               }
               else
               {
                  /* We got another info token in the CLID string. Error. */
                  error = VRG_TRUE;
               }
            }
            else 
            {
               /* We dont expect any other codes, only the start/info code
                  which are the same, else error */
               error = VRG_TRUE;
            }
         }
         else
#endif /* VRG_COUNTRY_CFG_NETHERLANDS */
         {
            if( *tokenStart == dtmfParms->startCode )
            {
               if( offset == 0 )
               {
                  /* Assign the calling number buffer for copying */
                  bufp = (char *)&parsedData->callingNumber[0];
               }
               else
               {
                  /* We have a start code after the first digit in the CLID string */
                  error = VRG_TRUE;
               }
            }
            else if( *tokenStart == dtmfParms->informationCode )
            {
               if( !infoCode )
               {
                  bufp = (char *)&infoBuffer[0];
                  infoCode = VRG_TRUE;
               }
               else
               {
                  /* We got another info token in the CLID string. Error. */
                  error = VRG_TRUE;
               }
            }
            else if( *tokenStart == dtmfParms->redirectingCode )
            {
               if( !redirCode )
               {
                  bufp = (char *)&parsedData->redirectingNumber[0];
                  redirCode = VRG_TRUE;
               }
               else
               {
                  /* We got another redirecting bnumber token in the CLID string. Error. */
                  error = VRG_TRUE;
               }
            }
            else
            {
               /* We found a start token which isnt a start token? Error */
               error = VRG_TRUE;
            }
         }

         /* At this point we will copy over the digits and conver them to char. 
            To convert, we add the decimal value necessary to convert the decimal 
            version of the HAPI digit to a char version. (See conversion macro). Ex:

           decimal "3" in collectBuffer will be converted to decimal "51" == '3' by 
           adding "48" = '0'. 
           
           We will not do this for the infocodes as they do not need to be in char
           form, but HAPI form */

         /* If we had an error before this point, do not do any copying! */
         while( (!tokenCopied) && (!error) && (!parseDone) )
         {
            for( j=0; j < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; j++ )
            {
               /* We use tokenStart + 1 as tokenStart is the digit marking what info
                  we have: 'A', 'D', 'C' etc... and the actual number is one element
                  after */
               if( offset + 1 + j == clidInfo->collectBufLen )
               {
                  /* Break, we have reached the end of the buffer (Buflen is 1 over the
                     actual buffer end). We also didnt hit the stop digit. */
#if PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE
                  /* If partial parsing available, we have reached the end of what we collected,
                     and saved what we could. We are done, just close the buffer we were on. */
                  if( *tokenStart != dtmfParms->informationCode )
                  {
                     *(bufp + j) = '\0';
                  }
                  else
                  {
                     *(bufp + j) = -1;
                  }
                  parseDone = VRG_TRUE;
#else
                  error = VRG_TRUE;
#endif /* PSTNCTL_DTMFCLIDRX_PARTIAL_CLID_PARSE */
                  break;
               }
               if( IS_STOP_TOKEN( (tokenStart + 1 + j), dtmfParms ) )
               {
                  /* We are at the end of this token, and either have another one or the
                     stop digit itself */

                  /* Close the number buffer. Mark the infoNumber buffer with -1 as it is
                     not stored with char digits */
                  if( *tokenStart != dtmfParms->informationCode )
                  {
                     *(bufp + j) = '\0';
                  }
                  else
                  {
                     *(bufp + j) = -1;
                  }
                  tokenCopied = VRG_TRUE;

                  if( *(tokenStart + 1 + j) == dtmfParms->endCode )
                  {
                     parseDone = VRG_TRUE;
                  }

                  break;
               }
               else
               {
                  /* Copy over the next digit and convert to char only if it is not an 
                     information code we are copying */
                  *(bufp + j) = *(tokenStart + 1 + j);
                  if( *tokenStart != dtmfParms->informationCode )
                  {
                     CONVERT_HAPI_TO_CHAR( (bufp + j) );
                  }
#if PSTNCTL_DTMFCLIDRX_DEBUG
                  VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "*(bufp + %d) = 0x%04x, %c", j, *(bufp + j), *(bufp + j) ));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
               }
            }
            /* Should check here if we filled the number?? */
            /* We maxed out our number buffer and did not find a stopping token or
               reach the end of the collection buffer */
         }
         tokenCopied = VRG_FALSE;
      }
      else
      {
         /* Not a valid start token at the begining of the CLID string. Do not try 
            parsing this as we shouldnt even be in the parser in this case */
         clidRxState->internalEvents |= (DTMFCLIDRX_EVT_DTMF_PARSE_ERROR | 
                                         DTMFCLIDRX_EVT_INVALID_DTMF_CLID);
         return;
      }
      /* By this point we exit the while loop if we finished parsing, had an 
         error, or exceeded the buffer (also an error) */
   } /* end while() loop */

   if( error )
   {
      clidRxState->internalEvents |= (DTMFCLIDRX_EVT_DTMF_PARSE_ERROR |
                                      DTMFCLIDRX_EVT_INVALID_DTMF_CLID);
      return;
   }
   /* If we have parseDone true, see if we have any info code recieved, and if
      so, extract the info. */

   if( parseDone && (infoBuffer[0] != -1) )
   {
      /* There is something in the info buffer. See if we match a code. We assume
         that the infocode in the DMTF parms struct has this format:
         
            'Info digit', ..code(ex 01, 0000000000).., 'End digit', 'ErrorCode'
            
         As currently defined in vrgEndptGlobe.c

         Our infoBuffer only contains the code itself, as we took into account
         the info code being sent in the middle of the string (very unlikely, but
         possible). We thus stripped out only the code itself. We will now format
         this buffer based on the format above */

#if PSTNCTL_DTMFCLIDRX_DEBUG
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Unformatted buffer= " ));
      for( i=0; i < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; i++ )
      {
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", infoBuffer[i] ));
      }
      VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      char formattedInfo[DTMFCLIDRX_MAX_NUMBER_SIZE + 1] = {0};
      i = 0;
      
      formattedInfo[i] = dtmfParms->informationCode;
      
      while( infoBuffer[i] != -1 )
      {
         formattedInfo[i+1] = infoBuffer[i];
         i++;
      }
      
      formattedInfo[++i] = dtmfParms->endCode;
      formattedInfo[++i] = HAPI_PTE_DTMFDDGTERROR;

#if PSTNCTL_DTMFCLIDRX_DEBUG
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Formatted buffer= " ));
      for( i=0; i < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; i++ )
      {
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "0x%04x ", formattedInfo[i] ));
      }
      VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      /* We use memcmp to avoid HAPI "0" being interpretted as '\0' with strncmp. To 
         be certain we match correctly, at this point we need the exact codes followed 
         by all null characters in the formattedInfo. The codes in vrgEndptGlobe.c are 
         all followed by trailing null characters after they are initialized. */

      if( memcmp( formattedInfo, dtmfParms->privateCode, 
                   sizeof(dtmfParms->privateCode) ) == 0 )
      {
         parsedData->infoRecieved = DTMFCLIDRX_INFO_PRIVATE;
      }
      else if( memcmp( formattedInfo, dtmfParms->unavailCode, 
                        sizeof(dtmfParms->unavailCode) ) == 0 )
      {
         parsedData->infoRecieved = DTMFCLIDRX_INFO_UNAVAILABLE;
      }
      else if( memcmp( formattedInfo, dtmfParms->errorCode, 
                        sizeof(dtmfParms->errorCode) ) == 0 )
      {
         parsedData->infoRecieved = DTMFCLIDRX_INFO_ERROR;
      }
      else
      {
         parsedData->infoRecieved = DTMFCLIDRX_INFO_NONE;

#if VRG_COUNTRY_CFG_NETHERLANDS
         if( clidRxState->country == VRG_COUNTRY_NETHERLANDS )
         {
            /* We got something, but it was not the 10 0's. Copy over the
               infobuffer to the calling number buffer and convert to char
               form */
            for( i=0; i < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; i++ )
            {
               if( infoBuffer[i] == -1 )
               {
                  parsedData->callingNumber[i] = '\0';
                  break;
               }
               parsedData->callingNumber[i] = infoBuffer[i];
               CONVERT_HAPI_TO_CHAR( &parsedData->callingNumber[i] );
            }
         }
#endif /* VRG_COUNTRY_CFG_NETHERLANDS */

      }
#if PSTNCTL_DTMFCLIDRX_DEBUG
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "parsedData->infoRecieved: %d", parsedData->infoRecieved ));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
   }

   /* Finally, copy over the CLID string recieved, and convert everything to char,
      and send up completion event. To convert, we add the decimal value necessary to
      convert the decimal version of the HAPI digit to a char version. (See 
      conversion macro) */

   if( parseDone )
   {
#if PSTNCTL_DTMFCLIDRX_DEBUG
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "parsedData->callingNumber: " ));
      for( i=0; i < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; i++ )
      {
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "[%c] ", parsedData->callingNumber[i] ));
      }
      VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));
      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "parsedData->redirectingNumber: " ));
      for( i=0; i < DTMFCLIDRX_MAX_NUMBER_SIZE + 1; i++ )
      {
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "[%c] ", parsedData->redirectingNumber[i] ));
      }
      VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));

      VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "parsedData->clidString:"));
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      /* The two buffers here are of the same size */
      for( i=0; i < clidInfo->collectBufLen + 1; i++ )
      {
         if( i == clidInfo->collectBufLen )
         {
            parsedData->clidString[i] = '\0';
            break;
         }
         parsedData->clidString[i] = clidInfo->collectBuffer[i];
         CONVERT_HAPI_TO_CHAR( &parsedData->clidString[i] );
#if PSTNCTL_DTMFCLIDRX_DEBUG
         VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "%c ", parsedData->clidString[i] ));
      }
      VRG_LOG_DBG((VRG_LOG_MOD_EPT, ""));
#else
      }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */

      if( clidRxState->internalEvents & DTMFCLIDRX_EVT_DTMF_COLLECT_COMPLETE )
      {
         /* Complete CLID recieved and parsed */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_VALID_DTMF_CLID;
      }
      else
      {
         /* Partial string recieved and parsed */
         clidRxState->internalEvents |= DTMFCLIDRX_EVT_PARTIAL_DTMF_CLID;
      }
   }

   return;
}

/*****************************************************************************
*
*  FUNCTION:   updateTimers
*
*  PURPOSE:    update the internal timers of the Clid Rx service based on task
*              rate
*
*  PARAMETERS: stateInfo - stateInfo containing timer structure array
*
*  RETURNS:    internal events from timers
*
*  NOTE:       This function assumes that we have checked that the Clid Rx
*              service was initialized and that the timeouts and event 
*              parameters are valid
*
*****************************************************************************/
static VRG_UINT16 updateTimers( DTMFCLIDRX_STATE_INFO *stateInfo )
{
   int i;
   VRG_UINT16 timerEvts = DTMFCLIDRX_EVT_NO_INTERNAL_EVENTS;

   for( i=0; i < DTMFCLIDRX_MAX_TIMERS; i++ )
   {
      if( stateInfo->timer[i].running )
      {
         stateInfo->timer[i].counter += PSTN_CTL_CFG_TASK_RATE_MS;

#if PSTNCTL_DTMFCLIDRX_DEBUG
         if( (stateInfo->timer[i].counter % ( 5 * PSTN_CTL_CFG_TASK_RATE_MS )) == 0 )
         {
            VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "Timer[%d]: Counter = %d, Timeout = %d", i,
                     stateInfo->timer[i].counter, stateInfo->timer[i].timeout ));
         }
#endif /* PSTNCTL_DTMFCLIDRX_DEBUG */
         
         if( stateInfo->timer[i].counter >= stateInfo->timer[i].timeout )
         {
            timerEvts |= stateInfo->timer[i].timeoutEvent;
         }
      }
   }

   return timerEvts;
}
