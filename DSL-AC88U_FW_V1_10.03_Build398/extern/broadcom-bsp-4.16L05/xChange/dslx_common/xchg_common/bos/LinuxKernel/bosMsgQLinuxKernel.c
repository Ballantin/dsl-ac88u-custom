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
*  Filename: bosMsgQLinuxKernel.c
*
****************************************************************************
*  Description:
*
*
****************************************************************************/
/**
*
*  @file    bosMsgQLinuxKernel.c
*
*  @brief   LinuxKernel implementation of the BOS Message Queue
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_MSGQ

#include <string.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosMsgQ.h>
#include <bosMsgQPrivate.h>
#include <str.h>
#include <bosSleep.h>
#include <bosLog.h>

#include <linux/msg.h>
#include <linux/jiffies.h>
#include <linux/timer.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define TIMEOUT_HDLR_MSG_LENGTH    1
#define BOS_Q_WAIT_TIME    10000  /* in microseconds */
#if 0
typedef enum
{
   BOS_TIMER_MESSAGE = 1,
   BOS_USER_MESSAGE,
} BOS_QMSG_TYPE;

/* Kernel buffer structure. This structure should
** be a superset of the structure msgbuf defined
** in linux/msg.h */
typedef struct BOS_LINUXKERNEL_MSGBUF
{
   long           mtype;      /* type of received/sent message */
   BOS_MSGQ_MSG   mtext;      /* text of the message */
} BOS_LINUXKERNEL_MSGBUF;

typedef struct BOS_QT_STUFF
{
   BOS_MSGQ    qid;
   BOS_BOOL        bTimeOut;
} BOS_QT_STUFF;

/* ---- Private Variables ------------------------------------------------ */

static   BOS_BOOL gBosMsgQInitialized = BOS_FALSE;

/* ---- Private Function Prototypes -------------------------------------- */

static BOS_STATUS CheckMsgQStatusLinuxKernel( unsigned long err, BOS_MSGQ_FUNC_ID funcId );

/* ---- External Function Prototypes -------------------------------------- */

// extern asmlinkage long kernel_sys_msgsnd (int msqid, struct msgbuf __user *msgp, size_t msgsz, int msgflg);


/* ---- Functions -------------------------------------------------------- */

/***************************************************************************/

BOS_STATUS bosMsgQInit( void )
{
   BOS_ASSERT( !gBosMsgQInitialized );

   bosMsgQInitFunctionNames();

   gBosMsgQInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosMsgQInit */

/***************************************************************************/

BOS_STATUS bosMsgQTerm( void )
{
   BOS_ASSERT( gBosMsgQInitialized );

   gBosMsgQInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosMsgQTerm */

/***************************************************************************/

BOS_STATUS bosMsgQCreate( const char *name, unsigned int qSize, BOS_MSGQ *qid )
{
   int q = 0;

   BOS_ASSERT( qid != NULL );

   q = sys_msgget( IPC_PRIVATE, 0 );

   if( q < 0 )
   {
      return CheckMsgQStatusLinuxKernel( BOS_STATUS_ERR, BOS_MSGQ_CREATE_FUNC_ID );
   }
   else
   {
      BOS_LOG(("bosMsgQCreate: Created message queue %s at address 0x%x \n", name, q));

      /* queue ID returned from sys_msgget can be 0.
      Since we don't want to use 0 as a valid BOS_MSGQ queue id,
      we increment it before teturning to the caller. */
      *qid = q + 1;
   }

   return( BOS_STATUS_OK );

} /* bosMsgQCreate */

/***************************************************************************/

BOS_STATUS bosMsgQDestroy( BOS_MSGQ *qid )
{
   int result;
   BOS_MSGQ queueId;

   BOS_ASSERT( qid != NULL );

   queueId = (*qid) - 1;

   result = sys_msgctl( queueId, IPC_RMID, NULL );

   if ( result == 0 )
   {
      BOS_LOG(("bosMsgQDestroy: Successfully deleted message queue at address 0x%x \n", (unsigned int)queueId));
      return( BOS_STATUS_OK );
   }
   else
   {
      return CheckMsgQStatusLinuxKernel( BOS_STATUS_ERR, BOS_MSGQ_DESTROY_FUNC_ID );
   }


} /* bosMsgQDestroy */

/***************************************************************************/

BOS_STATUS bosMsgQSend( BOS_MSGQ *qid, const BOS_MSGQ_MSG *qMsg)
{
   long        result;
   BOS_MSGQ    queueId;
   BOS_LINUXKERNEL_MSGBUF buf;

   BOS_ASSERT( qid  != NULL );
   BOS_ASSERT( qMsg != NULL );

   /* Decrement queue ID because it was incremented in bosMsgQCreate */
   queueId = (*qid) - 1;

   buf.mtype = BOS_USER_MESSAGE;
   memcpy( &buf.mtext, qMsg->buffer, sizeof(*qMsg) );

   result = sys_msgsnd( queueId, (struct msgbuf*)&buf, sizeof(buf.mtext), IPC_NOWAIT );

   if( result != 0 )
   {
      return CheckMsgQStatusLinuxKernel( BOS_STATUS_ERR, BOS_MSGQ_SEND_FUNC_ID );
   }

   return ( BOS_STATUS_OK );

} /* bosMsgQSend */

/***************************************************************************/

BOS_STATUS bosMsgQReceive( BOS_MSGQ *qid, BOS_MSGQ_MSG *qMsg)
{
   BOS_STATUS status;

   status = bosMsgQTimedReceive( qid, BOS_WAIT_FOREVER, qMsg );

   return( status );

} /* bosMsgQReceive */

/***************************************************************************/

BOS_STATUS bosMsgQTimedReceive( BOS_MSGQ *qid, BOS_TIME_MS timeoutMsec,
                                 BOS_MSGQ_MSG *qMsg)
{
   int errno;
   int         bytesRecvd;
   int         msgflag;
   BOS_STATUS  status;
   BOS_MSGQ    queueId;
   BOS_LINUXKERNEL_MSGBUF buf;

   BOS_ASSERT( qid  != NULL );
   BOS_ASSERT( qMsg != NULL );

   msgflag = IPC_NOWAIT;
   status = BOS_STATUS_OK;

   /* Decrement queue ID because it was incremented in bcmOsQueueCreate */
   queueId = (*qid) - 1;

   if ( timeoutMsec == BOS_WAIT_FOREVER )
   {
      msgflag = 0;
   }

   if( timeoutMsec > 0 )
   {
      long wait_msec = (long)( timeoutMsec );
      bytesRecvd = sys_msgrcv( queueId, (struct msgbuf*)&buf, sizeof(buf.mtext), 0, IPC_NOWAIT );

      while ( bytesRecvd <= 0 && wait_msec > 0 )
      {
         bosSleep( BOS_Q_WAIT_TIME/1000 );
         wait_msec -= ( BOS_Q_WAIT_TIME/1000 );
         bytesRecvd = sys_msgrcv( queueId, (struct msgbuf*)&buf, sizeof(buf.mtext), 0, IPC_NOWAIT );
      }
      if ( bytesRecvd <= 0 )
      {
         errno = bytesRecvd;
         if (errno == ENOMSG)
         {
            status = BOS_STATUS_TIMEOUT;
         }
         else
         {
            status = BOS_STATUS_ERR;
         }
      }
   }
   else
   {
      bytesRecvd = sys_msgrcv( queueId, (struct msgbuf*)&buf, sizeof(buf.mtext), 0, msgflag);
   }

   if( status == BOS_STATUS_OK )
   {
      if( bytesRecvd == sizeof(buf.mtext) )
      {
         /* message received */
         memcpy( qMsg->buffer, &buf.mtext, sizeof(buf.mtext) );
      }
      else
      {
         status = BOS_STATUS_ERR;
      }
   }
   else
   {
      status = BOS_STATUS_ERR;
   }

   return ( status );

} /* bosMsgQTimedReceive */

/**
 * @addtogroup bosMsgQInternal
 * @{
 */

/***************************************************************************/
/**
*  Translate a LinuxKernel error code into a BOS error code and report any
*  errors encountered.
*
*  @param   err      (in)  LinuxKernel Error code.
*  @param   funcId   (in)  ID of the function that we're checking for.
*/

static BOS_STATUS CheckMsgQStatusLinuxKernel( unsigned long err, BOS_MSGQ_FUNC_ID funcId )
{
   return bosErrorCheckStatusLinuxKernel( err,
                                   BOS_MAKE_ERROR_SOURCE( BOS_MSGQ_MODULE, funcId ));

} /* CheckMsgQStatusLinuxKernel */


/** @} */
#endif
#endif   /* BOS_CFG_MSGQ */

