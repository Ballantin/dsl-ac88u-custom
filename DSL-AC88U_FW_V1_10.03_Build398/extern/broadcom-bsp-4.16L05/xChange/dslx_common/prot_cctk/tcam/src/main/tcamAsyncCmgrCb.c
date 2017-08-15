/**********************************************************************************
** Copyright © 2010 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This file contains the Telephony Client Application Manager main.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */
BOS_SEM      gTcamAsyncSem;
BOS_BOOL     gbTcamAsyncInit = BOS_FALSE;
BOS_TASK_ID  gTcamAsyncTaskId = 0;
BOS_MSGQ     gTcamAsyncQid = 0;
BOS_BOOL     gbTcamAsyncQueueCreated = BOS_FALSE;
BOS_SEM      gTcamAsyncSem;
BOS_BOOL     gbTcamAsyncSemCreated = BOS_FALSE;
BOS_BOOL     gbTcamMainLoop = BOS_FALSE;

            
/* ---- Private Function Prototypes --------------------------------------------- */

/***********************************************************************************
** FUNCTION:   tcamAsyncGetEvt
**
** PURPOSE:    Get an event from the TCAM asynch queue.
**
** PARAMETERS: None.
**
** RETURNS:    A pointer to the dequeued event if available or NULL if nothing
**             is in the queue at this time.
***********************************************************************************/
static TCAMCMGREVT *tcamAsyncGetEvt ( void )
{
   BOS_MSGQ_MSG msgQ;
   static TCAMCMGREVT evt;

   if ( bosMsgQTimedReceive ( &gTcamAsyncQid, 0, &msgQ ) == BOS_STATUS_OK )
   {
      evt.uEvt  = msgQ.buffer[0];
      evt.uDat1 = msgQ.buffer[1];
      evt.uDat2 = msgQ.buffer[2];
      evt.pDat3 = (void *) msgQ.buffer[3];
      return &evt;
   }
   else
   {
      return NULL;
   }
}


/***********************************************************************************
** FUNCTION:   tcamAsyncMainRun
**
** PURPOSE:    Run the TCAM async thread.
**
** PARAMETERS: args - the arguments passed when this main is invoked by BOS
**                    framework.
**
** RETURNS:    Never.  This runs the infinite loop which takes care of the
**             TCAM async processing.
***********************************************************************************/
static void tcamAsyncMainRun ( BOS_TASK_ARG args )
{
   BOS_STATUS status;
   TCAMCMGREVT *pEvt;

   (void) args;

   while ( gbTcamMainLoop )
   {
      status = bosSemTimedTake( &gTcamAsyncSem, TCAMCFG_THREAD_SLEEP );

      /* Dequeue and process events if necessary.
      */
      while ( ( pEvt = tcamAsyncGetEvt () ) != NULL )
      {
         switch ( pEvt->uEvt )
         {
            case eCMCMD_ENDCALL:
            {
               BOS_UINT32 uAppIx = pEvt->uDat1;

               TCAMLOG_INFORMATION ((
                  "tcamAsyncMainRun: ending 0x%lX",
                  uAppIx ));
               cmgrCmd ( eCMCMD_ENDCALL, (void *) &uAppIx );
            }
            break;

            case eCMCMD_APPCALLHDL:
            {
               CMGRAPPCALLHDL cmdInfo;
               cmdInfo.uSesHdl = pEvt->uDat1;
               cmdInfo.uAppIx = pEvt->uDat2;

               TCAMLOG_INFORMATION ((
                  "tcamAsyncMainRun: setting handle 0x%lX for 0x%lX",
                  cmdInfo.uAppIx, cmdInfo.uSesHdl ));
               cmgrCmd ( eCMCMD_APPCALLHDL, (void *) &cmdInfo );
            }
            break;

#if CMGRCFG_PASS_THRU_MSG
            case eCMCMD_MSG_RESP:
            {
               CMGRAPPCALLHDL rsp;
               rsp.uAppIx = pEvt->uDat1;
               rsp.uSesHdl = pEvt->uDat2;

               cmgrCmd ( eCMCMD_MSG_RESP, (void *) &rsp );
            }
            break;
#endif /* CMGRCFG_PASS_THRU_MSG */

            default:
            break;
         }
      };
   };

   /* Must have exit the main loop for some reason.  Report a critical
   ** error since there is not much else we will be able to do here!
   */
   TCAMLOG_CRITICAL (( "tcamAsyncMainRun: TCAM async loop has exited!" ));
}


/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   tcamAsyncCmgrCbInit
**
** PURPOSE:    Creates the TCAM asynchronous CMGR callback support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
***********************************************************************************/
void tcamAsyncCmgrCbInit ( void )
{
   if ( gbTcamAsyncInit == BOS_TRUE )
   {
      TCAMLOG_WARNING ((
               "tcamAsyncCmgrCbInit: already initialized, ignoring!" ));
      return;
   }

   gbTcamAsyncInit = BOS_TRUE;

   /* Create necessary thread associated resources.
   */
   if ( bosMsgQCreate ( TCAMCFG_THREAD_QNAME,
                        TCAMCFG_THREAD_QDEPTH,
                        &gTcamAsyncQid ) != BOS_STATUS_OK )
   {
      TCAMLOG_CRITICAL (( "tcamAsyncCmgrCbInit: TCAM failed to create msg-q!" ));
      tcamAsyncCmgrCbDeinit();
      return;
   }
   gbTcamAsyncQueueCreated = BOS_TRUE;
   if ( bosSemCreate ( TCAMCFG_THREAD_SEMNAME,
                       BOS_CFG_SEM_INIT_COUNT,
                       BOS_CFG_SEM_MAX_COUNT,
                       &gTcamAsyncSem ) != BOS_STATUS_OK )
   {
      TCAMLOG_CRITICAL (( "tcamAsyncCmgrCbInit: TCAM failed to create sema-4!" ));
      tcamAsyncCmgrCbDeinit();
      return;
   }
   gbTcamAsyncSemCreated = BOS_TRUE;
   
   /* Create and launch the TCAM async thread.
   */
   gbTcamMainLoop = BOS_TRUE;
#if BOS_CFG_RESET_SUPPORT
   if ( bosTaskCreateEx ( TCAMCFG_THREAD_NAME,
                          TCAMCFG_THREAD_STACK,
                          TCAMCFG_THREAD_PRI,
                          NULL,
                          tcamAsyncMainRun,
                          NULL,
                          (BOS_TASK_ARG *) NULL,
                          &gTcamAsyncTaskId ) != BOS_STATUS_OK )
#else
   if ( bosTaskCreate   ( TCAMCFG_THREAD_NAME,
                          TCAMCFG_THREAD_STACK,
                          TCAMCFG_THREAD_PRI,
                          tcamAsyncMainRun,
                          (BOS_TASK_ARG *) NULL,
                          &gTcamAsyncTaskId ) != BOS_STATUS_OK )
#endif
   {
      TCAMLOG_CRITICAL (( "tcamAsyncCmgrCbInit: TCAM failed to create thread!" ));
      tcamAsyncCmgrCbDeinit();
      return;                    
   }

#if BOS_CFG_RESET_SUPPORT
   bosTaskSetNoReset( &gTcamAsyncTaskId );
#endif
}


/***********************************************************************************
** FUNCTION:   tcamAsyncCmgrCbDeinit
**
** PURPOSE:    Destroy the TCAM asynchronous CMGR callback support.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.  
***********************************************************************************/
void tcamAsyncCmgrCbDeinit ( void )
{
   if ( gbTcamAsyncInit == BOS_FALSE )
   {
      TCAMLOG_WARNING ((
               "tcamAsyncCmgrCbDeinit: not initialized, ignoring!" ));
      return;
   }

   if ( gbTcamAsyncSemCreated &&
        bosSemDestroy ( &gTcamAsyncSem ) != BOS_STATUS_OK )
   {
      TCAMLOG_WARNING ((
               "tcamAsyncCmgrCbDeinit: Fail semaphore destruction!" ));
   }

   if ( gbTcamAsyncQueueCreated &&
        (bosMsgQDestroy ( &gTcamAsyncQid ) != BOS_STATUS_OK) )
   {
      TCAMLOG_WARNING ((
               "tcamAsyncCmgrCbDeinit: Fail message queue destruction!" ));
   }

   /* Self killed */
   if ( gTcamAsyncTaskId &&
        (bosTaskDestroy ( &gTcamAsyncTaskId ) != BOS_STATUS_OK) )
   {
      TCAMLOG_CRITICAL ((
               "tcamAsyncCmgrCbDeinit: Fail task destruction!" ));
   }

   /* Reset internal variables to initial value.
   */
   gTcamAsyncTaskId = 0;
   gTcamAsyncQid = 0;
   gbTcamAsyncQueueCreated = BOS_FALSE;
   gbTcamAsyncSemCreated = BOS_FALSE;
   gbTcamAsyncInit = BOS_FALSE;
}


/***********************************************************************************
** FUNCTION:   tcamAsyncCmgrCbPost
**
** PURPOSE:    Post an event to the TCAM asynch queue.
**
** PARAMETERS: pEvt - pointer to the event to queue.
**
** RETURNS:    BOS_STATUS_OK in case of success, an error code otherwise.
***********************************************************************************/
BOS_STATUS tcamAsyncCmgrCbPost ( TCAMCMGREVT *pEvt )
{
   BOS_MSGQ_MSG msgQ;
   BOS_STATUS ret;

   msgQ.buffer[0] = pEvt->uEvt;
   msgQ.buffer[1] = pEvt->uDat1;
   msgQ.buffer[2] = pEvt->uDat2;
   msgQ.buffer[3] = (BOS_UINT32) pEvt->pDat3;

   if ( (ret = bosMsgQSend( &gTcamAsyncQid, &msgQ )) == BOS_STATUS_OK )
   {
      bosSemGive ( &gTcamAsyncSem );
   }

   return ret;
}
